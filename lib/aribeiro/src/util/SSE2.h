#ifndef SSE2__H
#define SSE2__H

#if defined(ARIBEIRO_SSE2)

#include <xmmintrin.h> // SSE1
#include <emmintrin.h> // SSE2


#ifdef _MSC_VER //_WIN32
//  Windows
//#ifdef _MSC_VER
#include <intrin.h>
//#else
//#define cpuid(info, x)    __cpuidex(info, x, 0)
//#endif
#else
//  GCC Intrinsics
#include <cpuid.h>
void cpuid(int info[4], int InfoType);
unsigned long long _xgetbv(unsigned int index);
#endif

class CPUInfo {

	CPUInfo();

public:

	static const CPUInfo &Instance();

	//  Misc.
	bool HW_MMX;
	bool HW_x64;
	bool HW_ABM;      // Advanced Bit Manipulation
	bool HW_RDRAND;
	bool HW_BMI1;
	bool HW_BMI2;
	bool HW_ADX;
	bool HW_PREFETCHWT1;

	//  SIMD: 128-bit
	bool HW_SSE;
	bool HW_SSE2;
	bool HW_SSE3;
	bool HW_SSSE3;
	bool HW_SSE41;
	bool HW_SSE42;
	bool HW_SSE4a;
	bool HW_AES;
	bool HW_SHA;

	//  SIMD: 256-bit
	bool HW_AVX;
	bool HW_XOP; // SSE5
	bool HW_FMA3;
	bool HW_FMA4;
	bool HW_AVX2;

	//  SIMD: 512-bit
	bool HW_AVX512F;    //  AVX512 Foundation
	bool HW_AVX512CD;   //  AVX512 Conflict Detection
	bool HW_AVX512PF;   //  AVX512 Prefetch
	bool HW_AVX512ER;   //  AVX512 Exponential + Reciprocal
	bool HW_AVX512VL;   //  AVX512 Vector Length Extensions
	bool HW_AVX512BW;   //  AVX512 Byte + Word
	bool HW_AVX512DQ;   //  AVX512 Doubleword + Quadword
	bool HW_AVX512IFMA; //  AVX512 Integer 52-bit Fused Multiply-Add
	bool HW_AVX512VBMI; //  AVX512 Vector Byte Manipulation Instructions

	bool OS_AVX_SUPPORTED;
};

/*
#pragma pack(push, 16)

class op{
public:
	float a[4] _MM_ALIGN16;
	float b[4] _MM_ALIGN16;
	float c[4] _MM_ALIGN16;
};

#pragma pack(pop)
*/

#include <stdio.h>

class CheckSSE2 {
public:
	CheckSSE2();
};

extern const CheckSSE2 ___CheckSSE2;

#if _MSC_VER 

//windows compatible
#define _SSE2_ALIGN_PRE _MM_ALIGN16

//other platforms compatible
#define _SSE2_ALIGN_POS
#define ARIBEIRO_INLINE __forceinline


#define _mm_f32_(v,i) v.m128_f32[i]
#define _mm_load_(x,y,z,w) _mm_set_ps(w,z,y,x)

#else

#define _SSE2_ALIGN_PRE

#define _SSE2_ALIGN_POS _MM_ALIGN16
#define ARIBEIRO_INLINE inline __attribute__((always_inline))

#define _mm_f32_(v,i) v[i]
#define _mm_load_(x,y,z,w) _mm_set_ps(w,z,y,x)

#endif


//
// Need to inherits this to objects allocated in the heap, avoiding missaligned data access when using SSE2
//
_SSE2_ALIGN_PRE class SSE2Object {

public:

	void* operator new(size_t size) {
		return _mm_malloc(size, 16);
	}

	void operator delete(void* p) {
		_mm_free(p);
	}

	void* operator new[](size_t size) {
		return _mm_malloc(size, 16);
	}

	void operator delete[](void* p) {
		_mm_free(p);
	}

} _SSE2_ALIGN_POS;


//
// Another way to write sse2 compatible classes, is use SSE2_CLASS_NEW_OPERATOR
//
//
//  these operators guarantees the 16 byte alignment of the class
//
#define SSE2_CLASS_NEW_OPERATOR \
void* operator new(size_t size) {\
	return _mm_malloc(size, 16);\
}\
void operator delete(void* p) { \
	_mm_free(p);\
}\
void* operator new[](size_t size) {\
	return _mm_malloc(size, 16);\
}\
void operator delete[](void* p) {\
	_mm_free(p);\
}

/*
is possible to overload the global new operator...

void* operator new     ( size_t size ) { return myAlloc( size ); }
void* operator new[]   ( size_t size ) { return myAlloc( size ); }
void  operator delete  ( void* ptr   ) { myFree( ptr ); }
void  operator delete[]( void* ptr   ) { myFree( ptr ); }

*/

#else

//
// Not SSE Code
//
#define _SSE2_ALIGN_PRE
#define _SSE2_ALIGN_POS

#if _MSC_VER 
	#define ARIBEIRO_INLINE __forceinline
#else
	#define ARIBEIRO_INLINE inline __attribute__((always_inline))
#endif


class SSE2Object {

public:

};

#define SSE2_CLASS_NEW_OPERATOR


#endif

#endif
