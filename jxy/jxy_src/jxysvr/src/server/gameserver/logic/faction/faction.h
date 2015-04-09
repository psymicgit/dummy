
#ifndef _FACTION_H_
#define _FACTION_H_

#include <protocol/server/protocommondata.h>
#include <sdsingleton.h>
#include <sdmutex.h>
#include <sdobjectpool.h>
#include <logic/base/basedefine.h>
#include <logic/rareitemroom/rareitemroompropmgr.h>
#include <logic/gymnasium/gymnasiumpropmgr.h>
#include <protocol/server/protocligs.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <map>
#include <deque>
#include <list>
#include <set>
#include "logic/base/basedefine.h"
#include "logic/gvg/gvgdef.h"

// 门派频道聊天历史保留个数
#define MAX_FACTION_CHAT_HISTORY (MAX_CHAT_NUM * 3)

class CPlayer;
class CFactionGvG;

struct ChatInfo;

using namespace SGDP;
using namespace std;

typedef set<UINT32> CSetHotSpringPlayerID;
typedef set<UINT32>::iterator CHotSpringPlayerIDItr;

//记录用户数据
typedef map<UINT32, DT_FACTION_PLAYER_DATA>	CMapFactionPlayerData;
typedef CMapFactionPlayerData::iterator			CMapFactionPlayerDataItr;


//记录用户日志
typedef deque<DT_FACTION_LOG_DATA>			CDequeFactionLogData;
typedef CDequeFactionLogData::iterator		CDequeFactionLogDataItr;

typedef map<UINT32, UINT32>						CMapFactionPlayerID;
typedef CMapFactionPlayerID::iterator				CMapFactionPlayerIDItr;

typedef multimap<UINT32, UINT32>					CMultiMapApplyFactionPlayer;
typedef CMultiMapApplyFactionPlayer::iterator		CMultiMapApplyFactionPlayerItr;
typedef CMultiMapApplyFactionPlayer::const_iterator	CMultiMapApplyFactionPlayerConstItr;
typedef pair<CMultiMapApplyFactionPlayerItr, CMultiMapApplyFactionPlayerItr> Range;

//名称
typedef map<string, UINT16>		CMapFactionName;
typedef CMapFactionName::iterator	CMapFactionNameItr;

struct ChatInfo;

// 聊天记录指针列表
typedef std::list<ChatInfo*> CFactionChatList; // 这个和CChatDataList定义其实一样，但如果包含<chatmag.h>会导致linux下编译不通过，暂时通过这个方法解决

typedef list<DT_FACTION_PLAYER_DATA*> CListSortPlayer;
typedef CListSortPlayer::iterator CListPlayerItr;
class CFactionMgr;
class CFaction
{

public:
    CFaction()
    {
        m_qwSaveTime = 0;
        m_dequeFactionLogData.clear();
        m_mapFactionPlayerData.clear();
        m_mapApplyFactionPlayerID.clear();
		m_setHotSpringPlayerID.clear();
		m_setAddHotSpringPlayerID.clear();
		m_setDelHotSpringPlayerID.clear();
		m_listSoryByJob.clear();
        m_listChat.clear();
        m_listChatPool.clear();
        m_bExceptionFlag = FALSE;
        m_byExt = 0;
		memset(&m_stDT_FACTION_BASE_DATA, 0x00, sizeof(DT_FACTION_BASE_DATA));
        memset(&m_stFactionGvGData, 0x00, sizeof(DT_FACTION_GVG_DATA));
		
    }

    ~CFaction()
    {
		m_setHotSpringPlayerID.clear();
		m_setAddHotSpringPlayerID.clear();
		m_setDelHotSpringPlayerID.clear();
        m_dequeFactionLogData.clear();
        m_mapFactionPlayerData.clear();
        m_mapApplyFactionPlayerID.clear();
		m_listSoryByJob.clear();
		memset(&m_stFactionGvGData, 0x00, sizeof(DT_FACTION_GVG_DATA));
        ClearChat(0);
    }

    void Init(DT_FACTION_DATA &stDT_FACTION_DATA);
    void Init(DT_FACTION_PLAYER_DATA_LST &stDT_FACTION_PLAYER_DATA_LST);

    DT_FACTION_BASE_DATA & GetDT_FACTION_BASE_DATA()
    {
        return m_stDT_FACTION_BASE_DATA;
    }

    UINT32 GetFactionID(){ return m_stDT_FACTION_BASE_DATA.dwFactionID; }
	
    UINT16 GetOriZoneID();

    //数据库保存玩家数据用
    void GetDT_FACTION_PLAYER_DATA_LST(DT_FACTION_PLAYER_DATA_LST &stDT_FACTION_PLAYER_DATA_LST);
    //数据库保存日志数据用
    void GetDT_FACTION_LOG_DATA_LST(DT_FACTION_LOG_DATA_LST &stDT_FACTION_LOG_DATA_LST);
    //数据库保存门派GVG数据
	VOID GetFactionGvGData(DT_FACTION_GVG_DATA& stGvGData);
    void GetDT_FACTION_PLAYER_CLI_LST(DT_FACTION_PLAYER_CLI_LST &stDT_FACTION_PLAYER_CLI_LST);
	void GetDT_FACTION_PLAYER_CLI(DT_FACTION_PLAYER_CLI& stCli, UINT32 dwPlayerID);
    void GetDT_FACTION_PLAYER_AUDIT_CLI_LST(DT_FACTION_PLAYER_AUDIT_CLI_LST &stDT_FACTION_PLAYER_AUDIT_CLI_LST);
	void GetDT_FACTION_PLAYER_AUDIT_CLI_LST2(DT_FACTION_PLAYER_AUDIT_CLI_LST2 &stDT_FACTION_PLAYER_AUDIT_CLI_LST);
	VOID GetDT_FACTION_COMMON_DATA(DT_FACTION_COMMON_DATA& stDT_FACTION_COMMON_DATA);

	DT_FACTION_COMMON_DATA& GetDT_FACTION_COMMON_DATA();
	//获取玩家帮派的基本信息
	void GetDT_FACTION_BASE_OWNER_DATA_CLI(DT_FACTION_BASE_OWNER_DATA_CLI &stDT_FACTION_BASE_OWNER_DATA_CLI, UINT32 dwJoinPlayerID);

	//获取玩家自己的信息
	VOID GetDT_FACTION_PLAYER_CLI_SELF(DT_FACTION_PLAYER_CLI& stDTFACTION_PLAYER_CLI, UINT32 dwPlayerID);

    DT_FACTION_GVG_DATA& GetFactionGvGData(){ return m_stFactionGvGData; }    

    //申请加入帮派
    UINT16 ApplyFactionData(UINT32 dwJoinPlayerID);

    //取消申请加入帮派
    UINT16 CancelApplyFactionPlayerData(UINT32 dwJoinPlayerID);

    //创建帮派
    UINT16 CreateFactionPlayerData(UINT32 dwJoinPlayerID, DT_FACTION_DATA &stDT_FACTION_DATA, vector<UINT32> &vec);

    //剔除玩家
    UINT16 KickedFaction(UINT32 dwJoinPlayerID, UINT32 dwKickedPlayerID, PKT_CLIGS_KICKED_FACTION_ACK& stAck);

    //打开帮派门贡面板
    UINT16 OpenDoorsTributeFaction(DT_DOORS_TRIBUTE_TODAY_DATA &stDoorsTributeTodayData, UINT32 dwJoinPlayerID);

    //门贡请求
    UINT16 DoorsTributeFaction(DT_DOORS_TRIBUTE_TODAY_DATA &stDoorsTributeTodayData, UINT8 dwDoorsTributeFlag, CPlayer* poPlayer);

    //领取奖励
    UINT16 RecvDoorsTributeFactionReq( CPlayer * poPlayer, PKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK& stAck );

    //增加日志
    void		AddFactionLogData(DT_FACTION_LOG_DATA stDT_FACTION_LOG_DATA);

    //查看玩家是否在成功加入
    BOOL		CkFactionPlayerData(UINT32 dwJoinPlayerID);

    //查看玩家是否在正在早请加入
    BOOL		CkFactionApplyPlayerData(UINT32 dwJoinPlayerID);

    //查看玩家职业个数
    UINT16	GetJobNum(em_Faction_Job ckjob);

    UINT16	JobFaction(UINT32 dwJoinPlayerID, UINT32 dwJobPlayerID, UINT8 byJobFlag );

    UINT16	SetNoticeFaction(UINT32 dwJoinPlayerID, UINT8 byFlag, TCHAR aszFactionNotice[MAX_FACTION_DESC_NUM]);

    UINT16	DissolveFaction( UINT32 dwJoinPlayerID );
	//审核玩家信息
    UINT16	AuditFaction( UINT32 dwJoinPlayerID, UINT8 byAuditFlag, UINT32 dwPlayerID, vector<UINT32> &vec, UINT32& dwPassNum, UINT32& dwNoPass);
	UINT16	AuditFaction( UINT8 byAuditFlag, UINT32 dwPlayerID, vector<UINT32> &vec);

	UINT16 CKAuditFaction( UINT8 byAuditFlag, UINT32 dwPlayerID );

	UINT16	GetDT_FACTION_LOG_DATA_CLI_LST(DT_FACTION_LOG_DATA_CLI_LST &stDT_FACTION_LOG_DATA_CLI_LST);

    static BOOL	CkFactionPlayerData(UINT32 &dwFactionID, UINT32 dwJoinPlayerID);
    static BOOL	CkFactionApplyPlayerData(UINT32 dwJoinPlayerID, list<UINT32> &vec);

	//当前最大积分
	UINT32 ResetMaxTaskIntegral();

	

	VOID AddSetHotSpringPlayerID(UINT32 dwPlayerID);

	VOID DelSetHotSpringPlayerID(UINT32 dwPlayerID);

	VOID SetHotSpringPlayerList(DT_HOT_SPRING_PLAYER_DATA_CLI_LST& stPlayerInfo);
	VOID SetHotSpringPlayerSingle(DT_HOT_SPRING_PLAYER_DATA_CLI& stPlayerInfo, UINT32 dwPlayerID);
	VOID SendSynchInfo();

	UINT16 ChangeOther(UINT32 dwPlayerID, UINT32 dwSendPlayerID, PKT_CLIGS_CHANGE_OTHER_ACK& stAck);
    UINT64 &GetSaveTime()
    {
        return m_qwSaveTime;
    }

	VOID GetFactionPlayerNum(UINT16& wFactionNumberPeople, UINT16& wMaxFactionNumberPeople);
	
	//人员变动标志
	VOID SetPlayerChangeToAll();

	VOID DeletePlayerDataByID(UINT32 dwPlayerID);

	

	VOID AfterGiveWineProc();

	VOID GetFactionBuileGroup(DT_FACTION_BUILD_LEVEL& stBuildFlag);

	UINT32 GetFactionBuildLevel(em_Faction_Build emBuildType);

	VOID GetFactionDataToCli(DT_FACTION_DATA_TO_CLI& stFactionData);
	//获取管理人员的信息
	VOID GetManagerInfo(DT_FACTION_PLAYER_CLI_LST& stPlayerLst);

//////////////////////////////////////////////////////////////////////////
	//武堂与珍品阁
	VOID SetFactionRareInfo();

	VOID SetFactionSkillInfo();

    const UpgradeRareItemMap& GetRareItemMap()
    {
        return m_mapRareitemInfo;
    }

    const CMapFactionSkill& GetFactionSkillMap()
    {
        return m_mapFactionSkill;
    }

	VOID CleanAllApply();
	////////////////////////////扣除、增加资金门贡等//////////////////////////////////////////////


	UINT32 GetFactionFunds();
	UINT32 GetDoorsTribute(UINT32 dwPlayerID);
	UINT64 GetSumDoorsTribute(UINT32 dwPlayerID);
	//添加门派协议
	VOID AddFactionFunds(UINT32 dwValue, UINT16 wOpType);
	//扣除门派资金
	VOID DecFactionFunds(UINT32 dwFunds, UINT16 wOpType);
	//给全体门派人员加门贡
	VOID AddDoorsTributeToAll(UINT32 dwDoorsTribute);
	//扣除门贡
	BOOL DeductDoorsTribute(UINT32 dwPlayerID, UINT32 dwDoorsTribute, UINT16 wOpType);
	//添加门贡
	VOID AddDoorsTribute(UINT32 dwPlayerID, UINT32 dwDoorsTribute, UINT16 wOpType, UINT64 qwParam1 = 0, UINT64 qwParam2 = 0, UINT64 qwParam3 = 0, UINT64 qwParam4 = 0);

	//增加门派捉妖任务积分
	VOID AddHuntingIntegral(UINT32 dwPlayerID, UINT32 dwValue);

	BOOL AddFindBackDoorsTribute(UINT32 dwPlayerID, UINT8 byFindType);
	/////////////////////////////////////GM///////////////////////////////////////////////////////
	VOID SetFactionLevelByGM(UINT32 dwFactionLevel);
	VOID ChangeFactionNameByGM(TCHAR aszFactionName[MAX_FACTION_NUM]);
	////////////////////////////扣除、增加资金门贡等//////////////////////////////////////////////

    CMapFactionPlayerData& GetFactionPlayers()
    {
        return m_mapFactionPlayerData;
    }

    // 添加成员的聊天数据
	VOID AddChatData(const ChatInfo& stChatData);

    // 添加门派活动聊天公告
    VOID AddFactionActivityChat(const string &strChatContent);

    CFactionChatList& GetChat()
    {
        return m_listChat;
    }
    CFactionChatList& GetChatCache()
    {
        return m_listChatPool;
    }

    // 清除本门派聊天记录（但保留部分聊天记录历史）
    VOID ClearChat(UINT16 wReserveCnt = MAX_FACTION_CHAT_HISTORY/* 保留多少条历史记录 */);

	VOID AfterHuntingProc();

	UINT32 GetApplyFactionNum();

	DT_FACTION_PLAYER_DATA* GetDT_Faction_Player_Data(UINT32 dwPlayerID);

	static UINT32 GetMultiApplyFaction(UINT32 dwPlayerID);
	//////////////////////////////////提示信息////////////////////////////////////////////////////
	VOID SetAllNewRareItemFlag();
	VOID SetAllNewSkillFlag();
	BOOL IsCanBuyRareItem(UINT32 dwPlayerID);
	BOOL IsCanLearnSkill(UINT32 dwPlayerID);
	/////////////////////////////发送信息/////////////////////////////////////////////
	VOID SendMessage(TCHAR* aszMsg, UINT16 wMsgID, em_Faction_Authority emAuth);//根据权限发送消息
	VOID SendMessageToAllFactionPlayer(TCHAR* aszSendMsg, UINT16 wMsgID, UINT32 dwPlayerID = 0);
	VOID SendMessageByAuthPrompt( );
	VOID SendMessageByGymnasiumPrompt();
	VOID SendMessageByRareItemPrompt();
	VOID SendMessageByShadePrompt();
	VOID SendMessageByHotSpringPrompt();
	VOID SendMessageByFactionPrompt();
	VOID SendMessageByLogNtf(DT_PLAYER_FACTION_BATTLELOG_DATA& stData);
    VOID SetExceptionFlag(BOOL	bExceptionFlag)
    {
        m_bExceptionFlag = bExceptionFlag;
    }
	BOOL GetExceptionFlag()
	{
		return m_bExceptionFlag;
	}

    UINT8	&GetExt()
    {
        return m_byExt;
    }
    VOID		GetAuditData(UINT8& byTodayPassNum, UINT64& qwLastAuditTime, UINT8 &byExt);
	UINT8 GetTodayPassNum();

    // 门派成员信息列表，用于查看其他门派的概要成员信息
    void GetFactionMemberInfoList(DT_FACTION_MEMBER_INFO_LIST&);

    // 把gvg所需的本门派战斗存入结构体，用于客户端同步
    BOOL SaveGvGPKInfoTo(DT_PK_FACTION&);

    BOOL SaveGvGInfoTo(DT_GVG_FACTION&);

    // 从数据库中调取本门派的上阵玩家到内存中
    void TakeFormationPlayersFromDB();

    // 本门派报名时默认初始化上阵玩家
    void AutoFormation(UINT16 wAutoFormationCnt);

    // 踢掉离开门派的阵上玩家
    void KickOutFormationLeavePlayers();

    BOOL IsAllFormationPlayerInMemory();

    CPlayerVec GetFormationPlayers();

    em_Faction_Job GetJob(UINT32 dwPlayerID);

	UINT8 GetFactionIconID();
	UINT16	RegisterFactionName(CHAR* pszDspName, CPlayer * poPlayer);//注册昵称

	
	VOID GetFactionBaseDataCli2(DT_FACTION_BASE_DATA_CLI2& stBaseCli);

	///////////////////////////GVG相关///////////////////////////////////////////////
	//获取门派阵型信息
	VOID GetFormationInfo(
		DT_FACTION_FORMATION_POS_LST_INFO& stFormationInfo,
		DT_GVG_FORMATION_SIMPLE_PLAYER_DATA_LST_CLI& stPlayerLstInfo,
		UINT16& wCurFactionLevel);

	VOID ResetFormationInfo();
	UINT16 GvGFactionEmbattle(PKT_CLIGS_GVG_FACTION_EMBATTLE_REQ& stAck);
    UINT64 ReCalcSumPower();
	UINT64 GetFactionGVGFunds(){ return m_stFactionGvGData.qwGVGFactionFunds; }
	UINT64 GetSumPower(){ return m_stFactionGvGData.qwSumPower; }
	VOID AddGvGFactionFunds(UINT32 dwFactionFunds);
	VOID ResetGvGFactionFunds();

    void AddGvGLog(DT_FACTION_SIMPLE_GVG_LOG_LIST &stSimpleLogList);

    // 添加与本服门派战斗的战况，此时战斗未开始
    void AddLocalGvGLogNotStarted(UINT32 dwFactionID1, UINT32 dwFactionID2, DT_GVG_ROUND&, VideoID videoID);

    // 添加与本服门派战斗的战况，此时战斗已结束
    void AddLocalGvGLog(DT_GVG_VIDEO &stVideo);

    // 添加本服内的战况
    void AddNoEnemyGvGLog(DT_GVG_ROUND&);

    UINT16 GetReadbleGvGLogCnt();

    void ClearOldGvG();

    BOOL IsJoin(UINT32 dwPlayerID);

	//添加连斩奖励
	VOID AddContinueKill(UINT32 dwPlayerID, UINT16 wSessionID, UINT8 byGvGRound, UINT8 byBattleIdx, UINT8 byKillNum);
	//添加个人门派战报
	VOID AddPlayerBattleLog(const UINT32& dwPlayerID, DT_PLAYER_FACTION_BATTLELOG_DATA& stLogData);
	//添加跨服战斗结果战报
	//轮空时：poFaction = NULL, bIsNext = TRUE 代表直接进入下一轮， FALSE 代表直接进入前4
	VOID AddGvGFactionBattleLog(UINT8 byRound, UINT8 byBattleIdx, GvGLogResult eResult, const TCHAR* poFactionName, BOOL bIsNext = TRUE);
	//重置连斩奖励
	VOID ResetContinueKill();
	//重置GVG数据
	VOID ResetFactionGvGData();

    VOID AddPlayerTosnapshot();

	VOID CheckGvGMedal();
	
private:
	UINT8 m_byTodayPassNum;  //今天审核通过人数
	UINT64 m_qwLastAuditTime;  //最后审核时间
    UINT8	m_byExt;				//扩展数据

	DT_FACTION_GVG_DATA					m_stFactionGvGData;//门派GVG信息
    CFactionChatList                    m_listChat;         // 本派当前未发送的聊天记录
    CFactionChatList                    m_listChatPool;     // 本派已发送的聊天记录历史
    UINT64							    m_qwSaveTime;
    DT_FACTION_BASE_DATA			    m_stDT_FACTION_BASE_DATA;		//门派基本信息
    CMapFactionPlayerData			    m_mapFactionPlayerData;			//门派当前玩家信息
    CMapFactionPlayerID				    m_mapApplyFactionPlayerID;			//申请加入门派信息
	BOOL							    m_bExceptionFlag;					//门派异常标识
    CDequeFactionLogData			    m_dequeFactionLogData;			//日志信息
	CListSortPlayer					    m_listSoryByJob;

	UpgradeRareItemMap				    m_mapRareitemInfo;			//本门派珍品上限	//物品ID与物品个数
	CMapFactionSkill				    m_mapFactionSkill;			//本门派武堂技能

	CSetHotSpringPlayerID			    m_setHotSpringPlayerID;		//温泉玩家列表
	CSetHotSpringPlayerID			    m_setAddHotSpringPlayerID;	//新进入玩家列表
	CSetHotSpringPlayerID			    m_setDelHotSpringPlayerID;	//离开玩家列表
	DT_FACTION_COMMON_DATA              m_stDT_FACTION_COMMON_DATA;	//门派状态数据

	static CMapFactionPlayerID			s_mapFactionPlayerID;	//key playerID, value factionID
	static CMultiMapApplyFactionPlayer	s_multimapApplyFactionPlayer;
};

#endif //#ifndef _ROLEMGR_H_

