#include "onlineencouragerecord.h"
#include "onlineencouragemgr.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/other/singleprammgr.h>
#include "logic/record/recordmgr.h"
#include <logic/base/basedefine.h>
#include <logic/item/itemmgr.h>
#include <sdloggerimpl.h>

IMPLEMENT_CLASS_POOL_MGR(COnlineEncourageRecord)
COnlineEncourageRecord::COnlineEncourageRecord()
{
	memset(&m_stDT_ONLINE_ENCOURAGE_RECORD_DATA, 0 ,sizeof(m_stDT_ONLINE_ENCOURAGE_RECORD_DATA));
}

COnlineEncourageRecord::~COnlineEncourageRecord()
{

}

BOOL COnlineEncourageRecord::Init(DT_ONLINE_ENCOURAGE_RECORD_DATA &stDT_ONLINE_ENCOURAGE_RECORD_DATA, CPlayer* poOwner)
{
	if(NULL == poOwner)
	{
		return FALSE;
	}

	memcpy(&m_stDT_ONLINE_ENCOURAGE_RECORD_DATA, &stDT_ONLINE_ENCOURAGE_RECORD_DATA, sizeof(DT_ONLINE_ENCOURAGE_RECORD_DATA));
	m_poOwner = poOwner;	

	return TRUE;
}

UINT16 COnlineEncourageRecord::GetCurEncourage(UINT16& wRecvCountDown, DT_ONLINE_ENCOURAGE_DATA& stOnlineEncourage)
{	
	//if(m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byRecvDays >= CSinglePramMgr::Instance()->GetOnlineEncourageKeepDays())
	//{
	//	USR_INFO( _SDT("COnlineEncourageRecord time too long,byRecvDays:%d,GetOnlineEncourageKeepDays:%d"), 
	//		m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byRecvDays, CSinglePramMgr::Instance()->GetOnlineEncourageKeepDays() );
	//	return ERR_RECV_ONLINE_ENCOURAGE::ID_HAVE_NOT_ENCOURAGE;
	//}

	//第一次领取或登录后第一次领取，以登录时间为准
	//BOOL bToday=IsToday( m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.qwLastRecvTime  );
	//if((0 == m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.qwLastRecvTime) || (m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.qwLastRecvTime< m_poOwner->GetTodayFirstLoginTime()))
	if(0 == m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.qwLastRecvTime)
	{
		m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.qwLastRecvTime = m_poOwner->GetTodayFirstLoginTime();
		m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex = 0;
	}

	SOnlineEncourageProp* pstOnlineEncourageProp = NULL;
	//今天
	if(IsToday(m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.qwLastRecvTime))
	{
		//DBG_INFO( _SDT( "already get online encourage, PlayerID:%d,last time get:%lld" ), 
			//m_poOwner->GetID(), m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.qwLastRecvTime );
		if(m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex >= COnlineEncourageMgr::Instance()->GetMaxPerDay())
		{
			//DBG_INFO( _SDT( "last get:%d bigger than max count:%d perday" ), 
				//m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex, COnlineEncourageMgr::Instance()->GetMaxPerDay() );
			return ERR_RECV_ONLINE_ENCOURAGE::ID_HAVE_RECV_ALL;
		}		
		else
		{
			pstOnlineEncourageProp = COnlineEncourageMgr::Instance()->GetEncourage(m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex + 1);
			if(NULL == pstOnlineEncourageProp)
			{
				DBG_INFO( _SDT( "pstOnlineEncourageProp is NULL, m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex + 1=%d" ), 
					m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex + 1 );
				RETURN_OTHER_ERR;
			}
			INT64 qwGap = SDTimeSecs() - m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.qwLastRecvTime;
			if(qwGap < 0)
			{
				wRecvCountDown = 0;
			}
			else if (qwGap > pstOnlineEncourageProp->wRecvCD )
			{
				wRecvCountDown = 0;
			}
			else
			{
				wRecvCountDown  = pstOnlineEncourageProp->wRecvCD - (UINT16)qwGap;
			}
		}
	}
	else
	{
		m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex = 0;
		pstOnlineEncourageProp = COnlineEncourageMgr::Instance()->GetEncourage(1);
		if(NULL == pstOnlineEncourageProp)
		{
			DBG_INFO( _SDT( "pstOnlineEncourageProp isNULL, idx=1" ));
			RETURN_OTHER_ERR;
		}
		wRecvCountDown = 0;
	}

	memcpy(&stOnlineEncourage, &(pstOnlineEncourageProp->stDT_ONLINE_ENCOURAGE_DATA), sizeof(stOnlineEncourage));

	

	return ERR_RECV_ONLINE_ENCOURAGE::ID_SUCCESS;
}

UINT16 COnlineEncourageRecord::RecvEncourage(UINT16& wRecvCountDown, UINT8& byNextOnlineEncourageFlag, 
	DT_ONLINE_ENCOURAGE_DATA& stNextOnlineEncourage, DT_DATA_AFTER_ONLINE_ENCOURAGE& stCurPlayerInfo)
{
	UINT8 byKeepDays = CSinglePramMgr::Instance()->GetOnlineEncourageKeepDays();

	////上次领取不是今天，也可以还有在线奖励
	//if((m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byRecvDays > byKeepDays) || 
	//	(m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byRecvDays == byKeepDays) )
	//	/*((m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byRecvDays == byKeepDays) &&(!IsToday(m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.qwLastRecvTime))))*/
	//{
	//	return ERR_RECV_ONLINE_ENCOURAGE::ID_HAVE_NOT_ENCOURAGE;
	//}

	//判断时间
	SOnlineEncourageProp* pstOnlineEncourageProp = NULL;
	BOOL bToday = FALSE;
	//今天
	if(IsToday(m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.qwLastRecvTime))
	{
		bToday = TRUE;

		if(m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex >= COnlineEncourageMgr::Instance()->GetMaxPerDay())
		{
			return ERR_RECV_ONLINE_ENCOURAGE::ID_HAVE_RECV_ALL;
		}		
		else
		{
			pstOnlineEncourageProp = COnlineEncourageMgr::Instance()->GetEncourage(m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex + 1);
			if(NULL == pstOnlineEncourageProp)
			{
				RETURN_OTHER_ERR;
			}
			INT64 qwGap = SDTimeSecs() - m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.qwLastRecvTime;
			if(qwGap < 0)
			{
				wRecvCountDown = 0;
			}
			else if (qwGap > pstOnlineEncourageProp->wRecvCD )
			{
				wRecvCountDown = 0;
			}
			else
			{
				wRecvCountDown  = pstOnlineEncourageProp->wRecvCD - (UINT16)qwGap;
			}
		}
	}
	else
	{
		bToday = FALSE;
		m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex = 0;
		pstOnlineEncourageProp = COnlineEncourageMgr::Instance()->GetEncourage(1);
		if(NULL == pstOnlineEncourageProp)
		{
			RETURN_OTHER_ERR;
		}
		wRecvCountDown = 0;		
	}

	//
	if(0 != wRecvCountDown)
	{
		return ERR_RECV_ONLINE_ENCOURAGE::ID_IN_CD;
	}
	
	//奖励装备只会给叠加数为1
	vector<UINT16> vecItemID;
	UINT8 byItemNum = pstOnlineEncourageProp->stDT_ONLINE_ENCOURAGE_DATA.byItemNum;
	DT_ITEM_DATA_CLI* pstItemInfoList = pstOnlineEncourageProp->stDT_ONLINE_ENCOURAGE_DATA.astItemInfoList;
	for(UINT8 byIdx = 0; byIdx < byItemNum; byIdx++)
	{
		vecItemID.push_back(pstItemInfoList->wKindID);
		pstItemInfoList++;
	}
	if(CItemMgr::Instance()->CkBagFull(m_poOwner, vecItemID))
	{
		return ERR_RECV_ONLINE_ENCOURAGE::ID_BAG_FULL;
	}

	//奖励
	Encourage(pstOnlineEncourageProp->stDT_ONLINE_ENCOURAGE_DATA);
	
	//修改信息
	m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex++;
	m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.qwLastRecvTime = SDTimeSecs();
	if(FALSE == bToday)
	{
		m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byRecvDays++;
	}	

	if(ERR_RECV_ONLINE_ENCOURAGE::ID_SUCCESS == GetCurEncourage(wRecvCountDown, stNextOnlineEncourage))
	{
		byNextOnlineEncourageFlag = 1;
	}

	const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = m_poOwner->GetDT_PLAYER_BASE_DATA();
	stCurPlayerInfo.qwCurCoin = stDT_PLAYER_BASE_DATA.qwCoin;
	stCurPlayerInfo.qwCurGold = stDT_PLAYER_BASE_DATA.qwGold;
	stCurPlayerInfo.qwCurScience = stDT_PLAYER_BASE_DATA.qwScience;
	stCurPlayerInfo.wPhyStrength = stDT_PLAYER_BASE_DATA.wPhyStrength;
	
	//SetSaveState(ESS_WANT_SAVE);

	return ERR_RECV_ONLINE_ENCOURAGE::ID_SUCCESS;
}


VOID COnlineEncourageRecord::Encourage(DT_ONLINE_ENCOURAGE_DATA& stDT_ONLINE_ENCOURAGE_DATA)
{
	//DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = m_poOwner->GetDT_PLAYER_BASE_DATA();

	//stDT_PLAYER_BASE_DATA.qwCoin += stDT_ONLINE_ENCOURAGE_DATA.dwCoin;
	//stDT_PLAYER_BASE_DATA.qwGold += stDT_ONLINE_ENCOURAGE_DATA.dwGold;
	//stDT_PLAYER_BASE_DATA.qwScience += stDT_ONLINE_ENCOURAGE_DATA.dwScience;
	//stDT_PLAYER_BASE_DATA.wPhyStrength += stDT_ONLINE_ENCOURAGE_DATA.wPhyStrength;
	m_poOwner->AddCoin(stDT_ONLINE_ENCOURAGE_DATA.dwCoin, CRecordMgr::EACT_ONLINEENCOURAGE, m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byRecvDays, m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex);
	m_poOwner->AddGold(stDT_ONLINE_ENCOURAGE_DATA.dwGold, CRecordMgr::EGGT_ONLINEENCOURAGE, m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byRecvDays, m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex);
	m_poOwner->AddScience(stDT_ONLINE_ENCOURAGE_DATA.dwScience, CRecordMgr::EAST_ONLINEENCOURAGE, m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byRecvDays, m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex);
	m_poOwner->AddPhyStrength(stDT_ONLINE_ENCOURAGE_DATA.wPhyStrength, CRecordMgr::EAPST_ONLINEENCOURAGE, m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byRecvDays, m_stDT_ONLINE_ENCOURAGE_RECORD_DATA.byLastRecvIndex);

	UINT8 byNum = stDT_ONLINE_ENCOURAGE_DATA.byItemNum;
	byNum =	byNum > MAX_ENCOURAGE_ITEM_KIND_NUM ? MAX_ENCOURAGE_ITEM_KIND_NUM : byNum;
	for(UINT8 byIdx = 0; byIdx < byNum; byIdx++)
	{
		DT_ITEM_DATA_CLI& stDT_ITEM_DATA_CLI = stDT_ONLINE_ENCOURAGE_DATA.astItemInfoList[byIdx];
		ECreateItemRet eRet = ECIR_SUCCESS;
		CItemMgr::Instance()->CreateItemIntoBag(m_poOwner, stDT_ITEM_DATA_CLI.wKindID, stDT_ITEM_DATA_CLI.wPileNum, eRet, 0, CRecordMgr::EAIT_ONLINEENCOURAGE);
	}
}


