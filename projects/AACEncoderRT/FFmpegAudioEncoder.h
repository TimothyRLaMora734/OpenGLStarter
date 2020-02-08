#ifndef FFMPEG_AUDIO_ENCODER_H
#define FFMPEG_AUDIO_ENCODER_H

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

BEGIN_DECLARE_DELEGATE(OnEncoderDataDelegate, AVCodecContext *audio_encoder_ctx, AVFrame *frame, AVPacket *encoded_audio_packet, uint8_t *data, int size ) CALL_PATTERN (audio_encoder_ctx, frame, encoded_audio_packet, data, size) END_DECLARE_DELEGATE;

void printSampleFmt(const AVSampleFormat &fmt) {

    switch(fmt){
        case AV_SAMPLE_FMT_NONE:
            fprintf(stdout, " AV_SAMPLE_FMT_NONE\n");
            break;
        case AV_SAMPLE_FMT_U8:
            fprintf(stdout, " AV_SAMPLE_FMT_U8\n");
            break;
        case AV_SAMPLE_FMT_S16:
            fprintf(stdout, " AV_SAMPLE_FMT_S16\n");
            break;
        case AV_SAMPLE_FMT_S32:
            fprintf(stdout, " AV_SAMPLE_FMT_S32\n");
            break;
        case AV_SAMPLE_FMT_FLT:
            fprintf(stdout, " AV_SAMPLE_FMT_FLT\n");
            break;
        case AV_SAMPLE_FMT_DBL:
            fprintf(stdout, " AV_SAMPLE_FMT_DBL\n");
            break;
        case AV_SAMPLE_FMT_U8P:
            fprintf(stdout, " AV_SAMPLE_FMT_U8P\n");
            break;
        case AV_SAMPLE_FMT_S16P:
            fprintf(stdout, " AV_SAMPLE_FMT_S16P\n");
            break;
        case AV_SAMPLE_FMT_S32P:
            fprintf(stdout, " AV_SAMPLE_FMT_S32P\n");
            break;
        case AV_SAMPLE_FMT_FLTP:
            fprintf(stdout, " AV_SAMPLE_FMT_FLTP\n");
            break;
        case AV_SAMPLE_FMT_DBLP:
            fprintf(stdout, " AV_SAMPLE_FMT_DBLP\n");
            break;
        case AV_SAMPLE_FMT_S64:
            fprintf(stdout, " AV_SAMPLE_FMT_S64\n");
            break;
        case AV_SAMPLE_FMT_S64P:
            fprintf(stdout, " AV_SAMPLE_FMT_S64P\n");
            break;
        default:
            break;
    }
}

void printSampleFmtArray(const AVSampleFormat *fmtList) {
    fprintf(stdout, "Supported Sample FMTs:\n");
    int count = 0;
    while (fmtList[count] != AV_SAMPLE_FMT_NONE){
        printSampleFmt(fmtList[count]);
        count++;
    }
    fprintf(stdout, "\n");
}

//needs this class to convert from float to float-planar input for the AAC encoder
class FFmpegSampleConverter {

    bool initialized;
    SwrContext *audio_convert_context;

public:

    AVFrame *converted_audio_frame;

    FFmpegSampleConverter(){
        initialized = false;
        audio_convert_context = NULL;
        converted_audio_frame = NULL;
    }

    virtual ~FFmpegSampleConverter(){
        if (converted_audio_frame != NULL)
            av_frame_free(&converted_audio_frame);
        if (audio_convert_context != NULL)
            swr_free(&audio_convert_context);
        converted_audio_frame = NULL;
        audio_convert_context = NULL;
    }

    void init(AVCodecContext *audio_encoder_ctx,
              AVSampleFormat input_sample_fmt = AV_SAMPLE_FMT_FLT,
              int input_sample_rate = 48000
              //AVSampleFormat targetFmt = AV_SAMPLE_FMT_FLTP
              ) {
        if (initialized)
            return;
        initialized = true;
        globalInitFFMPEG();

        audio_convert_context =
        swr_alloc_set_opts( NULL,
                            //
                            // target params
                            //
                            av_get_default_channel_layout(audio_encoder_ctx->channels),
                            audio_encoder_ctx->sample_fmt,
                            audio_encoder_ctx->sample_rate,
                            //
                            // src params
                            //
                            av_get_default_channel_layout(audio_encoder_ctx->channels),
                            input_sample_fmt,
                            input_sample_rate,
                            0,NULL);

        if (!audio_convert_context) {
            fprintf(stderr, "Could not allocate resample context\n");
            exit(1);
        }

        int ret_val = 0;
        //if ((ret_val = swr_init(audio_convert_context)) < 0) {
            //fprintf(stderr, "Could not open resample context\n");
            //exit(1);
        //}

        if (!(converted_audio_frame = av_frame_alloc())) {
            fprintf(stderr, "Could not allocate resampled frame\n");
            exit(1);
        }

        converted_audio_frame->nb_samples     = audio_encoder_ctx->frame_size;
        converted_audio_frame->format         = audio_encoder_ctx->sample_fmt;
        converted_audio_frame->channels       = audio_encoder_ctx->channels;
        converted_audio_frame->channel_layout = audio_encoder_ctx->channel_layout;
        converted_audio_frame->sample_rate    = audio_encoder_ctx->sample_rate;

        if ((ret_val = av_frame_get_buffer(converted_audio_frame, 0)) < 0) {
            fprintf(stderr, "Could not allocate a buffer for resampled frame samples (error '%s')\n", av_err2str(ret_val));
            exit(1);
        }

    }

    AVFrame * convert(AVFrame *frame) {
        if (!initialized)
            return frame;

        swr_convert_frame(audio_convert_context, converted_audio_frame, (const AVFrame*)frame);

        return converted_audio_frame;
    }

};

class FFmpegAudioEncoder {

    bool initialized;

    AVPacket *pkt;
    AVFrame *frame;
    AVCodecContext  *ctx;
    AVCodec *codec;

    //needs this to convert from float to float-planar to aac encoding
    FFmpegSampleConverter float2floatPlanarConverter;

public:

    OnEncoderDataDelegate OnData;

    AVCodecContext* getCtx(){
        return ctx;
    }

    FFmpegAudioEncoder(){
        initialized = false;
        pkt = NULL;
        frame = NULL;
        ctx = NULL;
        codec = NULL;
    }
    virtual ~FFmpegAudioEncoder() {

        if (frame != NULL)
            av_frame_free(&frame);
        if (pkt != NULL)
            av_packet_free(&pkt);
        if (ctx != NULL)
            avcodec_free_context(&ctx);

        pkt = NULL;
        frame = NULL;
        ctx = NULL;
        codec = NULL;

    }

    /*
     AAC CBR@512kbps
     AAC CBR@384kbps
     AAC CBR@320kbps
     AAC VBR@6
     AAC CBR@160kbps

     AAC 64 - 5
     AAC 128 - 6,5
     AAC 192 -8
     AAC 256 - 10
     AAC 320 - 10
     */
    void initAAC(int sampleRate = 48000 , int channels = 2, int audioBitrate = 256000 ) {
        if (initialized)
            return;
        initialized = true;
        globalInitFFMPEG();

        av_log_set_level(AV_LOG_WARNING);

        codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
        //codec = avcodec_find_encoder_by_name("AAC");
        if (codec == NULL) {
            fprintf(stderr, "Codec not found\n");
            exit(1);
        }

        printSampleFmtArray(codec->sample_fmts);

        ctx = avcodec_alloc_context3(codec);

        ctx->bit_rate = audioBitrate;
        //ctx->sample_fmt = AV_SAMPLE_FMT_S16;
        ctx->sample_fmt = AV_SAMPLE_FMT_FLTP;//codec->sample_fmts[0];
        ctx->sample_rate = sampleRate;
        ctx->channels = channels;
        ctx->channel_layout = av_get_default_channel_layout(channels);
        //ctx->channel_layout = AV_CH_LAYOUT_STEREO;
        //ctx->channels = av_get_channel_layout_nb_channels(ctx->channel_layout);
        ctx->profile = FF_PROFILE_AAC_LOW;//FF_PROFILE_AAC_MAIN;
		ctx->time_base.num = 1;
		ctx->time_base.den = sampleRate;
        ctx->codec_type = AVMEDIA_TYPE_AUDIO;
        // AAC default frame size
        ctx->frame_size = 1024;

        if (avcodec_open2(ctx, codec, NULL) < 0){
            fprintf(stderr, "Could not open codec\n");
            exit(1);
        }

        //packet for holding encoded output
        pkt = av_packet_alloc();
        if (!pkt) {
            fprintf(stderr, "could not allocate the packet\n");
            exit(1);
        }

        //frame containing input raw audio
        frame = av_frame_alloc();
        if (!frame) {
            fprintf(stderr, "Could not allocate audio frame\n");
            exit(1);
        }


        frame->nb_samples     = ctx->frame_size;
        frame->format         = AV_SAMPLE_FMT_FLT;//audio_encoder_ctx->sample_fmt;
        frame->channels       = ctx->channels;
        frame->channel_layout = ctx->channel_layout;
        frame->sample_rate    = ctx->sample_rate;

        //allocate the data buffers
        if (av_frame_get_buffer(frame, 0) < 0) {
            fprintf(stderr, "Could not allocate audio data buffers\n");
            exit(1);
        }

        float2floatPlanarConverter.init(ctx,AV_SAMPLE_FMT_FLT,sampleRate);

        //
        // Each sample will hold the set of channels of the configuration
        // example: channels = 2
        //          nb_samples = 512
        //
        //       data[index*2 + 0] // index at channel 0
        //       data[index*2 + 1] // same index at channel 1
        //
        // data length needs to be channels * nb_samples * sizeof( int16_t )
        //

        fprintf(stdout, "Using fmt:");
        printSampleFmt(ctx->sample_fmt);
        fprintf(stdout, "nb_samples: %i\n", frame->nb_samples);

    }

    int getTotalSamples() {
        return frame->nb_samples * ctx->channels;
    }

    void writeBufferFloat(const float *data, int frameCount) {
        if (!initialized)
            return;

        //int sampleCount = data.size();
        int framesWritten = 0;

        if (frameCount == 0)
            return;

        if (frameCount % frame->nb_samples != 0) {
            fprintf(stderr, "frameCount need to be a multiple of frame->nb_samples\n");
            exit(1);
        }

        while (framesWritten < frameCount) {

            if (av_frame_make_writable(frame) < 0) {
                fprintf(stderr, "Cannot make frame writable\n");
                exit(1);
            }

            float *writableBuffer = (float*) frame->data[0];
            for(int i=0;i<frame->nb_samples;i++){
                for(int j=0;j<ctx->channels;j++){
                    writableBuffer[i*ctx->channels + j] = data[framesWritten*ctx->channels + j];
                }
                framesWritten++;
            }

            AVFrame* converted_frame = float2floatPlanarConverter.convert(frame);

            //encode frame...
            //send the frame for encoding
            if (avcodec_send_frame(ctx, converted_frame) < 0) {
                fprintf(stderr, "Error sending the frame to the encoder\n");
                exit(1);
            }

            // read all the available output packets (in general there may be any
            // number of them
            int ret = 1;
            while (ret >= 0) {
                ret = avcodec_receive_packet(ctx, pkt);
                if ( ret == AVERROR(EAGAIN) ) {
                    //no frame available yet.... need to feed more frames to get any output
                    break;
                }else if (ret == AVERROR_EOF){
                    fprintf(stderr, "receive packet EOF\n");
                    exit(1);
                    //return;
                }
                else if (ret < 0) {
                    fprintf(stderr, "Error encoding audio frame\n");
                    exit(1);
                }

                fprintf(stdout, "data output: %i\n", pkt->size);
                // pkt->data : bin data
                // pkt->size : bin size bytes
                OnData( ctx, frame, pkt , pkt->data, pkt->size);
                av_packet_unref(pkt);
            }
        }
    }

    void flushQueuedFrames() {

        if (avcodec_send_frame(ctx, NULL) < 0) {
            fprintf(stderr, "Error sending the frame to the encoder\n");
            exit(1);
        }

        // read all the available output packets (in general there may be any
        // number of them
        int ret = 1;
        while (ret >= 0) {
            ret = avcodec_receive_packet(ctx, pkt);
            if ( ret == AVERROR(EAGAIN) ) {
                //no frame available yet.... need to feed more frames to get any output
                break;
            }else if (ret == AVERROR_EOF){
                //fprintf(stderr, "receive packet EOF\n");
                //exit(1);
                //return;
                //EOF -- end of stream package received...
                break;
            }
            else if (ret < 0) {
                fprintf(stderr, "Error encoding audio frame\n");
                exit(1);
            }

            fprintf(stdout, "data output: %i\n", pkt->size);
            // pkt->data : bin data
            // pkt->size : bin size bytes
            OnData( ctx, frame, pkt , pkt->data, pkt->size);
            av_packet_unref(pkt);
        }

    }

    SSE2_CLASS_NEW_OPERATOR

};


#endif
