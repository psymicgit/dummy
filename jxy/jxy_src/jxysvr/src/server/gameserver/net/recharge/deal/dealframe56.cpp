// DealFrame56.cpp: implementation of the CDealFrame56 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe56.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playermgr.h"
#include "logic/hero/hero.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame56::CDealFrame56()
{

}

CDealFrame56::~CDealFrame56()
{

}

void CDealFrame56::Release()
{

}

UINT16 CDealFrame56::GetFrameType()
{
    return 56;
}

void CDealFrame56::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    if((NULL == poClinet) || (NULL == pszParam4))
    {
        return;
    }

    const CID2PlayerMap & mapID2PlayerMap = CPlayerMgr::Instance()->GetID2PlayerMap();

    if (0 == mapID2PlayerMap.size())
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_NOT_DATA);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    CID2PlayerMapConstItr iter;
    UINT8 unIndex = 1;
    string strPlayerInfo = "";
	vector<string> jsonData;
    for (iter = mapID2PlayerMap.begin(); iter != mapID2PlayerMap.end(); iter++)
    {
        CPlayer *pPlayer = iter->second;

        if (NULL == pPlayer)
        {
            continue;
        }

        if (pPlayer->CkLogin())
        {
            DT_PLAYER_BASE_DATA stuPlayerBaseData = pPlayer->GetDT_PLAYER_BASE_DATA();
            CHero *pHero = pPlayer->GetCoachHero();

            if (NULL == pHero)
            {
                continue;
            }

            if (unIndex != 1)
            {
                strPlayerInfo = strPlayerInfo + ",";
            }

            DT_HERO_BASE_DATA stuHeroBaseData = pHero->GetDT_HERO_BASE_DATA();
            CHAR strInfo[128] = {0};

            SDStrcat(strInfo, "{\"playerid\":\"");
            CHAR strCash[16] = {0};
            sprintf(strCash, "%u", stuPlayerBaseData.dwID);
            SDStrcat(strInfo, strCash);
            SDStrcat(strInfo, "\",\"name\":\"");
            SDStrcat(strInfo, stuPlayerBaseData.aszDispName);
            SDStrcat(strInfo, "\",\"level\":\"");
            memset(strCash, 0, sizeof(strCash));
            sprintf(strCash, "%u", stuHeroBaseData.wLevel);
            SDStrcat(strInfo, strCash);
            SDStrcat(strInfo, "\"}");

			jsonData.push_back(strInfo);
			unIndex++;
        }
    }

    if (unIndex == 1)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_NOT_DATA);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

	string strRetInfo = "";
	strRetInfo = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &jsonData);
	poClinet->Rspone(strRetInfo.c_str());
}

void CDealFrame56::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT32 dwAreaID, UINT32 dwPlayerID, map<string, string> &mapField)
{
    if(NULL == poClinet)
    {
        return;
    }

    const CID2PlayerMap & mapID2PlayerMap = CPlayerMgr::Instance()->GetID2PlayerMap();

    if (0 == mapID2PlayerMap.size())
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_NOT_DATA);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    CID2PlayerMapConstItr iter;
    UINT8 unIndex = 1;
    string strPlayerInfo = "";


	vector<string> jsonData;
    for (iter = mapID2PlayerMap.begin(); iter != mapID2PlayerMap.end(); iter++)
    {
        CPlayer *pPlayer = iter->second;

        if (NULL == pPlayer)
        {
            continue;
        }

        if (pPlayer->CkLogin())
        {
            DT_PLAYER_BASE_DATA stuPlayerBaseData = pPlayer->GetDT_PLAYER_BASE_DATA();
            CHero *pHero = pPlayer->GetCoachHero();

            if (NULL == pHero)
            {
                continue;
            }

            if (unIndex != 1)
            {
                strPlayerInfo = strPlayerInfo + ",";
            }

            DT_HERO_BASE_DATA stuHeroBaseData = pHero->GetDT_HERO_BASE_DATA();
            CHAR strInfo[128] = {0};

            SDStrcat(strInfo, "{\"playerid\":\"");
            CHAR strCash[16] = {0};
            sprintf(strCash, "%u", stuPlayerBaseData.dwID);
            SDStrcat(strInfo, strCash);
            SDStrcat(strInfo, "\",\"name\":\"");
            SDStrcat(strInfo, stuPlayerBaseData.aszDispName);
            SDStrcat(strInfo, "\",\"level\":\"");
            memset(strCash, 0, sizeof(strCash));
            sprintf(strCash, "%u", stuHeroBaseData.wLevel);
            SDStrcat(strInfo, strCash);
            SDStrcat(strInfo, "\"}");

			jsonData.push_back(strInfo);
            unIndex++;
        }
    }

    if (unIndex == 1)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_NOT_DATA);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    string strRetInfo = "";
    strRetInfo = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &jsonData);
    poClinet->Rspone(strRetInfo.c_str());
}

string CDealFrame56::GetStrFrameType()
{
    return GET_ONLINE_PLAYER_INFO;
}

void CDealFrame56::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string, string> mapField;
    RsponeMsg(poClinet, mapField);
}

string CDealFrame56::GetStrFrameDesc()
{
    return SDGBK2UTF8(GET_ONLINE_PLAYER_INFO_DESC);
}
