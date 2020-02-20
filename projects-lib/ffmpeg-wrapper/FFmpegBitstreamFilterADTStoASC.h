#ifndef FFmpegBitstreamFilterADTStoASC__H
#define FFmpegBitstreamFilterADTStoASC__H

#include "FFmpegCommon.h"

class FFmpegBitstreamFilterADTStoASC{
    
    const AVBitStreamFilter *bsf;
    AVBSFContext *ctx;
    AVPacket *resultpkt;
    
    FFmpeg_OnAVPacketMethodPtrT OnData;
    bool initialized;

    void retrievePKT();

public:
    
    FFmpegBitstreamFilterADTStoASC();
    virtual ~FFmpegBitstreamFilterADTStoASC();
    
    void initFromStream(AVStream*stream, const FFmpeg_OnAVPacketMethodPtrT &OnData);
    void initFromCodecContext(const AVCodecContext *codecCTX,  const FFmpeg_OnAVPacketMethodPtrT &OnData);
    
    void sendAVPacket(AVPacket *pkt);
    void sendBuffer(void *buffer, size_t size);
};

#endif