#include "PlatformThread.h"

#include <stdio.h>

#if defined(ARIBEIRO_OS_WINDOWS)
	#include <process.h>

#elif defined(ARIBEIRO_OS_LINUX) || defined(ARIBEIRO_OS_ANDROID)
	#include <stdlib.h>
	#include <unistd.h>
	#include <sys/syscall.h>
	#include <sys/types.h>
	#include <signal.h>
	#include <pthread.h>

	#define GetCurrentThreadId() syscall(SYS_gettid)
#elif defined(ARIBEIRO_OS_IOS) || defined(ARIBEIRO_OS_MACOS)
	#include <stdlib.h>
	#include <unistd.h>
	#include <sys/syscall.h>
	#include <sys/types.h>
	#include <signal.h>
	#include <pthread.h>

	uint64_t GetCurrentThreadId() {
		uint64_t tid;
		pthread_threadid_np(NULL, &tid);
		return tid;
	}
#else
	#error Thread for this platform is not implemented...
#endif

namespace aRibeiro {

	//
	// GLOBALS STATIC
	//
	PlatformMutex PlatformThread::openedThreadsLock;
	std::map<THREAD_ID_TYPE, PlatformThread*> PlatformThread::openedThreads;

	void PlatformThread::registerThread(PlatformThread* thread) {
		THREAD_ID_TYPE tid = GetCurrentThreadId();
		openedThreadsLock.lock();
		openedThreads[tid] = thread;
		openedThreadsLock.unlock();
	}

	void PlatformThread::unregisterThread(PlatformThread* thread) {
		THREAD_ID_TYPE tid = GetCurrentThreadId();
		std::map<THREAD_ID_TYPE, PlatformThread*>::iterator it;
		openedThreadsLock.lock();
		if (openedThreads.size() > 0) {
			it = openedThreads.find(tid);
			if (it != openedThreads.end()) {
				PlatformThread* result = it->second;
				openedThreads.erase(it);
				result->exited = true;
			}
		}
		openedThreadsLock.unlock();
	}

	bool PlatformThread::isCurrentThreadInterrupted() {
		PlatformThread* thread = getCurrentThread();
		if (thread != NULL)
			return thread->interrupted;
		return false;
	}

	PlatformThread* PlatformThread::getCurrentThread() {
		PlatformThread* result = NULL;
		THREAD_ID_TYPE tid = GetCurrentThreadId();
		std::map<THREAD_ID_TYPE, PlatformThread*>::iterator it;
		openedThreadsLock.lock();
		if (openedThreads.size() > 0) {
			it = openedThreads.find(tid);
			if (it != openedThreads.end())
				result = it->second;
		}
		openedThreadsLock.unlock();
		return result;
	}

	//
	// GLOBALS - NON STATIC
	//
	void PlatformThread::interrupt() {
		interrupted = true;
	}

	PlatformThread::~PlatformThread() {
		wait();
		delete runEntryPoint;
		runEntryPoint = NULL;
	}

	void PlatformThread::start() {
		wait();
		// create OS thread
		//m_impl = new priv::ThreadImpl(this);

#if defined(ARIBEIRO_OS_WINDOWS)
		m_thread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &PlatformThread::entryPoint, this, 0, &m_threadId));

		if (!m_thread)
		{
			printf("Erro ao abrir thread...\n");
		}

#else
		m_isActive = pthread_create(&m_thread, NULL, &PlatformThread::entryPoint, this) == 0;
		if (!m_isActive)
		{
			printf("Erro ao abrir thread...\n");
		}
#endif

	}

	void PlatformThread::wait() {

		/*
		if (m_impl)
		{
			m_impl->wait();
			delete m_impl;
			m_impl = NULL;
		}
		*/
#if defined(ARIBEIRO_OS_WINDOWS)
		//wait handle
		if (m_thread)
		{
			// A thread cannot wait for itself!
			if (m_threadId != GetCurrentThreadId())
				WaitForSingleObject(m_thread, INFINITE);
		}
		//CLOSE THREAD
		if (m_thread)
			CloseHandle(m_thread);
		m_thread = NULL;
#else
		if (m_isActive)
		{
			// A thread cannot wait for itself!
			if (pthread_equal(pthread_self(), m_thread) == 0)
				pthread_join(m_thread, NULL);
		}
#endif
	}

	void PlatformThread::terminate() {
		/*
		if (m_impl)
		{
			m_impl->terminate();
			delete m_impl;
			m_impl = NULL;
		}*/

#if defined(ARIBEIRO_OS_WINDOWS)
		if (m_thread)
			TerminateThread(m_thread, 0);
		//CLOSE THREAD
		if (m_thread)
			CloseHandle(m_thread);
		m_thread = NULL;
#else
		if (m_isActive)
		{
#ifndef OS_ANDROID
			pthread_cancel(m_thread);
#else
			// See http://stackoverflow.com/questions/4610086/pthread-cancel-al
			pthread_kill(m_thread, SIGUSR1);
#endif
		}
#endif
	}

	//void PlatformThread::run()
	//{
	//	runEntryPoint->run();
	//}

#if defined(ARIBEIRO_OS_WINDOWS)

	unsigned int __stdcall PlatformThread::entryPoint(void* userData)
	{
		// The Thread instance is stored in the user data
		PlatformThread* owner = static_cast<PlatformThread*>(userData);
		registerThread(owner);
		// Forward to the owner
		owner->runEntryPoint->run();
		unregisterThread(owner);
		// Optional, but it is cleaner
		_endthreadex(0);
		return 0;
	}

#else
	void* PlatformThread::entryPoint(void* userData)
	{
		// The Thread instance is stored in the user data
		PlatformThread* owner = static_cast<PlatformThread*>(userData);

#ifndef OS_ANDROID
		// Tell the thread to handle cancel requests immediately
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
#endif
		registerThread(owner);
		// Forward to the owner
		owner->runEntryPoint->run();
		unregisterThread(owner);
		return NULL;
	}
#endif

}
