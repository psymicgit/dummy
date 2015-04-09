#ifndef _SET_MY_PAY_PROCESSED_H_
#define _SET_MY_PAY_PROCESSED_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include <logic/player/playergetdatamgr.h>


//自己订单处理完毕
struct SMyPayProcessed
{
	SMyPayProcessed()
	{
		wZoneID = 0;
		dwPlayerID=0;
		dwGivenGold=0;
		wErrCode = 0;
		dwRMB = 0;
		dwSessionID = 0;
		bySandBoxFlag = 0;
	}
	UINT16 wZoneID;
	UINT32 dwPlayerID;	//玩家ID
	UINT32 dwGivenGold;	//元宝数
	string strOrderID;	
	string strComment;
	UINT32 dwRMB;			//单位为分
	UINT8  bySandBoxFlag;
	UINT32 dwSessionID;
	UINT16 wErrCode;
};



/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CSetMyPayProcessed : public CBaseDBCMD
{
public:
	CSetMyPayProcessed();
	virtual ~CSetMyPayProcessed();
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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_SETMYPAYPPROCESSED ; }

private:
	BOOL SetPayProcessed(SGDP::ISDDBConnection* poDBConn);
private:
	SMyPayProcessed m_stMyPayProcessed; //订单处理结果信息
	BOOL m_bSuccess; //是否成功
};


#endif