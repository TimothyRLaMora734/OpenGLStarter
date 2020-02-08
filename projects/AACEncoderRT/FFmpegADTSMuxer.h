#ifndef FFMPEG_ADTS_MUXER_ENCODER_H
#define FFMPEG_ADTS_MUXER_ENCODER_H

#include "FFmpegInit.h"

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

//https://stackoverflow.com/questions/2410459/encode-audio-to-aac-with-libavcodec
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/log.h>
#include <libavformat/avformat.h>
#include <libavutil/timestamp.h>
#include <libswresample/swresample.h>
}

#if defined(OS_TARGET_linux)

#undef av_err2str
//#define av_err2str(errnum) av_make_error_string((char*)__builtin_alloca(AV_ERROR_MAX_STRING_SIZE), AV_ERROR_MAX_STRING_SIZE, errnum)

av_always_inline char *av_err2str(int errnum) {
static char str[AV_ERROR_MAX_STRING_SIZE];
memset(str, 0, sizeof(str));
return av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, errnum);
}

#undef av_ts2timestr
//#define av_ts2timestr(ts, tb) av_ts_make_time_string((char[AV_TS_MAX_STRING_SIZE]){0}, ts, tb)

av_always_inline char *av_ts2timestr(int64_t ts, AVRational *tb) {
static char str[AV_TS_MAX_STRING_SIZE];
memset(str, 0, sizeof(str));
return av_ts_make_time_string(str, ts, tb);
}

#endif

BEGIN_DECLARE_DELEGATE(OnMuxerDataDelegate, uint8_t *data, int size ) CALL_PATTERN (data, size) END_DECLARE_DELEGATE;

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

    static int writeData(void *userData, uint8_t *data, int size){
        FFmpegADTSMuxer *muxer = (FFmpegADTSMuxer*)userData;
        muxer->OnData(data, size);
        return size;
    }

public:

    OnMuxerDataDelegate OnData;

    FFmpegADTSMuxer() {
        adts_container = NULL;
        adts_container_ctx = NULL;
        adts_container_buffer = NULL;
        adts_container_buffer_size = 4096;

        adts_avio_ctx = NULL;
        adts_stream = NULL;
    }

    virtual ~FFmpegADTSMuxer(){

        if (adts_container_ctx != NULL)
            avformat_free_context(adts_container_ctx);
        if (adts_container_buffer != NULL)
            av_freep(&adts_container_buffer);
        if (adts_avio_ctx != NULL)
            av_freep(&adts_avio_ctx);

        adts_container = NULL;
        adts_container_ctx = NULL;
        adts_container_buffer = NULL;
        adts_avio_ctx = NULL;
        adts_stream = NULL;
    }


    void init(AVCodecContext* audio_encoder_ctx) {

        globalInitFFMPEG();

        encoded_pkt_counter = 1;

        /*
         * Create the ADTS container for the encoded frames
         */
        adts_container = av_guess_format("adts", NULL, NULL);

        int ret_val;

        if (!adts_container) {
            fprintf(stderr, "Could not find adts output format\n");
            exit(1);
        }
        if ((ret_val = avformat_alloc_output_context2(&adts_container_ctx, adts_container, "", NULL)) < 0) {
            fprintf(stderr, "Could not create output context (error '%s')\n", av_err2str(ret_val));
            exit(1);
        }
        if (!(adts_container_buffer = (uint8_t*)av_malloc(adts_container_buffer_size))) {
            fprintf(stderr, "Could not allocate a buffer for the I/O output context\n");
            exit(1);
        }
        // Create an I/O context for the adts container with a write callback (write_adts_muxed_data()), so that muxed data will be accessed through this function.
        if (!(adts_avio_ctx = avio_alloc_context(adts_container_buffer,
                                                 adts_container_buffer_size,
                                                 1,
                                                 this,
                                                 NULL ,
                                                 &FFmpegADTSMuxer::writeData,
                                                 NULL))) {
            fprintf(stderr, "Could not create I/O output context\n");
            exit(1);
        }
        // Link the container's context to the previous I/O context
        adts_container_ctx->pb = adts_avio_ctx;
        if (!(adts_stream = avformat_new_stream(adts_container_ctx, NULL))) {
            fprintf(stderr, "Could not create new stream\n");
            exit(1);
        }

        adts_stream->id = adts_container_ctx->nb_streams-1;
        // Copy the encoder's parameters
        avcodec_parameters_from_context(adts_stream->codecpar, audio_encoder_ctx);
        // Allocate the stream private data and write the stream header
        if (avformat_write_header(adts_container_ctx, NULL) < 0) {
            fprintf(stderr, "avformat_write_header() error\n");
            exit(1);
        }
    }

    //need to link this method to the OnData from the AudioEncoder
    void OnDataFromEncoder(AVCodecContext *audio_encoder_ctx, AVFrame *frame, AVPacket *encoded_audio_packet, uint8_t *data, int size){

        curr_pts = frame->nb_samples * ( encoded_pkt_counter - 1 );
        encoded_audio_packet->pts = encoded_audio_packet->dts = curr_pts;

        int ret_val;

        if ((ret_val = av_write_frame(adts_container_ctx, encoded_audio_packet)) < 0) { //(e)
            fprintf(stderr, "Error calling av_write_frame() (error '%s')\n", av_err2str(ret_val));
            exit(1);
        }

        fprintf(stderr, "Encoded AAC packet %d, size=%d, pts_time=%s\n", encoded_pkt_counter, encoded_audio_packet->size, av_ts2timestr(encoded_audio_packet->pts, &audio_encoder_ctx->time_base));

        encoded_pkt_counter++;
    }


    void endStream() {

        av_write_trailer(adts_container_ctx);

    }

    SSE2_CLASS_NEW_OPERATOR

};


#endif
