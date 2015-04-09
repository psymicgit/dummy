#include "player.h"
#include <logic/plunder/plundergridpropmgr.h>
#include <common/client/errdef.h>
#include <logic/player/playermgr.h>
#include <logic/player/playersavemgr.h>
#include <logic/other/rdchallengepropmgr.h>
#include <logic/race/racemgr.h>
#include <logic/plunder/plundermgr.h>
#include <logic/activity/bossbattle.h>
#include <logic/other/rdchallenge.h>
#include <logic/other/worship.h>
#include "logic/hero/hero.h"
#include "logic/yabiao/yabiaoinfo.h"
#include "logic/player/playerslogmgr.h"
#include "logic/medal/playermedal.h"
IMPLEMENT_BUILD_INIT_DESTORY(Worship, CWorship, DT_WORSHIP_DATA_LST)
IMPLEMENT_BUILD_INIT_DESTORY(RdChallenge, CRdChallenge, DT_RANDOM_CHALLENGE_DATA)
IMPLEMENT_BUILD_INIT_DESTORY_EX(Race, CRace, DT_RACE_DATA, stRaceBaseData)
IMPLEMENT_BUILD_INIT_DESTORY_EX(Plunder, CPlunder, DT_PLUNDER_DATA, stPlunderBaseData)

VOID CPlayer::GetBattleEncourage(DT_BATTLE_ENCOURAGE_DATA& stEncourageInfo, DT_DATA_AFTER_BATTLE_ENCOURAGE* pstAfterBattleEncourage)
{
    stEncourageInfo.dwCoin = m_stBattleEncourage.dwCoin;
    stEncourageInfo.dwExperience = m_stBattleEncourage.dwExperience;
    stEncourageInfo.dwStory = m_stBattleEncourage.dwStory;
    stEncourageInfo.dwJingJie = m_stBattleEncourage.dwJingJie;

    if(pstAfterBattleEncourage)
    {
        pstAfterBattleEncourage->qwCurCoin = m_stDT_PLAYER_BASE_DATA.qwCoin;
        pstAfterBattleEncourage->qwCurStory = m_stDT_PLAYER_BASE_DATA.qwStory;
        GetHeroUpgradeInfo(pstAfterBattleEncourage->byHeroNum, pstAfterBattleEncourage->astHeroUpgradeInfo);
        pstAfterBattleEncourage->qwCurJingJie = m_stDT_PLAYER_BASE_DATA.qwJingJie;
    }

    m_stBattleEncourage.dwCoin = 0;
    m_stBattleEncourage.dwExperience = 0;
    m_stBattleEncourage.dwStory = 0;
    m_stBattleEncourage.dwJingJie = 0;
}

VOID CPlayer::InitJewelryAttr()
{
	m_mapJewelryAttr.clear();
	CHero* poHero = m_mapFormationHero.GetFistData();
	while(poHero)
	{
		CJewelryMap& mapJewelry = poHero->GetJewelryMap();

		CJewelry* poJewelry = mapJewelry.GetFistData();
		while ( poJewelry){
			UINT8 byExtNum = poJewelry->GetDT_JEWELRY_DATA().stExtAttrLst.byExtAttrNum;
			for ( UINT8 byIdx = 0; byIdx < byExtNum; byIdx++){
				DT_BATTLE_ATTR_EXT_INFO& stInfo = poJewelry->GetDT_JEWELRY_DATA().stExtAttrLst.astExtAttrInfo[byIdx];
                if (CJewelry::IsAllHeroAttr((JewelryExtAttr)stInfo.byKindID)){
                    m_mapJewelryAttr[stInfo.byKindID] += stInfo.dwKindValue;
                }
			}

			poJewelry = mapJewelry.GetNextData();
		}

        poHero->GetAllHeroAddAttr(m_mapJewelryAttr);
		poHero = m_mapFormationHero.GetNextData();
	}
}

//初始化战斗属性
VOID CPlayer::InitBattleAttr(BOOL bResetHero)
{
	//重新计算饰品对战力的影响
	InitJewelryAttr();

    //计算属性
    //武将属性(武将中有装备)
    memset(&m_stMaxBattleAttribute, 0, sizeof(m_stMaxBattleAttribute));
    memset(&m_stCurBattleAttribute, 0, sizeof(m_stCurBattleAttribute));
    memset(&m_stHideBattleAttr, 0, sizeof(m_stHideBattleAttr));
    memset(&m_stExtBattleAttr, 0, sizeof(m_stExtBattleAttr));
    m_stDT_PLAYER_BASE_DATA.dwPower = 0;

    CHero* poHero = m_mapFormationHero.GetFistData();
    while(poHero)
    {
        if(bResetHero)
        {
            poHero->InitBattleAttribute();
        }
        DT_BATTLE_BASE_ATTRIBUTE& stDT_BATTLE_ATTRIBUTE = poHero->GetDT_BATTLE_ATTRIBUTE();
        m_stMaxBattleAttribute.qwHP += stDT_BATTLE_ATTRIBUTE.qwHP;
        m_stMaxBattleAttribute.dwAttack += stDT_BATTLE_ATTRIBUTE.dwAttack;
		m_stMaxBattleAttribute.dwDefend += stDT_BATTLE_ATTRIBUTE.dwDefend;

        //其他功能(器魂等)附加的隐藏属性
        DT_HIDE_BATTLE_ATTRIBUTE& stDT_HIDE_BATTLE_ATTRIBUTE = poHero->GetDT_HIDE_BATTLE_ATTRIBUTE();
        m_stHideBattleAttr.wHitRate += stDT_HIDE_BATTLE_ATTRIBUTE.wHitRate;
        m_stHideBattleAttr.wDodgeRate += stDT_HIDE_BATTLE_ATTRIBUTE.wDodgeRate;
        m_stHideBattleAttr.wCritRate += stDT_HIDE_BATTLE_ATTRIBUTE.wCritRate;
        m_stHideBattleAttr.wDeCritRate += stDT_HIDE_BATTLE_ATTRIBUTE.wDeCritRate;
        m_stHideBattleAttr.wAngryValue += stDT_HIDE_BATTLE_ATTRIBUTE.wAngryValue;

        //其他功能(器魂等)附加的扩展属性k
        DT_EXT_BATTLE_ATTRIBUTE& stDT_EXT_BATTLE_ATTRIBUTE = poHero->GetDT_EXT_BATTLE_ATTRIBUTE();
        m_stExtBattleAttr.wIncHurtRate += stDT_EXT_BATTLE_ATTRIBUTE.wIncHurtRate;
        m_stExtBattleAttr.wDecHurtRate += stDT_EXT_BATTLE_ATTRIBUTE.wDecHurtRate;
        m_stExtBattleAttr.dwFirstAttack += stDT_EXT_BATTLE_ATTRIBUTE.dwFirstAttack;

        m_stDT_PLAYER_BASE_DATA.dwPower += poHero->GetPower();

        poHero = m_mapFormationHero.GetNextData();
    }

    if(m_stDT_PLAYER_BASE_DATA.dwPower)
    {
        //记录战力
        // DBG_INFO(_SDT("[%s: %d]: InitBattleAttr PlayerID:%u, Power:%u!"), MSG_MARK, m_dwPlayerID, m_stDT_PLAYER_BASE_DATA.dwPower);
    }
	CPlayerMedal* poPlayerMedal = GetPlayerMedal();
	if ( NULL != poPlayerMedal)
	{
		poPlayerMedal->CheckMedalBySmallType( EMST_PLAYER_POWER );
	}
    CPlayerBaseDataCacheMgr::Instance()->OnPowerChange(m_dwPlayerID, m_stDT_PLAYER_BASE_DATA.dwPower);
    m_stDT_PLAYER_BASE_DATA.dwFirstAttack = m_stExtBattleAttr.dwFirstAttack;
    CPlayerBaseDataCacheMgr::Instance()->OnFirstAttackChange(m_dwPlayerID, m_stDT_PLAYER_BASE_DATA.dwFirstAttack);
}

//重置战斗属性
VOID CPlayer::ResetBattleAttr(BOOL bResetHero)
{
    InitBattleAttr(bResetHero);

    m_stCurBattleAttribute = m_stMaxBattleAttribute;
}

/*
//重置换算战斗属性(当前值按百分比换算)，用于战斗过程中强化/布阵
VOID CPlayer::ResetConvertBattleAttr()
{
	InitBattleAttr();

	//血量换算
	DT_STATE_DATA& stDT_STATE_DATA = m_oState.GetDT_STATE_DATA();
	m_stCurBattleAttribute.dwHP = (UINT32)(((FLOAT)stDT_STATE_DATA.dwInstanceCurHP / (FLOAT)stDT_STATE_DATA.dwInstanceMAXHP) * (FLOAT)m_stMaxBattleAttribute.dwHP);
	if(m_stCurBattleAttribute.dwHP > m_stMaxBattleAttribute.dwHP)
	{
		m_stCurBattleAttribute.dwHP = m_stMaxBattleAttribute.dwHP;
	}
}
*/

//UINT32 CPlayer::GetRank()
//{
//    return (NULL != m_poRace) ? m_poRace->GetDT_RACE_BASE_DATA().dwRank : 0;
//}
//
//
//CRace* CPlayer::AddRace(DT_RACE_BASE_DATA& stDT_RACE_BASE_DATA)
//{
//    CBuild* poBuild = GetBuild(EBK_RACEBUILD);
//    if(NULL == poBuild)
//    {
//        SYS_CRITICAL(_SDT("[%s: %d]: poBuild is null!"), MSG_MARK);
//        return NULL;
//    }
//
//    CRace* poRace = CRaceMgr::Instance()->CreateRace(m_dwPlayerID);
//    if(NULL == poRace)
//    {
//        return NULL;
//    }
//
//    DT_RACE_BATTLELOG_DATA_LIST stRaceBattleLog;
//    memset(&stRaceBattleLog, 0, sizeof(stRaceBattleLog));
//    if(!poRace->Init(stDT_RACE_BASE_DATA, this))
//    {
//        SYS_CRITICAL(_SDT("[%s: %d]: race init failed!"), MSG_MARK);
//        CRaceMgr::Instance()->RemoveRace(m_dwPlayerID);
//        return NULL;
//    }
//
//    //poRace->SetObjDataSaveType(EODST_INSERT);
//    //poRace->SetSaveState(ESS_WANT_SAVE);
//
//    m_poRace = poRace;
//
//    CPlayerSaveMgr::Instance()->AddPlayerRaceSaveData(this);
//
//    return poRace;
//}



//初始化竞技
//BOOL CPlayer::InitRace(DT_RACE_DATA& stDT_RACE_DATA)
//{
//    //还没有加入竞技场，跳过
//    if(0 == stDT_RACE_DATA.stRaceBaseData.dwRank)
//    {
//        return TRUE;
//    }
//
//    //playerpvp已经有竞技场，但player未保存
//    if(NULL == GetBuild(EBK_RACEBUILD))
//    {
//        if(NULL == NewBuild(EBK_RACEBUILD))
//        {
//            return FALSE;
//        }
//    }
//
//    CRace* poRace = CRaceMgr::Instance()->CreateRace(m_dwPlayerID);
//    if(NULL == poRace)
//    {
//        return FALSE;
//    }
//    if(!poRace->Init(stDT_RACE_DATA, this))
//    {
//        SYS_CRITICAL(_SDT("[%s: %d]: race init failed!"), MSG_MARK);
//        CRaceMgr::Instance()->RemoveRace(m_dwPlayerID);
//        return FALSE;
//    }
//
//    m_poRace = poRace;
//
//    return TRUE;
//}

//BOOL CPlayer::InitPlunder(DT_PLUNDER_BASE_DATA stDT_PLUNDER_BASE_DATA, DT_PLUNDER_BATTLELOG_DATA_LIST stPlunderBattleLog, DT_CAPTURE_DATA_LIST stCaptureData)
//{
//    //还没有解锁，跳过
//    if(0 == stDT_PLUNDER_BASE_DATA.byHaveDataFlag)
//    {
//        return TRUE;
//    }
//
//    CPlunder* poPlunder = CPlunderMgr::Instance()->CreatePlunder(m_dwPlayerID);
//    if(NULL == poPlunder)
//    {
//        return NULL;
//    }
//
//    //playerpvp已经有掠夺，但player未保存
//    if(NULL == GetBuild(EBK_PLUNDER))
//    {
//        if(NULL == NewBuild(EBK_PLUNDER))
//        {
//            return FALSE;
//        }
//    }
//
//    if(!poPlunder->Init(stDT_PLUNDER_BASE_DATA, stPlunderBattleLog, stCaptureData, this))
//    {
//        SYS_CRITICAL(_SDT("[%s: %d]: plunder init failed!"), MSG_MARK);
//        CPlunderMgr::Instance()->RemovePlunder(m_dwPlayerID);
//        return FALSE;
//    }
//
//    m_poPlunder = poPlunder;
//
//    return TRUE;
//}


BOOL CPlayer::GetDT_ENEMY_HERO_DATA(UINT8& byEnemyHeroNum, DT_ENEMY_HERO_DATA astEnemyInfoList[])
{
    CHero* poHero = m_mapFormationHero.GetFistData();
    while(poHero)
    {
        DT_ENEMY_HERO_DATA& stDT_ENEMY_HERO_DATA = astEnemyInfoList[byEnemyHeroNum++];
        DT_HERO_BASE_DATA& stDT_HERO_BASE_DATA = poHero->GetDT_HERO_BASE_DATA();
        stDT_ENEMY_HERO_DATA.byCoach = stDT_HERO_BASE_DATA.byCoach;
        stDT_ENEMY_HERO_DATA.wKindID = stDT_HERO_BASE_DATA.wKindID;
        stDT_ENEMY_HERO_DATA.wLevel = stDT_HERO_BASE_DATA.wLevel;
        stDT_ENEMY_HERO_DATA.wTalent = stDT_HERO_BASE_DATA.wTalent;
        stDT_ENEMY_HERO_DATA.stBattleAttribute = stDT_HERO_BASE_DATA.stBattleAttribute;
        poHero->GetEnemyEquip(stDT_ENEMY_HERO_DATA.stEquipList);

        poHero = m_mapFormationHero.GetNextData();
    }

    return TRUE;
}


//CPlunder* CPlayer::NewPlunder()
//{
//    //初始化掠夺信息
//    DT_PLUNDER_BASE_DATA stDT_PLUNDER_BASE_DATA;
//    memset(&stDT_PLUNDER_BASE_DATA, 0, sizeof(stDT_PLUNDER_BASE_DATA));
//    stDT_PLUNDER_BASE_DATA.byHaveDataFlag = 1;
//    DT_PLUNDER_BATTLELOG_DATA_LIST stPlunderBattleLog;
//    stPlunderBattleLog.byBattleLogNum = 0;
//    DT_CAPTURE_DATA_LIST stCaptureData;
//    stCaptureData.byCaptureNum = 0;
//    if(!InitPlunder(stDT_PLUNDER_BASE_DATA, stPlunderBattleLog, stCaptureData))
//    {
//        SYS_CRITICAL(_SDT("[%s: %d]: InitPlunder failed!"), MSG_MARK);
//        return NULL;
//    }
//    CPlayerSaveMgr::Instance()->AddPlayerPlunderSaveData(this);
//
//    CPlayerBaseDataCacheMgr::Instance()->OnOpenPlunder(m_dwPlayerID);
//
//    return m_poPlunder;
//}


VOID CPlayer::GetUnreadBattleLogNum(UINT8& byBattleLogTypeNum, DT_UNREAD_BATTLE_LOG_DATA_CLI astUnreadBattleLogInfo[MAX_BATTLE_LOG_TYPE_NUM])
{
    byBattleLogTypeNum = 0;
    if(m_poRace)
    {
        DT_UNREAD_BATTLE_LOG_DATA_CLI& stDT_UNREAD_BATTLE_LOG_DATA_CLI = astUnreadBattleLogInfo[byBattleLogTypeNum++];
        stDT_UNREAD_BATTLE_LOG_DATA_CLI.byBattleLogType = EBLT_RACE;
        stDT_UNREAD_BATTLE_LOG_DATA_CLI.byUnreadNum = m_poRace->GetDT_RACE_BASE_DATA().byUnreadBattleLogNum;
    }
    if(m_poPlunder)
    {
        DT_UNREAD_BATTLE_LOG_DATA_CLI& stDT_UNREAD_BATTLE_LOG_DATA_CLI = astUnreadBattleLogInfo[byBattleLogTypeNum++];
        stDT_UNREAD_BATTLE_LOG_DATA_CLI.byBattleLogType = EBLT_PLUNDER;
        stDT_UNREAD_BATTLE_LOG_DATA_CLI.byUnreadNum = m_poPlunder->GetDT_PLUNDER_BASE_DATA().byUnreadBattleLogNum;
    }
    DT_UNREAD_BATTLE_LOG_DATA_CLI& stDT_UNREAD_BATTLE_LOG_DATA_CLI = astUnreadBattleLogInfo[byBattleLogTypeNum++];
    stDT_UNREAD_BATTLE_LOG_DATA_CLI.byBattleLogType = EBLT_YABIAO;

    CYabiaoInfo* poYabiaoInfo = GetYabiaoInfo();
    if (poYabiaoInfo)
    {
        stDT_UNREAD_BATTLE_LOG_DATA_CLI.byUnreadNum = poYabiaoInfo->GetUnreadBattleLogNum();
    }

    if(m_poBossBattle)
    {
        DT_UNREAD_BATTLE_LOG_DATA_CLI& stDT_UNREAD_BATTLE_LOG_DATA_CLI = astUnreadBattleLogInfo[byBattleLogTypeNum++];
        stDT_UNREAD_BATTLE_LOG_DATA_CLI.byBattleLogType = EBLT_BOSSB;
        stDT_UNREAD_BATTLE_LOG_DATA_CLI.byUnreadNum = m_poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA().byUnreadBattleLogNum;
    }

	if (m_poFactionPlayer)
	{
		DT_UNREAD_BATTLE_LOG_DATA_CLI& stDT_UNREAD_BATTLE_LOG_DATA_CLI = astUnreadBattleLogInfo[byBattleLogTypeNum++];
		stDT_UNREAD_BATTLE_LOG_DATA_CLI.byBattleLogType = EBLT_FACTION;
		stDT_UNREAD_BATTLE_LOG_DATA_CLI.byUnreadNum = m_poFactionPlayer->GetUnReadFactionBattleLogNum();
	}
    
    if (GetCliVer() >= CPlayerLogMgr::Instance()->GetCliVersion())
    {
        PKT_CLIGS_UNREAD_BATTLE_NTF stNtf;
        CPlayerLogMgr::Instance()->GetUnReadLogNum( m_dwPlayerID, stNtf);

        for ( UINT8 byIdx = 0; byIdx < stNtf.byLogNum; byIdx++)
        {
            DT_UNREAD_BATTLE_LOG_DATA_CLI& stDT_UNREAD_BATTLE_LOG_DATA_CLI = astUnreadBattleLogInfo[byBattleLogTypeNum++];
            stDT_UNREAD_BATTLE_LOG_DATA_CLI.byBattleLogType = stNtf.astLogInfo[byIdx].byLogType;
            stDT_UNREAD_BATTLE_LOG_DATA_CLI.byUnreadNum = stNtf.astLogInfo[byIdx].byUnReadNum;
        }
    }
}

UINT32 CPlayer::GetRank()
{
    CRace* poRace = GetRace();
    if(NULL == poRace)
    {
        return 0;
    }

    return poRace->GetDT_RACE_BASE_DATA().dwRank;
}

UINT32 CPlayer::GetJewelryAttr(UINT32 dwKey)
{
	return m_mapJewelryAttr[dwKey];
}