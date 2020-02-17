#include "H264FrameDetector.h"


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



H264FrameDetector::H264FrameDetector() {
    currentFrame = 0;
    newFrameOnNextIDR = false;
    old_frame_num = 0;
    spsinfo_set = false;
    firstFrame = true;
    newFrameFound = false;
    last_frame_delta = 1;
}

void H264FrameDetector::analyseBufferForNewFrame(const uint8_t *nal, size_t nalSize, const sps_info &spsinfo,bool ppsset){

    uint8_t nal_bit = nal[0];
    uint8_t nal_type = (nal_bit & 0x1f);

    if ( nal_type == (NAL_TYPE_SPS) ||
        nal_type == (NAL_TYPE_PPS) ||
        nal_type == (NAL_TYPE_AUD) ||
        nal_type == (NAL_TYPE_SEI) ||
        (nal_type >= 14 && nal_type <= 18)
        ) {
        newFrameOnNextIDR = true;
    }

    if ((nal_type == NAL_TYPE_CSIDRP ||
            nal_type == NAL_TYPE_CSNIDRP)
        && spsinfo.set && ppsset ){

        aux.clear();
        //(8 + 3*(32*2+1) + 16) = max header per NALU slice bits = 27.375 bytes
        // 32 bytes + 8 (Possibility of Emulation in 32 bytes)
        int RBSPMaxBytes = 32 + 8;
        if (nalSize < (32 + 8))
            RBSPMaxBytes = nalSize;
        H264Parser::nal2RBSP(nal, nalSize, &aux, RBSPMaxBytes );
        uint8_t * rbspBuffer = &aux[0];
        uint32_t rbspBufferSize = aux.size();

        uint32_t frame_num_index = 8;//start counting after the nal_bit
        //first_mb_in_slice (ue)
        frame_num_index += H264Parser::bitsToSkip_ue(frame_num_index, rbspBuffer, rbspBufferSize);
        //slice_type (ue)
        frame_num_index += H264Parser::bitsToSkip_ue(frame_num_index, rbspBuffer, rbspBufferSize);
        //pic_parameter_set_id (ue)
        frame_num_index += H264Parser::bitsToSkip_ue(frame_num_index, rbspBuffer, rbspBufferSize);
        //now can read frame_num
        uint32_t frame_num = H264Parser::readbits(frame_num_index,
                                                    spsinfo.log2_max_frame_num,
                                                    rbspBuffer, rbspBufferSize);

        if (!spsinfo_set){
            old_frame_num = frame_num;
            spsinfo_set = true;//spsinfo.set
        }

        if (old_frame_num != frame_num){
            newFrameOnNextIDR = true;
            
            //frame_num will be in the range [0, ( 1 << spsinfo.log2_max_frame_num )[
            last_frame_delta = ( frame_num - old_frame_num ) & spsinfo.frame_num_mask;
            
            old_frame_num = frame_num;
        }
    }


    if (newFrameOnNextIDR &&(nal_type == NAL_TYPE_CSIDRP ||
                                nal_type == NAL_TYPE_CSNIDRP)) {
        newFrameOnNextIDR = false;
        if (firstFrame){//skip the first frame
            firstFrame = false;
        } else {
            newFrameFound = true;
            //currentFrame++;
            currentFrame += last_frame_delta;
        }
    }
}

void H264FrameDetector::reset(){
    newFrameFound = false;
}
