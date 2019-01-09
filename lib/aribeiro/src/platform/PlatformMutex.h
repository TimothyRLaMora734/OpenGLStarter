#ifndef PLATFORM_MUTEX_H
#define PLATFORM_MUTEX_H

#include <aribeiro/common.h>


#if defined(ARIBEIRO_OS_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	typedef CRITICAL_SECTION MutexObj;
#elif defined(ARIBEIRO_OS_LINUX) || defined(ARIBEIRO_OS_ANDROID) || defined(ARIBEIRO_OS_IOS) || defined(ARIBEIRO_OS_MACOS)
	#include <pthread.h>
	typedef pthread_mutex_t MutexObj;
#else
	#error Mutex for this platform is not implemented...
#endif

namespace aRibeiro {

	/// \brief PlatformMutex
	///
	/// Mutex common implementation.
	///
	/// \author Alessandro Ribeiro
	///
	class PlatformMutex {
	private:
		MutexObj mLock;
	public:
		PlatformMutex();
		virtual ~PlatformMutex();

		/// \brief lock
		///
		/// Lock the critical section
		///
		/// \author Alessandro Ribeiro
		///
		void lock();

		/// \brief unlock
		///
		/// Unlock the critical section
		///
		/// \author Alessandro Ribeiro
		///
		void unlock();
	};

}

#endif