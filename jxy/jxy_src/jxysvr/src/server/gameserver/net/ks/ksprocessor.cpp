#include "ksprocessor.h"
#include <framework/gsconfig.h>

#include <stdio.h>
#include <commondef.h>
#include <sdutil.h>
#include <errdef.h>
#include <common/server/utility.h>
#include <logic/player/playermgr.h>

#include "logic/record/recordmgr.h"
#include "logic/other/logicconfigmgr.h"
#include "protocol/server/protogmgs.h"
#include <logic/other/jsonopmgr.h>
#include <logic/battle/battlemgr.h>
#include <logic/monster/monstermgr.h>
#include <logic/item/itemmgr.h>
#include <logic/vip/vippropmgr.h>
#include <logic/hero/hero.h>
#include <logic/jingjie/jingjie.h>
#include <logic/equipmaster/equipmaster.h>
#include <logic/task/taskmgr.h>
#include <common/client/gmdef.h>
#include "logic/player/playerbasedatacachemgr.h"
#include <logic/player/playersavemgr.h>
#include "logic/instance/instance.h"
#include "protocol/server/protocligs.h"
#include <framework/gsapi.h>
#include <protogsdb.h>
#include <logic/player/player.h>
#include <logic/player/playergetdatamgr.h>
#include <logic/chat/chatmgr.h>
#include <logic/activity/bossbattlemgr.h>
#include <logic/other/errmsgmgr.h>
#include "logic/awaken/awaken.h"
#include "logic/record/recordmgr.h"
#include "logic/faction/factionmgr.h"
#include "net/gt/gtpktbuilder.h"
#include "net/db/dbpktbuilder.h"
#include "db/settongbupayprocessed.h"

#include "logic/activity/courage.h"
#include "logic/faction/factionmgr.h"
#include "logic/other/robotmgr.h"
#include "logic/other/commoninfomgr.h"
#include "logic/gm/gmmgr.h"
#include "framework/gsapi.h"
#include "logic/gvg/global/gvgmgr.h"
#include "logic/event/timereventmgr.h"
#include "logic/item/equip/equipmakemgr.h"
#include "logic/hero/heropropmgr.h"

using namespace std;

#define ROBOT_BASE_ID 10000000

CGMProcessor::CGMProcessor()
{
    Init() ;
}

CGMProcessor::~CGMProcessor()
{

}

CSDProtocol* CGMProcessor::GetProtocol()
{
    return CProtoGMGS::Instance();
}


BOOL CGMProcessor::Init()
{
    //使用CGTPipeChannel处理的消息,需要在CGTPipeChannel::OnRecvz中特别处理
    RegisterCommand(GMGS_PARM_INFO_REQ, CGMProcessor::OnParmInfoReq);
    //RegisterCommand(GMGS_GET_EQUIP_INFO_REQ, CGMProcessor::OnGetEquipInfoReq);

    return TRUE ;
}


BOOL CGMProcessor::OnParmInfoReq(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32 dwTransID            = pPipeHeader->dwTransID;


    PKT_GMGS_PARM_INFO_REQ* pReq = (PKT_GMGS_PARM_INFO_REQ*)pBody;
    if( sizeof(PKT_GMGS_PARM_INFO_REQ) != dwLen )
    {
        return FALSE;
    }

    PKT_GMGS_PARM_INFO_ACK stAck;
    memset(&stAck, 0x00, sizeof(PKT_GMGS_PARM_INFO_ACK));
    stAck.wErrCode	= ERR_GM_PARM_INFO::ID_SUCCESS;
    stAck.dwPlayerID	= pReq->dwPlayerID;
    stAck.dwSerialNumber = pReq->dwSerialNumber;

    stAck.wErrCode = GmReq(pReq->dwSerialNumber, pReq->byAction, pReq->dwPlayerID, pReq->dwParam1, pReq->qwParam2, pReq->qwParam3, pReq->aszParam4);
    if (ERR_GM_PARM_INFO::ID_PLAYER_NOT_MEMORT != stAck.wErrCode)
    {
        gsapi::GMMsgResponse(GMGS_PARM_INFO_ACK, pReq->dwPlayerID, (CHAR*)&stAck);
    }
    return TRUE;
}

UINT16 CGMProcessor::GmReq(UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR aszParam4[MAX_PARM_LEN], UINT32 dwTransID, string strDesc)
{
    USR_INFO(_SDT("[%s: %d]: GmReq dwSerialNumber[%u], byAction[%u],dwPlayerID[%u], dwParam1[%u], qwParam2[%llu], qwParam3[%llu], aszParam4[%s], dwTransID[%u]!"), MSG_MARK,
             dwSerialNumber, byAction, dwPlayerID, dwParam1, qwParam2, qwParam3, aszParam4, dwTransID);

    CSDServerID	oServerID(SDGetApp()->GetLocalID());
    if(oServerID.GetServerType() == TYPE_PK_SERVER)// || oServerID.GetServerType() == TYPE_GAME_SERVER)
    {
        return GmReq(NULL, dwSerialNumber, byAction, dwPlayerID, dwParam1, qwParam2, qwParam3, aszParam4, dwTransID, strDesc);
    }

    CPlayer* poPlayer = NULL;
    UINT16 wErrCode = ERR_GM_PARM_INFO::ID_SUCCESS;
    const SPlayerBaseData* pPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
    if (NULL == pPlayerBaseData && EGMT_RELOADCFG != byAction)
    {
        return ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR;
    }

    //针对单个用户的配置信息
	poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if(NULL == poPlayer && EGMT_RELOADCFG != byAction)
	{
		CGetPlayerDataMgr::Instance()->GetPlayerData(dwTransID, GET_PLAYERDATATYPE_GM, dwPlayerID, dwParam1, qwParam2, qwParam3, "", byAction, dwSerialNumber, strDesc);
		return ERR_GM_PARM_INFO::ID_PLAYER_NOT_MEMORT;
	}
    return GmReq(poPlayer, dwSerialNumber, byAction, dwPlayerID, dwParam1, qwParam2, qwParam3, aszParam4, dwTransID, strDesc);
}


UINT16 CGMProcessor::GmReq(CPlayer* poPlayer, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR aszParam4[MAX_PARM_LEN], UINT32 dwTransID, string strDesc)
{
    UINT16 wErrCode = ERR_GM_PARM_INFO::ID_SUCCESS;
    CHero*			poHero = NULL;
    ECreateItemRet	Ret = ECIR_SUCCESS;
    switch (byAction)
    {
    case EGMT_ADDCOIN:
    {
        poPlayer->AddCoin(dwParam1, CRecordMgr::EACT_GMCOMMAND, 0, 0, 0, 0, strDesc);
    }
    break;
    case EGMT_ADDGOLD:
    {
        poPlayer->AddGold(dwParam1, CRecordMgr::EGGT_ADDFLAGGM, 0, 0, 0, 0, strDesc);
    }
    break;
    case EGMT_ADDSTORY:
    {
        poPlayer->AddStory(dwParam1, CRecordMgr::EASTT_GMCOMMAND, 0, 0, 0, 0, strDesc);
    }
    break;
    case EGMT_ADDSCIENCE:
    {
        poPlayer->AddScience(dwParam1, CRecordMgr::EAST_GMCOMMAND, 0, 0, 0, 0, strDesc);
    }
    break;
    case EGMT_ADDPHYSTRENGTH:
    {
        poPlayer->AddPhyStrength(dwParam1, CRecordMgr::EAPST_GMCOMMAND, 0, 0, 0, 0, strDesc);
    }
    break;
    case EGMT_ADDBUILE:
    {
        CBuild* pBuild = poPlayer->NewBuild(dwParam1);
        if (NULL == pBuild)
        {
            wErrCode = ERR_GM_PARM_INFO::ID_OTHER_ERR;
        }
        break;
    }
    case EGMT_ADDITEM:
    {
        CItem* pItem = CItemMgr::Instance()->CreateItemIntoBag(poPlayer, dwParam1, (UINT16)qwParam2, Ret, 0, CRecordMgr::EAIT_GMCOMMAND);
        if (NULL == pItem)
        {
            wErrCode = ERR_GM_PARM_INFO::ID_OTHER_ERR;
        }
    }
    break;
    case EGMT_RELOADCFG:
    {
        CLogicConfigMgr::ReloadCfg((EConfigModule)dwParam1);
    }
    break;
    case EGMT_SETLEVEL:
    {
        poHero = poPlayer->GetCoachHero();
        if(poHero)
        {
            poHero->SetLevel(dwParam1);
            poPlayer->OnUpgrade(dwParam1);
        }
    }
    break;
    case EGMT_PASSALLCOMMINSTANCE:
    {
        PassAllCommonInstance(poPlayer);
    }
    break;
    case EGMT_PASSALLGUIDE:
    {
        for (UINT8 byGuideIdx = EGF_INVALID + 1; byGuideIdx < EGF_MAX; byGuideIdx++)
        {
            poPlayer->AddGuideRecord( (EGuideFunction) (byGuideIdx));
        }
    }
    break;
    case EGMT_ADDBLUE:
    {
        poPlayer->AddBlueGas(dwParam1, CRecordMgr::EABGT_GM , 0, 0, 0, 0, strDesc);
    }
    break;
    case EGMT_ADDPURPLE:
    {
        poPlayer->AddPurpleGas(dwParam1, CRecordMgr::EAPGT_GM , 0, 0, 0, 0, strDesc);
    }
    break;
    case EGMT_SETVIPLEVEL:
    {
        poPlayer->SetVipLevel(dwParam1, (UINT32)(qwParam2));
    }
    break;
    case EGMT_ADDSTUDY:
    {
        CAwaken *poAwaken = poPlayer->GetAwaken();
        if (poAwaken)
        {
            poAwaken->AddStudyForGm ((UINT32)(dwParam1), (UINT8)(qwParam2), (UINT8)(qwParam3));
        }
        else
        {
            wErrCode = ERR_GM_PARM_INFO::ID_OTHER_ERR;
        }
    }
    break;
    case EGMT_ADDSTUDYEXP:
    {
        CAwaken *poAwaken = poPlayer->GetAwaken();
        if (poAwaken)
        {
            poAwaken->AddStudyExpForGm(dwParam1 );
        }
        else
        {
            wErrCode = ERR_GM_PARM_INFO::ID_OTHER_ERR;
        }
        break;
    }
    case EGMT_DEC_CLIP:
    {
        CAwaken *poAwaken = poPlayer->GetAwaken();
        if (poAwaken)
        {
            poAwaken->DecStudyClipForGm(dwParam1 );
        }
        else
        {
            wErrCode = ERR_GM_PARM_INFO::ID_OTHER_ERR;
        }
    }
    break;
    case EGMT_ADDPAYGOLD:
    {
        //太危险，跳过
        break;

        /*
        //处理结果
        //同时把分转成单位元
        SBuyGoldProp buyInfo;
        UINT32		dwAmount;
        if( CVipPropMgr::Instance()->GetBuyPropByRMB(poPlayer->GetAuthType(), dwParam1, buyInfo ) )
        {
            dwAmount = buyInfo.dwGold;
        }
        else
        {
            //对于找不到的值，直接把分转换成元宝。一无等于10元宝
            dwAmount = dwParam1 * 10;
        }

        //玩家在线，则增加元宝
        poPlayer->AddGold(dwAmount, CRecordMgr::EGGT_ADDFLAGEXCHANGE, 0, 0, 0, 0, strDesc, dwParam1 * 100);//元转分
        */
        break;
    }
    case EGMT_ADDJINGJIE:
    {
        poPlayer->AddJingJie((UINT32)(dwParam1), CRecordMgr::EAJT_GM , 0, 0, 0, 0, strDesc);
    }
    break;
    case EGMT_DEC_ITEM:
    {
        poPlayer->DelItem((UINT16)(dwParam1));
    }
    break;
    case EGMT_SET_JINGJIE_LEVEL:
    {
        CJingJie *poJingJie = poPlayer->GetJingJie();
        if(NULL != poJingJie)
        {
            poJingJie->SetJingJieLevel((UINT32)dwParam1, (UINT32)qwParam2);
        }
        else
        {
            wErrCode = ERR_GM_PARM_INFO::ID_OTHER_ERR;
        }
    }
    break;
    case EGMT_SET_JINGJIE:
    {
        poPlayer->SetJingJie((UINT32)dwParam1, CRecordMgr::EAJT_GMSET , 0, 0, 0, 0, strDesc);
    }
    break;
    case EGMT_DECCOIN:
    {
        poPlayer->DecCoin(dwParam1, CRecordMgr::EDCT_GM , 0, 0, 0, 0, strDesc);
    }
    break;
    case EGMT_DECGOLD:
    {
        poPlayer->DecGold(dwParam1, CRecordMgr::EDGT_GM , 0, 0, 0, 0, strDesc);
    }
    break;
    case EGMT_DECRECHARGERMB:
    {
        poPlayer->DecPayByGM(dwParam1);
    }
    break;
    case EGMT_ADD_MEDAL:
    {
        poPlayer->AddMedal(dwParam1, CRecordMgr::EAMT_GM , 0, 0, 0, 0, strDesc);
    }
    break;
    case EGMT_ADD_FACTION_FUNDS:
    {
        CFaction* poFaction = poPlayer->GetFaction();
        if ( NULL != poFaction)
        {
            poFaction->AddFactionFunds(dwParam1, CRecordMgr::EAFF_GM);
        }
        else
        {
            wErrCode = ERR_GM_PARM_INFO::ID_OTHER_ERR;
        }
        break;
    }
    case EGMT_ADD_DOORSTRIBUTE:
    {
        CFaction* poFaction = poPlayer->GetFaction();
        if ( NULL != poFaction)
        {
            poFaction->AddDoorsTribute(poPlayer->GetID(), dwParam1, CRecordMgr::EAT_GM);
        }
        else
        {
            wErrCode = ERR_GM_PARM_INFO::ID_FACTION_NO_OPEN;
        }
    }
    break;
    case EGMT_SET_FACTION_LEVEL:
    {
        CFaction* poFaction = poPlayer->GetFaction();
        if ( NULL != poFaction)
        {
            poFaction->SetFactionLevelByGM(dwParam1);
        }
        else
        {
            wErrCode = ERR_GM_PARM_INFO::ID_FACTION_NO_OPEN;
        }
    }
    break;
    case EGMT_ADD_COURAGE_TIMES:
        {
            CCourage* pCourage = poPlayer->GetCourage();
            if ( NULL != pCourage)
            {
                DT_COURAGE_DATA &stCourageData = pCourage->GetDT_COURAGE_DATA();
                stCourageData.dwTodayJoinTimes += dwParam1;
            }
            else
            {
                wErrCode = ERR_GM_PARM_INFO::ID_ACTIVITY_NO_START_UP;
            }
        }
        break;

    case EGMT_RESET_EQUIP_MASTER:
    {
        SHeroProp *pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(148);
        if (pstHeroProp){
            for(UINT16 i = 0; i < pstHeroProp->vecFlyMaterial.size(); i++){
                SMaterialProp &stMaterial = pstHeroProp->vecFlyMaterial[i];
                CItemMgr::Instance()->CreateItemIntoBag(poPlayer, stMaterial.wMaterialID, stMaterial.wMaterialCnt, Ret, 0, CRecordMgr::EAIT_GMCOMMAND);
            }

            for(UINT16 i = 0; i < pstHeroProp->vecCostMaterial.size(); i++){
                SMaterialProp &stMaterial = pstHeroProp->vecCostMaterial[i];
                CItemMgr::Instance()->CreateItemIntoBag(poPlayer, stMaterial.wMaterialID, stMaterial.wMaterialCnt, Ret, 0, CRecordMgr::EAIT_GMCOMMAND);
            }
        }
        
        pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(147);
        if (pstHeroProp){
            for(UINT16 i = 0; i < pstHeroProp->vecFlyMaterial.size(); i++){
                SMaterialProp &stMaterial = pstHeroProp->vecFlyMaterial[i];
                CItemMgr::Instance()->CreateItemIntoBag(poPlayer, stMaterial.wMaterialID, stMaterial.wMaterialCnt, Ret, 0, CRecordMgr::EAIT_GMCOMMAND);
            }

            for(UINT16 i = 0; i < pstHeroProp->vecCostMaterial.size(); i++){
                SMaterialProp &stMaterial = pstHeroProp->vecCostMaterial[i];
                CItemMgr::Instance()->CreateItemIntoBag(poPlayer, stMaterial.wMaterialID, stMaterial.wMaterialCnt, Ret, 0, CRecordMgr::EAIT_GMCOMMAND);
            }
        }

        break;
    }
    case EGMT_ACTIVITY_CHAT:
    {
        DT_BOSS_BATTLE_DATA stReport;
        memset(&stReport, 0, sizeof(DT_BOSS_BATTLE_DATA));

        DT_BOSS_BATTLE_ENC_DATA &stKillEnc = stReport.stBossBKillEncInfo;
        stKillEnc.dwPlayerID = 10000;
        stKillEnc.dwCoin = 100000 * 9;
        stKillEnc.dwJingJie = 10000 * 9;

        const CID2PlayerMap &players = CPlayerMgr::Instance()->GetID2PlayerMap();
        CID2PlayerMapConstItr itr = players.begin();
        if(itr == players.end())
        {
            break;
        }

        CPlayer *poPlayer = itr->second;

        strncpy(stKillEnc.aszDispName, poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName, USERNAME_LEN);

        for(BYTE i = 0; i < 3; i++)
        {
            DT_BOSS_BATTLE_ENC_DATA &stEnc = stReport.astBossBTopEncInfo[i];
            stEnc.dwPlayerID = 1000 + i;
            stEnc.dwCoin = 100000 * (MAX_BOSS_ENC_TOP_NUM - i);
            stEnc.dwJingJie = 10000 * (MAX_BOSS_ENC_TOP_NUM - i);

            string strNickName = poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName;
            strNickName.push_back('0' + i);

            strncpy(stEnc.aszDispName, strNickName.c_str(), USERNAME_LEN);
        }

        stReport.stBossBKillEncInfo.dwCoin    = 10000;
        stReport.stBossBKillEncInfo.dwJingJie = 10000;

        CBossBattleMgr::Instance()->AddChatOnActivityOver(stReport);
        CBossBattleMgr::Instance()->AddChatOnBossKilled(stReport);

        CSDDateTime stDateTime = SDNow();
        stDateTime.IncSecond(10);
        tstring strDateTime = SDTimeToString(stDateTime);

        UINT64 qwNow = SDTimeSecs();
        CBossBattleMgr::Instance()->AddChatOnActivityStart(qwNow + 10, qwNow);

        break;
    }
    case EGMT_SYS_CHAT:
    {
        CBag &oBag = poPlayer->GetBag();

        typedef std::vector<CEquip*> CEquipVec;

        CEquipVec vecEquip;

        CID2EquipShmemMap &oEquips = oBag.GetEquipMap();
        for(CEquip *poEquip = oEquips.GetFistData(); poEquip; poEquip = oEquips.GetNextData()){
            if (poEquip->GetDT_EQUIP_DATA().wLevel < dwParam1){
                poEquip->GetDT_EQUIP_DATA().wLevel = dwParam1;
            }

            SEquipProp* pstEquipProp = poEquip->GetEquipProp();
            if (pstEquipProp->bIsLegendEquip){
                poEquip->SetEnchantLevel(60);
            }
        }
        break;
    }
    case EGMT_WORLD_CHAT:
    {
        vector<CHero*> setHero;
        poPlayer->GetPlayerAllHero(setHero);
        for (vector<CHero*>::iterator itr = setHero.begin(); itr != setHero.end(); ++itr){
            CHero *poHero = *itr;
            poHero->SetLevel(dwParam1);

            poHero->SetAbsorbKindID(0);

            DT_ABSORB_DATA_LST stAbsorbDataLst;
            memset(&stAbsorbDataLst, 0, sizeof(stAbsorbDataLst));

            poHero->InitAbsorb(&stAbsorbDataLst);
        }

        break;
    }
    case EGMT_RECORD_CHART:
    {
        for(UINT16 wEquipID = 1; wEquipID <= 172; wEquipID++){
            CItemMgr::Instance()->CreateItemIntoBag(poPlayer, wEquipID, Random(1, 2), Ret, 0, CRecordMgr::EAIT_GMCOMMAND);
        }

        break;
    }
    // 发送门派活动公告
    case EGMT_SEND_FACTION_ACTIVITY_CHAT:
    {
        PKT_CLIGS_FLY_ACK stAck;
        PKT_CLIGS_OPEN_FLY_REQ stReq;

        stReq.wHeroID = poPlayer->GetCoachHeroKindID();
        CGTPktBuilder::Instance()->OnOpenFlyAck(poPlayer, stReq);

        poPlayer->Fly(poPlayer->GetCoachHeroKindID(), stAck);
        break;
    }
    // 创建机器人帮派
    case EGMT_ROBOT_FACTION:
    {
        vector<CHero*> setHero;
        poPlayer->GetPlayerAllHero(setHero);
        for (vector<CHero*>::iterator itr = setHero.begin(); itr != setHero.end(); ++itr){
            CHero *poHero = *itr;
            poHero->ResetFly();
        }

        // poPlayer->Fly(poPlayer->GetCoachHeroKindID(), stAck);
        break;
    }
    // 门派内的人聊天，用于测试显示门派图标
    case EGMT_ADD_CHAT_WITH_FACTION:
    {
        const CEquipMakePropMgr::EquipMakeMap& mapEquipMake = CEquipMakePropMgr::Instance()->GetEquipMakeCfg();
        for(CEquipMakePropMgr::EquipMakeMap::const_iterator itr = mapEquipMake.begin(); itr != mapEquipMake.end(); ++itr){
            EquipMakeBookID equipMakeBookID = itr->first;
            const SEquipMakeProp &stMakeProp = itr->second;

            UINT16 wRandom = Random(0, stMakeProp.vecMaterial.size());

            CItemMgr::Instance()->CreateItemIntoBag(poPlayer, equipMakeBookID, Random(2, 6), Ret, 0, CRecordMgr::EAIT_GMCOMMAND);


            for (UINT16 i = 0; i < stMakeProp.vecMaterial.size(); i++){
                const SMaterialProp &stMaterialProp = stMakeProp.vecMaterial[i];
                CItem* pItem = CItemMgr::Instance()->CreateItemIntoBag(poPlayer, stMaterialProp.wMaterialID, Random(0, 1), Ret, 0, CRecordMgr::EAIT_GMCOMMAND);
            }
        }

        /*
        for (UINT16 wItemID = 27000; wItemID < 27227; wItemID += Random(2, 3)){
            CItem* pItem = CItemMgr::Instance()->CreateItemIntoBag(poPlayer, wItemID, 20, Ret, 0, CRecordMgr::EAIT_GMCOMMAND);
        }
        */
        break;
    }
    // 开启跨服战
    case EGMT_GVG_NEXT_ROUND:
    {
        if(CGvGMgr::Instance())
        {
            CTimerEventMgr::Instance()->AddGvGEvent();

            GVG_STAGE eCurStage  = (GVG_STAGE)CGvGMgr::Instance()->GetCurStage();
            GVG_STAGE eNextStage = gvgutil::GetNextStage(eCurStage);

            // 检测是否已成功进入到下一轮
            BOOL bSucc = CGvGMgr::Instance()->EnterNextStage();
            if(FALSE == bSucc)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CGvGMgr::EnterNextStage fail, <current stage = %u>, <target stage = %u>"), MSG_MARK, eCurStage, eNextStage);
            }
            else
            {
                USR_INFO(_SDT("[%s: %d]: CGvGMgr::EnterNextStage success, <before stage = %u>, <current stage = %u>, interval to next stage = %s"), MSG_MARK, eCurStage, eNextStage, gvgutil::TimeToDayHourMinuteSec(CTimerEventMgr::Instance()->GetGvGEventInterval()).c_str());
            }
        }
        
        break;
    }
    // 开启跨服战
    case EGMT_GVG_STOP:
    {
        if(CGvGMgr::Instance())
        {
            CTimerEventMgr::Instance()->StopGvGEvent();
            break;
        }
    }

    default:
    {
        wErrCode = ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR;
    }
    }
    return wErrCode;
}


VOID CGMProcessor::PassAllCommonInstance(CPlayer * poPlayer)
{
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
        USR_INFO(_SDT("[%s: %d]: SetTaskCond failed!"), MSG_MARK);
        return;
    }
    //重新初始化任务信息
    if(FALSE == poMainTask->Init(&stDT_TASK_DATA, poPlayer))
    {
        USR_INFO(_SDT("[%s: %d]: task init failed!"), MSG_MARK);
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
            stDT_INSTANCE_DATA.byScore = 1;
            stDT_INSTANCE_DATA.qwLastPassTime = SDTimeSecs();
            stDT_INSTANCE_DATA.dwSuccessNum = 1;
            DT_INSTANCE_ID& stInstanceID = stDT_INSTANCE_DATA.stInstanceID;
            stInstanceID.wSceneIdx = EIT_COMMON;
            stInstanceID.byTownIdx = byTownIdx;
            stInstanceID.byInstanceIdx = byInstanceIdx;
        }
    }

    poPlayer->GetInstance().AddSceneInstanceRecord(EIT_COMMON, stInstanceLst);
    poPlayer->CkOpenFunc();

	memset(&stInstanceLst, 0, sizeof(stInstanceLst));
	stInstanceLst.wSceneIdx = EIT_OUT_BOUND;
	CIndex2SceneTownPropMapItr itr = CInstancePropMgr::Instance()->GetAllSceneInstance().find(EIT_OUT_BOUND);
	if ( itr != CInstancePropMgr::Instance()->GetAllSceneInstance().end())
	{
		for ( CIndex2TownInstancePropMapItr TownItr = itr->second->m_mapTownInstance.begin(); TownItr != itr->second->m_mapTownInstance.end(); TownItr++)
		{	
			for ( CIndex2InstancePropMapItr InstanceItr = TownItr->second->m_mapInstance.begin(); InstanceItr != TownItr->second->m_mapInstance.end(); InstanceItr++ )
			{
				DT_INSTANCE_DATA& stDT_INSTANCE_DATA = stInstanceLst.astInstanceInfo[stInstanceLst.wInstanceNum++];
				stDT_INSTANCE_DATA.byScore = 1;
				stDT_INSTANCE_DATA.qwLastPassTime = SDTimeSecs();
				stDT_INSTANCE_DATA.dwSuccessNum = 1;
				DT_INSTANCE_ID& stInstanceID = stDT_INSTANCE_DATA.stInstanceID;
				stInstanceID.wSceneIdx = EIT_OUT_BOUND;
				stInstanceID.byTownIdx = TownItr->second->byTownIndex;
				stInstanceID.byInstanceIdx = InstanceItr->second->byIndex;
				DT_UNLOCK_DATA stUnlockInfo;
                poPlayer->GetInstance().GetCurBattleInstance().bFirstPass = TRUE;
                poPlayer->GetInstance().GetCurBattleInstance().wSceneIdx = stInstanceID.wSceneIdx;
                poPlayer->GetInstance().GetCurBattleInstance().byTownIdx = stInstanceID.byTownIdx;
                poPlayer->GetInstance().GetCurBattleInstance().byInstanceIdx = stInstanceID.byInstanceIdx;

				poPlayer->GetInstance().DoUnlock( stInstanceID.wSceneIdx, stInstanceID.byTownIdx, stInstanceID.byInstanceIdx, stUnlockInfo.byUnlockBuildKindID, stUnlockInfo.awUnlockHeroID );
			}
		}
	}
	poPlayer->GetInstance().AddSceneInstanceRecord(EIT_OUT_BOUND, stInstanceLst);
}

//BOOL	CGMProcessor::OnGetEquipInfoReq(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen)
//{
// SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
// UINT32 dwTransID            = pPipeHeader->dwTransID;


// PKT_GMGS_GET_EQUIP_INFO_REQ* pReq = (PKT_GMGS_GET_EQUIP_INFO_REQ*)pBody;
// if( sizeof(PKT_GMGS_GET_EQUIP_INFO_REQ) != dwLen )
// {
//	 return FALSE;
// }

// CPlayer*			poPlayer		=	NULL;

// //针对单个用户的配置信息
// poPlayer = CPlayerMgr::Instance()->FindPlayer(pReq->dwPlayerID);
// if(NULL == poPlayer)
// {
//	 return TRUE;
// }
// ((CGSApplication*)SDGetApp())->GMMsgResponse(pReq->dwPlayerID, pReq->dwSerialNumber, 0);
// return TRUE;
//}

UINT16 CGMProcessor::InsanceBattle(UINT32 dwPlayerID, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byInstanceNum, UINT16 wBattleNum, CNGString & strRet)
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        return ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR;
    }

    wBattleNum = wBattleNum == 0 ? 1 : wBattleNum;
    wBattleNum = wBattleNum > 1000 ? 1000 : wBattleNum;
    wSceneIdx = wSceneIdx == 0 ? 1 : wSceneIdx;
    byTownIdx = byTownIdx == 0 ? 1 : byTownIdx;
    byInstanceIdx = byInstanceIdx == 0 ? 1 : byInstanceIdx;
    UINT32 dwMyTotalHP = 0;
    for(UINT8 byLoopNum = 0; byLoopNum < byInstanceNum; byLoopNum++, byInstanceIdx++)
    {
        if(byInstanceIdx > 3)
        {
            byInstanceIdx = 1;
            byTownIdx++;
        }
        strRet += "\n";
        strRet += "###########################################################";
        strRet += "\n";
        strRet += "Instance:";
        strRet += wSceneIdx;
        strRet += "-";
        strRet += (UINT16)byTownIdx;
        strRet += "-";
        strRet += (UINT16)byInstanceIdx;
        strRet += "\n";
        for(UINT8 byBattleIndex = 1; byBattleIndex <= 3; byBattleIndex++)
        {
            CMonsterGroup* poMonsterGroup = CMonsterMgr::Instance()->FindMonsterGroup(wSceneIdx, byTownIdx, byInstanceIdx, byBattleIndex);
            if(NULL == poMonsterGroup)
            {
                byTownIdx++;
                byInstanceIdx = 1;
                break;
            }
            strRet += "	monster Idx:";
            strRet += (UINT16)byBattleIndex;
            strRet +=  "\n";
            UINT16 wTotalNum = 0;
            UINT16 wSuccessNum = 0;
            UINT16 wFailedNum = 0;
            map<UINT16, UINT16> mapSuccessRound;
            map<UINT16, UINT32> mapSuccessLeftHP;
            UINT32	dwSuccessMaxLeftHP = 0;
            UINT32	dwSuccessMinLeftHP = 0;
            map<UINT8, UINT8> mapDeadHeroNum;
            for(UINT16 wIdx = 0; wIdx < wBattleNum; wIdx++)
            {
                DT_BATTLE_DATA5 stDT_BATTLE_DATA = {0};
                if(CBattleMgr::Instance()->Battle4(poPlayer, poMonsterGroup, stDT_BATTLE_DATA, FALSE))
                {
                    wTotalNum++;
                    switch(stDT_BATTLE_DATA.byBattleResult)
                    {
                    case EBT_SUCCESS:
                    case EBT_INSTANCE_PASS:
                        wSuccessNum++;
                        break;
                    case EBT_FAILED:
                        wFailedNum++;
                        break;
                    default:
                        wFailedNum++;
                    }
                    if(EBT_FAILED != stDT_BATTLE_DATA.byBattleResult)
                    {
                        mapSuccessRound[stDT_BATTLE_DATA.byBattleRoundNum]++;
                        UINT8 byTmp = 0;
                        map<UINT8, INT32> mapMyObjHP;
                        dwMyTotalHP = 0;
                        for(byTmp = 0; byTmp < stDT_BATTLE_DATA.stMyselfInfo.byObjNum; byTmp++)
                        {
                            DT_BATTLE_OBJ_DATA2& stDT_BATTLE_OBJ_DATA = stDT_BATTLE_DATA.stMyselfInfo.astBattleObjInfo[byTmp];
                            mapMyObjHP[stDT_BATTLE_OBJ_DATA.byFomationIdx] = stDT_BATTLE_OBJ_DATA.stBattleAttr.dwHP;
                            dwMyTotalHP += stDT_BATTLE_OBJ_DATA.stBattleAttr.dwHP;
                        }
                        for(byTmp = 0; byTmp < stDT_BATTLE_DATA.byBattleRoundNum; byTmp++)
                        {
                            DT_BATTLE_ROUND_DATA5& stDT_BATTLE_ROUND_DATA = stDT_BATTLE_DATA.astBattleRoundInfo[byTmp];
                            if(1 == stDT_BATTLE_ROUND_DATA.byAttackerFlag)
                            {
                                DT_ATTACK_DATA3& stDT_ATTACK_DATA1 = stDT_BATTLE_ROUND_DATA.stAttackData.stBeforeAttack;
                                for(UINT8 byTmp2 = 0; byTmp2 < stDT_ATTACK_DATA1.stMyselfObjInfo.byHpChangeNum; byTmp2++)
                                {
                                    DT_HP_DATA2& stDT_HP_DATA = stDT_ATTACK_DATA1.stMyselfObjInfo.astHpChangeInfo[byTmp2];
                                    mapMyObjHP[stDT_HP_DATA.byFormationIdx] += stDT_HP_DATA.nHP;
                                }
                                DT_ATTACK_DATA3& stDT_ATTACK_DATA2 = stDT_BATTLE_ROUND_DATA.stAttackData.stAfterAttack;
                                for(UINT8 byTmp2 = 0; byTmp2 < stDT_ATTACK_DATA2.stMyselfObjInfo.byHpChangeNum; byTmp2++)
                                {
                                    DT_HP_DATA2& stDT_HP_DATA = stDT_ATTACK_DATA2.stMyselfObjInfo.astHpChangeInfo[byTmp2];
                                    mapMyObjHP[stDT_HP_DATA.byFormationIdx] += stDT_HP_DATA.nHP;
                                }
                            }
                        }
                        UINT8 byDeadNum = 0;
                        UINT32 dwLeftTotalHP = 0;
                        for(map<UINT8, INT32>::iterator itr = mapMyObjHP.begin(); itr != mapMyObjHP.end(); itr++)
                        {
                            if(itr->second <= 0)
                            {
                                byDeadNum++;
                            }
                            else
                            {
                                dwLeftTotalHP += itr->second;
                            }
                        }
                        if(0 != byDeadNum)
                        {
                            mapDeadHeroNum[byDeadNum]++;
                        }
                        if(0 != dwLeftTotalHP)
                        {
                            mapSuccessLeftHP[wIdx] = dwLeftTotalHP;
                        }
                    }

                }
            }
            strRet += "		fail	num:";
            strRet += wFailedNum;
            strRet +=  "\n";
            strRet += "		success num:";
            strRet += wSuccessNum;
            UINT16 wTmp = 0;
            for(map<UINT16, UINT16>::iterator itr = mapSuccessRound.begin(); itr != mapSuccessRound.end(); itr++, wTmp++)
            {
                if(0 == wTmp % 3)
                {
                    strRet += "\n";
                    strRet += "			";
                    strRet +=  itr->first;
                    strRet += " round:";
                    strRet += itr->second;
                    strRet += "		";
                }
                else
                {
                    strRet += itr->first;
                    strRet += " round:";
                    strRet +=  itr->second;
                    strRet +=  "		";
                }
            }
            strRet += "\n";
            if(mapDeadHeroNum.size() > 0)
            {
                strRet += "			";
            }
            for(map<UINT8, UINT8>::iterator itr = mapDeadHeroNum.begin(); itr != mapDeadHeroNum.end(); itr++)
            {
                strRet += "dead ";
                strRet +=  (UINT16)(itr->first);
                strRet +=  " num:";
                strRet +=  (UINT16)(itr->second);
                strRet += "		";
            }
            if(mapDeadHeroNum.size() > 0)
            {
                strRet += "\n";
            }
            if(mapSuccessLeftHP.size() > 0)
            {
                UINT64 qwAllHP = 0;
                dwSuccessMinLeftHP = dwMyTotalHP;
                for(map<UINT16, UINT32>::iterator itr = mapSuccessLeftHP.begin(); itr != mapSuccessLeftHP.end(); itr++)
                {
                    qwAllHP += itr->second;
                    dwSuccessMaxLeftHP = dwSuccessMaxLeftHP < itr->second ? itr->second : dwSuccessMaxLeftHP;
                    dwSuccessMinLeftHP = dwSuccessMinLeftHP > itr->second ? itr->second : dwSuccessMinLeftHP;
                }
                UINT32 dwAverageHP = (UINT32)(qwAllHP / mapSuccessLeftHP.size());
                strRet += "			left max hp:%";
                strRet += (dwSuccessMaxLeftHP * 100 / dwMyTotalHP);
                strRet += "	left min hp:%";
                strRet += (dwSuccessMinLeftHP * 100 / dwMyTotalHP);
                strRet += "	left average hp:%";
                strRet += (dwAverageHP * 100 / dwMyTotalHP);
                strRet += "\n";
            }
        }
    }

    return ERR_GM_PARM_INFO::ID_SUCCESS;
}


