
#include "getplayernotifyinfoevent.h"
#include "framework/nsapplication.h"
#include "logic/basedbccmd.h"
#include <sdutil.h>

CGetPlayerNotifyInfoEvent::CGetPlayerNotifyInfoEvent()
{

}

CGetPlayerNotifyInfoEvent::~CGetPlayerNotifyInfoEvent()
{

}

UINT32 CGetPlayerNotifyInfoEvent::OnEvent()
{
	CBaseDBCMD* poCMD = ((CNSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_GET_PLAYER_NOTIFY_INFO);
	if(NULL == poCMD)
	{
		SYS_CRITICAL(_SDT("[%s: %d]:OnEvent CGetPlayerNotifyInfoEvent ³öÏÖÒì³£ "), MSG_MARK);
		return 0;
	}

	((CNSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
	return 0;
}

INT32  CGetPlayerNotifyInfoEvent::OnDestroy()
{
    return 0;
}

