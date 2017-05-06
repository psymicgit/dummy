///<------------------------------------------------------------------------------
//< @file:   server\basic\singleton.h
//< @author: hongkunan
//< @date:   2015��1��13�� 16:2:11
//< @brief:	 ����ģʽ
//< Copyright (c) 2015 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _singleton_h_
#define _singleton_h_

#include "lock.h"

// ������
// ���磺��ϣ��ĳ����Aӵ�е�����������ɶ���class A : public Singleton<A>����ɵ���A::instance()
template <typename T>
class Singleton
{
private:
	struct born {
		born() { Singleton<T>::Instance(); }
	};

public:
	// �õ����������̰߳�ȫ��
	static T& Instance()
	{
		if (NULL == instance) {
			CreateInstance();
		}

		return *instance;
	}

	// �õ����������̰߳�ȫ��
	static void CreateInstance()
	{
		lock_guard_t<> lock(s_lock);
		if (NULL == instance) {
			T *t = new T;
			instance = t;
		}
	}

private:
	static born s_born;
	static mutex_t s_lock;

public:
	static T *instance;
};

template <typename T>
typename Singleton<T>::born Singleton<T>::s_born;

template <typename T>
T* Singleton<T>::instance = NULL;

template <typename T>
mutex_t Singleton<T>::s_lock;

#endif //_singleton_h_


/* �̰߳�ȫ��

///<------------------------------------------------------------------------------
//< @file:   server\basic\singleton.h
//< @author: hongkunan
//< @date:   2015��1��13�� 16:2:11
//< @brief:
//< Copyright (c) 2015 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _singleton_h_
#define _singleton_h_

// ����<�̰߳�ȫ>
template <typename T>
class Singleton
{
struct object_creator {
object_creator() { Singleton<T>::instance(); }
};

static object_creator create_object;

public:
static T* instance()
{
static T obj;
return &obj;
}

static void init()
{
t = new T();
}

#ifdef WIN
static T& instance()
{
pthread_once(&ponce_, &Singleton::init);
if(InterlockedCompareExchange((uint32_t*)once, 1, 0) == 0) {
init();
}

return *t;
}

private:
static uint32 ponce;
static T* t;
#else
static T& instance()
{
pthread_once(&ponce_, &Singleton::init);
return *t;
}

private:
static pthread_once_t ponce;
static T* t;
#endif
};

template <typename T>
typename Singleton<T>::object_creator Singleton<T>::create_object;

#ifdef WIN
template<typename T>
uint32 Singleton<T>::ponce = 1;

template<typename T>
T* Singleton<T>::value_ = NULL;
#else
template<typename T>
pthread_once_t Singleton<T>::ponce = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;
#endif

#endif //_singleton_h_

*/