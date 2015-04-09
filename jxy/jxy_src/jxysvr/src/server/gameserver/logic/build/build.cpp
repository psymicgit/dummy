#include "build.h"
#include <logic/player/player.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/base/logicfunc.h>
#include <logic/itemexchange/itemexchangemgr.h>
#include <logic/activityencourage/activityencourage.h>
#include <logic/race/racemgr.h>
#include <dll/sdframework/sdloggerimpl.h>
#include <logic/hero/hero.h>
#include "logic/hero/heropropmgr.h"
#include "logic/medal/playermedal.h"
CBuild::CBuild()
{
    memset(&m_stDT_BUILD_BASE_DATA, 0, sizeof(DT_BUILD_BASE_DATA));
}


CBuild::~CBuild()
{

}

BOOL CBuild::Init(DT_BUILD_BASE_DATA* pstDT_BUILD_BASE_DATA, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    memcpy(&m_stDT_BUILD_BASE_DATA, pstDT_BUILD_BASE_DATA, sizeof(DT_BUILD_BASE_DATA));
    m_stDT_BUILD_BASE_DATA.bySort = CBuildPropMgr::Instance()->GetBuildSort(m_stDT_BUILD_BASE_DATA.byKindID);
    m_poOwner = poOwner;
    return TRUE;
}


VOID CBuild::GetDT_BUILD_DATA_CLI(DT_BUILD_DATA_CLI& stDT_BUILD_DATA_CLI)
{
    stDT_BUILD_DATA_CLI.byKindID = m_stDT_BUILD_BASE_DATA.byKindID;
    stDT_BUILD_DATA_CLI.wLevel = m_stDT_BUILD_BASE_DATA.wLevel;
    stDT_BUILD_DATA_CLI.bySort = m_stDT_BUILD_BASE_DATA.bySort;
    stDT_BUILD_DATA_CLI.byHaveOpenFlag = m_stDT_BUILD_BASE_DATA.byHaveOpenFlag;

	//开启过了，记录
	m_stDT_BUILD_BASE_DATA.byHaveOpenFlag = 1;
    //冲值活动，对于第一次打开要闪，对于已经打开过的，有冲值的也闪
    switch(m_stDT_BUILD_BASE_DATA.byKindID)
    {
    case EBK_RECHARGE:
    {
        if (0 == stDT_BUILD_DATA_CLI.byHaveOpenFlag)
        {
            stDT_BUILD_DATA_CLI.byHaveOpenFlag = 1;
        }
        else
        {
            if (m_poOwner->GetDT_PLAYER_BASE_DATA().dwTotalAmount > 0)
            {
                stDT_BUILD_DATA_CLI.byHaveOpenFlag = 1;
            }
            else
            {
                stDT_BUILD_DATA_CLI.byHaveOpenFlag = 0;
            }
        }
    }
    break;
    case EBK_ACTIVITY_ENCOURAGE:
    {
        stDT_BUILD_DATA_CLI.byHaveOpenFlag = m_poOwner->GetActivityEncourage().GetCanActivityEncourageNum();
    }
    break;
    case EBK_ITEMEXCHANGE: //兑换活动
    {
        stDT_BUILD_DATA_CLI.byHaveOpenFlag = CItemExchangeMgr::Instance()->HaveExchangeActivity(m_poOwner) ? 1 : 0;
    }
    break;
    default:
        break;
    }

}


BOOL CBuild::AddFunctionModuleAndGuide()
{
	CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
	if ( NULL != poPlayerMedal)
	{
		poPlayerMedal->AddNewMedalByBuild(m_stDT_BUILD_BASE_DATA.byKindID);
	}
    switch(m_stDT_BUILD_BASE_DATA.byKindID)
    {
    case EBK_MAINTOWN:
        return AddFunctionModuleAndGuide_Maintown();
    case EBK_SCIENCE:
        return AddFunctionModuleAndGuide_ScienceTree();
    case EBK_SMITHY:
        return AddFunctionModuleAndGuide_Smithy();
    case EBK_RECRUIT:
        return AddFunctionModuleAndGuide_Recuit();
    case EBK_PVP:
        return TRUE;
    case EBK_GODWEAPON:
        return AddFunctionModuleAndGuide_Godweapon();
    case EBK_AWAKEN:
        return AddFunctionModuleAndGuide_Awaken();
    case EBK_PLUNDER:
        return AddFunctionModuleAndGuide_Plunder();
    case EBK_CROP:
        return AddFunctionModuleAndGuide_Crop();
    case EBK_ADVENTURE:
        return AddFunctionModuleAndGuide_Adventure();
    case EBK_JINGJIE:
        return AddFunctionModuleAndGuide_JingJie();
    case EBK_FACTION:
        return AddFunctionModuleAndGuide_Faction();
    case EBK_SHOP:
        return TRUE;
    case EBK_ELITEINSTANCE:
        return AddFunctionModuleAndGuide_EliteInstance();
    case EBK_ONHOOK:
        return AddFunctionModuleAndGuide_OnHook();
    case EBK_EQUPUPGRADE:
        return TRUE;
    case EBK_MONEYTREE:
        return AddFunctionModuleAndGuide_MoneyTree();
    case EBK_EXPRIENCE:
        return AddFunctionModuleAndGuide_Experience();
    case EBK_UPDATEATTR:
        return AddFunctionModuleAndGuide_UpdateAttr();
    case EBK_LEVY:
        return AddFunctionModuleAndGuide_Levy();
    case	EBK_FROMATION:
        return AddFunctionModuleAndGuide_Fromation();
    case EBK_MELTING:
        return AddFunctionModuleAndGuide_Melting();
    case EBK_EATBREAD:
        return AddFunctionModuleAndGuide_EatBread();
    case EBK_XIANGQIAN:
        return AddFunctionModuleAndGuide_Xiangqian();
    case EBK_COLLECTGAS:
        return AddFunctionModuleAndGuide_CollectGas();
    case EBK_CLIMBTOWER:
        return AddFunctionModuleAndGuide_ClimbTower();
    case EBK_ACTIVITY:
        return TRUE;
    case EBK_YABIAO:
        return AddFunctionModuleAndGuide_Yabiao();
    case EBK_SKILL:
        return AddFunctionModuleAndGuide_Skill();
    case EBK_BOSSBATTLE:
        return AddFunctionModuleAndGuide_BossB();
    case EBK_RECHARGE:
        return TRUE;
    case EBK_SCORE:
        return TRUE;
    case EBK_EXPTRAN:
        return TRUE;
    case EBK_RDCHALLENGE:
        return AddFunctionModuleAndGuide_RDC();
    case EBK_RACEBUILD:
        return AddFunctionModuleAndGuide_Race();
    case EBK_ACTIVITY_ENCOURAGE:
        return TRUE;
    case EBK_EQUIPCOMPOSE:
        return TRUE;
    case EBK_STUDYEXCHANGE:
        return TRUE;
    case EBK_SCORERATING:
        return TRUE;
    case EBK_BEGBLUE:
        return AddFunctionModuleAndGuide_BegBlue();
    case EBK_OPENLIGHT:
        return AddFunctionModuleAndGuide_OpenLight();
    case EBK_ITEMEXCHANGE:
        return TRUE;
    case EBK_VIEWCHAT:
        return TRUE;
    case EBK_SNDCHAT:
        return TRUE;
    case EBK_FRIENDS:
        return AddFunctionModuleAndGuide_Friends();
    case EBK_COURAGE:
        return AddFunctionModuleAndGuide_Courage();
    case EBK_EQUIP_MASTER:
        return AddFunctionModuleAndGuide_EquipMaster();
	case EBK_GVG:
		return AddFunctionModuleAndGuide_GVG();
	case EBK_SOUL:
		return AddFunctionModuleAndGuide_Soul();
		break;
	case EBK_MEDAL:
		return AddFunctionModuleAndGuide_Medal();
		break;
	case EBK_PET:
		return AddFunctionModuleAndGuide_Pet();
		break;
	case EBK_PETFEED:
		m_poOwner->AddGuideRecord(EGF_PETFEED);
		break;
	case EBK_PETMERGE:	
		m_poOwner->AddGuideRecord(EGF_PETMERGE);
		break;
	case EBK_PETUPSTAR:	
		m_poOwner->AddGuideRecord(EGF_PETUPSTAR);
		break;
	case EBK_PETWASH:		
		m_poOwner->AddGuideRecord(EGF_PETWASH);
		break;
	case EGF_PETLIANYAOHU:		
		m_poOwner->AddGuideRecord(EGF_PETLIANYAOHU);
		break;
    default:
        break;
    }
    return TRUE;
}



BOOL CBuild::AddFunctionModuleAndGuide_Awaken()
{
    if(NULL != m_poOwner->GetAwaken())
    {
        return TRUE;
    }

    DT_AWAKEN_DATA stAwakenData;
    memset(&stAwakenData, 0, sizeof(stAwakenData));
    stAwakenData.stBaseInfo.byLevel = EALK_KESHUI; //默认1级
    stAwakenData.byHaveDataFlag = 1;
    CAwaken* poAwaken = m_poOwner->NewAwaken(stAwakenData);
    if(NULL == poAwaken)
    {
        return FALSE;
    }

    //引导
    m_poOwner->CKGuide( EGF_AWAKEN);

    return TRUE;
}

BOOL CBuild::AddFunctionModuleAndGuide_EliteInstance()
{
    //没有功能模块

    //引导
    m_poOwner->CKGuide( EGF_ELITEINSTANCE);

    return TRUE;
}


BOOL CBuild::AddFunctionModuleAndGuide_Godweapon()
{
    m_poOwner->CKGuide( EGF_GODWEAPON);

    return TRUE;//
}


BOOL CBuild::AddFunctionModuleAndGuide_MoneyTree()
{
    if(NULL != m_poOwner->GetMoneyTree())
    {
        return TRUE;
    }
    DT_MONEY_TREE_DATA stDT_MONEY_TREE_DATA;
    memset(&stDT_MONEY_TREE_DATA, 0, sizeof(stDT_MONEY_TREE_DATA));
    stDT_MONEY_TREE_DATA.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewMoneyTree(stDT_MONEY_TREE_DATA))
    {
        return FALSE;
    }

    m_poOwner->CKGuide( EGF_ROCKMONEY);

    return TRUE;//
}


BOOL CBuild::AddFunctionModuleAndGuide_Plunder()
{
    if(NULL != m_poOwner->GetPlunder())
    {
        return TRUE;
    }

    DT_PLUNDER_DATA stDT_PLUNDER_DATA = {0};
    stDT_PLUNDER_DATA.stPlunderBaseData.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewPlunder(stDT_PLUNDER_DATA))
    {
        return FALSE;
    }
    m_poOwner->CKGuide( EGF_PLUNDER);
    return TRUE;//
}

BOOL CBuild::AddFunctionModuleAndGuide_Race()
{
    CC_LOOP_DO
    if(NULL != m_poOwner->GetRace())
    {
        break;
    }
    BOOL		bRecover = FALSE;
    UINT32	dwRank = 0;
    const SPlayerBaseData *poPlayerBaseData1 = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_poOwner->GetDT_PLAYER_BASE_DATA().dwID);
    if (NULL == poPlayerBaseData1)
    {
        return FALSE;
    }

    SPlayerBaseData *poPlayerBaseData2 = CRaceMgr::Instance()->GetPlayerBaseData(poPlayerBaseData1->dwRank);
    if (poPlayerBaseData1 == poPlayerBaseData2)
    {
        dwRank = poPlayerBaseData2->dwRank;
        if(0 == dwRank)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: Player[%u] exist rank is 0!"), MSG_MARK, m_poOwner->GetID());
        }
    }
    if(0 == dwRank)
    {
        bRecover = TRUE;
        dwRank = CRaceMgr::Instance()->GetNewRankID();
    }

    DT_RACE_DATA stDT_RACE_DATA = {0};
    stDT_RACE_DATA.stRaceBaseData.dwRank = dwRank;
    stDT_RACE_DATA.stRaceBaseData.byHaveDataFlag = 1;
    stDT_RACE_DATA.stRaceBaseData.dwRecvEncourageRank = stDT_RACE_DATA.stRaceBaseData.dwRank;//
    stDT_RACE_DATA.stRaceBaseData.qwRecvEncourageTime = SDTimeSecs();//第一天不给奖励

    if(NULL == m_poOwner->NewRace(stDT_RACE_DATA))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddRace failed!"), MSG_MARK);
        if (bRecover)
        {
            CRaceMgr::Instance()->ReturnRankID();//归还排名
        }
        return FALSE;
    }

    if(!CPlayerBaseDataCacheMgr::Instance()->OnRankNew(m_poOwner->GetDT_PLAYER_BASE_DATA().dwID, stDT_RACE_DATA.stRaceBaseData.dwRank))
    {
        if (bRecover)
        {
            CRaceMgr::Instance()->ReturnRankID();//归还排名
        }
        SYS_CRITICAL(_SDT("[%s: %d]: OnRankNew failed!"), MSG_MARK);
        return FALSE;
    }
    //m_poRace->SetSaveState(ESS_WANT_SAVE);
    //m_poRace->SetObjDataSaveType(EODST_INSERT);

    //引导
    m_poOwner->CKGuide( EGF_RACE);
    CC_LOOP_WHILE(0);

    //膜拜功能跟竞技场同时开放
    CC_LOOP_DO
    if(NULL != m_poOwner->GetWorship())
    {
        break;
    }
    DT_WORSHIP_DATA_LST stDT_WORSHIP_DATA_LST = {0};
    stDT_WORSHIP_DATA_LST.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewWorship(stDT_WORSHIP_DATA_LST))
    {
        return FALSE;
    }
    CC_LOOP_WHILE(0);
    return TRUE;
}


BOOL CBuild::AddFunctionModuleAndGuide_Talent()
{
    m_poOwner->CKGuide( EGF_DRUG);

    return TRUE;//
}

BOOL CBuild::AddFunctionModuleAndGuide_Recuit()
{
    m_poOwner->CKGuide( EGF_RECUIT);
    //若没有开阵型，则开阵型
    if(NULL == m_poOwner->GetBuild(EBK_FROMATION))
    {
        m_poOwner->NewBuild(EBK_FROMATION);
    }
    return TRUE;//
}

BOOL CBuild::AddFunctionModuleAndGuide_Smithy()
{
    m_poOwner->CKGuide( EGF_STRENGTHEN);

    return TRUE;//
}

BOOL CBuild::AddFunctionModuleAndGuide_Maintown()
{
    m_poOwner->CKGuide( EGF_CROWN);

    return TRUE;//
}

/*
BOOL CBuild::AddFunctionModuleAndGuide_Skill()
{
	m_poOwner->CKGuide( EGF_DRESSSKILL);
	return TRUE;//
}
*/

BOOL CBuild::AddFunctionModuleAndGuide_ScienceTree()
{
    if(NULL != m_poOwner->GetScienceTree())
    {
        return TRUE;
    }

    DT_SCIENCE_TREE_DATA stDT_SCIENCE_TREE_DATA = {0};
    stDT_SCIENCE_TREE_DATA.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewScienceTree(stDT_SCIENCE_TREE_DATA))
    {
        return FALSE;
    }

    m_poOwner->CKGuide( EGF_SCIENCETREE);
    return TRUE;
}

BOOL CBuild::AddFunctionModuleAndGuide_Crop()
{
    if(NULL != m_poOwner->GetCrop())
    {
        return TRUE;
    }
    DT_CROP_DATA stDT_CROP_DATA;
    memset(&stDT_CROP_DATA, 0, sizeof(DT_CROP_DATA));
    stDT_CROP_DATA.byHaveDataFlag = 1;
    stDT_CROP_DATA.astCropInfoList[0].byOpen = 1;//开放一块田地
    if(NULL == m_poOwner->NewCrop(stDT_CROP_DATA))
    {
        return FALSE;
    }

    m_poOwner->CKGuide( EGF_CROP);
    return TRUE;//
}

BOOL CBuild::AddFunctionModuleAndGuide_ClimbTower()
{
    if(NULL != m_poOwner->GetClimbTower())
    {
        return TRUE;
    }

    DT_CLIMB_TOWER_DATA stDT_CLIMB_TOWER_DATA = {0};
    stDT_CLIMB_TOWER_DATA.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewClimbTower(stDT_CLIMB_TOWER_DATA))
    {
        return FALSE;
    }

    m_poOwner->CKGuide( EGF_CLIMBTOWER);
    return TRUE;//
}

BOOL CBuild::AddFunctionModuleAndGuide_Adventure()
{
    //m_poOwner->CKGuide( EGF_ADVENTURE);
    return TRUE;//
}


BOOL CBuild::AddFunctionModuleAndGuide_Experience()//经验丹 添加功能模块及引导
{
    m_poOwner->CKGuide( EGF_EXPRIENCE);
    return TRUE;//
}

BOOL CBuild::AddFunctionModuleAndGuide_UpdateAttr()
{
    m_poOwner->CKGuide( EGF_UPDATEATTR );
    return TRUE;
}

BOOL CBuild::AddFunctionModuleAndGuide_Levy()
{
    m_poOwner->CKGuide( EGF_LEVY );
    return TRUE;
}

BOOL CBuild::AddFunctionModuleAndGuide_Melting()
{
    m_poOwner->CKGuide( EGF_MELTING);
    return TRUE;
}


BOOL CBuild::AddFunctionModuleAndGuide_Yabiao()
{
    if(NULL != m_poOwner->GetYabiaoInfo())
    {
        return TRUE;
    }

    DT_YABIAO_DATA stDT_YABIAO_DATA = {0};
    stDT_YABIAO_DATA.stYabiaoBaseInfo.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewYabiaoInfo(stDT_YABIAO_DATA))
    {
        return FALSE;
    }

    m_poOwner->CKGuide( EGF_YABIAO);
    return TRUE;
}

BOOL CBuild::AddFunctionModuleAndGuide_EatBread()
{
    if(NULL != m_poOwner->GetEatBread())
    {
        return TRUE;
    }
    //初始化吃包子信息
    DT_EAT_BREAD_DATA stDT_EAT_BREAD_DATA;
    memset(&stDT_EAT_BREAD_DATA, 0, sizeof(stDT_EAT_BREAD_DATA));
    stDT_EAT_BREAD_DATA.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewEatBread(stDT_EAT_BREAD_DATA))
    {
        return FALSE;
    }

    return TRUE;//
}


//打开建筑回调
VOID CBuild::OnOpen()
{
    m_stDT_BUILD_BASE_DATA.byHaveOpenFlag = 1;
    switch(m_stDT_BUILD_BASE_DATA.byKindID)
    {
    case EBK_RECRUIT:
        m_stDT_BUILD_BASE_DATA.byValue3 = 0; //对于招募馆，则表示是否存在新伙伴可招募
    default:
        break;
    }
}


BOOL CBuild::AddFunctionModuleAndGuide_JingJie()
{
    m_poOwner->CKGuide( EGF_JINGJIEUPGRADE);

    if(NULL != m_poOwner->GetJingJie())
    {
        return TRUE;
    }
    CHero *m_poCoachHero = m_poOwner->GetCoachHero();
    DT_JINGJIE_DATA stDT_JINGJIE_DATA;
    memset(&stDT_JINGJIE_DATA, 0, sizeof(stDT_JINGJIE_DATA));
    stDT_JINGJIE_DATA.byHaveDataFlag = 1;
    stDT_JINGJIE_DATA.stBaseData.byCareerID = m_poCoachHero->GetHeroProp()->byCareerID;
    stDT_JINGJIE_DATA.stBaseData.wLevel = 1; //默认1阶
    stDT_JINGJIE_DATA.stSkillActionData.byActionNum = 1; //招式为主将初始招式
    stDT_JINGJIE_DATA.stSkillActionData.astActionInfo[0].wID = m_poCoachHero->GetHeroProp()->wSkillActionID;
    stDT_JINGJIE_DATA.stSkillActionData.astActionInfo[0].wLevel = m_poCoachHero->GetHeroProp()->wSkillActionLevel;

    if(NULL == m_poOwner->NewJingJie(stDT_JINGJIE_DATA))
    {
        return FALSE;
    }

    return TRUE;//
}

BOOL CBuild::AddFunctionModuleAndGuide_Skill()
{
    //技能引导
    m_poOwner->CKGuide( EGF_SKILLUPGRADE);

    return TRUE;//
}

BOOL CBuild::AddFunctionModuleAndGuide_CollectGas()
{
    if(NULL != m_poOwner->GetCollectGas())
    {
        return TRUE;
    }

    DT_COLLECT_GAS_DATA stDT_COLLECT_GAS_DATA = {0};
    stDT_COLLECT_GAS_DATA.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewCollectGas(stDT_COLLECT_GAS_DATA))
    {
        return FALSE;
    }


    //适配旧版本
    if(m_poOwner->GetCliVer() >= 292)
    {
        m_poOwner->CKGuide( EGF_GASCOLLECT);
    }

    return TRUE;//
}


BOOL CBuild::AddFunctionModuleAndGuide_Xiangqian()
{
    m_poOwner->CKGuide( EGF_XIANGQIAN);
    return TRUE;
}

BOOL CBuild::AddFunctionModuleAndGuide_BossB()
{
    if(NULL != m_poOwner->GetBossBattle())
    {
        return TRUE;
    }
    DT_BOSS_BATTLE_PLAYER_DATA_INFO stDT_BOSS_BATTLE_PLAYER_DATA_INFO = {0};
    stDT_BOSS_BATTLE_PLAYER_DATA_INFO.stBossBattleData.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewBossBattle(stDT_BOSS_BATTLE_PLAYER_DATA_INFO))
    {
        return FALSE;
    }

    return TRUE;//
}

BOOL CBuild::AddFunctionModuleAndGuide_RDC()
{
    CC_LOOP_DO
    if(NULL != m_poOwner->GetRdChallenge())
    {
        break;;
    }

    DT_RANDOM_CHALLENGE_DATA stDT_RANDOM_CHALLENGE_DATA = {0};
    stDT_RANDOM_CHALLENGE_DATA.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewRdChallenge(stDT_RANDOM_CHALLENGE_DATA))
    {
        return FALSE;
    }
    m_poOwner->CKGuide( EGF_RDC);
    CC_LOOP_WHILE(0);


    //膜拜功能跟竞技场同时开放
    CC_LOOP_DO
    if(NULL != m_poOwner->GetWorship())
    {
        break;
    }
    DT_WORSHIP_DATA_LST stDT_WORSHIP_DATA_LST = {0};
    stDT_WORSHIP_DATA_LST.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewWorship(stDT_WORSHIP_DATA_LST))
    {
        return FALSE;
    }
    CC_LOOP_WHILE(0);

    return TRUE;//
}

BOOL CBuild::AddFunctionModuleAndGuide_OpenLight()
{
    m_poOwner->CKGuide( EGF_OPENLIGHT);

    return TRUE;//
}

BOOL CBuild::AddFunctionModuleAndGuide_Friends()
{
    if(NULL != m_poOwner->GetFriends())
    {
        return TRUE;
    }
    DT_FRIENDS_DATA stDT_FRIENDS_DATA = {0};
    stDT_FRIENDS_DATA.stFriendsBaseData.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewFriends(stDT_FRIENDS_DATA))
    {
        return FALSE;
    }

    //m_poOwner->CKGuide( EGF_FRIENDS);
    return TRUE;//
}


BOOL CBuild::AddFunctionModuleAndGuide_BegBlue()
{
    if(NULL != m_poOwner->GetBegBlue())
    {
        return TRUE;
    }

    DT_BEGBLUE_DATA stDT_BEGBLUE_DATA = {0};
    stDT_BEGBLUE_DATA.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewBegBlue(stDT_BEGBLUE_DATA))
    {
        return FALSE;
    }

    //m_poOwner->CKGuide( EGF_FRIENDS);
    return TRUE;//
}



BOOL CBuild::AddFunctionModuleAndGuide_EquipMaster()
{
    if(NULL != m_poOwner->GetEquipMaster())
    {
        return TRUE;
    }
    DT_EQUIP_MASTER stDT_EQUIP_MASTER = {0};
    stDT_EQUIP_MASTER.byHaveDataFlag = 1; // 设置为已解锁，如果装备精通建筑被关闭的话，数据将仍然保留

    if(NULL == m_poOwner->NewEquipMaster(stDT_EQUIP_MASTER))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CBuild::AddFunctionModuleAndGuide_Courage()
{
    if(NULL != m_poOwner->GetCourage())
    {
        return TRUE;
    }

    DT_COURAGE_DATA stDT_COURAGE_DATA = {0};
    stDT_COURAGE_DATA.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewCourage(stDT_COURAGE_DATA))
    {
        return FALSE;
    }

    return TRUE;//
}

BOOL CBuild::AddFunctionModuleAndGuide_Faction()
{
    if(NULL != m_poOwner->GetFactionPlayer())
    {
        return TRUE;
    }

    DT_PLAYER_FACTION_DATA stPlayerFaction = {0};
    stPlayerFaction.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewFactionPlayer(stPlayerFaction))
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CBuild::AddFunctionModuleAndGuide_Fromation()//布阵
{
    //if(NULL != m_poOwner->GetFormation())
    //{
    //    return TRUE;
    //}

    //DT_FORMATION_DATA stDT_FORMATION_DATA = {0};
    //stDT_FORMATION_DATA.byHaveDataFlag = 1;
    //if(NULL == m_poOwner->NewFormation(stDT_FORMATION_DATA))
    //{
    //    return FALSE;
    //}
    return TRUE;
}

BOOL CBuild::AddFunctionModuleAndGuide_GVG()
{
	if(NULL != m_poOwner->GetPlayerGVG())
	{
		return TRUE;
	}

	DT_PLAYER_GVG_DATA stGvg = {0};
	stGvg.byHaveDataFlag = 1;
	if(NULL == m_poOwner->NewPlayerGVG(stGvg))
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CBuild::AddFunctionModuleAndGuide_Soul()
{
	if(NULL != m_poOwner->GetPlayerSoul())
	{
		return TRUE;
	}

	DT_PLAYER_SOUL_DATA stSoul = {0};
	stSoul.byHaveDataFlag = 1;
	if(NULL == m_poOwner->NewPlayerSoul(stSoul))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CBuild::AddFunctionModuleAndGuide_Medal()
{
	if(NULL != m_poOwner->GetPlayerMedal())
	{
		return TRUE;
	}

	DT_PLAYER_MEDAL_DATA stMedal = {0};
	stMedal.byHaveDataFlag = 1;
	if(NULL == m_poOwner->NewPlayerMedal(stMedal))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CBuild::AddFunctionModuleAndGuide_Pet() {
	if (NULL != m_poOwner->GetPlayerPet()) {
		return TRUE;
	}
	DT_PLAYER_PET_DATA playerPetData = {0};
	playerPetData.byHaveDataFlag = 1;
	if (NULL == m_poOwner->NewPlayerPet(playerPetData)) {
		return FALSE;
	}
	m_poOwner->CKGuide(EGF_PET);
	return TRUE;
}

BOOL CBuild::AddFunctionModuleAndGuide_OnHook()	//挂机
{
    if(NULL != m_poOwner->GetOnhook())
    {
        return TRUE;
    }

    DT_ONHOOK_RCD_DATA stDT_ONHOOK_RCD_DATA = {0};
    stDT_ONHOOK_RCD_DATA.byHaveDataFlag = 1;
    if(NULL == m_poOwner->NewOnhook(stDT_ONHOOK_RCD_DATA))
    {
        return FALSE;
    }
    return TRUE;
}
