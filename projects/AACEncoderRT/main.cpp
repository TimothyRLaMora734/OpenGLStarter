
#include "FFmpegADTSMuxer.h"
#include "FFmpegAudioEncoder.h"
#include "RTAudioManager.h"

#include <stdio.h>
#include <libflv/FLVMuxer.h>

//play raw :
// ffplay -f f32le -ar 48000 -ac 2 output.raw
// ffplay -f aac output.aac
//
// convert the raw PCM to AAC
// ffmpeg -f f32le -ar 48000 -ac 2 -i output.raw -f adts -ar 48000 -ac 2 -ab 198000 output.aac
//
// fix tha aac generated to put in flv container using a bitstream filter:
// ffmpeg -f aac -i output2.aac -bsf:a aac_adtstoasc -f flv -acodec copy output.flv
//

FILE* outputRAW;
FILE* outputAAC;
FILE* outputFLV;

void flvWrite(const void* data, size_t size){
    fwrite(data, sizeof(uint8_t), (size_t)size, outputFLV);
}

FFmpegADTSMuxer adtsMuxer;
FFmpegAudioEncoder audioEncoder;
FLVMuxer flvMuxer(false, //_writeVideo,
                  true, // _writeAudio,
                  30, //_videoFPS
                  44100,//_audioSamplerate
                  OnDataMethodPtrT(&flvWrite));



void OnDataFromMuxer(uint8_t *data, int size){
    //printf("  OnDataFromMuxer size: %u\n", size);
    fwrite(data, sizeof(uint8_t), (size_t)size, outputAAC);
    flvMuxer.writeADTS(data, size);
}

void OnDataFromAudioCard(void *fltBuffer, uint32_t frames) {
    fwrite(fltBuffer, sizeof(float), frames*2, outputRAW);
    audioEncoder.writeBufferFloat((float*)fltBuffer, frames);
}

int main(int argc, char* argv[]){

    //RTAudioHelper::printDevices(RtAudio::WINDOWS_DS);
	//RTAudioHelper::printDevices();
	//return 0;

    PlatformPath::setWorkingPath(PlatformPath::getExecutablePath(argv[0]));
    
    outputAAC = fopen("output.aac","wb");
    if (!outputAAC)
        exit(-1);
    outputRAW = fopen("output.raw","wb");
    if (!outputRAW)
        exit(-1);
    outputFLV = fopen("output.flv","wb");
    if (!outputFLV)
        exit(-1);
    
    flvMuxer.writeHeader();

    //tunnel the data from encoder to the muxer
    audioEncoder.OnData.add(&adtsMuxer,&FFmpegADTSMuxer::OnDataFromEncoder);
    audioEncoder.initAAC();

    //initialize the muxer with the encoder parameter
    adtsMuxer.OnData.add(&OnDataFromMuxer);
    adtsMuxer.init(audioEncoder.getCtx());

#if defined(OS_TARGET_win)
	RTAudioInput inputAudio;// (RtAudio::WINDOWS_DS);
#else
	RTAudioInput inputAudio;
#endif

    inputAudio.OnData.add(&OnDataFromAudioCard);
    #if defined(OS_TARGET_mac)
    inputAudio.initInputFromDeviceName("Apple Inc.: Built-in Input");
    #elif defined(OS_TARGET_linux)
    inputAudio.initInputFromDeviceName("hw:HD Pro Webcam C920,0");
    #elif defined(OS_TARGET_win)
	inputAudio.initInputFromDeviceName("Microfone (HD Pro Webcam C920)");
    #endif

    fgetc(stdin);

    inputAudio.endStream();

    audioEncoder.flushQueuedFrames();
    adtsMuxer.endStream();
    flvMuxer.endOfStream();

    fclose(outputAAC);
    fclose(outputRAW);
    fclose(outputFLV);

    return 0;
}
