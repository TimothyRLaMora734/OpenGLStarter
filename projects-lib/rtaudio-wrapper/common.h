#ifndef RTAUDIO_COMMON_H_
#define RTAUDIO_COMMON_H_

#include <string.h> // memcpy
#include "int_lerp.h"
#include "int24_t.h"
#include "uint32_gcd.h"

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

DefineMethodPointer(RTAUDIO_OnAudioDataMethodPtrT, void, const void* data, size_t frames) VoidMethodCall(data, frames)


#endif
