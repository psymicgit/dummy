///<------------------------------------------------------------------------------
//< @file:   logic/pet/petobjmng.h
//< @author: yewenhui
//< @date:   2014年8月11日 10点10分
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include "pet.h"
#include "logic/base/singleton.h"

class CPetObjMng
	: public Singleton<CPetObjMng>
{
	typedef std::map<UINT64, CPet*> MapIdPet;

public:
	// default ctor dector is fine

	CPet*		CreatePet(UINT32 unPlayerId, UINT16 usPetId);
	VOID		RemovePet(UINT32 unPlayerId, UINT16 usPetId);
	CPet*		GetPet(UINT32 unPlayerId, UINT16 usPetId);

private:
	MapIdPet	m_mpPet;
	CSDObjectPool<CPet, CSDMutex> m_PetObjPool;
};

#define CREATE_PET_OBJ CPetObjMng::Instance()->CreatePet
#define REMOVE_PET_OBJ CPetObjMng::Instance()->RemovePet
#define FIND_PET_OBJ CPetObjMng::Instance()->GetPet
