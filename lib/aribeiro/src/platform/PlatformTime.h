#ifndef platform_time_h
#define platform_time_h

#include <aribeiro/common.h>

#if defined(ARIBEIRO_OS_WINDOWS)

#define WIN_LEAN_AND_MEAN
#include <windows.h>
#include <inttypes.h>

#include <time.h>

namespace aRibeiro {

	class w32PerformanceCounter {

	private:
		static double freqMicro;
		static double freqMillis;
		static uint64_t freqSec;
		static bool filledFreqConstants;

		LARGE_INTEGER CounterStart;
		LARGE_INTEGER CounterStartBackup;

	public:
		w32PerformanceCounter();
		void UndoReset();
		void Reset();
		//uint64_t GetCounterMillis(bool reset = false);
		int64_t GetCounterMicro(bool reset = false);
		//__int64 GetCounterSec(bool reset = false);
	};

}
#else

#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#ifdef __APPLE__
	#include <stdint.h> // uint64_t on iPhone
	#include <mach/mach_time.h>
#endif

namespace aRibeiro {

	class UnixMicroCounter {
	private:

#ifdef __APPLE__
		// && __iOS__
		// Get the timebase info
		mach_timebase_info_data_t info;
#endif

		double now_microseconds_double(void);
		uint64_t now_microseconds_uint64_t(void);
		uint64_t base;
	public:
		UnixMicroCounter();
		int64_t GetDeltaMicro(bool reset);
	};

}

#endif

namespace aRibeiro {

	/// \brief PlatformTime
	///
	/// Created to compute the inter frame delta time of an interactive application.
	/// \author Alessandro Ribeiro
	///
	class PlatformTime {

#if defined(ARIBEIRO_OS_WINDOWS)
		w32PerformanceCounter win32_counter;
#else
		UnixMicroCounter unix_counter;
#endif

	public:

		PlatformTime();

		/// \brief update
		///
		/// Must be called to compute the delta time.
		///
		/// Example:
		///
		/// PlatformTime time;
		/// float elapsedSinceStartTimeSec = 0;
		/// while (window.open()) {
		///   time.update();
		///
		///   // use delta time to advance the interactive system
		///   elapsedSinceStartTimeSec += time.deltaTime;
		///
		/// }
		///
		/// \author Alessandro Ribeiro
		///
		void update();

		float deltaTime;///<time between two updates in seconds. The value is multiplied by timeScale.
		float unscaledDeltaTime;///<time between two updates in seconds. Not affected by the timeScale.
		float timeScale;///<Value that is used to multiply the deltaTime

		int64_t deltaTimeMicro;///<time between two updates in microseconds. Not affected by the timeScale.
	};

}

#endif
