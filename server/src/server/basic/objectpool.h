///<------------------------------------------------------------------------------
//< @file:   server\basic\objectpool.h
//< @author: 洪坤安
//< @date:   2015年4月3日 23:35:15
//< @brief:  对象池
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _objectpool_h_
#define _objectpool_h_

#include <deque>
#include "lock.h"

// 对象池
template <typename T, typename MutexType = NonMutex /* 默认不持锁 */>
class ObjectPool
{
	typedef std::vector<T*> ObjectQueue;

	enum {
		POOL_EXPAND_SIZE = 100,
	};

public:
	ObjectPool(uint32 initSize = 0, uint32 growSize = 1) throw()
		: m_totalSize(0)
	{
		init(initSize, growSize);
	}

	~ObjectPool() throw()
	{
		lock_guard_t<MutexType> lock(m_mutex);

		for(typename ObjectQueue::iterator itr = m_objects.begin(); itr != m_objects.end(); ++itr) {
			::operator delete (*itr);
		}

		m_objects.clear();
	}

	void init(uint32 initSize, uint32 growSize)
	{
		m_initSize = initSize;
		m_growSize = growSize;

		lock_guard_t<MutexType> lock(m_mutex);
		grow(m_initSize);
	}

	// 分配一个对象, 如果对象构造函数没有参数，使用此接口
	inline T* alloc()
	{
		return new (allocate())T;
	}

	// 分配一个对象, 对象的构造函数包含一个输入参数
	template<typename P1>
	T* alloc(P1 p1)
	{
		return new (allocate())T(p1);
	}

	// 分配一个对象, 对象的构造函数包含2个输入参数
	template<typename P1, typename P2>
	T* alloc(P1 p1, P2 p2)
	{
		return new (allocate())T(p1, p2);
	}

	// 分配一个对象, 对象的构造函数包含3个输入参数
	template<typename P1, typename P2, typename P3>
	T* alloc(P1 p1, P2 p2, P3 p3)
	{
		return new (allocate())T(p1, p2, p3);
	}

	// 分配一个对象, 对象的构造函数包含4个输入参数
	template<typename P1, typename P2, typename P3, typename P4>
	T* alloc(P1 p1, P2 p2, P3 p3, P4 p4)
	{
		return new (allocate())T(p1, p2, p3, p4);
	}

	// 分配一个对象, 对象的构造函数包含5个输入参数
	template<typename P1, typename P2, typename P3, typename P4, typename P5>
	T* alloc(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		return new (allocate())T(p1, p2, p3, p4, p5);
	}

	// 分配一个对象, 对象的构造函数包含6个输入参数
	template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	T* alloc(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
	{
		return new (allocate())T(p1, p2, p3, p4, p5, p6);
	}

	// 归还一个对象
	void free(T * obj)
	{
		obj->~T();
		deallocate(obj);
	}

	void clear()
	{
		lock_guard_t<MutexType> lock(m_mutex);

		m_totalSize -= m_objects.size();

		for(typename ObjectQueue::iterator itr = m_objects.begin(); itr != m_objects.end(); ++itr) {
			T *obj = *itr;
			::operator delete (obj);
		}

		m_objects.clear();
	}

	int size()
	{
		return m_objects.size();
	}

private:
	T* allocate()
	{
		lock_guard_t<MutexType> lock(m_mutex);
		if (m_objects.empty()) {
			this->grow(m_growSize);
			m_growSize <<= 1;
		}

		T * first = m_objects.back();
		m_objects.pop_back();
		return first;
	}

	// 回收一个对象
	void deallocate(T* obj)
	{
		lock_guard_t<MutexType> lock(m_mutex);

		// 		for (size_t i = 0; i < m_objects.size(); i++) {
		// 			if (m_objects[i] == obj) {
		// 				return;
		// 			}
		// 		}
		//
		// 		for(typename ObjectQueue::iterator itr = m_objects.begin(); itr != m_objects.end(); ++itr) {
		// 			if (*itr == obj) {
		// 				return;
		// 			}
		// 		}


		//对于在内存池中的对象数据，当超过一定空闲个数时释放，防止内存出现暴涨
		if (m_objects.size() > (uint32)(m_initSize + m_growSize)) {
			// 			T * first = m_objects.front();
			::operator delete (obj);
			//
			// 			m_objects[0] = obj;
			// 			m_totalSize--;
		} else {
			m_objects.push_back(obj);
		}
	}

	// 一次性扩张出count个对象
	void grow(int count)
	{
		//注意：外部应加锁
		for (int i = 0; i < count ; i++) {
			T *obj = static_cast<T*>(::operator new (sizeof(T)));
			m_objects.push_back(obj);
		}

		m_totalSize += count;
	}

public:
	// 当前总对象数 = 被申请出去的对象数 + 空闲对象数
	int	m_totalSize;

	int	m_growSize;
	int	m_initSize;

private:
	ObjectQueue m_objects;
	MutexType m_mutex;
};

#endif //_objectpool_h_