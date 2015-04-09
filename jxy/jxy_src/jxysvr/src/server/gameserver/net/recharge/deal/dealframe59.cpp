// DealFrame59.cpp: implementation of the CDealFrame59 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe59.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include <logic/player/player.h>
#include <logic/player/playermgr.h>
#include "logic/player/playerbasedatacachemgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame59::CDealFrame59()
{

}

CDealFrame59::~CDealFrame59()
{

}

void CDealFrame59::Release()
{

}

UINT16 CDealFrame59::GetFrameType()
{
    return 59;
}

void CDealFrame59::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    if((NULL == poClinet) || (NULL == pszParam4))
    {
        return;
    }

    const SPlayerBaseData* pstBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);

    if(NULL == pstBaseData)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);

    if(NULL == poPlayer)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_IS_NOT_ONLINE);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    map<string , UINT32> mapData;
    mapData["cliver"] = poPlayer->GetCliVer();
    string strMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &mapData);
    poClinet->Rspone(strMsg);
}

void CDealFrame59::Deal( CRCClient* poClinet, UINT32 dwSerialNumber, UINT32 dwAreaID, UINT32 dwPlayerID, map<string, string> &mapField )
{
    if (NULL == poClinet)
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
    const SPlayerBaseData* pstBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(unPlayerId);
    if(NULL == pstBaseData)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(unPlayerId);
    if(NULL == poPlayer)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_MEMORT);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    map<string , UINT32> mapData;
    mapData["cliver"] = poPlayer->GetCliVer();
    string strMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_IS_NOT_ONLINE, &mapData);
    poClinet->Rspone(strMsg);
}

string CDealFrame59::GetStrFrameType()
{
    return GET_CLIVER;
}

void CDealFrame59::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string, string> mapField;
    mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;

    RsponeMsg(poClinet, mapField);
}

string CDealFrame59::GetStrFrameDesc()
{
    return SDGBK2UTF8(GET_CLIVER_DESC);
}

