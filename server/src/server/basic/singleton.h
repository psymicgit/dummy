///<------------------------------------------------------------------------------
//< @file:   server\basic\singleton.h
//< @author: hongkunan
//< @date:   2015年1月13日 16:2:11
//< @brief:
//< Copyright (c) 2015 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _singleton_h_
#define _singleton_h_

template <typename T>
class Singleton
{
	struct object_creator {
		object_creator() { Singleton<T>::Instance(); }
	};

	static object_creator create_object;

public:
	typedef T object_type;
	static object_type& Instance()
	{
		static object_type obj;
		return obj;
	}
};

template <typename T>
typename Singleton<T>::object_creator Singleton<T>::create_object;

#endif //_singleton_h_


/* 线程安全版

///<------------------------------------------------------------------------------
//< @file:   server\basic\singleton.h
//< @author: hongkunan
//< @date:   2015年1月13日 16:2:11
//< @brief:
//< Copyright (c) 2015 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _singleton_h_
#define _singleton_h_

// 单例<线程安全>
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