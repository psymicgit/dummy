

#ifndef _APP_PAY_HTTP_CMD_H_
#define _APP_PAY_HTTP_CMD_H_

#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protoclils.h"
#include <algorithm/sdmd5.h>



struct SAppPayInfo
{
	SAppPayInfo()
	{
		dwPlayerID = 0;
		dwQueryPlayerID = 0;
		strOrderID = "";
		strReceipt = "";
		nQueryCnt = 0;
		bBuyEnv = TRUE;
	}

	UINT32	dwPlayerID; //实际订单对应的玩家ID
	UINT32	dwQueryPlayerID; //请求充值的玩家账号ID(用于通知充值成功)
	string	strOrderID; //订单号
	string	strReceipt; //传输的数据
	INT32	nQueryCnt;
	BOOL		bBuyEnv;
};



/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CAppPayHttpCMD : public CBaseHttpCMD
{
public:
    CAppPayHttpCMD();
    ~CAppPayHttpCMD();

	virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_APP_PAY;
    }
    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);

	VOID ProcessPayResult();

	VOID SendErrorResult( INT32 dwPlayerID, INT32 nErrorCode, string strOrderID);

private:
	void Try( UINT32 dwPlayerID, string strReceipt, UINT32 dwQueryPlayerID, string strOrderID, INT32 nQueryCnt, BOOL bBuyEnv);
private:
    SAppPayInfo m_stAppPayInfo;

};

#endif //#ifndef _BASEDBCMD_H_

