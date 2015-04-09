#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include "singleprammgr.h"
#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include <dll/sdframework/sdloggerimpl.h>
#include "logic/lua/luamgr.h"
#define SQL_READ_SINGLEPRAM_STRING "select * from singlepram"


IMPLEMENT_SINGLETON_PROPMGR(CSinglePramMgr);

CSinglePramMgr::CSinglePramMgr()
{
    m_wPerfectFreeGodponGold = 0;
    m_dwGoldExperienceAddValue = 0;
    m_wFreeFlushBiaoCnt = 0;
    memset(m_szAppAddress, 0x00, sizeof(m_szAppAddress));
    m_wCoachExpRate = 0;
    memset(m_szScienceDspContent, 0, sizeof(m_szScienceDspContent));
    m_wOpenOprateingActivityLevel = 15;
    m_wEveryDayRecvFlowers = 0;
}

CSinglePramMgr::~CSinglePramMgr()
{

}

BOOL CSinglePramMgr::Init()
{
    return LoadFromDB();
}

VOID CSinglePramMgr::UnInit()
{
    m_wPerfectFreeGodponGold = 0;
    m_dwGoldExperienceAddValue = 0;
    m_wFreeFlushBiaoCnt = 0;
    memset(m_szAppAddress, 0x00, sizeof(m_szAppAddress));
    m_wCoachExpRate = 0;
    memset(m_szScienceDspContent, 0, sizeof(m_szScienceDspContent));
    m_wOpenOprateingActivityLevel = 15;
	m_dwCloseAreaBoosMulti = 0;
	m_dwMaxScienceLevel = 0;
	m_dwJewelryResolveReturn = 0;
}

BOOL CSinglePramMgr::LoadFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("singlepram").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    if(pRes->GetRecord())// ¦Ì¡À¨®DDD?¨¢1?¨º¡À
    {
        m_wMaxPlayerLevel = SGDP::SDAtou(pRes->GetFieldValueByName("MaxPlayerLevel"));
		m_byRechargeFlag = SDAtou(pRes->GetFieldValueByName("RechargeFlag"));
		m_byInterceptFlag = SDAtou(pRes->GetFieldValueByName("InterceptFlag"));
		m_byHotSpringFlag = SDAtou(pRes->GetFieldValueByName("HotSpringFlag"));
		m_byAdvertisementFlag = SDAtou(pRes->GetFieldValueByName("AdvertisementFlag"));
        m_byOnlineEncourageKeepDays = SGDP::SDAtou(pRes->GetFieldValueByName("OnlineEncourageKeepDays"));
        m_byLoginEncourageKeepDays = SGDP::SDAtou(pRes->GetFieldValueByName("LoginEncourageKeepDays"));
        m_wRefreshEliteNumPerDay = SGDP::SDAtou(pRes->GetFieldValueByName("RefreshEliteNumPerDay"));
        m_wGainCropCDGoldPerMin = SGDP::SDAtou(pRes->GetFieldValueByName("GainCropCDGoldPerMin"));
        m_byMaxHPGridNum = SGDP::SDAtou(pRes->GetFieldValueByName("MaxHPGridNum"));
        m_wRecruitReturnCoinPercentage = SGDP::SDAtou(pRes->GetFieldValueByName("RecruitReturnCoinPercentage"));
        m_wRecruitFavorability = SGDP::SDAtou(pRes->GetFieldValueByName("RecruitFavorability"));
        m_wJuniorLevel = SGDP::SDAtou(pRes->GetFieldValueByName("JuniorLevel"));
        //m_wResetClimbTower = SGDP::SDAtou(pRes->GetFieldValueByName("ResetClimbTower"));

        m_byTotalYabiaoCnt = SGDP::SDAtou( pRes->GetFieldValueByName("TotalYabiaoCnt"));
        m_byTotalJiebiaoCnt = SGDP::SDAtou( pRes->GetFieldValueByName("TotalJiebiaoCnt"));
        m_nMaxLevelCostGold = SGDP::SDAtou( pRes->GetFieldValueByName("MaxLevelCostGold"));
        m_byTotalBeJiebiaoCnt = SGDP::SDAtou( pRes->GetFieldValueByName("TotalBeJiebiaoCnt"));
        m_byJiebiaoViewPlayerCnt = SGDP::SDAtou( pRes->GetFieldValueByName("JiebiaoViewPlayerCnt"));
        m_wJiebiaoCD = SGDP::SDAtou( pRes->GetFieldValueByName("JiebiaoCD"));
        m_wClearJiebiaoCDCostGoldPerMin = SGDP::SDAtou( pRes->GetFieldValueByName("ClearJiebiaoCDCostGoldPerMin"));
        m_wUpdateRecordDataToSqlInterval = SGDP::SDAtou( pRes->GetFieldValueByName("UpdateRecordDataToSqlInterval"));

        m_wPerfectFreeGodponGold = SGDP::SDAtou( pRes->GetFieldValueByName("PerfectFreeGodweaponGold"));
        m_dwGoldExperienceAddValue = SGDP::SDAtou( pRes->GetFieldValueByName("GoldExperienceAddValue"));
        m_wGoldExperienceCostGold = SGDP::SDAtou( pRes->GetFieldValueByName("GoldExperienceCostValue"));
        m_wFreeFlushBiaoCnt = SGDP::SDAtou( pRes->GetFieldValueByName("FreeFlushBiaoCnt") );

        m_wHeroTransFreePercentage = SGDP::SDAtou( pRes->GetFieldValueByName("HeroTransFreePercentage"));
        m_wHeroTransGoldPercentage = SGDP::SDAtou( pRes->GetFieldValueByName("HeroTransGoldPercentage") );

        m_wTransGold = SGDP::SDAtou( pRes->GetFieldValueByName("TransGold") );

        UINT32 nLen = pRes->GetFieldLengthByName("AppAddress");
        nLen = nLen > sizeof(m_szAppAddress) ? sizeof(m_szAppAddress) : nLen;
        memcpy(m_szAppAddress, pRes->GetFieldValueByName("AppAddress"), nLen);

        m_wCoachExpRate = SGDP::SDAtou( pRes->GetFieldValueByName("CoachExpRate") );

        SDStrncpy(m_szScienceDspContent, pRes->GetFieldValueByName("ScienceDspContent"), MAX_CONTENT_SIZE - 1);

        m_wOpenOprateingActivityLevel = SGDP::SDAtou( pRes->GetFieldValueByName("OpenOprateingActivityLevel") );
        m_wSaveOpHistoryToDBInterval  = SGDP::SDAtou(pRes->GetFieldValueByName("SaveOpHistoryToDBInterval"));

        m_wReCalcHowManyTopRacer   = SGDP::SDAtou(pRes->GetFieldValueByName("ReCalcHowManyTopRacer"));
        m_bIsSaveNewRacerPowerToDB = SGDP::SDAtou(pRes->GetFieldValueByName("IsSaveNewRacerPowerToDB"));
		m_dwSavePlayerInterval = SGDP::SDAtou(pRes->GetFieldValueByName("SavePlayerSqlInterval"));
		m_wApplyFactionMaxNum = SGDP::SDAtou(pRes->GetFieldValueByName("ApplyFactionMaxNum"));
		m_byEveryDayAuditMaxNum = SGDP::SDAtou(pRes->GetFieldValueByName("EveryDayAuditMaxNum"));
		m_bySureOutOrangeAwaken = SGDP::SDAtou(pRes->GetFieldValueByName("SureOutOrangeAwaken"));
		m_byFreeChangeIconNum = SGDP::SDAtou( pRes->GetFieldValueByName("FreeChangeIcon"));
        m_wEveryDayRecvFlowers = SGDP::SDAtou(pRes->GetFieldValueByName("EveryDayRecvFlowers"));
        m_dwExpiresRecvFlower	= SGDP::SDAtou(pRes->GetFieldValueByName("ExpiresRecvFlower"));
		m_byNormalFindBackDoorsTribute = SGDP::SDAtou(pRes->GetFieldValueByName("NormalFindBackDoorsTribute"));
		m_byPerfectFindBackDoorsTribute = SGDP::SDAtou(pRes->GetFieldValueByName("PerfectFindBackDoorsTribute"));
		m_dwHotSpringExprienceInterval = SGDP::SDAtou(pRes->GetFieldValueByName("ExprienceAddInterval"));
		m_dwChangeFactionIcon = SGDP::SDAtou(pRes->GetFieldValueByName("ChangeFactionIcon"));
		m_dwCloseAreaBoosMulti = SGDP::SDAtou(pRes->GetFieldValueByName("CloseAreaBoosMulti"));
		m_dwMaxScienceLevel = SGDP::SDAtou(pRes->GetFieldValueByName("MaxScienceLevel"));
		m_byJewelryStrengIntervalLevel = SGDP::SDAtou(pRes->GetFieldValueByName("JewelryStrengIntervalLevel"));
		m_dwJewelryResolveReturn = SGDP::SDAtou(pRes->GetFieldValueByName("JewelryResolveReturn"));
		m_dwJewelryBlueNum = SGDP::SDAtou(pRes->GetFieldValueByName("JewelryBlueNum"));
		m_dwJewelryPurpleNum = SGDP::SDAtou(pRes->GetFieldValueByName("JewelryPurpleNum"));
		m_dwJewelryRedNum = SGDP::SDAtou(pRes->GetFieldValueByName("JewelryRedNum"));
		m_dwJewelryOrangeNum = SGDP::SDAtou(pRes->GetFieldValueByName("JewelryOrangeNum"));
		m_byMaxJewelryStrengNum = SGDP::SDAtou(pRes->GetFieldValueByName("MaxJewelryStrengNum"));
        m_dwCourageHelpTimes = SGDP::SDAtou(pRes->GetFieldValueByName("CourageHelpTimes"));
		m_byMinDisPlayerOutBoundNum = SGDP::SDAtou(pRes->GetFieldValueByName("MinDisPlayerOutBoundNum"));
		m_wMaxNeiDanInstanceNum = SDAtou(pRes->GetFieldValueByName("MaxNeiDanInstanceNum"));
	}
    else
    {
        return FALSE;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}