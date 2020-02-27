#ifndef RTAudioInput__H
#define RTAudioInput__H

#include <rtaudio/RtAudio.h>
#include "AudioResampler.h"

template<typename C>
class RTAudioInput {

public:

	RTAUDIO_OnAudioDataMethodPtrT OnData;

private:
	RtAudio audioBUS;
	uint32_t bufferFrames;

	uint32_t samplerateRequested;
	uint32_t realSamplerate;
	uint32_t realChannel;

	//in the case we need to resample the buffer from the hardware
	AudioResampler<C> audioResampler;

	static int inputRead(void * /*outputBuffer*/, void *inputBuffer, unsigned int nBufferFrames,
		double /*streamTime*/, RtAudioStreamStatus /*status*/, void *userData)
	{
		RTAudioInput *rtAudioInput = (RTAudioInput *)userData;
		if (rtAudioInput->endStream_called)
			return 2;
		//printf("readed frames: %i\n",nBufferFrames);

		if (rtAudioInput->samplerateRequested != rtAudioInput->realSamplerate) {
			//resample
			const C* fbuffer = (C*)inputBuffer;
			for (int i = 0; i < nBufferFrames; i++) {
				rtAudioInput->audioResampler.putSample(&fbuffer[i*(rtAudioInput->realChannel)]);
			}
		}
		else
			rtAudioInput->OnData(inputBuffer, nBufferFrames);

		return 0;
	}

	bool initialized;
	volatile bool endStream_called;

public:

	RTAudioInput(RtAudio::Api api = RtAudio::UNSPECIFIED) :
		audioBUS(api)
	{
		initialized = false;
		endStream_called = false;

		samplerateRequested = 0;
		realSamplerate = 0;
	}

	bool isStreaming() {
		if (!initialized)
			return false;

		return audioBUS.isStreamRunning();
	}

	void endStream() {
		endStream_called = true;

		if (!initialized)
			return;

		while (audioBUS.isStreamRunning()) {
			PlatformSleep::sleepMillis(100);
		}
		if (audioBUS.isStreamOpen()) audioBUS.closeStream();
	}

	void initInputFromDeviceName(const std::string &devicename,
		uint32_t _bufferFrames = 1024,
		RtAudioFormat format = RTAUDIO_FLOAT32,
		uint32_t samplerate = 44100,
		int channels = 2,
		int offset = 0,
		const RTAUDIO_OnAudioDataMethodPtrT &OnData = NULL) {
		if (initialized)
			return;
		initialized = true;

		this->OnData = OnData;

		samplerateRequested = samplerate;

		int count = audioBUS.getDeviceCount();
		bool found = false;
		RtAudio::DeviceInfo info;
		int deviceID = 0;
		for (int i = 0; i < count; i++) {
			info = audioBUS.getDeviceInfo(i);
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
		audioBUS.showWarnings(true);

		RtAudio::StreamParameters iParams;
		iParams.deviceId = deviceID;
		iParams.nChannels = channels;
		iParams.firstChannel = offset;


		bufferFrames = _bufferFrames;

		try {
			audioBUS.openStream(NULL, &iParams, format, samplerate, &bufferFrames,
				&RTAudioInput::inputRead,
				(void *)this);
		}
		catch (RtAudioError& e) {
			std::cout << '\n' << e.getMessage() << '\n' << std::endl;
			endStream();
			exit(1);
		}

		realSamplerate = audioBUS.getStreamSampleRate();
		realChannel = channels;
		printf("[RTAudioInput] need resample: %i requested: %u opened: %i \n", samplerateRequested != realSamplerate, samplerateRequested, realSamplerate);
		if (samplerateRequested != realSamplerate) {
			audioResampler.init(
				format != RTAUDIO_FLOAT32 && format != RTAUDIO_FLOAT64,
				bufferFrames, channels, realSamplerate, samplerateRequested,
				OnData);
		}

		try {
			audioBUS.startStream();
		}
		catch (RtAudioError& e) {
			std::cout << '\n' << e.getMessage() << '\n' << std::endl;
			endStream();
			exit(1);
		}

	}
};

#endif