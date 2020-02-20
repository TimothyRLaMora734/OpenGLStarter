#ifndef FFMPEG_INIT_H
#define FFMPEG_INIT_H

//https://stackoverflow.com/questions/2410459/encode-audio-to-aac-with-libavcodec
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/log.h>
#include <libavformat/avformat.h>
#include <libavutil/timestamp.h>
#include <libswresample/swresample.h>
}

#if defined(OS_TARGET_linux) || defined(OS_TARGET_win)

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

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

DefineMethodPointer(FFmpeg_OnDataMethodPtrT, void, const uint8_t *data, size_t s) VoidMethodCall(data,s)

DefineMethodPointer(FFmpeg_OnAVPacketMethodPtrT, void, AVPacket *data) VoidMethodCall(data)

DefineMethodPointer(FFmpeg_OnAVFrameAVPacketMethodPtrT, void, AVFrame *input_frame, AVPacket *encoded_packet) VoidMethodCall(input_frame,encoded_packet)

void globalInitFFMPEG();

#endif
