

#ifndef _GET_VERSION_NOTICE_HTTP_CMD_H_
#define _GET_VERSION_NOTICE_HTTP_CMD_H_

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
class CGetVersionNoticeHttpCMD : public CBaseHttpCMD
{
public:
    CGetVersionNoticeHttpCMD();
    ~CGetVersionNoticeHttpCMD();

    virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_GET_VERSION_NOTICE;
    }
    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);

private:
    BOOL	ProcessResult();

private:
    PKT_CLIZS_GET_VERSION_NOTICE_REQ2 stReq;
	PKT_CLIZS_GET_VERSION_NOTICE_ACK2 stAck;

};

#endif //#ifndef _BASEDBCMD_H_

