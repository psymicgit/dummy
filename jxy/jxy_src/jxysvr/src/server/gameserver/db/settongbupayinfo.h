#ifndef _TONGBU_PAY_MGR_H_
#define _TONGBU_PAY_MGR_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include <logic/player/playergetdatamgr.h>

//玩家的同步充值信息
struct STongBuPayInfo
{
	STongBuPayInfo()
	{
		dwPlayerID=0;
		dwUserID=0;
		dwZoneID=0;
		dwPlayerLevel=0;
		bSuccess = FALSE;
	}

	UINT32 dwPlayerID; //玩家ID
	string strOrderID; //订单号
	UINT32 dwUserID; //用户ID
	UINT32 dwZoneID; //区服ID
	UINT32 dwPlayerLevel; //玩家等级
	string strSource; //源，来自何处
	string strTableName; //充值表的名字
	BOOL   bSuccess;
};

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CSetTongBuPayInfo : public CBaseDBCMD
{
public:
	CSetTongBuPayInfo();
	virtual ~CSetTongBuPayInfo();
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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_SETTONGBUPAYINFO ; }

private:
	BOOL SaveTongBuPayInfo(SGDP::ISDDBConnection* poDBConn);
private:
	STongBuPayInfo m_stPayInfo; //充值信息
};



#endif