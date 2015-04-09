#include <db/autosqlbuf.h>
#include "createplayercmd.h"
#include "dbcommoperate.h"
#include <db/dbsqlstring.h>
#include <sdutil.h>
#include "server/utility.h"

#include <framework/gsapi.h>
#include <net/db/dbprocessor.h>
#include <sdloggerimpl.h>

CCreatePlayerCMD::CCreatePlayerCMD()
{
    m_dwPlayerDBID = 0;
    memset(&m_stReq, 0, sizeof(m_stReq));
    memset(&m_stAck, 0, sizeof(m_stAck));
}


CCreatePlayerCMD::~CCreatePlayerCMD()
{
}

VOID CCreatePlayerCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    SDTRY_BEGIN

    m_stAck.wErrCode = FAIL;//
    poDBConn->BeginTransaction();
    if(CreatePalyerData(poDBConn, m_stReq.dwUserID, m_stReq.aszUserName, m_stReq.aszDeviceID, m_stReq.aszNotifyID, &m_stReq.stPlayerData.stPlayerNoPvpData,  &m_stReq.stPlayerData.stPlayerPvpData))
    {
        poDBConn->CommitTransaction();
    }
    else
    {
        poDBConn->RollbackTransaction();
    }
    m_stAck.byGmFlag = m_stReq.byGmFlag;

    SDTRY_END

}

VOID CCreatePlayerCMD::OnExecuted()
{
    SDTRY_BEGIN

    gsapi::GetDBProcessor()->OnCreatePlayerAck(m_stAck, m_dwTransID);
    gsapi::GetDBMgr()->SetSaveProcessed();

    if(!m_bSuccess)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: create player failed! UserName[%s], DeviceID[%s], pNotifyID[%s]"), MSG_MARK, m_stReq.aszUserName, m_stReq.aszDeviceID, m_stReq.aszNotifyID);
    }

    SDTRY_END
}

BOOL CCreatePlayerCMD::CreatePalyerData(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, DT_PLAYER_NOPVP_DATA* pstPlayerData, DT_PLAYER_PVP_DATA* pstPlayerPvpData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);
    m_bSuccess = FALSE;

    if(!AddPlayerBase(poDBConn, dwPlayerID, pszUserName, pszDeviceID, pszNotifyID, pstPlayerData, pstPlayerPvpData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddPlayerBase failed!"), MSG_MARK);
        return FALSE;
    }

    if(!AddPlayerExt(poDBConn, dwPlayerID, pstPlayerData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddPlayerBase failed!"), MSG_MARK);
        return FALSE;
    }

    if(!AddBag(poDBConn, dwPlayerID, pstPlayerData->stBagEquipData, pstPlayerData->stBagGoodsData, pstPlayerData->stBagJewelryInfo))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddBag failed!"), MSG_MARK);
        return FALSE;
    }

    if(!AddBuild(poDBConn, dwPlayerID, pstPlayerData->stBuildData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddBuild failed!"), MSG_MARK);
        return FALSE;
    }

    if(!AddHeroBase(poDBConn, dwPlayerID, pstPlayerData->stHeroData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddHeroBase failed!"), MSG_MARK);
        return FALSE;
    }

    if(!AddHeroExt(poDBConn, dwPlayerID, pstPlayerData->stHeroData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddHeroExt failed!"), MSG_MARK);
        return FALSE;
    }

    if(!AddInstance(poDBConn, dwPlayerID, pstPlayerData->stInstanceData.stCommonInstanceData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddInstance failed!"), MSG_MARK);
        return FALSE;
    }

    if(!AddInstance(poDBConn, dwPlayerID, pstPlayerData->stInstanceData.stEliteInstanceData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddInstance failed!"), MSG_MARK);
        return FALSE;
    }

    if(!AddInstance(poDBConn, dwPlayerID, pstPlayerData->stInstanceData.stClimbTowerInstanceData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddInstance failed!"), MSG_MARK);
        return FALSE;
    }

    if(!AddTask(poDBConn, dwPlayerID, pstPlayerData->stTaskData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddTask failed!"), MSG_MARK);
        return FALSE;
    }

    if(!AddPvp(poDBConn, dwPlayerID, *pstPlayerPvpData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: AddPvp failed!"), MSG_MARK);
        return FALSE;
    }


    m_bSuccess = TRUE;

    return TRUE;
}

BOOL CCreatePlayerCMD::AddPlayerBase(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, DT_PLAYER_NOPVP_DATA* pstPlayerData, DT_PLAYER_PVP_DATA* pstPlayerPvpData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);
    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CAutoSqlBuf oSqlValues;
    CAutoSqlBuf oAutoTmp;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    CHAR* pszTmp = oAutoTmp.GetSqlBuf();
    CHAR* pszSqlValues = oSqlValues.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp) || (NULL == pszSqlValues))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    sprintf(pszSql, "insert into players(CreateTime, UpdateTime, LastLoginTime, LastLogoffTime, TodayFirstLoginTime, LastOrderID");
    sprintf(pszSqlValues, " values (Now(), Now(), Now(), Now(), Now(), '' ");
    INT32 nRet = 0;
    DT_PLAYER_BASE_DATA& stBaseData = pstPlayerData->stBaseData;
    ENCODEDATA3(stBaseData.stGuideRecordData, GuideRecordData, MAXENLEN_DT_GUIDE_RECORD_DATA);
    ADDSIMPLEFIELD(PlayerID, % u, dwPlayerID);
    ADDSIMPLEFIELD(DeviceID, % s, pszDeviceID);
    ADDSIMPLEFIELD(NotifyID, % s, pszNotifyID);
    ADDSIMPLEFIELD(UserName, % s, pszUserName);
    ADDSIMPLEFIELD(DispName, % s, stBaseData.aszDispName);
    ADDSIMPLEFIELD(Pay, % u,  stBaseData.dwTotalAmount);
    ADDSIMPLEFIELD(VipLevel, % u,  stBaseData.byVipLevel);
    ADDSIMPLEFIELD(Gold, % llu,  stBaseData.qwGold);
    ADDSIMPLEFIELD(Coin, % llu, stBaseData.qwCoin );
    ADDSIMPLEFIELD(Phystrength, % u, stBaseData.wPhyStrength );
    ADDSIMPLEFIELD(Science, % llu, stBaseData.qwScience );
    ADDSIMPLEFIELD(Story, % llu, stBaseData.qwStory );
    ADDSIMPLEFIELD(JingJie, % llu, stBaseData.qwJingJie );
    ADDSIMPLEFIELD(BlueGas, % llu, stBaseData.qwBlueGas );
    ADDSIMPLEFIELD(PurpleGas, % llu, stBaseData.qwPurpleGas );
    ADDSIMPLEFIELD(BagOpenNum, % u, stBaseData.byBagOpenNum );
    ADDSIMPLEFIELD(Firstattack, % u, stBaseData.dwFirstAttack );
    ADDSIMPLEFIELD(Power, % u, stBaseData.dwPower );
    ADDSIMPLEFIELD(MainHeroKindID, % u, pstPlayerData->stHeroData.astHeroInfoLst[0].stHeroBaseData.wKindID);
    ADDSIMPLEFIELD(Level, % u, pstPlayerData->stHeroData.astHeroInfoLst[0].stHeroBaseData.wLevel);
    ADDSIMPLEFIELD(RobotFunc, % u, stBaseData.byRobotFunc);
    ADDSIMPLEFIELD(Rank, % u, pstPlayerPvpData->stRaceData.stRaceBaseData.dwRank); //
    ADDSIMPLEFIELD(CaptureOwnerID, % u, pstPlayerPvpData->stPlunderData.stPlunderBaseData.dwCaptureOwnerPlayerID); //
    ADDSIMPLEFIELD(OpenPlunder, % u, pstPlayerPvpData->stPlunderData.stPlunderBaseData.byHaveDataFlag); //
    ADDSIMPLEFIELD(Silence, % u, 0 ); //禁言功能，默认为0，可发言
    ADDSIMPLEFIELD(ExtData, % u, stBaseData.dwExtData);
    ADDSIMPLEFIELD(EatBreadNotify, % u, pstPlayerData->stEatBreadInfo.byNotity);
    ADDSIMPLEFIELD(BossBattleNotify, % u, pstPlayerData->stBossBattlePlayerDataInfo.stBossBattleData.byNotity);
    ADDSIMPLEFIELD(CourageNofity, % u, pstPlayerData->stCourageData.byNotity);
    ADDSIMPLEFIELD(Version, % u, PROTOCOL_VERSION);
    COMBSQL();

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        m_stAck.wErrCode = FAIL;
        return FALSE;
    }
    m_stAck.wErrCode = SUCCESS;
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CCreatePlayerCMD::AddPlayerExt(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_NOPVP_DATA* pstPlayerData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CAutoSqlBuf oSqlValues;
    CAutoSqlBuf oAutoTmp;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    CHAR* pszTmp = oAutoTmp.GetSqlBuf();
    CHAR* pszSqlValues = oSqlValues.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp) || (NULL == pszSqlValues))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    //memset(pszSql, 0, MAX_SQL_LEN);
    sprintf(pszSql, "insert into player_ext (PlayerID, KeyID, Value, UpdateTime ) values ");

    INT32 nRet = 0;

    ENCODEDATA22(pstPlayerData->stLocaleData, LocaleData, EMDT_LocaleData, MAXENLEN_DT_LOCALE_DATA);
    ENCODEDATA2(pstPlayerData->stCurState, CurState, EMDT_CurState, MAXENLEN_DT_STATE_DATA);
    ENCODEDATA2(pstPlayerData->stOnlineEncRecdData, OnlineEncRecdData, EMDT_OnlineEncRecdData, MAXENLEN_DT_ONLINE_ENCOURAGE_RECORD_DATA);
    ENCODEDATA2(pstPlayerData->stLoginEncRecdData, LoginEncRecdData, EMDT_LoginEncRecdData, MAXENLEN_DT_LOGIN_ENCOURAGE_RECORD_DATA);
    ENCODEDATA2(pstPlayerData->stMoneyTreeData, MoneyTreeData, EMDT_MoneyTreeData, MAXENLEN_DT_MONEY_TREE_DATA);
    ENCODEDATA2(pstPlayerData->stInstanceData.stCommonBattleRcd, CommonBattleRcd, EMDT_CommonBattleRcd, MAXENLEN_DT_INSTANCE_BATTLE_DATA);
    ENCODEDATA2(pstPlayerData->stInstanceData.stEliteBattleRcd, EliteBattleRcd, EMDT_EliteBattleRcd, MAXENLEN_DT_INSTANCE_BATTLE_DATA);
    ENCODEDATA2(pstPlayerData->stJingJieData, JingJieData, EMDT_JingJieData, MAXENLEN_DT_JINGJIE_DATA);
    ENCODEDATA2(pstPlayerData->stPhystrengthData, PhystrengthData, EMDT_PhystrengthData, MAXENLEN_DT_PHYSTRENGTH_DATA);
    ENCODEDATA2(pstPlayerData->stFewParamData, FewParamData, EMDT_FewParamData, MAXENLEN_DT_FEW_PARAM_DATA);
    ENCODEDATA2(pstPlayerData->stCropData, CropData, EMDT_CropData, MAXENLEN_DT_CROP_DATA);
    ENCODEDATA2(pstPlayerData->stScienceTree, ScienceTree, EMDT_ScienceTree, MAXENLEN_DT_SCIENCE_TREE_DATA);
    ENCODEDATA2(pstPlayerData->stFormationInfo, FormationInfo, EMDT_FormationInfo, MAXENLEN_DT_FORMATION_DATA);
    ENCODEDATA2(pstPlayerData->stEatBreadInfo, EatBreadInfo, EMDT_EatBreadInfo, MAXENLEN_DT_EAT_BREAD_DATA);
    ENCODEDATA2(pstPlayerData->stYabiaoInfo.stYabiaoBaseInfo, YabiaoInfo, EMDT_YabiaoInfo, MAXENLEN_DT_YABIAO_BASE_DATA);
    ENCODEDATA2(pstPlayerData->stInstanceData.stClimbTowerBattleRcd, ClimbTowerBattleRcd, EMDT_ClimbTowerBattleRcd, MAXENLEN_DT_INSTANCE_BATTLE_DATA);
    ENCODEDATA2(pstPlayerData->stYabiaoInfo.stYabiaoBattleLog, YabiaoBattleLog, EMDT_YabiaoBattleLog, MAXENLEN_DT_YABIAO_BATTLELOG_DATA_LIST);
    ENCODEDATA2(pstPlayerData->stClimbTowerData, ClimbTowerData, EMDT_ClimbTowerData, MAXENLEN_DT_CLIMB_TOWER_DATA);
    ENCODEDATA2(pstPlayerData->stAwakenData, AwakenData, EMDT_AwakenData, MAXENLEN_DT_AWAKEN_DATA);
    ENCODEDATA2(pstPlayerData->stVipExtData, VipExtData, EMDT_VipExtData, MAXENLEN_DT_VIP_EXT_DATA);
    ENCODEDATA2(pstPlayerData->stLuaActivityRecordData, LuaActivityRecordData, EMDT_LuaActivityRecordData, MAXENLEN_DT_LUAACTIVITY_RECORD_LIST_DATA);
    ENCODEDATA2(pstPlayerData->stGasCollectData, GasCollectData, EMDT_GasCollectData, MAXENLEN_DT_COLLECT_GAS_DATA);
    ENCODEDATA2(pstPlayerData->stBossBattlePlayerDataInfo.stBossBattleData, BossBattleData, EMDT_BossBattleData, MAXENLEN_DT_BOSS_BATTLE_PLAYER_DATA);


    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        m_stAck.wErrCode = FAIL;
        return FALSE;
    }
    m_stAck.wErrCode = SUCCESS;
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CCreatePlayerCMD::AddBag(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_BAG_EQUIP_DATA_LST& stBagEquipData, DT_BAG_GOODS_DATA_LST& stBagGoodsData, DT_BAG_JEWELRY_DATA_LST& stBagJewelryData)
{
    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CAutoSqlBuf oSqlValues;
    CAutoSqlBuf oAutoTmp;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
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
		SYS_CRITICAL(_SDT("[%s: %d]: stBagJewelryData.EncodeSaveData is failed"), MSG_MARK);
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

    sprintf(pszSql, "insert into player_bags(PlayerID, Equip, Goods, Jewelry, UpdateTime) values('%u', '%s', '%s', '%s', now() ); ", dwPlayerID, szEquip, szGoods, szJewelry);


    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        m_stAck.wErrCode = FAIL;
        return FALSE;
    }
    m_stAck.wErrCode = SUCCESS;
    m_bSuccess = TRUE;
    return TRUE;

    m_bSuccess = TRUE;

    return TRUE;
}


BOOL CCreatePlayerCMD::AddBuild(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_BUILD_DATA_LIST& stBuildData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    if(0 == stBuildData.wBuildNum)
    {
        return TRUE;
    }

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    UINT16 wBuildNum = stBuildData.wBuildNum;
    sprintf(pszSql, "insert into player_builds(PlayerID, BuildID, Value1, Value2, Value3, Sort, HaveOpenFlag, UpdateTime) values ");
    for(UINT16 wIdx = 0; wIdx < wBuildNum; wIdx++)
    {
        //memset(pszBuff, 0, sizeof(MAX_SQL_LEN));
        DT_BUILD_BASE_DATA& stBaseData = stBuildData.astBuildInfoLst[wIdx];
        if(0 != wIdx)
        {
            strcat(pszSql, ", ");
        }

        sprintf(pszBuff, "( '%u', '%u', '%llu', '%llu', '%u', '%u', '%u', now() )",
                dwPlayerID, stBaseData.byKindID, stBaseData.qwValue1, stBaseData.qwValue2, stBaseData.byValue3, stBaseData.bySort, stBaseData.byHaveOpenFlag);

        strcat(pszSql, pszBuff);
    }

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        m_stAck.wErrCode = FAIL;
        return FALSE;
    }
    m_stAck.wErrCode = SUCCESS;
    m_bSuccess = TRUE;
    return TRUE;

}

BOOL CCreatePlayerCMD::AddHeroBase(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_HERO_DATA_LIST& stHeroData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    if(0 == stHeroData.wHeroNum)
    {
        return TRUE;
    }

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CAutoSqlBuf oSqlValues;
    CAutoSqlBuf oAutoTmp;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    CHAR* pszTmp = oAutoTmp.GetSqlBuf();
    CHAR* pszSqlValues = oSqlValues.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp) || (NULL == pszSqlValues))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    UINT16 wHeroNum = stHeroData.wHeroNum;
    sprintf(pszSql, "insert into player_heros (PlayerID, HeroID, Coach, Pos, Talent, Level, Exp, FormationIdx, \
		RecuitedFlag, VisitNum, LastUseDrugLevel, LastUseDrugTime, UseExpdrugPerDay, LastUseExpDrugTime, BattleAttribute, HideBattleAttr, UpdateAttrInfo, UpdateTime ) values ");
    for(UINT16 wIdx = 0; wIdx < wHeroNum; wIdx++)
    {
        //memset(pszBuff, 0, sizeof(MAX_SQL_LEN));
        DT_HERO_BASE_DATA& stBaseData = stHeroData.astHeroInfoLst[wIdx].stHeroBaseData;
        if(0 != wIdx)
        {
            strcat(pszSql, ", ");
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

        sprintf(pszBuff, "( '%u', '%u', '%u', '%u', '%u', '%u', '%llu', '%u', \
						 '%u', '%u', '%u', '%s', \
						 '%u', '%s', '%s', '%s', '%s', now() )",
                dwPlayerID, stBaseData.wKindID, stBaseData.byCoach, stBaseData.byPos, stBaseData.wTalent, stBaseData.wLevel, stBaseData.qwExperience, stBaseData.byFormationIdx,
                stBaseData.byRecuitedFlag, stBaseData.byVisitNum, stBaseData.byLastUseDrugLevel,  SDTimeToString(CSDDateTime(stBaseData.qwLastUseDrugTime)).c_str(),
                stBaseData.byUseExperienceDrugPerDay, SDTimeToString(CSDDateTime(stBaseData.qwLastUseExperienceDrugTime)).c_str(), szBattleAttribute, szHideBattleAttr, szUpdateAttrInfo);

        strcat(pszSql, pszBuff);
    }

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        m_stAck.wErrCode = FAIL;
        return FALSE;
    }
    m_stAck.wErrCode = SUCCESS;
    m_bSuccess = TRUE;
    return TRUE;
}


BOOL CCreatePlayerCMD::AddHeroExt(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_HERO_DATA_LIST& stHeroData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    if(0 == stHeroData.wHeroNum)
    {
        return TRUE;
    }

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CAutoSqlBuf oSqlValues;
    CAutoSqlBuf oAutoTmp;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
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
    for(UINT16 wIdx = 0; wIdx < wHeroNum; wIdx++)
    {
        UINT16 wHeroID = stHeroData.astHeroInfoLst[wIdx].stHeroBaseData.wKindID;

        DT_HERO_EQUIP_DATA_LST& stEquipDataInfo = stHeroData.astHeroInfoLst[wIdx].stEquipDataInfo;
        //memset(pszBuff, 0, MAX_SQL_LEN);
        INT32 nRet = stEquipDataInfo.EncodeSaveData(pszBuff, MAX_SQL_LEN);
        if(nRet <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: stBagEquipData.EncodeSaveData is failed"), MSG_MARK);
            return FALSE;
        }
        CHAR szEquipDataInfo[MAXENLEN_DT_HERO_EQUIP_DATA_LST * 2 + 1] = {0};
        UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,
                           nRet,
                           szEquipDataInfo,
                           sizeof(szEquipDataInfo));
        if(dwRetCode <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
            return FALSE;
        }

        if(0 != wIdx)
        {
            strcat(pszSql, ", ");
        }
        sprintf(pszBuff, "( '%u', '%u', '%u', '%s', now() )",
                dwPlayerID, wHeroID, EHEDT_Equip, szEquipDataInfo);
        strcat(pszSql, pszBuff);

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
        sprintf(pszBuff, ", ( '%u', '%u', '%u', '%s', now() )",
                dwPlayerID, wHeroID, EHEDT_Drug, szDrugDataInfo);
        strcat(pszSql, pszBuff);

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
        sprintf(pszBuff, ", ( '%u', '%u', '%u', '%s', now() )",
                dwPlayerID, wHeroID, EHEDT_Godweapon, szGodweaponInfo);
        strcat(pszSql, pszBuff);

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
        sprintf(pszBuff, ", ( '%u', '%u', '%u', '%s', now() )",
                dwPlayerID, wHeroID, EHEDT_Skill, szSkillInfo);
        strcat(pszSql, pszBuff);

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
        sprintf(pszBuff, ", ( '%u', '%u', '%u', '%s', now() )",
                dwPlayerID, wHeroID, EHEDT_Study, szStudyInfo);
        strcat(pszSql, pszBuff);

        //
        DT_ABSORB_DATA_LST& stAbsorbInfo = stHeroData.astHeroInfoLst[wIdx].stAbsorbInfo;
        //memset(pszBuff, 0, MAX_SQL_LEN);
        nRet = stAbsorbInfo.EncodeSaveData(pszBuff, MAX_SQL_LEN);
        if(nRet <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: stBagEquipData.EncodeSaveData is failed"), MSG_MARK);
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
        sprintf(pszBuff, ", ( '%u', '%u', '%u', '%s', now() )",
                dwPlayerID, wHeroID, EHEDT_Absorb, szAbsorbInfo);
        strcat(pszSql, pszBuff);
    }

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        m_stAck.wErrCode = FAIL;
        return FALSE;
    }
    m_stAck.wErrCode = SUCCESS;
    m_bSuccess = TRUE;
    return TRUE;
}


BOOL CCreatePlayerCMD::AddInstance(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_INSTANCE_DATA_LIST& stInstanceData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    if(0 == stInstanceData.wInstanceNum)
    {
        return TRUE;
    }

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    UINT16 wInstanceNum = stInstanceData.wInstanceNum;
    sprintf(pszSql, "insert into player_instances(PlayerID, SceneIdx, TownIdx, InstanceIdx, SuccessNum, FailedNum, LastPassTime, UpdateTime) values ");
    for(UINT16 wIdx = 0; wIdx < wInstanceNum; wIdx++)
    {
        //memset(pszBuff, 0, sizeof(MAX_SQL_LEN));
        DT_INSTANCE_DATA& stInstance = stInstanceData.astInstanceInfo[wIdx];
        if(0 != wIdx)
        {
            strcat(pszSql, ",");
        }

        sprintf(pszBuff, "( '%u', '%u', '%u', '%u', '%u', '%u', '%s', now() )",
                dwPlayerID, stInstanceData.wSceneIdx, stInstance.stInstanceID.byTownIdx, stInstance.stInstanceID.byInstanceIdx, stInstance.dwSuccessNum, stInstance.dwFailedNum, SDTimeToString(CSDDateTime(stInstance.qwLastPassTime)).c_str());

        strcat(pszSql, pszBuff);
    }

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        m_stAck.wErrCode = FAIL;
        return FALSE;
    }
    m_stAck.wErrCode = SUCCESS;
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CCreatePlayerCMD::AddTask(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_TASK_DATA_LIST& stTaskData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    if(0 == stTaskData.byTaskNum)
    {
        return TRUE;
    }

    m_bSuccess = FALSE;
    CAutoSqlBuf oAutoSqlBuf;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    UINT16 byTaskNum = stTaskData.byTaskNum;
    sprintf(pszSql, "insert into player_tasks(PlayerID, Line, Idx, State, StateUpdateTime, Type, Cond, UpdateTime) values ");
    for(UINT16 wIdx = 0; wIdx < byTaskNum; wIdx++)
    {
        //memset(pszBuff, 0, sizeof(MAX_SQL_LEN));
        DT_TASK_DATA& stTask = stTaskData.astTaskInfoList[wIdx];
        if(0 != wIdx)
        {
            strcat(pszSql, ",");
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

        strcat(pszSql, pszBuff);
    }

    ISDDBRecordSet*pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, NULL, &strError);
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
    }
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, dwPlayerID, strError.c_str());
        m_stAck.wErrCode = FAIL;
        return FALSE;
    }
    m_stAck.wErrCode = SUCCESS;
    m_bSuccess = TRUE;
    return TRUE;
}

BOOL CCreatePlayerCMD::AddPvp(SGDP::ISDDBConnection* poDBConn, UINT32 dwPlayerID, DT_PLAYER_PVP_DATA& stPlayerData)
{
    //CAutoCycle oAutoCycle(__FUNCTION__);

    CAutoSqlBuf oAutoSqlBuf;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CAutoSqlBuf oAutoTmp;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    CHAR* pszTmp = oAutoTmp.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    sprintf(pszSql, "insert into player_pvp (PlayerID, KeyID, Value, UpdateTime ) values ");

    INT32 nRet = 0;
    ENCODEDATA22(stPlayerData.stPlunderData.stPlunderBaseData, PlunderBaseData, EMDT_PlunderBaseData, MAXENLEN_DT_PLUNDER_BASE_DATA);
    ENCODEDATA2(stPlayerData.stPlunderData.stPlunderBattleLog, PlunderBattleLog, EMDT_PlunderBattleLog, MAXENLEN_DT_PLUNDER_BATTLELOG_DATA_LIST);
    ENCODEDATA2(stPlayerData.stPlunderData.stCaptureData, CaptureData, EMDT_CaptureData, MAXENLEN_DT_CAPTURE_DATA_LIST);
    ENCODEDATA2(stPlayerData.stEnemyData, EnemyData, EMDT_EnemyData, MAXENLEN_DT_ENEMY_DATA_LIST);
    ENCODEDATA2(stPlayerData.stRaceData.stRaceBattleLog, RaceData, EMDT_RaceData, MAXENLEN_DT_RACE_BASE_DATA);
    ENCODEDATA2(stPlayerData.stRaceData.stRaceBattleLog, RaceBattleLog, EMDT_RaceBattleLog, MAXENLEN_DT_RACE_BATTLELOG_DATA_LIST);

    strcat(pszSql, " on duplicate key update player_pvp.Value = values(player_pvp.Value);");

    UINT32 dwErr;
    string strError;
    nRet = poDBConn->ExecuteSql(pszSql, NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, dwErr, strError.c_str(), pszSql);
        m_bSuccess = FALSE;
        return FALSE;
    }


    m_bSuccess = TRUE;
    return TRUE;
}




VOID CCreatePlayerCMD::SetUserData(VOID* pUserData)
{
    memcpy(&m_stReq, pUserData, sizeof(m_stReq));
}