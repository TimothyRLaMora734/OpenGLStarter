#ifndef RTAudioManager__H
#define RTAudioManager__H

#include <rtaudio/RtAudio.h>

BEGIN_DECLARE_DELEGATE(OnAudioDelegate, void *buffer, uint32_t frames ) CALL_PATTERN (buffer, frames) END_DECLARE_DELEGATE;

uint32_t gcd( uint32_t a, uint32_t b )
{
	const uint32_t zero = 0;
	while ( true )
	{
		if ( a == zero )
			return b;
		b %= a;

		if ( b == zero )
			return a;
		a %= b;
	}
}

#define RESAMPLER_MEMCPY 1

class FloatAudioResampler {

public:

    OnAudioDelegate *OnData;

private:

    uint32_t    toSamplerate, // to samplerate
                fromSamplerate; // from samplerate
    int channels;
    int frames;

    int gcd;

    float *buffer;
    int bufferCount;

    //after use GCD
    int fromGCD;
    int toGCD;
    int fromCount;
    float *fromBuffer;

    float fromDelta;
    float fromDelta_inv;

    inline void putToBufferLerpSample(const float *sampleA, const float *sampleB, float lerp){
        for (int j=0;j<channels;j++){
            buffer[bufferCount*channels + j] = sampleA[j] * (1.0f - lerp) + sampleB[j] * lerp;
        }
        bufferCount = (bufferCount+1)%frames;
        if (bufferCount == 0)
            (*OnData)(buffer, frames);
    }

    inline void putToBufferSample(const float *sample) {
    #if RESAMPLER_MEMCPY
        memcpy( &buffer[bufferCount*channels], sample, sizeof(float)*channels );
    #else
        for (int j=0;j<channels;j++){
            buffer[bufferCount*channels + j] = sample[j];
        }
    #endif
        bufferCount = (bufferCount+1)%frames;
        if (bufferCount == 0)
            (*OnData)(buffer, frames);
    }

public:

    FloatAudioResampler(){
        buffer = NULL;
        fromBuffer = NULL;
    }

    virtual ~FloatAudioResampler(){
        if (buffer != NULL)
            delete[] buffer;
        if (fromBuffer != NULL)
            delete[] fromBuffer;
        buffer = NULL;
        fromBuffer = NULL;
    }

    void init(int frames, int channels, uint32_t fromSamplerate, uint32_t toSamplerate) {
        if (buffer != NULL)
            return;

        bufferCount = 0;

        this->toSamplerate = toSamplerate;
        this->fromSamplerate = fromSamplerate;
        this->channels = channels;
        this->frames = frames;
        this->gcd = ::gcd(toSamplerate, fromSamplerate);

        fromGCD = fromSamplerate/this->gcd;
        toGCD = toSamplerate/this->gcd;

        //minimum of 2 samples to make the code work right...
        int multiplier = 1;

        while (fromGCD*multiplier < 2) {
            multiplier++;
        }
        while (toGCD * multiplier < 2) {
            multiplier++;
        }

        fromGCD *= multiplier;
        toGCD *= multiplier;

        fromCount = 0;

        // 32000 2 (from:real)
        // 48000 3 (to:request)
        printf("gcd: %u requested(to) %u real(from) %u",this->gcd, toGCD, fromGCD);

        fromBuffer = new float[(fromGCD+1)*channels];
        buffer = new float[frames*channels];
        fromDelta = 1.0f/(float)(fromGCD);
        fromDelta_inv = (float)(fromGCD);
    }



    inline void putSample(const float *sample) {
        //printf("-------------------------\n", fromCount);
        //printf("            putSample[%i]\n", fromCount);

        if (buffer == NULL)
            return;

    #if RESAMPLER_MEMCPY
        memcpy( &fromBuffer[fromCount*channels], sample, sizeof(float)*channels );
    #else
        for (int j=0;j<channels;j++){
            fromBuffer[fromCount*channels+j] = sample[j];
        }
    #endif

        if (fromCount == 0)
            putToBufferSample(sample);

        fromCount++;

        if (fromCount >= fromGCD+1){
            //printf("            fromCount >= fromGCD\n");
            //you can process the From window with fromGCD+1 elements
            //to the window with toGCD+1 elements
            // each toGCD generated need to call putToBufferSample

            for (int i=1;i<toGCD;i++){

                float toGCDLerp = (float)i/(float)(toGCD);

                int indexFromA = (int)( toGCDLerp * fromDelta_inv ); // (float)(fromGCD) );
                int indexFromB = indexFromA + 1;

                if (indexFromA >= fromGCD){
                    fprintf(stderr, "error on resampling algorithm...\n");
                    exit(1);
                }

                float fromMin = (float)indexFromA * fromDelta ;// /(float)(fromGCD);

                //float fromMax = (float)indexFromB/(float)(fromGCD);
                //float fromDelta = fromMax - fromMin;

                //not discover the amount of lerp toGCDLerp is relative to min and max
                float lrp = (toGCDLerp - fromMin) * fromDelta_inv;// /fromDelta;

                //printf("               for toGCDLerp=%f indexFromA=%i fromMin=%f lrp=%f\n", toGCDLerp, indexFromA, fromMin, lrp);

                putToBufferLerpSample( &fromBuffer[indexFromA*channels], &fromBuffer[indexFromB*channels]  ,lrp);

            }

            //put sample 0 again
            putToBufferSample(sample);

            //copy the sample to position 0
            fromCount = 1;
        #if RESAMPLER_MEMCPY
            memcpy( fromBuffer, sample, sizeof(float)*channels );
        #else
            for (int j=0;j<channels;j++){
                fromBuffer[j] = sample[j];
            }
        #endif

        }

    }
};

class RTAudioInput {

    RtAudio devices;
    uint32_t bufferFrames;

    uint32_t samplerateRequested;
    uint32_t realSamplerate;
    uint32_t realChannel;

    //in the case we need to resample the buffer from the hardware
    FloatAudioResampler floatAudioResampler;

    static int inputRead( void * /*outputBuffer*/, void *inputBuffer, unsigned int nBufferFrames,
                     double /*streamTime*/, RtAudioStreamStatus /*status*/, void *userData )
    {
        RTAudioInput *rtAudioInput = (RTAudioInput *) userData;
        if (rtAudioInput->endStream_called)
            return 2;
        //printf("readed frames: %i\n",nBufferFrames);

        if (rtAudioInput->samplerateRequested != rtAudioInput->realSamplerate) {
            //resample
            const float* fbuffer = (float*)inputBuffer;
            for (int i=0;i< nBufferFrames;i++) {
                rtAudioInput->floatAudioResampler.putSample(&fbuffer[i*(rtAudioInput->realChannel)]);
            }
        } else
            rtAudioInput->OnData(inputBuffer, nBufferFrames);

        return 0;
    }

    bool initialized;
    volatile bool endStream_called;

public:

    OnAudioDelegate OnData;

    RTAudioInput(RtAudio::Api api = RtAudio::UNSPECIFIED):
		devices(api)
	{
        initialized = false;
        endStream_called = false;

        samplerateRequested = 0;
        realSamplerate = 0;
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
                                 uint32_t samplerate=44100,
                                 int channels = 2,
                                 int offset = 0) {
        if (initialized)
            return;
        initialized = true;

        samplerateRequested = samplerate;

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

        realSamplerate = devices.getStreamSampleRate();
        realChannel = channels;
        printf("[RTAudioInput] need resample: %i requested: %u opened: %i \n", samplerateRequested != realSamplerate ,  samplerateRequested, realSamplerate);
        if (samplerateRequested != realSamplerate){
            floatAudioResampler.OnData = &OnData;
            floatAudioResampler.init(bufferFrames, channels, realSamplerate, samplerateRequested);
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

    static void printDevices(RtAudio::Api api = RtAudio::UNSPECIFIED) {

        RtAudio devices(api);
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
