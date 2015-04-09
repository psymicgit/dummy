#include "logic/soul/playersoul.h"
#include "logic/soul/soul.h"
#include "errdef.h"
#include "logic/soul/soulpropmgr.h"
#include "logic/base/logicfunc.h"
#include "logic/player/player.h"
IMPLEMENT_CLASS_POOL_MGR(CPlayerSoul)

CPlayerSoul::CPlayerSoul() {
	memset(&m_stSoulData, 0, sizeof(DT_PLAYER_SOUL_DATA));
}

CPlayerSoul::~CPlayerSoul() {
	memset(&m_stSoulData, 0, sizeof(DT_PLAYER_SOUL_DATA));
}

BOOL CPlayerSoul::Init(const DT_PLAYER_SOUL_DATA& stLstData, CPlayer* poPlayer) {
	if ( NULL == poPlayer) 
	{
		return FALSE;
	}
	m_poOwner = poPlayer;
	/////////////////////////////过滤错误数据////////////////////////////
	DT_SOUL_LST_DATA stSoulBagData = {0};
	DT_SOUL_POTENCY_LST_DATA stPotencyData = {0};
	UINT16 wSoulNum = 0;
	for ( UINT16 wIdx = 0; wIdx < stLstData.stSoulBagData.wSoulNum; wIdx++)
	{
		const SoulBaseCfg* poBaseCfg = CSoulPropMgr::Instance()->GetSoulBaseCfg( stLstData.stSoulBagData.astSoulLstData[wIdx].wSoulID);
		if ( NULL != poBaseCfg )
		{
			stSoulBagData.astSoulLstData[wSoulNum] = stLstData.stSoulBagData.astSoulLstData[wIdx];
			stSoulBagData.astSoulLstData[wSoulNum].bySoulStar = poBaseCfg->ucStar;
			stSoulBagData.astSoulLstData[wSoulNum].bySoulType = poBaseCfg->ucSoulType;
			stPotencyData.astSoulPotencyLstData[wSoulNum] = stLstData.stSoulPotencyBagData.astSoulPotencyLstData[wIdx];
			if ( 20 <= stSoulBagData.astSoulLstData[wSoulNum].bySoulLevel)
			{
				SYS_CRITICAL("[%s:%d] Over 20: SoulID = %u, SoulLevel = %d, PlayerID = %u", MSG_MARK, stLstData.stSoulBagData.astSoulLstData[wIdx].wSoulID, stLstData.stSoulBagData.astSoulLstData[wIdx].bySoulLevel, m_poOwner->GetID());
			}
			wSoulNum++;
		}
		else
		{
			SYS_CRITICAL("[%s:%d] ID NOT EXIST: SoulID = %u, SoulLevel = %d, PlayerID = %u", MSG_MARK, stLstData.stSoulBagData.astSoulLstData[wIdx].wSoulID, stLstData.stSoulBagData.astSoulLstData[wIdx].bySoulLevel, m_poOwner->GetID());
		}
	}
	stSoulBagData.wSoulNum = wSoulNum;
	stPotencyData.wSoulNum = wSoulNum;
	/////////////////////////////过滤错误数据////////////////////////////
	
	m_stSoulData = stLstData;
	m_stSoulData.stSoulBagData = stSoulBagData;
	m_stSoulData.stSoulPotencyBagData = stPotencyData;
	initSoulData();
	
	return TRUE;
}

DT_PLAYER_SOUL_DATA& CPlayerSoul::GetDT_PLAYER_SOUL_DATA() {
	return m_stSoulData;
}

// 添加魂到背包里
BOOL CPlayerSoul::AddSoulToBag(CSoul* pSoul) {
	if (NULL == pSoul) {
		return FALSE;
	}
	
	DT_SOUL_DATA& rSouData = pSoul->GetDT_SOUL_DATA();	
	for (size_t n = 0; n < MAX_SOUL_BAG_NUM; ++n) {
		DT_SOUL_DATA& rBagSoulData = m_stSoulData.stSoulBagData.astSoulLstData[n];
		if (0 == rBagSoulData.wSoulID) {
			rBagSoulData = rSouData;
			m_stSoulData.stSoulPotencyBagData.astSoulPotencyLstData[n] = pSoul->GetDT_SOUL_POTENCY_DATA();
			if (0 == rSouData.dwSoulIdx) {
				rBagSoulData.dwSoulIdx = GenerateSoulIdx(rSouData.wSoulID);
			}
			
			++ m_stSoulData.stSoulBagData.wSoulNum;
			m_stSoulData.stSoulPotencyBagData.wSoulNum = m_stSoulData.stSoulBagData.wSoulNum;
			CSoul& soul = m_mpSoulData[CLogicFunc::Get3216Key(rSouData.wSoulID, rBagSoulData.dwSoulIdx)];
			soul.Init(m_poOwner, rBagSoulData, pSoul->GetDT_SOUL_POTENCY_DATA());
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CPlayerSoul::DelSoulFromBag(CSoul* pSoul) {
	if (NULL == pSoul) {
		return FALSE;
	}

	if (0 >= m_stSoulData.stSoulBagData.wSoulNum) {
		PRINT_ERR_INFO;
		return FALSE;
	}

	DT_SOUL_DATA& rSoulData = pSoul->GetDT_SOUL_DATA();
	for (size_t n = 0; n < MAX_SOUL_BAG_NUM; ++n) {
		if (m_stSoulData.stSoulBagData.astSoulLstData[n].wSoulID == rSoulData.wSoulID
			&& m_stSoulData.stSoulBagData.astSoulLstData[n].dwSoulIdx == rSoulData.dwSoulIdx)
		{
			for (size_t nIdx = n; nIdx < MAX_SOUL_BAG_NUM; ++nIdx) {
				if (nIdx != (MAX_SOUL_BAG_NUM - 1)) {
					memcpy(&m_stSoulData.stSoulBagData.astSoulLstData[nIdx], &m_stSoulData.stSoulBagData.astSoulLstData[nIdx + 1], sizeof(DT_SOUL_DATA));
					memcpy(&m_stSoulData.stSoulPotencyBagData.astSoulPotencyLstData[nIdx], &m_stSoulData.stSoulPotencyBagData.astSoulPotencyLstData[nIdx + 1], sizeof(DT_SOUL_POTENCY_DATA));
				} else {
					memset(&m_stSoulData.stSoulBagData.astSoulLstData[nIdx], 0, sizeof(DT_SOUL_DATA));
					memset(&m_stSoulData.stSoulPotencyBagData.astSoulPotencyLstData[nIdx], 0, sizeof(DT_SOUL_POTENCY_DATA));
				}
			}
			
			-- m_stSoulData.stSoulBagData.wSoulNum;
			m_stSoulData.stSoulPotencyBagData.wSoulNum = m_stSoulData.stSoulBagData.wSoulNum;
			UINT64 ullKey = CLogicFunc::Get3216Key(UINT32(rSoulData.wSoulID), UINT16(rSoulData.dwSoulIdx));
			CMapSoulItr itor = m_mpSoulData.find(ullKey);
			if (m_mpSoulData.end() != itor) {
				m_mpSoulData.erase(itor);
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CPlayerSoul::IsSoulBagFull() {
	for (size_t n = 0; n < MAX_SOUL_BAG_NUM; ++n) {
		if (0 == m_stSoulData.stSoulBagData.astSoulLstData[n].wSoulID) {
			return FALSE;
		}
	}
	return TRUE;
}

// 是否有魂
CSoul* CPlayerSoul::GetSoul(UINT16 usSoulId, UINT32 unIndex) {
	if (0 == usSoulId) {
		return NULL;
	}

	UINT64 ullKey = CLogicFunc::Get3216Key(UINT32(usSoulId), UINT16(unIndex));
	CMapSoulItr itor = m_mpSoulData.find(ullKey);
	if (m_mpSoulData.end() != itor) {
		return &itor->second;
	}

	return NULL;
}

VOID CPlayerSoul::GetSoulBagDataCli(DT_SOUL_DATA_LST_CLI& rSoulInfo, DT_SOUL_POTENCY_DATA_LST_CLI* pstBagSoulPotencyInfo) {
	rSoulInfo.wSoulNum = m_stSoulData.stSoulBagData.wSoulNum;
	if ( pstBagSoulPotencyInfo)
	{
		pstBagSoulPotencyInfo->wSoulNum = rSoulInfo.wSoulNum;
	}
	

	for (size_t n = 0; n < rSoulInfo.wSoulNum; ++n) {
		CSoul soul;
		soul.Init(m_poOwner, m_stSoulData.stSoulBagData.astSoulLstData[n], m_stSoulData.stSoulPotencyBagData.astSoulPotencyLstData[n]);
		soul.GetSoulCli(rSoulInfo.astSoulLstData[n]);
		if ( pstBagSoulPotencyInfo )
		{
			soul.GetSoulPotencyCli(pstBagSoulPotencyInfo->astSoulLstData[n]);
		}
		
	}
}

UINT8 CPlayerSoul::GetSoulBagLeftSize() {
	for (size_t n = 0; n < MAX_SOUL_BAG_NUM; ++n) {
		if (0 == m_stSoulData.stSoulBagData.astSoulLstData[n].wSoulID) {
			return MAX_SOUL_BAG_NUM - n;
		}
	}
	return 0;
}

void CPlayerSoul::initSoulData() {
	m_mpSoulData.clear();
	m_mpSoulIdx.clear();
	for (size_t n = 0; n < m_stSoulData.stSoulBagData.wSoulNum && n < MAX_SOUL_BAG_NUM; ++n) {
		DT_SOUL_DATA& rSoulData = m_stSoulData.stSoulBagData.astSoulLstData[n];
		if (0 != rSoulData.wSoulID) {
			rSoulData.dwSoulIdx = GenerateSoulIdx(rSoulData.wSoulID);
			
			CSoul& soul = m_mpSoulData[CLogicFunc::Get3216Key(rSoulData.wSoulID, rSoulData.dwSoulIdx)];
			soul.Init(m_poOwner, rSoulData, m_stSoulData.stSoulPotencyBagData.astSoulPotencyLstData[n]);
		}
	}
}

UINT16 CPlayerSoul::GenerateSoulIdx(UINT32 unSoulId) {
	UINT16& wSoulIdx = m_mpSoulIdx[unSoulId];
	return ++ wSoulIdx;
}

VOID CPlayerSoul::UpdateSoulData(CSoul* pSoul) {
	if (NULL == pSoul) {
		return;
	}

	const DT_SOUL_DATA& rSoulData = pSoul->GetDT_SOUL_DATA();
	for (size_t n = 0; n < MAX_SOUL_BAG_NUM; ++n) {
		DT_SOUL_DATA& rBagSoulData = m_stSoulData.stSoulBagData.astSoulLstData[n];
		if (rBagSoulData.wSoulID == rSoulData.wSoulID
			&& rBagSoulData.dwSoulIdx == rSoulData.dwSoulIdx)
		{
			rBagSoulData = rSoulData;
			m_stSoulData.stSoulPotencyBagData.astSoulPotencyLstData[n] = pSoul->GetDT_SOUL_POTENCY_DATA();
			return;
		}
	}
}
