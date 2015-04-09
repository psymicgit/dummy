///<------------------------------------------------------------------------------
//< @file:   recharge\deal\dealframe75showgvgpair.cpp
//< @author: hongkunan
//< @date:   2014年8月13日 10:35:38
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "dealframe75showgvgpair.h"
#include "framework/gsapi.h"
#include "logic/gvg/global/gvgmgr.h"
#include "common/server/ngstring.h"
#include "net/recharge/rcclient.h"
#include "framework/gsapplication.h"
#include "dll/sdu/sdserverid.h"

namespace gvgpairutil
{
    std::string GetJsonFactionByGroup(const DT_GVG_ARENA_DATA &stArena, BYTE byGroupID)
    {
        CNGString strFactions;

        char szBuf[2048] = {0};

        const DT_16_TO_8_TO_4_WAR_INFO &stWar16To8To4Info = stArena.stWar16To8List.astWar16To8To4Info[byGroupID - 1];
        for (UINT16 i = 0; i < stWar16To8To4Info.byFactionCnt; i++)
        {
            const DT_GVG_FACTION &stFaction = stWar16To8To4Info.astFactionList[i];
            
            std::string strFactionName = stFaction.stBaseData.aszFactionName;
            std::string strHeadName = stFaction.stBaseData.aszHeadName;
            
            strFactionName = ReplaceStrLoop(strFactionName, "\"", "");
            strHeadName = ReplaceStrLoop(strHeadName, "\"", "");

            sprintf(szBuf, "{\"pos\":\"%u\", \"zoneid\":\"%u\", \"factionid\":\"%u\", \"factionname\":\"%s\", \"leadername\":\"%s\"},", 
                i + 1, stFaction.wZoneID, stFaction.stBaseData.dwFactionID, strFactionName.c_str(), strHeadName.c_str());

            strFactions += szBuf;
        }

        if(strFactions.m_strData.size() > 1)
        {
            strFactions.m_strData.erase(strFactions.m_strData.end() - 1);
        }

        sprintf(szBuf, "{\"groupid\" : %u, \"factions\" : [%s]}", byGroupID, strFactions.m_strData.c_str());
        return szBuf;
    }
}

string CDealFrame75ShowGvGPair::GetStrFrameDesc()
{
    return SDGBK2UTF8(SHOW_GVG_PAIR_DESC);
}

void CDealFrame75ShowGvGPair::Interface(CRCClient* poClinet)
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string,string> mapField;
    mapField[ARENA_ID] = SDGBK2UTF8("赛区号")+ "|" + GM_PARAM_INT;

    RsponeMsg(poClinet,mapField);
}

void CDealFrame75ShowGvGPair::Deal(CRCClient* poClient,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField)
{
    if(NULL == poClient)
    {
        return;
    }

    if (mapField[ARENA_ID].empty())
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
        poClient->Rspone(strErrorMsg.c_str());
        return;
    }

    UINT32 dwArenaID = SDAtou(mapField[ARENA_ID].c_str());
    Deal(poClient, dwSerialNumber, 0, dwAreaID, dwPlayerID, dwArenaID, 0, 0, NULL, NULL, NULL);
}

void CDealFrame75ShowGvGPair::Deal(CRCClient* poClient, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5 , TCHAR *pszDesc)
{
    ShowGvGPair(poClient, dwParam1);
}

void CDealFrame75ShowGvGPair::ShowGvGPair(CRCClient* poClient, UINT32 dwArenaID)
{
    if(0 == dwArenaID || NULL == poClient)
    {
        return;
    }

    char szRspone[4096] = {0};

    if (!gsapi::GetGS()->IsServerType(TYPE_PK_SERVER))
    {
        sprintf(szRspone, "{\"status\":\"%s\"}", SDGBK2UTF8("本接口只在跨服战服务器下有效!").c_str());
        poClient->Rspone(szRspone);
        return;
    }

    GVG_STAGE eStage = CGvGMgr::Instance()->GetCurStage();
    if(eStage < GVG_3_CHAOS_WAR_END)
    {
        sprintf(szRspone, "{\"status\":\"%s\"}", SDGBK2UTF8("大乱斗仍未开始，对决名单为空!").c_str());
        poClient->Rspone(szRspone);
        return;
    }

    CGvGArena *poArena = CGvGMgr::Instance()->GetArena(dwArenaID);
    if (NULL == poArena)
    {
        sprintf(szRspone, "{\"status\":\"%s\"}", SDGBK2UTF8("找不到该区所在的赛区!").c_str());
        poClient->Rspone(szRspone);
        return;
    }

    if(!poArena->IsLogicDone(GVG_3_CHAOS_WAR_END))
    {
        sprintf(szRspone, "{\"status\":\"%s\"}", SDGBK2UTF8("大乱斗4强名单仍未决出，请耐心等待...").c_str());
        poClient->Rspone(szRspone);
        return;
    }

    const DT_GVG_ARENA_DATA &stArena = poArena->GetArenaData();
    
    std::string strArenaFactions;
    for(UINT8 i = 1; i <= 2; i++)
    {
        std::string strFactions = gvgpairutil::GetJsonFactionByGroup(stArena, i);
        strArenaFactions.append(strFactions);
        if (i < 2)
        {
            strArenaFactions.append(",");
        }
    }

    sprintf(szRspone, "{\"status\":\"success\", \"groups\" : [%s]}", strArenaFactions.c_str());
    poClient->Rspone(szRspone);
}
