#include "updateattr.h"

#include <common/client/errdef.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <sdloggerimpl.h>
#include <sdutil.h>

CUpdateAttr::CUpdateAttr()
{
}

CUpdateAttr::~CUpdateAttr()
{

}

BOOL CUpdateAttr::Init(DT_HERO_UPDATE_ATTR_DATA* pstDT_UPDATE_ATTR, CPlayer* poOwner)
{
	if(NULL == poOwner)
	{
		return FALSE;
	}
	m_poOwner = poOwner;
	return TRUE;
}