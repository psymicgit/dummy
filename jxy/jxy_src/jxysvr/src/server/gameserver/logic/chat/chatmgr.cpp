#include "chatmgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <framework/gsconfig.h>
#include <net/cli/usermgr.h>
#include <db/getchatcmd.h>
#include <logic/player/player.h>
#include <db/dbcommoperate.h>
#include <logic/other/errmsgmgr.h>
#include <errdef.h>
#include <framework/gsapi.h>
#include "wordfilter.h"
#include "logic/faction/factionmgr.h"
#include <dll/sdnet/sdpkg.h>
#include <math.h>

#define MAX_SEND_CHAT_NOTICE_TIME		60
#define MAX_SYNC_CHAT_NOTICE_TIME		600

// 聊天记录历史保留的个数
#define MAX_CHAT_HISTORY (MAX_CHAT_NUM * 3)

#define CLI_NEW_CHAT_SUPPORT_VERSION 301 // 客户端版本需要>=301才支持新的聊天频道：门派、活动频道

using namespace SGDP;

UINT32 CChatMgr::s_dwSerialNumber = 0;

namespace chatutil
{
// 根据聊天时间插入聊天记录，聊天时间如果是同一秒，则按流水号来排序
VOID InsertChatByTime(CChatList &listChat, ChatInfo &stChatInfo)
{
    for(CChatList::iterator itr = listChat.begin(); itr != listChat.end(); ++itr)
    {
        ChatInfo &stItrChat = *itr;

        // 聊天时间
        if(stItrChat.stData.qwChatTime > stChatInfo.stData.qwChatTime)
        {
            listChat.insert(itr, stChatInfo);
            return;
        }
        else if(stItrChat.stData.qwChatTime == stChatInfo.stData.qwChatTime)
        {
            if(stItrChat.stData.dwSerialNumber > stChatInfo.stData.dwSerialNumber)
            {
                listChat.insert(itr, stChatInfo);
                return;
            }
        }
    }

    listChat.push_back(stChatInfo);
}

// 获取门派的图标ID
BYTE GetFactionIconIDByFaction(UINT32 dwFactionID)
{
    CFaction *poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
    if(NULL == poFaction)
    {
        return NULL_FACTION_ICON_ID;
    }

    return poFaction->GetFactionIconID();
}
}

IMPLEMENT_SINGLETON(CChatMgr)

CChatMgr::CChatMgr() :
    m_wCurSysChatIdx(0)
{
    ResetChat();

    m_qwSendNoticeTime = SDTimeSecs();
    m_qwChatSyncTime = SDTimeSecs();
}

CChatMgr::~CChatMgr()
{
    ResetChat();
}

BOOL CChatMgr::Init()
{
    if(FALSE == CWordFilter::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CWordFilter::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Init failed!"), MSG_MARK);
        return FALSE;
    }

    if(!InitSysChatDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitSysChatDB failed!"), MSG_MARK);
        return FALSE;
    }
    if(!InitSilenceDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitSilenceDB failed!"), MSG_MARK);
        return FALSE;
    }
    if(!InitGMInfoDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitGMInfoDB failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

VOID CChatMgr::UnInit()
{
    ResetChat();
}

VOID CChatMgr::ResetChat()
{
    m_listWorldChat.clear();
    m_listActivityChat.clear();
    m_listSysChatNotExpired.clear();
    m_listSysChatNotice.clear();
}

// 将本地公告列表重置为数据库获取到的公告列表
VOID CChatMgr::SetSYSChatDataNoticeData(CSysChatNoticeList &listDBSysChat)
{
    // 直接用数据库的数据覆盖本地数据，不过要注意，数据库取出的聊天记录是没有流水号的
    m_listSysChatNotice = listDBSysChat;

    UINT32 i = 0;

    // 重新设置未过期的系统公告列表
    const UINT64 qwNow = SDTimeSecs();

    m_listSysChatNotExpired.clear();

    // 把未过期的系统公告塞入列表
    for(CSysChatNoticeList::iterator itr = m_listSysChatNotice.begin(); itr != m_listSysChatNotice.end(); ++itr)
    {
        SYS_CHAT_DATA &stSysChat = *itr;
        stSysChat.stData.dwSerialNumber = ++i; // 这里设置流水号是因为客户端不接受相同流水号的聊天记录

        // 是否已过期
        if(stSysChat.qwExpiresTime >= qwNow)
        {
            // 未过期
            stSysChat.stData.dwSerialNumber = AllocChatSerialNum(); // 对于未过期的聊天记录，确保该聊天记录具有较大的流水号
            m_listSysChatNotExpired.push_back(stSysChat);
        }
    }
}

VOID CChatMgr::SetSilenceData(CChatSilenceByDeviceIDMap &mapSilenceByDeviceID,
                              CChatSilenceByUserIDMap &mapSilenceByUserID,
                              CChatSilenceByPlayerIDMap	&mapSilenceByPlayerID)
{

    m_mapSilenceByDeviceID.clear();
    m_mapSilenceByUserID.clear();
    m_mapSilenceByPlayerID.clear();
    m_mapSilenceByDeviceID = mapSilenceByDeviceID;
    m_mapSilenceByUserID = mapSilenceByUserID;
    m_mapSilenceByPlayerID = mapSilenceByPlayerID;

}

VOID CChatMgr::SetGMData(CStr32Map mapGmDeviceID, C3232Map mapGmPlayerID)
{
    m_mapGmDeviceID.clear();
    m_mapGmPlayerID.clear();

    m_mapGmDeviceID = mapGmDeviceID;
    m_mapGmPlayerID = mapGmPlayerID;
}

// 增加gm聊天
VOID CChatMgr::AddGmData(UINT32 dwPlayerID, string strDeviceID, UINT8 byOptType)
{
    if (0 != dwPlayerID)
    {
        m_mapGmPlayerID.insert(pair<UINT32, UINT32>(dwPlayerID, byOptType));
        m_oSyncSilenceWordfileter2Db.m_mapGmPlayerID.insert(pair<UINT32, UINT32>(dwPlayerID, byOptType));
    }
    if (strDeviceID.size())
    {
        m_mapGmDeviceID.insert(pair<string, UINT32>(strDeviceID, byOptType));
        m_oSyncSilenceWordfileter2Db.m_mapGmDeviceID.insert(pair<string, UINT32>(strDeviceID, byOptType));
    }
}

// 添加系统聊天公告
VOID CChatMgr::AddSysChat(string strChatContent, string strExpiresTime, UINT8 byOptType)
{
    if (strChatContent.empty())
    {
        return;
    }

    const UINT64 qwNow = SDTimeSecs();

    SYS_CHAT_DATA stSysChat;
    stSysChat.byOptType = byOptType;

    CSDDateTime stDateTime;
    if(SDTimeFromString(strExpiresTime, stDateTime))
    {
        stSysChat.qwExpiresTime = stDateTime.GetTimeValue();
    }
    else
    {
        stSysChat.qwExpiresTime = qwNow;
    }

    stSysChat.stData.dwPlayerID = 0;
    stSysChat.stData.byChatType = ECT_SYS;
    stSysChat.stData.dwSerialNumber = AllocChatSerialNum();
    stSysChat.stData.qwChatTime = qwNow;
    stSysChat.stData.dwChatLen = strChatContent.size() >= MAX_CHAT_CONTENT_NUM ? MAX_CHAT_CONTENT_NUM - 1 : strChatContent.size();
    memcpy(stSysChat.stData.abyChatContent, strChatContent.c_str(), stSysChat.stData.dwChatLen);

    m_listSysChatNotice.push_back(stSysChat);
    m_oSyncSilenceWordfileter2Db.m_listSYSChatDataNotice.push_back(stSysChat);

    // 是否未过期，未过期的话将其插入未过期公告列表，使其定时发送
    if(qwNow <= stSysChat.qwExpiresTime)
    {
        m_listSysChatNotExpired.push_back(stSysChat);
    }

}

// 添加活动聊天公告
VOID CChatMgr::AddActivityChat(const string &strChatContent, UINT64 qwActivityStartTime/* 活动开始时间，当>0时表示时间到时才发，当=0时表示立即发送*/)
{
    if (strChatContent.empty())
    {
        return;
    }

    ACTIVITY_CHAT_DATA stActChat;
    BuildActivityChat(stActChat, strChatContent, qwActivityStartTime);

    m_listActivityChat.push_back(stActChat);
}

// 构造活动公告数据
VOID CChatMgr::BuildActivityChat(ACTIVITY_CHAT_DATA &stActChat, const string &strChatContent, UINT64 qwActivityStartTime/*活动开始时间，当>0时表示时间到时才发，当=0时表示立即发送*/)
{
    memset(&stActChat, 0, sizeof(stActChat));

    stActChat.qwActivityStartTime = qwActivityStartTime;

    CHAT_DATA &stChat = stActChat.stData;
    stChat.dwPlayerID = 0;
    stChat.byChatType = ECT_ACTIVITY;
    stChat.qwChatTime = (qwActivityStartTime == 0 ? SDTimeSecs() : qwActivityStartTime);
    stChat.dwChatLen  = (strChatContent.size() >= MAX_CHAT_CONTENT_NUM ? MAX_CHAT_CONTENT_NUM - 1 : strChatContent.size());
    memcpy(stChat.abyChatContent, strChatContent.c_str(), stChat.dwChatLen);
    stChat.dwSerialNumber = AllocChatSerialNum();
}

// 添加门派活动公告
ChatInfo* CChatMgr::BuildFactionActivityChat(UINT32 dwFactionID, const string &strChatContent)
{
    ChatInfo *pstChatInfo = m_oChatDataPool.Alloc(); // 新分配一个ChatInfo
    if (NULL == pstChatInfo)
    {
        return NULL;
    }

    CHAT_DATA &stChat = pstChatInfo->stData;
    memset(&stChat, 0, sizeof(stChat));

    stChat.dwPlayerID = 0;
    stChat.byChatType = ECT_FACTION;
    stChat.qwChatTime = SDTimeSecs();
    stChat.dwChatLen  = (strChatContent.size() >= MAX_CHAT_CONTENT_NUM ? MAX_CHAT_CONTENT_NUM - 1 : strChatContent.size());
    memcpy(stChat.abyChatContent, strChatContent.c_str(), stChat.dwChatLen);
    stChat.dwSerialNumber = AllocChatSerialNum();

    pstChatInfo->byVipLevel      = 0;
    pstChatInfo->dwFactionIconID = chatutil::GetFactionIconIDByFaction(dwFactionID);

    return pstChatInfo;
}

VOID CChatMgr::AddSilenceData(vector<string> &vecSilenceByDeviceID, vector<string> &vecSilenceByUserID, vector<string>	&vecSilenceByPlayerID)
{
    UINT32 dwSize = vecSilenceByDeviceID.size();
    UINT32 dwIdx = 0;
    for(dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        if(0 == vecSilenceByDeviceID[dwIdx].compare("0"))
        {
            continue;
        }
        m_mapSilenceByDeviceID[vecSilenceByDeviceID[dwIdx]] = 1;
        m_oSyncSilenceWordfileter2Db.m_mapModifySilenceByDeviceID[vecSilenceByDeviceID[dwIdx]] = 1;
    }
    dwSize = vecSilenceByUserID.size();
    for(dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        if(0 == vecSilenceByUserID[dwIdx].compare("0"))
        {
            continue;
        }
        m_mapSilenceByUserID[SDAtou(vecSilenceByUserID[dwIdx].c_str())] = 1;
        m_oSyncSilenceWordfileter2Db.m_mapModifySilenceByUserID[SDAtou(vecSilenceByUserID[dwIdx].c_str())] = 1;
    }
    dwSize = vecSilenceByPlayerID.size();
    for(dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        if(0 == vecSilenceByPlayerID[dwIdx].compare("0"))
        {
            continue;
        }
        m_mapSilenceByPlayerID[SDAtou(vecSilenceByPlayerID[dwIdx].c_str())] = 1;
        m_oSyncSilenceWordfileter2Db.m_mapModifySilenceByPlayerID[SDAtou(vecSilenceByPlayerID[dwIdx].c_str())] = 1;
    }
}



VOID CChatMgr::DelSilenceData(vector<string> &vecSilenceByDeviceID, vector<string> &vecSilenceByUserID, vector<string>	&vecSilenceByPlayerID)
{
    UINT32 dwSize = vecSilenceByDeviceID.size();
    UINT32 dwIdx = 0;
    for(dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        if(0 == vecSilenceByDeviceID[dwIdx].compare("0"))
        {
            continue;
        }
        m_mapSilenceByDeviceID.erase(vecSilenceByDeviceID[dwIdx]);
        m_oSyncSilenceWordfileter2Db.m_mapModifySilenceByDeviceID[vecSilenceByDeviceID[dwIdx]] = 0;
    }
    dwSize = vecSilenceByUserID.size();
    for(dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        if(0 == vecSilenceByUserID[dwIdx].compare("0"))
        {
            continue;
        }
        m_mapSilenceByUserID.erase(SDAtou(vecSilenceByUserID[dwIdx].c_str()));
        m_oSyncSilenceWordfileter2Db.m_mapModifySilenceByUserID[SDAtou(vecSilenceByUserID[dwIdx].c_str())] = 0;
    }
    dwSize = vecSilenceByPlayerID.size();
    for(dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        if(0 == vecSilenceByPlayerID[dwIdx].compare("0"))
        {
            continue;
        }
        m_mapSilenceByPlayerID.erase(SDAtou(vecSilenceByPlayerID[dwIdx].c_str()));
        m_oSyncSilenceWordfileter2Db.m_mapModifySilenceByPlayerID[SDAtou(vecSilenceByPlayerID[dwIdx].c_str())] = 0;
    }
}


VOID CChatMgr::SetWordFilterData(CWordFilterDataMap &mapWordFilterData, CWordFilterDataMap &mapDspNameWordFilterData)
{
    CWordFilter::Instance()->SetWordFilterData(mapWordFilterData, mapDspNameWordFilterData);
}

VOID CChatMgr::AddWordFilterData(vector<string> &vecWordFilterData)
{
    UINT32 dwSize = vecWordFilterData.size();
    UINT32 dwIdx = 0;
    CWordFilterDataMap& mapWordFilterData = CWordFilter::Instance()->GetWordFilterData();
    for(dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        TWordFilterData stWordFilterData;
        stWordFilterData.Init();
        _SDTStrncpy(stWordFilterData.m_szWordFilters, vecWordFilterData[dwIdx].c_str(), MAX_WORD_FILTER);
        KMP_GetNext(stWordFilterData.m_szWordFilters, stWordFilterData.m_kmpvalue);	// 得到一个与内容有关的数值m_kmpvalue[i]
        mapWordFilterData[stWordFilterData.m_szWordFilters] = stWordFilterData;
        m_oSyncSilenceWordfileter2Db.m_mapModifyWordFilter[vecWordFilterData[dwIdx]] = 1;
    }
}

VOID CChatMgr::DelWordFilterData(vector<string> &vecWordFilterData)
{
    UINT32 dwSize = vecWordFilterData.size();
    UINT32 dwIdx = 0;
    CWordFilterDataMap& mapWordFilterData = CWordFilter::Instance()->GetWordFilterData();
    for(dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        mapWordFilterData.erase(vecWordFilterData[dwIdx]);
        m_oSyncSilenceWordfileter2Db.m_mapModifyWordFilter[vecWordFilterData[dwIdx]] = 0;
    }
}


UINT32 CChatMgr::OnEvent()
{
    SendChat();

    static UINT16 wIdx = 1;
    //更新聊天数据到期
    if (SDTimeSecs() - m_qwChatSyncTime > CGSConfig::Instance()->GetChatSyncTime())
    {
        CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_CHAT);
        poCMD->SetUserData(&m_oSyncSilenceWordfileter2Db);
        if(0 == wIdx++ % 3) // 3倍间隔取一次数据
        {
            m_oSyncSilenceWordfileter2Db.m_bGetDataFromDB = TRUE;
        }
        if(NULL != poCMD)
        {
            gsapi::GetDBMgr()->AddPayCommand(poCMD);
        }
        m_qwChatSyncTime = SDTimeSecs();
    }
    return 0;
}

// 取出系统公告列表
BOOL CChatMgr::InitSysChatDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetPayDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

    m_oSyncSilenceWordfileter2Db.m_wZoneID = gsapi::GetZoneID();
    m_listSysChatNotice.clear();
    CHAR szSql[1024] = {0};
    sprintf(szSql, "select NoticeID, NoticeContent, ExpiresTime from syschat where ZoneID='%u' or ZoneID='0' order by ZoneID asc, ExpiresTime asc;", m_oSyncSilenceWordfileter2Db.m_wZoneID);

    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet);
    if(SDDB_HAS_RECORDSET != nRet && SDDB_NO_RECORDSET != nRet && SDDB_SUCCESS != nRet)
    {
        return FALSE;
    }

    if(SDDB_HAS_RECORDSET == nRet)
    {
        UINT64 qwNow = SDTimeSecs();

        while(pRecordSet->GetRecord())
        {
            SYS_CHAT_DATA stSysChat;
            stSysChat.uiNoticeID = SDAtou(pRecordSet->GetFieldValueByName("NoticeID"));
            stSysChat.stData.dwPlayerID = 0;
            stSysChat.stData.aszUserName[0] = '\0';// 这里昵称为空就行
            stSysChat.stData.byChatType = ECT_SYS;
            stSysChat.stData.dwChatLen = pRecordSet->GetFieldLengthByName("NoticeContent");
            stSysChat.stData.dwChatLen = stSysChat.stData.dwChatLen >= MAX_CHAT_CONTENT_NUM ? MAX_CHAT_CONTENT_NUM - 1 : stSysChat.stData.dwChatLen;
            stSysChat.stData.dwSerialNumber = AllocChatSerialNum();
            memcpy(stSysChat.stData.abyChatContent, (VOID*)(pRecordSet->GetFieldValueByName("NoticeContent")), stSysChat.stData.dwChatLen);
            GetDateTime2Stamp(stSysChat.qwExpiresTime, "ExpiresTime");
            stSysChat.stData.qwChatTime = qwNow > stSysChat.qwExpiresTime ? stSysChat.qwExpiresTime : qwNow;

            m_listSysChatNotice.push_back(stSysChat);

            // 是否未过期，未过期的话塞入当前未过期列表
            if(stSysChat.qwExpiresTime >= qwNow)
            {
                // 未过期
                m_listSysChatNotExpired.push_back(stSysChat);
            }
        }
    }

    if(pRecordSet)
    {
        pRecordSet->Release();
    }

    m_qwChatSyncTime = SDTimeSecs();
    return TRUE;
}

BOOL CChatMgr::InitSilenceDB()
{
    UINT32 dwPlayerID	=	0;
    UINT32 dwUserID	=	0;
    string strDeviceID = "";

    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetPayDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from  silencedeviceid;", &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            strDeviceID	= pRecordSet->GetFieldValueByName("DeviceID");
            if (0 != strDeviceID.length())
            {
                m_mapSilenceByDeviceID[strDeviceID] = 1;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    /////////////////////////////////////

    nRet = pDBSession->ExecuteSqlRs("select * from  silenceplayerid;", &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            dwPlayerID	= SDAtou(pRecordSet->GetFieldValueByName("PlayerID"));
            if (0 != dwPlayerID)
            {
                m_mapSilenceByPlayerID[dwPlayerID] = 1;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    /////////////////////////////////////

    nRet = pDBSession->ExecuteSqlRs("select * from  silenceuserid;", &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            dwUserID	= SDAtou(pRecordSet->GetFieldValueByName("UserID"));
            if (0 != dwUserID)
            {
                m_mapSilenceByUserID[dwUserID] = 1;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}


BOOL CChatMgr::InitGMInfoDB()
{
    UINT32 dwPlayerID	=	0;
    UINT32 dwUserID	=	0;
    string strDeviceID = "";

    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetPayDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = pDBSession->ExecuteSqlRs("select * from  gmdeviceid;", &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            strDeviceID	= pRecordSet->GetFieldLengthByName("DeviceID");
            if (0 != strDeviceID.length())
            {
                m_mapGmDeviceID[strDeviceID] = 1;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    /////////////////////////////////////

    nRet = pDBSession->ExecuteSqlRs("select * from  gmplayerid;", &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            dwPlayerID	= SDAtou(pRecordSet->GetFieldValueByName("PlayerID"));
            if (0 != dwPlayerID)
            {
                m_mapGmPlayerID[dwPlayerID] = 1;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }


    return TRUE;
}

BOOL CChatMgr::IsGm(CPlayer *poPlayer)
{
    CUser* poUser = poPlayer->GetUser();

    return (IsGmByPlayerID(poPlayer->GetID()))
           || ((NULL != poUser) && (m_mapGmDeviceID.find(poUser->GetDeviceID()) != m_mapGmDeviceID.end()));
}

BOOL CChatMgr::IsGm(UINT32 dwPlayerID)
{
    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        return IsGmByPlayerID(dwPlayerID);
    }

    return IsGm(poPlayer);
}

BOOL CChatMgr::IsGmByPlayerID(UINT32 dwPlayerID)
{
    return m_mapGmPlayerID.find(dwPlayerID) != m_mapGmPlayerID.end();
}

// 是否被禁言
BOOL CChatMgr::IsSilenced(CPlayer *poPlayer)
{
    //获取用户信息
    CUser* poUser = poPlayer->GetUser();
    if (NULL != poUser)
    {
        //根据设备ID检测是否禁言
        string strDeviceID = poUser->GetDeviceID();
        if (m_mapSilenceByDeviceID.find(strDeviceID) != m_mapSilenceByDeviceID.end())
        {
            return TRUE;
        }
        //根据UserID检测是否禁言
        if (m_mapSilenceByUserID.find(poUser->GetUserID()) != m_mapSilenceByUserID.end())
        {
            return TRUE;
        }
    }
    //根据玩家ID检测是否禁言
    if (IsSilencedByPlayerID(poPlayer->GetID()))
    {
        return TRUE;
    }

    return FALSE;
}

// 根据玩家ID检测是否被禁言
BOOL CChatMgr::IsSilencedByPlayerID(UINT32 dwPlayerID)
{
    if (m_mapSilenceByPlayerID.find(dwPlayerID) != m_mapSilenceByPlayerID.end())
    {
        return TRUE;
    }

    return FALSE;
}

// 清理世界频道聊天记录
VOID CChatMgr::ClearWorldChat()
{
    // 清掉世界频道的旧聊天记录
    m_listWorldChatPool.insert(m_listWorldChatPool.end(), m_listWorldChat.begin(), m_listWorldChat.end());
    m_listWorldChat.clear();

    // 清掉部分聊天记录缓存，只保存最后面的几条数据
    while (m_listWorldChatPool.size() > MAX_CHAT_HISTORY)
    {
        CChatDataListItr itr  = m_listWorldChatPool.begin();
        if (itr != m_listWorldChatPool.end())
        {
            m_oChatDataPool.Free(*itr);
            m_listWorldChatPool.erase(itr);
        }
    }
}

// 清理活动频道公告记录
VOID CChatMgr::ClearActivityChat()
{
    // 清理活动频道的旧公告记录，有些未发出的公告则保留
    for (CActivityChatListItr chatitr = m_listActivityChat.begin(); chatitr != m_listActivityChat.end();)
    {
        ACTIVITY_CHAT_DATA &stChat = *chatitr;

        // 若发送时间未到，则保留该聊天
        if (stChat.qwActivityStartTime > 0 && SDTimeSecs() < stChat.qwActivityStartTime)
        {
            chatitr++;
            continue;
        }

        m_listActivityChatPool.push_back(stChat);
        m_listActivityChat.erase(chatitr++);
    }

    // 清掉部分聊天公告缓存，只保存最近的几个记录
    UINT32 dwSize = m_listActivityChatPool.size();
    if(dwSize > MAX_CHAT_HISTORY)
    {
        CActivityChatListItr eraseItr = m_listActivityChatPool.begin();
        advance(eraseItr, dwSize - MAX_CHAT_HISTORY);

        m_listActivityChatPool.erase(m_listActivityChatPool.begin(), eraseItr);
    }
}

// 清理各门派的聊天记录
VOID CChatMgr::ClearFactionChat()
{
    CMapFaction &mapFaction = CFactionMgr::Instance()->GetFactions();

    for(CMapFaction::iterator itr = mapFaction.begin(); itr != mapFaction.end(); ++itr)
    {
        CFaction *poFaction = itr->second;
        if(NULL == poFaction)
        {
            continue;
        }

        poFaction->ClearChat();
    }
}

VOID CChatMgr::FreeChat(ChatInfo *pstChat)
{
    if(NULL == pstChat)
    {
        return;
    }

    m_oChatDataPool.Free(pstChat);
}

// 取出世界频道的聊天记录
VOID CChatMgr::TakeWorldChat(CChatList &listChat)
{
    for(CChatDataList::iterator itr = m_listWorldChat.begin(); itr != m_listWorldChat.end(); ++itr)
    {
        ChatInfo &stChatInfo = **itr;

        // 被禁言玩家的聊天将不被下发
        if(IsSilencedByPlayerID(stChatInfo.stData.dwPlayerID))
        {
            continue;
        }

        listChat.push_back(stChatInfo); // 深拷贝
    }
}

// 取出门派频道的聊天记录
VOID CChatMgr::TakeFactionChat(PKT_CLIGS_CHAT_NTF &stAck, UINT32 dwPlayerID, CFaction *poFaction)
{
    memset(&stAck, 0x00, sizeof(PKT_CLIGS_CHAT_NTF));

    if(NULL == poFaction)
    {
        return;
    }

    // 取出门派聊天记录
    CChatDataList &listChatData = poFaction->GetChat();
    for (CChatDataList::iterator chatitr = listChatData.begin(); chatitr != listChatData.end(); chatitr++)
    {
        if(stAck.byChatNum >= MAX_CHAT_NUM)
        {
            break;
        }

        ChatInfo *pstChatInfo = *chatitr;
        if (pstChatInfo->stData.dwPlayerID == dwPlayerID)
        {
            continue;
        }

        // 玩家若被禁言，则在门派内的聊天不会被下发
        if(IsSilencedByPlayerID(pstChatInfo->stData.dwPlayerID))
        {
            continue;
        }

        // 存入消息包
        memcpy(&stAck.astChatData[stAck.byChatNum], &pstChatInfo->stData, sizeof(CHAT_DATA));
        stAck.abyVipLevel[stAck.byChatNum]          = pstChatInfo->byVipLevel;
        stAck.abyFactionIconIDList[stAck.byChatNum] = pstChatInfo->dwFactionIconID;
        stAck.byChatNum++;
    }
}

// 获取系统公告
VOID CChatMgr::TakeSysChat(CChatList &listChat)
{
    const UINT64 qwNow = SDTimeSecs();

    // 每隔一段时间才发送系统公告
    if (qwNow - m_qwSendNoticeTime < CGSConfig::Instance()->GetChatSendNotice())
    {
        return;
    }

    m_qwSendNoticeTime = qwNow;

    // 清除已过期的系统公告
    for(CSysChatNoticeList::iterator itr = m_listSysChatNotExpired.begin(); itr != m_listSysChatNotExpired.end(); )
    {
        SYS_CHAT_DATA &stSysChat = *itr;

        // 是否已过期
        if(qwNow > stSysChat.qwExpiresTime)
        {
            // 已过期
            m_listSysChatNotExpired.erase(itr++);
        }
        else
        {
            ++itr;
        }
    }

    if(m_listSysChatNotExpired.empty())
    {
        return;
    }

    // 递增当前索引
    ++m_wCurSysChatIdx;
    if(m_wCurSysChatIdx >= m_listSysChatNotExpired.size())
    {
        m_wCurSysChatIdx = 0;
    }

    // 轮流播放未过期的系统公告
    CSysChatNoticeList::iterator itr = m_listSysChatNotExpired.begin();
    advance(itr, m_wCurSysChatIdx);

    SYS_CHAT_DATA &stSysChat = *itr;

    // 将未过期记录存入列表
    ChatInfo stChatInfo;

    memcpy(&stChatInfo.stData, &stSysChat.stData, sizeof(CHAT_DATA));
    stChatInfo.stData.qwChatTime     = SDTimeSecs();
    stChatInfo.stData.dwSerialNumber = AllocChatSerialNum();
    stChatInfo.byVipLevel            = 0;
    stChatInfo.dwFactionIconID       = NULL_FACTION_ICON_ID;

    listChat.push_back(stChatInfo);
}

// 获取活动公告(全服通知)
VOID CChatMgr::TakeActivityChat(CChatList &listChat/*活动公告*/)
{
    UINT64 qwNow = SDTimeSecs();

    for (CActivityChatListItr chatitr = m_listActivityChat.begin(); chatitr != m_listActivityChat.end(); ++chatitr)
    {
        ACTIVITY_CHAT_DATA &stActChat = *chatitr;

        // 发送该活动公告的时间点是否已到
        if (stActChat.qwActivityStartTime > 0 && qwNow < stActChat.qwActivityStartTime)
        {
            continue;
        }

        // 把聊天公告存入列表
        ChatInfo stChatInfo;

        memcpy(&stChatInfo.stData, &stActChat.stData, sizeof(CHAT_DATA));
        stChatInfo.byVipLevel      = 0;
        stChatInfo.dwFactionIconID = NULL_FACTION_ICON_ID;

        listChat.push_back(stChatInfo);
    }
}

// 发送聊天记录列表给玩家，如果聊天记录较多，将拆成几个包来发
VOID CChatMgr::SendChatList(CChatList &listChat, CUser *poUser)
{
    if(NULL == poUser || listChat.empty())
    {
        return;
    }

    CPlayer* poPlayer = poUser->GetPlayer();
    if(NULL == poPlayer)
    {
        return;
    }

    // 须有开启聊天
    if(!poPlayer->GetBuild(EBK_VIEWCHAT))
    {
        return;
    }

    // 是否是机器人
    if(poPlayer->IsRobot())
    {
        return;
    }

    UINT32 dwPlayerID = poPlayer->GetID();

    static PKT_CLIGS_CHAT_NTF stAck;
    memset(&stAck, 0x00, sizeof(PKT_CLIGS_CHAT_NTF));

    // 拆成几个包发出去
    for(CChatList::iterator itr = listChat.begin(); itr != listChat.end(); ++itr)
    {
        ChatInfo &stChatInfo    = *itr;
        CHAT_DATA &stCliChat = stAck.astChatData[stAck.byChatNum];

        //检测好友关系，如果是黑名单
        if (poPlayer->GetFriends())
        {
            if(EFSRT_BLACKLIST == poPlayer->GetFriends()->CkRelationType(stChatInfo.stData.dwPlayerID))
            {
                continue;
            }
        }

        // 检测客户端版本是否支持门派和活动聊天频道
        if(poPlayer->GetCliVer() < CLI_NEW_CHAT_SUPPORT_VERSION)
        {
            if(ECT_FACTION == stChatInfo.stData.byChatType || ECT_ACTIVITY == stChatInfo.stData.byChatType)
            {
                continue;
            }
        }

        // 如果是活动公告类型，判断玩家是否已开启活动中心
        if(ECT_ACTIVITY == stChatInfo.stData.byChatType && NULL == poPlayer->GetBuild(EBK_ACTIVITY))
        {
            continue;
        }

        // 检测该聊天记录是否是玩家本人发出，是的话不再下发
        if (stChatInfo.stData.dwPlayerID == dwPlayerID)
        {
            continue;
        }

        memcpy(&stCliChat, &stChatInfo.stData, sizeof(CHAT_DATA));
        stAck.abyVipLevel[stAck.byChatNum]          = stChatInfo.byVipLevel;
        stAck.abyFactionIconIDList[stAck.byChatNum] = stChatInfo.dwFactionIconID;

        stAck.byChatNum++;

        // 是否需要再多拆一个包
        if(MAX_CHAT_NUM == stAck.byChatNum)
        {
            poUser->SendMsg((CHAR*)&stAck, CLIGS_CHAT_NTF);
            memset(&stAck, 0x00, sizeof(PKT_CLIGS_CHAT_NTF));
        }
    }

    // 把最后的那个消息包也发出去
    if(stAck.byChatNum > 0)
    {
        poUser->SendMsg((CHAR*)&stAck, CLIGS_CHAT_NTF);
    }
}

// 添加世界频道聊天记录
VOID CChatMgr::AddWorldChat(ChatInfo *pstChat)
{
    m_listWorldChat.push_back(pstChat);
    if(m_listWorldChat.size() >= MAX_CHAT_NUM)
    {
        SendChat();
    }
}

UINT16 CChatMgr::AddChatData(CPlayer *poPlayer, CHAT_DATA &stCHAT_DATA, TCHAR* pszErrInfo)
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    // 客户端机制是这样的：如果pszErrInfo不为空，则优先显示pszErrInfo，否则根据错误码进行判断，为了兼容旧客户端，有的错误提示无法用错误码来发送

    // 是否拥有聊天权限
    if(!poPlayer->GetBuild(EBK_SNDCHAT))
    {
        SDStrcpy(pszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("LEVEL_NOT_REACH") .c_str());
        RETURN_OTHER_ERR;
    }

    // 检测玩家是否被禁言
    if(IsSilenced(poPlayer))
    {
        USR_INFO(_SDT("[%s: %d]: player [%u] is Silence \n"), MSG_MARK, stCHAT_DATA.dwPlayerID);
        return ERR_CHAT::ID_CHAT_SILENCE_ERROR;
    }

    // 是否拥有对应频道的聊天权限
    if(ECT_FACTION == stCHAT_DATA.byChatType && NULL == poPlayer->GetFaction())
    {
        return ERR_CHAT::ID_CHAT_NOT_ENTER_FACTION;
    }

    //检测是否有关键字过滤
    TCHAR	*pWordFilter = NULL;
    pWordFilter = CWordFilter::Instance()->VerifyString((const TCHAR *)stCHAT_DATA.abyChatContent);
    if(NULL != pWordFilter)
    {
		//聊天内容中不允许包含<XXX>关键字
		std::vector<string> vecFillMsg;
		vecFillMsg.push_back(pWordFilter);
        SDStrcpy(pszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("WORDFILTER", &vecFillMsg) .c_str());
        return  ERR_CHAT::ID_CHAT_VERIFY_ERROR;
    }

    //gm使用系统发言
    if(IsGm(poPlayer))
    {
        stCHAT_DATA.byChatType = ECT_GM;
    }

    ChatInfo *pstChat = m_oChatDataPool.Alloc();
    if (NULL == pstChat)
    {
        return  ERR_CHAT::ID_CHAT_BUSY_ERROR;
    }

    memcpy(&pstChat->stData, &stCHAT_DATA, sizeof(CHAT_DATA));
    pstChat->byVipLevel      = poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel;
    pstChat->dwFactionIconID = poPlayer->GetFactionIconID();

    switch(stCHAT_DATA.byChatType)
    {
        // 全服频道
    case ECT_USER:
        AddWorldChat(pstChat);
        break;

    case ECT_GM:
        pstChat->dwFactionIconID = NULL_FACTION_ICON_ID;
        AddWorldChat(pstChat);
        break;

        // 门派频道
    case ECT_FACTION:
        CFactionMgr::Instance()->AddChatData(poPlayer->GetID(), *pstChat);
        break;
    }

    return ERR_CHAT::ID_SUCCESS;
}

// 发送聊天 = 世界聊天 + 门派聊天 + 系统公告 + 活动公告
VOID CChatMgr::SendChat()
{
    // 获取需要全服广播的聊天记录 = 世界聊天 + 系统公告 + 活动公告
    CChatList listChat;

    // 系统频道公告记录
    TakeSysChat(listChat);

    // 活动频道公告记录
    TakeActivityChat(listChat);

    // 世界频道聊天记录
    TakeWorldChat(listChat);

    if (!listChat.empty())
    {
        // 遍历所有在线玩家，一一给他们发送聊天信息
        CSessionID2UserMapItr itr = CUserMgr::Instance()->m_mapSessionID2User.begin();
        for (itr; itr != CUserMgr::Instance()->m_mapSessionID2User.end(); itr++)
        {
            CUser* poUser = itr->second;

            // 发送聊天记录列表给玩家，如果聊天记录较多则拆成几个包来发
            SendChatList(listChat, poUser);
        }
    }

    // 发送门派聊天记录
    SendFactionChat();

    // 清理聊天记录
    ClearWorldChat();
    ClearFactionChat();
    ClearActivityChat();
}

// 派发指定门派内的聊天信息
VOID CChatMgr::SendChatInFaction(CFaction *poFaction)
{
    if(NULL == poFaction)
    {
        return;
    }

    // 取出门派聊天记录
    CChatDataList &listChatData = poFaction->GetChat();
    if(listChatData.empty())
    {
        return;
    }

    PKT_CLIGS_CHAT_NTF stAck;

    // 给门派内的玩家一一发送门派频道的聊天信息
    CMapFactionPlayerData &mapPlayers = poFaction->GetFactionPlayers();
    for(CMapFactionPlayerData::iterator itr = mapPlayers.begin(); itr != mapPlayers.end(); ++itr)
    {
        DT_FACTION_PLAYER_DATA &stData = itr->second;

        CUser* poUser = CUserMgr::Instance()->FindUser(stData.dwJoinPlayerID);
        if (NULL == poUser)
        {
            continue;
        }

        CPlayer* poPlayer = poUser->GetPlayer();
        if(NULL == poPlayer)
        {
            continue;
        }

        // 检测客户端版本是否支持门派频道
        if(poPlayer->GetCliVer() < CLI_NEW_CHAT_SUPPORT_VERSION)
        {
            continue;
        }

        // 须有开启聊天、门派
        if(NULL == poPlayer->GetBuild(EBK_FACTION) || NULL == poPlayer->GetBuild(EBK_VIEWCHAT))
        {
            continue;
        }

        TakeFactionChat(stAck, stData.dwJoinPlayerID, poFaction);
        if(stAck.byChatNum > 0)
        {
            poUser->SendMsg((CHAR*)&stAck, CLIGS_CHAT_NTF);
        }
    }

    // 清理聊天记录
    poFaction->ClearChat();
}

VOID CChatMgr::AddFriendsOnline(UINT32 dwPlayerID, vector<UINT32> vecPlayerID)
{
    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if (NULL == poPlayer)
    {
        return;
    }

    PKT_CLIGS_CHAT_NTF stAck;
    memset(&stAck, 0x00, sizeof(PKT_CLIGS_CHAT_NTF));

    CHAT_DATA &stCHAT_DATA = stAck.astChatData[0];

    std::vector<string> vecFillMsg;
    vecFillMsg.clear();
    vecFillMsg.push_back(poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName);

    //输出结果：{好友名字}上线了
    string strReport	   =  CMsgDefMgr::Instance()->GetErrMsg("ONLINE_REPORT", &vecFillMsg);
    stCHAT_DATA.dwPlayerID = 0;
    stCHAT_DATA.byChatType = ECT_SYS;
    stCHAT_DATA.qwChatTime = SDTimeSecs();
    stCHAT_DATA.dwChatLen  = (strReport.size() >= MAX_CHAT_CONTENT_NUM ? MAX_CHAT_CONTENT_NUM - 1 : strReport.size());
    memcpy(stCHAT_DATA.abyChatContent, strReport.c_str(), stCHAT_DATA.dwChatLen);
    stCHAT_DATA.dwSerialNumber = AllocChatSerialNum();

    stAck.byChatNum               = 1;
    stAck.abyVipLevel[0]          = 0;
    stAck.abyFactionIconIDList[0] = NULL_FACTION_ICON_ID;

    for (vector<UINT32>::iterator itr = vecPlayerID.begin(); itr != vecPlayerID.end(); itr++)
    {
        CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(*itr);
        if (NULL == poPlayer)
        {
            continue;
        }
        // 须有开启聊天
        if(NULL == poPlayer->GetBuild(EBK_VIEWCHAT))
        {
            continue;
        }
        poPlayer->SendMsg((CHAR*)&stAck, CLIGS_CHAT_NTF);
    }
}

// 同步各门派内未派发的聊天记录
VOID CChatMgr::SendFactionChat()
{
    CMapFaction &mapFaction = CFactionMgr::Instance()->GetFactions();
    for(CMapFactionItr itr = mapFaction.begin(); itr != mapFaction.end(); ++itr)
    {
        CFaction *poFaction = itr->second;
        SendChatInFaction(poFaction);
    }
}

// 取出最近聊天记录存到消息包
VOID CChatMgr::GetChatData(PKT_CLIGS_OPEN_CHAT_ACK &stAck, CPlayer* poPlayer, UINT32 dwSerialNumber)
{
    CChatList listChat;
    GetChatCacheList(listChat, poPlayer, dwSerialNumber, MAX_CHAT_NUM, FALSE);

    // 存入消息包
    memset(&stAck, 0x00, sizeof(PKT_CLIGS_CHAT_NTF));
    for (CChatList::iterator itr = listChat.begin(); itr != listChat.end() && stAck.byChatNum < MAX_CHAT_NUM; itr++)
    {
        ChatInfo &stChatInfo = *itr;

        memcpy(&stAck.astChatData[stAck.byChatNum], &stChatInfo.stData, sizeof(CHAT_DATA));
        stAck.abyVipLevel[stAck.byChatNum]          = stChatInfo.byVipLevel;
        stAck.abyFactionIconIDList[stAck.byChatNum] = stChatInfo.dwFactionIconID;

        stAck.byChatNum++;
    }
}

// 取出最近聊天记录存到数组
VOID CChatMgr::GetChatData(CHAT_DATA astChatData[100], UINT8& byChatNum)
{
    CChatList listChat;
    GetChatCacheList(listChat, NULL, 0, 100, TRUE);

    byChatNum = 0;
    for (CChatList::iterator itr = listChat.begin(); itr != listChat.end() && byChatNum < 100; itr++)
    {
        ChatInfo &stChatInfo = *itr;
        memcpy(&astChatData[byChatNum++], &stChatInfo.stData, sizeof(CHAT_DATA));
    }
}

// 取出最近的聊天记录存到列表，含世界、门派频道
VOID CChatMgr::GetChatCacheList(CChatList &listChat, CPlayer* poPlayer, UINT32 dwSerialNumber, UINT16 wGetCnt/* 取几条聊天记录 */, BOOL bGm/*是否是gm：gm可以看到被禁言玩家的聊天*/)
{
    listChat.clear();

    // 先把世界频道、门派频道的最近几条聊天记录都取出来放到列表，这个列表根据聊天时间排好序
    GetWorldChatCache(listChat, dwSerialNumber, wGetCnt, bGm);
    GetSysChatCache(listChat, dwSerialNumber, wGetCnt);

    if(NULL != poPlayer)
    {
        // 检测客户端版本是否支持门派和活动频道
        if(poPlayer->GetCliVer() >= CLI_NEW_CHAT_SUPPORT_VERSION)
        {
            GetFactionChatCache(listChat, poPlayer, dwSerialNumber, wGetCnt);

            // 判断玩家是否已开启活动中心，否则不发送活动公告记录
            if(poPlayer->GetBuild(EBK_ACTIVITY))
            {
                GetActivityChatCache(listChat, dwSerialNumber, wGetCnt);
            }
        }
    }

    // 然后只保留最近几条聊天记录
    CChatList::iterator cutItr = listChat.begin();
    UINT32 dwSize = listChat.size();
    if(dwSize > wGetCnt)
    {
        advance(cutItr, dwSize - wGetCnt);
    }

    listChat.erase(listChat.begin(), cutItr);
}

// 取出最近的世界频道聊天记录
VOID CChatMgr::GetWorldChatCache(CChatList &listChat, UINT32 dwSerialNumber, UINT16 wGetCnt/* 取几条聊天记录 */, BOOL bGm/*是否是gm：gm可以看到被禁言玩家的聊天*/)
{
    CChatDataListItr itr = m_listWorldChatPool.begin();

    UINT32 dwSize = m_listWorldChatPool.size();
    if(dwSize > wGetCnt)
    {
        advance(itr, dwSize - wGetCnt);
    }

    // 取出大于指定流水号的最近几条世界频道记录
    for (; itr != m_listWorldChatPool.end(); itr++)
    {
        ChatInfo *pstChat = *itr;

        // 被禁言玩家的聊天将不被下发给普通玩家
        if(FALSE == bGm && IsSilencedByPlayerID(pstChat->stData.dwPlayerID))
        {
            continue;
        }

        if (pstChat->stData.dwSerialNumber > dwSerialNumber)
        {
            listChat.push_back(*pstChat);
        }
    }
}

// 取出最近的系统频道公告记录
VOID CChatMgr::GetSysChatCache(CChatList &listChat, UINT32 dwSerialNumber, UINT16 wGetCnt/* 取几条聊天记录 */)
{
    CSysChatNoticeListItr itr = m_listSysChatNotice.begin();

    UINT32 dwSize = m_listSysChatNotice.size();
    if(dwSize > wGetCnt)
    {
        advance(itr, dwSize - wGetCnt);
    }

    // 取出大于指定流水号的公告记录
    for (; itr != m_listSysChatNotice.end(); itr++)
    {
        SYS_CHAT_DATA &stSysChat = *itr;
        if (stSysChat.stData.dwSerialNumber > dwSerialNumber)
        {
            ChatInfo stChatInfo;

            // 存入消息包
            memcpy(&stChatInfo.stData, &stSysChat.stData, sizeof(CHAT_DATA));
            stChatInfo.byVipLevel      = 0;
            stChatInfo.dwFactionIconID = NULL_FACTION_ICON_ID;

            // 根据聊天时间将聊天记录顺序插入列表中
            chatutil::InsertChatByTime(listChat, stChatInfo);
        }
    }
}

// 取出最近的活动频道公告记录
VOID CChatMgr::GetActivityChatCache(CChatList &listChat, UINT32 dwSerialNumber, UINT16 wGetCnt/* 取几条聊天记录 */)
{
    CActivityChatListItr itr = m_listActivityChatPool.begin();

    UINT32 dwSize = m_listActivityChatPool.size();
    if(dwSize > wGetCnt)
    {
        advance(itr, dwSize - wGetCnt);
    }

    // 取出大于指定流水号的活动频道记录
    for (; itr != m_listActivityChatPool.end(); itr++)
    {
        ACTIVITY_CHAT_DATA &stActChat = *itr;
        if (stActChat.stData.dwSerialNumber > dwSerialNumber)
        {
            ChatInfo stChatInfo;

            // 存入消息包
            memcpy(&stChatInfo.stData, &stActChat.stData, sizeof(CHAT_DATA));
            stChatInfo.byVipLevel      = 0;
            stChatInfo.dwFactionIconID = NULL_FACTION_ICON_ID;

            // 根据聊天时间将聊天记录顺序插入列表中
            chatutil::InsertChatByTime(listChat, stChatInfo);
        }
    }
}

// 取出最近的门派频道聊天记录
VOID CChatMgr::GetFactionChatCache(CChatList &listChat, CPlayer* poPlayer, UINT32 dwSerialNumber, UINT16 wGetCnt/* 取几条聊天记录 */)
{
    CFaction *poFaction = poPlayer->GetFaction();
    if(NULL == poFaction)
    {
        return;
    }

    // 先取出门派频道聊天记录列表
    CChatDataList &listFactionChat = poFaction->GetChatCache();
    CChatDataList::iterator itr = listFactionChat.begin();

    UINT32 dwSize = listFactionChat.size();
    if(dwSize > wGetCnt)
    {
        advance(itr, dwSize - wGetCnt);
    }

    // 取出大于指定流水号的最近几条活动频道记录
    for (; itr != listFactionChat.end(); itr++)
    {
        ChatInfo *pstChat = *itr;
        if (pstChat->stData.dwSerialNumber > dwSerialNumber)
        {
            // 根据聊天时间将聊天记录顺序插入列表中
            chatutil::InsertChatByTime(listChat, *pstChat);
        }
    }
}

BOOL CChatMgr::DspNameHaveSensitiveWord(CPlayer* poPlayer, const CHAR* pszData)
{
    if(NULL == poPlayer)
    {
        return TRUE;
    }

    //gm可以使用敏感词
    if(IsGm(poPlayer))
    {
        return FALSE;
    }

    if(!CWordFilter::Instance()->VerifyDspNameString(pszData))
    {
        return  TRUE;
    }

    return FALSE;
}


//显示名称
string CChatMgr::EraseDspNameSuffix(const TCHAR aszDispName[USERNAME_LEN])
{
    TCHAR aszCopyDispName[USERNAME_LEN];
    SDStrncpy(aszCopyDispName, aszDispName, USERNAME_LEN);

    for(UINT16 i = 0; i < USERNAME_LEN; i++)
    {
        if(aszCopyDispName[i] == '(' || aszCopyDispName[i] == '#')
        {
            aszCopyDispName[i] = '\0';
            break;
        }
    }

    return aszCopyDispName;
}
