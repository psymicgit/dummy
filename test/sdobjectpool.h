/******************************************************************************
		

sdobjectpool.h - 对象池类，分配和管理对象，可以支持多线程模式

******************************************************************************/


#ifndef SDOBJECTPOOL_H
#define SDOBJECTPOOL_H
/**
* @file sdobjectpool.h

* @brief 对象池类
*
**/
#include "sdobject_allocator.h"

#define SDNew               new

namespace SGDP
{
    /**
    * @defgroup groupmemorypool 内存池(包括对象池)
    * @ingroup  SGDP
    * @{
    */

    /**
    *@brief 对象池类
    */
    template <typename _Tp, typename _Alloc=ObjectAllocator<_Tp> >
    class CSDObjectPool
    {
    public:

		/**
		* @brief 构造函数，分配给定大小的内存
		* @param dwInitCount : 内存的初始大小
		* @param dwGrouCount : 内存块的个数
		*/
        CSDObjectPool(UINT32 dwInitCount = 0, UINT32 dwGrouCount = 1)
            :m_allocator(dwInitCount,dwGrouCount)
        {
        }

        /**
        * @brief
        * 分配一个对象, 如果对象构造函数没有参数，使用此接口
        * @return 返回对象的指针，如果失败返回NULL
        */
        inline _Tp*   Alloc()
        {
            return SDNew (m_allocator.allocate())_Tp;
        }

        /**
        * @brief
        * 分配一个对象, 对象的构造函数包含一个输入参数
        * @return 返回对象的指针，如果失败返回NULL
        */
        template<typename P1>
        _Tp *  Alloc(P1 p1)
        {
            return SDNew (m_allocator.allocate())_Tp(p1);
        }

		 /**
        * @brief
        * 分配一个对象, 对象的构造函数包含2个输入参数
        * @return 返回对象的指针，如果失败返回NULL
        */
        template<typename P1,typename P2>
        _Tp*  Alloc(P1 p1,P2 p2)
        {
            return SDNew (m_allocator.allocate())_Tp(p1,p2);
        }

		 /**
        * @brief
        * 分配一个对象, 对象的构造函数包含3个输入参数
        * @return 返回对象的指针，如果失败返回NULL
        */
        template<typename P1,typename P2,typename P3>
        _Tp*  Alloc(P1 p1,P2 p2,P3 p3)
        {
            return SDNew (m_allocator.allocate())_Tp(p1,p2,p3);
        }

		 /**
        * @brief
        * 分配一个对象, 对象的构造函数包含4个输入参数
        * @return 返回对象的指针，如果失败返回NULL
        */
        template<typename P1,typename P2,typename P3,typename P4>
        _Tp*  Alloc(P1 p1,P2 p2,P3 p3,P4 p4)
        {
            return SDNew (m_allocator.allocate())_Tp(p1,p2,p3,p4);
        }

		 /**
        * @brief
        * 分配一个对象, 对象的构造函数包含5个输入参数
        * @return 返回对象的指针，如果失败返回NULL
        */
        template<typename P1,typename P2,typename P3,typename P4,typename P5>
        _Tp*  Alloc(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5)
        {
            return SDNew (m_allocator.allocate())_Tp(p1,p2,p3,p4,p5);
        }

        /**
        * @brief
        * 归还一个对象
        * @param pObj : 对象指针
        * @return void
        */
        void  Free(_Tp * p)
        {
            p->~_Tp();
            m_allocator.deallocate(p);
        }

        /**
        * @brief 获取对象池
        * @return 对象池
        */
        _Alloc &  GetAllocator()
        {
            return m_allocator;
        }

    private:
        _Alloc m_allocator;

    };

}

using namespace SGDP;

#endif //

