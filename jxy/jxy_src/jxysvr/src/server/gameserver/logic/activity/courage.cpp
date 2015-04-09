
#include "courage.h"
#include <common/client/errdef.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/activity/couragemgr.h>
#include <logic/activity/couragepropmgr.h>
#include <logic/other/state.h>

using namespace SGDP;

IMPLEMENT_CLASS_POOL_MGR(CCourage)

CCourage::CCourage()
{
    memset(&m_stCourageData, 0, sizeof(m_stCourageData));
    m_listLevelGroup.clear();
    m_vecGroupID.clear();
    m_mapGroupIDNum.clear();
    m_mapBackGroupID.clear();
}


CCourage::~CCourage()
{

}

BOOL CCourage::Init(DT_COURAGE_DATA &stCourageData, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    m_poOwner = poOwner;
    memcpy(&m_stCourageData, &stCourageData, sizeof(DT_COURAGE_DATA));
    return TRUE;
}

VOID CCourage::GetDT_ACTIVITY_BASE_DATA(DT_ACTIVITY_BASE_DATA& stDT_ACTIVITY_BASE_DATA)
{	
    memset(&stDT_ACTIVITY_BASE_DATA, 0, sizeof(stDT_ACTIVITY_BASE_DATA));
    stDT_ACTIVITY_BASE_DATA.byKindID = EAK_COURAGE;
    const CCourageActivityTimePropMap& mapActivityEndTimeProp = CCouraPropMgr::Instance()->GetCourageActivityEndTimeProp();

    UINT32 dwCurSecond = GetDayPassSecond(m_stCourageData.qwLastTime);
    BOOL bDay = IsToday(m_stCourageData.qwLastTime);
    for(CCourageActivityTimePropMapCItr itr = mapActivityEndTimeProp.begin(); ((itr != mapActivityEndTimeProp.end()) && (stDT_ACTIVITY_BASE_DATA.byActivityTimeNum <MAX_ACTIVITY_TIME_NUM)); itr++)
    {
        const SCourageActivityTimeProp& stProp = itr->second;
        DT_ACTIVITY_TIME_DATA& stDT_ACTIVITY_TIME_DATA = stDT_ACTIVITY_BASE_DATA.astActivityTimeInfo[stDT_ACTIVITY_BASE_DATA.byActivityTimeNum++];
        memset(&stDT_ACTIVITY_TIME_DATA, 0, sizeof(stDT_ACTIVITY_TIME_DATA));
        stDT_ACTIVITY_TIME_DATA.byStartHour = stProp.byStartHour;
        stDT_ACTIVITY_TIME_DATA.byStartMin = stProp.byStartMin;
        stDT_ACTIVITY_TIME_DATA.wKeepTime = stProp.wKeepTime;
        if((dwCurSecond >= stProp.dwStartTime) && (dwCurSecond < stProp.dwEndTime) && (bDay))
        {
            stDT_ACTIVITY_TIME_DATA.wEndCountDown = (UINT16)(stProp.dwEndTime - dwCurSecond);
        }
    }
    stDT_ACTIVITY_BASE_DATA.byNotity = m_stCourageData.byNotity;
}

BOOL CCourage::GetPrompt( DT_FUNC_COMMON_PROMPT& stPrompt )
{
    memset(&stPrompt, 0, sizeof(stPrompt));
    stPrompt.byBuildKindID = EBK_ACTIVITY;
    stPrompt.byExt = EAK_COURAGE;
    const SCourageActivityTimeProp* pstProp = CCouraPropMgr::Instance()->GetActivityTimeProp();
    const SCourageActivityTimeProp* pstNextProp = NULL;
    UINT32 dwPassSes = GetDayPassSecond();
    if(pstProp)
    {
        stPrompt.byFuncFlag = EAK_COURAGE;
        UINT32 dwCurSecond = GetDayPassSecond(m_stCourageData.qwLastTime);
        BOOL bDay = IsToday(m_stCourageData.qwLastTime);

        stPrompt.dwFuncCountDown = (pstProp->dwEndTime > dwPassSes) ? pstProp->dwEndTime - dwPassSes : 0;
        return TRUE;

    }
    else
    {
        pstNextProp = CCouraPropMgr::Instance()->GetNextActivityTimeProp();
    }

    stPrompt.byFuncFlag = 0;
    if(pstNextProp)
    {
        //今天内的活动		
        if(pstNextProp->dwStartTime > dwPassSes)
        {
            stPrompt.dwFuncCountDown = pstNextProp->dwStartTime - dwPassSes;
        }
        //今天已没有活动，下一个活动为明天的第一个活动
        else
        {
            stPrompt.dwFuncCountDown = SECONDES_PER_DAY - dwPassSes + pstNextProp->dwStartTime;
        }

        return FALSE;
    }

    return FALSE;
}

VOID CCourage::RatePassBoxEnc( UINT16 wLevel, DT_PASS_ENCOURAGE_BOX_DATA& stPassEncourageBox )
{
    UINT8 abyHitItemIdx[MAX_ENCOURAGE_BOX_GROUP_NUM] = {0};
    SCourageInfoProp* pstInstanceProp = CCouraPropMgr::Instance()->GetCourageInfoProp(wLevel);
    if(NULL == pstInstanceProp)
    {
        //m_poOwner->GetState().OnRatePassBox(abyHitItemIdx);
        //SYS_CRITICAL(_SDT("[%s: %d]: RatePassBoxEnc pstInstanceProp[%d.%d.%d] is NULL!"), MSG_MARK, wSceneIdx, byTownIdx, byInstanceIdx);
        return ;
    }
    SPassCourageBoxProp& stPassBoxProp = pstInstanceProp->stPassCourageBoxProp;
    CItemCourageGroupMap& mapItemGroup = stPassBoxProp.mapItemGroup;
    if(0 == mapItemGroup.size())
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: RatePassBoxEnc pstInstanceProp[%d.%d.%d] mapItemGroup size is 0!"), MSG_MARK, wSceneIdx, byTownIdx, byInstanceIdx);
       // m_poOwner->GetState().OnRatePassBox(abyHitItemIdx);
        return ;
    }

    UINT8 byGroupNum = 0;
    for(CItemCourageGroupMapItr itr = mapItemGroup.begin(); ((itr != mapItemGroup.end()) && (byGroupNum < MAX_ENCOURAGE_BOX_GROUP_NUM)); itr++, byGroupNum++)
    {
        SItemCourageGroupProp& stItemGroupProp = itr->second;
        DT_PASS_ENCOURAGE_BOX_GROUP_DATA& stBoxGroupInfo = stPassEncourageBox.astBoxGroupInfo[byGroupNum];
        UINT16 wItemKindID = 0;
        UINT16 wPileNum = 0;
        UINT8 byHitItemIdx = 0; //0表示未命中
        {
            CItemRateMap& mapItem = stItemGroupProp.mapItem;
            //计算随机值
            CRandomVec vecRandom;
            CItemRateMapItr itr;
            for(itr = mapItem.begin(); itr != mapItem.end(); itr++)
            {
                vecRandom.push_back(itr->second.wRate);
            }
            //随机
            INT32 nRandomHitIdx = GetRandomIdx(vecRandom, 10000);//万分比
            if(RANDOM_UNHIT != nRandomHitIdx) //命中
            {
                byHitItemIdx = nRandomHitIdx + 1; //
                UINT8 byIdx = 0;
                for(itr = mapItem.begin(); itr != mapItem.end(); itr++, byIdx++)
                {
                    if(byIdx == nRandomHitIdx)
                    {
                        wItemKindID = itr->first;
                        wPileNum = itr->second.wItemNum;
                    }
                }
            }
            else
            {
                //USR_INFO(_SDT("[%s: %d]: RatePassBoxEnc pstInstanceProp[%d.%d.%d] UNHIT!"), MSG_MARK, wSceneIdx, byTownIdx, byInstanceIdx);
                byHitItemIdx = 0;
            }
        }
        abyHitItemIdx[byGroupNum] = byHitItemIdx;

        stBoxGroupInfo.byHitItemIdx = byHitItemIdx;
        memcpy(&stBoxGroupInfo.stItemInfo, &stItemGroupProp.stCourageBoxItem, sizeof(DT_ITEM_DATA_LIST_CLI));
    }
    stPassEncourageBox.byGroupNum = byGroupNum;

    m_poOwner->GetState().OnRatePassBox(abyHitItemIdx);

}

VOID CCourage::SetSelectType(UINT8 bySelectType)
{
	m_stCourageData.bySelectGroupType = bySelectType;

}

UINT8 CCourage::GetSelectType()
{
	return m_stCourageData.bySelectGroupType;
}

VOID CCourage::GetCourageGroup(DT_COURAGE_GROUP_DATA_CLI_LST& stDataLst, BOOL bSelect)
{
	//获取符合条件的队伍
	if( !CCourageMgr::Instance()->GetRightGroup( m_poOwner, bSelect) )
	{
		return ;
	}
	//符合条件的队伍数据
	CListLevelGroupItr itrNew = m_listLevelGroup.begin();
	for ( itrNew; itrNew != m_listLevelGroup.end() && stDataLst.wTeamNum < CCouraPropMgr::Instance()->GetMaxGroupNum(); itrNew++)
	{
		CCourageGroup *poCourageGroup = *itrNew;
		if ((poCourageGroup == NULL) || (poCourageGroup->GetDT_COURAGE_GROUP_DATA().wPlayerNum == 0))
		{
			continue;
		}

		DT_COURAGE_GROUP_BASE_DATA_CLI &stCoruageGroupBaseData = stDataLst.astTeamInfo[stDataLst.wTeamNum];
		stCoruageGroupBaseData.byJoinType = poCourageGroup->GetDT_COURAGE_GROUP_DATA().byJoinType;
		stCoruageGroupBaseData.dwGroupID = poCourageGroup->GetDT_COURAGE_GROUP_DATA().dwGroupID;
		stCoruageGroupBaseData.wPlayerNum = poCourageGroup->GetDT_COURAGE_GROUP_DATA().wPlayerNum;
		stCoruageGroupBaseData.wPlayerMaxNum = MAX_COURAGE_GROUP_PLAYER_NUM;
		stCoruageGroupBaseData.wMinLevel = poCourageGroup->GetDT_COURAGE_GROUP_DATA().wMinLevel;
		memcpy(&stCoruageGroupBaseData.stInstanceID, &poCourageGroup->GetDT_COURAGE_GROUP_DATA().stInstanceID,sizeof(DT_INSTANCE_ID));
		const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(poCourageGroup->GetDT_COURAGE_GROUP_DATA().dwLeaderID);
		if (poPlayerBaseData != NULL)
		{
			SDStrcpy(stCoruageGroupBaseData.stLeaderData.aszDispPlayerName, poPlayerBaseData->aszDispName);
			stCoruageGroupBaseData.stLeaderData.wLevel = poPlayerBaseData->wLevel;
			stCoruageGroupBaseData.stLeaderData.wKindID = poPlayerBaseData->wMainHeroKindID;
		}

		stDataLst.wTeamNum++;
	}

}