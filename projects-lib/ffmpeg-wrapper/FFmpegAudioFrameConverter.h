#ifndef FFmpegAudioFrameConverter__H
#define FFmpegAudioFrameConverter__H

#include "FFmpegCommon.h"

//needs this class to convert from 
//  float to float-planar
//input for the AAC encoder
class FFmpegAudioFrameConverter {

    bool initialized;
    SwrContext *audio_convert_context;

public:

    AVFrame *converted_audio_frame;

    FFmpegAudioFrameConverter();

    virtual ~FFmpegAudioFrameConverter();

    //AVSampleFormat targetFmt = AV_SAMPLE_FMT_FLTP
    void init(AVCodecContext *audio_encoder_ctx,
              AVSampleFormat input_sample_fmt = AV_SAMPLE_FMT_FLT,
              int input_sample_rate = 44100);

    AVFrame * convert(AVFrame *frame);
};


#endif
