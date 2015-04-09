// DealFrame45.cpp: implementation of the CDealFrame45 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe45.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include <db/autosqlbuf.h>
#include <framework/gsapi.h>
#include "logic/player/playerbasedatacachemgr.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame45::CDealFrame45()
{

}

CDealFrame45::~CDealFrame45()
{

}

void CDealFrame45::Release()
{
	
}

UINT16 CDealFrame45::GetFrameType()
{
	return 45;
}

void CDealFrame45::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    if((NULL == poClinet) || (NULL == pszParam4))
    {
        return;
    }
    CUser* poUser = CUserMgr::Instance()->FindUser(dwPlayerID);
    if(NULL == poUser)
    {
        poClinet->Rspone("{\"status\":\"Player no in mem!\"}");
        return;
    }
    PKT_GSGT_KICK_OUT_REQ stReq;
    stReq.dwPlayerID = dwPlayerID;
    stReq.byKickOutType = 0;
    CSDPipeChannel* poGtChannel = gsapi::GetGTPipeChannel(poUser->GetZoneID());
    if (NULL == poGtChannel)
    {
        poClinet->Rspone("{\"status\":\"GateServer not connect!\"}");
        return;
    }
    poGtChannel->SendMsg(poClinet->GetCliSessionID(), GSGT_KICK_OUT_REQ, (CHAR*)(&stReq)); //收到即可返回应答
}

void CDealFrame45::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if(NULL == poClinet)
	{
		return;
	}

	if (mapField[PLAYER_ID].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}


	UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
	const SPlayerBaseData* pPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(unPlayerId);

	if (NULL == pPlayerBaseData)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}


	CUser* poUser = CUserMgr::Instance()->FindUser(unPlayerId);

	if(NULL == poUser)
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_IS_NOT_ONLINE);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	PKT_GSGT_KICK_OUT_REQ stReq;
	stReq.dwPlayerID = unPlayerId;
    stReq.byKickOutType = 0;
	CSDPipeChannel* poGtChannel = gsapi::GetGTPipeChannel(poUser->GetZoneID());

	if (NULL == poGtChannel)
	{	
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	poGtChannel->SendMsg(poClinet->GetCliSessionID(), GSGT_KICK_OUT_REQ, (CHAR*)(&stReq)); //收到即可返回应答
	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS);
	poClinet->Rspone(strErrorMsg.c_str());
}

string CDealFrame45::GetStrFrameType()
{
	return KICKOUT;
}

void CDealFrame45::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;

	RsponeMsg(poClinet,mapField);
}

string CDealFrame45::GetStrFrameDesc()
{
	return SDGBK2UTF8(KICK_OUT_DESC);
}

