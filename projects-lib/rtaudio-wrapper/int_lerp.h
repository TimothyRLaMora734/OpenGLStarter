#ifndef int_lerp__H
#define int_lerp__H

#define _UINT8_MAX 0x7f
#define _UINT16_MAX 0x7fff
#define _UINT24_MAX 0x7fffff
#define _UINT32_MAX 0x7fffffff

#include <stdint.h>

static inline
int32_t int_lerp32(const int32_t&a_32i, const int32_t&b_32i, float lerp) {
	int32_t delta_32i = b_32i - a_32i;
	int32_t result_32i;
	if (delta_32i < 0) {
		delta_32i = -delta_32i;
		float delta_32f = (float)delta_32i;
		result_32i = a_32i - (int32_t)(delta_32f * lerp + 0.5f);
	}
	else {
		float delta_32f = (float)delta_32i;
		result_32i = a_32i + (int32_t)(delta_32f * lerp + 0.5f);
	}
	return result_32i;
}

static inline
int64_t int_lerp64(const int64_t&a_64i, const int64_t&b_64i, float lerp) {
	int64_t delta_64i = b_64i - a_64i;
	int64_t result_64i;
	if (delta_64i < 0) {
		delta_64i = -delta_64i;
		double delta_64f = (double)delta_64i;
		result_64i = a_64i - (int64_t)(delta_64f * lerp + 0.5);
	}
	else {
		double delta_64f = (double)delta_64i;
		result_64i = a_64i + (int64_t)(delta_64f * lerp + 0.5);
	}
	return result_64i;
}


template<typename C>
static inline
C int_lerp(const C&a, const C&b, float lerp) {
	if (sizeof(C) >= 4)
		return (int32_t)int_lerp64(a, b, lerp);
	else
		return int_lerp32(a, b, lerp);
}


#endif