#include <aribeiro/aribeiro.h>

using namespace aRibeiro;



#include <stdio.h>
//
// Test: PlatformTime PlatformSleep
//
void test_PlatformTime_and_PlatformSleep() {

	printf("---------------------------------------\n");
	printf("  test_PlatformTime_and_PlatformSleep\n");
	printf("---------------------------------------\n");

	PlatformTime time;

	time.timeScale = 0.5f;
	printf("time.timeScale = 0.5f\n");

	time.update();
	PlatformSleep::sleepMillis(5);
	time.update();

#if !defined(ARIBEIRO_OS_WINDOWS)

	printf("PlatformSleep::sleepMillis(5): %lld\n", time.deltaTimeMicro);

#else

	printf("PlatformSleep::sleepMillis(5): %I64d\n", time.deltaTimeMicro);

#endif




	time.update();
	PlatformSleep::busySleepMicro(300);
	time.update();

#if !defined(ARIBEIRO_OS_WINDOWS)
	printf("PlatformSleep::busySleepMicro(300): %lld\n", time.deltaTimeMicro);
#else
	printf("PlatformSleep::busySleepMicro(300): %I64d\n", time.deltaTimeMicro);
#endif


	time.update();
	PlatformSleep::sleepMillis(2000);
	time.update();

	printf("time.deltaTime (2 secs): %f\n", time.deltaTime);
	printf("time.unscaledDeltaTime (2 secs): %f\n", time.unscaledDeltaTime);

	printf("\n");
}
//
// Test: PlatformThread PlatformMutex
//
volatile int sharedCounter = 0;

void Thread1_IncrementCounter() {
	sharedCounter = 0;
	printf("Thread 1 Start...\n");
	PlatformSleep::sleepMillis(3000);
	printf("Thread 1 SharedCounter Increment...\n");
	sharedCounter++;
	PlatformSleep::sleepMillis(3000);
	printf("Thread 1 SharedCounter Increment...\n");
	sharedCounter++;
	PlatformSleep::sleepMillis(3000);
	printf("Thread 1 SharedCounter Increment...\n");
	sharedCounter++;
	PlatformSleep::sleepMillis(3000);
	printf("Thread 1 End...\n");
}

void Thread2_CheckCounter() {
	int localCounter = 0;

	printf("  Thread 2 Start...\n");

	while (localCounter < 3) {
		if (localCounter < sharedCounter) {
			printf("  Thread 2 Detected SharedCounter modification...\n");
			printf("  Thread 2 LocalCounter Increment...\n");
			localCounter++;
		}
		//avoid 100% CPU using by this thread
		PlatformSleep::sleepMillis(1);
	}
	printf("  Thread 2 End...\n");
}

void Thread3_Interrupt() {

	printf("Thread3_Interrupt Start...\n");
	printf("Thread3_Interrupt waiting interrupt from another thread...\n");

	while (!PlatformThread::isCurrentThreadInterrupted()) {
		//avoid 100% CPU using by this thread
		PlatformSleep::sleepMillis(3000);
	}

	printf("Thread3_Interrupt Interrupt Detected !!!\n");
	printf("Thread3_Interrupt End...\n");
}

PlatformMutex mutex;

void Thread4_Mutex() {
	printf("Thread4_Mutex Start...\n");

	mutex.lock();

	printf("Thread4_Mutex Critical Section Enter Success !!!\n");

	mutex.unlock();

	printf("Thread4_Mutex End...\n");
}

void test_PlatformThread_and_PlatformMutex() {

	printf("---------------------------------------\n");
	printf("  test_PlatformThread\n");
	printf("---------------------------------------\n");

	{
		PlatformThread t1(Thread1_IncrementCounter);
		PlatformThread t2(Thread2_CheckCounter);

		t1.start();
		t2.start();
	}
	printf("\n");

	printf("  InterruptTest\n");
	printf("----------------\n");
	{
		PlatformThread t3(Thread3_Interrupt);
		t3.start();


		PlatformSleep::sleepMillis(4000);
		printf("  MainThread: Interrupt and Waiting thread To Finish!!!\n");
		t3.interrupt();
		t3.wait();
		printf("  MainThread: Thread Finish Detected!!!\n");

	}


	printf("\n");

	printf("  MutexTest\n");
	printf("----------------\n");
	{
		PlatformThread t3(Thread4_Mutex);

		printf("  MainThread: Mutex lock before thread start\n");
		mutex.lock();

		t3.start();

		PlatformSleep::sleepSec(3);

		printf("  MainThread: Mutex unlock\n");
		mutex.unlock();

	}

	printf("\n");
}

//
// Test: PlatformPath
//
void test_PlatformPath(char* argv0) {
	printf("---------------------------------------\n");
	printf("  test_PlatformPath\n");
	printf("---------------------------------------\n");

	std::string saveGamePath = PlatformPath::getSaveGamePath("CompanyName", "GameName");
	printf("  SaveGamePath: %s\n", saveGamePath.c_str());

	std::string exePath = PlatformPath::getExecutablePath(argv0);
	std::string workPath = PlatformPath::getWorkingPath();

	printf("  exePath: %s\n", exePath.c_str());
	printf("  workPath: %s\n", workPath.c_str());

	printf(" changeWorkPath!!!\n");
	PlatformPath::setWorkingPath(PlatformPath::getExecutablePath(argv0));

	workPath = PlatformPath::getWorkingPath();
	printf("  new workPath: %s\n", workPath.c_str());

	printf("\n");
}

//
// Test: Util test
//

void DelegateFunctionOutside(int v) {
	printf("  executed function DelegateFunctionOutside: %i\n", v);
}
class DelegateTest {
public:
	BEGIN_DECLARE_DELEGATE(MethodDelegateWithIntParameter, int v) CALL_PATTERN(v) END_DECLARE_DELEGATE;

	MethodDelegateWithIntParameter OnInt;

	DelegateTest() {

		printf(" DelegateTest\n");

		OnInt.add(this, &DelegateTest::int1);
		OnInt.add(this, &DelegateTest::int2);
		OnInt.add(&DelegateFunctionOutside);

		OnInt.call(10);
	}

	void int1(int v) {
		printf("  executed method int1: %i\n", v);
	}

	void int2(int v) {
		printf("  executed method int2: %i\n", v);
	}
};

void test_Util() {
	printf("---------------------------------------\n");
	printf("  test_Util\n");
	printf("---------------------------------------\n");
	{
		DelegateTest();
	}

	printf("\n");

	printf(" getFirstMacAddress\n");
	getFirstMacAddress();

	printf("\n");
	printf(" setNullAndDelete\n");
	int* test = new int;

#if defined(ARIBEIRO_32_BIT)
	printf(" address before: 0x%x\n", (unsigned int)test);
#elif defined(ARIBEIRO_64_BIT)

#if !defined(ARIBEIRO_OS_WINDOWS)
	printf(" address before: 0x%llx\n", (uint64_t)test);
#else
	printf(" address before: 0x%I64x\n", (uint64_t)test);
#endif

#else
	printf(" ARCH DETECT ERROR...\n");
#endif

	setNullAndDelete(test);

#if defined(ARIBEIRO_32_BIT)
	printf(" address after: 0x%x\n", (unsigned int)test);
#elif defined(ARIBEIRO_64_BIT)
#if !defined(ARIBEIRO_OS_WINDOWS)
	printf(" address after: 0x%llx\n", (uint64_t)test);
#else
	printf(" address after: 0x%I64x\n", (uint64_t)test);
#endif
#else
	printf(" ARCH DETECT ERROR...\n");
#endif

	printf("\n");
}


int main(int argc, char* argv[]) {

	test_PlatformPath(argv[0]);
	test_PlatformTime_and_PlatformSleep();
	test_PlatformThread_and_PlatformMutex();
	test_Util();



	printf("  press any key to exit...\n");
	fgetc(stdin);

	return 0;
}
