// DealFrame68.cpp: implementation of the CDealFrame68 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe68.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playerbasedatacachemgr.h"
#include "logic/player/playermgr.h"
#include "logic/hero//heropropmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame68::CDealFrame68()
{

}

CDealFrame68::~CDealFrame68()
{

}

void CDealFrame68::Release()
{

}

UINT16 CDealFrame68::GetFrameType()
{
    return 68;
}

void CDealFrame68::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    if((NULL == poClinet) || (NULL == pszParam4))
    {
        return;
    }

    //验证玩家是否存在
    const SPlayerBaseData* pstBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
    if(NULL == pstBaseData)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    //验证是否存在伙伴ID
    SHeroProp *pHero = CHeroPropMgr::Instance()->GetHeroProp(dwParam1);
    if(NULL == pHero)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_HERO_NOT_EXIST_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    //角色为主角
    if (pHero->wKindID < 10)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_HERO_NOT_EXIST_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);

    if(NULL == poPlayer)
    {
        //内存中查找不到，需要到数据库中去查询
        CGetPlayerDataMgr::Instance()->GetPlayerData( dwPlayerID, GET_PLAYERDATATYPE_ADD_HERO, dwPlayerID, dwParam1, qwParam2, 0, "", 0, 0, "",poClinet);  
        return;
    }

    UINT16 wErrCode = poPlayer->GMRecuit(dwParam1, qwParam2);

    string strMsg = GetRsponeResult(wErrCode);
    poClinet->Rspone(strMsg);
}

void CDealFrame68::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
    if (NULL == poClinet)
    {
        return;
    }

    if (mapField[PLAYER_ID].empty())
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
    const SPlayerBaseData* pstBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(unPlayerId);
    if(NULL == pstBaseData)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(unPlayerId);

    if(NULL == poPlayer)
    {
        //内存中查找不到，需要到数据库中去查询
       // CGetPlayerDataMgr::Instance()->GetPlayerData( unPlayerId, GET_PLAYERDATATYPE_ADD_HERO, unPlayerId, dwParam1, qwParam2, 0, "", 0, 0, "",poClinet);  
        return;
    }

   // UINT16 wErrCode = poPlayer->GMRecuit(dwParam1, qwParam2);

   // string strMsg = GetRsponeResult(wErrCode);
   // poClinet->Rspone(strMsg);
}

string CDealFrame68::GetStrFrameType()
{
    return ADD_HERO;
}

void CDealFrame68::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string,string> mapField;
    mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID)+ "|" + GM_PARAM_PLAYER_ID;
   // mapField[ADD_COURAGE_TIMES_PARAM] = SDGBK2UTF8(ADD_COURAGE_TIMES_GBK)+ "|" + GM_PARAM_INT;

    RsponeMsg(poClinet,mapField);
}

string CDealFrame68::GetStrFrameDesc()
{
    return SDGBK2UTF8(ADD_HERO_DESC);
}

