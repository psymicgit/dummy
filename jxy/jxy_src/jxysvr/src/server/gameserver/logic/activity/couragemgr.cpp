#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <framework/gsapi.h>
#include "couragemgr.h"
#include "couragepropmgr.h"
#include "logic/event/timereventmgr.h"
#include "logic/lua/luamgr.h"
#include <logic/other/errmsgmgr.h>
#include <logic/build/build.h>
#include <logic/activity/courage.h>
#include <logic/hero/hero.h>
#include <logic/hero/heropropmgr.h>
#include <logic/player/player.h>
#include <logic/player/playermgr.h>
#include <logic/monster/playergroupmgr.h>
#include <logic/battle/battlemgr.h>
#include <logic/other/locale.h>
#include <logic/vip/vippropmgr.h>
#include <logic/jingjie/jingjie.h>
#include <logic/jingjie/jingjiepropmgr.h>
#include <logic/instance/instancepropmgr.h>
#include <logic/friendsys/friends.h>
#include <logic/item/itemcomposepropmgr.h>
#include <logic/item/jewelry/jewelrypropmgr.h>
#include <logic/other/singleprammgr.h>
#include <vector>
#include <db/dbmgr.h>
#include <errdef.h>

#include <db/dbmgr.h>
#include <db/basedbccmd.h>
#include "logic/medal/playermedal.h"
#include "logic/other/fewparam.h"
const UINT32 MAX_GROUP_ID = 4000000000;
UINT32 CCourageMgr::dwCreateGroupID = 1;

#define  INVALID_GROUP_INDEX  3
//自定义排序函数
bool SortByLevel(CCourageGroup* v1,  CCourageGroup* v2)//注意：本函数的参数的类型一定要与容器中元素的类型一致
{
    if (v1->GetDT_COURAGE_GROUP_DATA().wMinLevel == v2->GetDT_COURAGE_GROUP_DATA().wMinLevel)
    {
        if (v1->GetDT_COURAGE_GROUP_DATA().byJoinType == v2->GetDT_COURAGE_GROUP_DATA().byJoinType)
        {
            return v1->GetDT_COURAGE_GROUP_DATA().dwGroupID > v2->GetDT_COURAGE_GROUP_DATA().dwGroupID;
        }
        else return v1->GetDT_COURAGE_GROUP_DATA().byJoinType > v2->GetDT_COURAGE_GROUP_DATA().byJoinType;
    }
    else return v1->GetDT_COURAGE_GROUP_DATA().wMinLevel > v2->GetDT_COURAGE_GROUP_DATA().wMinLevel;
}

CCourageGroupMap CCourageGroup::s_mapCourageGroup;

IMPLEMENT_SINGLETON(CCourageMgr);

CCourageMgr::CCourageMgr()
{
    m_dwGroupID = 0;
    m_qwOverTime = 0; //活动结束时间
    m_qwStartTime = 0; //活动开始时间
    m_qwSaveTime = 0; //活动保存更新时间
    m_m_byActivityID = 0; //活动ID
    m_bActivityOver = TRUE;
    m_mapAllEnterPlayer.clear();
    m_mapPlayerGroupMap.clear();
    m_mapAllEnterGropPlayer.clear();
    dwCreateGroupID = 1;
    m_qwRefreshTime = 0;
    bHaveData = FALSE;
    memset(&CreateGroupNum,0x00,sizeof(CreateGroupNum));
}

CCourageMgr::~CCourageMgr()
{

}

BOOL CCourageMgr::Init()
{
    // 初始pMgr管理器
    if(FALSE == CCouraPropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CCouraPropMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CCouraPropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CCouraPropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }
    if(!InitModuleDataFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitDataFromDB failed!"), MSG_MARK);
        return FALSE;
    }
    if(!InitEvent())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitEvent failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}


BOOL CCourageMgr::Init(DT_COURAGE_GROUP_DATA_LST stDT_COURAGE_GROUP_DATA_LST)
{
    m_dwGroupID		=	0;
    m_qwOverTime		=	stDT_COURAGE_GROUP_DATA_LST.qwOverTime;
    m_qwStartTime		=	stDT_COURAGE_GROUP_DATA_LST.qwStartTime;
    m_qwSaveTime		=	stDT_COURAGE_GROUP_DATA_LST.qwSaveTime;
    m_m_byActivityID	=	stDT_COURAGE_GROUP_DATA_LST.byActivityID;
    for (UINT16 idx = 0; idx < stDT_COURAGE_GROUP_DATA_LST.wGroupNum; idx++)
    {
        DT_COURAGE_GROUP_DATA & stDT_COURAGE_GROUP_DATA = stDT_COURAGE_GROUP_DATA_LST.astGroupData[idx];
        CCourageGroup *poCourageGroup = m_oCourageGroupPool.Alloc();
        if (NULL == poCourageGroup)
        {
            break;
        }
        poCourageGroup->Init(stDT_COURAGE_GROUP_DATA);
        m_mapGroupID2CourageGroup[stDT_COURAGE_GROUP_DATA.dwGroupID] = poCourageGroup;
        if (stDT_COURAGE_GROUP_DATA.dwGroupID > m_dwGroupID)
        {
            m_dwGroupID = stDT_COURAGE_GROUP_DATA.dwGroupID;
        }
    }
    if (m_dwGroupID != 0)
    {
        m_dwGroupID++;
    }
    return TRUE;
}

VOID CCourageMgr::UnInit()
{

}

BOOL CCourageMgr::InitEvent()
{
    const CCourageActivityTimePropMap& mapProp = CCouraPropMgr::Instance()->GetCourageActivityTimePropMap();
    for(CCourageActivityTimePropMapCItr itr = mapProp.begin(); itr != mapProp.end(); itr++)
    {
        const SCourageActivityTimeProp& stProp = itr->second;
        CTimerEventMgr::Instance()->AddCourageEvent(stProp.byActivityKind, TRUE);
        CTimerEventMgr::Instance()->AddCourageEvent(stProp.byActivityKind, FALSE);
    }
    return TRUE;
}


BOOL	CCourageMgr::OnStartEvent(UINT8 byActivityID)
{
    const SCourageActivityTimeProp* pstProp = CCouraPropMgr::Instance()->GetCouragePropByID(byActivityID);
    if(NULL == pstProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CCourageActivityTimePropMap[%d] is NULL!"), MSG_MARK, byActivityID);
        return FALSE;
    }
 
    if (bHaveData == FALSE)
    {
        for(CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.begin(); itr != m_mapGroupID2CourageGroup.end(); itr++)
        {
            m_oCourageGroupPool.Free(itr->second);
        }
        m_mapGroupID2CourageGroup.clear();
    }
    m_bActivityOver = FALSE;
    m_m_byActivityID = pstProp->byActivityKind;
    m_qwStartTime = SDTimeSecs();
    m_qwOverTime = 0;
    return TRUE;

}

BOOL	CCourageMgr::OnEndEvent(UINT8 byActivityID)
{
    //清空之前保存一份数据
    m_bActivityOver = TRUE;
    m_qwOverTime = SDTimeSecs();
    SaveData();
    bHaveData = FALSE;
    m_dwGroupID = 0;
    dwCreateGroupID = 1;
    memset(&CreateGroupNum,0x00,sizeof(CreateGroupNum));
    for(C3232MapItr itr = m_mapAllEnterGropPlayer.begin(); itr != m_mapAllEnterGropPlayer.end(); itr++)
    {
        UINT32 dwID = itr->second;
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwID);
        if(NULL == poPlayer)
        {
            continue;
        }
        CCourage* poCourage = poPlayer->GetCourage();;
        if(NULL == poCourage)
        {
            continue;
        }
        DT_COURAGE_DATA &stCourageData = poCourage->GetDT_COURAGE_DATA();
        stCourageData.dwGroupID = 0;
        //位置回调
        if (poPlayer->GetLocale().GetDT_LOCALE_DATA().byLocaleType == EPL_COURAGE)
        {
            poPlayer->GetLocale().OnLeaveCourage();
        }
        //stCourageData.qwLastTime = SDTimeSecs();
        //poPlayer->GetLocale().OnLeaveCourage();
    }
    m_mapAllEnterGropPlayer.clear();
    m_mapAllEnterPlayer.clear();
    for(CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.begin(); itr != m_mapGroupID2CourageGroup.end(); itr++)
    {
        m_oCourageGroupPool.Free(itr->second);
    }
    m_mapGroupID2CourageGroup.clear();
    return TRUE;
}

BOOL	CCourageMgr::OnSave()
{
    //活动是否开启
    if(m_bActivityOver)
    {
        //活动未开始
        return TRUE;
    }

    if (m_m_byActivityID == 0)
    {
        return TRUE;
    }

    if (SDTimeSecs() - m_qwSaveTime < 10)
    {
        return TRUE;
    }

    m_qwSaveTime = SDTimeSecs();

    static DT_COURAGE_GROUP_DATA_LST stDT_COURAGE_GROUP_DATA_LST;
    memset(&stDT_COURAGE_GROUP_DATA_LST, 0x00, sizeof(DT_COURAGE_GROUP_DATA_LST));
    stDT_COURAGE_GROUP_DATA_LST.qwOverTime		= m_qwOverTime;
    stDT_COURAGE_GROUP_DATA_LST.qwStartTime		= m_qwStartTime;
    stDT_COURAGE_GROUP_DATA_LST.qwSaveTime		= m_qwSaveTime;
    stDT_COURAGE_GROUP_DATA_LST.byActivityID		= m_m_byActivityID;
    for(CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.begin(); itr != m_mapGroupID2CourageGroup.end(); itr++)
    {
        DT_COURAGE_GROUP_DATA &stDT_COURAGE_GROUP_DATA = stDT_COURAGE_GROUP_DATA_LST.astGroupData[stDT_COURAGE_GROUP_DATA_LST.wGroupNum++];
        memcpy(&stDT_COURAGE_GROUP_DATA, &itr->second->GetDT_COURAGE_GROUP_DATA(), sizeof(DT_COURAGE_GROUP_DATA));
    }
    if (stDT_COURAGE_GROUP_DATA_LST.wGroupNum == 0)
    {
        return TRUE;
    }
    //保存数据
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVE_COURAGE_DATA);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return FALSE;
    }
    poCMD->SetUserData((VOID*)(&stDT_COURAGE_GROUP_DATA_LST), 0);
    return gsapi::GetDBMgr()->AddCommand(poCMD);
}

BOOL CCourageMgr::InitModuleDataFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

    CHAR szSql[1024] = {0};
    sprintf(szSql, "select * from module where KeyID = '%d';", EMDT_COURAGE);

    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );

    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            UINT32 dwLen = pRecordSet->GetFieldLengthByName( "Value" );
            if (0 != dwLen)
            {
                DT_COURAGE_GROUP_DATA_LST stDT_COURAGE_GROUP_DATA = {0};
                if( -1 == stDT_COURAGE_GROUP_DATA.DecodeSaveData( pRecordSet->GetFieldValueByName("Value"), dwLen ) )
                {
                    SYS_CRITICAL( _SDT("[%s: %d] stDT_COURAGE_GROUP_DATA.DecodeSaveData Failed!"), MSG_MARK);
                    return FALSE;
                }
                /*m_mapGroupID2CourageGroup[stDT_COURAGE_GROUP_DATA.byActivityID] = stDT_COURAGE_GROUP_DATA;*/
                Init(stDT_COURAGE_GROUP_DATA);
                bHaveData = TRUE;
            }
        }
    }
    else if(SDDB_NO_RECORDSET == nRet)
    {
        if(NULL != pRecordSet)
        {
            pRecordSet->Release();
            pRecordSet = NULL;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}

BOOL CCourageMgr::InitModuleLogDataFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

    CHAR szSql[1024] = {0};
    sprintf(szSql, "select * from modulelog where KeyID = '%d' order by OverTime desc limit %u;", EMDT_COURAGE, MAX_BOSSB_BATTLE_LOG_NUM * 2);

    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            string strOverTime = pRecordSet->GetFieldValueByName( "OverTime" );
            UINT32 dwLen = pRecordSet->GetFieldLengthByName( "Value" );
            if (0 != dwLen)
            {
                DT_BOSS_BATTLE_DATA stDT_BOSS_BATTLE_DATA = {0};
                if( -1 == stDT_BOSS_BATTLE_DATA.DecodeSaveData( pRecordSet->GetFieldValueByName("Value"), dwLen ) )
                {
                    SYS_CRITICAL( _SDT("[%s: %d] DT_BOSS_BATTLE_DATA.DecodeSaveData Failed!"), MSG_MARK);
                    continue;
                }
                //m_mapBossBBLData[stDT_BOSS_BATTLE_DATA.qwOverTime] = stDT_BOSS_BATTLE_DATA;
            }
        }

    }
    else if(SDDB_NO_RECORDSET == nRet)
    {
        if(NULL != pRecordSet)
        {
            pRecordSet->Release();
            pRecordSet = NULL;
        }
        return TRUE;
    }
    else
    {
        SYS_CRITICAL( _SDT("[%s: %d] InitModuleLogDataFromDB exec sql failed[%s]!"), MSG_MARK, strError.c_str());
        return FALSE;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}

//打开勇气试炼
UINT16 CCourageMgr::OnOpenCourage(CPlayer* poPlayer, DT_COURAGE_INFO_CLI &stCourageInfo, DT_COURAGE_GROUP_DATA_CLI_LST &stCORUAGE_GROUP_DATA, UINT8& bySelect)
{
    if((NULL == poPlayer) && (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);
    if ( NULL == pBuild )
    {
        //活动未开放
        return ERR_OPEN_COURAGE::ID_LOCK;
    }

    pBuild->OnOpen();
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_OPEN_COURAGE::ID_UNOPEN;
    }
    UINT32 dwPlayerID = poPlayer->GetID();
    //添加管理
    if(m_mapAllEnterPlayer.find(dwPlayerID) == m_mapAllEnterPlayer.end())
    {
        m_mapAllEnterPlayer[dwPlayerID] = dwPlayerID;
    }
    //清除管理
    if (m_mapAllEnterGropPlayer.find(dwPlayerID) != m_mapAllEnterGropPlayer.end())
    {
        m_mapAllEnterGropPlayer.erase(dwPlayerID);
    }
    //清除数据
    ClearGroupMemberData(poPlayer);
    //是否重置数据
    ResetCourageData(poPlayer);
    //获取最新试炼次数
    GetCourageTimes(poPlayer);

    memset(&stCourageInfo, 0x00, sizeof(DT_COURAGE_INFO_CLI));

    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    stCourageData.dwGroupID = 0;
    stCourageData.byHavePass = EBR_NO_PASS;
    stCourageData.byHaveAward = 0;
    stCourageData.wAverageLevel = 0;

    memset(&stCourageData.stGroupPlayerInfo,0x00,sizeof(stCourageData.stGroupPlayerInfo));
    memset(&stCourageData.stSceneData,0x00,sizeof(stCourageData.stSceneData));
    memset(&stCourageData.stjewelryInfoLst,0x00,sizeof(stCourageData.stjewelryInfoLst));

    stCourageInfo.dwMaxCourageTimes = stCourageData.dwTotalTimes;
    stCourageInfo.dwLeftCourageTimes = stCourageData.dwTodayJoinTimes;//stCourageData.dwTotalTimes >= stCourageData.dwTodayJoinTimes ? stCourageData.dwTotalTimes - stCourageData.dwTodayJoinTimes : 0;
    stCourageInfo.dwHelpCourageTimes = stCourageData.dwTodayHelpTimes;
    //stCourageData.qwLastTime = SDTimeSecs();
    //"在协助n次，挑战次数+m"
    GetHelpPrompt(poPlayer, stCourageInfo.aszHelpCouragePrompt);
    //试炼次数用完
    stCourageInfo.byBattleType = stCourageData.dwTodayJoinTimes > 0 ? EBT_JOIN : EBT_HELP;
    CCourage* poCourage = poPlayer->GetCourage();
    if ( NULL == poCourage)
    {
		return ERR_OPEN_COURAGE::ID_UNOPEN;
    }
	bySelect = poCourage->GetSelectType();
	poCourage->GetCourageGroup(stCORUAGE_GROUP_DATA);
    //返回成功
    return ERR_OPEN_COURAGE::ID_SUCCESS;
}

VOID CCourageMgr::ResetCourageData(CPlayer* poPlayer)
{
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    if (!IsToday(stCourageData.qwLastTime))
    {
        UINT8 byBackNotity = stCourageData.byNotity;
        memset(&stCourageData, 0x00, sizeof(DT_COURAGE_DATA));
        stCourageData.dwTotalTimes = CLuamgr::Instance()->GetTotalFreeCourageTimes();
        stCourageData.dwTodayJoinTimes = CLuamgr::Instance()->GetTotalFreeCourageTimes();
        stCourageData.qwLastTime = SDTimeSecs();
        stCourageData.wGroupIndex = 0;
        stCourageData.byNotity = byBackNotity;
        stCourageData.byHavePass = EBR_NO_PASS;
		stCourageData.dwTodayHelpTimes = 0;
		stCourageData.dwGiveFreeTimes = ESM_NO;
        return;
    }
    const SCourageActivityTimeProp* pstActivityTimeProp = CCouraPropMgr::Instance()->GetActivityTimeProp();
    if(pstActivityTimeProp == NULL) //不在活动时间进入活动
    {
        stCourageData.wGroupIndex = 0;
        stCourageData.dwGroupID = 0;
        stCourageData.qwLastTime = SDTimeSecs();
        stCourageData.byHavePass = EBR_NO_PASS;
        return;
    }
    //在活动时间内 但是不是同一时段
    const SCourageActivityTimeProp* pstLastActivityTimeProp = CCouraPropMgr::Instance()->GetActivityTimeProp(stCourageData.qwLastTime);
    if (pstActivityTimeProp != pstLastActivityTimeProp)
    {
        stCourageData.wGroupIndex = 0;
        stCourageData.dwGroupID = 0;
        stCourageData.dwTodayHelpTimes = 0;
        stCourageData.qwLastTime = SDTimeSecs();
        stCourageData.byHavePass = EBR_NO_PASS;
        return;
    }
}

//打开创建队伍
UINT16 CCourageMgr::OnCourageOpenCreateGroup( CPlayer* poPlayer, PKT_CLIGS_COURAGE_OPEN_CREATE_GROUP_ACK& psAck)
{
    if((NULL == poPlayer) && (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        //建筑不存在
        return ERR_COURAGE_OPEN_CREATE_GROUP::ID_UNOPEN;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_COURAGE_OPEN_CREATE_GROUP::ID_UNOPEN;
    }

    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    if (stCourageData.dwTodayJoinTimes == 0)
    {
        //试炼次数不足
        return ERR_COURAGE_OPEN_CREATE_GROUP::ID_SUCCESS;
    }
    UINT8   byTownIdx = 1;
    UINT16	wSceneIdx = EIT_COURAGE;
    UINT16  wLevel = poPlayer->GetLevel();

    //从低级到高级排序
    while((CkGVEUnlock(wLevel, wSceneIdx, byTownIdx)) && (byTownIdx <= MAX_TOWN_PER_SCENE_NUM))
    {
        //UINT16	wScore = 0;
        //GetGVEInstanceRecordData( poPlayer, psAck.astTownInstanceInfo[byTownIdx - 1],wSceneIdx, byTownIdx);
        //psAck.awChallengeNums[byTownIdx - 1] = CreateGroupNum[byTownIdx - 1];
        byTownIdx++;
    }

    UINT8 byIndex = 0;
    //从高级到低级排序  暂时城镇写死 8个
    for (UINT8 byTownIdxTemp = 8; byTownIdxTemp > 0; byTownIdxTemp--)
    {
        if (CkGVEUnlock(wLevel,wSceneIdx,byTownIdxTemp) == FALSE)
        {
            continue;
        }
        GetGVEInstanceRecordData( poPlayer, psAck.astTownInstanceInfo[byIndex],wSceneIdx, byTownIdxTemp);
        psAck.awChallengeNums[byIndex++] = CreateGroupNum[byTownIdxTemp - 1];
    }
    //下一个副本未解锁的信息
    STownInstanceProp * pstTownInstanceProp = NULL;
    pstTownInstanceProp = CInstancePropMgr::Instance()->GetSceneInstance(wSceneIdx, byTownIdx);
    BYTE byMaxLevel = 0;
    //没有配置 已经是满级了 取玩家等级
    if (NULL == pstTownInstanceProp)
    {
        psAck.wMaxLevel = wLevel;
        byMaxLevel = 1;
    }
    else
    {
        for (UINT32 dwInstanceIdx = 1; dwInstanceIdx <= pstTownInstanceProp->m_mapInstance.size(); dwInstanceIdx++)
        {
            SInstanceProp* pstInstanceProp = NULL;
            pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(wSceneIdx, byTownIdx, dwInstanceIdx);
            if (NULL == pstInstanceProp)
            {
                psAck.wMaxLevel = wLevel;
                break;;
            }
            
            psAck.wMaxLevel = pstInstanceProp->wEnterLevel;
            break;
        }
    }
    if (byMaxLevel == 1)
    {
        psAck.stMaxSceneData.byTownIdx = 0;
    }
    else
    {
        psAck.stMaxSceneData.byTownIdx = byTownIdx;
    }
    psAck.byTownNum = --byTownIdx;
    psAck.stMaxSceneData.wSceneIdx = EIT_COURAGE;
    psAck.stMaxSceneData.byInstanceIdx = 1;
 
    return ERR_COURAGE_OPEN_CREATE_GROUP::ID_SUCCESS;
}

//确认创建队伍
UINT16 CCourageMgr::OnCourageCreateGroup(CPlayer* poPlayer, UINT8 byJoinType, DT_INSTANCE_ID wInstanceID, UINT32 &dwGroupID )
{
    if((NULL == poPlayer) && (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        //建筑不存在
        return ERR_COURAGE_CREATE_GROUP::ID_LOCK;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_COURAGE_CREATE_GROUP::ID_UNOPEN;
    }
    if(HaveBringHero(poPlayer) == FALSE)
    {
        return ERR_COURAGE_CREATE_GROUP::ID_NO_HERO;
    }
    //验证副本场景序号
    if (wInstanceID.wSceneIdx != EIT_COURAGE)
    {
        return ERR_COURAGE_CREATE_GROUP::ID_NO_SCENE;
    }

    STownInstanceProp * pstTownInstanceProp = NULL;
    pstTownInstanceProp = CInstancePropMgr::Instance()->GetSceneInstance(wInstanceID.wSceneIdx, wInstanceID.byTownIdx);
    if (NULL == pstTownInstanceProp)
    {
        return ERR_COURAGE_CREATE_GROUP::ID_NO_SCENE;
    }

    //判断索引是否越界
    if (wInstanceID.byInstanceIdx > pstTownInstanceProp->m_mapInstance.size())
    {
        return ERR_COURAGE_CREATE_GROUP::ID_NO_SCENE;;
    }
    SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(wInstanceID.wSceneIdx, wInstanceID.byTownIdx, wInstanceID.byInstanceIdx);
    if(NULL == pstInstanceProp)
    {
        return ERR_COURAGE_CREATE_GROUP::ID_NO_SCENE;;
    }

    if (pstInstanceProp->wEnterLevel > poPlayer->GetLevel())
    {
        return ERR_COURAGE_CREATE_GROUP::ID_NO_LEVEL;
    }
    //是否重置数据
    ResetCourageData(poPlayer);
    //获取最新试炼次数
    GetCourageTimes(poPlayer);
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    if (stCourageData.dwTodayJoinTimes == 0)
    {
        dwGroupID = 0;
        //试炼次数不足
        return ERR_COURAGE_CREATE_GROUP::ID_NO_TIMES;
    }
	DT_COURAGE_GROUP_DATA stCOURAGE_GROUP_DATA = {0};
    //验证是否在队伍中
    BOOL bHaveInGroup = HaveInGroup(poPlayer, stCOURAGE_GROUP_DATA, stCourageData.dwGroupID);
    if (bHaveInGroup == TRUE)
    {
        dwGroupID = stCourageData.dwGroupID;
        //返回成功
        return ERR_COURAGE_CREATE_GROUP::ID_SUCCESS;
    }

    stCourageData.dwGroupID = 0;
    if (m_mapGroupID2CourageGroup.size() >= MAX_COURAGE_GROUP_NUM)
    {
        //队伍已达最大数量，队伍创建失败
        return ERR_COURAGE_CREATE_GROUP::ID_MAX_GROUP;
    }
    //如果创建是帮派 则检查是否在帮派中
    if (byJoinType == EJT_FACTION)
    {
        UINT32 wFactionID = poPlayer->GetFactionID(poPlayer->GetID());
        if (wFactionID == 0)
        {
            return ERR_COURAGE_CREATE_GROUP::ID_NO_FACTION;
        }
    }
    //如果创建的是好友 则判断好友系统是否开放
    if (byJoinType == EJT_FRIEND)
    {
        CFriends *poFriends = poPlayer->GetFriends();
        if (NULL == poFriends)
        {
            return ERR_COURAGE_CREATE_GROUP::ID_NO_FACTION;
        }
    }

    dwCreateGroupID = dwCreateGroupID > MAX_GROUP_ID ? 1 : dwCreateGroupID;
    dwCreateGroupID = dwCreateGroupID > m_dwGroupID ? dwCreateGroupID : m_dwGroupID;
    byJoinType = byJoinType > EJT_FRIEND ? EJT_ALL : byJoinType;
    DT_COURAGE_GROUP_DATA  stDT_COURAGE_GROUP_DATA;
    memset(&stDT_COURAGE_GROUP_DATA, 0x00, sizeof(stDT_COURAGE_GROUP_DATA));
    CCourageGroup *poCourageGroup = m_oCourageGroupPool.Alloc();
    if (NULL == poCourageGroup)
    {
        return  ERR_COURAGE_CREATE_GROUP::ID_MAX_GROUP;
    }

    UINT16 wLevel = 0;
    stDT_COURAGE_GROUP_DATA.byJoinType = byJoinType;
    stDT_COURAGE_GROUP_DATA.dwGroupID = dwCreateGroupID++;
    stDT_COURAGE_GROUP_DATA.dwLeaderID = poPlayer->GetID();
    stDT_COURAGE_GROUP_DATA.wMinLevel = pstInstanceProp->wEnterLevel;
    stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].byBattleType = EBT_JOIN;
    stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].dwPlayerID = poPlayer->GetID();
    stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].byMemberType = EBI_LEADER;
    stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].byReadyType = EBR_READYING;
    stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].wLevel = poPlayer->GetLevel();
    stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].wKindID = poPlayer->GetCoachHeroKindID();
    stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].wHeroID = GetMaxLevelHero(poPlayer,wLevel);
    stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].wHeroLevel = wLevel;
    memcpy(&stDT_COURAGE_GROUP_DATA.stInstanceID,&wInstanceID,sizeof(DT_INSTANCE_ID));

    UINT16 wJingJieLevel = 0;
    if (NULL != poPlayer->GetJingJie())
    {
        wJingJieLevel = poPlayer->GetJingJie()->GetDT_JINGJIE_DATA().stBaseData.wLevel;
        stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].byColor = (UINT8)CJingJiePropMgr::Instance()->GetJingJieColor(wJingJieLevel); 
    }     
    else
    {
        stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].byColor = 0;
    }

    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    SDStrcpy(stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].aszDispPlayerName, stDT_PLAYER_BASE_DATA.aszDispName);
    stCourageData.wGroupIndex = stDT_COURAGE_GROUP_DATA.wPlayerNum;

    stDT_COURAGE_GROUP_DATA.wPlayerNum++;
    poCourageGroup->Init(stDT_COURAGE_GROUP_DATA);
    m_mapGroupID2CourageGroup[stDT_COURAGE_GROUP_DATA.dwGroupID] = poCourageGroup;
    dwGroupID = stDT_COURAGE_GROUP_DATA.dwGroupID;

    stCourageData.dwGroupID = stDT_COURAGE_GROUP_DATA.dwGroupID;
    //记录当前选择的副本信息
    stCourageData.stSceneData.wSceneIdx = wInstanceID.wSceneIdx;
    stCourageData.stSceneData.byTownIdx = wInstanceID.byTownIdx;
    stCourageData.stSceneData.byInstanceIdx = wInstanceID.byInstanceIdx;

    //当前副本队伍加1
    CreateGroupNum[wInstanceID.byTownIdx - 1]++;

    //打印日志
    USR_INFO(_SDT("[%s: %d]:PlayerID [%d]CreateGroup Success!"), MSG_MARK, poPlayer->GetID());
    //添加定时器
    //CTimerEventMgr::Instance()->AddCourageFireEvent(poPlayer->GetID());
    return ERR_COURAGE_CREATE_GROUP::ID_SUCCESS;
}

//加入队伍或者协助队伍
UINT16 CCourageMgr::OnCourageJoinGroup( CPlayer* poPlayer, UINT8 byBattleType, UINT32 dwGroupID)
{
    if((NULL == poPlayer) && (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        //建筑不存在
        return ERR_COURAGE_BATTLE_ENTER::ID_LOCK;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_COURAGE_BATTLE_ENTER::ID_UNOPEN;
    }
    if(HaveBringHero(poPlayer) == FALSE)
    {
        return ERR_COURAGE_BATTLE_ENTER::ID_NO_HERO;
    }
    //是否重置数据
    ResetCourageData(poPlayer);
    //获取最新试炼次数
    GetCourageTimes(poPlayer);

    DT_COURAGE_GROUP_DATA stCOURAGE_GROUP_DATA;
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    //验证是否在队伍中
    BOOL bHaveInGroup = HaveInGroup(poPlayer, stCOURAGE_GROUP_DATA, stCourageData.dwGroupID);
    if (bHaveInGroup == TRUE)
    {
        //返回成功
        return ERR_COURAGE_BATTLE_ENTER::ID_SUCCESS;
    }

    if ((stCourageData.dwTodayHelpTimes >= CSinglePramMgr::Instance()->GetCourageHelpTimes()) && (stCourageData.dwTodayJoinTimes == 0))
    {
		UINT64 qwPassTime = GetDayPassSecond();
		const CCourageActivityTimePropMap& mapTime = CCouraPropMgr::Instance()->GetCourageActivityEndTimeProp();
		if( qwPassTime < mapTime.begin()->second.dwEndTime)
		{
			return ERR_COURAGE_BATTLE_ENTER::ID_NO_HELP_TIMES_NOON;
		}
		else
		{
			return ERR_COURAGE_BATTLE_ENTER::ID_NO_HELP_TIMES_NIGHT;
		}
        return ERR_COURAGE_BATTLE_ENTER::ID_NO_HELP_TIMES;
    }
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(dwGroupID);
    if (itr == m_mapGroupID2CourageGroup.end())
    {
        return ERR_COURAGE_BATTLE_ENTER::ID_NO_GROUP;
    }
    UINT16 wPlayNum = itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum;
    //队伍是否已满
    if (wPlayNum >= MAX_COURAGE_GROUP_PLAYER_NUM)
    {
        //您所选择的队伍已经满人了
        return ERR_COURAGE_BATTLE_ENTER::ID_HAS_FULL;
    }
    if (poPlayer->GetLevel() < itr->second->GetDT_COURAGE_GROUP_DATA().wMinLevel)
    {
        //等级尚未达到加入的条件
        return ERR_COURAGE_BATTLE_ENTER::ID_NO_ENOUGH_LEVEL;
    }
    //保存组ID
    stCourageData.dwGroupID = dwGroupID;
    BOOL bRet = SetEnterGroupData(poPlayer);
    if (bRet == TRUE)
    {
        //加入队伍的玩家添加定时器
        //CTimerEventMgr::Instance()->AddCourageFireEvent(poPlayer->GetID());
        return ERR_COURAGE_BATTLE_ENTER::ID_SUCCESS;
    }

    return ERR_COURAGE_BATTLE_ENTER::ID_NO_GROUP;
    
}

//添加或者更改伙伴
UINT16 CCourageMgr::OnCourageOperateHero( CPlayer* poPlayer , DT_COURAGE_GROUP_HERO_DATA_CLI &stCOURAGE_GROUP_HERO_DATA)
{
    if((NULL == poPlayer) && (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        //活动未开放
        return ERR_COURAGE_OPERATE_HERO::ID_LOCK;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_COURAGE_OPERATE_HERO::ID_UNOPEN;
    }
    if(HaveBringHero(poPlayer) == FALSE)
    {
        return ERR_COURAGE_OPERATE_HERO::ID_NO_HERO;
    }
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(stCourageData.dwGroupID);
    if (itr == m_mapGroupID2CourageGroup.end())
    {
        //准备后不能进行此操作
        return ERR_COURAGE_OPERATE_HERO::ID_NO_IN_GROUP;
    }
    CFormationHeroShmemMap& mapFormationHero = poPlayer->GetFormationHeroMap();
    CHero* poHero = mapFormationHero.GetFistData();
    while(poHero)
    {
        const DT_HERO_BASE_DATA& stDT_HERO_BASE_DATA = poHero->GetDT_HERO_BASE_DATA();
        if (poPlayer->GetCoachHeroKindID() == stDT_HERO_BASE_DATA.wKindID)
        {
            poHero = mapFormationHero.GetNextData();
            continue;
        }
        stCOURAGE_GROUP_HERO_DATA.astHeroData[stCOURAGE_GROUP_HERO_DATA.wHeroNum].wLevel = stDT_HERO_BASE_DATA.wLevel;
        stCOURAGE_GROUP_HERO_DATA.astHeroData[stCOURAGE_GROUP_HERO_DATA.wHeroNum].wKindID = stDT_HERO_BASE_DATA.wKindID;
        SHeroProp* pstHeroProp = poHero->GetHeroProp();
        if (pstHeroProp != NULL)
        {
            SDStrncpy(stCOURAGE_GROUP_HERO_DATA.astHeroData[stCOURAGE_GROUP_HERO_DATA.wHeroNum].aszDispName, pstHeroProp->szName, NAME_LEN_MAX - 1);
        }
        stCOURAGE_GROUP_HERO_DATA.wHeroNum++;
        poHero = mapFormationHero.GetNextData();
    }

    return ERR_COURAGE_OPERATE_HERO::ID_SUCCESS;
}

//确认添加或者更改伙伴
UINT16 CCourageMgr::OnCourageConfirmHero( CPlayer* poPlayer , UINT16 dwKindID, DT_COURAGE_HERO_DATA& stCOURAGE_HERO_DATA)
{
    if((NULL == poPlayer) && (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        //建筑不存在
        return ERR_COURAGE_CONFIRM_HERO::ID_LOCK;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_COURAGE_CONFIRM_HERO::ID_UNOPEN;
    }
    if(HaveBringHero(poPlayer) == FALSE)
    {
        return ERR_COURAGE_CONFIRM_HERO::ID_NO_HERO;
    }
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(stCourageData.dwGroupID);
    if ((itr == m_mapGroupID2CourageGroup.end()) || (itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[stCourageData.wGroupIndex].dwPlayerID != poPlayer->GetID()))
    {
        //尚未加入队伍不能进行此操作
        return ERR_COURAGE_CONFIRM_HERO::ID_NO_IN_GROUP;
    }

    if (itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[stCourageData.wGroupIndex].byReadyType == EBR_READY)
    {
        //准备后不能进行此操作
        return ERR_COURAGE_CONFIRM_HERO::ID_HAS_REANY;
    }
    
    CHero *pHero = poPlayer->GetHero(dwKindID);
    if (pHero == NULL)
    {
        //武将不存在
        return ERR_COURAGE_CONFIRM_HERO::ID_NO_HERO_INFO;
    }

    stCOURAGE_HERO_DATA.wKindID = dwKindID;
    const DT_HERO_BASE_DATA& stDT_HERO_BASE_DATA = pHero->GetDT_HERO_BASE_DATA();
    stCOURAGE_HERO_DATA.wLevel = stDT_HERO_BASE_DATA.wLevel;

    SHeroProp* pstHeroProp = pHero->GetHeroProp();
    if (pstHeroProp != NULL)
    {
        SDStrncpy(stCOURAGE_HERO_DATA.aszDispName, pstHeroProp->szName, NAME_LEN_MAX - 1);
    }

    itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[stCourageData.wGroupIndex].wHeroID = dwKindID;
    itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[stCourageData.wGroupIndex].wHeroLevel = stDT_HERO_BASE_DATA.wLevel;
    PKT_CLIGS_COURAGE_OPERATE_HERO_NTF stNtf;
    memcpy(&stNtf.stPlayerGroupData,&itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[stCourageData.wGroupIndex],sizeof(stNtf.stPlayerGroupData));
    for(UINT8 byIndex = 0; byIndex < MAX_COURAGE_GROUP_PLAYER_NUM; byIndex++)
    {
        CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer(itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndex].dwPlayerID);
        if((NULL == pPlayer) || (!pPlayer->CkLogin()) || (pPlayer->GetID() == poPlayer->GetID()))
        {
            continue;
        }
        //通知
        pPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_COURAGE_OPERATE_HERO_NTF);
    }

    return ERR_COURAGE_CONFIRM_HERO::ID_SUCCESS;

}

//踢掉队员
UINT16 CCourageMgr::OnCourageFireGroupMember( CPlayer* poPlayer, UINT32 dwGroupID, UINT32 dwPlayID,UINT32 &dwFirePlayID, UINT16 &wAverage)
{
    if((NULL == poPlayer) && (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        //建筑不存在
        return ERR_COURAGE_FIRE_GROUP_MEMBER::ID_LOCK;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_COURAGE_FIRE_GROUP_MEMBER::ID_UNOPEN;
    }
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(stCourageData.dwGroupID);
    if (itr == m_mapGroupID2CourageGroup.end())
    {
        //队伍不存在
        return ERR_COURAGE_FIRE_GROUP_MEMBER::ID_NO_GROUP;
    }
    DT_COURAGE_GROUP_DATA &stGroupData = itr->second->GetDT_COURAGE_GROUP_DATA();
    if ((poPlayer->GetID() != stGroupData.dwLeaderID) || (dwPlayID == poPlayer->GetID()))
    {
        //没有权限
        return ERR_COURAGE_FIRE_GROUP_MEMBER::ID_NO_POWER;
    }

    UINT8 byGroupIndex = GetInGroupIndex(dwPlayID,stGroupData.dwGroupID);
    if (byGroupIndex == INVALID_GROUP_INDEX)
    {
        return ERR_COURAGE_FIRE_GROUP_MEMBER::ID_SUCCESS;
    }
    memset(&stGroupData.astPlayerData[byGroupIndex], 0x00, sizeof(stGroupData.astPlayerData[byGroupIndex]));

    if (itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum > 0)
    {
        itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum--;
    }

    CPlayer* pPlayerNow = CPlayerMgr::Instance()->FindPlayer(dwPlayID);
    if (pPlayerNow == NULL)
    {
        //玩家不存在
        return ERR_COURAGE_FIRE_GROUP_MEMBER::ID_NO_PLAYER;
    }

    //给被T玩家通知 被T了
    PKT_CLIGS_COURAGE_PLAYER_FIRED_NTF stNtfNow;
    stNtfNow.byErrCode = ERR_COURAGE_PLAYER_FIRED::ID_SUCCESS;
    SetPlayerGroupValue(pPlayerNow, stNtfNow.stPlayerCourageData, stNtfNow.stGroupData);

    //添加管理
    if(m_mapAllEnterPlayer.find(pPlayerNow->GetID()) == m_mapAllEnterPlayer.end())
    {
        m_mapAllEnterPlayer[pPlayerNow->GetID()] = pPlayerNow->GetID();
    }

    pPlayerNow->SendMsg((const CHAR*)(&stNtfNow), CLIGS_COURAGE_PLAYER_FIRED_NTF);
    //离开队伍场景
    pPlayerNow->GetLocale().OnLeaveCourage();
    //给队伍里面别的玩家发送通知
    PKT_CLIGS_COURAGE_QUIT_GROUP_NTF stNtf;
    stNtf.wAverageLevel = 0;
    stNtf.dwPlayerID = dwPlayID;
    for(UINT8 byIndex = 0; byIndex < MAX_COURAGE_GROUP_PLAYER_NUM; byIndex++)
    {
        CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer(itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndex].dwPlayerID);
        if((NULL == pPlayer) /*|| (!pPlayer->CkLogin())*/ || (pPlayer->GetID() == poPlayer->GetID()) || (pPlayer->GetID() == dwPlayID))
        {
            continue;
        }
        //给另外玩家推送通知
        pPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_COURAGE_QUIT_GROUP_NTF);
    }
    dwFirePlayID = dwPlayID;
    //更新队伍信息
    return ERR_COURAGE_FIRE_GROUP_MEMBER::ID_SUCCESS;
}

//购买试炼
UINT16 CCourageMgr::OnCourageBuyPractiseTimes( CPlayer* poPlayer , PKT_CLIGS_COURAGE_BUY_PRACTISE_TIMES_ACK &stk)
{
    if((NULL == poPlayer) && (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        //建筑不存在
        return ERR_COURAGE_BUY_PRACTISE_TIMES::ID_LOCK;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_COURAGE_BUY_PRACTISE_TIMES::ID_UNOPEN;
    }
    memset(&stk,0x00,sizeof(stk));
    //是否重置数据
    ResetCourageData(poPlayer);
    //获取最新试炼次数
    GetCourageTimes(poPlayer);

    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    //获取当前VIP等级可购买最大次数
    UINT16 wMaxTimes = CVipPropMgr::Instance()->GetIncNum(EVINF_COURAGE, poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel);//CLuamgr::Instance()->GetVipBuyCourageTimes(poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel);
    if (wMaxTimes <= stCourageData.dwBuyTimes)
    {
        //超过最大购买次数
        return ERR_COURAGE_BUY_PRACTISE_TIMES::ID_MAX_BUY_TIMES;
    }
    //byBuyTimes = stCourageData.byBuyTimes;
    UINT32 dwGold = CLuamgr::Instance()->GetFuncCost(poPlayer, EGCF_BUY_ENCOURAGE_TIMES, 0, stCourageData.dwBuyTimes);
    if (dwGold > poPlayer->GetDT_PLAYER_BASE_DATA().qwGold)
    {
        //元宝不足
        return ERR_COURAGE_BUY_PRACTISE_TIMES::ID_NO_GOLD;
    }
    poPlayer->DecGold(dwGold, CRecordMgr::EDGT_BUY_COURAGE_TIMES, stCourageData.dwBuyTimes++);

    stCourageData.dwBuyAddTimes++;
    stCourageData.dwTodayJoinTimes++;
    stk.byCurTimes = stCourageData.dwTodayJoinTimes;/*stCourageData.dwTotalTimes >= stCourageData.dwTodayJoinTimes ? stCourageData.dwTotalTimes - stCourageData.dwTodayJoinTimes : 0;*/
    stk.byTotalTimes = stCourageData.dwTotalTimes;
    stk.dwNextBuyGold = CLuamgr::Instance()->GetFuncCost(poPlayer, EGCF_BUY_ENCOURAGE_TIMES, 0, stCourageData.dwBuyTimes);
    stk.byLeftBuyTimes = wMaxTimes >= stCourageData.dwBuyTimes ? wMaxTimes - stCourageData.dwBuyTimes : 0;

    return ERR_COURAGE_BUY_PRACTISE_TIMES::ID_SUCCESS;
}


//玩家准备
UINT16 CCourageMgr::OnCouragePlayerReady( CPlayer* poPlayer, UINT8 byTpye, UINT8 &byReadyTpye )
{
    if((NULL == poPlayer) && (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        //建筑不存在
        return ERR_COURAGE_PLAYER_READY::ID_LOCK;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_COURAGE_PLAYER_READY::ID_UNOPEN;
    }
    if(HaveBringHero(poPlayer) == FALSE)
    {
        return ERR_COURAGE_PLAYER_READY::ID_NO_HERO;
    }
    byTpye = byTpye > EBR_READY ? EBR_READYING : byTpye;
    byReadyTpye = byTpye == EBR_READYING ? EBR_READY : EBR_READYING;
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(stCourageData.dwGroupID);
    if (itr == m_mapGroupID2CourageGroup.end())
    {
        //失败
        return ERR_COURAGE_PLAYER_READY::ID_NO_GROUP;
    }

    for (UINT8 byIndex = 0; byIndex < MAX_COURAGE_GROUP_PLAYER_NUM; byIndex++)
    {
        if (itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndex].dwPlayerID != poPlayer->GetID())
        {
            continue;
        }

        //验证战斗类型是否正确
        UINT8 byBattleType = stCourageData.dwTodayJoinTimes <= 0 ? EBT_HELP : EBT_JOIN;
        itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndex].byBattleType = byBattleType;
        itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndex].byReadyType = byReadyTpye;
        PKT_CLIGS_COURAGE_PLAYER_READY_NTF stNtf;
        stNtf.byReadyType = byReadyTpye;
        stNtf.dwPlayerID = poPlayer->GetID();
        for(UINT8 byIndexnew = 0; byIndexnew < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexnew++)
        {
            CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer(itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndexnew].dwPlayerID);
            if((NULL == pPlayer) /*|| (!pPlayer->CkLogin()) */|| (pPlayer->GetID() == poPlayer->GetID()))
            {
                continue;
            }
            //if (itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndexnew].byReceiveNotice == 1)
            //{
            //    continue;
            //}
            //通知
            pPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_COURAGE_PLAYER_READY_NTF);
        }
        //成功
        return ERR_COURAGE_PLAYER_READY::ID_SUCCESS;
    }
    return ERR_COURAGE_PLAYER_READY::ID_NO_GROUP;
}

//玩家开始 生成战斗信息
UINT16 CCourageMgr::OnCouragPlayerStart( CPlayer* poPlayer ,PKT_CLIGS_COURAGE_PLAYER_START_ACK5 &stk)
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        //建筑不存在
        return ERR_COURAGE_PLAYER_START::ID_LOCK;
    }

    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_COURAGE_PLAYER_START::ID_UNOPEN;
    }
    if(HaveBringHero(poPlayer) == FALSE)
    {
        return ERR_COURAGE_PLAYER_START::ID_NO_HERO;
    }

    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    if (stCourageData.dwTodayJoinTimes == 0)
    {
        USR_INFO(_SDT("[%s: %d]:PlayerID [%d]CourageTimes error,Times[%d]!"), MSG_MARK, poPlayer->GetID(),stCourageData.dwTodayJoinTimes);
        return ERR_COURAGE_PLAYER_START::ID_NO_POWER;
    }
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(stCourageData.dwGroupID);
    if (itr == m_mapGroupID2CourageGroup.end())
    {
        //队伍不存在
        return ERR_COURAGE_PLAYER_START::ID_NO_GROUP;
    }

    DT_COURAGE_GROUP_DATA &stGroupData = itr->second->GetDT_COURAGE_GROUP_DATA();
    //最少2人开始
    if ((stGroupData.wPlayerNum < (MAX_COURAGE_GROUP_PLAYER_NUM -1 )) || (stGroupData.wPlayerNum == 0))
    {
        //人数不够不能开始游戏哦
        return ERR_COURAGE_PLAYER_START::ID_NO_FULL;
    }

    //队长才能开始
    if (stGroupData.dwLeaderID != poPlayer->GetID())
    {
        //只有队长才有权限
        return ERR_COURAGE_PLAYER_START::ID_NO_POWER;
    }
    
    //是否都是准备状态
    for (UINT8 byIndex = 0; byIndex < MAX_COURAGE_GROUP_PLAYER_NUM; byIndex++)
    {
        if ((stGroupData.astPlayerData[byIndex].dwPlayerID == poPlayer->GetID()) || (stGroupData.astPlayerData[byIndex].dwPlayerID == 0))
        {
            continue;
        }

        if (stGroupData.astPlayerData[byIndex].byReadyType != EBR_READY)
        {
            //还有队员未准备
            return ERR_COURAGE_PLAYER_START::ID_NO_ALL_READY;
        }
    }

    //返回怪物信息
    DT_MONSTER_DATA_LIST* pstDT_MONSTER_DATA_LIST = CMonsterMgr::Instance()->GetInstanceMonster(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
    if(NULL == pstDT_MONSTER_DATA_LIST)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr pstDT_MONSTER_DATA_LIST pstInstanceProp[%d.%d.%d] pstDT_MONSTER_DATA_LIST is NULL!"), MSG_MARK, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
        return ERR_COMMON::ID_OTHER_ERR;
    }
    else
    {
        memcpy(&(stk.stMonsterInfo), pstDT_MONSTER_DATA_LIST, sizeof(DT_MONSTER_DATA_LIST));
    }

    //保存队员信息
    for (BYTE byIndex = 0 ;byIndex < MAX_COURAGE_GROUP_PLAYER_NUM; byIndex ++ )
    {
        DT_COURAGE_PLAYERID_HEROID_DATA& COURAGE_PLAYER_DATA = stk.stGroupPlayerInfo.astPlayerInfo[stk.stGroupPlayerInfo.byPlayerNum++];
        COURAGE_PLAYER_DATA.dwPlayID = stGroupData.astPlayerData[byIndex].dwPlayerID;
        COURAGE_PLAYER_DATA.wHeroID = stGroupData.astPlayerData[byIndex].wHeroID;
        COURAGE_PLAYER_DATA.wKindID = stGroupData.astPlayerData[byIndex].wKindID;
        COURAGE_PLAYER_DATA.byBattleType = stGroupData.astPlayerData[byIndex].byBattleType;
        COURAGE_PLAYER_DATA.byColor = stGroupData.astPlayerData[byIndex].byColor;

        CPlayer *pPlayerNew = CPlayerMgr::Instance()->FindPlayer(stGroupData.astPlayerData[byIndex].dwPlayerID);
        if (pPlayerNew == NULL)
        {
            continue;
        }
        CHero *pHero = pPlayerNew->GetCoachHero();
        if (pHero == NULL)
        {
            continue;
        }
        DT_HERO_BASE_DATA_CLI stDT_HERO_BASE_DATA_CLI;
        pHero->GetDT_HERO_BASE_DATA_CLI(stDT_HERO_BASE_DATA_CLI);
        COURAGE_PLAYER_DATA.dwExperience = stDT_HERO_BASE_DATA_CLI.dwExperience;
        COURAGE_PLAYER_DATA.dwUpgradeExperience = stDT_HERO_BASE_DATA_CLI.dwUpgradeExperience;
        COURAGE_PLAYER_DATA.wLevel = pPlayerNew->GetLevel();
        const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = pPlayerNew->GetDT_PLAYER_BASE_DATA();
        SDStrcpy(COURAGE_PLAYER_DATA.aszDispPlayerName, stDT_PLAYER_BASE_DATA.aszDispName);

    }

    //保存队伍平均等级
    itr->second->GetDT_COURAGE_GROUP_DATA().wAverageLevel = 0;

    vector<CHero*> vecHero;
    vecHero.resize(7);
    vecHero[0] = NULL;
    for (UINT16 wPlayerNum = 0; wPlayerNum < MAX_COURAGE_GROUP_PLAYER_NUM; wPlayerNum++)
    {
        DT_COURAGE_PLAYER_DATA &stDT_COURAGE_PLAYER_DATA = itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[wPlayerNum];
        DT_COURAGE_PLAYERID_HEROID_DATA& COURAGE_PLAYER_DATA = stk.stGroupPlayerInfo.astPlayerInfo[wPlayerNum];
        CPlayer* v_poPlayer = CPlayerMgr::Instance()->FindPlayer(stDT_COURAGE_PLAYER_DATA.dwPlayerID);
        if (NULL == v_poPlayer)
        {
            continue;
        }
        UINT16 wKindID = 0;
        const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stDT_COURAGE_PLAYER_DATA.dwPlayerID);
        if (poPlayerBaseData != NULL )
        {
            wKindID = poPlayerBaseData->wMainHeroKindID;
            CHero *v_poHero = v_poPlayer->GetCoachHero();
            if (NULL == v_poHero)
            {
                continue;
            }
            BYTE byflag = 0;
            //为战士 则站在前面
            if (wKindID == 1 || wKindID == 4)
            {
                byflag = 1;
                vecHero[wPlayerNum+1] = v_poHero;
                COURAGE_PLAYER_DATA.byCurAttackObjIdx = wPlayerNum+1;
            }
            else
            {
                byflag = 2;
                vecHero[wPlayerNum+4] = v_poHero;
                COURAGE_PLAYER_DATA.byCurAttackObjIdx = wPlayerNum+4;
            }
            v_poHero = v_poPlayer->GetHero(stDT_COURAGE_PLAYER_DATA.wHeroID);
            if (NULL == v_poHero)
            {
                continue;
            }
            if (byflag == 1)
            {
                vecHero[wPlayerNum+4] = v_poHero;
            }
            else if (byflag == 2)
            {
                vecHero[wPlayerNum+1] = v_poHero;
            }
        }
    }

    //对于玩家组队信息每次都不一样，每次创建后都要删除
    CPlayerGroup *poPlayerGroup = CPlayerGroupMgr::Instance()->CreatePlayerGroup(vecHero);
    vecHero.clear();
    if (NULL == poPlayerGroup)
    {
        return ERR_COURAGE_BATTLE::ID_FAILED;
    }
    
    //保存玩家组队信息 在客户端崩溃和换设备时候下发 
    for (BYTE byIndex = 0 ;byIndex < MAX_COURAGE_GROUP_PLAYER_NUM ; byIndex ++ )
    {
        CPlayer *pPlayerNew = CPlayerMgr::Instance()->FindPlayer(stGroupData.astPlayerData[byIndex].dwPlayerID);
        if (pPlayerNew == NULL)
        {
            continue;
        }
        CCourage *poCourage = pPlayerNew->GetCourage();
        if (poCourage == NULL)
        {
            continue;
        }
        DT_COURAGE_DATA &stCourageDataNew = poCourage->GetDT_COURAGE_DATA();
        memcpy(&stCourageDataNew.stGroupPlayerInfo,&stk.stGroupPlayerInfo,sizeof(stk.stGroupPlayerInfo));
    }


    BOOL bPass = EBR_PASS; //是否通过战斗

    CMonsterGroup* poMonsterGroup = CMonsterMgr::Instance()->FindMonsterGroup(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx, 1);
    if(NULL == poMonsterGroup)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr poMonsterGroup pstInstanceProp[%d.%d.%d] poMonsterGroup is NULL!"), MSG_MARK, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
        return ERR_COMMON::ID_OTHER_ERR;
    }

    if(!CBattleMgr::Instance()->Battle4(poPlayerGroup, poMonsterGroup, stk.stBattleInfoCli.astBattleInfo[stk.stBattleInfoCli.byBattleNum]))
    {
        SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr Battle is Failed!"), MSG_MARK);
        return ERR_COMMON::ID_OTHER_ERR;
    }
    if (EBT_FAILED == stk.stBattleInfoCli.astBattleInfo[stk.stBattleInfoCli.byBattleNum++].byBattleResult)
    {
        bPass = EBR_NO_PASS;
    }

    stk.byPassFlag = bPass;
    //通过并且有试炼次数
    if (bPass == EBR_PASS && stCourageData.dwTodayJoinTimes > 0)
    {
        RatePassBoxEnc(poPlayer,stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx,stk.stjewelryInfoLst);
    }

    //副本序号
    memcpy(&stk.stInstanceID, &stCourageData.stSceneData, sizeof(stCourageData.stSceneData));

    //给队伍里面人发通知   
    for (UINT8 byIndexPlayer = 0; byIndexPlayer < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexPlayer++)
    {
        CPlayer* pPlayerNow = CPlayerMgr::Instance()->FindPlayer(stGroupData.astPlayerData[byIndexPlayer].dwPlayerID);
        if ((pPlayerNow == NULL) || (pPlayerNow->GetID() == poPlayer->GetID()) || pPlayerNow->GetCourage() == NULL)
        {
            continue;
        }

        
        if (pPlayerNow->GetCliVer() >= VERSION_PET)
        {
            PKT_CLIGS_COURAGE_LEADER_START_NTF5 stNtfNow;
            memset(&stNtfNow,0x00,sizeof(stNtfNow));
            memcpy(&stNtfNow.stBattleInfo.stGroupPlayerInfo, &stk.stGroupPlayerInfo, sizeof(stk.stGroupPlayerInfo));  
            memcpy(&stNtfNow.stBattleInfo.stMonsterInfo, &stk.stMonsterInfo, sizeof(stk.stMonsterInfo));  
            memcpy(&stNtfNow.stBattleInfo.stInstanceID, &stk.stInstanceID, sizeof(stk.stInstanceID)); 

            stNtfNow.stBattleInfo.stBattleInfoCli.byBattleNum = stk.stBattleInfoCli.byBattleNum;
            memcpy( &stNtfNow.stBattleInfo.stBattleInfoCli.astBattleInfo[0], &stk.stBattleInfoCli.astBattleInfo[0], sizeof(stNtfNow.stBattleInfo.stBattleInfoCli.astBattleInfo[0]));

            stNtfNow.stBattleInfo.byPassFlag = bPass;

            DT_COURAGE_DATA &stCourageDataNow = pPlayerNow->GetCourage()->GetDT_COURAGE_DATA();
            //记录进入副本信息
            memcpy(&stCourageDataNow.stSceneData, &stCourageData.stSceneData,sizeof(stCourageData.stSceneData));
			//协助提示 无次数时候下发 队长是不可能下发的
			if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes == 0)
			{
				stNtfNow.stBattleInfo.byHelpID = GetHelpID(stCourageDataNow.dwTodayHelpTimes, stCourageDataNow.dwGiveFreeTimes);
				stCourageDataNow.dwTodayHelpTimes++;
			}

            if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes > 0)
            {
                RatePassBoxEnc(pPlayerNow,stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, 
                    stCourageData.stSceneData.byInstanceIdx,stNtfNow.stBattleInfo.stjewelryInfoLst);
            }
            pPlayerNow->SendMsg((CHAR*)&stNtfNow, CLIGS_COURAGE_LEADER_START_NTF5);
        }
		else if ( pPlayerNow->GetCliVer() >= VERSION_SOUL)
		{
			PKT_CLIGS_COURAGE_LEADER_START_NTF3 stNtfNow;
			memset(&stNtfNow,0x00,sizeof(stNtfNow));
			memcpy(&stNtfNow.stBattleInfo.stGroupPlayerInfo, &stk.stGroupPlayerInfo, sizeof(stk.stGroupPlayerInfo));  
			memcpy(&stNtfNow.stBattleInfo.stMonsterInfo, &stk.stMonsterInfo, sizeof(stk.stMonsterInfo));  
			memcpy(&stNtfNow.stBattleInfo.stInstanceID, &stk.stInstanceID, sizeof(stk.stInstanceID)); 

			stNtfNow.stBattleInfo.stBattleInfoCli.byBattleNum = stk.stBattleInfoCli.byBattleNum;
			CBattleMgr::Instance()->Convert5To4( stk.stBattleInfoCli.astBattleInfo[0], stNtfNow.stBattleInfo.stBattleInfoCli.astBattleInfo[0]);
			

			stNtfNow.stBattleInfo.byPassFlag = bPass;

			DT_COURAGE_DATA &stCourageDataNow = pPlayerNow->GetCourage()->GetDT_COURAGE_DATA();
			//记录进入副本信息
			memcpy(&stCourageDataNow.stSceneData, &stCourageData.stSceneData,sizeof(stCourageData.stSceneData));
			//协助提示 无次数时候下发 队长是不可能下发的
			if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes == 0)
			{
				stNtfNow.stBattleInfo.byHelpID = GetHelpID(stCourageDataNow.dwTodayHelpTimes, stCourageDataNow.dwGiveFreeTimes);
				stCourageDataNow.dwTodayHelpTimes++;
			}

			if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes > 0)
			{
				RatePassBoxEnc(pPlayerNow,stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, 
					stCourageData.stSceneData.byInstanceIdx,stNtfNow.stBattleInfo.stjewelryInfoLst);
			}
			pPlayerNow->SendMsg((CHAR*)&stNtfNow, CLIGS_COURAGE_LEADER_START_NTF3);
		}

        UINT32 dwPlayerID = pPlayerNow->GetID();
        //清除管理
        if (m_mapAllEnterGropPlayer.find(dwPlayerID) != m_mapAllEnterGropPlayer.end())
        {
            m_mapAllEnterGropPlayer.erase(dwPlayerID);
        }
        //删除解散的定时器
        //CTimerEventMgr::Instance()->DelCourageFireEvent(pPlayerNow->GetID());   
        pPlayerNow->GetLocale().OnEnterCourageBattle(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
    }
    
    //删除解散的定时器
    //CTimerEventMgr::Instance()->DelCourageFireEvent(poPlayer->GetID());   
    PassBattle(itr->second->GetDT_COURAGE_GROUP_DATA().dwGroupID,itr->second->GetDT_COURAGE_GROUP_DATA().wAverageLevel,bPass);
    
    //清除管理
    if (m_mapAllEnterGropPlayer.find(poPlayer->GetID()) != m_mapAllEnterGropPlayer.end())
    {
        m_mapAllEnterGropPlayer.erase(poPlayer->GetID());
    }

    CPlayerGroupMgr::Instance()->RemovePlayerGroup(poPlayerGroup);
    poPlayerGroup = NULL;

    poPlayer->GetLocale().OnEnterCourageBattle(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);

    CreateGroupNum[stCourageData.stSceneData.byTownIdx - 1] = CreateGroupNum[stCourageData.stSceneData.byTownIdx - 1] >= 1 ? CreateGroupNum[stCourageData.stSceneData.byTownIdx - 1] - 1 : 0;

    return ERR_COURAGE_PLAYER_START::ID_SUCCESS;
}

UINT16 CCourageMgr::OnCouragPlayerStart2( CPlayer* poPlayer ,PKT_CLIGS_COURAGE_PLAYER_START_ACK2 &stk)
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        //建筑不存在
        return ERR_COURAGE_PLAYER_START::ID_LOCK;
    }

    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_COURAGE_PLAYER_START::ID_UNOPEN;
    }
    if(HaveBringHero(poPlayer) == FALSE)
    {
        return ERR_COURAGE_PLAYER_START::ID_NO_HERO;
    }

    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    if (stCourageData.dwTodayJoinTimes == 0)
    {
        return ERR_COURAGE_PLAYER_START::ID_NO_POWER;
    }
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(stCourageData.dwGroupID);

    if (itr == m_mapGroupID2CourageGroup.end())
    {
        //队伍不存在
        return ERR_COURAGE_PLAYER_START::ID_NO_GROUP;
    }

    DT_COURAGE_GROUP_DATA &stGroupData = itr->second->GetDT_COURAGE_GROUP_DATA();

    //最少2人开始
    if ((stGroupData.wPlayerNum < (MAX_COURAGE_GROUP_PLAYER_NUM -1 )) || (stGroupData.wPlayerNum == 0))
    {
        //人数不够不能开始游戏哦
        return ERR_COURAGE_PLAYER_START::ID_NO_FULL;
    }

    //队长才能开始
    if (itr->second->GetDT_COURAGE_GROUP_DATA().dwLeaderID != poPlayer->GetID())
    {
        //只有队长才有权限
        return ERR_COURAGE_PLAYER_START::ID_NO_POWER;
    }

    //是否都是准备状态
    for (UINT8 byIndex = 0; byIndex < MAX_COURAGE_GROUP_PLAYER_NUM; byIndex++)
    {
        if ((stGroupData.astPlayerData[byIndex].dwPlayerID == poPlayer->GetID()) || (stGroupData.astPlayerData[byIndex].dwPlayerID == 0))
        {
            continue;
        }

        if (stGroupData.astPlayerData[byIndex].byReadyType != EBR_READY)
        {
            //还有队员未准备
            return ERR_COURAGE_PLAYER_START::ID_NO_ALL_READY;
        }
    }

    //返回怪物信息
    DT_MONSTER_DATA_LIST* pstDT_MONSTER_DATA_LIST = CMonsterMgr::Instance()->GetInstanceMonster(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
    if(NULL == pstDT_MONSTER_DATA_LIST)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr pstDT_MONSTER_DATA_LIST pstInstanceProp[%d . %d . %d] pstDT_MONSTER_DATA_LIST is NULL!"), MSG_MARK, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
        return ERR_COMMON::ID_OTHER_ERR;
    }
    else
    {
        memcpy(&(stk.stMonsterInfo), pstDT_MONSTER_DATA_LIST, sizeof(DT_MONSTER_DATA_LIST));
    }

    //保存队员信息
    for (BYTE byIndex = 0 ;byIndex < MAX_COURAGE_GROUP_PLAYER_NUM; byIndex ++ )
    {
        DT_COURAGE_PLAYERID_HEROID_DATA& COURAGE_PLAYER_DATA = stk.stGroupPlayerInfo.astPlayerInfo[stk.stGroupPlayerInfo.byPlayerNum++];
        COURAGE_PLAYER_DATA.dwPlayID = stGroupData.astPlayerData[byIndex].dwPlayerID;
        COURAGE_PLAYER_DATA.wHeroID = stGroupData.astPlayerData[byIndex].wHeroID;
        COURAGE_PLAYER_DATA.wKindID = stGroupData.astPlayerData[byIndex].wKindID;
        COURAGE_PLAYER_DATA.byBattleType = stGroupData.astPlayerData[byIndex].byBattleType;
        COURAGE_PLAYER_DATA.byColor = stGroupData.astPlayerData[byIndex].byColor;

        CPlayer *pPlayerNew = CPlayerMgr::Instance()->FindPlayer(stGroupData.astPlayerData[byIndex].dwPlayerID);
        if (pPlayerNew == NULL)
        {
            continue;
        }
        CHero *pHero = pPlayerNew->GetCoachHero();
        if (pHero == NULL)
        {
            continue;
        }
        DT_HERO_BASE_DATA_CLI stDT_HERO_BASE_DATA_CLI;
        pHero->GetDT_HERO_BASE_DATA_CLI(stDT_HERO_BASE_DATA_CLI);
        COURAGE_PLAYER_DATA.dwExperience = stDT_HERO_BASE_DATA_CLI.dwExperience;
        COURAGE_PLAYER_DATA.dwUpgradeExperience = stDT_HERO_BASE_DATA_CLI.dwUpgradeExperience;
        COURAGE_PLAYER_DATA.wLevel = pPlayerNew->GetLevel();
        const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = pPlayerNew->GetDT_PLAYER_BASE_DATA();
        SDStrcpy(COURAGE_PLAYER_DATA.aszDispPlayerName, stDT_PLAYER_BASE_DATA.aszDispName);

    }

    //保存队伍平均等级
    itr->second->GetDT_COURAGE_GROUP_DATA().wAverageLevel = 0;


    vector<CHero*> vecHero;
    vecHero.resize(7);
    vecHero[0] = NULL;
    for (UINT16 wPlayerNum = 0; wPlayerNum < MAX_COURAGE_GROUP_PLAYER_NUM; wPlayerNum++)
    {
        DT_COURAGE_PLAYER_DATA &stDT_COURAGE_PLAYER_DATA = stGroupData.astPlayerData[wPlayerNum];
        DT_COURAGE_PLAYERID_HEROID_DATA& COURAGE_PLAYER_DATA = stk.stGroupPlayerInfo.astPlayerInfo[wPlayerNum];
        CPlayer* v_poPlayer = CPlayerMgr::Instance()->FindPlayer(stDT_COURAGE_PLAYER_DATA.dwPlayerID);
        if (NULL == v_poPlayer)
        {
            continue;
        }
        UINT16 wKindID = 0;
        const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stDT_COURAGE_PLAYER_DATA.dwPlayerID);
        if (poPlayerBaseData != NULL )
        {
            wKindID = poPlayerBaseData->wMainHeroKindID;
            CHero *v_poHero = v_poPlayer->GetCoachHero();
            if (NULL == v_poHero)
            {
                continue;
            }
            BYTE byflag = 0;
            //为战士 则站在前面
            if (wKindID == 1 || wKindID == 4)
            {
                byflag = 1;
                vecHero[wPlayerNum+1] = v_poHero;
                COURAGE_PLAYER_DATA.byCurAttackObjIdx = wPlayerNum+1;
            }
            else
            {
                byflag = 2;
                vecHero[wPlayerNum+4] = v_poHero;
                COURAGE_PLAYER_DATA.byCurAttackObjIdx = wPlayerNum+4;
            }
            v_poHero = v_poPlayer->GetHero(stDT_COURAGE_PLAYER_DATA.wHeroID);
            if (NULL == v_poHero)
            {
                continue;
            }
            if (byflag == 1)
            {
                vecHero[wPlayerNum+4] = v_poHero;
            }
            else if (byflag == 2)
            {
                vecHero[wPlayerNum+1] = v_poHero;
            }
        }
    }

    //对于玩家组队信息每次都不一样，每次创建后都要删除
    CPlayerGroup *poPlayerGroup = CPlayerGroupMgr::Instance()->CreatePlayerGroup(vecHero);
    vecHero.clear();
    if (NULL == poPlayerGroup)
    {
        return ERR_COURAGE_BATTLE::ID_FAILED;
    }

    //保存玩家组队信息 在客户端崩溃和换设备时候下发 
    for (BYTE byIndex = 0 ;byIndex < MAX_COURAGE_GROUP_PLAYER_NUM ; byIndex ++ )
    {
        CPlayer *pPlayerNew = CPlayerMgr::Instance()->FindPlayer(stGroupData.astPlayerData[byIndex].dwPlayerID);
        if (pPlayerNew == NULL)
        {
            continue;
        }
        CCourage *poCourage = pPlayerNew->GetCourage();
        if (poCourage == NULL)
        {
            continue;
        }
        DT_COURAGE_DATA &stCourageDataNew = poCourage->GetDT_COURAGE_DATA();
        memcpy(&stCourageDataNew.stGroupPlayerInfo,&stk.stGroupPlayerInfo,sizeof(stk.stGroupPlayerInfo));
    }


    BOOL bPass = EBR_PASS; //是否通过战斗

    CMonsterGroup* poMonsterGroup = CMonsterMgr::Instance()->FindMonsterGroup(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx, 1);
    if(NULL == poMonsterGroup)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr poMonsterGroup pstInstanceProp[%d.%d.%d] poMonsterGroup is NULL!"), MSG_MARK, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
        return ERR_COMMON::ID_OTHER_ERR;
    }
	DT_BATTLE_DATA5 stBattleData = {0};
    if(!CBattleMgr::Instance()->Battle4(poPlayerGroup, poMonsterGroup, stBattleData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr Battle is Failed!"), MSG_MARK);
        return ERR_COMMON::ID_OTHER_ERR;
    }
	CBattleMgr::Instance()->Convert5To3(stBattleData, stk.stBattleInfoCli.astBattleInfo[stk.stBattleInfoCli.byBattleNum]);
    if (EBT_FAILED == stk.stBattleInfoCli.astBattleInfo[stk.stBattleInfoCli.byBattleNum++].byBattleResult)
    {
        bPass = EBR_NO_PASS;
    }

    stk.byPassFlag = bPass;
    //通过并且有试炼次数
    if (bPass == EBR_PASS && stCourageData.dwTodayJoinTimes > 0)
    {
        RatePassBoxEnc(poPlayer,stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx,stk.stjewelryInfoLst);
    }

    //副本序号
    memcpy(&stk.stInstanceID,&stCourageData.stSceneData,sizeof(stCourageData.stSceneData));

    //给队伍里面人发通知   
    for (UINT8 byIndexPlayer = 0; byIndexPlayer < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexPlayer++)
    {
        CPlayer* pPlayerNow = CPlayerMgr::Instance()->FindPlayer(stGroupData.astPlayerData[byIndexPlayer].dwPlayerID);
        if ((pPlayerNow == NULL) || (pPlayerNow->GetID() == poPlayer->GetID()) || pPlayerNow->GetCourage() == NULL)
        {
            continue;
        }
		if ( VER_PET <= pPlayerNow->GetCliVer())
		{
			PKT_CLIGS_COURAGE_LEADER_START_NTF5 stNtfNow;
			memset(&stNtfNow,0x00,sizeof(stNtfNow));
			memcpy(&stNtfNow.stBattleInfo.stGroupPlayerInfo, &stk.stGroupPlayerInfo, sizeof(stk.stGroupPlayerInfo));  
			memcpy(&stNtfNow.stBattleInfo.stMonsterInfo, &stk.stMonsterInfo, sizeof(stk.stMonsterInfo));  
			memcpy(&stNtfNow.stBattleInfo.stInstanceID, &stk.stInstanceID, sizeof(stk.stInstanceID));  

			stNtfNow.stBattleInfo.byPassFlag = bPass;
			stNtfNow.stBattleInfo.stBattleInfoCli.byBattleNum = stk.stBattleInfoCli.byBattleNum;
			memcpy(&stNtfNow.stBattleInfo.stBattleInfoCli.astBattleInfo[0], &stBattleData, sizeof(stNtfNow.stBattleInfo.stBattleInfoCli.astBattleInfo[0]));

			DT_COURAGE_DATA &stCourageDataNow = pPlayerNow->GetCourage()->GetDT_COURAGE_DATA();
			//记录进入副本信息
			memcpy(&stCourageDataNow.stSceneData, &stCourageData.stSceneData,sizeof(stCourageData.stSceneData));
			//协助提示 无次数时候下发 队长是不可能下发的
			if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes == 0)
			{
				stNtfNow.stBattleInfo.byHelpID = GetHelpID(stCourageDataNow.dwTodayHelpTimes, stCourageDataNow.dwGiveFreeTimes);
				stCourageDataNow.dwTodayHelpTimes++;
			}
			if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes > 0)
			{
				RatePassBoxEnc(pPlayerNow,stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, 
					stCourageData.stSceneData.byInstanceIdx,stNtfNow.stBattleInfo.stjewelryInfoLst);
			}
			pPlayerNow->SendMsg((CHAR*)&stNtfNow, CLIGS_COURAGE_LEADER_START_NTF5);
		}
		else if ( pPlayerNow->GetCliVer() >= VERSION_SOUL)
		{
			PKT_CLIGS_COURAGE_LEADER_START_NTF3 stNtfNow;
			memset(&stNtfNow,0x00,sizeof(stNtfNow));
			memcpy(&stNtfNow.stBattleInfo.stGroupPlayerInfo, &stk.stGroupPlayerInfo, sizeof(stk.stGroupPlayerInfo));  
			memcpy(&stNtfNow.stBattleInfo.stMonsterInfo, &stk.stMonsterInfo, sizeof(stk.stMonsterInfo));  
			memcpy(&stNtfNow.stBattleInfo.stInstanceID, &stk.stInstanceID, sizeof(stk.stInstanceID));  

			stNtfNow.stBattleInfo.byPassFlag = bPass;
			stNtfNow.stBattleInfo.stBattleInfoCli.byBattleNum = stk.stBattleInfoCli.byBattleNum;
			CBattleMgr::Instance()->Convert5To4(stBattleData, stNtfNow.stBattleInfo.stBattleInfoCli.astBattleInfo[0]);

			DT_COURAGE_DATA &stCourageDataNow = pPlayerNow->GetCourage()->GetDT_COURAGE_DATA();
			//记录进入副本信息
			memcpy(&stCourageDataNow.stSceneData, &stCourageData.stSceneData,sizeof(stCourageData.stSceneData));
			//协助提示 无次数时候下发 队长是不可能下发的
			if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes == 0)
			{
				stNtfNow.stBattleInfo.byHelpID = GetHelpID(stCourageDataNow.dwTodayHelpTimes, stCourageDataNow.dwGiveFreeTimes);
				stCourageDataNow.dwTodayHelpTimes++;
			}
			if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes > 0)
			{
				RatePassBoxEnc(pPlayerNow,stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, 
					stCourageData.stSceneData.byInstanceIdx,stNtfNow.stBattleInfo.stjewelryInfoLst);
			}
			USR_INFO("[%s:%u] Instance.SceneIdx=%u, TownIdx=%u, InstanceIdx=%u, PlayerID=%u", MSG_MARK, stk.stInstanceID.wSceneIdx, stk.stInstanceID.byTownIdx, stk.stInstanceID.byInstanceIdx, pPlayerNow->GetID());
			pPlayerNow->SendMsg((CHAR*)&stNtfNow, CLIGS_COURAGE_LEADER_START_NTF3);
		}
        else
        {
            PKT_CLIGS_COURAGE_LEADER_START_NTF2 stNtfNow;
            memset(&stNtfNow,0x00,sizeof(stNtfNow));
            memcpy(&stNtfNow.stBattleInfo.stGroupPlayerInfo, &stk.stGroupPlayerInfo, sizeof(stk.stGroupPlayerInfo));  
            memcpy(&stNtfNow.stBattleInfo.stMonsterInfo, &stk.stMonsterInfo, sizeof(stk.stMonsterInfo));  
            memcpy(&stNtfNow.stBattleInfo.stInstanceID, &stk.stInstanceID, sizeof(stk.stInstanceID));  
			CBattleMgr::Instance()->Convert5To3(stBattleData, stNtfNow.stBattleInfo.stBattleInfoCli.astBattleInfo[0]);
			stNtfNow.stBattleInfo.stBattleInfoCli.byBattleNum = 1;
            stNtfNow.stBattleInfo.byPassFlag = bPass;

            DT_COURAGE_DATA &stCourageDataNow = pPlayerNow->GetCourage()->GetDT_COURAGE_DATA();
            //记录进入副本信息
            memcpy(&stCourageDataNow.stSceneData, &stCourageData.stSceneData,sizeof(stCourageData.stSceneData));
			//协助提示 无次数时候下发 队长是不可能下发的
			if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes == 0)
			{
				stNtfNow.stBattleInfo.byHelpID = GetHelpID(stCourageDataNow.dwTodayHelpTimes, stCourageDataNow.dwGiveFreeTimes);
				stCourageDataNow.dwTodayHelpTimes++;
			}
            if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes > 0)
            {
                RatePassBoxEnc(pPlayerNow,stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, 
                    stCourageData.stSceneData.byInstanceIdx,stNtfNow.stBattleInfo.stjewelryInfoLst);
            }
            pPlayerNow->SendMsg((CHAR*)&stNtfNow, CLIGS_COURAGE_LEADER_START_NTF2);
        }

        UINT32 dwPlayerID = pPlayerNow->GetID();
        //清除管理
        if (m_mapAllEnterGropPlayer.find(dwPlayerID) != m_mapAllEnterGropPlayer.end())
        {
            m_mapAllEnterGropPlayer.erase(dwPlayerID);
        }
        pPlayerNow->GetLocale().OnEnterCourageBattle(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
    }

    //删除解散的定时器
    //CTimerEventMgr::Instance()->DelCourageFireEvent(poPlayer->GetID());   
    PassBattle(itr->second->GetDT_COURAGE_GROUP_DATA().dwGroupID,itr->second->GetDT_COURAGE_GROUP_DATA().wAverageLevel,bPass);

    //清除管理
    if (m_mapAllEnterGropPlayer.find(poPlayer->GetID()) != m_mapAllEnterGropPlayer.end())
    {
        m_mapAllEnterGropPlayer.erase(poPlayer->GetID());
    }

    CPlayerGroupMgr::Instance()->RemovePlayerGroup(poPlayerGroup);
    poPlayerGroup = NULL;

    poPlayer->GetLocale().OnEnterCourageBattle(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);

    CreateGroupNum[stCourageData.stSceneData.byTownIdx - 1] = CreateGroupNum[stCourageData.stSceneData.byTownIdx - 1] >= 1 ? CreateGroupNum[stCourageData.stSceneData.byTownIdx - 1] - 1 : 0;

    return ERR_COURAGE_PLAYER_START::ID_SUCCESS;
}

UINT16 CCourageMgr::OnCouragPlayerStart3( CPlayer* poPlayer ,PKT_CLIGS_COURAGE_PLAYER_START_ACK3 &stk)
{
	if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
	{
		RETURN_OTHER_ERR;
	}
	CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

	if ( NULL == pBuild )
	{
		//建筑不存在
		return ERR_COURAGE_PLAYER_START::ID_LOCK;
	}

	//是否到活动时间
	if (m_bActivityOver)
	{
		return ERR_COURAGE_PLAYER_START::ID_UNOPEN;
	}
	if(HaveBringHero(poPlayer) == FALSE)
	{
		return ERR_COURAGE_PLAYER_START::ID_NO_HERO;
	}

	DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
	if (stCourageData.dwTodayJoinTimes == 0)
	{
		return ERR_COURAGE_PLAYER_START::ID_NO_POWER;
	}
	CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(stCourageData.dwGroupID);

	if (itr == m_mapGroupID2CourageGroup.end())
	{
		//队伍不存在
		return ERR_COURAGE_PLAYER_START::ID_NO_GROUP;
	}

	DT_COURAGE_GROUP_DATA &stGroupData = itr->second->GetDT_COURAGE_GROUP_DATA();

	//最少2人开始
	if ((stGroupData.wPlayerNum < (MAX_COURAGE_GROUP_PLAYER_NUM -1 )) || (stGroupData.wPlayerNum == 0))
	{
		//人数不够不能开始游戏哦
		return ERR_COURAGE_PLAYER_START::ID_NO_FULL;
	}

	//队长才能开始
	if (itr->second->GetDT_COURAGE_GROUP_DATA().dwLeaderID != poPlayer->GetID())
	{
		//只有队长才有权限
		return ERR_COURAGE_PLAYER_START::ID_NO_POWER;
	}

	//是否都是准备状态
	for (UINT8 byIndex = 0; byIndex < MAX_COURAGE_GROUP_PLAYER_NUM; byIndex++)
	{
		if ((stGroupData.astPlayerData[byIndex].dwPlayerID == poPlayer->GetID()) || (stGroupData.astPlayerData[byIndex].dwPlayerID == 0))
		{
			continue;
		}

		if (stGroupData.astPlayerData[byIndex].byReadyType != EBR_READY)
		{
			//还有队员未准备
			return ERR_COURAGE_PLAYER_START::ID_NO_ALL_READY;
		}
	}

	//返回怪物信息
	DT_MONSTER_DATA_LIST* pstDT_MONSTER_DATA_LIST = CMonsterMgr::Instance()->GetInstanceMonster(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
	if(NULL == pstDT_MONSTER_DATA_LIST)
	{
		SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr pstDT_MONSTER_DATA_LIST pstInstanceProp[%d . %d . %d] pstDT_MONSTER_DATA_LIST is NULL!"), MSG_MARK, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
		return ERR_COMMON::ID_OTHER_ERR;
	}
	else
	{
		memcpy(&(stk.stMonsterInfo), pstDT_MONSTER_DATA_LIST, sizeof(DT_MONSTER_DATA_LIST));
	}

	//保存队员信息
	for (BYTE byIndex = 0 ;byIndex < MAX_COURAGE_GROUP_PLAYER_NUM; byIndex ++ )
	{
		DT_COURAGE_PLAYERID_HEROID_DATA& COURAGE_PLAYER_DATA = stk.stGroupPlayerInfo.astPlayerInfo[stk.stGroupPlayerInfo.byPlayerNum++];
		COURAGE_PLAYER_DATA.dwPlayID = stGroupData.astPlayerData[byIndex].dwPlayerID;
		COURAGE_PLAYER_DATA.wHeroID = stGroupData.astPlayerData[byIndex].wHeroID;
		COURAGE_PLAYER_DATA.wKindID = stGroupData.astPlayerData[byIndex].wKindID;
		COURAGE_PLAYER_DATA.byBattleType = stGroupData.astPlayerData[byIndex].byBattleType;
		COURAGE_PLAYER_DATA.byColor = stGroupData.astPlayerData[byIndex].byColor;

		CPlayer *pPlayerNew = CPlayerMgr::Instance()->FindPlayer(stGroupData.astPlayerData[byIndex].dwPlayerID);
		if (pPlayerNew == NULL)
		{
			continue;
		}
		CHero *pHero = pPlayerNew->GetCoachHero();
		if (pHero == NULL)
		{
			continue;
		}
		DT_HERO_BASE_DATA_CLI stDT_HERO_BASE_DATA_CLI;
		pHero->GetDT_HERO_BASE_DATA_CLI(stDT_HERO_BASE_DATA_CLI);
		COURAGE_PLAYER_DATA.dwExperience = stDT_HERO_BASE_DATA_CLI.dwExperience;
		COURAGE_PLAYER_DATA.dwUpgradeExperience = stDT_HERO_BASE_DATA_CLI.dwUpgradeExperience;
		COURAGE_PLAYER_DATA.wLevel = pPlayerNew->GetLevel();
		const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = pPlayerNew->GetDT_PLAYER_BASE_DATA();
		SDStrcpy(COURAGE_PLAYER_DATA.aszDispPlayerName, stDT_PLAYER_BASE_DATA.aszDispName);

	}

	//保存队伍平均等级
	itr->second->GetDT_COURAGE_GROUP_DATA().wAverageLevel = 0;


	//生成队伍战力信息
	// BOOL bSucess = CreateGroupPower(itr->second->GetDT_COURAGE_GROUP_DATA().dwGroupID ,stk.stGroupPlayerInfo.astPlayerInfo);

	vector<CHero*> vecHero;
	vecHero.resize(7);
	vecHero[0] = NULL;
	for (UINT16 wPlayerNum = 0; wPlayerNum < MAX_COURAGE_GROUP_PLAYER_NUM; wPlayerNum++)
	{
		DT_COURAGE_PLAYER_DATA &stDT_COURAGE_PLAYER_DATA = stGroupData.astPlayerData[wPlayerNum];
		DT_COURAGE_PLAYERID_HEROID_DATA& COURAGE_PLAYER_DATA = stk.stGroupPlayerInfo.astPlayerInfo[wPlayerNum];
		CPlayer* v_poPlayer = CPlayerMgr::Instance()->FindPlayer(stDT_COURAGE_PLAYER_DATA.dwPlayerID);
		if (NULL == v_poPlayer)
		{
			continue;
		}
		UINT16 wKindID = 0;
		const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stDT_COURAGE_PLAYER_DATA.dwPlayerID);
		if (poPlayerBaseData != NULL )
		{
			wKindID = poPlayerBaseData->wMainHeroKindID;
			CHero *v_poHero = v_poPlayer->GetCoachHero();
			if (NULL == v_poHero)
			{
				continue;
			}
			BYTE byflag = 0;
			//为战士 则站在前面
			if (wKindID == 1 || wKindID == 4)
			{
				byflag = 1;
				vecHero[wPlayerNum+1] = v_poHero;
				COURAGE_PLAYER_DATA.byCurAttackObjIdx = wPlayerNum+1;
			}
			else
			{
				byflag = 2;
				vecHero[wPlayerNum+4] = v_poHero;
				COURAGE_PLAYER_DATA.byCurAttackObjIdx = wPlayerNum+4;
			}
			v_poHero = v_poPlayer->GetHero(stDT_COURAGE_PLAYER_DATA.wHeroID);
			if (NULL == v_poHero)
			{
				continue;
			}
			if (byflag == 1)
			{
				vecHero[wPlayerNum+4] = v_poHero;
			}
			else if (byflag == 2)
			{
				vecHero[wPlayerNum+1] = v_poHero;
			}
		}
	}

	//对于玩家组队信息每次都不一样，每次创建后都要删除
	CPlayerGroup *poPlayerGroup = CPlayerGroupMgr::Instance()->CreatePlayerGroup(vecHero);
	vecHero.clear();
	if (NULL == poPlayerGroup)
	{
		return ERR_COURAGE_BATTLE::ID_FAILED;
	}

	//保存玩家组队信息 在客户端崩溃和换设备时候下发 
	for (BYTE byIndex = 0 ;byIndex < MAX_COURAGE_GROUP_PLAYER_NUM ; byIndex ++ )
	{
		CPlayer *pPlayerNew = CPlayerMgr::Instance()->FindPlayer(stGroupData.astPlayerData[byIndex].dwPlayerID);
		if (pPlayerNew == NULL)
		{
			continue;
		}
		CCourage *poCourage = pPlayerNew->GetCourage();
		if (poCourage == NULL)
		{
			continue;
		}
		DT_COURAGE_DATA &stCourageDataNew = poCourage->GetDT_COURAGE_DATA();
		memcpy(&stCourageDataNew.stGroupPlayerInfo,&stk.stGroupPlayerInfo,sizeof(stk.stGroupPlayerInfo));
	}


	BOOL bPass = EBR_PASS; //是否通过战斗

	CMonsterGroup* poMonsterGroup = CMonsterMgr::Instance()->FindMonsterGroup(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx, 1);
	if(NULL == poMonsterGroup)
	{
		SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr poMonsterGroup pstInstanceProp[%d.%d.%d] poMonsterGroup is NULL!"), MSG_MARK, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
		return ERR_COMMON::ID_OTHER_ERR;
	}
	DT_BATTLE_DATA5 stBattle = {0};
	if(!CBattleMgr::Instance()->Battle4(poPlayerGroup, poMonsterGroup, stBattle))
	{
		SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr Battle is Failed!"), MSG_MARK);
		return ERR_COMMON::ID_OTHER_ERR;
	}
	CBattleMgr::Instance()->Convert5To4(stBattle,  stk.stBattleInfoCli.astBattleInfo[stk.stBattleInfoCli.byBattleNum++]);
	if (EBT_FAILED == stk.stBattleInfoCli.astBattleInfo[stk.stBattleInfoCli.byBattleNum - 1].byBattleResult)
	{
		bPass = EBR_NO_PASS;
	}

	stk.byPassFlag = bPass;
	//通过并且有试炼次数
	if (bPass == EBR_PASS && stCourageData.dwTodayJoinTimes > 0)
	{
		RatePassBoxEnc(poPlayer,stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx,stk.stjewelryInfoLst);
	}

	//副本序号
	memcpy(&stk.stInstanceID,&stCourageData.stSceneData,sizeof(stCourageData.stSceneData));

	//给队伍里面人发通知   
	for (UINT8 byIndexPlayer = 0; byIndexPlayer < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexPlayer++)
	{
		CPlayer* pPlayerNow = CPlayerMgr::Instance()->FindPlayer(stGroupData.astPlayerData[byIndexPlayer].dwPlayerID);
		if ((pPlayerNow == NULL) || (pPlayerNow->GetID() == poPlayer->GetID()) || pPlayerNow->GetCourage() == NULL)
		{
			continue;
		}

		if ( VER_PET <=  pPlayerNow->GetCliVer())
		{
			PKT_CLIGS_COURAGE_LEADER_START_NTF5 stNtfNow;
			memset(&stNtfNow,0x00,sizeof(stNtfNow));
			memcpy(&stNtfNow.stBattleInfo.stGroupPlayerInfo, &stk.stGroupPlayerInfo, sizeof(stk.stGroupPlayerInfo));  
			memcpy(&stNtfNow.stBattleInfo.stMonsterInfo, &stk.stMonsterInfo, sizeof(stk.stMonsterInfo));  
			memcpy(&stNtfNow.stBattleInfo.stInstanceID, &stk.stInstanceID, sizeof(stk.stInstanceID));  

			stNtfNow.stBattleInfo.byPassFlag = bPass;
			stNtfNow.stBattleInfo.stBattleInfoCli.byBattleNum = stk.stBattleInfoCli.byBattleNum;
			memcpy(&stNtfNow.stBattleInfo.stBattleInfoCli.astBattleInfo[0], &stBattle, sizeof(stNtfNow.stBattleInfo.stBattleInfoCli.astBattleInfo[0]));

			DT_COURAGE_DATA &stCourageDataNow = pPlayerNow->GetCourage()->GetDT_COURAGE_DATA();
			//记录进入副本信息
			memcpy(&stCourageDataNow.stSceneData, &stCourageData.stSceneData,sizeof(stCourageData.stSceneData));
			//协助提示 无次数时候下发 队长是不可能下发的
			if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes == 0)
			{
				stNtfNow.stBattleInfo.byHelpID = GetHelpID(stCourageDataNow.dwTodayHelpTimes, stCourageDataNow.dwGiveFreeTimes);
				stCourageDataNow.dwTodayHelpTimes++;
			}
			if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes > 0)
			{
				RatePassBoxEnc(pPlayerNow,stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, 
					stCourageData.stSceneData.byInstanceIdx,stNtfNow.stBattleInfo.stjewelryInfoLst);
			}
			pPlayerNow->SendMsg((CHAR*)&stNtfNow, CLIGS_COURAGE_LEADER_START_NTF5);
		}
		else if ( pPlayerNow->GetCliVer() >= VERSION_SOUL)
		{
			PKT_CLIGS_COURAGE_LEADER_START_NTF3 stNtfNow;
			memset(&stNtfNow,0x00,sizeof(stNtfNow));
			memcpy(&stNtfNow.stBattleInfo.stGroupPlayerInfo, &stk.stGroupPlayerInfo, sizeof(stk.stGroupPlayerInfo));  
			memcpy(&stNtfNow.stBattleInfo.stMonsterInfo, &stk.stMonsterInfo, sizeof(stk.stMonsterInfo));  
			memcpy(&stNtfNow.stBattleInfo.stInstanceID, &stk.stInstanceID, sizeof(stk.stInstanceID));  

			stNtfNow.stBattleInfo.byPassFlag = bPass;
			stNtfNow.stBattleInfo.stBattleInfoCli.byBattleNum = stk.stBattleInfoCli.byBattleNum;
			CBattleMgr::Instance()->Convert5To4(stBattle, stNtfNow.stBattleInfo.stBattleInfoCli.astBattleInfo[0]);
			DT_COURAGE_DATA &stCourageDataNow = pPlayerNow->GetCourage()->GetDT_COURAGE_DATA();
			//记录进入副本信息
			memcpy(&stCourageDataNow.stSceneData, &stCourageData.stSceneData,sizeof(stCourageData.stSceneData));
			//协助提示 无次数时候下发 队长是不可能下发的
			if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes == 0)
			{
				stNtfNow.stBattleInfo.byHelpID = GetHelpID(stCourageDataNow.dwTodayHelpTimes, stCourageDataNow.dwGiveFreeTimes);
				stCourageDataNow.dwTodayHelpTimes++;
			}
			if (bPass == EBR_PASS && stCourageDataNow.dwTodayJoinTimes > 0)
			{
				RatePassBoxEnc(pPlayerNow,stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, 
					stCourageData.stSceneData.byInstanceIdx,stNtfNow.stBattleInfo.stjewelryInfoLst);
			}
			pPlayerNow->SendMsg((CHAR*)&stNtfNow, CLIGS_COURAGE_LEADER_START_NTF3);
		}

		UINT32 dwPlayerID = pPlayerNow->GetID();
		//清除管理
		if (m_mapAllEnterGropPlayer.find(dwPlayerID) != m_mapAllEnterGropPlayer.end())
		{
			m_mapAllEnterGropPlayer.erase(dwPlayerID);
		}
		//删除解散的定时器
		//CTimerEventMgr::Instance()->DelCourageFireEvent(pPlayerNow->GetID());   
		pPlayerNow->GetLocale().OnEnterCourageBattle(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
	}

	//删除解散的定时器
	//CTimerEventMgr::Instance()->DelCourageFireEvent(poPlayer->GetID());   
	PassBattle(itr->second->GetDT_COURAGE_GROUP_DATA().dwGroupID,itr->second->GetDT_COURAGE_GROUP_DATA().wAverageLevel,bPass);

	//清除管理
	if (m_mapAllEnterGropPlayer.find(poPlayer->GetID()) != m_mapAllEnterGropPlayer.end())
	{
		m_mapAllEnterGropPlayer.erase(poPlayer->GetID());
	}

	CPlayerGroupMgr::Instance()->RemovePlayerGroup(poPlayerGroup);
	poPlayerGroup = NULL;

	poPlayer->GetLocale().OnEnterCourageBattle(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);

	CreateGroupNum[stCourageData.stSceneData.byTownIdx - 1] = CreateGroupNum[stCourageData.stSceneData.byTownIdx - 1] >= 1 ? CreateGroupNum[stCourageData.stSceneData.byTownIdx - 1] - 1 : 0;

	return ERR_COURAGE_PLAYER_START::ID_SUCCESS;
}

//获取最新的试炼次数
VOID CCourageMgr::GetCourageTimes( CPlayer* poPlayer )
{
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    if (stCourageData.dwTodayHelpTimes == CLuamgr::Instance()->GetTotalHelpCourageTimes() && stCourageData.dwGiveFreeTimes == 0)
    {
        //stCourageData.dwTotalTimes += CLuamgr::Instance()->GetAddCourageTimes();
        stCourageData.dwTodayJoinTimes += CLuamgr::Instance()->GetAddCourageTimes();
		USR_INFO("[%s : %d] AddCourageTime, PlayerID = %u", MSG_MARK, poPlayer->GetID());
        stCourageData.dwGiveFreeTimes = 1;
    }
}

//玩家离开
UINT16 CCourageMgr::OnCouragLeaveGroup( CPlayer* poPlayer, DT_COURAGE_INFO_CLI &stCOURAGE_INFO, DT_COURAGE_GROUP_DATA_CLI_LST &stCOURAGE_GROUP_DATA )
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        //建筑不存在
        return ERR_COURAGE_LEAVE_GROUP::ID_LOCK;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_COURAGE_LEAVE_GROUP::ID_UNOPEN;
    }
    //离开队伍场景
    poPlayer->GetLocale().OnLeaveCourage();
    //是否重置数据
    ResetCourageData(poPlayer);
    //获取最新试炼次数
    GetCourageTimes(poPlayer);
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(stCourageData.dwGroupID);
    if (itr == m_mapGroupID2CourageGroup.end())
    {
        //重新获取所有队伍 排出符合条件的队伍
        SetPlayerGroupValue(poPlayer, stCOURAGE_INFO, stCOURAGE_GROUP_DATA);

        UINT32 dwPlayerID = poPlayer->GetID();
        //添加管理
        if(m_mapAllEnterPlayer.find(dwPlayerID) == m_mapAllEnterPlayer.end())
        {
            m_mapAllEnterPlayer[dwPlayerID] = poPlayer->GetID();
        }
        //清除管理
        if (m_mapAllEnterGropPlayer.find(dwPlayerID) != m_mapAllEnterGropPlayer.end())
        {
            m_mapAllEnterGropPlayer.erase(dwPlayerID);
        }

        //队伍不存在
        return ERR_COURAGE_LEAVE_GROUP::ID_SUCCESS;
    }

    DT_COURAGE_GROUP_DATA &stGroupData = itr->second->GetDT_COURAGE_GROUP_DATA();
    if (stGroupData.wPlayerNum > 0)
    {
        stGroupData.wPlayerNum--;
    }
    UINT8 byGroupIndex = GetInGroupIndex(poPlayer->GetID(),stGroupData.dwGroupID);
    if (byGroupIndex == INVALID_GROUP_INDEX)
    {
        return ERR_COURAGE_LEAVE_GROUP::ID_SUCCESS;
    }
    UINT8 byLeftNum = 0;
    BOOL bDelete = FALSE;
    //队长离开了 则队伍解散
    if (stGroupData.dwLeaderID == poPlayer->GetID())
    {
        //队伍解散
        bDelete = TRUE;
        PKT_CLIGS_COURAGE_GROUP_DISMISS_NTF stNtfNow;
        stNtfNow.byErrCode = ERR_COURAGE_GROUP_DISMISS::ID_SUCCESS;
        vector<UINT32>PlayerID;
        for (UINT8 byIndexPlayer = 0; byIndexPlayer < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexPlayer++)
        {
            CPlayer* pPlayerNow = CPlayerMgr::Instance()->FindPlayer(stGroupData.astPlayerData[byIndexPlayer].dwPlayerID);
            if ((pPlayerNow == NULL) || (pPlayerNow->GetID() == poPlayer->GetID()))
            {
                continue;
            }
            PlayerID.push_back(pPlayerNow->GetID());
        }
        //删除队伍数据
        m_oCourageGroupPool.Free(itr->second);
        m_mapGroupID2CourageGroup.erase(itr);

        for (UINT8 byIndex = 0; byIndex < PlayerID.size();byIndex++)
        {
            CPlayer* pPlayerNew = CPlayerMgr::Instance()->FindPlayer(PlayerID[byIndex]);
            if ((pPlayerNew == NULL)/* || (!pPlayerNew->CkLogin())*/)
            {
                continue;
            }

            UINT32 dwPlayerID = pPlayerNew->GetID();
            //添加管理
            if(m_mapAllEnterPlayer.find(dwPlayerID) == m_mapAllEnterPlayer.end())
            {
                m_mapAllEnterPlayer[dwPlayerID] = dwPlayerID;
            }
            //清除管理
            if (m_mapAllEnterGropPlayer.find(dwPlayerID) != m_mapAllEnterGropPlayer.end())
            {
                m_mapAllEnterGropPlayer.erase(dwPlayerID);
            }

            SetPlayerGroupValue(pPlayerNew, stNtfNow.stPlayerCourageData, stNtfNow.stGroupData);
            pPlayerNew->SendMsg((CHAR*)&stNtfNow, CLIGS_COURAGE_GROUP_DISMISS_NTF);
            //离开队伍场景
            pPlayerNew->GetLocale().OnLeaveCourage();
        }
        CreateGroupNum[stCourageData.stSceneData.byTownIdx - 1] = CreateGroupNum[stCourageData.stSceneData.byTownIdx - 1] >= 1 ? CreateGroupNum[stCourageData.stSceneData.byTownIdx - 1] - 1 : 0;

    }
    else  //给队伍里面别的玩家发送通知
    {
        memset(&stGroupData.astPlayerData[byGroupIndex], 0x00, sizeof(stGroupData.astPlayerData[byGroupIndex]));
        PKT_CLIGS_COURAGE_QUIT_GROUP_NTF stNtf;
        stNtf.wAverageLevel = 0;
        stNtf.dwPlayerID = poPlayer->GetID();
        for(UINT8 byIndexNew = 0; byIndexNew < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexNew++)
        {
            CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer(stGroupData.astPlayerData[byIndexNew].dwPlayerID);
            if ((pPlayer == NULL) || (pPlayer->GetID() == poPlayer->GetID()) /*|| (itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndexNew].byReceiveNotice == 1)*/)
            {
                byLeftNum++;
                continue;
            }
            if(!pPlayer->CkLogin())
            {
                continue;
            }
            //给另外玩家推送通知
            pPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_COURAGE_QUIT_GROUP_NTF);
        }

    }

    //离开之后没有玩家 清除处理
    //if ((byLeftNum == MAX_COURAGE_GROUP_PLAYER_NUM) || (bDelete == TRUE))
    //{
    //    m_oCourageGroupPool.Free(itr->second);
    //    m_mapGroupID2CourageGroup.erase(itr);
    //}
    //位置调出
    poPlayer->GetLocale().OnLeaveCourage();

    UINT32 dwPlayerID = poPlayer->GetID();
    //添加管理
    if(m_mapAllEnterPlayer.find(dwPlayerID) == m_mapAllEnterPlayer.end())
    {
        m_mapAllEnterPlayer[dwPlayerID] = dwPlayerID;
    }
    //清除管理
    if (m_mapAllEnterGropPlayer.find(dwPlayerID) != m_mapAllEnterGropPlayer.end())
    {
        m_mapAllEnterGropPlayer.erase(dwPlayerID);
    }
    //重新获取所有队伍 排出符合条件的队伍
    SetPlayerGroupValue(poPlayer, stCOURAGE_INFO, stCOURAGE_GROUP_DATA);

    //更新队伍信息
    return ERR_COURAGE_LEAVE_GROUP::ID_SUCCESS;

}

//筛选符合条件的队伍
BOOL CCourageMgr::GetRightGroup( CPlayer* poPlayer ,BOOL bReset)
{
    CCourage* poCourage = poPlayer->GetCourage();
    if((NULL == poPlayer) || (NULL == poCourage))
    {
        RETURN_OTHER_ERR;
    }

    poCourage->m_listLevelGroup.clear();
    poCourage->m_vecGroupID.clear();
    poCourage->m_mapGroupIDNum.clear();
    UINT16 wGroupNum = 0;
    UINT16 wLevel = poPlayer->GetLevel();

    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.begin();
    for (itr; itr != m_mapGroupID2CourageGroup.end() && wGroupNum < CCouraPropMgr::Instance()->GetMaxGroupNum(); ++itr)
    {
        //如果为0则是废数据PASS掉
        if (itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum == 0)
        {
            continue;
        }
        //判断等级是否满足 判断是否已经满人
        if ((wLevel < itr->second->GetDT_COURAGE_GROUP_DATA().wMinLevel) || (itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum >= MAX_COURAGE_GROUP_PLAYER_NUM))
        {
            continue;
        }
        //判断关系是否满足
        UINT8 byJoinType = itr->second->GetDT_COURAGE_GROUP_DATA().byJoinType;
		if ( byJoinType == poCourage->GetSelectType() || EJT_ALL == poCourage->GetSelectType())
		{
			switch(byJoinType)
			{
			case EJT_ALL:
				{
					poPlayer->GetCourage()->m_listLevelGroup.push_back(itr->second); 
					poPlayer->GetCourage()->m_vecGroupID.push_back(itr->second->GetDT_COURAGE_GROUP_DATA().dwGroupID);
					break;
				}
			case EJT_FACTION:
				{

					CPlayer* pLeadPlayer = CPlayerMgr::Instance()->FindPlayer(itr->second->GetDT_COURAGE_GROUP_DATA().dwLeaderID);
					UINT32 wLeadFactionID = 0;
					if (pLeadPlayer != NULL)
					{
						wLeadFactionID = pLeadPlayer->GetFactionID(itr->second->GetDT_COURAGE_GROUP_DATA().dwLeaderID);
					}
					UINT32 wFactionID = poPlayer->GetFactionID(poPlayer->GetID());
					if (wLeadFactionID == wFactionID && wLeadFactionID != 0)
					{
						poPlayer->GetCourage()->m_listLevelGroup.push_back(itr->second);
						poPlayer->GetCourage()->m_vecGroupID.push_back(itr->second->GetDT_COURAGE_GROUP_DATA().dwGroupID);
					}
					
					break;
				}
			case  EJT_FRIEND:
				{

					UINT8 byTpye = EFSRT_OTHER;

					CFriends *poFriends = poPlayer->GetFriends();
					if (NULL == poFriends)
					{
						break;
					}

					poFriends->GetRelationType(itr->second->GetDT_COURAGE_GROUP_DATA().dwLeaderID, byTpye);
					UINT8 byLeaderType = EFSRT_OTHER;
					CPlayer* poLeader = CPlayerMgr::Instance()->FindPlayer(itr->second->GetDT_COURAGE_GROUP_DATA().dwLeaderID);
					if ( NULL == poLeader)
					{
						break;
					}
					CFriends* poLeaderFriends = poLeader->GetFriends();
					if ( NULL == poLeaderFriends)
					{
						break;
					}
					poLeaderFriends->GetRelationType(poPlayer->GetID(), byLeaderType);
					if (byTpye == EFSRT_FRIEND && EFSRT_FRIEND == byLeaderType)
					{
						poPlayer->GetCourage()->m_listLevelGroup.push_back(itr->second);
						poPlayer->GetCourage()->m_vecGroupID.push_back(itr->second->GetDT_COURAGE_GROUP_DATA().dwGroupID);
					}
					
					break;
				}
			default:
				break;
			}
		}
        
        wGroupNum = poPlayer->GetCourage()->m_vecGroupID.size();
    }
    if (poPlayer->GetCourage()->m_listLevelGroup.size() == 0)
    {
        //没有合适的队伍
        return FALSE;
    }
    //进行排序
    poCourage->m_listLevelGroup.sort(SortByLevel);
    if (bReset == TRUE)
    {
        CListLevelGroupItr itrNew = poPlayer->GetCourage()->m_listLevelGroup.begin();
        //排序后赋值
        for ( itrNew; itrNew != poPlayer->GetCourage()->m_listLevelGroup.end() ; itrNew++)
        {
            CCourageGroup *poCourageGroup = *itrNew;
            if (poCourageGroup == NULL)
            {
                continue;
            }
            poPlayer->GetCourage()->m_mapGroupIDNum[poCourageGroup->GetDT_COURAGE_GROUP_DATA().dwGroupID] = poCourageGroup->GetDT_COURAGE_GROUP_DATA().wPlayerNum;
        }
    }
    
    return TRUE;
}

//打开购买试炼
UINT16 CCourageMgr::OnCourageOpenShop( CPlayer* poPlayer , UINT8 &byBuyTimes, UINT32 &dwGold)
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        //建筑不存在
        return ERR_OPEN_COURAGE_SHOP::ID_LOCK;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_OPEN_COURAGE_SHOP::ID_UNOPEN;
    }
    //是否重置数据
    ResetCourageData(poPlayer);
    //获取最新试炼次数
    GetCourageTimes(poPlayer);
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    UINT16 wMaxTimes = CVipPropMgr::Instance()->GetIncNum(EVINF_COURAGE, poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel);//CLuamgr::Instance()->GetVipBuyCourageTimes(poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel);
    //剩余购买次数
    byBuyTimes = wMaxTimes >= stCourageData.dwBuyTimes ? wMaxTimes - stCourageData.dwBuyTimes : 0;
    dwGold = CLuamgr::Instance()->GetFuncCost(poPlayer, EGCF_BUY_ENCOURAGE_TIMES, 0, stCourageData.dwBuyTimes);
    return ERR_OPEN_COURAGE_SHOP::ID_SUCCESS;
}

//获取协助提示信息
VOID CCourageMgr::GetHelpPrompt(CPlayer* poPlayer, TCHAR aszHelpCouragePrompt[HELP_COURAGE_LEN] )
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        return;
    }
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    //免费试炼次数用完
    if (stCourageData.dwTodayJoinTimes <= 0 && stCourageData.dwGiveFreeTimes == 0)
    {
        UINT32 wAllTimes = CLuamgr::Instance()->GetTotalHelpCourageTimes();
        UINT32 wLeftTimes = wAllTimes <= stCourageData.dwTodayHelpTimes ? 0 : wAllTimes - stCourageData.dwTodayHelpTimes;
        UINT32 wAddTimes = CLuamgr::Instance()->GetAddCourageTimes();
        vector<string> vecFillMsg;
        CHAR szBuff[11] = {0};
        sprintf(szBuff, "%u", wLeftTimes);
        vecFillMsg.push_back(szBuff);
        sprintf(szBuff, "%u", wAddTimes);
        vecFillMsg.push_back(szBuff);
        memset(aszHelpCouragePrompt, 0, HELP_COURAGE_LEN);
        SGDP::SDStrncpy(aszHelpCouragePrompt, CMsgDefMgr::Instance()->GetErrMsg("CG_COURAGE_PROMPT", &vecFillMsg).c_str(), HELP_COURAGE_LEN - 1);
    }
    //没用完的时候
    //else if (stCourageData.dwTodayJoinTimes > 0 && stCourageData.dwGiveFreeTimes == 0)
    //{
    //    UINT32 wAddTimes = CLuamgr::Instance()->GetAddCourageTimes();
    //    vector<string> vecFillMsg;
    //    CHAR szBuff[11] = {0};
    //    sprintf(szBuff, "%u", wAddTimes);
    //    vecFillMsg.push_back(szBuff);
    //    memset(aszHelpCouragePrompt, 0, HELP_COURAGE_LEN);
    //    SGDP::SDStrncpy(aszHelpCouragePrompt, CMsgDefMgr::Instance()->GetErrMsg("CG_COURAGE_FIRST_PROMT", &vecFillMsg).c_str(), HELP_COURAGE_LEN - 1);
    //}
    if (stCourageData.dwTodayJoinTimes == 0 && stCourageData.dwGiveFreeTimes == 1)
    {
        memset(aszHelpCouragePrompt, 0, HELP_COURAGE_LEN);
        SGDP::SDStrncpy(aszHelpCouragePrompt, CMsgDefMgr::Instance()->GetErrMsg("CG_COURAGE_NONE_PROMPT").c_str(), HELP_COURAGE_LEN - 1);
    }
}


VOID CCourageMgr::SaveData()
{
    static DT_COURAGE_GROUP_DATA_LST stDT_COURAGE_GROUP_DATA_LST;
    memset(&stDT_COURAGE_GROUP_DATA_LST, 0x00, sizeof(DT_COURAGE_GROUP_DATA_LST));
    stDT_COURAGE_GROUP_DATA_LST.qwOverTime		= m_qwOverTime;
    stDT_COURAGE_GROUP_DATA_LST.qwStartTime		= m_qwStartTime;
    stDT_COURAGE_GROUP_DATA_LST.qwSaveTime		= m_qwSaveTime;
    stDT_COURAGE_GROUP_DATA_LST.byActivityID		= m_m_byActivityID;
    for(CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.begin(); itr != m_mapGroupID2CourageGroup.end(); itr++)
    {
        DT_COURAGE_GROUP_DATA &stDT_COURAGE_GROUP_DATA = stDT_COURAGE_GROUP_DATA_LST.astGroupData[stDT_COURAGE_GROUP_DATA_LST.wGroupNum++];
        memcpy(&stDT_COURAGE_GROUP_DATA, &itr->second->GetDT_COURAGE_GROUP_DATA(), sizeof(DT_COURAGE_GROUP_DATA));
    }

    //保存数据
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVE_COURAGE_DATA);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return;
    }
    poCMD->SetUserData((VOID*)(&stDT_COURAGE_GROUP_DATA_LST), 0);
    gsapi::GetDBMgr()->AddCommand(poCMD);
}
CCourage* CCourageMgr::CreateCourage()
{
    return m_oCourageShmemObjMgr.Alloc();
}

VOID CCourageMgr::RemoveCourage(CCourage *poCourage)
{
    if(NULL == poCourage)
    {
        return;
    }

    m_oCourageShmemObjMgr.Free(poCourage);
}

CCourageGroup *CCourageMgr::GetCourageGroup(CPlayer* poPlayer)
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        return NULL;
    }
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    //判断是否已经在队伍中
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(stCourageData.dwGroupID);
    if(itr != m_mapGroupID2CourageGroup.end())
    {
        return itr->second;
    }
    else
    {
        return NULL;
    }
}

//设置队伍数据
VOID CCourageMgr::SetPlayerGroupValue( CPlayer* poPlayer, DT_COURAGE_INFO_CLI &stCOURAGE_INFO, DT_COURAGE_GROUP_DATA_CLI_LST &stCOURAGE_GROUP_DATA )
{
    if (poPlayer == NULL)
    {
        return;
    }
    //是否重置数据
    ResetCourageData(poPlayer);
    //获取最新试炼次数
    GetCourageTimes(poPlayer);
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    stCourageData.dwGroupID = 0;
    //stCourageData.qwLastTime = SDTimeSecs();
    stCourageData.wGroupIndex = 0;

    stCOURAGE_INFO.dwMaxCourageTimes = stCourageData.dwTotalTimes;
    stCOURAGE_INFO.dwLeftCourageTimes = stCourageData.dwTodayJoinTimes;
    stCOURAGE_INFO.dwHelpCourageTimes = stCourageData.dwTodayHelpTimes;
    //"在协助n次，挑战次数+m"
    GetHelpPrompt(poPlayer, stCOURAGE_INFO.aszHelpCouragePrompt);
    //免费试炼次数用完
    if (stCourageData.dwTodayJoinTimes <= 0)
    {
        stCOURAGE_INFO.byBattleType = EBT_HELP;
    }
    //重新获取所有队伍 排出符合条件的队伍
    BOOL bHaveGroup = GetRightGroup(poPlayer);
    if (bHaveGroup == FALSE)
    {
        //没有合适的队伍
        return;
    }
    CListLevelGroupItr itrNew = poPlayer->GetCourage()->m_listLevelGroup.begin();
    //排序后赋值
    for ( itrNew; itrNew != poPlayer->GetCourage()->m_listLevelGroup.end(); itrNew++)
    {
        CCourageGroup *poCourageGroup = *itrNew;
        if ((poCourageGroup == NULL) || (poCourageGroup->GetDT_COURAGE_GROUP_DATA().wPlayerNum == 0))
        {
            continue;
        }
        DT_COURAGE_GROUP_BASE_DATA_CLI &stCoruageGroupBaseData = stCOURAGE_GROUP_DATA.astTeamInfo[stCOURAGE_GROUP_DATA.wTeamNum++];
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
    }
}

//判断是否在队伍中
BOOL CCourageMgr::HaveInGroup( CPlayer* poPlayer, DT_COURAGE_GROUP_DATA &stCOURAGE_GROUP_DATA , UINT32 dwGroupID)
{
    //判断是否已经在队伍中
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(dwGroupID);
    if(itr == m_mapGroupID2CourageGroup.end())
    {
        return FALSE;
    }
    for (UINT8 byIndex = 0; byIndex < MAX_COURAGE_GROUP_PLAYER_NUM; byIndex++)
    {
        if (itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndex].dwPlayerID != poPlayer->GetID())
        {
            continue;
        }
        stCOURAGE_GROUP_DATA.byJoinType = itr->second->GetDT_COURAGE_GROUP_DATA().byJoinType;
        stCOURAGE_GROUP_DATA.dwGroupID = itr->second->GetDT_COURAGE_GROUP_DATA().dwGroupID;
        stCOURAGE_GROUP_DATA.wMinLevel = itr->second->GetDT_COURAGE_GROUP_DATA().wMinLevel;
        stCOURAGE_GROUP_DATA.wPlayerMaxNum = MAX_COURAGE_GROUP_PLAYER_NUM;


        memcpy(&stCOURAGE_GROUP_DATA.astPlayerData, &itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData, sizeof(stCOURAGE_GROUP_DATA.astPlayerData));
        stCOURAGE_GROUP_DATA.wPlayerNum = itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum;
        if (itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum == 0)
        {
            stCOURAGE_GROUP_DATA.wAverageLevel = 0;
        }
        else
        {
            stCOURAGE_GROUP_DATA.wAverageLevel = (itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[0].wLevel + itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[1].wLevel
                                                  + itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[2].wLevel) / itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum;
        }
        return TRUE;

    }
    return FALSE;
}

//玩家刷新队伍
UINT16 CCourageMgr::OnCourageRefreshGroup( CPlayer* poPlayer, DT_COURAGE_GROUP_DATA_CLI_LST &stCOURAGE_GROUP_DATA )
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        return ERR_COURAGE_GROUP_REFRESH::ID_UNOPEN;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_COURAGE_GROUP_REFRESH::ID_UNOPEN;
    }
    //清除数据
    ClearGroupMemberData(poPlayer);
    UINT32 dwPlayerID = poPlayer->GetID();
    //添加管理
    if(m_mapAllEnterPlayer.find(dwPlayerID) == m_mapAllEnterPlayer.end())
    {
        m_mapAllEnterPlayer[dwPlayerID] = dwPlayerID;
    }
    //重新获取所有队伍 排出符合条件的队伍
    BOOL bHaveGroup = GetRightGroup(poPlayer);
    if (bHaveGroup == FALSE)
    {
        //没有合适的队伍
        return ERR_COURAGE_GROUP_REFRESH::ID_SUCCESS;
    }
    CListLevelGroupItr itrNew = poPlayer->GetCourage()->m_listLevelGroup.begin();
    //排序后赋值
    for ( itrNew; itrNew != poPlayer->GetCourage()->m_listLevelGroup.end(); itrNew++)
    {
        CCourageGroup *poCourageGroup = *itrNew;
        if (poCourageGroup == NULL)
        {
            continue;
        }
        DT_COURAGE_GROUP_BASE_DATA_CLI &stCoruageGroupBaseData = stCOURAGE_GROUP_DATA.astTeamInfo[stCOURAGE_GROUP_DATA.wTeamNum++];
        stCoruageGroupBaseData.byJoinType = poCourageGroup->GetDT_COURAGE_GROUP_DATA().byJoinType;
        stCoruageGroupBaseData.dwGroupID = poCourageGroup->GetDT_COURAGE_GROUP_DATA().dwGroupID;
        stCoruageGroupBaseData.wPlayerNum = poCourageGroup->GetDT_COURAGE_GROUP_DATA().wPlayerNum;
        stCoruageGroupBaseData.wPlayerMaxNum = MAX_COURAGE_GROUP_PLAYER_NUM;
        stCoruageGroupBaseData.wMinLevel = poCourageGroup->GetDT_COURAGE_GROUP_DATA().wMinLevel;

        const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(poCourageGroup->GetDT_COURAGE_GROUP_DATA().dwLeaderID);
        if (poPlayerBaseData != NULL)
        {
            SDStrcpy(stCoruageGroupBaseData.stLeaderData.aszDispPlayerName, poPlayerBaseData->aszDispName);
            stCoruageGroupBaseData.stLeaderData.wLevel = poPlayerBaseData->wLevel;
            stCoruageGroupBaseData.stLeaderData.wKindID = poPlayerBaseData->wMainHeroKindID;
        }
    }
    //更新队伍信息
    return ERR_COURAGE_GROUP_REFRESH::ID_SUCCESS;
}

//玩家进入队伍界面
UINT16 CCourageMgr::OnCourageEnterGroup( CPlayer* poPlayer, DT_COURAGE_GROUP_DATA &stCOURAGE_GROUP_DATA )
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        return ERR_COURAGE_ENTER_GROUP::ID_LOCK;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        poPlayer->GetLocale().OnLeaveCourage();
        return ERR_COURAGE_ENTER_GROUP::ID_UNOPEN;
    }
    if(HaveBringHero(poPlayer) == FALSE)
    {
         poPlayer->GetLocale().OnLeaveCourage();
        return ERR_COURAGE_ENTER_GROUP::ID_NO_HERO;
    }
    //是否重置数据
    ResetCourageData(poPlayer);
    //获取最新试炼次数
    GetCourageTimes(poPlayer);
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    //验证是否在队伍中
    BOOL bHaveInGroup = HaveInGroup(poPlayer, stCOURAGE_GROUP_DATA, stCourageData.dwGroupID);

    if (bHaveInGroup == FALSE)
    {
        return ERR_COURAGE_ENTER_GROUP::ID_NO_GROUP;
    }

    UINT32 dwPlayerID = poPlayer->GetID();
    //添加进入队伍管理
    if(m_mapAllEnterGropPlayer.find(dwPlayerID) == m_mapAllEnterGropPlayer.end())
    {
        m_mapAllEnterGropPlayer[dwPlayerID] = dwPlayerID;
    }
    //清除管理
    if (m_mapAllEnterPlayer.find(dwPlayerID) != m_mapAllEnterPlayer.end())
    {
        m_mapAllEnterPlayer.erase(dwPlayerID);
    }

    poPlayer->GetLocale().OnEnterCourage();
    //返回成功
    return ERR_COURAGE_ENTER_GROUP::ID_SUCCESS;

}

//下线了或者离开了活动
UINT16 CCourageMgr::OnLeave( CPlayer* poPlayer, BOOL bDisconnect /*= FALSE*/ )
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        return ERR_LEAVE_COURAGE::ID_SUCCESS;
    }

    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        return ERR_LEAVE_COURAGE::ID_LOCK;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_LEAVE_COURAGE::ID_UNOPEN;
    }
    //C3232MapItr itrPlayID = m_mapAllEnterPlayer.find(poPlayer->GetID());
    //if (itrPlayID == m_mapAllEnterPlayer.end())
    //{
    //    return ERR_LEAVE_COURAGE::ID_SUCCESS;
    //}
    ////赋值离开时间
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    //stCourageData.qwLastTime = SDTimeSecs();

    //管理清除
    //m_mapAllEnterPlayer.erase(itrPlayID);
    //if(!bDisconnect)
    //{
    //    poPlayer->GetLocale().OnLeaveCourage();
    //}

    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(stCourageData.dwGroupID);
    if (itr == m_mapGroupID2CourageGroup.end())
    {
        stCourageData.dwGroupID = 0;
        return ERR_LEAVE_COURAGE::ID_SUCCESS;
    } 
    if (itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum > 0)
    {
        itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum--;
    }

    if (itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum <= 0)
    {
        stCourageData.dwGroupID = 0;
        m_oCourageGroupPool.Free(itr->second);
        m_mapGroupID2CourageGroup.erase(itr);
        return ERR_LEAVE_COURAGE::ID_SUCCESS;
    }
 
    UINT8 byLeftNum = 0;
    BOOL bDelete = FALSE;
    //队长离开了 则队伍解散
    if (itr->second->GetDT_COURAGE_GROUP_DATA().dwLeaderID == poPlayer->GetID())
    {
        //队伍解散
        bDelete = TRUE;
        PKT_CLIGS_COURAGE_GROUP_DISMISS_NTF stNtfNow;
        stNtfNow.byErrCode = ERR_COURAGE_GROUP_DISMISS::ID_SUCCESS;
        for (UINT8 byIndexPlayer = 0; byIndexPlayer < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexPlayer++)
        {
            CPlayer* pPlayerNow = CPlayerMgr::Instance()->FindPlayer(itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndexPlayer].dwPlayerID);
            if ((pPlayerNow == NULL) || (pPlayerNow->GetID() == poPlayer->GetID()))
            {
                continue;
            }
            SetPlayerGroupValue(pPlayerNow, stNtfNow.stPlayerCourageData, stNtfNow.stGroupData);
            pPlayerNow->SendMsg((CHAR*)&stNtfNow, CLIGS_COURAGE_GROUP_DISMISS_NTF);
        }

    }
    else  //给队伍里面别的玩家发送通知
    {
        memset(&itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[stCourageData.wGroupIndex], 0x00, sizeof(itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[stCourageData.wGroupIndex]));
        PKT_CLIGS_COURAGE_QUIT_GROUP_NTF stNtf;
        UINT32 wAverageLevel = 0;
        if (itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum == 0)
        {
            wAverageLevel = 0;
        }
        else
        {
            wAverageLevel = (itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[0].wLevel + itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[1].wLevel +
                itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[2].wLevel) / itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum;
        }
        stNtf.wAverageLevel = wAverageLevel;
        stNtf.dwPlayerID = poPlayer->GetID();
        for(UINT8 byIndexNew = 0; byIndexNew < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexNew++)
        {
            CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer(itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndexNew].dwPlayerID);
            if ((pPlayer == NULL) || (pPlayer->GetID() == poPlayer->GetID()))
            {
                byLeftNum++;
                continue;
            }
            if(!pPlayer->CkLogin())
            {
                continue;
            }
            //给另外玩家推送通知
            pPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_COURAGE_QUIT_GROUP_NTF);
        }
    }
    //离开之后没有玩家 清除处理
    if ((byLeftNum == MAX_COURAGE_GROUP_PLAYER_NUM) || (bDelete == TRUE))
    {
        m_oCourageGroupPool.Free(itr->second);
        m_mapGroupID2CourageGroup.erase(itr);
    }
    
    stCourageData.dwGroupID = 0;
    return ERR_LEAVE_COURAGE::ID_SUCCESS;    
}

//超时解散队伍
VOID CCourageMgr::OverTimeDismiss( UINT32 dwPlayerID )
{ 

    //玩家数据还在内存中
    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        return;
    }

    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        return;
    }
    //将原来定时器删除
    //CTimerEventMgr::Instance()->DelCourageFireEvent(poPlayer->GetID());  
    //是否到活动时间
    if (m_bActivityOver)
    {
        return;
    }
    //赋值离开时间
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    //stCourageData.qwLastTime = SDTimeSecs();


    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(stCourageData.dwGroupID);
    if (itr == m_mapGroupID2CourageGroup.end())
    {
        stCourageData.dwGroupID = 0;
        return;
    } 

    if (itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum > 0)
    {
        itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum--;
    }

    if (itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum <= 0)
    {
        stCourageData.dwGroupID = 0;
        m_oCourageGroupPool.Free(itr->second);
        m_mapGroupID2CourageGroup.erase(itr);
        //给自己推送解散通知
        PKT_CLIGS_COURAGE_GROUP_OVERTIME_NTF StNtf;
        StNtf.byExt = 0;
        poPlayer->SendMsg((const CHAR*)(&StNtf), CLIGS_COURAGE_GROUP_OVERTIME_NTF);

        //添加管理
        if(m_mapAllEnterPlayer.find(dwPlayerID) == m_mapAllEnterPlayer.end())
        {
            m_mapAllEnterPlayer[dwPlayerID] = dwPlayerID;
        }
        //清除管理
        if (m_mapAllEnterGropPlayer.find(dwPlayerID) != m_mapAllEnterGropPlayer.end())
        {
            m_mapAllEnterGropPlayer.erase(dwPlayerID);
        }

        return;
    }

    UINT8 byLeftNum = 0;
    BOOL bDelete = FALSE;
    //队长离开了 则队伍解散
    if (itr->second->GetDT_COURAGE_GROUP_DATA().dwLeaderID == poPlayer->GetID())
    {
        //队伍解散
        bDelete = TRUE;
        PKT_CLIGS_COURAGE_GROUP_DISMISS_NTF stNtfNow;
        stNtfNow.byErrCode = ERR_COURAGE_GROUP_DISMISS::ID_SUCCESS;
        for (UINT8 byIndexPlayer = 0; byIndexPlayer < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexPlayer++)
        {
            CPlayer* pPlayerNow = CPlayerMgr::Instance()->FindPlayer(itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndexPlayer].dwPlayerID);
            if ((pPlayerNow == NULL) || (pPlayerNow->GetID() == poPlayer->GetID()))
            {
                continue;
            }
            UINT32 dwPlayerIDNow = pPlayerNow->GetID();
            //添加管理
            if(m_mapAllEnterPlayer.find(dwPlayerIDNow) == m_mapAllEnterPlayer.end())
            {
                m_mapAllEnterPlayer[dwPlayerIDNow] = dwPlayerIDNow;
            }
            //清除管理
            if (m_mapAllEnterGropPlayer.find(dwPlayerIDNow) != m_mapAllEnterGropPlayer.end())
            {
                m_mapAllEnterGropPlayer.erase(dwPlayerIDNow);
            }
            SetPlayerGroupValue(pPlayerNow, stNtfNow.stPlayerCourageData, stNtfNow.stGroupData);
            pPlayerNow->SendMsg((CHAR*)&stNtfNow, CLIGS_COURAGE_GROUP_DISMISS_NTF);
        }

    }
    else  //给队伍里面别的玩家发送通知
    {
        memset(&itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[stCourageData.wGroupIndex], 0x00, sizeof(itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[stCourageData.wGroupIndex]));
        PKT_CLIGS_COURAGE_QUIT_GROUP_NTF stNtf;
        UINT32 wAverageLevel = 0;
        if (itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum == 0)
        {
            wAverageLevel = 0;
        }
        else
        {
            wAverageLevel = (itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[0].wLevel + itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[1].wLevel +
                itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[2].wLevel) / itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum;
        }
        stNtf.wAverageLevel = wAverageLevel;
        stNtf.dwPlayerID = poPlayer->GetID();
        for(UINT8 byIndexNew = 0; byIndexNew < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexNew++)
        {
            CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer(itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndexNew].dwPlayerID);
            if ((pPlayer == NULL) || (pPlayer->GetID() == poPlayer->GetID()))
            {
                byLeftNum++;
                continue;
            }
            if(!pPlayer->CkLogin())
            {
                continue;
            }
            //给另外玩家推送通知
            pPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_COURAGE_QUIT_GROUP_NTF);
        }
    }
    //离开之后没有玩家 清除处理
    if ((byLeftNum == MAX_COURAGE_GROUP_PLAYER_NUM) || (bDelete == TRUE))
    {
        m_oCourageGroupPool.Free(itr->second);
        m_mapGroupID2CourageGroup.erase(itr);
    }

    //给自己推送解散通知
    PKT_CLIGS_COURAGE_GROUP_OVERTIME_NTF StNtf;
    StNtf.byExt = 0;
    poPlayer->SendMsg((const CHAR*)(&StNtf), CLIGS_COURAGE_GROUP_OVERTIME_NTF);
    stCourageData.dwGroupID = 0;

    //添加管理
    if(m_mapAllEnterPlayer.find(dwPlayerID) == m_mapAllEnterPlayer.end())
    {
        m_mapAllEnterPlayer[dwPlayerID] = dwPlayerID;
    }
    //清除管理
    if (m_mapAllEnterGropPlayer.find(dwPlayerID) != m_mapAllEnterGropPlayer.end())
    {
        m_mapAllEnterGropPlayer.erase(dwPlayerID);
    }
}

 //确认携带了一个伙伴
BOOL CCourageMgr::HaveBringHero( CPlayer* poPlayer )
{
    if (poPlayer == NULL)
    {
        return FALSE;
    }
    CFormationHeroShmemMap& mapFormationHero = poPlayer->GetFormationHeroMap();
    CHero* poHero = mapFormationHero.GetFistData();
    UINT8 byHeroNum = 0;
    while(poHero)
    {   
        byHeroNum++;
        poHero = mapFormationHero.GetNextData();
    }
    //有一个是主角 玩家自己
    if (byHeroNum <= 1)
    {
       return FALSE;
    }
    return TRUE;
}

//刷新队伍
VOID CCourageMgr::RefreshGroupNtf()
{
    //给所有在线的还在活动面板玩家发送更新信息
    for(C3232MapItr itr = m_mapAllEnterPlayer.begin(); itr != m_mapAllEnterPlayer.end(); itr++)
    {
        UINT32 dwID = itr->second;
        CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwID);
        if((NULL == poPlayer) || (!poPlayer->CkLogin()))
        {
            continue;
        }
        CCourage* poCourage = poPlayer->GetCourage();;
        if(NULL == poCourage)
        {
            continue;
        }
        DT_COURAGE_DATA &stCourageData = poCourage->GetDT_COURAGE_DATA();
        if (stCourageData.dwGroupID != 0 /*|| stCourageData.byReceiveNotice == 1*/)
        {
            continue;
        }

        //重新获取所有队伍 排出符合条件的队伍
        BOOL bHaveGroup = GetRightGroup(poPlayer,TRUE);
        //比较数据是否有变化
        UINT16 bSameData = CompareMapGroup(poPlayer->GetCourage()->m_mapBackGroupID, poPlayer->GetCourage()->m_mapGroupIDNum);
        if (bHaveGroup == FALSE && bSameData == 1)
        {
            continue;
        }

        PKT_CLIGS_COURAGE_GROUP_REFRESH_NTF sNtf;
        memset(&sNtf,0x00,sizeof(sNtf));

        CListLevelGroupItr itrNew = poPlayer->GetCourage()->m_listLevelGroup.begin();
        poPlayer->GetCourage()->m_mapBackGroupID.clear();
        //排序后赋值
        for ( itrNew; itrNew != poPlayer->GetCourage()->m_listLevelGroup.end() && sNtf.stGroupData.wTeamNum < CCouraPropMgr::Instance()->GetMaxGroupNum(); itrNew++)
        {
            CCourageGroup *poCourageGroup = *itrNew;
            if ((poCourageGroup == NULL) || (poCourageGroup->GetDT_COURAGE_GROUP_DATA().wPlayerNum == 0))
            {
                continue;
            }
            DT_COURAGE_GROUP_BASE_DATA_CLI &stCoruageGroupBaseData = sNtf.stGroupData.astTeamInfo[sNtf.stGroupData.wTeamNum++];
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
            poPlayer->GetCourage()->m_mapBackGroupID[poCourageGroup->GetDT_COURAGE_GROUP_DATA().dwGroupID] = poCourageGroup->GetDT_COURAGE_GROUP_DATA().wPlayerNum;
        }
        poPlayer->SendMsg((CHAR*)&sNtf,CLIGS_COURAGE_GROUP_REFRESH_NTF);        
    }
}

//创建测试数据 
VOID CCourageMgr::CreateTestData()
{
    static bool a = TRUE;
    if(a == FALSE) return;
	UINT8 abyLevel[30] = {120, 120, 110, 110,100,100,90,90,80,80,70,70,60,60,50,50};
	UINT8 byLevel = 0;
    UINT32 PLAYID[30]={0};
	const CPlayerID2PlayerBaseDataMap& mapPlayer = CPlayerBaseDataCacheMgr::Instance()->GetPlayerID2PlayerBaseDataMap();
	UINT8 byPlayerNUm = 0;
	for ( CPlayerID2PlayerBaseDataMap::const_iterator itr = mapPlayer.begin(); itr != mapPlayer.end() && byPlayerNUm < 30; itr++)
	{
		if( itr->second->wLevel > 50 )
		{
			PLAYID[byPlayerNUm++] = itr->first;
		}
	}
	UINT8 byJoinType = EJT_ALL;
    for (UINT8 byindex =0;byindex<18&&a==TRUE;byindex++)
    {  
		if ( byJoinType > 2)
		{
			byJoinType = 0;
		}
		byLevel = abyLevel[byindex];
		if ( 0 == byLevel)
		{
			byLevel = Random(50, 90);
			byLevel = (byLevel / 10) * 10;
		}
		
        const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(PLAYID[byindex]);
        if( NULL == poPlayerBaseData )
        {
            continue;
        }
        dwCreateGroupID = dwCreateGroupID > MAX_GROUP_ID ? 1 : dwCreateGroupID;
        dwCreateGroupID = dwCreateGroupID > m_dwGroupID ? dwCreateGroupID : m_dwGroupID;
        
        DT_COURAGE_GROUP_DATA  stDT_COURAGE_GROUP_DATA;
        memset(&stDT_COURAGE_GROUP_DATA, 0x00, sizeof(stDT_COURAGE_GROUP_DATA));
        CCourageGroup *poCourageGroup = m_oCourageGroupPool.Alloc();
        if (NULL != poCourageGroup)
        {
            stDT_COURAGE_GROUP_DATA.byJoinType = byJoinType++;
            stDT_COURAGE_GROUP_DATA.dwGroupID = dwCreateGroupID++;
            stDT_COURAGE_GROUP_DATA.dwLeaderID = PLAYID[byindex];
            stDT_COURAGE_GROUP_DATA.wMinLevel = byLevel;
            stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].byBattleType = EBT_JOIN;
            stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].dwPlayerID = PLAYID[byindex];
            stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].byMemberType = EBI_LEADER;
            stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].byReadyType = EBR_READYING;
            stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].wLevel = poPlayerBaseData->wLevel;
            stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].wKindID = poPlayerBaseData->wMainHeroKindID;
            TCHAR      aszDispPlayerName[USERNAME_LEN]="";
            SDStrcpy(stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].aszDispPlayerName, poPlayerBaseData->aszDispName);
            stDT_COURAGE_GROUP_DATA.wPlayerNum++;

            BYTE byRand = Random(0,1);
            if (byRand == 1)
            {
                BYTE byTemp = (byindex+1) >= 18 ? 0 : (byindex+1);
                const SPlayerBaseData* poRPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(PLAYID[byTemp]);
                if( NULL == poRPlayerBaseData )
                {
                    continue;
                }

                stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].byBattleType = EBT_JOIN;
                stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].dwPlayerID = PLAYID[byTemp];
                stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].byMemberType = EBI_MEMBER;
                stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].byReadyType = EBR_READYING;
                stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].wLevel = poRPlayerBaseData->wLevel;
                stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].wKindID = poRPlayerBaseData->wMainHeroKindID;

                TCHAR      aszDispPlayerName[USERNAME_LEN]="";
                SDStrcpy(stDT_COURAGE_GROUP_DATA.astPlayerData[stDT_COURAGE_GROUP_DATA.wPlayerNum].aszDispPlayerName, poRPlayerBaseData->aszDispName);
                stDT_COURAGE_GROUP_DATA.wPlayerNum++;
            }

            poCourageGroup->Init(stDT_COURAGE_GROUP_DATA);
            m_mapGroupID2CourageGroup[stDT_COURAGE_GROUP_DATA.dwGroupID] = poCourageGroup;
        }
    }
    a = FALSE;


}

//通过战斗
VOID CCourageMgr::PassBattle(UINT32 dwGroupID,UINT16 wLevel, BOOL bPass)
{
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(dwGroupID);
    if (itr == m_mapGroupID2CourageGroup.end())
    {
        //失败
        return;
    }

    //扣除一次勇气试炼  放在领取奖励那边扣除了
    for (UINT8 byIndex = 0; byIndex < MAX_COURAGE_GROUP_PLAYER_NUM; byIndex++)
    {
        UINT32 dwPlayerID = itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndex].dwPlayerID;
        CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
        if ((poPlayer == NULL) /*|| (!poPlayer->CkLogin()) */|| (poPlayer->GetCourage() == NULL))
        {
            continue;
        }
        CCourage *poCourage = poPlayer->GetCourage();
        if (poCourage == NULL)
        {
            continue;
        }
        DT_COURAGE_DATA &stCourageData = poCourage->GetDT_COURAGE_DATA();

        //清除队伍ID
        stCourageData.dwGroupID = 0; 
        stCourageData.wAverageLevel = wLevel;
        stCourageData.byHavePass = bPass;
    }
    //清除对应数据
    m_oCourageGroupPool.Free(itr->second);
    m_mapGroupID2CourageGroup.erase(itr);

}

//清除队伍里面玩家数据
VOID CCourageMgr::ClearGroupMemberData( CPlayer* poPlayer )
{
    if (poPlayer == NULL)
    {
        return;
    }
    //遍历一次容器 查找出含有玩家信息 服务端崩溃存在没保存玩家位置的可能，防止玩家在一个队伍里面存在两个数据
    vector<UINT32>vec_GroupID;
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.begin();
    for (;itr != m_mapGroupID2CourageGroup.end(); ++itr)
    {
        DT_COURAGE_GROUP_DATA &stCourageGroup = itr->second->GetDT_COURAGE_GROUP_DATA();
        if (stCourageGroup.wPlayerNum == 0)
        {
            continue;
        }
        UINT8 byInGroupIndex = GetInGroupIndex(poPlayer->GetID(),stCourageGroup.dwGroupID);
        if (byInGroupIndex == INVALID_GROUP_INDEX)
        {
            continue;
        }
        //清除玩家在队伍里面的数据
        memset(&stCourageGroup.astPlayerData[byInGroupIndex], 0x00, sizeof(stCourageGroup.astPlayerData[byInGroupIndex]));
        //人数减1
        stCourageGroup.wPlayerNum--;
        UINT8 byLeftNum = 0;
        //队长离开了 则队伍解散
        if (stCourageGroup.dwLeaderID == poPlayer->GetID())
        {
            PKT_CLIGS_COURAGE_GROUP_DISMISS_NTF stNtfNow;
            stNtfNow.byErrCode = ERR_COURAGE_GROUP_DISMISS::ID_SUCCESS;
            for (UINT8 byIndexPlayer = 0; byIndexPlayer < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexPlayer++)
            {
                CPlayer* pPlayerNow = CPlayerMgr::Instance()->FindPlayer(stCourageGroup.astPlayerData[byIndexPlayer].dwPlayerID);
                if ((pPlayerNow == NULL) || (pPlayerNow->GetID() == poPlayer->GetID()) || (pPlayerNow->GetCourage() == NULL))
                {
                    continue;
                }
                SetPlayerGroupValue(pPlayerNow, stNtfNow.stPlayerCourageData, stNtfNow.stGroupData);
                pPlayerNow->SendMsg((CHAR*)&stNtfNow, CLIGS_COURAGE_GROUP_DISMISS_NTF);
                //位置调出
                pPlayerNow->GetLocale().OnLeaveCourage();
            }
            //队伍解散
            vec_GroupID.push_back(stCourageGroup.dwGroupID);
        }
        else  //给队伍里面别的玩家发送通知
        {
            PKT_CLIGS_COURAGE_QUIT_GROUP_NTF stNtf;
            if (stCourageGroup.wPlayerNum == 0)
            {
                vec_GroupID.push_back(stCourageGroup.dwGroupID);
            }
            else
            {
                stNtf.wAverageLevel = 0;
                stNtf.dwPlayerID = poPlayer->GetID();
                for(UINT8 byIndexNew = 0; byIndexNew < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexNew++)
                {
                    CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer(stCourageGroup.astPlayerData[byIndexNew].dwPlayerID);
                    if ((pPlayer == NULL) || (pPlayer->GetID() == poPlayer->GetID()) || (!pPlayer->CkLogin()))
                    {
                        continue;
                    }
                    //给另外玩家推送通知
                    pPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_COURAGE_QUIT_GROUP_NTF);
                }
            }
        }
    }
    poPlayer->GetLocale().OnLeaveCourage();
    
    vector<UINT32>::iterator itrvec = vec_GroupID.begin();
    for (UINT16 wIndex = 0; wIndex < vec_GroupID.size(); wIndex++)
    {
        CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(vec_GroupID[wIndex]);
        //离开之后没有玩家 清除处理
        if (itr != m_mapGroupID2CourageGroup.end())
        {
            m_oCourageGroupPool.Free(itr->second);
            m_mapGroupID2CourageGroup.erase(itr);
        }
    }
}

//领取勇气奖励
UINT16 CCourageMgr::OnCourageGetAwardReq( CPlayer* poPlayer,PKT_CLIGS_COURAGE_GET_AWARD_ACK &stAck, UINT32 dwIndex)
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        return ERR_COURAGE_GET_AWARD::ID_LOCK;
    }

    memset(&stAck,0x00,sizeof(stAck));

    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();

    //通关了才给算奖励
    if (stCourageData.byHavePass == EBR_NO_PASS)
    {
        //位置回调
        poPlayer->GetLocale().OnLeaveCourageBattle();
        return ERR_COURAGE_GET_AWARD::ID_SUCCESS; 
    }
    UINT16 wErrcode = ERR_COURAGE_GET_AWARD::ID_SUCCESS;

    //只有当有试炼次数的时候才算背包空间
    if ( ESM_YES == stCourageData.byHaveAward && dwIndex == 0)
    {
        wErrcode = CreateItem(poPlayer);
        //创建奖励信息不成功 则背包已满
        if (wErrcode != ERR_COURAGE_GET_AWARD::ID_SUCCESS)
        {
            //奖励信息不清除 直到下次领取
            return wErrcode;
        }
    }


    //背包空间满的时候 点选放弃奖励
    if (dwIndex == 1)
    {
        //位置回调
        poPlayer->GetLocale().OnLeaveCourageBattle();
        stCourageData.byHavePass = EBR_NO_PASS;
        stCourageData.byHaveAward = 0;
        stCourageData.wAverageLevel = 0;
        stCourageData.dwGroupID = 0;
        memset(&stCourageData.stGroupPlayerInfo,0x00,sizeof(stCourageData.stGroupPlayerInfo));
        memset(&stCourageData.stSceneData,0x00,sizeof(stCourageData.stSceneData));
        memset(&stCourageData.stjewelryInfoLst,0x00,sizeof(stCourageData.stjewelryInfoLst));
        return ERR_COURAGE_GET_AWARD::ID_SUCCESS;
    } 

    //成功领取奖励则清除保存的奖励信息
    stCourageData.byHavePass = EBR_NO_PASS;
    stCourageData.byHaveAward = 0;
    stCourageData.wAverageLevel = 0;
    stCourageData.dwGroupID = 0;
    memset(&stCourageData.stGroupPlayerInfo,0x00,sizeof(stCourageData.stGroupPlayerInfo));
    memset(&stCourageData.stSceneData,0x00,sizeof(stCourageData.stSceneData));
    memset(&stCourageData.stjewelryInfoLst,0x00,sizeof(stCourageData.stjewelryInfoLst));
    //位置回调
    poPlayer->GetLocale().OnLeaveCourageBattle();

    return wErrcode;
}

//勇气战斗结果
UINT16 CCourageMgr::OnCouragGetBattleResultReq( CPlayer* poPlayer, PKT_CLIGS_COURAGE_GET_BATTLE_RESULT_ACK &stAck )
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        return ERR_COURAGE_GET_BATTLE_RESULT::ID_LOCK;
    }
    memset(&stAck,0x00,sizeof(stAck));
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    poPlayer->GetLocale().OnEnterCourageBattle(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
    //领取奖励之后就设置为未通关了
    if (stCourageData.byHavePass == EBR_NO_PASS)
    {
        stAck.byHavePass = EBR_NO_PASS;
        return ERR_COURAGE_GET_BATTLE_RESULT::ID_SUCCESS;
    }
    stAck.byHavePass = EBR_PASS;
    //协助者没有奖励 
    if (stCourageData.dwTodayJoinTimes > 0)
    {
        //获取奖励信息
        //GetPassBox(poPlayer,stAck.stPassEncourage);
        for (UINT16 wIndex = 0 ;wIndex < stCourageData.stjewelryInfoLst.wJewelryNum; wIndex++)
        {
            CJewelryPropMgr::Instance()->GetJewelryCli(stCourageData.stjewelryInfoLst.astJewelryInfo[wIndex],stAck.stjewelryInfoLst.astJewelryInfo[wIndex]);
        }
        stAck.stjewelryInfoLst.wJewelryNum = stCourageData.stjewelryInfoLst.wJewelryNum;
    }
    //协助提示 无次数时候下发 队长是不可能下发的
    if (stCourageData.byHavePass == EBR_PASS && stCourageData.dwTodayJoinTimes == 0)
    {
        stAck.byHelpID = GetHelpID(stCourageData.dwTodayHelpTimes, stCourageData.dwGiveFreeTimes);
    }
    //队员信息
    memcpy(&stAck.stGroupPlayerInfo,&stCourageData.stGroupPlayerInfo,sizeof(stCourageData.stGroupPlayerInfo));
    return ERR_COURAGE_GET_BATTLE_RESULT::ID_SUCCESS;

}

//BOOL CCourageMgr::GetMonsterID(UINT16 wLevel, UINT8 byMonsterNum, vector<UINT16>& vecMonsterID )
//{
//    const CCourageMonsterMap& mapCourageMonster = CMonsterPropMgr::Instance()->GetCourageMonsterProp();
//    UINT8 byCount = mapCourageMonster.count(wLevel);
//    while (byCount ==0 && wLevel != 0)
//    {
//        wLevel--;
//        byCount = mapCourageMonster.count(wLevel);
//    }
//    if (byCount == 0)
//    {
//        return FALSE;
//    }
//    vector<UINT16>RomID;
//    CCourageMonsterMapConstItr itr = mapCourageMonster.lower_bound(wLevel);
//    if (itr == mapCourageMonster.end())
//    {
//        return FALSE;
//    }
//    UINT8 byIndex = 0;
//    for (;itr !=mapCourageMonster.end() && byIndex<byCount; itr++, byIndex++)
//    {
//        RomID.push_back(itr->second.wKindID);
//    }
//    UINT8 byRandIndex =0;
//    UINT16 wID = 0;
//    for (UINT8 by = 0; by < byMonsterNum ;by++)
//    {
//        byRandIndex = Random(0, RomID.size()-1);
//        vecMonsterID.push_back(RomID[byRandIndex]);
//    }
//    if (vecMonsterID.size() == 0 || vecMonsterID.size() != byMonsterNum)
//    {
//        return FALSE;
//    }
//    return TRUE;
// 
//
//}


//BOOL CCourageMgr::GetMonsterID( UINT16 wLevel, UINT8 byMonsterNum, map<UINT8,vector<UINT16>>& mapMonsterID )
//{
//    const CCourageMonsterMap& mapCourageMonster = CMonsterPropMgr::Instance()->GetCourageMonsterProp();
//    UINT8 byCount = mapCourageMonster.count(wLevel);
//    while (byCount ==0 && wLevel != 0)
//    {
//        wLevel--;
//        byCount = mapCourageMonster.count(wLevel);
//    }
//    if (byCount == 0)
//    {
//        return FALSE;
//    }
//    vector<UINT16>RomID;
//    CCourageMonsterMapConstItr itr = mapCourageMonster.lower_bound(wLevel);
//    if (itr == mapCourageMonster.end())
//    {
//        return FALSE;
//    }
//    UINT8 byIndex = 0;
//    for (;itr !=mapCourageMonster.end() && byIndex<byCount; itr++, byIndex++)
//    {
//        RomID.push_back(itr->second.wKindID);
//    }
//    UINT8 byRandIndex =0;
//    UINT16 wID = 0;
//    vector<UINT16>vecMonsterID;
//    vecMonsterID.clear();
//    switch(byMonsterNum)
//    {
//        case 3:   // 1+1+1
//            {
//                for (UINT8 by = 0; by < byMonsterNum ;by++)
//                {
//                    byRandIndex = Random(0, RomID.size()-1);
//                    vecMonsterID.push_back(RomID[byRandIndex]);
//                    mapMonsterID.insert(make_pair(by,vecMonsterID));
//                    vecMonsterID.clear();
//                }
//                break;
//            }
//        case 7:   //3+3+1
//            {
//                UINT8 by = 0;
//                for (; by < 3 ;by++)
//                {
//                    byRandIndex = Random(0, RomID.size()-1);
//                    vecMonsterID.push_back(RomID[byRandIndex]);
//                    mapMonsterID.insert(make_pair(1,vecMonsterID));
//                }
//                vecMonsterID.clear();
//                for (; by < 6 ;by++)
//                {
//                    byRandIndex = Random(0, RomID.size()-1);
//                    vecMonsterID.push_back(RomID[byRandIndex]);
//                    mapMonsterID.insert(make_pair(2,vecMonsterID));
//                }
//                vecMonsterID.clear();
//                byRandIndex = Random(0, RomID.size()-1);
//                vecMonsterID.push_back(RomID[byRandIndex]);
//                mapMonsterID.insert(make_pair(3,vecMonsterID));
//                break;
//            }
//        case 9:   //3+3+3
//            {
//                UINT8 by = 0;
//                    for (; by < 3 ;by++)
//                    {
//                        byRandIndex = Random(0, RomID.size()-1);
//                        vecMonsterID.push_back(RomID[byRandIndex]);
//                        mapMonsterID.insert(make_pair(1,vecMonsterID));
//                    }
//                    vecMonsterID.clear();
//                    for (; by < 6 ;by++)
//                    {
//                        byRandIndex = Random(0, RomID.size()-1);
//                        vecMonsterID.push_back(RomID[byRandIndex]);
//                        mapMonsterID.insert(make_pair(2,vecMonsterID));
//                    }
//                    vecMonsterID.clear();
//                    for (; by < 9 ;by++)
//                    {
//                        byRandIndex = Random(0, RomID.size()-1);
//                        vecMonsterID.push_back(RomID[byRandIndex]);
//                        mapMonsterID.insert(make_pair(3,vecMonsterID));
//                    }
//                break;
//            }
//    }
//    if (mapMonsterID.size() == 0 || mapMonsterID.size() != 3)
//    {
//        return FALSE;
//    }
//    return TRUE;
//}

//随机获取妖怪 客户端发上来的ID为显示ID 必须为9只怪
//BOOL CCourageMgr::GetMonsterID( UINT16 wLevel, UINT8 byMonsterNum, CMapMonsterIID& mapMonsterID, vector<UINT16> vecMonsterFaceID)
//{
//    //必须为9只怪
//    if (vecMonsterFaceID.size() != 9)
//    {
//        //return FALSE;
//
//        vecMonsterFaceID.clear();
//        for (UINT8 byIndex = 0; byIndex < 9; byIndex++)
//        {
//            vecMonsterFaceID.push_back(101);
//        }
//    }
//    srand((UINT32)(time(NULL) + rand()));
//    //随机阵型
//    BOOL bAttack = FALSE;
//    const CCourageMonsterMap& mapCourageMonster = CMonsterPropMgr::Instance()->GetCourageMonsterProp();
//    BOOL bExistAttack = FALSE;
//    BOOL bExistDefend = FALSE;
//    BOOL bStop = FALSE;
//    //保证有一只攻击怪和防御怪
//    while (bStop == FALSE && wLevel != 0)
//    {
//        CCourageMonsterMapConstItr itrTemp = mapCourageMonster.lower_bound(wLevel);
//        for (;itrTemp != mapCourageMonster.end() && itrTemp->second.wLevel <= wLevel; ++itrTemp)
//        {
//            if((bExistDefend == TRUE) && (bExistAttack == TRUE))
//            {
//                bStop = TRUE;
//                break;
//            }
//            if (itrTemp->second.byCareerID == 1)
//            {
//                bExistAttack = TRUE;
//            }
//            else
//            {
//                bExistDefend = TRUE;
//            }
//        }
//        if (bStop == FALSE)
//        {
//            wLevel--;
//        }
//    }
//    if ( bStop == FALSE)
//    {
//        return FALSE;
//    }
//    vector<UINT16>RomAttackID;
//    vector<UINT16>RomDefendID;
//    CCourageMonsterMapConstItr itr = mapCourageMonster.lower_bound(wLevel);
//    if (itr == mapCourageMonster.end())
//    {
//        return FALSE;
//    }
//    //获取防御和攻击怪
//    for (;itr != mapCourageMonster.end() && itr->second.wLevel <= wLevel; ++itr)
//    {
//        if (itr->second.byCareerID == 1)
//        {
//            RomAttackID.push_back(itr->second.wKindID);
//        }
//        else
//        {
//            RomDefendID.push_back(itr->second.wKindID);
//        }
//    }
//    bAttack = RomAttackID.size() > RomDefendID.size() ? TRUE : FALSE;
//    UINT8 byRandIndex =0;
//    vecCourageMonster vecMonsterID;
//    vecMonsterID.clear(); 
//    SCourageMonsterProp sprop;
//   
//    UINT8 byMonsterIndex = 0;
//    //3波怪
//    for (UINT8 by = 1; by < 4 ;by++)
//    {
//        //随机阵型
//        bAttack = Random(0,1);
//        vecMonsterID.clear();
//        for (UINT8 byIndex = 0 ;byIndex < 3; byIndex++)
//        {
//            if (bAttack == 0)
//            {
//                // 2 4 6 防御阵型
//                if(byIndex == 0)
//                {
//                    byRandIndex = Random(0, RomAttackID.size()-1);
//                    sprop.wMonsterID = RomAttackID[byRandIndex];
//                }
//                else
//                {   
//                    byRandIndex = Random(0, RomDefendID.size()-1);
//                    sprop.wMonsterID = RomDefendID[byRandIndex];
//                }
//                sprop.byFormationIdx = 2*byIndex + 2;
//
//            }
//            else
//            {
//                // 1 3 5 攻击阵型
//                if (byIndex == 2)
//                {
//                    byRandIndex = Random(0, RomDefendID.size()-1);
//                    sprop.wMonsterID = RomDefendID[byRandIndex];
//                }
//                else
//                {
//                    byRandIndex = Random(0, RomAttackID.size()-1);
//                    sprop.wMonsterID = RomAttackID[byRandIndex];
//                }
//                
//                sprop.byFormationIdx = 2*byIndex + 1;
//            }
//            if (byMonsterIndex >= MAX_MONSTER_NUM)
//            {
//                return FALSE;
//            }
//            sprop.wFaceID = vecMonsterFaceID[byMonsterIndex++];
//            vecMonsterID.push_back(sprop);
//        }
//        mapMonsterID.insert(make_pair(by,vecMonsterID));
//    }
//  
//    if (mapMonsterID.size() == 0 || mapMonsterID.size() != 3)
//    {
//        return FALSE;
//    }
//    return TRUE;
//}

//设置默认伙伴
UINT16 CCourageMgr::GetMaxLevelHero( CPlayer* poPlayer, UINT16 &wLevel)
{
    if (NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }
    CFormationHeroShmemMap& mapFormationHero = poPlayer->GetFormationHeroMap();
    CHero* poHero = mapFormationHero.GetFistData();
    UINT16 wMaxLevel = 0;
    UINT16 wBackLevel = 0;
    UINT16 wKindID = 0;
    while(poHero)
    {
        const DT_HERO_BASE_DATA& stDT_HERO_BASE_DATA = poHero->GetDT_HERO_BASE_DATA();
        if (poPlayer->GetCoachHeroKindID() == stDT_HERO_BASE_DATA.wKindID)
        {
            poHero = mapFormationHero.GetNextData();
            continue;
        }
        wMaxLevel = stDT_HERO_BASE_DATA.wLevel;
        if (wMaxLevel > wBackLevel)
        {
            wBackLevel = wMaxLevel;
            wKindID = stDT_HERO_BASE_DATA.wKindID;
            wLevel  =stDT_HERO_BASE_DATA.wLevel;
        }
        poHero = mapFormationHero.GetNextData();
    }
    return wKindID;
}

UINT16 CCourageMgr::OnLeaveBattle( CPlayer* poPlayer )
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        return ERR_COURAGE_BATTLE_LEAVE_GROUP::ID_SUCCESS;
    }

    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        return ERR_COURAGE_BATTLE_LEAVE_GROUP::ID_LOCK;
    }
    poPlayer->GetLocale().OnLeaveCourageBattle();
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    stCourageData.byHavePass = EBR_NO_PASS;
    stCourageData.byHaveAward = 0;
    stCourageData.wAverageLevel = 0;

    memset(&stCourageData.stGroupPlayerInfo,0x00,sizeof(stCourageData.stGroupPlayerInfo));
    memset(&stCourageData.stSceneData,0x00,sizeof(stCourageData.stSceneData));
    memset(&stCourageData.stjewelryInfoLst,0x00,sizeof(stCourageData.stjewelryInfoLst));
    return ERR_COURAGE_BATTLE_LEAVE_GROUP::ID_SUCCESS;
}

BOOL CCourageMgr::SetEnterGroupData( CPlayer* poPlayer )
{
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    //验证战斗类型是否正确
    UINT8 byJoinBattleType = stCourageData.dwTodayJoinTimes <= 0 ? EBT_HELP : EBT_JOIN;

    //验证队伍ID是否存在，是否满足条件
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(stCourageData.dwGroupID);
    if (itr == m_mapGroupID2CourageGroup.end())
    {
        //队伍不存在
        return FALSE;
    }
    DT_COURAGE_GROUP_DATA &stGroupData = itr->second->GetDT_COURAGE_GROUP_DATA();
    //队长数据在创建队伍时候就已经赋值
    if (stGroupData.dwLeaderID == poPlayer->GetID())
    {
        return TRUE;
    }
    UINT16 wPlayNum = stGroupData.wPlayerNum;
    if ((wPlayNum >= MAX_COURAGE_GROUP_PLAYER_NUM) || (wPlayNum == 0))
    {
        return FALSE;
    }
    DT_COURAGE_GROUP_DATA stCOURAGE_GROUP_DATA;
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();
    UINT8 byPlayIndex  = 0;
    if (wPlayNum > 1 )
    {
        //玩家ID为0 则当前索引没有玩家
        if (stGroupData.astPlayerData[wPlayNum].dwPlayerID != 0)
        {
            wPlayNum = 1;
        }
    }
    SDStrcpy(stGroupData.astPlayerData[wPlayNum].aszDispPlayerName, stDT_PLAYER_BASE_DATA.aszDispName);

    stCOURAGE_GROUP_DATA.byJoinType = stGroupData.byJoinType;
    stCOURAGE_GROUP_DATA.dwGroupID = stGroupData.dwGroupID;
    stCOURAGE_GROUP_DATA.wPlayerMaxNum = MAX_COURAGE_GROUP_PLAYER_NUM;
    stCOURAGE_GROUP_DATA.wMinLevel = stGroupData.wMinLevel;

    UINT16 wLevel = 0;
    stGroupData.astPlayerData[wPlayNum].byBattleType = byJoinBattleType;
    stGroupData.astPlayerData[wPlayNum].byMemberType = EBI_MEMBER;
    stGroupData.astPlayerData[wPlayNum].byReadyType = EBR_READYING;
    stGroupData.astPlayerData[wPlayNum].dwPlayerID = poPlayer->GetID();
    stGroupData.astPlayerData[wPlayNum].wLevel = poPlayer->GetLevel();
    stGroupData.astPlayerData[wPlayNum].wKindID = poPlayer->GetCoachHeroKindID();
    stGroupData.astPlayerData[wPlayNum].wHeroID = GetMaxLevelHero(poPlayer,wLevel);
    stGroupData.astPlayerData[wPlayNum].wHeroLevel = wLevel;

    UINT16 wJingJieLevel = 0;
    if (NULL != poPlayer->GetJingJie())
    {
        wJingJieLevel = poPlayer->GetJingJie()->GetDT_JINGJIE_DATA().stBaseData.wLevel;
        stGroupData.astPlayerData[wPlayNum].byColor = (UINT8)CJingJiePropMgr::Instance()->GetJingJieColor(wJingJieLevel); 
    }     
    else
    {
        stGroupData.astPlayerData[wPlayNum].byColor = 0;
    }

    memcpy(&stCOURAGE_GROUP_DATA.astPlayerData, &stGroupData.astPlayerData, sizeof(stCOURAGE_GROUP_DATA.astPlayerData));

    stCourageData.wGroupIndex = wPlayNum;
    stGroupData.wPlayerNum++;
    stCOURAGE_GROUP_DATA.wPlayerNum = stGroupData.wPlayerNum;
 
    //给队伍里面别的玩家发送通知
    PKT_CLIGS_COURAGE_JOIN_GROUP_NTF stNtf;
    memset(&stNtf.stPlayerGroupData, 0x00, sizeof(DT_COURAGE_PLAYER_DATA));
    memcpy(&stNtf.stPlayerGroupData, &stCOURAGE_GROUP_DATA.astPlayerData[stCourageData.wGroupIndex], sizeof(stNtf.stPlayerGroupData));
    stNtf.dwAverageLevel = 0;
    for(UINT8 byIndex = 0; byIndex < MAX_COURAGE_GROUP_PLAYER_NUM; byIndex++)
    {
        CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer(itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndex].dwPlayerID);
        if((NULL == pPlayer) /*|| (!pPlayer->CkLogin())*/ || (pPlayer->GetID() == poPlayer->GetID()))
        {
            continue;
        }
        //通知
        pPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_COURAGE_JOIN_GROUP_NTF);
    }

    //返回成功
    return TRUE;
}

VOID CCourageMgr::OnEvent()
{
    //活动是否开启
    if(m_bActivityOver)
    {
        //活动未开始
        return;
    }

    if (m_m_byActivityID == 0)
    {
        return;
    }

    RefreshGroupNtf();
}

UINT16 CCourageMgr::OnLeaveCourage( CPlayer* poPlayer )
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        return ERR_LEAVE_COURAGE::ID_SUCCESS;
    }

    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        return ERR_LEAVE_COURAGE::ID_LOCK;
    }
    //是否到活动时间
    if (m_bActivityOver)
    {
        return ERR_LEAVE_COURAGE::ID_UNOPEN;
    }
    C3232MapItr itrPlayID = m_mapAllEnterPlayer.find(poPlayer->GetID());
    if (itrPlayID != m_mapAllEnterPlayer.end())
    {
        m_mapAllEnterPlayer.erase(itrPlayID);
    }
    ////赋值离开时间
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    //stCourageData.qwLastTime = SDTimeSecs();

    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(stCourageData.dwGroupID);
    if (itr == m_mapGroupID2CourageGroup.end())
    {
        stCourageData.dwGroupID = 0;
        return ERR_LEAVE_COURAGE::ID_SUCCESS;
    } 
    if (itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum > 0)
    {
         itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum--;
    }

    if (itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum == 0)
    {
        stCourageData.dwGroupID = 0;
        m_oCourageGroupPool.Free(itr->second);
        m_mapGroupID2CourageGroup.erase(itr);
        return ERR_LEAVE_COURAGE::ID_SUCCESS;
    }

   
    UINT8 byLeftNum = 0;
    BOOL bDelete = FALSE;
    //队长离开了 则队伍解散
    if (itr->second->GetDT_COURAGE_GROUP_DATA().dwLeaderID == poPlayer->GetID())
    {
        //队伍解散
        bDelete = TRUE;
        PKT_CLIGS_COURAGE_GROUP_DISMISS_NTF stNtfNow;
        stNtfNow.byErrCode = ERR_COURAGE_GROUP_DISMISS::ID_SUCCESS;
        for (UINT8 byIndexPlayer = 0; byIndexPlayer < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexPlayer++)
        {
            CPlayer* pPlayerNow = CPlayerMgr::Instance()->FindPlayer(itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndexPlayer].dwPlayerID);
            if ((pPlayerNow == NULL) || (pPlayerNow->GetID() == poPlayer->GetID()))
            {
                continue;
            }
            SetPlayerGroupValue(pPlayerNow, stNtfNow.stPlayerCourageData, stNtfNow.stGroupData);
            pPlayerNow->SendMsg((CHAR*)&stNtfNow, CLIGS_COURAGE_GROUP_DISMISS_NTF);
        }

    }
    else  //给队伍里面别的玩家发送通知
    {
        memset(&itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[stCourageData.wGroupIndex], 0x00, sizeof(itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[stCourageData.wGroupIndex]));
        PKT_CLIGS_COURAGE_QUIT_GROUP_NTF stNtf;
        UINT32 wAverageLevel = 0;
        if (itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum == 0)
        {
            wAverageLevel = 0;
        }
        else
        {
            wAverageLevel = (itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[0].wLevel + itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[1].wLevel +
                itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[2].wLevel) / itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum;
        }
        stNtf.wAverageLevel = wAverageLevel;
        stNtf.dwPlayerID = poPlayer->GetID();
        for(UINT8 byIndexNew = 0; byIndexNew < MAX_COURAGE_GROUP_PLAYER_NUM; byIndexNew++)
        {
            CPlayer* pPlayer = CPlayerMgr::Instance()->FindPlayer(itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndexNew].dwPlayerID);
            if ((pPlayer == NULL) || (pPlayer->GetID() == poPlayer->GetID()))
            {
                continue;
            }
            if(!pPlayer->CkLogin())
            {
                continue;
            }
            //给另外玩家推送通知
            pPlayer->SendMsg((const CHAR*)(&stNtf), CLIGS_COURAGE_QUIT_GROUP_NTF);
        }
    }
    //离开之后没有玩家 清除处理
    if ((byLeftNum == MAX_COURAGE_GROUP_PLAYER_NUM) || (bDelete == TRUE))
    {
        m_oCourageGroupPool.Free(itr->second);
        m_mapGroupID2CourageGroup.erase(itr);
    }
    
    stCourageData.dwGroupID = 0;
    return ERR_LEAVE_COURAGE::ID_SUCCESS;    
}

 //随机加入队伍或者协助队伍
UINT16 CCourageMgr::OnCourageRandomJoinGroup( CPlayer* poPlayer,PKT_CLIGS_COURAGE_RANDOM_JOIN_GROUP_ACK &stAck )
{
    if((NULL == poPlayer) || (NULL == poPlayer->GetCourage()))
    {
        RETURN_OTHER_ERR;
    }
    CBuild* pBuild = poPlayer->GetBuild(EBK_COURAGE);

    if ( NULL == pBuild )
    {
        return ERR_COURAGE_RANDOM_JOIN_GROUP::ID_LOCK;
    }
    if(m_bActivityOver)
    {
        //活动未开始
        return ERR_COURAGE_RANDOM_JOIN_GROUP::ID_UNOPEN;
    }

    if(HaveBringHero(poPlayer) == FALSE)
    {
        return ERR_COURAGE_RANDOM_JOIN_GROUP::ID_NO_HERO;
    }
    //是否重置数据
    ResetCourageData(poPlayer);
    //获取最新试炼次数
    GetCourageTimes(poPlayer);
    DT_COURAGE_GROUP_DATA stCOURAGE_GROUP_DATA;
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    //验证是否在队伍中
    BOOL bHaveInGroup = HaveInGroup(poPlayer, stCOURAGE_GROUP_DATA, stCourageData.dwGroupID);
    if (bHaveInGroup == TRUE)
    {
        //返回成功
        return ERR_COURAGE_RANDOM_JOIN_GROUP::ID_SUCCESS;
    }
    if ((stCourageData.dwTodayHelpTimes >= CSinglePramMgr::Instance()->GetCourageHelpTimes()) && (stCourageData.dwTodayJoinTimes == 0))
    {
		UINT64 qwPassTime = GetDayPassSecond();
		const CCourageActivityTimePropMap& mapTime = CCouraPropMgr::Instance()->GetCourageActivityEndTimeProp();
		if( qwPassTime < mapTime.begin()->second.dwEndTime)
		{
			return ERR_COURAGE_RANDOM_JOIN_GROUP::ID_NO_HELP_TIMES_NOON;
		}
		else
		{
			return ERR_COURAGE_RANDOM_JOIN_GROUP::ID_NO_HELP_TIMES_NIGHT;
		}
        return ERR_COURAGE_RANDOM_JOIN_GROUP::ID_NO_HELP_TIMES;
    }
    //获取符合条件的队伍
    BOOL bHaveGroup = GetRightGroup(poPlayer);
    if (bHaveGroup == FALSE || poPlayer->GetCourage()->m_vecGroupID.size() == 0)
    {
        return ERR_COURAGE_RANDOM_JOIN_GROUP::ID_NO_RIGHT_GROUP;
    }
    UINT32 dwRandIndex = Random(0, poPlayer->GetCourage()->m_vecGroupID.size() - 1);
    UINT32 dwGroupNewID = poPlayer->GetCourage()->m_vecGroupID[dwRandIndex];

    if (dwGroupNewID == 0)
    {
        //队伍不存在
        return ERR_COURAGE_RANDOM_JOIN_GROUP::ID_NO_RIGHT_GROUP;
    }

    UINT8 byMaxIndex = 100;
    while (TRUE)
    {
        //防止死循环
        if (byMaxIndex == 0)
        {
            //队伍不存在
            return ERR_COURAGE_RANDOM_JOIN_GROUP::ID_NO_RIGHT_GROUP;

        }
        CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(dwGroupNewID);
        if (itr == m_mapGroupID2CourageGroup.end())
        {
            dwRandIndex = Random(0, poPlayer->GetCourage()->m_vecGroupID.size() - 1);
            dwGroupNewID = poPlayer->GetCourage()->m_vecGroupID[dwRandIndex];
            byMaxIndex--;
            continue;
        }
        //在验证一次数据
        if (itr->second->GetDT_COURAGE_GROUP_DATA().wMinLevel <= poPlayer->GetLevel()&& itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum < MAX_COURAGE_GROUP_PLAYER_NUM)
        {
            break;
        }
        
        dwRandIndex = Random(0, poPlayer->GetCourage()->m_vecGroupID.size() - 1);
        dwGroupNewID = poPlayer->GetCourage()->m_vecGroupID[dwRandIndex];
        byMaxIndex--;

    }
    //保存组ID
    stCourageData.dwGroupID = dwGroupNewID;
    BOOL bRet = SetEnterGroupData(poPlayer);
    if (bRet == FALSE)
    {
         return ERR_COURAGE_RANDOM_JOIN_GROUP::ID_NO_RIGHT_GROUP;
    }

    C3232MapItr itrPlayID = m_mapAllEnterPlayer.find(poPlayer->GetID());
    if (itrPlayID != m_mapAllEnterPlayer.end())
    {
        m_mapAllEnterPlayer.erase(itrPlayID);
    }
    stAck.dwGroupID = dwGroupNewID;
    CListLevelGroupItr itrNew = poPlayer->GetCourage()->m_listLevelGroup.begin();
    //排序后赋值
    for ( itrNew; itrNew != poPlayer->GetCourage()->m_listLevelGroup.end() && stAck.stGroupData.wTeamNum < CCouraPropMgr::Instance()->GetMaxGroupNum(); itrNew++)
    {
        CCourageGroup *poCourageGroup = *itrNew;
        if (poCourageGroup == NULL)
        {
            continue;
        }

        DT_COURAGE_GROUP_BASE_DATA_CLI &stCoruageGroupBaseData = stAck.stGroupData.astTeamInfo[stAck.stGroupData.wTeamNum++];
        stCoruageGroupBaseData.byJoinType = poCourageGroup->GetDT_COURAGE_GROUP_DATA().byJoinType;
        stCoruageGroupBaseData.dwGroupID = poCourageGroup->GetDT_COURAGE_GROUP_DATA().dwGroupID;
        stCoruageGroupBaseData.wPlayerNum = poCourageGroup->GetDT_COURAGE_GROUP_DATA().wPlayerNum;
        stCoruageGroupBaseData.wPlayerMaxNum = MAX_COURAGE_GROUP_PLAYER_NUM;
        stCoruageGroupBaseData.wMinLevel = poCourageGroup->GetDT_COURAGE_GROUP_DATA().wMinLevel;

        const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(poCourageGroup->GetDT_COURAGE_GROUP_DATA().dwLeaderID);
        if (poPlayerBaseData != NULL)
        {
            SDStrcpy(stCoruageGroupBaseData.stLeaderData.aszDispPlayerName, poPlayerBaseData->aszDispName);
            stCoruageGroupBaseData.stLeaderData.wLevel = poPlayerBaseData->wLevel;
            stCoruageGroupBaseData.stLeaderData.wKindID = poPlayerBaseData->wMainHeroKindID;
        }
    }
    //加入队伍的玩家添加定时器
    //CTimerEventMgr::Instance()->AddCourageFireEvent(poPlayer->GetID());   
    return ERR_COURAGE_RANDOM_JOIN_GROUP::ID_SUCCESS;    
    
}


UINT16 CCourageMgr::CompareMapGroup( CMapGroupIDNum m_mapBackGroupID, CMapGroupIDNum m_mapGroupIDNum )
{
    CMapGroupIDNumDItr itr1 = m_mapBackGroupID.begin();
    CMapGroupIDNumDItr itr2 = m_mapGroupIDNum.begin();
    if (m_mapBackGroupID.size() == m_mapGroupIDNum.size() && m_mapBackGroupID.size() == 0)
    {
        return 1;
    }
    if (m_mapBackGroupID.size() != m_mapGroupIDNum.size() || itr1 == m_mapBackGroupID.end() || itr2 == m_mapGroupIDNum.end())
    {
        return 0;
    }
    for (itr1,itr2;itr1 != m_mapBackGroupID.end()&itr2 != m_mapGroupIDNum.end(); ++itr1, ++itr2)
    {
         if((itr1->first != itr2->first) || (itr1->second != itr2->second))
         {
             return 0;
         }
    }
    return 2;
}

//生成队伍战力信息
BOOL CCourageMgr::CreateGroupPower( UINT32 dwGroupID ,DT_COURAGE_PLAYERID_HEROID_DATA astPlayerInfo[MAX_COURAGE_MEMBER_NUM])
{
    //判断是否在队伍中
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(dwGroupID);
    if(itr == m_mapGroupID2CourageGroup.end())
    {
        return FALSE;
    }
    //寻找队伍的战力信息
    CPlayerGroupMapItr itrGourpMap = m_mapPlayerGroupMap.find(dwGroupID);
    if (itrGourpMap == m_mapPlayerGroupMap.end())
    {
        vector<CHero*> vecHero;
        vecHero.resize(7);
        vecHero[0] = NULL;
        for (UINT16 wPlayerNum = 0; wPlayerNum < itr->second->GetDT_COURAGE_GROUP_DATA().wPlayerNum && wPlayerNum < MAX_COURAGE_GROUP_PLAYER_NUM; wPlayerNum++)
        {
            DT_COURAGE_PLAYER_DATA &stDT_COURAGE_PLAYER_DATA = itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[wPlayerNum];
            DT_COURAGE_PLAYERID_HEROID_DATA& COURAGE_PLAYER_DATA = astPlayerInfo[wPlayerNum];
            CPlayer* v_poPlayer = CPlayerMgr::Instance()->FindPlayer(stDT_COURAGE_PLAYER_DATA.dwPlayerID);
            if (NULL == v_poPlayer)
            {
                continue;
            }
            UINT16 wKindID = 0;
            const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stDT_COURAGE_PLAYER_DATA.dwPlayerID);
            if (poPlayerBaseData != NULL )
            {
                wKindID = poPlayerBaseData->wMainHeroKindID;
                CHero *v_poHero = v_poPlayer->GetCoachHero();
                if (NULL == v_poHero)
                {
                    continue;
                }
                BYTE byflag = 0;
                //为战士 则站在前面
                if (wKindID == 1 || wKindID == 4)
                {
                    byflag = 1;
                    vecHero[wPlayerNum+1] = v_poHero;
                    COURAGE_PLAYER_DATA.byCurAttackObjIdx = wPlayerNum+1;
                }
                else
                {
                    byflag = 2;
                    vecHero[wPlayerNum+4] = v_poHero;
                    COURAGE_PLAYER_DATA.byCurAttackObjIdx = wPlayerNum+4;
                }
                v_poHero = v_poPlayer->GetHero(stDT_COURAGE_PLAYER_DATA.wHeroID);
                if (NULL == v_poHero)
                {
                    continue;
                }
                if (byflag == 1)
                {
                    vecHero[wPlayerNum+4] = v_poHero;
                }
                else if (byflag == 2)
                {
                    vecHero[wPlayerNum+1] = v_poHero;
                }
            }
        }
        //对于玩家组队信息每次都不一样，每次创建后都要删除
        CPlayerGroup *poPlayerGroup = CPlayerGroupMgr::Instance()->CreatePlayerGroup(vecHero);
        vecHero.clear();
        if (NULL == poPlayerGroup)
        {
            return FALSE;
        }
        //CPlayerGroup *poPlayGroupBack = m_oPlayerCouragePool.Alloc();
        //memcpy(poPlayGroupBack,poPlayerGroup,sizeof(CPlayerGroup));
        m_mapPlayerGroupMap.insert(make_pair(itr->second->GetDT_COURAGE_GROUP_DATA().dwGroupID,poPlayerGroup));
    }
    
    return TRUE;
}

VOID CCourageMgr::GetGVEInstanceRecordData(CPlayer* poPlayer, DT_TOWN_INSTANCE_RECORD_DATA_CLI &stData, UINT16 wSceneIdx, UINT8 byTownIndex)
{
    //获取城镇信息
    STownInstanceProp* pstTownInstanceProp = NULL;
    pstTownInstanceProp = CInstancePropMgr::Instance()->GetSceneInstance(wSceneIdx, byTownIndex);
    if (NULL == pstTownInstanceProp)
    {
        return ;
    }
    stData.byIndex = byTownIndex;
    for (UINT32 dwInstanceIdx = 1; dwInstanceIdx <= pstTownInstanceProp->m_mapInstance.size(); dwInstanceIdx++)
    {
        SInstanceProp* pstInstanceProp = NULL;
        pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(wSceneIdx, byTownIndex, dwInstanceIdx);
        if (NULL == pstInstanceProp)
        {
            continue;
        }

        //满足进入副本的等级要求
        if (poPlayer->GetLevel() >= pstInstanceProp->wEnterLevel && pstInstanceProp->byIndex < 5) 
        {
            //解锁的数量刚好为当前的索引
            stData.byInstanceNum = pstInstanceProp->byIndex;
            UINT8 byInstaceID = pstInstanceProp->byIndex >= 1 ? pstInstanceProp->byIndex -1 : 0;
            DT_INSTANCE_RECORD_DATA_CLI &stDT_INSTANCE_RECORD_DATA_CLI = stData.astInstanceInfo[byInstaceID];
            stDT_INSTANCE_RECORD_DATA_CLI.byIndex = pstInstanceProp->byIndex;
            stDT_INSTANCE_RECORD_DATA_CLI.wEnterLevel = pstInstanceProp->wEnterLevel;                      
            stDT_INSTANCE_RECORD_DATA_CLI.byScore = GetScore(pstInstanceProp->wEnterLevel);
            memcpy(&( stDT_INSTANCE_RECORD_DATA_CLI.stBurstItemInfo), &(pstInstanceProp->stPassBoxProp.stDT_BURST_ITEM_DATA), sizeof(DT_BURST_ITEM_DATA));
            return;
        }
        else
        {
            break;
        }
    }

}

BOOL CCourageMgr::CkGVEUnlock(UINT16 wLevel, UINT16 wSceanID, UINT8 byTownID)
{
    //一个城镇对应一个副本来处理
    STownInstanceProp * pstTownInstanceProp = NULL;
    pstTownInstanceProp = CInstancePropMgr::Instance()->GetSceneInstance(wSceanID, byTownID);
    if (NULL == pstTownInstanceProp)
    {   
        SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr CkGVEUnlock pstInstanceProp[%d.%d.%d] is NULL!"), MSG_MARK, wSceanID, byTownID, 1);
        return FALSE;
    }

    for (UINT32 dwInstanceIdx = 1; dwInstanceIdx <= pstTownInstanceProp->m_mapInstance.size(); dwInstanceIdx++)
    {
        SInstanceProp* pstInstanceProp = NULL;
        pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(wSceanID, byTownID, dwInstanceIdx);
        if (NULL == pstInstanceProp)
        {
            continue;
        }

        //满足进入副本的等级要求
        if (wLevel >= pstInstanceProp->wEnterLevel) 
        {
            return TRUE;
        }
        //第一个都不满足就跳出循环
        break;
    }

    return FALSE;
}

//计算通关宝箱奖励
VOID CCourageMgr::RatePassBoxEnc(CPlayer* poPlayer, UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, DT_COURAGE_JEWELRY_DATA_LST& stJewlryDataList)
{
	if ( NULL == poPlayer)
	{
		return ;
	}
	CFewParam& poFewParam = poPlayer->GetFewParam();
	poFewParam.AddGVEPassNum();
	//成功通关 扣除一次试炼次数 扣除在领取奖励那边扣了
	if (poPlayer->GetCourage()->GetDT_COURAGE_DATA().dwTodayJoinTimes > 0)
	{
		USR_INFO("[%s:%u] PlayerID = %u, CurJoinTime = %u", MSG_MARK, poPlayer->GetID(), poPlayer->GetCourage()->GetDT_COURAGE_DATA().dwTodayJoinTimes);
		poPlayer->GetCourage()->GetDT_COURAGE_DATA().dwTodayJoinTimes--;
		//有奖励可领取
		poPlayer->GetCourage()->GetDT_COURAGE_DATA().byHaveAward = ESM_YES;
		CPlayerMedal* poPlayerMedal = poPlayer->GetPlayerMedal();
		if ( NULL != poPlayerMedal)
		{
			poPlayerMedal->AddArrestNum();
			poPlayerMedal->CheckMedalBySmallType(EMST_GVE_COURAGE);
		}
		
	}
	else
	{
		return ;
	}
	

    UINT8 abyHitItemIdx[MAX_GVE_ENCOURAGE_BOX_GROUP_NUM] = {0};
    SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(wSceneIdx, byTownIdx, byInstanceIdx);
    if(NULL == pstInstanceProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr RatePassBoxEnc pstInstanceProp[%d.%d.%d] is NULL!"), MSG_MARK, wSceneIdx, byTownIdx, byInstanceIdx);
        return ;
    }
    SPassBoxProp& stPassBoxProp = pstInstanceProp->stPassBoxProp;
    CItemGroupMap& mapItemGroup = stPassBoxProp.mapItemGroup;
    if(0 == mapItemGroup.size())
    {
        SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr RatePassBoxEnc pstInstanceProp[%d.%d.%d] mapItemGroup size is 0!"), MSG_MARK, wSceneIdx, byTownIdx, byInstanceIdx);
        return ;
    }

    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();

    UINT8 byGroupNum = 0;
    for(CItemGroupMapItr itrMap = mapItemGroup.begin(); (itrMap != mapItemGroup.end()); itrMap++)
    {
        SItemGroupProp& stItemGroupProp = itrMap->second;
        UINT16 wItemKindID = 0;
        UINT8 byHitItemIdx = 0; //0表示未命中       
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
                if(byIdx != nRandomHitIdx)
                {
                    continue;
                }
                wItemKindID = itr->first;
                UINT16 wJewelryID = RateInnerBox(wItemKindID);
                if((wJewelryID != 0) && (byGroupNum < MAX_GVE_ENCOURAGE_BOX_GROUP_NUM))
                {
                    //生成首饰的随机属性
                    CreateJewelryData(poPlayer, wJewelryID,stCourageData.stjewelryInfoLst.astJewelryInfo[byGroupNum]);
                    CJewelryPropMgr::Instance()->GetJewelryCli(stCourageData.stjewelryInfoLst.astJewelryInfo[byGroupNum],stJewlryDataList.astJewelryInfo[byGroupNum]);
                    byGroupNum++;
                }               
            }
        }
        else
        {
            //USR_INFO(_SDT("PlayerID:[%d] [%s: %d]: RatePassBoxEnc pstInstanceProp[%u.%u.%u] UNHIT!"),poPlayer->GetID(), MSG_MARK, wSceneIdx, byTownIdx, byInstanceIdx);
            byHitItemIdx = 0;
        }
        
    }
    stCourageData.stjewelryInfoLst.wJewelryNum = byGroupNum;
    stJewlryDataList.wJewelryNum = byGroupNum;
    //记录进入的副本序号
    stCourageData.stSceneData.wSceneIdx = wSceneIdx;
    stCourageData.stSceneData.byTownIdx = byTownIdx;
    stCourageData.stSceneData.byInstanceIdx = byInstanceIdx;
}

UINT16 CCourageMgr::PassBoxEnc(CPlayer* poPlayer, DT_PASS_ENCOURAGE_BOX_DATA2& stDT_PASS_ENCOURAGE_BOX_DATA, DT_DATA_AFTER_PASS_ENCOURAGE_BOX* pstAfterPassEncourage)
{
    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
    if(NULL == pstInstanceProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: PassBoxEnc pstInstanceProp[%d.%d.%d] is NULL!"), MSG_MARK, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
        RETURN_OTHER_ERR;
    }

    UINT8 abyHitItemIdx[MAX_GVE_ENCOURAGE_BOX_GROUP_NUM] = {0};
    
    CItemGroupMap& mapItemGroup = pstInstanceProp->stPassBoxProp.mapItemGroup;
    CItemKindID2NumMap mapHitItem; //命中物品map
    UINT32 dwCoin = 0;
    UINT32 dwGold = 0;
    UINT32 dwBlueGas = 0;
    UINT8 byGroupNum = 0;
    UINT8 byNeedBagGrid = 0;
    CBag& oBag = poPlayer->GetBag();
    for(CItemGroupMapItr itrGoup = mapItemGroup.begin(); ((itrGoup != mapItemGroup.end()) && (byGroupNum < MAX_GVE_ENCOURAGE_BOX_GROUP_NUM)); itrGoup++, byGroupNum++)
    {
        UINT8 byHitItemIdx = abyHitItemIdx[byGroupNum];
        DT_PASS_ENCOURAGE_BOX_GROUP_DATA& stBoxGroupInfo = stDT_PASS_ENCOURAGE_BOX_DATA.astBoxGroupInfo[byGroupNum];
        UINT16 wItemKindID = 0;
        UINT16 wPileNum = 0;
        if((0 != byHitItemIdx) && (byHitItemIdx <= itrGoup->second.stBoxItem.byItemNum)) //0表示未命中
        {
            DT_ITEM_DATA_CLI& stDT_ITEM_DATA_CLI = itrGoup->second.stBoxItem.astItemList[byHitItemIdx - 1];
            wItemKindID = stDT_ITEM_DATA_CLI.wKindID;
            wPileNum = stDT_ITEM_DATA_CLI.wPileNum;
        }
        else
        {
            USR_INFO(_SDT("PlayerID: [%d] [%s: %d]: PassBoxEnc pstInstanceProp[%d.%d.%d] byHitItemIdx[%d] !<= byItemNum[%d]!"),poPlayer->GetID(), MSG_MARK, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx, byHitItemIdx, itrGoup->second.stBoxItem.byItemNum);
            byHitItemIdx = 0; //避免byHitItemIdx值异常
        }

        stBoxGroupInfo.byHitItemIdx = byHitItemIdx;
        if((0 != wItemKindID) && (0 != byHitItemIdx))
        {
            BOOL bGive = FALSE;
            SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wItemKindID);
            if(pstGoodsProp)
            {
                switch(pstGoodsProp->byMainKindID)
                {
                case EGMK_COIN:
                {
                    UINT32 dwValue = pstGoodsProp->dwValue * wPileNum;
                    dwCoin += dwValue;
                    stBoxGroupInfo.dwCoin = dwValue;
                    bGive = TRUE;
                }
                break;
                case EGMK_GOLD:
                {
                    UINT32 dwValue = pstGoodsProp->dwValue * wPileNum;
                    dwGold += dwValue;
                    stBoxGroupInfo.dwGold = dwValue;
                    bGive = TRUE;
                }
                break;
                case EGMK_BLUEGAS:
                {
                    UINT32 dwValue = pstGoodsProp->dwValue * wPileNum;
                    dwBlueGas += dwValue;
                    stBoxGroupInfo.dwBlueGas = dwBlueGas;
                    bGive = TRUE;
                }
                break;
                }
            }

            if((FALSE == bGive) && (0 != wItemKindID) && (0 != byHitItemIdx))
            {
                mapHitItem[wItemKindID] += wPileNum;

                stBoxGroupInfo.stItemInfo.byItemNum = 1;
                DT_ITEM_DATA_CLI& stDT_ITEM_DATA_CLI = stBoxGroupInfo.stItemInfo.astItemList[0];
                stDT_ITEM_DATA_CLI.wKindID = wItemKindID;
                stDT_ITEM_DATA_CLI.wPileNum = wPileNum;
                if(pstGoodsProp)
                {
                    stDT_ITEM_DATA_CLI.byGoodsMainKindID = pstGoodsProp->byMainKindID;
                }
                if(!oBag.GetGoodsItem(wItemKindID))
                {
                    byNeedBagGrid++; //装备不会配置一种同时给两把
                }
            }
        }
    }
    stDT_PASS_ENCOURAGE_BOX_DATA.byGroupNum = byGroupNum;

    if(oBag.GetIdleSize() < byNeedBagGrid)
    {
        return ERR_COURAGE_GET_AWARD::ID_BAG_FULL;
    }

    //真正给奖励
    poPlayer->AddCoin(dwCoin, CRecordMgr::EACT_INSTANCE, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
    poPlayer->AddGold(dwGold, CRecordMgr::EGGT_ADDGOLDINSTANCE, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
    poPlayer->AddBlueGas(dwBlueGas, CRecordMgr::EABGT_INSTANCEBURST, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
    for(CItemKindID2NumMapItr itrItem = mapHitItem.begin(); itrItem != mapHitItem.end(); itrItem++)
    {
        UINT16 wItemKindID = itrItem->first;
        UINT16 wPileNum = itrItem->second;

        //铜币/元宝/蓝气直接使用
        SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wItemKindID);
        if(pstGoodsProp)
        {
            switch(pstGoodsProp->byMainKindID)
            {
            case EGMK_COIN:
            {
                //m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin += pstGoodsProp->dwValue * wPileNum;
                poPlayer->AddCoin(pstGoodsProp->dwValue * wPileNum, CRecordMgr::EACT_INSTANCEGOODS, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
            }
            continue;
            case EGMK_GOLD:
            {
                //m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold += pstGoodsProp->dwValue * wPileNum;
                poPlayer->AddGold(pstGoodsProp->dwValue * wPileNum, CRecordMgr::EGGT_ADDGOLDINSTANCEGOODS, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
            }
            continue;
            case EGMK_BLUEGAS:
            {
                poPlayer->AddBlueGas(pstGoodsProp->dwValue * wPileNum, CRecordMgr::EABGT_INSTANCEBURST, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
            }
            continue;
            }
        }

        ECreateItemRet Ret = ECIR_SUCCESS;
        CItem* poItem = CItemMgr::Instance()->CreateItemIntoBag(poPlayer, wItemKindID, wPileNum, Ret, 0, CRecordMgr::EAIT_INSTANCE);
    }


    if(pstAfterPassEncourage)
    {
        pstAfterPassEncourage->qwCurCoin = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
        pstAfterPassEncourage->qwCurGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
        pstAfterPassEncourage->qwCurBlueGas = poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas;
    }

    return ERR_COURAGE_GET_AWARD::ID_SUCCESS;
}

VOID CCourageMgr::GetPassBox( CPlayer* poPlayer, DT_PASS_ENCOURAGE_BOX_DATA2& stDT_PASS_ENCOURAGE_BOX_DATA )
{

    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();
    UINT8 abyHitItemIdx[MAX_GVE_ENCOURAGE_BOX_GROUP_NUM] = {0};
    //赋值命中物品的序号

    //获取通过副本的奖励信息
    SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
    if(NULL == pstInstanceProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr RatePassBoxEnc pstInstanceProp[%d.%d.%d] is NULL!"), MSG_MARK, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
        return ;
    }
    SPassBoxProp& stPassBoxProp = pstInstanceProp->stPassBoxProp;
    CItemGroupMap& mapItemGroup = stPassBoxProp.mapItemGroup;
    if(0 == mapItemGroup.size())
    {
        SYS_CRITICAL(_SDT("[%s: %d]:CCourageMgr RatePassBoxEnc pstInstanceProp[%d.%d.%d] mapItemGroup size is 0!"), MSG_MARK, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx);
        return ;
    }

    UINT8 byGroupNum = 0;
    for(CItemGroupMapItr itrGoup = mapItemGroup.begin(); ((itrGoup != mapItemGroup.end()) && (byGroupNum < MAX_GVE_ENCOURAGE_BOX_GROUP_NUM)); itrGoup++, byGroupNum++)
    {
        SItemGroupProp& stItemGroupProp = itrGoup->second;
        //命中的序号ID
        UINT8 byHitItemIdx = abyHitItemIdx[byGroupNum];
        DT_PASS_ENCOURAGE_BOX_GROUP_DATA& stBoxGroupInfo = stDT_PASS_ENCOURAGE_BOX_DATA.astBoxGroupInfo[byGroupNum];
        UINT16 wItemKindID = 0;
        UINT16 wPileNum = 0;

        if((0 != byHitItemIdx) && (byHitItemIdx <= itrGoup->second.stBoxItem.byItemNum)) //0表示未命中
        {
            DT_ITEM_DATA_CLI& stDT_ITEM_DATA_CLI = itrGoup->second.stBoxItem.astItemList[byHitItemIdx - 1];
            wItemKindID = stDT_ITEM_DATA_CLI.wKindID;
            wPileNum = stDT_ITEM_DATA_CLI.wPileNum;
        }
        else
        {
            USR_INFO(_SDT("[%s: %d]: PassBoxEnc pstInstanceProp[%d.%d.%d] byHitItemIdx[%d] !<= byItemNum[%d]!"), MSG_MARK, stCourageData.stSceneData.wSceneIdx, stCourageData.stSceneData.byTownIdx, stCourageData.stSceneData.byInstanceIdx, byHitItemIdx, itrGoup->second.stBoxItem.byItemNum);
            byHitItemIdx = 0; //避免byHitItemIdx值异常
        }

        stBoxGroupInfo.byHitItemIdx = byHitItemIdx;
        memcpy(&stBoxGroupInfo.stItemInfo, &stItemGroupProp.stBoxItem, sizeof(DT_ITEM_DATA_LIST_CLI));
        //if((0 != wItemKindID) && (0 != byHitItemIdx))
        //{
        //    BOOL bGive = FALSE;
        //    SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wItemKindID);
        //    if(pstGoodsProp)
        //    {
        //        switch(pstGoodsProp->byMainKindID)
        //        {
        //        case EGMK_COIN:
        //            {
        //                UINT32 dwValue = pstGoodsProp->dwValue * wPileNum;
        //                dwCoin += dwValue;
        //                stBoxGroupInfo.dwCoin = dwValue;
        //                bGive = TRUE;
        //            }
        //            break;
        //        case EGMK_GOLD:
        //            {
        //                UINT32 dwValue = pstGoodsProp->dwValue * wPileNum;
        //                dwGold += dwValue;
        //                stBoxGroupInfo.dwGold = dwValue;
        //                bGive = TRUE;
        //            }
        //            break;
        //        case EGMK_BLUEGAS:
        //            {
        //                UINT32 dwValue = pstGoodsProp->dwValue * wPileNum;
        //                dwBlueGas += dwValue;
        //                stBoxGroupInfo.dwBlueGas = dwBlueGas;
        //                bGive = TRUE;
        //            }
        //            break;
        //        }
        //    }

        //    if((FALSE == bGive) && (0 != wItemKindID) && (0 != byHitItemIdx))
        //    {
        //        mapHitItem[wItemKindID] += wPileNum;

        //        stBoxGroupInfo.stItemInfo.byItemNum = 1;
        //        DT_ITEM_DATA_CLI& stDT_ITEM_DATA_CLI = stBoxGroupInfo.stItemInfo.astItemList[0];
        //        stDT_ITEM_DATA_CLI.wKindID = wItemKindID;
        //        stDT_ITEM_DATA_CLI.wPileNum = wPileNum;
        //        if(pstGoodsProp)
        //        {
        //            stDT_ITEM_DATA_CLI.byGoodsMainKindID = pstGoodsProp->byMainKindID;
        //        }
        //    }
        //}
    }
    stDT_PASS_ENCOURAGE_BOX_DATA.byGroupNum = byGroupNum;
}

UINT16 CCourageMgr::RateInnerBox( UINT16 wKindID)
{
    //验证命中的是否是内部宝箱
    SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wKindID);
    if(NULL == pstGoodsProp)
    {
        return 0;
    }
    //为内部宝箱
    if(EGMK_INNERBOX != pstGoodsProp->byMainKindID)
    {
        return 0;
    }

    SItemComposeProp* pstItemComposeProp = CItemComposePropMgr::Instance()->GetProp(wKindID);
    if(NULL == pstItemComposeProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstItemComposeProp is NULL!"), MSG_MARK);
        return 0;;
    }

    CItemRateMap& mapComposeItem = pstItemComposeProp->mapComposeItem;
    if(0 == mapComposeItem.size())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: mapComposeItem[%d] is 0!"), MSG_MARK, wKindID);
        return 0;;
    }

    CRandomVec vecRandom;
    for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end(); itr++)
    {
        vecRandom.push_back(itr->second.wRate);
    }

    UINT16 wInnerKindID = 0;
    //随机
    INT32 nRandomHitIdx = GetRandomIdx(vecRandom, 10000);
    if(RANDOM_UNHIT == nRandomHitIdx)
    {
        return 0;
    }

    UINT8 byIdx = 0;
    UINT16 wNum = 1;
    for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end(); itr++, byIdx++)
    {
        if(byIdx == nRandomHitIdx)
        {
            wInnerKindID = itr->first;
            wNum = itr->second.wItemNum;
            break;
        }
    }

    if(0 == wInnerKindID)
    {
        return 0;
    }                            

    return wInnerKindID;
}

//GVE的奖励是从随机礼包中随机到一个随机礼包，在从随机礼包中随机到一个首饰  资源的增加暂时没加处理
UINT16 CCourageMgr::CreateItem( CPlayer* poPlayer )
{
    if (poPlayer == NULL || poPlayer->GetCourage() == NULL)
    {
        RETURN_OTHER_ERR;
    }

    DT_COURAGE_DATA &stCourageData = poPlayer->GetCourage()->GetDT_COURAGE_DATA();

    CBag& oBag = poPlayer->GetBag();

    if(oBag.GetIdleSize() < stCourageData.stjewelryInfoLst.wJewelryNum)
    {
        return ERR_COURAGE_GET_AWARD::ID_BAG_FULL;
    }

    //真正给奖励
    for(UINT16 wIndex = 0; wIndex < stCourageData.stjewelryInfoLst.wJewelryNum;wIndex++)
    {
        UINT16 wPileNum = 1;
        ECreateItemRet Ret = ECIR_SUCCESS;
        CItem* poItem = CItemMgr::Instance()->CreateJewelryIntoBag(poPlayer, stCourageData.stjewelryInfoLst.astJewelryInfo[wIndex], wPileNum, Ret, 0, CRecordMgr::EAIT_COURAGE);
        if (Ret != ECIR_SUCCESS)
        {
            USR_INFO(_SDT("PlayerID: [%d] [%s: %d]: CreateJewelryFailed[%d.%d.%d] wItemKindID[%d] "),poPlayer->GetID(), MSG_MARK, stCourageData.stjewelryInfoLst.astJewelryInfo[wIndex].wJewelryID);
        }
    }

    return  ERR_COURAGE_GET_AWARD::ID_SUCCESS;
}

UINT8 CCourageMgr::GetScore(UINT16 wLevel)
{   
    UINT8 byBackLevel = wLevel/10 > 5 ? wLevel/10 - 5 : 1;
    return byBackLevel > 5 ? 5 : byBackLevel;
}

VOID CCourageMgr::CreateJewelryData(CPlayer* poPlayer, UINT16 wJewelryID, DT_JEWELRY_DATA &st_DT_JEWELRY_DATA )
{
    memset(&st_DT_JEWELRY_DATA, 0x00, sizeof(DT_JEWELRY_DATA));
    if(! CJewelryPropMgr::Instance()->GenerateJewelryData(poPlayer, wJewelryID, st_DT_JEWELRY_DATA))
    {
        USR_INFO(_SDT("PlayerID: [%d] [%s: %d]: CreateJewelryFailed[%d.%d.%d] wItemKindID[%d] "),poPlayer->GetID(), MSG_MARK, wJewelryID);
    }
}

UINT8 CCourageMgr::GetHelpID( UINT32 dwHelpTimes, UINT32 dwGiveFreeTimes)
{
	if ( 1 == dwGiveFreeTimes )
	{
		return 3;//已领取过奖励
	}
    if (dwHelpTimes == 0)
    {
        return 1;
    }
    else if (dwHelpTimes == 1)
    {
        return 2;
    }
    else
        return 3;
}

VOID CCourageMgr::TransData( const DT_BATTLE_DATA2& stBataData2, DT_BATTLE_DATA3 &stBataData3 )
{
    memcpy(&stBataData3.stMyselfInfo,&stBataData2.stMyselfInfo,sizeof(stBataData2.stMyselfInfo));
    memcpy(&stBataData3.stMyselfInfoAfterBattle,&stBataData2.stMyselfInfoAfterBattle,sizeof(stBataData2.stMyselfInfoAfterBattle));
    memcpy(&stBataData3.stEnemyInfo,&stBataData2.stEnemyInfo,sizeof(stBataData2.stEnemyInfo));
    stBataData3.byBattleResult = stBataData2.byBattleResult;
    stBataData3.byBattleRoundNum = stBataData2.byBattleRoundNum;
    stBataData3.byScore = stBataData2.byScore;
    for (UINT8 byIndex =0 ; byIndex < stBataData2.byBattleRoundNum; byIndex++)
    {
        //memcpy(&stBataData3.astBattleRoundInfo[byIndex],&stBataData2.astBattleRoundInfo[byIndex],sizeof(DT_BATTLE_ROUND_DATA2));
        stBataData3.astBattleRoundInfo[byIndex].byAttackerFlag = stBataData2.astBattleRoundInfo[byIndex].byAttackerFlag;
        stBataData3.astBattleRoundInfo[byIndex].stAttackData.byAttackerIdx = stBataData2.astBattleRoundInfo[byIndex].stAttackData.byAttackerIdx;
        stBataData3.astBattleRoundInfo[byIndex].stAttackData.wAttackSkillID = stBataData2.astBattleRoundInfo[byIndex].stAttackData.wAttackSkillID;
        stBataData3.astBattleRoundInfo[byIndex].stAttackData.bySkipFlag = stBataData2.astBattleRoundInfo[byIndex].stAttackData.bySkipFlag;
        stBataData3.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum = stBataData2.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum;

        memcpy(&stBataData3.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo,&stBataData2.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo,
            sizeof(stBataData2.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo));
        memcpy(&stBataData3.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo,&stBataData2.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo,
            sizeof(stBataData2.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo));


        DT_ATTACK_DATA2 &stDtData3 = stBataData3.astBattleRoundInfo[byIndex].stAttackData.stBeforeAttack;
        const DT_ATTACK_DATA &stDtData2 = stBataData2.astBattleRoundInfo[byIndex].stAttackData.stBeforeAttack;


        stDtData3.stEnemyObjInfo.byBuffChangeNum = stDtData2.stEnemyObjInfo.byBuffChangeNum;
        for (UINT8 byBuffIndex = 0; byBuffIndex < stDtData2.stEnemyObjInfo.byBuffChangeNum; byBuffIndex++)
        {
            memcpy(&stDtData3.stEnemyObjInfo.astBuffChangeInfo[byBuffIndex],&stDtData2.stEnemyObjInfo.astBuffChangeInfo[byBuffIndex],sizeof(stDtData2.stEnemyObjInfo.astBuffChangeInfo[byBuffIndex]));
        }
        stDtData3.stEnemyObjInfo.byHpChangeNum = stDtData2.stEnemyObjInfo.byHpChangeNum;
        for (UINT8 byHpChangeIndex = 0; byHpChangeIndex < stDtData2.stEnemyObjInfo.byHpChangeNum; byHpChangeIndex++)
        {
            memcpy(&stDtData3.stEnemyObjInfo.astHpChangeInfo[byHpChangeIndex],&stDtData2.stEnemyObjInfo.astHpChangeInfo[byHpChangeIndex],
                sizeof(stDtData2.stEnemyObjInfo.astHpChangeInfo[byHpChangeIndex]));
        }
        stDtData3.stEnemyObjInfo.byAngryChangeNum = stDtData2.stEnemyObjInfo.byAngryChangeNum;
        for (UINT8 byAngryChangeIndex = 0; byAngryChangeIndex < stDtData2.stEnemyObjInfo.byAngryChangeNum; byAngryChangeIndex++)
        {
            memcpy(&stDtData3.stEnemyObjInfo.astAngryChangeInfo[byAngryChangeIndex],&stDtData2.stEnemyObjInfo.astAngryChangeInfo[byAngryChangeIndex],
                sizeof(stDtData2.stEnemyObjInfo.astAngryChangeInfo[byAngryChangeIndex]));
        }
        stDtData3.stEnemyObjInfo.bySkillEffectIDChangeNum = stDtData2.stEnemyObjInfo.bySkillEffectIDChangeNum;
        for (UINT8 bySkillIndex = 0; bySkillIndex < stDtData2.stEnemyObjInfo.bySkillEffectIDChangeNum; bySkillIndex++)
        {
            memcpy(&stDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],&stDtData2.stEnemyObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],
                sizeof(stDtData2.stEnemyObjInfo.astSkillEffectIDChangeInfo[bySkillIndex]));
        }


        stDtData3.stMyselfObjInfo.byBuffChangeNum = stDtData2.stMyselfObjInfo.byBuffChangeNum;
        for (UINT8 byBuffIndex = 0; byBuffIndex < stDtData2.stMyselfObjInfo.byBuffChangeNum; byBuffIndex++)
        {
            memcpy(&stDtData3.stMyselfObjInfo.astBuffChangeInfo[byBuffIndex],&stDtData2.stMyselfObjInfo.astBuffChangeInfo[byBuffIndex],sizeof(stDtData2.stMyselfObjInfo.astBuffChangeInfo[byBuffIndex]));
        }
        stDtData3.stMyselfObjInfo.byHpChangeNum = stDtData2.stMyselfObjInfo.byHpChangeNum;
        for (UINT8 byHpChangeIndex = 0; byHpChangeIndex < stDtData2.stMyselfObjInfo.byHpChangeNum; byHpChangeIndex++)
        {
            memcpy(&stDtData3.stMyselfObjInfo.astHpChangeInfo[byHpChangeIndex],&stDtData2.stMyselfObjInfo.astHpChangeInfo[byHpChangeIndex],
                sizeof(stDtData2.stMyselfObjInfo.astHpChangeInfo[byHpChangeIndex]));
        }
        stDtData3.stMyselfObjInfo.byAngryChangeNum = stDtData2.stMyselfObjInfo.byAngryChangeNum;
        for (UINT8 byAngryChangeIndex = 0; byAngryChangeIndex < stDtData2.stMyselfObjInfo.byAngryChangeNum; byAngryChangeIndex++)
        {
            memcpy(&stDtData3.stMyselfObjInfo.astAngryChangeInfo[byAngryChangeIndex],&stDtData2.stMyselfObjInfo.astAngryChangeInfo[byAngryChangeIndex],
                sizeof(stDtData2.stMyselfObjInfo.astAngryChangeInfo[byAngryChangeIndex]));
        }
        stDtData3.stMyselfObjInfo.bySkillEffectIDChangeNum = stDtData2.stMyselfObjInfo.bySkillEffectIDChangeNum;
        for (UINT8 bySkillIndex = 0; bySkillIndex < stDtData2.stMyselfObjInfo.bySkillEffectIDChangeNum; bySkillIndex++)
        {
            memcpy(&stDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],&stDtData2.stMyselfObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],
                sizeof(stDtData2.stMyselfObjInfo.astSkillEffectIDChangeInfo[bySkillIndex]));
        }

        DT_ATTACK_DATA2 &stDtDataA3 = stBataData3.astBattleRoundInfo[byIndex].stAttackData.stAfterAttack;
        const DT_ATTACK_DATA &stDtDataA2 = stBataData2.astBattleRoundInfo[byIndex].stAttackData.stAfterAttack;


        stDtDataA3.stEnemyObjInfo.byBuffChangeNum = stDtDataA2.stEnemyObjInfo.byBuffChangeNum;
        for (UINT8 byBuffIndex = 0; byBuffIndex < stDtDataA2.stEnemyObjInfo.byBuffChangeNum; byBuffIndex++)
        {
            memcpy(&stDtDataA3.stEnemyObjInfo.astBuffChangeInfo[byBuffIndex],&stDtDataA2.stEnemyObjInfo.astBuffChangeInfo[byBuffIndex],sizeof(stDtDataA2.stEnemyObjInfo.astBuffChangeInfo[byBuffIndex]));
        }
        stDtDataA3.stEnemyObjInfo.byHpChangeNum = stDtDataA2.stEnemyObjInfo.byHpChangeNum;
        for (UINT8 byHpChangeIndex = 0; byHpChangeIndex < stDtDataA2.stEnemyObjInfo.byHpChangeNum; byHpChangeIndex++)
        {
            memcpy(&stDtDataA3.stEnemyObjInfo.astHpChangeInfo[byHpChangeIndex],&stDtDataA2.stEnemyObjInfo.astHpChangeInfo[byHpChangeIndex],
                sizeof(stDtDataA2.stEnemyObjInfo.astHpChangeInfo[byHpChangeIndex]));
        }
        stDtDataA3.stEnemyObjInfo.byAngryChangeNum = stDtDataA2.stEnemyObjInfo.byAngryChangeNum;
        for (UINT8 byAngryChangeIndex = 0; byAngryChangeIndex < stDtDataA2.stEnemyObjInfo.byAngryChangeNum; byAngryChangeIndex++)
        {
            memcpy(&stDtDataA3.stEnemyObjInfo.astAngryChangeInfo[byAngryChangeIndex],&stDtDataA2.stEnemyObjInfo.astAngryChangeInfo[byAngryChangeIndex],
                sizeof(stDtDataA2.stEnemyObjInfo.astAngryChangeInfo[byAngryChangeIndex]));
        }
        stDtDataA3.stEnemyObjInfo.bySkillEffectIDChangeNum = stDtDataA2.stEnemyObjInfo.bySkillEffectIDChangeNum;
        for (UINT8 bySkillIndex = 0; bySkillIndex < stDtDataA2.stEnemyObjInfo.bySkillEffectIDChangeNum; bySkillIndex++)
        {
            memcpy(&stDtDataA3.stEnemyObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],&stDtDataA2.stEnemyObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],
                sizeof(stDtDataA2.stEnemyObjInfo.astSkillEffectIDChangeInfo[bySkillIndex]));
        }


        stDtDataA3.stMyselfObjInfo.byBuffChangeNum = stDtDataA2.stMyselfObjInfo.byBuffChangeNum;
        for (UINT8 byBuffIndex = 0; byBuffIndex < stDtDataA2.stMyselfObjInfo.byBuffChangeNum; byBuffIndex++)
        {
            memcpy(&stDtDataA3.stMyselfObjInfo.astBuffChangeInfo[byBuffIndex],&stDtDataA2.stMyselfObjInfo.astBuffChangeInfo[byBuffIndex],sizeof(stDtDataA2.stMyselfObjInfo.astBuffChangeInfo[byBuffIndex]));
        }
        stDtDataA3.stMyselfObjInfo.byHpChangeNum = stDtDataA2.stMyselfObjInfo.byHpChangeNum;
        for (UINT8 byHpChangeIndex = 0; byHpChangeIndex < stDtDataA2.stMyselfObjInfo.byHpChangeNum; byHpChangeIndex++)
        {
            memcpy(&stDtDataA3.stMyselfObjInfo.astHpChangeInfo[byHpChangeIndex],&stDtDataA2.stMyselfObjInfo.astHpChangeInfo[byHpChangeIndex],
                sizeof(stDtDataA2.stMyselfObjInfo.astHpChangeInfo[byHpChangeIndex]));
        }
        stDtDataA3.stMyselfObjInfo.byAngryChangeNum = stDtDataA2.stMyselfObjInfo.byAngryChangeNum;
        for (UINT8 byAngryChangeIndex = 0; byAngryChangeIndex < stDtDataA2.stMyselfObjInfo.byAngryChangeNum; byAngryChangeIndex++)
        {
            memcpy(&stDtDataA3.stMyselfObjInfo.astAngryChangeInfo[byAngryChangeIndex],&stDtDataA2.stMyselfObjInfo.astAngryChangeInfo[byAngryChangeIndex],
                sizeof(stDtDataA2.stMyselfObjInfo.astAngryChangeInfo[byAngryChangeIndex]));
        }
        stDtDataA3.stMyselfObjInfo.bySkillEffectIDChangeNum = stDtDataA2.stMyselfObjInfo.bySkillEffectIDChangeNum;
        for (UINT8 bySkillIndex = 0; bySkillIndex < stDtDataA2.stMyselfObjInfo.bySkillEffectIDChangeNum; bySkillIndex++)
        {
            memcpy(&stDtDataA3.stMyselfObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],&stDtDataA2.stMyselfObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],
                sizeof(stDtDataA2.stMyselfObjInfo.astSkillEffectIDChangeInfo[bySkillIndex]));
        }
    }
}

VOID CCourageMgr::TransData2( const DT_BATTLE_DATA3& stBataData3, DT_BATTLE_DATA2 &stBataData2 )
{
    memcpy(&stBataData2.stMyselfInfo,&stBataData3.stMyselfInfo,sizeof(stBataData2.stMyselfInfo));
    memcpy(&stBataData2.stMyselfInfoAfterBattle,&stBataData3.stMyselfInfoAfterBattle,sizeof(stBataData2.stMyselfInfoAfterBattle));
    memcpy(&stBataData2.stEnemyInfo,&stBataData3.stEnemyInfo,sizeof(stBataData2.stEnemyInfo));
    stBataData2.byBattleResult = stBataData3.byBattleResult;
    stBataData2.byBattleRoundNum = stBataData3.byBattleRoundNum > MAX_BATTLE_ROUND_NUM ? MAX_BATTLE_ROUND_NUM :  stBataData3.byBattleRoundNum;
    stBataData2.byScore = stBataData3.byScore;
    for (UINT8 byIndex =0 ; byIndex < stBataData3.byBattleRoundNum && byIndex < MAX_BATTLE_ROUND_NUM; byIndex++)
    {
        //memcpy(&stBataData3.astBattleRoundInfo[byIndex],&stBataData2.astBattleRoundInfo[byIndex],sizeof(DT_BATTLE_ROUND_DATA2));
        stBataData2.astBattleRoundInfo[byIndex].byAttackerFlag = stBataData3.astBattleRoundInfo[byIndex].byAttackerFlag;
        stBataData2.astBattleRoundInfo[byIndex].stAttackData.byAttackerIdx = stBataData3.astBattleRoundInfo[byIndex].stAttackData.byAttackerIdx;
        stBataData2.astBattleRoundInfo[byIndex].stAttackData.wAttackSkillID = stBataData3.astBattleRoundInfo[byIndex].stAttackData.wAttackSkillID;
        stBataData2.astBattleRoundInfo[byIndex].stAttackData.bySkipFlag = stBataData3.astBattleRoundInfo[byIndex].stAttackData.bySkipFlag;
        stBataData2.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum = stBataData3.astBattleRoundInfo[byIndex].stAttackData.byBeAttackerIdxNum;

        memcpy(&stBataData2.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo,&stBataData3.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo,
            sizeof(stBataData2.astBattleRoundInfo[byIndex].stAttackData.stGodweaponAttackInfo));
        memcpy(&stBataData2.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo,&stBataData3.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo,
            sizeof(stBataData2.astBattleRoundInfo[byIndex].stAttackData.astBeAttackerIdxInfo));


        DT_ATTACK_DATA &stDtData3 = stBataData2.astBattleRoundInfo[byIndex].stAttackData.stBeforeAttack;
        const DT_ATTACK_DATA2 &stDtData2 = stBataData3.astBattleRoundInfo[byIndex].stAttackData.stBeforeAttack;


        stDtData3.stEnemyObjInfo.byBuffChangeNum = stDtData2.stEnemyObjInfo.byBuffChangeNum;
        for (UINT8 byBuffIndex = 0; byBuffIndex < stDtData2.stEnemyObjInfo.byBuffChangeNum; byBuffIndex++)
        {
            memcpy(&stDtData3.stEnemyObjInfo.astBuffChangeInfo[byBuffIndex],&stDtData2.stEnemyObjInfo.astBuffChangeInfo[byBuffIndex],sizeof(stDtData3.stEnemyObjInfo.astBuffChangeInfo[byBuffIndex]));
        }
        stDtData3.stEnemyObjInfo.byHpChangeNum = stDtData2.stEnemyObjInfo.byHpChangeNum;
        for (UINT8 byHpChangeIndex = 0; byHpChangeIndex < stDtData2.stEnemyObjInfo.byHpChangeNum; byHpChangeIndex++)
        {
            memcpy(&stDtData3.stEnemyObjInfo.astHpChangeInfo[byHpChangeIndex],&stDtData2.stEnemyObjInfo.astHpChangeInfo[byHpChangeIndex],
                sizeof(stDtData3.stEnemyObjInfo.astHpChangeInfo[byHpChangeIndex]));
        }
        stDtData3.stEnemyObjInfo.byAngryChangeNum = stDtData2.stEnemyObjInfo.byAngryChangeNum;
        for (UINT8 byAngryChangeIndex = 0; byAngryChangeIndex < stDtData2.stEnemyObjInfo.byAngryChangeNum; byAngryChangeIndex++)
        {
            memcpy(&stDtData3.stEnemyObjInfo.astAngryChangeInfo[byAngryChangeIndex],&stDtData2.stEnemyObjInfo.astAngryChangeInfo[byAngryChangeIndex],
                sizeof(stDtData3.stEnemyObjInfo.astAngryChangeInfo[byAngryChangeIndex]));
        }
        stDtData3.stEnemyObjInfo.bySkillEffectIDChangeNum = stDtData2.stEnemyObjInfo.bySkillEffectIDChangeNum;
        for (UINT8 bySkillIndex = 0; bySkillIndex < stDtData2.stEnemyObjInfo.bySkillEffectIDChangeNum; bySkillIndex++)
        {
            memcpy(&stDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],&stDtData2.stEnemyObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],
                sizeof(stDtData3.stEnemyObjInfo.astSkillEffectIDChangeInfo[bySkillIndex]));
        }


        stDtData3.stMyselfObjInfo.byBuffChangeNum = stDtData2.stMyselfObjInfo.byBuffChangeNum;
        for (UINT8 byBuffIndex = 0; byBuffIndex < stDtData2.stMyselfObjInfo.byBuffChangeNum; byBuffIndex++)
        {
            memcpy(&stDtData3.stMyselfObjInfo.astBuffChangeInfo[byBuffIndex],&stDtData2.stMyselfObjInfo.astBuffChangeInfo[byBuffIndex],sizeof(stDtData3.stMyselfObjInfo.astBuffChangeInfo[byBuffIndex]));
        }
        stDtData3.stMyselfObjInfo.byHpChangeNum = stDtData2.stMyselfObjInfo.byHpChangeNum;
        for (UINT8 byHpChangeIndex = 0; byHpChangeIndex < stDtData2.stMyselfObjInfo.byHpChangeNum; byHpChangeIndex++)
        {
            memcpy(&stDtData3.stMyselfObjInfo.astHpChangeInfo[byHpChangeIndex],&stDtData2.stMyselfObjInfo.astHpChangeInfo[byHpChangeIndex],
                sizeof(stDtData3.stMyselfObjInfo.astHpChangeInfo[byHpChangeIndex]));
        }
        stDtData3.stMyselfObjInfo.byAngryChangeNum = stDtData2.stMyselfObjInfo.byAngryChangeNum;
        for (UINT8 byAngryChangeIndex = 0; byAngryChangeIndex < stDtData2.stMyselfObjInfo.byAngryChangeNum; byAngryChangeIndex++)
        {
            memcpy(&stDtData3.stMyselfObjInfo.astAngryChangeInfo[byAngryChangeIndex],&stDtData2.stMyselfObjInfo.astAngryChangeInfo[byAngryChangeIndex],
                sizeof(stDtData3.stMyselfObjInfo.astAngryChangeInfo[byAngryChangeIndex]));
        }
        stDtData3.stMyselfObjInfo.bySkillEffectIDChangeNum = stDtData2.stMyselfObjInfo.bySkillEffectIDChangeNum;
        for (UINT8 bySkillIndex = 0; bySkillIndex < stDtData2.stMyselfObjInfo.bySkillEffectIDChangeNum; bySkillIndex++)
        {
            memcpy(&stDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],&stDtData2.stMyselfObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],
                sizeof(stDtData3.stMyselfObjInfo.astSkillEffectIDChangeInfo[bySkillIndex]));
        }

        DT_ATTACK_DATA &stDtDataA3 = stBataData2.astBattleRoundInfo[byIndex].stAttackData.stAfterAttack;
        const DT_ATTACK_DATA2 &stDtDataA2 = stBataData3.astBattleRoundInfo[byIndex].stAttackData.stAfterAttack;


        stDtDataA3.stEnemyObjInfo.byBuffChangeNum = stDtDataA2.stEnemyObjInfo.byBuffChangeNum;
        for (UINT8 byBuffIndex = 0; byBuffIndex < stDtDataA2.stEnemyObjInfo.byBuffChangeNum; byBuffIndex++)
        {
            memcpy(&stDtDataA3.stEnemyObjInfo.astBuffChangeInfo[byBuffIndex],&stDtDataA2.stEnemyObjInfo.astBuffChangeInfo[byBuffIndex],sizeof(stDtDataA3.stEnemyObjInfo.astBuffChangeInfo[byBuffIndex]));
        }
        stDtDataA3.stEnemyObjInfo.byHpChangeNum = stDtDataA2.stEnemyObjInfo.byHpChangeNum;
        for (UINT8 byHpChangeIndex = 0; byHpChangeIndex < stDtDataA2.stEnemyObjInfo.byHpChangeNum; byHpChangeIndex++)
        {
            memcpy(&stDtDataA3.stEnemyObjInfo.astHpChangeInfo[byHpChangeIndex],&stDtDataA2.stEnemyObjInfo.astHpChangeInfo[byHpChangeIndex],
                sizeof(stDtDataA3.stEnemyObjInfo.astHpChangeInfo[byHpChangeIndex]));
        }
        stDtDataA3.stEnemyObjInfo.byAngryChangeNum = stDtDataA2.stEnemyObjInfo.byAngryChangeNum;
        for (UINT8 byAngryChangeIndex = 0; byAngryChangeIndex < stDtDataA2.stEnemyObjInfo.byAngryChangeNum; byAngryChangeIndex++)
        {
            memcpy(&stDtDataA3.stEnemyObjInfo.astAngryChangeInfo[byAngryChangeIndex],&stDtDataA2.stEnemyObjInfo.astAngryChangeInfo[byAngryChangeIndex],
                sizeof(stDtDataA3.stEnemyObjInfo.astAngryChangeInfo[byAngryChangeIndex]));
        }
        stDtDataA3.stEnemyObjInfo.bySkillEffectIDChangeNum = stDtDataA2.stEnemyObjInfo.bySkillEffectIDChangeNum;
        for (UINT8 bySkillIndex = 0; bySkillIndex < stDtDataA2.stEnemyObjInfo.bySkillEffectIDChangeNum; bySkillIndex++)
        {
            memcpy(&stDtDataA3.stEnemyObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],&stDtDataA2.stEnemyObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],
                sizeof(stDtDataA3.stEnemyObjInfo.astSkillEffectIDChangeInfo[bySkillIndex]));
        }


        stDtDataA3.stMyselfObjInfo.byBuffChangeNum = stDtDataA2.stMyselfObjInfo.byBuffChangeNum;
        for (UINT8 byBuffIndex = 0; byBuffIndex < stDtDataA2.stMyselfObjInfo.byBuffChangeNum; byBuffIndex++)
        {
            memcpy(&stDtDataA3.stMyselfObjInfo.astBuffChangeInfo[byBuffIndex],&stDtDataA2.stMyselfObjInfo.astBuffChangeInfo[byBuffIndex],sizeof(stDtDataA3.stMyselfObjInfo.astBuffChangeInfo[byBuffIndex]));
        }
        stDtDataA3.stMyselfObjInfo.byHpChangeNum = stDtDataA2.stMyselfObjInfo.byHpChangeNum;
        for (UINT8 byHpChangeIndex = 0; byHpChangeIndex < stDtDataA2.stMyselfObjInfo.byHpChangeNum; byHpChangeIndex++)
        {
            memcpy(&stDtDataA3.stMyselfObjInfo.astHpChangeInfo[byHpChangeIndex],&stDtDataA2.stMyselfObjInfo.astHpChangeInfo[byHpChangeIndex],
                sizeof(stDtDataA3.stMyselfObjInfo.astHpChangeInfo[byHpChangeIndex]));
        }
        stDtDataA3.stMyselfObjInfo.byAngryChangeNum = stDtDataA2.stMyselfObjInfo.byAngryChangeNum;
        for (UINT8 byAngryChangeIndex = 0; byAngryChangeIndex < stDtDataA2.stMyselfObjInfo.byAngryChangeNum; byAngryChangeIndex++)
        {
            memcpy(&stDtDataA3.stMyselfObjInfo.astAngryChangeInfo[byAngryChangeIndex],&stDtDataA2.stMyselfObjInfo.astAngryChangeInfo[byAngryChangeIndex],
                sizeof(stDtDataA3.stMyselfObjInfo.astAngryChangeInfo[byAngryChangeIndex]));
        }
        stDtDataA3.stMyselfObjInfo.bySkillEffectIDChangeNum = stDtDataA2.stMyselfObjInfo.bySkillEffectIDChangeNum;
        for (UINT8 bySkillIndex = 0; bySkillIndex < stDtDataA2.stMyselfObjInfo.bySkillEffectIDChangeNum; bySkillIndex++)
        {
            memcpy(&stDtDataA3.stMyselfObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],&stDtDataA2.stMyselfObjInfo.astSkillEffectIDChangeInfo[bySkillIndex],
                sizeof(stDtDataA3.stMyselfObjInfo.astSkillEffectIDChangeInfo[bySkillIndex]));
        }
    }
}

UINT8 CCourageMgr::GetInGroupIndex( UINT32 dwPlayerID, UINT32 dwGroupID )
{
    //验证队伍ID是否存在，是否满足条件
    CGroupID2CourageGroupMapItr itr = m_mapGroupID2CourageGroup.find(dwGroupID);
    if (itr == m_mapGroupID2CourageGroup.end())
    {
        //队伍不存在
        return INVALID_GROUP_INDEX;
    }
    for (UINT8 byIndex = 0; byIndex < MAX_COURAGE_MEMBER_NUM; byIndex++)
    {
        if (itr->second->GetDT_COURAGE_GROUP_DATA().astPlayerData[byIndex].dwPlayerID != dwPlayerID)
        {
            continue;
        }
        return byIndex;
    }
    return INVALID_GROUP_INDEX;
}