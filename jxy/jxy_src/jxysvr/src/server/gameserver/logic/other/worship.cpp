#include "worship.h"
#include "worshippropmgr.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <common/client/errdef.h>
#include <logic/other/phystrengthmgr.h>
#include <db/autosqlbuf.h>
#include "logic/record/recordmgr.h"
#include <logic/vip/vippropmgr.h>
#include <logic/lua/luamgr.h>
#include <logic/other/singleprammgr.h>
#include <logic/battle/battlemgr.h>
#include <logic/plunder/plunderbasepropmgr.h>
#include <logic/other/worshippropmgr.h>
#include <logic/race/racemgr.h>
#include "logic/medal/playermedal.h"
#include <logic/base/logicfunc.h>
#include <sdloggerimpl.h>
IMPLEMENT_CLASS_POOL_MGR(CWorship)
CWorship::CWorship()
{
    memset(&m_stDT_WORSHIP_DATA_LST, 0 , sizeof(m_stDT_WORSHIP_DATA_LST));
	m_mapDT_WORSHIP_DATA.clear();
}

CWorship::~CWorship()
{

}

BOOL CWorship::Init(DT_WORSHIP_DATA_LST& stDT_WORSHIP_DATA_LST, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }

    memcpy(&m_stDT_WORSHIP_DATA_LST, &stDT_WORSHIP_DATA_LST, sizeof(stDT_WORSHIP_DATA_LST));
    m_poOwner = poOwner;
	UINT16 wNum = m_stDT_WORSHIP_DATA_LST.wNum;
	for(UINT16 wIdx = 0; wIdx < wNum; wIdx++)
	{
		DT_WORSHIP_DATA& stDT_WORSHIP_DATA = m_stDT_WORSHIP_DATA_LST.astWorshipInfo[wIdx];		
		m_mapDT_WORSHIP_DATA[CLogicFunc::Get88Key(stDT_WORSHIP_DATA.byWorshipType, stDT_WORSHIP_DATA.byWorshipTypeLevel)] = &stDT_WORSHIP_DATA;
	}
	AddNewProp(CWorshipPropMgr::Instance()->GetRaceWorshipProp());
	AddNewProp(CWorshipPropMgr::Instance()->GetRDCWorshipProp());

    return TRUE;
}

VOID CWorship::AddNewProp(CLevel2WorshipPropMap& mapProp)
{
	for(CLevel2WorshipPropMapItr itr = mapProp.begin(); ((itr != mapProp.end()) && (m_stDT_WORSHIP_DATA_LST.wNum < MAX_WORSHIP_NUM)); itr++)
	{
		SWorshipProp& stProp = itr->second;
		if(m_mapDT_WORSHIP_DATA.find(CLogicFunc::Get88Key(stProp.byWorshipType, stProp.byWorshipTypeLevel)) == m_mapDT_WORSHIP_DATA.end())
		{
			DT_WORSHIP_DATA& stDT_WORSHIP_DATA = m_stDT_WORSHIP_DATA_LST.astWorshipInfo[m_stDT_WORSHIP_DATA_LST.wNum++];	
			stDT_WORSHIP_DATA.byWorshipType = stProp.byWorshipType;
			stDT_WORSHIP_DATA.byWorshipTypeLevel = stProp.byWorshipTypeLevel;
			stDT_WORSHIP_DATA.qwLastWorshipTime = 0;

			m_mapDT_WORSHIP_DATA[CLogicFunc::Get88Key(stDT_WORSHIP_DATA.byWorshipType, stDT_WORSHIP_DATA.byWorshipTypeLevel)] = &stDT_WORSHIP_DATA;
		}
	}
}

UINT16 CWorship::OpenWorshipTab(UINT8 byWorshipType, DT_WORSHIP_DATA_CLI_LST& stWorshipInfo)
{
	memset(&stWorshipInfo, 0, sizeof(stWorshipInfo));
	if((EWT_RACE == byWorshipType) && (!m_poOwner->GetBuild(EBK_RACEBUILD)) ||
		(EWT_RDC == byWorshipType) && (!m_poOwner->GetBuild(EBK_RDCHALLENGE)))
	{
		return ERR_OPEN_WORSHIP_TAB::ID_UNLOCK;
	}

	//没多少，直接遍历
	UINT16 wNum = m_stDT_WORSHIP_DATA_LST.wNum;
	for(UINT16 wIdx = 0; wIdx < wNum; wIdx++)
	{
		DT_WORSHIP_DATA& stDT_WORSHIP_DATA = m_stDT_WORSHIP_DATA_LST.astWorshipInfo[wIdx];
		if(stDT_WORSHIP_DATA.byWorshipType != byWorshipType)
		{
			continue;
		}
		DT_WORSHIP_DATA_CLI& stDT_WORSHIP_DATA_CLI = stWorshipInfo.astWorshipInfo[stWorshipInfo.wNum++];
		GetDT_WORSHIP_DATA_CLI(stDT_WORSHIP_DATA, stDT_WORSHIP_DATA_CLI);
	}
	return ERR_OPEN_WORSHIP_TAB::ID_SUCCESS;
}

VOID CWorship::GetDT_WORSHIP_DATA_CLI(DT_WORSHIP_DATA& stDT_WORSHIP_DATA, DT_WORSHIP_DATA_CLI& stDT_WORSHIP_DATA_CLI)
{
	SWorshipProp* pstProp = CWorshipPropMgr::Instance()->GetWorshipProp(stDT_WORSHIP_DATA.byWorshipType, stDT_WORSHIP_DATA.byWorshipTypeLevel);
	if(NULL == pstProp)
	{
		DBG_INFO(_SDT("[%s: %d]: SWorshipProp WorshipType[%d] WorshipTypeLevel[] is NULL!"), MSG_MARK, stDT_WORSHIP_DATA.byWorshipType, stDT_WORSHIP_DATA.byWorshipTypeLevel);  
		return;
	}
	if(EWT_RACE == stDT_WORSHIP_DATA.byWorshipType)
	{
		SPlayerBaseData* pstPlayerBaseData = CRaceMgr::Instance()->GetPlayerBaseData(stDT_WORSHIP_DATA.byWorshipTypeLevel);
		if(pstPlayerBaseData)
		{
			SDStrncpy(stDT_WORSHIP_DATA_CLI.aszDispName, pstPlayerBaseData->aszDispName, USERNAME_LEN-1);
			stDT_WORSHIP_DATA_CLI.dwPlayerID = pstPlayerBaseData->dwID;
			stDT_WORSHIP_DATA_CLI.dwCoachID = pstPlayerBaseData->wMainHeroKindID;
		}
	}

	stDT_WORSHIP_DATA_CLI.byWorshipTypeLevel = stDT_WORSHIP_DATA.byWorshipTypeLevel;
	stDT_WORSHIP_DATA_CLI.dwNeedCoin = pstProp->dwNeedCoin;
	stDT_WORSHIP_DATA_CLI.dwNeedGold = pstProp->dwNeedGold;
	stDT_WORSHIP_DATA_CLI.byNeedVipLevel = pstProp->byNeedVipLevel;
	stDT_WORSHIP_DATA_CLI.dwEncJingJie = pstProp->dwEncJingJie;
	stDT_WORSHIP_DATA_CLI.dwEncPurpleGas = pstProp->dwEncPurpleGas;
	stDT_WORSHIP_DATA_CLI.byHaveWorship = IsToday(stDT_WORSHIP_DATA.qwLastWorshipTime) ? 1 : 0;	

}

UINT16 CWorship::Worship(UINT8 byWorshipType, UINT8 byWorshipTypeLevel, DT_WORSHIP_DATA_CLI_LST& stWorshipInfo)
{
	if((EWT_RACE == byWorshipType) && (!m_poOwner->GetBuild(EBK_RACEBUILD)) ||
		(EWT_RDC == byWorshipType) && (!m_poOwner->GetBuild(EBK_RDCHALLENGE)))
	{
		return ERR_WORSHIP::ID_UNLOCK;
	}

	OpenWorshipTab(byWorshipType, stWorshipInfo);
	
	if(!CKCanWorship(byWorshipType, FALSE, byWorshipTypeLevel))
	{
		return ERR_WORSHIP::ID_HAVE_DONE;
	}

	SWorshipProp* pstProp = CWorshipPropMgr::Instance()->GetWorshipProp(byWorshipType, byWorshipTypeLevel);
	if(NULL == pstProp)
	{
		DBG_INFO(_SDT("[%s: %d]: SWorshipProp WorshipType[%d] WorshipTypeLevel[] is NULL!"), MSG_MARK, byWorshipType, byWorshipTypeLevel);  
		return ERR_WORSHIP::ID_NO_DATA;
	}

	CDT_WORSHIP_DATAMapItr itr = m_mapDT_WORSHIP_DATA.find(CLogicFunc::Get88Key(byWorshipType, byWorshipTypeLevel));
	if(itr == m_mapDT_WORSHIP_DATA.end())
	{
		return ERR_WORSHIP::ID_NO_DATA;
	}
	DT_WORSHIP_DATA* pstDT_WORSHIP_DATA = itr->second;

	if(IsToday(pstDT_WORSHIP_DATA->qwLastWorshipTime))
	{
		return ERR_WORSHIP::ID_HAVE_DONE;
	}
	
	if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin < pstProp->dwNeedCoin)
	{
		return ERR_COMMON::ID_COIN_NOT_ENOUGH;
	}
	
	if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < pstProp->dwNeedGold)
	{
		return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
	}

	if(m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel < pstProp->byNeedVipLevel)
	{
		return ERR_COMMON::ID_VIP_LEVEL_NOT_REACH;
	}
	CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
	if ( NULL != poPlayerMedal)
	{
		//拜神
		if ( 2 == byWorshipType )
		{
			if ( 3 == byWorshipTypeLevel)
			{
				poPlayerMedal->AddWorshipGod3Num();
			}
			else if ( 2 == byWorshipTypeLevel)
			{
				poPlayerMedal->AddWorshipGod2Num();
			}
			else if ( 1 == byWorshipTypeLevel)
			{
				poPlayerMedal->AddWorshipGod1Num();
			}
			poPlayerMedal->CheckMedalBySmallType(EMST_WORSHIP_PLAYER);
		}
		//膜拜
		else if(1 == byWorshipType)
		{
			if ( 3 == byWorshipTypeLevel)
			{
				poPlayerMedal->AddWorshipPlayer3Num();
			}
			else if ( 2 == byWorshipTypeLevel)
			{
				poPlayerMedal->AddWorshipPlayer2Num();
			}
			else if ( 1 == byWorshipTypeLevel)
			{
				poPlayerMedal->AddWorshipPlayer1Num();
			}
			poPlayerMedal->CheckMedalBySmallType(EMST_WORSHIP_PLAYER);
		}
	}
	
	if(0 != pstProp->dwNeedCoin)
	{
		m_poOwner->DecCoin(pstProp->dwNeedCoin, CRecordMgr::EDCT_WORSHIP, pstProp->byWorshipType, pstProp->byNeedVipLevel);
	}
	if(0 != pstProp->dwNeedGold)
	{
		m_poOwner->DecGold(pstProp->dwNeedGold, CRecordMgr::EDGT_WORSHIP, pstProp->byWorshipType, pstProp->byNeedVipLevel);
	}

	if(0 != pstProp->dwEncJingJie)
	{
		m_poOwner->AddJingJie(pstProp->dwEncJingJie, CRecordMgr::EAJT_WORSHIP);
	}
	if(0 != pstProp->dwEncPurpleGas)
	{
		m_poOwner->AddPurpleGas(pstProp->dwEncPurpleGas, CRecordMgr::EAPGT_WORSHIP);
	}

	pstDT_WORSHIP_DATA->qwLastWorshipTime = SDTimeSecs();
	OpenWorshipTab(byWorshipType, stWorshipInfo);

	return ERR_WORSHIP::ID_SUCCESS;
}


BOOL CWorship::GetPrompt(DT_FUNC_COMMON_PROMPT& stPvpPrompt)
{
	memset(&stPvpPrompt, 0, sizeof(&stPvpPrompt));
	stPvpPrompt.byBuildKindID = EBK_PVP;	

	if(CKCanWorship(EWT_RACE, TRUE))
	{
		stPvpPrompt.byFuncFlag = EPP_WORSHIP;
		return TRUE;
	}
	if(CKCanWorship(EWT_RDC, TRUE))
	{
		stPvpPrompt.byFuncFlag = EPP_LIGHTING;
		return TRUE;
	}	

	return FALSE;
}

BOOL CWorship::CKCanWorship(UINT8 byWorshipType, BOOL bPrompt, UINT8 byWorshipTypeLevel)
{
	if((EWT_RACE == byWorshipType) && (!m_poOwner->GetBuild(EBK_RACEBUILD)) ||
		(EWT_RDC == byWorshipType) && (!m_poOwner->GetBuild(EBK_RDCHALLENGE)))
	{
		return FALSE;
	}

	//个数不多，直接遍历	
	BOOL bCanWorship = FALSE;
	for(CDT_WORSHIP_DATAMapItr itr = m_mapDT_WORSHIP_DATA.begin(); itr != m_mapDT_WORSHIP_DATA.end(); itr++)
	{
		DT_WORSHIP_DATA* pstDT_WORSHIP_DATA = itr->second;
		if((byWorshipType == pstDT_WORSHIP_DATA->byWorshipType) && ((byWorshipTypeLevel == pstDT_WORSHIP_DATA->byWorshipTypeLevel) || (0 == byWorshipTypeLevel)))
		{
			if(IsToday(pstDT_WORSHIP_DATA->qwLastWorshipTime))
			{
				//提示，只提示一个
				if(bPrompt)
				{
					return FALSE;
				}	
			}
			else
			{
				if(!bPrompt)
				{
					return TRUE;
				}	
				bCanWorship = TRUE;
			}
		}		
	}

	return bCanWorship;
}
