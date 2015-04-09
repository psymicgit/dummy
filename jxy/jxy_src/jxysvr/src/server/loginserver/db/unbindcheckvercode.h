

#ifndef _UNBIND_CHECK_VER_CODE_CMD_H_
#define _UNBIND_CHECK_VER_CODE_CMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclils.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CUnBindCheckVerCodeCMD : public CBaseDBCMD
{
public:
    CUnBindCheckVerCodeCMD();
    ~CUnBindCheckVerCodeCMD();

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
        return EDBCT_UNBINDCHECKVERCODE ;
    }

protected:
    BOOL CheckVerCode(SGDP::ISDDBConnection* poDBConn, PKT_CLILS_UNBIND_CHECK_VER_CODE_REQ* pstReq);
private:
    PKT_CLILS_UNBIND_CHECK_VER_CODE_ACK m_stAck;
};

#endif //#ifndef _LOGINCMD_H_

