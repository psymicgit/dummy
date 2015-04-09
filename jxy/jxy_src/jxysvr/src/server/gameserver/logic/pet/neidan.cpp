///<------------------------------------------------------------------------------
//< @file:   logic\pet\neidan.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê10ÔÂ20ÈÕ 9:28:19
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/pet/neidan.h"
#include "logic/pet/petdef.h"
#include "logic/pet/petpropmng.h"
#include "logic/pet/pet.h"
#include "logic/pet/playerpet.h"


IMPLEMENT_CLASS_POOL_MGR(CPetNeiDan);

CPetNeiDan::CPetNeiDan()
{
	memset(&m_stPetNeiDan, 0, sizeof(DT_NEIDAN_DATA));
	m_poPet = NULL;
}

CPetNeiDan::~CPetNeiDan()
{
	memset(&m_stPetNeiDan, 0, sizeof(DT_NEIDAN_DATA));
	m_poPet = NULL;
}

BOOL CPetNeiDan::Init(CPlayer* poPlayer, const DT_NEIDAN_DATA& stNeiDan)
{
	if ( NULL == poPlayer)
	{
		return FALSE;
	}

	m_poOwner = poPlayer;
	m_stPetNeiDan = stNeiDan;

	return TRUE;
}

SPetNeiDanProp* CPetNeiDan::GetBaseProp() 
{
	return CPetPropMng::Instance()->GetPetNeiDanProp( m_stPetNeiDan.wNeiDanID);
}

BOOL CPetNeiDan::GetUpdataProp(SPetNeiDanUpdateProp*& poCurUpdataProp, SPetNeiDanUpdateProp*& poBeforUpdataProp)
{
	SPetNeiDanProp* poBaseProp = GetBaseProp();
	if ( NULL == poBaseProp)
	{
		return FALSE;
	}
	
	poCurUpdataProp = CPetPropMng::Instance()->GetPetNeiDanUpdataProp( m_stPetNeiDan.wNeiDanLevel, poBaseProp->byQuality, poBaseProp->byType);
	if ( NULL == poCurUpdataProp)
	{
		return FALSE;
	}

	poBeforUpdataProp = CPetPropMng::Instance()->GetPetNeiDanUpdataProp( m_stPetNeiDan.wNeiDanLevel - 1, poBaseProp->byQuality, poBaseProp->byType);
	return TRUE;
	
}

VOID CPetNeiDan::SetIdx(UINT16 wIdx)
{
	m_stPetNeiDan.wNeiDanIdx = wIdx;
}

VOID CPetNeiDan::SetPet(CPet* poPet)
{
	m_poPet = poPet;
}

BOOL CPetNeiDan::Dress(CPet* poPet)
{
	SPetNeiDanProp* poProp = GetBaseProp();
	if ( NULL == poProp)
	{
		return FALSE;
	}
	CPlayerPet* poPlayerPet = m_poOwner->GetPlayerPet();
	if ( NULL == poPlayerPet)
	{
		return FALSE;
	}

	CPetNeiDan* poUnDress = poPet->UnDressNeiDan(poProp->byWuXing);
	poPet->DressNeiDan(this);
	if ( (NULL != poUnDress) && (poUnDress != this) )
	{
		poPlayerPet->AddNeiDanToBag( poUnDress );
	}
	m_poPet = poPet;
	return TRUE;
}

BOOL CPetNeiDan::UnDress()
{
	if ( NULL != m_poPet)
	{
		SPetNeiDanProp* poProp = GetBaseProp();
		if ( NULL == poProp)
		{
			return FALSE;
		}

		CPlayerPet* poPlayerPet = m_poOwner->GetPlayerPet();
		if ( NULL == poPlayerPet)
		{
			return FALSE;
		}

		if( !poPlayerPet->AddNeiDanToBag( this ))
		{
			return FALSE;
		}

		m_poPet->UnDressNeiDan(poProp->byWuXing);
		m_poPet = NULL;
		
	}
	return TRUE;
}

VOID CPetNeiDan::GetNeiDanCli(DT_NEIDAN_CLI& stCli)
{
	SPetNeiDanProp* poBaseProp = GetBaseProp();
	SPetNeiDanUpdateProp* poCurUpdataProp = NULL;
	SPetNeiDanUpdateProp* poBeforUpdataProp = NULL;
	if ( NULL == poBaseProp || !GetUpdataProp(poCurUpdataProp, poBeforUpdataProp))
	{
		return ;
	}
	UINT64 qwBeferExperience = 0;
	if ( NULL != poBeforUpdataProp)
	{
		qwBeferExperience = poBeforUpdataProp->qwExperience;
	}
	stCli.stNeiDan.wItemID = m_stPetNeiDan.wNeiDanID;
	stCli.stNeiDan.wItemIdx = m_stPetNeiDan.wNeiDanIdx;
	stCli.wNeiDanLevel = m_stPetNeiDan.wNeiDanLevel;
	stCli.dwCostCoin = poBaseProp->dwCostCoin;
	stCli.dwLingLi = poBaseProp->dwBaseLingLi + poBaseProp->dwIncLingLi * m_stPetNeiDan.wNeiDanLevel;
	stCli.wDressReq = poBaseProp->wDressReq;
	stCli.dwMaxExperience = static_cast<UINT32>(poCurUpdataProp->qwExperience - qwBeferExperience);
	stCli.dwCurExperience = static_cast<UINT32>(m_stPetNeiDan.qwNeiDanExcpetion - qwBeferExperience);
	stCli.qwBackExperience = poBaseProp->dwBaseExperience + m_stPetNeiDan.qwNeiDanExcpetion;
}

VOID CPetNeiDan::GetNeiDanMark(DT_ITEM_MARK_DATA& stCli)
{
	stCli.wItemID = m_stPetNeiDan.wNeiDanID;
	stCli.wItemIdx = m_stPetNeiDan.wNeiDanIdx;
}

UINT64 CPetNeiDan::GetBackExperience()
{
	SPetNeiDanProp* poBaseProp = GetBaseProp();
	if ( NULL == poBaseProp)
	{
		return 0;
	}
	return poBaseProp->dwBaseExperience + m_stPetNeiDan.qwNeiDanExcpetion;
}

UINT32 CPetNeiDan::GetBackCoin()
{
	SPetNeiDanProp* poBaseProp = GetBaseProp();
	if ( NULL == poBaseProp)
	{
		return 0;
	}
	return poBaseProp->dwCostCoin;
}

VOID CPetNeiDan::AddExperience(UINT64 qwExperience)
{
	const SPetNeiDanProp* poBaseProp = GetBaseProp();
	if ( NULL == poBaseProp)
	{
		return ;
	}
	m_stPetNeiDan.qwNeiDanExcpetion += qwExperience;
	UINT16 wOldLevel = m_stPetNeiDan.wNeiDanLevel;
	m_stPetNeiDan.wNeiDanLevel = CPetPropMng::Instance()->GetNeiDanLevel(m_stPetNeiDan.qwNeiDanExcpetion, poBaseProp->byQuality, poBaseProp->byType);
	if ( (NULL != m_poPet) && wOldLevel != m_stPetNeiDan.wNeiDanLevel)
	{
		m_poOwner->ResetBattleAttr(TRUE);
	}
}