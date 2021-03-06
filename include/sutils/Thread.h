/*
 * Thread.h
 *
 *  Created on: Jan 1, 2017
 *      Author: jeffrey
 */

#ifndef VMODULE_SUTILS_THREAD_H
#define VMODULE_SUTILS_THREAD_H

#include <string>
#include <stdint.h>
#include <memory>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sutils/Condition.h>
#include <sutils/Mutex.h>
#include <sutils/RefBase.h>
#include <sutils/ThreadDefs.h>
#include <sutils/ILogger.h>

namespace vmodule {
class CThread: virtual public RefBase {
public:
	CThread();
	virtual ~CThread();
	virtual status_t run(const char* name = 0, size_t stack = 0);
	virtual void requestExit();
	virtual status_t readyToRun();
	void Sleep(unsigned int milliseconds=0);
	void Wakeup();
	status_t requestExitAndWait();
	status_t join();
	bool isRunning() const;
#if defined(TARGET_ANDROID)
	// Return the thread's kernel ID, same as the thread itself calling gettid(),
	// or -1 if the thread is not running.
	pid_t getTid() const;
#endif
	vthread_id_t ThreadId() const;
	static CThread* GetCurrentThread();
	static inline void SetLogger(ILogger* logger_) { CThread::logger = logger_; }
	static inline ILogger* GetLogger() { return CThread::logger; }
protected:
	// exitPending() returns true if requestExit() has been called.
	bool exitPending() const;

private:
	// Derived class must implement threadLoop(). The thread starts its life
	// here. There are two ways of using the Thread object:
	// 1) loop: if threadLoop() returns true, it will be called again if
	//          requestExit() wasn't called.
	// 2) once: if threadLoop() returns false, the thread will exit upon return.
	virtual bool threadLoop() = 0;

private:
	CThread& operator=(const CThread&);
	static int _threadLoop(void* user);
	// always hold mLock when reading or writing
	vthread_id_t    m_ThreadId;
	mutable Mutex   mLock;
	Condition       mExitedCondition;
	Condition       mWaitCondition;
	status_t        mStatus;
	// note that all accesses of mExitPending and mRunning need to hold mLock
	volatile bool   mExitPending;
	volatile bool   mRunning;
	sp<CThread>     mHoldSelf;
#if defined(TARGET_ANDROID)
    // legacy for debugging, not used by getTid() as it is set by the child thread
    // and so is not initialized until the child reaches that point
    pid_t           mTid;
#endif
    std::string m_ThreadName;
    static ILogger* logger;
};
}

#endif //endof VMODULE_THREAD_H

