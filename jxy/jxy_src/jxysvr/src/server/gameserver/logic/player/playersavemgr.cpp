#include "playermgr.h"
#include "playersavemgr.h"
#include <net/cli/user.h>
#include <net/db/dbpktbuilder.h>
#include <framework/gsconfig.h>
#include <framework/gsapi.h>
#include <logic/activity/bossbattle.h>
#include <logic/activity/bossbbattlelog.h>
#include <logic/plunder/plunderbattlelog.h>
#include <logic/plunder/plundercapture.h>
#include <logic/plunder/plunder.h>
#include <logic/task/taskmgr.h>
#include <logic/activity/eatbread.h>
#include <logic/activity/courage.h>
#include <logic/equipmaster/equipmaster.h>
#include <logic/build/build.h>
#include <logic/crop/crop.h>
#include <logic/race/race.h>
#include <logic/hero/hero.h>
#include "logic/other/rdchallenge.h"
#include "logic/other/worship.h"
#include "logic/moneytree/moneytree.h"
#include "logic/jingjie/jingjie.h"
#include "logic/other/phystrength.h"
#include "logic/science/sciencetree.h"
#include "logic/formation/formation.h"
#include "logic/yabiao/yabiaoinfo.h"
#include "logic/climbtower/climbtower.h"
#include "logic/activity/luaactivity.h"
#include "logic/vip/vipextdata.h"
#include "logic/skill/collectgas.h"
#include "logic/activityencourage/activityencourage.h"
#include "logic/score/score.h"
#include "logic/other/equipcompose.h"
#include "logic/other/gatherscience.h"
#include "logic/begbluegas/begblue.h"
#include "logic/other/locale.h"
#include "logic/other/state.h"
#include "logic/other/fewparam.h"
#include "logic/onhook/onhook.h"
#include "logic/awaken/awaken.h"
#include <logic/encourage/onlineencouragerecord.h>
#include <logic/encourage/loginencouragerecord.h>
#include "logic/gvg/playergvg.h"
#include "db/saveplayerdatacmd.h"
#include "logic/soul/playersoul.h"
#include "logic/medal/playermedal.h"
#include "logic/pet/playerpet.h"

IMPLEMENT_SINGLETON(CPlayerSaveMgr)

CPlayerSaveMgr::CPlayerSaveMgr():
    m_oPlayerSaveDataPool(MAX_PLAYERSAVEDATA_POOL_NUM),
    m_oPlunderSaveDataPool(MAX_PLAYERSAVEDATA_POOL_NUM),
    m_oPlayersPlunderSaveDataPool(MAX_PLAYERSAVEDATA_POOL_NUM),
    m_oRaceSaveDataPool(MAX_PLAYERSAVEDATA_POOL_NUM),
    m_oPlayersRaceSaveDataPool(MAX_PLAYERSAVEDATA_POOL_NUM)
{
    m_mapPlunderChangePlayer.clear();
    m_mapRaceChangePlayer.clear();
}

CPlayerSaveMgr::~CPlayerSaveMgr()
{
}


BOOL CPlayerSaveMgr::Init()
{
    m_dwPvpSaveNum = CGSConfig::Instance()->GetPvpSaveNum();
    return TRUE;
}


VOID CPlayerSaveMgr::UnInit()
{
    for(CID2PlayerSaveDataMapItr itr = m_mapID2PlayerSaveData.begin(); itr != m_mapID2PlayerSaveData.end(); itr++)
    {
        m_oPlayerSaveDataPool.Free(itr->second);
    }
    m_mapID2PlayerSaveData.clear();

}

BOOL CPlayerSaveMgr::SavePlayerNoPvPData(CPlayer* poPlayer, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData)
{
    memset(&stPlayerNoPvpData, 0, sizeof(stPlayerNoPvpData));

    if (0 == poPlayer->GetDT_PLAYER_BASE_DATA().dwID)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CPlayerSaveMgr::SavePlayerNoPvPData error, save player failed ,detecting playerid = %u!"), MSG_MARK, poPlayer->GetID());
        return FALSE;
    }

    //保存玩家基本数据
    if(!GetPlayerBaseSaveData(poPlayer, stPlayerNoPvpData.stBaseData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetPlayerBaseSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //保存玩家消费记录数据
    if(!GetPlayerTodayConsumeData(poPlayer, stPlayerNoPvpData.stTodayConsume))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetPlayerTodayConsumeData failed!"), MSG_MARK);
        return FALSE;
    }

    //保存玩家充值推送数据
    if(!GetExchangeResultLog(poPlayer, stPlayerNoPvpData.stExchangeResultLog))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetExchangeResultLog failed!"), MSG_MARK);
        return FALSE;
    }


    CUser* poUser = poPlayer->GetUser();
    if((NULL != poUser) && (stPlayerNoPvpData.stBaseData.dwID != poUser->GetPlayerID()))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: PlayerID not match!"), MSG_MARK);
        stPlayerNoPvpData.stBaseData.dwID = poUser->GetPlayerID();
    }

    //保存玩家武将数据
    if(!GetHeroSaveData(poPlayer, stPlayerNoPvpData.stHeroData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetHeroSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //保存玩家建筑数据
    if(!GetBuildSaveData(poPlayer, stPlayerNoPvpData.stBuildData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBuildSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //保存玩家副本记录信息
    if(!GetInstanceRecordSaveData(poPlayer, stPlayerNoPvpData.stInstanceData.stCommonInstanceData, stPlayerNoPvpData.stInstanceData.stEliteInstanceData, stPlayerNoPvpData.stInstanceData.stClimbTowerInstanceData, stPlayerNoPvpData.stInstanceData.stOutBoundInstanceData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetInstanceRecordSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //保存玩家背包道具数据
    if(!GetBagItemSaveData(poPlayer, stPlayerNoPvpData.stBagEquipData, stPlayerNoPvpData.stBagGoodsData, stPlayerNoPvpData.stBagJewelryInfo))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBagItemSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //保存玩家任务数据
    if(!GetTaskSaveData(poPlayer, stPlayerNoPvpData.stTaskData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetTaskSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //保存玩家位置信息
    if(!GetLocaleSaveData(poPlayer, stPlayerNoPvpData.stLocaleData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetRaceSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //保存玩家状态信息
    if(!GetStateSaveData(poPlayer, stPlayerNoPvpData.stCurState))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetStateSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //保存在线奖励信息
    if(!GetOnlineEncRecd(poPlayer, stPlayerNoPvpData.stOnlineEncRecdData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetOnlineEncRecd failed!"), MSG_MARK);
        return FALSE;
    }

    //保存登录奖励信息
    if(!GetLoginEncRecd(poPlayer, stPlayerNoPvpData.stLoginEncRecdData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetLoginEncRecd failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存挂机信息
    if(!GetOnhookRecd(poPlayer, stPlayerNoPvpData.stOnhookRcdData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetOnhookRecd failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存摇钱树信息
    if(!GetMoneyTree(poPlayer, stPlayerNoPvpData.stMoneyTreeData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetMoneyTree failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存最大战斗记录信息
    if(!GetMaxBattleRecord(poPlayer, stPlayerNoPvpData.stInstanceData.stCommonBattleRcd, stPlayerNoPvpData.stInstanceData.stEliteBattleRcd, stPlayerNoPvpData.stInstanceData.stClimbTowerBattleRcd))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetMaxBattleRecord failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存境界信息
    if(!GetJingJieSaveData(poPlayer, stPlayerNoPvpData.stJingJieData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetJingJieSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存的种值信息
    if(!GetCropSaveData(poPlayer, stPlayerNoPvpData.stCropData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetCropSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存的科技信息
    if(!GetScienceTreeSaveData( poPlayer, stPlayerNoPvpData.stScienceTree ))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: ScienceTree failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存体力信息
    if(!GetPhystrengthSaveData(poPlayer, stPlayerNoPvpData.stPhystrengthData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetPhystrengthSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存稀少参数信息
    if(!GetFewParamSaveData(poPlayer, stPlayerNoPvpData.stFewParamData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetFewParamSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存阵型信息
    if(!GetFormationSaveData(poPlayer, stPlayerNoPvpData.stFormationInfo))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetFormationSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存的押镖信息
    if(!GetYabiaoSaveData(poPlayer, stPlayerNoPvpData.stYabiaoInfo.stYabiaoBaseInfo))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetYabiaoSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存吃包子信息
    if(!GetEatBreadSaveData(poPlayer, stPlayerNoPvpData.stEatBreadInfo))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetEatBreadSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存押镖战报信息
    if(!GetYabiaoBattleLogData(poPlayer, stPlayerNoPvpData.stYabiaoInfo.stYabiaoBattleLog))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetYabiaoBattleLogData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存押镖战报信息
    if(!GetClimbTowerSaveData(poPlayer, stPlayerNoPvpData.stClimbTowerData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetClimbTowerSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存押镖战报信息
    if(!GetAwaken(poPlayer, stPlayerNoPvpData.stAwakenData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetAwaken failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存lua脚本活动记录信息
    if(!GetActivityRecordDataSaveData(poPlayer, stPlayerNoPvpData.stLuaActivityRecordData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetActivityRecordDataSaveData failed!"), MSG_MARK);
        return FALSE;
    }

    if(!GetVipExtData( poPlayer, stPlayerNoPvpData.stVipExtData ))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetVipExtData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存集气信息
    if(!GetCollectGas( poPlayer, stPlayerNoPvpData.stGasCollectData ))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetCollectGas failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存boss战基本信息
    if(!GetBossBBase( poPlayer, stPlayerNoPvpData.stBossBattlePlayerDataInfo.stBossBattleData ))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBossBBase failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存boss战战报信息
    if(!GetBossBBLData( poPlayer, stPlayerNoPvpData.stBossBattlePlayerDataInfo.stBossBBLData ))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBossBBLData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存活动奖励信息
    if (!GetActivityEncourageData( poPlayer, stPlayerNoPvpData.stActivityEncourageData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetActivityEncourageData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存随机挑战信息
    if (!GetRdChallengeData( poPlayer, stPlayerNoPvpData.stRdChallengeData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetRdChallengeData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存膜拜信息
    if (!GetWorshipData( poPlayer, stPlayerNoPvpData.stWorshipData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetWorshipData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存评分信息
    if (!GetScoreData( poPlayer, stPlayerNoPvpData.stScoreData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetScoreData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存装备合成信息
    if (!GetEquipCompose( poPlayer, stPlayerNoPvpData.stEquipComposeData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetEquipCompose failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存聚魂信息
    if (!GetGatherScience( poPlayer, stPlayerNoPvpData.stGatherScienceData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetEquipCompose failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存求仙露信息
    if (!GetBegBlue( poPlayer, stPlayerNoPvpData.stBegBlueData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBegBlue failed!"), MSG_MARK);
        return FALSE;
    }
    //获取要保存的好友基本信息
    if (!GetFriendsBaseData( poPlayer, stPlayerNoPvpData.stFriendsData.stFriendsBaseData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetFriendsBaseData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存玩家的好友信息
    if (!GetRelationDataList( poPlayer, stPlayerNoPvpData.stFriendsData.stRelationDataList))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetRelationDataList failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存玩家的好友信息
    if (!GetDataEvenList( poPlayer, stPlayerNoPvpData.stFriendsData.stDataEvenList))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetDataEvenList failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存玩家的好友信息
    if (!GetRecvFlowerLogList( poPlayer, stPlayerNoPvpData.stFriendsData.stRecvFlowerLogList))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetDataEvenList failed!"), MSG_MARK);
        return FALSE;
    }

    //获取要保存玩家的装备精通信息
    if (!GetEquipMaster( *poPlayer, stPlayerNoPvpData.stEquipMasterData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetEquipMaster failed!"), MSG_MARK);
        return FALSE;
    }
    //获取勇气试炼信息
    if (!GetCourageData( poPlayer, stPlayerNoPvpData.stCourageData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetCourageData failed!"), MSG_MARK);
        return FALSE;
    }

    //获取珍品阁信息
    if (!GetFactionPlayer(poPlayer, stPlayerNoPvpData.stPlayerFactionData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetFactionPlayer failed!"), MSG_MARK);
        return FALSE;
    }

    if ( !GetPlayerGVG(poPlayer, stPlayerNoPvpData.stGvGData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetPlayerGVG failed!"), MSG_MARK);
        return FALSE;
    }

	if ( !GetPlayerSoul(poPlayer, stPlayerNoPvpData.stSoulData))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: GetPlayerSoul failed!"), MSG_MARK);
		return FALSE;
	}

	if ( !GetPlayerMedal(poPlayer, stPlayerNoPvpData.stMedalData))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: GetPlayerSoul failed!"), MSG_MARK);
		return FALSE;
	}

	if (!GetPlayerPet(poPlayer, stPlayerNoPvpData.stPetData)) {
		SYS_CRITICAL(_SDT("[%s: %d]: GetPlayerPet failed!"), MSG_MARK);
		return FALSE;
	}

    //保存上次处理的订单号
    _SDTStrcpy( stPlayerNoPvpData.szLastOrderID, poPlayer->GetLastOrderID() );

    //保存最后登录登出时间
    stPlayerNoPvpData.qwLoginTime = poPlayer->GetLoginTime();
    stPlayerNoPvpData.qwLogoffTime = poPlayer->GetLogoffTime();

    return TRUE;
}

BOOL CPlayerSaveMgr::SavePlayerPvPData(CPlayer* poPlayer, DT_PLAYER_PVP_DATA& stPlayerPvpData)
{

    memset(&stPlayerPvpData, 0, sizeof(stPlayerPvpData));

    if (0 == poPlayer->GetDT_PLAYER_BASE_DATA().dwID)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CPlayerSaveMgr::SavePlayerPvPData error, save player failed ,detecting playerid = %u!"), MSG_MARK, poPlayer->GetID());
        return FALSE;
    }

    if(!GetPlunderBase(poPlayer, stPlayerPvpData.stPlunderData.stPlunderBaseData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetPlunderBase failed!"), MSG_MARK);
        return FALSE;
    }
    if(!GetPlunderBattleLog(poPlayer, stPlayerPvpData.stPlunderData.stPlunderBattleLog))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetPlunderBattleLog failed!"), MSG_MARK);
        return FALSE;
    }
    if(!GetPlunderCapture(poPlayer, stPlayerPvpData.stPlunderData.stCaptureData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetPlunderCapture failed!"), MSG_MARK);
        return FALSE;
    }
    if(!GetEnemy(poPlayer, stPlayerPvpData.stEnemyData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetEnemy failed!"), MSG_MARK);
        return FALSE;
    }
    if(!GetRaceSaveData(poPlayer, stPlayerPvpData.stRaceData.stRaceBaseData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetRaceSaveData failed!"), MSG_MARK);
        return FALSE;
    }
    if(!GetRaceBattleLog(poPlayer, stPlayerPvpData.stRaceData.stRaceBattleLog))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetRaceBattleLog failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

BOOL CPlayerSaveMgr::SavePlayerData(CPlayer* poPlayer, ESaveDBType type, DT_PLAYER_DATA * pstDT_PLAYER_DATA)
{	
	//检测玩家ID是否正常玩家，过虑非正常玩家数据，有点相当于初始化的作用
	if (0 == poPlayer->GetDT_PLAYER_BASE_DATA().dwID)
	{
        SYS_CRITICAL(_SDT("[%s: %d]: error, save player failed ,detecting playerid = %u!"), MSG_MARK, poPlayer->GetID());
		return FALSE;
	}

    if (NULL == pstDT_PLAYER_DATA)
    {
        UINT32 dwTransID = poPlayer->GetDT_PLAYER_BASE_DATA().dwID;//两者相同，非登录玩家没有CUser

        CPlayerSaveData* poPlayerSaveData = m_oPlayerSaveDataPool.Alloc();
        if(NULL == poPlayerSaveData)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CPlayerSaveMgr::SavePlayerData Alloc CPlayerSaveData failed, PlayerID = %u!"), MSG_MARK, poPlayer->GetID());
            return FALSE;
        }
        
        DT_PLAYER_NOPVP_DATA& stPlayerNoPvPData = poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData;
        BOOL bSucc = SavePlayerNoPvPData(poPlayer, stPlayerNoPvPData);
        if(!bSucc)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CPlayerSaveMgr::SavePlayerData SavePlayerNoPvPData failed!"), MSG_MARK);
            m_oPlayerSaveDataPool.Free( poPlayerSaveData );
            return FALSE;
        }

        //如果数据毫无任何变动，则不保存
        const DT_PLAYER_NOPVP_DATA *pLastData = gsapi::GetDBMgr()->GetLastData(poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID );
        if( pLastData && memcmp(pLastData, &stPlayerNoPvPData, sizeof( DT_PLAYER_NOPVP_DATA )) == 0 )
        {
            //DBG_INFO( "玩家数据没有任何变动" );
            m_oPlayerSaveDataPool.Free( poPlayerSaveData );
            return FALSE;
        }

        //保存
        CDBPktBuilder::Instance()->SavePlayerDataReq( dwTransID, poPlayerSaveData, type, poPlayer->IsAddOnlineTime() );
        if( ESDBT_GAME == type )
        {
            poPlayer->SetSaveState(ESS_SAVEING);
        }
    }
    else
    {
        DT_PLAYER_NOPVP_DATA& stPlayerNoPvPData = pstDT_PLAYER_DATA->stPlayerNoPvpData;

        //对于要同步的数据，是要传输PVP数据
        DT_PLAYER_PVP_DATA& stPlayerPvpData = pstDT_PLAYER_DATA->stPlayerPvpData;

        BOOL bSucc = SavePlayerNoPvPData(poPlayer, stPlayerNoPvPData);
        if(!bSucc)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CPlayerSaveMgr::SavePlayerData SavePlayerNoPvPData failed!"), MSG_MARK);
            return FALSE;
        }

        bSucc = SavePlayerPvPData(poPlayer, stPlayerPvpData);
        if(!bSucc)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CPlayerSaveMgr::SavePlayerData SavePlayerPvPData failed!"), MSG_MARK);
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CPlayerSaveMgr::OnSavePlayerDataAck(UINT32 dwPlayerID, CPlayerSaveData* poPlayerSaveData)
{
    if(poPlayerSaveData)
    {
        //清除对象
        m_oPlayerSaveDataPool.Free(poPlayerSaveData);
    }

    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);

    //可能非登录玩家已被清除内存
    if(NULL == poPlayer)
    {
        DBG_INFO(_SDT("[%s: %d]: CPlayerSaveMgr::OnSavePlayerDataAck the player[%u] is not in memory!"), MSG_MARK, dwPlayerID);
        return TRUE;
    }

    if(ESS_SAVEING == poPlayer->GetSaveState())
    {
        poPlayer->SetSaveState(ESS_NON_SAVE);
    }

    poPlayer->GetOldCareerBeforeChange().wKindID = 0;//下次不需再保存

    return TRUE;
}

BOOL CPlayerSaveMgr::OnSavePlayerDataAck(CPlayersPlunderSaveData* poPlayersPlunderSaveData)
{
    if(NULL == poPlayersPlunderSaveData)
    {
        return FALSE;
    }
    vector<SPlunderSaveData*>& vecPlunderSaveData = poPlayersPlunderSaveData->m_vecPlunderSaveData;
    UINT32 dwSize = vecPlunderSaveData.size();
    for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        m_oPlunderSaveDataPool.Free(vecPlunderSaveData[dwIdx]);
    }
    //m_mapSavingPlayersPlunder.erase(poPlayersPlunderSaveData);
    m_oPlayersPlunderSaveDataPool.Free(poPlayersPlunderSaveData);

    return TRUE;
}

BOOL CPlayerSaveMgr::OnSavePlayerDataAck(CPlayersRaceSaveData* poPlayersRaceSaveData)
{
    if(NULL == poPlayersRaceSaveData)
    {
        return FALSE;
    }
    vector<SRaceSaveData*>& vecRaceSaveData = poPlayersRaceSaveData->m_vecRaceSaveData;
    UINT32 dwSize = vecRaceSaveData.size();
    for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        m_oRaceSaveDataPool.Free(vecRaceSaveData[dwIdx]);
    }
    //m_mapSavingPlayersRace.erase(poPlayersRaceSaveData);
    m_oPlayersRaceSaveDataPool.Free(poPlayersRaceSaveData);

    return TRUE;
}

BOOL CPlayerSaveMgr::GetPlayerBaseSaveData(CPlayer* poPlayer, DT_PLAYER_BASE_DATA& stPlayerBaseData)
{

    memcpy(&stPlayerBaseData, &(poPlayer->GetDT_PLAYER_BASE_DATA()), sizeof(DT_PLAYER_BASE_DATA));
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(poAwaken)
    {
        stPlayerBaseData.qwStudyExp = poAwaken->GetDT_AWAKEN_DATA().stBaseInfo.qwStudyExp;
    }
    CRace* poRace = poPlayer->GetRace();
    if(poRace)
    {
        stPlayerBaseData.qwRecvRankEncTime = poRace->GetDT_RACE_BASE_DATA().qwRecvEncourageTime;
    }

    return TRUE;
}

BOOL	CPlayerSaveMgr::GetExchangeResultLog (CPlayer* poPlayer, DT_EXCHANGE_RESULT_LOG& stDT_EXCHANGE_RESULT_LOG)//获取要保存玩家基本信息
{
	memcpy(&stDT_EXCHANGE_RESULT_LOG, &(poPlayer->GetDT_EXCHANGE_RESULT_LOG()), sizeof(DT_EXCHANGE_RESULT_LOG));
	return TRUE;
}

BOOL	CPlayerSaveMgr::GetPlayerTodayConsumeData(CPlayer* poPlayer, DT_PLAYER_TODAY_CONSUME_DATA &stDT_PLAYER_TODAY_CONSUME_DATA)
{
    memcpy(&stDT_PLAYER_TODAY_CONSUME_DATA, &(poPlayer->GetDT_PLAYER_TODAY_CONSUME_DATA()), sizeof(DT_PLAYER_TODAY_CONSUME_DATA));
    return TRUE;
}

BOOL CPlayerSaveMgr::GetHeroSaveData(CPlayer* poPlayer, DT_HERO_DATA_LIST& stHeroData)
{
    UINT16 wHeroNum = 0;
    CHero* poHero = poPlayer->m_mapKind2Hero.GetFistData();
    while(poHero && wHeroNum < MAX_HERO_NUM)
    {
        DT_HERO_DATA& stDT_HERO_DATA = stHeroData.astHeroInfoLst[wHeroNum];
		DT_JEWELRY_DATA_LST& stDT_JEWELRY_DATA_LST = stHeroData.astjewelryInfoLst[wHeroNum];
		DT_HERO_SOUL_DATA& rSoulData = stHeroData.astSoulInfoLst[wHeroNum];
        memcpy(&stDT_HERO_DATA.stHeroBaseData, &(poHero->GetDT_HERO_BASE_DATA()), sizeof(DT_HERO_BASE_DATA));

        //装备
        CEquipMap& mapEquip = poHero->GetEquipMap();
        UINT16 wEquipNum = 0;
        CEquip* poEquip = mapEquip.GetFistData();
        while(poEquip && wEquipNum < EQUIP_POS_NUM)
        {
            memcpy(&stDT_HERO_DATA.stEquipDataInfo.astEquipList[wEquipNum], &(poEquip->GetDT_EQUIP_DATA()), sizeof(DT_EQUIP_DATA));
            stDT_HERO_DATA.stEquipDataInfo.awEnchantLevel[wEquipNum] = poEquip->GetEnchantLevel();

            wEquipNum++;
            poEquip = mapEquip.GetNextData();
        }
        stDT_HERO_DATA.stEquipDataInfo.wEquipNum = wEquipNum;

        //器魂
        DT_GODWEAPON_DATA& stDT_GODWEAPON_DATA = stDT_HERO_DATA.stGodweaponInfo;
        stDT_GODWEAPON_DATA.byHaveDataFlag = 0;//不需保存
        CGodWeapon* poGodWeapon = poHero->GetGodweapon();
        if(poGodWeapon)
        {
            stDT_GODWEAPON_DATA.byHaveDataFlag = 1;//保存
            memcpy(&stDT_GODWEAPON_DATA, &(poGodWeapon->GetDT_GODWEAPON_DATA()), sizeof(DT_GODWEAPON_DATA));
        }

        //丹药
        CLevel2DrugMap& mapDrug = poHero->GetDrugMap();
        UINT8 byDrugLevelNum = 0;
        CDrug* poDrug = mapDrug.GetFistData();
        while(poDrug && byDrugLevelNum < MAX_DRUG_LEVEL)
        {
            memcpy(&(stDT_HERO_DATA.stDrugDataInfo.astDrugList[byDrugLevelNum]), &(poDrug->GetDT_DRUG_DATA()), sizeof(DT_DRUG_DATA));

            byDrugLevelNum++;
            poDrug = mapDrug.GetNextData();
        }
        stDT_HERO_DATA.stDrugDataInfo.byDrugLevelNum = byDrugLevelNum;

        //技能
        poHero->GetHeroSkill(stDT_HERO_DATA.stSkillInfo);

        //心得
        memcpy(&stDT_HERO_DATA.stStudyInfo, poHero->GetStudyInfo(), sizeof(stDT_HERO_DATA.stStudyInfo));

        //伟承
        memcpy(&stDT_HERO_DATA.stAbsorbInfo, &poHero->GetDT_ABSORB_DATA_LST(), sizeof(stDT_HERO_DATA.stAbsorbInfo));

		//饰品
		poHero->GetJeweryData(stDT_JEWELRY_DATA_LST);

		// 战魂
		memcpy(&rSoulData, &poHero->GetHeroSoulData(), sizeof(DT_HERO_SOUL_DATA));

        poHero = poPlayer->m_mapKind2Hero.GetNextData();
        wHeroNum++;
    }
    stHeroData.wHeroNum = wHeroNum;

    //若转职业则修改职业信息
    DT_HERO_BASE_DATA& stDT_HERO_BASE_DATA = poPlayer->GetOldCareerBeforeChange();
    if(0 != stDT_HERO_BASE_DATA.wKindID && stHeroData.wHeroNum < MAX_HERO_NUM)
    {
        DT_HERO_DATA& stDT_HERO_DATA = stHeroData.astHeroInfoLst[stHeroData.wHeroNum++];
        memcpy(&stDT_HERO_DATA.stHeroBaseData, &stDT_HERO_BASE_DATA, sizeof(stDT_HERO_BASE_DATA));
    }


    return TRUE;
}


BOOL CPlayerSaveMgr::GetBuildSaveData(CPlayer* poPlayer, DT_BUILD_DATA_LIST& stBuildData)
{
    CBuild* poBuild = poPlayer->m_mapKind2Build.GetFistData();
    while(poBuild && stBuildData.wBuildNum < MAX_BUILD_NUM)
    {
        memcpy(&stBuildData.astBuildInfoLst[stBuildData.wBuildNum++], &(poBuild->GetDT_BUILD_BASE_DATA()), sizeof(DT_BUILD_BASE_DATA));
        poBuild = poPlayer->m_mapKind2Build.GetNextData();
    }

    return TRUE;
}

BOOL CPlayerSaveMgr::GetInstanceRecordSaveData(CPlayer* poPlayer, DT_INSTANCE_DATA_LIST& stCommonInstanceData, DT_INSTANCE_DATA_LIST& stEliteInstanceData, 
	DT_INSTANCE_DATA_LIST& stClimbTowerInstanceData, DT_INSTANCE_DATA_LIST& stOutBoundInstanceData)
{

    stCommonInstanceData.wSceneIdx = EIT_COMMON;
    stEliteInstanceData.wSceneIdx = EIT_ELITE;
    stClimbTowerInstanceData.wSceneIdx = EIT_TOWER;
	stOutBoundInstanceData.wSceneIdx = EIT_OUT_BOUND;
    CInstanceRecord* poInstanceRecord = NULL;

    //可在普通副本信息
	CInstanceRecordShmemMap& mapCommonInstanceRecord = poPlayer->GetInstance().GetCommonInstanceRecord();
	poInstanceRecord = mapCommonInstanceRecord.GetFistData();
	while(poInstanceRecord && stCommonInstanceData.wInstanceNum < MAX_INSTANCE_PER_SCENE_NUM)
    {
        memcpy(&(stCommonInstanceData.astInstanceInfo[stCommonInstanceData.wInstanceNum++]), &(poInstanceRecord->GetDT_INSTANCE_DATA()), sizeof(DT_INSTANCE_DATA));

        poInstanceRecord = mapCommonInstanceRecord.GetNextData();
    }

    //可在精武副本信息
    CInstanceRecordShmemMap& mapEliteInstanceRecord = poPlayer->GetInstance().GetEliteInstanceRecord();
    poInstanceRecord = mapEliteInstanceRecord.GetFistData();
    while(poInstanceRecord && stEliteInstanceData.wInstanceNum < MAX_INSTANCE_PER_SCENE_NUM)
    {
        memcpy(&(stEliteInstanceData.astInstanceInfo[stEliteInstanceData.wInstanceNum++]), &(poInstanceRecord->GetDT_INSTANCE_DATA()), sizeof(DT_INSTANCE_DATA));

        poInstanceRecord = mapEliteInstanceRecord.GetNextData();
    }

    //保存塔副本信息
    CInstanceRecordShmemMap& mapClimbTowerInstanceRecord = poPlayer->GetInstance().GetClimbTowerInstanceRecord();
    poInstanceRecord = mapClimbTowerInstanceRecord.GetFistData();
    while(poInstanceRecord && stClimbTowerInstanceData.wInstanceNum < MAX_INSTANCE_PER_SCENE_NUM)
    {
        memcpy(&(stClimbTowerInstanceData.astInstanceInfo[stClimbTowerInstanceData.wInstanceNum++]), &(poInstanceRecord->GetDT_INSTANCE_DATA()), sizeof(DT_INSTANCE_DATA));

        poInstanceRecord = mapClimbTowerInstanceRecord.GetNextData();
    }

	//保存外传副本信息
	CInstanceRecordShmemMap& mapOutBoundInstanceRecord = poPlayer->GetInstance().GetOutBoundInstanceRecord();
	poInstanceRecord = mapOutBoundInstanceRecord.GetFistData();
	while(poInstanceRecord && stOutBoundInstanceData.wInstanceNum < MAX_INSTANCE_PER_SCENE_NUM)
	{
		memcpy(&(stOutBoundInstanceData.astInstanceInfo[stOutBoundInstanceData.wInstanceNum++]), &(poInstanceRecord->GetDT_INSTANCE_DATA()), sizeof(DT_INSTANCE_DATA));

		poInstanceRecord = mapOutBoundInstanceRecord.GetNextData();
	}
    return TRUE;
}

BOOL CPlayerSaveMgr::GetBagItemSaveData(CPlayer* poPlayer, DT_BAG_EQUIP_DATA_LST& stBagEquipData, DT_BAG_GOODS_DATA_LST& stBagGoodsData, DT_BAG_JEWELRY_DATA_LST& stBagJewelryData)
{
	UINT16 wBagItemNum = 0;
    CID2ItemShmemMap& mapItem = poPlayer->GetBag().GetItemMap();
    CItem* poItem = mapItem.GetFistData();
    do
    {
        if(NULL == poItem)
        {
            break;
        }

        switch(poItem->GetItemKind())
        {
        case EIK_EQUIP:
        {
            CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
            if(NULL == poEquip)
            {
                continue;
            }
            if (wBagItemNum < MAX_BAG_OPEN_NUM_NEW)
            {
                DT_EQUIP_DATA &stEquip = stBagEquipData.astEquipList[stBagEquipData.wEquipNum];

                stEquip = poEquip->GetDT_EQUIP_DATA();
                stBagEquipData.awEnchantLevel[stBagEquipData.wEquipNum] = poEquip->GetEnchantLevel();

                stBagEquipData.wEquipNum++;
				wBagItemNum++;
            }
            else
            {
                SYS_CRITICAL( _SDT( "@ Bag Equip Full PlayerID:%u !" ), poPlayer->GetID() );
            }
        }
        break;
        case EIK_GOODS:
        {
            CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
            if(NULL == poGoods)
            {
                continue;
            }

            if (wBagItemNum < MAX_BAG_OPEN_NUM_NEW)
            {
                memcpy(&stBagGoodsData.astGoodsList[stBagGoodsData.wGoodsNum++], &(poGoods->GetDT_GOODS_DATA()), sizeof(DT_GOODS_DATA));
				wBagItemNum++;
            }
            else
            {
                SYS_CRITICAL( _SDT( "@ Bag Good Full PlayerID:%u !" ), poPlayer->GetID() );
            }
        }
        break;
		case EIK_JEWELRY:
		{
			CJewelry* poJewelry = dynamic_cast<CJewelry*>(poItem);
			if ( NULL == poJewelry)
			{
				continue;
			}
			if (wBagItemNum < MAX_BAG_OPEN_NUM_NEW)
			{
				memcpy(&stBagJewelryData.astJewelryInfo[stBagJewelryData.wJewelryNum++], &(poJewelry->GetDT_JEWELRY_DATA()), sizeof(DT_JEWELRY_DATA));
				wBagItemNum++;
			}
			else
			{
				SYS_CRITICAL( _SDT( "@ Bag Jewelry Full PlayerID:%u !" ), poPlayer->GetID() );
			}
		}
		break;
        default:
            continue;
        }

    }
    while(poItem = mapItem.GetNextData());

    return TRUE;
}


//获取要保存任务信息
BOOL CPlayerSaveMgr::GetTaskSaveData(CPlayer* poPlayer, DT_TASK_DATA_LIST& stTaskData)
{
    UINT8 byTaskLineNum = 0;
    CLineIdx2TaskShmemMap& mapAllTask = poPlayer->GetAllTask();
    CTask* poTask = mapAllTask.GetFistData();
    while(poTask && stTaskData.byTaskNum < MAX_TASK_LINE_NUM)
    {
        memcpy(&stTaskData.astTaskInfoList[stTaskData.byTaskNum++], &(poTask->GetDT_TASK_DATA()), sizeof(DT_TASK_DATA));

        poTask = mapAllTask.GetNextData();
    }

    return TRUE;
}

//获取要保存竞技信息
BOOL CPlayerSaveMgr::GetRaceSaveData(CPlayer* poPlayer, DT_RACE_BASE_DATA& stRaceData)
{
    CRace* poRace = poPlayer->GetRace();
    if(NULL == poRace)
    {
        return TRUE;
    }

    memcpy(&stRaceData, &(poRace->GetDT_RACE_BASE_DATA()), sizeof(DT_RACE_BASE_DATA));

    return TRUE;
}

//获取要保存当前位置信息
BOOL CPlayerSaveMgr::GetLocaleSaveData(CPlayer* poPlayer, DT_LOCALE_DATA& stLocaleData)
{
    memcpy(&stLocaleData, &(poPlayer->GetLocale().GetDT_LOCALE_DATA()), sizeof(DT_LOCALE_DATA));

    return TRUE;
}

//获取要保存当前状态信息
BOOL CPlayerSaveMgr::GetStateSaveData(CPlayer* poPlayer, DT_STATE_DATA& stStateData)
{
    memcpy(&stStateData, &(poPlayer->GetState().GetDT_STATE_DATA()), sizeof(DT_STATE_DATA));

    return TRUE;
}

//获取要保存在线奖励信息
BOOL CPlayerSaveMgr::GetOnlineEncRecd(CPlayer* poPlayer, DT_ONLINE_ENCOURAGE_RECORD_DATA& stOnlineRecd)
{

    memcpy(&stOnlineRecd, &(poPlayer->GetOnlineEncourageRecord().GetDT_DATA()), sizeof(DT_ONLINE_ENCOURAGE_RECORD_DATA));
    return TRUE;
}


//获取要保存登录奖励信息
BOOL CPlayerSaveMgr::GetLoginEncRecd(CPlayer* poPlayer, DT_LOGIN_ENCOURAGE_RECORD_DATA& stLoginRecd)
{
    memcpy(&stLoginRecd, &(poPlayer->GetLoginEncourageRecord().GetDT_DATA()), sizeof(DT_LOGIN_ENCOURAGE_RECORD_DATA));
    return TRUE;
}

//获取要保存挂机信息
BOOL CPlayerSaveMgr::GetOnhookRecd(CPlayer* poPlayer, DT_ONHOOK_RCD_DATA& stOnhookRcdData)
{
	COnhook* poOnhook = poPlayer->GetOnhook();
	if (NULL == poOnhook)
	{
		return TRUE;
	}

    memcpy(&stOnhookRcdData, &(poOnhook->GetDT_ONHOOK_RCD_DATA()), sizeof(DT_ONHOOK_RCD_DATA));

    return TRUE;
}


//获取要竞技场战报信息
BOOL CPlayerSaveMgr::GetRaceBattleLog(CPlayer* poPlayer, DT_RACE_BATTLELOG_DATA_LIST& stRaceBattleLog)
{
    CRace* poRace = poPlayer->GetRace();
    if(NULL == poRace)
    {
        return TRUE;
    }
    CRaceBattleLogDeq& deqBattleLog = poRace->GetBattleLogDeq();
    for(CRaceBattleLogDeqItr itr = deqBattleLog.begin(); ((itr != deqBattleLog.end()) && (stRaceBattleLog.byBattleLogNum < MAX_RACE_BATTLELOG_NUM)); itr++)
    {
        CRaceBattleLog* poBattleLog = *itr;
        if(poBattleLog && stRaceBattleLog.byBattleLogNum < MAX_RACE_BATTLELOG_NUM)
        {
            memcpy(&stRaceBattleLog.astBattleLogInfo[stRaceBattleLog.byBattleLogNum++], &(poBattleLog->GetDT_RACE_BATTLELOG_DATA()), sizeof(DT_RACE_BATTLELOG_DATA));
        }
    }

    return TRUE;
}


//获取要保存掠夺基本信息
BOOL CPlayerSaveMgr::GetPlunderBase(CPlayer* poPlayer, DT_PLUNDER_BASE_DATA& stPlunderBaseData)
{
    CPlunder* poPlunder = poPlayer->GetPlunder();
    if(NULL == poPlunder)
    {
        return TRUE;
    }
    memcpy(&stPlunderBaseData, &(poPlunder->GetDT_PLUNDER_BASE_DATA()), sizeof(DT_PLUNDER_BASE_DATA));

    return TRUE;
}

//获取要保存掠夺战报信息
BOOL CPlayerSaveMgr::GetPlunderBattleLog(CPlayer* poPlayer, DT_PLUNDER_BATTLELOG_DATA_LIST& stPlunderBattleLog)
{
    CPlunder* poPlunder = poPlayer->GetPlunder();
    if(NULL == poPlunder)
    {
        return TRUE;
    }

    CPlunderBattleLogDeq& deqBattleLog = poPlunder->GetBattleLogDeq();
    for(CPlunderBattleLogDeqItr itr = deqBattleLog.begin(); ((itr != deqBattleLog.end()) && (stPlunderBattleLog.byBattleLogNum < MAX_PLUNDER_BATTLELOG_NUM)); itr++)
    {
        CPlunderBattleLog* poBattleLog = *itr;
        if(poBattleLog && stPlunderBattleLog.byBattleLogNum < MAX_PLUNDER_BATTLELOG_NUM)
        {
            memcpy(&stPlunderBattleLog.astBattleLogInfo[stPlunderBattleLog.byBattleLogNum++], &(poBattleLog->GetDT_PLUNDER_BATTLELOG_DATA()), sizeof(DT_PLUNDER_BATTLELOG_DATA));
        }
    }

    return TRUE;
}

//获取要保存boss战基本信息
BOOL CPlayerSaveMgr::GetBossBBase(CPlayer* poPlayer, DT_BOSS_BATTLE_PLAYER_DATA& stBossBattlePlayerData)
{
    CBossBattle* poBossBattle = poPlayer->GetBossBattle();
    if(NULL == poBossBattle)
    {
        return TRUE;
    }
    memcpy(&stBossBattlePlayerData, &(poBossBattle->GetDT_BOSS_BATTLE_PLAYER_DATA()), sizeof(DT_BOSS_BATTLE_PLAYER_DATA));

    return TRUE;
}

//获取要保存boss战战报信息
BOOL CPlayerSaveMgr::GetBossBBLData(CPlayer* poPlayer, DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA_LST& stBossBBLData)
{
    CBossBattle* poBossBattle = poPlayer->GetBossBattle();
    if(NULL == poBossBattle)
    {
        return TRUE;
    }

    CBossBBattleLogMap& mapBattleLog = poBossBattle->GetBattleLogMap();
    for(CBossBBattleLogMapItr itr = mapBattleLog.begin(); ((itr != mapBattleLog.end()) && (stBossBBLData.byNum < MAX_BOSSB_BATTLE_LOG_NUM)); itr++)
    {
        CBossBBattleLog* poBattleLog = itr->second;
        if(poBattleLog && stBossBBLData.byNum < MAX_BOSSB_BATTLE_LOG_NUM)
        {
            memcpy(&stBossBBLData.astBossBattleBL[stBossBBLData.byNum++], &(poBattleLog->GetDT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA()), sizeof(DT_BOSS_BATTLE_PLAYER_BATTLE_LOG_DATA));
        }
    }

    return TRUE;
}

//获取要保存随机挑战信息
BOOL CPlayerSaveMgr::GetRdChallengeData(CPlayer* poPlayer, DT_RANDOM_CHALLENGE_DATA& stRdChallengeData)
{
    if(NULL == poPlayer)
    {
        return FALSE;
    }
    CRdChallenge *poRdChallenge = poPlayer->GetRdChallenge();
    if (NULL == poRdChallenge)
    {
        return TRUE;
    }
    memcpy(&stRdChallengeData, &(poRdChallenge->GetDT_RANDOM_CHALLENGE_DATA()), sizeof(stRdChallengeData));

    return TRUE;
}

//获取要保存膜拜信息
BOOL CPlayerSaveMgr::GetWorshipData(CPlayer* poPlayer, DT_WORSHIP_DATA_LST& stWorshipData)
{
    if(NULL == poPlayer)
    {
        return FALSE;
    }
    CWorship *poWorship = poPlayer->GetWorship();
    if (NULL == poWorship)
    {
        return TRUE;
    }
    memcpy(&stWorshipData, &(poWorship->GetDT_WORSHIP_DATA_LST()), sizeof(stWorshipData));

    return TRUE;
}

//获取要保存俘虏信息
BOOL CPlayerSaveMgr::GetPlunderCapture(CPlayer* poPlayer, DT_CAPTURE_DATA_LIST& stCaptureData)
{
    CPlunder* poPlunder = poPlayer->GetPlunder();
    if(NULL == poPlunder)
    {
        return TRUE;
    }

    CPlayerID2CaptureMap& mapCapture = poPlunder->GetCaptureMap();
    CPlunderCapture* poCapture  = mapCapture.GetFistData();
    while(poCapture && stCaptureData.byCaptureNum < MAX_CAPTURE_NUM)
    {
        memcpy(&stCaptureData.astCaptureInfo[stCaptureData.byCaptureNum++], &(poCapture->GetDT_CAPTURE_DATA()), sizeof(DT_CAPTURE_DATA));
        poCapture  = mapCapture.GetNextData();
    }


    return TRUE;
}


//获取要保存仇敌信息
BOOL CPlayerSaveMgr::GetEnemy(CPlayer* poPlayer, DT_ENEMY_DATA_LIST& stEnemyData)
{
    CEnemy* poEnemy = NULL;
    CShmemMap<UINT32, CEnemy> &mapEnemy = poPlayer->m_mapEnemy;
    poEnemy = mapEnemy.GetFistData();
    while(poEnemy && stEnemyData.byEnemyNum < MAX_ENEMY_NUM)
    {
        memcpy(&stEnemyData.astEnemyInfo[stEnemyData.byEnemyNum++], &(poEnemy->GetDT_ENEMY_DATA()), sizeof(DT_ENEMY_DATA));

        poEnemy = mapEnemy.GetNextData();
    }

    return TRUE;
}

//获取要保存摇钱树信息
BOOL CPlayerSaveMgr::GetMoneyTree(CPlayer* poPlayer, DT_MONEY_TREE_DATA& stMoneyTreeData)
{
    CMoneyTree* poMoneyTree = poPlayer->GetMoneyTree();
    if(NULL == poMoneyTree)
    {
        return TRUE;
    }

    memcpy(&stMoneyTreeData, &(poMoneyTree->GetDT_MONEY_TREE_DATA()), sizeof(DT_MONEY_TREE_DATA));

    return TRUE;
}


//获取要保存悟道信息
BOOL CPlayerSaveMgr::GetAwaken(CPlayer* poPlayer, DT_AWAKEN_DATA& stAwaken)
{
    stAwaken.stBaseInfo.byLevel = 0; //表示没数据
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(NULL == poAwaken)
    {
        if (NULL != poPlayer->GetBuild(EBK_AWAKEN))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: GetAwaken Faild, player[%d]!"), MSG_MARK, poPlayer->GetID());
        }
        return TRUE;
    }
    memcpy(&stAwaken, &poAwaken->GetDT_AWAKEN_DATA(), sizeof(DT_AWAKEN_DATA));
    if(0 == stAwaken.stBaseInfo.byLevel)
    {
        if (NULL != poPlayer->GetBuild(EBK_AWAKEN))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: GetAwaken Faild, player[%d]!"), MSG_MARK, poPlayer->GetID());
        }
        stAwaken.stBaseInfo.byLevel = EALK_KESHUI;
    }

    return TRUE;
}


//获取要保存最大战斗记录信息
BOOL CPlayerSaveMgr::GetMaxBattleRecord(CPlayer* poPlayer, DT_INSTANCE_BATTLE_DATA& stCommonBattleRcd, DT_INSTANCE_BATTLE_DATA& stEliteBattleRcd, DT_INSTANCE_BATTLE_DATA& stClimbTowerBattleRcd)
{

    CInstanceBattleRcd& oCommonBattleRcd = poPlayer->GetInstance().GetCommonBattleRcd();
    memcpy(&stCommonBattleRcd, &(oCommonBattleRcd.GetDT_INSTANCE_BATTLE_DATA()), sizeof(DT_INSTANCE_BATTLE_DATA));

    CInstanceBattleRcd& oEliteBattleRcd = poPlayer->GetInstance().GetEliteBattleRcd();
    memcpy(&stEliteBattleRcd, &(oEliteBattleRcd.GetDT_INSTANCE_BATTLE_DATA()), sizeof(DT_INSTANCE_BATTLE_DATA));

    CInstanceBattleRcd& oClimbTowerBattleRcd = poPlayer->GetInstance().GetClimbTowerBattleRcd();
    memcpy(&stClimbTowerBattleRcd, &(oClimbTowerBattleRcd.GetDT_INSTANCE_BATTLE_DATA()), sizeof(DT_INSTANCE_BATTLE_DATA));


    return TRUE;
}

//获取要保存境界信息
BOOL CPlayerSaveMgr::GetJingJieSaveData(CPlayer* poPlayer, DT_JINGJIE_DATA& stJingJieData)
{
    CJingJie* poJingJie = poPlayer->GetJingJie();
    if(NULL == poJingJie)
    {
        return TRUE;
    }

    memcpy(&stJingJieData, &(poJingJie->GetDT_JINGJIE_DATA()), sizeof(DT_JINGJIE_DATA));

    return TRUE;
}



BOOL CPlayerSaveMgr::GetCropSaveData(CPlayer* poPlayer, DT_CROP_DATA& stDT_CROP_DATA)
{
    CCrop* poCrop = poPlayer->GetCrop();
    if(NULL == poCrop)
    {
        return TRUE;
    }
    memcpy(&stDT_CROP_DATA, &(poCrop->GetDT_CROP_DATA()), sizeof(DT_CROP_DATA));
    return TRUE;
}

//获取要保存体力信息
BOOL CPlayerSaveMgr::GetPhystrengthSaveData(CPlayer* poPlayer, DT_PHYSTRENGTH_DATA& stPhystrengthData)
{
    memcpy(&stPhystrengthData, &(poPlayer->GetPhystrength().GetDT_PHYSTRENGTH_DATA()), sizeof(DT_PHYSTRENGTH_DATA));

    return TRUE;
}

//获取要保存稀少参数信息
BOOL CPlayerSaveMgr::GetFewParamSaveData(CPlayer* poPlayer, DT_FEW_PARAM_DATA& stFewParamData)
{
    memcpy(&stFewParamData, &(poPlayer->GetFewParam().GetDT_FEW_PARAM_DATA()), sizeof(DT_FEW_PARAM_DATA));

    return TRUE;
}

//获取科技树的信息
BOOL CPlayerSaveMgr::GetScienceTreeSaveData( CPlayer *poPlayer, DT_SCIENCE_TREE_DATA &stScienceTreeData )
{
    CScienceTree* poScienceTree = poPlayer->GetScienceTree();
    if(NULL == poScienceTree)
    {
        return TRUE;
    }

    memcpy(&stScienceTreeData, &(poScienceTree->GetDT_SCIENCE_TREE_DATA()), sizeof(DT_SCIENCE_TREE_DATA));

    return TRUE;
}

//获取要保存阵型信息
BOOL CPlayerSaveMgr::GetFormationSaveData( CPlayer *poPlayer, DT_NEW_FORMATION_DATA &stFormationData )
{
    CFormation &oFormation = poPlayer->GetFormation();
    memcpy(&stFormationData, &(oFormation.GetDT_FORMATION_DATA()), sizeof(DT_NEW_FORMATION_DATA));

    return TRUE;
}


//获取要保存吃包子信息

BOOL CPlayerSaveMgr::GetYabiaoSaveData( CPlayer *poPlayer, DT_YABIAO_BASE_DATA &stYabiaoData )
{
    CYabiaoInfo* poYabiaoInfo = poPlayer->GetYabiaoInfo();
    if(NULL == poYabiaoInfo)
    {
        return TRUE;
    }

    memcpy( &stYabiaoData,  &(poYabiaoInfo->GetDT_YABIAO_BASE_DATA()), sizeof( DT_YABIAO_BASE_DATA ) );

    return TRUE;
}

//获取要保存吃包子信息
BOOL CPlayerSaveMgr::GetEatBreadSaveData( CPlayer *poPlayer, DT_EAT_BREAD_DATA &stEatBreadData )
{
    CEatBread* poEatBread = poPlayer->GetEatBread();
    if(poEatBread)
    {
        memcpy(&stEatBreadData, &(poEatBread->GetDT_EAT_BREAD_DATA()), sizeof(DT_EAT_BREAD_DATA));
    }

    return TRUE;
}

BOOL CPlayerSaveMgr::GetYabiaoBattleLogData( CPlayer *poPlayer, DT_YABIAO_BATTLELOG_DATA_LIST &stYabiaoBattleLogData )
{
    CYabiaoInfo *poYabiaoInfo = poPlayer->GetYabiaoInfo();
    if (NULL == poYabiaoInfo)
    {
        return TRUE;
    }
    const CYabiaoInfo::CYabiaoBattleLogDeq &deqData = poYabiaoInfo->GetYabiaoBattleLog();
    for( UINT32 nIndex = 0; nIndex < deqData.size() && nIndex < MAX_YABIAO_BATTLELOG_NUM; ++nIndex )
    {
        if (NULL == deqData[nIndex])
        {
            continue;
        }

        stYabiaoBattleLogData.astPaoshangBattleLogInfo[stYabiaoBattleLogData.byBattleLogNum] =
            deqData[nIndex]->GetDT_PAOSHANG_BATTLELOG_DATA();
        ++stYabiaoBattleLogData.byBattleLogNum;
    }

    return TRUE;
}


BOOL CPlayerSaveMgr::GetClimbTowerSaveData(CPlayer* poPlayer, DT_CLIMB_TOWER_DATA & stDT_CLIMB_TOWER_DATA)
{
    CClimbTower *poClimbTower = poPlayer->GetClimbTower();
    if (NULL == poClimbTower)
    {
        return TRUE;
    }

    memcpy(&stDT_CLIMB_TOWER_DATA, &(poClimbTower->GetDT_CLIMB_TOWER_DATA()), sizeof(DT_CLIMB_TOWER_DATA));
    return TRUE;
}

BOOL CPlayerSaveMgr::GetActivityRecordDataSaveData(CPlayer* poPlayer, DT_LUAACTIVITY_RECORD_LIST_DATA & stActivityRecordData)
{
    poPlayer->GetLuaActivity().GetDT_ACTIVITY_RECORD_DATA(stActivityRecordData);
    return TRUE;
}

BOOL CPlayerSaveMgr::GetVipExtData( CPlayer *poPlayer, DT_VIP_EXT_DATA &stDT_VIP_EXT_DATA )
{
    memcpy(&stDT_VIP_EXT_DATA, &(poPlayer->GetVipExtData().GetDT_VIP_EXT_DATA()), sizeof(DT_VIP_EXT_DATA));
    return TRUE;
}

BOOL CPlayerSaveMgr::GetCollectGas( CPlayer *poPlayer, DT_COLLECT_GAS_DATA &stCollectGas )
{
    CCollectGas *poCollectGas = poPlayer->GetCollectGas();
    if (NULL == poCollectGas)
    {
        return TRUE;
    }
    memcpy(&stCollectGas, &poCollectGas->GetDT_COLLECT_GAS_DATA(), sizeof(DT_COLLECT_GAS_DATA));
    return TRUE;
}


BOOL	CPlayerSaveMgr::GetActivityEncourageData( CPlayer *poPlayer, DT_ACTIVITY_ENCOURAGE_DATA &stActivityEncourage )//获取要保存活动奖励信息
{
    memcpy(&stActivityEncourage, &(poPlayer->GetActivityEncourage().GeDT_ACTIVITY_ENCOURAGE_DATA()), sizeof(DT_ACTIVITY_ENCOURAGE_DATA));
    return TRUE;
}

BOOL CPlayerSaveMgr::GetScoreData(CPlayer* poPlayer, DT_SCORE_DATA_LST& stScoreData) //获取要保存膜拜信息
{
    CScore *poScore = poPlayer->GetScore();
    if (NULL == poScore)
    {
        return TRUE;
    }

    memcpy(&stScoreData, &(poScore->GetDT_SCORE_DATA_LST()), sizeof(DT_SCORE_DATA_LST));
    return TRUE;
}

BOOL CPlayerSaveMgr::GetEquipCompose(CPlayer* poPlayer, DT_EQUIP_COMPOSE_DATA& stEquipComposeData) //获取要保存装备合成信息
{
    memcpy(&stEquipComposeData, &poPlayer->GetEquipCompse().GetDT_EQUIP_COMPOSE_DATA(), sizeof(DT_EQUIP_COMPOSE_DATA));
    return TRUE;
}

BOOL	CPlayerSaveMgr::GetGatherScience(CPlayer* poPlayer, DT_GATHER_SCIENCE_DATA &stGatherScienceData) //获取要保存聚魂信息
{
    CGatherScience *poGatherScience = poPlayer->GetGatherScience();
    if (NULL == poGatherScience)
    {
        return TRUE;
    }

    memcpy(&stGatherScienceData, &poGatherScience->GetDT_GATHER_SCIENCE_DATA(), sizeof(DT_GATHER_SCIENCE_DATA));
    return TRUE;
}

BOOL CPlayerSaveMgr::GetBegBlue(CPlayer* poPlayer, DT_BEGBLUE_DATA &stBegBlueData) //获取要保存求仙露信息
{
	CBegBlue *poBegBlue = poPlayer->GetBegBlue();
	if (NULL == poBegBlue)
	{
		return TRUE;
	}

    memcpy(&stBegBlueData, &poBegBlue->GetDT_BEGBLUE_DATA(), sizeof(DT_BEGBLUE_DATA));
    return TRUE;
}

//BOOL CPlayerSaveMgr::GetSendFlower(CPlayer* poPlayer, DT_SEND_FLOWER_DATA_LIST& stSendFlowerDataList) //获取要保存送鲜花信息
//{
//	memcpy(&stSendFlowerDataList, &poPlayer->GetSendFlower().GetDT_SEND_FLOWER_DATA_LIST(), sizeof(DT_SEND_FLOWER_DATA_LIST));
//	return TRUE;
//}

BOOL CPlayerSaveMgr::GetFriendsBaseData(CPlayer* poPlayer, DT_FRIENDS_BASE_DATA& stDT_FRIENDS_BASE_DATA) //获取要保存的送鲜花信息 
{
	CFriends *poFriends = poPlayer->GetFriends();
	if (NULL == poFriends)
	{
		return TRUE;
	}
	memcpy(&stDT_FRIENDS_BASE_DATA, &poFriends->GetDT_FRIENDS_BASE_DATA(), sizeof(DT_FRIENDS_BASE_DATA));
	return TRUE;
}

BOOL CPlayerSaveMgr::GetRelationDataList(CPlayer* poPlayer, DT_RELATION_DATA_LIST &stRelationDataList) //获取要保存的玩家朋友关系
{
	CFriends *poFriends = poPlayer->GetFriends();
	if (NULL == poFriends)
	{
		return TRUE;
	}
    poFriends->GetRelationDataList(stRelationDataList);
    return TRUE;
}

BOOL CPlayerSaveMgr::GetDataEvenList(CPlayer* poPlayer, DT_DATA_EVEN_LIST &stDataEvenList) //获取要保存的玩家朋友关系
{
	CFriends *poFriends = poPlayer->GetFriends();
	if (NULL == poFriends)
	{
		return TRUE;
	}
    poFriends->GetDataEvenList(stDataEvenList);
    return TRUE;
}


BOOL CPlayerSaveMgr::GetRecvFlowerLogList(CPlayer* poPlayer, DT_RECV_FLOWER_LOG_LIST &stRecvFlowerLogList) //获取玩家收花日志
{
	CFriends *poFriends = poPlayer->GetFriends();
	if (NULL == poFriends)
	{
		return TRUE;
	}
    poFriends->GetRecvFlowerLogList(stRecvFlowerLogList);
    return TRUE;
}

BOOL CPlayerSaveMgr::GetEquipMaster(CPlayer &oPlayer, DT_EQUIP_MASTER &stData)//获取装备精通信息
{
    if(NULL == oPlayer.GetEquipMaster())
    {
        return TRUE;
    }

    oPlayer.GetEquipMaster()->CopyTo(stData);
    return TRUE;
}
BOOL CPlayerSaveMgr::GetCourageData(CPlayer* poPlayer, DT_COURAGE_DATA &stDT_COURAGE_DATA) //获取勇气信息
{
    CCourage* poCourage = poPlayer->GetCourage();
    if(NULL == poCourage)
    {
        return TRUE;
    }
    memcpy(&stDT_COURAGE_DATA, &poCourage->GetDT_COURAGE_DATA(), sizeof(DT_COURAGE_DATA));
    return TRUE;
}
BOOL CPlayerSaveMgr::GetFactionPlayer(CPlayer* poPlayer, DT_PLAYER_FACTION_DATA& stRateItemGoodsData)
{
	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if (poFactionPlayer == NULL)
	{
		return TRUE;
	}

	memcpy(&stRateItemGoodsData, &poFactionPlayer->GetDT_PLAYER_FACTION_DATA(), sizeof(DT_PLAYER_FACTION_DATA));
	return TRUE;
}

BOOL CPlayerSaveMgr::GetPlayerGVG(CPlayer* poPlayer, DT_PLAYER_GVG_DATA& stGVGData)
{
	CPlayerGVG* poPlayerGVG = poPlayer->GetPlayerGVG();
	if (poPlayerGVG == NULL)
	{
		return TRUE;
	}

	memcpy(&stGVGData, &poPlayerGVG->GetPlayerGVGData(), sizeof(DT_PLAYER_GVG_DATA));
	return TRUE;
}

BOOL CPlayerSaveMgr::GetPlayerSoul(CPlayer* poPlayer, DT_PLAYER_SOUL_DATA& stSoulData)
{
	CPlayerSoul* poPlayerSoul = poPlayer->GetPlayerSoul();
	if (poPlayerSoul == NULL)
	{
		return TRUE;
	}

	memcpy(&stSoulData, &poPlayerSoul->GetDT_PLAYER_SOUL_DATA(), sizeof(DT_PLAYER_SOUL_DATA));
	return TRUE;
}

BOOL CPlayerSaveMgr::GetPlayerMedal(CPlayer* poPlayer, DT_PLAYER_MEDAL_DATA& stMedalData)
{
	CPlayerMedal* poPlayerMedal = poPlayer->GetPlayerMedal();
	if (poPlayerMedal == NULL)
	{
		return TRUE;
	}

	memcpy(&stMedalData, &poPlayerMedal->GetDT_PLAYER_MEDAL_DATA(), sizeof(DT_PLAYER_MEDAL_DATA));
	return TRUE;
}

BOOL CPlayerSaveMgr::GetPlayerPet(CPlayer* pPlayer, DT_PLAYER_PET_DATA& rPetData) {
	CPlayerPet* pPlayerPet = pPlayer->GetPlayerPet();
	if (NULL == pPlayerPet) {
		return TRUE;
	}

	memcpy(&rPetData, &pPlayerPet->GetDT_PLAYER_PET_DATA(), sizeof(DT_PLAYER_PET_DATA));
	return TRUE;
}

VOID CPlayerSaveMgr::AddPlayerPlunderSaveData( CPlayer* poPlayer1, CPlayer* poPlayer2, CPlayer* poPlayer3 )
{
    if(poPlayer1)
    {
        poPlayer1->SetUpdateTime();
        m_mapPlunderChangePlayer[poPlayer1->GetID()] = poPlayer1->GetID();
    }
    if(poPlayer2)
    {
        poPlayer2->SetUpdateTime();
        m_mapPlunderChangePlayer[poPlayer2->GetID()] = poPlayer2->GetID();
    }
    if(poPlayer3)
    {
        poPlayer3->SetUpdateTime();
        m_mapPlunderChangePlayer[poPlayer3->GetID()] = poPlayer3->GetID();
    }

    UINT32 dwSize = m_mapPlunderChangePlayer.size();
    if(dwSize >= m_dwPvpSaveNum)
    {
        SavePlayersPlunderData();
    }
}

VOID CPlayerSaveMgr::AddPlayerRaceSaveData(CPlayer* poPlayer1, CPlayer* poPlayer2)
{
    if(poPlayer1)
    {
        poPlayer1->SetUpdateTime();
        m_mapRaceChangePlayer[poPlayer1->GetID()] = poPlayer1->GetID();
    }
    if(poPlayer2)
    {
        poPlayer2->SetUpdateTime();
        m_mapRaceChangePlayer[poPlayer2->GetID()] = poPlayer2->GetID();
    }

    UINT32 dwSize = m_mapRaceChangePlayer.size();
    if(dwSize >= m_dwPvpSaveNum)
    {
        SavePlayersRaceData();
    }
}

VOID CPlayerSaveMgr::SavePlayersPlunderData()
{
    CPlayersPlunderSaveData* pstPlayersPlunderSaveData = m_oPlayersPlunderSaveDataPool.Alloc();
    HashMap<UINT32, UINT32>::iterator itr = m_mapPlunderChangePlayer.begin();
    for(; itr != m_mapPlunderChangePlayer.end(); itr++)
    {
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer( itr->first );
        if( NULL == poPlayer )
        {
            SYS_CRITICAL( _SDT( "player:%u not found!" ), itr->first );
            continue;
        }

        //如果是机器人，则不保存机器人信息
        if (ERFK_PLUNDER == poPlayer->GetDT_PLAYER_BASE_DATA().byRobotFunc)
        {
            continue;
        }

        SPlunderSaveData* pstPlunderSaveData = m_oPlunderSaveDataPool.Alloc();
        if (NULL == pstPlunderSaveData)
        {
            continue;
        }

        pstPlunderSaveData->dwPlayerID = poPlayer->GetDT_PLAYER_BASE_DATA().dwID;
        GetPlunderBase(poPlayer, pstPlunderSaveData->stPlunderBaseData);
        GetPlunderBattleLog(poPlayer, pstPlunderSaveData->stPlunderBattleLog);
        GetPlunderCapture(poPlayer, pstPlunderSaveData->stCaptureData);
        GetEnemy(poPlayer, pstPlunderSaveData->stEnemyData);
        pstPlayersPlunderSaveData->m_vecPlunderSaveData.push_back(pstPlunderSaveData);
    }

    if(pstPlayersPlunderSaveData->m_vecPlunderSaveData.size() > 0)
    {
        CDBPktBuilder::Instance()->SavePlayersPlunderDataReq(pstPlayersPlunderSaveData);
    }
    else
    {
        m_oPlayersPlunderSaveDataPool.Free(pstPlayersPlunderSaveData);
    }
    m_mapPlunderChangePlayer.clear();
}

VOID CPlayerSaveMgr::SavePlayersRaceData()
{
    CPlayersRaceSaveData* pstPlayersRaceSaveData = m_oPlayersRaceSaveDataPool.Alloc();

    HashMap<UINT32, UINT32>::iterator itr = m_mapRaceChangePlayer.begin();
    for(; itr != m_mapRaceChangePlayer.end(); itr++)
    {
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer( itr->first );
        if( NULL == poPlayer )
        {
            // SYS_CRITICAL( _SDT( "CPlayerSaveMgr::SavePlayersRaceData player[%u] not found!" ), itr->first );
            continue;
        }

        //如果是机器人，则不保存机器人信息
        if (ERFK_PLUNDER == poPlayer->GetDT_PLAYER_BASE_DATA().byRobotFunc)
        {
            continue;
        }

        SRaceSaveData* pstRaceSaveData = m_oRaceSaveDataPool.Alloc();
        if (NULL == pstRaceSaveData)
        {
            continue;
        }
        pstRaceSaveData->dwPlayerID = poPlayer->GetDT_PLAYER_BASE_DATA().dwID;
        GetRaceSaveData(poPlayer, pstRaceSaveData->stRaceData);
        GetRaceBattleLog(poPlayer, pstRaceSaveData->stRaceBattleLog);
        pstPlayersRaceSaveData->m_vecRaceSaveData.push_back(pstRaceSaveData);
    }

    if(pstPlayersRaceSaveData->m_vecRaceSaveData.size() > 0)
    {
        CDBPktBuilder::Instance()->SavePlayersRaceDataReq(pstPlayersRaceSaveData);
    }
    else
    {
        m_oPlayersRaceSaveDataPool.Free(pstPlayersRaceSaveData);
    }

    m_mapRaceChangePlayer.clear();
}

VOID CPlayerSaveMgr::SaveAllPlayerPvpData()
{
    SavePlayersPlunderData();
    SavePlayersRaceData();
}
