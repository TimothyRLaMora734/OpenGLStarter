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

#define USE_BITSTREAM_FILTER 1

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

BEGIN_DECLARE_DELEGATE(OnMuxerDataDelegate, uint8_t *data, int size ) CALL_PATTERN (data, size) END_DECLARE_DELEGATE;



class BitstreamFilterADTStoASC{
    
    const AVBitStreamFilter *bsf;
    AVBSFContext *ctx;
    
public:
    
    AVPacket mpkt = { 0 };
    
    
    BitstreamFilterADTStoASC(){
        bsf = av_bsf_get_by_name("aac_adtstoasc");
        ctx = NULL;
    }
    virtual ~BitstreamFilterADTStoASC(){
        if (ctx != NULL)
            av_bsf_free(&ctx);
        ctx = NULL;
    }
    
    void initFromStream(AVStream*stream){

        int ret = av_bsf_alloc(bsf, &ctx);
        if (ret < 0){
            fprintf(stderr,"error to allocate AACToASCBitstreamFilter\n");
            exit(-1);
        }
        
        
        //input config
        ret = avcodec_parameters_copy(ctx->par_in, stream->codecpar);
        if (ret < 0){
            fprintf(stderr,"error to copy codec parameter\n");
            exit(-1);
        }
        ctx->time_base_in = stream->time_base;
        
        ret = av_bsf_init(ctx);
        if (ret < 0){
            fprintf(stderr,"error on av_bsf_init\n");
            exit(-1);
        }
        
        /*
        //output config
        ret = avcodec_parameters_copy(stream->codecpar, ctx->par_out);
        if (ret < 0){
            fprintf(stderr,"error on avcodec_parameters_copy\n");
            exit(-1);
        }
        stream->time_base = ctx->time_base_out;
        */
    }
    
    void initFromCodecContext(const AVCodecContext *codecCTX){
        
        int ret = av_bsf_alloc(bsf, &ctx);
        if (ret < 0){
            fprintf(stderr,"error to allocate AACToASCBitstreamFilter\n");
            exit(-1);
        }
        ctx->time_base_in = codecCTX->time_base;
        
        ret = avcodec_parameters_from_context(ctx->par_in, codecCTX);
        if (ret < 0){
            fprintf(stderr,"error to copy codec parameter\n");
            exit(-1);
        }
        
        ret = av_bsf_init(ctx);
        if (ret < 0){
            fprintf(stderr,"error on av_bsf_init\n");
            exit(-1);
        }
        
        
        //ret = avcodec_parameters_copy(out->codecpar, ctx->par_out);
        //if (ret < 0)
        //goto fail;
        //out->time_base = ctx->time_base_out;
    }
    
    void sendPKT(AVPacket *pkt){
        
        //memset(&mpkt,sizeof(AVPacket),0);
        //mpkt.data = pkt->data;
        //mpkt.size = pkt->size;
        
        int ret = av_bsf_send_packet(ctx, pkt);
        
        if (ret < 0){
            fprintf(stderr,"error on av_bsf_send_packet\n");
            exit(-1);
        }
    }
    
    void sendBuffer(void *buffer, size_t size){
        
        memset(&mpkt,sizeof(AVPacket),0);
        mpkt.data = (uint8_t*)buffer;
        mpkt.size = size;
        
        int ret = av_bsf_send_packet(ctx, &mpkt);
        
        if (ret < 0){
            fprintf(stderr,"error on av_bsf_send_packet\n");
            exit(-1);
        }
    }
    
    bool getPKT(){
        
        memset(&mpkt,sizeof(AVPacket),0);
        
        int ret = av_bsf_receive_packet(ctx, &mpkt);
        if (ret >= 0) {
            return true;
        }
        else if (ret == AVERROR(EAGAIN)) {
            return false;
        }else if (ret == AVERROR_EOF) {
            return false;
        }else if (ret < 0){
            fprintf(stderr,"error to use filter...\n");
            exit(-1);
        }
        return false;
    }
    
    void unrefPKT(){
        av_packet_unref(&mpkt);
    }
    
    void flush(){
        int ret = 0;
        while (ret >= 0) {
            ret = av_bsf_receive_packet(ctx, &mpkt);
            av_packet_unref(&mpkt);
        }
    }
    
};

static inline uint32_t readbit(int bitPos,  const uint8_t* data, int size){
    int dataPosition = bitPos / 8;
    int bitPosition = 7 - bitPos % 8;
    if (dataPosition >= size){
        fprintf(stderr,"error to access bit...\n");
        exit(-1);
    }
    return (data[dataPosition] >> bitPosition) & 0x01;
}

static inline uint32_t readbits(int bitPos, int length, const uint8_t* data, int size){
    if (length > 32){
        fprintf(stderr,"readbits length greated than 32\n");
        exit(-1);
    }
    uint32_t result = 0;
    for(int i=0;i<length;i++){
        result <<= 1;
        result = result | readbit(bitPos+i, data, size);
    }
    return result;
}

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

#if USE_BITSTREAM_FILTER
    BitstreamFilterADTStoASC filter;
    uint8_t FakeADTSHeader[7];
#endif

    static int writeData(void *userData, uint8_t *data, int size){
        FFmpegADTSMuxer *muxer = (FFmpegADTSMuxer*)userData;
        
        if (size<=7){
            printf("[FFmpegADTSMuxer] pkt size too small: %i\n",size);
            exit(-1);
        }
        //bits 54 - 55
        uint32_t num_frames = (data[6] & 0x03) + 1;//readbits(54,2,data,size)+1;
        if (num_frames != 1){
            printf("[FFmpegADTSMuxer] NumFrames > 1 not implemented: %u\n",num_frames);
            exit(-1);
        }
        
#if USE_BITSTREAM_FILTER
        
        BitstreamFilterADTStoASC &filter = muxer->filter;
        filter.sendBuffer(data, size);
        while (filter.getPKT()){
            
            uint8_t *FakeADTSHeader = muxer->FakeADTSHeader;
            
            if (filter.mpkt.side_data_elems > 0){
                //need to add ADTS Header here
                if (filter.mpkt.side_data_elems <= 0){
                    printf("Bitstream Filter without extradata\n");
                    exit(-1);
                }
                AVPacketSideData *sideData = &filter.mpkt.side_data[0];
                if (sideData->type != AV_PKT_DATA_NEW_EXTRADATA){
                    printf("Wrong extradata\n");
                    exit(-1);
                }
                if (sideData->size < 2){
                    printf("Missing extradata parameters\n");
                    exit(-1);
                }
                
                FakeADTSHeader[0] = 0xff;
                FakeADTSHeader[1] = 0xf1;
                //{0xff,0xf1,0,0,0,0,0};
                
                // bytes: [11111000]
                uint8_t objectType = (sideData->data[0] >> 3) - 1;//2 bits
                // bytes: [00000111] [10000000]
                uint8_t samplingIndex = ((sideData->data[0] & 0x07) << 1) | ( sideData->data[1] >> 7 );//4bits
                // bytes: [00000000] [01111000]
                uint8_t channelConfig = (sideData->data[1] >> 3) & 0xf; // 3 bits
                
                //16 - 17 [00000000][0000000][11000000]
                FakeADTSHeader[2] = objectType << 6;
                //bits 18 - 21 [00000000][0000000][00111100]
                FakeADTSHeader[2] |= samplingIndex << 2;
                //bits 23 - 25 [00000000][0000000][00000001][11000000]
                FakeADTSHeader[2] |= channelConfig >> 2;
                FakeADTSHeader[3] = channelConfig << 6;
                
            }
            
            uint32_t framelength = (filter.mpkt.size + 7 ) & 0x1fff;//13 bits
            if (framelength != (filter.mpkt.size + 7 )){
                printf("wrong framelength\n");
                exit(-1);
            }
            //bits 30 - 42 [00000000][0000000][00000000][00000011][11111111][11100000]
            FakeADTSHeader[3] &= ~0x03;
            FakeADTSHeader[3] |= framelength >> 11;
            FakeADTSHeader[4] = (framelength >> 3) & 0xff;
            FakeADTSHeader[5] = (framelength << 5) & 0xff;
            
            //printf(" [FFmpegADTSMuxer] framelength size calculated: %u (%i)\n", framelength-7, size - (framelength-7) );
            
            muxer->OnData(FakeADTSHeader, 7);
            muxer->OnData(filter.mpkt.data, filter.mpkt.size);
            
            filter.unrefPKT();
        }
#else
        muxer->OnData(data, size);
#endif
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
        
#if USE_BITSTREAM_FILTER
        memset(FakeADTSHeader,sizeof(FakeADTSHeader),0);
#endif
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
        
        //filter.initFromStream(adts_stream);
#if USE_BITSTREAM_FILTER
        filter.initFromCodecContext(audio_encoder_ctx);
#endif
    }

    //need to link this method to the OnData from the AudioEncoder
    void OnDataFromEncoder(AVCodecContext *audio_encoder_ctx, AVFrame *frame, AVPacket *encoded_audio_packet, uint8_t *data, int size){

        curr_pts = frame->nb_samples * ( encoded_pkt_counter - 1 );
        encoded_audio_packet->pts = encoded_audio_packet->dts = curr_pts;
        
        /*
        filter.sendPKT(encoded_audio_packet);
        while(filter.getPKT()){
            int ret_val;
            
            filter.mpkt.pts = filter.mpkt.dts = curr_pts;
            
            if ((ret_val = av_write_frame(adts_container_ctx, &filter.mpkt)) < 0) { //(e)
                fprintf(stderr, "Error calling av_write_frame() (error '%s')\n", av_err2str(ret_val));
                exit(1);
            }
            
            fprintf(stderr, "Encoded AAC packet %d, size=%d, pts_time=%s\n",
                    encoded_pkt_counter,
                    filter.mpkt.size,
                    av_ts2timestr(filter.mpkt.pts, &audio_encoder_ctx->time_base));
            
            filter.unrefPKT();
        }
        */
        
        int ret_val;
        if ((ret_val =
             av_write_frame
             (adts_container_ctx, encoded_audio_packet)) < 0) { //(e)
            fprintf(stderr, "Error calling av_write_frame() (error '%s')\n", av_err2str(ret_val));
            exit(1);
        }
        fprintf(stderr, "Encoded AAC packet %d, size=%d, pts_time=%s\n",
                encoded_pkt_counter,
                encoded_audio_packet->size,
                av_ts2timestr(encoded_audio_packet->pts, &audio_encoder_ctx->time_base));
        
        
        encoded_pkt_counter++;
    }


    void endStream() {

        av_write_trailer(adts_container_ctx);

    }

    SSE2_CLASS_NEW_OPERATOR

};


#endif
