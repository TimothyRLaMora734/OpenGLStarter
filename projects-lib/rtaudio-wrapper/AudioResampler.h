#ifndef AUDIO_RESAMPLER_H_
#define AUDIO_RESAMPLER_H_

#include "common.h"

template<typename C>
class AudioResampler {

	RTAUDIO_OnAudioDataMethodPtrT OnData;

	bool isFloat;
	uint32_t    toSamplerate, // to samplerate
		fromSamplerate; // from samplerate
	int channels;
	int frames;

	int gcd;

	C *buffer;
	int bufferCount;

	//after use GCD
	int fromGCD;
	int toGCD;
	int fromCount;
	C *fromBuffer;

	float fromDelta;
	float fromDelta_inv;

	inline void putToBufferLerpSample(const C *sampleA, const C *sampleB, float lerp) {
		if (isFloat) {
			for (int j = 0; j < channels; j++) {
				buffer[bufferCount*channels + j] = sampleA[j] * (C)(1.0f - lerp) + sampleB[j] * (C)lerp;
			}
		}
		else {
			if (sizeof(C) >= 4) {
				for (int j = 0; j < channels; j++) {
					buffer[bufferCount*channels + j] = (int32_t)int_lerp64(sampleA[j], sampleB[j], lerp);
				}
			}
			else {
				for (int j = 0; j < channels; j++) {
					buffer[bufferCount*channels + j] = int_lerp32(sampleA[j], sampleB[j], lerp);
				}
			}
		}
		bufferCount = (bufferCount + 1) % frames;
		if (bufferCount == 0)
			OnData(buffer, frames);
	}

	inline void putToBufferSample(const C *sample) {
		memcpy(&buffer[bufferCount*channels], sample, sizeof(C)*channels);

		bufferCount = (bufferCount + 1) % frames;
		if (bufferCount == 0)
			OnData(buffer, frames);
	}

public:

	AudioResampler() {
		buffer = NULL;
		fromBuffer = NULL;
		isFloat = false;
	}

	virtual ~AudioResampler() {
		if (buffer != NULL)
			delete[] buffer;
		if (fromBuffer != NULL)
			delete[] fromBuffer;
		buffer = NULL;
		fromBuffer = NULL;
	}

	void init(bool isInt, 
			  int frames, 
			  int channels, 
			  uint32_t fromSamplerate, 
			  uint32_t toSamplerate,
			  const RTAUDIO_OnAudioDataMethodPtrT &OnData) {
		if (buffer != NULL)
			return;
		isFloat = !isInt;
		this->OnData = OnData;
		bufferCount = 0;

		this->toSamplerate = toSamplerate;
		this->fromSamplerate = fromSamplerate;
		this->channels = channels;
		this->frames = frames;
		this->gcd = uint32_gcd(toSamplerate, fromSamplerate);

		fromGCD = fromSamplerate / this->gcd;
		toGCD = toSamplerate / this->gcd;

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
		//printf("gcd: %u requested(to) %u real(from) %u", this->gcd, toGCD, fromGCD);

		fromBuffer = new C[(fromGCD + 1)*channels];
		buffer = new C[frames*channels];
		fromDelta = 1.0f / (float)(fromGCD);
		fromDelta_inv = (float)(fromGCD);
	}

	inline void putSample(const C *sample) {
		//printf("-------------------------\n", fromCount);
		//printf("            putSample[%i]\n", fromCount);

		if (buffer == NULL)
			return;

		memcpy(&fromBuffer[fromCount*channels], sample, sizeof(C)*channels);

		if (fromCount == 0)
			putToBufferSample(sample);

		fromCount++;

		if (fromCount >= fromGCD + 1) {
			//printf("            fromCount >= fromGCD\n");
			//you can process the From window with fromGCD+1 elements
			//to the window with toGCD+1 elements
			// each toGCD generated need to call putToBufferSample

			for (int i = 1; i < toGCD; i++) {

				float toGCDLerp = (float)i / (float)(toGCD);

				int indexFromA = (int)(toGCDLerp * fromDelta_inv); // (float)(fromGCD) );
				int indexFromB = indexFromA + 1;

				if (indexFromA >= fromGCD) {
					fprintf(stderr, "error on resampling algorithm...\n");
					exit(1);
				}

				float fromMin = (float)indexFromA * fromDelta;// /(float)(fromGCD);

				//float fromMax = (float)indexFromB/(float)(fromGCD);
				//float fromDelta = fromMax - fromMin;

				//not discover the amount of lerp toGCDLerp is relative to min and max
				float lrp = (toGCDLerp - fromMin) * fromDelta_inv;// /fromDelta;

				//printf("               for toGCDLerp=%f indexFromA=%i fromMin=%f lrp=%f\n", toGCDLerp, indexFromA, fromMin, lrp);

				putToBufferLerpSample(&fromBuffer[indexFromA*channels], &fromBuffer[indexFromB*channels], lrp);

			}

			//put sample 0 again
			putToBufferSample(sample);

			//copy the sample to position 0
			fromCount = 1;

			memcpy(fromBuffer, sample, sizeof(C)*channels);

		}

	}
};


#endif
