#include "FLVMuxer.h"

// https://yumichan.net/video-processing/video-compression/introduction-to-h264-nal-unit/

#define NAL_IDC_FIELD (1 << 5) // 0x20
#define NAL_IDC_FRAME (2 << 5) // 0x40
#define NAL_IDC_PICTURE (3 << 5) // 0x60

#define NAL_TYPE_CSNIDRP 0x01 // Coded slice of a non-IDR picture
#define NAL_TYPE_CSDPA 0x02 // Coded slice data partition A
#define NAL_TYPE_CSDPB 0x03 //   Coded slice data partition B
#define NAL_TYPE_CSDPC 0x04 //   Coded slice data partition C
#define NAL_TYPE_CSIDRP 0x05 //   Coded slice of an IDR picture
#define NAL_TYPE_SEI 0x06 //   Supplemental enhancement information (SEI)
#define NAL_TYPE_SPS 0x07 //   Sequence parameter set
#define NAL_TYPE_PPS 0x08 //   Picture parameter set
#define NAL_TYPE_AUD 0x09 //   Access unit delimiter
#define NAL_TYPE_EOSEQ 0x0a // 10 //   End of sequence
#define NAL_TYPE_EOSTREAM 0x0b // 11 //   End of stream
#define NAL_TYPE_FillerData 0x0c // 12 //   Filler data
#define NAL_TYPE_SPSE 0x0d // 13 //   Sequence parameter set extension
#define NAL_TYPE_PrefixNALU 0x0e // 14 //   Prefix NAL unit
#define NAL_TYPE_SSPS 0x0f //15 //   Subset sequence parameter set
#define NAL_TYPE_CSOAACPWP 0x13 //19 //   Coded slice of an auxiliary coded picture without partitioning
#define NAL_TYPE_CSE 0x14 // 20 //   Coded slice extension
#define NAL_TYPE_CSEFDVC 0x15 // 21 //   Coded slice extension for depth view components


FLVMuxer::FLVMuxer(
    bool _writeVideo,
    bool _writeAudio,
    uint32_t _videoFPS,
    uint32_t _audioSamplerate,
    const OnDataMethodPtrT &_OnWriteData):
        mFLVWritter(),
        mADTSParser(OnDataMethodPtrT(this,&FLVMuxer::onADTSChunk)),
        mH264Parser(OnDataMethodPtrT(this,&FLVMuxer::onH264Chunk)){
    
    OnWriteData = _OnWriteData;

    writeVideo = _writeVideo;
    writeAudio = _writeAudio;

    //the video puts 1 sample per time
    H264Timestamp = FractionalIncrementer(1000,_videoFPS);
    //the audio puts 1024 samples per time
    ADTSTimestamp = FractionalIncrementer(1000*1024,_audioSamplerate);

    spsInfo.set = false;
    lastCurrentFrame = 0;
}

void FLVMuxer::onADTSChunk(const void* buffer, size_t size){
    //
    // Extract AAC audio configuration from AAC header
    //
    uint8_t *ibuffer = (uint8_t*)buffer;
    uint8_t aacAudioObjectType = (ibuffer[2] & 0xc0) >> 6;
    uint8_t aacSampleFrequencyIndex =  (ibuffer[2] & 0x3c) >> 2;
    uint8_t aacChannelConfiguration = (ibuffer[3] & 0xc0) >> 6;

    if (ADTSTimestamp.value_ms == 0){
        //write header
        mFLVWritter.writeAudioHeader(aacAudioObjectType, 
            aacSampleFrequencyIndex, 
            aacChannelConfiguration,
            ADTSTimestamp.value_ms,0);
        mFLVWritter.flush(OnWriteData);
    }

    mFLVWritter.writeAudioFrame(ibuffer, size, 
        aacSampleFrequencyIndex, 
        aacChannelConfiguration,
        ADTSTimestamp.value_ms,0);
    mFLVWritter.flush(OnWriteData);

    ADTSTimestamp.increment();
}

void FLVMuxer::onH264Chunk(const void* buffer, size_t size){
    uint8_t *data = (uint8_t*)buffer;

    uint8_t nal_bit = data[0];
    uint8_t nal_type = (nal_bit & 0x1f);

    mH264FrameDetector.analyseBufferForNewFrame(data, size, spsInfo, pps.size() > 0);

    if (mH264FrameDetector.newFrameFound){
        mH264FrameDetector.reset();

        //write the previous nalBuffer set related to previous frame.
        uint8_t firstNALtype = nalBuffer[4] & 0x1f;

        bool iskeyframe = (firstNALtype == NAL_TYPE_CSIDRP);
        mFLVWritter.writeVideoFrame(nalBuffer, iskeyframe, H264Timestamp.value_ms, 0);
        mFLVWritter.flush( OnWriteData );

        nalBuffer.clear();

        uint32_t delta = mH264FrameDetector.currentFrame - lastCurrentFrame;
        lastCurrentFrame = mH264FrameDetector.currentFrame;
        for(uint32_t i=0;i<delta;i++)
            H264Timestamp.increment();
        //videoTimestamp_ms = (mH264NewFrameDetection.currentFrame * 1000)/30;
    }

    //0x67
    //if (nal_bit == (NAL_IDC_PICTURE | NAL_TYPE_SPS) ) {
    if ( nal_type == (NAL_TYPE_SPS) ) {
        //fprintf(stdout, " processing: 0x%x (SPS)\n",nal_bit);

        sps.clear();
        //max 26 bytes on sps header 
        //(read until log2_max_frame_num_minus4)
        H264Parser::nal2RBSP( data, size , &sps, 26 );
        spsInfo = H264Parser::parseSPS(sps);

        sps.clear();
        for(int i=0;i<size;i++)
            sps.push_back(data[i]);
    }
    //0x68
    //else if (nal_bit == (NAL_IDC_PICTURE | NAL_TYPE_PPS) ) {
    else if ( nal_type == (NAL_TYPE_PPS) ) {
        //fprintf(stdout, " processing: 0x%x (PPS)\n",nal_bit);

        if (sps.size() == 0){
            fprintf(stderr,"trying to set PPS without previous SPS.\n");
            exit(-1);
        }

        pps.clear();
        for(int i=0;i<size;i++)
            pps.push_back(data[i]);

        mFLVWritter.writeVideoSequenceHeader(sps,
            pps, 
            spsInfo.profile_idc,
            spsInfo.constraints,
            spsInfo.level_idc,
            H264Timestamp.value_ms,0);
        mFLVWritter.flush(OnWriteData);

    }
    //0x65, 0x61, 0x41
    else if ( nal_type == NAL_TYPE_CSIDRP ||
                nal_type == NAL_TYPE_CSNIDRP ) {

        if (sps.size() > 0 && pps.size() > 0) {
            mFLVWritter.writeUInt32( size, &nalBuffer );
            for(int i=0;i<size;i++)
                nalBuffer.push_back(data[i]);
        }

    } else if (nal_type == (NAL_TYPE_SEI)) {
        //fprintf(stdout, " ignoring SEI bit: 0x%x type: 0x%x\n",nal_bit, nal_type);
    } else {
        // nal_bit type not implemented...
        fprintf(stdout, "Error: unknown NAL bit: 0x%x type: 0x%x\n",nal_bit, nal_type);
        exit(-1);
    }
}

void FLVMuxer::writeHeader() {
    mFLVWritter.writeFLVFileHeader(writeAudio,writeVideo);
    mFLVWritter.flush(OnWriteData);
}

void FLVMuxer::writeADTS(const void* buffer, size_t size) {
    mADTSParser.parse((uint8_t*)buffer,size);
}
void FLVMuxer::writeH264(const void* buffer, size_t size) {
    mH264Parser.parse((uint8_t*)buffer,size);
}

void FLVMuxer::endOfStream() {
    mH264Parser.endOfStream();
    if (H264Timestamp.value_ms > 0){

        // I don't know if is OK to push the 
        // remainder H264 stream before a 
        // complete frame detection.
        if (nalBuffer.size() > 0){
            uint8_t firstNALtype = nalBuffer[4] & 0x1f;

            bool iskeyframe = (firstNALtype == NAL_TYPE_CSIDRP);
            mFLVWritter.writeVideoFrame(nalBuffer, iskeyframe, H264Timestamp.value_ms,0);
            mFLVWritter.flush(OnWriteData);

            nalBuffer.clear();

            H264Timestamp.increment();
        }

        mFLVWritter.writeVideoEndOfStream(H264Timestamp.value_ms,0);
        mFLVWritter.flush(OnWriteData);
    }
}
