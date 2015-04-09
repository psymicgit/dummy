#ifndef _SET_APP_PAY_PROCESSED_H_
#define _SET_APP_PAY_PROCESSED_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include <logic/player/playergetdatamgr.h>
#include "thirdparty/jsoncpp-src/include/json/json.h"

#define STR_GLOBAL_TABLE_NAME "global_amount"

//苹果订单处理完毕
struct SAppPayProcessed
{
	SAppPayProcessed()
	{
		dwPlayerID=0;
		dwGold=0;
		dwErrorCode=0;
		dwQueryPlayer=0;
		bBuyEnv = 0;
		strTableName = "";
		strOrderID = "";
		dwRMB = 0;
	}
	UINT32	dwPlayerID;		//玩家ID
	UINT32	dwGold;			//充值数
	UINT32	dwErrorCode;		//苹果返回的错误码
	string	strTableName;		//该服对应的数据库表名
	string	strOrderID;		//订单号
	UINT32	dwQueryPlayer;	//请求的玩家
	BOOL		bBuyEnv;			//发送的地址信息
	UINT32	dwRMB;			//（以分为单位）
};

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CSetAppPayProcessed : public CBaseDBCMD
{
public:
	CSetAppPayProcessed();
	virtual ~CSetAppPayProcessed();
	/**
	* @brief 异步执行SQL命令
	* @poDBConn 异步执行SQL命令的Connection连接
	* @return VOID
	*/		
	VOID SDAPI OnExecuteSql(SGDP::ISDDBConnection* poDBConn);

	/**
	* @brief 同步执行SQL命令相关的逻辑部分
	* @return VOID
	*/
    VOID SDAPI OnExecuted();

	//设置数据
	virtual VOID SetUserData(VOID* pUserData);

public:
	virtual EDBCmdType GetDBCmdType() { return EDBCT_SETPAYPPROCESSED ; }

private:
	BOOL SetPayProcessed(SGDP::ISDDBConnection* poDBConn);
private:
	SAppPayProcessed m_stAppPayProcessed; //订单处理结果信息
	BOOL m_bSuccess; //是否成功
};


#endif