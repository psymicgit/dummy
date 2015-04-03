///<------------------------------------------------------------------------------
//< @file:   server\basic\lock.h
//< @author: 洪坤安
//< @date:   2015年1月13日 18:31:38
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _lock_h_
#define _lock_h_

// 锁：windows下为自旋锁和互斥锁混合，linux下则为互斥锁
class mutex_t
{
public:
	mutex_t();
	~mutex_t();
	bool lock();
	bool unlock();
	bool try_lock();

#ifdef WIN
public:
	CRITICAL_SECTION& get_mutex() {return m_lock;}

private:
	CRITICAL_SECTION m_lock;

#else
public:
	pthread_mutex_t& get_mutex() {return m_mutex;}

private:
	pthread_mutex_t              m_mutex;
#endif
};

// 空锁，什么也不做
class NonMutex
{
public:
	inline void lock() {}
	inline void unlock() {}
};

// 首先检测是否支持asm级别的mutex，支持的话则实现fast_mutex
// Check if we can support the assembly language level implementation (otherwise
// revert to the system API)
#if (defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))) || \
	(defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))) || \
	(defined(__GNUC__) && (defined(__ppc__)))
	#define _FAST_MUTEX_ASM_
#else
	#define _FAST_MUTEX_SYS_
#endif

/// fast_mutex来源于tinythread++库，基本功能与mutex相同，但无法与条件变量结合使用，适用于仅有lock和unlock的场景
/// 具体说明见下方tinythread++原英文注释
///
/// Fast mutex class.
/// This is a mutual exclusion object for synchronizing access to shared
/// memory areas for several threads. It is similar to the tthread::mutex class,
/// but instead of using system level functions, it is implemented as an atomic
/// spin lock with very low CPU overhead.
///
/// The \c fast_mutex class is NOT compatible with the \c condition_variable
/// class (however, it IS compatible with the \c lock_guard class). It should
/// also be noted that the \c fast_mutex class typically does not provide
/// as accurate thread scheduling as a the standard \c mutex class does.
///
/// Because of the limitations of the class, it should only be used in
/// situations where the mutex needs to be locked/unlocked very frequently.
///
/// @note The "fast" version of this class relies on inline assembler language,
/// which is currently only supported for 32/64-bit Intel x86/AMD64 and
/// PowerPC architectures on a limited number of compilers (GNU g++ and MS
/// Visual C++).
/// For other architectures/compilers, system functions are used instead.
class fast_mutex
{
public:
	/// Constructor.
#if defined(_FAST_MUTEX_ASM_)
	fast_mutex() : mLock(0) {}
#else
	fast_mutex()
	{
#if defined(WIN)
		InitializeCriticalSection(&mHandle);
#elif defined(POSIX)
		pthread_mutex_init(&mHandle, NULL);
#endif
	}
#endif

#if !defined(_FAST_MUTEX_ASM_)
	/// Destructor.
	~fast_mutex()
	{
#if defined(WIN)
		DeleteCriticalSection(&mHandle);
#elif defined(POSIX)
		pthread_mutex_destroy(&mHandle);
#endif
	}
#endif

	/// Lock the mutex.
	/// The method will block the calling thread until a lock on the mutex can
	/// be obtained. The mutex remains locked until \c unlock() is called.
	/// @see lock_guard
	inline void lock()
	{
#if defined(_FAST_MUTEX_ASM_)
		bool gotLock;
		do {
			gotLock = try_lock();
			if(!gotLock) {
#if defined(WIN)
				Sleep(0);
#elif defined(POSIX)
				sched_yield();
#endif
			}
		}
		while(!gotLock);
#else
#if defined(WIN)
		EnterCriticalSection(&mHandle);
#elif defined(POSIX)
		pthread_mutex_lock(&mHandle);
#endif
#endif
	}

	/// Try to lock the mutex.
	/// The method will try to lock the mutex. If it fails, the function will
	/// return immediately (non-blocking).
	/// @return \c true if the lock was acquired, or \c false if the lock could
	/// not be acquired.
	inline bool try_lock()
	{
#if defined(_FAST_MUTEX_ASM_)
		int oldLock;
#if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
		asm volatile (
		    "movl $1,%%eax\n\t"
		    "xchg %%eax,%0\n\t"
		    "movl %%eax,%1\n\t"
		    : "=m" (mLock), "=m" (oldLock)
		    :
		    : "%eax", "memory"
		);
#elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
		int *ptrLock = &mLock;
		__asm {
			mov eax, 1
			mov ecx, ptrLock
			xchg eax, [ecx]
			mov oldLock, eax
		}
#elif defined(__GNUC__) && (defined(__ppc__))
		int newLock = 1;
		asm volatile (
		    "\n1:\n\t"
		    "lwarx  %0,0,%1\n\t"
		    "cmpwi  0,%0,0\n\t"
		    "bne-   2f\n\t"
		    "stwcx. %2,0,%1\n\t"
		    "bne-   1b\n\t"
		    "isync\n"
		    "2:\n\t"
		    : "=&r" (oldLock)
		    : "r" (&mLock), "r" (newLock)
		    : "cr0", "memory"
		);
#endif
		return (oldLock == 0);
#else
#if defined(WIN)
		return TryEnterCriticalSection(&mHandle) ? true : false;
#elif defined(POSIX)
		return (pthread_mutex_trylock(&mHandle) == 0) ? true : false;
#endif
#endif
	}

	/// Unlock the mutex.
	/// If any threads are waiting for the lock on this mutex, one of them will
	/// be unblocked.
	inline void unlock()
	{
#if defined(_FAST_MUTEX_ASM_)
#if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
		asm volatile (
		    "movl $0,%%eax\n\t"
		    "xchg %%eax,%0\n\t"
		    : "=m" (mLock)
		    :
		    : "%eax", "memory"
		);
#elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
		int *ptrLock = &mLock;
		__asm {
			mov eax, 0
			mov ecx, ptrLock
			xchg eax, [ecx]
		}
#elif defined(__GNUC__) && (defined(__ppc__))
		asm volatile (
		    "sync\n\t"  // Replace with lwsync where possible?
		    : : : "memory"
		);
		mLock = 0;
#endif
#else
#if defined(WIN)
		LeaveCriticalSection(&mHandle);
#elif defined(POSIX)
		pthread_mutex_unlock(&mHandle);
#endif
#endif
	}

private:
#if defined(_FAST_MUTEX_ASM_)
	int mLock;
#else
#if defined(WIN)
	CRITICAL_SECTION mHandle;
#elif defined(POSIX)
	pthread_mutex_t mHandle;
#endif
#endif
};

template <typename T = mutex_t>
class lock_guard_t
{
public:
	lock_guard_t(T& mutex_):
		m_flag(false),
		m_mutex(mutex_)
	{
		m_mutex.lock();
		m_flag = true;
	}

	~lock_guard_t()
	{
		if (is_locked()) {
			unlock();
		}
	}

	void unlock()
	{
		m_flag = false;
		m_mutex.unlock();
	}

	bool is_locked() const { return m_flag; }
private:
	bool m_flag;
	T&      m_mutex;
};

class condition_var_t
{
public:
	condition_var_t(mutex_t& mutex_);
	~condition_var_t();
	bool wait();
	bool wait(int ms);

	bool notify();
	bool broadcast();

private:
	mutex_t&                         m_lock;

#ifdef WIN
	CONDITION_VARIABLE m_condVariable;
#else
	pthread_cond_t                m_cond;
#endif
};

#endif //_lock_h_