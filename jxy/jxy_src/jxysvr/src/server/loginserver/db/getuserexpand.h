

#ifndef _GET_MOBILE_CMD_H_
#define _GET_MOBILE_CMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclils.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CGetUserExpandCMD : public CBaseDBCMD
{
public:
    CGetUserExpandCMD();
    ~CGetUserExpandCMD();

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
        return EDBCT_GETUSEREXPAND ;
    }

protected:
    BOOL GetUserExpand(SGDP::ISDDBConnection* poDBConn, PKT_CLILS_GET_USER_EXPAND_REQ* pstReq);
private:
    PKT_CLILS_GET_USER_EXPAND_ACK m_stAck;
};

#endif //#ifndef _LOGINCMD_H_

