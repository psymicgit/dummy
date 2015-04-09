

#ifndef _UNBIND_H_
#define _UNBIND_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclizs.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CUnBindDeviceCMD : public CBaseDBCMD
{
public:
    CUnBindDeviceCMD();
    ~CUnBindDeviceCMD();

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
        return EDBCT_UNBIND_DEVICE ;
    }

protected:
    BOOL UnBind(SGDP::ISDDBConnection* poDBConn, PKT_CLIZS_UN_BIND_DEVICE_REQ* pstReq);
private:
    PKT_CLIZS_UN_BIND_DEVICE_ACK m_stAck;
};

#endif //#ifndef _LOGINCMD_H_

