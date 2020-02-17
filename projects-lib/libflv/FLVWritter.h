#ifndef FLV_WRITER_H
#define FLV_WRITER_H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "common.h"

class FLVWritter{

    void writeUInt8(uint8_t v);
    void writeUInt16(uint32_t v);
    void writeUInt24(uint32_t v);
    void writeUInt32(uint32_t v);
    void writeUInt32Timestamp(uint32_t v);

    uint8_t aac2FlvAudioTag(uint8_t aacSampleFrequencyIndex,
                            uint8_t aacChannelConfiguration);

    bool endOfStreamWritten;
public:
    std::vector<uint8_t> buffer;

    void flush(OnDataMethodPtrT &OnData);

    void reset();
    void writeFLVFileHeader(bool haveAudio, bool haveVideo);

    // Video FLV write calls
    void writeVideoSequenceHeader(
        const std::vector<uint8_t> &sps, 
        const std::vector<uint8_t> &pps, 
        uint8_t profile_idc, // comes from spsinfo
        uint8_t constraints, // comes from spsinfo
        uint8_t level_idc, // comes from spsinfo
        uint32_t timestamp_ms,
        int streamID);
    void writeVideoFrame(
        const std::vector<uint8_t> &data, 
        bool keyframe, 
        uint32_t timestamp_ms, 
        int streamID);
    void writeVideoEndOfStream(uint32_t timestamp_ms, int streamID);

    void writeAudioHeader(
        uint8_t aacAudioObjectType, // comes from ADTS header
        uint8_t aacSampleFrequencyIndex, // comes from ADTS header
        uint8_t aacChannelConfiguration, // comes from ADTS header
        uint32_t timestamp_ms, 
        int streamID);
    void writeAudioFrame(
        const uint8_t *data,
        size_t size,
        uint8_t aacSampleFrequencyIndex, // comes from ADTS header
        uint8_t aacChannelConfiguration, // comes from ADTS header
        uint32_t timestamp_ms, 
        int streamID);

    //void writeAudioEndOfStream(uint32_t timestamp_ms, int streamID);

    static void writeUInt32(uint32_t v, std::vector<uint8_t> *data);
};

#endif
