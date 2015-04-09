#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <framework/gsapi.h>
#include <logic/item/goods/goodspropmgr.h>
#include <logic/item/itemcomposepropmgr.h>
#include "couragepropmgr.h"
#include <sdxmlfile.h>
#include <db/dbmgr.h>
#include "framework/gsapplication.h"
using namespace SGDP;

IMPLEMENT_SINGLETON_PROPMGR(CCouraPropMgr);

CCouraPropMgr::CCouraPropMgr()
{
	wMaxGroupNum = 0;
	m_mapCouragProp.clear();
    m_mapCourageActivityTimeProp.clear();
    m_mapCourageActivityEndTimeProp.clear();
    m_mapCourageInfoProp.clear();
    m_mapCouragePassEnProp.clear();
    m_mapGroupMonsterProp.clear();
    m_mapCourageEncourageProp.clear();
}

CCouraPropMgr::~CCouraPropMgr()
{
    CCouraPropMgr::DestroyInstance();
}

BOOL CCouraPropMgr::Init()
{
    if(!LoadActivityTimeFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CourageLoadActivityTimeFromDB failed!"), MSG_MARK);
        return FALSE;
    }
    //if (!LoadCourageAwardFromDB())
    //{
    //    SYS_CRITICAL(_SDT("[%s: %d]: CourageLoadAwardFromDB failed!"), MSG_MARK);
    //    return FALSE;
    //}
    //LoadCourageMonsterFDB();
	return TRUE;
}

VOID CCouraPropMgr::UnInit()
{
	wMaxGroupNum = 0;
	m_mapCouragProp.clear();
    m_mapCourageActivityTimeProp.clear();
    m_mapCourageActivityEndTimeProp.clear();
    m_mapCourageInfoProp.clear();
    m_mapCouragePassEnProp.clear();
    m_mapCourageEncourageProp.clear();
}

//从数据源获取数据
BOOL CCouraPropMgr::LoadActivityTimeFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("courageactivitytimeprop",  "order by ActivityKind, StartTime asc").c_str(), &pRes, &dwErrID,  &strErr);
    // 打印查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        SCourageActivityTimeProp stProp;
        stProp.byActivityKind = SDAtou(pRes->GetFieldValueByName("ActivityKind"));
        CHAR szTmp[5] = {0};
        SDStrncpy(szTmp, pRes->GetFieldValueByName("StartTime"), 4);
        CHAR szTmp2[3] = {0};
        memcpy(szTmp2, szTmp, 2);
        stProp.byStartHour = SDAtou(szTmp2);
        stProp.byStartMin = SDAtou(szTmp + 2);
        if((stProp.byStartHour > 23) || (stProp.byStartMin > 59))
        {
            DBG_INFO(_SDT("[%s: %d]: StartTime invalid, Hour[%d], Min[%d]!"), MSG_MARK, stProp.byStartHour, stProp.byStartMin);
            return FALSE;
        }
        stProp.wKeepTime = SDAtou(pRes->GetFieldValueByName("KeepTime"));

        stProp.dwStartTime = stProp.byStartHour * 3600 + stProp.byStartMin * 60;
        stProp.dwEndTime = stProp.dwStartTime + stProp.wKeepTime;
        stProp.wMaxGroupNum = SDAtou(pRes->GetFieldValueByName("MaxGroupNum"));

        m_mapCourageActivityTimeProp[stProp.dwStartTime] = stProp;
        m_mapCourageActivityEndTimeProp[stProp.dwEndTime] = stProp;
        m_mapCouragProp[stProp.byActivityKind] = stProp;
        wMaxGroupNum = SDAtou(pRes->GetFieldValueByName("MaxGroupNum"));
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    if(m_mapCourageActivityEndTimeProp.size() == 0)
    {
        DBG_INFO(_SDT("[%s: %d]: activitytimeprop no data!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

const SCourageActivityTimeProp* CCouraPropMgr::GetActivityTimeProp( UINT64 qwEnterTime /*= SDTimeSecs()*/ )
{
    UINT32 dwDayPassSecond = GetDayPassSecond(qwEnterTime);
    CCourageActivityTimePropMapItr itr = m_mapCourageActivityEndTimeProp.upper_bound(dwDayPassSecond); //key为结束时间dwEndTime
    if(itr != m_mapCourageActivityEndTimeProp.end())
    {
        SCourageActivityTimeProp* pstProp = &itr->second;
        if(pstProp->dwStartTime <= dwDayPassSecond)
        {
            return pstProp;
        }
    }

    return NULL;
}

const SCourageActivityTimeProp* CCouraPropMgr::GetNextActivityTimeProp( UINT64 qwStartTime /*= SDTimeSecs()*/ )
{
    UINT32 dwDayPassSecond = GetDayPassSecond(qwStartTime);
    CCourageActivityTimePropMapItr itr = m_mapCourageActivityTimeProp.upper_bound(dwDayPassSecond); //key为结束时间dwEndTime
    if(itr != m_mapCourageActivityTimeProp.end())
    {
        SCourageActivityTimeProp* pstProp = &itr->second;
        if(pstProp->dwStartTime <= dwDayPassSecond)
        {
            return pstProp;
        }
    }

    return NULL;
}

BOOL CCouraPropMgr::LoadAwardFromDB()
{

    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("courageaward", " order by Level").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    while(pRes->GetRecord())
    {
        SCourageInfoProp stCourageInfo;
        stCourageInfo.wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
        if(0 == stCourageInfo.wLevel)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: Level 0 invalid!"), MSG_MARK);
            return FALSE;
        }
        SGDP::CSDXMLFile xmlFile;
        std::string strValue;
        if(FALSE == xmlFile.LoadData(pRes->GetFieldValueByName("CourageAward"), pRes->GetFieldLengthByName("CourageAward")))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: xml file load failed!"), MSG_MARK);
            return FALSE;
        }
        CSDXMLNode rootNode = xmlFile.GetRootNode();
        if(rootNode == NULL)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: xml obj err!"), MSG_MARK);
            return FALSE;
        }

        UINT16	wLastEnterLevel		=	0;
        UINT8	byForwardIndex	=	0;
        CSDXMLNode xmlNodePropCourage = rootNode["Courage"];
        while(xmlNodePropCourage != NULL)
        {
            //奖励
            CSDXMLNode xmlNodeProp = xmlNodePropCourage["Encourage"];
            if(xmlNodeProp == NULL)
            {
                return FALSE;
            }
            stCourageInfo.stPassCourageEncourageProp.dwCoin = ReadOneValue(EBKIND_COIN, xmlNodeProp);
            stCourageInfo.stPassCourageEncourageProp.dwStory = ReadOneValue(EBKIND_STORY, xmlNodeProp);
            stCourageInfo.stPassCourageEncourageProp.dwExperience = ReadOneValue(EBKIND_EXPERIENCE, xmlNodeProp);

            if(FALSE == ReadEncourageItem(stCourageInfo.stPassCourageBoxProp, xmlNodeProp))
            {
                return FALSE;
            }
            m_mapCourageInfoProp[stCourageInfo.wLevel] = stCourageInfo;
            xmlNodePropCourage = xmlNodePropCourage.Sibling("Courage");
        }

    }

    return TRUE;
}

//读取单个属性值
UINT32 CCouraPropMgr::ReadOneValue(EBurstKind eBurstKind, CSDXMLNode& xmlNode)
{
    if(xmlNode == NULL)
    {
        return 0;
    }

    CHAR* pszReadItemName = NULL;
    switch(eBurstKind)
    {
    case EBKIND_COIN:
        pszReadItemName = "Coin";
        break;
    case EBKIND_GOLD:
        pszReadItemName = "Gold";
        break;
    case EBKIND_EXPERIENCE:
        pszReadItemName = "Experience";
        break;
    case EBKIND_PHYSTRENGTH:
        pszReadItemName = "PhyStrength";
        break;
    case EBKIND_SCIENCE:
        pszReadItemName = "Science";
        break;
    case EBKIND_STORY:
        pszReadItemName = "Story";
        break;
    default:
        return FALSE;
    }

    CSDXMLNode xmlNodeProp = xmlNode[pszReadItemName];
    if(xmlNodeProp != NULL)
    {
        return xmlNodeProp("Value").AsInteger();
    }

    return 0;
}

BOOL CCouraPropMgr::ReadEncourageItem(SPassCourageBoxProp& stPassBoxProp, CSDXMLNode& xmlNode)
{
    if(xmlNode == NULL)
    {
        return TRUE;
    }

    CHAR* pszReadItemName = "Item";
    CSDXMLNode xmlItemNodeProp = xmlNode[pszReadItemName];
    if(xmlItemNodeProp == NULL)
    {
        return TRUE;//没有配置爆出物品
    }

    CSDXMLNode xmlNodePropGroup = xmlItemNodeProp["Group"];
    UINT8 byGroupIdx = 1;//组序号从1开始
    while(xmlNodePropGroup != NULL)
    {
        CSDXMLNode xmlNodeProp = xmlNodePropGroup["Prop"];
        UINT16	wFirstItemKindID = 0;
        UINT16	wFirstItemPileNum = 0;
        while(xmlNodeProp != NULL)
        {
            UINT16 wKindID = xmlNodeProp("KindID").AsInteger();
            UINT16 wNum = xmlNodeProp("Num").AsInteger();
            UINT16 wRate = xmlNodeProp("Rate").AsInteger();
            wRate = wRate > 100 ? 100 : wRate;
            if((0 == wKindID) || (0 == wNum))
            {
                //SYS_CRITICAL(_SDT("[%s: %d]: CInstancePropMgr::ReadEncourageItem failed ((0 == wKindID) || (0 == wNum) || (0 == wRate)), InstanceID[%d.%d.%d]\n"),
                //    MSG_MARK, m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
                break;
            }
            SItemRate stItemRate;
            stItemRate.wItemNum = wNum;
            stItemRate.wRate = wRate * 100; //万分比
            SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wKindID);

            //内部宝箱全部拆开
            if((pstGoodsProp) && (EGMK_INNERBOX == pstGoodsProp->byMainKindID))
            {
                SItemComposeProp* pstItemComposeProp = CItemComposePropMgr::Instance()->GetProp(wKindID);
                if(NULL == pstItemComposeProp)
                {
                    SYS_CRITICAL(_SDT("[%s: %d]: pstItemComposeProp is NULL,wKindID[%d]!"), MSG_MARK, wKindID);
                    return FALSE;
                }

                CItemRateMap& mapComposeItem = pstItemComposeProp->mapComposeItem;
                if(0 == mapComposeItem.size())
                {
                    SYS_CRITICAL(_SDT("[%s: %d]: mapComposeItem[%d] is 0!"), MSG_MARK, wKindID);
                    return FALSE;
                }
                SItemCourageGroupProp &stItemGroupProp = stPassBoxProp.mapItemGroup[byGroupIdx];
                for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end();  itr++)
                {
                    SItemRate stRate = itr->second;
                    stRate.wRate = stRate.wRate * wRate / 100;
                    stRate.wRate = stRate.wRate == 0 ? 1 : stRate.wRate;
                    stItemGroupProp.mapItem[itr->first] = stRate;

                    // m_mapBurstItemKind2InstanceID[itr->first] = m_stInstanceIDTmp;//只需记录最后产出该道具的，可直接覆盖
                }
                stItemGroupProp.wFirstItemKindID = wFirstItemKindID;
                stItemGroupProp.wFirstItemPileNum = wFirstItemPileNum;
            }
            else
            {
                SItemCourageGroupProp& stItemGroupProp = stPassBoxProp.mapItemGroup[byGroupIdx];
                stItemGroupProp.mapItem[wKindID] = stItemRate;
                stItemGroupProp.wFirstItemKindID = wFirstItemKindID;
                stItemGroupProp.wFirstItemPileNum = wFirstItemPileNum;
            }
            xmlNodeProp = xmlNodeProp.Sibling("Prop");
        }

        xmlNodePropGroup = xmlNodePropGroup.Sibling("Group");
    }


    return TRUE;
}

SCourageInfoProp* CCouraPropMgr::GetCourageInfoProp( UINT16 wLevel )
{
    CCourageInfoPropMapItr itr = m_mapCourageInfoProp.find(wLevel);
    if (itr != m_mapCourageInfoProp.end())
    {
        return &itr->second;
    }
    return NULL;
}

SCouragePassEncourageProp* CCouraPropMgr::GetCourageAwardProp( UINT16 wLevel )
{
    SCouragePassEncouragePropMapItr itr = m_mapCouragePassEnProp.find(wLevel);
    if (itr != m_mapCouragePassEnProp.end())
    {
        return &itr->second;
    }
    return NULL;
}

BOOL CCouraPropMgr::LoadCourageAwardFromDB()
{
    SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("courageawardprop", " order by Level").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    UINT8 byRowNum = 0;

    while(pRes->GetRecord()) // 当有行结果时
    {
        
        SCourageEncourage stProp1;
        SCouragePassEncourageProp stProp;
        stProp.wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
        stProp.dwStory = SGDP::SDAtou(pRes->GetFieldValueByName("Story"));
        stProp.dwMedal = SGDP::SDAtou(pRes->GetFieldValueByName("Medal"));
        //stProp.dwCoin = SGDP::SDAtou(pRes->GetFieldValueByName("Coin"));
        //stProp.dwExperience = SGDP::SDAtou(pRes->GetFieldValueByName("Experience"));
        //stProp.dwGold = SGDP::SDAtou(pRes->GetFieldValueByName("Gold")); 
        //stProp.dwBlueGas = SGDP::SDAtou(pRes->GetFieldValueByName("BlueGas"));

        stProp1.wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
        string strJsonData	= pRes->GetFieldValueByName("JsonValues");
        if(!JsonParseResItem(stProp1.vecJsonEncourageItemInfo, strJsonData))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: JsonParseResItem error, strJsonData:%s!"), MSG_MARK, strJsonData.c_str());
            return FALSE;
        }

        m_mapCouragePassEnProp[stProp.wLevel] = stProp;
        m_mapCourageEncourageProp[stProp1.wLevel] = stProp1;
        byRowNum++;
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

BOOL CCouraPropMgr::LoadCourageMonsterFDB()
{
    SGDP::ISDDBSession*	pDBSession = ((CGSApplication*)SDGetApp())->GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("couragemonster", " order by Level").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    UINT8 byRowNum = 0;

    while(pRes->GetRecord()) // 当有行结果时
    {
        SWholeMonsterProp *pstWholeMonsterProp1 = SDNew SWholeMonsterProp();
        SGroupMonsterProp *stProp = SDNew SGroupMonsterProp();
        SSingleMonsterProp stSingleProp;
        stProp->wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
        if(SDAtou(pRes->GetFieldValueByName("KindFirstID1")) == 0)
        {
            SDDelete pstWholeMonsterProp1;
            pstWholeMonsterProp1 = NULL;
            SYS_CRITICAL(_SDT("[%s: %d]: KindFirstID1 no data!"), MSG_MARK);
            return FALSE;
        }
        stSingleProp.wMonsterID = SDAtou(pRes->GetFieldValueByName("KindFirstID1"));
        stSingleProp.byFormationIdx = SDAtou(pRes->GetFieldValueByName("FirstIdx1"));
        pstWholeMonsterProp1->vecMonster.push_back(stSingleProp);

        if (SDAtou(pRes->GetFieldValueByName("KindFirstID2"))!=0)
        {
            stSingleProp.wMonsterID = SDAtou(pRes->GetFieldValueByName("KindFirstID2"));
            stSingleProp.byFormationIdx = SDAtou(pRes->GetFieldValueByName("FirstIdx2"));
            pstWholeMonsterProp1->vecMonster.push_back(stSingleProp);
        }

        if (SDAtou(pRes->GetFieldValueByName("KindFirstID3"))!=0)
        {
            stSingleProp.wMonsterID = SDAtou(pRes->GetFieldValueByName("KindFirstID3"));
            stSingleProp.byFormationIdx = SDAtou(pRes->GetFieldValueByName("FirstIdx3"));
            pstWholeMonsterProp1->vecMonster.push_back(stSingleProp);
        }

        stProp->vecWholeMonsterProp.push_back(pstWholeMonsterProp1);

        SWholeMonsterProp *pstWholeMonsterProp2 = SDNew SWholeMonsterProp();;
        if(SDAtou(pRes->GetFieldValueByName("KindSecondID1")) == 0)
        {
            SDDelete pstWholeMonsterProp2;
            pstWholeMonsterProp2 = NULL;
            SYS_CRITICAL(_SDT("[%s: %d]: KindSecondID1 no data!"), MSG_MARK);
            return FALSE;
        }
        stSingleProp.wMonsterID = SDAtou(pRes->GetFieldValueByName("KindSecondID1"));
        stSingleProp.byFormationIdx = SDAtou(pRes->GetFieldValueByName("SecondIdx1"));
        pstWholeMonsterProp2->vecMonster.push_back(stSingleProp);

        if (SDAtou(pRes->GetFieldValueByName("KindSecondID2"))!=0)
        {
            stSingleProp.wMonsterID = SDAtou(pRes->GetFieldValueByName("KindSecondID2"));
            stSingleProp.byFormationIdx = SDAtou(pRes->GetFieldValueByName("SecondIdx2"));
            pstWholeMonsterProp2->vecMonster.push_back(stSingleProp);
        }

        if (SDAtou(pRes->GetFieldValueByName("KindSecondID3"))!=0)
        {
            stSingleProp.wMonsterID = SDAtou(pRes->GetFieldValueByName("KindSecondID3"));
            stSingleProp.byFormationIdx = SDAtou(pRes->GetFieldValueByName("SecondIdx3"));
            pstWholeMonsterProp2->vecMonster.push_back(stSingleProp);
        }

        stProp->vecWholeMonsterProp.push_back(pstWholeMonsterProp2);
 
        SWholeMonsterProp *pstWholeMonsterProp3 = SDNew SWholeMonsterProp();;
        if(SDAtou(pRes->GetFieldValueByName("KindThirdID1")) == 0)
        {
            SDDelete pstWholeMonsterProp3;
            pstWholeMonsterProp3 = NULL;
            SYS_CRITICAL(_SDT("[%s: %d]: KindThirdID1 no data!"), MSG_MARK);
            return FALSE;
        }
        stSingleProp.wMonsterID = SDAtou(pRes->GetFieldValueByName("KindThirdID1"));
        stSingleProp.byFormationIdx = SDAtou(pRes->GetFieldValueByName("ThirdIdx1"));
        pstWholeMonsterProp3->vecMonster.push_back(stSingleProp);

        if (SDAtou(pRes->GetFieldValueByName("KindThirdID2"))!=0)
        {
            stSingleProp.wMonsterID = SDAtou(pRes->GetFieldValueByName("KindThirdID2"));
            stSingleProp.byFormationIdx = SDAtou(pRes->GetFieldValueByName("ThirdIdx2"));
            pstWholeMonsterProp3->vecMonster.push_back(stSingleProp);
        }
        
        if (SDAtou(pRes->GetFieldValueByName("KindThirdID3"))!=0)
        {
            stSingleProp.wMonsterID = SDAtou(pRes->GetFieldValueByName("KindThirdID3"));
            stSingleProp.byFormationIdx = SDAtou(pRes->GetFieldValueByName("ThirdIdx3"));
            pstWholeMonsterProp3->vecMonster.push_back(stSingleProp);
        }
        stProp->vecWholeMonsterProp.push_back(pstWholeMonsterProp3);

        m_mapGroupMonsterProp[stProp->wLevel] = stProp;
        byRowNum++;
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;

}

SGroupMonsterProp* CCouraPropMgr::GetCourageMonsterProp(UINT16 wLevel )
{
    CGroupMonsterPropMapItr itr = m_mapGroupMonsterProp.find(wLevel);
    if (itr != m_mapGroupMonsterProp.end())
    {
        return itr->second;
    }
    return NULL;
}

