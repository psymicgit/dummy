

#ifndef _BIND_CHECK_VER_CODE_HTTP_CMD_H_
#define _BIND_CHECK_VER_CODE_HTTP_CMD_H_

#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protoclils.h"
#include <algorithm/sdmd5.h>



struct SBindCheckVerCodeInfo
{
    SBindCheckVerCodeInfo()
    {
        dwUserID = 0;
        memset(abyVerCode,0,sizeof(abyVerCode));
    }
    UINT32           dwUserID; //用户ID
    UINT8            abyVerCode[MAX_MOBILE_LENGTH+1]; //验证码
};


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CBindCheckVerCodeHttpCMD : public CBaseHttpCMD
{
public:
    CBindCheckVerCodeHttpCMD();
    ~CBindCheckVerCodeHttpCMD();

	virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_BIND_CHECK_VER_CODE;
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
    BOOL	OnBindCheckVerCodeAck (UINT32 dwTransID, UINT16 wErrCode);

    BOOL	OnBindCheckVerCodeAck (UINT32 dwTransID, UINT16 wErrCode, TCHAR aszMobile[MAX_MOBILE_LENGTH]);

private:
    SBindCheckVerCodeInfo stBindCheckVerCodeInfo;

};

#endif //#ifndef _BASEDBCMD_H_

