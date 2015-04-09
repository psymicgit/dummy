///<------------------------------------------------------------------------------
//< @file:   recharge\deal\dealframe76setgvgpair.cpp
//< @author: hongkunan
//< @date:   2014年8月13日 10:35:31
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "dealframe76setgvgpair.h"
#include "framework/gsapi.h"
#include "logic/gvg/global/gvgmgr.h"
#include "common/server/ngstring.h"
#include "net/recharge/rcclient.h"
#include "framework/gsapplication.h"
#include "dll/sdu/sdserverid.h"

#define OLD_POS "old_pos"
#define NEW_POS "new_pos"

string CDealFrame76SetGvGPair::GetStrFrameDesc()
{
    return SDGBK2UTF8(SET_GVG_PAIR_DESC);
}

void CDealFrame76SetGvGPair::Interface(CRCClient* poClinet)
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string,string> mapField;
    mapField[ZONE_ID] = SDGBK2UTF8("区号")+ "|" + GM_PARAM_INT;
    mapField[OLD_POS] = SDGBK2UTF8("门派的旧位置1~8")+ "|" + GM_PARAM_INT;
    mapField[NEW_POS] = SDGBK2UTF8("门派的新位置1~8")+ "|" + GM_PARAM_INT;

    RsponeMsg(poClinet,mapField);
}

void CDealFrame76SetGvGPair::Deal(CRCClient* poClient,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField)
{
    if(NULL == poClient)
    {
        return;
    }

    if (mapField[ZONE_ID].empty())
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
        poClient->Rspone(strErrorMsg.c_str());
        return;
    }

    UINT32 dwZoneID = SDAtou(mapField[ZONE_ID].c_str());
    UINT8 byOldPos = (UINT8)SDAtou(mapField[OLD_POS].c_str());
    UINT8 byNewPos = (UINT8)SDAtou(mapField[NEW_POS].c_str());
    
    Deal(poClient, dwSerialNumber, 0, dwAreaID, dwPlayerID, dwZoneID, byOldPos, byNewPos, NULL, NULL, NULL);
}

void CDealFrame76SetGvGPair::Deal(CRCClient* poClient, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5 , TCHAR *pszDesc)
{
    SwapFactionPos(poClient, dwParam1, (UINT8)qwParam2, (UINT8)qwParam3);
}

void CDealFrame76SetGvGPair::SwapFactionPos(CRCClient* poClient, UINT32 dwZoneID, UINT8 byOldPos, UINT8 byNewPos)
{
    if(0 == dwZoneID)
    {
        return;
    }

    char szReply[4096] = {0};

    if (!gsapi::GetGS()->IsServerType(TYPE_PK_SERVER))
    {
        sprintf(szReply, "{\"status\":\"%s\"}", SDGBK2UTF8("本接口只在跨服战服务器下有效!").c_str());
        poClient->Rspone(szReply);
        return;
    }

    if (!((1 <= byOldPos && byOldPos <= GVG_16_TO_8_FACTION_CNT) && (1 <= byNewPos && byNewPos <= GVG_16_TO_8_FACTION_CNT)))
    {
        sprintf(szReply, "{\"status\":\"%s\"}", SDGBK2UTF8("传入的门派位置超出范围，值必须是1~16!").c_str());
        poClient->Rspone(szReply);
        return;
    }

    if (byOldPos == byNewPos)
    {
        sprintf(szReply, "{\"status\":\"%s\"}", SDGBK2UTF8("两个门派在同一位置，无法交换!").c_str());
        poClient->Rspone(szReply);
        return;
    }

    GVG_STAGE eStage = CGvGMgr::Instance()->GetCurStage();
    if(GVG_3_CHAOS_WAR_END != eStage)
    {
        if(eStage < GVG_3_CHAOS_WAR_END)
        {
            sprintf(szReply, "{\"status\":\"%s\"}", SDGBK2UTF8("大乱斗仍未开始，无法操作!").c_str());
        }
        else
        {
            sprintf(szReply, "{\"status\":\"%s\"}", SDGBK2UTF8("16进8已开始，无法操作!").c_str());
        }

        poClient->Rspone(szReply);
        return;
    }

    CGvGArena *poArena = CGvGMgr::Instance()->GetArenaByZoneID(dwZoneID);
    if (NULL == poArena)
    {
        sprintf(szReply, "{\"status\":\"%s\"}", SDGBK2UTF8("找不到该区所在的赛区!").c_str());
        poClient->Rspone(szReply);
        return;
    }

    if(!poArena->IsLogicDone(GVG_3_CHAOS_WAR_END))
    {
        sprintf(szReply, "{\"status\":\"%s\"}", SDGBK2UTF8("大乱斗4强名单仍未决出，请耐心等待...").c_str());
        poClient->Rspone(szReply);
        return;
    }

    DT_GVG_ARENA_DATA &stArena = poArena->GetArenaData();
    BYTE byGroupID = gvgutil::GetGroupIDByZone(dwZoneID, stArena.stGroupZoneIDList);
    if(0 == byGroupID)
    {
        sprintf(szReply, "{\"status\":\"%s%u%s\"}", SDGBK2UTF8("找不到").c_str(), dwZoneID, SDGBK2UTF8("区所在的组!").c_str());
        poClient->Rspone(szReply);
        return;
    }

    DT_16_TO_8_TO_4_WAR_INFO &stWar16To8To4Info = stArena.stWar16To8List.astWar16To8To4Info[byGroupID - 1];
    if (byOldPos > stWar16To8To4Info.byFactionCnt || byNewPos > stWar16To8To4Info.byFactionCnt)
    {
        sprintf(szReply, "{\"status\":\"%s\"}", SDGBK2UTF8("传入的门派位置太大!").c_str());
        poClient->Rspone(szReply);
        return;
    }

    DT_GVG_FACTION &stSrcFaction = stWar16To8To4Info.astFactionList[byOldPos - 1];
    DT_GVG_FACTION &stDstFaction = stWar16To8To4Info.astFactionList[byNewPos - 1];

    if (0 == stSrcFaction.stBaseData.dwFactionID)
    {
        sprintf(szReply, "{\"status\":\"%s\"}", SDGBK2UTF8("操作失败：无法对空门派进行操作").c_str());
        poClient->Rspone(szReply);
        return;
    }

    if (0 == stDstFaction.stBaseData.dwFactionID)
    {
        sprintf(szReply, "{\"status\":\"%s\"}", SDGBK2UTF8("操作失败：目标位置上必须有门派").c_str());
        poClient->Rspone(szReply);
        return;
    }  

    const DT_GVG_FACTION stSwapFaction = stSrcFaction;
    stSrcFaction = stDstFaction;
    stDstFaction = stSwapFaction;

    sprintf(szReply, "{\"status\":\"success\", \"msg\": \"%s[%s]%s[%s]%s\"}", 
        SDGBK2UTF8("操作成功：成功交换").c_str(), stDstFaction.stBaseData.aszFactionName, SDGBK2UTF8("门派和").c_str(), stSrcFaction.stBaseData.aszFactionName, SDGBK2UTF8("门派").c_str());

    poClient->Rspone(szReply);
}
