// DealFrame43.cpp: implementation of the CDealFrame43 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe43.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/chat/chatmgr.h"
#include "logic/player/playerbasedatacachemgr.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame43::CDealFrame43()
{

}

CDealFrame43::~CDealFrame43()
{

}

void CDealFrame43::Release()
{

}

UINT16 CDealFrame43::GetFrameType()
{
    return 43;
}

void CDealFrame43::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    if((NULL == poClinet) || (NULL == pszParam4))
    {
        return;
    }
    if (0 == dwPlayerID)
    {
        dwPlayerID = CPlayerBaseDataCacheMgr::Instance()->FindPlayer(pszParam4);
    }
    if (0 == dwPlayerID)
    {
        poClinet->Rspone("{\"status\":\"no data!\"}");
        return;
    }
    CChatMgr::Instance()->AddGmData(dwPlayerID, "", dwParam1);

    string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS);
    poClinet->Rspone(strErrorMsg.c_str());
}

void CDealFrame43::Deal( CRCClient* poClinet, UINT32 dwSerialNumber, UINT32 dwAreaID, UINT32 dwPlayerID, map<string, string> &mapField )
{
    if(NULL == poClinet)
    {
        return;
    }

    if (mapField[PLAYER_ID].empty() || mapField[OPTTYPE_PARAM].empty())
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
    UINT8 unOptType = SDAtou(mapField[OPTTYPE_PARAM].c_str());
    const SPlayerBaseData* pstBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(unPlayerId);

    if(NULL == pstBaseData)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
    }

    CChatMgr::Instance()->AddGmData(unPlayerId, "", unOptType);

    string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS);
    poClinet->Rspone(strErrorMsg.c_str());
}

string CDealFrame43::GetStrFrameType()
{
    return SET_GM_INFO;
}

void CDealFrame43::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string, string> mapField;
    mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;
    mapField[OPTTYPE_PARAM] = SDGBK2UTF8(SET_GM_INFO_OPTTYPE_GBK) + "|" + GM_PARAM_INT;

    RsponeMsg(poClinet, mapField);
}

string CDealFrame43::GetStrFrameDesc()
{
    return SDGBK2UTF8(SET_GM_INFO_DESC);
}

