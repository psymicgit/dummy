#ifndef CHATMGR_H_
#define CHATMGR_H_

#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdmutex.h>

#include <list>
#include <logic/base/basepropmgr.h>
#include <protocommondata.h>
#include <db/getchatcmd.h>

using namespace SGDP;

class CUser;

struct tagCHAT_DATA;
typedef tagCHAT_DATA CHAT_DATA;

struct ACTIVITY_CHAT_DATA;

typedef struct tagSHILDED
{
    CHAR		szDeviceID[DEVICEID_LEN];
    UINT32	dwUserID;
    UINT32	dwPlayerID;
    tagSHILDED()
    {
        memset(this, 0x00, sizeof(tagSHILDED));
    }
} CSHILDED;


// 发送给玩家的聊天数据
struct ChatInfo
{
    ChatInfo()
        : byVipLevel(0)
        , dwFactionIconID(NULL_FACTION_ICON_ID)
    {
        memset(&stData, 0, sizeof(stData));
    }

    CHAT_DATA  stData;      // 玩家发送的聊天数据
    BYTE       byVipLevel;  // 玩家vip等级
    BYTE       dwFactionIconID; // 玩家所属门派的门派图标ID
};

// 活动聊天公告列表
typedef std::list<ACTIVITY_CHAT_DATA>   	CActivityChatList;
typedef CActivityChatList::iterator        CActivityChatListItr;

// 聊天记录指针列表
typedef std::list<ChatInfo*>		    	CChatDataList;
typedef CChatDataList::iterator		    	CChatDataListItr;

// 聊天列表
typedef std::list<ChatInfo>		    	    CChatList;

// 聊天回应消息包列表
typedef std::vector<PKT_CLIGS_CHAT_NTF>		CChatAckVec;
typedef CChatAckVec::iterator			    CChatAckVecItr;

typedef std::map<UINT32, UINT32>		    CChatSilenceMap;
typedef CChatSilenceMap::iterator           CChatSilenceMapItr;

class CPlayer;
class CFaction;

// 聊天管理器
class CChatMgr
{
public:
    DECLARE_SINGLETON(CChatMgr)

    BOOL Init();

    VOID UnInit();

    VOID ResetChat();

    UINT16	AddChatData(CPlayer *poPlayer, CHAT_DATA &stCHAT_DATA, TCHAR* pszErrInfo);

    // 添加世界频道聊天记录
    VOID AddWorldChat(ChatInfo*);

    UINT32	OnEvent();

    // 将本地公告列表重置为数据库获取到的公告列表
    VOID SetSYSChatDataNoticeData(CSysChatNoticeList &listChatDataNotice);
    VOID SetWordFilterData(CWordFilterDataMap &mapWordFilterData, CWordFilterDataMap &mapDspNameWordFilterData);

    VOID AddWordFilterData(vector<string> &vecWordFilterData);
    VOID DelWordFilterData(vector<string> &vecWordFilterData);

    // 取出最近聊天记录存到消息包
    VOID GetChatData(PKT_CLIGS_OPEN_CHAT_ACK &stAck, CPlayer* poPlayer, UINT32 dwSerialNumber);

    // 取出最近聊天记录存到数组
    VOID GetChatData(CHAT_DATA  astChatData[100], UINT8& byChatNum);


    VOID SetSilenceData(CChatSilenceByDeviceIDMap &mapSilenceByDeviceID,
                        CChatSilenceByUserIDMap &mapSilenceByUserID,
                        CChatSilenceByPlayerIDMap	&mapSilenceByPlayerID);

    VOID SetGMData(CStr32Map mapGmDeviceID, C3232Map mapGmPlayerID);

    // 增加gm聊天
    VOID AddGmData(UINT32 dwPlayerID, string strDeviceID, UINT8 byOptType);

    // 添加系统聊天公告
    VOID AddSysChat(string strChatContent, string strExpiresTime, UINT8 byOptType);

    // 添加活动聊天公告
    VOID AddActivityChat(const string &strChatContent, UINT64 qwActivityStartTime = 0/*活动开始时间，当>0时表示时间到时才发，当=0时表示立即发送*/);

	VOID AddFriendsOnline(UINT32 dwPlayerID, vector<UINT32> vecPlayerID);

    // 构造活动公告数据
    VOID BuildActivityChat(ACTIVITY_CHAT_DATA&, const string &strChatContent, UINT64 qwActivityStartTime = 0/*活动开始时间，当>0时表示时间到时才发，当=0时表示立即发送*/);

    // 添加门派活动公告
    ChatInfo* BuildFactionActivityChat(UINT32 dwFactionID, const string &strChatContent);

    VOID AddSilenceData(vector<string> &vecSilenceByDeviceID,
                        vector<string> &vecSilenceByUserID,
                        vector<string>	&vecSilenceByPlayerID);

    VOID DelSilenceData(vector<string> &vecSilenceByDeviceID,
                        vector<string> &vecSilenceByUserID,
                        vector<string>	&vecSilenceByPlayerID);


    BOOL DspNameHaveSensitiveWord(CPlayer* poPlayer, const CHAR* pszData);//true:有敏感词

    // 移除合区冲突时生成的昵称后缀，比如玩家名称：小桥流水(囧3区)->小桥流水
    string EraseDspNameSuffix(const TCHAR aszDispName[USERNAME_LEN]);

    SSyncSilenceWordfileter2Db& GetSyncSilenceWordfileter2Db()
    {
        return m_oSyncSilenceWordfileter2Db;
    }

    CChatSilenceByDeviceIDMap&	GetSilenceByDeviceID()
    {
        return m_mapSilenceByDeviceID;
    }
    CChatSilenceByUserIDMap&	GetSilenceByUserID()
    {
        return m_mapSilenceByUserID;
    }
    CChatSilenceByPlayerIDMap&	GetSilenceByPlayerID()
    {
        return m_mapSilenceByPlayerID;
    }

    // 派发指定门派内的聊天信息
    VOID SendChatInFaction(CFaction *poFaction);

    // 回收聊天信息内存
    VOID FreeChat(ChatInfo*);

    UINT32 AllocChatSerialNum()
    {
        return ++s_dwSerialNumber;
    }

    // 是否是gm
    BOOL IsGm(CPlayer *poPlayer);

    // 是否是gm
    BOOL IsGm(UINT32 dwPlayerID);

private:
    // 发送聊天 = 世界聊天 + 门派聊天 + 系统公告 + 活动公告
    VOID SendChat();

    // 同步各门派内未派发的聊天记录
    VOID SendFactionChat();

    // 清理世界频道聊天记录
    VOID ClearWorldChat();

    // 清理活动频道公告记录
    VOID ClearActivityChat();

    // 清理各门派的聊天记录
    VOID ClearFactionChat();

    // 取出世界频道的聊天记录
    VOID TakeWorldChat(CChatList &listChat);

    // 取出门派频道的聊天记录存入消息包
    VOID TakeFactionChat(PKT_CLIGS_CHAT_NTF &stAck, UINT32 dwPlayerID, CFaction *poFaction);

    // 获取系统公告
    VOID TakeSysChat(CChatList &listChat);

    // 获取活动公告(全服通知)
    VOID TakeActivityChat(CChatList &listChat/*活动公告*/);

    // 发送聊天记录列表给玩家，如果聊天记录较多，将拆成几个包来发
    VOID SendChatList(CChatList &listChat, CUser *poUser);

    // 取出最近的聊天记录存到列表，含世界、门派频道
    VOID GetChatCacheList(CChatList &listChat, CPlayer* poPlayer, UINT32 dwSerialNumber, UINT16 wGetCnt/* 取几条聊天记录 */, BOOL bGm/*是否是gm：gm可以看到被禁言玩家的聊天*/);

    // 取出最近的世界频道聊天记录
    VOID GetWorldChatCache(CChatList &listChat, UINT32 dwSerialNumber, UINT16 wGetCnt/* 取几条聊天记录 */, BOOL bGm/*是否是gm：gm可以看到被禁言玩家的聊天*/);

    // 取出最近的系统频道公告记录
    VOID GetSysChatCache(CChatList &listChat, UINT32 dwSerialNumber, UINT16 wGetCnt/* 取几条聊天记录 */);

    // 取出最近的活动频道公告记录
    VOID GetActivityChatCache(CChatList &listChat, UINT32 dwSerialNumber, UINT16 wGetCnt/* 取几条聊天记录 */);

    // 取出最近的门派频道聊天记录
    VOID GetFactionChatCache(CChatList &listChat, CPlayer* poPlayer, UINT32 dwSerialNumber, UINT16 wGetCnt/* 取几条聊天记录 */);

    // 是否是gm
    BOOL IsGmByPlayerID(UINT32 dwPlayerID);

    // 是否被禁言
    BOOL IsSilenced(CPlayer *poPlayer);

    // 根据玩家ID检测是否被禁言
    BOOL IsSilencedByPlayerID(UINT32 dwPlayerID);

protected:
    CChatMgr();
    ~CChatMgr();

protected:
    // 取出系统公告列表
    BOOL InitSysChatDB();
    BOOL InitSilenceDB();
    BOOL InitGMInfoDB();

private:
    CChatDataList						m_listWorldChat;         // 当前未发送的世界频道聊天记录
    CChatDataList						m_listWorldChatPool;	 // 世界频道聊天记录历史

    CActivityChatList					m_listActivityChat;      // 当前未发送的活动频道聊天记录
    CActivityChatList                   m_listActivityChatPool;	 // 活动频道聊天数据历史

    UINT16                              m_wCurSysChatIdx;        // 当前轮播的系统公告记录索引
    CSysChatNoticeList  				m_listSysChatNotExpired; // 未过期的系统公告记录
    CSysChatNoticeList				    m_listSysChatNotice;     // 系统公告记录列表 = 未过期 + 已过期

    UINT64								m_qwSendNoticeTime;      // 上次发送系统公告的时间
    UINT64								m_qwChatSyncTime;        // 上次从数据库同步聊天记录的时间

    CChatSilenceByDeviceIDMap			m_mapSilenceByDeviceID;
    CChatSilenceByUserIDMap				m_mapSilenceByUserID;
    CChatSilenceByPlayerIDMap			m_mapSilenceByPlayerID;
    CSDObjectPool<ChatInfo, CSDMutex>	m_oChatDataPool; //玩家聊天公告信息

    SSyncSilenceWordfileter2Db			m_oSyncSilenceWordfileter2Db;

    CStr32Map							m_mapGmDeviceID;
    C3232Map							m_mapGmPlayerID;

    static UINT32                       s_dwSerialNumber;        // 流水号
};

#endif
