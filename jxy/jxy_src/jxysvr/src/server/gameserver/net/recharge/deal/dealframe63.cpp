// DealFrame63.cpp: implementation of the CDealFrame63 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe63.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playerbasedatacachemgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame63::CDealFrame63()
{

}

CDealFrame63::~CDealFrame63()
{

}

void CDealFrame63::Release()
{

}

UINT16 CDealFrame63::GetFrameType()
{
    return 63;
}

void CDealFrame63::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    if((NULL == poClinet) || (NULL == pszParam4))
    {
        return;
    }

    if(dwParam1 == 0)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    CPlayerRankDataVec m_vecPlayerRankData;
    m_vecPlayerRankData.clear();
    CPlayerBaseDataCacheMgr::Instance()->GetRankByLvEx(dwParam1,m_vecPlayerRankData);

    if(m_vecPlayerRankData.size() == 0)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_NOT_DATA);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    map<string , SPlayerRankData> mapData;
    CPlayerRankDataVecItr itr = m_vecPlayerRankData.begin();
    UINT32 dwIndex = 1;
    UINT32 dwMax = dwParam1 + 1;
    vector<string> jsonData;
    CHAR ss[USERNAME_LEN];
    string str;
    for (itr,dwIndex; itr!=m_vecPlayerRankData.end()&&dwIndex< dwMax; ++itr,dwIndex++)
    {
        str = "";
        memset(ss,0x00,sizeof(ss));
        str+="{\"rank\":";
        sprintf(ss,"%u",dwIndex);
        str+=ss;
        str+=",";
        str+="\"playerID\":";
        memset(ss,0x00,sizeof(ss));
        sprintf(ss,"%u",itr->dwPlayerID);
        str+=ss;
        str+=",";
        memset(ss,0x00,sizeof(ss));
        str+="\"level\":";
        sprintf(ss,"%u",itr->wLevel);
        str+=ss;
        str+="}";
        jsonData.push_back(str);
    }

    string strMsg = "";
    strMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &jsonData);
    poClinet->Rspone(strMsg);
}

void CDealFrame63::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
    if (NULL == poClinet)
    {
        return;
    }

    return;
}

string CDealFrame63::GetStrFrameType()
{
    return GET_RANK;
}

void CDealFrame63::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string,string> mapField;
    mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID)+ "|" + GM_PARAM_PLAYER_ID;
    //mapField[ADD_COURAGE_TIMES_PARAM] = SDGBK2UTF8(ADD_COURAGE_TIMES_GBK)+ "|" + GM_PARAM_INT;

    RsponeMsg(poClinet,mapField);
}

string CDealFrame63::GetStrFrameDesc()
{
    return SDGBK2UTF8(GET_RANK_DESC);
}

