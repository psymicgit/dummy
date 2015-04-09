// DealFrame58.cpp: implementation of the CDealFrame58 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe58.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame58::CDealFrame58()
{

}

CDealFrame58::~CDealFrame58()
{

}

void CDealFrame58::Release()
{

}

UINT16 CDealFrame58::GetFrameType()
{
    return 58;
}

void CDealFrame58::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    if((NULL == poClinet) || (NULL == pszParam4))
    {
        return;
    }
    UINT16 wErrCode = CGMProcessor::GmReq(dwSerialNumber, byAction, dwPlayerID, dwParam1, qwParam2, qwParam3, pszParam4, poClinet->GetCliSessionID(), pszDesc);
    CHAR szRet[128] = {0};
    if(0 == wErrCode)
    {
        sprintf(szRet, "{\"status\":\"success\"}");
    }
    else
    {
        sprintf(szRet, "{\"status\":\"error_%d\"}", wErrCode);
    }
    poClinet->Rspone(szRet);
}

void CDealFrame58::Deal( CRCClient* poClinet, UINT32 dwSerialNumber, UINT32 dwAreaID, UINT32 dwPlayerID, map<string, string> &mapField )
{
    if (NULL == poClinet)
    {
        return;
    }

    if (mapField[PLAYER_ID].empty() || mapField[SET_FACTION_LEVEL_VALUE_PARAM].empty())
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    UINT32 qwParam1 = SDAtou(mapField[SET_FACTION_LEVEL_VALUE_PARAM].c_str());
    UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
    UINT16 wErrCode = CGMProcessor::GmReq(dwSerialNumber, (UINT8)GetFrameType(), unPlayerId, qwParam1, 0, 0, "", poClinet->GetCliSessionID(), mapField["desc"].c_str());

    if (ERR_GM_PARM_INFO::ID_SUCCESS == wErrCode)
    {
        string strErrorMsg = GetRsponeResult(wErrCode);
        poClinet->Rspone(strErrorMsg.c_str());
    }
    else
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_FACTION_NO_OPEN);
        poClinet->Rspone(strErrorMsg.c_str());
    }
}

string CDealFrame58::GetStrFrameType()
{
    return SET_FACTION_LEVEL;
}

void CDealFrame58::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string, string> mapField;
    mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;
    mapField[SET_FACTION_LEVEL_VALUE_PARAM] = SDGBK2UTF8(SET_FACTION_LEVEL_VALUE_GBK) + "|" + GM_PARAM_INT;

    RsponeMsg(poClinet, mapField);
}

string CDealFrame58::GetStrFrameDesc()
{
    return SDGBK2UTF8(SET_FACTION_LEVEL_DESC);
}

