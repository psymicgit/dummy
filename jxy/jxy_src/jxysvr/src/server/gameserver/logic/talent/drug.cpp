#include "drug.h"
#include <common/client/errdef.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <sdloggerimpl.h>
#include <sdutil.h>

#include "drugpropmgr.h"
using namespace SGDP;

CDrug::CDrug()
{
    memset(&m_stDT_DRUG_DATA, 0, sizeof(m_stDT_DRUG_DATA));
    m_wTotalTalent = 0;
}


CDrug::~CDrug()
{
	
}

BOOL CDrug::Init(DT_DRUG_DATA* pstDT_DRUG_DATA, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    m_poOwner = poOwner;

    SDrugProp* pstDrugProp = CDrugPropMgr::Instance()->GetDrugProp(pstDT_DRUG_DATA->byLevel);
    if(NULL == pstDrugProp)
    {
		SYS_CRITICAL(_SDT("[%s: %d]: pstDrugProp is NULL, level[%d]!"), MSG_MARK, pstDT_DRUG_DATA->byLevel);
        return FALSE;
    }

    memcpy(&m_stDT_DRUG_DATA, pstDT_DRUG_DATA, sizeof(DT_DRUG_DATA));

    //¼ÆËã×ÊÖÊ
    CountTalent();

    return TRUE;
}

VOID CDrug::CountTalent()
{
	SDrugProp* pstDrugProp = CDrugPropMgr::Instance()->GetDrugProp(m_stDT_DRUG_DATA.byLevel);
	if(NULL == pstDrugProp)
	{
		return ;
	}

	UINT8 byNum = m_stDT_DRUG_DATA.byNum;
	m_wTotalTalent = 0;
	for(UINT8 byIdx = 0; byIdx < byNum; byIdx++)
	{
		m_wTotalTalent += pstDrugProp->wBaseTalent - pstDrugProp->wDecTalent * byIdx;
	}
}

VOID CDrug::OnUse()
{
    CountTalent();
}

SDrugProp* CDrug::GetDrugProp()
{
	SDrugProp* pstDrugProp = CDrugPropMgr::Instance()->GetDrugProp(m_stDT_DRUG_DATA.byLevel);
	if(NULL == pstDrugProp)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: pstDrugProp is NULL, level[%d]!"), MSG_MARK, m_stDT_DRUG_DATA.byLevel);
	}

	return pstDrugProp;
}

BOOL CDrug::CkFull()
{
	SDrugProp* pstDrugProp = CDrugPropMgr::Instance()->GetDrugProp(m_stDT_DRUG_DATA.byLevel);
	if(NULL == pstDrugProp)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: pstDrugProp is NULL, level[%d]!"), MSG_MARK, m_stDT_DRUG_DATA.byLevel);
		return TRUE;
	}
	return (m_stDT_DRUG_DATA.byNum >= pstDrugProp->byCanUseNum);
}
