

#ifndef _CCREATEPLAYERCMD_H_
#define _CCREATEPLAYERCMD_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CCreatePlayerCMD : public CBaseDBCMD
{
public:
	CCreatePlayerCMD();
	~CCreatePlayerCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_CREATE ; }

protected:
	///////////////////////////////////////////////////////////////////////////////////////////////	
	BOOL CreatePalyerData(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, DT_PLAYER_NOPVP_DATA* pstPlayerData, DT_PLAYER_PVP_DATA* pstPlayerPvpData);
	
	BOOL AddPlayerBase(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, DT_PLAYER_NOPVP_DATA* pstPlayerData, DT_PLAYER_PVP_DATA* pstPlayerPvpData);	
	BOOL AddPlayerExt(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA* pstPlayerData);
	BOOL AddBag(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_BAG_EQUIP_DATA_LST& stBagEquipData, DT_BAG_GOODS_DATA_LST& stBagGoodsData, DT_BAG_JEWELRY_DATA_LST& stBagJewelryData);
	BOOL AddBuild(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_BUILD_DATA_LIST& stBuildData);
	BOOL AddHeroBase(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_HERO_DATA_LIST& stHeroData);
	BOOL AddHeroExt(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_HERO_DATA_LIST& stHeroData);
	BOOL AddInstance(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_INSTANCE_DATA_LIST& stInstanceData);
	BOOL AddTask(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_TASK_DATA_LIST& stTaskData);
	BOOL AddPvp(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_PVP_DATA& stPlayerData);

private:
	UINT32	m_dwPlayerDBID;
	DT_GSDB_CREATE_PALYER_DATA_REQ m_stReq;
	DT_GSDB_CREATE_PALYER_DATA_ACK m_stAck;
};

#endif //#ifndef _GETPLAYERDATACMD_H_

