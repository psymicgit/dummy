// DealFrame65.cpp: implementation of the CDealFrame65 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe65.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playerbasedatacachemgr.h"
#include "logic/player/playermgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame65::CDealFrame65()
{

}

CDealFrame65::~CDealFrame65()
{

}

void CDealFrame65::Release()
{

}

UINT16 CDealFrame65::GetFrameType()
{
    return 65;
}

void CDealFrame65::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    if((NULL == poClinet) || (NULL == pszParam4))
    {
        return;
    }
    const SPlayerBaseData* pstBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwPlayerID);
    if(NULL == pstBaseData)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);

    if(NULL == poPlayer)
    {
        //string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_IS_NOT_ONLINE);
        //poClinet->Rspone(strErrorMsg.c_str());
        //return;
        //内存中查找不到，需要到数据库中去查询
        CGetPlayerDataMgr::Instance()->GetPlayerData( dwPlayerID, GET_PLAYERDATATYPE_GET_FRIENDS_NUM, dwPlayerID, 0, 0, 0, "", 0, 0, "",poClinet);  
        return;
    }

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_NOT_DATA);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    map<string , UINT32> mapData;
    mapData["friendsnum"] = poFriends->GetFriendsNum();
    string strMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &mapData);
    poClinet->Rspone(strMsg);
}

void CDealFrame65::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
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
        CGetPlayerDataMgr::Instance()->GetPlayerData( unPlayerId, GET_PLAYERDATATYPE_GET_FRIENDS_NUM, unPlayerId, 0, 0, 0, "", 0, 0, "",poClinet);  
        return;
    }

    CFriends *poFriends = poPlayer->GetFriends();
    if (NULL == poFriends)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_NOT_DATA);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    map<string , UINT32> mapData;
    mapData["friendsnum"] = poFriends->GetFriendsNum();
    string strMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &mapData);
    poClinet->Rspone(strMsg);
}

string CDealFrame65::GetStrFrameType()
{
    return GET_FRIEDS_NUM;
}

void CDealFrame65::Interface( CRCClient* poClinet )
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

string CDealFrame65::GetStrFrameDesc()
{
    return SDGBK2UTF8(GET_FRIEDS_NUM_DESC);
}

