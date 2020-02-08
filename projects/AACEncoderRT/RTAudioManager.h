#ifndef RTAudioManager__H
#define RTAudioManager__H

#include <rtaudio/RtAudio.h>

BEGIN_DECLARE_DELEGATE(OnAudioDelegate, void *buffer, uint32_t frames ) CALL_PATTERN (buffer, frames) END_DECLARE_DELEGATE;

class RTAudioInput {
    
    RtAudio devices;
    uint32_t bufferFrames;
    
    static int inputRead( void * /*outputBuffer*/, void *inputBuffer, unsigned int nBufferFrames,
                     double /*streamTime*/, RtAudioStreamStatus /*status*/, void *userData )
    {
        RTAudioInput *rtAudioInput = (RTAudioInput *) userData;
        if (rtAudioInput->endStream_called)
            return 2;
        printf("readed frames: %i\n",nBufferFrames);
        rtAudioInput->OnData(inputBuffer, nBufferFrames);
        
        return 0;
    }
    
    bool initialized;
    volatile bool endStream_called;
    
public:
    
    OnAudioDelegate OnData;
    
    RTAudioInput() {
        initialized = false;
        endStream_called = false;
    }
    
    bool isStreaming() {
        if (!initialized)
            return false;
        
        return devices.isStreamRunning();
    }
    
    void endStream() {
        endStream_called = true;
        
        if (!initialized)
            return;
        
        while ( devices.isStreamRunning() ) {
            PlatformSleep::sleepMillis(100);
        }
        if ( devices.isStreamOpen() ) devices.closeStream();
    }
    
    void initInputFromDeviceName(const std::string &devicename,
                                 uint32_t _bufferFrames = 1024,
                                 RtAudioFormat format = RTAUDIO_FLOAT32 ,
                                 uint32_t samplerate=48000,
                                 int channels = 2,
                                 int offset = 0) {
        if (initialized)
            return;
        initialized = true;
        
        int count = devices.getDeviceCount();
        bool found = false;
        RtAudio::DeviceInfo info;
        int deviceID = 0;
        for(int i=0;i<count;i++){
            info = devices.getDeviceInfo(i);
            if (info.name.compare(devicename) == 0) {
                found = true;
                deviceID = i;
                break;
            }
        }
        
        if (!found) {
            fprintf(stderr, "cannot find the device: %s\n", devicename.c_str());
            exit(1);
        }
        
        
        // Let RtAudio print messages to stderr.
        devices.showWarnings( true );
        
        RtAudio::StreamParameters iParams;
        iParams.deviceId = deviceID;
        iParams.nChannels = channels;
        iParams.firstChannel = offset;
        
        
        bufferFrames = _bufferFrames;
        
        try {
            devices.openStream( NULL, &iParams, format, samplerate, &bufferFrames,
                               &RTAudioInput::inputRead,
                               (void *)this );
        }
        catch ( RtAudioError& e ) {
            std::cout << '\n' << e.getMessage() << '\n' << std::endl;
            endStream();
            exit(1);
        }
        
        try {
            devices.startStream();
        }
        catch ( RtAudioError& e ) {
            std::cout << '\n' << e.getMessage() << '\n' << std::endl;
            endStream();
            exit(1);
        }
        
    }
};

class RTAudioHelper {
public:

    static void printDevices() {
        
        RtAudio devices;
        int count = devices.getDeviceCount();
        RtAudio::DeviceInfo info;
        
        printf("number of devices: %i\n", devices.getDeviceCount());
        
        for(int i=0;i<count;i++){
            info = devices.getDeviceInfo(i);
            printf("\nDevice Name = %s\n",info.name.c_str());
            if (!info.probed)
                continue;
            
            std::cout << "Output Channels = " << info.outputChannels << '\n';
            std::cout << "Input Channels = " << info.inputChannels << '\n';
            std::cout << "Duplex Channels = " << info.duplexChannels << '\n';
            if ( info.isDefaultOutput ) std::cout << "This is the default output device.\n";
            else std::cout << "This is NOT the default output device.\n";
            if ( info.isDefaultInput ) std::cout << "This is the default input device.\n";
            else std::cout << "This is NOT the default input device.\n";
            if ( info.nativeFormats == 0 )
                std::cout << "No natively supported data formats(?)!";
            else {
                std::cout << "Natively supported data formats:\n";
                if ( info.nativeFormats & RTAUDIO_SINT8 )
                    std::cout << "  8-bit int\n";
                if ( info.nativeFormats & RTAUDIO_SINT16 )
                    std::cout << "  16-bit int\n";
                if ( info.nativeFormats & RTAUDIO_SINT24 )
                    std::cout << "  24-bit int\n";
                if ( info.nativeFormats & RTAUDIO_SINT32 )
                    std::cout << "  32-bit int\n";
                if ( info.nativeFormats & RTAUDIO_FLOAT32 )
                    std::cout << "  32-bit float\n";
                if ( info.nativeFormats & RTAUDIO_FLOAT64 )
                    std::cout << "  64-bit float\n";
            }
            if ( info.sampleRates.size() < 1 )
                std::cout << "No supported sample rates found!";
            else {
                std::cout << "Supported sample rates = ";
                for (unsigned int j=0; j<info.sampleRates.size(); j++)
                    std::cout << info.sampleRates[j] << " ";
            }
            std::cout << std::endl;
        }
    }
    
    
};


#endif
