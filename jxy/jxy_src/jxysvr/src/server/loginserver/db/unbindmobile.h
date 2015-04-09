

#ifndef _UNBIND_MOBILE_CMD_H_
#define _UNBIND_MOBILE_CMD_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclils.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CUnBindMobileCMD : public CBaseDBCMD
{
public:
    CUnBindMobileCMD();
    ~CUnBindMobileCMD();

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
        return EDBCT_UNBINDMOBILE ;
    }

protected:
    BOOL UnBindMobile(SGDP::ISDDBConnection* poDBConn, PKT_CLILS_UNBIND_MOBILE_REQ* pstReq);
private:
	char		m_szMobile[MAX_MOBILE_LENGTH];
    PKT_CLILS_UNBIND_MOBILE_ACK m_stAck;
};

#endif //#ifndef _LOGINCMD_H_

