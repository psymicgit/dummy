#ifndef _SAVEPRIVATEMSGLOG_CMD_H_
#define _SAVEPRIVATEMSGLOG_CMD_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include "logic/friendsys/privatemsgmgr.h"




/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CSavePrivateMsgLogCmd : public CBaseDBCMD
{
public:
    CSavePrivateMsgLogCmd();
    virtual ~CSavePrivateMsgLogCmd();
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
        return EDBCT_SAVEPRIVATEMSGLOG ;
    }

private:
    UINT16				m_wErrCode;
	CPrivateMsgList		m_dbPrivateMsgList;
};


#endif