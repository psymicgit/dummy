#ifndef _SET_TONGBU_PAY_PROCESSED_H_
#define _SET_TONGBU_PAY_PROCESSED_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include <logic/player/playergetdatamgr.h>

#define STR_GLOBAL_TABLE_NAME "global_amount"

//同步订单处理完毕
struct STongBuPayProcessed
{
    STongBuPayProcessed()
    {
        dwPlayerID = 0;
        dwGold = 0;
        wErrCode = 0;
        dwSessionID = 0;
		dwRMB = 0;
    }
    UINT32 dwPlayerID;	//玩家ID
    UINT32 dwGold;		//充值元宝数
    string strTableName;	//该服对应的数据库表名
    string strOrderID;
    string strSource;
    string strPartner;
    UINT16 wErrCode;
    UINT32 dwSessionID;
	UINT32 dwRMB;			//充值数（以分为单位）

};

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CSetTongBuPayProcessed : public CBaseDBCMD
{
public:
    CSetTongBuPayProcessed();
    virtual ~CSetTongBuPayProcessed();
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
	virtual EDBCmdType GetDBCmdType() { return EDBCT_SETTONGBUPAYPPROCESSED ; }

private:
    BOOL SetPayProcessed(SGDP::ISDDBConnection* poDBConn);
private:
    STongBuPayProcessed m_stPayProcessed; //订单处理结果信息
};


#endif