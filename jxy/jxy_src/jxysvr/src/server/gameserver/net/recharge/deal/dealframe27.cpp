// DealFrame27.cpp: implementation of the CDealFrame27 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe27.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame27::CDealFrame27()
{

}

CDealFrame27::~CDealFrame27()
{

}

void CDealFrame27::Release()
{
	
}

UINT16 CDealFrame27::GetFrameType()
{
	return 27;
}

void CDealFrame27::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	//CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(SDAtou(mapField[PPLAYERID].c_str()));
	//if(NULL == poPlayer)
	//{
	//	CGetPlayerDataMgr::Instance()->GetPlayerData(m_dwCliSessionID, GET_PLAYERDATATYPE_GMPLAYERINFO, dwPlayerID, dwParam1, qwParam2, qwParam3, (TCHAR*)(strParam4.c_str()), byAction, dwSerialNumber);
	//}
	//else
	//{
	//	DT_PLAYER_DATA stDT_PLAYER_DATA = {0};
	//	CPlayerSaveMgr::Instance()->SavePlayerData(poPlayer, ESDBT_GAME, &stDT_PLAYER_DATA);
	//	CJsonOpMgr::Instance()->Instance()->AddJsonParsePlayerData(stDT_PLAYER_DATA, m_dwCliSessionID);
	//}
}

void CDealFrame27::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{

}

string CDealFrame27::GetStrFrameType()
{
	return GET_PLAYER_INFO;
}

void CDealFrame27::Interface( CRCClient* poClinet )
{

	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;

	RsponeMsg(poClinet,mapField);
}

string CDealFrame27::GetStrFrameDesc()
{
	return SDGBK2UTF8(GET_PLAYER_INFO_DESC);
}

