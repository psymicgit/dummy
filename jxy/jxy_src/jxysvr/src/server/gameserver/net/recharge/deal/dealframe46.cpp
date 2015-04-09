// DealFrame46.cpp: implementation of the CDealFrame46 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe46.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include <json/json.h>
#include "logic/activityencourage/activityencouragemgr.h"
#include <framework/gsapi.h>

#include <db/dbmgr.h>
#include <db/basedbccmd.h>
#include <common/server/utility.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame46::CDealFrame46()
{

}

CDealFrame46::~CDealFrame46()
{

}

void CDealFrame46::Release()
{
	
}

UINT16 CDealFrame46::GetFrameType()
{
	return 46;
}

void CDealFrame46::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    //Json::Value  Json_root;
    //Json::Reader reader;
    //if( !reader.parse( pszParam4, Json_root ) )
    //{
    //    SYS_CRITICAL( _SDT( "json parse error, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}

    ////解析返回的状态码
    //if (!Json_root.isObject())
    //{
    //    SYS_CRITICAL( _SDT( "json is not Object, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}

    ////检测数据个数，如果为零刚为出错，防止崩溃
    //if (!Json_root.size())
    //{
    //    SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}

    //SChangeActivityEncouragePro stChangeActivityEncouragePro;
    //if(Json_root["ZoneID"].isInt())
    //{
    //    stChangeActivityEncouragePro.wZoneID = Json_root["ZoneID"].asInt();
    //}
    //else
    //{
    //    SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}

    //if (0 != stChangeActivityEncouragePro.wZoneID && stChangeActivityEncouragePro.wZoneID != gsapi::GetZoneID() )
    //{
    //    SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}

    //SActivityEncouragePro & stActivityEncouragePro = stChangeActivityEncouragePro.stActivityEncourageProp;
    //if(Json_root["ActivityID"].isInt())
    //{
    //    stActivityEncouragePro.dwActivityID = Json_root["ActivityID"].asInt();
    //}
    //else
    //{
    //    SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}
    //stActivityEncouragePro.wActivityType = ACTE_NORMAL;
    //string strActivityName = "";
    //if(Json_root["ActivityName"].isString())
    //{
    //    strActivityName = Json_root["ActivityName"].asString();
    //}
    //else
    //{
    //    SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}
    //SDStrcpy(stActivityEncouragePro.szActivityName, strActivityName.c_str());
    //string strActivityDesc = "";
    //if(Json_root["ActivityDesc"].isString())
    //{
    //    strActivityDesc = Json_root["ActivityDesc"].asString();
    //}
    //else
    //{
    //    SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}
    //SDStrcpy(stActivityEncouragePro.szActivityDesc, strActivityDesc.c_str());

    //if(Json_root["IsNeedCdKey"].isInt())
    //{
    //    stActivityEncouragePro.byNeedCdKey = Json_root["IsNeedCdKey"].asInt();
    //}
    //else
    //{
    //    SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}

    //CRangPlayerIDVector vecRangPlayerID;
    //Json::Value Json_PlayerID = Json_root["PlayerID"];
    ////解析返回的状态码
    //if (!Json_PlayerID.isObject())
    //{
    //    SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}

    //UINT8 byNum = 0;
    //if(Json_PlayerID["Num"].isInt())
    //{
    //    byNum = Json_PlayerID["Num"].asInt();
    //}
    //else
    //{
    //    SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}

    //for (UINT8 i = 1; i <=  byNum; i++)
    //{
    //    char szTemp[56] = {0};
    //    sprintf(szTemp, "%u", i);

    //    Json::Value Json_Range = Json_PlayerID[szTemp];
    //    //解析返回的状态码
    //    if (!Json_Range.isObject())
    //    {
    //        SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //        poClinet->Rspone("{\"status\":\"Json error\"}");
    //        return;
    //    }
    //    UINT32 dwStartPlayerID;
    //    UINT32 dwEndPlayerID;
    //    if(Json_Range["StartPlayerID"].isInt())
    //    {
    //        dwStartPlayerID = Json_Range["StartPlayerID"].asInt();
    //    }
    //    else
    //    {
    //        SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //        poClinet->Rspone("{\"status\":\"Json error\"}");
    //        return;
    //    }
    //    if(Json_Range["EndPlayerID"].isInt())
    //    {
    //        dwEndPlayerID = Json_Range["EndPlayerID"].asInt();
    //    }
    //    else
    //    {
    //        SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //        poClinet->Rspone("{\"status\":\"Json error\"}");
    //        return;
    //    }
    //    stActivityEncouragePro.mapActivityEncourageStartPlayerID[dwStartPlayerID] = dwEndPlayerID;
    //    stActivityEncouragePro.mapActivityEncourageEndPlayerID[dwEndPlayerID] = dwStartPlayerID;
    //}

    //Json::Value Json_Encourage = Json_root["Encourage"];
    ////解析返回的状态码
    //if (!Json_Encourage.isObject())
    //{
    //    SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}

    //if(Json_Encourage["Num"].isInt())
    //{
    //    byNum = Json_Encourage["Num"].asInt();
    //}
    //else
    //{
    //    SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}

    //CJsonResItemVector vecFactionActivityEncourageItem;
    //for (UINT8 i = 1; i <=  byNum; i++)
    //{
    //    char szTemp[56] = {0};
    //    sprintf(szTemp, "%u", i);
    //    Json::Value Json_Item = Json_Encourage[szTemp];
    //    //解析返回的状态码
    //    if (!Json_Item.isObject())
    //    {
    //        SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //        poClinet->Rspone("{\"status\":\"Json error\"}");
    //        return;
    //    }

    //    CJsonEncourageItem item;
    //    if(Json_Item["KindValue"].isInt())
    //    {
    //        item.dwKindValue = Json_Item["KindValue"].asInt();
    //    }
    //    else
    //    {
    //        SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //        poClinet->Rspone("{\"status\":\"Json error\"}");
    //        return;
    //    }
    //    if(Json_Item["KindType"].isInt())
    //    {
    //        item.byKindType = Json_Item["KindType"].asInt();
    //    }
    //    else
    //    {
    //        SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //        poClinet->Rspone("{\"status\":\"Json error\"}");
    //        return;
    //    }
    //    if(Json_Item["Num"].isInt())
    //    {
    //        item.byNum = Json_Item["Num"].asInt();
    //    }
    //    else
    //    {
    //        SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //        poClinet->Rspone("{\"status\":\"Json error\"}");
    //        return;
    //    }
    //    stActivityEncouragePro.vecActivityEncourageItem.push_back(item);
    //}


    //CC_LOOP_DO
    //CC_LOOP_BREAK(0 == stActivityEncouragePro.byNeedCdKey)
    //string strCdKey = "";
    //Json::Value Json_CdKey = Json_root["CdKey"];
    ////解析返回的状态码
    //CC_LOOP_BREAK(!Json_CdKey.isObject())
    //CC_LOOP_BREAK(!Json_CdKey["Num"].isInt())
    //byNum = Json_CdKey["Num"].asInt();
    //for (UINT8 i = 1; i <=  byNum; i++)
    //{
    //    char szTemp[56] = {0};
    //    sprintf(szTemp, "%u", i);
    //    CC_LOOP_BREAK(!Json_CdKey[szTemp].isString())
    //    strCdKey = Json_CdKey[szTemp].asString();
    //    stActivityEncouragePro.vecActivityEncourageCdKey.push_back(strCdKey);
    //}
    //CC_LOOP_WHILE(0)

    //stChangeActivityEncouragePro.bChangeFlag = TRUE;
    //CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_ACTIVITYENCOURAGE);
    //if(NULL == poCMD)
    //{
    //    SYS_CRITICAL( _SDT( "json error, result:%s" ), pszParam4 );
    //    poClinet->Rspone("{\"status\":\"Json error\"}");
    //    return;
    //}
    //poCMD->SetUserData(&stChangeActivityEncouragePro);
    //gsapi::GetDBMgr()->AddPayCommand(poCMD);
    //poClinet->Rspone("{\"status\":\"success\"}");
}

void CDealFrame46::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	//if (NULL == poClinet)
	//{
	//	return;
	//}

	//if (mapField[ACTIVITY_ENCOURAGE_VALUE_PARAM].empty())
	//{
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//}

	//string strActivity = mapField[ACTIVITY_ENCOURAGE_VALUE_PARAM];

	//Json::Value  Json_root;
	//Json::Reader reader;
	//if( !reader.parse( strActivity.c_str(), Json_root ) )
	//{
	//	SYS_CRITICAL( _SDT( "json parse error, result:%s" ), strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//	return;
	//}

	////解析返回的状态码
	//if (!Json_root.isObject())
	//{
	//	SYS_CRITICAL( _SDT( "json is not Object, result:%s" ),  strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//	return;
	//}

	////检测数据个数，如果为零刚为出错，防止崩溃
	//if (!Json_root.size())
	//{
	//	SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//	return;
	//}

	//SChangeActivityEncouragePro stChangeActivityEncouragePro;
	//if(Json_root["ZoneID"].isInt())
	//{
	//	stChangeActivityEncouragePro.wZoneID = Json_root["ZoneID"].asInt();
	//}
	//else
	//{
	//	SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//	return;
	//}

	//if (0 != stChangeActivityEncouragePro.wZoneID && stChangeActivityEncouragePro.wZoneID != gsapi::GetZoneID() )
	//{
	//	SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//	return;
	//}

	//SActivityEncouragePro & stActivityEncouragePro = stChangeActivityEncouragePro.stActivityEncourageProp;
	//if(Json_root["ActivityID"].isInt())
	//{
	//	stActivityEncouragePro.dwActivityID = Json_root["ActivityID"].asInt();
	//}
	//else
	//{
	//	SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//	return;
	//}
	//stActivityEncouragePro.wActivityType = ACTE_NORMAL;
	//string strActivityName = "";
	//if(Json_root["ActivityName"].isString())
	//{
	//	strActivityName = Json_root["ActivityName"].asString();
	//}
	//else
	//{
	//	SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//	return;
	//}
	//SDStrcpy(stActivityEncouragePro.szActivityName, strActivityName.c_str());
	//string strActivityDesc = "";
	//if(Json_root["ActivityDesc"].isString())
	//{
	//	strActivityDesc = Json_root["ActivityDesc"].asString();
	//}
	//else
	//{
	//	SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//}
	//SDStrcpy(stActivityEncouragePro.szActivityDesc, strActivityDesc.c_str());

	//if(Json_root["IsNeedCdKey"].isInt())
	//{
	//	stActivityEncouragePro.byNeedCdKey = Json_root["IsNeedCdKey"].asInt();
	//}
	//else
	//{
	//	SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//}

	//CRangPlayerIDVector vecRangPlayerID;
	//Json::Value Json_PlayerID = Json_root["PlayerID"];
	////解析返回的状态码
	//if (!Json_PlayerID.isObject())
	//{
	//	SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//}

	//UINT8 byNum = 0;
	//if(Json_PlayerID["Num"].isInt())
	//{
	//	byNum = Json_PlayerID["Num"].asInt();
	//}
	//else
	//{
	//	SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//}

	//for (UINT8 i = 1; i <=  byNum; i++)
	//{
	//	char szTemp[56] = {0};
	//	sprintf(szTemp, "%u", i);

	//	Json::Value Json_Range = Json_PlayerID[szTemp];
	//	//解析返回的状态码
	//	if (!Json_Range.isObject())
	//	{
	//		SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//		poClinet->Rspone(strErrorMsg.c_str());
	//	}
	//	UINT32 dwStartPlayerID;
	//	UINT32 dwEndPlayerID;
	//	if(Json_Range["StartPlayerID"].isInt())
	//	{
	//		dwStartPlayerID = Json_Range["StartPlayerID"].asInt();
	//	}
	//	else
	//	{
	//		SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//		poClinet->Rspone(strErrorMsg.c_str());
	//		return;
	//	}
	//	if(Json_Range["EndPlayerID"].isInt())
	//	{
	//		dwEndPlayerID = Json_Range["EndPlayerID"].asInt();
	//	}
	//	else
	//	{
	//		SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//		poClinet->Rspone(strErrorMsg.c_str());
	//		return;
	//	}
	//	stActivityEncouragePro.mapActivityEncourageStartPlayerID[dwStartPlayerID] = dwEndPlayerID;
	//	stActivityEncouragePro.mapActivityEncourageEndPlayerID[dwEndPlayerID] = dwStartPlayerID;
	//}

	//Json::Value Json_Encourage = Json_root["Encourage"];
	////解析返回的状态码
	//if (!Json_Encourage.isObject())
	//{
	//	SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//	return;
	//}

	//if(Json_Encourage["Num"].isInt())
	//{
	//	byNum = Json_Encourage["Num"].asInt();
	//}
	//else
	//{
	//	SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//	return;
	//}

	//CJsonResItemVector vecFactionActivityEncourageItem;
	//for (UINT8 i = 1; i <=  byNum; i++)
	//{
	//	char szTemp[56] = {0};
	//	sprintf(szTemp, "%u", i);
	//	Json::Value Json_Item = Json_Encourage[szTemp];
	//	//解析返回的状态码
	//	if (!Json_Item.isObject())
	//	{
	//		SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//		poClinet->Rspone(strErrorMsg.c_str());
	//		return;
	//	}

	//	CJsonEncourageItem item;
	//	if(Json_Item["KindValue"].isInt())
	//	{
	//		item.dwKindValue = Json_Item["KindValue"].asInt();
	//	}
	//	else
	//	{
	//		SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//		poClinet->Rspone(strErrorMsg.c_str());
	//		return;
	//	}
	//	if(Json_Item["KindType"].isInt())
	//	{
	//		item.byKindType = Json_Item["KindType"].asInt();
	//	}
	//	else
	//	{
	//		SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//		poClinet->Rspone(strErrorMsg.c_str());
	//		return;
	//	}
	//	if(Json_Item["Num"].isInt())
	//	{
	//		item.byNum = Json_Item["Num"].asInt();
	//	}
	//	else
	//	{
	//		SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//		poClinet->Rspone(strErrorMsg.c_str());
	//		return;
	//	}
	//	stActivityEncouragePro.vecActivityEncourageItem.push_back(item);
	//}


	//CC_LOOP_DO
	//	CC_LOOP_BREAK(0 == stActivityEncouragePro.byNeedCdKey)
	//	string strCdKey = "";
	//Json::Value Json_CdKey = Json_root["CdKey"];
	////解析返回的状态码
	//CC_LOOP_BREAK(!Json_CdKey.isObject())
	//	CC_LOOP_BREAK(!Json_CdKey["Num"].isInt())
	//	byNum = Json_CdKey["Num"].asInt();
	//for (UINT8 i = 1; i <=  byNum; i++)
	//{
	//	char szTemp[56] = {0};
	//	sprintf(szTemp, "%u", i);
	//	CC_LOOP_BREAK(!Json_CdKey[szTemp].isString())
	//		strCdKey = Json_CdKey[szTemp].asString();
	//	stActivityEncouragePro.vecActivityEncourageCdKey.push_back(strCdKey);
	//}
	//CC_LOOP_WHILE(0)

	//	stChangeActivityEncouragePro.bChangeFlag = TRUE;
	//CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_ACTIVITYENCOURAGE);
	//if(NULL == poCMD)
	//{
	//	SYS_CRITICAL( _SDT( "json error, result:%s" ), strActivity.c_str() );
	//	string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_OTHER_ERR);
	//	poClinet->Rspone(strErrorMsg.c_str());
	//	return;
	//}
	//poCMD->SetUserData(&stChangeActivityEncouragePro);
	//gsapi::GetDBMgr()->AddPayCommand(poCMD);
	//string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS);
	//poClinet->Rspone(strErrorMsg.c_str());
}

string CDealFrame46::GetStrFrameType()
{
	return ACTIVITY_ENCOURAGE;
}

void CDealFrame46::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[ACTIVITY_ENCOURAGE_VALUE_PARAM] = SDGBK2UTF8(ACTIVITY_ENCOURAGE_GBK) + "|" + GM_PARAM_STR;

	RsponeMsg(poClinet,mapField);
}

string CDealFrame46::GetStrFrameDesc()
{
	return SDGBK2UTF8(ACTIVITY_ENCOURAGE_DESC);
}

