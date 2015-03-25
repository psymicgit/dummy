/********************************************************************
            Copyright (c) 2009, 欢乐连线工作室
                   All rights reserved
         
    创建日期：  2009年12月10日 10时44分
    文件名称：  PoolBase.h
    说    明：  内存池化类的内存池基类实现   
    
    当前版本：  1.00
    作    者：  Conserlin
    概    述：  创建 
	使用说明:   此类是一个抽象类，不可实例化，它封装了池的实现和对T的new和delete方法进行重载.
	            任何想池化的类一旦派生(继承)至此类，就具有池的功能。此类是一个模板类，有三个
				模板参数，分别是: T, BatchSize, Alloc，它们功能定义是：
				1. T
				   要池化的子类
			    2. BatchSize
				   每次申请池的大小，即开辟多少个T内存空间
			    3. Alloc
				   池分配器, 按一定的规格实现，用户也可自己实现规格化的分配器

			   使用例子:
			       class SubClass : public PoolBase<SubClass, 100> 
				   {
				   ......
				   }

				   ...
				   // 从池中申请内存块
				   SubClass *pSubClass = new SubClass();
				   ...
				   // 将内存块返还给池
				   delete pSubClass;


			   ps: 此类不支持堆数组的申请，如 new T[12]，它将导致编译失败。具体内容请见文档。
			   　　	            

*********************************************************************/
#pragma once
#include <vector>
#include "simpleallocator.h"

#define AllowPoolBase
#ifndef AllowPoolBase
template <
typename T,                                               // 内存分配的类型
size_t BatchSize                                         // 每次开辟多少个T内存空间
>	
struct PoolBase{};
#else
template <
		  typename T,                                               // 内存分配的类型
          size_t BatchSize,                                         // 每次开辟多少个T内存空间
	      template <typename, size_t> class Alloc = SimpleAllocator // 分配器，默认为SimpleAllocator
         >	
class PoolBase
{
	typedef Alloc<T, BatchSize> MyAlloc;
public:
	virtual ~PoolBase() {
	}
	
	// 重载T的 new 和 delete
	void* operator new(size_t s) {
		//T *temp = myAlloc.allocateBlock();
		//return ::new (temp)T();
		return myAlloc.allocateBlock();
	}

	void operator delete(void* p) {
		// ((T*)p)->~T();
		myAlloc.releaseBlock((T*)p);
	}
		
private:
	// 不支持数组申请
	void* operator new[](size_t s) {
		return NULL;
	}

	void operator delete[](void* p) {		
	};

private:
	// 针对T类型所有实例的单一池分配
	static MyAlloc myAlloc;
};

template <typename T, size_t BatchSize, template <typename, size_t> class Alloc>
typename PoolBase<T, BatchSize, Alloc>::MyAlloc PoolBase<T, BatchSize, Alloc>::myAlloc;
#endif // AllowPoolBase
