#include "instancerecord.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <sdloggerimpl.h>


CInstanceRecord::CInstanceRecord()
{
	memset(&m_stDT_INSTANCE_DATA, 0, sizeof(m_stDT_INSTANCE_DATA));
}


CInstanceRecord::~CInstanceRecord()
{

}

BOOL CInstanceRecord::Init(DT_INSTANCE_DATA* pstDT_INSTANCE_DATA, CPlayer* poOwner)
{
	if((NULL == pstDT_INSTANCE_DATA) || (NULL == poOwner))
	{
		return FALSE;
	}

	SInstanceProp*	pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(pstDT_INSTANCE_DATA->stInstanceID.wSceneIdx
		, pstDT_INSTANCE_DATA->stInstanceID.byTownIdx, pstDT_INSTANCE_DATA->stInstanceID.byInstanceIdx);
	if(NULL == pstInstanceProp)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: error, pstInstanceProp is NULL, instance ID = [%d.%d.%d], playerID = %u"), MSG_MARK,
			pstDT_INSTANCE_DATA->stInstanceID.wSceneIdx, pstDT_INSTANCE_DATA->stInstanceID.byTownIdx, pstDT_INSTANCE_DATA->stInstanceID.byInstanceIdx, poOwner->GetID());
		return FALSE;
	}

	memcpy(&m_stDT_INSTANCE_DATA, pstDT_INSTANCE_DATA, sizeof(m_stDT_INSTANCE_DATA));
	m_poOwner = poOwner;

	return TRUE;
}


SInstanceProp* CInstanceRecord::GetInstanceProp()
{
	SInstanceProp*	pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(m_stDT_INSTANCE_DATA.stInstanceID.wSceneIdx
		, m_stDT_INSTANCE_DATA.stInstanceID.byTownIdx, m_stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx);

	if(NULL == pstInstanceProp)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: pstInstanceProp is NULL, ID[%d.%d.%d] "), MSG_MARK, m_stDT_INSTANCE_DATA.stInstanceID.wSceneIdx
			, m_stDT_INSTANCE_DATA.stInstanceID.byTownIdx, m_stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx);   		
	}

	return pstInstanceProp;
}

