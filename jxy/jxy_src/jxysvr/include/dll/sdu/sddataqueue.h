


#ifndef SDDATAQUEUE_H
#define SDDATAQUEUE_H
/**
* @file sdthrdqueue.h

* @brief 循环数据队列
*
**/
#include "sdtype.h"

namespace SGDP
{
    /**
    * @defgroup grouploopbuffer 定长的循环数据队列
    * @ingroup  SGDP
    * @{
    */

    /**
    *@brief 循环指针队列，存取为缓冲区指针，支持单线程读写
    */
    class CSDDataQueue
    {
    public:
        CSDDataQueue();
        ~CSDDataQueue();

        /**
        * @brief
        * 初始化循环缓冲区
        * @param nSize ：分配的长度，实际长度为分配的长度加1
        * @return 成功返回TRUE，失败返回FALSE
        */
        BOOL  Init(INT32 nSize);

        /**
        * @brief
        * 将使用完毕的Buffer插入循环缓冲区的结尾
        * @param ptr : 指向Buffer的地址
        * @return 如果循环缓冲区拥有的大小大于nLen，返回TRUE，否则返回FALSE
        * @remark 此函数不是线程安全的
        */
        BOOL  PushBack(VOID *ptr);

        /**
        * @brief
        * 获取新的Buffer
        * @return 指向Buffer的地址，如果失败，返回NULL
        * @remark 此函数不是线程安全的
        */
        VOID*  PopFront();

    protected:
        VOID            **m_pArray;	/**<存取地址缓冲区*/
        INT32			m_nHead;	/**<缓冲区头部*/
        INT32			m_nTail;	/**<缓冲区尾部*/
        INT32			m_nSize;	/**<缓冲区大小*/
    };

    /** @} */
}

#endif

