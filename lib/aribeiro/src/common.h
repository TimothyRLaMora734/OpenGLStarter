#ifndef asilva_defs_h
#define asilva_defs_h

#include "stdlib.h" // NULL definition

//#pragma warning( disable : <number> )

#ifdef _WIN32
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4309)
#pragma warning(disable:4018)
#endif

#ifdef _WIN32

#define WIN_LEAN_AND_MEAN

#include <Windows.h>
#include <wchar.h>

#ifndef swprintf
	#define swprintf _snwprintf
#endif

#endif


#include <stdint.h>

#if INTPTR_MAX == INT32_MAX
	#define ARIBEIRO_32_BIT
#elif INTPTR_MAX == INT64_MAX
	#define ARIBEIRO_64_BIT
#else
	#error "Environment not 32 or 64-bit."
#endif

#define ALIGN_BYTES_COUNT 4
#if _WIN32 && _MSC_VER

#define ALIGN_BEGIN
#define ALIGN_END __declspec(align(ALIGN_BYTES_COUNT))

#else

#define ALIGN_BEGIN __attribute__((aligned(ALIGN_BYTES_COUNT)))
#define ALIGN_END

#endif

/*

Example align custom class

class ALIGN_BEGIN Object {
public:
	virtual ~Object() {}
} ALIGN_END;


*/

#define ARIBEIRO_API

////////////////////////////////////////////////////////////
// Identify the operating system
// see http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
////////////////////////////////////////////////////////////
#if defined(_WIN32)

	// Windows
	#define ARIBEIRO_OS_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif

#elif defined(__APPLE__) && defined(__MACH__)
	// Apple platform, see which one it is
	#include "TargetConditionals.h"
	#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
		// iOS
		#define ARIBEIRO_OS_IOS
	#elif TARGET_OS_MAC
		// MacOS
		#define ARIBEIRO_OS_MACOS
	#else
		// Unsupported Apple system
		#error This Apple operating system is not supported by the library
	#endif
#elif defined(__unix__)

	// UNIX system, see which one it is
	#if defined(__ANDROID__)
		// Android
		#define ARIBEIRO_OS_ANDROID
	#elif defined(__linux__)
		// Linux
		#define ARIBEIRO_OS_LINUX
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		// FreeBSD
		#define ARIBEIRO_OS_FREEBSD
	#else
		// Unsupported UNIX system
		#error This UNIX operating system is not supported by the library
	#endif
#else
	// Unsupported system
	#error This operating system is not supported by the library
#endif

#endif
