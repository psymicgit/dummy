

#ifndef _GET_OPRATEINGACTIVITY_CMD_H_
#define _GET_OPRATEINGACTIVITY_CMD_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include <protocol/server/protocligs.h>
using namespace std;


//运营活动配置
struct SOprateingActivityProp
{
	DT_OPERATING_ACTIVITY_DATA stProtData;//协议信息
	UINT64	qwStartTime;				//开启时间
	UINT64	qwEndTime;					//结束时间
	BOOL	bOpenSoulAty;				// 开启战魂活动
	//UINT8	byTowerIdx;					//城镇Idx
	//UINT8	byInstanceIdx;				//副本Idx
	//UINT8	byInstanceIdx2;				//副本Idx2
	//UINT8	byInstanceIdx3;				//副本Idx3

};

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CGetOprateingActivityCMD : public CBaseDBCMD
{
public:
	CGetOprateingActivityCMD();
	~CGetOprateingActivityCMD();

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

public:
	virtual EDBCmdType GetDBCmdType() { return EDBCT_GETOPRATEINGACTIVITY ; }

	virtual VOID SetUserData(VOID* pUserData);
protected:


private:
	SOprateingActivityProp		m_stOprateingActivityProp;
	UINT16						m_wZoneID;
};

#endif //#ifndef _GETPLAYERDATACMD_H_

