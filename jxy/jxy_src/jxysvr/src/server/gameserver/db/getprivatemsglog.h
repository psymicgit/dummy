#ifndef _GET_PRIVATE_MSG_LOG_CMD_H_
#define _GET_PRIVATE_MSG_LOG_CMD_H_

#include <db/basedbccmd.h>
#include <sddb.h>
#include "protocol/server/protogsdb.h"
#include "logic/friendsys/privatemsgmgr.h"


enum em_GetPrivateMsg
{
	em_Msg = 1,//聊天记录
	em_Log = 2,//聊天日志
};


typedef struct tagGET_PRIVATEMSGLOG
{
    tagGET_PRIVATEMSGLOG()
    {
		dwSenderID = 0;
		dwRecverID = 0;
		strMsgContent = "";
		dwStartIdx = 0;
		byGetMsgNum = 0;
    }
	BOOL   byType;
    UINT32 dwSenderID;
	UINT32 dwRecverID;
	string strMsgContent;
	UINT32 dwStartIdx; //起始序号
	UINT8  byGetMsgNum; //获取消息个数(向更小的序号方向取)
} GET_PRIVATEMSGLOG;

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CGetPrivateMsgLogCmd : public CBaseDBCMD
{
public:
    CGetPrivateMsgLogCmd();
    virtual ~CGetPrivateMsgLogCmd();
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
        return EDBCT_GET_PRIVATEMSGLOG ;
    }

private:
    UINT16				m_wErrCode;
    GET_PRIVATEMSGLOG		m_GET_PRIVATEMSGLOG;
	//消息类型
	SPrivateMsgList		m_listPrivateMsg;
};


#endif