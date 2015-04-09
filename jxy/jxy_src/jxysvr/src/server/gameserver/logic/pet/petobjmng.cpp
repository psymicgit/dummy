#include "petobjmng.h"
#include "logic/base/logicfunc.h"

CPet* CPetObjMng::CreatePet(UINT32 unPlayerId, UINT16 usPetId) {
	CPet* pPet = m_PetObjPool.Alloc();
	if (NULL == pPet) {
		return NULL;
	}
	UINT64 ullKey = CLogicFunc::GetObjKey(unPlayerId, usPetId);
	m_mpPet[ullKey] = pPet;

	return pPet;
}

VOID CPetObjMng::RemovePet(UINT32 unPlayerId, UINT16 usPetId) {
	UINT64 ullKey = CLogicFunc::GetObjKey(unPlayerId, usPetId);
	MapIdPet::iterator itor = m_mpPet.find(ullKey);
	if (itor != m_mpPet.end()) {
		m_PetObjPool.Free(itor->second);
		m_mpPet.erase(itor);
	}
}

CPet* CPetObjMng::GetPet(UINT32 unPlayerId, UINT16 usPetId) {
	UINT64 ullKey = CLogicFunc::GetObjKey(unPlayerId, usPetId);
	MapIdPet::const_iterator itor = m_mpPet.find(ullKey);
	if (itor != m_mpPet.end()) {
		return itor->second;
	}
	return NULL;
}