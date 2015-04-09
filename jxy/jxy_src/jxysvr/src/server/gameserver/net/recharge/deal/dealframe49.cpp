// DealFrame49.cpp: implementation of the CDealFrame49 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe49.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame49::CDealFrame49()
{

}

CDealFrame49::~CDealFrame49()
{

}

void CDealFrame49::Release()
{

}

UINT16 CDealFrame49::GetFrameType()
{
    return 49;
}

void CDealFrame49::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
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

void CDealFrame49::Deal( CRCClient* poClinet, UINT32 dwSerialNumber, UINT32 dwAreaID, UINT32 dwPlayerID, map<string, string> &mapField)
{
    if (NULL == poClinet)
    {
        return;
    }

    if ( mapField[START_SERVER_TIME_PARAM].empty())
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    string strServerTime = mapField[START_SERVER_TIME_PARAM];
    UINT16 wErrCode = CGMProcessor::GmReq(dwSerialNumber, (UINT8)GetFrameType(), 0, 0, 0, 0, (char*)strServerTime.c_str(), poClinet->GetCliSessionID(), mapField["desc"].c_str());

    string strErrorMsg = GetRsponeResult(wErrCode);
    poClinet->Rspone(strErrorMsg.c_str());
}

string CDealFrame49::GetStrFrameType()
{
    return START_SERVER;
}

void CDealFrame49::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string, string> mapField;
    mapField[START_SERVER_TIME_PARAM] = SDGBK2UTF8(START_SERVER_TIME_GBK) + "|" + GM_PARAM_DATATIME;

    RsponeMsg(poClinet, mapField);
}

string CDealFrame49::GetStrFrameDesc()
{
    return SDGBK2UTF8(START_SERVER_DESC);
}

