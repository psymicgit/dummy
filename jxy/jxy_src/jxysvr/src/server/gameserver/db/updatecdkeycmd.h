#ifndef _UPDATE_CD_KEY_CMD_H_
#define _UPDATE_CD_KEY_CMD_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include "protocol/server/protocligs.h"


typedef struct tagUPDATE_CK_KEY
{
	tagUPDATE_CK_KEY()
	{ 
		memset(this, 0, sizeof(tagUPDATE_CK_KEY)); 
	}
	UINT64           qwActivityID; //活动类型
	TCHAR            aszSeriesNo[MAX_SERIES_NO_NUM]; //系列码
}UPDATE_CK_KEY, *pUPDATE_CK_KEY;



/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CUpdateCdKeyCmd : public CBaseDBCMD
{
public:
	CUpdateCdKeyCmd();
	virtual ~CUpdateCdKeyCmd();
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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_UPDATE_CK_KEY ; }

private:
	UINT16			m_wErrCode;
	UPDATE_CK_KEY		m_stData;
	BOOL CUpdateCdKey(SGDP::ISDDBConnection* poDBConn);

};


#endif