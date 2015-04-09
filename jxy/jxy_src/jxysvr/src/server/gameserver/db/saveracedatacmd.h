

#ifndef _SAVEPLAYERSRACEDATACMD_H_
#define _SAVEPLAYERSRACEDATACMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include <logic/player/playersavemgr.h>

using namespace  SGDP;


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CSavePlayersRaceDataCMD : public CBaseDBCMD
{
public:
	CSavePlayersRaceDataCMD();
	~CSavePlayersRaceDataCMD();

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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_SAVERACE ; }

protected:
	///////////////////////////////////////////////////////////////////////////////////////////////
	BOOL	SaveData(ISDDBConnection* poDBConn);//保存
	
private:
	

	CPlayersRaceSaveData* m_poPlayersRaceSaveInfo;
};

#endif //#ifndef _GETPLAYERDATACMD_H_

