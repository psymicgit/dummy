#ifndef _SET_RECREATE_PAY_PROCESSED_H_
#define _SET_RECREATE_PAY_PROCESSED_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include <logic/player/playergetdatamgr.h>


//自己订单处理完毕
struct SRecreatePayProcessed
{
    SRecreatePayProcessed()
    {
        wZoneID = 0;
        dwPlayerID = 0;
        dwGold = 0;
        dwRMB = 0;
        dwSessionID = 0;
        bySandBoxFlag = 0;
        byVipLevel = 0;
		dwPayID = 0;
    }
    UINT16	wZoneID;
    UINT32	dwPlayerID; //玩家ID
    UINT32	dwGold; //元宝数
    string	strOrderID;
    string	strComment;
    UINT32	dwRMB;
    UINT8	bySandBoxFlag;
    UINT32	dwSessionID;
    UINT8	byVipLevel;
	UINT32	dwPayID;
};



/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CSetRecreatePayProcessed : public CBaseDBCMD
{
public:
    CSetRecreatePayProcessed();
    virtual ~CSetRecreatePayProcessed();
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
        return EDBCT_SETRECREATEPAYPROCESSED ;
    }

private:
    BOOL SetPayProcessed(SGDP::ISDDBConnection* poDBConn);
private:
    vector<SRecreatePayProcessed> m_vecRecreatePayProcessed; //订单处理结果信息
    BOOL m_bSuccess; //是否成功
};


#endif