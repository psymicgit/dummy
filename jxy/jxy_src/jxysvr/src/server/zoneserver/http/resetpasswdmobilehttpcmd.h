

#ifndef _RESETPASSWDMOBILE_HTTP_CMD_H_
#define _RESETPASSWDMOBILE_HTTP_CMD_H_

#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protoclizs.h"
#include <algorithm/sdmd5.h>






/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CResetPasswdMobileHttpCMD : public CBaseHttpCMD
{
public:
    CResetPasswdMobileHttpCMD();
    ~CResetPasswdMobileHttpCMD();

    virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_RESETPASSWDMOBILE;
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

private:
    PKT_CLIZS_RESET_PASSWD_MOBILE_REQ stReq;
	PKT_CLIZS_RESET_PASSWD_MOBILE_ACK stAck;

};

#endif //#ifndef _BASEDBCMD_H_

