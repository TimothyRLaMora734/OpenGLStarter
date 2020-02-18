#ifndef FLV_MUXER_H_
#define FLV_MUXER_H_

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "FractionalIncrementer.h"
#include "common.h"

#include "ADTSParser.h"
#include "H264Parser.h"
#include "H264FrameDetector.h"
#include "FLVWritter.h"

class FLVMuxer{
    FLV_OnDataMethodPtrT OnWriteData;

    bool writeVideo;
    bool writeAudio;

    FLVWritter mFLVWritter;
    
    //ADTS/AAC
    ADTSParser mADTSParser;
    FractionalIncrementer ADTSTimestamp;
    
    // H264
    H264Parser mH264Parser;
    H264FrameDetector mH264FrameDetector;
    sps_info spsInfo;
    uint32_t lastCurrentFrame;
    //contains the several NALs until complete a frame... 
    //after that can write to FLV
    std::vector<uint8_t> nalBuffer;
    std::vector<uint8_t> sps;
    std::vector<uint8_t> pps;

    FractionalIncrementer H264Timestamp;

    void onADTSChunk(const void* buffer, size_t size);
    void onH264Chunk(const void* buffer, size_t size);

public:
    FLVMuxer(bool _writeVideo,
             bool _writeAudio,
             uint32_t _videoFPS = 30,
             uint32_t _audioSamplerate = 44100,
             const FLV_OnDataMethodPtrT &_OnWriteData = NULL);

    void writeHeader();
    void writeADTS(const void* buffer, size_t size);
    void writeH264(const void* buffer, size_t size);
    void endOfStream();
};

#endif
