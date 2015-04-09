#include "noticemgr.h"
#include <sdloggerimpl.h>
#include <json/json.h>
#include <algorithm>
#include <string>
#include <algorithm/sdmd5.h>

#include "dll/sdframework/sdutil.h"
#include "json/json.h"
#include "common/server/ngstring.h"
#include "common/client/errdef.h"
#include "framework/gtapplication.h"
#include "net/cli/clipktbuilder.h"

using namespace std;
using namespace SGDP;
#define IOS_AND_BOUND 100
#define IOS_DEFAULT_NOTICE 3
#define AND_DEFAULT_NOTICE 101

IMPLEMENT_SINGLETON(CNoticeMgr)

CNoticeMgr::CNoticeMgr()
{

}

CNoticeMgr::~CNoticeMgr()
{

}

BOOL CNoticeMgr::Init()
{
    return TRUE;
}

VOID CNoticeMgr::UnInit()
{

}

DT_NOTICE_DATA * CNoticeMgr::GetDT_NOTICE_DATA(UINT8 byAutyType)
{
	MAP_DT_NOTICE_DATAItr itr;
	itr = m_mapDT_NOTICE_DATA.find(byAutyType);
	if (itr != m_mapDT_NOTICE_DATA.end())
	{
		return &itr->second;
	}
	else
	{
		if ( byAutyType > IOS_AND_BOUND)
		{
			itr = m_mapDT_NOTICE_DATA.find(AND_DEFAULT_NOTICE);
		}
		else
		{
			itr = m_mapDT_NOTICE_DATA.find(IOS_DEFAULT_NOTICE);
		}
		if ( itr != m_mapDT_NOTICE_DATA.end() )
		{
			return &itr->second;
		}
		else
		{
			return NULL;
		}
		return NULL;
	}
}

DT_TIME_OUT * CNoticeMgr::GetDT_TIME_OUT(UINT8 byAutyType)
{
	MAP_DT_TIME_OUTItr itr;
	itr = m_mapDT_TIME_OUT.find(byAutyType);
	if (itr != m_mapDT_TIME_OUT.end())
	{
		return &itr->second;
	}
	else
	{
		return NULL;
	}
}
