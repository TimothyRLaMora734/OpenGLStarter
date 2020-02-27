
#include <ffmpeg-wrapper/FFmpegADTSMuxer.h>
#include <ffmpeg-wrapper/FFmpegAudioEncoder.h>
#include <rtaudio-wrapper/RTAudioInput.h>
#include <libflv/FLVMuxer.h>

#include <stdio.h>


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

void OnDataFromFLV(const void* data, size_t size){
    fwrite(data, sizeof(uint8_t), (size_t)size, outputFLV);
}

FFmpegADTSMuxer adtsMuxer;
FFmpegAudioEncoder audioEncoder;
FLVMuxer flvMuxer(false, //_writeVideo,
                  true, // _writeAudio,
                  30, //_videoFPS
                  44100,//_audioSamplerate
                  FLV_OnDataMethodPtrT(&OnDataFromFLV));


void OnDataFromMuxer(const uint8_t *data, size_t size){
    //printf("  OnDataFromMuxer size: %u\n", size);
    fwrite(data, sizeof(uint8_t), (size_t)size, outputAAC);
    flvMuxer.writeADTS(data, size);
}

void OnDataFromAudioCard(const void *fltBuffer, uint32_t frames) {
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
    audioEncoder.initAAC(44100,//sample rate
                         2 ,//channels
                         192000 ,//bitrate
                         FFmpeg_OnAVFrameAVPacketMethodPtrT(&adtsMuxer,&FFmpegADTSMuxer::writeData));

    //initialize the muxer with the encoder parameter
    adtsMuxer.init(audioEncoder.getCtx(),
                   &OnDataFromMuxer);

#if defined(OS_TARGET_win)
	RTAudioInput<float> inputAudio(RtAudio::WINDOWS_DS);
#else
	RTAudioInput<float> inputAudio;
#endif

#if defined(OS_TARGET_mac)
    inputAudio.initInputFromDeviceName("Apple Inc.: Built-in Input"
		1024,//_bufferFrames
		RTAUDIO_FLOAT32,//format
		44100,//samplerate
		2,//channels
		0,//offset
		&OnDataFromAudioCard //OnData
	);
#elif defined(OS_TARGET_linux)
    inputAudio.initInputFromDeviceName("hw:HD Pro Webcam C920,0"
		1024,//_bufferFrames
		RTAUDIO_FLOAT32,//format
		44100,//samplerate
		2,//channels
		0,//offset
		&OnDataFromAudioCard //OnData
	);
#elif defined(OS_TARGET_win)
	inputAudio.initInputFromDeviceName("Microfone (HD Pro Webcam C920)",
		1024,//_bufferFrames
		RTAUDIO_FLOAT32,//format
		44100,//samplerate
		2,//channels
		0,//offset
		&OnDataFromAudioCard //OnData
	);
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
