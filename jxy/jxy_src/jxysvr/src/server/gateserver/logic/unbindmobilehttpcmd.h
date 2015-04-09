

#ifndef _UN_BIND_MOBILE_HTTP_CMD_H_
#define _UN_BIND_MOBILE_HTTP_CMD_H_

#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protoclils.h"
#include <algorithm/sdmd5.h>



struct SUnBindInfo
{
    SUnBindInfo()
    {
        dwUserID = 0;
    }
    UINT32           dwUserID; //用户ID
};


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CUnBindMobileHttpCMD : public CBaseHttpCMD
{
public:
    CUnBindMobileHttpCMD();
    ~CUnBindMobileHttpCMD();

	virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_UNBIND_MOBILE;
    }
    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);

private:
    VOID	ProcessResult();
    //处理认证消息
    BOOL	OnUnBindMobileAck (UINT32 dwTransID, UINT16 wErrCode);

    BOOL	OnUnBindMobileAck (UINT32 dwTransID, UINT16 wErrCode, UINT16 wCountDown);

private:
    SUnBindInfo stUnBindInfo;

};

#endif //#ifndef _BASEDBCMD_H_

