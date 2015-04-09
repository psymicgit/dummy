///<------------------------------------------------------------------------------
//< @file:   win\server\singleton.h
//< @author: yewenhui
//< @date:   2014年5月4日 17:8:53
//< @brief:  根据boost创建的单例模板类
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _singleton_h_
#define _singleton_h_

template <typename T>
class Singleton
{
	struct object_creator
	{
		object_creator(){ Singleton<T>::Instance(); }
		//		inline void do_nothing()const {}
	};

	static object_creator create_object;

public:
	typedef T object_type;
	static object_type* Instance()
	{
		static object_type obj;
		//		create_object.do_nothing();
		return &obj;
	}
};

template <typename T>
typename Singleton<T>::object_creator Singleton<T>::create_object;

#endif //_singleton_h_
