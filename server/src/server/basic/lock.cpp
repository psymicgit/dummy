///<------------------------------------------------------------------------------
//< @file:   server\basic\lock.cpp
//< @author: 洪坤安
//< @date:   2015年1月13日 18:31:54
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "lock.h"

#define SPIN_COUNT 2000

mutex_t::mutex_t()
{
#ifdef WIN
	InitializeCriticalSectionAndSpinCount(&m_lock, SPIN_COUNT);
#else
	pthread_mutex_init(&m_mutex, NULL);
#endif
}

mutex_t::~mutex_t()
{
#ifdef WIN
	DeleteCriticalSection(&m_lock);
#else
	pthread_mutex_destroy(&m_mutex);
#endif
}

bool mutex_t::lock()
{
#ifdef WIN
	EnterCriticalSection(&m_lock);
	return true;
#else
	if (pthread_mutex_lock(&m_mutex)) {
		return false;
	}
	return true;
#endif
}

bool mutex_t::unlock()
{
#ifdef WIN
	LeaveCriticalSection(&m_lock);
	return true;
#else
	if (pthread_mutex_unlock(&m_mutex)) {
		return false;
	}
	return true;
#endif
}

bool mutex_t::try_lock()
{
#ifdef WIN
	return TryEnterCriticalSection(&m_lock) == TRUE;
#else
	if (pthread_mutex_trylock(&m_mutex)) {
		return false;
	}

	return true;
#endif
}

condition_var_t::condition_var_t(mutex_t& mutex_)
	: m_mutex(mutex_)
{
#ifdef WIN
	assert(m_event = CreateEvent(NULL, TRUE, FALSE, NULL));
	n_waiting = n_to_wake = generation = 0;
#else
	pthread_cond_init(&m_cond, NULL);
#endif
}

condition_var_t::~condition_var_t()
{
#ifdef WIN
	CloseHandle(m_event);
#else
	pthread_cond_destroy(&m_cond);
#endif
}

bool condition_var_t::wait()
{
#ifdef WIN
	return this->time_wait(0);
#else
	return 0 == pthread_cond_wait(&m_cond, &m_mutex.get_mutex());
#endif
}

bool condition_var_t::time_wait(int us_)
{
#ifdef WIN
	int generation_at_start;
	int waiting = 1;
	int result = -1;

	DWORD ms = INFINITE, ms_orig = INFINITE, startTime, endTime;
	if (us_ > 0) {
		ms_orig = ms = us_ * 1000;
	}

	m_lock.lock();
	++n_waiting;
	generation_at_start = generation;
	m_lock.unlock();

	m_mutex.unlock();

	startTime = GetTickCount();
	do {
		DWORD res;
		res = WaitForSingleObject(m_event, ms);
		m_lock.lock();
		if (n_to_wake &&
		        generation != generation_at_start) {
			--n_to_wake;
			--n_waiting;
			result = 0;
			waiting = 0;
			goto out;
		}
		else if (res != WAIT_OBJECT_0) {
			result = (res == WAIT_TIMEOUT) ? 1 : -1;
			--n_waiting;
			waiting = 0;
			goto out;
		}
		else if (ms != INFINITE) {
			endTime = GetTickCount();
			if (startTime + ms_orig <= endTime) {
				result = 1; /* Timeout */
				--n_waiting;
				waiting = 0;
				goto out;
			}
			else {
				ms = startTime + ms_orig - endTime;
			}
		}
		/* If we make it here, we are still waiting. */
		if (n_to_wake == 0) {
			/* There is nobody else who should wake up; reset
			 * the event. */
			ResetEvent(m_event);
		}
out:
		m_lock.unlock();
	}
	while (waiting);

	m_mutex.lock();

	m_lock.lock();
	if (!n_waiting)
		ResetEvent(m_event);
	m_lock.unlock();

	return result != -1;
#else
	timespec ts = {us_ / (1000000), us_*(1000)};
	return 0 == pthread_cond_timedwait(&m_cond, &m_mutex.get_mutex(), &ts);
#endif
}

bool condition_var_t::notify()
{
#ifdef WIN
	m_lock.lock();
	++n_to_wake;
	generation++;
	SetEvent(m_event);
	m_lock.unlock();

	return true;
#else
	return 0 == pthread_cond_signal(&m_cond);
#endif
}

bool condition_var_t::broadcast()
{
#ifdef WIN
	m_lock.lock();
	n_to_wake = n_waiting;
	generation++;
	SetEvent(m_event);
	m_lock.unlock();

	return true;
#else
	return 0 == pthread_cond_broadcast(&m_cond);
#endif
}
