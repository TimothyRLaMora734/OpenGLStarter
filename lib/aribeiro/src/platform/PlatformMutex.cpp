#include "PlatformMutex.h"

namespace aRibeiro {

#if defined(ARIBEIRO_OS_WINDOWS)

	PlatformMutex::PlatformMutex() {
		InitializeCriticalSection(&mLock);
	}

	PlatformMutex::~PlatformMutex() {
		DeleteCriticalSection(&mLock);
	}

	void PlatformMutex::lock() {
		EnterCriticalSection(&mLock);
	}

	void PlatformMutex::unlock() {
		LeaveCriticalSection(&mLock);
	}

#elif defined(ARIBEIRO_OS_LINUX) || defined(ARIBEIRO_OS_ANDROID) || defined(ARIBEIRO_OS_IOS) || defined(ARIBEIRO_OS_MACOS)

	PlatformMutex::PlatformMutex() {
		// Set it recursive
		pthread_mutexattr_t attributes;
		pthread_mutexattr_init(&attributes);
		pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_RECURSIVE);

		pthread_mutex_init(&mLock, &attributes);
	}

	PlatformMutex::~PlatformMutex() {
		pthread_mutex_destroy(&mLock);
	}

	void PlatformMutex::lock() {
		pthread_mutex_lock(&mLock);
	}

	void PlatformMutex::unlock() {
		pthread_mutex_unlock(&mLock);
	}

#else
#error Mutex for this platform is not implemented...
#endif

}
