#ifndef FFmpegADTSMuxer__H
#define FFmpegADTSMuxer__H

#include "FFmpegCommon.h"
#include "FFmpegBitstreamFilterADTStoASC.h"

#define USE_BITSTREAM_FILTER 1

class FFmpegADTSMuxer {

    AVOutputFormat *adts_container;
    AVFormatContext *adts_container_ctx;
    uint8_t *adts_container_buffer;
    size_t adts_container_buffer_size;

    //callback registration
    AVIOContext *adts_avio_ctx;
    AVStream *adts_stream;

    int64_t curr_pts;
    int encoded_pkt_counter;

    FFmpeg_OnDataMethodPtrT OnData;

    //external ref:
    AVCodecContext* audio_encoder_ctx;

    bool initialized;

#if USE_BITSTREAM_FILTER
    FFmpegBitstreamFilterADTStoASC filter;
    uint8_t FakeADTSHeader[7];

    void onDataFromFilter( AVPacket* pkt );
#endif

    static int onDataFromMuxer(void *userData, uint8_t *data, int size);

public:

    FFmpegADTSMuxer();
    virtual ~FFmpegADTSMuxer();

    void init(AVCodecContext* audio_encoder_ctx, const FFmpeg_OnDataMethodPtrT &OnData);
    //need to link this method to the OnData from the AudioEncoder
    void writeData(AVFrame *input_frame, AVPacket *encoded_packet);
    void endStream();
};

#endif