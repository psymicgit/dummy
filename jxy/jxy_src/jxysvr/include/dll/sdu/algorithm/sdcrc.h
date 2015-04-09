
#ifndef SDCRC_H
#define SDCRC_H
/**
* @file sdcrc.h

* @brief crc check
*
**/
#include "sdtype.h"
#include <string>

namespace SGDP
{
    /**
    * @defgroup groupsrc Crc校验
    * @ingroup  SGDP
    * @{
    */

    /**
    * @brief
    * 得到一块缓冲区对应的crc校验码
    * @param pBuf : 输入的缓冲区
    * @param dwLen : 缓冲区长度
    * @param dwCrcVal : 必须大于等于0，小于256，编码位
    * @return crc校验码
    */
    UINT32  SDCRC(const VOID* pBuf, UINT32 dwLen, UINT32 dwCrcVal = 0);

    /**
    * @brief
    * 计算文件内容的crc校验码,注意计算CRC时使用了二进制方式读取,
    * 如果对同一个文件的crc值不一致,先确认文件的读取方式.
    * @param pszFileName : 文件名
    * @param bFileMapping : 是否将文件映射到一块内存上
    * @return 文件内容的crc校验码，失败返回0
    */
    UINT32  SDFileCRC(const TCHAR* pszFileName, BOOL bFileMapping = TRUE);

    /** @} */
}

#endif

