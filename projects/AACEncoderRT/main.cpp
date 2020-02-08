
#include "FFmpegADTSMuxer.h"
#include "FFmpegAudioEncoder.h"
#include "RTAudioManager.h"

#include <stdio.h>

//play raw :
// ffplay -f f32le -ar 48000 -ac 2 output.raw
// ffplay -f aac output.aac
//
// convert the raw PCM to AAC
// ffmpeg -f f32le -ar 48000 -ac 2 -i output.raw -f adts -ar 48000 -ac 2 -ab 198000 output.aac
//

FILE* outputRAW;
FILE* outputAAC;

FFmpegADTSMuxer adtsMuxer;
FFmpegAudioEncoder audioEncoder;

void OnDataFromMuxer(uint8_t *data, int size){
    fwrite(data, sizeof(uint8_t), (size_t)size, outputAAC);
}

void OnDataFromAudioCard(void *fltBuffer, uint32_t frames) {
    fwrite(fltBuffer, sizeof(float), frames*2, outputRAW);
    audioEncoder.writeBufferFloat((float*)fltBuffer, frames);
}

int main(int argc, char* argv[]){

    PlatformPath::setWorkingPath(PlatformPath::getExecutablePath(argv[0]));

    outputAAC = fopen("output.aac","wb");
    if (!outputAAC)
        exit(-1);
    outputRAW = fopen("output.raw","wb");
    if (!outputRAW)
        exit(-1);

    //tunnel the data from encoder to the muxer
    audioEncoder.OnData.add(&adtsMuxer,&FFmpegADTSMuxer::OnDataFromEncoder);
    audioEncoder.initAAC();

    //initialize the muxer with the encoder parameter
    adtsMuxer.OnData.add(&OnDataFromMuxer);
    adtsMuxer.init(audioEncoder.getCtx());

    RTAudioInput inputAudio;
    inputAudio.OnData.add(&OnDataFromAudioCard);
    #if defined(OS_TARGET_mac)
    inputAudio.initInputFromDeviceName("Apple Inc.: Built-in Input");
    #elif defined(OS_TARGET_linux)
    inputAudio.initInputFromDeviceName("default");
    #elif defined(OS_TARGET_win)
	inputAudio.initInputFromDeviceName("Microfone (HD Pro Webcam C920)");
    #endif

    fgetc(stdin);

    inputAudio.endStream();

    audioEncoder.flushQueuedFrames();
    adtsMuxer.endStream();

    fclose(outputAAC);
    fclose(outputRAW);

    return 0;
}
