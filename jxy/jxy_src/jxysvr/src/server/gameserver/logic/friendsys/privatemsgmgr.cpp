#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <common/client/errdef.h>
#include <common/client/commondef.h>
#include "privatemsgmgr.h"
#include "sddb.h"
#include <framework/gsapi.h>
#include <logic/base/logicfunc.h>
#include "logic/player/player.h"

#include <db/dbmgr.h>
#include <db/basedbccmd.h>

using namespace SGDP;



const int MAX_PRIVATE_MSG_POOL_CACHE_NUM = 50;
const int MAX_MSG_TIMES = 600;

IMPLEMENT_SINGLETON(CPrivateMsgMgr);

CPrivateMsgMgr::CPrivateMsgMgr()
{
    m_dbMsgTime = SDTimeSecs();
    m_dbLastTime = SDTimeSecs();
    m_dbPrivateMsgList.clear();
    m_mapPrivateMsg.clear();
}

CPrivateMsgMgr::~CPrivateMsgMgr()
{

}

BOOL CPrivateMsgMgr::Init()
{
    m_dbLastTime = SDTimeSecs();
    m_dbPrivateMsgList.clear();
    m_mapPrivateMsg.clear();
    return TRUE;
}

VOID CPrivateMsgMgr::UnInit()
{

}

// 添加延时同步的私聊消息sql
UINT16 CPrivateMsgMgr::AddPrivateMsg(SPrivateMsg& stPrivateMsg)
{
    SPrivateMsgList *	poPrivateMsgList = NULL;
    do
    {
        UINT64 qwKey1 = 0;
        qwKey1 = CLogicFunc::Get3232Key(stPrivateMsg.dwSenderID, stPrivateMsg.dwRecverID);
        CPrivateMsgMapItr itr = m_mapPrivateMsg.find(qwKey1);
        if (itr != m_mapPrivateMsg.end())
        {
            poPrivateMsgList = itr->second;
            break;
        }

        UINT64 qwKey2 = 0;
        qwKey2 = CLogicFunc::Get3232Key(stPrivateMsg.dwRecverID, stPrivateMsg.dwSenderID);
        itr = m_mapPrivateMsg.find(qwKey2);
        if (itr != m_mapPrivateMsg.end())
        {
            poPrivateMsgList = itr->second;
            break;
        }

        poPrivateMsgList = m_oPrivateMsgVecPool.Alloc();
        m_mapPrivateMsg.insert(make_pair(qwKey1, poPrivateMsgList));
        m_mapPrivateMsg.insert(make_pair(qwKey2, poPrivateMsgList));
    }
    while (0);

    if (NULL != poPrivateMsgList)
    {
        stPrivateMsg.dwMsgIdx = ++poPrivateMsgList->dwMaxMsgIdx;
        poPrivateMsgList->qwLastTime = SDTimeSecs();
        poPrivateMsgList->listPrivateMsg.push_back(stPrivateMsg);

        //数据新增加，保存
        m_dbPrivateMsgList.push_back(stPrivateMsg);

        //对于新加入的数据检测是否保存过多，内存只保留50条。
        if (poPrivateMsgList->listPrivateMsg.size() > MAX_PRIVATE_MSG_POOL_CACHE_NUM)
        {
            poPrivateMsgList->listPrivateMsg.pop_front();
        }
    }
    return 0;
}

// 检测玩家的私聊信息
BOOL  CPrivateMsgMgr::CkPrivateMsg(UINT32 dwSenderID, UINT32 dwRecverID)
{
    UINT64			qwKey = 0;
    SPrivateMsgList *	poPrivateMsgList = NULL;
    do
    {
        qwKey = CLogicFunc::Get3232Key(dwSenderID, dwRecverID);
        CPrivateMsgMapItr itr = m_mapPrivateMsg.find(qwKey);
        if (itr != m_mapPrivateMsg.end())
        {
            return TRUE;
        }
        qwKey = CLogicFunc::Get3232Key(dwRecverID, dwSenderID);
        itr = m_mapPrivateMsg.find(qwKey);
        if (itr != m_mapPrivateMsg.end())
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CkPrivateMsg very import error!"), MSG_MARK);
            return FALSE;
        }
    }
    while (0);
    return FALSE;
}

//设置消息
BOOL CPrivateMsgMgr::SetPrivateMsgVec(UINT32 dwSenderID, UINT32 dwRecverID, SPrivateMsgList listPrivateMsg)
{
    do
    {
        UINT64 qwKey1 = CLogicFunc::Get3232Key(dwSenderID, dwRecverID);
        CPrivateMsgMapItr itr = m_mapPrivateMsg.find(qwKey1);
        if (itr != m_mapPrivateMsg.end())
        {
            SYS_CRITICAL(_SDT("[%s: %d]: SetPrivateMsgVec very import error at key1, sender = %u, recver = %u!!"), MSG_MARK, dwSenderID, dwRecverID);
            return TRUE;
        }

        UINT64 qwKey2 = CLogicFunc::Get3232Key(dwRecverID, dwSenderID);
        itr = m_mapPrivateMsg.find(qwKey2);
        if (itr != m_mapPrivateMsg.end())
        {
            SYS_CRITICAL(_SDT("[%s: %d]: SetPrivateMsgVec very import error at key2, recver = %u, sender = %u!!"), MSG_MARK, dwSenderID, dwRecverID);
            return TRUE;
        }

        SPrivateMsgList *poPrivateMsgList = m_oPrivateMsgVecPool.Alloc();
        if (NULL == poPrivateMsgList)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: SetPrivateMsgVec very import error: alloc msglist fail!!"), MSG_MARK);
            return FALSE;
        }

        *poPrivateMsgList = listPrivateMsg;
        m_mapPrivateMsg.insert(make_pair(qwKey1, poPrivateMsgList));
        m_mapPrivateMsg.insert(make_pair(qwKey2, poPrivateMsgList));
    }
    while (0);
    return FALSE;
}

// 发送私聊消息
UINT16 CPrivateMsgMgr::SendPrivateMsg(CPlayer* poPlayer, CPlayer* poTargetPlayer, TCHAR aszMsgContent[PRIVATE_MSG_CONTENT_LEN])
{
    // 接收者是否在线
    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        return ERR_SEND_PRIVATE_MSG::ID_NO_OPEN;
    }

    UINT8 byFriendSysRelationType = poFriends->CkRelationType(poTargetPlayer->GetID());

    // 检查别人和你的关系
    if(EFSRT_BLACKLIST == byFriendSysRelationType)
    {
        return ERR_SEND_PRIVATE_MSG::ID_REF_PLAYER_ALREADY_IN_YOUR_BLACKLIST; // 发送消息失败, 你已把对方拉入黑名单
    }

	CFriends *poTargetFriends = poTargetPlayer->GetFriends();
	if (NULL == poTargetFriends)
	{
		return ERR_SEND_PRIVATE_MSG::ID_NO_OPEN;
	}

    // 检查自己是不是被对方拉入黑名单
    byFriendSysRelationType = poTargetFriends->CkRelationType(poPlayer->GetID());
    if(EFSRT_BLACKLIST == byFriendSysRelationType)
    {
        return ERR_SEND_PRIVATE_MSG::ID_YOU_ARE_IN_REF_PLAYER_BLACKLIST; // 发送消息失败, 对方已把你拉入黑名单
    }

    // 如果自己对于收消息方来说是路人甲，那么将自己加到对方的陌生人列表里
    if(EFSRT_OTHER == byFriendSysRelationType)
    {
        byFriendSysRelationType = EFSRT_STRANGER;
        poTargetFriends->AddStranger(poPlayer->GetID());
    }

    SPrivateMsg stPrivateMsg;
    stPrivateMsg.dwSenderID		= poPlayer->GetID();
    stPrivateMsg.dwRecverID		= poTargetPlayer->GetID();
    SDStrncpy(stPrivateMsg.aszMsgContent, aszMsgContent, PRIVATE_MSG_CONTENT_LEN);
    stPrivateMsg.qwSendTime		= SGDP::SDTimeSecs();

	//增加最近聊天记录
	poFriends->AddFinallyContact(poTargetPlayer->GetID());
	poTargetFriends->AddFinallyContact(poPlayer->GetID());

    AddPrivateMsg(stPrivateMsg);

    // 1 对方在线
    if (poTargetPlayer->CkLogin())
    {
        stPrivateMsg.byIsRead = 1;

        // 1.2 推送消息
        PKT_CLIGS_ONLINE_PRIVATE_MSG_NTF stNtf;
        stNtf.dwMsgIdx = stPrivateMsg.dwMsgIdx;
        stNtf.dwSendPlayerID = poPlayer->GetID();
        memcpy(stNtf.aszSendPlayerName, poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName, USERNAME_LEN);
        stNtf.wMainHeroKindID = poPlayer->GetCoachHeroKindID();
        stNtf.byRelationType = byFriendSysRelationType;
        memcpy(stNtf.aszMsgContent, aszMsgContent, PRIVATE_MSG_CONTENT_LEN);
        stNtf.qwSendTime = stPrivateMsg.qwSendTime;
        poTargetPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_ONLINE_PRIVATE_MSG_NTF);
    }
    else
    {
        stPrivateMsg.byIsRead			= 0;
        poTargetFriends->AddUnReadEven(poPlayer->GetID());
    }

    return ERR_SEND_PRIVATE_MSG::ID_SUCCESS;
}

// 获取与某玩家的私聊记录
UINT16 CPrivateMsgMgr::GetPrivateMsgLogList(UINT32 dwSenderID, UINT32 dwRecverID, UINT32 dwStartIdx, UINT8 byGetMsgNum, UINT32& dwTotalNum, DT_PRIVATE_MSG_LIST_CLI& stMsgListCli)
{
    if (byGetMsgNum > MAX_PRIVATE_MSG_LIST_ELEM_NUM)
    {
        return ERR_GET_PRIVATE_MSG_LOG_LIST::ID_INVALID_REQ_PARAM; // 无效的参数
    }

    UINT64 qwKey = CLogicFunc::Get3232Key(dwSenderID, dwRecverID);
    CPrivateMsgMapItr itr = m_mapPrivateMsg.find(qwKey);
    if (itr == m_mapPrivateMsg.end())
    {
        return ERR_GET_PRIVATE_MSG_LOG_LIST::ID_NO_REF_DATA; // 无效的参数
    }

    SPrivateMsgList *poPrivateMsgList = itr->second;
    dwTotalNum = poPrivateMsgList->listPrivateMsg.size();

    stMsgListCli.dwMsgNum = 0;

    // 由于私聊列表中的私聊记录越往end()的方向越新，所以这里用reverse_iterator来从后往前取小于指定索引的私聊记录
    for(CPrivateMsgList::reverse_iterator v_itr = poPrivateMsgList->listPrivateMsg.rbegin(); v_itr != poPrivateMsgList->listPrivateMsg.rend(); v_itr++)
    {
        SPrivateMsg &stMsg = *v_itr;

        // 对于0上来的序号，直接从最后面打包
        if (0 != dwStartIdx && stMsg.dwMsgIdx >= dwStartIdx)
        {
            continue;
        }

        if(stMsgListCli.dwMsgNum >= byGetMsgNum || stMsgListCli.dwMsgNum >= MAX_PRIVATE_MSG_LIST_ELEM_NUM)
        {
            break;
        }

        DT_PRIVATE_MSG_DATA &stMsgData = stMsgListCli.astListData[stMsgListCli.dwMsgNum];
        memset(&stMsgData, 0x00, sizeof(DT_PRIVATE_MSG_DATA));

        stMsgData.dwMsgIdx   = stMsg.dwMsgIdx;

        //检测是否是自己发出的
        stMsgData.byIsRecv = (stMsg.dwSenderID == dwSenderID ? 0 : 1);

        // 将收到的未读的私聊设置为已读，并将变更存入数据库
        //if (0 == stMsg.byIsRead && stMsg.dwRecverID == dwSenderID)
        //{
        //    stMsg.byIsRead = 1;

        //    //数据有变化，做修改保存
        //    m_dbPrivateMsgList.push_back(stMsg);
        //}

        stMsgData.qwSendTime = stMsg.qwSendTime;
        memcpy(&stMsgData.aszMsgContent, stMsg.aszMsgContent, PRIVATE_MSG_CONTENT_LEN);

        stMsgListCli.dwMsgNum++;
    }

    // 然后颠倒发给客户端的私聊列表的顺序，使得越往数组后面私聊越新
    std::reverse(stMsgListCli.astListData, stMsgListCli.astListData + stMsgListCli.dwMsgNum);
    return ERR_GET_PRIVATE_MSG_LOG_LIST::ID_SUCCESS;
}

//定时保存数据
BOOL CPrivateMsgMgr::OnSave()
{
    // 定期检测聊天记录，对于过期的数据删除
    UINT64 qwNow = SDTimeSecs();

    do{
        // 每10秒钟检测一次，不然太浪费时间了。
        if(qwNow - m_dbMsgTime < 10)
        {
            break;
        }

        m_dbMsgTime = qwNow;
		//把未读消息设置成已读
		if ( 0 < m_setReadID.size())
		{
			CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SET_READ_PRIVATE_MSG);
			if(NULL == poCMD)
			{
				SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
				return FALSE;
			}

			poCMD->SetUserData( &m_setReadID, 0 );
			gsapi::GetDBMgr()->AddCommand( poCMD );
			m_setReadID.clear();
		}

        SPrivateMsgList	*poExpiredMsgList = NULL;
        for (CPrivateMsgMapItr v_itr = m_mapPrivateMsg.begin(); v_itr != m_mapPrivateMsg.end(); v_itr++)
        {
            SPrivateMsgList	*poMsgList = v_itr->second;
            if(qwNow - poMsgList->qwLastTime > MAX_MSG_TIMES)
            {
                poExpiredMsgList = poMsgList;
                break;
            }
        }

        if(NULL == poExpiredMsgList)
        {
            break;
        }

        for (CPrivateMsgMapItr v_itr = m_mapPrivateMsg.begin(); v_itr != m_mapPrivateMsg.end();)
        {
            SPrivateMsgList	*poMsgList = v_itr->second;
            if(poMsgList == poExpiredMsgList)
            {
                m_mapPrivateMsg.erase(v_itr++);
            }
            else
            {
                v_itr++;
            }
        }

        m_oPrivateMsgVecPool.Free(poExpiredMsgList);
    }while(0);

    //检测内存是否有数据
    if(m_dbPrivateMsgList.empty())
    {
        return FALSE;
    }

    // 内存中的数据超过50条或者检测到时间已经超过一秒
    if(m_dbPrivateMsgList.size() > 50 || (qwNow - m_dbLastTime > 1))
    {
        CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVEPRIVATEMSGLOG);
        if(NULL == poCMD)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
            return FALSE;
        }

        poCMD->SetUserData( &m_dbPrivateMsgList, 0 );
        gsapi::GetDBMgr()->AddCommand( poCMD );

        m_dbPrivateMsgList.clear();
        m_dbLastTime = qwNow;

        return TRUE;
    }

	
    return FALSE;
}

VOID CPrivateMsgMgr::AddReadMsg(UINT32 dwSendID, UINT32 dwRecvID)
{
	m_setReadID.insert(CLogicFunc::Get3232Key(dwSendID, dwRecvID));
}