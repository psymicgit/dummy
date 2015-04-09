#ifndef _RECORD_PLAYER_LOGINOFF_H
#define _RECORD_PLAYER_LOGINOFF_H

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"

typedef struct tagPlayerLogin
{
    tagPlayerLogin()
    {
        dwPlayerID = 0;
        bLogin = FALSE;
    }

    UINT32 dwPlayerID;
    BOOL bLogin;
	string strIP;
} SPlayerLogin;

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CRecordPlayerLoginoff: public CBaseDBCMD
{
public:
    CRecordPlayerLoginoff();
    ~CRecordPlayerLoginoff();
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
    virtual EDBCmdType GetDBCmdType()
    {
        return EDBCT_LOGINOFFINFO;
    }
private:
    SPlayerLogin m_stLogin; //登入登出信息
    string m_strSql; //执行的SQL语句的记录
};

#endif