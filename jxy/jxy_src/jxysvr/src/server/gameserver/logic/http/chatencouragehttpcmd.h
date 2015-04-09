

#ifndef _CHAT_ENCOURAGE_HTTP_CMD_H_
#define _CHAT_ENCOURAGE_HTTP_CMD_H_

#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protocligs.h"
#include <algorithm/sdmd5.h>


class CPlayer;
/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CChatEncourageHttpCMD : public CBaseHttpCMD
{
public:
    CChatEncourageHttpCMD();
    ~CChatEncourageHttpCMD();

    virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_CHATENCOURAGE;
    }
    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);

protected:
	BOOL ProcessResult(CPlayer *poPlayer);
private:
    PKT_CLIGS_CHAT_REQ stReq;
	PKT_CLIGS_CHAT_ACK stAck;

};

#endif //#ifndef _BASEDBCMD_H_

