#include "FFmpegAudioEncoder.h"

static inline void printSampleFmt(const AVSampleFormat &fmt) {

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

static inline void printSampleFmtArray(const AVSampleFormat *fmtList) {
    fprintf(stdout, "Supported Sample FMTs:\n");
    int count = 0;
    while (fmtList[count] != AV_SAMPLE_FMT_NONE){
        printSampleFmt(fmtList[count]);
        count++;
    }
    fprintf(stdout, "\n");
}

AVCodecContext* FFmpegAudioEncoder::getCtx(){
    return ctx;
}

FFmpegAudioEncoder::FFmpegAudioEncoder(){
    initialized = false;
    pkt = NULL;
    frame = NULL;
    ctx = NULL;
    codec = NULL;
}
FFmpegAudioEncoder::~FFmpegAudioEncoder() {

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

void FFmpegAudioEncoder::initAAC(int sampleRate , int channels , int audioBitrate ,
            const FFmpeg_OnAVFrameAVPacketMethodPtrT &OnData ) {
    if (initialized)
        return;
    initialized = true;
    globalInitFFMPEG();

    this->OnData = OnData;

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

    float2floatPlanarFrameConverter.init(ctx,AV_SAMPLE_FMT_FLT,sampleRate);

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

int FFmpegAudioEncoder::getTotalSamples() {
    return frame->nb_samples * ctx->channels;
}

void FFmpegAudioEncoder::writeBufferFloat(const float *data, int frameCount) {
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

        AVFrame* converted_frame = float2floatPlanarFrameConverter.convert(frame);

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

            //fprintf(stdout, "data output: %i\n", pkt->size);
            // pkt->data : bin data
            // pkt->size : bin size bytes
            OnData( converted_frame, pkt );
            av_packet_unref(pkt);
        }
    }
}

void FFmpegAudioEncoder::flushQueuedFrames() {

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

        //fprintf(stdout, "data output: %i\n", pkt->size);
        // pkt->data : bin data
        // pkt->size : bin size bytes

        if (float2floatPlanarFrameConverter.converted_audio_frame != NULL)
            OnData( float2floatPlanarFrameConverter.converted_audio_frame, pkt );
        else
            OnData( frame, pkt );

        av_packet_unref(pkt);
    }

}
