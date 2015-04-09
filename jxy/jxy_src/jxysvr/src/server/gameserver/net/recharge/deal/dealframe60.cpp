// DealFrame60.cpp: implementation of the CDealFrame60 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe60.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include <logic/player/player.h>
#include<dll/sdu/sddir.h>
#include<db/saveplayerdatacmd.h>
#include <db/autosqlbuf.h>
#include <framework/gsapi.h>
#include "logic/player/playerbasedatacachemgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define LOOK_UP_FILE 1
#define RETREATE_PLAYER 2
#define ALL_PLAYER_RETREATE 3

CDealFrame60::CDealFrame60()
{

}

CDealFrame60::~CDealFrame60()
{

}

void CDealFrame60::Release()
{

}

UINT16 CDealFrame60::GetFrameType()
{
    return 60;
}

void CDealFrame60::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    if((NULL == poClinet) || (NULL == pszParam4))
    {
        return;
    }

    this->DealCmd(poClinet, dwPlayerID, dwParam1, pszParam4);

}

void CDealFrame60::DealCmd(CRCClient* poClinet, UINT32 unPlayerId, UINT8 unType, string strFileName)
{
    const SPlayerBaseData* pPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(unPlayerId);

    if (NULL == pPlayerBaseData)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    switch(unType)
    {
    case LOOK_UP_FILE:
    {
        char aszFileDir[256];
        SDSnprintf(aszFileDir, 256 - 1, "%s%s/%s/%u", (const CHAR*)SDGetModulePath(), SAVE_PLAYER_DATA_SQL, strFileName.c_str(), unPlayerId);

        CSDDirectory cDire;
        cDire.Open(aszFileDir);
        vector<string> vecFileName;
        cDire.ReadAllFile(vecFileName);

        if (0 == vecFileName.size())
        {
            string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_NOT_DATA);
            poClinet->Rspone(strErrorMsg.c_str());
            return;
        }

        for (UINT32 i = 0; i < vecFileName.size(); i++)
        {
            vecFileName[i] = "\"" + vecFileName[i] + "\"";
        }

        string strMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &vecFileName);
        poClinet->Rspone(strMsg);
    }
    break;
    case RETREATE_PLAYER:
    {
        if (strFileName.size() != 14)
        {
            string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
            poClinet->Rspone(strErrorMsg.c_str());
            return;
        }

        CSDDateTime oDateTime;
        UINT64 dwTime = 0;
        string strCasch;

        strCasch = strFileName.substr(0, 4);
        UINT32 unYear = SDAtou(strCasch.c_str());
        strCasch = strFileName.substr(4, 2);
        UINT32 unMonth = SDAtou(strCasch.c_str());
        strCasch = strFileName.substr(6, 2);
        UINT32 unDay = SDAtou(strCasch.c_str());
        strCasch = strFileName.substr(8, 2);
        UINT32 unHour = SDAtou(strCasch.c_str());
        strCasch = strFileName.substr(10, 2);
        UINT32 unMin = SDAtou(strCasch.c_str());
        strCasch = strFileName.substr(12, 2);
        UINT32 unSec = SDAtou(strCasch.c_str());

        if(!oDateTime.SetDateTime(unYear, unMonth, unDay, unHour, unMin, unSec))
        {
            string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
            poClinet->Rspone(strErrorMsg.c_str());
            return;
        }

        string strTime = oDateTime.ToString("YYYY-mm-dd") ;
        dwTime = oDateTime.GetTimeValue();
        BOOL bExist = FALSE;
        string strFile = strFileName + ".sql";

        char aszFileDir[256];
        SDSnprintf(aszFileDir, 256 - 1, "%s%s/%s/%u", (const CHAR*)SDGetModulePath(), SAVE_PLAYER_DATA_SQL, strTime.c_str(), unPlayerId);

        CSDDirectory cDire;
        cDire.Open(aszFileDir);
        vector<string> vecFileName;
        cDire.ReadAllFile(vecFileName);

        for (UINT32 i = 0; i < vecFileName.size(); i++)
        {
            if (0 == vecFileName[i].compare(strFile))
            {
                bExist = TRUE;
                break;
            }
        }

        if (!bExist)
        {
            string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_FILE_NO_EXIST);
            poClinet->Rspone(strErrorMsg.c_str());
            return;
        }

        CUser* poUser = CUserMgr::Instance()->FindUser(unPlayerId);

        if(NULL != poUser)
        {
            PKT_GSGT_KICK_OUT_REQ stReq;
            stReq.dwPlayerID = unPlayerId;
            stReq.byKickOutType = 1;
            CSDPipeChannel* poGtChannel = gsapi::GetGTPipeChannel(poUser->GetZoneID());

            if (NULL != poGtChannel)
            {
                poGtChannel->SendMsg(poClinet->GetCliSessionID(), GSGT_KICK_OUT_REQ, (CHAR*)(&stReq)); //收到即可返回应答
            }
        }

        memset(aszFileDir, 0, sizeof(aszFileDir));
        SDSnprintf(aszFileDir, 256 - 1, "%s%s/%s/%u/%s", (const CHAR*)SDGetModulePath(), SAVE_PLAYER_DATA_SQL, strTime.c_str(), unPlayerId, strFile.c_str());
        BOOL bResult = CPlayerMgr::Instance()->RetreatePlayerData(unPlayerId, aszFileDir, dwTime);

        if (bResult)
        {
            string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS);
            poClinet->Rspone(strErrorMsg.c_str());
            return;
        }
        else
        {
            string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
            poClinet->Rspone(strErrorMsg.c_str());
            return;
        }
    }
    break;
    case ALL_PLAYER_RETREATE:
    {
        //CPlayerMgr::Instance()->AllPlayerRetreate(poClinet->GetCliSessionID());
    }
    break;
    default:
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_TYPE_NOT_EXIT_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
    }
    break;
    }
}

void CDealFrame60::Deal( CRCClient* poClinet, UINT32 dwSerialNumber, UINT32 dwAreaID, UINT32 dwPlayerID, map<string, string> &mapField )
{
    if (NULL == poClinet)
    {
        return;
    }

    if (mapField[PLAYER_ID].empty() || mapField[RETREATE_TYPE_PARAM].empty() || mapField[RETREATE_FILE_NAME_PAMAM].empty())
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
    UINT8 unType = SDAtou(mapField[RETREATE_TYPE_PARAM].c_str());
    string strFileName = mapField[RETREATE_FILE_NAME_PAMAM].c_str();

    this->DealCmd(poClinet, unPlayerId, unType, strFileName);
}

string CDealFrame60::GetStrFrameType()
{
    return RETREATE;

}

void CDealFrame60::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string, string> mapField;
    mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;
    mapField[RETREATE_TYPE_PARAM] = SDGBK2UTF8(RETREATE_TYPE_GBK) + "|" + GM_PARAM_INT;
    mapField[RETREATE_FILE_NAME_PAMAM] = SDGBK2UTF8(RETREATE_FILE_NAME_GBK) + "|" + GM_PARAM_INT;

    RsponeMsg(poClinet, mapField);
}

string CDealFrame60::GetStrFrameDesc()
{
    return SDGBK2UTF8(RETREATE_DESC);
}

