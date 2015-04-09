// DealFrame59.cpp: implementation of the CDealFrame59 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe61.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include <logic/player/player.h>
#include <logic/player/playermgr.h>
#include "logic/player/playerbasedatacachemgr.h"
#include "logic/faction/faction.h"
#include "logic/faction/factionmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame61::CDealFrame61()
{

}

CDealFrame61::~CDealFrame61()
{

}

void CDealFrame61::Release()
{

}

UINT16 CDealFrame61::GetFrameType()
{
    return 61;
}

void CDealFrame61::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
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

    CFaction* poFaction = CFactionMgr::Instance()->GetFactionByPlayerID(dwPlayerID);
    if ( NULL != poFaction)
    {
        poFaction->ChangeFactionNameByGM(pszParam4);
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS);
        poClinet->Rspone(strErrorMsg.c_str());
    }
    else
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_FACTION_NO_OPEN);
        poClinet->Rspone(strErrorMsg.c_str());
    }
}

void CDealFrame61::Deal( CRCClient* poClinet, UINT32 dwSerialNumber, UINT32 dwAreaID, UINT32 dwPlayerID, map<string, string> &mapField )
{
    if (NULL == poClinet)
    {
        return;
    }

    if (mapField[PLAYER_ID].empty() || mapField[RESET_FACTION_NAME_PAMAM].empty())
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
    string strNewFactionName = mapField[RESET_FACTION_NAME_PAMAM].c_str();

    const SPlayerBaseData* pstBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(unPlayerId);
    if(NULL == pstBaseData)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    CFaction* poFaction = CFactionMgr::Instance()->GetFactionByPlayerID(unPlayerId);
    if ( NULL != poFaction)
    {
        poFaction->ChangeFactionNameByGM(const_cast<TCHAR*>(strNewFactionName.c_str()));
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS);
        poClinet->Rspone(strErrorMsg.c_str());
    }
    else
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_FACTION_NO_OPEN);
        poClinet->Rspone(strErrorMsg.c_str());
    }
}

string CDealFrame61::GetStrFrameType()
{
    return RESET_FACTION_NAME;
}

void CDealFrame61::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string, string> mapField;
    mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;
    mapField[RESET_FACTION_NAME_PAMAM] = SDGBK2UTF8(FACTION_NAME_GBK) + "|" + GM_PARAM_STR;

    RsponeMsg(poClinet, mapField);
}

string CDealFrame61::GetStrFrameDesc()
{
    return SDGBK2UTF8(RESET_FACTION_NAME_DESC);
}

