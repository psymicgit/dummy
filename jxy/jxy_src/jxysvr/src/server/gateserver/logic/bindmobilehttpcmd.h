

#ifndef _BIND_MOBILE_HTTP_CMD_H_
#define _BIND_MOBILE_HTTP_CMD_H_

#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protoclils.h"
#include <algorithm/sdmd5.h>



struct SBindInfo
{
    SBindInfo()
    {
        dwUserID = 0;
        memset(abyMObile,0,sizeof(abyMObile));
    }
    UINT32           dwUserID; //用户ID
    UINT8            abyMObile[MAX_MOBILE_LENGTH+1]; //用户手机号

};


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CBindMobileHttpCMD : public CBaseHttpCMD
{
public:
    CBindMobileHttpCMD();
    ~CBindMobileHttpCMD();

	virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_BIND_MOBILE;
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
    BOOL	OnBindMobileAck (UINT32 dwTransID, UINT16 wErrCode);

    BOOL	OnBindMobileAck (UINT32 dwTransID, UINT16 wErrCode, UINT16 wCountDown);

private:
    SBindInfo stBindInfo;

};

#endif //#ifndef _BASEDBCMD_H_

