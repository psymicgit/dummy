
#include "gmmgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <logic/base/basedefine.h>
#include <common/client/errdef.h>
#include <logic/player/playermgr.h>
#include <db/autosqlbuf.h>
#include <logic/base/basedefine.h>
#include <logic/race/racepropmgr.h>
#include <logic/task/taskmgr.h>
#include <logic/monster/monstermgr.h>
#include <logic/other/logicconfigmgr.h>
#include <db/autosqlbuf.h>
#include <protocol/server/protogsns.h>
#include "logic/record/recordmgr.h"
#include <common/client/gmdef.h>
#include <logic/hero/hero.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <framework/gsapi.h>
#include <logic/instance/instance.h>
#include <protogsdb.h>
#include <logic/player/playergetdatamgr.h>
#include <logic/player/player.h>

IMPLEMENT_SINGLETON(CGmMgr)


CGmMgr::CGmMgr()
{

}

CGmMgr::~CGmMgr()
{

}

BOOL CGmMgr::Init()
{
    return TRUE;
}


VOID CGmMgr::UnInit()
{

}

VOID CGmMgr::AddCoin(UINT32 dwPlayerID, UINT32 dwCoin)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwPlayerID, GET_PLAYERDATATYPE_GM, dwPlayerID, dwCoin, 0, 0, "", EGMT_ADDCOIN);
        return;
    }

    //poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin += dwCoin;
    poPlayer->AddCoin(dwCoin, CRecordMgr::EACT_GMCOMMAND);
}


VOID CGmMgr::AddGold(UINT32 dwPlayerID, UINT32 dwGold)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwPlayerID, GET_PLAYERDATATYPE_GM, dwPlayerID, dwGold, 0, 0, "", EGMT_ADDGOLD);
        return;
    }
    poPlayer->AddGold(dwGold, CRecordMgr::EGGT_ADDFLAGGM);
}

VOID CGmMgr::AddStory(UINT32 dwPlayerID, UINT32 dwStory)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwPlayerID, GET_PLAYERDATATYPE_GM, dwPlayerID, dwStory, 0, 0, "", EGMT_ADDSTORY);
        return;
    }

    //poPlayer->GetDT_PLAYER_BASE_DATA().qwStory += dwStory;
    poPlayer->AddStory(dwStory, CRecordMgr::EASTT_GMCOMMAND);
}


VOID CGmMgr::AddScience(UINT32 dwPlayerID, UINT32 dwScience)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwPlayerID, GET_PLAYERDATATYPE_GM, dwPlayerID, dwScience, 0, 0, "", EGMT_ADDSCIENCE);
        return;
    }

    poPlayer->AddScience(dwScience, CRecordMgr::EAST_GMCOMMAND);
}

VOID CGmMgr::AddPhystrength(UINT32 dwPlayerID, UINT16 wPhystrength)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwPlayerID, GET_PLAYERDATATYPE_GM, dwPlayerID, wPhystrength, 0, 0, "", EGMT_ADDPHYSTRENGTH);
        return;
    }
    poPlayer->AddPhyStrength(wPhystrength, CRecordMgr::EAPST_GMCOMMAND);
}

VOID CGmMgr::SetTalent(UINT32 dwPlayerID, UINT16 wTalent)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        return;
    }

    poPlayer->GetCoachHero()->GetDT_HERO_BASE_DATA().wTalent = wTalent;
}

VOID CGmMgr::SetLevel(UINT32 dwPlayerID, UINT16 wLevel)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        return;
    }
    CHero* poHero = poPlayer->GetCoachHero();
    if(poHero)
    {
        //poHero->GetDT_HERO_BASE_DATA().wLevel = wLevel;
        poHero->SetLevel(wLevel);
        poPlayer->OnUpgrade(wLevel);
    }

}

VOID CGmMgr::AddRobot(UINT32 dwStartPlayerID, UINT8 byNum)
{
    for(UINT8 byIdx = 0; byIdx < byNum; byIdx++)
    {
        CPlayerMgr::Instance()->CreateNewPlayerGM(dwStartPlayerID + byIdx);
    }

    //将机器人读入内存中方可修改功能
    for(UINT8 byIdx = 0; byIdx < byNum; byIdx++)
    {
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwStartPlayerID + byIdx, GET_PLAYERDATATYPE_SYNCDATA, dwStartPlayerID + byIdx);
    }
}


VOID CGmMgr::AddBuild(UINT32 dwPlayerID, UINT8 byBuildKindID)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwPlayerID, GET_PLAYERDATATYPE_GM, dwPlayerID, byBuildKindID, 0, 0, "", EGMT_ADDBUILE);
        return;
    }

    poPlayer->NewBuild(byBuildKindID);
}

VOID CGmMgr::AddSkill(UINT32 dwPlayerID, UINT16 wSkillID, UINT8 bySlotIdx)
{
    //CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    //if(NULL == poPlayer)
    //{
    //	return;
    //}

    //poPlayer->AddSkill(wSkillID);
    //DT_SKILL_DATA_CLI stDressSkillInfo;
    //UINT8 byExchangeFlag;
    //DT_SKILL_DATA_CLI stExchangeSkillInfo;
    //poPlayer->DressSkill(wSkillID, bySlotIdx, 0, stDressSkillInfo, byExchangeFlag, stExchangeSkillInfo);
}

VOID CGmMgr::AddItem(UINT32 dwPlayerID, UINT16 wItemID, UINT16 wNum)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwPlayerID, GET_PLAYERDATATYPE_GM, dwPlayerID, wItemID, wNum, 0, "", EGMT_ADDITEM);
        return;
    }

    ECreateItemRet Ret = ECIR_SUCCESS;
    CItemMgr::Instance()->CreateItemIntoBag(poPlayer, wItemID, wNum, Ret, 0, CRecordMgr::EAIT_GMCOMMAND);
}

VOID CGmMgr::PassAllGuide(UINT32 dwPlayerID)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        
        return;
    }

    //for (UINT8 byGuideIdx = EGF_INVALID + 1; byGuideIdx <= 32; byGuideIdx++)
    //{
    //	poPlayer->CKGuide( (EGuideFunction) (0x1 << byGuideIdx));
    //}
}


VOID CGmMgr::PassGuide(UINT32 dwPlayerID, UINT8 byGuideIdx)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {

        
        return;
    }
    if((0 == byGuideIdx) || (byGuideIdx >= EGF_MAX))
    {
        return;
    }

    poPlayer->AddGuideRecord((EGuideFunction)byGuideIdx);
}


VOID CGmMgr::PassAllCommonInstance(UINT32 dwPlayerID)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {

        
        return;
    }

    CTask* poMainTask = poPlayer->GetTaskByLine(MAIN_LINE_TASK);
    if(NULL == poMainTask)
    {
        return;
    }
    CTaskLineProp* pstMainLineTaskProp  = CTaskPropMgr::Instance()->GetTaskLineProp(MAIN_LINE_TASK);
    if((NULL == pstMainLineTaskProp) || (0 == pstMainLineTaskProp->mapTaskProp.size()))
    {
        return;
    }
    STaskProp* pstMainTaskProp = pstMainLineTaskProp->mapTaskProp.rbegin()->second;
    UINT32 dwMaxIdx = pstMainTaskProp->dwTaskIdx;
    switch(poMainTask->GetDT_TASK_DATA().byTaskState)
    {
    case ETS_GIVEN:
    {
        poPlayer->GetGiveTask().DelData(MAIN_LINE_TASK);
    }
    break;
    case ETS_ACCEPTED:
    {
        poPlayer->GetAcceptTask().DelData(MAIN_LINE_TASK);
    }
    break;
    case ETS_ENCOURAGE:
    {
        poPlayer->GetEncourageTask().DelData(MAIN_LINE_TASK);
    }
    break;
    case ETS_FINISHED:
    {
        poPlayer->GetFinishTask().DelData(MAIN_LINE_TASK);
    }
    break;
    default:
        break;
    }
    DT_TASK_DATA& stDT_TASK_DATA = poMainTask->GetDT_TASK_DATA();
    stDT_TASK_DATA.dwTaskIdx = dwMaxIdx;
    stDT_TASK_DATA.byTaskState = ETS_ACCEPTED;
    stDT_TASK_DATA.qwStateUpdateTime = SDTimeSecs();
    poPlayer->SetLastMainLineTaskProp(pstMainTaskProp->dwTaskIdx);

    if(!CTaskMgr::Instance()->SetTaskCond(poPlayer, stDT_TASK_DATA.byTaskLine, stDT_TASK_DATA.dwTaskIdx, stDT_TASK_DATA.byTaskType, stDT_TASK_DATA.stTaskConditionInfo))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SetTaskCond failed!"), MSG_MARK);
        return;
    }
    //重新初始化任务信息
    if(FALSE == poMainTask->Init(&stDT_TASK_DATA, poPlayer))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: task init failed!"), MSG_MARK);
        return;
    }
    poPlayer->RecordChangeTask(poMainTask);
    DT_INSTANCE_DATA_LIST stInstanceLst;
    memset(&stInstanceLst, 0, sizeof(stInstanceLst));
    stInstanceLst.wSceneIdx = EIT_COMMON;
    DT_INSTANCE_ID stMaxInstanceID = pstMainTaskProp->stInstanceID;
    for(UINT8 byTownIdx = 1; ((byTownIdx <= stMaxInstanceID.byTownIdx) && (stInstanceLst.wInstanceNum < MAX_INSTANCE_PER_SCENE_NUM)); byTownIdx++)
    {
        for(UINT8 byInstanceIdx = 1; byInstanceIdx <= 3; byInstanceIdx++)
        {
            DT_INSTANCE_DATA& stDT_INSTANCE_DATA = stInstanceLst.astInstanceInfo[stInstanceLst.wInstanceNum++];
            //stDT_INSTANCE_DATA.byScore = 5;
            stDT_INSTANCE_DATA.qwLastPassTime = SDTimeSecs();
            //stDT_INSTANCE_DATA.wPassTimes = 1;
            DT_INSTANCE_ID& stInstanceID = stDT_INSTANCE_DATA.stInstanceID;
            stInstanceID.wSceneIdx = EIT_COMMON;
            stInstanceID.byTownIdx = byTownIdx;
            stInstanceID.byInstanceIdx = byInstanceIdx;
        }
    }

    poPlayer->GetInstance().AddSceneInstanceRecord(EIT_COMMON, stInstanceLst);
    poPlayer->CkOpenFunc();
}

VOID CGmMgr::AddHero(UINT32 dwPlayerID, UINT16 wHeroID)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {

        
        return;
    }
    poPlayer->NewHero(wHeroID);
}

VOID CGmMgr::ClrEliteChallenge( UINT32 dwPlayerID )
{
    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer( dwPlayerID );
    if( NULL == poPlayer )
    {

        
        return;
    }

    poPlayer->GetInstance().ClrChallengeTime();
}

VOID CGmMgr::ResetClimbTowerChallenge( UINT32 dwPlayerID )
{
    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer( dwPlayerID );
    if( NULL == poPlayer )
    {

        
        return;
    }
    for ( int i = 1; i <= MAX_CLIMB_TOWER_NUM; i++)
    {
        poPlayer->GetInstance().ResetChallengeTime(i);
    }
}

VOID CGmMgr::Notice(UINT32 dwPlayerID, char * szData)
{
    const SPlayerBaseData *poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
    if( NULL == poPlayerBaseData )
    {
        return;
    }

    //检测玩家是否是机器人。
    if(ERFK_PLAYER != poPlayerBaseData->byRobotFunc)
    {
        return;
    }

    if( 0 == strlen( (char*) poPlayerBaseData->abyToken))
    {
        return;
    }

    gsapi::NoticeMsg(dwPlayerID, ENT_OTHER, (char*)poPlayerBaseData->abyToken, SDGBK2UTF8(szData).c_str());
}

VOID CGmMgr::NoticeAll(char *szData)
{
    SPlayerBaseData * poPlayerBaseData = NULL;
    const CPlayerID2PlayerBaseDataMap & mapPlayerID2PlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerID2PlayerBaseDataMap();
    CPlayerID2PlayerBaseDataMapConstItr itr;

    for (itr = mapPlayerID2PlayerBaseData.begin(); itr != mapPlayerID2PlayerBaseData.end(); itr++)
    {
        poPlayerBaseData = itr->second;
        if( NULL == poPlayerBaseData )
        {
            continue;
        }

        //检测玩家是否是机器人。
        if(ERFK_PLAYER != poPlayerBaseData->byRobotFunc)
        {
            continue;
        }

        if( 0 == strlen( (char*) poPlayerBaseData->abyToken))
        {
            continue;
        }
        gsapi::NoticeMsg(poPlayerBaseData->dwID, ENT_OTHER, (char*)poPlayerBaseData->abyToken, SDGBK2UTF8(szData).c_str());
    }
}


VOID CGmMgr::ClrClimbTowerChallenge( UINT32 dwPlayerID )
{
    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer( dwPlayerID );
    if( NULL == poPlayer )
    {
        
        return;
    }
    for ( UINT8 byTower = 1; byTower <= MAX_CLIMB_TOWER_NUM; byTower++)
    {
        poPlayer->GetInstance().DelClimbTownInstaceRecord(EIT_TOWER, byTower);
    }
}




VOID CGmMgr::ReloadCfg(UINT8 byModuleKind)
{
    CLogicConfigMgr::ReloadCfg((EConfigModule)byModuleKind);
}


//////////////////////////////////////////////////////////////////////////
VOID CGmMgr::AddAllCoin(UINT32 dwCoin)
{
    CPlayer*			v_poPlayer = NULL;
    const CID2PlayerMap &	v_ID2PlayerMap = CPlayerMgr::Instance()->GetID2PlayerMap();
    for (CID2PlayerMapConstItr itr = v_ID2PlayerMap.begin(); itr != v_ID2PlayerMap.end(); itr++)
    {
        v_poPlayer = itr->second;
        if (NULL == v_poPlayer)
        {
            continue;
        }
        v_poPlayer->AddCoin(dwCoin, CRecordMgr::EACT_GMCOMMAND);
    }
}

VOID CGmMgr::AddAllGold(UINT32 dwGold)
{
    CPlayer*			v_poPlayer = NULL;
    const CID2PlayerMap &	v_ID2PlayerMap = CPlayerMgr::Instance()->GetID2PlayerMap();
    for (CID2PlayerMapConstItr itr = v_ID2PlayerMap.begin(); itr != v_ID2PlayerMap.end(); itr++)
    {
        v_poPlayer = itr->second;
        if (NULL == v_poPlayer)
        {
            continue;
        }
        v_poPlayer->AddGold(dwGold, CRecordMgr::EGGT_ADDFLAGGM);
    }
}

VOID CGmMgr::AddAllStory(UINT32 dwStory)
{
    CPlayer*			v_poPlayer = NULL;
    const CID2PlayerMap &	v_ID2PlayerMap = CPlayerMgr::Instance()->GetID2PlayerMap();
    for (CID2PlayerMapConstItr itr = v_ID2PlayerMap.begin(); itr != v_ID2PlayerMap.end(); itr++)
    {
        v_poPlayer = itr->second;
        if (NULL == v_poPlayer)
        {
            continue;
        }
        v_poPlayer->AddStory(dwStory, CRecordMgr::EASTT_GMCOMMAND);
    }
}

VOID CGmMgr::AddAllScience(UINT32 dwScience)
{
    CPlayer*			v_poPlayer = NULL;
    const CID2PlayerMap &	v_ID2PlayerMap = CPlayerMgr::Instance()->GetID2PlayerMap();
    for (CID2PlayerMapConstItr itr = v_ID2PlayerMap.begin(); itr != v_ID2PlayerMap.end(); itr++)
    {
        v_poPlayer = itr->second;
        if (NULL == v_poPlayer)
        {
            continue;
        }
        v_poPlayer->AddScience(dwScience, CRecordMgr::EAST_GMCOMMAND);
    }
}

VOID CGmMgr::AddAllPhystrength(UINT16 wPhystrength)
{
    CPlayer*			v_poPlayer = NULL;
    const CID2PlayerMap &	v_ID2PlayerMap = CPlayerMgr::Instance()->GetID2PlayerMap();
    for (CID2PlayerMapConstItr itr = v_ID2PlayerMap.begin(); itr != v_ID2PlayerMap.end(); itr++)
    {
        v_poPlayer = itr->second;
        if (NULL == v_poPlayer)
        {
            continue;
        }
        v_poPlayer->AddPhyStrength(wPhystrength, CRecordMgr::EAPST_GMCOMMAND);
    }
}