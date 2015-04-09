#include "saveplayerdatacmd.h"
#include "dbsqlstring.h"
#include <sdutil.h>
#include "server/utility.h"
#include <logic/player/playersavemgr.h>
#include "common/server/base64.h"
#include "dbcommoperate.h"
#include <db/autosqlbuf.h>
#include <net/db/dbprocessor.h>
#include <framework/gsapi.h>
#include <sdloggerimpl.h>
#include "logic/lua/luamgr.h"
#include <fstream>
#include "dll/sdu/sddir.h"

CSavePlayerDataCMD::CSavePlayerDataCMD()
{

}


CSavePlayerDataCMD::~CSavePlayerDataCMD()
{
}

VOID CSavePlayerDataCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    CDBMgr *pMgr = gsapi::GetDBMgr();
    UINT32 dwPlayerID = m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID;

    // 检测是否能找到玩家的diff数据
    if(NULL == gsapi::GetDBMgr()->FindDiff(dwPlayerID))
    {
        if(CPlayerMgr::Instance()->FindPlayer(dwPlayerID))
        {
            DBG_INFO(_SDT("[%s: %d] error: FindDiff failed, detecting player[%d] is not in playerdiffmap and can not find player in playermgr!"), MSG_MARK, dwPlayerID);
        }
        else
        {
            SYS_CRITICAL(_SDT("[%s: %d]: error: FindDiff failed, detecting player[%d] is not in playerdiffmap, and find player failed!"), MSG_MARK, dwPlayerID);
        }

        // 如果找不到玩家的diff数据，则强制让该玩家的所有数据再重新保存一次
        m_oPlayerSaveInfo.bSaveAll = TRUE;
    }

    gsapi::GetDBMgr()->StartDiff(dwPlayerID, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData);
    if (pMgr->IsExistErrorInfo(ERROR_PLAYER, dwPlayerID ))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: warning: detecting error info existed, PlayerID[%d]!"), MSG_MARK, dwPlayerID);
        m_oPlayerSaveInfo.bSaveAll = TRUE;
    }

    poDBConn->BeginTransaction();
    if(SaveData(poDBConn))
    {
        //保存成功,若存在错误文件则删除
        if (pMgr->IsExistErrorInfo(ERROR_PLAYER, dwPlayerID ))
        {
            if( !pMgr->DeleteErrorInfo(ERROR_PLAYER, dwPlayerID ) )
            {
                //删除文件失败
                SYS_CRITICAL(_SDT("[%s: %d]: DeleteErrorInfo failed, PlayerID[%d]!"), MSG_MARK, dwPlayerID);
            }
        }
        poDBConn->CommitTransaction();
        gsapi::GetDBMgr()->EndDiff(dwPlayerID);
        return ;//
    }
    poDBConn->RollbackTransaction();

    m_oPlayerSaveInfo.bSaveAll = TRUE;
    //记录错误的SQL信息
    vector<string> vecSqlAll = GetAllSql(poDBConn);

    gsapi::GetDBMgr()->AddErrorInfo(ERROR_PLAYER, dwPlayerID, vecSqlAll );
    gsapi::GetDBMgr()->EndDiff(dwPlayerID);
}

VOID CSavePlayerDataCMD::OnExecuted()
{
    if(!m_bSuccess)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: save player[%u] data failed!"), MSG_MARK, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID);
        m_oPlayerSaveInfo.poPlayerSaveData->m_wErrCode = FAIL;
    }
    else
    {
        m_oPlayerSaveInfo.poPlayerSaveData->m_wErrCode = SUCCESS;
    }

    gsapi::GetDBProcessor()->OnSavePlayerDataAck(m_oPlayerSaveInfo.poPlayerSaveData, m_dwTransID);

    gsapi::GetDBMgr()->SetSaveProcessed();
}

BOOL CSavePlayerDataCMD::GetPlayerBaseSql( SGDP::ISDDBConnection* poDBConn, CHAR *pszSql, DT_PLAYER_NOPVP_DATA* pstPlayerData )
{
    if( NULL == pszSql )
    {
        return FALSE;
    }


    CAutoSqlBuf oAutoSaveEnDeBuf;
    CAutoSqlBuf oSqlValues;
    CAutoSqlBuf oAutoTmp;
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    CHAR* pszTmp = oAutoTmp.GetSqlBuf();
    CHAR* pszSqlValues = oSqlValues.GetSqlBuf();

    if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp) || (NULL == pszSqlValues))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp) || (NULL == pszSqlValues))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    CSDDateTime oDateTime(pstPlayerData->qwLoginTime);
    tstring strLoginTime = SDTimeToString(oDateTime);

    oDateTime.SetTimeValue(pstPlayerData->qwLogoffTime);
    tstring strLogoffTime = SDTimeToString(oDateTime);

    oDateTime.SetTimeValue(pstPlayerData->stBaseData.qwTodayFirstLoginTime);
    tstring strTodayFirstLoginTime = SDTimeToString(oDateTime);


    oDateTime.SetTimeValue(pstPlayerData->stBaseData.qwRecvRankEncTime);
    tstring strRecvRankEncTime = SDTimeToString(oDateTime);


    INT32 nAddOnlineTime = 0;
    if( m_oPlayerSaveInfo.poPlayerSaveData->m_bAddOnlineTime )
    {
        nAddOnlineTime = INT32( pstPlayerData->qwLogoffTime - pstPlayerData->qwLoginTime );
        if( nAddOnlineTime < 0 )
        {
            //SYS_CRITICAL( _SDT( "online time <0 error!" ) ); //未下线则会<0
            nAddOnlineTime = 0;
        }
        nAddOnlineTime = nAddOnlineTime > 3600 ? 3600 : nAddOnlineTime;
    }

    sprintf(pszSql, "update players set AllOnlineTime=AllOnlineTime+'%d', UpdateTime= '%s'",  nAddOnlineTime, SDNow().ToString().c_str());
    INT32 nRet = 0;

    DT_PLAYER_BASE_DATA& stBaseData = pstPlayerData->stBaseData;
    UPDATEDATA2(stBaseData.stGuideRecordData, GuideRecordData, MAXENLEN_DT_GUIDE_RECORD_DATA);
	poDBConn->EscapeString( stBaseData.aszDispName, SDStrlen(stBaseData.aszDispName), pszTmp, MAX_SQL_LEN - 1);
    UPDATESIMPLEFIELD2(DispName, % s, pszTmp);
    UPDATESIMPLEFIELD2(Pay, % u, stBaseData.dwTotalAmount);
    UPDATESIMPLEFIELD2(VipLevel, % d, stBaseData.byVipLevel);
    UPDATESIMPLEFIELD2(Gold, % llu, stBaseData.qwGold);
    UPDATESIMPLEFIELD2(Coin, % llu, stBaseData.qwCoin);
    UPDATESIMPLEFIELD2(Phystrength, % u, stBaseData.wPhyStrength);
    UPDATESIMPLEFIELD2(Science, % llu, stBaseData.qwScience);
    UPDATESIMPLEFIELD2(JingJie, % llu, stBaseData.qwJingJie);
    UPDATESIMPLEFIELD2(Story, % llu, stBaseData.qwStory);
    UPDATESIMPLEFIELD2(JingJie, % llu, stBaseData.qwJingJie);
    UPDATESIMPLEFIELD2(BlueGas, % llu, stBaseData.qwBlueGas);
    UPDATESIMPLEFIELD2(PurpleGas, % llu, stBaseData.qwPurpleGas);
    UPDATESIMPLEFIELD2(StudyExp, % llu, stBaseData.qwStudyExp);
    UPDATESIMPLEFIELD2(BagOpenNum, % u, stBaseData.byBagOpenNum);
    UPDATESIMPLEFIELD2(Firstattack, % u, stBaseData.dwFirstAttack);
    UPDATESIMPLEFIELD2(Power, % u, stBaseData.dwPower);
    UPDATESIMPLEFIELD2(MainHeroKindID, % u, pstPlayerData->stHeroData.astHeroInfoLst[0].stHeroBaseData.wKindID);
    UPDATESIMPLEFIELD2(Level, % u, pstPlayerData->stHeroData.astHeroInfoLst[0].stHeroBaseData.wLevel);
    UPDATESIMPLEFIELD2(RobotFunc, % u, stBaseData.byRobotFunc);
    UPDATESIMPLEFIELD2(LastLoginTime, % s, strLoginTime.c_str());
    UPDATESIMPLEFIELD2(LastLogoffTime, % s, strLogoffTime.c_str());
    UPDATESIMPLEFIELD2(TodayFirstLoginTime, % s, strTodayFirstLoginTime.c_str());
    UPDATESIMPLEFIELD2(LastOrderID, % s, pstPlayerData->szLastOrderID);
    UPDATESIMPLEFIELD2(EatBreadNotify, % u, pstPlayerData->stEatBreadInfo.byNotity);
    UPDATESIMPLEFIELD2(CourageNofity, % u, pstPlayerData->stCourageData.byNotity);
    UPDATESIMPLEFIELD2(BossBattleNotify, % u, pstPlayerData->stBossBattlePlayerDataInfo.stBossBattleData.byNotity);
    UPDATESIMPLEFIELD2(RecvRankEncTime, % s, strRecvRankEncTime.c_str());
    UPDATESIMPLEFIELD2(ExtData, % u, stBaseData.dwExtData);
    UPDATESIMPLEFIELD2(TotalRMB, % llu, stBaseData.qwTotalRMB);
    UPDATESIMPLEFIELD2(FlowerSum, % u, stBaseData.dwFlowerSum);
    UPDATESIMPLEFIELD2(CourageMedal, % llu, stBaseData.qwMedal);
    UPDATESIMPLEFIELD2(RMBGold, % u, stBaseData.dwRMBGold);
    UPDATESIMPLEFIELD2(RMBGoldGift, % u, stBaseData.dwRMBGoldGift);
    UPDATESIMPLEFIELD2(GmGold, % u, stBaseData.dwGmGold);
    UPDATESIMPLEFIELD2(OriZoneID, % u, stBaseData.wOriZoneID);
    //
    sprintf(pszTmp, " where PlayerID ='%u';", pstPlayerData->stBaseData.dwID);
    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszTmp);

    return TRUE;
}



BOOL CSavePlayerDataCMD::GetPlayerTodayConsumeSql( SGDP::ISDDBConnection* poDBConn, char *pszSql, DT_PLAYER_NOPVP_DATA* pstPlayerData )
{
    if( NULL == pszSql )
    {
        return FALSE;
    }


    CAutoSqlBuf oSqlValues;
    CAutoSqlBuf oAutoTmp;
    CHAR* pszTmp = oAutoTmp.GetSqlBuf();
    CHAR* pszSqlValues = oSqlValues.GetSqlBuf();

    if((NULL == pszSql) || (NULL == pszTmp) || (NULL == pszSqlValues))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    if((NULL == pszSql) || (NULL == pszTmp) || (NULL == pszSqlValues))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    DT_PLAYER_TODAY_CONSUME_DATA &stDT_PLAYER_TODAY_CONSUME_DATA = pstPlayerData->stTodayConsume;
    CSDDateTime oDateTime(stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime);
    tstring strCreateTime = SDTimeToString("YYYY-mm-dd", oDateTime);
    sprintf(pszSql, "insert into player_consume (");
    INSERTINTOSIMPLEFIELD1(PlayerID, "%u", pstPlayerData->stBaseData.dwID);
    INSERTINTOSIMPLEFIELD2(CreateTime, "%s", strCreateTime.c_str());

    INSERTINTOSIMPLEFIELD2(AddGold, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddGold);
    INSERTINTOSIMPLEFIELD2(DecGold, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwDecGold);
    INSERTINTOSIMPLEFIELD2(AfterGold, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterGold);

    INSERTINTOSIMPLEFIELD2(AddCoin, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddCoin);
    INSERTINTOSIMPLEFIELD2(DecCoin, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwDecCoin);
    INSERTINTOSIMPLEFIELD2(AfterCoin, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterCoin);

    INSERTINTOSIMPLEFIELD2(AddPhystrength, "%u", stDT_PLAYER_TODAY_CONSUME_DATA.wAddPhyStrength);
    INSERTINTOSIMPLEFIELD2(DecPhystrength, "%u", stDT_PLAYER_TODAY_CONSUME_DATA.wDecPhyStrength);
    INSERTINTOSIMPLEFIELD2(AfterPhystrength, "%u", stDT_PLAYER_TODAY_CONSUME_DATA.wAfterPhyStrength);

    INSERTINTOSIMPLEFIELD2(AddScience, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddScience);
    INSERTINTOSIMPLEFIELD2(DecScience, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwDecScience);
    INSERTINTOSIMPLEFIELD2(AfterScience, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterScience);

    INSERTINTOSIMPLEFIELD2(AddStory, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddStory);
    INSERTINTOSIMPLEFIELD2(DecStory, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwDecStory);
    INSERTINTOSIMPLEFIELD2(AfterStory, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterStory);

    INSERTINTOSIMPLEFIELD2(AddJingJie, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddJingJie);
    INSERTINTOSIMPLEFIELD2(DecJingJie, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwDecJingJie);
    INSERTINTOSIMPLEFIELD2(AfterJingJie, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterJingJie);

    INSERTINTOSIMPLEFIELD2(AddBlueGas, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddBlueGas);
    INSERTINTOSIMPLEFIELD2(DecBlueGas, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwDecBlueGas);
    INSERTINTOSIMPLEFIELD2(AfterBlueGas, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterBlueGas);

    INSERTINTOSIMPLEFIELD2(AddPurpleGas, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddPurpleGas);
    INSERTINTOSIMPLEFIELD2(DecPurpleGas, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwDecPurpleGas);
    INSERTINTOSIMPLEFIELD2(AfterPurpleGas, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterPurpleGas);

    INSERTINTOSIMPLEFIELD2(AddStudyExp, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddStudyExp);
    INSERTINTOSIMPLEFIELD2(DecStudyExp, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwDecStudyExp);
    INSERTINTOSIMPLEFIELD2(AfterStudyExp, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterStudyExp);

    INSERTINTOSIMPLEFIELD2(AddAmount, "%u", stDT_PLAYER_TODAY_CONSUME_DATA.dwAddAmount);
    INSERTINTOSIMPLEFIELD2(AfterAmount, "%u", stDT_PLAYER_TODAY_CONSUME_DATA.dwAfterAmount);

    INSERTINTOSIMPLEFIELD2(AddExperience, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddExperience);
    INSERTINTOSIMPLEFIELD2(AfterExperience, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterExperience);

    INSERTINTOSIMPLEFIELD2(AddLevel, "%u", stDT_PLAYER_TODAY_CONSUME_DATA.wAddLevel);
    INSERTINTOSIMPLEFIELD2(AfterLevel, "%u", stDT_PLAYER_TODAY_CONSUME_DATA.wAfterLevel);

    INSERTINTOSIMPLEFIELD2(AddVipLevel, "%d", stDT_PLAYER_TODAY_CONSUME_DATA.byAddVipLevel);
    INSERTINTOSIMPLEFIELD2(AfterVipLevel, "%d", stDT_PLAYER_TODAY_CONSUME_DATA.byAfterVipLevel);

    INSERTINTOSIMPLEFIELD2(AddMedal, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddMedal);
    INSERTINTOSIMPLEFIELD2(DecMedal, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwDecMedal);
    INSERTINTOSIMPLEFIELD2(AfterMedal, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterMedal);
    INSERTINTOSIMPLEFIELD2(AddTotalRMB, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddTotalRMB);
    INSERTINTOSIMPLEFIELD2(AfterTotalRMB, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterTotalRMB);
    INSERTINTOSIMPLEFIELD3(UpdateTime, now());

	INSERTINTOSIMPLEFIELD2(AddDoorsTribute, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddDoorsTribute);
	INSERTINTOSIMPLEFIELD2(DecDoorsTribute, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwDecDoorsTribute);
	INSERTINTOSIMPLEFIELD2(AfterDoorsTribute, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterDoorsTribute);

    INSERTINTOSIMPLEFIELD2(RedStudyNum, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddRedStudyNum);
    INSERTINTOSIMPLEFIELD2(TaijiStudyNum, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwAddTaijiStudyNum);

    INSERTINTOSIMPLEFIELD2(BuyPhyStrengthNum, "%llu", stDT_PLAYER_TODAY_CONSUME_DATA.qwBuyPhyStrengthNum);
    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ") values (");
    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszTmp);
    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ") on duplicate key update \
				   		player_consume.AddGold = values(player_consume.AddGold) \
						,player_consume.DecGold = values(player_consume.DecGold) \
				   		,player_consume.AfterGold = values(player_consume.AfterGold) \
				   		,player_consume.AddCoin = values(player_consume.AddCoin) \
						,player_consume.DecCoin = values(player_consume.DecCoin) \
				   		,player_consume.AfterCoin = values(player_consume.AfterCoin) \
				   		,player_consume.AddPhystrength = values(player_consume.AddPhystrength) \
						,player_consume.DecPhystrength = values(player_consume.DecPhystrength) \
				   		,player_consume.AfterPhystrength = values(player_consume.AfterPhystrength) \
				   		,player_consume.AddScience = values(player_consume.AddScience) \
						,player_consume.DecScience = values(player_consume.DecScience) \
				   		,player_consume.AfterScience = values(player_consume.AfterScience) \
				   		,player_consume.AddStory = values(player_consume.AddStory) \
						,player_consume.DecStory = values(player_consume.DecStory) \
				   		,player_consume.AfterStory = values(player_consume.AfterStory) \
				   		,player_consume.AddJingJie = values(player_consume.AddJingJie) \
						,player_consume.DecJingJie = values(player_consume.DecJingJie) \
				   		,player_consume.AfterJingJie = values(player_consume.AfterJingJie) \
				   		,player_consume.AddBlueGas = values(player_consume.AddBlueGas) \
						,player_consume.DecBlueGas = values(player_consume.DecBlueGas) \
				   		,player_consume.AfterBlueGas = values(player_consume.AfterBlueGas) \
						,player_consume.AddPurpleGas = values(player_consume.AddPurpleGas) \
						,player_consume.DecPurpleGas = values(player_consume.DecPurpleGas) \
						,player_consume.AfterPurpleGas = values(player_consume.AfterPurpleGas) \
						,player_consume.AddStudyExp = values(player_consume.AddStudyExp) \
						,player_consume.DecStudyExp = values(player_consume.DecStudyExp) \
						,player_consume.AfterStudyExp = values(player_consume.AfterStudyExp) \
						,player_consume.AddAmount = values(player_consume.AddAmount) \
						,player_consume.AfterAmount = values(player_consume.AfterAmount) \
						,player_consume.AddExperience = values(player_consume.AddExperience) \
						,player_consume.AfterExperience = values(player_consume.AfterExperience) \
						,player_consume.AddLevel = values(player_consume.AddLevel) \
						,player_consume.AfterLevel = values(player_consume.AfterLevel) \
						,player_consume.AddVipLevel = values(player_consume.AddVipLevel) \
						,player_consume.AfterVipLevel = values(player_consume.AfterVipLevel) \
						,player_consume.AddTotalRMB = values(player_consume.AddTotalRMB) \
						,player_consume.AfterTotalRMB = values(player_consume.AfterTotalRMB) \
							,player_consume.AddMedal = values(player_consume.AddMedal) \
							,player_consume.DecMedal = values(player_consume.DecMedal) \
							,player_consume.AfterMedal = values(player_consume.AfterMedal) \
						,player_consume.UpdateTime = values(player_consume.UpdateTime)\
						,player_consume.AddDoorsTribute = values(player_consume.AddDoorsTribute)\
						,player_consume.DecDoorsTribute = values(player_consume.DecDoorsTribute)\
						,player_consume.AfterDoorsTribute = values(player_consume.AfterDoorsTribute)\
                        ,player_consume.RedStudyNum = values(player_consume.RedStudyNum) \
                        ,player_consume.TaijiStudyNum = values(player_consume.TaijiStudyNum) \
                        ,player_consume.BuyPhyStrengthNum = values(player_consume.BuyPhyStrengthNum)");


    return TRUE;
}


BOOL CSavePlayerDataCMD::GetPlayerExtSql( SGDP::ISDDBConnection* poDBConn, char *pszSql, DT_PLAYER_NOPVP_DATA* pstPlayerData, BOOL &bChanged )
{
    bChanged = FALSE;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CAutoSqlBuf oSqlValues;
    CAutoSqlBuf oAutoTmp;

    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    CHAR* pszTmp = oAutoTmp.GetSqlBuf();
    CHAR* pszSqlValues = oSqlValues.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp) || (NULL == pszSqlValues))
    {
        SYS_CRITICAL(_SDT("[ % s: % d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    sprintf(pszSql, "insert into player_ext (PlayerID, KeyID, Value, UpdateTime ) values ");

    INT32 nRet = 0;
    UINT16 wDataNum = 0;
    UINT32 dwPlayerID = pstPlayerData->stBaseData.dwID;

    // 再次检测能否找到对应该玩家的diff旧数据，如果找不到，并且标志位未被设置为全部重新保存，则说明是严重异常，很有可能导致玩家的操作回档而资源不回档
    if(NULL == gsapi::GetDBMgr()->FindDiff(dwPlayerID))
    {
        // 2014年4月3日：找不到玩家的CPlayerDiff数据是可能出现的，当玩家下线一个小时后，服务器清除该玩家，逻辑线程那边会将该玩家的CPlayerDiff实例被清除，
        // 所以很可能导致这里正好找不到Difff数据
        if(FALSE == m_oPlayerSaveInfo.bSaveAll)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: warning: saveplayerext -> detecting player[%d] is not in playerdiffmap, and m_oPlayerSaveInfo.bSaveAll == FALSE!"), MSG_MARK, dwPlayerID);
        }

        m_oPlayerSaveInfo.bSaveAll = TRUE;
    }
        
    ENCODEDATA_SAVE2(pstPlayerData->stLocaleData, EMDT_LocaleData);
    ENCODEDATA_SAVE2(pstPlayerData->stCurState, EMDT_CurState);
    ENCODEDATA_SAVE2(pstPlayerData->stOnlineEncRecdData, EMDT_OnlineEncRecdData);
    ENCODEDATA_SAVE2(pstPlayerData->stLoginEncRecdData, EMDT_LoginEncRecdData );
    ENCODEDATA_SAVE2(pstPlayerData->stMoneyTreeData, EMDT_MoneyTreeData );
    ENCODEDATA_SAVE2(pstPlayerData->stInstanceData.stCommonBattleRcd, EMDT_CommonBattleRcd );
    ENCODEDATA_SAVE2(pstPlayerData->stInstanceData.stEliteBattleRcd, EMDT_EliteBattleRcd );
    ENCODEDATA_SAVE2(pstPlayerData->stJingJieData, EMDT_JingJieData );
    ENCODEDATA_SAVE2(pstPlayerData->stPhystrengthData, EMDT_PhystrengthData );
    ENCODEDATA_SAVE2(pstPlayerData->stFewParamData, EMDT_FewParamData );
    ENCODEDATA_SAVE2(pstPlayerData->stCropData, EMDT_CropData );
    ENCODEDATA_SAVE2(pstPlayerData->stScienceTree, EMDT_ScienceTree);
    ENCODEDATA_SAVE2(pstPlayerData->stFormationInfo, EMDT_FormationInfo);
    ENCODEDATA_SAVE2(pstPlayerData->stEatBreadInfo, EMDT_EatBreadInfo );
    ENCODEDATA_SAVE2(pstPlayerData->stYabiaoInfo.stYabiaoBaseInfo, EMDT_YabiaoInfo );
    ENCODEDATA_SAVE2(pstPlayerData->stInstanceData.stClimbTowerBattleRcd, EMDT_ClimbTowerBattleRcd );
    ENCODEDATA_SAVE2(pstPlayerData->stYabiaoInfo.stYabiaoBattleLog, EMDT_YabiaoBattleLog );
    ENCODEDATA_SAVE2(pstPlayerData->stClimbTowerData, EMDT_ClimbTowerData );
    ENCODEDATA_SAVE2(pstPlayerData->stAwakenData, EMDT_AwakenData );
    ENCODEDATA_SAVE2(pstPlayerData->stVipExtData, EMDT_VipExtData);
    ENCODEDATA_SAVE2(pstPlayerData->stLuaActivityRecordData, EMDT_LuaActivityRecordData );
    ENCODEDATA_SAVE2(pstPlayerData->stGasCollectData, EMDT_GasCollectData );
    ENCODEDATA_SAVE2(pstPlayerData->stBossBattlePlayerDataInfo.stBossBattleData, EMDT_BossBattleData );
    ENCODEDATA_SAVE2(pstPlayerData->stActivityEncourageData, EMDT_ACTIVITY_ENCOURAGE);
    ENCODEDATA_SAVE2(pstPlayerData->stBossBattlePlayerDataInfo.stBossBBLData, EMDT_BossBBL );
    ENCODEDATA_SAVE2(pstPlayerData->stRdChallengeData, EMDT_RdChallengeData);
    ENCODEDATA_SAVE2(pstPlayerData->stWorshipData, EMDT_WorshipData );
    ENCODEDATA_SAVE2(pstPlayerData->stScoreData, EMDT_ScoreData );
    ENCODEDATA_SAVE2(pstPlayerData->stEquipComposeData, EMDT_EquipComposeData );
    ENCODEDATA_SAVE2(pstPlayerData->stGatherScienceData, EMDT_GatherScienceData );
    ENCODEDATA_SAVE2(pstPlayerData->stBegBlueData , EMDT_BegBlueData );
    ENCODEDATA_SAVE2(pstPlayerData->stFriendsData.stFriendsBaseData, EMDT_FriendsBaseData );
    ENCODEDATA_SAVE2(pstPlayerData->stCourageData, EMDT_Courage);
    ENCODEDATA_SAVE2(pstPlayerData->stPlayerFactionData, EMDT_PlayerFactionData );
    ENCODEDATA_SAVE2(pstPlayerData->stEquipMasterData, EMDT_EquipMaster );
    ENCODEDATA_SAVE2(pstPlayerData->stExchangeResultLog, EMDT_ExchangeResultLog);
	ENCODEDATA_SAVE2(pstPlayerData->stGvGData, EMDT_GVGData );
	ENCODEDATA_SAVE2(pstPlayerData->stSoulData , EMDT_SoulData);
	ENCODEDATA_SAVE2(pstPlayerData->stMedalData, EMDT_MedalData);
	ENCODEDATA_SAVE2(pstPlayerData->stPetData, EMDT_PetData);

    if(0 == wDataNum)
    {
        if(NULL == gsapi::GetDBMgr()->FindDiff(dwPlayerID))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: serious error: saveplayerext failed, detecting player[%d] is not in playerdiffmap and save player failed!"), MSG_MARK, dwPlayerID);
        }

        return FALSE;
    }

    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, " on duplicate key update player_ext.Value = values(player_ext.Value), player_ext.UpdateTime = values(player_ext.UpdateTime); ");
    bChanged = TRUE;

    return TRUE;
}

BOOL CSavePlayerDataCMD::SavePlayerBase(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA* pstPlayerData)
{

    //CAutoCycle oAutoCycle(__FUNCTION__);

    //基本信息整体有更新就更新,因为有冗余字段
    //if(!gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerBase))
    //{
    //	return TRUE;
    //}

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    if((NULL == pszSql))
    {
        SYS_CRITICAL(_SDT("[ % s: % d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    if( !GetPlayerBaseSql( poDBConn, pszSql, pstPlayerData ) )
    {
        SYS_CRITICAL(_SDT("[ % s: % d]: GetPlayerBaseSql is FALSE"), MSG_MARK);
        return FALSE;
    }

    INT32 nRet = 0;

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    UINT32 dwEffectRowNum = 0;
    nRet = poDBConn->ExecuteSql(pszSql, NULL, &dwEffectRowNum, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[ % s: % d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        return FALSE;
    }
    else if(dwEffectRowNum == 0)
    {
        SYS_CRITICAL(_SDT("[ % s: % d]: exec affect row! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        return TRUE;
    }
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CSavePlayerDataCMD::SavePlayerTodayConsume(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA* pstPlayerData)
{
    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    if((NULL == pszSql))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    if( !GetPlayerTodayConsumeSql( poDBConn, pszSql, pstPlayerData ) )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetPlayerBaseSql is FALSE"), MSG_MARK);
        return FALSE;
    }

    INT32 nRet = 0;
    string strError;
    UINT32 dwEffectRowNum = 0;
    nRet = poDBConn->ExecuteSql(pszSql, NULL, &dwEffectRowNum, NULL, &strError);
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        return FALSE;
    }
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CSavePlayerDataCMD::SavePlayerExt(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA* pstPlayerData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    BOOL bChanged = FALSE;
    if( !GetPlayerExtSql( poDBConn, pszSql, pstPlayerData, bChanged ) )
    {
        if( bChanged )
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }

    }

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        return FALSE;
    }
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CSavePlayerDataCMD::GetSaveBagSql( SGDP::ISDDBConnection* poDBConn, char *pszSql, UINT32 dwPlayerID, 
	DT_BAG_EQUIP_DATA_LST& stBagEquipData, 
	DT_BAG_GOODS_DATA_LST& stBagGoodsData, 
	DT_BAG_JEWELRY_DATA_LST& stBagJewelryData )
{
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CAutoSqlBuf oSqlValues;
    CAutoSqlBuf oAutoTmp;

    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    //memset(pszBuff, 0, MAX_SQL_LEN);
    INT32 nRet = stBagEquipData.EncodeSaveData(pszBuff, MAX_SQL_LEN);
    if(nRet <= 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: stBagEquipData.EncodeSaveData is failed"), MSG_MARK);
        return FALSE;
    }
    CHAR szEquip[MAXENLEN_DT_BAG_EQUIP_DATA_LST * 2 + 1] = {0};
    UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,
                       nRet,
                       szEquip,
                       sizeof(szEquip));
    if(dwRetCode <= 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    nRet = stBagGoodsData.EncodeSaveData(pszBuff, MAX_SQL_LEN);
    if(nRet <= 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: stBagGoodsData.EncodeSaveData is failed"), MSG_MARK);
        return FALSE;
    }
    CHAR szGoods[MAXENLEN_DT_BAG_GOODS_DATA_LST * 2 + 1] = {0};
    dwRetCode = poDBConn->EscapeString(pszBuff,
                                       nRet,
                                       szGoods,
                                       sizeof(szGoods));
    if(dwRetCode <= 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

	memset(pszBuff, 0, MAX_SQL_LEN);
	nRet = stBagJewelryData.EncodeSaveData(pszBuff, MAX_SQL_LEN);
	if(nRet <= 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: stBagGoodsData.EncodeSaveData is failed"), MSG_MARK);
		return FALSE;
	}
	CHAR szJewelry[MAXENLEN_DT_BAG_JEWELRY_DATA_LST * 2 + 1] = {0};
	dwRetCode = poDBConn->EscapeString(pszBuff,
		nRet,
		szJewelry,
		sizeof(szJewelry));
	if(dwRetCode <= 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
		return FALSE;
	}

    sprintf(pszSql, "insert into player_bags(PlayerID, Equip, Goods, UpdateTime, Jewelry) values ('%u', '%s', '%s', now(), '%s')  on duplicate key update player_bags.Equip = values(player_bags.Equip), player_bags.Goods = values(player_bags.Goods), player_bags.Jewelry = values(player_bags.Jewelry); ", dwPlayerID, szEquip, szGoods, szJewelry);

    return TRUE;
}

BOOL CSavePlayerDataCMD::SaveBag(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_BAG_EQUIP_DATA_LST& stBagEquipData, DT_BAG_GOODS_DATA_LST& stBagGoodsData, DT_BAG_JEWELRY_DATA_LST& stBagJewelryData)
{
    if( !m_oPlayerSaveInfo.bSaveAll )
    {
        if(!gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerBag))
        {
            return TRUE;
        }
    }

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    if( !GetSaveBagSql( poDBConn, pszSql, dwPlayerID, stBagEquipData, stBagGoodsData, stBagJewelryData ) )
    {
        return FALSE;
    }

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        return FALSE;
    }
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CSavePlayerDataCMD::GetSaveBuildSql( SGDP::ISDDBConnection* poDBConn, char *pszSql, UINT32 dwPlayerID, DT_BUILD_DATA_LIST& stBuildData, BOOL &bChanged, BOOL bGetAll )
{
    bChanged = TRUE;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    UINT16 wBuildNum = stBuildData.wBuildNum;
    sprintf(pszSql, "insert into player_builds(PlayerID, BuildID, Value1, Value2, Value3, Sort, HaveOpenFlag, UpdateTime) values ");
    UINT16 wDataNum = 0;
    for(UINT16 wIdx = 0; wIdx < wBuildNum && wIdx < MAX_BUILD_NUM; wIdx++)
    {
        //memset(pszBuff, 0, sizeof(MAX_SQL_LEN));
        DT_BUILD_BASE_DATA& stBaseData = stBuildData.astBuildInfoLst[wIdx];

        if( !m_oPlayerSaveInfo.bSaveAll )
        {
            if(!gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerBuild, stBaseData.byKindID) && bGetAll)
            {
                continue;
            }
        }



        if(0 != wDataNum++)
        {
            SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ",");
        }
        sprintf(pszBuff, "( '%u', '%u', '%llu', '%llu', '%u', '%u', '%u', now() )",
                dwPlayerID, stBaseData.byKindID, stBaseData.qwValue1, stBaseData.qwValue2, stBaseData.byValue3, stBaseData.bySort, stBaseData.byHaveOpenFlag);

        SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
    }
    if(0 == wDataNum)
    {
        bChanged = FALSE;
        return FALSE;
    }
    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, " on duplicate key update player_builds.Value1 = values(player_builds.Value1), player_builds.Value2 = values(player_builds.Value2), \
           player_builds.Value3 = values(player_builds.Value3), player_builds.Sort = values(player_builds.Sort), \
           player_builds.HaveOpenFlag = values(player_builds.HaveOpenFlag), player_builds.UpdateTime = values(player_builds.UpdateTime);");

    return TRUE;
}

BOOL CSavePlayerDataCMD::SaveBuild(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_BUILD_DATA_LIST& stBuildData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    if(0 == stBuildData.wBuildNum)
    {
        return TRUE;
    }

    if(!gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerBuildAll))
    {
        return TRUE;
    }

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    BOOL bChanged = FALSE;
    if( !GetSaveBuildSql( poDBConn, pszSql, dwPlayerID, stBuildData, bChanged ) )
    {
        if( bChanged )
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        return FALSE;
    }
    m_bSuccess = TRUE;
    return TRUE;

}

BOOL CSavePlayerDataCMD::GetSaveHeroBaseSql( SGDP::ISDDBConnection* poDBConn, char *pszSql, UINT32 dwPlayerID, DT_HERO_DATA_LIST& stHeroData, BOOL &bChanged, BOOL bGetAll )
{
    bChanged = FALSE;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CAutoSqlBuf oSqlValues;
    CAutoSqlBuf oAutoTmp;
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    CHAR* pszTmp = oAutoTmp.GetSqlBuf();
    CHAR* pszSqlValues = oSqlValues.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp) || (NULL == pszSqlValues))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    UINT16 wHeroNum = stHeroData.wHeroNum;
    sprintf(pszSql, "insert into player_heros (PlayerID, HeroID, Coach, Pos, Talent, Level, Exp, FormationIdx, "\
            "RecuitedFlag, VisitNum, LastUseDrugLevel, LastUseDrugTime, UseExpdrugPerDay, LastUseExpDrugTime, BattleAttribute, HideBattleAttr, UpdateAttrInfo, UpdateTime, FlyExp) values ");
    UINT16 wDataNum = 0;
    for(UINT16 wIdx = 0; wIdx < wHeroNum && wIdx < MAX_HERO_NUM; wIdx++)
    {
        //memset(pszBuff, 0, sizeof(MAX_SQL_LEN));
        DT_HERO_BASE_DATA& stBaseData = stHeroData.astHeroInfoLst[wIdx].stHeroBaseData;

        if( !m_oPlayerSaveInfo.bSaveAll )
        {
            if(!gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerHeroBase, stBaseData.wKindID) && bGetAll)
            {
                continue;
            }
        }

        if(0 != wDataNum++)
        {
            SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ", ");
        }

        DT_BATTLE_ATTRIBUTE& stBattleAttribute = stHeroData.astHeroInfoLst[wIdx].stHeroBaseData.stBattleAttribute;
        //memset(pszBuff, 0, MAX_SQL_LEN);
        INT32 nRet = stBattleAttribute.EncodeSaveData(pszBuff, MAX_SQL_LEN);
        if(nRet <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: stBagEquipData.EncodeSaveData is failed"), MSG_MARK);
            return FALSE;
        }
        CHAR szBattleAttribute[MAXENLEN_DT_BATTLE_ATTRIBUTE * 2 + 1] = {0};
        UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,
                           nRet,
                           szBattleAttribute,
                           sizeof(szBattleAttribute));
        if(dwRetCode <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
            return FALSE;
        }

        DT_HIDE_BATTLE_ATTRIBUTE& stHideBattleAttr = stHeroData.astHeroInfoLst[wIdx].stHeroBaseData.stHideBattleAttr;
        //memset(pszBuff, 0, MAX_SQL_LEN);
        nRet = stHideBattleAttr.EncodeSaveData(pszBuff, MAX_SQL_LEN);
        if(nRet <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: stBagEquipData.EncodeSaveData is failed"), MSG_MARK);
            return FALSE;
        }
        CHAR szHideBattleAttr[MAXENLEN_DT_HIDE_BATTLE_ATTRIBUTE * 2 + 1] = {0};
        dwRetCode = poDBConn->EscapeString(pszBuff,
                                           nRet,
                                           szHideBattleAttr,
                                           sizeof(szHideBattleAttr));
        if(dwRetCode <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
            return FALSE;
        }

        DT_HERO_UPDATE_ATTR_DATA& stUpdateAttrInfo = stHeroData.astHeroInfoLst[wIdx].stHeroBaseData.stUpdateAttrInfo;
        //memset(pszBuff, 0, MAX_SQL_LEN);
        nRet = stUpdateAttrInfo.EncodeSaveData(pszBuff, MAX_SQL_LEN);
        if(nRet <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: stBagEquipData.EncodeSaveData is failed"), MSG_MARK);
            return FALSE;
        }
        CHAR szUpdateAttrInfo[MAXENLEN_DT_HERO_UPDATE_ATTR_DATA * 2 + 1] = {0};
        dwRetCode = poDBConn->EscapeString(pszBuff,
                                           nRet,
                                           szUpdateAttrInfo,
                                           sizeof(szUpdateAttrInfo));
        if(dwRetCode <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
            return FALSE;
        }

        sprintf(pszBuff, "( '%u', '%u', '%u', '%u', '%u', '%u', '%llu', '%u', "\
                "'%u', '%u', '%u', '%s', "\
                "'%u', '%s', '%s', '%s', '%s', now(), %u)",
                dwPlayerID, stBaseData.wKindID, stBaseData.byCoach, stBaseData.byPos, stBaseData.wTalent, stBaseData.wLevel, stBaseData.qwExperience, stBaseData.byFormationIdx,
                stBaseData.byRecuitedFlag, stBaseData.byVisitNum, stBaseData.byLastUseDrugLevel, SDTimeToString(CSDDateTime(stBaseData.qwLastUseDrugTime)).c_str(),
                stBaseData.byUseExperienceDrugPerDay, SDTimeToString(CSDDateTime(stBaseData.qwLastUseExperienceDrugTime)).c_str(), szBattleAttribute, szHideBattleAttr, szUpdateAttrInfo, stBaseData.dwFlyExp);

        SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
    }
    if(0 == wDataNum)
    {
        return FALSE;
    }

    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, " on duplicate key update \
           player_heros.Pos = values(player_heros.Pos), player_heros.Talent = values(player_heros.Talent), player_heros.Level = values(player_heros.Level), \
           player_heros.Exp = values(player_heros.Exp), player_heros.FormationIdx = values(player_heros.FormationIdx), player_heros.RecuitedFlag = values(player_heros.RecuitedFlag), \
           player_heros.VisitNum = values(player_heros.VisitNum), player_heros.LastUseDrugLevel = values(player_heros.LastUseDrugLevel), player_heros.LastUseDrugTime = values(player_heros.LastUseDrugTime), \
           player_heros.UseExpdrugPerDay = values(player_heros.UseExpdrugPerDay), player_heros.LastUseExpDrugTime = values(player_heros.LastUseExpDrugTime), \
           player_heros.BattleAttribute = values(player_heros.BattleAttribute), player_heros.HideBattleAttr = values(player_heros.HideBattleAttr), player_heros.UpdateAttrInfo = values(player_heros.UpdateAttrInfo), \
           player_heros.Coach = values(player_heros.Coach), player_heros.UpdateTime = values(player_heros.UpdateTime), player_heros.FlyExp = values(player_heros.FlyExp);");

    bChanged = TRUE;

    return TRUE;
}

BOOL CSavePlayerDataCMD::SaveHeroBase(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_HERO_DATA_LIST& stHeroData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);
    if(0 == stHeroData.wHeroNum)
    {
        return TRUE;
    }

    if(!gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerHeroAll))
    {
        return TRUE;
    }

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    BOOL bChanged = FALSE;
    if( !GetSaveHeroBaseSql( poDBConn, pszSql, dwPlayerID, stHeroData, bChanged ) )
    {
        if( bChanged )
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        return FALSE;
    }
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CSavePlayerDataCMD::GetSaveHeroExtSql( SGDP::ISDDBConnection* poDBConn, char *pszSql, UINT32 dwPlayerID, DT_HERO_DATA_LIST& stHeroData, BOOL &bChanged, BOOL bGetAll )
{
    bChanged = FALSE;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CAutoSqlBuf oSqlValues;
    CAutoSqlBuf oAutoTmp;
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    CHAR* pszTmp = oAutoTmp.GetSqlBuf();
    CHAR* pszSqlValues = oSqlValues.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp) || (NULL == pszSqlValues))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    UINT16 wHeroNum = stHeroData.wHeroNum;
    sprintf(pszSql, "insert into player_hero_ext (PlayerID, HeroID, KeyID, Value, UpdateTime ) values ");
    UINT16 wDataNum = 0;
    for(UINT16 wIdx = 0; wIdx < wHeroNum && wIdx < MAX_HERO_NUM; wIdx++)
    {
        UINT16 wHeroID = stHeroData.astHeroInfoLst[wIdx].stHeroBaseData.wKindID;
        if(2 == stHeroData.astHeroInfoLst[wIdx].stHeroBaseData.byCoach) //2表示已被转换成其他职业
        {
            continue;
        }
        INT32 nRet = 0;
        UINT32 dwRetCode = 0;
        if( (m_oPlayerSaveInfo.bSaveAll || gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerHeroExt, wHeroID, EHEDT_Equip)) || bGetAll)
        {
            DT_HERO_EQUIP_DATA_LST& stEquipDataInfo = stHeroData.astHeroInfoLst[wIdx].stEquipDataInfo;
            //memset(pszBuff, 0, MAX_SQL_LEN);
            nRet = stEquipDataInfo.EncodeSaveData(pszBuff, MAX_SQL_LEN);
            if(nRet <= 0)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: stBagEquipData.EncodeSaveData is failed"), MSG_MARK);
                return FALSE;
            }
            CHAR szEquipDataInfo[MAXENLEN_DT_HERO_EQUIP_DATA_LST * 2 + 1] = {0};
            dwRetCode = poDBConn->EscapeString(pszBuff,
                                               nRet,
                                               szEquipDataInfo,
                                               sizeof(szEquipDataInfo));
            if(dwRetCode <= 0)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
                return FALSE;
            }
            if(0 != wDataNum++)
            {
                SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ", ");
            }
            sprintf(pszBuff, "( '%u', '%u', '%u', '%s', now() )",
                    dwPlayerID, wHeroID, EHEDT_Equip, szEquipDataInfo);
            SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
        }

        if( (m_oPlayerSaveInfo.bSaveAll || gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerHeroExt, wHeroID, EHEDT_Drug)) || bGetAll)
        {
            DT_DRUG_DATA_LST& stDrugDataInfo = stHeroData.astHeroInfoLst[wIdx].stDrugDataInfo;
            //memset(pszBuff, 0, MAX_SQL_LEN);
            nRet = stDrugDataInfo.EncodeSaveData(pszBuff, MAX_SQL_LEN);
            if(nRet <= 0)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: stBagEquipData.EncodeSaveData is failed"), MSG_MARK);
                return FALSE;
            }
            CHAR szDrugDataInfo[MAXENLEN_DT_DRUG_DATA_LST * 2 + 1] = {0};
            dwRetCode = poDBConn->EscapeString(pszBuff,
                                               nRet,
                                               szDrugDataInfo,
                                               sizeof(szDrugDataInfo));
            if(dwRetCode <= 0)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
                return FALSE;
            }
            if(0 != wDataNum++)
            {
                SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ", ");
            }
            sprintf(pszBuff, "( '%u', '%u', '%u', '%s', now() )",
                    dwPlayerID, wHeroID, EHEDT_Drug, szDrugDataInfo);
            SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
        }

        if((m_oPlayerSaveInfo.bSaveAll || gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerHeroExt, wHeroID, EHEDT_Godweapon)) || bGetAll )
        {
            DT_GODWEAPON_DATA& stGodweaponInfo = stHeroData.astHeroInfoLst[wIdx].stGodweaponInfo;
            //memset(pszBuff, 0, MAX_SQL_LEN);
            nRet = stGodweaponInfo.EncodeSaveData(pszBuff, MAX_SQL_LEN);
            if(nRet <= 0)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: stBagEquipData.EncodeSaveData is failed"), MSG_MARK);
                return FALSE;
            }
            CHAR szGodweaponInfo[MAXENLEN_DT_GODWEAPON_DATA * 2 + 1] = {0};
            dwRetCode = poDBConn->EscapeString(pszBuff,
                                               nRet,
                                               szGodweaponInfo,
                                               sizeof(szGodweaponInfo));
            if(dwRetCode <= 0)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
                return FALSE;
            }
            if(0 != wDataNum++)
            {
                SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ", ");
            }
            sprintf(pszBuff, "( '%u', '%u', '%u', '%s', now() )",
                    dwPlayerID, wHeroID, EHEDT_Godweapon, szGodweaponInfo);
            SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
        }

        if( (m_oPlayerSaveInfo.bSaveAll || gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerHeroExt, wHeroID, EHEDT_Skill)) || bGetAll )
        {
            DT_SKILL_DATA_LST& stSkillInfo = stHeroData.astHeroInfoLst[wIdx].stSkillInfo;
            //memset(pszBuff, 0, MAX_SQL_LEN);
            nRet = stSkillInfo.EncodeSaveData(pszBuff, MAX_SQL_LEN);
            if(nRet <= 0)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: stBagEquipData.EncodeSaveData is failed"), MSG_MARK);
                return FALSE;
            }
            CHAR szSkillInfo[MAXENLEN_DT_SKILL_DATA_LST * 2 + 1] = {0};
            dwRetCode = poDBConn->EscapeString(pszBuff,
                                               nRet,
                                               szSkillInfo,
                                               sizeof(szSkillInfo));
            if(dwRetCode <= 0)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
                return FALSE;
            }
            if(0 != wDataNum++)
            {
                SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ", ");
            }
            sprintf(pszBuff, "( '%u', '%u', '%u', '%s', now() )",
                    dwPlayerID, wHeroID, EHEDT_Skill, szSkillInfo);
            SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
        }

        if( (m_oPlayerSaveInfo.bSaveAll || gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerHeroExt, wHeroID, EHEDT_Study)) || bGetAll )
        {
            DT_AWAKEN_DATA_LST& stStudyInfo = stHeroData.astHeroInfoLst[wIdx].stStudyInfo;
            //memset(pszBuff, 0, MAX_SQL_LEN);
            nRet = stStudyInfo.EncodeSaveData(pszBuff, MAX_SQL_LEN);
            if(nRet <= 0)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: stBagEquipData.EncodeSaveData is failed"), MSG_MARK);
                return FALSE;
            }
            CHAR szStudyInfo[MAXENLEN_DT_AWAKEN_DATA_LST * 2 + 1] = {0};
            dwRetCode = poDBConn->EscapeString(pszBuff,
                                               nRet,
                                               szStudyInfo,
                                               sizeof(szStudyInfo));
            if(dwRetCode <= 0)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
                return FALSE;
            }
            if(0 != wDataNum++)
            {
                SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ", ");
            }
            sprintf(pszBuff, "( '%u', '%u', '%u', '%s', now() )",
                    dwPlayerID, wHeroID, EHEDT_Study, szStudyInfo);
            SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
        }

        if( (m_oPlayerSaveInfo.bSaveAll || gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerHeroExt, wHeroID, EHEDT_Absorb)) || bGetAll )
        {
            DT_ABSORB_DATA_LST& stAbsorbInfo = stHeroData.astHeroInfoLst[wIdx].stAbsorbInfo;
            //memset(pszBuff, 0, MAX_SQL_LEN);
            nRet = stAbsorbInfo.EncodeSaveData(pszBuff, MAX_SQL_LEN);
            if(nRet <= 0)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: stAbsorbInfo.EncodeSaveData is failed"), MSG_MARK);
                return FALSE;
            }
            CHAR szAbsorbInfo[MAXENLEN_DT_ABSORB_DATA_LST * 2 + 1] = {0};
            dwRetCode = poDBConn->EscapeString(pszBuff,
                                               nRet,
                                               szAbsorbInfo,
                                               sizeof(szAbsorbInfo));
            if(dwRetCode <= 0)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
                return FALSE;
            }
            if(0 != wDataNum++)
            {
                SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ", ");
            }
            sprintf(pszBuff, "( '%u', '%u', '%u', '%s', now() )",
                    dwPlayerID, wHeroID, EHEDT_Absorb, szAbsorbInfo);
            SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
        }

		if( (m_oPlayerSaveInfo.bSaveAll || gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerHeroExt, wHeroID, EHEDT_Jewelry)) || bGetAll )
		{
			DT_JEWELRY_DATA_LST& stJewelryLst = stHeroData.astjewelryInfoLst[wIdx];
			//memset(pszBuff, 0, MAX_SQL_LEN);
			nRet = stJewelryLst.EncodeSaveData(pszBuff, MAX_SQL_LEN);
			if(nRet <= 0)
			{
				SYS_CRITICAL(_SDT("[%s: %d]: stJewelryLst.EncodeSaveData is failed"), MSG_MARK);
				return FALSE;
			}
			CHAR szJewelryLst[MAXENLEN_DT_JEWELRY_DATA_LST * 2 + 1] = {0};
			dwRetCode = poDBConn->EscapeString(pszBuff,
				nRet,
				szJewelryLst,
				sizeof(szJewelryLst));
			if(dwRetCode <= 0)
			{
				SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
				return FALSE;
			}
			if(0 != wDataNum++)
			{
				SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ", ");
			}
			sprintf(pszBuff, "( '%u', '%u', '%u', '%s', now() )",
				dwPlayerID, wHeroID, EHEDT_Jewelry, szJewelryLst);
			SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
		}
		if( (m_oPlayerSaveInfo.bSaveAll || bGetAll
			|| gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerHeroExt, wHeroID, EHEDT_SOUL))) 
		{
			DT_HERO_SOUL_DATA& stSoulLst = stHeroData.astSoulInfoLst[wIdx];
			//memset(pszBuff, 0, MAX_SQL_LEN);
			nRet = stSoulLst.EncodeSaveData(pszBuff, MAX_SQL_LEN);
			if(nRet <= 0) {
				SYS_CRITICAL(_SDT("[%s: %d]: stSoulLst.EncodeSaveData is failed"), MSG_MARK);
				return FALSE;
			}
			CHAR szSoulLst[MAXENLEN_DT_HERO_SOUL_DATA * 2 + 1] = {0};
			dwRetCode = poDBConn->EscapeString(pszBuff, nRet, szSoulLst, sizeof(szSoulLst));
			if(dwRetCode <= 0) {
				SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
				return FALSE;
			}
			if(0 != wDataNum++) {
				SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ", ");
			}
			sprintf(pszBuff, "( '%u', '%u', '%u', '%s', now() )", dwPlayerID, wHeroID, EHEDT_SOUL, szSoulLst);
			SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
		}
    }
    if(0 == wDataNum)
    {
        return FALSE;
    }
    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, " on duplicate key update player_hero_ext.Value = values(player_hero_ext.Value);");

    bChanged = TRUE;

    return TRUE;
}

BOOL CSavePlayerDataCMD::SaveHeroExt(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_HERO_DATA_LIST& stHeroData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    if(0 == stHeroData.wHeroNum)
    {
        return TRUE;
    }

    if( !m_oPlayerSaveInfo.bSaveAll )
    {
        if(!gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerHeroAll))
        {
            return TRUE;
        }
    }

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    BOOL bChanged = FALSE;
    if( !GetSaveHeroExtSql( poDBConn, pszSql, dwPlayerID, stHeroData, bChanged ) )
    {
        if( bChanged )
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        return FALSE;
    }
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CSavePlayerDataCMD::GetSaveInstanceSql( SGDP::ISDDBConnection* poDBConn, char *pszSql, UINT32 dwPlayerID, DT_INSTANCE_DATA_LIST& stInstanceData, BOOL &bChanged, BOOL bGetAll )
{
    bChanged = FALSE;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    UINT16 wInstanceNum = stInstanceData.wInstanceNum;
    sprintf(pszSql, "insert into player_instances(PlayerID, SceneIdx, TownIdx, InstanceIdx, Score, SuccessNum, FailedNum, LastConFailedNum, MaxConFailedNum, LastPassTime, UpdateTime) values ");
    UINT16 wDataNum = 0;
    for(UINT16 wIdx = 0; wIdx < wInstanceNum && wIdx < MAX_INSTANCE_PER_SCENE_NUM; wIdx++)
    {
        //memset(pszBuff, 0, sizeof(MAX_SQL_LEN));
        DT_INSTANCE_DATA& stInstance = stInstanceData.astInstanceInfo[wIdx];
        if( !m_oPlayerSaveInfo.bSaveAll )
        {
            if( !gsapi::GetDBMgr()->HaveChanged(
                        dwPlayerID, EDT_PlayerInstance, stInstance.stInstanceID.wSceneIdx, stInstance.stInstanceID.byTownIdx, stInstance.stInstanceID.byInstanceIdx )
                    && bGetAll)
            {
                continue;
            }
        }

        if(0 != wDataNum++)
        {
            SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ",");
        }

        sprintf(pszBuff, "( '%u', '%u', '%u', '%u', '%u', '%u', '%u', "\
                "'%u', '%u','%s', now() )",
                dwPlayerID, stInstanceData.wSceneIdx, stInstance.stInstanceID.byTownIdx, stInstance.stInstanceID.byInstanceIdx, stInstance.byScore, stInstance.dwSuccessNum, stInstance.dwFailedNum,
                stInstance.wLastConFailedNum,  stInstance.wMaxConFailedNum, SDTimeToString(CSDDateTime(stInstance.qwLastPassTime)).c_str());
        SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
    }
    if(0 == wDataNum)
    {
        return FALSE;
    }
    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, " on duplicate key update player_instances.LastPassTime = values(player_instances.LastPassTime), player_instances.UpdateTime = values(player_instances.UpdateTime) "\
           ", player_instances.Score = values(player_instances.Score)"\
           ", player_instances.SuccessNum = values(player_instances.SuccessNum)"\
           ", player_instances.LastConFailedNum = values(player_instances.LastConFailedNum)"\
           ", player_instances.MaxConFailedNum = values(player_instances.MaxConFailedNum)"\
           ", player_instances.FailedNum = values(player_instances.FailedNum);");

    bChanged = TRUE;

    return TRUE;
}

BOOL CSavePlayerDataCMD::SaveInstance(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_INSTANCE_DATA_LIST& stInstanceData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    if(0 == stInstanceData.wInstanceNum)
    {
        return TRUE;
    }

    if( !m_oPlayerSaveInfo.bSaveAll )
    {
        if(!gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerInstanceAll))
        {
            return TRUE;
        }
    }


    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    BOOL bChanged = FALSE;
    if( !GetSaveInstanceSql( poDBConn, pszSql, dwPlayerID, stInstanceData, bChanged ) )
    {
        if( bChanged )
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        return FALSE;
    }
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CSavePlayerDataCMD::GetSaveTaskSql( SGDP::ISDDBConnection* poDBConn, char *pszSql, UINT32 dwPlayerID, DT_TASK_DATA_LIST& stTaskData, BOOL &bChanged, BOOL bGetAll )
{
    bChanged = FALSE;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    UINT16 byTaskNum = stTaskData.byTaskNum;
    sprintf(pszSql, "insert into player_tasks(PlayerID, Line, Idx, State, StateUpdateTime, Type, Cond, UpdateTime) values ");
    UINT16 wDataNum = 0;
    for(UINT16 wIdx = 0; wIdx < byTaskNum && wIdx < MAX_TASK_LINE_NUM; wIdx++)
    {
        //memset(pszBuff, 0, sizeof(MAX_SQL_LEN));
        DT_TASK_DATA& stTask = stTaskData.astTaskInfoList[wIdx];

        if( !m_oPlayerSaveInfo.bSaveAll )
        {
            if( !gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerTask, stTask.byTaskLine) && bGetAll)
            {
                continue;
            }
        }

        if(0 != wDataNum++)
        {
            SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ",");
        }

        DT_TASK_CONDITION& stTaskConditionInfo = stTask.stTaskConditionInfo;
        //memset(pszBuff, 0, MAX_SQL_LEN);
        INT32 nRet = stTaskConditionInfo.EncodeSaveData(stTask.byTaskType, pszBuff, MAX_SQL_LEN);
        if(nRet <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: stBagEquipData.EncodeSaveData is failed"), MSG_MARK);
            return FALSE;
        }
        CHAR szTaskConditionInfo[MAXENLEN_DT_TASK_CONDITION * 2 + 1] = {0};
        UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,
                           nRet,
                           szTaskConditionInfo,
                           sizeof(szTaskConditionInfo));
        if(dwRetCode <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
            return FALSE;
        }

        sprintf(pszBuff, "( '%u', '%u', '%u', '%u', '%s', '%u', '%s', now() )",
                dwPlayerID, stTask.byTaskLine, stTask.dwTaskIdx, stTask.byTaskState, SDTimeToString(CSDDateTime(stTask.qwStateUpdateTime)).c_str(), stTask.byTaskType, szTaskConditionInfo);

        SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
    }
    if(0 == wDataNum)
    {
        return FALSE;
    }
    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, " on duplicate key update player_tasks.Idx = values(player_tasks.Idx), player_tasks.Idx = values(player_tasks.Idx), \
           player_tasks.State = values(player_tasks.State), player_tasks.StateUpdateTime = values(player_tasks.StateUpdateTime), player_tasks.Type = values(player_tasks.Type),\
           player_tasks.Cond = values(player_tasks.Cond), player_tasks.UpdateTime = values(player_tasks.UpdateTime);");

    bChanged = TRUE;

    return TRUE;
}

BOOL CSavePlayerDataCMD::SaveTask(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_TASK_DATA_LIST& stTaskData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    if(0 == stTaskData.byTaskNum)
    {
        return TRUE;
    }

    if( !m_oPlayerSaveInfo.bSaveAll )
    {
        if(!gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerTaskAll))
        {
            return TRUE;
        }
    }

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    BOOL bChanged = FALSE;
    if( !GetSaveTaskSql( poDBConn, pszSql, dwPlayerID, stTaskData, bChanged ) )
    {
        if( !bChanged )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        return FALSE;
    }
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CSavePlayerDataCMD::GetSaveSendFlowerLogSql( SGDP::ISDDBConnection* poDBConn, char *pszSql, UINT32 dwPlayerID, DT_RECV_FLOWER_LOG_LIST &stRecvFlowerLogList, BOOL &bChanged, BOOL bGetAll )
{
    bChanged = FALSE;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    UINT32 dwLogNum = stRecvFlowerLogList.dwLogNum;
    sprintf(pszSql, "insert into send_flower_log(sendPid, recvPid, flowerNum, sendTime, isRead) values ");
    UINT16 wDataNum = 0;
    for(UINT16 wIdx = 0; wIdx < dwLogNum && wIdx < MAX_RECV_FLOWER_LOG_LIST_ELEM_NUM; wIdx++)
    {
        DT_RECV_FLOWER_DATA_LOG& stDT_RECV_FLOWER_DATA_LOG = stRecvFlowerLogList.astListData[wIdx];
        if(0 != wDataNum++)
        {
            SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ",");
        }
        CSDDateTime oDateTime(stDT_RECV_FLOWER_DATA_LOG.qwSendTime);
        tstring strSendTime = SDTimeToString(oDateTime);
        sprintf(pszBuff, "('%u', '%u', '%u', '%s', '%u')", stDT_RECV_FLOWER_DATA_LOG.dwSendPlayerID, dwPlayerID, stDT_RECV_FLOWER_DATA_LOG.dwFlowerNum, strSendTime.c_str(), stDT_RECV_FLOWER_DATA_LOG.byIsRead);
        SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
    }
    if(0 == wDataNum)
    {
        return FALSE;
    }
    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, " on duplicate key update send_flower_log.isRead = values(send_flower_log.isRead);");
    bChanged = TRUE;
    return TRUE;
}


BOOL CSavePlayerDataCMD::SaveSendFlowerLog(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_RECV_FLOWER_LOG_LIST &stRecvFlowerLogList)
{
    if(0 == stRecvFlowerLogList.dwLogNum)
    {
        return TRUE;
    }

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    BOOL bChanged = FALSE;
    if( !GetSaveSendFlowerLogSql( poDBConn, pszSql, dwPlayerID, stRecvFlowerLogList, bChanged ) )
    {
        if( !bChanged )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        return FALSE;
    }
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CSavePlayerDataCMD::GetSaveRelationDataSql1( SGDP::ISDDBConnection* poDBConn, char *pszSql, UINT32 dwPlayerID, DT_RELATION_DATA_LIST &stRelationDataList, BOOL &bChanged, BOOL bGetAll )
{
    bChanged = FALSE;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    sprintf(pszSql, "delete from  player_relation where pidA = %u and pidB not in (0 ", dwPlayerID);//加个0，主是用于保证执行的SQL语句一定成功
    for(UINT16 wIdx = 0; wIdx < stRelationDataList.dwRelationNum && wIdx < MAX_RELATION_LIST_ELEM_NUM; wIdx++)
    {
        DT_RELATION_DATA& stDT_RELATION_DATA = stRelationDataList.astListData[wIdx];
        char v_szTemp[56] = {0};
        sprintf(v_szTemp, ", %u", stDT_RELATION_DATA.dwPlayerIDB);
        SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, v_szTemp);
    }
    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ")");
    bChanged = TRUE;
    return TRUE;
}

BOOL CSavePlayerDataCMD::GetSaveRelationDataSql2( SGDP::ISDDBConnection* poDBConn, char *pszSql, UINT32 dwPlayerID, DT_RELATION_DATA_LIST &stRelationDataList, BOOL &bChanged, BOOL bGetAll )
{
    bChanged = FALSE;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    UINT32 dwNum = stRelationDataList.dwRelationNum;
    sprintf(pszSql, "insert into player_relation(pidA, pidB, relation, addTime) values ");
    UINT16 wDataNum = 0;
    for(UINT16 wIdx = 0; wIdx < dwNum && wIdx < MAX_RELATION_LIST_ELEM_NUM; wIdx++)
    {
        DT_RELATION_DATA& stDT_RELATION_DATA = stRelationDataList.astListData[wIdx];
        if(0 != wDataNum++)
        {
            SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ",");
        }
        CSDDateTime oDateTime(stDT_RELATION_DATA.qwAddTime);
        tstring strSendTime = SDTimeToString(oDateTime);
        sprintf(pszBuff, "('%u', '%u', '%u', '%s')", dwPlayerID, stDT_RELATION_DATA.dwPlayerIDB, stDT_RELATION_DATA.byrelation, strSendTime.c_str());
        SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, pszBuff);
    }
    if(0 == wDataNum)
    {
        return FALSE;
    }
    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, " on duplicate key update player_relation.relation = values(player_relation.relation);");
    bChanged = TRUE;
    return TRUE;
}

BOOL CSavePlayerDataCMD::GetSaveEvenDataSql( SGDP::ISDDBConnection* poDBConn, char *pszSql, UINT32 dwPlayerID, DT_DATA_EVEN_LIST &stDataEvenList, BOOL &bChanged, BOOL bGetAll )
{
    bChanged = FALSE;
    if((NULL == pszSql))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }
    if(0 == stDataEvenList.dwRelationNum)
    {
        return FALSE;
    }
    CHAR szBuff[1024] = {0};
    for(UINT16 wIdx = 0; wIdx < stDataEvenList.dwRelationNum && wIdx < MAX_RELATION_LIST_ELEM_NUM; wIdx++)
    {
        DT_RELATION_DATA_EVEN& stDT_RELATION_DATA_EVEN = stDataEvenList.astRelationEvenData[wIdx];
        memset(szBuff, 0x00, sizeof(szBuff));
        sprintf(szBuff, "UPDATE player_relation set isRead = 1 where pidA = %u and pidB = %u;", stDT_RELATION_DATA_EVEN.dwPlayerIDA, dwPlayerID);
        SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, szBuff);
    }
    bChanged = TRUE;
    return TRUE;
}



BOOL CSavePlayerDataCMD::SaveRelationData(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_RELATION_DATA_LIST &stRelationDataList)
{
    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet;
    BOOL bChanged = FALSE;
    if( GetSaveRelationDataSql1( poDBConn, pszSql, dwPlayerID, stRelationDataList, bChanged ) )
    {
        nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
        if(NULL != pRecordSet)
        {
            pRecordSet->Release();
            pRecordSet = NULL;
        }
        if(nRet < 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
            return FALSE;
        }
    }

    bChanged = FALSE;
    if( GetSaveRelationDataSql2( poDBConn, pszSql, dwPlayerID, stRelationDataList, bChanged ) )
    {
        nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
        if(NULL != pRecordSet)
        {
            pRecordSet->Release();
            pRecordSet = NULL;
        }
        if(nRet < 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
            return FALSE;
        }
    }
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CSavePlayerDataCMD::SaveEvenData(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_DATA_EVEN_LIST &stDT_DATA_EVEN_LIST)
{
    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet;
    BOOL bChanged = FALSE;
    bChanged = FALSE;
    if( GetSaveEvenDataSql( poDBConn, pszSql, dwPlayerID, stDT_DATA_EVEN_LIST, bChanged ) )
    {
        nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
        if(NULL != pRecordSet)
        {
            pRecordSet->Release();
            pRecordSet = NULL;
        }
        if(nRet < 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
            return FALSE;
        }
    }

    m_bSuccess = TRUE;
    return TRUE;
}


BOOL CSavePlayerDataCMD::SaveData(SGDP::ISDDBConnection* poDBConn)
{
    m_bSuccess = FALSE;
    UINT32 dwPlayerID = m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID;
    DT_PLAYER_NOPVP_DATA* pstPlayerData = &(m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData);


    if( gsapi::GetDBMgr()->FindPlayerSaveTime(dwPlayerID) )
    {
        SavePlayerDataSqlByFile(poDBConn, dwPlayerID);
    }

    if(!SavePlayerBase(poDBConn, dwPlayerID, pstPlayerData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SavePlayerBase failed!"), MSG_MARK);
        return FALSE;
    }


    if(!SavePlayerTodayConsume(poDBConn, dwPlayerID, pstPlayerData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SavePlayerTodayConsume failed!"), MSG_MARK);
        return FALSE;
    }

    if(!SavePlayerExt(poDBConn, dwPlayerID, pstPlayerData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SavePlayerExt failed!"), MSG_MARK);
        return FALSE;
    }

    if(!SaveBag(poDBConn, dwPlayerID, pstPlayerData->stBagEquipData, pstPlayerData->stBagGoodsData, pstPlayerData->stBagJewelryInfo))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SaveBag failed!"), MSG_MARK);
        return FALSE;
    }

    if(!SaveBuild(poDBConn, dwPlayerID, pstPlayerData->stBuildData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SaveBuild failed!"), MSG_MARK);
        return FALSE;
    }

    if(!SaveHeroBase(poDBConn, dwPlayerID, pstPlayerData->stHeroData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SaveHeroBase failed!"), MSG_MARK);
        return FALSE;
    }

    if(!SaveHeroExt(poDBConn, dwPlayerID, pstPlayerData->stHeroData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SaveHeroExt failed!"), MSG_MARK);
        return FALSE;
    }

    if(!SaveInstance(poDBConn, dwPlayerID, pstPlayerData->stInstanceData.stCommonInstanceData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SaveInstance failed!"), MSG_MARK);
        return FALSE;
    }

    if(!SaveInstance(poDBConn, dwPlayerID, pstPlayerData->stInstanceData.stEliteInstanceData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SaveInstance failed!"), MSG_MARK);
        return FALSE;
    }

    if(!SaveInstance(poDBConn, dwPlayerID, pstPlayerData->stInstanceData.stClimbTowerInstanceData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SaveInstance failed!"), MSG_MARK);
        return FALSE;
    }

	if(!SaveInstance(poDBConn, dwPlayerID, pstPlayerData->stInstanceData.stOutBoundInstanceData))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: SaveInstance failed!"), MSG_MARK);
		return FALSE;
	}

    if(!SaveTask(poDBConn, dwPlayerID, pstPlayerData->stTaskData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SaveTask failed!"), MSG_MARK);
        return FALSE;
    }

    if(!SaveSendFlowerLog(poDBConn, dwPlayerID, pstPlayerData->stFriendsData.stRecvFlowerLogList))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SaveSendFlowerLog failed!"), MSG_MARK);
        return FALSE;
    }
    if(!SaveRelationData(poDBConn, dwPlayerID, pstPlayerData->stFriendsData.stRelationDataList))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SaveRelationData failed!"), MSG_MARK);
        return FALSE;
    }

    if(!SaveEvenData(poDBConn, dwPlayerID, pstPlayerData->stFriendsData.stDataEvenList))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SaveEvenData failed!"), MSG_MARK);
        return FALSE;
    }


    gsapi::GetDBMgr()->EndDiff(dwPlayerID);

    m_oPlayerSaveInfo.poPlayerSaveData->m_wErrCode = SUCCESS;
    m_bSuccess = TRUE;

    return TRUE;
}

BOOL CSavePlayerDataCMD::SavePlayerDataSqlByFile(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID)
{
    TCHAR aszFileDir[256] = {0};
    TCHAR aszFilePath[256] = {0};
    CPlayerMgr::Instance()->GetTodaySavePlayerPath(aszFileDir, dwPlayerID);
    BOOL bRet = SDDirectoryExists(aszFileDir);
    if ( !bRet)
    {
        SDCreateDirectory(aszFileDir, TRUE);
    }
    CSDDateTime oDateTime(SDTimeSecs());
    SDSnprintf(aszFilePath, 256 - 1, "%s/%d%02d%02d%02d%02d%02d.sql", aszFileDir, oDateTime.GetYear(), oDateTime.GetMonth(), oDateTime.GetDay(),
               oDateTime.GetHour(), oDateTime.GetMinute(), oDateTime.GetSecond());
    ofstream outFile;
    outFile.open(aszFilePath, ios::trunc | ios::out);
    vector<string> vecAllSql = GetAllSqlBySaveText(poDBConn);
    for (vector<string>::iterator itr = vecAllSql.begin(); itr != vecAllSql.end(); itr++)
    {
        itr->append("\n");
        outFile.write(itr->c_str(), itr->size());
    }
    outFile.close();

    return TRUE;
}

VOID CSavePlayerDataCMD::SetUserData(VOID* pUserData)
{
    m_oPlayerSaveInfo = *(SPlayerSaveInfo*)(pUserData);

    //获得上次该玩家的存储信息
    //auto *pLastData=( (CGSApplication *)SDGetApp() )->GetDBMgr()->GetLastSaveData(
    //	m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID );
    //if( NULL!=pLastData )
    //{
    //	m_oPlayerSaveInfo.poPlayerSaveData->m_stLastPlayerData=*pLastData;
    //}
}

vector<string> CSavePlayerDataCMD::GetAllSql(SGDP::ISDDBConnection* poDBConn)
{
    vector<string> vecStrSql;
    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        if(GetPlayerBaseSql( poDBConn, pszSql, &m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData ))
        {
            vecStrSql.push_back( pszSql );
        }
    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bChanged;
        BOOL bRet = GetPlayerExtSql( poDBConn, pszSql, &m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData, bChanged );
        if( bRet && bChanged )
        {
            vecStrSql.push_back( pszSql );
        }
    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

        BOOL bRet = GetSaveBagSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                   m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBagEquipData, 
								   m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBagGoodsData,
								   m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBagJewelryInfo);

        if(bRet)
        {
            vecStrSql.push_back( pszSql );
        }
    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bChanged;

        BOOL bRet = GetSaveBuildSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                     m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBuildData, bChanged );

        if( bRet && bChanged )
        {
            vecStrSql.push_back( pszSql );
        }
    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bChanged;

        BOOL bRet = GetSaveHeroBaseSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                        m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stHeroData, bChanged );

        if( bRet && bChanged )
        {
            vecStrSql.push_back( pszSql );
        }
    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bChanged;

        BOOL bRet = GetSaveHeroExtSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                       m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stHeroData, bChanged );
        if( bRet && bChanged )
        {
            vecStrSql.push_back( pszSql );
        }
    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bChanged;

        BOOL bRet = GetSaveInstanceSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                        m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stInstanceData.stCommonInstanceData, bChanged );

        if( bRet && bChanged )
        {
            vecStrSql.push_back( pszSql );
        }
    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bChanged;

        BOOL bRet = GetSaveInstanceSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                        m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stInstanceData.stEliteInstanceData, bChanged );

        if( bRet && bChanged )
        {
            vecStrSql.push_back( pszSql );
        }

    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bChanged;

        BOOL bRet = GetSaveInstanceSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                        m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stInstanceData.stClimbTowerInstanceData, bChanged );

        if( bRet && bChanged )
        {
            vecStrSql.push_back( pszSql );
        }

    }

    {
        BOOL SaveTask(SGDP::ISDDBConnection * poDBConn, UINT32 dwPlayerID, DT_TASK_DATA_LIST & stTaskData);
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bChanged;

        BOOL bRet = GetSaveTaskSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                    m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stTaskData, bChanged );

        if( bRet && bChanged )
        {
            vecStrSql.push_back( pszSql );
        }
    }
    return vecStrSql;
}

vector<string> CSavePlayerDataCMD::GetAllSqlBySaveText(SGDP::ISDDBConnection* poDBConn)
{
    UINT32 dwPlayerID = m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID;
    TCHAR aszDelSql[256] = {0};
    vector<string> vecStrSql;
    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

        if(GetPlayerBaseSql( poDBConn, pszSql, &m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData ))
        {
            string strSql(pszSql);
            string::size_type pos1 = strSql.find("AllOnlineTime");
            string::size_type pos2 = strSql.find("UpdateTime");
            strSql.erase(pos1, pos2 - pos1);
            vecStrSql.push_back( strSql );
        }
    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bRet = GetPlayerExSqlBySaveLocal( poDBConn, pszSql, &m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData );
        if( bRet )
        {
            memset(aszDelSql, 0, 256);
            SDSprintf(aszDelSql, "delete from player_ext where PlayerID=%u;", dwPlayerID);
            vecStrSql.push_back(aszDelSql);
            vecStrSql.push_back( pszSql );
        }
    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

        BOOL bRet = GetSaveBagSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                   m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBagEquipData, 
								   m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBagGoodsData,
								   m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBagJewelryInfo);

        if(bRet)
        {
            memset(aszDelSql, 0, 256);
            SDSprintf(aszDelSql, "delete from player_bags where PlayerID=%u;", dwPlayerID);
            vecStrSql.push_back(aszDelSql);
            vecStrSql.push_back( pszSql );
        }
    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bChanged;

        BOOL bRet = GetSaveBuildSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                     m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBuildData, bChanged, FALSE);

        if( bRet )
        {
            memset(aszDelSql, 0, 256);
            SDSprintf(aszDelSql, "delete from player_builds where PlayerID=%u;", dwPlayerID);
            vecStrSql.push_back(aszDelSql);
            vecStrSql.push_back( pszSql );
        }
    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bChanged;

        BOOL bRet = GetSaveHeroBaseSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                        m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stHeroData, bChanged, FALSE );

        if( bRet  )
        {
            memset(aszDelSql, 0, 256);
            SDSprintf(aszDelSql, "delete from player_heros where PlayerID=%u;", dwPlayerID);
            vecStrSql.push_back(aszDelSql);
            vecStrSql.push_back( pszSql );
        }
    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bChanged;

        BOOL bRet = GetSaveHeroExtSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                       m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stHeroData, bChanged, TRUE );
        if( bRet )
        {
            memset(aszDelSql, 0, 256);
            SDSprintf(aszDelSql, "delete from player_hero_ext where PlayerID=%u;", dwPlayerID);
            vecStrSql.push_back(aszDelSql);
            vecStrSql.push_back( pszSql );
        }
    }

    {
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bChanged;

        BOOL bRet = GetSaveInstanceSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                        m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stInstanceData.stCommonInstanceData, bChanged, FALSE );

        if( bRet )
        {
            memset(aszDelSql, 0, 256);
            SDSprintf(aszDelSql, "delete from player_instances where PlayerID=%u;", dwPlayerID);
            vecStrSql.push_back(aszDelSql);
            vecStrSql.push_back( pszSql );
        }

        memset(pszSql, 0, MAX_SQL_LEN);
        bRet = GetSaveInstanceSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                   m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stInstanceData.stEliteInstanceData, bChanged, FALSE );

        if( bRet )
        {
            vecStrSql.push_back( pszSql );
        }

        memset(pszSql, 0, MAX_SQL_LEN);
        bRet = GetSaveInstanceSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                   m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stInstanceData.stClimbTowerInstanceData, bChanged, FALSE );

        if( bRet )
        {
            vecStrSql.push_back( pszSql );
        }

    }

    {
        BOOL SaveTask(SGDP::ISDDBConnection * poDBConn, UINT32 dwPlayerID, DT_TASK_DATA_LIST & stTaskData);
        CAutoSqlBuf oAutoSqlBuf;
        CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
        BOOL bChanged;

        BOOL bRet = GetSaveTaskSql( poDBConn, pszSql, m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stBaseData.dwID,
                                    m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.stPlayerNoPvpData.stTaskData, bChanged, FALSE );

        if( bRet  )
        {
            memset(aszDelSql, 0, 256);
            SDSprintf(aszDelSql, "delete from player_tasks where PlayerID=%u;", dwPlayerID);
            vecStrSql.push_back(aszDelSql);
            vecStrSql.push_back( pszSql );
        }
    }
    return vecStrSql;
}

BOOL CSavePlayerDataCMD::GetPlayerExSqlBySaveLocal( SGDP::ISDDBConnection* poDBConn, char *pszSql, DT_PLAYER_NOPVP_DATA* pstPlayerData)
{
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CAutoSqlBuf oSqlValues;
    CAutoSqlBuf oAutoTmp;

    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    CHAR* pszTmp = oAutoTmp.GetSqlBuf();
    CHAR* pszSqlValues = oSqlValues.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp) || (NULL == pszSqlValues))
    {
        SYS_CRITICAL(_SDT("[ % s: % d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    INT32 nRet = 0;
    UINT16 wDataNum = 0;
    UINT32 dwPlayerID = pstPlayerData->stBaseData.dwID;

    SDSprintf(pszSql, "insert into player_ext (PlayerID, KeyID, Value, UpdateTime ) values ");

    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stLocaleData, LocaleData, EMDT_LocaleData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stCurState, CurState, EMDT_CurState);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stOnlineEncRecdData, OnlineEncRecdData, EMDT_OnlineEncRecdData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stLoginEncRecdData, LoginEncRecdData, EMDT_LoginEncRecdData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stMoneyTreeData, MoneyTreeData, EMDT_MoneyTreeData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stInstanceData.stCommonBattleRcd, CommonBattleRcd, EMDT_CommonBattleRcd);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stInstanceData.stEliteBattleRcd, EliteBattleRcd, EMDT_EliteBattleRcd);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stJingJieData, JingJieData, EMDT_JingJieData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stPhystrengthData, PhystrengthData, EMDT_PhystrengthData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stFewParamData, FewParamData, EMDT_FewParamData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stCropData, CropData, EMDT_CropData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stScienceTree, ScienceTree, EMDT_ScienceTree);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stFormationInfo, FormationInfo, EMDT_FormationInfo);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stEatBreadInfo, EatBreadInfo, EMDT_EatBreadInfo);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stYabiaoInfo.stYabiaoBaseInfo, YabiaoInfo, EMDT_YabiaoInfo);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stInstanceData.stClimbTowerBattleRcd, ClimbTowerBattleRcd, EMDT_ClimbTowerBattleRcd);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stYabiaoInfo.stYabiaoBattleLog, YabiaoBattleLog, EMDT_YabiaoBattleLog);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stClimbTowerData, ClimbTowerData, EMDT_ClimbTowerData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stAwakenData, AwakenData, EMDT_AwakenData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stVipExtData, VipExtData, EMDT_VipExtData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stLuaActivityRecordData, LuaActivityRecordData, EMDT_LuaActivityRecordData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stGasCollectData, GasCollectData, EMDT_GasCollectData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stBossBattlePlayerDataInfo.stBossBattleData, BossBattleData, EMDT_BossBattleData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stActivityEncourageData, ActivityEncourageData, EMDT_ACTIVITY_ENCOURAGE);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stBossBattlePlayerDataInfo.stBossBBLData, BossBBLData, EMDT_BossBBL);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stRdChallengeData, RdChallengeData, EMDT_RdChallengeData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stWorshipData, WorshipData, EMDT_WorshipData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stScoreData, ScoreData, EMDT_ScoreData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stEquipComposeData, EquipComposeData, EMDT_EquipComposeData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stGatherScienceData, GatherScienceData, EMDT_GatherScienceData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stBegBlueData, BegBlueData, EMDT_BegBlueData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stFriendsData.stFriendsBaseData, FriendsBaseData, EMDT_FriendsBaseData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stCourageData, CourageData, EMDT_Courage);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stPlayerFactionData, PlayerFactionData, EMDT_PlayerFactionData);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stEquipMasterData, EquipMasterData, EMDT_EquipMaster);
    ENCODEDATA_SAVE_LOCAL(pstPlayerData->stExchangeResultLog, ExchangeResultLog, EMDT_ExchangeResultLog);
	ENCODEDATA_SAVE_LOCAL(pstPlayerData->stGvGData, GvgData, EMDT_GVGData);
	ENCODEDATA_SAVE_LOCAL(pstPlayerData->stMedalData, MedalData, EMDT_MedalData);
	ENCODEDATA_SAVE_LOCAL(pstPlayerData->stSoulData, SoulData, EMDT_SoulData);
	ENCODEDATA_SAVE_LOCAL(pstPlayerData->stPetData, PetData, EMDT_PetData);

    if(0 == wDataNum)
    {
        return FALSE;
    }
    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, " on duplicate key update player_ext.Value = values(player_ext.Value), player_ext.UpdateTime = values(player_ext.UpdateTime); ");

    return TRUE;
}