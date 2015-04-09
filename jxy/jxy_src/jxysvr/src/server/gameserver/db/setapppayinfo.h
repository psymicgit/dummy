#ifndef _APP_PAY_MGR_H_
#define _APP_PAY_MGR_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include <logic/player/playergetdatamgr.h>

//玩家的充值信息，目前为appstore充值用
struct SPayInfo
{
	SPayInfo()
	{
		dwPlayerID=0;
		dwUserID=0;
		dwZoneID=0;
		dwPlayerLevel=0;
		dwQueryPlayerID=0;
		strOrderID = "";
		strTableName = "";
		strDeviceID = "";
		strReceipt = "";

	}

	UINT32 dwPlayerID; //玩家ID
	UINT32 dwQueryPlayerID; //请求的玩家ID
	string strOrderID; //订单号
	UINT32 dwUserID; //用户ID
	UINT32 dwZoneID; //区服ID
	string strSource; //源，来自何处
	UINT32 dwPlayerLevel; //玩家等级
	string strTableName; //充值表的名字
	string strDeviceID;//设备号
	string strReceipt; //app返回给客户端的receipt，发给苹果服务器进行验证
};

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CSetAppPayInfo : public CBaseDBCMD
{
public:
	CSetAppPayInfo();
	virtual ~CSetAppPayInfo();
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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_SETPAYINFO ; }

private:
	BOOL SaveAppPayInfo(SGDP::ISDDBConnection* poDBConn);
private:
	SPayInfo m_stPayInfo; //充值信息
	BOOL m_bInsertSuccess; //是否成功
	BOOL m_bRepeat; //是否重复设置
	UINT32 m_dwAlreadyPayGoldValue; //充值元宝数,元宝为单位
};



#endif