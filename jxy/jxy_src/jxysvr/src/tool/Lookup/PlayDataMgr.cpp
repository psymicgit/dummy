#include "stdafx.h"
#include "PlayDataMgr.h"
#include "protocommondata.h"
#include "dbmgr.h"

#include <sddb.h>
#include <sdutil.h>
#include <sdutil.h>




CPlayDataMgr * CPlayDataMgr::instance = NULL;
CPlayDataMgr::CPlayDataMgr()
{

}

CPlayDataMgr::~CPlayDataMgr()
{

}

CPlayDataMgr * CPlayDataMgr::Instance()
{
    if (NULL == instance)
    {
        instance = new CPlayDataMgr();
    }
    return instance;
}

void CPlayDataMgr::GetVectorID(vector<UINT> & _vecPlay, emOptType _emOptType)
{

}

void CPlayDataMgr::GetVectorID(vector<UINT> & _vecPlay, CString _strFound, emOptType _emOptType)
{

}



DT_PLAYER_DATA * CPlayDataMgr::GetDT_PLAYER_DATA(UINT uiID)
{
    CMapDT_PLAYER_DATA_Itr itr;
    itr = mapDT_PLAYER_DATA.find(uiID);
    if (itr != mapDT_PLAYER_DATA.end())
    {
        return itr->second;
    }
    return NULL;
}


BOOL CPlayDataMgr::InitDB()
{
    map<UINT, DT_PLAYER_DATA*>::iterator iter = mapDT_PLAYER_DATA.begin();
    while(iter != mapDT_PLAYER_DATA.end())
    {
        delete iter->second;
        mapDT_PLAYER_DATA.erase(iter);
        iter = mapDT_PLAYER_DATA.begin();
    }

    SGDP::ISDDBSession*	pDBSession = CDBMgr::Instance()->GetDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

    CHAR szSql[1024] = {0};
    DT_PLAYER_DATA*	pstDT_PLAYER_DATA = NULL;
    sprintf(szSql, "select * from player");
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {

        UINT32		dwMaxRank = 0;
        char			v_szTemp[1024] = {0};
        CString		v_strTemp = "";
        while(pRecordSet->GetRecord())
        {
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    return TRUE;
}


BOOL CPlayDataMgr::SaveDB(UINT uiID)
{
    CMapDT_PLAYER_DATA_Itr itr;
    itr = mapDT_PLAYER_DATA.find(uiID);
    if (itr == mapDT_PLAYER_DATA.end())
    {
        return FALSE;
    }

    DT_PLAYER_DATA *pstDT_PLAYER_DATA = itr->second;
    SGDP::ISDDBSession*	pDBSession = CDBMgr::Instance()->GetDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

    char		v_szData[sizeof(DT_PLAYER_BASE_DATA) + 1] = {0};
    memcpy_s(v_szData, sizeof(DT_PLAYER_BASE_DATA) + 1, &pstDT_PLAYER_DATA->stBaseData, sizeof(DT_PLAYER_BASE_DATA));
    DT_PLAYER_BASE_DATA *v_pDT_PLAYER_BASE_DATA = (DT_PLAYER_BASE_DATA*)v_szData;


    char		v_szTemp[1024] = {0};
    memcpy_s(v_szTemp, 1024, v_pDT_PLAYER_BASE_DATA->aszDispName, USERNAME_LEN);
    CString v_strTemp = v_szTemp;
    ConvertGBKToUtf8(v_strTemp);
    memset(v_pDT_PLAYER_BASE_DATA->aszDispName, 0x00, USERNAME_LEN);
    memcpy_s(v_pDT_PLAYER_BASE_DATA->aszDispName, USERNAME_LEN, v_strTemp.GetBuffer(0), v_strTemp.GetLength() + 1);

    CHAR szBaseData[ sizeof(DT_PLAYER_BASE_DATA) * 2 + 1] = {0};
    UINT32 dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stBaseData),
                       sizeof(DT_PLAYER_BASE_DATA),
                       szBaseData,
                       sizeof(szBaseData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szBagData[ sizeof(DT_BAG_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stBagData),
                                         sizeof(DT_BAG_DATA),
                                         szBagData,
                                         sizeof(szBagData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szBuildData[ sizeof(DT_BUILD_DATA_LIST) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stBuildData),
                                         sizeof(DT_BUILD_DATA_LIST),
                                         szBuildData,
                                         sizeof(szBuildData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szCurState[ sizeof(DT_STATE_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stCurState),
                                         sizeof(DT_STATE_DATA),
                                         szCurState,
                                         sizeof(szCurState));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szTaskData[ sizeof(DT_TASK_DATA_LIST) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stTaskData),
                                         sizeof(DT_TASK_DATA_LIST),
                                         szTaskData,
                                         sizeof(szTaskData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szBagEquipData[ sizeof(DT_BAG_EQUIP_DATA_LST) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stBagEquipData),
                                         sizeof(DT_BAG_EQUIP_DATA_LST),
                                         szBagEquipData,
                                         sizeof(szBagEquipData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szBagGoodsData[ sizeof(DT_BAG_GOODS_DATA_LST) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stBagGoodsData),
                                         sizeof(DT_BAG_GOODS_DATA_LST),
                                         szBagGoodsData,
                                         sizeof(szBagGoodsData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szHeroData[ sizeof(DT_HERO_DATA_LIST) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stHeroData),
                                         sizeof(DT_HERO_DATA_LIST),
                                         szHeroData,
                                         sizeof(szHeroData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szCommonBattleRcd[ sizeof(DT_INSTANCE_BATTLE_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stCommonBattleRcd),
                                         sizeof(DT_INSTANCE_BATTLE_DATA),
                                         szCommonBattleRcd,
                                         sizeof(szCommonBattleRcd));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szEliteBattleRcd[ sizeof(DT_INSTANCE_BATTLE_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stEliteBattleRcd),
                                         sizeof(DT_INSTANCE_BATTLE_DATA),
                                         szEliteBattleRcd,
                                         sizeof(szEliteBattleRcd));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szCommonInstanceData[ sizeof(DT_INSTANCE_DATA_LIST) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stCommonInstanceData),
                                         sizeof(DT_INSTANCE_DATA_LIST),
                                         szCommonInstanceData,
                                         sizeof(szCommonInstanceData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szEliteInstanceData[ sizeof(DT_INSTANCE_DATA_LIST) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stEliteInstanceData),
                                         sizeof(DT_INSTANCE_DATA_LIST),
                                         szEliteInstanceData,
                                         sizeof(szEliteInstanceData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szLocaleData[ sizeof(DT_LOCALE_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stLocaleData),
                                         sizeof(DT_LOCALE_DATA),
                                         szLocaleData,
                                         sizeof(szLocaleData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szLoginEncRecdData[ sizeof(DT_LOGIN_ENCOURAGE_RECORD_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stLoginEncRecdData),
                                         sizeof(DT_LOGIN_ENCOURAGE_RECORD_DATA),
                                         szLoginEncRecdData,
                                         sizeof(szLoginEncRecdData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szMoneyTreeData[ sizeof(DT_MONEY_TREE_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stMoneyTreeData),
                                         sizeof(DT_MONEY_TREE_DATA),
                                         szMoneyTreeData,
                                         sizeof(szMoneyTreeData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szOnhookRcdData[ sizeof(DT_ONHOOK_RCD_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stOnhookRcdData),
                                         sizeof(DT_ONHOOK_RCD_DATA),
                                         szOnhookRcdData,
                                         sizeof(szOnhookRcdData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szOnlineEncRecdData[ sizeof(DT_ONLINE_ENCOURAGE_RECORD_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stOnlineEncRecdData),
                                         sizeof(DT_ONLINE_ENCOURAGE_RECORD_DATA),
                                         szOnlineEncRecdData,
                                         sizeof(szOnlineEncRecdData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szPhystrengthData[ sizeof(DT_PHYSTRENGTH_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stPhystrengthData),
                                         sizeof(DT_PHYSTRENGTH_DATA),
                                         szPhystrengthData,
                                         sizeof(szPhystrengthData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szSkillData[ sizeof(DT_SKILL_DATA_LIST) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stSkillData),
                                         sizeof(DT_SKILL_DATA_LIST),
                                         szSkillData,
                                         sizeof(szSkillData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szCropData[ sizeof(DT_CROP_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stCropData),
                                         sizeof(DT_CROP_DATA),
                                         szCropData,
                                         sizeof(szCropData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szFewParamData[ sizeof(DT_FEW_PARAM_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stFewParamData),
                                         sizeof(DT_FEW_PARAM_DATA),
                                         szFewParamData,
                                         sizeof(szFewParamData));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    CHAR szScienceTree[ sizeof(DT_SCIENCE_TREE_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stScienceTree),
                                         sizeof(DT_SCIENCE_TREE_DATA),
                                         szScienceTree,
                                         sizeof(szScienceTree));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }


    CHAR szFormationInfo[ sizeof(DT_FORMATION_DATA) * 2 + 1] = {0};
    dwRetCode = pDBSession->EscapeString((CHAR *) & (pstDT_PLAYER_DATA->stFormationInfo),
                                         sizeof(DT_FORMATION_DATA),
                                         szFormationInfo,
                                         sizeof(szFormationInfo));
    if(dwRetCode <= 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }

    static char		pszSql[655350] = {0};
    sprintf(pszSql, "update player set "\
            "BaseData='%s', Bag='%s', Build='%s', Curstate='%s', "\
            "Curtask='%s', Equip='%s', Goods='%s', Hero='%s', "\
            "CommonBattleRcd='%s', EliteBattleRcd='%s', CommonInstanceData='%s', EliteInstanceData='%s', "\
            "Locale='%s', LoginEncourageRecord='%s', MoneyTree='%s', OnhookRecord='%s', OnlineEncourageRecord='%s', "\
            "Phystrength='%s', Skill='%s', Crop='%s', FewParam='%s',"\
            "FormationInfo='%s', ScienceTree='%s', "\
            "UpdateTime=now(), Gold='%llu', LastOrderID='%s' where ID ='%u'; ",
            szBaseData, szBagData, szBuildData, szCurState,
            szTaskData, szBagEquipData, szBagGoodsData, szHeroData,
            szCommonBattleRcd, szEliteBattleRcd, szCommonInstanceData, szEliteInstanceData,
            szLocaleData, szLoginEncRecdData, szMoneyTreeData, szOnhookRcdData, szOnlineEncRecdData,
            szPhystrengthData, szSkillData, szCropData, szFewParamData,
            szFormationInfo, szScienceTree,
            pstDT_PLAYER_DATA->stBaseData.qwGold, pstDT_PLAYER_DATA->szLastOrderID, pstDT_PLAYER_DATA->stBaseData.dwID);

    INT32 nRet = 0;
    UINT32 dwErrorCode = 0;
    ISDDBRecordSet*pRecordSet = NULL;
    string strErrorDes;
    nRet = pDBSession->ExecuteSqlRs(pszSql, &pRecordSet, &dwErrorCode, &strErrorDes );
    if(NULL != pRecordSet)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed!Error:%s Sql:%s"), MSG_MARK, strErrorDes.c_str(), pszSql);
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    if(nRet < 0)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed!Error:%s Sql:%s"), MSG_MARK, strErrorDes.c_str(), pszSql);
        return FALSE;
    }

    return TRUE;
}



