

#ifndef _PLAYUNBINDCMD_H_
#define _PLAYUNBINDCMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include <protocol/server/protogsdb.h>


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CPlayUnBindCMD : public CBaseDBCMD
{
public:
    CPlayUnBindCMD();
    ~CPlayUnBindCMD();

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
        return EDBCT_UNBIND ;
    }

protected:
    BOOL UnBind(SGDP::ISDDBConnection* poDBConn, UINT8  byAuthType, UINT32 dwPlayID, TCHAR* pszUserName);

private:
    tagPKT_CLILS_UNBIND_ACK	m_stAck;
};

#endif //#ifndef _LOGINCMD_H_

