#include "FLVWritter.h"
#include "FLVMuxer.h"//OnDataMethodPtrT

void FLVWritter::flush(OnDataMethodPtrT &OnData){
    if (buffer.size() <= 0)
        return;
    if (OnData != NULL)
        OnData(&buffer[0],buffer.size());
    buffer.clear();
}

void FLVWritter::reset(){
    buffer.clear();
}

void FLVWritter::writeFLVFileHeader(bool haveAudio, bool haveVideo){
    uint8_t flv_header[13] = {
        0x46, 0x4c, 0x56, 0x01, 0x05,
        0x00, 0x00, 0x00, 0x09, 0x00,
        0x00, 0x00, 0x00
    };

    if (haveAudio && haveVideo) {
        flv_header[4] = 0x05;
    } else if (haveAudio && !haveVideo) {
        flv_header[4] = 0x04;
    } else if (!haveAudio && haveVideo) {
        flv_header[4] = 0x01;
    } else {
        flv_header[4] = 0x00;
    }

    for(int i=0;i<13;i++)
        buffer.push_back(flv_header[i]);
}

void FLVWritter::writeUInt8(uint8_t v) {
    buffer.push_back(v);
}

void FLVWritter::writeUInt16(uint32_t v){
    buffer.push_back((v >> 8) & 0xff);
    buffer.push_back((v) & 0xff);
}

void FLVWritter::writeUInt24(uint32_t v){
    buffer.push_back((v >> 16) & 0xff);
    buffer.push_back((v >> 8) & 0xff);
    buffer.push_back((v) & 0xff);
}

void FLVWritter::writeUInt32(uint32_t v){
    buffer.push_back((v >> 24) & 0xff);
    buffer.push_back((v >> 16) & 0xff);
    buffer.push_back((v >> 8) & 0xff);
    buffer.push_back((v) & 0xff);
}

void FLVWritter::writeUInt32(uint32_t v, std::vector<uint8_t> *data){
    data->push_back((v >> 24) & 0xff);
    data->push_back((v >> 16) & 0xff);
    data->push_back((v >> 8) & 0xff);
    data->push_back((v) & 0xff);
}

void FLVWritter::writeUInt32Timestamp(uint32_t v){
    buffer.push_back((v >> 16) & 0xff);
    buffer.push_back((v >> 8) & 0xff);
    buffer.push_back((v) & 0xff);
    buffer.push_back((v >> 24) & 0xff);
}

void FLVWritter::writeVideoSequenceHeader(
    const std::vector<uint8_t> &sps, 
    const std::vector<uint8_t> &pps, 
    uint8_t profile_idc, // comes from spsinfo
    uint8_t constraints, // comes from spsinfo
    uint8_t level_idc, // comes from spsinfo
    uint32_t timestamp_ms,
    int streamID) {

    if (endOfStreamWritten)
        return;

    //
    // flv tag header = 11 bytes
    //
    writeUInt8(0x09);//tagtype video
    writeUInt24( sps.size() + pps.size() + 16 );//data len
    writeUInt32Timestamp( timestamp_ms );//timestamp
    writeUInt24( streamID );//stream id 0

    //
    // Message Body = 16 bytes + SPS bytes + PPS bytes
    //
    //flv VideoTagHeader
    writeUInt8(0x17);//key frame, AVC 1:keyframe 7:h264
    writeUInt8(0x00);//avc sequence header
    writeUInt24( 0x00 );//composition time

    //flv VideoTagBody --AVCDecoderCOnfigurationRecord
    writeUInt8(0x01);//configurationversion
    writeUInt8(profile_idc);//avcprofileindication
    writeUInt8(constraints);//profilecompatibilty
    writeUInt8(level_idc);//avclevelindication
    writeUInt8(0xFC | 0x03); //reserved (6 bits), NALU length size - 1 (2 bits)
    writeUInt8(0xe0 | 0x01); // first reserved, second number of SPS

    writeUInt16( sps.size() ); //sequence parameter set length
    //H264 sequence parameter set raw data
    for(int i=0;i<sps.size();i++)
        writeUInt8(sps[i]);

    writeUInt8(0x01); // number of PPS

    writeUInt16(pps.size()); //picture parameter set length
    //H264 picture parameter set raw data
    for(int i=0;i<pps.size();i++)
        writeUInt8(pps[i]);

    if (buffer.size() != sps.size() + pps.size() + 16 + 11 ){
        fprintf(stderr, "error writeVideoSequenceHeader\n");
        exit(-1);
    }

    // previous tag size
    writeUInt32(buffer.size());
}

void FLVWritter::writeVideoFrame(const std::vector<uint8_t> &data, bool keyframe, uint32_t timestamp_ms, int streamID){

    if (endOfStreamWritten)
        return;

    writeUInt8(0x09);//tagtype video
    writeUInt24( data.size() + 5 );//data len
    writeUInt32Timestamp( timestamp_ms );//timestamp
    writeUInt24( streamID );//stream id 0)

    if (keyframe)
        writeUInt8(0x17);//key frame, AVC 1:keyframe 2:inner frame 7:H264
    else
        writeUInt8(0x27);//key frame, AVC 1:keyframe 2:inner frame 7:H264

    writeUInt8(0x01);//avc NALU unit
    writeUInt24(0x00);//composit time ??????????

    for(int i=0;i<data.size();i++)
        writeUInt8(data[i]);

    if (buffer.size() != data.size() + 5 + 11 ){
        fprintf(stderr, "error writeVideoFrame\n");
        exit(-1);
    }

    // previous size
    writeUInt32(buffer.size());

}

void FLVWritter::writeVideoEndOfStream(uint32_t timestamp_ms, int streamID){
    if (endOfStreamWritten)
        return;

    endOfStreamWritten = true;

    writeUInt8(0x09);//tagtype video
    writeUInt24( 5 );//data len
    writeUInt32Timestamp( timestamp_ms );//timestamp
    writeUInt24( streamID );//stream id 0)

    writeUInt8(0x17);//key frame, AVC 1:keyframe 2:inner frame 7:H264
    writeUInt8(0x02);//avc EOS
    writeUInt24(0x00);//composit time ??????????

    if (buffer.size() != 5 + 11 ){
        fprintf(stderr, "error writeVideoEOS\n");
        exit(-1);
    }

    // previous size
    writeUInt32(buffer.size());
}


uint8_t FLVWritter::aac2FlvAudioTag(uint8_t aacSampleFrequencyIndex,
                                    uint8_t aacChannelConfiguration) {
    //
    // Convert to FLV audio tag
    //
    // 0 mono, 1 stero
    uint8_t flvSoundType = aacChannelConfiguration - 1;
    uint8_t flvSoundRate = 0;
    switch (aacSampleFrequencyIndex) {
        case 10:
            flvSoundRate = 1;//11025
            break;
        case 7:
            flvSoundRate = 2;//22050
            break;
        case 4:
            flvSoundRate = 3;//44100
            break;
        default:
            return 0;
    }
    return 0xa0 | (flvSoundRate << 2) | 0x02 | flvSoundType;
}


void FLVWritter::writeAudioHeader(
    uint8_t aacAudioObjectType,
    uint8_t aacSampleFrequencyIndex,
    uint8_t aacChannelConfiguration,
    uint32_t timestamp_ms, 
    int streamID){
    if (endOfStreamWritten)
        return;

    writeUInt8(0x08);//tagtype audio
    writeUInt24( 4 );//data len
    writeUInt32Timestamp( timestamp_ms );//timestamp
    writeUInt24( streamID );//stream id 0
    
    //flv AudioTagHeader
    writeUInt8(aac2FlvAudioTag(aacSampleFrequencyIndex,
                               aacChannelConfiguration )); // sound format aac
    writeUInt8(0x00); //aac sequence header
    //flv AudioSpecificConfig
    writeUInt8(((aacAudioObjectType + 1) << 3) | (aacSampleFrequencyIndex >> 1));
    writeUInt8(((aacSampleFrequencyIndex & 0x01) << 7) | (aacChannelConfiguration << 3));

    if (buffer.size() != 4 + 11 ){
        fprintf(stderr, "error writeAudioHeader\n");
        exit(-1);
    }
    // previous size
    writeUInt32(buffer.size());
}

void FLVWritter::writeAudioFrame(
    const uint8_t *data,
    size_t size,
    uint8_t aacSampleFrequencyIndex,
    uint8_t aacChannelConfiguration,
    uint32_t timestamp_ms, 
    int streamID){
    
    if (endOfStreamWritten)
        return;

    const uint32_t AAC_ADTS_HEADER_SIZE = 7;
    
    writeUInt8(0x08);//tagtype audio
    writeUInt24( 2 + size - AAC_ADTS_HEADER_SIZE );//data len
    writeUInt32Timestamp( timestamp_ms );//timestamp
    writeUInt24( streamID );//stream id 0
    
    //flv AudioTagHeader
    writeUInt8(aac2FlvAudioTag(aacSampleFrequencyIndex,
                               aacChannelConfiguration )); // sound format aac
    writeUInt8(0x01); //aac raw data
    //flv raw aac data
    for (int i=AAC_ADTS_HEADER_SIZE;i<size;i++)
        writeUInt8(data[i]);
    
    if (buffer.size() != 2 + size - AAC_ADTS_HEADER_SIZE + 11 ){
        fprintf(stderr, "error writeAudioFrame\n");
        exit(-1);
    }
    // previous size
    writeUInt32(buffer.size());
}

/*
void FLVWritter::writeAudioEndOfStream(uint32_t timestamp_ms, int streamID) {
    //

}
*/


