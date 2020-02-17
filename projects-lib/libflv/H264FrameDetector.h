#ifndef H264_FRAME_DETECTOR_H
#define H264_FRAME_DETECTOR_H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "H264Parser.h"

//  Detection of the first VCL NAL unit of a primary coded picture
// https://stackoverflow.com/questions/19642736/count-frames-in-h-264-bitstream?rq=1
class H264FrameDetector {
    std::vector<uint8_t> aux;

    bool newFrameOnNextIDR;
    uint32_t old_frame_num;
    uint32_t last_frame_delta;
    bool spsinfo_set;

    bool firstFrame;
public:
    uint32_t currentFrame;
    bool newFrameFound;

    H264FrameDetector();

    void analyseBufferForNewFrame(
        const uint8_t *nal, size_t nalSize, 
        const sps_info &spsinfo,
        bool ppsset);
    void reset();

};


#endif
