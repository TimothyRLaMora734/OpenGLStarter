#ifndef platform_sleep_h
#define platform_sleep_h

#include <aribeiro/common.h>
#include "PlatformTime.h"

namespace aRibeiro {

	/// \brief PlatformSleep
	///
	/// Include sleep in several platforms.
	///
	/// \author Alessandro Ribeiro
	///
	class PlatformSleep {
	public:

		/// \brief sleepSec
		///
		/// Sleep for an amount of seconds.
		///
		/// \author Alessandro Ribeiro
		/// \param secs seconds
		///
		static void sleepSec(int secs);

		/// \brief sleepMillis
		///
		/// Sleep for an amount of milliseconds.
		///
		/// \author Alessandro Ribeiro
		/// \param millis milliseconds
		///
		static void sleepMillis(int millis);

		/// \brief busySleepMicro
		///
		/// Sleep for an amount of microseconds.
		/// NOTICE: This method consumes 100% CPU while sleeping when it is in windows OS.
		///
		/// \author Alessandro Ribeiro
		/// \param micros microseconds
		///
		static void busySleepMicro(int64_t micros);
	};

}

#endif