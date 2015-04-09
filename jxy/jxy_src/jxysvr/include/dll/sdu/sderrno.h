
#ifndef SDERRNO_H
#define SDERRNO_H
/**
* @file sderrno.h
* @brief 错误代码
*
**/
#include "sdtype.h"

#if (defined(WIN32) || defined(WIN64))
#else
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sdstring.h>
#endif
/**
* @brief Type for specifying an error or status code.
*/
namespace SGDP
{
    /**
    * @defgroup grouperror errno操作
    * @ingroup  SGDP
    * @{
    */

    typedef INT32  SDErrNo;

#if (defined(WIN32) || defined(WIN64))
    /**
    * @brief
    * 设置errno变量
    * @param dwErrCode : 传入的error code
    * @return VOID
    */
    inline VOID  SDSetLastError(DWORD dwErrCode)
    {
        SetLastError(dwErrCode);
    }

    /**
    * @brief
    * 获取errno变量
    * @return 返回errno code
    */
    inline SDErrNo  SDGetLastError()
    {
        return GetLastError();
    }

    /**
    * @brief
    * 将error变量转换成字符串
    * @param errnum : 传入的error code
    * @return error code对应的字符串
    */
    inline TCHAR*  SDStrError(INT32 errnum)
    {
		return _tcserror(errnum);
    }

    /**
    * @brief
    * 得到错误情况下的字符串描述
    * @return 错误的字符串描述
    */
    inline TCHAR*  SDLastStrError()
    {
		return _tcserror(NULL);
    }
#else
    /**
    * @brief
    * 设置error变量
    * @param dwErrCode : 传入的error code
    * @return VOID
    */
    inline VOID  SDSetLastError(INT32 dwErrCode)
    {
        errno = dwErrCode;
    }

    /**
    * @brief
    * 获取error变量
    * @return 返回error code
    */
    inline SDErrNo  SDGetLastError()
    {
        return errno;
    }

    /**
    * @brief
    * 将error变量转换成字符串
    * @param errnum : 传入的error code
    * @return errno code对应的字符串
    */
    inline TCHAR*  SDStrError(INT32 errnum)
    {
        CHAR* szError = strerror(errnum);
        if (szError)
        {
            return (TCHAR*)_SDTA2T(szError);
        }
        return _SDT("");
    }

    /**
    * @brief
    * 得到错误情况下的字符串描述
    * @return 错误的字符串描述
    */
    inline TCHAR*   SDLastStrError()
    {
        return SDStrError(errno);
    }
#endif // WIN32 


} // namespace SGDP

#endif // 

