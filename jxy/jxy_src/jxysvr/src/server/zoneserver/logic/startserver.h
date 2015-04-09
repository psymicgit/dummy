

#ifndef _START_SERVER_CMD_H_
#define _START_SERVER_CMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclils.h"


typedef struct _tagSTART_TIME
{
	UINT16		wZoneID;
	CHAR			aszTime[256];
	_tagSTART_TIME()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(_tagSTART_TIME));
	}
} tSTART_TIME;


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CStartServerCMD : public CBaseDBCMD
{
public:
    CStartServerCMD();
    ~CStartServerCMD();

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
    virtual EDBCmdType GetDBCmdType()
    {
        return EDBCT_STARTSERVICE ;
    }

protected:
    BOOL Start(SGDP::ISDDBConnection* poDBConn, tSTART_TIME* pstReq);

private:
	UINT16 m_wErrCode;
};

#endif //#ifndef _LOGINCMD_H_

