

#ifndef _GETPLAYERDATACMD_H_
#define _GETPLAYERDATACMD_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include <logic/player/playergetdatamgr.h>


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CGetPlayerDataCMD : public CBaseDBCMD
{
public:
	CGetPlayerDataCMD();
	~CGetPlayerDataCMD();

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


	virtual VOID SetUserData(VOID* pUserData);

public:
	virtual EDBCmdType GetDBCmdType() { return EDBCT_GET1 ; }

	static BOOL GetData(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData, DT_PLAYER_PVP_DATA& stPlayerPvpData, UINT16& wErrCode);

	//如果有发现数据存数错误，备份的SQL文件，则执行SQL，成功则返回SUCCESS,否则返回拒绝登录
	static INT32 CheckAndProcessBackupSql( SGDP::ISDDBConnection *poDBConn, UINT32 dwPlayerID );
protected:


	static BOOL GetPlayerBase(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData);
	static BOOL GetPlayerTodayConsume(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData);
	static BOOL GetPlayerExt(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData);
	static BOOL GetBag(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData);
	static BOOL GetBuild(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData);
	static BOOL GetHeroBase(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData);
	static BOOL GetHeroExt(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData);
	static BOOL GetInstance(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData);
	static BOOL GetTask(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData);
	static BOOL GetPvp(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_PVP_DATA& stPlayerPvpData);
	static BOOL GetRelationData(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData);
	static BOOL GetEvenData(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData);
	static BOOL GetSendFlowerLog(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData);

private:
	SGetPlayerData* m_pstGetPlayerData;
};

#endif //#ifndef _GETPLAYERDATACMD_H_

