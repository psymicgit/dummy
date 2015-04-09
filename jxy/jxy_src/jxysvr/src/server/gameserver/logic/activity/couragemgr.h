
#ifndef _COURAGEMGR_H_
#define _COURAGEMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sdobjectpool.h>
#include <logic/base/basepropmgr.h>
#include <protocol/server/protocommondata.h>
#include <logic/base/logicfunc.h>
#include <map>
#include <list>
#include <protocligs.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/monster/playergroup.h>
#include <logic/monster/monstermgr.h>
#include "courage.h"

using namespace std;
using namespace SGDP;

class CCourageGroup;
typedef map<UINT64, CCourageGroup*> CCourageGroupMap;
typedef CCourageGroupMap::iterator  CCourageGroupMapItr;


class CCourageGroup
{
public:
    CCourageGroup()
    {

    }
    ~CCourageGroup()
    {
        for (CCourageGroupMapItr itr = s_mapCourageGroup.begin(); itr != s_mapCourageGroup.end();)
        {
            if (itr->second == this)
            {
                s_mapCourageGroup.erase(itr++);
            }
            else
            {
                itr++;
            }
        }
    }
    void Init(DT_COURAGE_GROUP_DATA &stDT_COURAGE_GROUP_DATA)
    {
        for (UINT16 idx = 0; idx < stDT_COURAGE_GROUP_DATA.wPlayerNum; idx++)
        {
            s_mapCourageGroup[CLogicFunc::Get3232Key(stDT_COURAGE_GROUP_DATA.dwGroupID, stDT_COURAGE_GROUP_DATA.astPlayerData[idx].dwPlayerID)] = this;
        }
        memcpy(&m_stDT_COURAGE_GROUP_DATA, &stDT_COURAGE_GROUP_DATA, sizeof(DT_COURAGE_GROUP_DATA));
    }
	DT_COURAGE_GROUP_DATA &GetDT_COURAGE_GROUP_DATA()
	{
		return m_stDT_COURAGE_GROUP_DATA;
	}
    static CCourageGroup* GetCourageGroup(UINT32 dwGroupID, UINT32 dwPlayerID)
    {
        CCourageGroupMapItr itr = s_mapCourageGroup.find(CLogicFunc::Get3232Key(dwGroupID, dwPlayerID));
        if (itr != s_mapCourageGroup.end())
        {
            return itr->second;
        }
        else
        {
            return NULL;
        }
    }
private:
    DT_COURAGE_GROUP_DATA		m_stDT_COURAGE_GROUP_DATA;
    static CCourageGroupMap	s_mapCourageGroup;
};

//战斗类型
enum em_Battle_Type
{
    EBT_JOIN = 0, //加入战斗
    EBT_HELP = 1, //协助战斗
};

//队员身份
enum em_Battle_Identity
{
    EBI_MEMBER = 0, //队员
    EBI_LEADER = 1, //队长
};

//准备状态
enum em_Battle_Ready
{
    EBR_READYING = 0, // 准备中
    EBR_READY  = 1, //已经准备
    EBR_NONE = 2,
};

//加入队伍类型
enum em_Join_Battle_Type
{
    EJBT_SELECT = 0, //选择加入
    EJBT_RAND = 1, //随机加入
};

enum em_Battle_State
{
    EBS_NO_BATTLE = 0, //不在战斗中
    EBS_BATTLING = 1, //在战斗中
};

enum em_Battle_Result
{
    EBR_PASS = 0, //通过战斗
    EBR_NO_PASS = 1, //没通过战斗
};

class CCourageMgr
{
    typedef map<UINT32, CCourageGroup*> CGroupID2CourageGroupMap;
    typedef CGroupID2CourageGroupMap::iterator  CGroupID2CourageGroupMapItr;

    typedef map<UINT32, CPlayerGroup*> CPlayerGroupMap;
    typedef CPlayerGroupMap::iterator CPlayerGroupMapItr;
public:

    DECLARE_SINGLETON(CCourageMgr);

public:

    BOOL Init();

    BOOL Init(DT_COURAGE_GROUP_DATA_LST stDT_COURAGE_GROUP_DATA_LST);

    VOID UnInit();

	BOOL InitEvent();	
public:

	BOOL			OnStartEvent(UINT8 byActivityID);
	BOOL			OnEndEvent(UINT8 byActivityID);
	BOOL			OnSave();
    CCourage*       CreateCourage();
    VOID            RemoveCourage(CCourage* poCourage);
    VOID            SaveData();
    VOID            OnEvent();
	//打开勇气试炼
    UINT16  OnOpenCourage(CPlayer* poPlayer,DT_COURAGE_INFO_CLI &stCourageInfo,DT_COURAGE_GROUP_DATA_CLI_LST &stCORUAGE_GROUP_DATA, UINT8& bySelect);
    //打开创建队伍
    UINT16  OnCourageOpenCreateGroup(CPlayer* poPlayer, PKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_ACK& psAck);
    //确认创建队伍
    UINT16  OnCourageCreateGroup(CPlayer* poPlayer,UINT8 byJoinType,DT_INSTANCE_ID wInstanceID,UINT32 &dwGroupID);
    //加入队伍或者协助队伍
    UINT16  OnCourageJoinGroup(CPlayer* poPlayer,UINT8 byBattleType,UINT32 dwGroupID);
    //随机加入队伍或者协助队伍
    UINT16  OnCourageRandomJoinGroup(CPlayer* poPlayer,PKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_ACK &stAck);
    //添加或者更改伙伴
    UINT16  OnCourageOperateHero(CPlayer* poPlayer ,DT_COURAGE_GROUP_HERO_DATA_CLI &stCOURAGE_GROUP_HERO_DATA);
    //确认添加或者更改伙伴
    UINT16  OnCourageConfirmHero(CPlayer* poPlayer,UINT16 dwKindID,DT_COURAGE_HERO_DATA& stCOURAGE_HERO_DATA);
    //踢掉队员
    UINT16  OnCourageFireGroupMember(CPlayer* poPlayer,UINT32 dwGroupID,UINT32 dwPlayID ,UINT32 &dwFirePlayID, UINT16 &wAverage);
    //购买试炼
    UINT16  OnCourageBuyPractiseTimes(CPlayer* poPlayer,PKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_ACK &stk);
    //打开购买试炼
    UINT16  OnCourageOpenShop(CPlayer* poPlayer,UINT8 &byBuyTimes,UINT32 &dwGold);
    //玩家准备
    UINT16  OnCouragePlayerReady(CPlayer* poPlayer,UINT8 byTpye,UINT8 &byReadyTpye);
    //玩家开始
    UINT16 OnCouragPlayerStart(CPlayer* poPlayer, PKT_CLIGS_COURAGE_PLAYER_START_ACK5 &stk);
    //玩家开始2
    UINT16 OnCouragPlayerStart2(CPlayer* poPlayer, PKT_CLIGS_COURAGE_PLAYER_START_ACK2 &stk);
	//玩家开始3
	UINT16 OnCouragPlayerStart3(CPlayer* poPlayer, PKT_CLIGS_COURAGE_PLAYER_START_ACK3 &stk);
    //玩家离开
    UINT16 OnCouragLeaveGroup(CPlayer* poPlayer,DT_COURAGE_INFO_CLI &stCOURAGE_INFO,DT_COURAGE_GROUP_DATA_CLI_LST &stCOURAGE_GROUP_DATA);
    //玩家刷新队伍
    UINT16 OnCourageRefreshGroup(CPlayer* poPlayer,DT_COURAGE_GROUP_DATA_CLI_LST &stCOURAGE_GROUP_DATA);
    //玩家进入队伍界面
    UINT16 OnCourageEnterGroup(CPlayer* poPlayer,DT_COURAGE_GROUP_DATA &stCOURAGE_GROUP_DATA);
    //检查数据是否要重置
    VOID    ResetCourageData(CPlayer* poPlayer);
    //获取最新次数
    VOID    GetCourageTimes(CPlayer* poPlayer);
    //筛选符合条件的队伍
    BOOL    GetRightGroup(CPlayer* poPlayer ,BOOL bReset = FALSE);
    //符合条件队伍赋值
    VOID    SetPlayerGroupValue(CPlayer* poPlayer,DT_COURAGE_INFO_CLI &stCOURAGE_INFO,DT_COURAGE_GROUP_DATA_CLI_LST &stCOURAGE_GROUP_DATA);
    //领取勇气奖励
    UINT16 OnCourageGetAwardReq(CPlayer* poPlayer,PKT_CLIGS_COURAGE_GET_AWARD_ACK &stAck, UINT32 dwIndex = 0);
    //获取勇气战斗结果
    UINT16 OnCouragGetBattleResultReq(CPlayer* poPlayer, PKT_CLIGS_COURAGE_GET_BATTLE_RESULT_ACK &stAck);
    //判断是否在队伍中
    BOOL    HaveInGroup(CPlayer* poPlayer, DT_COURAGE_GROUP_DATA &stCOURAGE_GROUP_DATA ,UINT32 dwGroupID); 
    //玩家离开
    UINT16  OnLeave(CPlayer* poPlayer, BOOL bDisconnect = FALSE);
    //玩家战斗时候离开
    UINT16  OnLeaveBattle(CPlayer* poPlayer);
    //玩家离开
    UINT16  OnLeaveCourage(CPlayer* poPlayer);
    //超时离开队伍
    VOID  OverTimeDismiss(UINT32 dwPlayerID);
    //确认携带了一个伙伴
    BOOL  HaveBringHero(CPlayer* poPlayer);
    //设置默认伙伴
    UINT16  GetMaxLevelHero(CPlayer* poPlayer, UINT16 &wLevel);
    //进入队伍设置队伍数据
    BOOL    SetEnterGroupData(CPlayer* poPlayer);
    //比较队伍数据
    UINT16  CompareMapGroup(CMapGroupIDNum	m_mapBackGroupID, CMapGroupIDNum m_mapGroupIDNum);
    //生成队伍战力信息
    BOOL  CreateGroupPower(UINT32 dwGroupID, DT_COURAGE_PLAYERID_HEROID_DATA astPlayerInfo[MAX_COURAGE_MEMBER_NUM]);
    //战斗数据转换
    VOID  TransData(const DT_BATTLE_DATA2& stBataData2, DT_BATTLE_DATA3 &stBataData3);
    VOID  TransData2(const DT_BATTLE_DATA3& stBataData3, DT_BATTLE_DATA2 &stBataData2);

protected:
    BOOL InitModuleDataFromDB();
    BOOL InitModuleLogDataFromDB();

	CCourageGroup *GetCourageGroup(CPlayer* poPlayer);

protected:
    CCourageMgr();
    virtual ~CCourageMgr();

protected:
	UINT64								m_qwOverTime;             //活动结束时间
	UINT64								m_qwStartTime;            //活动开始时间
	UINT64								m_qwSaveTime;             //活动保存更新时间
    UINT64                              m_qwRefreshTime;          //刷新时间
	UINT8								m_m_byActivityID;         //活动ID
    UINT32								m_dwGroupID;              //创建队伍后的最大队伍ID
    CGroupID2CourageGroupMap			m_mapGroupID2CourageGroup;//所有队伍的信息
    CPlayerGroupMap                     m_mapPlayerGroupMap;      //保存队伍战力      
    CSDObjectPool<CCourageGroup, CSDMutex>	m_oCourageGroupPool;  //队伍对象池
    CSDObjectPool<CCourage, CSDMutex>       m_oCourageShmemObjMgr;	
    CSDObjectPool<CPlayerGroup, CSDMutex>   m_oPlayerCouragePool; 
    BOOL		m_bActivityOver;
    //获取协助提示
    VOID GetHelpPrompt(CPlayer* poPlayer,TCHAR aszHelpCouragePrompt[HELP_COURAGE_LEN]);
    //战斗
    //UINT16 CourageBattle(CPlayerGroup* poMyself, CMonsterGroup* poEnemy,UINT8 byBattleIdx, UINT16 wLevel,DT_BATTLE_DATA2& stDT_BATTLE_DATA, UINT8& byPassFlag,
    //    DT_COURAGE_AWARD_DATA& stPassEncourage, DT_COURAGE_AWARD_DATA& stAfterPassEncourage);
    //通过战斗
    VOID PassBattle(UINT32 dwGroupID,UINT16 wLevel, BOOL bPass);
    //清除队伍里面玩家数据
    VOID ClearGroupMemberData(CPlayer* poPlayer);
    //记录进入勇气
    C3232Map	m_mapAllEnterPlayer;
    //记录进入组队
    C3232Map    m_mapAllEnterGropPlayer;
    //刷新队伍
    VOID RefreshGroupNtf();
    //随机获取妖怪
    //BOOL GetMonsterID(UINT16 wLevel, UINT8 byMonsterNum, vector<UINT16>& vecMonsterID);   
    //BOOL GetMonsterID(UINT16 wLevel, UINT8 byMonsterNum, map<UINT8,vector<UINT16>>& mapMonsterID);   
    //BOOL GetMonsterID(UINT16 wLevel, UINT8 byMonsterNum, CMapMonsterIID& mapMonsterID, vector<UINT16> vecMonsterFaceID); 
    //获得玩家可以进入副本的信息
    VOID	GetGVEInstanceRecordData(CPlayer* poPlayer, DT_TOWN_INSTANCE_RECORD_DATA_CLI &stData, UINT16 wSceneIdx, UINT8 byTownIndex);
    //验证当前副本是否解锁
    BOOL    CkGVEUnlock(UINT16 wLevel, UINT16 wSceanID, UINT8 byTownID);
    //获取宝箱的首饰信息
    VOID    RatePassBoxEnc(CPlayer* poPlayer, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, DT_COURAGE_JEWELRY_DATA_LST& stJewlryDataList);
    //通关宝箱信息
    UINT16  PassBoxEnc(CPlayer* poPlayer, DT_PASS_ENCOURAGE_BOX_DATA2& stDT_PASS_ENCOURAGE_BOX_DATA, DT_DATA_AFTER_PASS_ENCOURAGE_BOX* pstAfterPassEncourage);
    //获取通关宝箱信息
    VOID    GetPassBox(CPlayer* poPlayer, DT_PASS_ENCOURAGE_BOX_DATA2& stDT_PASS_ENCOURAGE_BOX_DATA);
    //获得宝箱随机物品ID
    UINT16  RateInnerBox(UINT16 wKindID);
    //内部宝箱创建首饰
    UINT16  CreateItem(CPlayer* poPlayer);
    //获取副本的难度
    UINT8   GetScore(UINT16 wLevel);
    //创建首饰的随机属性
    VOID    CreateJewelryData(CPlayer* poPlayer, UINT16 wJewelryID, DT_JEWELRY_DATA &st_DT_JEWELRY_DATA);
    //获取协议提示索引（和客户端配合）
    UINT8   GetHelpID(UINT32 dwHelpTimes, UINT32 dwGiveFreeTimes);
    //获取玩家在队伍里面的索引
    UINT8   GetInGroupIndex(UINT32 dwPlayerID, UINT32 dwGroupID);


private:
    static UINT32 dwCreateGroupID;                   //创建队伍ID
    BOOL   bHaveData;                                //数据库是否有队伍数据
    VOID   CreateTestData();                         //生成测试数据
    UINT32 CreateGroupNum[MAX_TOWN_PER_SCENE_NUM];   //当前创建队伍的人数
};

#endif // 
