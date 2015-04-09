/******************************************************************************
		


******************************************************************************/


#ifndef SDSHMEM_H
#define SDSHMEM_H
/**
* @file sdshmem.h

* @brief 共享内存工具
*
**/
#include <string>
#include "sdtype.h"

namespace SGDP
{
    /**
    * @defgroup groupsharememory 共享内存
    * @ingroup  SGDP
    * @{
    */

    /**
    * @brief 共享内存的结构体
    *
    */
    struct SShmem
    {
		SShmem()
		{
			Init();
		}
		void Init()
		{
			handle = NULL;
			mem = NULL;
		}
        SDHANDLE    handle;		/**<文件对应的handle*/
        VOID *      mem;		/**<共享内存的起始地址*/
        std::string file;		/**<共享内存对应的文件路径*/
    };

    /**
    * @brief
    * 打开一个共享内存
    * @param pszFileName : 共享内存映射文件
    * @param bCreate : 如果没有该共享内存时，是否创建共享内存
    * @param size : 共享内存大小
    * @return 共享内存的结构体
    */
    SShmem  SDOpenShmem( const CHAR * pszFileName, BOOL bCreate = FALSE, UINT64 size = 0 );

    /**
    * @brief
    * 创建一个共享内存
    * @param pszFileName : 共享内存映射文件
    * @param size : 共享内存大小
    * @return 共享内存的结构体
    */
    SShmem  SDCreateShmem( const CHAR * pszFileName, UINT64 size );

    /**
    * @brief
    * 创建一个共享内存
    * @param file : 共享内存映射文件
    * @param size : 共享内存大小
    * @return 共享内存的结构体
    */
    BOOL  SDShmemExist( const CHAR * pszFileName );

	/**
    * @brief
    * 创建一个共享内存
    * @param file : 共享内存映射文件
    * @param size : 共享内存大小
    * @return 共享内存的结构体
    */
	BOOL SDAttachShm(SShmem *shm, const CHAR *file);

	/**
    * @brief
    * 关闭一个共享内存
    * 在windows操作系统中，如果打开次数等于关闭次数，内存将被销毁
    * 在linux操作系统中， 则直接被销毁
    * @param shm : 共享内存的结构体
    * @return VOID
    */
    VOID  SDUnAttachShm( SShmem * shm );

    /**
    * @brief
    * 关闭一个共享内存
    * 在windows操作系统中，如果打开次数等于关闭次数，内存将被销毁
    * 在linux操作系统中， 则直接被销毁
    * @param shm : 共享内存的结构体
    * @return VOID
    */
    VOID  SDCloseShmem( SShmem * shm );

	/**
    * @brief
    * 关闭一个共享内存
    * 仅用于linux， 共享内存直接被销毁
    * @return VOID
    */
    VOID  SDCloseShmemByPath(CHAR* pszFilePath, UINT32 dwSize);

    /**
    * @brief
    * 这是一个共享内存类
    */
    class CSDShmem
    {
    public:
        CSDShmem();

        /**
        * @brief
        * 打开一个共享内存
        * @param pszFileName : 文件路径
        * @return 指向共享内存的指针
        */
        VOID *  Open( const CHAR * pszFileName );

        /**
        * @brief
        * 创建一个共享内存
        * @param pszFileName : 文件路径
        * @param size : 文件大小
        * @return 指向共享内存的指针
        */
        VOID *  Create( const CHAR * pszFileName, UINT64 size );

        /**
        * @brief
        * 关闭共享内存
        * @return VOID
        */
        VOID  Close();

        /**
        * @brief
        * 获取共享内存句柄
        * @return 共享内存句柄
        */
        SDHANDLE GetHandle();

    private:
        SShmem m_shm;
    };

    /** @} */
}//

#endif


