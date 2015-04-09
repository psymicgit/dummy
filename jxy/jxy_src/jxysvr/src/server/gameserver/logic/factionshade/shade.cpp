#include "shade.h"
#include <logic/faction/factionmgr.h>
#include <logic/vip/vippropmgr.h>
#include <logic/other/errmsgmgr.h>
#include <logic/lua/luamgr.h>
#include <logic/factionshade/shadepropmgr.h>
#include <logic/monster/monstermgr.h>
#include <logic/monster/monster.h>
#include <logic/battle/battlemgr.h>
#include <logic/item/itemmgr.h>
#include <logic/monster/shademonstermgr.h>
#include <logic/monster/shademonstergroup.h>
#include <logic/event/huntingevent.h>
#include "logic/player/player.h"

#include "logic/awaken/awaken.h"



BOOL CShade::Init(DT_SHADE_DATA &stDT_SHADE_DATA, CPlayer* poOwner)
{
    memset(&m_stShadeData, 0, sizeof(DT_SHADE_DATA));
    memcpy(&m_stShadeData, &stDT_SHADE_DATA, sizeof(DT_SHADE_DATA));
    m_poOwner = poOwner;
    m_dwPlayerID = m_poOwner->GetID();

    return TRUE;
}

CFaction* CShade::GetFaction()
{
    UINT32 dwFactionID = 0;
    BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, m_poOwner->GetID());
    if ( !bRet )
    {
        return NULL;
    }

    return CFactionMgr::Instance()->GetFaction(dwFactionID);
}

UINT32 CShade::GetFactionID()
{
    UINT32 dwFactionID = 0;
    BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, m_poOwner->GetID());
    if ( !bRet )
    {
        return 0;
    }
    return dwFactionID;
}

UINT16 CShade::OpenShade(PKT_CLIGS_OPEN_SHADE_ACK& stAck)
{
    CFaction* poFaction = GetFaction();
    if (NULL == poFaction)
    {
        return ERR_OPEN_SHADE::ID_FACTION_NOT_EXIST;
    }

    BOOL bRet = IsOpen();
    if ( !bRet )
    {
        return ERR_OPEN_SHADE::ID_SHADE_NOT_EXIST;
    }
    //重置捉妖次数
    EachUpdataShade();

    DT_FACTION_COMMON_DATA& stFactionTaskScheduleData = poFaction->GetDT_FACTION_COMMON_DATA();
	stAck.stNewHuntingScheduleInfo.byLastSchedule = stFactionTaskScheduleData.stHuntingTaskSchedule.byCurCollectiveTaskSchedule;

    //赋值信息
    GetShadeInfo(stAck.stShadeInfo);

    GetHuntingInfoCli(stAck.stHuntingInfo, stFactionTaskScheduleData, stAck.stNewHuntingScheduleInfo);

    return ERR_OPEN_SHADE::ID_SUCCESS;
}

UINT16 CShade::OpenGoldMode(DT_MONSTER_INFO& stMonsterInfo, UINT32& dwSyncGold)
{
    CFaction* poFaction = GetFaction();
    if (NULL == poFaction)
    {
        return ERR_OPEN_GOLDMODE::ID_FACTION_NOT_EXIST;
    }
    BOOL bRet = IsOpen();
    if ( !bRet )
    {
        return ERR_OPEN_GOLDMODE::ID_SHADE_NOT_EXIST;
    }

    //检查任务是否开启
    bRet = CheckTaskOpen(EFTT_HUNTING);
    if (FALSE == bRet)
    {
        return ERR_OPEN_GOLDMODE::ID_TIME_NOT_ENOUGH;
    }

    //更新配置
    EachUpdataShade();
    DT_FACTION_COMMON_DATA& stFactionTaskScheduleData = poFaction->GetDT_FACTION_COMMON_DATA();

    if( 1 == m_stShadeData.stHuntingInfo.byGoldModeFlag )  //黄金模式已开启
    {
        return ERR_OPEN_GOLDMODE::ID_STATE_IS_OPEN;
    }

    //检查当前VIP等级功能是否开放
    bRet = CVipPropMgr::Instance()->CkOpenFunc(EVF_OPENGOLDMODE, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
    if (FALSE == bRet)
    {
        return ERR_COMMON::ID_VIP_LEVEL_NOT_REACH;
    }

    if (stFactionTaskScheduleData.stHuntingTaskSchedule.dwMaxHuntingNum <= m_stShadeData.stHuntingInfo.dwHuntionNum)
    {
        return ERR_OPEN_GOLDMODE::ID_HUNTING_NUM_NOT_ENOUGH;
    }

    UINT32 dwNeedGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_FACTION_OPEN_GOLD, 0);
    const UINT64 qwPlayerGold = m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold;
    if ( qwPlayerGold < dwNeedGold )
    {
        return ERR_OPEN_GOLDMODE::ID_GOLD_NOT_ENOUGH;
    }
    m_poOwner->DecGold(dwNeedGold, CRecordMgr::EDGT_OPEN_GOLD_MODE );

    m_stShadeData.stHuntingInfo.byGoldModeFlag = 1;

    if (0 != m_stShadeData.stHuntingInfo.byMonsterNum)
    {
        GetMonsterInfo( stMonsterInfo);
    }

    //赋值最新集体任务信息
    //GetHuntingInfoCli(stHuntingInfoCli, stFactionTaskScheduleData);

    dwSyncGold = static_cast<UINT32>(m_poOwner->GetSyncReward(ACTE_GOLD));
    return ERR_OPEN_GOLDMODE::ID_SUCCESS;
}

UINT16 CShade::OpenShadeUpgrade(DT_SHADE_UPGRADE_INFO& stShadeUpgradeInfo)
{
    CFaction* poFaction = GetFaction();
    UINT32 dwFactionID = GetFactionID();
    if ( NULL == poFaction)
    {
        return ERR_OPEN_SHADE_UPGRADE::ID_FACTION_NOT_EXIST;
    }

    BOOL bRet = IsOpen();
    if ( !bRet )
    {
        return ERR_OPEN_SHADE_UPGRADE::ID_SHADE_NOT_EXIST;
    }

    //检查权限
    bRet = CFactionMgr::Instance()->CheckPlayerAuthority(m_dwPlayerID, EFA_Build);
    stShadeUpgradeInfo.byIsUpgradeLimit = bRet ? ESM_YES : ESM_NO;
    //重置数据
    EachUpdataShade();

    UINT32 dwShadeLevel = poFaction->GetFactionBuildLevel(EFB_Shade);

    return GetShadeUpgradeInfo( stShadeUpgradeInfo, dwFactionID, dwShadeLevel);
}

UINT16 CShade::ShadeUpgrade(PKT_CLIGS_SHADE_UPGRADE_ACK& stAck)
{
    CFaction* poFaction = GetFaction();
    UINT32 dwFactionID = GetFactionID();
    if ( NULL == poFaction)
    {
        return ERR_SHADE_UPGRADE::ID_FACTION_NOT_EXIST;
    }

    BOOL bRet = IsOpen();
    if ( !bRet )
    {
        return ERR_SHADE_UPGRADE::ID_SHADE_NOT_OPEN;
    }

    //检查权限
    bRet = CFactionMgr::Instance()->CheckPlayerAuthority(m_dwPlayerID, EFA_Build);
    if ( FALSE == bRet)
    {
        return ERR_SHADE_UPGRADE::ID_LIMIT_NOT_ENOUGH;
    }

     bRet = CheckTaskOpen(EFTT_HUNTING);
     if ( bRet )
     {
         return ERR_SHADE_UPGRADE::ID_ACTIVI_NOT_UPGRADE;
     }
    //更新配置
    EachUpdataShade();
    DT_FACTION_COMMON_DATA& stFactionTaskScheduleData = poFaction->GetDT_FACTION_COMMON_DATA();
	stAck.stNewHuntingScheduleInfo.byLastSchedule = stFactionTaskScheduleData.stHuntingTaskSchedule.byCurCollectiveTaskSchedule;

    UINT32 dwShadeLevel = poFaction->GetFactionBuildLevel(EFB_Shade);
    DT_SHADE_UPGRADE_INFO stShadeUpgradeInfo = {0};

    UINT16 wRet = GetShadeUpgradeInfo( stShadeUpgradeInfo, dwFactionID, dwShadeLevel);

    if ( ERR_OPEN_SHADE_UPGRADE::ID_SHADE_LEVEL_IS_FULL == wRet )
    {
        return ERR_SHADE_UPGRADE::ID_SHADE_LEVEL_IS_FULL;
    }
    else if ( 0 == stShadeUpgradeInfo.byFactionFundsEnough )
    {
        return ERR_SHADE_UPGRADE::ID_FACTION_FUNDS_NOT_ENOUGH;
    }
    else if ( 0 == stShadeUpgradeInfo.byFactionLevelEnough)
    {
        return ERR_SHADE_UPGRADE::ID_FACTION_LEVEL_NOT_ENOUGH;
    }

    DT_FACTION_BASE_DATA& stFactionBaseData = poFaction->GetDT_FACTION_BASE_DATA();

    //减去升级消耗门派资金
    poFaction->DecFactionFunds(stShadeUpgradeInfo.dwUpgradeNeedFactionFunds, CRecordMgr::EDFF_UPGRADE_SHADE);

    //暗部等级提升
    stFactionBaseData.dwShadeLevel += 1;

    //日志
    DT_FACTION_LOG_DATA stFactionLogData;
    stFactionLogData.dwPlayerID1 = m_dwPlayerID;
    DT_FACTION_PLAYER_CLI stFactionPlayerCli = {0};
    poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli, m_dwPlayerID);
    stFactionLogData.byPlayerJobType1 = stFactionPlayerCli.byJobType;
    stFactionLogData.byAction = EFLID_UpgradeBuild;
    stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
    stFactionLogData.adwParamList[0] = stShadeUpgradeInfo.dwUpgradeNeedFactionFunds;//需要资金
    stFactionLogData.adwParamList[1] = EFB_Shade;//建筑ID
    stFactionLogData.adwParamList[2] = stFactionBaseData.dwShadeLevel;//提升后的等级
    poFaction->AddFactionLogData(stFactionLogData);

    //根据升级效果提升
    UpdataUpgradeData();

    GetHuntingInfoCli(stAck.stAfterUpgradeHuntingInfo, stFactionTaskScheduleData, stAck.stNewHuntingScheduleInfo);

    wRet = GetShadeUpgradeInfo(stAck.stNextUpgradeInfo, dwFactionID, stFactionBaseData.dwShadeLevel);
    //没有下一级信息时，满级
    if( ERR_OPEN_SHADE_UPGRADE::ID_SHADE_LEVEL_IS_FULL == wRet )
    {
        stAck.byFullLevelMark = ESM_YES;
    }
    else
    {
        stAck.byFullLevelMark = ESM_NO;
    }

    stAck.byShadeLevel = stFactionBaseData.dwShadeLevel;
    stAck.dwFactionFunds = poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds;
    return ERR_SHADE_UPGRADE::ID_SUCCESS;
}

UINT16 CShade::OpenHunting(PKT_CLIGS_OPEN_HUNTING_ACK& stAck)
{
    CFaction* poFaction = GetFaction();
    if ( 0 == poFaction)
    {
        return ERR_OPEN_HUNTING::ID_FACTION_NOT_EXIST;
    }
    BOOL bRet = IsOpen();
    if ( !bRet )
    {
        return ERR_OPEN_HUNTING::ID_SHADE_NOT_EXIST;
    }

    bRet = CheckTaskOpen(EFTT_HUNTING);
    if ( FALSE == bRet)
    {
        return ERR_OPEN_HUNTING::ID_TIME_NOT_ENOUGH;
    }

    //更新配置
    EachUpdataShade();
    DT_FACTION_COMMON_DATA& stFactionTaskScheduleData = poFaction->GetDT_FACTION_COMMON_DATA();
	stAck.stNewHuntingScheduleInfo.byLastSchedule = stFactionTaskScheduleData.stHuntingTaskSchedule.byCurCollectiveTaskSchedule;
    //赋值所有奖励信息
    GetRewardInfoAll(stAck.stRewardInfoAll);
    //赋值其它信息
    GetHuntingOtherInfo(stAck.stOtherInfo);

    //获取妖怪信息
    UINT8 byMonsterNum = m_stShadeData.stHuntingInfo.byMonsterNum;
    if (0 == byMonsterNum)
    {
        stAck.byMonsterExist = ESM_NO;
        memset(&stAck.stMonsterInfo, 0, sizeof(stAck.stMonsterInfo));
    }
    else
    {
        stAck.byMonsterExist = ESM_YES;
        GetMonsterInfo( stAck.stMonsterInfo);
    }

    stAck.byOpenGoldFlag = m_stShadeData.stHuntingInfo.byGoldModeFlag;
    stAck.byVIPLevel = CVipPropMgr::Instance()->GetOpenLevel(EVF_OPENGOLDMODE);
    stAck.dwNeedGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_FACTION_OPEN_GOLD, 0);
	DT_HUNTING_SCHEDULE_INFO stInfoTemp;
	GetScheduleInfo(stInfoTemp, stAck.stNewHuntingScheduleInfo);
    return ERR_OPEN_HUNTING::ID_SUCCESS;
}

UINT16 CShade::FindMonster(vector<UINT16>& vecMonsterOrange, DT_MONSTER_INFO& stMonsterInfo )
{
    CFaction* poFaction = GetFaction();
    if ( NULL == poFaction)
    {
        return ERR_FIND_MONSTER::ID_FACTION_NOT_EXIST;
    }

    BOOL bRet = IsOpen();
    if ( !bRet )
    {
        return ERR_FIND_MONSTER::ID_SHADE_NOT_EXIST;
    }

    bRet = CheckTaskOpen(EFTT_HUNTING);
    if ( FALSE == bRet)
    {
        return ERR_FIND_MONSTER::ID_TIME_NOT_ENOUGH;
    }
    //更新配置
    EachUpdataShade();
    DT_FACTION_COMMON_DATA& stFactionTaskScheduleData = poFaction->GetDT_FACTION_COMMON_DATA();

    UINT8 byVIPLevel = m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel;
    //查看VIP等级
    bRet = CVipPropMgr::Instance()->CkOpenFunc(EVF_FINDMONSTER, byVIPLevel);
    if (FALSE == bRet)
    {
        return ERR_COMMON::ID_VIP_LEVEL_NOT_REACH;
    }

    if (EC_ORANGE == m_stShadeData.stHuntingInfo.byCurMonsterType)
    {
        return ERR_FIND_MONSTER::ID_IS_MAX_MONSTER;
    }

    if (stFactionTaskScheduleData.stHuntingTaskSchedule.dwMaxHuntingNum <= m_stShadeData.stHuntingInfo.dwHuntionNum)
    {
        return ERR_FIND_MONSTER::ID_HUNTING_NUM_NOT_ENOUGH_FIND;
    }
    //查看元宝
    UINT64 qwGold = m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold;
    UINT64 qwNeedGold = CLuamgr::Instance()->GetFindMonsterGold();
    if ( qwGold < qwNeedGold)
    {
        return ERR_FIND_MONSTER::ID_GOLD_NOT_ENOUGH;
    }
    m_poOwner->DecGold(static_cast<UINT32>(qwNeedGold), CRecordMgr::EDGT_FIND_ORANGE_MONSTER);
    m_stShadeData.stHuntingInfo.byMonsterNum = vecMonsterOrange.size();
    UINT8 byMonsterNum = 0;
    for ( vector<UINT16>::iterator itr = vecMonsterOrange.begin(); itr != vecMonsterOrange.end(); itr++)
    {
        m_stShadeData.stHuntingInfo.awMonsterGroup[byMonsterNum++] = *itr;
    }
    m_stShadeData.stHuntingInfo.byCurMonsterType = EC_ORANGE;
    GetMonsterInfo( stMonsterInfo);
    return ERR_FIND_MONSTER::ID_SUCCESS;
}

UINT16 CShade::ChangeMonster(vector<UINT16>& vecMonsterBlue, vector<UINT16>& vecMonsterPurple, vector<UINT16>& vecMonsterRed,
                             vector<UINT16>& vecMonsterOrange, DT_MONSTER_INFO& stMonsterInfo, UINT32& dwGoldNum )
{
    CFaction* poFaction = GetFaction();
    if ( 0 == poFaction)
    {
        return ERR_FIND_MONSTER::ID_FACTION_NOT_EXIST;
    }

    BOOL bRet = IsOpen();
    if ( !bRet )
    {
        return ERR_FIND_MONSTER::ID_SHADE_NOT_EXIST;
    }

    bRet = CheckTaskOpen(EFTT_HUNTING);
    if ( FALSE == bRet)
    {
        return ERR_FIND_MONSTER::ID_TIME_NOT_ENOUGH;
    }
    //更新配置
    EachUpdataShade();
    if( EC_ORANGE == m_stShadeData.stHuntingInfo.byCurMonsterType )
    {
        return ERR_FIND_MONSTER::ID_IS_MAX_MONSTER;
    }

    if ( poFaction->GetDT_FACTION_COMMON_DATA().stHuntingTaskSchedule.dwMaxHuntingNum <= m_stShadeData.stHuntingInfo.dwHuntionNum)
    {
        return ERR_FIND_MONSTER::ID_HUNTING_NUM_NOT_ENOUGH_CHANGE;
    }
    //获取免费信息
    //已更换次数
    UINT8& byChangeNum = m_stShadeData.stHuntingInfo.byHuntingChangeNum;
    //当前花费
    UINT32 dwCost = 0;

    UINT8 byMaxFreeNum = CLuamgr::Instance()->GetFreeRefreshNum();
    //已更换次数
    if ( byMaxFreeNum <= byChangeNum)
    {
        dwCost = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_SHADE_CHANGE_MONSTER, 0, byChangeNum - byMaxFreeNum + 1);
    }
    else
    {
        dwCost = 0;
    }


    //检查资金
    if (0 != dwCost)
    {
        if (m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < dwCost)
        {
            return ERR_FIND_MONSTER::ID_GOLD_NOT_ENOUGH;
        }
        m_poOwner->DecGold(dwCost, CRecordMgr::EDGT_CHANGE_MONSTER);
        dwGoldNum = static_cast<UINT32>(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold);
    }
    //换妖+1
    byChangeNum += 1;
    UINT8 byMonsterType = CShadePropMgr::Instance()->GetMonsterType(m_stShadeData.stHuntingInfo.byCurMonsterType);
    m_stShadeData.stHuntingInfo.byCurMonsterType = byMonsterType;


    UINT8 byMonsterNum = vecMonsterBlue.size();
    vector<UINT16>::iterator itr;
    switch (byMonsterType)
    {
    case EC_BLUE:
        itr = vecMonsterBlue.begin();
        break;
    case EC_PURPLE:
        itr = vecMonsterPurple.begin();
        break;
    case EC_RED:
        itr = vecMonsterRed.begin();
        break;
    case EC_ORANGE:
        itr = vecMonsterOrange.begin();
        break;
    }
    for (UINT8 byIndex = 0; byIndex < byMonsterNum; byIndex++)
    {
        m_stShadeData.stHuntingInfo.awMonsterGroup[byIndex] = *itr;
        itr++;
    }
    GetMonsterInfo( stMonsterInfo);
    m_stShadeData.stHuntingInfo.byMonsterNum = byMonsterNum;

    return ERR_FIND_MONSTER::ID_SUCCESS;
}

UINT16 CShade::OnHuntingBattle(PKT_CLIGS_HUNTING_BATTLE_ACK5& stAck)
{
    CFaction* poFaction = GetFaction();
    if (NULL == poFaction)
    {
        return ERR_HUNTING_BATTLE::ID_FACTION_NOT_EXIST;
    }

    BOOL bRet = IsOpen();
    if ( !bRet )
    {
        return ERR_HUNTING_BATTLE::ID_SHADE_NOT_EXIST;
    }

    //更新配置
    EachUpdataShade();
    DT_FACTION_COMMON_DATA& stFactionTaskScheduleData = poFaction->GetDT_FACTION_COMMON_DATA();
    //当前妖怪
	stAck.stNewHuntingScheduleInfo.byLastSchedule = stFactionTaskScheduleData.stHuntingTaskSchedule.byCurCollectiveTaskSchedule;
    if ( 0 == m_stShadeData.stHuntingInfo.byMonsterNum)
    {
        return ERR_HUNTING_BATTLE::ID_MONSTER_NOT_EXIST;
    }

    DT_MONSTER_INFO stMonsterInfo = {0};
    GetMonsterInfo(stMonsterInfo);

    vector<UINT16> vecMonsterID;
    for (UINT8 byIndex = 0; byIndex < m_stShadeData.stHuntingInfo.byMonsterNum; byIndex++)
    {
        vecMonsterID.push_back(m_stShadeData.stHuntingInfo.awMonsterGroup[byIndex]);
    }
    UINT16 wPlayerLevel = m_poOwner->GetLevel();
    CShadeMonsterGroup* pMonster = CShadeMonsterMgr::Instance()->CreateShadeMonsterGroup(wPlayerLevel, m_stShadeData.stHuntingInfo.byCurMonsterType, vecMonsterID);
    CBattleMgr::Instance()->Battle4(m_poOwner, pMonster, stAck.stBattleInfo);
    //替换战斗数据
    CShadeMonsterMgr::Instance()->RemoveShadeMonsterGroup(pMonster);
    //对方赢没有奖励
    if ( 1 == stAck.stBattleInfo.byBattleResult)
    {
        memset(&stAck.stBattleReward, 0, sizeof(DT_REWARD_INFO));
    }
    //我方赢
    else
    {
        stAck.stBattleReward = stMonsterInfo.stMonsterReward;
        SHuntingRewardMultiMap& mapRewardGoods = CShadePropMgr::Instance()->GetHuntingReward();
        UINT32 dwMonsterType = m_stShadeData.stHuntingInfo.byCurMonsterType;
        size_t dwSize = mapRewardGoods.count(dwMonsterType);
        SHuntingRewardMultiItr it = mapRewardGoods.find(dwMonsterType);
        for (size_t dwIndex = 0; dwIndex < dwSize; dwIndex++, it++)
        {
            BOOL bRet = GetGoodsByRandom(it->second.dwOutProbability);
            if (bRet)
            {
                AddRewardByShade(it->second.dwRewardType, it->second.dwRewardValue);
            }
        }
        //胜利之后清空怪物组
        m_stShadeData.stHuntingInfo.byMonsterNum = 0;
        memset(m_stShadeData.stHuntingInfo.awMonsterGroup, 0, sizeof(m_stShadeData.stHuntingInfo.awMonsterGroup));
        m_stShadeData.stHuntingInfo.dwHuntionNum += 1;
        if ( EC_ORANGE == dwMonsterType)
        {
            //日志
            DT_FACTION_LOG_DATA stFactionLogData;
            stFactionLogData.dwPlayerID1 = m_dwPlayerID;
            DT_FACTION_PLAYER_CLI stFactionPlayerCli = {0};
            poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli, m_dwPlayerID);
            stFactionLogData.byPlayerJobType1 = stFactionPlayerCli.byJobType;
            stFactionLogData.byAction = EFLID_CatchOrange;
            stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
            poFaction->AddFactionLogData(stFactionLogData);
        }
    }


    GetScheduleInfo(stAck.stTaskScheduleInfo, stAck.stNewHuntingScheduleInfo);


    if (m_stShadeData.stHuntingInfo.qwLastSyncIntegral < stFactionTaskScheduleData.stHuntingTaskSchedule.qwLastChangeScheduleTime )
    {
        stAck.byScheduleChangeFlag = ESM_YES;
        m_stShadeData.stHuntingInfo.qwLastSyncIntegral = SGDP::SDTimeMicroSec();
    }
    else
    {
        stAck.byScheduleChangeFlag = ESM_NO;
    }

    UINT32 dwMaxNum = stFactionTaskScheduleData.stHuntingTaskSchedule.dwMaxHuntingNum;
    UINT32 dwCurNum = m_stShadeData.stHuntingInfo.dwHuntionNum;
    stAck.byHuntingLeftNum = dwMaxNum < dwCurNum ? 0 : dwMaxNum - dwCurNum;
    //收妖所需元宝
    stAck.dwClosedMonsterNeedGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_FACTION_CLOSE_MONSTER, 0);
    stAck.byIsGoldMode = m_stShadeData.stHuntingInfo.byGoldModeFlag;
    stAck.dwSyncDoorsTribute = static_cast<UINT32>(m_poOwner->GetSyncReward(ACTE_DoorsTribute));
    return ERR_HUNTING_BATTLE::ID_SUCCESS;
}

UINT16 CShade::OnHuntingBattle2(PKT_CLIGS_HUNTING_BATTLE_ACK2& stAck)
{
    CFaction* poFaction = GetFaction();
    if (NULL == poFaction)
    {
        return ERR_HUNTING_BATTLE::ID_FACTION_NOT_EXIST;
    }

    BOOL bRet = IsOpen();
    if ( !bRet )
    {
        return ERR_HUNTING_BATTLE::ID_SHADE_NOT_EXIST;
    }

    //更新配置
    EachUpdataShade();
    DT_FACTION_COMMON_DATA& stFactionTaskScheduleData = poFaction->GetDT_FACTION_COMMON_DATA();
    //当前妖怪
    stAck.stNewHuntingScheduleInfo.byLastSchedule = stFactionTaskScheduleData.stHuntingTaskSchedule.byCurCollectiveTaskSchedule;
    if ( 0 == m_stShadeData.stHuntingInfo.byMonsterNum)
    {
        return ERR_HUNTING_BATTLE::ID_MONSTER_NOT_EXIST;
    }

    DT_MONSTER_INFO stMonsterInfo = {0};
    GetMonsterInfo(stMonsterInfo);

    vector<UINT16> vecMonsterID;
    for (UINT8 byIndex = 0; byIndex < m_stShadeData.stHuntingInfo.byMonsterNum; byIndex++)
    {
        vecMonsterID.push_back(m_stShadeData.stHuntingInfo.awMonsterGroup[byIndex]);
    }
    UINT16 wPlayerLevel = m_poOwner->GetLevel();
    CShadeMonsterGroup* pMonster = CShadeMonsterMgr::Instance()->CreateShadeMonsterGroup(wPlayerLevel, m_stShadeData.stHuntingInfo.byCurMonsterType, vecMonsterID);
    CBattleMgr::Instance()->Battle2(m_poOwner, pMonster, stAck.stBattleInfo);
    //替换战斗数据
    CShadeMonsterMgr::Instance()->RemoveShadeMonsterGroup(pMonster);
    //对方赢没有奖励
    if ( 1 == stAck.stBattleInfo.byBattleResult)
    {
        memset(&stAck.stBattleReward, 0, sizeof(DT_REWARD_INFO));
    }
    //我方赢
    else
    {
        stAck.stBattleReward = stMonsterInfo.stMonsterReward;
        SHuntingRewardMultiMap& mapRewardGoods = CShadePropMgr::Instance()->GetHuntingReward();
        UINT32 dwMonsterType = m_stShadeData.stHuntingInfo.byCurMonsterType;
        size_t dwSize = mapRewardGoods.count(dwMonsterType);
        SHuntingRewardMultiItr it = mapRewardGoods.find(dwMonsterType);
        for (size_t dwIndex = 0; dwIndex < dwSize; dwIndex++, it++)
        {
            BOOL bRet = GetGoodsByRandom(it->second.dwOutProbability);
            if (bRet)
            {
                AddRewardByShade(it->second.dwRewardType, it->second.dwRewardValue);
            }
        }
        //胜利之后清空怪物组
        m_stShadeData.stHuntingInfo.byMonsterNum = 0;
        memset(m_stShadeData.stHuntingInfo.awMonsterGroup, 0, sizeof(m_stShadeData.stHuntingInfo.awMonsterGroup));
        m_stShadeData.stHuntingInfo.dwHuntionNum += 1;
        if ( EC_ORANGE == dwMonsterType)
        {
            //日志
            DT_FACTION_LOG_DATA stFactionLogData;
            stFactionLogData.dwPlayerID1 = m_dwPlayerID;
            DT_FACTION_PLAYER_CLI stFactionPlayerCli = {0};
            poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli, m_dwPlayerID);
            stFactionLogData.byPlayerJobType1 = stFactionPlayerCli.byJobType;
            stFactionLogData.byAction = EFLID_CatchOrange;
            stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
            poFaction->AddFactionLogData(stFactionLogData);
        }
    }


    GetScheduleInfo(stAck.stTaskScheduleInfo, stAck.stNewHuntingScheduleInfo);


    if (m_stShadeData.stHuntingInfo.qwLastSyncIntegral < stFactionTaskScheduleData.stHuntingTaskSchedule.qwLastChangeScheduleTime )
    {
        stAck.byScheduleChangeFlag = ESM_YES;
        m_stShadeData.stHuntingInfo.qwLastSyncIntegral = SGDP::SDTimeMicroSec();
    }
    else
    {
        stAck.byScheduleChangeFlag = ESM_NO;
    }

    UINT32 dwMaxNum = stFactionTaskScheduleData.stHuntingTaskSchedule.dwMaxHuntingNum;
    UINT32 dwCurNum = m_stShadeData.stHuntingInfo.dwHuntionNum;
    stAck.byHuntingLeftNum = dwMaxNum < dwCurNum ? 0 : dwMaxNum - dwCurNum;
    //收妖所需元宝
    stAck.dwClosedMonsterNeedGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_FACTION_CLOSE_MONSTER, 0);
    stAck.byIsGoldMode = m_stShadeData.stHuntingInfo.byGoldModeFlag;
    stAck.dwSyncDoorsTribute = static_cast<UINT32>(m_poOwner->GetSyncReward(ACTE_DoorsTribute));
    return ERR_HUNTING_BATTLE::ID_SUCCESS;
}

UINT16 CShade::OnHuntingBattle3(PKT_CLIGS_HUNTING_BATTLE_ACK3& stAck)
{
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return ERR_HUNTING_BATTLE::ID_FACTION_NOT_EXIST;
	}

	BOOL bRet = IsOpen();
	if ( !bRet )
	{
		return ERR_HUNTING_BATTLE::ID_SHADE_NOT_EXIST;
	}

	//更新配置
	EachUpdataShade();
	DT_FACTION_COMMON_DATA& stFactionTaskScheduleData = poFaction->GetDT_FACTION_COMMON_DATA();
	//当前妖怪
	stAck.stNewHuntingScheduleInfo.byLastSchedule = stFactionTaskScheduleData.stHuntingTaskSchedule.byCurCollectiveTaskSchedule;
	if ( 0 == m_stShadeData.stHuntingInfo.byMonsterNum)
	{
		return ERR_HUNTING_BATTLE::ID_MONSTER_NOT_EXIST;
	}

	DT_MONSTER_INFO stMonsterInfo = {0};
	GetMonsterInfo(stMonsterInfo);

	vector<UINT16> vecMonsterID;
	for (UINT8 byIndex = 0; byIndex < m_stShadeData.stHuntingInfo.byMonsterNum; byIndex++)
	{
		vecMonsterID.push_back(m_stShadeData.stHuntingInfo.awMonsterGroup[byIndex]);
	}
	UINT16 wPlayerLevel = m_poOwner->GetLevel();
	CShadeMonsterGroup* pMonster = CShadeMonsterMgr::Instance()->CreateShadeMonsterGroup(wPlayerLevel, m_stShadeData.stHuntingInfo.byCurMonsterType, vecMonsterID);
	CBattleMgr::Instance()->Battle3(m_poOwner, pMonster, stAck.stBattleInfo);
	//替换战斗数据
	CShadeMonsterMgr::Instance()->RemoveShadeMonsterGroup(pMonster);
	//对方赢没有奖励
	if ( 1 == stAck.stBattleInfo.byBattleResult)
	{
		memset(&stAck.stBattleReward, 0, sizeof(DT_REWARD_INFO));
	}
	//我方赢
	else
	{
		stAck.stBattleReward = stMonsterInfo.stMonsterReward;
		SHuntingRewardMultiMap& mapRewardGoods = CShadePropMgr::Instance()->GetHuntingReward();
		UINT32 dwMonsterType = m_stShadeData.stHuntingInfo.byCurMonsterType;
		size_t dwSize = mapRewardGoods.count(dwMonsterType);
		SHuntingRewardMultiItr it = mapRewardGoods.find(dwMonsterType);
		for (size_t dwIndex = 0; dwIndex < dwSize; dwIndex++, it++)
		{
			BOOL bRet = GetGoodsByRandom(it->second.dwOutProbability);
			if (bRet)
			{
				AddRewardByShade(it->second.dwRewardType, it->second.dwRewardValue);
			}
		}
		//胜利之后清空怪物组
		m_stShadeData.stHuntingInfo.byMonsterNum = 0;
		memset(m_stShadeData.stHuntingInfo.awMonsterGroup, 0, sizeof(m_stShadeData.stHuntingInfo.awMonsterGroup));
		m_stShadeData.stHuntingInfo.dwHuntionNum += 1;
		if ( EC_ORANGE == dwMonsterType)
		{
			//日志
			DT_FACTION_LOG_DATA stFactionLogData;
			stFactionLogData.dwPlayerID1 = m_dwPlayerID;
			DT_FACTION_PLAYER_CLI stFactionPlayerCli = {0};
			poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli, m_dwPlayerID);
			stFactionLogData.byPlayerJobType1 = stFactionPlayerCli.byJobType;
			stFactionLogData.byAction = EFLID_CatchOrange;
			stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
			poFaction->AddFactionLogData(stFactionLogData);
		}
	}


	GetScheduleInfo(stAck.stTaskScheduleInfo, stAck.stNewHuntingScheduleInfo);


	if (m_stShadeData.stHuntingInfo.qwLastSyncIntegral < stFactionTaskScheduleData.stHuntingTaskSchedule.qwLastChangeScheduleTime )
	{
		stAck.byScheduleChangeFlag = ESM_YES;
		m_stShadeData.stHuntingInfo.qwLastSyncIntegral = SGDP::SDTimeMicroSec();
	}
	else
	{
		stAck.byScheduleChangeFlag = ESM_NO;
	}

	UINT32 dwMaxNum = stFactionTaskScheduleData.stHuntingTaskSchedule.dwMaxHuntingNum;
	UINT32 dwCurNum = m_stShadeData.stHuntingInfo.dwHuntionNum;
	stAck.byHuntingLeftNum = dwMaxNum < dwCurNum ? 0 : dwMaxNum - dwCurNum;
	//收妖所需元宝
	stAck.dwClosedMonsterNeedGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_FACTION_CLOSE_MONSTER, 0);
	stAck.byIsGoldMode = m_stShadeData.stHuntingInfo.byGoldModeFlag;
	stAck.dwSyncDoorsTribute = static_cast<UINT32>(m_poOwner->GetSyncReward(ACTE_DoorsTribute));
	return ERR_HUNTING_BATTLE::ID_SUCCESS;
}

UINT16 CShade::FirstFindMonster(vector<UINT16>& vecMonsterBlue, vector<UINT16>& vecMonsterPurple, vector<UINT16>& vecMonsterRed,
                                vector<UINT16>& vecMonsterOrange, DT_MONSTER_INFO& stMonsterInfo)
{
    CFaction* poFaction = GetFaction();
    if (NULL == poFaction)
    {
        return ERR_FIND_MONSTER::ID_FACTION_NOT_EXIST;
    }

    BOOL bRet = IsOpen();
    if ( !bRet )
    {
        return ERR_FIND_MONSTER::ID_SHADE_NOT_EXIST;
    }

    bRet = CheckTaskOpen(EFTT_HUNTING);
    if ( FALSE == bRet)
    {
        return ERR_FIND_MONSTER::ID_TIME_NOT_ENOUGH;
    }

    //重置捉妖次数
    EachUpdataShade();


    if( 0 != m_stShadeData.stHuntingInfo.byMonsterNum)
    {
        return ERR_FIND_MONSTER::ID_MONSTER_IS_EXIST;
    }


    if (poFaction->GetDT_FACTION_COMMON_DATA().stHuntingTaskSchedule.dwMaxHuntingNum <= m_stShadeData.stHuntingInfo.dwHuntionNum)
    {
        return ERR_FIND_MONSTER::ID_NOT_FIND_MONSTER_NUM;
    }

    UINT8 byMonsterType = CShadePropMgr::Instance()->GetMonsterType(m_stShadeData.stHuntingInfo.byCurMonsterType, m_stShadeData.stHuntingInfo.byAbandomMonster);
    vector<UINT16>::iterator itr;
    switch(byMonsterType)
    {
    case EC_BLUE:
        itr = vecMonsterBlue.begin();
        break;
    case EC_PURPLE:
        itr = vecMonsterPurple.begin();
        break;
    case EC_RED:
        itr = vecMonsterRed.begin();
        break;
    case EC_ORANGE:
        itr = vecMonsterOrange.begin();
        break;
    }
    m_stShadeData.stHuntingInfo.byMonsterNum = vecMonsterBlue.size();
    for (UINT8 byIndex = 0; byIndex < m_stShadeData.stHuntingInfo.byMonsterNum; byIndex++)
    {
        m_stShadeData.stHuntingInfo.awMonsterGroup[byIndex] = *itr;
        itr++;
    }
    m_stShadeData.stHuntingInfo.byCurMonsterType = byMonsterType;
    GetMonsterInfo( stMonsterInfo);

    return ERR_FIND_MONSTER::ID_SUCCESS;
}

UINT16 CShade::ClosedMonster(UINT8 byChoice, PKT_CLIGS_CLOSED_MONSTER_ACK& stAck)
{
    CFaction* poFaction = GetFaction();
    if (NULL == poFaction)
    {
        return ERR_CLOSED_MONSTER::ID_FACTION_NOT_EXIST;
    }

    BOOL bRet = IsOpen();
    if ( !bRet )
    {
        return ERR_CLOSED_MONSTER::ID_SHADE_NOT_EXIST;
    }
    DT_FACTION_HUNTING_SCHEDULE_DATA& stData = poFaction->GetDT_FACTION_COMMON_DATA().stHuntingTaskSchedule;
	stAck.stNewHuntingScheduleInfo.byLastSchedule = stData.byCurCollectiveTaskSchedule;
    if(0 == m_stShadeData.stHuntingInfo.byMonsterNum)
    {
        return ERR_CLOSED_MONSTER::ID_MONSTER_NOT_EXIST;
    }

    if (ESM_YES == byChoice)
    {
        UINT32 dwCloseMonsterGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_FACTION_CLOSE_MONSTER, 0);
        if( m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < dwCloseMonsterGold)
        {
            return ERR_CLOSED_MONSTER::ID_GOLD_NOT_ENOUGH;
        }

        m_poOwner->DecGold(dwCloseMonsterGold, CRecordMgr::EDGT_CLOSE_MONSTER);
        DT_MONSTER_INFO stMonsterInfo = {0};
        GetMonsterInfo( stMonsterInfo);
        //发放奖励
        m_stShadeData.stHuntingInfo.dwHuntionNum += 1;
        BOOL bRet = FALSE;
        for(UINT8 byIndex = 0 ; byIndex < stMonsterInfo.stMonsterReward.byRewardNum; byIndex++)
        {
            bRet = GetGoodsByRandom(stMonsterInfo.stMonsterReward.astRewardMsg[byIndex].byOutProbability);
            if (bRet)
            {
                stAck.astRewardMsg[stAck.byRewardNum] = stMonsterInfo.stMonsterReward.astRewardMsg[byIndex];
                stAck.astRewardMsg[stAck.byRewardNum].byOutProbability = 100;
                AddRewardByShade(stAck.astRewardMsg[stAck.byRewardNum].byGoodsType, stAck.astRewardMsg[stAck.byRewardNum].dwRewardValue);
                stAck.byRewardNum++;
            }
        }

        m_stShadeData.stHuntingInfo.byAbandomMonster = ESM_NO;

       
        if (1 != stData.byCurCollectiveTaskSchedule)
        {
            if (m_stShadeData.stHuntingInfo.qwLastSyncIntegral < stData.qwLastChangeScheduleTime )
            {
                stAck.byScheduleChangeFlag = ESM_YES;
                m_stShadeData.stHuntingInfo.qwLastSyncIntegral = SGDP::SDTimeMicroSec();
            }
            else
            {
                stAck.byScheduleChangeFlag = ESM_NO;
            }
        }

        if (EC_ORANGE == stMonsterInfo.stMonsterReward.byRewardType)
        {
            //日志
            DT_FACTION_LOG_DATA stFactionLogData;
            stFactionLogData.dwPlayerID1 = m_dwPlayerID;
            DT_FACTION_PLAYER_CLI stFactionPlayerCli = {0};
            poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli, m_dwPlayerID);
            stFactionLogData.byPlayerJobType1 = stFactionPlayerCli.byJobType;
            stFactionLogData.byAction = EFLID_CatchOrange;
            stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
            poFaction->AddFactionLogData(stFactionLogData);
        }

    }
    else
    {
        m_stShadeData.stHuntingInfo.byAbandomMonster = ESM_YES;
    }

	//同步积分
	GetScheduleInfo(stAck.stTaskScheduleInfo, stAck.stNewHuntingScheduleInfo);
    m_stShadeData.stHuntingInfo.byMonsterNum = 0;
    memset(m_stShadeData.stHuntingInfo.awMonsterGroup, 0, sizeof(m_stShadeData.stHuntingInfo.awMonsterGroup));
    UINT32 dwMaxNum = stData.dwMaxHuntingNum;
    UINT32 dwCurNum = m_stShadeData.stHuntingInfo.dwHuntionNum;
    stAck.byHuntingLeftNum = dwMaxNum < dwCurNum ? 0 : dwMaxNum - dwCurNum;
    stAck.byCloseMonsterChoice = byChoice;
    stAck.dwSyncDoorsTribute = static_cast<UINT32>(m_poOwner->GetSyncReward(ACTE_DoorsTribute));
    stAck.qwSyncGold = m_poOwner->GetSyncReward(ACTE_GOLD);
    stAck.byIsGoldMode = m_stShadeData.stHuntingInfo.byGoldModeFlag;
    return ERR_CLOSED_MONSTER::ID_SUCCESS;
}

VOID CShade::EachUpdataShade()
{
    CFaction* poFaction = GetFaction();
    UINT32 dwFactionID = GetFactionID();
    if (NULL == poFaction)
    {
        return ;
    }
    if ( !IsToday(m_stShadeData.qwLastUpdataTime))
    {
        //捉妖次数重置
        m_stShadeData.stHuntingInfo.dwHuntionNum = 0;
        //换怪重置
        m_stShadeData.stHuntingInfo.byHuntingChangeNum = 0;
        //黄金模式
        m_stShadeData.stHuntingInfo.byGoldModeFlag = 0;

        m_stShadeData.stHuntingInfo.byIsIgnorePrompt = 0;
        m_stShadeData.stHuntingInfo.qwLastSyncIntegral = 0;
        //更新时间
        m_stShadeData.qwLastUpdataTime = SGDP::SDTimeSecs();
    }

    DT_FACTION_HUNTING_SCHEDULE_DATA& stData = poFaction->GetDT_FACTION_COMMON_DATA().stHuntingTaskSchedule;
    if ( !IsToday(stData.qwLastUpdateTime))
    {
        SShadeTaskInfoProp stShadeTaskInfo = {0};
        CFactorPropMgr::Instance()->GetTaskInfoProp(EFTT_HUNTING, stShadeTaskInfo);
        ////任务档数
        UINT32 dwShadeLevel = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Shade);
        //等级上升，档数上升
        stData.byCollectiveTaskSchedule = CShadePropMgr::Instance()->GetHuntingMaxSchedule(dwShadeLevel);
        //任务ID
        stData.byTaskID = EFTT_HUNTING;
        //当前任务档数
        stData.byCurCollectiveTaskSchedule = 1;
        //当前积分
        stData.dwCurIntegral = 0;
        ////捉妖上限
        SShadeUpgradeProp stShadeProp = {0};
        CShadePropMgr::Instance()->GetShadeUpgradeProp(dwShadeLevel, stShadeProp);
        stData.dwMaxHuntingNum = stShadeProp.dwHuntingMonsterMaxNum;
        //当前最大积分
        STaskRewardProp stTaskRewardProp = {0};
        CFactorPropMgr::Instance()->GetTaskRewardProp( dwShadeLevel, stData.byCurCollectiveTaskSchedule, stTaskRewardProp);
        stData.dwMaxIntegral = stTaskRewardProp.dwMaxIntegral;//非黄金模式第一档上限积分
        stData.qwLastChangeScheduleTime = 0;
        stData.qwLastUpdateTime = SGDP::SDTimeSecs();
    }
}

//获取暗部信息
VOID CShade::GetShadeInfo(DT_SHADE_INFO& stShadeInfo)
{
    CFaction* poFaction = GetFaction();
    UINT32 dwFactionID = GetFactionID();
    if (NULL == poFaction)
    {
        return ;
    }
    stShadeInfo.byUpgradeLimit = CFactionMgr::Instance()->CheckPlayerAuthority(m_dwPlayerID, EFA_Build);

    stShadeInfo.qwCoin = m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin;
    stShadeInfo.qwGold = m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold;
    stShadeInfo.dwShadeLevel = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Shade);

    DT_FACTION_PLAYER_CLI stDT_FACTION_PLAYER_CLI = {0};
    poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stDT_FACTION_PLAYER_CLI, m_dwPlayerID);
    stShadeInfo.dwDoorsTribute = stDT_FACTION_PLAYER_CLI.dwDoorsTributeValues;

    stShadeInfo.byPlayerVIPLevel = m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel;
}

//获取捉跃信息
VOID CShade::GetHuntingInfoCli(DT_HUNTING_INFO_CLI& stHuntingInfoCli, DT_FACTION_COMMON_DATA& stFactionTaskScheduleData, DT_HUNTING_SCHEDULE_INFO2& stInfo2)
{
    SDStrcpy(stHuntingInfoCli.aszTaskBeginTimeMsg, CFactorPropMgr::Instance()->GetShadeTime().c_str());

    //任务描述
    SShadeTaskInfoProp stShadeTaskInfo = {0};
    CFactorPropMgr::Instance()->GetTaskInfoProp(EFTT_HUNTING, stShadeTaskInfo);
    SDStrcpy(stHuntingInfoCli.aszTaskDescMsg, stShadeTaskInfo.aszTaskDesc);
    stHuntingInfoCli.byTaskID = stFactionTaskScheduleData.stHuntingTaskSchedule.byTaskID;

    //剩余捉妖次数
    UINT32 dwLeftHuntingNum = 0;

    if (stFactionTaskScheduleData.stHuntingTaskSchedule.dwMaxHuntingNum < m_stShadeData.stHuntingInfo.dwHuntionNum)
    {
        dwLeftHuntingNum = 0;
    }
    else
    {
        dwLeftHuntingNum = stFactionTaskScheduleData.stHuntingTaskSchedule.dwMaxHuntingNum - m_stShadeData.stHuntingInfo.dwHuntionNum;
    }
    //剩余次数
    stHuntingInfoCli.byHuntingNum = dwLeftHuntingNum;
    BOOL bRet = CheckTaskOpen(EFTT_HUNTING);
    if (bRet)
    {
        stHuntingInfoCli.byActiviFlag = ESM_YES;
    }
    else
    {
        CHuntingEvent oEvent;
        oEvent.SetParam(TRUE);
        stHuntingInfoCli.dwHuntingCountDown = oEvent.GetInterval();
        stHuntingInfoCli.byActiviFlag = ESM_NO;;
    }
    GetScheduleInfo(stHuntingInfoCli.stHuntingScheduleInfo, stInfo2);

    GetDT_ACTIVITY_BASE_DATA(stHuntingInfoCli.stActiviInfo);
}

UINT16 CShade::GetShadeUpgradeInfo( DT_SHADE_UPGRADE_INFO& stShadeUpgradeInfo, UINT32 dwFactionID, UINT32 dwShadeLevel)
{
    CFaction* poFaction = GetFaction();
    if ( NULL == poFaction)
    {
        return ERR_OPEN_SHADE_UPGRADE::ID_FACTION_NOT_EXIST;
    }

    //获取配置
    SShadeUpgradeProp stCurShadeProp = {0};
    BOOL bRet = CShadePropMgr::Instance()->GetShadeUpgradeProp(dwShadeLevel, stCurShadeProp);

    if ( FALSE == bRet)
    {
        return ERR_OPEN_SHADE_UPGRADE::ID_SHADE_LEVEL_IS_FULL;
    }

    //所需资金， 等级要求
    stShadeUpgradeInfo.dwUpgradeNeedFactionFunds = stCurShadeProp.dwNeedFactionFunds;
    stShadeUpgradeInfo.dwUpgradeNeedFactionLevel = dwShadeLevel + 1;


    DT_FACTION_BASE_DATA stFactionBaseData = poFaction->GetDT_FACTION_BASE_DATA();
    //门派资金不足
    if ( stFactionBaseData.dwFactionFunds <  stShadeUpgradeInfo.dwUpgradeNeedFactionFunds)
    {
        stShadeUpgradeInfo.byFactionFundsEnough = 0;
    }
    else
    {
        stShadeUpgradeInfo.byFactionFundsEnough = 1;
    }

    //门派等级不足
    if ( stFactionBaseData.dwFactionLevel < stShadeUpgradeInfo.dwUpgradeNeedFactionLevel)
    {
        stShadeUpgradeInfo.byFactionLevelEnough = 0;
    }
    else
    {
        stShadeUpgradeInfo.byFactionLevelEnough = 1;
    }

    //升级效果获取
    SShadeUpgradeProp stNextShadeProp = {0};
    bRet = CShadePropMgr::Instance()->GetShadeUpgradeProp(dwShadeLevel + 1, stNextShadeProp);
    if ( FALSE == bRet)
    {
        return ERR_OPEN_SHADE_UPGRADE::ID_SHADE_LEVEL_IS_FULL;
    }
    string strMsg;
    vector<string> vecStr;
    UINT8 byMsgNum = 0;
    if ( ESM_NO == stCurShadeProp.byHuntingTask)
    {
        if ( ESM_YES == stNextShadeProp.byHuntingTask)
        {
            vecStr.clear();
            vecStr.push_back(CMsgDefMgr::Instance()->GetErrMsg("TASK_HUNTING", NULL));
            strMsg = CMsgDefMgr::Instance()->GetErrMsg("TASK_UNLOCK", &vecStr);
            SDStrncpy(stShadeUpgradeInfo.astUpgradeMsgLst[byMsgNum].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN - 1);
            byMsgNum++;
        }
    }
    TCHAR aszMsg[FACTION_UPGRADE_MSG_LEN] = {0};
    //捉妖次数
    UINT8 byDiffNum = stNextShadeProp.dwHuntingMonsterMaxNum - stCurShadeProp.dwHuntingMonsterMaxNum;
    if ( 0 != byDiffNum )
    {
        vecStr.clear();
        SDSnprintf(aszMsg, FACTION_UPGRADE_MSG_LEN - 1, "%u", byDiffNum);
        vecStr.push_back(aszMsg);
        strMsg = CMsgDefMgr::Instance()->GetErrMsg("HUNTING_TIME_UP", &vecStr);
        SDStrncpy(stShadeUpgradeInfo.astUpgradeMsgLst[byMsgNum].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN - 1);
        byMsgNum++;
    }
    ////档数上限//已取消显示
    //byDiffNum = stNextShadeProp.dwHuntingMonsterSchedule - stCurShadeProp.dwHuntingMonsterSchedule;
    //if ( 0 != byDiffNum)
    //{
    //    vecStr.clear();
    //    SDSnprintf(aszMsg, FACTION_UPGRADE_MSG_LEN - 1, "%u", byDiffNum);
    //    vecStr.push_back(aszMsg);
    //    strMsg = CMsgDefMgr::Instance()->GetErrMsg("TASK_SCHEDULE_UP", &vecStr);
    //    SDStrncpy(stShadeUpgradeInfo.astUpgradeMsgLst[byMsgNum].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN - 1);
    //    byMsgNum++;
    //}

    strMsg = CMsgDefMgr::Instance()->GetErrMsg("SHADE_UPGRADE_MSG", NULL);
    SDStrncpy(stShadeUpgradeInfo.astUpgradeMsgLst[byMsgNum].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN - 1);
    byMsgNum++;


    stShadeUpgradeInfo.byUpgradeMsgNum = byMsgNum;

    stShadeUpgradeInfo.byShadelevel = poFaction->GetFactionBuildLevel(EFB_Shade);
    return ERR_OPEN_SHADE_UPGRADE::ID_SUCCESS;
}

VOID CShade::UpdataUpgradeData()
{
    CFaction* poFaction = GetFaction();
    if ( NULL == poFaction)
    {
        return ;
    }

    UINT32 dwShadeLevel = poFaction->GetFactionBuildLevel(EFB_Shade);
    DT_FACTION_HUNTING_SCHEDULE_DATA& stHuntingData = poFaction->GetDT_FACTION_COMMON_DATA().stHuntingTaskSchedule;

    SShadeUpgradeProp stCurShadeProp = {0};
    BOOL bRet = CShadePropMgr::Instance()->GetShadeUpgradeProp(dwShadeLevel, stCurShadeProp);
    if ( FALSE == bRet)
    {
        return;
    }

    stHuntingData.byCollectiveTaskSchedule = stCurShadeProp.dwHuntingMonsterSchedule;
    stHuntingData.dwMaxHuntingNum = stCurShadeProp.dwHuntingMonsterMaxNum;

    STaskRewardProp stTaskRewardProp = {0};
    CFactorPropMgr::Instance()->GetTaskRewardProp( dwShadeLevel, stHuntingData.byCurCollectiveTaskSchedule, stTaskRewardProp);
    stHuntingData.dwMaxIntegral = stTaskRewardProp.dwMaxIntegral;

}

BOOL CShade::CheckTaskOpen(UINT8 byTaskType)
{
    CFaction* poFaction = GetFaction();
    if ( NULL == poFaction )
    {
        return FALSE;
    }

    //根据任务ID检查任务解锁等级
    switch (byTaskType)
    {
    case EFTT_HUNTING:
    {
        SShadeTaskInfoProp stShadeTaskProp = {0};
        CFactorPropMgr::Instance()->GetTaskInfoProp(EFTT_HUNTING, stShadeTaskProp);
        UINT32 dwPassSecond =  SGDP::GetDayPassSecond();
        if( stShadeTaskProp.dwStartTime < dwPassSecond && dwPassSecond < stShadeTaskProp.dwEndTime)
        {
            return TRUE;
        }
    }
    break;
    default:
        return FALSE;
        break;
    }

    return FALSE;
}

VOID CShade::GetRewardInfoAll(DT_REWARD_INFO_ALL& stRewardInfoAll)
{
    stRewardInfoAll.byTypeNum = MONSTER_TYPE_NUM;
    SHuntingRewardMultiMap& mapHuntingReward = CShadePropMgr::Instance()->GetHuntingReward();
    UINT32 dwIndex = 0;
    for (UINT8 byIndex = EC_BLUE; byIndex <= EC_ORANGE; byIndex++)
    {
        SHuntingMonsterProp stHuntingMonsterProp = {0};
        CShadePropMgr::Instance()->GetHuntingMonsterProp(byIndex, stHuntingMonsterProp);
        //品质类型标识
        stRewardInfoAll.astRewardInfo[dwIndex].byRewardType = stHuntingMonsterProp.dwMonsterType;
        //品质奖励个数
        stRewardInfoAll.astRewardInfo[dwIndex].byRewardNum = mapHuntingReward.count(byIndex);
        //品质信息
        PairHuntingReward itPair = mapHuntingReward.equal_range(byIndex);
        UINT32 dwNum = 0;
        for (SHuntingRewardMultiItr it = itPair.first; it != itPair.second; it++)
        {
            GetRewardInfoMsg(stRewardInfoAll.astRewardInfo[dwIndex].astRewardMsg[dwNum], it->second);
            dwNum++;
        }
        dwIndex++;
    }
}

VOID CShade::GetRewardInfoMsg(DT_REWARD_SINGLE_INFO& stRewardInfo, SHuntingReward stHuntingReward)
{
    stRewardInfo.byGoodsType = stHuntingReward.dwRewardType;
    stRewardInfo.dwRewardValue = stHuntingReward.dwRewardValue;
    stRewardInfo.byOutProbability = stHuntingReward.dwOutProbability;
    if ( ESM_YES == m_stShadeData.stHuntingInfo.byGoldModeFlag && stRewardInfo.byGoodsType != ACTE_StudyClip)
    {
        stRewardInfo.byMulti = ESM_YES;
    }
    else
    {
        stRewardInfo.byMulti = ESM_NO;
    }
}

VOID CShade::GetHuntingOtherInfo(DT_HUNTING_OTHER_INFO& stOtherInfo)
{
    CFaction* poFaction = GetFaction();
    if (NULL == poFaction)
    {
        return ;
    }

    DT_FACTION_COMMON_DATA& stFactionTaskScheduleData = poFaction->GetDT_FACTION_COMMON_DATA();
    if (stFactionTaskScheduleData.stHuntingTaskSchedule.dwMaxHuntingNum < m_stShadeData.stHuntingInfo.dwHuntionNum)
    {
        stOtherInfo.byCanHuntingNum = 0;
    }
    else
    {
        stOtherInfo.byCanHuntingNum = stFactionTaskScheduleData.stHuntingTaskSchedule.dwMaxHuntingNum - m_stShadeData.stHuntingInfo.dwHuntionNum;
    }

    DT_FACTION_PLAYER_CLI stFactionPlayer = {0};
    poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayer, m_dwPlayerID);
    //门贡
    stOtherInfo.dwDoorsTribute = stFactionPlayer.dwDoorsTributeValues;
    //元宝
    stOtherInfo.qwGold = m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold;

    SShadeTaskInfoProp stShadeTaskInfo = {0};
    CFactorPropMgr::Instance()->GetTaskInfoProp(EFTT_HUNTING, stShadeTaskInfo);

    if(  SGDP::GetDayPassSecond() < stShadeTaskInfo.dwStartTime || (stShadeTaskInfo.dwStartTime + stShadeTaskInfo.wKeepTime) < SGDP::GetDayPassSecond() )
    {
        stOtherInfo.dwHuntingTime = 0;
    }
    else
    {
        stOtherInfo.dwHuntingTime = stShadeTaskInfo.wKeepTime - (SGDP::GetDayPassSecond() -  stShadeTaskInfo.dwStartTime);
    }
    UINT8 byChangeNum = m_stShadeData.stHuntingInfo.byHuntingChangeNum;

    //已更换次数

    GetFreeMsg(stOtherInfo.stChangeMonsterMsg);
    stOtherInfo.stFindMonsterMsg.byPlayerVIPLevel = m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel;
    stOtherInfo.stFindMonsterMsg.byNeedVIPLevel = static_cast<UINT8>(CVipPropMgr::Instance()->GetOpenLevel(EVF_FINDMONSTER));
    stOtherInfo.stFindMonsterMsg.dwNeedGold = CLuamgr::Instance()->GetFindMonsterGold();
    stOtherInfo.dwMaxTaskIntegral = stFactionTaskScheduleData.stHuntingTaskSchedule.dwMaxIntegral;
    stOtherInfo.dwCurTaskIntegral = stFactionTaskScheduleData.stHuntingTaskSchedule.dwCurIntegral;
    stOtherInfo.byCurPotNum = stFactionTaskScheduleData.stHuntingTaskSchedule.byCurCollectiveTaskSchedule;
    stOtherInfo.bySumPotNum = stFactionTaskScheduleData.stHuntingTaskSchedule.byCollectiveTaskSchedule;
    TCHAR* aszPotName = CShadePropMgr::Instance()->GetHuntingPotName(stFactionTaskScheduleData.stHuntingTaskSchedule.byCurCollectiveTaskSchedule);
    if ( NULL != aszPotName )
    {
        SDStrncpy( stOtherInfo.aszCurHuntingPotName, aszPotName, HUNTING_POT_NAME - 1);
    }

}

//怪物信息
BOOL CShade::GetMonsterInfo( DT_MONSTER_INFO& stMonsterInfo)
{
    if (0 == m_stShadeData.stHuntingInfo.byMonsterNum)
    {
        return FALSE;
    }
    UINT8 byMonsterType = m_stShadeData.stHuntingInfo.byCurMonsterType;
    stMonsterInfo.wMonsterID = m_stShadeData.stHuntingInfo.awMonsterGroup[0];
    UINT16 wPlayerLevel = m_poOwner->GetLevel();
    //怪物战力
    const SMonsterProp* poShadeMonster = CShadeMonsterMgr::Instance()->GetShadeMonsterPropByLevelType(wPlayerLevel, byMonsterType);
    if (NULL == poShadeMonster)
    {
        return FALSE;
    }

    stMonsterInfo.dwMonsterPower = CShadeMonsterMgr::Instance()->GetPowerByProp( poShadeMonster) * 3;
    //奖励信息
    SHuntingRewardMultiMap& mapHuntingReward = CShadePropMgr::Instance()->GetHuntingReward();
    stMonsterInfo.stMonsterReward.byRewardType = byMonsterType;
    //品质奖励个数
    stMonsterInfo.stMonsterReward.byRewardNum = mapHuntingReward.count(byMonsterType);
    //品质信息
    PairHuntingReward itPair = mapHuntingReward.equal_range(byMonsterType);
    UINT32 dwNum = 0;
    for (SHuntingRewardMultiItr it = itPair.first; it != itPair.second && dwNum < HUNTING_REWARD_MGS_NUM ; it++)
    {
        GetRewardInfoMsg(stMonsterInfo.stMonsterReward.astRewardMsg[dwNum], it->second);
        dwNum++;
    }
    return TRUE;
}

VOID CShade::AddRewardByShade(UINT32 dwRewardType, UINT32 dwRewardValue, BOOL bIsSchedule)
{
    CFaction* poFaction = GetFaction();
    UINT32 dwFactionID = GetFactionID();
    if ( NULL == poFaction)
    {
        return ;
    }
    switch (dwRewardType)
    {
    case ACTE_GOOD:
    {
        UINT16 wPileNum = 1;//物品个数
        ECreateItemRet Ret; //结果
        CItemMgr::Instance()->CreateItemIntoBag(m_poOwner, dwRewardValue, wPileNum, Ret, 0, CRecordMgr::EAIT_ENCOURAGE);
    }
    break;
    case ACTE_DoorsTribute:
    {
        if (1 == m_stShadeData.stHuntingInfo.byGoldModeFlag)
        {
            UINT32 dwMulti = CLuamgr::Instance()->GetGoldModeMulti();
            dwRewardValue *= dwMulti;
        }
        if (!bIsSchedule)
        {
            poFaction->AddDoorsTribute(m_dwPlayerID, dwRewardValue, CRecordMgr::EAT_HUNTING);
        }
        else
        {
            poFaction->AddDoorsTributeToAll(dwRewardValue);
        }

    }
    break;
    case ACTE_FactionFunds:
    {
        poFaction->AddFactionFunds(dwRewardValue, CRecordMgr::EAFF_HUNTING);
    }
    break;
    case ACTE_FactionTaskIntegral:
    {
        if (1 == m_stShadeData.stHuntingInfo.byGoldModeFlag)
        {
            UINT32 dwMulti = CLuamgr::Instance()->GetGoldModeMulti();
            dwRewardValue *= dwMulti;
        }
        poFaction->AddHuntingIntegral(m_dwPlayerID, dwRewardValue);
    }
    break;
    case ACTE_StudyClip:
    {
        CAwaken* poAwaken = m_poOwner->GetAwaken();
        if ( NULL != poAwaken )
        {
            poAwaken->AddStudyClip(dwRewardValue, CRecordMgr::EASCTUDYTCLIP_HUNTING);
        }
    }
    break;
    default:
        break;
    }
}

BOOL CShade::GetGoodsByRandom(UINT32 dwProbability)
{
    if (100 < dwProbability )
    {
        dwProbability = 100;
    }
    UINT32 dwRandomNum = GetRdNum() % 100;
    if (dwRandomNum < dwProbability)
    {
        return TRUE;
    }
    return FALSE;
}

UINT16 CShade::GetHuntingIntegral(PKT_CLIGS_HUNTING_ITEGRAL_ACK& stAck)
{
    CFaction* poFaction = GetFaction();
    if (NULL == poFaction)
    {
        return ERR_HUNTING_ITEGRAL::ID_SHADE_NOT_EXIST;
    }
    DT_FACTION_HUNTING_SCHEDULE_DATA& stData = poFaction->GetDT_FACTION_COMMON_DATA().stHuntingTaskSchedule;
	stAck.stNewHuntingScheduleInfo.byLastSchedule = stData.byCurCollectiveTaskSchedule;
    GetScheduleInfo(stAck.stScheduleInfo, stAck.stNewHuntingScheduleInfo);
    if( stData.qwLastChangeScheduleTime < m_stShadeData.stHuntingInfo.qwLastSyncIntegral)
    {
        stAck.byIsChangeFlag = ESM_NO;
    }
    else
    {
        if (1 != stData.byCurCollectiveTaskSchedule )
        {
            stAck.byIsChangeFlag = ESM_YES;
        }
        m_stShadeData.stHuntingInfo.qwLastSyncIntegral = SGDP::SDTimeMicroSec();
    }
    return ERR_HUNTING_ITEGRAL::ID_SUCCESS;
}

BOOL CShade::IsOpen()
{
    CFaction* poFaction = GetFaction();
    if ( NULL == poFaction)
    {
        return FALSE;
    }

    if ( 0 == poFaction->GetDT_FACTION_BASE_DATA().dwShadeLevel)
    {
        return FALSE;
    }

    return TRUE;
}

VOID CShade::SendRewardSchedule()
{
    CFaction* poFaction = GetFaction();
    UINT32 dwFactionID = GetFactionID();
    if (NULL == poFaction)
    {
        return ;
    }
    DT_FACTION_COMMON_DATA& stFactionTaskScheduleData = poFaction->GetDT_FACTION_COMMON_DATA();

    //发放奖励
    STaskRewardProp stTaskRewardProp =  {0};
    UINT32 dwShadeLevel = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Shade);
    CFactorPropMgr::Instance()->GetTaskRewardProp( dwShadeLevel, stFactionTaskScheduleData.stHuntingTaskSchedule.byCurCollectiveTaskSchedule, stTaskRewardProp);

    UINT8 byGoodsType = stTaskRewardProp.byRewardType;
    UINT32 dwRewardValue = stTaskRewardProp.dwReardValue;
    AddRewardByShade(byGoodsType, dwRewardValue, TRUE);

    //日志
    DT_FACTION_LOG_DATA stFactionLogData;
    stFactionLogData.byAction = EFLID_ChangeSchedule;
    stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
    stFactionLogData.adwParamList[0] = stFactionTaskScheduleData.stHuntingTaskSchedule.byCurCollectiveTaskSchedule;
    stFactionLogData.adwParamList[1] = dwRewardValue;
    poFaction->AddFactionLogData(stFactionLogData);
}

VOID CShade::GetPrompt(DT_FUNC_COMMON_PROMPT& stPrompt)
{
    CFaction* poFaction = GetFaction();
    if (NULL == poFaction)
    {
        return ;
    }
    memset(&stPrompt, 0, sizeof(stPrompt));
    stPrompt.byBuildKindID = EFB_Shade;

    BOOL bRet = CShadePropMgr::Instance()->GetOpenFlag();
    if (bRet)
    {
        UINT32 dwMaxHunting = poFaction->GetDT_FACTION_COMMON_DATA().stHuntingTaskSchedule.dwMaxHuntingNum;
        if ( m_stShadeData.stHuntingInfo.dwHuntionNum < dwMaxHunting)
        {
            stPrompt.byFuncFlag = EFPID_CanHunting;
        }
    }
}

VOID CShade::GetScheduleInfo(DT_HUNTING_SCHEDULE_INFO& stInfo, DT_HUNTING_SCHEDULE_INFO2& stNewInfo)
{
    memset(&stInfo, 0, sizeof(DT_HUNTING_SCHEDULE_INFO));
	memset(&stNewInfo, 0, sizeof(DT_HUNTING_SCHEDULE_INFO2));
    CFaction* poFaction = GetFaction();
    UINT32 dwFactionID = GetFactionID();
    if (NULL == poFaction)
    {
        return ;
    }
    DT_FACTION_HUNTING_SCHEDULE_DATA& stData = poFaction->GetDT_FACTION_COMMON_DATA().stHuntingTaskSchedule;
    const STaskRewardPropMap& mapReward = CFactorPropMgr::Instance()->GetTaskRewardMap();
    pair<STaskRewardPropMap::const_iterator, STaskRewardPropMap::const_iterator> itr;
    UINT32 dwShadeLevel = CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Shade);
    itr = mapReward.equal_range(dwShadeLevel);

	UINT32 dwMaxPot = CShadePropMgr::Instance()->GetHuntingMaxSchedule(dwShadeLevel);
	if ( dwMaxPot < stData.byCurCollectiveTaskSchedule)
	{
		stData.byCurCollectiveTaskSchedule = dwMaxPot;
	}
	if ( dwMaxPot < stData.byCollectiveTaskSchedule)
	{
		stData.byCollectiveTaskSchedule = dwMaxPot;
	}
    //旧版
	stInfo.byHuntingPotPos = stData.byCurCollectiveTaskSchedule;
    stInfo.byHuntingPotNum = stData.byCollectiveTaskSchedule;
	stInfo.dwFactionFunds = poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds;

	//新版
	stNewInfo.byHuntingPotPos = stData.byCurCollectiveTaskSchedule;
	stNewInfo.byHuntingPotNum = stData.byCollectiveTaskSchedule;
	stNewInfo.dwFactionFunds = poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds;

    UINT8 byIndex = 0;
    for ( STaskRewardPropMap::const_iterator oneItr = itr.first; oneItr != itr.second; oneItr++)
    {
        TCHAR* aszName = CShadePropMgr::Instance()->GetHuntingPotName(byIndex + 1);
        if ( NULL != aszName)
        {
            SDStrncpy( stInfo.astHuntingPotInfo[byIndex].aszPotName, aszName, HUNTING_POT_NAME - 1);
			SDStrncpy( stNewInfo.astHuntingPotInfo[byIndex].aszPotName, aszName, HUNTING_POT_NAME - 1);
        }
        stInfo.astHuntingPotInfo[byIndex].dwMaxintegral = oneItr->second.dwMaxIntegral;
        if (oneItr->second.byCollectiveTaskSchedule < stData.byCurCollectiveTaskSchedule)
        {
            stInfo.astHuntingPotInfo[byIndex].dwCurIntegral = oneItr->second.dwMaxIntegral;
        }
        else if ( oneItr->second.byCollectiveTaskSchedule == stData.byCurCollectiveTaskSchedule)
        {
            stInfo.astHuntingPotInfo[byIndex].dwCurIntegral = stData.dwCurIntegral;
        }
        else
        {
            stInfo.astHuntingPotInfo[byIndex].dwCurIntegral = 0;
        }
        stInfo.astHuntingPotInfo[byIndex].dwRewardFunds = oneItr->second.dwReardValue;
		///////////////////////////////////新版///////////////////////////////////////
		stNewInfo.astHuntingPotInfo[byIndex].dwMaxintegral = oneItr->second.dwMaxIntegral;
		if (oneItr->second.byCollectiveTaskSchedule < stData.byCurCollectiveTaskSchedule)
		{
			stNewInfo.astHuntingPotInfo[byIndex].dwCurIntegral = oneItr->second.dwMaxIntegral;
		}
		else if ( oneItr->second.byCollectiveTaskSchedule == stData.byCurCollectiveTaskSchedule)
		{
			stNewInfo.astHuntingPotInfo[byIndex].dwCurIntegral = stData.dwCurIntegral;
		}
		else
		{
			stNewInfo.astHuntingPotInfo[byIndex].dwCurIntegral = 0;
		}
		stNewInfo.astHuntingPotInfo[byIndex].dwRewardFunds = oneItr->second.dwReardValue;
		stNewInfo.astHuntingPotInfo[byIndex].byIsUnlock = ESM_YES;
		stNewInfo.astHuntingPotInfo[byIndex].byUnLockLevel = 0;
        byIndex++;
    }
    

}

VOID CShade::GetDT_ACTIVITY_BASE_DATA(DT_ACTIVITY_BASE_DATA& stDT_ACTIVITY_BASE_DATA)
{
    memset(&stDT_ACTIVITY_BASE_DATA, 0, sizeof(DT_ACTIVITY_BASE_DATA));
    stDT_ACTIVITY_BASE_DATA.byActivityTimeNum = 1;
    stDT_ACTIVITY_BASE_DATA.byKindID = EAK_HUNTING;
    SShadeTaskInfoProp stProp;
    CFactorPropMgr::Instance()->GetTaskInfoProp(EFTT_HUNTING, stProp);
    stDT_ACTIVITY_BASE_DATA.byNotity = 1;//没用
    stDT_ACTIVITY_BASE_DATA.astActivityTimeInfo[0].byStartHour = stProp.byStartHour;
    stDT_ACTIVITY_BASE_DATA.astActivityTimeInfo[0].byStartMin = stProp.byStartMin;
    stDT_ACTIVITY_BASE_DATA.astActivityTimeInfo[0].wAheadTime = 0;
    stDT_ACTIVITY_BASE_DATA.astActivityTimeInfo[0].wKeepTime = stProp.wKeepTime;
    UINT32 dwPassTime = SGDP::GetDayPassSecond();
    if ( dwPassTime < stProp.dwStartTime)
    {
        stDT_ACTIVITY_BASE_DATA.astActivityTimeInfo[0].byHaveFinishFlag = ESM_NO;
    }
    else if (dwPassTime < stProp.dwEndTime)
    {
        stDT_ACTIVITY_BASE_DATA.astActivityTimeInfo[0].byHaveFinishFlag = ESM_NO;
        stDT_ACTIVITY_BASE_DATA.astActivityTimeInfo[0].wEndCountDown = stProp.dwEndTime - dwPassTime;
    }
    else
    {
        stDT_ACTIVITY_BASE_DATA.astActivityTimeInfo[0].byHaveFinishFlag = ESM_YES;
    }
}

VOID CShade::GetFreeMsg(DT_CHANGE_MONSTER_INFO& stInfo)
{
    UINT8 byMaxFreeNum = CLuamgr::Instance()->GetFreeRefreshNum();
    stInfo.byMaxFreeNum = byMaxFreeNum;
    UINT8 byChangeNum = m_stShadeData.stHuntingInfo.byHuntingChangeNum;
    if (stInfo.byMaxFreeNum < byChangeNum)
    {
        stInfo.byCurFreeNum = 0;
    }
    else
    {
        stInfo.byCurFreeNum = stInfo.byMaxFreeNum - byChangeNum;
    }
    stInfo.dwChangeMonsterGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_SHADE_CHANGE_MONSTER, 0, byChangeNum - byMaxFreeNum + 1);
}