#include "gsprocessor.h"
#include "sdutil.h"
#include "framework/nsapplication.h"
#include "protocol/server/protogsns.h"
#include <common/client/errdef.h>
#include <framework/nsapplication.h>
#include "logic/dbmgr.h"
#include "logic/basedbccmd.h"

#include <framework/nsapplication.h>
extern CSDApplication* SDGetApp();

CGSProcessor::CGSProcessor()
{
    Init();
}

CGSProcessor::~CGSProcessor()
{
}

BOOL CGSProcessor::Init()
{
    RegisterCommand(GSNS_PLAYER_DEFAULT_NTF, CGSProcessor::OnPlayerDefaultNTF);
    return TRUE;
}

CSDProtocol* CGSProcessor::GetProtocol()
{
    return CProtoGSNS::Instance();
}

BOOL CGSProcessor::OnPlayerDefaultNTF(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32			dwTransID  = pPipeHeader->dwTransID;
    if (sizeof(tagPKT_GSNS_PLAYER_DEFAULT_NTF) != dwLen)
    {
        return FALSE;
    }

    PKT_GSNS_PLAYER_DEFAULT_NTF* pNtf = (PKT_GSNS_PLAYER_DEFAULT_NTF*)pszBody;
    if (NULL == pNtf)
    {
        return FALSE;
    }

	//DBG_INFO(_SDT("收到玩家:%u的通知"), pNtf->dwPlayerID);

    CPlayerNotifyInfoMap &mapPlayerNotifyInfo = ((CNSApplication*)SDGetApp())->GetPlayerNotifyInfoMap();
    CPlayerNotifyInfoMapItr itr = mapPlayerNotifyInfo.find(pNtf->dwPlayerID);
    if (itr == mapPlayerNotifyInfo.end())
    {
		//DBG_INFO(_SDT("收到玩家:%u的通知 不发送"), pNtf->dwPlayerID);

        return FALSE;
    }

    ((CNSApplication*)SDGetApp())->GetNotifications()->SSL_PushNoticeEx((CHAR*)pNtf->abyToken, (CHAR*)pNtf->aszPayLoad, itr->second);
    return TRUE;
}
