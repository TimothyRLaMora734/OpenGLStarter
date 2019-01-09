#ifndef PLATFORM_THREAD_H
#define PLATFORM_THREAD_H

#include <aribeiro/common.h>
#include <aribeiro/PlatformMutex.h>
#include <map>

// Fix for unaligned stack with clang and GCC on Windows XP 32-bit
#if defined(ARIBEIRO_OS_WINDOWS) && (defined(__clang__) || defined(__GNUC__))
	#define ALIGN_STACK __attribute__((__force_align_arg_pointer__))
#else
	#define ALIGN_STACK
#endif

#if defined(ARIBEIRO_OS_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	typedef DWORD THREAD_ID_TYPE;
#elif defined(ARIBEIRO_OS_LINUX) || defined(ARIBEIRO_OS_ANDROID)
	#include <sys/syscall.h>
	#include <sys/types.h>
	typedef pid_t THREAD_ID_TYPE;
#elif defined(ARIBEIRO_OS_IOS) || defined(ARIBEIRO_OS_MACOS)
	#include <sys/types.h>
	typedef uint64_t THREAD_ID_TYPE;
#else
	#error Thread for this platform is not implemented...
#endif

namespace aRibeiro {

	//
	// Thread Function templates...
	//
	struct ThreadRunEntryPoint
	{
		virtual ~ThreadRunEntryPoint() {}
		virtual void run() = 0;
	};

	template <typename T>
	struct ThreadRunFunctor : ThreadRunEntryPoint
	{
		ThreadRunFunctor(T functor) : m_functor(functor) {}
		virtual void run() { m_functor(); }
		T m_functor;
	};

	template <typename F, typename A>
	struct ThreadRunFunctorWithArg : ThreadRunEntryPoint
	{
		ThreadRunFunctorWithArg(F function, A arg) : m_function(function), m_arg(arg) {}
		virtual void run() { m_function(m_arg); }
		F m_function;
		A m_arg;
	};

	template <typename C>
	struct ThreadRunMemberFunc : ThreadRunEntryPoint
	{
		ThreadRunMemberFunc(void(C::*function)(), C* object) : m_function(function), m_object(object) {}
		virtual void run() { (m_object->*m_function)(); }
		void(C::*m_function)();
		C* m_object;
	};

	/// \brief PlatformThread
	///
	/// Manager one thread in several platforms. You can use functions, structs and classes.
	/// \author Alessandro Ribeiro
	///
	class PlatformThread
	{
	private:

		//
		// OS SPECIFIC ATTRIBUTES
		//
#if defined(ARIBEIRO_OS_WINDOWS)

		ALIGN_STACK static unsigned int __stdcall entryPoint(void* userData);
		HANDLE m_thread;
		unsigned int m_threadId;

#else
		static void* entryPoint(void* userData);
		pthread_t m_thread;
		bool      m_isActive;
#endif

	private:

		static PlatformMutex openedThreadsLock;
		static std::map<THREAD_ID_TYPE, PlatformThread*> openedThreads;
		static void registerThread(PlatformThread* thread);
		static void unregisterThread(PlatformThread* thread);

		volatile bool interrupted;
		volatile bool exited;

		ThreadRunEntryPoint *runEntryPoint;

		//void run();
	public:

		/// \brief Check if the current thread is interrupted
		///
		/// A thread can be interrupted by calling interrupt() method from the parent thread.
		///
		/// \author Alessandro Ribeiro
		/// \return true if the thread is interrupted
		///
		static bool isCurrentThreadInterrupted();

		/// \brief Get the current Thread
		///
		/// Get the current thread. If the thread is not spawned by the PlatformThread, it will return null.
		///
		/// \author Alessandro Ribeiro
		/// \return A reference to the current thread spawned.
		///
		static PlatformThread* getCurrentThread();

		~PlatformThread();

		/// \brief Interrupt
		///
		/// Set a flag to the thread indicating that it is interrupted.
		///
		/// \author Alessandro Ribeiro
		///
		void interrupt();

		/// \brief Start
		///
		/// Spawn the thread. It can be called just once.
		///
		/// \author Alessandro Ribeiro
		///
		void start();

		/// \brief Wait
		///
		/// Wait the current thread to terminate their execution scope. Can be called once.
		///
		/// \author Alessandro Ribeiro
		///
		void wait();

		/// \brief Terminate
		///
		/// Force the target thread to terminate. It does not garantee the execution scope to terminate. It is dangerous to call this method.
		///
		/// \author Alessandro Ribeiro
		///
		void terminate();

		/// \brief Constructor (function)
		///
		/// Creates a thread associated with a function as its run point.
		///
		/// example:
		///
		/// struct Task
		/// {
		///     void operator()()
		///     {
		///         ...
		///     }
		/// };
		///
		/// PlatformThread thread(Task());
		///
		/// \author Alessandro Ribeiro
		///
		template <typename F>
		PlatformThread(F function) {
			runEntryPoint = new ThreadRunFunctor<F>(function);
			interrupted = false;
			exited = false;
#if defined(ARIBEIRO_OS_WINDOWS)
			m_thread = NULL;
#else
			m_isActive = false;
#endif
		}

		/// \brief Constructor (function, argument)
		///
		/// Creates a thread associated with a function as its run point.
		/// Can pass an argument to the thread.
		///
		/// example:
		///
		/// void threadFunc(int argument)
		/// PlatformThread thread(&threadFunc, 5);
		///
		/// \author Alessandro Ribeiro
		///
		template <typename F, typename A>
		PlatformThread(F function, A argument) {
			runEntryPoint = new ThreadRunFunctorWithArg<F, A>(function, argument);
			interrupted = false;
			exited = false;
#if defined(ARIBEIRO_OS_WINDOWS)
			m_thread = NULL;
#else
			m_isActive = false;
#endif
		}

		/// \brief Constructor (method, object)
		///
		/// Creates a thread associated with a method as its run point.
		/// Need to pass the object as the second parameter
		///
		/// example:
		///
		/// class Task
		/// {
		/// public:
		///     void run()
		///     {
		///         ...
		///     }
		/// };
		/// Task task;
		/// PlatformThread thread(&Task::run, &task);
		///
		/// \author Alessandro Ribeiro
		///
		template <typename C>
		PlatformThread(void(C::*function)(), C* object) {
			runEntryPoint = new ThreadRunMemberFunc<C>(function, object);
			interrupted = false;
			exited = false;
#if defined(ARIBEIRO_OS_WINDOWS)
			m_thread = NULL;
#else
			m_isActive = false;
#endif
		}
	};

}



#endif