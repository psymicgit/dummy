#ifndef _PRIVATE_MSG_MGR_H_
#define _PRIVATE_MSG_MGR_H_

#include <sdtype.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <list>
#include <sdsingleton.h>
#include <protocommondata.h>
#include <set>

typedef struct tagDT_PRIVATE_MSG_LIST_CLI DT_PRIVATE_MSG_LIST_CLI;

using namespace SGDP;

// 延迟同步的私聊消息
struct SPrivateMsg
{
    UINT32	dwSenderID;	// 发方ID
    UINT32	dwRecverID;	// 收方ID
    UINT32	dwMsgIdx;		//流水号
    TCHAR	aszMsgContent[PRIVATE_MSG_CONTENT_LEN]; // 消息内容
    UINT64	qwSendTime;	// 发送时间
    INT8		byIsRead;		//是否已读

    SPrivateMsg()
    {
        Init();
    }
    void Init()
    {
        memset(this, 0x00, sizeof(SPrivateMsg));
    }
};

//私聊消息(按收方玩家分组)
typedef std::list<SPrivateMsg> CPrivateMsgList; // key为收方玩家ID
typedef CPrivateMsgList::iterator CPrivateMsgListItr;
typedef CPrivateMsgList::reverse_iterator CPrivateMsgListRItr;

// 延迟同步的私聊消息
struct SPrivateMsgList
{
    CPrivateMsgList	listPrivateMsg;
    UINT32           dwMaxMsgIdx;
    UINT64			qwLastTime; // 最后访问时间

    SPrivateMsgList()
    {
        dwMaxMsgIdx = 1;
        listPrivateMsg.clear();
        qwLastTime = SDTimeSecs();
    }
};

//私聊消息(按收方玩家分组)
typedef std::map<UINT64, SPrivateMsgList*> CPrivateMsgMap; // key为收方玩家ID
typedef CPrivateMsgMap::iterator CPrivateMsgMapItr;


class CPlayer;

class CPrivateMsgMgr
{
public:
    DECLARE_SINGLETON(CPrivateMsgMgr);

public:

    BOOL Init();

    VOID UnInit();


public:

    // 添加延时同步的私聊消息sql
    UINT16 AddPrivateMsg(SPrivateMsg& stPrivateMsg);

    // 检测玩家的私聊信息
    BOOL	CkPrivateMsg(UINT32 dwSenderID, UINT32 dwRecverID);

    //设置消息
    BOOL	SetPrivateMsgVec(UINT32 dwSenderID, UINT32 dwRecverID, SPrivateMsgList listPrivateMsg);

    UINT16 SendPrivateMsg(CPlayer* poPlayer, CPlayer* poTargetPlayer, TCHAR aszMsgContent[PRIVATE_MSG_CONTENT_LEN]);


    // 9获取与某玩家的私聊记录
    UINT16 GetPrivateMsgLogList(UINT32 dwSenderID, UINT32 dwRecverID, UINT32 dwStartIdx, UINT8 byGetMsgNum, UINT32& dwTotalNum, DT_PRIVATE_MSG_LIST_CLI& stPrivateMsgList);

    //定时保存数据
    BOOL OnSave();

	//添加已读信息
	VOID AddReadMsg(UINT32 dwSendID, UINT32 dwRecvID);
protected:
    CPrivateMsgMgr();
    virtual ~CPrivateMsgMgr();

protected:

private:
	UINT64					m_dbMsgTime;
    CPrivateMsgMap			m_mapPrivateMsg; //
	set<UINT64>					m_setReadID;	//已读玩家列表
    CSDObjectPool<SPrivateMsgList, CSDMutex>	m_oPrivateMsgVecPool;

    //数据库变化要保存的例表
    UINT64					m_dbLastTime;
    CPrivateMsgList			m_dbPrivateMsgList;
};

#endif // 




