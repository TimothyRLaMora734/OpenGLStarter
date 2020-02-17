#ifndef H264_PARSER_H
#define H264_PARSER_H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "common.h"

struct sps_info{
    uint8_t profile_idc;
    uint8_t constraints;
    uint8_t level_idc;
    uint32_t log2_max_frame_num;
    uint32_t frame_num_mask;
    bool set;
};

class H264Parser {
    
    enum State{
        None,
        Data,
        NAL0,
        NAL1,

        EMULATION0,
        EMULATION1,
        EMULATION_FORCE_SKIP
    };

    OnDataMethodPtrT OnWriteData;
    std::vector<uint8_t> buffer;
    State nalState; // nal = network abstraction layer
    State writingState;

    void putByte(uint8_t byte);

public:

    H264Parser(const OnDataMethodPtrT &_OnWriteData);
    virtual ~H264Parser();

    void endOfStream();

    // rawBuffer might has emulation bytes
    // Raw Byte Sequence Payload (RBSP)
    void parse(const uint8_t* ibuffer, int size);


    static uint32_t readbit(int bitPos,  const uint8_t* data, int size);

    // leading 0`s count equals the number of next bits after bit 1
    //
    //  Example: 01x  001xx 0001xxx 00001xxxx
    //
    //  The max number of bits is equal 32 in this sintax
    //
    static int bitsToSkip_ue( int start,  const uint8_t* data, int size);
    static uint32_t read_golomb_ue(int start,  const uint8_t* data, int size);
    static uint32_t readbits(int bitPos, int length, const uint8_t* data, int size);
    static sps_info parseSPS(const std::vector<uint8_t> sps_rbsp);

    // Raw Byte Sequence Payload (RBSP) -- without the emulation prevention bytes
    // maxSize is used to parse just the beggining of the nal structure...
    // avoid process all buffer size on NAL new frame check
    static void nal2RBSP(const uint8_t *buffer, size_t size,
                                std::vector<uint8_t> *rbsp,
                                int maxSize = -1);

};



#endif
