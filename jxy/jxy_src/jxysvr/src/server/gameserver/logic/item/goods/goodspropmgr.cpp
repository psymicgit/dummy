#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <common/client/commondef.h>
#include <framework/gsapi.h>
#include "goodspropmgr.h"
#include "logic/crop/cropseedpropmgr.h"
#include <protocligs.h>
#include <db/dbmgr.h>

using namespace std;

#define SQL_READ_GOODS_STRING "select * from goodsprop"


IMPLEMENT_SINGLETON_PROPMGR(CGoodsPropMgr);

CGoodsPropMgr::CGoodsPropMgr()
{

}

CGoodsPropMgr::~CGoodsPropMgr()
{
}

BOOL CGoodsPropMgr::Init()
{
    return LoadFromDB();
}

VOID CGoodsPropMgr::UnInit()
{
    m_mapGemMakerProp.clear();
    Clear();
}

CGoodsPropMgr::CGoodsPropPriMap* CGoodsPropMgr::GetPropPriMap()
{
    return &(m_PriKeyPropMap);
}

VOID CGoodsPropMgr::FillGoodsData( SGoodsProp *pstGoodsProp, DT_GOODS_DATA_CLI& stDT_GOODS_DATA_CLI, UINT16 wPileCount )
{
    memset(&stDT_GOODS_DATA_CLI, 0, sizeof(stDT_GOODS_DATA_CLI));
    stDT_GOODS_DATA_CLI.wKindID	= pstGoodsProp->wKindID;
    stDT_GOODS_DATA_CLI.wUseLevel	= pstGoodsProp->wUseLevel;
    stDT_GOODS_DATA_CLI.wPileCount	= wPileCount;
    //stDT_GOODS_DATA_CLI.wBagPos = m_stDT_GOODS_DATA.wBagPos;
    stDT_GOODS_DATA_CLI.dwRecoverPrice		= pstGoodsProp->dwRecoverPrice;
    stDT_GOODS_DATA_CLI.byGoodsMainKindID	= pstGoodsProp->byMainKindID;
    stDT_GOODS_DATA_CLI.dwValue			= pstGoodsProp->dwValue;
    stDT_GOODS_DATA_CLI.byComposeItemNum	= pstGoodsProp->byComposeItemNum;
    stDT_GOODS_DATA_CLI.wComposeCount		= pstGoodsProp->wComposeCount;
    stDT_GOODS_DATA_CLI.wComposeKindID		= pstGoodsProp->wComposeKindID;
    stDT_GOODS_DATA_CLI.dwParam1 = pstGoodsProp->dwParam1;
    stDT_GOODS_DATA_CLI.dwParam2 = pstGoodsProp->dwParam2;
    stDT_GOODS_DATA_CLI.dwParam3 = pstGoodsProp->dwParam3;
    stDT_GOODS_DATA_CLI.dwParam4 = pstGoodsProp->dwParam4;
    memcpy(&stDT_GOODS_DATA_CLI.awComposeItem, &pstGoodsProp->awComposeItem, sizeof(stDT_GOODS_DATA_CLI.awComposeItem));
    SCropProp *poCropProp = NULL;
    poCropProp = CCropSeedPropMgr::Instance()->GetCropProp(stDT_GOODS_DATA_CLI.wKindID);
    if (NULL != poCropProp)
    {
        stDT_GOODS_DATA_CLI.bySeedFlag = 1;
        stDT_GOODS_DATA_CLI.astSeedData[0].byType = poCropProp->byType;
        stDT_GOODS_DATA_CLI.astSeedData[0].dwValue = poCropProp->dwObtainValue;
        stDT_GOODS_DATA_CLI.astSeedData[0].wKindID = poCropProp->wObtainKindID;
    }
}


// 从数据源获取数据
BOOL CGoodsPropMgr::LoadFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("goodsprop").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    // 打印查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        SGoodsProp stGoodsProp;
        memset(&stGoodsProp, 0, sizeof(SGoodsProp));
        stGoodsProp.wKindID = SGDP::SDAtou(pRes->GetFieldValueByName("KindID"));
        _SDTStrncpy(stGoodsProp.szName, _SDTUTF82T(pRes->GetFieldValueByName("Name")), NAME_LEN_MAX - 1);
        //_SDTStrncpy(stGoodsProp.szIntroduction, _SDTUTF82T(pRes->GetFieldValueByName("Introduction").c_str()), INTRODUCTION_LEN_MAX);
        stGoodsProp.byMainKindID = SGDP::SDAtou(pRes->GetFieldValueByName("MainKindID"));
        stGoodsProp.wUseLevel = SGDP::SDAtou(pRes->GetFieldValueByName("UseLevel"));
        stGoodsProp.dwBuyCoin = SGDP::SDAtou(pRes->GetFieldValueByName("BuyCoin"));
        stGoodsProp.dwBuyGold = SGDP::SDAtou(pRes->GetFieldValueByName("BuyGold"));
        stGoodsProp.dwRecoverPrice = SGDP::SDAtou(pRes->GetFieldValueByName("RecoverPrice"));
        stGoodsProp.wUseTimesPerDay = SGDP::SDAtou(pRes->GetFieldValueByName("UseTimesPerDay"));
        stGoodsProp.wUseCD = SGDP::SDAtou(pRes->GetFieldValueByName("UseCD"));
        stGoodsProp.dwValue = SGDP::SDAtou(pRes->GetFieldValueByName("Value"));

        stGoodsProp.wComposeCount = SGDP::SDAtou(pRes->GetFieldValueByName("ComposeCount"));
        stGoodsProp.wComposeKindID = SGDP::SDAtou(pRes->GetFieldValueByName("ComposeKindID"));
        stGoodsProp.dwParam1 = SGDP::SDAtou( pRes->GetFieldValueByName( "Param1" ) );
        stGoodsProp.dwParam2 = SGDP::SDAtou( pRes->GetFieldValueByName( "Param2" ) );
        stGoodsProp.dwParam3 = SGDP::SDAtou( pRes->GetFieldValueByName( "Param3" ) );
        stGoodsProp.dwParam4 = SGDP::SDAtou( pRes->GetFieldValueByName( "Param4" ) );
        if (0 != stGoodsProp.dwBuyCoin && stGoodsProp.dwRecoverPrice > stGoodsProp.dwBuyCoin)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CGoodsPropMgr RecoverPrice Error!"), MSG_MARK);
            continue;
        }
        Add(stGoodsProp.wKindID, stGoodsProp);

        if(EGMK_GEMMAKER == stGoodsProp.byMainKindID)
        {
            m_mapGemMakerProp[UINT16(stGoodsProp.dwParam2)] = stGoodsProp; //dwParam2为制作卷等级(对应宝石等级)
        }
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}

SGoodsProp* CGoodsPropMgr::GetGemMakerProp(UINT16 wGemMakerLevel)
{
    CGoodsPropPriMapItr itr = m_mapGemMakerProp.find(wGemMakerLevel);
    if(itr != m_mapGemMakerProp.end())
    {
        return &itr->second;
    }

    return NULL;
}
