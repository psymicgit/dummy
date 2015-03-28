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
	: m_lock(mutex_)
{
#ifdef WIN
	InitializeConditionVariable(&m_condVariable);
#else
	pthread_cond_init(&m_cond, NULL);
#endif
}

condition_var_t::~condition_var_t()
{
#ifdef WIN
	broadcast();
#else
	pthread_cond_destroy(&m_cond);
#endif
}

bool condition_var_t::wait()
{
#ifdef WIN
	return this->wait(INFINITE);
#else
	return 0 == pthread_cond_wait(&m_cond, &m_lock.get_mutex());
#endif
}

bool condition_var_t::wait(int ms)
{
#ifdef WIN
	BOOL ret = SleepConditionVariableCS(&m_condVariable, &m_lock.get_mutex(), ms);
	return ret > 0;
#else
	timespec ts = {ms / (1000000), ms*(1000)};
	return 0 == pthread_cond_timedwait(&m_cond, &m_lock.get_mutex(), &ts);
#endif
}

bool condition_var_t::notify()
{
#ifdef WIN
	WakeConditionVariable(&m_condVariable);
	return true;
#else
	return 0 == pthread_cond_signal(&m_cond);
#endif
}

bool condition_var_t::broadcast()
{
#ifdef WIN
	WakeAllConditionVariable(&m_condVariable);
	return true;
#else
	return 0 == pthread_cond_broadcast(&m_cond);
#endif
}
