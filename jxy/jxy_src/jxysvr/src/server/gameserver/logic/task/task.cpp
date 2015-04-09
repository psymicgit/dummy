#include "task.h"
#include "taskpropmgr.h"
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <net/gt/gtpktbuilder.h>
#include <logic/task/taskprop.h>
#include <logic/task/finishconditon.h>
#include <logic/task/acceptconditon.h>
#include <common/client/errdef.h>
#include "logic/record/recordmgr.h"
#include <logic/base/basedefine.h>
#include <logic/item/itemmgr.h>
#include <logic/instance/instance.h>
#include <logic/hero/hero.h>
#include <logic/jingjie/jingjie.h>
#include <logic/player/player.h>
#include <utility.h>

using namespace SGDP;



CTask::CTask()
{
    m_byCanAcceptFlag = 0;
    memset(&m_stDT_TASK_DATA, 0, sizeof(m_stDT_TASK_DATA));
}


CTask::~CTask()
{

}

BOOL CTask::Init(DT_TASK_DATA* pstDT_TASK_DATA, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }

    STaskProp* poTaskProp = CTaskPropMgr::Instance()->GetTaksProp(pstDT_TASK_DATA->byTaskLine, pstDT_TASK_DATA->dwTaskIdx);
    if(NULL == poTaskProp)
    {
        CTaskLineProp* poTaskLineProp = CTaskPropMgr::Instance()->GetTaskLineProp(pstDT_TASK_DATA->byTaskLine);
        if(poTaskLineProp)
        {
            poTaskProp = poTaskLineProp->GetNextTask(pstDT_TASK_DATA->dwTaskIdx);
        }
        if(NULL == poTaskProp)
        {
            USR_INFO(_SDT("[%s: %d]: poTaskProp is NULL(%u, %u)!"), MSG_MARK, pstDT_TASK_DATA->byTaskLine, pstDT_TASK_DATA->dwTaskIdx);
            return FALSE;
        }
    }

    //重新设置任务类型，以防些任务改变，至任务无法完成。
    pstDT_TASK_DATA->byTaskType = poTaskProp->eTaskType;
    memcpy(&m_stDT_TASK_DATA, pstDT_TASK_DATA, sizeof(DT_TASK_DATA));
    m_stDT_TASK_DATA.dwTaskIdx = poTaskProp->dwTaskIdx;
    if(0 == m_stDT_TASK_DATA.qwStateUpdateTime)//
    {
        m_stDT_TASK_DATA.qwStateUpdateTime = SDTimeSecs();
    }

    //如果该任务是无效状态，但是任务属性又有下一个任务，直接向下调整到下一个任务上去。
    if(ETS_INVALID == m_stDT_TASK_DATA.byTaskState && 0 != poTaskProp->dwNextTaskIdx)
    {
        memset(&m_stDT_TASK_DATA, 0x00, sizeof(DT_TASK_DATA));
        m_stDT_TASK_DATA.byTaskLine = pstDT_TASK_DATA->byTaskLine;
        m_stDT_TASK_DATA.dwTaskIdx =  poTaskProp->dwNextTaskIdx;
        poTaskProp = CTaskPropMgr::Instance()->GetTaksProp(m_stDT_TASK_DATA.byTaskLine, m_stDT_TASK_DATA.dwTaskIdx);
        if(NULL == poTaskProp)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: poTaskProp is NULL(%u, %u)!"), MSG_MARK, m_stDT_TASK_DATA.byTaskLine, m_stDT_TASK_DATA.dwTaskIdx);
            return FALSE;
        }
        //重新设置任务类型，以防些任务改变，至任务无法完成。
        m_stDT_TASK_DATA.byTaskState = ETS_GIVEN;
        m_stDT_TASK_DATA.byTaskType = poTaskProp->eTaskType;
        m_stDT_TASK_DATA.qwStateUpdateTime = SDTimeSecs();
    }

    m_poOwner = poOwner;
    if(ETS_GIVEN == m_stDT_TASK_DATA.byTaskState)
    {
        CheckAccept();//检测是否可接
    }

    if(ETS_ACCEPTED == m_stDT_TASK_DATA.byTaskState)
    {
        CheckFinish();//检测是否完成
    }

    return TRUE;
}


UINT16 CTask::CheckAccept()
{
    STaskProp* poTaskProp = GetTaskProp();
    if(NULL == poTaskProp)
    {
        RETURN_OTHER_ERR;
    }


    UINT16 wErrCode = CheckAccept(poTaskProp->mapAcceptCondition);
    //可接受,修改状态,下发任务接受通知
    if(ERR_ACCEPT_TASK::ID_SUCCESS == wErrCode)
    {
        m_stDT_TASK_DATA.byTaskState = ETS_ACCEPTED;
        m_stDT_TASK_DATA.qwStateUpdateTime = SDTimeSecs();
        if(MAIN_LINE_TASK == m_stDT_TASK_DATA.byTaskLine)
        {
            m_poOwner->SetLastMainLineTaskProp(poTaskProp->dwTaskIdx);
        }
    }

    return wErrCode;
}


BOOL CTask::CheckFinish()
{
    STaskProp* poTaskProp = GetTaskProp();
    if(NULL == poTaskProp)
    {
        return FALSE;
    }

    //任务完成，修改状态, 下发任务完成通知
    if(CheckFinish(poTaskProp->poFinishCondition))
    {
        m_stDT_TASK_DATA.byTaskState = ETS_ENCOURAGE;
        m_stDT_TASK_DATA.qwStateUpdateTime = SDTimeSecs();
        //SetSaveState(ESS_WANT_SAVE);
        return TRUE;
    }

    return FALSE;
}


VOID CTask::GetDT_TASK_BASE_DATA_CLI(DT_TASK_BASE_DATA& stDT_TASK_BASE_DATA)
{
    stDT_TASK_BASE_DATA.byTaskType = m_stDT_TASK_DATA.byTaskType;

    STaskProp* poTaskProp = GetTaskProp();
    if(poTaskProp)
    {
        _SDTStrncpy(stDT_TASK_BASE_DATA.aszTaskDispName, poTaskProp->strDispName.c_str(), MAX_TASK_DSPNAME_LEN - 1);
        _SDTStrncpy(stDT_TASK_BASE_DATA.aszTaskDesc, poTaskProp->strDesc.c_str(), MAX_TASK_DESC_LEN - 1);

        //返回完成条件
        switch(m_stDT_TASK_DATA.byTaskType)
        {
        case CROWN:
        {
            CFinishCondition_Crown* poFC = dynamic_cast<CFinishCondition_Crown*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stCrown.wCrownTimes = poFC->wCrownTimes;
            }
        }
        break;
        case RECRUITHERO:
        {
            CFinishCondition_RecruitHero* poFC = dynamic_cast<CFinishCondition_RecruitHero*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stRecruitHero.wRecruitHeroKindID1 = poFC->wRecruitHeroKindID1;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stRecruitHero.wRecruitHeroKindID2 = poFC->wRecruitHeroKindID2;
            }
        }
        break;
        case UPGRADESKILL:
        {
            //UINT16 wUnlockIdx = 0;
            //CFinishCondition_UpgradeSkill* poFC = dynamic_cast<CFinishCondition_UpgradeSkill*>(poTaskProp->poFinishCondition);
            //if(poFC)
            //{
            //    wUnlockIdx = poFC->wUnlockIdx;
            //    stDT_TASK_BASE_DATA.stTaskConditionInfo.stUpgradeSkill.wLevel = poFC->wLevel;
            //}

            //CJingJie *poJingJie = m_poOwner->GetJingJie();
            //if ((0 != wUnlockIdx) && (NULL != poJingJie))
            //{
            //    stDT_TASK_BASE_DATA.stTaskConditionInfo.stUpgradeSkill.wSkillActionID = poJingJie->GetSkillActionIDByUnlockIdx(wUnlockIdx);
            //}
            //else
            //{
            //    stDT_TASK_BASE_DATA.stTaskConditionInfo.stUpgradeSkill.wSkillActionID = 0;
            //}

            CFinishCondition_UpgradeSkill* poFC = dynamic_cast<CFinishCondition_UpgradeSkill*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stUpgradeSkill.wSkillActionID = 0;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stUpgradeSkill.wLevel = poFC->wLevel;
            }
        }
        break;
        case UPGRADEJINGJIE:
        {
            CFinishCondition_UpgradeJingJie* poFC = dynamic_cast<CFinishCondition_UpgradeJingJie*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stUpgradeJingJie.wLevel = poFC->wLevel;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stUpgradeJingJie.wSubLevel = poFC->wSubLevel;
            }
        }
        break;
        case MOSAICGEM:
        {
            CFinishCondition_MosaicGem* poFC = dynamic_cast<CFinishCondition_MosaicGem*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stMosaicGem.wNum = poFC->wNum;
            }
        }
        break;
        case CLIMBTOWER:
        {
            CFinishCondition_ClimbTower* poFC = dynamic_cast<CFinishCondition_ClimbTower*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stClimbTower.wNum = poFC->wNum;
            }
        }
        break;
        case FIRSTPASSINSTANCE:
        {
            CFinishCondition_FirstPassInstance* poFC = dynamic_cast<CFinishCondition_FirstPassInstance*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stFirstPassInstance.wSceneIdx = poFC->wSceneIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stFirstPassInstance.byTownIdx = poFC->byTownIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stFirstPassInstance.byInstanceIdx = poFC->byInstanceIdx;
            }
        }
        break;
        case SCOREPASSINSTANCE:
        {
            CFinishCondition_ScorePassInstance* poFC = dynamic_cast<CFinishCondition_ScorePassInstance*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stScorePassInstance.wSceneIdx = poFC->wSceneIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stScorePassInstance.byTownIdx = poFC->byTownIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stScorePassInstance.byInstanceIdx = poFC->byInstanceIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stScorePassInstance.byScore = poFC->byScore;
            }
        }
        break;
        case TIMESPASSINSTANCE:
        {
            CFinishCondition_TimesPassInstance* poFC = dynamic_cast<CFinishCondition_TimesPassInstance*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stTimesPassInstance.wSceneIdx = poFC->wSceneIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stTimesPassInstance.byTownIdx = poFC->byTownIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stTimesPassInstance.byInstanceIdx = poFC->byInstanceIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stTimesPassInstance.byTimes = poFC->byTimes;
            }
        }
        break;
        case KILLMONSTERINSTANCE:
        {
            CFinishCondition_KillMonstersInstance* poFC = dynamic_cast<CFinishCondition_KillMonstersInstance*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stKillMonstersInstance.wSceneIdx = poFC->wSceneIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stKillMonstersInstance.byTownIdx = poFC->byTownIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stKillMonstersInstance.byInstanceIdx = poFC->byInstanceIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stKillMonstersInstance.wMonsterKindID = poFC->wMonsterKindID;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stKillMonstersInstance.byKillNum = poFC->byKillNum;
            }
        }
        break;
        case STRENGTHENEQUIPLEVEL:
        {
            CFinishCondition_StrengthenEquipLevel* poFC = dynamic_cast<CFinishCondition_StrengthenEquipLevel*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stStrengthenEquipLevel.wEquipKindID = poFC->wEquipKindID;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stStrengthenEquipLevel.wLevel = poFC->wLevel;
            }
        }
        break;
        case STRENGTHENEQUIPNUM:
        {
            CFinishCondition_StrengthenEquipNum* poFC = dynamic_cast<CFinishCondition_StrengthenEquipNum*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stStrengthenEquipNum.wEquipKindID = poFC->wEquipKindID;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stStrengthenEquipNum.wNum = poFC->wNum;
            }
        }
        break;
        break;
        case ACTIVATEGODWEAPON:
        {
            CFinishCondition_ActivateGodweapon* poFC = dynamic_cast<CFinishCondition_ActivateGodweapon*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stActivateGodweapon.wHeroKindID = poFC->wHeroKindID;
            }
        }
        break;
        case UPGRADEGODWEAPON:
        {
            CFinishCondition_UpgradeGodweapon* poFC = dynamic_cast<CFinishCondition_UpgradeGodweapon*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                DT_UPGRADEGODWEAPON_TASK_CONDITION& stUpgradeGodweapon = stDT_TASK_BASE_DATA.stTaskConditionInfo.stUpgradeGodweapon;
                stUpgradeGodweapon.wHeroKindID = poFC->wHeroKindID;
                stUpgradeGodweapon.wQuality = poFC->wQuality;
                stUpgradeGodweapon.wLevel = poFC->wLevel;
            }
        }
        break;

        case CHALLENGE:
        {
            CFinishCondition_Challenge* poFC = dynamic_cast<CFinishCondition_Challenge*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stChallenge.wChallengeNum = poFC->wChallengeNum;
            }
        }
        break;
        case PLUNDER:
        {
            CFinishCondition_Plunder* poFC = dynamic_cast<CFinishCondition_Plunder*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stPlunder.wPlunderNum = poFC->wPlunderNum;
            }
        }
        break;
        case USEEXPRIENCEDRUG:
        {
            CFinishCondition_UseExprienceDrug* poFC = dynamic_cast<CFinishCondition_UseExprienceDrug*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stUseExprienceDrug.wNum	=	poFC->wNum;
            }
        }
        break;
        case MELTEQUIP:
        {
            CFinishCondition_MultEquip* poFC = dynamic_cast<CFinishCondition_MultEquip*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stMeltEquip.wNum	=	poFC->wNum;
            }
        }
        break;
        case STUDY:
        {
            CFinishCondition_Study* poFC = dynamic_cast<CFinishCondition_Study*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stStudy.byLevel	=	poFC->byLevel;
            }
        }
        break;
        case COMPOSEGOOD:
        {
            CFinishCondition_ComposeGood* poFC = dynamic_cast<CFinishCondition_ComposeGood*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stComposeGood.wMaterialID  = poFC->wMaterialID;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stComposeGood.wMaterialNum = poFC->wMaterialNum;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stComposeGood.wKindID	=	poFC->wKindID;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stComposeGood.wNum = poFC->wNum;
            }
        }
        break;
        case ELITEINSTANCE:
        {
            CFinishCondition_EliteInstance* poFC = dynamic_cast<CFinishCondition_EliteInstance*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stEliteInstance.wSceneIdx = poFC->wSceneIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stEliteInstance.byTownIdx = poFC->byTownIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stEliteInstance.byInstanceIdx = poFC->byInstanceIdx;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stEliteInstance.byUpgradeFlag = 0;
            }
        }
        break;
        case UPGRADSCIENCE:
        {
            CFinishCondition_UpgradeScience* poFC = dynamic_cast<CFinishCondition_UpgradeScience*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stUpgradeScience.wNum = poFC->wNum;
            }
        }
        break;
        case STRENGTHATTACK:
        {
            CFinishCondition_StrengthAttack* poFC = dynamic_cast<CFinishCondition_StrengthAttack*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stStrengthAttack.wNum = poFC->wNum;
            }
        }
        break;
        case STRENGTHLIFE:
        {
            CFinishCondition_StrengthLife* poFC = dynamic_cast<CFinishCondition_StrengthLife*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stStrengthLife.wNum = poFC->wNum;
            }
        }
        break;
        case CROP:
        {
            CFinishCondition_Crop* poFC = dynamic_cast<CFinishCondition_Crop*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stCrop.wNum = poFC->wNum;
            }
        }
        break;
        case DRESSEQUIP:
        {
            CFinishCondition_DressEquip* poFC = dynamic_cast<CFinishCondition_DressEquip*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stDressEquip.wEquipID = poFC->wEquipID;
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stDressEquip.byDressFlag = 0;
            }
        }
        break;
        case RECRUITHERONUM:
        {
            CFinishCondition_RecruitHeroNum* poFC = dynamic_cast<CFinishCondition_RecruitHeroNum*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stRecruitHeroNum.wNum = poFC->wNum;
            }
        }
        break;

        case OPENLIGHT:
        {
            CFinishCondition_OpenLight* poFC = dynamic_cast<CFinishCondition_OpenLight*>(poTaskProp->poFinishCondition);
            if(poFC)
            {
                stDT_TASK_BASE_DATA.stTaskConditionInfo.stOpenLight.byExt = poFC->byExt;
            }
        }
        break;

        default:
            break;
        }
    }
    //memcpy(&(stDT_TASK_BASE_DATA.stTaskConditionInfo), &(m_oConditionCheck.m_stTaskConditionInfo), sizeof(DT_TASK_CONDITION));

    //奖励
    GetDT_TASK_ENCOURAGE_DATA(stDT_TASK_BASE_DATA.stTaskEncourage);
}

VOID CTask::GetDT_TASK_ENCOURAGE_DATA(DT_TASK_ENCOURAGE_DATA& stTaskEncourage)
{
    STaskProp* poTaskProp = GetTaskProp();
    if(NULL == poTaskProp)
    {
        return;
    }
    STaskEncourageProp& stTaskEncourageProp = poTaskProp->stTaskEncourageProp;

    stTaskEncourage.dwCoin = stTaskEncourageProp.dwCoin;
    stTaskEncourage.dwGold = stTaskEncourageProp.dwGold;
    stTaskEncourage.wPhyStrength = stTaskEncourageProp.wPhyStrength;
    stTaskEncourage.dwScience = stTaskEncourageProp.dwScience;
    //stTaskEncourage.dwFame = stTaskEncourageProp.dwFame;
    stTaskEncourage.dwExperience = stTaskEncourageProp.dwExperience;
    stTaskEncourage.dwStory = stTaskEncourageProp.dwStory;

    stTaskEncourage.dwBlueGas = stTaskEncourageProp.dwBlueGas;
    stTaskEncourage.dwPurpleGas = stTaskEncourageProp.dwPurpleGas;
    stTaskEncourage.dwJingJie = stTaskEncourageProp.dwJingJie;
    //道具
    CItemKindID2NumMap& mapItem = stTaskEncourageProp.mapItemNum;
    UINT8 byNum = 0;
    for(CItemKindID2NumMapItr itr = mapItem.begin(); ((itr != mapItem.end()) && (byNum < MAX_ENCOURAGE_ITEM_KIND_NUM)); itr++)
    {
        UINT16 wKindID = itr->first;
        UINT8 byItemKind = CItemMgr::Instance()->GetItemKindBYKindID(wKindID);
        if(EIK_GOODS == byItemKind)
        {
            SGoodsProp* pstProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wKindID);
            if(pstProp)
            {
                DT_ITEM_DATA_CLI& stDT_ITEM_DATA_CLI = stTaskEncourage.astItemInfoList[byNum++];
                stDT_ITEM_DATA_CLI.wKindID = itr->first;
                stDT_ITEM_DATA_CLI.byGoodsMainKindID = pstProp->byMainKindID;
                stDT_ITEM_DATA_CLI.wPileNum = itr->second;
            }
        }
        else
        {
            DT_ITEM_DATA_CLI& stDT_ITEM_DATA_CLI = stTaskEncourage.astItemInfoList[byNum++];
            stDT_ITEM_DATA_CLI.wKindID = itr->first;
            stDT_ITEM_DATA_CLI.byGoodsMainKindID = 0;
            stDT_ITEM_DATA_CLI.wPileNum = itr->second;
        }
    }
    stTaskEncourage.byItemNum = byNum;
}

//奖励
UINT16 CTask::Encourage()
{
    STaskProp* poTaskProp = GetTaskProp();
    if((ETS_ENCOURAGE != m_stDT_TASK_DATA.byTaskState) || (NULL == poTaskProp))
    {
        return ERR_RECV_TASK_ENCOURAGE::ID_STATE_ERR;
    }

    STaskEncourageProp& stSTaskEncourageProp = poTaskProp->stTaskEncourageProp;
    vector<UINT16> vecItemID;
    for(CItemKindID2NumMapItr itr = stSTaskEncourageProp.mapItemNum.begin(); itr != stSTaskEncourageProp.mapItemNum.end(); itr++)
    {
        vecItemID.push_back(itr->first);
    }
    if(CItemMgr::Instance()->CkBagFull(m_poOwner, vecItemID))
    {
        return ERR_RECV_TASK_ENCOURAGE::ID_BAG_FULL_ERR;
    }

    m_poOwner->AddCoin(stSTaskEncourageProp.dwCoin, CRecordMgr::EACT_TASK, poTaskProp->byTaskLineIdx, poTaskProp->dwTaskIdx);
    m_poOwner->AddGold(stSTaskEncourageProp.dwGold, CRecordMgr::EGGT_ADDGOLDTASK, poTaskProp->byTaskLineIdx, poTaskProp->dwTaskIdx);
    m_poOwner->AddPhyStrength(stSTaskEncourageProp.wPhyStrength, CRecordMgr::EAPST_TASK, poTaskProp->byTaskLineIdx, poTaskProp->dwTaskIdx);
    m_poOwner->AddScience(stSTaskEncourageProp.dwScience, CRecordMgr::EAST_TASK, poTaskProp->byTaskLineIdx, poTaskProp->dwTaskIdx);
    m_poOwner->AddStory(stSTaskEncourageProp.dwStory, CRecordMgr::EASTT_TASK, poTaskProp->byTaskLineIdx, poTaskProp->dwTaskIdx);

    m_poOwner->AddBlueGas(stSTaskEncourageProp.dwBlueGas, CRecordMgr::EABGT_TASK, poTaskProp->byTaskLineIdx, poTaskProp->dwTaskIdx);
    m_poOwner->AddPurpleGas(stSTaskEncourageProp.dwPurpleGas, CRecordMgr::EAPGT_TASK, poTaskProp->byTaskLineIdx, poTaskProp->dwTaskIdx);
    m_poOwner->AddJingJie(stSTaskEncourageProp.dwJingJie, CRecordMgr::EAJT_TASK, poTaskProp->byTaskLineIdx, poTaskProp->dwTaskIdx);

    m_poOwner->AllotExperience(stSTaskEncourageProp.dwExperience);

    //道具奖励
    for(CItemKindID2NumMapItr itr = stSTaskEncourageProp.mapItemNum.begin(); itr != stSTaskEncourageProp.mapItemNum.end(); itr++)
    {
        ECreateItemRet Ret = ECIR_SUCCESS;
        CItem* poItem = CItemMgr::Instance()->CreateItemIntoBag(m_poOwner, itr->first, itr->second, Ret, 0, CRecordMgr::EAIT_TASK);
        if(NULL == poItem)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CreateItem failed[%d], ItemKindID=%d !"), MSG_MARK, Ret, itr->first);
        }
        //若为装备，则判断是否需穿戴引导
        else if(EIK_EQUIP == CItemMgr::Instance()->GetItemKindBYKindID(poItem->GetItemKindID()))
        {
            m_poOwner->CKGuide( EGF_EQUIP);
        }
    }

    //修改状态
    m_stDT_TASK_DATA.byTaskState = ETS_FINISHED;
    //SetSaveState(ESS_WANT_SAVE);

    return ERR_RECV_TASK_ENCOURAGE::ID_SUCCESS;
}




UINT16 CTask::CheckAccept(CAcceptType2CheckMap& mapAcceptCondition)
{
    if(NULL == m_poOwner)
    {
        RETURN_OTHER_ERR;
    }

    m_byCanAcceptFlag = 0;
    UINT16 wErrCode = 0;
    //接受条件是与的关系
    for(CAcceptType2CheckMapItr itr = mapAcceptCondition.begin();  itr != mapAcceptCondition.end(); itr++)
    {
        CAcceptCondition* poAcceptCondition = itr->second;
        switch(poAcceptCondition->GetAcceptType())
        {
        case EACT_PLAYER:
        {
            if(m_poOwner->GetLevel() >= ((CAcceptCondition_Player*)poAcceptCondition)->wPlayerLevel)
            {
                wErrCode = ERR_ACCEPT_TASK::ID_SUCCESS;
            }
            else
            {
                return ERR_ACCEPT_TASK::ID_NOT_RACHE_LEVEL;
            }
        }
        break;
        case EACT_INSTANCE:
        {

            DT_INSTANCE_ID& stCommonInstanceID = m_poOwner->GetInstance().GetMaxCommonInstanceID();
            DT_INSTANCE_ID& stEliteInstanceID = m_poOwner->GetInstance().GetMaxEliteInstanceID();

            UINT16 wSceneIdx = ((CAcceptCondition_Instance*)poAcceptCondition)->wSceneIdx;
            UINT16 byTownIdx = ((CAcceptCondition_Instance*)poAcceptCondition)->byTownIdx;
            UINT16 byInstanceIdx = ((CAcceptCondition_Instance*)poAcceptCondition)->byInstanceIdx;
            if(EIT_COMMON == wSceneIdx)
            {
                if((byTownIdx < stCommonInstanceID.byTownIdx)
                        || (byTownIdx == stCommonInstanceID.byTownIdx) && (byInstanceIdx <= stCommonInstanceID.byInstanceIdx))
                {
                    wErrCode = ERR_ACCEPT_TASK::ID_SUCCESS;
                }
                else
                {
                    return ERR_ACCEPT_TASK::ID_NOT_PASS_INSTANCE;
                }
            }
            else if(EIT_ELITE == wSceneIdx)
            {
                if((byTownIdx < stEliteInstanceID.byTownIdx)
                        || (byTownIdx == stEliteInstanceID.byTownIdx) && (byInstanceIdx <= stEliteInstanceID.byInstanceIdx))
                {
                    wErrCode = ERR_ACCEPT_TASK::ID_SUCCESS;
                }
                else
                {
                    return ERR_ACCEPT_TASK::ID_NOT_PASS_INSTANCE;
                }
            }
            else
            {
                RETURN_OTHER_ERR;
            }
        }
        break;
        //case EACT_BUILD:
        //{
        //    CBuild* poBuild = NULL;
        //    //任意建筑等级达到
        //    if(0 == ((CAcceptCondition_Build*)poAcceptCondition)->byBuildKindID)
        //    {
        //        //取主城等级（主城等级最高）
        //        poBuild = m_poOwner->GetBuild(EBK_MAINTOWN);
        //    }
        //    else
        //    {
        //        poBuild = m_poOwner->GetBuild((EBuildKind)((CAcceptCondition_Build*)poAcceptCondition)->byBuildKindID);
        //    }
        //    if(poBuild)
        //    {
        //        if(poBuild->GetDT_BUILD_BASE_DATA().wLevel >= ((CAcceptCondition_Build*)poAcceptCondition)->wLevel)
        //        {
        //            wErrCode = ERR_ACCEPT_TASK::ID_SUCCESS;
        //        }
        //        else
        //        {
        //            return ERR_ACCEPT_TASK::ID_NOT_RACHE_BUILD_LEVEL;
        //        }
        //    }
        //    else
        //    {
        //        RETURN_OTHER_ERR;
        //    }
        //}
        //break;
        case EACT_MAINTASK:
        {
            if(m_poOwner->CkTaskFinish(MAIN_LINE_TASK, ((CAcceptCondition_MainTask*)poAcceptCondition)->dwTaskIdx))
            {
                wErrCode = ERR_ACCEPT_TASK::ID_SUCCESS;
            }
            else
            {
                return ERR_ACCEPT_TASK::ID_NOT_PASS_MAINTASK;
            }
        }
        break;
        default:
            RETURN_OTHER_ERR;
            break;
        }
    }

    m_byCanAcceptFlag = 1;

    return ERR_ACCEPT_TASK::ID_SUCCESS;
}

BOOL CTask::CheckFinish(CFinishCondition* poFinishCondition)
{
    switch(m_stDT_TASK_DATA.byTaskType)
    {
    case CROWN:
    {
        CFinishCondition_Crown* poFinCond = dynamic_cast<CFinishCondition_Crown*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        if(m_stDT_TASK_DATA.stTaskConditionInfo.stCrown.wCrownTimes >= poFinCond->wCrownTimes)
        {
            return TRUE;
        }
    }
    break;
    case RECRUITHERO:
    {
        CFinishCondition_RecruitHero* poFinCond = dynamic_cast<CFinishCondition_RecruitHero*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        CHero* poHero = m_poOwner->GetHero(poFinCond->wRecruitHeroKindID1);
        if((poHero) && (1 == poHero->GetDT_HERO_BASE_DATA().byRecuitedFlag))
        {
            return TRUE;
        }
        poHero = m_poOwner->GetHero(poFinCond->wRecruitHeroKindID2);
        if((poHero) && (1 == poHero->GetDT_HERO_BASE_DATA().byRecuitedFlag))
        {
            return TRUE;
        }
        return FALSE;
    }
    break;
    case FIRSTPASSINSTANCE:
    {
        CFinishCondition_FirstPassInstance* poFinCond = dynamic_cast<CFinishCondition_FirstPassInstance*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        CInstanceRecord* poInstanceRecord = m_poOwner->GetInstance().GetSingleInstaceRecord(poFinCond->wSceneIdx, poFinCond->byTownIdx, poFinCond->byInstanceIdx);
        if(NULL == poInstanceRecord)
        {
            return FALSE;
        }
        return TRUE;
    }
    break;
    case SCOREPASSINSTANCE:
    {
        CFinishCondition_ScorePassInstance* poFinCond = dynamic_cast<CFinishCondition_ScorePassInstance*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        CInstanceRecord* poInstanceRecord = m_poOwner->GetInstance().GetSingleInstaceRecord(poFinCond->wSceneIdx, poFinCond->byTownIdx, poFinCond->byInstanceIdx);
        if(NULL == poInstanceRecord)
        {
            return FALSE;
        }

        //if(poInstanceRecord->GetDT_INSTANCE_DATA().byScore >= poFinCond->byScore)
        //{
        //    return TRUE;
        //}
        //else
        //{
        //    return FALSE;
        //}
    }
    break;
    case TIMESPASSINSTANCE:
    {
        CFinishCondition_TimesPassInstance* poFinCond = dynamic_cast<CFinishCondition_TimesPassInstance*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        if(m_stDT_TASK_DATA.stTaskConditionInfo.stTimesPassInstance.byTimes >= poFinCond->byTimes)
        {
            return TRUE;
        }
    }
    break;
    case KILLMONSTERINSTANCE:
    {
        CFinishCondition_KillMonstersInstance* poFinCond = dynamic_cast<CFinishCondition_KillMonstersInstance*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        if(m_stDT_TASK_DATA.stTaskConditionInfo.stKillMonstersInstance.byKillNum >= poFinCond->byKillNum)
        {
            return TRUE;
        }

        return FALSE;
    }
    break;
    case STRENGTHENEQUIPLEVEL:
    {
        CFinishCondition_StrengthenEquipLevel* poFinCond = dynamic_cast<CFinishCondition_StrengthenEquipLevel*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        //任意装备
        if(0 == poFinCond->wEquipKindID)
        {
            CPlayerEquipShmemMap& mapEquip = m_poOwner->GetAllEquip();
            CEquip* poEquip = mapEquip.GetFistData();
            while(poEquip)
            {
                if(poEquip->GetDT_EQUIP_DATA().wLevel >= poFinCond->wLevel)
                {
                    return TRUE;
                }

                poEquip = mapEquip.GetNextData();
            }
        }
        else
        {
            CPlayerEquipShmemMap& mapEquip = m_poOwner->GetAllEquip();
            CEquip* poEquip = mapEquip.GetFistData();
            while(poEquip)
            {
                if((poEquip->GetDT_EQUIP_DATA().wKindID == poFinCond->wEquipKindID) && (poEquip->GetDT_EQUIP_DATA().wLevel >= poFinCond->wLevel))
                {
                    return TRUE;
                }

                poEquip = mapEquip.GetNextData();
            }
        }
        return FALSE;
    }
    break;
    case STRENGTHENEQUIPNUM:
    {
        CFinishCondition_StrengthenEquipNum* poFinCond = dynamic_cast<CFinishCondition_StrengthenEquipNum*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        if(m_stDT_TASK_DATA.stTaskConditionInfo.stStrengthenEquipNum.wNum >= poFinCond->wNum)
        {
            return TRUE;
        }

        return FALSE;
    }
    break;
    case ACTIVATEGODWEAPON:
    {
        CFinishCondition_ActivateGodweapon* poFinCond = dynamic_cast<CFinishCondition_ActivateGodweapon*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        CHero* poHero = NULL;
        if(0 == poFinCond->wHeroKindID)
        {
            poHero = m_poOwner->GetCoachHero();
        }
        else
        {
            poHero = m_poOwner->GetHero(poFinCond->wHeroKindID);
        }

        if((poHero) && (poHero->GetGodweapon()))
        {
            return TRUE;
        }

        if(0xFFFF == poFinCond->wHeroKindID)
        {
            return TRUE;
        }
        return FALSE;
    }
    break;
    case UPGRADEGODWEAPON:
    {
        CFinishCondition_UpgradeGodweapon* poFinCond = dynamic_cast<CFinishCondition_UpgradeGodweapon*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }


        CHero* poHero = NULL;

        if(0 == poFinCond->wHeroKindID)
        {
            poHero = m_poOwner->GetCoachHero();
        }
        else
        {
            poHero = m_poOwner->GetHero(poFinCond->wHeroKindID);
        }

        if(NULL == poHero)
        {
            return FALSE;
        }
        CGodWeapon* poGodWeapon = poHero->GetGodweapon();
        if(NULL == poGodWeapon)
        {
            return FALSE;
        }

        DT_GODWEAPON_DATA& stDT_GODWEAPON_DATA = poGodWeapon->GetDT_GODWEAPON_DATA();
        if((stDT_GODWEAPON_DATA.wQuality > poFinCond->wQuality) ||
                ((stDT_GODWEAPON_DATA.wQuality == poFinCond->wQuality) && (stDT_GODWEAPON_DATA.wLevel >= poFinCond->wLevel)))
        {
            return TRUE;
        }
        return FALSE;
    }
    break;
    case USEEXPRIENCEDRUG:
    {
        CFinishCondition_UseExprienceDrug* poFinCond = dynamic_cast<CFinishCondition_UseExprienceDrug*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        if(m_stDT_TASK_DATA.stTaskConditionInfo.stUseExprienceDrug.wNum >= poFinCond->wNum)
        {
            return TRUE;
        }
        return FALSE;
    }
    break;
    case MELTEQUIP:
    {
        CFinishCondition_MultEquip* poFinCond = dynamic_cast<CFinishCondition_MultEquip*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        if(m_stDT_TASK_DATA.stTaskConditionInfo.stMeltEquip.wNum >= poFinCond->wNum)
        {
            return TRUE;
        }
        return FALSE;
    }
    break;
    case STUDY:
    {
        CFinishCondition_Study* poFinCond = dynamic_cast<CFinishCondition_Study*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        //if(m_stDT_TASK_DATA.stTaskConditionInfo.stStudy.byLevel >= poFinCond->byLevel)
        //{
        //    return TRUE;
        //}

        UINT8 byMaxLevel = 0;
        CFormationHeroShmemMap& mapHero = m_poOwner->GetFormationHeroMap();
        CHero* poHero = mapHero.GetFistData();
        while(poHero)
        {
            UINT8 byLevel = poHero->GetMaxStudyLevel();
            if (byLevel > byMaxLevel)
            {
                byMaxLevel = byLevel;
            }
            poHero = mapHero.GetNextData();
        }
        if(byMaxLevel >= poFinCond->byLevel)
        {
            return TRUE;
        }
        return FALSE;
    }
    break;
    case COMPOSEGOOD:
    {
        CFinishCondition_ComposeGood* poFinCond = dynamic_cast<CFinishCondition_ComposeGood*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        if(m_stDT_TASK_DATA.stTaskConditionInfo.stComposeGood.wNum >= poFinCond->wNum)
        {
            return TRUE;
        }
        return FALSE;
    }
    break;
    case ELITEINSTANCE:
    {
        CFinishCondition_EliteInstance* poFinCond = dynamic_cast<CFinishCondition_EliteInstance*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        CInstanceRecord* poInstanceRecord = m_poOwner->GetInstance().GetSingleInstaceRecord(poFinCond->wSceneIdx, poFinCond->byTownIdx, poFinCond->byInstanceIdx);
        if(NULL == poInstanceRecord)
        {
            return FALSE;
        }
        return TRUE;
    }
    break;
    case UPGRADSCIENCE:
    {
        CFinishCondition_UpgradeScience* poFinCond = dynamic_cast<CFinishCondition_UpgradeScience*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }

        if(m_stDT_TASK_DATA.stTaskConditionInfo.stUpgradeScience.wNum >= poFinCond->wNum)
        {
            return TRUE;
        }
        return FALSE;
    }
    break;
    case STRENGTHATTACK:
    {
        CFinishCondition_StrengthAttack* poFinCond = dynamic_cast<CFinishCondition_StrengthAttack*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }

        if(m_stDT_TASK_DATA.stTaskConditionInfo.stStrengthAttack.wNum >= poFinCond->wNum)
        {
            return TRUE;
        }
        return FALSE;
    }
    break;
    case STRENGTHLIFE:
    {
        CFinishCondition_StrengthLife* poFinCond = dynamic_cast<CFinishCondition_StrengthLife*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }

        if(m_stDT_TASK_DATA.stTaskConditionInfo.stStrengthLife.wNum >= poFinCond->wNum)
        {
            return TRUE;
        }
        return FALSE;
    }
    break;
    case CROP:
    {
        CFinishCondition_Crop* poFinCond = dynamic_cast<CFinishCondition_Crop*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }

        if(m_stDT_TASK_DATA.stTaskConditionInfo.stCrop.wNum >= poFinCond->wNum)
        {
            return TRUE;
        }
        return FALSE;
    }
    break;
    case CHALLENGE:
    {
        CFinishCondition_Challenge* poFinCond = dynamic_cast<CFinishCondition_Challenge*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;

        }
        if(m_stDT_TASK_DATA.stTaskConditionInfo.stChallenge.wChallengeNum >= poFinCond->wChallengeNum)
        {
            return TRUE;
        }
    }
    break;
    case PLUNDER:
    {
        CFinishCondition_Plunder* poFinCond = dynamic_cast<CFinishCondition_Plunder*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        if(m_stDT_TASK_DATA.stTaskConditionInfo.stPlunder.wPlunderNum >= poFinCond->wPlunderNum)
        {
            return TRUE;
        }

        return FALSE;
    }
    break;
    case UPGRADESKILL:
    {
        CFinishCondition_UpgradeSkill* poFinCond = dynamic_cast<CFinishCondition_UpgradeSkill*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        CSkill* poSkill = m_poOwner->GetCoachSkill();
        if(NULL == poSkill)
        {
            return FALSE;
        }
        if((poSkill->GetDT_SKILL_DATA().wActionLevel) >= poFinCond->wLevel)
        {
            return TRUE;
        }

        return FALSE;
    }
    break;
    case UPGRADEJINGJIE:
    {
        CFinishCondition_UpgradeJingJie* poFinCond = dynamic_cast<CFinishCondition_UpgradeJingJie*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        //获取境界指针并检测是否开放
        CJingJie *poJingJie = m_poOwner->GetJingJie();
        if (NULL == poJingJie)
        {
            return FALSE;
        }
        DT_JINGJIE_BASE_DATA& stBaseData = poJingJie->GetDT_JINGJIE_DATA().stBaseData;
        if((stBaseData.wLevel > poFinCond->wLevel) ||
                ((stBaseData.wLevel == poFinCond->wLevel) && (stBaseData.wSubLevel >= poFinCond->wSubLevel)))
        {
            return TRUE;
        }
        return FALSE;
    }
    break;
    case MOSAICGEM:
    {
        CFinishCondition_MosaicGem* poFinCond = dynamic_cast<CFinishCondition_MosaicGem*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        //获取当前玩家的装备最多的镶嵌个数。
        if(m_poOwner->GetMaxMosaicGemNum() >= poFinCond->wNum)
        {
            return TRUE;
        }
    }
    break;
    case CLIMBTOWER:
    {
        CFinishCondition_ClimbTower* poFinCond = dynamic_cast<CFinishCondition_ClimbTower*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        //获取当前玩家的层数。
        if(m_poOwner->GetInstance().GetMaxPassClimbTower() >= poFinCond->wNum)
        {
            return TRUE;
        }
        return FALSE;
    }
    break;
    case DRESSEQUIP:
    {
        CFinishCondition_DressEquip* poFinCond = dynamic_cast<CFinishCondition_DressEquip*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        if(1 == m_stDT_TASK_DATA.stTaskConditionInfo.stDressEquip.byDressFlag)
        {
            return TRUE;
        }
    }
    break;
    case RECRUITHERONUM:
    {
        CFinishCondition_RecruitHeroNum* poFinCond = dynamic_cast<CFinishCondition_RecruitHeroNum*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        //获取当前玩家的装备最多的镶嵌个数。
        if((m_poOwner->GetHeroAllCarry() - 1) >= poFinCond->wNum)
        {
            return TRUE;
        }
    }
    break;

    case OPENLIGHT:
    {
        CFinishCondition_OpenLight* poFinCond = dynamic_cast<CFinishCondition_OpenLight*>(poFinishCondition);
        if(NULL == poFinCond)
        {
            return FALSE;
        }
        if(m_stDT_TASK_DATA.stTaskConditionInfo.stOpenLight.byExt >= poFinCond->byExt)
        {
            return TRUE;
        }
        return FALSE;
    }
    break;

    default:
        return FALSE;
    }

    return FALSE;
}


STaskProp* CTask::GetTaskProp()
{
    STaskProp* poTaskProp = CTaskPropMgr::Instance()->GetTaksProp(m_stDT_TASK_DATA.byTaskLine, m_stDT_TASK_DATA.dwTaskIdx);
    if(NULL == poTaskProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: poTaskProp is NULL, line[%d], idx[%d]!"), MSG_MARK, m_stDT_TASK_DATA.byTaskLine, m_stDT_TASK_DATA.dwTaskIdx);
    }
    return poTaskProp;
}
