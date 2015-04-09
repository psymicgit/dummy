#ifndef _GET_PLAYER_CONSUME_H_
#define _GET_PLAYER_CONSUME_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include "protocol/server/protocommondata.h"


typedef struct _tagGetPlayerConsume
{
    UINT32 dwPlayerID;
    string strStartDate;
    string strEndDate;
} DT_GetPlayerConsume;

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CGetPlayerConsume : public CBaseDBCMD
{
public:
    CGetPlayerConsume();
    virtual ~CGetPlayerConsume();
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
    virtual EDBCmdType GetDBCmdType()
    {
        return EDBCT_GET_PLAYER_CONSUME ;
    }

private:
	DT_GetPlayerConsume					m_stGetPlayerConsume;
    DT_PLAYER_TODAY_CONSUME_DATA			m_stDT_PLAYER_TODAY_CONSUME_DATA;

};

#endif