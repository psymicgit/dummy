
#include "getplayerdatacmd.h"
#include <db/dbsqlstring.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include <common/server/base64.h>
#include "dbcommoperate.h"
#include <db/autosqlbuf.h>
#include <sdloggerimpl.h>

#include <framework/gsapi.h>
#include <net/db/dbprocessor.h>

/**
* @brief 用户保存类型
*/
/*
enum ESaveUserToDbType
{
    ESRTDT_UNVALID = -1,
    ESRTDT_SHUTDOWN_EXIT    ,       ///< 玩家断线
    ESRTDT_GATESHUTDOWN_EXIT,       ///< GT断线
    ESRTDT_TIMER            ,       ///< TIMER例行保存
    ESRTDT_KICKOUT_ROLE     ,       ///< 踢人
    ESRTDT_MAX
};
*/


CGetPlayerDataCMD::CGetPlayerDataCMD()
{
    m_pstGetPlayerData = NULL;
}


CGetPlayerDataCMD::~CGetPlayerDataCMD()
{
}

VOID CGetPlayerDataCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    SDTRY_BEGIN

    UINT32 dwPlayerID = m_pstGetPlayerData->stGetPlayerDataReq.dwPlayerID;


    m_pstGetPlayerData->stGetPlayerDataAck.wErrCode = CheckAndProcessBackupSql( poDBConn, dwPlayerID );
    if( m_pstGetPlayerData->stGetPlayerDataAck.wErrCode != SUCCESS )
    {
        return;
    }

    DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData = m_pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerNoPvpData;
    DT_PLAYER_PVP_DATA& stPlayerPvpData = m_pstGetPlayerData->stGetPlayerDataAck.stDT_PLAYER_DATA.stPlayerPvpData;

    m_bSuccess = GetData(poDBConn, dwPlayerID, stPlayerNoPvpData, stPlayerPvpData, m_pstGetPlayerData->stGetPlayerDataAck.wErrCode);


    SDTRY_END

}

INT32 CGetPlayerDataCMD::CheckAndProcessBackupSql( SGDP::ISDDBConnection *poDBConn, UINT32 dwPlayerID )
{
    //如果是在登录时获取，需要判断是否发生存储错误，如果发生存储错误，则需要再次保存数据，如果依然失败，则不允许登录，并返回错误
    if( gsapi::GetDBMgr()->IsExistErrorInfo(ERROR_PLAYER, dwPlayerID ))
    {
        ISDDBRecordSet* pRecordSet = NULL;
        string strError;
        CHAR szSql[512] = {0};
        UINT64 qwDBUpdateTime = 0;
        sprintf(szSql, "select UpdateTime from players where PlayerID='%u';", dwPlayerID);
        INT32 nRet = poDBConn->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );
        if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
        {
            GetDateTime2Stamp(qwDBUpdateTime, "UpdateTime");
        }
        if(NULL != pRecordSet)
        {
            pRecordSet->Release();
            pRecordSet = NULL;
        }
        if(nRet < 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
            return REFUSE_LOGIN;
        }
        //若文件保存的时间比DB的更加新，则执行文件sql
        if(gsapi::GetDBMgr()->GetErrorFileUpdateTime(ERROR_PLAYER, dwPlayerID) > qwDBUpdateTime)
        {
            BOOL m_bError = FALSE;
            vector<string> vecStrList = gsapi::GetDBMgr()->GetSqlList(ERROR_PLAYER,  dwPlayerID );
            poDBConn->BeginTransaction();
            for( INT32 nIndex = 0; nIndex < INT32(vecStrList.size()); ++nIndex )
            {
                string strError;
                UINT64 qwInsertId = 0;
                UINT32 dwErr = 0;
                INT32 nRet = poDBConn->ExecuteSql(vecStrList[nIndex].c_str(), &qwInsertId, NULL, &dwErr, &strError );
                if(nRet < 0)
                {
                    SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
                    m_bError = TRUE;
                    break;
                }
            }

            if( m_bError )
            {
                poDBConn->RollbackTransaction();
                //m_pstGetPlayerData->stGetPlayerDataAck.wErrCode=REFUSE_LOGIN;
                return REFUSE_LOGIN;
            }

            poDBConn->CommitTransaction();
        }

        //删除失败，下次保存时再删除
        if( !gsapi::GetDBMgr()->DeleteErrorInfo(ERROR_PLAYER, dwPlayerID ) )
        {
            //删除文件失败
            SYS_CRITICAL(_SDT("[%s: %d]: DeleteErrorInfo failed, PlayerID[%d]!"), MSG_MARK, dwPlayerID);
        }

    }

    return SUCCESS;
}

VOID CGetPlayerDataCMD::OnExecuted()
{
    SDTRY_BEGIN

    gsapi::GetDBProcessor()->OnPlayerDataAck(m_pstGetPlayerData, m_dwTransID);


    SDTRY_END
}

BOOL CGetPlayerDataCMD::GetData(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData, DT_PLAYER_PVP_DATA& stPlayerPvpData, UINT16& wErrCode)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    wErrCode = FAIL;

    if(!GetPlayerBase(poDBConn, dwPlayerID, stPlayerNoPvpData))
    {
        wErrCode = GET_NODATA;
        //USR_INFO(_SDT("[%s: %d]: GetPlayerBase failed! playerid:%d"), MSG_MARK, dwPlayerID);    //新增玩家肯定没有
        return FALSE;
    }

    if(!GetPlayerTodayConsume(poDBConn, dwPlayerID, stPlayerNoPvpData))
    {
        wErrCode = GET_NODATA;
        //USR_INFO(_SDT("[%s: %d]: GetPlayerBase failed! playerid:%d"), MSG_MARK, dwPlayerID);    //新增玩家肯定没有
        return FALSE;
    }

    if(!GetPlayerExt(poDBConn, dwPlayerID, stPlayerNoPvpData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetPlayerExt failed! playerid:%d"), MSG_MARK, dwPlayerID);
        return FALSE;
    }

    if(!GetBag(poDBConn, dwPlayerID, stPlayerNoPvpData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBag failed! playerid:%d"), MSG_MARK, dwPlayerID);
        return FALSE;
    }

    if(!GetBuild(poDBConn, dwPlayerID, stPlayerNoPvpData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBuild failed! playerid:%d"), MSG_MARK, dwPlayerID);
        return FALSE;
    }

    if(!GetHeroBase(poDBConn, dwPlayerID, stPlayerNoPvpData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetHeroBase failed! playerid:%d"), MSG_MARK, dwPlayerID);
        return FALSE;
    }

    if(!GetHeroExt(poDBConn, dwPlayerID, stPlayerNoPvpData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBag GetHeroExt! playerid:%d"), MSG_MARK, dwPlayerID);
        return FALSE;
    }

    if(!GetInstance(poDBConn, dwPlayerID, stPlayerNoPvpData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetInstance failed! playerid:%d"), MSG_MARK, dwPlayerID);
        return FALSE;
    }

    if(!GetTask(poDBConn, dwPlayerID, stPlayerNoPvpData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetTask failed! playerid:%d"), MSG_MARK, dwPlayerID);
        return FALSE;
    }

    if(!GetRelationData(poDBConn, dwPlayerID, stPlayerNoPvpData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetRelationData failed! playerid:%d"), MSG_MARK, dwPlayerID);
        return FALSE;
    }

    if(!GetEvenData(poDBConn, dwPlayerID, stPlayerNoPvpData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetEvenData failed! playerid:%d"), MSG_MARK, dwPlayerID);
        return FALSE;
    }

    if(!GetSendFlowerLog(poDBConn, dwPlayerID, stPlayerNoPvpData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSendFlowerLog failed! playerid:%d"), MSG_MARK, dwPlayerID);
        return FALSE;
    }

    if(!GetPvp(poDBConn, dwPlayerID, stPlayerPvpData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetPvp failed! playerid:%d"), MSG_MARK, dwPlayerID);
        return FALSE;
    }

    wErrCode = SUCCESS;

    return TRUE;
}


BOOL CGetPlayerDataCMD::GetPlayerBase(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from players where PlayerID='%u'", dwPlayerID);

    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );

    DT_PLAYER_BASE_DATA& stBaseData = stPlayerNoPvpData.stBaseData;
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        stBaseData.dwID = dwPlayerID;
        UINT32 dwLen = pRecordSet->GetFieldLengthByName("NotifyID");
        dwLen = dwLen > MAX_TOKEN_NUM ? MAX_TOKEN_NUM : dwLen;
        memcpy(stPlayerNoPvpData.abyNoticeID, (VOID*)(pRecordSet->GetFieldValueByName("NotifyID")), dwLen);
        dwLen = pRecordSet->GetFieldLengthByName("DispName");
        dwLen = dwLen > USERNAME_LEN ? USERNAME_LEN : dwLen;
        memcpy(stBaseData.aszDispName, (VOID*)(pRecordSet->GetFieldValueByName("DispName")), dwLen);
        stBaseData.dwTotalAmount = SDAtou( pRecordSet->GetFieldValueByName("Pay") );
        stBaseData.byVipLevel = SDAtou( pRecordSet->GetFieldValueByName("VipLevel") );
        stBaseData.qwGold = SDAtou64( pRecordSet->GetFieldValueByName("Gold") );
        stBaseData.qwCoin = SDAtou64( pRecordSet->GetFieldValueByName("Coin") );
        stBaseData.wPhyStrength = SDAtou( pRecordSet->GetFieldValueByName("Phystrength") );
        stBaseData.qwScience = SDAtou64( pRecordSet->GetFieldValueByName("Science") );
        stBaseData.qwStory = SDAtou64( pRecordSet->GetFieldValueByName("Story") );
        stBaseData.qwJingJie = SDAtou64( pRecordSet->GetFieldValueByName("JingJie") );
        stBaseData.qwBlueGas = SDAtou64( pRecordSet->GetFieldValueByName("BlueGas") );
        stBaseData.qwPurpleGas = SDAtou64( pRecordSet->GetFieldValueByName("PurpleGas") );
        stBaseData.byBagOpenNum = SDAtou( pRecordSet->GetFieldValueByName("BagOpenNum") );
        GetEncodeDataFromDB3( "GuideRecordData", stBaseData.stGuideRecordData, dwPlayerID );
        stBaseData.byRobotFunc = SDAtou( pRecordSet->GetFieldValueByName("RobotFunc") );
        stBaseData.dwPower = SDAtou( pRecordSet->GetFieldValueByName("Power") );
        stBaseData.dwFirstAttack = SDAtou( pRecordSet->GetFieldValueByName("Firstattack") );
        stBaseData.qwStudyExp    = SDAtou64( pRecordSet->GetFieldValueByName("StudyExp") );
        stBaseData.dwExtData     = SDAtou( pRecordSet->GetFieldValueByName("ExtData") );
        stBaseData.qwTotalRMB    = SDAtou64( pRecordSet->GetFieldValueByName("TotalRMB") );
        stBaseData.dwFlowerSum   = SDAtou( pRecordSet->GetFieldValueByName("FlowerSum") );
        stBaseData.qwMedal       = SDAtou( pRecordSet->GetFieldValueByName("CourageMedal") );
        stBaseData.dwRMBGold     = SDAtou( pRecordSet->GetFieldValueByName("RMBGold") );
        stBaseData.dwRMBGoldGift = SDAtou( pRecordSet->GetFieldValueByName("RMBGoldGift") );
        stBaseData.dwGmGold      = SDAtou( pRecordSet->GetFieldValueByName("GmGold") );
        stBaseData.wOriZoneID    = SDAtou( pRecordSet->GetFieldValueByName("OriZoneID") );

        dwLen = pRecordSet->GetFieldLengthByName("LastOrderID");
        dwLen = dwLen > MAX_ORDER_ID_LEN ? MAX_ORDER_ID_LEN : dwLen;
        memcpy(stPlayerNoPvpData.szLastOrderID, (VOID*)(pRecordSet->GetFieldValueByName("LastOrderID")), dwLen);

        GetDateTime2Stamp(stPlayerNoPvpData.qwLoginTime, "LastLoginTime");
        GetDateTime2Stamp(stPlayerNoPvpData.qwLogoffTime, "LastLogoffTime");
        GetDateTime2Stamp(stBaseData.qwTodayFirstLoginTime,  "TodayFirstLoginTime");
        GetDateTime2Stamp(stBaseData.qwRecvRankEncTime,  "RecvRankEncTime");
    }
    else
    {
        //SYS_CRITICAL( _SDT("Player[%d] not exist!"), m_pstGetPlayerData->stGetPlayerDataReq.dwPlayerID);
        return FALSE;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}

BOOL CGetPlayerDataCMD::GetPlayerTodayConsume(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData)
{

    CSDDateTime oDateTime(SDTimeSecs());
    tstring strTodayTime = SDTimeToString("YYYY-mm-dd", oDateTime);

    CHAR		szSQL[256] = {0};
    sprintf(szSQL, "select * from player_consume where PlayerID='%u' and CreateTime = '%s'", dwPlayerID, strTodayTime.c_str());

    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );

    DT_PLAYER_TODAY_CONSUME_DATA& stDT_PLAYER_TODAY_CONSUME_DATA = stPlayerNoPvpData.stTodayConsume;
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        stDT_PLAYER_TODAY_CONSUME_DATA.dwID = dwPlayerID;
        GetDateTime2Stamp(stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime, "CreateTime");

        stDT_PLAYER_TODAY_CONSUME_DATA.qwAddGold = SDAtou64( pRecordSet->GetFieldValueByName("AddGold") );
        stDT_PLAYER_TODAY_CONSUME_DATA.qwDecGold = SDAtou64( pRecordSet->GetFieldValueByName("DecGold") );

        stDT_PLAYER_TODAY_CONSUME_DATA.qwAddCoin = SDAtou64( pRecordSet->GetFieldValueByName("AddCoin") );
        stDT_PLAYER_TODAY_CONSUME_DATA.qwDecCoin = SDAtou64( pRecordSet->GetFieldValueByName("DecCoin") );

        stDT_PLAYER_TODAY_CONSUME_DATA.wAddPhyStrength = SDAtou( pRecordSet->GetFieldValueByName("AddPhystrength") );
        stDT_PLAYER_TODAY_CONSUME_DATA.wDecPhyStrength = SDAtou( pRecordSet->GetFieldValueByName("DecPhystrength") );

        stDT_PLAYER_TODAY_CONSUME_DATA.qwAddScience = SDAtou64( pRecordSet->GetFieldValueByName("AddScience") );
        stDT_PLAYER_TODAY_CONSUME_DATA.qwDecScience = SDAtou64( pRecordSet->GetFieldValueByName("DecScience") );

        stDT_PLAYER_TODAY_CONSUME_DATA.qwAddStory = SDAtou64( pRecordSet->GetFieldValueByName("AddStory") );
        stDT_PLAYER_TODAY_CONSUME_DATA.qwDecStory = SDAtou64( pRecordSet->GetFieldValueByName("DecStory") );

        stDT_PLAYER_TODAY_CONSUME_DATA.qwAddJingJie = SDAtou64( pRecordSet->GetFieldValueByName("AddJingJie") );
        stDT_PLAYER_TODAY_CONSUME_DATA.qwDecJingJie = SDAtou64( pRecordSet->GetFieldValueByName("DecJingJie") );

        stDT_PLAYER_TODAY_CONSUME_DATA.qwAddBlueGas = SDAtou64( pRecordSet->GetFieldValueByName("AddBlueGas") );
        stDT_PLAYER_TODAY_CONSUME_DATA.qwDecBlueGas = SDAtou64( pRecordSet->GetFieldValueByName("DecBlueGas") );

        stDT_PLAYER_TODAY_CONSUME_DATA.qwAddPurpleGas = SDAtou64( pRecordSet->GetFieldValueByName("AddPurpleGas") );
        stDT_PLAYER_TODAY_CONSUME_DATA.qwDecPurpleGas = SDAtou64( pRecordSet->GetFieldValueByName("DecPurpleGas") );

        stDT_PLAYER_TODAY_CONSUME_DATA.qwAddStudyExp = SDAtou64( pRecordSet->GetFieldValueByName("AddStudyExp") );
        stDT_PLAYER_TODAY_CONSUME_DATA.qwDecStudyExp = SDAtou64( pRecordSet->GetFieldValueByName("DecStudyExp") );

        stDT_PLAYER_TODAY_CONSUME_DATA.qwAddExperience = SDAtou64( pRecordSet->GetFieldValueByName("AddExperience") );

        stDT_PLAYER_TODAY_CONSUME_DATA.wAddLevel = SDAtou( pRecordSet->GetFieldValueByName("AddLevel") );

        stDT_PLAYER_TODAY_CONSUME_DATA.byAddVipLevel = SDAtou( pRecordSet->GetFieldValueByName("AddVipLevel") );

        stDT_PLAYER_TODAY_CONSUME_DATA.dwAddAmount = SDAtou( pRecordSet->GetFieldValueByName("AddAmount") );
        stDT_PLAYER_TODAY_CONSUME_DATA.qwAddTotalRMB = SDAtou( pRecordSet->GetFieldValueByName("AddTotalRMB") );

        stDT_PLAYER_TODAY_CONSUME_DATA.qwAddMedal = SDAtou64( pRecordSet->GetFieldValueByName("AddMedal") );
        stDT_PLAYER_TODAY_CONSUME_DATA.qwDecMedal = SDAtou64( pRecordSet->GetFieldValueByName("DecMedal") );

		stDT_PLAYER_TODAY_CONSUME_DATA.qwAddDoorsTribute = SDAtou64(pRecordSet->GetFieldValueByName("AddDoorsTribute"));
		stDT_PLAYER_TODAY_CONSUME_DATA.qwDecDoorsTribute = SDAtou64(pRecordSet->GetFieldValueByName("DecDoorsTribute"));

        stDT_PLAYER_TODAY_CONSUME_DATA.qwAddRedStudyNum = SDAtou64(pRecordSet->GetFieldValueByName("RedStudyNum"));
        stDT_PLAYER_TODAY_CONSUME_DATA.qwAddTaijiStudyNum = SDAtou64(pRecordSet->GetFieldValueByName("TaijiStudyNum"));

        stDT_PLAYER_TODAY_CONSUME_DATA.qwBuyPhyStrengthNum = SDAtou( pRecordSet->GetFieldValueByName("BuyPhyStrengthNum") );
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}


BOOL CGetPlayerDataCMD::GetPlayerExt(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from player_ext where PlayerID='%u'", dwPlayerID);

    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );

    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            UINT16 wKey = SDAtou( pRecordSet->GetFieldValueByName("KeyID") );
            switch(wKey)
            {
            case EMDT_LocaleData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stLocaleData, dwPlayerID );
            }
            break;
            case EMDT_CurState:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stCurState, dwPlayerID );
            }
            break;
            case EMDT_OnlineEncRecdData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stOnlineEncRecdData, dwPlayerID );
            }
            break;
            case EMDT_LoginEncRecdData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stLoginEncRecdData, dwPlayerID );
            }
            break;
            case EMDT_MoneyTreeData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stMoneyTreeData, dwPlayerID );
            }
            break;
            case EMDT_CommonBattleRcd:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stInstanceData.stCommonBattleRcd, dwPlayerID );
            }
            break;
            case EMDT_EliteBattleRcd:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stInstanceData.stEliteBattleRcd, dwPlayerID );
            }
            break;
            case EMDT_JingJieData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stJingJieData, dwPlayerID );
            }
            break;
            case EMDT_PhystrengthData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stPhystrengthData, dwPlayerID );
            }
            break;
            case EMDT_FewParamData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stFewParamData, dwPlayerID );
            }
            break;
            case EMDT_CropData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stCropData, dwPlayerID );
            }
            break;
            case EMDT_ScienceTree:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stScienceTree, dwPlayerID );
            }
            break;
            case EMDT_FormationInfo:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stFormationInfo, dwPlayerID );
            }
            break;
            case EMDT_EatBreadInfo:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stEatBreadInfo, dwPlayerID );
            }
            break;
            case EMDT_YabiaoInfo:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stYabiaoInfo.stYabiaoBaseInfo, dwPlayerID );
            }
            break;
            case EMDT_ClimbTowerBattleRcd:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stInstanceData.stClimbTowerBattleRcd, dwPlayerID );
            }
            break;
            case EMDT_YabiaoBattleLog:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stYabiaoInfo.stYabiaoBattleLog, dwPlayerID );
            }
            break;
            case EMDT_ClimbTowerData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stClimbTowerData, dwPlayerID );
            }
            break;
            case EMDT_AwakenData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stAwakenData, dwPlayerID );
            }
            break;
            case EMDT_VipExtData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stVipExtData, dwPlayerID );
            }
            break;
            case EMDT_LuaActivityRecordData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stLuaActivityRecordData, dwPlayerID );
            }
            break;
            case EMDT_GasCollectData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stGasCollectData, dwPlayerID );
            }
            break;
            case EMDT_BossBattleData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stBossBattlePlayerDataInfo.stBossBattleData, dwPlayerID );
            }
            break;
            case EMDT_ACTIVITY_ENCOURAGE:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stActivityEncourageData, dwPlayerID );
            }
            break;
            case EMDT_BossBBL:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stBossBattlePlayerDataInfo.stBossBBLData, dwPlayerID );
            }
            break;
            case EMDT_RdChallengeData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stRdChallengeData, dwPlayerID );
            }
            break;
            case EMDT_WorshipData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stWorshipData, dwPlayerID );
            }
            break;
            case EMDT_ScoreData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stScoreData, dwPlayerID );
            }
            break;
            case EMDT_EquipComposeData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stEquipComposeData, dwPlayerID );
            }
            break;
            case EMDT_GatherScienceData:
            {
                GetEncodeDataFromDB2( stPlayerNoPvpData.stGatherScienceData, dwPlayerID );
            }
            break;
            case EMDT_BegBlueData:
            {
                GetEncodeDataFromDB2(stPlayerNoPvpData.stBegBlueData, dwPlayerID);
            }
            break;
            case EMDT_FriendsBaseData:
            {
                GetEncodeDataFromDB2(stPlayerNoPvpData.stFriendsData.stFriendsBaseData, dwPlayerID);
            }
            break;
            case EMDT_Courage:
            {
                GetEncodeDataFromDB2(stPlayerNoPvpData.stCourageData, dwPlayerID);
            }
            break;
            case EMDT_PlayerFactionData:
            {
                GetEncodeDataFromDB2(stPlayerNoPvpData.stPlayerFactionData, dwPlayerID);
            }
            break;
            case EMDT_EquipMaster:
            {
                GetEncodeDataFromDB2(stPlayerNoPvpData.stEquipMasterData, dwPlayerID);
            }
            break;
            case EMDT_ExchangeResultLog:
            {
                GetEncodeDataFromDB2(stPlayerNoPvpData.stExchangeResultLog, dwPlayerID);
            }
            break;
			case EMDT_GVGData:
			{
				GetEncodeDataFromDB2(stPlayerNoPvpData.stGvGData, dwPlayerID);
			}
			break;
			case EMDT_SoulData:
			{
				GetEncodeDataFromDB2(stPlayerNoPvpData.stSoulData, dwPlayerID);
			}
			break;
			case EMDT_MedalData:
			{
				GetEncodeDataFromDB2(stPlayerNoPvpData.stMedalData, dwPlayerID);
			}
			break;
			case EMDT_PetData: {
				GetEncodeDataFromDB2(stPlayerNoPvpData.stPetData, dwPlayerID);
			}
			break;
            default:
            {
                SYS_CRITICAL( _SDT("Get Player[%d] ext data, key[%d] invalid"), dwPlayerID, wKey );
                break;
            }
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}

BOOL CGetPlayerDataCMD::GetBag(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData)

{
    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from player_bags where PlayerID='%u'", dwPlayerID);

    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );

    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        GetEncodeDataFromDB3("Equip", stPlayerNoPvpData.stBagEquipData, dwPlayerID );
        GetEncodeDataFromDB3("Goods", stPlayerNoPvpData.stBagGoodsData, dwPlayerID );
		GetEncodeDataFromDB3("Jewelry", stPlayerNoPvpData.stBagJewelryInfo, dwPlayerID );
    }
    else
    {
		//不要提交
		if ( SDDB_NO_RECORDSET != nRet)
		{
			if(NULL != pRecordSet)
			{
				pRecordSet->Release();
				pRecordSet = NULL;
			}
			SYS_CRITICAL( _SDT("Player[%d] no bag data"), dwPlayerID);
			return FALSE;
		}
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}

BOOL CGetPlayerDataCMD::GetBuild(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData)

{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from player_builds where PlayerID='%u'", dwPlayerID);

    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );

    DT_BUILD_DATA_LIST& stBuildData = stPlayerNoPvpData.stBuildData;
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord() && stBuildData.wBuildNum < MAX_BUILD_NUM)
        {
            DT_BUILD_BASE_DATA& stBuildInfo = stBuildData.astBuildInfoLst[stBuildData.wBuildNum++];
            stBuildInfo.byKindID = SDAtou( pRecordSet->GetFieldValueByName("BuildID") );
            stBuildInfo.qwValue1 = SDAtou64( pRecordSet->GetFieldValueByName("Value1") );
            stBuildInfo.qwValue2 = SDAtou64( pRecordSet->GetFieldValueByName("Value2") );
            stBuildInfo.byValue3 = SDAtou( pRecordSet->GetFieldValueByName("Value3") );
            stBuildInfo.bySort = SDAtou( pRecordSet->GetFieldValueByName("Sort") );
            stBuildInfo.byHaveOpenFlag = SDAtou( pRecordSet->GetFieldValueByName("HaveOpenFlag") );
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}

BOOL CGetPlayerDataCMD::GetHeroBase(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData)

{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from player_heros where PlayerID='%u' and Coach != 2", dwPlayerID);

    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );

    UINT32 dwLen = 0;
    DT_HERO_DATA_LIST& stHeroData = stPlayerNoPvpData.stHeroData;
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord() && stHeroData.wHeroNum < MAX_HERO_NUM)
        {
            DT_HERO_BASE_DATA& stHeroBase = stHeroData.astHeroInfoLst[stHeroData.wHeroNum++].stHeroBaseData;
            stHeroBase.wKindID = SDAtou( pRecordSet->GetFieldValueByName("HeroID") );
            stHeroBase.byPos = SDAtou( pRecordSet->GetFieldValueByName("Pos") );
            stHeroBase.byCoach = SDAtou( pRecordSet->GetFieldValueByName("Coach") );
            stHeroBase.wTalent = SDAtou( pRecordSet->GetFieldValueByName("Talent") );
            stHeroBase.wLevel = SDAtou( pRecordSet->GetFieldValueByName("Level") );
            stHeroBase.qwExperience = SDAtou64( pRecordSet->GetFieldValueByName("Exp") );
            stHeroBase.byFormationIdx = SDAtou( pRecordSet->GetFieldValueByName("FormationIdx") );
            stHeroBase.byRecuitedFlag = SDAtou( pRecordSet->GetFieldValueByName("RecuitedFlag") );
            stHeroBase.byVisitNum = SDAtou( pRecordSet->GetFieldValueByName("VisitNum") );
            stHeroBase.byLastUseDrugLevel = SDAtou( pRecordSet->GetFieldValueByName("LastUseDrugLevel") );
            GetDateTime2Stamp(stHeroBase.qwLastUseDrugTime, "LastUseDrugTime");
            stHeroBase.byUseExperienceDrugPerDay = SDAtou( pRecordSet->GetFieldValueByName("UseExpdrugPerDay") );
            GetDateTime2Stamp(stHeroBase.qwLastUseExperienceDrugTime, "LastUseExpDrugTime");
            GetEncodeDataFromDB3("BattleAttribute", stHeroBase.stBattleAttribute, dwPlayerID);
            GetEncodeDataFromDB3("HideBattleAttr", stHeroBase.stHideBattleAttr, dwPlayerID);
            GetEncodeDataFromDB3("UpdateAttrInfo", stHeroBase.stUpdateAttrInfo, dwPlayerID);
            stHeroBase.dwFlyExp = SDAtou(pRecordSet->GetFieldValueByName("FlyExp"));
        }
    }
    else
    {
        SYS_CRITICAL( _SDT("Player[%d] no hero data"), dwPlayerID);
        return FALSE;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}

BOOL CGetPlayerDataCMD::GetHeroExt(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from player_hero_ext where PlayerID='%u' order by HeroID;", dwPlayerID);

    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );

    DT_HERO_DATA_LIST& stHeroDataLst = stPlayerNoPvpData.stHeroData;
    map<UINT16, DT_HERO_DATA*> mapHero;
	map<UINT16, DT_JEWELRY_DATA_LST*> mapHeroJewelry;
	map<UINT16, DT_HERO_SOUL_DATA*> mapSoul;
    UINT16 wHeroNum = stHeroDataLst.wHeroNum;
    for(UINT8 byIdx = 0; byIdx < wHeroNum; byIdx++)
    {
        mapHero[stHeroDataLst.astHeroInfoLst[byIdx].stHeroBaseData.wKindID] = &(stHeroDataLst.astHeroInfoLst[byIdx]);
		mapHeroJewelry[stHeroDataLst.astHeroInfoLst[byIdx].stHeroBaseData.wKindID] = &(stHeroDataLst.astjewelryInfoLst[byIdx]);
		mapSoul[stHeroDataLst.astHeroInfoLst[byIdx].stHeroBaseData.wKindID] = &(stHeroDataLst.astSoulInfoLst[byIdx]);
    }

    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            UINT16 wHeroID = SDAtou( pRecordSet->GetFieldValueByName("HeroID") );
            DT_HERO_DATA* pstDT_HERO_DATA = mapHero[wHeroID];
			DT_JEWELRY_DATA_LST* pstJewelryDatalst = mapHeroJewelry[wHeroID];
			DT_HERO_SOUL_DATA* pSoul = mapSoul[wHeroID];
            if(NULL == pstDT_HERO_DATA || NULL == pSoul)
            {
                continue;
            }
            UINT16 wKey = SDAtou( pRecordSet->GetFieldValueByName("KeyID") );
            switch(wKey)
            {
            case EHEDT_Equip:
            {
                GetEncodeDataFromDB2( pstDT_HERO_DATA->stEquipDataInfo, dwPlayerID );
            }
            break;
            case EHEDT_Drug:
            {
                GetEncodeDataFromDB2( pstDT_HERO_DATA->stDrugDataInfo, dwPlayerID );
            }
            break;
            case EHEDT_Godweapon:
            {
                GetEncodeDataFromDB2( pstDT_HERO_DATA->stGodweaponInfo, dwPlayerID );
            }
            break;
            case EHEDT_Skill:
            {
                GetEncodeDataFromDB2( pstDT_HERO_DATA->stSkillInfo, dwPlayerID );
            }
            break;
            case EHEDT_Study:
            {
                GetEncodeDataFromDB2( pstDT_HERO_DATA->stStudyInfo, dwPlayerID );
            }
            break;
            case EHEDT_Absorb:
            {
                GetEncodeDataFromDB2( pstDT_HERO_DATA->stAbsorbInfo, dwPlayerID );
            }
            break;
			case EHEDT_Jewelry:
			{
				GetEncodeDataFromDB2( (*pstJewelryDatalst), dwPlayerID);
			}
			break;
			case EHEDT_SOUL:
				{
					GetEncodeDataFromDB2((*pSoul), dwPlayerID);
				}
				break;
            default:
                SYS_CRITICAL( _SDT("Get Player[%d] hero ext data, key[%d] invalid"), dwPlayerID, wKey );
                break;
            }
        }
    }
    else
    {
        SYS_CRITICAL( _SDT("Player[%d] no hero ext data"), dwPlayerID);
        return FALSE;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}

BOOL CGetPlayerDataCMD::GetInstance(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData)
{
    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from player_instances where PlayerID='%u' order by SceneIdx, TownIdx, InstanceIdx", dwPlayerID);

    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );

    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            DT_INSTANCE_DATA_LIST* pstInstanceData = NULL;
            UINT16 wSceneIdx = SDAtou( pRecordSet->GetFieldValueByName("SceneIdx") );
            if(EIT_COMMON == wSceneIdx)
            {
                pstInstanceData = &(stPlayerNoPvpData.stInstanceData.stCommonInstanceData);
            }
            else if(EIT_ELITE == wSceneIdx)
            {
                pstInstanceData = &(stPlayerNoPvpData.stInstanceData.stEliteInstanceData);
            }
            else if(EIT_TOWER == wSceneIdx)
            {
                pstInstanceData = &(stPlayerNoPvpData.stInstanceData.stClimbTowerInstanceData);
            }
			else if ( EIT_OUT_BOUND == wSceneIdx)
			{
				pstInstanceData = &(stPlayerNoPvpData.stInstanceData.stOutBoundInstanceData);
			}
            if(NULL == pstInstanceData)
            {
                continue;
            }
            if(pstInstanceData->wInstanceNum >= MAX_INSTANCE_PER_SCENE_NUM)
            {
                continue;
            }
            pstInstanceData->wSceneIdx = wSceneIdx;
            DT_INSTANCE_DATA& stInstance = pstInstanceData->astInstanceInfo[pstInstanceData->wInstanceNum++];
            stInstance.stInstanceID.wSceneIdx = wSceneIdx;
            stInstance.stInstanceID.byTownIdx = SDAtou( pRecordSet->GetFieldValueByName("TownIdx") );
            stInstance.stInstanceID.byInstanceIdx = SDAtou( pRecordSet->GetFieldValueByName("InstanceIdx") );
            stInstance.byScore = SDAtou( pRecordSet->GetFieldValueByName("Score") );
            stInstance.dwSuccessNum = SDAtou( pRecordSet->GetFieldValueByName("SuccessNum") );
            stInstance.dwFailedNum = SDAtou( pRecordSet->GetFieldValueByName("FailedNum") );
            GetDateTime2Stamp(stInstance.qwLastPassTime, "LastPassTime");
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}

BOOL CGetPlayerDataCMD::GetTask(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData)
{
    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from player_tasks where PlayerID='%u' order by Line, Idx;", dwPlayerID);

    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );

    DT_TASK_DATA_LIST& stTaskLst = stPlayerNoPvpData.stTaskData;
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord() && stTaskLst.byTaskNum < MAX_TASK_LINE_NUM)
        {
            DT_TASK_DATA& stTask = stTaskLst.astTaskInfoList[stTaskLst.byTaskNum++];
            stTask.byTaskLine = SDAtou( pRecordSet->GetFieldValueByName("Line") );
            stTask.dwTaskIdx = SDAtou( pRecordSet->GetFieldValueByName("Idx") );
            stTask.byTaskState = SDAtou( pRecordSet->GetFieldValueByName("State") );
            GetDateTime2Stamp(stTask.qwStateUpdateTime, "StateUpdateTime");
            stTask.byTaskType = SDAtou( pRecordSet->GetFieldValueByName("Type") );
            UINT32 dwLen = pRecordSet->GetFieldLengthByName( "Cond" );
            if (0 != dwLen)
            {
                if( -1 == stTask.stTaskConditionInfo.DecodeSaveData(stTask.byTaskType, pRecordSet->GetFieldValueByName("Cond"), dwLen ) )
                {
                    SYS_CRITICAL( _SDT("[%s: %d] [PlayerID:%u]: stTaskConditionInfo.DecodeSaveData Failed!"), MSG_MARK, dwPlayerID);
                    return FALSE;
                }
            }
        }
    }
    else
    {
        SYS_CRITICAL( _SDT("Player[%d] no task data"), dwPlayerID);
        return FALSE;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}


BOOL CGetPlayerDataCMD::GetPvp(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_PVP_DATA& stPlayerPvpData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from player_pvp where PlayerID='%u'", dwPlayerID);

    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );

    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            UINT16 wKey = SDAtou( pRecordSet->GetFieldValueByName("KeyID") );
            switch(wKey)
            {
            case EMDT_RaceData:
            {
                GetEncodeDataFromDB2( stPlayerPvpData.stRaceData.stRaceBaseData, dwPlayerID );
            }
            break;
            case EMDT_RaceBattleLog:
            {
                GetEncodeDataFromDB2( stPlayerPvpData.stRaceData.stRaceBattleLog, dwPlayerID );
            }
            break;
            case EMDT_PlunderBaseData:
            {
                GetEncodeDataFromDB2( stPlayerPvpData.stPlunderData.stPlunderBaseData, dwPlayerID );
            }
            break;
            case EMDT_PlunderBattleLog:
            {
                GetEncodeDataFromDB2( stPlayerPvpData.stPlunderData.stPlunderBattleLog, dwPlayerID );
            }
            break;
            case EMDT_CaptureData:
            {
                GetEncodeDataFromDB2( stPlayerPvpData.stPlunderData.stCaptureData, dwPlayerID );
            }
            break;
            case EMDT_EnemyData:
            {
                GetEncodeDataFromDB2( stPlayerPvpData.stEnemyData, dwPlayerID );
            }
            break;
            default:
                SYS_CRITICAL( _SDT("Get Player[%d] ext data, key[%d] invalid"), dwPlayerID, wKey );
                break;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}


BOOL CGetPlayerDataCMD::GetRelationData(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData)
{
    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select pidA, pidB, relation, addTime from player_relation  where pidA= %u order by addTime desc limit %u ", dwPlayerID, MAX_RELATION_LIST_ELEM_NUM);
    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );

    DT_RELATION_DATA_LIST &stRelationDataList = stPlayerNoPvpData.stFriendsData.stRelationDataList;
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord() && stRelationDataList.dwRelationNum < MAX_RELATION_LIST_ELEM_NUM)
        {
            DT_RELATION_DATA& stDT_RELATION_DATA = stRelationDataList.astListData[stRelationDataList.dwRelationNum++];
            stDT_RELATION_DATA.dwPlayerIDB = SDAtou( pRecordSet->GetFieldValueByName("pidB") );
            stDT_RELATION_DATA.byrelation = SDAtou( pRecordSet->GetFieldValueByName("relation") );
            GetDateTime2Stamp(stDT_RELATION_DATA.qwAddTime, "addTime");
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    return TRUE;
}

BOOL CGetPlayerDataCMD::GetEvenData(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData)
{
    CHAR szSQL[1024] = {0};
    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    sprintf(szSQL, "select * from player_relation where pidB = %u and relation = 0 and isRead = 0 order by addTime desc limit %u", dwPlayerID, MAX_RELATION_LIST_ELEM_NUM);
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );
    DT_DATA_EVEN_LIST &stDT_DATA_EVEN_LIST = stPlayerNoPvpData.stFriendsData.stDataEvenList;
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord() && stDT_DATA_EVEN_LIST.dwRelationNum < MAX_RELATION_LIST_ELEM_NUM)
        {
            DT_RELATION_DATA_EVEN& stDT_RELATION_DATA_EVEN = stDT_DATA_EVEN_LIST.astRelationEvenData[stDT_DATA_EVEN_LIST.dwRelationNum++];
            stDT_RELATION_DATA_EVEN.dwPlayerIDA = SDAtou( pRecordSet->GetFieldValueByName("pidA") );
            stDT_RELATION_DATA_EVEN.byisRead = SDAtou( pRecordSet->GetFieldValueByName("isRead") );
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    sprintf(szSQL, "select sendPid, recvPid, count(recvPid) as UnReadNum  from private_chat_log where recvPid = %u and isRead = 0 GROUP BY sendPid limit %u", dwPlayerID, MAX_RELATION_LIST_ELEM_NUM);
    nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord() && stDT_DATA_EVEN_LIST.dwUnReadNum < MAX_RELATION_LIST_ELEM_NUM)
        {
            DT_UNREAD_DATA_EVEN& stDT_UNREAD_DATA_EVEN = stDT_DATA_EVEN_LIST.astUnReadEvenData[stDT_DATA_EVEN_LIST.dwUnReadNum++];
            stDT_UNREAD_DATA_EVEN.dwSendPid = SDAtou( pRecordSet->GetFieldValueByName("sendPid") );
            stDT_UNREAD_DATA_EVEN.dwUnReadNum = SDAtou( pRecordSet->GetFieldValueByName("UnReadNum") );
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}


BOOL CGetPlayerDataCMD::GetSendFlowerLog(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA& stPlayerNoPvpData)
{
    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from send_flower_log where recvPid = %u order by sendTime desc limit %u ", dwPlayerID, MAX_RECV_FLOWER_LOG_LIST_ELEM_NUM);
    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );

    DT_RECV_FLOWER_LOG_LIST &stRecvFlowerLogList = stPlayerNoPvpData.stFriendsData.stRecvFlowerLogList;
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord() && stRecvFlowerLogList.dwLogNum < MAX_RECV_FLOWER_LOG_LIST_ELEM_NUM)
        {
            DT_RECV_FLOWER_DATA_LOG& stDT_RECV_FLOWER_DATA_LOG = stRecvFlowerLogList.astListData[stRecvFlowerLogList.dwLogNum++];
            stDT_RECV_FLOWER_DATA_LOG.dwSendPlayerID = SDAtou( pRecordSet->GetFieldValueByName("sendPid") );
            stDT_RECV_FLOWER_DATA_LOG.dwFlowerNum = SDAtou( pRecordSet->GetFieldValueByName("flowerNum") );
            GetDateTime2Stamp(stDT_RECV_FLOWER_DATA_LOG.qwSendTime, "sendTime");
            stDT_RECV_FLOWER_DATA_LOG.byIsRead = SDAtou( pRecordSet->GetFieldValueByName("isRead") );
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    return TRUE;
}


VOID CGetPlayerDataCMD::SetUserData(VOID* pUserData)
{
    m_pstGetPlayerData = (SGetPlayerData*)pUserData;
}
