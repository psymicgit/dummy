

#ifndef _LOGINEX_HTTP_CMD_H_
#define _LOGINEX_HTTP_CMD_H_

#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protoclils.h"
#include <algorithm/sdmd5.h>



struct SLoginInfoEx
{
    SLoginInfoEx()
    {
		Init();
    }
	void Init()
	{
		strIp = "";
		memset(&stReq, 0x00, sizeof(PKT_CLILS_LOGIN_EX_REQ));
	}
	PKT_CLILS_LOGIN_EX_REQ		stReq;
	string					strIp;

};


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CLoginExHttpCMD : public CBaseHttpCMD
{
public:
    CLoginExHttpCMD();
    ~CLoginExHttpCMD();

	virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_LOGINEX;
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
    BOOL	OnLoginExAck_Failed (UINT16 wErrCode);

    BOOL	OnLoginExAck_Success (UINT32 dwUserID, UINT8 byDaiChong);

	void GetPacketBuffer();

private:
    SLoginInfoEx			stLoginInfo;
	PKT_CLILS_LOGIN_EX_ACK	stAck;

    // 玩家原来所在的区（即玩家创建角色时所在的区，这个区ID是用于不会变的）
    UINT16 m_wOriZoneID;
};

#endif //#ifndef _BASEDBCMD_H_

