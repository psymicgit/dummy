// DealFrame22.cpp: implementation of the CDealFrame22 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe22.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame22::CDealFrame22()
{

}

CDealFrame22::~CDealFrame22()
{

}

void CDealFrame22::Release()
{

}

UINT16 CDealFrame22::GetFrameType()
{
    return 22;
}

void CDealFrame22::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    if((NULL == poClinet) || (NULL == pszParam4))
    {
        return;
    }
    UINT16 wErrCode = CGMProcessor::GmReq(dwSerialNumber, byAction, dwPlayerID, dwParam1, qwParam2, qwParam3, pszParam4, poClinet->GetCliSessionID(), pszDesc);
    if (ERR_GM_PARM_INFO::ID_PLAYER_NOT_MEMORT == wErrCode)
        return;

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

void CDealFrame22::Deal( CRCClient* poClinet, UINT32 dwSerialNumber, UINT32 dwAreaID, UINT32 dwPlayerID, map<string, string> &mapField )
{
    if (NULL == poClinet)
    {
        return;
    }

    if (mapField[ALL_ITEM_TYPE_PARAM].empty() || mapField[ALL_ITEM_COUNT_PARAM].empty())
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    UINT32 qwParam1 = SDAtou(mapField[ALL_ITEM_TYPE_PARAM].c_str());
    UINT32 qwParam2 = SDAtou(mapField[ALL_ITEM_COUNT_PARAM].c_str());
    UINT16 wErrCode = CGMProcessor::GmReq(dwSerialNumber, (UINT8)GetFrameType(), 0, qwParam1, qwParam2, 0, "", poClinet->GetCliSessionID(), mapField["desc"].c_str());
    if (ERR_GM_PARM_INFO::ID_PLAYER_NOT_MEMORT == wErrCode)
        return;
    string strErrorMsg = GetRsponeResult(wErrCode);
    poClinet->Rspone(strErrorMsg.c_str());
}

string CDealFrame22::GetStrFrameType()
{
    return ALL_ITEM;
}

void CDealFrame22::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string, string> mapField;
    mapField[ALL_ITEM_TYPE_PARAM] = SDGBK2UTF8(ALL_ITEM_TYPE_GBK) + "|" + GM_PARAM_INT;
    mapField[ALL_ITEM_COUNT_PARAM] = SDGBK2UTF8(ALL_ITEM_COUNT_GBK) + "|" + GM_PARAM_INT;

    RsponeMsg(poClinet, mapField);
}

string CDealFrame22::GetStrFrameDesc()
{
    return SDGBK2UTF8(ALL_ITEM_DESC);
}

