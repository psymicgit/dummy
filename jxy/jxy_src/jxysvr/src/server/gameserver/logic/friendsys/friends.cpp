#include "friends.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include "errdef.h"
#include "logic/other/singleprammgr.h"
#include <framework/gsapi.h>
#include "logic/record/recordmgr.h"
#include "logic/lua/luamgr.h"
#include "logic/friendsys/sendflowerpropmgr.h"
#include "logic/friendsys/privatemsgmgr.h"
#include "sddb.h"
#include "logic/player/playerbasedatacachemgr.h"
#include "common/server/utility.h"
#include "logic/player/playermgr.h"
#include "logic/build/build.h"
#include "logic/faction/factionmgr.h"
#include "logic/item/itemmgr.h"
#include "logic/medal/playermedal.h"

const int MAX_PRIVATE_MSG_CACHE_NUM = 50;
const int MAX_STRANGER_NUM			= 10;
const int MAX_BLACKLIST_NUM		= 50;
const int MAX_FRIEND_NUM			= (MAX_RELATION_LIST_ELEM_NUM - MAX_STRANGER_NUM - MAX_BLACKLIST_NUM);


enum em_RecvFlower
{
    em_RECVD = 0,		//已接收
    em_UNRECV = 1,	//未接收
    em_UNNOTIFY = 2,	//未通知
};

//最后连接入的比较规则
bool CmpFCD(const DT_FINALLY_CONTACT_DATA &_X,  const DT_FINALLY_CONTACT_DATA &_Y) // 回调函数
{
    if (_X.qwLastContactTime > _Y.qwLastContactTime)
    {
        return true;
    }
    return false;
}

bool CmpRFR( const DT_RECV_FLOWER_DATA_LOG & _x,  const DT_RECV_FLOWER_DATA_LOG & _y)
{
    if (_x.qwSendTime > _y.qwSendTime)
    {
        return true;
    }
    return false;
}

bool CmpRFRE( const DT_RECV_FLOWER_DATA_EX_LOG & _x,  const DT_RECV_FLOWER_DATA_EX_LOG & _y)
{
    if (_x.qwSendTime < _y.qwSendTime)
    {
        return true;
    }
    return false;
}



IMPLEMENT_CLASS_POOL_MGR(CFriends)


CMultiMapFriends	CFriends::s_multiMapFriends;
CFriends::CFriends()
{
    // 好友系统相关成员变量
    m_mapFriend.clear(); // 好友
    m_mapStranger.clear();	// 陌生人
    m_mapBlackList.clear(); // 黑名单
    m_deqFlowerLog.clear(); // 收花队列(容量为20)
    m_mapFriend.clear(); // 好友
    m_mapStranger.clear();	// 陌生人
    m_mapBlackList.clear(); // 黑名单
    m_deqFlowerLog.clear(); // 收花队列(容量为20)
    memset(&m_stDT_FRIENDS_BASE_DATA, 0 , sizeof(DT_FRIENDS_BASE_DATA));
}

CFriends::~CFriends()
{
	// 好友系统相关成员变量
	m_mapFriend.clear(); // 好友
	m_mapStranger.clear();	// 陌生人
	m_mapBlackList.clear(); // 黑名单
	m_deqFlowerLog.clear(); // 收花队列(容量为20)
	m_mapFriend.clear(); // 好友
	m_mapStranger.clear();	// 陌生人
	m_mapBlackList.clear(); // 黑名单
	m_deqFlowerLog.clear(); // 收花队列(容量为20)
	memset(&m_stDT_FRIENDS_BASE_DATA, 0 , sizeof(DT_FRIENDS_BASE_DATA));
}

VOID CFriends::CkDataEveryDay()
{
	if( !IsToday(m_stDT_FRIENDS_BASE_DATA.qwLastSendFlowerTime) )
	{
		m_stDT_FRIENDS_BASE_DATA.dwSendFlowerTimes = 0;
		m_stDT_FRIENDS_BASE_DATA.qwLastSendFlowerTime = SDTimeSecs();
	}
}

BOOL CFriends::ChkAndSetSendFlowerInitVal()
{
    if (!IsToday(m_stDT_FRIENDS_BASE_DATA.qwLastSendFlowerTime))
    {
        memset(&m_stDT_FRIENDS_BASE_DATA, 0, sizeof(DT_FRIENDS_BASE_DATA));
        m_stDT_FRIENDS_BASE_DATA.qwLastSendFlowerTime = SGDP::SDTimeSecs();
        CSendFlowerPropMap& mapSendFlowerProp = CSendFlowerMgr::Instance()->GetSendFlowerPropMap();

        size_t i = 0;
        for (CSendFlowerPropMapIter iter = mapSendFlowerProp.begin(); iter != mapSendFlowerProp.end() && i < MAX_SEND_FLOWER_DATA_NUM; iter++)
        {
            m_stDT_FRIENDS_BASE_DATA.astListData[i].dwSendFlowerNum = iter->second.dwFlowerNum;
            m_stDT_FRIENDS_BASE_DATA.astListData[i].byUsedSendTimes = 0;
            i++;
        }

        m_stDT_FRIENDS_BASE_DATA.byElemNum = mapSendFlowerProp.size();
    }
    return TRUE;
}


//最后连接人
VOID CFriends::AddFinallyContact(UINT32 dwPlayerID)
{
    for (CFinallyContactListIter itr = m_listFinallyContact.begin(); itr != m_listFinallyContact.end(); itr++)
    {
        if (dwPlayerID == itr->dwPlayerID)
        {
            itr->qwLastContactTime = SDTimeSecs();
            m_listFinallyContact.sort(CmpFCD);
            return;
        }
    }

    DT_FINALLY_CONTACT_DATA stDT_FINALLY_CONTACT_DATA;
    stDT_FINALLY_CONTACT_DATA.dwPlayerID = dwPlayerID;
    stDT_FINALLY_CONTACT_DATA.qwLastContactTime = SDTimeSecs();
    m_listFinallyContact.push_front(stDT_FINALLY_CONTACT_DATA);
    m_listFinallyContact.sort(CmpFCD);

    //超出个数，删除最久那个
    if (m_listFinallyContact.size() > MAX_FINALLY_CONTACT_DATA_NUM)
    {
        m_listFinallyContact.pop_back();
    }
}

DT_FRIENDS_BASE_DATA& CFriends::GetDT_FRIENDS_BASE_DATA()
{
    DT_FINALLY_CONTACT_DATA_LST &stDT_FINALLY_CONTACT_DATA_LST = m_stDT_FRIENDS_BASE_DATA.stFinallyContactDataList;
    memset(&stDT_FINALLY_CONTACT_DATA_LST, 0x00, sizeof(DT_FINALLY_CONTACT_DATA_LST)); //最后联系列表
    for (CFinallyContactListIter itr = m_listFinallyContact.begin(); itr != m_listFinallyContact.end() &&
            stDT_FINALLY_CONTACT_DATA_LST.byFinallyContactNum < MAX_FINALLY_CONTACT_DATA_NUM; itr++)
    {
        memcpy(&stDT_FINALLY_CONTACT_DATA_LST.astFinallyContactData[stDT_FINALLY_CONTACT_DATA_LST.byFinallyContactNum++], &(*itr), sizeof(DT_FINALLY_CONTACT_DATA));
    }

    m_stDT_FRIENDS_BASE_DATA.dwLogNum = 0;
    memset(&m_stDT_FRIENDS_BASE_DATA.astLogData, 0x00, sizeof(m_stDT_FRIENDS_BASE_DATA.astLogData)); //最后联系列表
    CRecvFlowerDataExListItr itr ;
    for ( itr = m_listRecvFlowerDataEx.begin(); itr != m_listRecvFlowerDataEx.end() &&
            m_stDT_FRIENDS_BASE_DATA.dwLogNum < MAX_RECV_FLOWER_LOG_LIST_ELEM_EX_NUM;)
    {
        //超过2天不再下发
        if (SDTimeSecs() - itr->qwSendTime < CSinglePramMgr::Instance()->GetExpiresRecvFlower())
        {
            memcpy(&m_stDT_FRIENDS_BASE_DATA.astLogData[m_stDT_FRIENDS_BASE_DATA.dwLogNum++], &(*itr), sizeof(DT_RECV_FLOWER_DATA_EX_LOG));
            itr++;
        }
        else
        {
            m_listRecvFlowerDataEx.erase(itr++);
        }
    }

    return m_stDT_FRIENDS_BASE_DATA;
}


BOOL CFriends::Init(DT_FRIENDS_DATA &stDT_FRIENDS_DATA, CPlayer* poOwner)
{
    m_poOwner = poOwner;
	memset(&m_stDT_FRIENDS_BASE_DATA, 0, sizeof(DT_FRIENDS_BASE_DATA));
    memcpy(&m_stDT_FRIENDS_BASE_DATA, &stDT_FRIENDS_DATA.stFriendsBaseData, sizeof(DT_FRIENDS_BASE_DATA));
    DT_FINALLY_CONTACT_DATA_LST &stFinallyContactDataList = m_stDT_FRIENDS_BASE_DATA.stFinallyContactDataList;

    for (UINT8 byFinallyContactNum = 0; byFinallyContactNum < stFinallyContactDataList.byFinallyContactNum; byFinallyContactNum++)
    {
        m_listFinallyContact.push_back(stFinallyContactDataList.astFinallyContactData[byFinallyContactNum]);
    }
    m_listFinallyContact.sort(CmpFCD);

    ChkAndSetSendFlowerInitVal();

    UINT32 dwIdx = 0;
    for ( dwIdx = 0; dwIdx < stDT_FRIENDS_DATA.stRecvFlowerLogList.dwLogNum; dwIdx++)
    {
        m_listRecvFlowerData.push_back(stDT_FRIENDS_DATA.stRecvFlowerLogList.astListData[dwIdx]);
    }
    m_listRecvFlowerData.sort(CmpRFR);

    //收花日志
    for ( dwIdx = 0; dwIdx < stDT_FRIENDS_DATA.stFriendsBaseData.dwLogNum; dwIdx++)
    {
        DT_RECV_FLOWER_DATA_EX_LOG &stDT_RECV_FLOWER_DATA_EX_LOG = stDT_FRIENDS_DATA.stFriendsBaseData.astLogData[dwIdx];
        if (0 == stDT_RECV_FLOWER_DATA_EX_LOG.dwSendPlayerID || 0 == stDT_RECV_FLOWER_DATA_EX_LOG.dwKindID)
        {
            continue;
        }
        m_listRecvFlowerDataEx.push_back(stDT_FRIENDS_DATA.stFriendsBaseData.astLogData[dwIdx]);
    }
    m_listRecvFlowerDataEx.sort(CmpRFRE);

    for ( dwIdx = 0; dwIdx < stDT_FRIENDS_DATA.stRelationDataList.dwRelationNum; dwIdx++)
    {
        DT_RELATION_DATA &stDT_RELATION_DATA = stDT_FRIENDS_DATA.stRelationDataList.astListData[dwIdx]; //列表信息
        switch (stDT_RELATION_DATA.byrelation) // (byRelationType关系类型:0好友, 1陌生人, 2黑名单)
        {
        case EFSRT_FRIEND:
            m_mapFriend.insert(make_pair(stDT_RELATION_DATA.dwPlayerIDB, stDT_RELATION_DATA.qwAddTime));
            //保存好友的全局关系表
            s_multiMapFriends.Add(stDT_RELATION_DATA.dwPlayerIDB, m_poOwner->GetID());
            break;
        case EFSRT_STRANGER:
            m_mapStranger.insert(make_pair(stDT_RELATION_DATA.dwPlayerIDB, stDT_RELATION_DATA.qwAddTime));
            break;
        case EFSRT_BLACKLIST:
            m_mapBlackList.insert(make_pair(stDT_RELATION_DATA.dwPlayerIDB, stDT_RELATION_DATA.qwAddTime));
            break;
        default:
            break;
        }
    }

    for ( dwIdx = 0; dwIdx < stDT_FRIENDS_DATA.stDataEvenList.dwRelationNum; dwIdx++)
    {
        DT_RELATION_DATA_EVEN &stDT_RELATION_DATA_EVEN = stDT_FRIENDS_DATA.stDataEvenList.astRelationEvenData[dwIdx]; //列表信息
        m_mapRelationEven[stDT_RELATION_DATA_EVEN.dwPlayerIDA] = 0;
    }

    for ( dwIdx = 0; dwIdx < stDT_FRIENDS_DATA.stDataEvenList.dwUnReadNum; dwIdx++)
    {
        DT_UNREAD_DATA_EVEN &stDT_UNREAD_DATA_EVEN = stDT_FRIENDS_DATA.stDataEvenList.astUnReadEvenData[dwIdx]; //列表信息
        m_mapUnReadEven[stDT_UNREAD_DATA_EVEN.dwSendPid] = stDT_UNREAD_DATA_EVEN.dwUnReadNum;
    }
    return TRUE;
}

// 1获取关系玩家列表
UINT16 CFriends::GetFriendSysPlayerList(UINT8 byGetListType, UINT8 byPageCapacity, UINT8 byPageNo, UINT32& dwTotalNum, DT_FRIEND_SYS_PLAYER_LIST_CLI& stFriendSysPlayerList)
{
    if (0 == byPageCapacity)
    {
        return ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_INVALID_PAGE_CAPACITY; // 无效的分页容量
    }

    CBuild *	poBuild = m_poOwner->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        return ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_SUCCESS;
    }
    poBuild->OnOpen();

    // 好友系统玩家信息
    SFSPlayerInfo stFSPlayerInfo;

    // 在线的玩家
    CFSPlayerSet setOnlinePlayers;

    // 离线的玩家
    CFSPlayerSet setOfflinePlayers;

    // 根据获取列表类型，获取对应的列表
    switch (byGetListType) // (0好友, 1陌生人, 2黑名单)
    {
    case EFSRT_FRIEND:
    {
        // 好友set
        for(CPlayerIDMapIter iter = m_mapFriend.begin(); iter != m_mapFriend.end(); iter++)
        {
            GetFSPlayerInfoByID(iter->first, stFSPlayerInfo);

            if (stFSPlayerInfo.byIsOnline)
            {
                setOnlinePlayers.insert(stFSPlayerInfo);
            }
            else
            {
                setOfflinePlayers.insert(stFSPlayerInfo);
            }
        }
    }
    break;
    case EFSRT_STRANGER:
    {
        // 陌生人set
        for(CPlayerIDMapIter iter = m_mapStranger.begin(); iter != m_mapStranger.end(); iter++)
        {
            GetFSPlayerInfoByID(iter->first, stFSPlayerInfo);

            if (stFSPlayerInfo.byIsOnline)
            {
                setOnlinePlayers.insert(stFSPlayerInfo);
            }
            else
            {
                setOfflinePlayers.insert(stFSPlayerInfo);
            }
        }
    }
    break;
    case EFSRT_BLACKLIST:
    {
        // 黑名单set
        for(CPlayerIDMapIter iter = m_mapBlackList.begin(); iter != m_mapBlackList.end(); iter++)
        {
            GetFSPlayerInfoByID(iter->first, stFSPlayerInfo);

            if (stFSPlayerInfo.byIsOnline)
            {
                setOnlinePlayers.insert(stFSPlayerInfo);
            }
            else
            {
                setOfflinePlayers.insert(stFSPlayerInfo);
            }
        }
    }
    break;
    case EFSRT_FINALLYCONTACT:
    {
        // 黑名单set
        for(CFinallyContactListIter iter = m_listFinallyContact.begin(); iter != m_listFinallyContact.end(); iter++)
        {
            GetFSPlayerInfoByID(iter->dwPlayerID, stFSPlayerInfo);

            if (stFSPlayerInfo.byIsOnline)
            {
                setOnlinePlayers.insert(stFSPlayerInfo);
            }
            else
            {
                setOfflinePlayers.insert(stFSPlayerInfo);
            }
        }
    }
    break;
    default:
        break;
    }

    // 将在线的和不在线的好友集合拼接在一起
    CFSPlayerVec collFSPlayerVec;

    for(CFSPlayerSetIter iter = setOnlinePlayers.begin(); iter != setOnlinePlayers.end(); iter++)
    {
        collFSPlayerVec.push_back(*iter);
    }

    for(CFSPlayerSetIter iter = setOfflinePlayers.begin(); iter != setOfflinePlayers.end(); iter++)
    {
        collFSPlayerVec.push_back(*iter);
    }

    // 好友总数
    dwTotalNum = collFSPlayerVec.size();

    UINT16 wFullPages = dwTotalNum / byPageCapacity;
    UINT16 wRemainder = dwTotalNum % byPageCapacity;
    UINT16 wTotalPages = wFullPages + ((wRemainder > 0) ? 1 : 0);

    if (byPageCapacity > MAX_FRIEND_SYS_PLAYER_LIST_ELEM_NUM)
    {
        return ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_INVALID_PAGE_CAPACITY; // 无效的分页容量
    }

    if (byPageNo > wTotalPages)
    {
        return ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_SUCCESS; // 无相关内容
    }

    UINT16 wStartIdx = (byPageNo - 1) * byPageCapacity;
    UINT16 wEndIdx   = byPageNo * byPageCapacity - 1;

    if (UINT32(wEndIdx + 1) > dwTotalNum)
    {
        wEndIdx = dwTotalNum - 1;
    }

    //CFSPlayerVecIter pos = collFSPlayerVec.begin();
    //CFSPlayerVecIter startPos = std::advance(pos, wStartIdx);
    //CFSPlayerVecIter endPos   = std::advance(pos, wEndIdx);

    stFriendSysPlayerList.dwPlayerNum = wEndIdx - wStartIdx + 1;
    size_t i = 0;
    for(size_t j = wStartIdx; j <= wEndIdx; j++)
    {
        stFriendSysPlayerList.astListData[i].dwPlayerID = collFSPlayerVec[j].dwPlayerID;
        memcpy(stFriendSysPlayerList.astListData[i].aszPlayerName, collFSPlayerVec[j].strPlayerName.c_str(), collFSPlayerVec[j].strPlayerName.length());
        stFriendSysPlayerList.astListData[i].wMainHeroKindID = collFSPlayerVec[j].wMainHeroKindID;
        stFriendSysPlayerList.astListData[i].byIsOnline = collFSPlayerVec[j].byIsOnline;
        i++;
    }

    return ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_SUCCESS;
}


// 1获取关系玩家列表
UINT16 CFriends::GetFriendSysPlayerList2(UINT8 byGetListType, UINT8 byPageCapacity, UINT8 byPageNo, UINT32& dwTotalNum, DT_FRIEND_SYS_PLAYER_LIST_CLI2& stFriendSysPlayerList)
{
    if (0 == byPageCapacity)
    {
        return ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_INVALID_PAGE_CAPACITY; // 无效的分页容量
    }

    CBuild *	poBuild = m_poOwner->GetBuild(EBK_FRIENDS);
    if(NULL == poBuild)
    {
        return ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_SUCCESS;
    }
    poBuild->OnOpen();

    // 好友系统玩家信息
    SFSPlayerInfo stFSPlayerInfo;

    // 在线的玩家
    CFSPlayerSet setOnlinePlayers;

    // 离线的玩家
    CFSPlayerSet setOfflinePlayers;

    // 根据获取列表类型，获取对应的列表
    switch (byGetListType) // (0好友, 1陌生人, 2黑名单)
    {
    case EFSRT_FRIEND:
    {
        // 好友set
        for(CPlayerIDMapIter iter = m_mapFriend.begin(); iter != m_mapFriend.end(); iter++)
        {
            GetFSPlayerInfoByID(iter->first, stFSPlayerInfo);

            if (stFSPlayerInfo.byIsOnline)
            {
                setOnlinePlayers.insert(stFSPlayerInfo);
            }
            else
            {
                setOfflinePlayers.insert(stFSPlayerInfo);
            }
        }
    }
    break;
    case EFSRT_STRANGER:
    {
        // 陌生人set
        for(CPlayerIDMapIter iter = m_mapStranger.begin(); iter != m_mapStranger.end(); iter++)
        {
            GetFSPlayerInfoByID(iter->first, stFSPlayerInfo);

            if (stFSPlayerInfo.byIsOnline)
            {
                setOnlinePlayers.insert(stFSPlayerInfo);
            }
            else
            {
                setOfflinePlayers.insert(stFSPlayerInfo);
            }
        }
    }
    break;
    case EFSRT_BLACKLIST:
    {
        // 黑名单set
        for(CPlayerIDMapIter iter = m_mapBlackList.begin(); iter != m_mapBlackList.end(); iter++)
        {
            GetFSPlayerInfoByID(iter->first, stFSPlayerInfo);

            if (stFSPlayerInfo.byIsOnline)
            {
                setOnlinePlayers.insert(stFSPlayerInfo);
            }
            else
            {
                setOfflinePlayers.insert(stFSPlayerInfo);
            }
        }
    }
    break;
    case EFSRT_FINALLYCONTACT:
    {
        // 黑名单set
        for(CFinallyContactListIter iter = m_listFinallyContact.begin(); iter != m_listFinallyContact.end(); iter++)
        {
            GetFSPlayerInfoByID(iter->dwPlayerID, stFSPlayerInfo);

            if (stFSPlayerInfo.byIsOnline)
            {
                setOnlinePlayers.insert(stFSPlayerInfo);
            }
            else
            {
                setOfflinePlayers.insert(stFSPlayerInfo);
            }
        }
    }
    break;
    default:
        break;
    }

    // 将在线的和不在线的好友集合拼接在一起
    CFSPlayerVec collFSPlayerVec;

    for(CFSPlayerSetIter iter = setOnlinePlayers.begin(); iter != setOnlinePlayers.end(); iter++)
    {
        collFSPlayerVec.push_back(*iter);
    }

    for(CFSPlayerSetIter iter = setOfflinePlayers.begin(); iter != setOfflinePlayers.end(); iter++)
    {
        collFSPlayerVec.push_back(*iter);
    }

    // 好友总数
    dwTotalNum = collFSPlayerVec.size();

    UINT16 wFullPages = dwTotalNum / byPageCapacity;
    UINT16 wRemainder = dwTotalNum % byPageCapacity;
    UINT16 wTotalPages = wFullPages + ((wRemainder > 0) ? 1 : 0);

    if (byPageCapacity > MAX_FRIEND_SYS_PLAYER_LIST_ELEM_NUM)
    {
        return ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_INVALID_PAGE_CAPACITY; // 无效的分页容量
    }

    if (byPageNo > wTotalPages)
    {
        return ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_SUCCESS; // 无相关内容
    }

    UINT16 wStartIdx = (byPageNo - 1) * byPageCapacity;
    UINT16 wEndIdx   = byPageNo * byPageCapacity - 1;

    if (UINT32(wEndIdx + 1) > dwTotalNum)
    {
        wEndIdx = dwTotalNum - 1;
    }

    //CFSPlayerVecIter pos = collFSPlayerVec.begin();
    //CFSPlayerVecIter startPos = std::advance(pos, wStartIdx);
    //CFSPlayerVecIter endPos   = std::advance(pos, wEndIdx);

    stFriendSysPlayerList.dwPlayerNum = wEndIdx - wStartIdx + 1;
    size_t i = 0;
    for(size_t j = wStartIdx; j <= wEndIdx; j++)
    {
        stFriendSysPlayerList.astListData[i].dwPlayerID = collFSPlayerVec[j].dwPlayerID;
        memcpy(stFriendSysPlayerList.astListData[i].aszPlayerName, collFSPlayerVec[j].strPlayerName.c_str(), collFSPlayerVec[j].strPlayerName.length());
        stFriendSysPlayerList.astListData[i].wMainHeroKindID = collFSPlayerVec[j].wMainHeroKindID;
        stFriendSysPlayerList.astListData[i].byIsOnline = collFSPlayerVec[j].byIsOnline;
        stFriendSysPlayerList.astListData[i].byRelation = CkRelationType(collFSPlayerVec[j].dwPlayerID);
        i++;
    }

    return ERR_GET_FRIEND_SYS_PLAYER_LIST::ID_SUCCESS;
}





UINT16 CFriends::GetFSPlayerInfoByID(UINT32 dwRefPlayerID, SFSPlayerInfo& stFSPlayerInfo)
{
    // 1
    stFSPlayerInfo.dwPlayerID = dwRefPlayerID;

    const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwRefPlayerID);
    if (NULL == pstPlayerBaseData)
    {
        return 1; // 找不到相关数据
    }

    // 2
    stFSPlayerInfo.strPlayerName = pstPlayerBaseData->aszDispName;

    // 3
    stFSPlayerInfo.wMainHeroKindID = pstPlayerBaseData->wMainHeroKindID;

    // 4
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwRefPlayerID);
    if (NULL == poPlayer)
    {
        stFSPlayerInfo.byIsOnline = FALSE;
    }
    else
    {
        stFSPlayerInfo.byIsOnline = poPlayer->CkLogin();
    }

    return 0;
}

// 2删除好友
UINT16 CFriends::DelFriend(UINT32 dwFirendPlayerID)
{
    // 2 修改内存数据
    // 从好友集合里删除
    CPlayerIDMapIter iter = m_mapFriend.find(dwFirendPlayerID);
    if (iter != m_mapFriend.end())
    {
        m_mapFriend.erase(dwFirendPlayerID);

        //删除全局被动关系表
        s_multiMapFriends.earse(dwFirendPlayerID, m_poOwner->GetID());
    }
    AddStranger(dwFirendPlayerID);
    return ERR_DEL_FRIEND::ID_SUCCESS;
}

// 3查找玩家
UINT16 CFriends::SearchPlayer(TCHAR aszPlayerName[USERNAME_LEN], UINT8 byPageCapacity, UINT8 byPageNo, UINT32& dwTotalNum, DT_FRIEND_SYS_PLAYER_LIST_CLI& stFriendSysPlayerList)
{
    if (HaveSqlToken(aszPlayerName))
    {
        return ERR_SEARCH_PLAYER::ID_NO_REF_DATA; // 无相关数据
    }

    if (byPageCapacity > MAX_FRIEND_SYS_PLAYER_LIST_ELEM_NUM)
    {
        return ERR_SEARCH_PLAYER::ID_INVALID_PAGE_CAPACITY; // 无效的分页容量
    }

    USR_INFO(_SDT("[%s: %d]:PlayerName:%s!"), MSG_MARK, aszPlayerName);
    dwTotalNum = 0;
    CPlayerID2PlayerBaseDataMapConstItr itr;
    const CPlayerID2PlayerBaseDataMap &mapPlayerID2PlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerID2PlayerBaseDataMap();
    for ( itr = mapPlayerID2PlayerBaseData.begin(); itr != mapPlayerID2PlayerBaseData.end(); itr++)
    {
        if (m_poOwner->GetID() == itr->second->dwID)
        {
            continue;
        }

        BOOL bIsFound = TRUE;
        CC_LOOP_DO
        string strTemp = itr->second->aszDispName;
        string::size_type loc = strTemp.find(aszPlayerName, 0 );
        CC_LOOP_BREAK( loc != string::npos )
        char v_szTemp[56] = {0};
        sprintf(v_szTemp, "%u", itr->second->dwID);
        strTemp = v_szTemp;
        loc = strTemp.find(aszPlayerName, 0 );
        CC_LOOP_BREAK( loc != string::npos )
        bIsFound = FALSE;
        CC_LOOP_WHILE(0);
        //没有查找到，移到下一位
        if(!bIsFound)
            continue;
        dwTotalNum++;
    }

    UINT32 dwLimitOffset = 0;
    dwLimitOffset = (byPageNo - 1) * byPageCapacity;

    if (dwLimitOffset >= dwTotalNum)
    {
        return ERR_SEARCH_PLAYER::ID_SUCCESS; // 无相关数据
    }

    UINT32	dwIdx = 0;
    SFSPlayerInfo stFSPlayerInfo;
    CFSPlayerVec collFSPlayerVec;
    for ( itr = mapPlayerID2PlayerBaseData.begin(); itr != mapPlayerID2PlayerBaseData.end(); itr++)
    {
        if (m_poOwner->GetID() == itr->second->dwID)
        {
            continue;
        }

        BOOL bIsFound = TRUE;
        CC_LOOP_DO
        string strTemp = itr->second->aszDispName;
        std::string::size_type loc = strTemp.find(aszPlayerName, 0 );
        CC_LOOP_BREAK( loc != string::npos )
        char v_szTemp[56] = {0};
        sprintf(v_szTemp, "%u", itr->second->dwID);
        strTemp = v_szTemp;
        loc = strTemp.find(aszPlayerName, 0 );
        CC_LOOP_BREAK( loc != string::npos )
        bIsFound = FALSE;
        CC_LOOP_WHILE(0);

        //没有找到，直接跳过
        CC_LOOP_CONTINUE(!bIsFound)

        dwIdx++;
        if (dwIdx <= dwLimitOffset)
        {
            continue;
        }
        else if (dwIdx <= (dwLimitOffset + byPageCapacity))
        {
            GetFSPlayerInfoByID(itr->second->dwID, stFSPlayerInfo);
            collFSPlayerVec.push_back(stFSPlayerInfo);
        }
        else
        {
            break;
        }
    }

    stFriendSysPlayerList.dwPlayerNum = collFSPlayerVec.size();
    size_t i = 0;
    for(CFSPlayerVecIter iter = collFSPlayerVec.begin(); iter != collFSPlayerVec.end(); iter++)
    {
        stFriendSysPlayerList.astListData[i].dwPlayerID = iter->dwPlayerID;
        memcpy(stFriendSysPlayerList.astListData[i].aszPlayerName, iter->strPlayerName.c_str(), iter->strPlayerName.length());
        stFriendSysPlayerList.astListData[i].wMainHeroKindID = iter->wMainHeroKindID;
        stFriendSysPlayerList.astListData[i].byIsOnline = iter->byIsOnline;
        i++;
    }

    return ERR_SEARCH_PLAYER::ID_SUCCESS;
}



// 3查找玩家
UINT16 CFriends::SearchPlayer2(TCHAR aszPlayerName[USERNAME_LEN], UINT8 byPageCapacity, UINT8 byPageNo, UINT32& dwTotalNum, DT_FRIEND_SYS_PLAYER_LIST_CLI2& stFriendSysPlayerList)
{
    if (HaveSqlToken(aszPlayerName))
    {
        return ERR_SEARCH_PLAYER::ID_NO_REF_DATA; // 无相关数据
    }

    if (byPageCapacity > MAX_FRIEND_SYS_PLAYER_LIST_ELEM_NUM)
    {
        return ERR_SEARCH_PLAYER::ID_INVALID_PAGE_CAPACITY; // 无效的分页容量
    }

    USR_INFO(_SDT("[%s: %d]:PlayerName:%s!"), MSG_MARK, aszPlayerName);
    dwTotalNum = 0;
    CPlayerID2PlayerBaseDataMapConstItr itr;
    const CPlayerID2PlayerBaseDataMap &mapPlayerID2PlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerID2PlayerBaseDataMap();
    for ( itr = mapPlayerID2PlayerBaseData.begin(); itr != mapPlayerID2PlayerBaseData.end(); itr++)
    {
        if (m_poOwner->GetID() == itr->second->dwID)
        {
            continue;
        }

        BOOL bIsFound = TRUE;
        CC_LOOP_DO
        string strTemp = itr->second->aszDispName;
        string::size_type loc = strTemp.find(aszPlayerName, 0 );
        CC_LOOP_BREAK( loc != string::npos )
        char v_szTemp[56] = {0};
        sprintf(v_szTemp, "%u", itr->second->dwID);
        strTemp = v_szTemp;
        loc = strTemp.find(aszPlayerName, 0 );
        CC_LOOP_BREAK( loc != string::npos )
        bIsFound = FALSE;
        CC_LOOP_WHILE(0);
        //没有查找到，移到下一位
        if(!bIsFound)
            continue;
        dwTotalNum++;
    }

    UINT32 dwLimitOffset = 0;
    dwLimitOffset = (byPageNo - 1) * byPageCapacity;

    if (dwLimitOffset >= dwTotalNum)
    {
        return ERR_SEARCH_PLAYER::ID_SUCCESS; // 无相关数据
    }

    UINT32	dwIdx = 0;
    SFSPlayerInfo stFSPlayerInfo;
    CFSPlayerVec collFSPlayerVec;
    for ( itr = mapPlayerID2PlayerBaseData.begin(); itr != mapPlayerID2PlayerBaseData.end(); itr++)
    {
        if (m_poOwner->GetID() == itr->second->dwID)
        {
            continue;
        }

        BOOL bIsFound = TRUE;
        CC_LOOP_DO
        string strTemp = itr->second->aszDispName;
        std::string::size_type loc = strTemp.find(aszPlayerName, 0 );
        CC_LOOP_BREAK( loc != string::npos )
        char v_szTemp[56] = {0};
        sprintf(v_szTemp, "%u", itr->second->dwID);
        strTemp = v_szTemp;
        loc = strTemp.find(aszPlayerName, 0 );
        CC_LOOP_BREAK( loc != string::npos )
        bIsFound = FALSE;
        CC_LOOP_WHILE(0);

        //没有找到，直接跳过
        CC_LOOP_CONTINUE(!bIsFound)

        dwIdx++;
        if (dwIdx <= dwLimitOffset)
        {
            continue;
        }
        else if (dwIdx <= (dwLimitOffset + byPageCapacity))
        {
            GetFSPlayerInfoByID(itr->second->dwID, stFSPlayerInfo);
            collFSPlayerVec.push_back(stFSPlayerInfo);
        }
        else
        {
            break;
        }
    }

    stFriendSysPlayerList.dwPlayerNum = collFSPlayerVec.size();
    size_t i = 0;
    for(CFSPlayerVecIter iter = collFSPlayerVec.begin(); iter != collFSPlayerVec.end(); iter++)
    {
        stFriendSysPlayerList.astListData[i].dwPlayerID = iter->dwPlayerID;
        memcpy(stFriendSysPlayerList.astListData[i].aszPlayerName, iter->strPlayerName.c_str(), iter->strPlayerName.length());
        stFriendSysPlayerList.astListData[i].wMainHeroKindID = iter->wMainHeroKindID;
        stFriendSysPlayerList.astListData[i].byIsOnline = iter->byIsOnline;
        stFriendSysPlayerList.astListData[i].byRelation = CkRelationType(iter->dwPlayerID);
        i++;
    }

    return ERR_SEARCH_PLAYER::ID_SUCCESS;
}


// 4查看玩家概要信息
UINT16 CFriends::ViewPlayerInfo(UINT32 dwPlayerID, PKT_CLIGS_VIEW_PLAYER_ACK& stAck)
{
    if (0 == dwPlayerID)
    {
        RETURN_OTHER_ERR;
    }
    // 1
    stAck.stPlayerGeneralInfo.dwPlayerID = dwPlayerID;

    const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
    if (NULL == pstPlayerBaseData)
    {
        return ERR_VIEW_PLAYER::ID_NO_REF_DATA; // 找不到相关数据
    }

    // 2
    memcpy(stAck.stPlayerGeneralInfo.aszPlayerName, pstPlayerBaseData->aszDispName, USERNAME_LEN);

    // 3
    stAck.stPlayerGeneralInfo.wMainHeroKindID = pstPlayerBaseData->wMainHeroKindID;

    // 4
    stAck.stPlayerGeneralInfo.wPlayerLvl = pstPlayerBaseData->wLevel;

    // 5
    stAck.stPlayerGeneralInfo.dwPlayerPower = pstPlayerBaseData->dwPower;

    // 6
    stAck.stPlayerGeneralInfo.dwRank = pstPlayerBaseData->dwRank;

    // 7
    CPlayerIDMapIter iter = m_mapFriend.find(dwPlayerID);
    if (iter != m_mapFriend.end())
    {
        stAck.stPlayerGeneralInfo.byRelationType = EFSRT_FRIEND;
    }
    else if ((iter = m_mapBlackList.find(dwPlayerID)) != m_mapBlackList.end())
    {
        stAck.stPlayerGeneralInfo.byRelationType = EFSRT_BLACKLIST;
    }
    else
    {
        stAck.stPlayerGeneralInfo.byRelationType = EFSRT_STRANGER;
    }
    //获取鲜花总数
    stAck.stPlayerGeneralInfo.qwFlowerSum = CPlayerBaseDataCacheMgr::Instance()->GetFlower(dwPlayerID);

    UINT32 dwFactionID = 0;
    BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, dwPlayerID);
    if ( bRet )
    {
        CFaction* poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
        if ( NULL != poFaction)
        {
            SDStrncpy(stAck.stPlayerGeneralInfo.aszFactionName, poFaction->GetDT_FACTION_BASE_DATA().aszFactionName, MAX_FACTION_NUM - 1);

            stAck.byFactionIconID = poFaction->GetFactionIconID();
        }
    }
    stAck.stPlayerGeneralInfo.byVipLevel = pstPlayerBaseData->byVipLevel;
	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if ( NULL != poPlayer)
	{
		poPlayer->GetAllCarryHeroAttr(stAck.stAttrInfo, stAck.abyJobType);
		stAck.wHeroNum = stAck.stAttrInfo.wHeroNum;
		CBuild* poBuild = poPlayer->GetBuild(EBK_GODWEAPON);
		if ( NULL != poBuild)
		{
			stAck.byGodWeaponOpen = ESM_YES;
		}
		poBuild = poPlayer->GetBuild(EBK_AWAKEN);
		if ( NULL != poBuild)
		{
			stAck.byAwakenOpen = ESM_YES;
		}
		poBuild = poPlayer->GetBuild(EBK_JEWELRY);
		if ( NULL != poBuild)
		{
			stAck.byJewelryOpen = ESM_YES;
		}
		poBuild = poPlayer->GetBuild(EBK_SOUL);
		if (NULL != poBuild) {
			stAck.bySoulOpen = ESM_YES;
		}
		poBuild = poPlayer->GetBuild(EBK_PET);
		if ( NULL != poBuild)
		{
			stAck.byPetOpen = ESM_YES;
		}

		CPlayerMedal* poPlayerMedal = poPlayer->GetPlayerMedal();
		if ( NULL != poPlayerMedal)
		{
			poPlayerMedal->GetUseMedal(stAck.stMedalInfo);
		}
	}

	
    return ERR_VIEW_PLAYER::ID_SUCCESS;
}

// 5添加好友
UINT16 CFriends::AddFriend(UINT32 dwPlayerID)
{
    if (0 == dwPlayerID)
    {
        RETURN_OTHER_ERR;
    }

    if(m_poOwner->GetID() == dwPlayerID)
    {
        return ERR_ADD_FRIEND::ID_REF_MYSELF;
    }

    if (m_mapFriend.size() >= MAX_FRIEND_NUM)
    {
        return ERR_ADD_FRIEND::ID_FRIEND_NUM_TO_UPPERLIMIT;
    }

    UINT16 wErrCode = ERR_ADD_FRIEND::ID_SUCCESS;
    // 2 修改内存数据
    // 如果在陌生人列表里还能找到该玩家，先从陌生人集合里删除
    CPlayerIDMapIter iter = m_mapStranger.find(dwPlayerID);
    if (iter != m_mapStranger.end())
    {
        m_mapStranger.erase(iter);
    }

    iter = m_mapBlackList.find(dwPlayerID);
    if (iter != m_mapBlackList.end())
    {
        m_mapBlackList.erase(iter);
    }

    // 添加到m_mapFriend
    iter = m_mapFriend.find(dwPlayerID);
    if (iter == m_mapFriend.end())
    {
        m_mapFriend.insert(make_pair(dwPlayerID, SDTimeSecs()));
        //保存好友的全局关系表
        s_multiMapFriends.Add(dwPlayerID, m_poOwner->GetID());
    }
    else
    {
        return ERR_ADD_FRIEND::ID_REF_PLAYER_HAS_BEEN_YOUR_FRIEND;
    }

    // 被添加者是否在线
    BOOL bIsRecverOnline;
    CPlayer* poTargetPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if (NULL == poTargetPlayer)
    {
        bIsRecverOnline = FALSE;
    }
    else
    {
        bIsRecverOnline = poTargetPlayer->CkLogin();

        // 生成添加好友通知
        CFriends *poTargetFriends = poTargetPlayer->GetFriends();
        if (NULL != poTargetFriends)
        {
            poTargetFriends->AddRelationEven(m_poOwner->GetID());
        }
    }

    // 给被添加者推送添加好友通知
    if(bIsRecverOnline)
    {
        PKT_CLIGS_ONLINE_PRIVATE_MSG_NTF stNtf;
        const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_poOwner->GetID());
        if (NULL == pstPlayerBaseData)
        {
            return ERR_SEND_PRIVATE_MSG::ID_SELF_INFO_LOST; // 找不到玩家自己相关数据
        }
        stNtf.dwMsgIdx = 0;
        stNtf.dwSendPlayerID = m_poOwner->GetID();
        memcpy(stNtf.aszSendPlayerName, pstPlayerBaseData->aszDispName, USERNAME_LEN);
        stNtf.wMainHeroKindID = pstPlayerBaseData->wMainHeroKindID;
        stNtf.byRelationType = EFSRT_BEEN_ADD_FRIEND_NOTICE;

        memset(stNtf.aszMsgContent, 0, PRIVATE_MSG_CONTENT_LEN);
        stNtf.qwSendTime = SGDP::SDTimeSecs();

        poTargetPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_ONLINE_PRIVATE_MSG_NTF);
    }
    return wErrCode;
}

// 打开送鲜花面板
UINT16 CFriends::OpenSendFlowerTab(DT_SEND_FLOWER_PROP_LIST_CLI& stSendFlowerPropList, UINT32 dwRecvPlayerID)
{
    CSendFlowerMgr::Instance()->GetSendFlowerProp(stSendFlowerPropList);

    // 检查并更新送鲜花状态数据(每天重置)
    ChkAndSetSendFlowerInitVal();

    // 状态值
    DT_FRIENDS_BASE_DATA& stDT_FRIENDS_BASE_DATA = GetDT_FRIENDS_BASE_DATA();
    stDT_FRIENDS_BASE_DATA.qwLastSendFlowerTime = SGDP::SDTimeSecs();

    // 配置上限值
    CSendFlowerPropMap& mapSendFlowerProp = CSendFlowerMgr::Instance()->GetSendFlowerPropMap();
    size_t i = 0;
    for (CSendFlowerPropMapIter iter = mapSendFlowerProp.begin(); iter != mapSendFlowerProp.end() && i < MAX_SEND_FLOWER_DATA_NUM; iter++)
    {
        stSendFlowerPropList.astListData[i].dwLeftSendTimes  = (iter->second.wSendMaxTimes <= stDT_FRIENDS_BASE_DATA.astListData[i].byUsedSendTimes) ? 0 : (iter->second.wSendMaxTimes - stDT_FRIENDS_BASE_DATA.astListData[i].byUsedSendTimes);
        i++;
    }

    return ERR_OPEN_SEND_FLOWER_TAB::ID_SUCCESS;
}


// 6送鲜花
UINT16 CFriends::SendFlower(CPlayer* poTargetPlayer, UINT32 dwSendFlowerNum, UINT64& qwRecverFlowerSum)
{
    //检测对方是否为自己好友
    if(EFSRT_FRIEND != CkRelationType(poTargetPlayer->GetID()))
    {
        return ERR_SEND_FLOWER::ID_OTHER_NOT_FRIEND; // 无效的送花规格
    }

    // 获取配置值
    CSendFlowerPropMap& mapSendFlowerProp = CSendFlowerMgr::Instance()->GetSendFlowerPropMap();

    CSendFlowerPropMapIter iter = mapSendFlowerProp.find(dwSendFlowerNum);
    if (iter == mapSendFlowerProp.end())
    {
        return ERR_SEND_FLOWER::ID_INVALID_SEND_FLOWER_NUM; // 无效的送花规格
    }

    SSendFlowerProp& stSendFlowerProp = iter->second;

    UINT16 wUsedSendFlowerTimes = 0;
    DT_FRIENDS_BASE_DATA& stDT_FRIENDS_BASE_DATA = GetDT_FRIENDS_BASE_DATA();
    stDT_FRIENDS_BASE_DATA.qwLastSendFlowerTime = SGDP::SDTimeSecs();

    // 获取玩家已用送花次数
    size_t pos = 0;
    for (size_t i = 0; i < MAX_SEND_FLOWER_DATA_NUM; i++)
    {
        if (dwSendFlowerNum == stDT_FRIENDS_BASE_DATA.astListData[i].dwSendFlowerNum)
        {
            wUsedSendFlowerTimes = stDT_FRIENDS_BASE_DATA.astListData[i].byUsedSendTimes;
            pos = i;
            break;
        }
    }

    // 根据配置检查送花次数是否达到上限
    UINT16 wMaxSendTimes = stSendFlowerProp.wSendMaxTimes;
    if (wUsedSendFlowerTimes >= wMaxSendTimes)
    {
        return ERR_SEND_FLOWER::ID_SEND_CHANCE_USEUP; // 送花次数已用完
    }

    // VIP等级要求
    if (m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel < stSendFlowerProp.byNeedVipLvl)
    {
        return ERR_COMMON::ID_VIP_LEVEL_NOT_REACH; // VIP等级不足
    }

    // 元宝或金钱够不够
    if (1 == stSendFlowerProp.wCostKindID) // 花费铜钱
    {
        if (m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin < stSendFlowerProp.dwCostKindIDNum)
        {
            return ERR_COMMON::ID_COIN_NOT_ENOUGH; // 铜钱不足
        }
    }

    if (2 == stSendFlowerProp.wCostKindID) // 花费元宝
    {
        if (m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < stSendFlowerProp.dwCostKindIDNum)
        {
            return ERR_COMMON::ID_GOLD_NOT_ENOUGH; // 元宝不足
        }
    }

    CFriends *poTargetFriends = poTargetPlayer->GetFriends();
    if (NULL == poTargetFriends)
    {
        return ERR_SEND_FLOWER::ID_SUCCESS; // 无效的送花规格
    }

    // 更新数据库(生成收花记录 + 更新对方的鲜花总数)
    poTargetFriends->AddRecvFlowerData(m_poOwner->GetID(), dwSendFlowerNum);

    //获取鲜花总数
    qwRecverFlowerSum = CPlayerBaseDataCacheMgr::Instance()->GetFlower(poTargetPlayer->GetID());


    // 如果自己对于收花方来说是路人甲，那么将自己加到对方的陌生人列表里
    if(EFSRT_OTHER == poTargetFriends->CkRelationType(m_poOwner->GetID()))
    {
        poTargetFriends->AddStranger(m_poOwner->GetID());
    }

    // 已用送花次数加1
    stDT_FRIENDS_BASE_DATA.astListData[pos].byUsedSendTimes += 1;
    //UINT16& wUsedSendFlowerTimes = iter4->second;
    //++wUsedSendFlowerTimes;

    // 扣除铜钱或者元宝
    if (1 == stSendFlowerProp.wCostKindID) // 扣除铜钱
    {
        m_poOwner->DecCoin(stSendFlowerProp.dwCostKindIDNum, CRecordMgr::EDCT_SEND_FLOWER, UINT64(poTargetPlayer->GetID()), UINT64(dwSendFlowerNum));
    }

    if (2 == stSendFlowerProp.wCostKindID) // 扣除元宝
    {
        m_poOwner->DecGold(stSendFlowerProp.dwCostKindIDNum, CRecordMgr::EDGT_SEND_FLOWER, UINT64(poTargetPlayer->GetID()), UINT64(dwSendFlowerNum));
    }

    // 得到境界
    m_poOwner->AddJingJie(stSendFlowerProp.dwGetKindIDNum, CRecordMgr::EAJT_SEND_FLOWER, UINT64(poTargetPlayer->GetID()), UINT64(dwSendFlowerNum));

    return ERR_SEND_FLOWER::ID_SUCCESS;
}

// 7根据玩家ID获取鲜花和收花记录
UINT16 CFriends::GetFlowerByPlayerID(UINT64& qwFlowerSum, UINT32& dwFlowerRanking, DT_RECV_FLOWER_LOG_LIST_CLI& stRecvFlowerLogList)
{
    //获取鲜花总数
    qwFlowerSum = CPlayerBaseDataCacheMgr::Instance()->GetFlower(m_poOwner->GetID());
    dwFlowerRanking = CPlayerBaseDataCacheMgr::Instance()->GetFlowerRanking(m_poOwner->GetID());
    CRecvFlowerDataListItr itr = m_listRecvFlowerData.begin();
    for ( itr = m_listRecvFlowerData.begin(); itr != m_listRecvFlowerData.end() && stRecvFlowerLogList.dwLogNum < MAX_RECV_FLOWER_LOG_LIST_ELEM_NUM; itr++)
    {
        DT_RECV_FLOWER_LOG & astListData = stRecvFlowerLogList.astListData[stRecvFlowerLogList.dwLogNum];
        astListData.dwSendPlayerID = itr->dwSendPlayerID;
        const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(astListData.dwSendPlayerID);
        if (NULL == pstPlayerBaseData)
        {
            continue; // 找不到相关数据
        }
        memcpy(astListData.aszSendPlayerName, pstPlayerBaseData->aszDispName, USERNAME_LEN);
        astListData.dwFlowerNum = itr->dwFlowerNum;
        astListData.qwSendTime = itr->qwSendTime;
        stRecvFlowerLogList.dwLogNum++;
    }
    return ERR_GET_FLOWER_BY_PLAYERID::ID_SUCCESS;
}


// 检查玩家间的关系(无视是否在线)
UINT16 CFriends::GetRelationType(UINT32 dwRefPlayerID, UINT8& byRelationType)
{
    // 玩家在线或者离线缓存直接从内存取数据
    CPlayerIDMapIter iter = m_mapFriend.find(dwRefPlayerID);
    if (iter != m_mapFriend.end())
    {
        byRelationType = EFSRT_FRIEND;
        return 0;
    }

    iter = m_mapStranger.find(dwRefPlayerID);
    if (iter != m_mapStranger.end())
    {
        byRelationType = EFSRT_STRANGER;
        return 0;
    }

    iter = m_mapBlackList.find(dwRefPlayerID);
    if (iter != m_mapBlackList.end())
    {
        byRelationType = EFSRT_BLACKLIST;
        return 0;
    }
    byRelationType = EFSRT_OTHER;

    return 0;
}





// 添加陌生人
UINT16 CFriends::AddStranger(UINT32 dwRefPlayerID)
{
    UINT16 wErrCode = 0;
    // 2 修改内存数据
    // 如果在陌生人列表里还能找到该玩家，先从陌生人集合里删除
    CPlayerIDMapIter iter = m_mapBlackList.find(dwRefPlayerID);
    if (iter != m_mapBlackList.end())
    {
        m_mapBlackList.erase(iter);
    }

    // 从好友集合中删除
    iter = m_mapFriend.find(dwRefPlayerID);
    if (iter != m_mapFriend.end())
    {
        m_mapFriend.erase(iter);

        //删除全局被动关系表
        s_multiMapFriends.earse(dwRefPlayerID, m_poOwner->GetID());
    }

    // 添加到黑名单
    iter = m_mapStranger.find(dwRefPlayerID);
    if (iter == m_mapStranger.end())
    {
        m_mapStranger.insert(make_pair(dwRefPlayerID, SDTimeSecs()));
    }

    if (m_mapStranger.size() > MAX_STRANGER_NUM)
    {
        CPlayerIDMapIter	del = m_mapStranger.end();
        UINT64			qwTime = SDTimeSecs();
        for (iter = m_mapStranger.begin(); iter != m_mapStranger.end(); iter++)
        {
            if (iter->second < qwTime)
            {
                del = iter;
            }
        }
        m_mapStranger.erase(del);
    }

    return wErrCode;
}

// 11拉入黑名单
UINT16 CFriends::AddToBlackList(CPlayer *poTargetPlayer)
{
    if (0 == poTargetPlayer->GetID())
    {
        RETURN_OTHER_ERR;
    }

    if (m_poOwner->GetID() == poTargetPlayer->GetID())
    {
        return ERR_ADD_TO_BLACKLIST::ID_NOT_JOIN_BLACK_SELF;
    }

    if (m_mapBlackList.size() >= MAX_BLACKLIST_NUM )
    {
        return ERR_ADD_TO_BLACKLIST::ID_BLACKLIST_NUM_TO_UPPERLIMIT;
    }
    UINT16	wErrCode = ERR_ADD_TO_BLACKLIST::ID_SUCCESS;
    // 2 修改内存数据
    // 如果在陌生人列表里还能找到该玩家，先从陌生人集合里删除
    CPlayerIDMapIter iter = m_mapStranger.find(poTargetPlayer->GetID());
    if (iter != m_mapStranger.end())
    {
        m_mapStranger.erase(iter);
    }

    // 从好友集合中删除
    iter = m_mapFriend.find(poTargetPlayer->GetID());
    if (iter != m_mapFriend.end())
    {
        m_mapFriend.erase(poTargetPlayer->GetID());

        //删除全局被动关系表
        s_multiMapFriends.earse(poTargetPlayer->GetID(), m_poOwner->GetID());
    }

    // 添加到黑名单
    iter = m_mapBlackList.find(poTargetPlayer->GetID());
    if (iter == m_mapBlackList.end())
    {
        m_mapBlackList.insert(make_pair(poTargetPlayer->GetID(), SDTimeSecs()));
    }
    else
    {
        wErrCode = ERR_ADD_TO_BLACKLIST::ID_REF_PLAYER_ALREADY_IN_YOUR_BLACKLIST;
    }
    return wErrCode;
}

// 12移除黑名单
UINT16 CFriends::MoveFromBlackList(UINT32 dwPlayerID)
{
    if (0 == dwPlayerID)
    {
        RETURN_OTHER_ERR;
    }
    UINT16 wErrCode = 0;
    // 从黑名单集合里删除
    CPlayerIDMapIter iter = m_mapBlackList.find(dwPlayerID);
    if (iter != m_mapBlackList.end())
    {
        m_mapBlackList.erase(dwPlayerID);
    }
    AddStranger(dwPlayerID);
    return wErrCode;
}

// 获取发离线消息的玩家列表
UINT16 CFriends::GetSendOfflineMsgPlayerList(DT_SEND_OFFLINE_MSG_PLAYER_LIST_CLI& stSendOfflineMsgPlayerList)
{
    CPlayerIDEvenMapItr itr;
    for ( itr = m_mapUnReadEven.begin(); itr != m_mapUnReadEven.end() && stSendOfflineMsgPlayerList.dwPlayerNum < MAX_SEND_OFFLINE_MSG_PLAYER_LIST_ELEM_NUM; itr++)
    {
        DT_OFFLINE_MSG_SENDER_INFO &stDT_OFFLINE_MSG_SENDER_INFO = stSendOfflineMsgPlayerList.astListData[stSendOfflineMsgPlayerList.dwPlayerNum];
        stDT_OFFLINE_MSG_SENDER_INFO.dwSendPlayerID = itr->first;
        const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(itr->first);
        if (NULL == pstPlayerBaseData)
        {
            continue;
        }

        memcpy(stDT_OFFLINE_MSG_SENDER_INFO.aszSendPlayerName, pstPlayerBaseData->aszDispName, USERNAME_LEN);
        stDT_OFFLINE_MSG_SENDER_INFO.wMainHeroKindID = pstPlayerBaseData->wMainHeroKindID;
        GetRelationType(stDT_OFFLINE_MSG_SENDER_INFO.dwSendPlayerID, stDT_OFFLINE_MSG_SENDER_INFO.byRelationType);
        stDT_OFFLINE_MSG_SENDER_INFO.wOfflineMsgCnt  = itr->second;
        stSendOfflineMsgPlayerList.dwPlayerNum++;
    }


    // 获取被加为好友事件转成的通知
    for ( itr = m_mapRelationEven.begin(); itr != m_mapRelationEven.end() && stSendOfflineMsgPlayerList.dwPlayerNum < MAX_SEND_OFFLINE_MSG_PLAYER_LIST_ELEM_NUM; itr++)
    {
        DT_OFFLINE_MSG_SENDER_INFO &stDT_OFFLINE_MSG_SENDER_INFO = stSendOfflineMsgPlayerList.astListData[stSendOfflineMsgPlayerList.dwPlayerNum];
        stDT_OFFLINE_MSG_SENDER_INFO.dwSendPlayerID = itr->first;
        const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(itr->first);
        if (NULL == pstPlayerBaseData)
        {
            continue;
        }

        memcpy(stDT_OFFLINE_MSG_SENDER_INFO.aszSendPlayerName, pstPlayerBaseData->aszDispName, USERNAME_LEN);
        stDT_OFFLINE_MSG_SENDER_INFO.wMainHeroKindID = pstPlayerBaseData->wMainHeroKindID;
        stDT_OFFLINE_MSG_SENDER_INFO.byRelationType = EFSRT_BEEN_ADD_FRIEND_NOTICE;
        stDT_OFFLINE_MSG_SENDER_INFO.wOfflineMsgCnt  = itr->second;
        stSendOfflineMsgPlayerList.dwPlayerNum++;
    }

    return ERR_GET_SEND_OFFLINE_MSG_PLAYER_LIST::ID_SUCCESS;
}


// 读取关于某个玩家把你添加为好友的事件通知
UINT16 CFriends::ReadOfflineMsg(UINT32 dwAddPlayerID)
{
    CPlayerIDEvenMapItr itr = m_mapRelationEven.find(dwAddPlayerID);
    if (itr != m_mapRelationEven.end())
    {
        itr->second = 1; //标志为已读
    }
    return ERR_READ_OFFLINE_MSG::ID_SUCCESS;
}

void CFriends::AddRecvFlowerData(UINT32 dwSendPlayerID, UINT32 dwFlowerNum)
{
    DT_RECV_FLOWER_DATA_LOG stDT_RECV_FLOWER_DATA_LOG;
    memset(&stDT_RECV_FLOWER_DATA_LOG, 0x00, sizeof(stDT_RECV_FLOWER_DATA_LOG));
    stDT_RECV_FLOWER_DATA_LOG.dwSendPlayerID = dwSendPlayerID;
    stDT_RECV_FLOWER_DATA_LOG.dwFlowerNum = dwFlowerNum;
    stDT_RECV_FLOWER_DATA_LOG.qwSendTime = SDTimeSecs();
    stDT_RECV_FLOWER_DATA_LOG.byIsRead = 1;
    m_listRecvFlowerData.push_front(stDT_RECV_FLOWER_DATA_LOG);
    if (m_listRecvFlowerData.size() > MAX_RECV_FLOWER_LOG_LIST_ELEM_NUM)
    {
        m_listRecvFlowerData.pop_back();
    }

    m_poOwner->AddFlowerNum(dwFlowerNum);
    CPlayerBaseDataCacheMgr::Instance()->AddFlower(m_poOwner->GetID(), dwFlowerNum);
}

void CFriends::AddRecvFlowerDataEx(UINT32 dwSendPlayerID, UINT32 dwKindID)
{
    DT_RECV_FLOWER_DATA_EX_LOG stDT_RECV_FLOWER_DATA_EX_LOG;
    memset(&stDT_RECV_FLOWER_DATA_EX_LOG, 0x00, sizeof(stDT_RECV_FLOWER_DATA_EX_LOG));
    stDT_RECV_FLOWER_DATA_EX_LOG.dwSendPlayerID = dwSendPlayerID;
    stDT_RECV_FLOWER_DATA_EX_LOG.dwKindID = dwKindID;
    stDT_RECV_FLOWER_DATA_EX_LOG.qwSendTime = SDTimeSecs();
    stDT_RECV_FLOWER_DATA_EX_LOG.byFlag = em_UNNOTIFY;
    m_listRecvFlowerDataEx.push_front(stDT_RECV_FLOWER_DATA_EX_LOG);
    if (m_listRecvFlowerDataEx.size() > MAX_RECV_FLOWER_LOG_LIST_ELEM_EX_NUM)
    {
        m_listRecvFlowerDataEx.pop_back();
    }
    m_listRecvFlowerDataEx.sort(CmpRFRE);
}

//检测送花是否可送送花
BOOL CFriends::CkRecvFlowerDataEx()
{
    CRecvFlowerDataExListItr itr;
    for ( itr = m_listRecvFlowerDataEx.begin(); itr != m_listRecvFlowerDataEx.end();)
    {
        //超过2天不再下发
        if (SDTimeSecs() - itr->qwSendTime < CSinglePramMgr::Instance()->GetExpiresRecvFlower())
        {
            itr++;
        }
        else
        {
            m_listRecvFlowerDataEx.erase(itr++);
        }
    }
    m_listRecvFlowerDataEx.sort(CmpRFRE);
    if (m_listRecvFlowerDataEx.size() >= MAX_RECV_FLOWER_LOG_LIST_ELEM_EX_NUM)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

UINT8 CFriends::GetUnReceiveNum()
{
    UINT8 byUnReceiveNum = 0;
    CRecvFlowerDataExListItr itr;
    for ( itr = m_listRecvFlowerDataEx.begin(); itr != m_listRecvFlowerDataEx.end(); itr++)
    {
        if (em_RECVD != itr->byFlag)
        {
            byUnReceiveNum++;
        }
    }
    return byUnReceiveNum;

}

UINT8 CFriends::CkRelationType(UINT32 dwPlayerID)
{
    CPlayerIDMapIter itr = m_mapFriend.find(dwPlayerID);
    if (itr != m_mapFriend.end())
    {
        return EFSRT_FRIEND;
    }

    itr = m_mapStranger.find(dwPlayerID);
    if (itr != m_mapStranger.end())
    {
        return EFSRT_STRANGER;
    }

    itr = m_mapBlackList.find(dwPlayerID);
    if (itr != m_mapBlackList.end())
    {
        return EFSRT_BLACKLIST;
    }

    return EFSRT_OTHER;
}


VOID CFriends::AddRelationEven(UINT32 dwPlayerIDA)
{
    CPlayerIDEvenMapItr itr = m_mapRelationEven.find(dwPlayerIDA);
    if (itr != m_mapRelationEven.end())
    {
        return;
    }
    m_mapRelationEven[dwPlayerIDA] = 0;
}

VOID CFriends::AddUnReadEven(UINT32 dwSendPlayerID)
{
    CPlayerIDEvenMapItr itr = m_mapUnReadEven.find(dwSendPlayerID);
    if (itr != m_mapUnReadEven.end())
    {
        itr->second++;
    }
    else
    {
        m_mapUnReadEven[dwSendPlayerID] = 1;
    }
}

VOID CFriends::DelUnReadEven(UINT32 dwSendPlayerID)
{
    CPlayerIDEvenMapItr itr = m_mapUnReadEven.find(dwSendPlayerID);
    if (itr != m_mapUnReadEven.end())
    {
        m_mapUnReadEven.erase(itr);
    }
	CPrivateMsgMgr::Instance()->AddReadMsg( dwSendPlayerID, m_poOwner->GetID());
}

// 获取玩家的离线消息数量
UINT16 CFriends::GetOfflinePrivateMsgNum(UINT16& wOfflineMsgNum)
{
    wOfflineMsgNum = 0;
    CPlayerIDEvenMapItr itr;
    for ( itr = m_mapUnReadEven.begin(); itr != m_mapUnReadEven.end(); itr++)
    {
        wOfflineMsgNum += itr->second;
    }
    wOfflineMsgNum += m_mapRelationEven.size();
    return wOfflineMsgNum;
}





BOOL CFriends::GetRelationDataList(DT_RELATION_DATA_LIST &stRelationDataList)
{
    CPlayerIDMapIter iter;

    for (iter = m_mapFriend.begin(); iter != m_mapFriend.end() && stRelationDataList.dwRelationNum < MAX_RELATION_LIST_ELEM_NUM; iter++)
    {
        DT_RELATION_DATA &stDT_RELATION_DATA = stRelationDataList.astListData[stRelationDataList.dwRelationNum];
        stDT_RELATION_DATA.dwPlayerIDB = iter->first;
        stDT_RELATION_DATA.byrelation  = EFSRT_FRIEND;
        stDT_RELATION_DATA.qwAddTime	= iter->second;
        stRelationDataList.dwRelationNum++;
    }

    for (iter = m_mapStranger.begin(); iter != m_mapStranger.end() && stRelationDataList.dwRelationNum < MAX_RELATION_LIST_ELEM_NUM; iter++)
    {
        DT_RELATION_DATA &stDT_RELATION_DATA = stRelationDataList.astListData[stRelationDataList.dwRelationNum];
        stDT_RELATION_DATA.dwPlayerIDB = iter->first;
        stDT_RELATION_DATA.byrelation  = EFSRT_STRANGER;
        stDT_RELATION_DATA.qwAddTime	= iter->second;
        stRelationDataList.dwRelationNum++;
    }

    for (iter = m_mapBlackList.begin(); iter != m_mapBlackList.end() && stRelationDataList.dwRelationNum < MAX_RELATION_LIST_ELEM_NUM; iter++)
    {
        DT_RELATION_DATA &stDT_RELATION_DATA = stRelationDataList.astListData[stRelationDataList.dwRelationNum];
        stDT_RELATION_DATA.dwPlayerIDB = iter->first;
        stDT_RELATION_DATA.byrelation  = EFSRT_BLACKLIST;
        stDT_RELATION_DATA.qwAddTime	= iter->second;
        stRelationDataList.dwRelationNum++;
    }
    return TRUE;
}

BOOL CFriends::GetDataEvenList(DT_DATA_EVEN_LIST &stDT_DATA_EVEN_LIST)
{
    //修改数据库，保存好友通知为已读,
    //对于好友增加通知消息，在数据库获取后直接删除，这种数据并不是很重要。
    CPlayerIDEvenMapItr itr;
    for (itr = m_mapRelationEven.begin(); itr != m_mapRelationEven.end() && stDT_DATA_EVEN_LIST.dwRelationNum < MAX_RELATION_LIST_ELEM_NUM;)
    {
        if (1 == itr->second)
        {
            DT_RELATION_DATA_EVEN &stDT_RELATION_DATA_EVEN = stDT_DATA_EVEN_LIST.astRelationEvenData[stDT_DATA_EVEN_LIST.dwRelationNum++]; //列表信息
            stDT_RELATION_DATA_EVEN.dwPlayerIDA = itr->first;
            m_mapRelationEven.erase(itr++);
        }
        else
        {
            itr++;
        }
    }
    return TRUE;
}

BOOL CFriends::GetRecvFlowerLogList(DT_RECV_FLOWER_LOG_LIST &stRecvFlowerLogList)
{
    CRecvFlowerDataListItr itr = m_listRecvFlowerData.begin();
    for ( itr = m_listRecvFlowerData.begin(); itr != m_listRecvFlowerData.end() && stRecvFlowerLogList.dwLogNum < MAX_RECV_FLOWER_LOG_LIST_ELEM_EX_NUM; itr++)
    {
        DT_RECV_FLOWER_DATA_LOG & stDT_RECV_FLOWER_DATA_LOG = stRecvFlowerLogList.astListData[stRecvFlowerLogList.dwLogNum++];
        memcpy(&stDT_RECV_FLOWER_DATA_LOG, &(*itr), sizeof(DT_RECV_FLOWER_DATA_LOG));
    }
    return TRUE;
}

UINT16 CFriends::OpenRecvFlowerTab(DT_RECV_FLOWER_LOG_LIST_CLI_EX &stRecvFlowerLogListCLiEx)
{
    CRecvFlowerDataExListItr itr;
    for ( itr = m_listRecvFlowerDataEx.begin(); itr != m_listRecvFlowerDataEx.end() && stRecvFlowerLogListCLiEx.byLogNum < MAX_RECV_FLOWER_LOG_LIST_ELEM_EX_NUM;)
    {
        //超过2天不再下发
        if (SDTimeSecs() - itr->qwSendTime < CSinglePramMgr::Instance()->GetExpiresRecvFlower())
        {
            DT_RECV_FLOWER_LOG_CLI_EX &stDT_RECV_FLOWER_LOG_CLI_EX = stRecvFlowerLogListCLiEx.astListData[stRecvFlowerLogListCLiEx.byLogNum++];
            stDT_RECV_FLOWER_LOG_CLI_EX.dwSendPlayerID = itr->dwSendPlayerID;
            const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(itr->dwSendPlayerID);
            if (NULL == pstPlayerBaseData)
            {
                itr++;
                continue;
            }
            memcpy(stDT_RECV_FLOWER_LOG_CLI_EX.aszSendPlayerName, pstPlayerBaseData->aszDispName, USERNAME_LEN);
            stDT_RECV_FLOWER_LOG_CLI_EX.dwKindID	= itr->dwKindID;
            stDT_RECV_FLOWER_LOG_CLI_EX.qwSendTime	= itr->qwSendTime;

            //把未通知的全部改成已通知
            if (em_UNNOTIFY == itr->byFlag)
            {
                itr->byFlag = em_UNRECV;
            }
            stDT_RECV_FLOWER_LOG_CLI_EX.byFlag		= itr->byFlag;
            stDT_RECV_FLOWER_LOG_CLI_EX.qwExpires	= CSinglePramMgr::Instance()->GetExpiresRecvFlower() - (SDTimeSecs() - itr->qwSendTime);

            if (em_UNRECV == itr->byFlag)
            {
                SRecvFlowerEncourage *poRecvFlowerEncourage = CSendFlowerMgr::Instance()->GetRecvFlowerEncourage(itr->dwKindID);
                if (NULL == poRecvFlowerEncourage)
                {
                    itr++;
                    continue;
                }
                JsonPackResItem(poRecvFlowerEncourage->vecJsonEncourageItemInfo, stDT_RECV_FLOWER_LOG_CLI_EX.stResItemData);
            }
            else
            {
                DT_RES_NUM_ITEM_DATA &stResNumItemData = itr->stResNumItemData;
                DT_RES_ITEM_DATA &stResItemData = stDT_RECV_FLOWER_LOG_CLI_EX.stResItemData;
                for (UINT8 byResNum = 0; byResNum < stResNumItemData.byResNum; byResNum++)
                {
                    stResItemData.astResList[byResNum] = stResNumItemData.astResList[byResNum];
                }
                stResItemData.byResNum = stResNumItemData.byResNum;
                for (UINT8 byItemNum = 0; byItemNum < stResNumItemData.byItemNum; byItemNum++)
                {
                    CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(stResNumItemData.astItemList[byItemNum].wKindID, stResNumItemData.astItemList[byItemNum].wNum, stResItemData.astItemList[byItemNum]);
                }
                stResItemData.byItemNum = stResNumItemData.byItemNum;
            }
            itr++;
        }
        else
        {
            m_listRecvFlowerDataEx.erase(itr++);
        }
    }

    if(!IsToday(m_stDT_FRIENDS_BASE_DATA.qwLastRecvFlowerTime))
    {
        m_stDT_FRIENDS_BASE_DATA.dwRecvFlowerTimes = 0;
        m_stDT_FRIENDS_BASE_DATA.qwLastRecvFlowerTime = SDTimeSecs();
    }
    stRecvFlowerLogListCLiEx.byUnReceiveNum = GetUnReceiveNum();
    stRecvFlowerLogListCLiEx.byAlsoReceive = CSinglePramMgr::Instance()->GetEveryDayRecvFlowers() - m_stDT_FRIENDS_BASE_DATA.dwRecvFlowerTimes;
    return 0;
}


UINT16 CFriends::AcceptFlower(UINT32 dwSendPlayerID, UINT64 qwSendTime, PKT_CLIGS_ACCEPT_FLOWER_ACK &stAck)
{


    //检测送花时间
    if(!IsToday(m_stDT_FRIENDS_BASE_DATA.qwLastRecvFlowerTime))
    {
        m_stDT_FRIENDS_BASE_DATA.qwLastRecvFlowerTime = SDTimeSecs();
        m_stDT_FRIENDS_BASE_DATA.dwRecvFlowerTimes = 0;
    }

    if (m_stDT_FRIENDS_BASE_DATA.dwRecvFlowerTimes >= CSinglePramMgr::Instance()->GetEveryDayRecvFlowers())
    {
        return ERR_ACCEPT_FLOWER::ID_TODAY_NO_TIME;
    }

    UINT16 wErrCode = ERR_ACCEPT_FLOWER::ID_REQ_ERR;
    CRecvFlowerDataExListItr itr = m_listRecvFlowerDataEx.begin();
    for ( itr = m_listRecvFlowerDataEx.begin(); itr != m_listRecvFlowerDataEx.end(); itr++)
    {
        if (itr->qwSendTime != qwSendTime || itr->dwSendPlayerID != dwSendPlayerID)
        {
            continue;
        }

        SRecvFlowerEncourage *poRecvFlowerEncourage = CSendFlowerMgr::Instance()->GetRecvFlowerEncourage(itr->dwKindID);
        if (NULL == poRecvFlowerEncourage)
        {
            continue;
        }

        if (em_RECVD == itr->byFlag)
        {
            wErrCode = ERR_ACCEPT_FLOWER::ID_RECEVED;
            continue;
        }

        if(BAG_FULL_ERR == JsonGetResItemVec(poRecvFlowerEncourage->vecJsonEncourageItemInfo, m_poOwner, stAck.stGainResItemData, stAck.stRsyncResItemData, itr->stResNumItemData, CRecordMgr::EAIT_SENDFLOWER))
        {
            wErrCode = ERR_ACCEPT_FLOWER::ID_BAG_FULL;
            break;
        }
        itr->byFlag = em_RECVD;
        if(!IsToday(m_stDT_FRIENDS_BASE_DATA.qwLastRecvFlowerTime))
        {
            m_stDT_FRIENDS_BASE_DATA.dwRecvFlowerTimes = 0;
            m_stDT_FRIENDS_BASE_DATA.qwLastRecvFlowerTime = SDTimeSecs();
        }
        m_stDT_FRIENDS_BASE_DATA.dwRecvFlowerTimes++;
        wErrCode = ERR_ACCEPT_FLOWER::ID_SUCCESS;
        break;
    }

    m_listRecvFlowerDataEx.sort(CmpRFRE);
    OpenRecvFlowerTab(stAck.stRecvFlowerLogListCLiEx);
    return wErrCode;
}

// 打开送鲜花面板
UINT16 CFriends::OpenSendFlowerTabEx(PKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK &stAck)
{
	//检查送花时间
	CkDataEveryDay();
    int a = sizeof(PKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK);
    CRecvFlowerPropVector &vecRecvFlowerProp = CSendFlowerMgr::Instance()->GetCRecvFlowerPropVector();
    for (CRecvFlowerPropVectorItr itr = vecRecvFlowerProp.begin(); itr != vecRecvFlowerProp.end(); itr++)
    {
        DT_ITEM_FLOWER &stDT_ITEM_FLOWER = stAck.astItemList[stAck.byItemNum++];
        SRecvFlowerEncourage *poRecvFlowerEncourage = CSendFlowerMgr::Instance()->GetRecvFlowerEncourage(itr->wKindID);
        if (NULL == poRecvFlowerEncourage)
        {
            continue;
        }
        JsonPackResItem(poRecvFlowerEncourage->vecJsonEncourageItemInfo, stDT_ITEM_FLOWER.stResItemData);
        CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(itr->wKindID, m_poOwner->GetBag().GetGoodsPileNum(itr->wKindID), stDT_ITEM_FLOWER.stItem);
    }

    // 好友set
    for(CPlayerIDMapIter itr = m_mapFriend.begin(); itr != m_mapFriend.end(); itr++)
    {
        DT_FRIENDS_CLI &stDT_FRIENDS_CLI = stAck.astFriendsList[stAck.byFriendsNum++];
        stDT_FRIENDS_CLI.dwPlayerID = itr->first;
        const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(itr->first);
        if (NULL == pstPlayerBaseData)
        {
            continue;
        }
        memcpy(stDT_FRIENDS_CLI.aszDispName, pstPlayerBaseData->aszDispName, USERNAME_LEN);
        stDT_FRIENDS_CLI.byCareerID = (UINT8)pstPlayerBaseData->wMainHeroKindID;
        UINT32 dwFactionID = 0;
        BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, pstPlayerBaseData->dwID);
        if ( !bRet )
        {
            continue;
        }
        CFaction* poFaction = CFactionMgr::Instance()->GetFaction(dwFactionID);
        if ( NULL != poFaction)
        {
            SDStrncpy(stDT_FRIENDS_CLI.aszFactionName, poFaction->GetDT_FACTION_BASE_DATA().aszFactionName, MAX_FACTION_NUM - 1);
        }

        CFaction* poTargetFaction = m_poOwner->GetFaction();
        if (NULL == poTargetFaction)
        {
            continue;
        }
        if (dwFactionID == poTargetFaction->GetDT_FACTION_BASE_DATA().dwFactionID)
        {
            stDT_FRIENDS_CLI.bySameFaction = 1;
        }
        else
        {
            stDT_FRIENDS_CLI.bySameFaction = 0;
        }
    }

    stAck.byUnReceiveNum = GetUnReceiveNum();
    UINT32 dwNextFlowerNum = 0;
    SSendFlowerEncourage *poEncourage = NULL;
    poEncourage = CSendFlowerMgr::Instance()->GetNextSendFlowerEncourage(m_stDT_FRIENDS_BASE_DATA.dwSendFlowerTimes, dwNextFlowerNum);
    if (NULL == poEncourage)
    {
		RETURN_OTHER_ERR;
    }
	SSendFlowerEncourage* poMaxEncourage = CSendFlowerMgr::Instance()->GetMaxSendFlowerEncourage();
	if ( NULL == poMaxEncourage )
	{
		RETURN_OTHER_ERR;
	}
	
	//当送鲜花超过上限
	if ( poMaxEncourage->dwFlowerNum <= m_stDT_FRIENDS_BASE_DATA.dwSendFlowerTimes)
	{
		JsonPackResItem(poEncourage->vecJsonEncourageItemInfo, stAck.stResItemData);
		stAck.byAgainNum = poMaxEncourage->dwFlowerNum - (m_stDT_FRIENDS_BASE_DATA.dwSendFlowerTimes % poMaxEncourage->dwFlowerNum);
		return ERR_OPEN_SEND_FLOWER_TAB::ID_SUCCESS;
	}
    JsonPackResItem(poEncourage->vecJsonEncourageItemInfo, stAck.stResItemData);
    stAck.byAgainNum = dwNextFlowerNum > m_stDT_FRIENDS_BASE_DATA.dwSendFlowerTimes ? dwNextFlowerNum - m_stDT_FRIENDS_BASE_DATA.dwSendFlowerTimes : 0;
    return ERR_OPEN_SEND_FLOWER_TAB::ID_SUCCESS;
}

UINT16 CFriends::SendFlowerEx(CPlayer* poTargetPlayer, UINT32 dwKindID, PKT_CLIGS_SEND_FLOWER_EX_ACK &stAck)
{
	//检查送花时间
	CkDataEveryDay();

    if(EFSRT_FRIEND != CkRelationType(poTargetPlayer->GetID()))
    {
        return ERR_SEND_FLOWER::ID_OTHER_NOT_FRIEND; // 无效的送花规格
    }

    CFriends *poTargetFriends  = poTargetPlayer->GetFriends();
    if(NULL == poTargetFriends)
    {
        return ERR_SEND_FLOWER::ID_OTHER_NOT_OPEN_FRIENDS;
    }

    //获取丹对应的物品ID
    SRecvFlowerEncourage*	pstRecvFlowerEncourage	= NULL;
    pstRecvFlowerEncourage = CSendFlowerMgr::Instance()->GetRecvFlowerEncourage(dwKindID);
    if (NULL == pstRecvFlowerEncourage)
    {
        return ERR_SEND_FLOWER::ID_ITEM_TYPE_ERR;
    }

    //获取背包中的物品属性
    CGoods*	poGoods		= NULL;
    poGoods	= m_poOwner->GetBag().GetGoodsItem(dwKindID);
    if(NULL == poGoods)
    {
        return ERR_SEND_FLOWER::ID_BAG_NOT_EXIT;
    }

    SGoodsProp *poGoodsProp = poGoods->GetGoodsProp();
    if(NULL == poGoodsProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pile num is 0, but exist!"), MSG_MARK);//异常。。。
        return ERR_SEND_FLOWER::ID_ITEM_TYPE_ERR;
    }

    //检测等级丹药剩余个数
    if(0 == poGoods->GetCurPileNum())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pile num is 0, but exist!"), MSG_MARK);//异常。。。
        m_poOwner->OnDeleteItem(poGoods);//清除
        return ERR_SEND_FLOWER::ID_BAG_NOT_EXIT;
    }

    //在对方信息中加入送花启记录
    if(NULL != poTargetFriends && FALSE == poTargetFriends->CkRecvFlowerDataEx())
    {
        return ERR_SEND_FLOWER::ID_OTHER_RECV_FLOWER_FULL;
    }

    UINT32 dwNum = 0;
    SSendFlowerEncourage *poEncourage = CSendFlowerMgr::Instance()->GetSendFlowerEncourage(m_stDT_FRIENDS_BASE_DATA.dwSendFlowerTimes + 1);
    if (NULL != poEncourage)
    {
		DT_RES_NUM_ITEM_DATA stResNumItemData = {0};
		if(BAG_FULL_ERR == JsonGetResItemVec(poEncourage->vecJsonEncourageItemInfo, m_poOwner, stAck.stGainResItemData, stAck.stRsyncResItemData, stResNumItemData, CRecordMgr::EAIT_SENDFLOWER))
		{
			return ERR_SEND_FLOWER::ID_BAG_FULL;
		}
    }

    m_stDT_FRIENDS_BASE_DATA.dwSendFlowerTimes++;
    SSendFlowerEncourage *poMaxEncourage = CSendFlowerMgr::Instance()->GetMaxSendFlowerEncourage();
    if (NULL == poMaxEncourage)
    {
        RETURN_OTHER_ERR;
    }

    //扣除操作
    poGoods->DecPileNum(1, CRecordMgr::EDIT_SEND_FLOWER);

    if(0 == poGoods->GetCurPileNum())
    {
        m_poOwner->OnDeleteItem(poGoods);		//清除叠加数为0的丹药物品
    }

    //在对方信息中加入送花启记录
    if(NULL != poTargetFriends)
    {
        poTargetFriends->AddRecvFlowerDataEx(m_poOwner->GetID(), dwKindID);
    }


    // 给被添加者推送添加好友通知
    if(poTargetPlayer->CkLogin())
    {
        PKT_CLIGS_ONLINE_PRIVATE_MSG_NTF stNtf;
        const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_poOwner->GetID());
        if (NULL != pstPlayerBaseData)
        {
            stNtf.dwMsgIdx = 0;
            stNtf.dwSendPlayerID = m_poOwner->GetID();
            memcpy(stNtf.aszSendPlayerName, pstPlayerBaseData->aszDispName, USERNAME_LEN);
            stNtf.wMainHeroKindID = pstPlayerBaseData->wMainHeroKindID;
            stNtf.byRelationType = EFSRT_BEEN_SEND_FLOWER_NOTICE;
            memset(stNtf.aszMsgContent, 0, PRIVATE_MSG_CONTENT_LEN);
            stNtf.qwSendTime = SGDP::SDTimeSecs();
            poTargetPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_ONLINE_PRIVATE_MSG_NTF);
        }
    }

    //同步数据
    CRecvFlowerPropVector &vecRecvFlowerProp = CSendFlowerMgr::Instance()->GetCRecvFlowerPropVector();
    for (CRecvFlowerPropVectorItr itr = vecRecvFlowerProp.begin(); itr != vecRecvFlowerProp.end(); itr++)
    {
        DT_ITEM_DATA_CLI2 &stDT_ITEM_DATA_CLI2 = stAck.astItemList[stAck.byItemNum++];
        CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(itr->wKindID, m_poOwner->GetBag().GetGoodsPileNum(itr->wKindID), stAck.astItemList[stAck.byItemNum++]);
    }

    UINT32 dwNextFlowerNum = 0;
    poEncourage = CSendFlowerMgr::Instance()->GetNextSendFlowerEncourage(m_stDT_FRIENDS_BASE_DATA.dwSendFlowerTimes, dwNextFlowerNum);
    if (NULL == poEncourage)
    {
		RETURN_OTHER_ERR;
    }
	//当送鲜花超过上限
	if ( poMaxEncourage->dwFlowerNum <= m_stDT_FRIENDS_BASE_DATA.dwSendFlowerTimes)
	{
		JsonPackResItem(poEncourage->vecJsonEncourageItemInfo, stAck.stResItemData);
		stAck.byAgainNum = poMaxEncourage->dwFlowerNum - (m_stDT_FRIENDS_BASE_DATA.dwSendFlowerTimes % poMaxEncourage->dwFlowerNum);
		return ERR_OPEN_SEND_FLOWER_TAB::ID_SUCCESS;
	}
    JsonPackResItem(poEncourage->vecJsonEncourageItemInfo, stAck.stResItemData);
    stAck.byAgainNum = dwNextFlowerNum > m_stDT_FRIENDS_BASE_DATA.dwSendFlowerTimes ? dwNextFlowerNum - m_stDT_FRIENDS_BASE_DATA.dwSendFlowerTimes : 0;
    return ERR_SEND_FLOWER::ID_SUCCESS;
}

void CFriends::CkRecvFlowerNtf()
{
    CRecvFlowerDataExListItr itr;
    for ( itr = m_listRecvFlowerDataEx.begin(); itr != m_listRecvFlowerDataEx.end();)
    {
        //超过2天不再下发
        if (SDTimeSecs() - itr->qwSendTime < CSinglePramMgr::Instance()->GetExpiresRecvFlower())
        {
            //把未通知的全部改成已通知
            if (em_UNNOTIFY != itr->byFlag)
            {
                itr++;
                continue;
            }

            const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(itr->dwSendPlayerID);
            if (NULL == pstPlayerBaseData)
            {
                itr++;
                continue;
            }
            PKT_CLIGS_ONLINE_PRIVATE_MSG_NTF stNtf;
            stNtf.dwMsgIdx = 0;
            stNtf.dwSendPlayerID = itr->dwSendPlayerID;
            memcpy(stNtf.aszSendPlayerName, pstPlayerBaseData->aszDispName, USERNAME_LEN);
            stNtf.wMainHeroKindID = pstPlayerBaseData->wMainHeroKindID;
            stNtf.byRelationType = EFSRT_BEEN_SEND_FLOWER_NOTICE;
            memset(stNtf.aszMsgContent, 0, PRIVATE_MSG_CONTENT_LEN);
            stNtf.qwSendTime = SGDP::SDTimeSecs();
            m_poOwner->SendMsg((const CHAR*)(&stNtf), CLIGS_ONLINE_PRIVATE_MSG_NTF);
            itr++;
        }
        else
        {
            m_listRecvFlowerDataEx.erase(itr++);
        }
    }
}


