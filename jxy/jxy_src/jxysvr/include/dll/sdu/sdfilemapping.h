/******************************************************************************
		


******************************************************************************/

#ifndef SD_FILEMAPPING_H
#define SD_FILEMAPPING_H
/**
* @file sdfilemapping.h

* @brief 文件映射类
*
**/
#include <string>

#if (defined(WIN32) || defined(WIN64))
#include <Windows.h>
#else
#include <sys/types.h>
#include <sys/mman.h>
#include <stddef.h>
#include <unistd.h>
#endif

#include "sdtype.h"

namespace SGDP
{
    /**
    * @defgroup groupfilemapping 文件映射
    * @ingroup  SGDP
    * @{
    */


    /**
    * @brief 文件映射结构体
    *
    */
    struct SFileMapping
    {
        VOID* mem;				/**<共享内存的起始地址*/
        VOID * pos;            /** 读取内存映射的位置*/
        UINT32 size;			/**<映射内存的大小*/
        SDHANDLE maphandle;		/**<文件对应的handle*/
    };

    /**
    * @brief
    * 映射一个文件段到内存中
    * @param stFileMapping : 文件映射结构体
    * @param pszFileName : 文件路径
    * @param dwBeg : 映射起始的文件位置，此位置必须是文件分配粒度的整数倍
    * @param dwSize : 内存大小，如果为0，整个文件都被映射
    * @return TRUE为映射成功，FALSE为映射失败
    */
    BOOL  SDFileMapping(SFileMapping& stFileMapping, const TCHAR* pszFileName, UINT32 dwBeg =0 , UINT32 dwSize = 0 );

    /**
    * @brief
    * 取消文件与内存的映射关系
    * @param stFileMapping : 文件映射结构体
    * @return VOID
    */
    VOID  SDFileUnMapping(SFileMapping& stFileMapping);

    /**
    * @brief
    * 将内存中的内容写入内存映射的文件中
    * @param pMem : 指向写入映射文件的内存起始地址
    * @param len : 需要写入的大小，单位byte
    * @return VOID
    */
    VOID  SDFileFlushMapping(VOID* pMem, UINT32 len);

    /** @} */
}//

#endif

