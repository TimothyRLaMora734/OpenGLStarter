#ifndef FFmpegAudioEncoder__H
#define FFmpegAudioEncoder__H

#include "FFmpegCommon.h"
#include "FFmpegAudioFrameConverter.h"

class FFmpegAudioEncoder {

    bool initialized;

    AVPacket *pkt;
    AVFrame *frame;
    AVCodecContext  *ctx;
    AVCodec *codec;

    //needs this to convert from float to float-planar to aac encoding
    FFmpegAudioFrameConverter float2floatPlanarFrameConverter;

    FFmpeg_OnAVFrameAVPacketMethodPtrT OnData;

public:
    AVCodecContext* getCtx();
    FFmpegAudioEncoder();
    virtual ~FFmpegAudioEncoder();

    /*
    Common AAC configurations:
     512kbps, 384kbps, 320kbps, 6, 160kbps
     64kbps(quality:5), 128kbps(quality:6,5), 192kbps(quality:8),
     256kbps(quality:10), 320kbps(quality:10)

    Parameters example:
        sampleRate = 44100
        channels = 2
        audioBitrate = 192000
     */
    void initAAC(int sampleRate , int channels , int audioBitrate ,
                const FFmpeg_OnAVFrameAVPacketMethodPtrT &OnData );
    int getTotalSamples();
    void writeBufferFloat(const float *data, int frameCount);
    void flushQueuedFrames();

};

#endif