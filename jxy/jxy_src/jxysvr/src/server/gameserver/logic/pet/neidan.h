///<------------------------------------------------------------------------------
//< @file:   logic\pet\neidan.h
//< @author: Kang_zhiquan
//< @date:   2014年10月20日 9:27:5
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _neidan_h_
#define _neidan_h_

#include "logic/base/baseobj.h"
struct SPetNeiDanProp;
struct SPetNeiDanUpdateProp;
class CPet;
class CPetNeiDan : public CBaseObj
{
public:
	CPetNeiDan();
	~CPetNeiDan();
	BOOL Init(CPlayer* poPlayer, const DT_NEIDAN_DATA& stNeiDan);
	const DT_NEIDAN_DATA& GetDT_NEIDAN_DATA(){ return m_stPetNeiDan; }
	SPetNeiDanProp* GetBaseProp();
	BOOL GetUpdataProp(SPetNeiDanUpdateProp*& poCurUpdataProp, SPetNeiDanUpdateProp*& poBeforUpdataProp);
	
	VOID SetIdx(UINT16 wIdx);
	VOID SetPet(CPet* poPet);
public:
	BOOL Dress(CPet* poPet);
	BOOL UnDress();
	VOID GetNeiDanCli(DT_NEIDAN_CLI& stCli);
	VOID GetNeiDanMark(DT_ITEM_MARK_DATA& stCli);
	UINT64 GetBackExperience();
	UINT32 GetBackCoin();
	VOID AddExperience(UINT64 qwExperience);
private:
	DT_NEIDAN_DATA m_stPetNeiDan;
	CPet* m_poPet;//不一定有宠物佩戴
};

DECLARE_CLASS_POOL_MGR(CPetNeiDan);
#endif //_neidan_h_