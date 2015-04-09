
#ifndef _SEND_FLOWER_H_
#define _SEND_FLOWER_H_

#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <logic/base/baseobj.h>
#include <queue>
#include <set>
#include <map>
#include <common/server/utility.h>
#include <list>
using namespace std;


// 好友系统，收花信息(送花者、送的花和数量、送花时间)
struct SRecvFlowerInfo
{
    UINT32 dwSenderID;
    UINT32 dwFlowerNum;
    UINT64 qwSendTime;
};

typedef std::deque<SRecvFlowerInfo> CRecvFlowerLogDeq; // 好友系统，收花队列(容量为20)
typedef CRecvFlowerLogDeq::iterator CRecvFlowerLogDeqIter;


// 好友系统玩家信息
struct SFSPlayerInfo
{
    UINT32           dwPlayerID; //玩家ID
    string           strPlayerName; //玩家名
    UINT16           wMainHeroKindID; //玩家主角角色ID
    UINT8            byIsOnline; //是否在线
};

// 按玩家名排序仿函数
struct strLess
{
    bool operator() (const SFSPlayerInfo& p1, const SFSPlayerInfo& p2) const
    {
        return p1.strPlayerName < p2.strPlayerName;
    }
};

typedef std::set<SFSPlayerInfo, strLess> CFSPlayerSet;
typedef CFSPlayerSet::iterator CFSPlayerSetIter;

typedef std::vector<SFSPlayerInfo> CFSPlayerVec;
typedef CFSPlayerVec::iterator CFSPlayerVecIter;

// 好友系统，送花统计信息
typedef std::map<UINT32, UINT16> CSendFlowerStatMap; // key为送花的数量，value为已经送出的次数
typedef CSendFlowerStatMap::iterator CSendFlowerStatMapIter;

struct SSendFlowerStat
{
    UINT64 qwLastUpdateTime; // 最后更新时间(用于跨天重置)
    CSendFlowerStatMap m_mapSendFlowerStat;
};

// 好友系统，关于某个收花方的送花明细统计信息
typedef std::map<UINT32, SSendFlowerStat> CSendFlowerDetailStatMap; // key为收花方ID
typedef CSendFlowerDetailStatMap::iterator CSendFlowerDetailStatMapIter;


typedef list<DT_RECV_FLOWER_DATA_LOG> CRecvFlowerDataList;
typedef CRecvFlowerDataList::iterator CRecvFlowerDataListItr;

typedef list<DT_RECV_FLOWER_DATA_EX_LOG>	CRecvFlowerDataExList;
typedef CRecvFlowerDataExList::iterator	CRecvFlowerDataExListItr;


typedef std::map<UINT32, UINT32>		CPlayerIDEvenMap;
typedef CPlayerIDEvenMap::iterator		CPlayerIDEvenMapItr;

// 好友系统, 通用玩家set和vector
typedef map<UINT32, UINT64> CPlayerIDMap;
typedef CPlayerIDMap::iterator CPlayerIDMapIter;


typedef std::list<DT_FINALLY_CONTACT_DATA> CFinallyContactList; // 好友系统，最后联系人
typedef CFinallyContactList::iterator CFinallyContactListIter;





class CMultiMapFriends
{
    typedef multimap<UINT32, UINT32>		MultiMapFriends;
    typedef MultiMapFriends::iterator		MultiMapFriendsItr;
    typedef MultiMapFriends::const_iterator	MultiMapFriendsConstItr;
    typedef pair<MultiMapFriendsItr,		MultiMapFriendsItr> FriendsRange;

public:
    CMultiMapFriends()
    {
        m_multiMapFriends.clear();
    }
    ~CMultiMapFriends()
    {
        m_multiMapFriends.clear();
    }

    void Add(UINT32 dwPlayerA, UINT32 dwPlayerB)
    {
        //查找玩家被动好友关系表
        BOOL bFound = FALSE;
        FriendsRange   range = m_multiMapFriends.equal_range(dwPlayerA);
        for(MultiMapFriendsItr itr = range.first; itr != range.second; itr++)
        {
            if (itr->second == dwPlayerB)
            {
                bFound = TRUE;
                break;
            }
        }

        //保存好友的全局关系表
        if (!bFound)
        {
            m_multiMapFriends.insert(make_pair(dwPlayerA, dwPlayerB));
        }
    }

    void earse(UINT32 dwPlayerA, UINT32 dwPlayerB)
    {
        FriendsRange   range = m_multiMapFriends.equal_range(dwPlayerA);
        for(MultiMapFriendsItr itr = range.first; itr != range.second;)
        {
            if (itr->second == dwPlayerB)
            {
                m_multiMapFriends.erase(itr++);
                break;
            }
            else
            {
                itr++;
            }
        }
    }

    void GetFriends(UINT32 dwPlayerID, vector<UINT32> &vecPlayerid)
    {
        FriendsRange   range = m_multiMapFriends.equal_range(dwPlayerID);
        for(MultiMapFriendsItr itr = range.first; itr != range.second; itr++)
        {
            vecPlayerid.push_back(itr->second);
        }
    }

private:
    MultiMapFriends m_multiMapFriends;

};



class CPlayer;
class CFriends: public CBaseObj
{
public:
    CFriends();
    ~CFriends();
public:
    // 初始化相关对象

    BOOL Init(DT_FRIENDS_DATA &stDT_FRIENDS_DATA, CPlayer* poOwner);



    DT_FRIENDS_BASE_DATA& GetDT_FRIENDS_BASE_DATA();

    // 1获取关系玩家列表
    UINT16 GetFriendSysPlayerList(UINT8 byGetListType, UINT8 byPageCapacity, UINT8 byPageNo, UINT32& dwTotalNum, DT_FRIEND_SYS_PLAYER_LIST_CLI& stFriendSysPlayerList);

	// 1获取关系玩家列表
	UINT16 GetFriendSysPlayerList2(UINT8 byGetListType, UINT8 byPageCapacity, UINT8 byPageNo, UINT32& dwTotalNum, DT_FRIEND_SYS_PLAYER_LIST_CLI2& stFriendSysPlayerList);

    // 2删除好友
    UINT16 DelFriend(UINT32 dwFirendPlayerID);

    // 3查找玩家(协议需修改)
    UINT16 SearchPlayer(TCHAR aszPlayerName[USERNAME_LEN], UINT8 byPageCapacity, UINT8 byPageNo, UINT32& dwTotalNum, DT_FRIEND_SYS_PLAYER_LIST_CLI& stFriendSysPlayerList);

	// 3查找玩家(协议需修改)
	UINT16 SearchPlayer2(TCHAR aszPlayerName[USERNAME_LEN], UINT8 byPageCapacity, UINT8 byPageNo, UINT32& dwTotalNum, DT_FRIEND_SYS_PLAYER_LIST_CLI2& stFriendSysPlayerList);


    // 4查看玩家概要信息
    UINT16 ViewPlayerInfo(UINT32 dwPlayerID, PKT_CLIGS_VIEW_PLAYER_ACK& stAck);

    // 5添加好友
    UINT16 AddFriend(UINT32 dwPlayerID);

    // 6打开送鲜花面板
    UINT16 OpenSendFlowerTab(DT_SEND_FLOWER_PROP_LIST_CLI& stSendFlowerPropList, UINT32 dwRecvPlayerID);


    // 7送鲜花
    UINT16 SendFlower(CPlayer* poTargetPlayer, UINT32 dwSendFlowerNum, UINT64& qwRecverFlowerSum);

    // 8获取鲜花信息
    UINT16 GetFlowerByPlayerID(UINT64& qwFlowerSum, UINT32& dwMyFlowerRanking, DT_RECV_FLOWER_LOG_LIST_CLI& stRecvFlowerLogList);


    // 检查玩家间的关系(无视是否在线)
    UINT16 GetRelationType(UINT32 dwRefPlayerID, UINT8& byRelationType);

    // 添加陌生人
    UINT16 AddStranger(UINT32 dwRefPlayerID);

    // 12拉入黑名单
    UINT16 AddToBlackList(CPlayer *poTargetPlayer);

    // 13移除黑名单
    UINT16 MoveFromBlackList(UINT32 dwPlayerID);


    // 14获取发离线消息的玩家列表
    UINT16 GetSendOfflineMsgPlayerList(DT_SEND_OFFLINE_MSG_PLAYER_LIST_CLI& stSendOfflineMsgPlayerList);



    // 读取关于某个玩家把你添加为好友的事件通知(你离线的时候)
    UINT16 ReadOfflineMsg(UINT32 dwAddPlayerID);

    void AddRecvFlowerData(UINT32 dwSendPlayerID, UINT32 dwFlowerNum);

    UINT8 CkRelationType(UINT32 dwPlayerID);

    VOID AddRelationEven(UINT32 dwPlayerID);
    VOID AddUnReadEven(UINT32 dwSendPlayerID);
    VOID DelUnReadEven(UINT32 dwSendPlayerID);
    // 获取玩家的离线消息数量
    UINT16 GetOfflinePrivateMsgNum(UINT16& wOfflineMsgNum);

    //最后连接人
    VOID AddFinallyContact(UINT32 dwPlayerID);


    UINT16 OpenRecvFlowerTab(DT_RECV_FLOWER_LOG_LIST_CLI_EX &stRecvFlowerLogListCLiEx);

    //送花
    UINT16 AcceptFlower(UINT32 dwSendPlayerID, UINT64 qwSendTime, PKT_CLIGS_ACCEPT_FLOWER_ACK &stAck);

    // 6打开送鲜花面板
    UINT16 OpenSendFlowerTabEx(PKT_CLIGS_OPEN_SEND_FLOWER_TAB_EX_ACK &stAck);

    // 7送鲜花
    UINT16 SendFlowerEx(CPlayer* poTargetPlayer, UINT32 dwKindID, PKT_CLIGS_SEND_FLOWER_EX_ACK &stAck);

    //增加送花
    void AddRecvFlowerDataEx(UINT32 dwSendPlayerID, UINT32 dwKindID);

	//检测送花是否可送送花
	BOOL CkRecvFlowerDataEx();

	UINT8 GetUnReceiveNum();

    UINT32  GetFriendsNum() { return m_mapFriend.size(); }
public:
    BOOL GetRelationDataList(DT_RELATION_DATA_LIST &stRelationDataList);

    BOOL GetDataEvenList(DT_DATA_EVEN_LIST &stDT_DATA_EVEN_LIST);

    BOOL GetRecvFlowerLogList(DT_RECV_FLOWER_LOG_LIST &stRecvFlowerLogList);

	void CkRecvFlowerNtf();

protected:
    //
    UINT16 GetFSPlayerInfoByID(UINT32 dwRefPlayerID, SFSPlayerInfo& stFSPlayerInfo);


    // 根据用户名能查找的玩家个数
    UINT16 GetSearchPlayerRowCnt(TCHAR aszPlayerName[USERNAME_LEN], UINT32& dwTotalNum);

    BOOL ChkAndSetSendFlowerInitVal();

	VOID CkDataEveryDay();

public:
    static void GetFriends(UINT32 dwPlayerID, vector<UINT32> &vecPlayerid)
    {
        s_multiMapFriends.GetFriends(dwPlayerID, vecPlayerid);
    }


private:
    CPlayerIDMap			m_mapFriend; // 好友
    CPlayerIDMap			m_mapStranger; // 陌生人
    CPlayerIDMap			m_mapBlackList; // 黑名单
    CPlayerIDEvenMap		m_mapRelationEven;
    CPlayerIDEvenMap		m_mapUnReadEven;
    CRecvFlowerLogDeq		m_deqFlowerLog; // 收花队列(容量为20)
    CRecvFlowerDataList	m_listRecvFlowerData;
    CRecvFlowerDataExList	m_listRecvFlowerDataEx;
    DT_FRIENDS_BASE_DATA	m_stDT_FRIENDS_BASE_DATA;

    CFinallyContactList	m_listFinallyContact;

    static CMultiMapFriends	s_multiMapFriends;


};
DECLARE_CLASS_POOL_MGR(CFriends)

#endif // #ifndef 