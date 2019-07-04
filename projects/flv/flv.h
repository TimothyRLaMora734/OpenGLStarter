#ifndef FLV__H
#define FLV__H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

// https://yumichan.net/video-processing/video-compression/introduction-to-h264-nal-unit/

#define NAL_IDC_FIELD (1 << 5)
#define NAL_IDC_FRAME (2 << 5)
#define NAL_IDC_PICTURE (3 << 5)

#define NAL_TYPE_CSNIDRP 1 // Coded slice of a non-IDR picture
#define NAL_TYPE_CSDPA 2 // Coded slice data partition A
#define NAL_TYPE_CSDPB 3 //   Coded slice data partition B
#define NAL_TYPE_CSDPC 4 //   Coded slice data partition C
#define NAL_TYPE_CSIDRP 5 //   Coded slice of an IDR picture
#define NAL_TYPE_SEI 6 //   Supplemental enhancement information (SEI)
#define NAL_TYPE_SPS 7 //   Sequence parameter set
#define NAL_TYPE_PPS 8 //   Picture parameter set
#define NAL_TYPE_AUD 9 //   Access unit delimiter
#define NAL_TYPE_EOSEQ 10 //   End of sequence
#define NAL_TYPE_EOSTREAM 11 //   End of stream
#define NAL_TYPE_FD 12 //   Filler data
#define NAL_TYPE_SPSE 13 //   Sequence parameter set extension
#define NAL_TYPE_PNALU 14 //   Prefix NAL unit
#define NAL_TYPE_SSPS 15 //   Subset sequence parameter set
#define NAL_TYPE_CSOAACPWP 19 //   Coded slice of an auxiliary coded picture without partitioning
#define NAL_TYPE_CSE 20 //   Coded slice extension
#define NAL_TYPE_CSEFDVC 21 //   Coded slice extension for depth view components

class ParserH264 {
    
    std::vector<uint8_t> buffer;
    
    enum State{
        None,
        Data,
        NAL0,
        NAL1,
        NAL2
    };
    State nalState; // nal = network abstraction layer
    State writingState;
    
    void putByte(uint8_t byte) {
        if (byte == 0x00 && nalState == None)
            nalState = NAL0;
        else if (byte == 0x00 && nalState == NAL0)
            nalState = NAL1;
        else if (byte == 0x00 && (nalState == NAL1 || nalState == NAL2) )
            nalState = NAL2;
        else if (byte == 0x01 && nalState == NAL2){
            nalState = None;
            //detected a NAL info
            if (writingState == None){
                writingState = Data;
                buffer.push_back(0x00);
                buffer.push_back(0x00);
                buffer.push_back(0x00);
            } else if (writingState == Data){
                buffer.pop_back();// 0x00
                buffer.pop_back();// 0x00
                buffer.pop_back();// 0x00
                
                chunkDetectedH264(&buffer[0],buffer.size());
                buffer.resize(3);
            }
        } else
            nalState = None;
        
        if (writingState == Data)
            buffer.push_back(byte);
    }
    
public:
    
    ParserH264() {
        nalState = None;
        writingState = None;
    }
    
    virtual ~ParserH264(){
    }
    
    virtual void chunkDetectedH264(const uint8_t* ibuffer, int size){
    }
    
    void endOfStreamH264() {
        if (buffer.size() <= 0)
            return;
        
        chunkDetectedH264(&buffer[0],buffer.size());
        buffer.clear();
        writingState = None;
        nalState = None;
    }
    
    void parseH264(const uint8_t* ibuffer, int size) {
        for(int i=0;i<size;i++){
            putByte(ibuffer[i]);
        }
    }
};


#define AAC_ADTS_HEADER_SIZE 7


uint8_t aac2FlvAudioTag(uint8_t aacAudioObjectType,
                        uint8_t aacSampleFrequencyIndex,
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

uint8_t aac2FlvObjTAndFreq(uint8_t aacAudioObjectType,
                           uint8_t aacSampleFrequencyIndex,
                           uint8_t aacChannelConfiguration) {
    uint8_t audioObjectType = aacAudioObjectType + 1;
    return (audioObjectType << 3) | (aacSampleFrequencyIndex >> 1);
}

class ParserAAC {
    std::vector<uint8_t> buffer;
    
    enum State{
        None,
        Data,
        ADTS_SECOND_BYTE,
        ADTS_HEADER_SIZE,
        ADTS_LENGTH_SIZE
    };
    
    State adtsState;
    uint32_t frameLength;
    
    void putByte(uint8_t byte) {
        
        if ( byte == 0xff ) { // && adtsState == None ) {
            adtsState = ADTS_SECOND_BYTE;
        } else if ( (byte & 0xf0) == 0xf0 && adtsState == ADTS_SECOND_BYTE) {
            adtsState = ADTS_HEADER_SIZE;
            
            buffer.clear();
            buffer.push_back(0xff);
            buffer.push_back(byte);
            
        } else if (adtsState == ADTS_HEADER_SIZE) {
            buffer.push_back(byte);
            if (buffer.size() == ADTS_HEADER_SIZE) {
                frameLength = ((buffer[3] & 0x03) << 11) | (buffer[4] << 3) | ((buffer[5] & 0xe0) >> 5);
                adtsState = ADTS_LENGTH_SIZE;
            }
        } else if (adtsState == ADTS_LENGTH_SIZE) {
            buffer.push_back(byte);
            if (buffer.size() == ADTS_HEADER_SIZE + frameLength) {
                chunkDetectedAAC(&buffer[0], buffer.size());
                adtsState = None;
            }
        } else {
            adtsState = None;
        }
    }
    
public:
    
    ParserAAC() {
        adtsState = None;
    }
    
    virtual ~ParserAAC(){
    }
    
    virtual void chunkDetectedAAC(const uint8_t* ibuffer, int size){
    }
    
    void parseAAC(const uint8_t* ibuffer, int size) {
        for(int i=0;i<size;i++){
            putByte(ibuffer[i]);
        }
    }
};


class FLVWritter{
public:
    std::vector<uint8_t> buffer;
    
    void flushToFD(int fd) {
        if (buffer.size() <= 0)
            return;
        ::write(fd,&buffer[0],buffer.size());
        buffer.clear();
    }
    
    void reset(){
        buffer.clear();
    }
    
    void writeFLVFileHeader(bool haveAudio, bool haveVideo){
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
    
    void writeUInt8(uint8_t v) {
        buffer.push_back(v);
    }
    
    void writeUInt16(uint32_t v){
        buffer.push_back((uint8_t)(v >> 8));
        buffer.push_back((uint8_t)(v));
    }
    
    void writeUInt24(uint32_t v){
        buffer.push_back((uint8_t)(v >> 16));
        buffer.push_back((uint8_t)(v >> 8));
        buffer.push_back((uint8_t)(v));
    }
    
    void writeUInt32(uint32_t v){
        buffer.push_back((uint8_t)(v >> 24));
        buffer.push_back((uint8_t)(v >> 16));
        buffer.push_back((uint8_t)(v >> 8));
        buffer.push_back((uint8_t)(v));
    }
    
    void writeUInt32Timestamp(uint32_t v){
        buffer.push_back((uint8_t)(v >> 16));
        buffer.push_back((uint8_t)(v >> 8));
        buffer.push_back((uint8_t)(v));
        buffer.push_back((uint8_t)(v >> 24));
    }
    
};



enum FLV_TAG_TYPE {
    FLV_TAG_TYPE_AUDIO = 0x08,
    FLV_TAG_TYPE_VIDEO = 0x09,
    FLV_TAG_TYPE_META = 0x12
};


class FLVFileWriter: public ParserAAC, public ParserH264 {
    
    std::vector<uint8_t> lastSPS;
    
public:
    FLVWritter mFLVWritter;
    int fd;
    bool firstAudioWrite;
    uint32_t audioTimestamp_ms;
    uint32_t videoTimestamp_ms;
    
    FLVFileWriter ( const char* file ) {
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC , 0644 );
        if (fd < 0){
            fprintf(stderr,"error to create flv file\n");
            exit(-1);
        }
        
        //have audio, have video
        mFLVWritter.writeFLVFileHeader(false, true);
        mFLVWritter.flushToFD(fd);
        
        firstAudioWrite = true;
        audioTimestamp_ms = 0;
        videoTimestamp_ms = 0;
    }
    
    virtual ~FLVFileWriter(){
        if (fd >= 0){
            mFLVWritter.flushToFD(fd);
            close(fd);
        }
    }
    
    void chunkDetectedH264(const uint8_t* ibuffer, int size) {
        if (size < 4){
            fprintf(stdout, "  error On h264 chunk detection\n");
            return;
        }

        uint8_t nal_bit = ibuffer[4];
        //0x67
        if (nal_bit == (NAL_IDC_PICTURE | NAL_TYPE_SPS) ) {
            
            fprintf(stdout, " processing: 0x%x (SPS)\n",nal_bit);
            
            //store information to use when arrise PPS nal_bit, probably the next NAL detection
            lastSPS.clear();
            for(int i=4;i<size;i++)
                lastSPS.push_back(ibuffer[i]);
        }
        else if (nal_bit == (NAL_IDC_PICTURE | NAL_TYPE_PPS) ) {
            
            fprintf(stdout, " processing: 0x%x (PPS)\n",nal_bit);
            
            //must be called just after the SPS detection
            int32_t bodyLength = lastSPS.size() + (size-4) + 16;
            
            //
            // flv tag header = 11 bytes
            //
            mFLVWritter.writeUInt8(0x09);//tagtype video
            mFLVWritter.writeUInt24( bodyLength );//data len
            mFLVWritter.writeUInt32Timestamp( videoTimestamp_ms );//timestamp
            mFLVWritter.writeUInt24( 0 );//stream id 0
            
            //
            // Message Body = 16 bytes + SPS bytes + PPS bytes
            //
            //flv VideoTagHeader
            mFLVWritter.writeUInt8(0x17);//key frame, AVC 1:keyframe 7:h264
            mFLVWritter.writeUInt8(0x00);//avc sequence header
            mFLVWritter.writeUInt24( 0x00 );//composit time ??????????
            
            //flv VideoTagBody --AVCDecoderCOnfigurationRecord
            mFLVWritter.writeUInt8(0x01);//configurationversion
            mFLVWritter.writeUInt8(lastSPS[1]);//avcprofileindication
            mFLVWritter.writeUInt8(lastSPS[2]);//profilecompatibilty
            mFLVWritter.writeUInt8(lastSPS[3]);//avclevelindication
            mFLVWritter.writeUInt8(0xff); //reserved + lengthsizeminusone
            mFLVWritter.writeUInt8(0xe0 | 0x01); // first reserved, second number of SPS
            
            mFLVWritter.writeUInt16( lastSPS.size() ); //sequence parameter set length
            //H264 sequence parameter set raw data
            for(int i=0;i<lastSPS.size();i++)
                mFLVWritter.writeUInt8(lastSPS[i]);
            
            mFLVWritter.writeUInt8(0x01); // number of PPS
            
            mFLVWritter.writeUInt16(size-4); //picture parameter set length
            //H264 picture parameter set raw data
            for(int i=4;i<size;i++)
                mFLVWritter.writeUInt8(ibuffer[i]);
            
            //
            // previous tag size
            //
            uint32_t currentSize = mFLVWritter.buffer.size();
            if (currentSize != bodyLength + 11 ){
                fprintf(stderr, "error to write flv video tag NAL_TYPE_PPS\n");
                exit(-1);
            }
            mFLVWritter.writeUInt32(currentSize);//data len
            mFLVWritter.flushToFD(fd);
            
            
            //videoTimestamp_ms += 66;
        }
        //0x65
        else if (nal_bit == (NAL_IDC_FRAME | NAL_TYPE_CSIDRP) ) {
            
            fprintf(stdout, " processing: 0x%x (0x65)\n",nal_bit);
            
            videoTimestamp_ms += 66;
            
            uint32_t bodyLength = (size-4) + 5 + 4;//flv VideoTagHeader +  NALU length
            
            //
            // flv tag header = 11 bytes
            //
            mFLVWritter.writeUInt8(0x09);//tagtype video
            mFLVWritter.writeUInt24( bodyLength );//data len
            mFLVWritter.writeUInt32Timestamp( videoTimestamp_ms );//timestamp
            mFLVWritter.writeUInt24( 0 );//stream id 0
            
            //
            // Message Body = VideoTagHeader(5) + NALLength(4) + NAL raw data
            //
            //flv VideoTagHeader
            mFLVWritter.writeUInt8(0x17);//key frame, AVC 1:keyframe 2:inner frame 7:H264
            mFLVWritter.writeUInt8(0x01);//avc NALU unit
            mFLVWritter.writeUInt24(0x00);//composit time ??????????
            mFLVWritter.writeUInt32(size-4);//nal length
            
            //nal raw data
            for(int i=4;i<size;i++)
                mFLVWritter.writeUInt8(ibuffer[i]);
            
            //
            // previous tag size
            //
            uint32_t currentSize = mFLVWritter.buffer.size();
            if (currentSize != bodyLength + 11 ){
                fprintf(stderr, "error to write flv video tag NAL_TYPE_CSIDRP\n");
                exit(-1);
            }
            mFLVWritter.writeUInt32(currentSize);//data len
            mFLVWritter.flushToFD(fd);
        }
        //0x61
        else if (nal_bit == (NAL_IDC_FRAME | NAL_TYPE_CSNIDRP) ) {
            
            fprintf(stdout, " processing: 0x%x (0x61)\n",nal_bit);
            
            videoTimestamp_ms += 66;
            
            uint32_t bodyLength = (size-4) + 5 + 4;//flv VideoTagHeader +  NALU length
            
            //
            // flv tag header = 11 bytes
            //
            mFLVWritter.writeUInt8(0x09);//tagtype video
            mFLVWritter.writeUInt24( bodyLength );//data len
            mFLVWritter.writeUInt32Timestamp( videoTimestamp_ms );//timestamp
            mFLVWritter.writeUInt24( 0 );//stream id 0
            
            //
            // Message Body = VideoTagHeader(5) + NALLength(4) + NAL raw data
            //
            //flv VideoTagHeader
            mFLVWritter.writeUInt8(0x27);//key frame, AVC 1:keyframe 2:inner frame 7:H264
            mFLVWritter.writeUInt8(0x01);//avc NALU unit
            mFLVWritter.writeUInt24(0x00);//composit time ??????????
            mFLVWritter.writeUInt32(size-4);//nal length
            
            // raw nal data
            for(int i=4;i<size;i++)
                mFLVWritter.writeUInt8(ibuffer[i]);
            
            //
            // previous tag size
            //
            uint32_t currentSize = mFLVWritter.buffer.size();
            if (currentSize != bodyLength + 11 ){
                fprintf(stderr, "error to write flv video tag NAL_TYPE_CSNIDRP\n");
                exit(-1);
            }
            mFLVWritter.writeUInt32(currentSize);//data len
            mFLVWritter.flushToFD(fd);
            
        } else {
            // nal_bit type not implemented...
            fprintf(stdout, "unknown NAL bit: 0x%x\n",nal_bit);
        }
    }
    
    void chunkDetectedAAC(const uint8_t* ibuffer, int size){
        //
        // Extract AAC audio configuration from AAC header
        //
        uint8_t aacAudioObjectType = (ibuffer[2] & 0xc0) >> 6;
        uint8_t aacSampleFrequencyIndex =  (ibuffer[2] & 0x3c) >> 2;
        uint8_t aacChannelConfiguration = (ibuffer[3] & 0xc0) >> 6;
        
        //
        // write audio config
        //
        if (firstAudioWrite){
            firstAudioWrite = false;
            
            uint32_t bodyLength = 2 + 2; //AudioTagHeader + AudioSpecificConfig
            
            //
            // flv tag header = 11 bytes
            //
            mFLVWritter.writeUInt8(0x08);//tagtype audio
            mFLVWritter.writeUInt24( bodyLength );//data len
            mFLVWritter.writeUInt32Timestamp( audioTimestamp_ms );//timestamp
            mFLVWritter.writeUInt24( 0 );//stream id 0
            //
            // Message Body = 4 bytes
            //
            //flv AudioTagHeader
            mFLVWritter.writeUInt8( aac2FlvAudioTag(aacAudioObjectType,
                                                    aacSampleFrequencyIndex,
                                                    aacChannelConfiguration ) ); // sound format aac
            mFLVWritter.writeUInt8(0x00); //aac sequence header
            //flv AudioSpecificConfig
            mFLVWritter.writeUInt8( aac2FlvObjTAndFreq(aacAudioObjectType,
                                                       aacSampleFrequencyIndex,
                                                       aacChannelConfiguration ) );
            mFLVWritter.writeUInt8(((aacSampleFrequencyIndex & 0x01) << 7) |
                                   (aacChannelConfiguration << 3));
            
            //
            // previous tag size
            //
            uint32_t currentSize = mFLVWritter.buffer.size();
            if (currentSize != bodyLength + 11 ){
                fprintf(stderr, "error to write flv audio tag\n");
                exit(-1);
            }
            mFLVWritter.writeUInt32(currentSize);//data len
            mFLVWritter.flushToFD(fd);
        }
        
        //
        // write audio data
        //
        uint32_t bodyLength = 2 + size - AAC_ADTS_HEADER_SIZE; // AudioTagHeader + raw AAC w/o header
        
        //
        // flv tag header = 11 bytes
        //
        mFLVWritter.writeUInt8(0x08);//tagtype audio
        mFLVWritter.writeUInt24( bodyLength );//data len
        mFLVWritter.writeUInt32Timestamp( audioTimestamp_ms );//timestamp
        mFLVWritter.writeUInt24( 0 );//stream id 0
        //
        // Message Body = 2 + size - AAC_ADTS_HEADER_SIZE bytes
        //
        //flv AudioTagHeader
        mFLVWritter.writeUInt8( aac2FlvAudioTag(aacAudioObjectType,
                                                aacSampleFrequencyIndex,
                                                aacChannelConfiguration ) ); // sound format aac
        mFLVWritter.writeUInt8(0x01); //aac raw data
        //flv raw aac data
        for (int i=AAC_ADTS_HEADER_SIZE;i<size;i++)
            mFLVWritter.writeUInt8(ibuffer[i]);
        //
        // previous tag size
        //
        uint32_t currentSize = mFLVWritter.buffer.size();
        if (currentSize != bodyLength + 11 ){
            fprintf(stderr, "error to write flv audio data\n");
            exit(-1);
        }
        mFLVWritter.writeUInt32(currentSize);//data len
        mFLVWritter.flushToFD(fd);
        audioTimestamp_ms += 100;
    }
};

#endif