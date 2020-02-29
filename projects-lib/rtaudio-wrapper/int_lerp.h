#ifndef int_lerp__H
#define int_lerp__H

#define _UINT8_MAX 0x7f
#define _UINT16_MAX 0x7fff
#define _UINT24_MAX 0x7fffff
#define _UINT32_MAX 0x7fffffff

#include <stdint.h>

static inline
int32_t int_lerp32_single(const int32_t&a, const int32_t&b, float lerp) {
	float result = (float)a * (1.0f - lerp) + (float)b * lerp;
	if (result < 0)
		result -= 0.5f;
	else
		result += 0.5f;
	return (int32_t)result;
}


static inline
int32_t int_lerp32_double(const int32_t&a, const int32_t&b, double lerp) {
	double result = (double)a * (1.0 - lerp) + (double)b * lerp;
	if (result < 0)
		result -= 0.5f;
	else
		result += 0.5f;
	if (result <= -_UINT32_MAX)
		return -_UINT32_MAX;
	else if (result >= _UINT32_MAX)
		return _UINT32_MAX;
	return (int32_t)result;
}

template<typename C>
static inline
C int_lerp(const C&a, const C&b, float lerp) {
	if (sizeof(C) >= 4)
		return int_lerp32_double(a, b, lerp);
	else
		return int_lerp32_single(a, b, lerp);
}


#endif