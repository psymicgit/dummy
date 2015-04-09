#include "factionmgr.h"

#include <logic/player/playermgr.h>
#include <logic/monster/monster.h>
#include <logic/battle/battlemgr.h>
#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include <logic/event/timereventmgr.h>
#include <logic/monster/monstermgr.h>
#include <logic/lua/luamgr.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <net/db/dbpktbuilder.h>
#include <logic/record/recordmgr.h>
#include <logic/item/itemmgr.h>
#include <algorithm>
#include <functional>
#include <net/cli/usermgr.h>
#include <logic/base/basedefine.h>
#include <logic/activityencourage/activityencouragemgr.h>
#include <db/dbcommoperate.h>
#include "factionprop.h"
#include <logic/other/errmsgmgr.h>
#include <logic/faction/faction.h>
#include <errdef.h>
#include <logic/player/player.h>
#include <logic/other/locale.h>
#include <logic/chat/chatmgr.h>
#include "logic/chat/wordfilter.h"
#include "logic/other/singleprammgr.h"
#include "logic/gvg/local/gvgbuild.h"

using namespace std;
#define FACTION_SAVE_TIME	10


class CFactionSort
{
public:
	BOOL operator()(UINT32 dwFactionID1, UINT32 dwFactionID2)
	{
		CFaction* poFaction1 = CFactionMgr::Instance()->GetFaction(dwFactionID1);
		CFaction* poFaction2 = CFactionMgr::Instance()->GetFaction(dwFactionID2);
		if (NULL == poFaction1)
		{
			return FALSE;
		}
		if (NULL == poFaction2)
		{
			return TRUE;
		}
		if (poFaction1->GetDT_FACTION_BASE_DATA().dwFactionLevel < poFaction2->GetDT_FACTION_BASE_DATA().dwFactionLevel)
		{
			return TRUE;
		}
		else if (poFaction1->GetDT_FACTION_BASE_DATA().dwFactionLevel == poFaction2->GetDT_FACTION_BASE_DATA().dwFactionLevel)
		{
			if (poFaction1->GetDT_FACTION_BASE_DATA().qwFactionFundsSum < poFaction2->GetDT_FACTION_BASE_DATA().qwFactionFundsSum)
			{
				return TRUE;
			}
			else if (poFaction1->GetDT_FACTION_BASE_DATA().qwFactionFundsSum == poFaction2->GetDT_FACTION_BASE_DATA().qwFactionFundsSum)
			{
				UINT16 wPlayerNum1 = 0;
				UINT16 wPlayerNum2 = 0;
				UINT16 wMax = 0;
				poFaction1->GetFactionPlayerNum(wPlayerNum1, wMax);
				poFaction2->GetFactionPlayerNum(wPlayerNum2, wMax);
				if (wPlayerNum1 < wPlayerNum2)
				{
					return TRUE;
				}
				else if (wPlayerNum1 == wPlayerNum2)
				{
					if (poFaction1->GetDT_FACTION_BASE_DATA().qwFactionCreateTime < poFaction2->GetDT_FACTION_BASE_DATA().qwFactionCreateTime)
					{
						return TRUE;
					}
				}
			}
		}
		return FALSE;
	}
};

//自定义排序函数
bool SortByRank(CFaction* v1,  CFaction* v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    if (v1->GetDT_FACTION_BASE_DATA().dwFactionLevel > v2->GetDT_FACTION_BASE_DATA().dwFactionLevel)
    {
        return TRUE;
    }
	else if ( v1->GetDT_FACTION_BASE_DATA().dwFactionLevel == v2->GetDT_FACTION_BASE_DATA().dwFactionLevel )
	{
		if (v1->GetDT_FACTION_BASE_DATA().qwFactionFundsSum > v2->GetDT_FACTION_BASE_DATA().qwFactionFundsSum)
		{
			return TRUE;
		}
	}
    return FALSE;
}



//////////////////////////////////////////////////////////////////////////

IMPLEMENT_SINGLETON(CFactionMgr)

CFactionMgr::CFactionMgr():
    m_dwMaxFactionRank(0)
{
    m_qwSaveTime = SDTimeSecs();
}

CFactionMgr::~CFactionMgr()
{

}


BOOL CFactionMgr::Init()
{
	if (FALSE == CFactorPropMgr::CreateInstance())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: InitDataFromDB failed!"), MSG_MARK);
		return FALSE;
	}

	if (FALSE == CFactorPropMgr::Instance()->Init())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: Instance failed!"), MSG_MARK);
		return FALSE;
	}
	else
	{
		SYS_CRITICAL(_SDT("[%s: %d]: Instance Ok!"), MSG_MARK);
	}

	if (FALSE == CFactorAuthorityPropMgr::CreateInstance())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: InitDataFromDB failed!"), MSG_MARK);
		return FALSE;
	}

	if (FALSE == CFactorAuthorityPropMgr::Instance()->Init())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: Instance failed!"), MSG_MARK);
		return FALSE;
	}
	else
	{
		SYS_CRITICAL(_SDT("[%s: %d]: Instance Ok!"), MSG_MARK);
	}

    if(!InitDataFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitDataFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

BOOL CFactionMgr::InitDataFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }


    //门派排名规则：
    //1、不同等级的，等级高排在前
    //2、同等级的，门派累计资金高的排在前
    //3、同等级、门派累计资金相同的，先创建的排在前

    CHAR szSql[1024] = {0};
    sprintf(szSql, "select * from faction order by FactionLevel, FactionFundsSum, FactionCreateTime;");


    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while (pRecordSet->GetRecord())
        {
            static DT_FACTION_DATA		stDT_FACTION_DATA = {0};
            memset(&stDT_FACTION_DATA, 0, sizeof(DT_FACTION_DATA));

            UINT32 dwLen = 0;
            stDT_FACTION_DATA.stFactionBaseData.dwFactionID = SGDP::SDAtou(pRecordSet->GetFieldValueByName("FactionID"));

            dwLen = pRecordSet->GetFieldLengthByName("FactionName");
            dwLen = dwLen >= MAX_FACTION_NUM ? MAX_FACTION_NUM - 1 : dwLen;
            SDStrncpy( stDT_FACTION_DATA.stFactionBaseData.aszFactionName, pRecordSet->GetFieldValueByName("FactionName"), dwLen);

            stDT_FACTION_DATA.stFactionBaseData.dwFactionPlayerID = SGDP::SDAtou(pRecordSet->GetFieldValueByName("FactionPlayerID"));

            stDT_FACTION_DATA.stFactionBaseData.dwFactionRank = SGDP::SDAtou(pRecordSet->GetFieldValueByName("FactionRank"));

            stDT_FACTION_DATA.stFactionBaseData.dwFactionLevel = SGDP::SDAtou(pRecordSet->GetFieldValueByName("FactionLevel"));

            stDT_FACTION_DATA.stFactionBaseData.dwRareItemRoomGrade = SGDP::SDAtou(pRecordSet->GetFieldValueByName("RareItemRoomLevel"));

            stDT_FACTION_DATA.stFactionBaseData.dwGymnasiumLevel = SGDP::SDAtou(pRecordSet->GetFieldValueByName("GymnasiumLevel"));

            stDT_FACTION_DATA.stFactionBaseData.dwShadeLevel = SGDP::SDAtou(pRecordSet->GetFieldValueByName("ShadeLevel"));

            stDT_FACTION_DATA.stFactionBaseData.dwHotSpringLevel = SGDP::SDAtou(pRecordSet->GetFieldValueByName("HotSpringLevel"));

            stDT_FACTION_DATA.stFactionBaseData.dwFactionFunds = SGDP::SDAtou(pRecordSet->GetFieldValueByName("FactionFunds"));

            stDT_FACTION_DATA.stFactionBaseData.dwColdPrisonLevel = SGDP::SDAtou(pRecordSet->GetFieldValueByName("ColdPrisonLevel"));

            stDT_FACTION_DATA.stFactionBaseData.dwFactionTowerLevel = SGDP::SDAtou(pRecordSet->GetFieldValueByName("FactionTowerLevel"));

            stDT_FACTION_DATA.stFactionBaseData.qwFactionFundsSum = SGDP::SDAtou64(pRecordSet->GetFieldValueByName("FactionFundsSum"));

			stDT_FACTION_DATA.byTodayPassNum = SGDP::SDAtou(pRecordSet->GetFieldValueByName("TodayPassNum"));

			GetDateTime2Stamp(stDT_FACTION_DATA.qwLastAuditTime, "LastAuditTime");

			stDT_FACTION_DATA.byExt = SGDP::SDAtou(pRecordSet->GetFieldValueByName("Ext"));
            dwLen = pRecordSet->GetFieldLengthByName("FactionNotice");
            dwLen = dwLen >= MAX_FACTION_DESC_NUM ? MAX_FACTION_DESC_NUM - 1 : dwLen;
            SDStrncpy( stDT_FACTION_DATA.stFactionBaseData.aszFactionNotice, pRecordSet->GetFieldValueByName("FactionNotice"), dwLen);

            dwLen = pRecordSet->GetFieldLengthByName("FactionAnnouncement");
            dwLen = dwLen >= MAX_FACTION_DESC_NUM ? MAX_FACTION_DESC_NUM - 1 : dwLen;
            SDStrncpy( stDT_FACTION_DATA.stFactionBaseData.aszFactionAnnouncement, pRecordSet->GetFieldValueByName("FactionAnnouncement"), dwLen);

            dwLen = pRecordSet->GetFieldLengthByName("FactionLogDataLst");
            if (0 != dwLen)
            {
                if( -1 == stDT_FACTION_DATA.stFactionLogDataLst.DecodeSaveData(pRecordSet->GetFieldValueByName("FactionLogDataLst"), dwLen ) )
                {
                    SYS_CRITICAL( _SDT("[%s: %d] stDT_FACTION_DATA.DecodeSaveData Failed!"), MSG_MARK);
                    //return FALSE;
                }
            }

            dwLen = pRecordSet->GetFieldLengthByName("FactionTaskSchedule");
            if (0 != dwLen)
            {
                if( -1 == stDT_FACTION_DATA.stFactionCommonData.DecodeSaveData(pRecordSet->GetFieldValueByName("FactionTaskSchedule"), dwLen ) )
                {
                    SYS_CRITICAL( _SDT("[%s: %d] stDT_FACTION_DATA.DecodeSaveData Failed!"), MSG_MARK);
                    //return FALSE;
                }
            }

            GetDateTime2Stamp(stDT_FACTION_DATA.stFactionBaseData.qwFactionCreateTime,  "FactionCreateTime");

            dwLen = pRecordSet->GetFieldLengthByName("GvGFormation");
            if (0 != dwLen)
            {
                if( -1 == stDT_FACTION_DATA.stGvGData.stGvGFormation.DecodeSaveData(pRecordSet->GetFieldValueByName("GvGFormation"), dwLen ) )
                {
                    SYS_CRITICAL( _SDT("[%s: %d] stDT_FACTION_DATA.DecodeSaveData Failed!"), MSG_MARK);
                    //return FALSE;
                }
            }

            dwLen = pRecordSet->GetFieldLengthByName("GvGLog");
            if (0 != dwLen)
            {
                if( -1 == stDT_FACTION_DATA.stGvGData.stGvGLogList.DecodeSaveData(pRecordSet->GetFieldValueByName("GvGLog"), dwLen ) )
                {
                    SYS_CRITICAL( _SDT("[%s: %d] stDT_FACTION_DATA.DecodeSaveData Failed!"), MSG_MARK);
                    //return FALSE;
                }
            }

			 stDT_FACTION_DATA.stGvGData.qwGVGFactionFunds = SGDP::SDAtou64(pRecordSet->GetFieldValueByName("GVGFactionFunds"));
			 stDT_FACTION_DATA.stGvGData.qwSumPower = SGDP::SDAtou64(pRecordSet->GetFieldValueByName("SumPower"));
            //门派排名
            stDT_FACTION_DATA.stFactionBaseData.dwFactionRank = ++m_dwMaxFactionRank;

            CFaction	*poFaction = m_oFactionShmemObjMgr.Alloc();
            if (NULL == poFaction)
            {
                continue;
            }
            poFaction->Init(stDT_FACTION_DATA);
            m_mapFaction[stDT_FACTION_DATA.stFactionBaseData.dwFactionID] = poFaction;
            m_listRankFaction.push_back(poFaction);
            m_mapFactionName[poFaction->GetDT_FACTION_BASE_DATA().aszFactionName] = poFaction;
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    sprintf(szSql, "select * from factionplayer order by FactionID;");
    nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );
    if(SDDB_HAS_RECORDSET == nRet)
    {

        UINT32	dwLastFactionID = 0;
        DT_FACTION_PLAYER_DATA_LST stDT_FACTION_PLAYER_DATA_LST;
        memset(&stDT_FACTION_PLAYER_DATA_LST, 0x00, sizeof(stDT_FACTION_PLAYER_DATA_LST));
        while (pRecordSet->GetRecord())
        {
            UINT32 dwLen = 0;
            UINT32 dwFactionID = SGDP::SDAtou(pRecordSet->GetFieldValueByName("FactionID"));
            CMapFactionItr itr = m_mapFaction.find(dwFactionID);
            if (itr == m_mapFaction.end())
            {
                continue;
            }

            if (dwFactionID != dwLastFactionID && 0 != dwLastFactionID)
            {
                CMapFactionItr itr = m_mapFaction.find(dwLastFactionID);
                if (itr != m_mapFaction.end())
                {
                    CFaction *poFaction = itr->second;
                    poFaction->Init(stDT_FACTION_PLAYER_DATA_LST);
                }
                memset(&stDT_FACTION_PLAYER_DATA_LST, 0x00, sizeof(stDT_FACTION_PLAYER_DATA_LST));
            }

            if (stDT_FACTION_PLAYER_DATA_LST.dwFactionNum >= DT_FACTION_PLAYER_NUM)
            {
                continue;
            }

            dwLastFactionID = dwFactionID;
            DT_FACTION_PLAYER_DATA &stDT_FACTION_PLAYER_DATA = stDT_FACTION_PLAYER_DATA_LST.astFactionPlayerData[stDT_FACTION_PLAYER_DATA_LST.dwFactionNum++]; //帮派用户信息
            stDT_FACTION_PLAYER_DATA.dwJoinPlayerID = SGDP::SDAtou(pRecordSet->GetFieldValueByName("JoinPlayerID"));
            stDT_FACTION_PLAYER_DATA.byJoinState = SGDP::SDAtou(pRecordSet->GetFieldValueByName("JoinState"));
            stDT_FACTION_PLAYER_DATA.dwDoorsTribute = SGDP::SDAtou(pRecordSet->GetFieldValueByName("DoorsTribute"));
			stDT_FACTION_PLAYER_DATA.qwDoorsTributeSum = SGDP::SDAtou64(pRecordSet->GetFieldValueByName("DoorsTributeSum"));
			stDT_FACTION_PLAYER_DATA.byJobType = SGDP::SDAtou(pRecordSet->GetFieldValueByName("JobType"));
			stDT_FACTION_PLAYER_DATA.byDrinkFlag = SGDP::SDAtou(pRecordSet->GetFieldValueByName("DrinkFlag"));
			stDT_FACTION_PLAYER_DATA.byNewGoodFlag = SGDP::SDAtou(pRecordSet->GetFieldValueByName("NewGoodsFlag"));
			stDT_FACTION_PLAYER_DATA.byNewSkillFlag = SGDP::SDAtou(pRecordSet->GetFieldValueByName("NewSkillFlag"));
			GetDateTime2Stamp(stDT_FACTION_PLAYER_DATA.qwJoinTime, "JoinTime");
			dwLen = pRecordSet->GetFieldLengthByName("UnLoginBattleLog");
			if (0 != dwLen)
			{
				if( -1 == stDT_FACTION_PLAYER_DATA.stUnLoginBattleLogList.DecodeSaveData(pRecordSet->GetFieldValueByName("UnLoginBattleLog"), dwLen ) )
				{
					SYS_CRITICAL( _SDT("[%s: %d] stUnLoginBattleLogList.DecodeSaveData Failed!"), MSG_MARK);
					//return FALSE;
				}
			}

			dwLen = pRecordSet->GetFieldLengthByName("ContinueKillKey");
			if (0 != dwLen)
			{
				if( -1 == stDT_FACTION_PLAYER_DATA.stContinueKillKey.DecodeSaveData(pRecordSet->GetFieldValueByName("ContinueKillKey"), dwLen ) )
				{
					SYS_CRITICAL( _SDT("[%s: %d] stContinueKillKey.DecodeSaveData Failed!"), MSG_MARK);
					//return FALSE;
				}
			}
		}
        CMapFactionItr itr = m_mapFaction.find(dwLastFactionID);
        if (itr != m_mapFaction.end())
        {
            CFaction *poFaction = itr->second;
            poFaction->Init(stDT_FACTION_PLAYER_DATA_LST);
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}


VOID CFactionMgr::UnInit()
{


}

void CFactionMgr::OnSave(BOOL bAll)
{
    //数据每秒种检测一次，防止浪费时间做无用检查
    if(SDTimeSecs() - m_qwSaveTime < 1)
        return;
    m_qwSaveTime = SDTimeSecs();

    CMapFactionItr itr;
    for ( itr = m_mapFaction.begin(); itr != m_mapFaction.end(); itr++)
    {
        CFaction *poFaction = itr->second;
        if (NULL == poFaction)
        {
            continue;
        }

        if((TRUE == bAll)
                || ((TRUE != bAll) && (SDTimeSecs() - poFaction->GetSaveTime()) > (FACTION_SAVE_TIME + (GetRdNum() % FACTION_SAVE_TIME))) )
        {

            static DT_FACTION_DATA stDT_FACTION_DATA = {0};
            memset(&stDT_FACTION_DATA, 0x00, sizeof(DT_FACTION_DATA));
            memcpy(&stDT_FACTION_DATA.stFactionBaseData, &poFaction->GetDT_FACTION_BASE_DATA(), sizeof(DT_FACTION_BASE_DATA));
            poFaction->GetDT_FACTION_PLAYER_DATA_LST(stDT_FACTION_DATA.stFactionPlayerDataLst);
            poFaction->GetDT_FACTION_LOG_DATA_LST(stDT_FACTION_DATA.stFactionLogDataLst);
			poFaction->GetDT_FACTION_COMMON_DATA(stDT_FACTION_DATA.stFactionCommonData);
			poFaction->GetAuditData(stDT_FACTION_DATA.byTodayPassNum, stDT_FACTION_DATA.qwLastAuditTime, stDT_FACTION_DATA.byExt);
			poFaction->GetFactionGvGData(stDT_FACTION_DATA.stGvGData);
            CDBPktBuilder::Instance()->SaveFactionDataReq(&stDT_FACTION_DATA);
            poFaction->GetSaveTime() = SDTimeSecs();
        }
    }
}


CFaction * CFactionMgr::GetFaction(UINT32 dwFactionID)
{
    CMapFactionItr itr = m_mapFaction.find(dwFactionID);
    if(itr != m_mapFaction.end())
    {
        return itr->second;
    }
    return NULL;
}

CFaction* CFactionMgr::GetFactionByPlayerID(UINT32 dwPlayerID)
{
	UINT32 dwFactionID = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, dwPlayerID);
	if (bRet)
	{
		return GetFaction(dwFactionID);
	}
	return NULL;
}
//创建帮派
UINT16 CFactionMgr::CreateFactionReq(CPlayer* poPlayer, UINT16 byCreateFactionType, TCHAR aszFactionName[MAX_FACTION_NUM], TCHAR aszFactionDescName[MAX_FACTION_DESC_NUM], UINT32 dwFactionID, DT_FACTION_OWNER_DATA &stFactionOwnerData)
{
    //查找玩家是否在帮派中
    UINT32 v_dwFactionID;
    if(CFaction::CkFactionPlayerData(v_dwFactionID, poPlayer->GetID()))
    {
        return ERR_CREATE_FACTION::ID_PLAYER_HAVE_FACTION;
    }

    static DT_FACTION_DATA stDT_FACTION_DATA = {0};
	memset(&stDT_FACTION_DATA, 0, sizeof(DT_FACTION_DATA));
    stDT_FACTION_DATA.stFactionBaseData.dwFactionID = dwFactionID;
    stDT_FACTION_DATA.stFactionBaseData.dwFactionPlayerID = poPlayer->GetID();
    SDStrncpy(stDT_FACTION_DATA.stFactionBaseData.aszFactionName, aszFactionName, MAX_FACTION_NUM - 1);
    stDT_FACTION_DATA.stFactionBaseData.dwFactionLevel = 1;
	/////////////////////建筑等级///////////////////////////////////////////
	 DT_FACTION_BUILD_LEVEL stFactionBuild = {0};
	GetFactionBuileGroupByLevel(1,stFactionBuild);
	stDT_FACTION_DATA.stFactionBaseData.dwGymnasiumLevel = (stFactionBuild.byGymnasiumLevel == 0 ? ESM_YES : ESM_NO);
	stDT_FACTION_DATA.stFactionBaseData.dwShadeLevel = (stFactionBuild.byShadeLevel == 0 ? ESM_YES : ESM_NO);
	stDT_FACTION_DATA.stFactionBaseData.dwRareItemRoomGrade = (stFactionBuild.byRareitemLevel == 0 ? ESM_YES : ESM_NO);
	stDT_FACTION_DATA.stFactionBaseData.dwHotSpringLevel = (stFactionBuild.byHotSpringLevel == 0 ? ESM_YES : ESM_NO);
	stDT_FACTION_DATA.stFactionBaseData.dwColdPrisonLevel = (stFactionBuild.byColdPrisonLevel == 0 ? ESM_YES : ESM_NO);
	stDT_FACTION_DATA.stFactionBaseData.dwFactionTowerLevel = (stFactionBuild.byFactionTowerLevel == 0 ? ESM_YES : ESM_NO);
	////////////////////////////////////////////////////////////////
    stDT_FACTION_DATA.stFactionBaseData.dwFactionRank = ++m_dwMaxFactionRank;
    SDStrncpy(stDT_FACTION_DATA.stFactionBaseData.aszFactionAnnouncement, aszFactionDescName, MAX_FACTION_DESC_NUM - 1);

    stDT_FACTION_DATA.stFactionPlayerDataLst.dwFactionNum = 1;
    stDT_FACTION_DATA.stFactionPlayerDataLst.astFactionPlayerData[0].dwJoinPlayerID	=	poPlayer->GetID();
    stDT_FACTION_DATA.stFactionPlayerDataLst.astFactionPlayerData[0].byJobType		=	em_Faction_MartialHead;
	stDT_FACTION_DATA.stFactionPlayerDataLst.astFactionPlayerData[0].qwJoinTime		=	SGDP::SDTimeSecs();

    CFaction	*poFaction = m_oFactionShmemObjMgr.Alloc();
    if (NULL == poFaction)
    {
        return ERR_COMMON::ID_OTHER_ERR;
    }
	
    vector<UINT32> vecFactionID;
    UINT16	wErrCode = poFaction->CreateFactionPlayerData(poPlayer->GetID(), stDT_FACTION_DATA, vecFactionID);

    if ( wErrCode == ERR_CREATE_FACTION::ID_SUCCESS)
    {
        m_mapFaction[dwFactionID] = poFaction;
        m_listRankFaction.push_back(poFaction);
        m_mapFactionName[poFaction->GetDT_FACTION_BASE_DATA().aszFactionName] = poFaction;

        //重新排名等级
		FactionSort();

        //清掉所有申请记录
        vector<UINT32>::iterator vecitr;
        for (vecitr = vecFactionID.begin(); vecitr != vecFactionID.end(); vecitr++)
        {
            CMapFactionItr itr = m_mapFaction.find(*vecitr);
            if (itr != m_mapFaction.end())
            {
                itr->second->CancelApplyFactionPlayerData(poPlayer->GetID());
            }
        }
		poFaction->GetDT_FACTION_BASE_OWNER_DATA_CLI(stFactionOwnerData.stFactionBaseData, poPlayer->GetID());
        poFaction->GetDT_FACTION_PLAYER_CLI_LST(stFactionOwnerData.stFactionPlayerCliLst);
    }
    else
    {
        m_oFactionShmemObjMgr.Free(poFaction);
    }
    return wErrCode;
}

//申请加入帮派
UINT16 CFactionMgr::ApplyFactionDataReq(UINT32 dwFactionID, UINT32 dwJoinPlayerID, UINT32& dwJoinFactionCDTime)
{
	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwJoinPlayerID);
	if (NULL == poPlayer)
	{
		return ERR_APPLY_FACTION::ID_PLAYER_NOT_EXIST;
	}
	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if (NULL == poFactionPlayer)
	{
		return ERR_APPLY_FACTION::ID_UNOPEN;
	}
	DT_LEAVE_FACTION_MARK& stPlayerMark = poFactionPlayer->GetDT_PLAYER_MARK_DATA().stLeaveFactionMark;
	if ( ESM_YES == stPlayerMark.byLeaveMark )
	{
		UINT32 dwIntervalTime = CLuamgr::Instance()->GetJoinFactionInterval();
		UINT64 dwCurTime = SGDP::SDTimeSecs();
		if ( dwCurTime < stPlayerMark.qwLeaveTime + dwIntervalTime)
		{
			dwJoinFactionCDTime = static_cast<UINT32>(stPlayerMark.qwLeaveTime + dwIntervalTime - dwCurTime);
			return ERR_APPLY_FACTION::ID_TIME_NOT_ENOUGH;
		}
		stPlayerMark.byLeaveMark = ESM_NO;
		stPlayerMark.qwLeaveTime = 0;
	}

	UINT16 wApplyNum = CFactionMgr::Instance()->GetApplyFactionNum(dwJoinPlayerID);
	if( CSinglePramMgr::Instance()->GetApplyFactionMaxNum() <= wApplyNum)
	{
		return ERR_APPLY_FACTION::ID_APPLY_FACTION_NUM_IS_MAX;
	}
	
    CMapFactionItr itr = m_mapFaction.find(dwFactionID);
    if(itr == m_mapFaction.end())
    {
        return ERR_APPLY_FACTION::ID_NOT_EXIT_FACTION;
    }
    return itr->second->ApplyFactionData(dwJoinPlayerID);
}

//取消申请加入帮派
UINT16 CFactionMgr::CancelApplyFactionDataReq(UINT32 dwFactionID, UINT32 dwJoinPlayerID)
{
    CMapFactionItr itr = m_mapFaction.find(dwFactionID);
    if(itr == m_mapFaction.end())
    {
        return ERR_CANCEL_APPLY_FACTION::ID_NOT_EXIT_FACTION;
    }
    return itr->second->CancelApplyFactionPlayerData(dwJoinPlayerID);
}

UINT16 CFactionMgr::KickedFactionReq(UINT32 dwJoinPlayerID, UINT32 dwKickedPlayerID, PKT_CLIGS_KICKED_FACTION_ACK& stAck)
{
    //查找玩家是否在帮派中
    UINT32 v_dwFactionID;
    if(!CFaction::CkFactionPlayerData(v_dwFactionID, dwJoinPlayerID))
    {
        return ERR_KICKED_FACTION::ID_NOT_APPLY_FACTION;
    }

    CMapFactionItr itr = m_mapFaction.find(v_dwFactionID);
    if(itr == m_mapFaction.end())
    {
        return ERR_KICKED_FACTION::ID_NOT_EXIT_FACTION;
    }
    return itr->second->KickedFaction(dwJoinPlayerID, dwKickedPlayerID, stAck);
}

//
UINT16	 CFactionMgr::OpenApplyFactionReq(DT_FACTION_BASE_DATA_CLI_LST & stDT_FACTION_BASE_DATA_CLI_LST, UINT32 dwJoinPlayerID)
{
    list<UINT32> listFactionID;
    if(!CFaction::CkFactionApplyPlayerData(dwJoinPlayerID, listFactionID))
    {
        return ERR_OPEN_APPLY_FACTION::ID_SUCCESS;
    }
	listFactionID.sort(CFactionSort());
    list<UINT32>::iterator listitr;
    for (listitr = listFactionID.begin(); listitr != listFactionID.end() && stDT_FACTION_BASE_DATA_CLI_LST.wFactionNum < MAX_FACTION_CLI_NUM; listitr++)
    {
        CMapFactionItr itr = m_mapFaction.find(*listitr);
        if (itr == m_mapFaction.end())
        {
            continue;
        }

        CFaction *poFaction = itr->second;
        const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(poFaction->GetDT_FACTION_BASE_DATA().dwFactionPlayerID);
        if( NULL == poPlayerBaseData )
        {
            continue;
        }

        DT_FACTION_BASE_DATA_CLI &stDT_FACTION_BASE_DATA_CLI = stDT_FACTION_BASE_DATA_CLI_LST.astFactionBaseData[stDT_FACTION_BASE_DATA_CLI_LST.wFactionNum++];
        stDT_FACTION_BASE_DATA_CLI.byJoinFlag = 1;
		poFaction->GetFactionPlayerNum(stDT_FACTION_BASE_DATA_CLI.wFactionNumberPeople, stDT_FACTION_BASE_DATA_CLI.wFactionMaxPeople);
        SDStrncpy(stDT_FACTION_BASE_DATA_CLI.aszDispName, poPlayerBaseData->aszDispName, USERNAME_LEN - 1);
		DT_FACTION_DATA_TO_CLI stCli = {0};
		poFaction->GetFactionDataToCli(stCli);
        memcpy(&stDT_FACTION_BASE_DATA_CLI.stFactionBaseData, &stCli, sizeof(DT_FACTION_BASE_DATA));
    }

    return ERR_OPEN_APPLY_FACTION::ID_SUCCESS;
}


UINT16	 CFactionMgr::GetRankFactionCliLst(DT_FACTION_BASE_DATA_CLI_LST & stDT_FACTION_BASE_DATA_CLI_LST, UINT32 dwJoinPlayerID)
{
    //重新排名等级
	FactionSort();
    CListRankFactionItr itr;
    for ( itr = m_listRankFaction.begin(); itr != m_listRankFaction.end() && stDT_FACTION_BASE_DATA_CLI_LST.wFactionNum < MAX_FACTION_CLI_NUM; itr++)
    {
        CFaction *poFaction = *itr;
        const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(poFaction->GetDT_FACTION_BASE_DATA().dwFactionPlayerID);
        if( NULL == poPlayerBaseData )
        {
            continue;
        }

		DT_FACTION_BASE_DATA_CLI &stDT_FACTION_BASE_DATA_CLI = stDT_FACTION_BASE_DATA_CLI_LST.astFactionBaseData[stDT_FACTION_BASE_DATA_CLI_LST.wFactionNum++];
		if (poFaction->CkFactionPlayerData(dwJoinPlayerID))
		{
			stDT_FACTION_BASE_DATA_CLI.byJoinFlag = 2;
		}
		else if(poFaction->CkFactionApplyPlayerData(dwJoinPlayerID))
		{
			stDT_FACTION_BASE_DATA_CLI.byJoinFlag = 1;
		}
		else
		{
			stDT_FACTION_BASE_DATA_CLI.byJoinFlag = 0;
		}
		poFaction->GetFactionPlayerNum(stDT_FACTION_BASE_DATA_CLI.wFactionNumberPeople, stDT_FACTION_BASE_DATA_CLI.wFactionMaxPeople);
        SDStrcat(stDT_FACTION_BASE_DATA_CLI.aszDispName, poPlayerBaseData->aszDispName);
		poFaction->GetFactionDataToCli(stDT_FACTION_BASE_DATA_CLI.stFactionBaseData);
    }
    return ERR_OPEN_RANK_FACTION::ID_SUCCESS;
}

UINT16	 CFactionMgr::GetSearchFactionCliLst(DT_FACTION_BASE_DATA_CLI_LST & stDT_FACTION_BASE_DATA_CLI_LST, TCHAR aszFactionName[MAX_FACTION_NUM], UINT32 dwJoinPlayerID)
{
    CListRankFactionItr itr = m_listRankFaction.begin();
    for ( ; itr != m_listRankFaction.end() && stDT_FACTION_BASE_DATA_CLI_LST.wFactionNum < MAX_FACTION_CLI_NUM; itr++)
    {
        string strName = (*itr)->GetDT_FACTION_BASE_DATA().aszFactionName;
        strName = ToLowerByString(strName.c_str());
        string strFactionName = ToLowerByString(aszFactionName);
        string::size_type loc = strName.find(strFactionName, 0 );
        if( loc != string::npos )
        {
            CFaction *poFaction = *itr;
            DT_FACTION_BASE_DATA_CLI &stDT_FACTION_BASE_DATA_CLI = stDT_FACTION_BASE_DATA_CLI_LST.astFactionBaseData[stDT_FACTION_BASE_DATA_CLI_LST.wFactionNum++];
            if (poFaction->CkFactionPlayerData(dwJoinPlayerID))
            {
                stDT_FACTION_BASE_DATA_CLI.byJoinFlag = 2;
            }
            else if(poFaction->CkFactionApplyPlayerData(dwJoinPlayerID))
            {
                stDT_FACTION_BASE_DATA_CLI.byJoinFlag = 1;
            }
            else
            {
                stDT_FACTION_BASE_DATA_CLI.byJoinFlag = 0;
            }
			poFaction->GetFactionDataToCli(stDT_FACTION_BASE_DATA_CLI.stFactionBaseData);
			DT_FACTION_PLAYER_CLI_LST stFactionPlayerList = {0};
			poFaction->GetDT_FACTION_PLAYER_CLI_LST(stFactionPlayerList);
			poFaction->GetFactionPlayerNum(stDT_FACTION_BASE_DATA_CLI.wFactionNumberPeople, stDT_FACTION_BASE_DATA_CLI.wFactionMaxPeople);
			const SPlayerBaseData* poPlayerBase = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(poFaction->GetDT_FACTION_BASE_DATA().dwFactionPlayerID);
			if (NULL == poPlayerBase)
			{
				return ERR_SEARCH_FACTION::ID_NOT_PLAYER_FACTION_HEAD;
			}
			SDStrncpy(stDT_FACTION_BASE_DATA_CLI.aszDispName, poPlayerBase->aszDispName, sizeof(stDT_FACTION_BASE_DATA_CLI.aszDispName));
        }
		else
		{
			UINT32 dwFactionRank = SGDP::SDAtou(aszFactionName);
			if (0 != dwFactionRank)
			{
				if (dwFactionRank == (*itr)->GetDT_FACTION_BASE_DATA().dwFactionRank)
				{
					CFaction *poFaction = *itr;
					DT_FACTION_BASE_DATA_CLI &stDT_FACTION_BASE_DATA_CLI = stDT_FACTION_BASE_DATA_CLI_LST.astFactionBaseData[stDT_FACTION_BASE_DATA_CLI_LST.wFactionNum++];
					if (poFaction->CkFactionPlayerData(dwJoinPlayerID))
					{
						stDT_FACTION_BASE_DATA_CLI.byJoinFlag = 2;
					}
					else if(poFaction->CkFactionApplyPlayerData(dwJoinPlayerID))
					{
						stDT_FACTION_BASE_DATA_CLI.byJoinFlag = 1;
					}
					else
					{
						stDT_FACTION_BASE_DATA_CLI.byJoinFlag = 0;
					}
					poFaction->GetFactionDataToCli(stDT_FACTION_BASE_DATA_CLI.stFactionBaseData);
					DT_FACTION_PLAYER_CLI_LST stFactionPlayerList = {0};
					poFaction->GetDT_FACTION_PLAYER_CLI_LST(stFactionPlayerList);
					poFaction->GetFactionPlayerNum(stDT_FACTION_BASE_DATA_CLI.wFactionNumberPeople, stDT_FACTION_BASE_DATA_CLI.wFactionMaxPeople);
					const SPlayerBaseData* poPlayerBase = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(poFaction->GetDT_FACTION_BASE_DATA().dwFactionPlayerID);
					if (NULL == poPlayerBase)
					{
						return ERR_SEARCH_FACTION::ID_NOT_PLAYER_FACTION_HEAD;
					}
					SDStrncpy(stDT_FACTION_BASE_DATA_CLI.aszDispName, poPlayerBase->aszDispName, sizeof(stDT_FACTION_BASE_DATA_CLI.aszDispName));
				}
			}
			
		}

    }
    return ERR_SEARCH_FACTION::ID_SUCCESS;
}


UINT16 CFactionMgr::OnOpenOwnerFactionReq(PKT_CLIGS_OPEN_OWNER_FACTION_ACK & stAck, UINT32 dwJoinPlayerID)
{
    UINT32 dwFactionID;
    if(!CFaction::CkFactionPlayerData(dwFactionID, dwJoinPlayerID))
    {
        return ERR_OPEN_OWNER_FACTION::ID_PLAYER_NOT_HAVE_FACTION;
    }
    CFaction * poFaction = GetFaction(dwFactionID);
    if (NULL == poFaction)
    {
        return ERR_OPEN_OWNER_FACTION::ID_PLAYER_NOT_HAVE_FACTION;
    }
	BOOL bRet = poFaction->GetExceptionFlag();
	if ( bRet )
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}
	FactionSort();

	poFaction->GetDT_FACTION_BASE_OWNER_DATA_CLI(stAck.stFactionOwnerData.stFactionBaseData, dwJoinPlayerID);
    poFaction->GetDT_FACTION_PLAYER_CLI_LST(stAck.stFactionOwnerData.stFactionPlayerCliLst);
	if ( 0 != stAck.stFactionOwnerData.stFactionBaseData.stFactionAuthority.byAudit)
	{
		stAck.stFactionOwnerData.byAuditNum = poFaction->GetApplyFactionNum();
	}
	
    return ERR_OPEN_OWNER_FACTION::ID_SUCCESS;
}

UINT16 CFactionMgr::OpenDoorsTributeFactionReq(PKT_CLIGS_OPEN_DOORS_TRIBUTE_FACTION_ACK& stAck, UINT32 dwJoinPlayerID)
{
    UINT32 dwFactionID;
    if(!CFaction::CkFactionPlayerData(dwFactionID, dwJoinPlayerID))
    {
        return ERR_OPEN_DOORS_TRIBUTE_FACTION::ID_PLAYER_NOT_HAVE_FACTION;
    }

    CFaction * poFaction = GetFaction(dwFactionID);
    if (NULL == poFaction)
    {
        return ERR_OPEN_DOORS_TRIBUTE_FACTION::ID_PLAYER_NOT_HAVE_FACTION;
    }

	BOOL bRet = poFaction->GetExceptionFlag();
	if(bRet)
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}

	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwJoinPlayerID);
	if ( NULL == poPlayer)
	{
		return ERR_OPEN_DOORS_TRIBUTE_FACTION::ID_NOT_FIND_PLAYER;
	}
	UINT32 dwVipLevel = poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel;
    GetDoorsTributeBaseData(stAck.stDoorsTributeBaseData, stAck.stEncourageData, dwVipLevel, poFaction->GetDT_FACTION_BASE_DATA().dwFactionLevel);
	DT_FACTION_PLAYER_CLI stCLi = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stCLi, dwJoinPlayerID);
	stAck.dwDoorsTribute = stCLi.dwDoorsTributeValues;
	stAck.byCoinRequireTime = (UINT8)CFactorPropMgr::Instance()->GetCoinMaxTimeEveryDay(poFaction->GetDT_FACTION_BASE_DATA().dwFactionLevel);
    return poFaction->OpenDoorsTributeFaction(stAck.stDoorsTributeTodayData, dwJoinPlayerID);
}

UINT16 CFactionMgr::DoorsTributeFactionReq(PKT_CLIGS_DOORS_TRIBUTE_FACTION_ACK& stAck, UINT8 byDoorsTributeFlag, CPlayer* poPlayer)
{
    UINT32 dwFactionID;
    if(!CFaction::CkFactionPlayerData(dwFactionID, poPlayer->GetID()))
    {
        return ERR_DOORS_TRIBUTE_FACTION::ID_PLAYER_NOT_HAVE_FACTION;
    }

    CFaction * poFaction = GetFaction(dwFactionID);
    if (NULL == poFaction)
    {
        return ERR_DOORS_TRIBUTE_FACTION::ID_PLAYER_NOT_HAVE_FACTION;
    }
	BOOL bRet = poFaction->GetExceptionFlag();
	if ( bRet )
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}
	UINT32 dwVipLevel = poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel;
	DT_ITEM_LIST_INFO stItem = {0};
    GetDoorsTributeBaseData(stAck.stDoorsTributeBaseData, stItem, dwVipLevel, poFaction->GetDT_FACTION_BASE_DATA().dwFactionLevel);
    UINT16 wErrCode = poFaction->DoorsTributeFaction(stAck.stDoorsTributeTodayData, byDoorsTributeFlag, poPlayer);
	stAck.dwFactionFunds = poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds;
	DT_FACTION_PLAYER_CLI stCLi = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stCLi, poPlayer->GetID());
	stAck.dwDoorsTribute = stCLi.dwDoorsTributeValues;
	stAck.qwSumDoorsTribute = stCLi.qwDoorsTributeSum;
    //重新排名等级
	FactionSort();
    if ( wErrCode == ERR_COMMON::ID_VIP_LEVEL_NOT_REACH)
    {
		stAck.byNeedVIPLevel = poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel + 1;
    }
    return wErrCode;
}

UINT16 CFactionMgr::RecvDoorsTributeFactionReq( CPlayer * poPlayer , PKT_CLIGS_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE_ACK& stAck)
{
    UINT32 dwFactionID;
    if(!CFaction::CkFactionPlayerData(dwFactionID, poPlayer->GetID()))
    {
        return ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_PLAYER_NOT_HAVE_FACTION;
    }

    CFaction * poFaction = GetFaction(dwFactionID);
    if (NULL == poFaction)
    {
        return ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_PLAYER_NOT_HAVE_FACTION;
    }
	BOOL bRet = poFaction->GetExceptionFlag();
	if ( bRet )
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}
    return poFaction->RecvDoorsTributeFactionReq(poPlayer, stAck);
}


UINT16	CFactionMgr::JobFactionReq(UINT32 dwJoinPlayerID, UINT32 dwJobPlayerID, UINT8 byJobFlag, PKT_CLIGS_JOB_FACTION_ACK& stAck )
{
    UINT32	v_dwFactionID;
    if(!CFaction::CkFactionPlayerData(v_dwFactionID, dwJoinPlayerID))
    {
        return ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_PLAYER_NOT_IN_FACTION;
    }

    CFaction * poFaction = GetFaction(v_dwFactionID);
    if (NULL == poFaction)
    {
        return ERR_RECV_DOORS_TRIBUTE_FACTION_ENCOURAGE::ID_PLAYER_NOT_IN_FACTION;
    }
	BOOL bRet = poFaction->GetExceptionFlag();
	if ( bRet )
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}

	UINT16 wRet = poFaction->JobFaction(dwJoinPlayerID, dwJobPlayerID, byJobFlag);

	if ( wRet == ERR_JOB_FACTION::ID_SUCCESS)
	{
		poFaction->GetDT_FACTION_PLAYER_CLI_LST(stAck.stFactionPlayerDataLst);
		stAck.stDoorsTributeLst.wDoorsTributeNum = stAck.stFactionPlayerDataLst.wFactionNum;

		for ( UINT16 wIndex = 0; wIndex < stAck.stFactionPlayerDataLst.wFactionNum; wIndex++)
		{
			CFactorPropMgr::Instance()->GetDoorsTributeInfo(stAck.stFactionPlayerDataLst.astFactionPlayerCli[wIndex].qwDoorsTributeSum, stAck.stDoorsTributeLst.astDoorsTributeInfo[wIndex]);
		}
	}
	return wRet;
}

// 设置帮派基本信息
UINT16	CFactionMgr::SetNoticeFactionReq( UINT32 dwJoinPlayerID, UINT8 byFlag, TCHAR aszFactionNotice[MAX_FACTION_DESC_NUM])
{
    UINT32	v_dwFactionID;
    if(!CFaction::CkFactionPlayerData(v_dwFactionID, dwJoinPlayerID))
    {
        return ERR_SET_NOTICE_FACTION::ID_PLAYER_NOT_IN_FACTION;
    }

    CFaction * poFaction = GetFaction(v_dwFactionID);
    if (NULL == poFaction)
    {
        return ERR_SET_NOTICE_FACTION::ID_PLAYER_NOT_IN_FACTION;
    }
	BOOL bRet = poFaction->GetExceptionFlag();
	if ( bRet )
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}
	//检查参数
	if( !CWordFilter::Instance()->VerifyDspNameString(aszFactionNotice))
	{
		return ERR_SET_NOTICE_FACTION::ID_STRING_NOT_ENOUGH;
	}
    return poFaction->SetNoticeFaction(dwJoinPlayerID, byFlag, aszFactionNotice);
}

UINT16	CFactionMgr::DissolveFactionReq( UINT32 dwJoinPlayerID )
{
    UINT32	v_dwFactionID;
    if(!CFaction::CkFactionPlayerData(v_dwFactionID, dwJoinPlayerID))
    {
        return ERR_DISSOLVE_FACTION::ID_PLAYER_NOT_IN_FACTION;
    }

    CFaction * poFaction = GetFaction(v_dwFactionID);
    if (NULL == poFaction)
    {
        return ERR_DISSOLVE_FACTION::ID_PLAYER_NOT_IN_FACTION;
    }

    if(CGvGBuild::Instance()->IsApply(v_dwFactionID))
    {
        return ERR_DISSOLVE_FACTION::ID_IN_GVG_ACTIVITY;
    }

    UINT16  wErrCode = poFaction->DissolveFaction(dwJoinPlayerID);
    if(ERR_DISSOLVE_FACTION::ID_SUCCESS == wErrCode)
    {
        m_mapFaction.erase(v_dwFactionID);
        m_listRankFaction.remove(poFaction);
        m_mapFactionName.erase(poFaction->GetDT_FACTION_BASE_DATA().aszFactionName);

        m_oFactionShmemObjMgr.Free(poFaction);
    }

    //重新排名等级
	FactionSort();
    return wErrCode;
}

UINT16	CFactionMgr::OpenAuditFaction(PKT_CLIGS_OPEN_AUDIT_FACTION_ACK &stAck, UINT32 dwJoinPlayerID)
{
    UINT32	v_dwFactionID;
    if(!CFaction::CkFactionPlayerData(v_dwFactionID, dwJoinPlayerID))
    {
        return ERR_OPEN_AUDIT_FACTION::ID_PLAYER_NOT_IN_FACTION;
    }

    CFaction * poFaction = GetFaction(v_dwFactionID);
    if (NULL == poFaction)
    {
        return ERR_OPEN_AUDIT_FACTION::ID_PLAYER_NOT_IN_FACTION;
    }
	BOOL bRet = poFaction->GetExceptionFlag();
	if ( bRet )
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}
    poFaction->GetDT_FACTION_PLAYER_AUDIT_CLI_LST(stAck.stFactionPlayerAuditCliLst);
    return ERR_OPEN_AUDIT_FACTION::ID_SUCCESS;
}

UINT16	CFactionMgr::AuditFactionReq(UINT32 dwJoinPlayerID, UINT32 dwFactionNum, DT_FACTION_AUDIT_PLAYER_DATA astAuditPlayerData[DT_FACTION_PLAYER_NUM], PKT_CLIGS_AUDIT_FACTION_ACK& stAck)
{
    UINT32	v_dwFactionID;
    if(!CFaction::CkFactionPlayerData(v_dwFactionID, dwJoinPlayerID))
    {
        return ERR_AUDIT_FACTION::ID_PLAYER_NOT_IN_FACTION;
    }

    CFaction * poFaction = GetFaction(v_dwFactionID);
    if (NULL == poFaction)
    {
        return ERR_AUDIT_FACTION::ID_PLAYER_NOT_IN_FACTION;
    }
	BOOL bRet = poFaction->GetExceptionFlag();
	if ( bRet )
	{
		return ERR_COMMON::ID_FACTION_IS_EXCEPTION;
	}

	DT_FACTION_PLAYER_CLI stPlayerCLi = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stPlayerCLi, dwJoinPlayerID);
	if (!CFactorAuthorityPropMgr::Instance()->CkJobAuthority((em_Faction_Job)stPlayerCLi.byJobType, EFA_Audit))
	{
		return ERR_AUDIT_FACTION::ID_PLAYER_NOT_AUTHOR;
	}

    UINT16 wErrCode = ERR_AUDIT_FACTION::ID_SUCCESS;

	if ( 0 != dwFactionNum)
	{
		if( astAuditPlayerData[0].byAuditFlag )
		{
			UINT16 wCurPlayerNum = 0;
			UINT16 wMaxPlayerNum = 0;
			poFaction->GetFactionPlayerNum(wCurPlayerNum, wMaxPlayerNum);
			
			if ( wMaxPlayerNum <= wCurPlayerNum)
			{
				return ERR_AUDIT_FACTION::ID_PLAYER_NUM_IS_MAX;
			}

			if ( (wMaxPlayerNum < wCurPlayerNum + dwFactionNum) || ( CSinglePramMgr::Instance()->GetEveryDayAuditMaxNum() < dwFactionNum + poFaction->GetTodayPassNum()))
			{
				UINT16 wFactionLeftNum = wMaxPlayerNum - wCurPlayerNum;
				UINT16 wTodayPassNum = CSinglePramMgr::Instance()->GetEveryDayAuditMaxNum() - poFaction->GetTodayPassNum();
				stAck.wLeftAuditNum = wFactionLeftNum < wTodayPassNum ? wFactionLeftNum : wTodayPassNum;
				return ERR_AUDIT_FACTION::ID_AUDIT_NUM_IS_MAX;
			}
		}
	}
	BOOL bAlreadyJoinOther = FALSE;
	BOOL bCancelApply = FALSE;
    for (UINT32 i = 0; i < dwFactionNum; i++)
    {
        vector<UINT32> vecFactionID;
        wErrCode = poFaction->CKAuditFaction( astAuditPlayerData[i].byAuditFlag, astAuditPlayerData[i].dwPlayerID );
        
		if(ERR_AUDIT_FACTION::ID_ALREADY_AUDIT_OTHER == wErrCode)
        {
			bAlreadyJoinOther = TRUE;
            continue;
        }
		else if( ERR_AUDIT_FACTION::ID_ALREAD_CANCEL_APPLY == wErrCode)
		{
			bCancelApply = TRUE;
			continue;
		}
    }
	stAck.byLeftPlayerNum = poFaction->GetApplyFactionNum();
	poFaction->SetPlayerChangeToAll();
	poFaction->GetFactionPlayerNum(stAck.wCurPlayerNum, stAck.wMaxPlayerNum);

	if ( 1 == dwFactionNum)
	{
		if (bAlreadyJoinOther)
		{
			wErrCode = ERR_AUDIT_FACTION::ID_ALREADY_AUDIT_OTHER;
		}
		else if ( bCancelApply )
		{
			wErrCode = ERR_AUDIT_FACTION::ID_ALREAD_CANCEL_APPLY;
		}
		
	}
	else if( 1 < dwFactionNum)
	{
		if ( bAlreadyJoinOther)
		{
			wErrCode = ERR_AUDIT_FACTION::ID_MULTI_ALREADY_AUDIT_OTHER;
		}
		else if ( bCancelApply )
		{
			wErrCode = ERR_AUDIT_FACTION::ID_MULTI_ALREAD_CANCEL_APPLY;
		}
		
	}
	
	if ( ERR_AUDIT_FACTION::ID_SUCCESS == wErrCode )
	{
		for (UINT32 i = 0; i < dwFactionNum; i++)
		{
			vector<UINT32> vecFactionID;
			wErrCode = poFaction->AuditFaction(dwJoinPlayerID, astAuditPlayerData[i].byAuditFlag, astAuditPlayerData[i].dwPlayerID, vecFactionID, stAck.dwPassPlayerNum, stAck.dwNoPassPlayerNum);
			if(ERR_AUDIT_FACTION::ID_SUCCESS == wErrCode)
			{
				//清掉所有申请记录
				vector<UINT32>::iterator vecitr;
				for (vecitr = vecFactionID.begin(); vecitr != vecFactionID.end(); vecitr++)
				{
					CMapFactionItr itr = m_mapFaction.find(*vecitr);
					if (itr != m_mapFaction.end())
					{
						itr->second->CancelApplyFactionPlayerData(astAuditPlayerData[i].dwPlayerID);
					}
				}
			}
		}
	}
	poFaction->GetDT_FACTION_PLAYER_AUDIT_CLI_LST(stAck.stFactionPlayerAuditCliLst);
	poFaction->GetFactionPlayerNum(stAck.wCurPlayerNum, stAck.wMaxPlayerNum);
    return wErrCode;
}


UINT16	CFactionMgr::GetDT_FACTION_LOG_DATA_CLI_LST(DT_FACTION_LOG_DATA_CLI_LST &stDT_FACTION_LOG_DATA_CLI_LST, UINT32 dwJoinPlayerID)
{
    UINT32	v_dwFactionID;
    if(!CFaction::CkFactionPlayerData(v_dwFactionID, dwJoinPlayerID))
    {
        return ERR_OPEN_LOG_FACTION::ID_PLAYER_NOT_HAVE_FACTION;
    }

    CFaction * poFaction = GetFaction(v_dwFactionID);
    if (NULL == poFaction)
    {
        return ERR_OPEN_LOG_FACTION::ID_PLAYER_NOT_HAVE_FACTION;
    }

    return poFaction->GetDT_FACTION_LOG_DATA_CLI_LST(stDT_FACTION_LOG_DATA_CLI_LST);
}


//////////////////////////////////////////////////////////////////////////
void	  CFactionMgr::GetDoorsTributeBaseData(DT_DOORS_TRIBUTE_BASE_DATA &stDoorsTributeBaseData, DT_ITEM_LIST_INFO &stEncourageData, UINT32 dwVipLevel, UINT32 dwFactionLevel)
{
    stDoorsTributeBaseData.wCoinMaxTimeEveryDay		= CFactorPropMgr::Instance()->GetCoinMaxTimeEveryDay(dwFactionLevel);
    stDoorsTributeBaseData.dwCoinEveryTimeValues		= CLuamgr::Instance()->GetCostCoinByDonate(dwFactionLevel);
    stDoorsTributeBaseData.dwCoinEveryTimeDoorsTributeValues = CLuamgr::Instance()->GetDoorsTributeByDonateCoin(dwFactionLevel);
	stDoorsTributeBaseData.dwCoinEveryTimeFundsValues = CLuamgr::Instance()->GetFundsByDonateCoin(dwFactionLevel);

    stDoorsTributeBaseData.wGoldMaxTimeEveryDay =	CFactorPropMgr::Instance()->GetGoldMaxTimeEveryDay(dwVipLevel);
    stDoorsTributeBaseData.dwGoldEveryTimeValues =	CLuamgr::Instance()->GetCostGoldByDonate(dwFactionLevel);
    stDoorsTributeBaseData.dwGoldEveryTimeDoorsTributeValues = CLuamgr::Instance()->GetDoorsTributeByDonateGold(dwFactionLevel);
	stDoorsTributeBaseData.dwGoldEveryTimeFundsValues = CLuamgr::Instance()->GetFundsByDonateGold(dwFactionLevel);

	const CMapDonateReward& mapReward = CFactorPropMgr::Instance()->GetDonateReward();
	pair<CMapDonateRewardItr, CMapDonateRewardItr> pairItr = mapReward.equal_range(dwFactionLevel);
	for ( CMapDonateRewardItr itr = pairItr.first; itr != pairItr.second; itr++)
	{
		switch(itr->second.byRewardType)
		{
		case ACTE_GOOD:
			CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(itr->second.dwRewardValue, itr->second.byNum, stEncourageData.astItemInfo[stEncourageData.byItemNum]);
			stEncourageData.byItemNum++;
			break;
		default:
			break;
		}
	}

}

BOOL  CFactionMgr::DeductDoorsTribute( UINT32 dwPlayerID, UINT32 dwBuyCost, UINT16 wOpType)
{
	UINT32 dwFaction = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFaction, dwPlayerID);
	if ( !bRet )
	{
		return FALSE;
	}
	CFaction* pFaction = GetFaction(dwFaction);
	if ( NULL == pFaction )
	{
		return FALSE;
	}
	return pFaction->DeductDoorsTribute(dwPlayerID, dwBuyCost, wOpType);
}

VOID CFactionMgr::AddDoorsTribute(UINT32 dwPlayerID, UINT32 dwDoorsTribute, UINT16 wOpType)
{
	UINT32 dwFaction = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFaction, dwPlayerID);
	if ( !bRet )
	{
		return;
	}
	CFaction* pFaction = GetFaction(dwFaction);
	if ( NULL == pFaction )
	{
		return ;
	}
	pFaction->AddDoorsTribute(dwPlayerID, dwDoorsTribute, wOpType);
}

UINT8 CFactionMgr::CheckPlayerAuthority(UINT32 dwPlayerID, em_Faction_Authority emAuth)
{
	UINT32 dwFaction = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFaction, dwPlayerID);
	if ( !bRet )
	{
		return FALSE;
	}
	CFaction* pFaction = GetFaction(dwFaction);
	if ( NULL == pFaction )
	{
		return FALSE;
	}
	DT_FACTION_PLAYER_CLI stFactionPlayerCli = {0};
	pFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli, dwPlayerID);
	em_Faction_Job emJob = static_cast<em_Faction_Job>(stFactionPlayerCli.byJobType);
	if(CFactorAuthorityPropMgr::Instance()->CkJobAuthority(emJob, emAuth))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

VOID CFactionMgr::AddHuntingIntegral(UINT32 dwPlayerID, UINT32 dwValue)
{
	UINT32 dwFaction = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFaction, dwPlayerID);
	if ( !bRet )
	{
		return;
	}
	CFaction* pFaction = GetFaction(dwFaction);
	if ( NULL == pFaction )
	{
		return ;
	}
	pFaction->AddHuntingIntegral(dwPlayerID, dwValue);
}

VOID CFactionMgr::AddFactionFunds(UINT32 dwPlayerID, UINT32 dwValue, UINT16 wOpType)
{
	UINT32 dwFaction = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFaction, dwPlayerID);
	if ( !bRet )
	{
		return;
	}
	CFaction* pFaction = GetFaction(dwFaction);
	if ( NULL == pFaction )
	{
		return ;
	}
	pFaction->AddFactionFunds(dwValue, wOpType);
}

UINT32 CFactionMgr::ResetMaxTaskIntegral(UINT32 dwPlayerID)
{
	UINT32 dwFaction = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFaction, dwPlayerID);
	if ( !bRet )
	{
		return 0;
	}
	CFaction* pFaction = GetFaction(dwFaction);
	if ( NULL == pFaction )
	{
		return 0;
	}
	return pFaction->ResetMaxTaskIntegral();
}

//给一个门派的所有玩家添加门贡
VOID CFactionMgr::AddDoorsTributeToAll(UINT32 dwFactionID, UINT32 dwDoorsTribute)
{
	CFaction* pFaction = GetFaction(dwFactionID);
	if ( NULL == pFaction)
	{
		return ;
	}
	pFaction->AddDoorsTributeToAll(dwDoorsTribute);
}

VOID CFactionMgr::AddSetHotSpringPlayerID(UINT32 dwPlayerID)
{
	UINT32 dwFaction = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFaction, dwPlayerID);
	if ( !bRet )
	{
		return ;
	}
	CFaction* pFaction = GetFaction(dwFaction);
	if ( NULL == pFaction)
	{
		return ;
	}

	pFaction->AddSetHotSpringPlayerID(dwPlayerID);
}

VOID CFactionMgr::DelSetHotSpringPlayerID(UINT32 dwPlayerID)
{
	UINT32 dwFaction = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFaction, dwPlayerID);
	if ( !bRet )
	{
		return ;
	}
	CFaction* pFaction = GetFaction(dwFaction);
	if ( NULL == pFaction)
	{
		return ;
	}
	pFaction->DelSetHotSpringPlayerID(dwPlayerID);
}

VOID CFactionMgr::SetHotSpringPlayerList(DT_HOT_SPRING_PLAYER_DATA_CLI_LST& stPlayerInfo, UINT32 dwFactionID)
{
	CFaction* pFaction = GetFaction(dwFactionID);
	if ( NULL == pFaction)
	{
		return ;
	}
	pFaction->SetHotSpringPlayerList(stPlayerInfo);
}

BOOL CFactionMgr::Run()
{
	
	CMapFactionItr mapIt = m_mapFaction.begin();
	for ( ; mapIt != m_mapFaction.end(); mapIt++)
	{
		if ( NULL != mapIt->second)
		{
			mapIt->second->SendSynchInfo();
		}
		
	}
	return TRUE;
}

UINT16 CFactionMgr::ChangeOther(UINT32 dwPlayerID, UINT32 dwSendPlayerID, PKT_CLIGS_CHANGE_OTHER_ACK& stAck)
{
	UINT32	v_dwFactionID;
	if(!CFaction::CkFactionPlayerData(v_dwFactionID, dwPlayerID))
	{
		return ERR_CHANGE_OTHER::ID_FACTION_NOT_EXIST;
	}

	CFaction * pFaction = GetFaction(v_dwFactionID);
	if ( NULL == pFaction)
	{
		return ERR_CHANGE_OTHER::ID_FACTION_NOT_EXIST;
	}
	return pFaction->ChangeOther(dwPlayerID, dwSendPlayerID, stAck);
}

UINT16 CFactionMgr::OpenFactionUpgrade(UINT32 dwPlayerID, DT_FACTION_UPGRADE_ALL_INFO& stUpgradeAllInfo)
{
	UINT32	v_dwFactionID;
	if(!CFaction::CkFactionPlayerData(v_dwFactionID, dwPlayerID))
	{
		return ERR_OPEN_FACTION_UPGRADE::ID_FACTION_NOT_EXIST;
	}

	CFaction * poFaction = GetFaction(v_dwFactionID);
	if ( NULL == poFaction)
	{
		return ERR_OPEN_FACTION_UPGRADE::ID_FACTION_NOT_EXIST;
	}
	//检验权限
	UINT8 byRet = CheckPlayerAuthority(dwPlayerID, EFA_Build);
	stUpgradeAllInfo.byIsUpgradeLimit = byRet;
	UINT32 dwFactionLevel = poFaction->GetDT_FACTION_BASE_DATA().dwFactionLevel;
	stUpgradeAllInfo.byFactionLevel = dwFactionLevel;
	BOOL bRet = GetFactionUpgradeInfo(dwFactionLevel, stUpgradeAllInfo);
	if (FALSE == bRet )
	{
		return ERR_OPEN_FACTION_UPGRADE::ID_IS_FULL_LEVEL;
	}
	GetFactionUpgradeInfo(dwFactionLevel, stUpgradeAllInfo);
	if( poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds < stUpgradeAllInfo.dwNeedFactionFunds)
	{
		stUpgradeAllInfo.byFunds_Is_Enough = ESM_NO;
	}
	else
	{
		stUpgradeAllInfo.byFunds_Is_Enough = ESM_YES;
	}
	
	return ERR_OPEN_FACTION_UPGRADE::ID_SUCCESS;
}

BOOL CFactionMgr::GetFactionUpgradeInfo(UINT32 dwFactionLevel, DT_FACTION_UPGRADE_ALL_INFO& stUpgradeAllInfo)
{

	
	SFactionUpgradeProp stCurFactionUpgrade = {0};
	BOOL bRet = CFactorPropMgr::Instance()->GetFactionUpgradeProp(dwFactionLevel, stCurFactionUpgrade);
	if ( FALSE == bRet)
	{
		return FALSE;
	}

	stUpgradeAllInfo.dwNeedFactionFunds = stCurFactionUpgrade.dwNeedFactionFunds;

	SFactionUpgradeProp stNextFactionUpgrade = {0};
	bRet = CFactorPropMgr::Instance()->GetFactionUpgradeProp(dwFactionLevel + 1, stNextFactionUpgrade);
	if ( FALSE == bRet)
	{
		return FALSE;
	}
	UINT32 dwUpgradeMsgNum = 0;
	string strMsg;
	vector<string> vecStr;
	TCHAR aszMsg[FACTION_UPGRADE_MSG_LEN] = {0};
	UINT32 dwDiffNum = stNextFactionUpgrade.dwFactionPlayerMaxNum - stCurFactionUpgrade.dwFactionPlayerMaxNum;
	if ( 0 != dwDiffNum)
	{
		vecStr.clear();
		SDSnprintf(aszMsg, FACTION_UPGRADE_MSG_LEN, "%u", dwDiffNum);
		vecStr.push_back(aszMsg);
		strMsg = CMsgDefMgr::Instance()->GetErrMsg("FACTION_UPGRADE_BY_PLAYER", &vecStr);
		SDStrncpy(stUpgradeAllInfo.astUpgradeInfo[dwUpgradeMsgNum].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN - 1);
		dwUpgradeMsgNum++;
	}

	if (0 == stCurFactionUpgrade.byRareitemMark )
	{
		if (0 != stNextFactionUpgrade.byRareitemMark)
		{
			vecStr.clear();
			vecStr.push_back(CMsgDefMgr::Instance()->GetErrMsg("FACTION_BUILD_RARE", NULL));
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("FACTION_BUILD_UNLOCK", &vecStr);
			SDStrncpy(stUpgradeAllInfo.astUpgradeInfo[dwUpgradeMsgNum].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN - 1);
			dwUpgradeMsgNum++;
		}
	}

	if (0 == stCurFactionUpgrade.byGymnasiumMark )
	{
		if (0 != stNextFactionUpgrade.byGymnasiumMark)
		{
			vecStr.clear();
			vecStr.push_back(CMsgDefMgr::Instance()->GetErrMsg("FACTION_BUILD_GYMNASIUM", NULL));
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("FACTION_BUILD_UNLOCK", &vecStr);
			SDStrncpy(stUpgradeAllInfo.astUpgradeInfo[dwUpgradeMsgNum].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN - 1);
			dwUpgradeMsgNum++;
		}
	}

	if (0 == stCurFactionUpgrade.byShadeMark )
	{
		if (0 != stNextFactionUpgrade.byShadeMark)
		{
			vecStr.clear();
			vecStr.push_back(CMsgDefMgr::Instance()->GetErrMsg("FACTION_BUILD_SHADE", NULL));
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("FACTION_BUILD_UNLOCK", &vecStr);
			SDStrncpy(stUpgradeAllInfo.astUpgradeInfo[dwUpgradeMsgNum].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN - 1);
			dwUpgradeMsgNum++;
		}
	}

	if (0 == stCurFactionUpgrade.byHotSpringMark )
	{
		if (0 != stNextFactionUpgrade.byHotSpringMark)
		{
			vecStr.clear();
			vecStr.push_back(CMsgDefMgr::Instance()->GetErrMsg("FACTION_BUILD_HOTSPRING", NULL));
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("FACTION_BUILD_UNLOCK", &vecStr);
			SDStrncpy(stUpgradeAllInfo.astUpgradeInfo[dwUpgradeMsgNum].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN - 1);
			dwUpgradeMsgNum++;
		}
	}

	if (0 == stCurFactionUpgrade.byColdPrison)
	{
		if ( 0 != stNextFactionUpgrade.byColdPrison)
		{
			vecStr.clear();
			vecStr.push_back(CMsgDefMgr::Instance()->GetErrMsg("FACTION_BUILD_COLD_PRISON", NULL));
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("FACTION_BUILD_UNLOCK", &vecStr);
			SDStrncpy(stUpgradeAllInfo.astUpgradeInfo[dwUpgradeMsgNum].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN - 1);
		}
	}

	if (0 == stCurFactionUpgrade.byFactionTower)
	{
		if ( 0!= stNextFactionUpgrade.byFactionTower)
		{
			vecStr.clear();
			vecStr.push_back(CMsgDefMgr::Instance()->GetErrMsg("FACTION_BUILD_TOWER", NULL));
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("FACTION_BUILD_UNLOCK", &vecStr);
			SDStrncpy(stUpgradeAllInfo.astUpgradeInfo[dwUpgradeMsgNum].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN - 1);
		}
	}

	stUpgradeAllInfo.byUpgradeNum = dwUpgradeMsgNum;
	return TRUE;
}

UINT16 CFactionMgr::FactionUpgrade(UINT32 dwPlayerID, PKT_CLIGS_FACTION_UPGRADE_ACK& stAck)
{
	UINT32	v_dwFactionID;
	if(!CFaction::CkFactionPlayerData(v_dwFactionID, dwPlayerID))
	{
		return ERR_FACTION_UPGRADE::ID_FACTION_NOT_EXIST;
	}

	CFaction * poFaction = GetFaction(v_dwFactionID);
	if ( NULL == poFaction)
	{
		return ERR_FACTION_UPGRADE::ID_FACTION_NOT_EXIST;
	}
	//检验权限
	UINT8 byRet = CheckPlayerAuthority(dwPlayerID, EFA_Build);
	if ( 0 == byRet )
	{
		return ERR_FACTION_UPGRADE::ID_UPGRADE_LIMIT_NOT_ENOUGH;
	}
	DT_FACTION_BASE_DATA& stFactionBaseData = poFaction->GetDT_FACTION_BASE_DATA();
	UINT32 dwFactionLevel = stFactionBaseData.dwFactionLevel;
	SFactionUpgradeProp stFactionUpgrade = {0};
	BOOL bRet = CFactorPropMgr::Instance()->GetFactionUpgradeProp(dwFactionLevel + 1, stFactionUpgrade);
	if (bRet == FALSE)
	{
		return ERR_FACTION_UPGRADE::ID_FACTION_LEVEL_IS_FULL;
	}
	SFactionUpgradeProp stCurFactionUpgrade = {0};
	CFactorPropMgr::Instance()->GetFactionUpgradeProp(dwFactionLevel, stCurFactionUpgrade);
	if( stFactionBaseData.dwFactionFunds < stCurFactionUpgrade.dwNeedFactionFunds)
	{
		return ERR_FACTION_UPGRADE::ID_FACTION_FUNDS_NOT_ENOUGH;
	}
	else
	{
		//扣除资金
		poFaction->DecFactionFunds(stCurFactionUpgrade.dwNeedFactionFunds, CRecordMgr::EDFF_UPGRADE_FACTION);
	}

	stFactionBaseData.dwFactionLevel += 1;

	DT_FACTION_LOG_DATA stFactionLogData = {0};
	stFactionLogData.dwPlayerID1 = dwPlayerID;
	DT_FACTION_PLAYER_CLI stFactionPlayerCli = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli, dwPlayerID);
	stFactionLogData.byPlayerJobType1 = stFactionPlayerCli.byJobType;
	UINT32 dwLogNum = 0;
	stFactionLogData.adwParamList[dwLogNum] = stCurFactionUpgrade.dwNeedFactionFunds;//花费多少资金
	dwLogNum++;
	stFactionLogData.adwParamList[dwLogNum] = stFactionBaseData.dwFactionLevel;//提升到几级门派
	dwLogNum++;

	//个人门派战报
	DT_PLAYER_FACTION_BATTLELOG_DATA stLogData = {0};
	stLogData.byFactionLogType = EFBT_FactionUpgrade;
	stLogData.byNewFlag = ESM_YES;
	stLogData.qwLogTime = SGDP::SDTimeSecs();
	stLogData.byParamNum = 0;
	stLogData.abyparamList[stLogData.byParamNum++] = stFactionBaseData.dwFactionLevel;
	stLogData.abyparamList[stLogData.byParamNum++] = static_cast<UINT8>(stFactionUpgrade.dwFactionPlayerMaxNum);

	bRet = FALSE;

	PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
	if (0 == stFactionBaseData.dwRareItemRoomGrade )
	{
		if ( ESM_YES == stFactionUpgrade.byRareitemMark)
		{
			stFactionBaseData.dwRareItemRoomGrade = 1;
			poFaction->SetFactionRareInfo();
			stFactionLogData.adwParamList[dwLogNum] = EFB_Rareitem;
			stLogData.abyparamList[stLogData.byParamNum++] = EFB_Rareitem;
			dwLogNum++;
			bRet = TRUE;
			poFaction->SendMessageByRareItemPrompt();
		}
	}

	if (0 == stFactionBaseData.dwGymnasiumLevel)
	{
		if ( ESM_YES == stFactionUpgrade.byGymnasiumMark)
		{
			stFactionBaseData.dwGymnasiumLevel = 1;
			poFaction->SetFactionSkillInfo();
			stFactionLogData.adwParamList[dwLogNum] = EFB_Gymnasium;
			stLogData.abyparamList[stLogData.byParamNum++] = EFB_Gymnasium;
			poFaction->SetAllNewSkillFlag();
			dwLogNum++;
			bRet = TRUE;
			poFaction->SendMessageByGymnasiumPrompt();
		}
	}

	if ( 0 == stFactionBaseData.dwShadeLevel)
	{
		if ( ESM_YES == stFactionUpgrade.byShadeMark)
		{
			stFactionBaseData.dwShadeLevel = 1;
			stFactionLogData.adwParamList[dwLogNum] = EFB_Shade;
			stLogData.abyparamList[stLogData.byParamNum++] = EFB_Shade;
			dwLogNum++;
			bRet = TRUE;
			poFaction->SendMessageByShadePrompt();
		}
	}

	if (0 == stFactionBaseData.dwHotSpringLevel)
	{
		if ( ESM_YES == stFactionUpgrade.byHotSpringMark)
		{
			stFactionBaseData.dwHotSpringLevel = 1;
			stFactionLogData.adwParamList[dwLogNum] = EFB_HotSpring;
			stLogData.abyparamList[stLogData.byParamNum++] = EFB_HotSpring;
			dwLogNum++;
			bRet = TRUE;
			poFaction->SendMessageByHotSpringPrompt();
		}
	}
	poFaction->SendMessageByFactionPrompt();
	poFaction->SendMessageByLogNtf(stLogData);
	
	if ( !bRet)
	{
		stFactionLogData.byAction = EFLID_UpgradeFaction;
	}
	else
	{
		stFactionLogData.byAction = EFLID_UpgradeFactionAndBuild;
	}

	stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
	poFaction->AddFactionLogData(stFactionLogData);

	bRet = GetFactionUpgradeInfo(stFactionBaseData.dwFactionLevel , stAck.stNextUpgradeAllInfo);
	if ( FALSE == bRet)
	{
		stAck.byIsMaxLevel = ESM_YES;
	}
	else
	{
		stAck.byIsMaxLevel = ESM_NO;
	}

	if( poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds < stAck.stNextUpgradeAllInfo.dwNeedFactionFunds)
	{
		stAck.stNextUpgradeAllInfo.byFunds_Is_Enough = ESM_NO;
	}
	else
	{
		stAck.stNextUpgradeAllInfo.byFunds_Is_Enough = ESM_YES;
	}

	stAck.stSyncInfo.dwFactionFunds = stFactionBaseData.dwFactionFunds;
	stAck.stSyncInfo.dwFactionMaxNum = CFactorPropMgr::Instance()->GetFactionPlayerMaxNum(stFactionBaseData.dwFactionLevel);
	DT_FACTION_BUILD_LEVEL stFlag = {0}; 
	CFactionMgr::Instance()->GetFactionBuileGroupByLevel(stFactionBaseData.dwFactionLevel, stFlag);
	stAck.stSyncInfo.byRareitemFlag = stFlag.byRareitemLevel;
	stAck.stSyncInfo.byGymnasiumFlag = stFlag.byGymnasiumLevel;
	stAck.stSyncInfo.byShadeFlag = stFlag.byShadeLevel;
	stAck.stSyncInfo.byHotFlag = stFlag.byHotSpringLevel;
	stAck.stSyncInfo.byColdPrisonFlag = stFlag.byColdPrisonLevel;
	stAck.stSyncInfo.byFactionTowerFlag = stFlag.byFactionTowerLevel;
	

	stAck.stCurBuildLevel.byRareitemLevel = GetFactionBuildLevel(v_dwFactionID, EFB_Rareitem);
	stAck.stCurBuildLevel.byGymnasiumLevel = GetFactionBuildLevel(v_dwFactionID, EFB_Gymnasium);
	stAck.stCurBuildLevel.byShadeLevel = GetFactionBuildLevel(v_dwFactionID, EFB_Shade);
	stAck.stCurBuildLevel.byHotSpringLevel = GetFactionBuildLevel(v_dwFactionID, EFB_HotSpring);
	stAck.stCurBuildLevel.byColdPrisonLevel = GetFactionBuildLevel(v_dwFactionID, EFB_ColdPrison);
	stAck.stCurBuildLevel.byFactionTowerLevel = GetFactionBuildLevel(v_dwFactionID, EFB_FactionTower);
	stAck.byFactionLevel = static_cast<UINT8>(stFactionBaseData.dwFactionLevel);
	FactionSort();
	stAck.stSyncInfo.byFactionRank = static_cast<UINT8>(stFactionBaseData.dwFactionRank);
	stAck.dwFactionFunds = poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds;
	return ERR_FACTION_UPGRADE::ID_SUCCESS;
}

//解散门派
VOID CFactionMgr::DissolveFaction(CFaction* poFaction)
{
	if (NULL == poFaction)
	{
		return;
	}
	UINT32 dwHeadID = poFaction->GetDT_FACTION_BASE_DATA().dwFactionPlayerID;
	poFaction->DeletePlayerDataByID(dwHeadID);

	UINT32 dwFactionID = poFaction->GetDT_FACTION_BASE_DATA().dwFactionID;
    {
        //排名去掉
        CListRankFactionItr itr;
        for (itr = m_listRankFaction.begin(); itr != m_listRankFaction.end(); itr++)
        {
            if (poFaction == *itr)
            {
                m_listRankFaction.erase(itr);
                break;
            }
        }
    }

    {
        //去掉映射
        CMapFactionItr itr = m_mapFaction.find(dwFactionID);
        if ( itr != m_mapFaction.end())
        {
            m_mapFaction.erase(itr);
        }
    }

    {
        //去掉名称
        CFactionNameMapItr itr = m_mapFactionName.find(poFaction->GetDT_FACTION_BASE_DATA().aszFactionName);
        if ( itr != m_mapFactionName.end())
        {
            m_mapFactionName.erase(itr);
        }
	}
	gsapi::GetDBMgr()->OnFactionRelease(dwFactionID);
	//回收资源
	m_oFactionShmemObjMgr.Free(poFaction);
	DecreaseRank();
}

// 添加门派活动公告
void CFactionMgr::AddFactionActivityChat(const string &strChatContent)
{
    if(strChatContent.empty())
    {
        return;
    }

    // 在每个门派的聊天列表中加入门派活动公告
    for(CMapFaction::iterator itr = m_mapFaction.begin(); itr != m_mapFaction.end(); ++itr)
    {
        CFaction *poFaction = itr->second;
        if(NULL == poFaction)
        {
            continue;
        }
		if (0 != poFaction->GetFactionBuildLevel(EFB_Shade) )
		{
			poFaction->AddFactionActivityChat(strChatContent);
		}
        
    }
}

BOOL CFactionMgr::AddChatData(UINT32 dwPlayerID, const ChatInfo& stChatData)
{
	UINT32	v_dwFactionID;
	if(!CFaction::CkFactionPlayerData(v_dwFactionID, dwPlayerID))
	{
		return FALSE;
	}

	CFaction * poFaction = GetFaction(v_dwFactionID);
	if ( NULL == poFaction)
	{
		return FALSE;
	}

	poFaction->AddChatData(stChatData);
	return TRUE;
}

BOOL CFactionMgr::GetFactionBuileGroupByLevel(UINT32 dwFactionLevel, DT_FACTION_BUILD_LEVEL& stBuildFlag)
{
    SFactionUpgradeProp stProp = {0};
    BOOL bRet = CFactorPropMgr::Instance()->GetFactionUpgradeProp(dwFactionLevel, stProp);
    if ( !bRet)
    {
        return FALSE;
    }
	if ( 1 == stProp.byRareitemMark)
	{
		stBuildFlag.byRareitemLevel = 0;	
	}
	else
	{
		stBuildFlag.byRareitemLevel = CFactorPropMgr::Instance()->GetUnLockLevel(EFB_Rareitem);
	}

    if ( 1 == stProp.byGymnasiumMark)
    {
		stBuildFlag.byGymnasiumLevel = 0;
    }
	else
	{
		stBuildFlag.byGymnasiumLevel = CFactorPropMgr::Instance()->GetUnLockLevel(EFB_Gymnasium);
	}

	if ( 1 == stProp.byShadeMark )
	{
		stBuildFlag.byShadeLevel = 0;
	}
	else
	{
		stBuildFlag.byShadeLevel = CFactorPropMgr::Instance()->GetUnLockLevel(EFB_Shade);
	}

	if ( ESM_YES == CSinglePramMgr::Instance()->GetHotSpringFlag())
	{

	}
	else if ( 1 == stProp.byHotSpringMark)
	{
		stBuildFlag.byHotSpringLevel = 0;
	}
	else
	{
		stBuildFlag.byHotSpringLevel = CFactorPropMgr::Instance()->GetUnLockLevel(EFB_HotSpring);
	}

	if ( 1 == stProp.byColdPrison )
	{
		stBuildFlag.byColdPrisonLevel = 0;
	}
	else
	{
		stBuildFlag.byColdPrisonLevel = CFactorPropMgr::Instance()->GetUnLockLevel(EFB_ColdPrison);
	}

	if ( 1 == stProp.byFactionTower)
	{
		stBuildFlag.byFactionTowerLevel = 0;
	}
	else
	{
		stBuildFlag.byFactionTowerLevel = CFactorPropMgr::Instance()->GetUnLockLevel(EFB_FactionTower);
	}

    return TRUE;
}

UINT32 CFactionMgr::GetFactionBuildLevel(UINT32 dwFactionID, em_Faction_Build emBuildType)
{
	CFaction* oFaction = GetFaction(dwFactionID);
	if ( NULL == oFaction)
	{
		return 0;
	}
	return oFaction->GetFactionBuildLevel(emBuildType);
}

BOOL CFactionMgr::IsExitFactionName(TCHAR* aszFactionName)
{
    CFactionNameMapItr itr = m_mapFactionName.find(aszFactionName);
    if (itr != m_mapFactionName.end())
    {
        return TRUE;
    }
    return FALSE;
}

void CFactionMgr::TakeTopFactions(CListRankFaction &vecTopFaction, UINT16 byTakeCnt)
{
    if(byTakeCnt <= 0)
    {
        return;
    }

    //重新排名等级
    FactionSort();

    // 取出前几名门派
    for(CListRankFactionItr itr = m_listRankFaction.begin(); itr != m_listRankFaction.end(); ++itr)
    {
        CFaction *poFaction = *itr;
        vecTopFaction.push_back(poFaction);

        byTakeCnt--;

        if(byTakeCnt <= 0)
        {
            break;
        }
    }
}

CListRankFaction& CFactionMgr::GetRankFactions()
{
    //重新排名
    FactionSort();

    return m_listRankFaction;
}

BOOL CFactionMgr::RegFactionName(TCHAR* aszAddFactionName, CFaction *poFaction, TCHAR* aszDelFactionName)
{
    m_mapFactionName.erase(aszDelFactionName);
	m_mapFactionName[aszAddFactionName] = poFaction;
	return FALSE;
}

VOID CFactionMgr::SendHuntingActiviEndNtf()
{
	CMapFactionItr itr = m_mapFaction.begin();
	PKT_CLIGS_ACTIVITY_END_NTF stNtf;
	stNtf.byBuildID = EBK_FACTION;
	stNtf.byFuncTypeID = EFB_Shade;
	for ( ; itr != m_mapFaction.end(); itr++)
	{
		itr->second->SendMessageToAllFactionPlayer((TCHAR*)&stNtf, CLIGS_ACTIVITY_END_NTF);
		itr->second->SendMessageByShadePrompt();
	}
}

VOID CFactionMgr::FactionSort()
{
	m_listRankFaction.sort(SortByRank);
	UINT32 dwIndex = 1;
	for ( CListRankFactionItr itr = m_listRankFaction.begin(); itr != m_listRankFaction.end(); itr++)
	{
		(*itr)->GetDT_FACTION_BASE_DATA().dwFactionRank = dwIndex++;
	}
}

UINT16 CFactionMgr::GetApplyFactionNum(UINT32 dwPlayerID)
{
	list<UINT32> listFactionID;
	CFaction::CkFactionApplyPlayerData(dwPlayerID, listFactionID);
	return listFactionID.size();
}
