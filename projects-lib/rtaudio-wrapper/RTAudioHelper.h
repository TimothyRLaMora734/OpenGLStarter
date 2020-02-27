#ifndef RTAudioHelper__H
#define RTAudioHelper__H

#include <rtaudio/RtAudio.h>

class RTAudioHelper {
public:
	static void printDevices(RtAudio::Api api = RtAudio::UNSPECIFIED);
};

#endif