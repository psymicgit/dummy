#include "cliprocessor.h"
#include "sdutil.h"
#include "framework/gmapplication.h"
#include "protocol/server/protocliks.h"
#include "protocol/server/protogmgs.h"
#include "protocol/server/protomsgm.h"
#include <common/client/errdef.h>
#include "common/server/utility.h"
#include "logic/basedbccmd.h"

enum em_action
{
    em_oauthstring	=	0,
    em_oauthuser		=	1,
    em_serverinfo		=	2,
    em_setplayerinfo	=	3,
    em_controlserver	=	4,
    em_getzoneinfo	=	5,
};


CCliProcessor::CCliProcessor()
{
    Init();
}

CCliProcessor::~CCliProcessor()
{

}

BOOL CCliProcessor::Init()
{
    return TRUE;
}

BOOL CCliProcessor::ProcessPacket(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    CGMClient * poGmClient	=	(CGMClient*)poReceiver;
    if (NULL == poGmClient)
    {
        return FALSE;
    }

	//UINT32	v_nStartFlag = 0;
	//UINT32	v_nEndFlag = 0;

	//UINT32	v_nStartIndex = 0;
	//UINT32	v_nEndIndex = 0;
	//for (UINT32 i = 0; i < dwLen; i++)
	//{
	//	if ('{' == pszBody[i])
	//	{
	//		v_nStartFlag++;
	//		if (0 == v_nStartIndex)
	//		{
	//			v_nStartIndex = i;
	//		}
	//	}
	//	if ('}' == pszBody[i])
	//	{
	//		v_nEndFlag++;
	//		v_nEndIndex = i;
	//	}
	//	if (0 != v_nStartFlag && v_nStartFlag == v_nEndFlag)
	//	{
	//		break;
	//	}
	//}



    Json::Reader reader;
    Json::Value  json_object;
    if (!reader.parse(pszBody, json_object))
    {
        return FALSE;
    }

    if (CLIENT_STATE_AUTHED != poGmClient->GetLoginFlag())
    {
        switch(json_object["action"].asUInt())
        {
        case em_oauthstring:
            GetOauthStringReq(poReceiver, json_object["data"]);
            break;
        case em_oauthuser:
            OauthUserInfoReq(poReceiver, json_object["data"]);
            break;
        default:
            DefaultReq(poReceiver, json_object["data"]);
            break;
        }
        return TRUE;
    }
    switch(json_object["action"].asUInt())
    {
    case em_oauthstring:
        GetOauthStringReq(poReceiver, json_object["data"]);
        break;
    case em_oauthuser:
        OauthUserInfoReq(poReceiver, json_object["data"]);
        break;
    case em_serverinfo:
        GetServerInfoReq(poReceiver, json_object["data"]);
        break;
    case em_setplayerinfo:
        SetPlayerInfoReq(poReceiver, json_object["data"]);
        break;
    case em_controlserver:
        ConstrolServerReq(poReceiver, json_object["data"]);
        break;
    case em_getzoneinfo:
        GetZoneInfoReq(poReceiver, json_object["data"]);
        break;
    default:
        DefaultReq(poReceiver, json_object["data"]);
        break;
    }
    return TRUE;
}

BOOL CCliProcessor::GetOauthStringReq(VOID * poReceiver, Json::Value & json_object)
{
    CGMClient * poGmClient	=	(CGMClient*)poReceiver;
    if (NULL == poGmClient)
    {
        return FALSE;
    }

    char *	pRandStr = poGmClient->GetRandStr();

    Json::Value json_root;
    json_root["action"] = Json::Value(em_oauthstring);

    Json::Value json_data;
    json_data["oauth_string"] = Json::Value(pRandStr);
    json_root["data"] = json_data;

    poGmClient->Send(json_root.toStyledString().c_str(), json_root.toStyledString().size());
    return TRUE;
}

BOOL CCliProcessor::OauthUserInfoReq(VOID * poReceiver, Json::Value & json_object)
{
    CGMClient * poGmClient	=	(CGMClient*)poReceiver;
    if (NULL == poGmClient)
    {
        return FALSE;
    }

    string	strOauthName		=	json_object["oauth_name"].asString();
    string	strOauthString	=	json_object["oauth_string"].asString();


    Json::Value json_root;
    Json::Value json_data;
    json_root["action"] = Json::Value(em_oauthuser);

    if(poGmClient->Oauth((char*)strOauthName.c_str(), (char*)strOauthString.c_str()))
    {
        poGmClient->SetLoginFlag(CLIENT_STATE_AUTHED);
        json_data["oauth_state"]	= Json::Value(0);
        json_data["oauth_msg"]		= Json::Value("认证成功");
    }
    else
    {
        poGmClient->SetLoginFlag(CLIENT_STATE_UNAUTHED);
        json_data["oauth_state"] = Json::Value(em_oauthuser);
        json_data["oauth_msg"]		= Json::Value("认证失败");
    }

    json_root["data"] = json_data;
    poGmClient->Send(json_root.toStyledString().c_str(), json_root.toStyledString().size());
    return TRUE;
}

BOOL CCliProcessor::GetServerInfoReq(VOID * poReceiver, Json::Value & json_object)
{
    CGMClient * poGmClient	=	(CGMClient*)poReceiver;
    if (NULL == poGmClient)
    {
        return FALSE;
    }

    UINT32	dwAreaID		=	json_object["areaid"].asUInt();
    PKT_GMGS_GET_SERVER_INFO_REQ stReq;
    memset(&stReq, 0x00, sizeof(PKT_GMGS_GET_SERVER_INFO_REQ));
    stReq.dwAreaID = dwAreaID;
    stReq.dwSerialNumber = ((CGMApplication*)SDGetApp())->GetClientMgr().SetCliSerialNumber(poGmClient);


	DBG_INFO(_SDT("SerialNumber:%u, AreaID:%u"), stReq.dwSerialNumber, stReq.dwAreaID);

    if (!((CGMApplication*)SDGetApp())->SendMsg2MonitorServer(GMGS_GET_SERVER_INFO_REQ, dwAreaID, (CHAR*)&stReq))
    {
        Json::Value json_root;
		json_root["action"] = Json::Value(em_serverinfo);
		Json::Value json_data;
		json_data["state"] = Json::Value(0);
		json_data["msg"] = Json::Value("服务器未连接");
		json_root["data"] = json_data;
		poGmClient->Send(json_root.toStyledString().c_str(), json_root.toStyledString().size());
	}
	return TRUE;
}


BOOL CCliProcessor::SetPlayerInfoReq(VOID * poReceiver, Json::Value & json_object)
{
    CGMClient * poGmClient	=	(CGMClient*)poReceiver;
    if (NULL == poGmClient)
    {
        return FALSE;
    }

    UINT32	dwAreaID		=	0;
    UINT32	dwPlayerID	=	0;
    UINT8	byType		=	0;
    UINT32	dwParm1		=	0;
    UINT32	dwParm2		=	0;
    UINT32	dwParm3		=	0;
    std::string strParm4;

    if(json_object["areaid"].isInt())
    {
        dwAreaID		=	json_object["areaid"].asUInt();
    }
    if(json_object["playerid"].isInt())
    {
        dwPlayerID	=	json_object["playerid"].asUInt();
    }
    if(json_object["type"].isInt())
    {
        byType		=	json_object["type"].asUInt();
    }
    if(json_object["parm1"].isInt())
    {
        dwParm1		=	json_object["parm1"].asUInt();
    }
    if(json_object["parm2"].isInt())
    {
        dwParm2		=	json_object["parm2"].asUInt();
    }
    if(json_object["parm3"].isInt())
    {
        dwParm3		=	json_object["parm3"].asUInt();
    }
    if(json_object["parm4"].isString())
    {
        strParm4	=	json_object["parm4"].asString();
    }

    PKT_GMGS_PARM_INFO_REQ stReq;
    memset(&stReq, 0x00, sizeof(stReq));
    stReq.dwPlayerID = dwPlayerID;
    stReq.dwAreaID = dwAreaID;
    stReq.byAction = byType;
    stReq.dwParam1 = dwParm1;
    stReq.qwParam2 = dwParm2;
    stReq.qwParam3 = dwParm3;
    SDStrncpy(stReq.aszParam4, strParm4.c_str(), MAX_PARM_LEN);
    stReq.dwSerialNumber = ((CGMApplication*)SDGetApp())->GetClientMgr().SetCliSerialNumber(poGmClient);


    //CBaseDBCMD* poCMD = ((CGMApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_RECORD_INFO);
    //poCMD->SetUserData((VOID*)&stReq, sizeof(PKT_GMGS_PARM_INFO_REQ), ERI_ADD);
    //((CGMApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);

    //if(NULL == poCMD)
    //{
    //    Json::Value json_root;
    //    json_root["action"] = Json::Value(em_serverinfo);
    //    Json::Value json_data;
    //    json_data["state"] = Json::Value(5);
    //    json_data["msg"] = Json::Value("GM服务器未知错误");
    //    json_root["data"] = json_data;
    //    poGmClient->Send(json_root.toStyledString().c_str(), json_root.toStyledString().size());
    //    return FALSE;
    //}

    if (!((CGMApplication*)SDGetApp())->SendMsg2GameServer(dwAreaID, dwPlayerID, (CHAR*)&stReq))
    {
        Json::Value json_root;
        json_root["action"] = Json::Value(em_serverinfo);
        Json::Value json_data;
        json_data["state"] = Json::Value(5);
        json_data["msg"] = Json::Value("服务器未连接");
        json_root["data"] = json_data;
        poGmClient->Send(json_root.toStyledString().c_str(), json_root.toStyledString().size());
    }
    return TRUE;
}



BOOL CCliProcessor::ConstrolServerReq(VOID * poReceiver, Json::Value & json_object)
{
    CGMClient * poGmClient	=	(CGMClient*)poReceiver;
    if (NULL == poGmClient)
    {
        return FALSE;
    }

    UINT32	dwAreaID	 = 0;
    UINT8	byUseFlag = 0;
    if(json_object["areaid"].isInt())
    {
        dwAreaID		=	json_object["areaid"].asUInt();
    }
    if(json_object["useflag"].isInt())
    {
        byUseFlag	=	json_object["useflag"].asInt();
    }

    PKT_GMGS_CONTROL_REQ stReq;
    memset(&stReq, 0x00, sizeof(PKT_GMGS_CONTROL_REQ));
    stReq.dwSerialNumber = ((CGMApplication*)SDGetApp())->GetClientMgr().SetCliSerialNumber(poGmClient);
    stReq.byUseFlag = byUseFlag;
    stReq.dwAreaID = dwAreaID;
    const char *p = json_object["server"].asCString();
    if (NULL != p)
    {
        strcat(stReq.aszServer, json_object["server"].asCString());
    }

    if (!((CGMApplication*)SDGetApp())->SendMsg2MonitorServer(GMGS_CONTROL_REQ, dwAreaID, (CHAR*)&stReq))
    {
        Json::Value json_root;
        json_root["action"] = Json::Value(em_controlserver);
        Json::Value json_data;
        json_data["state"] = Json::Value(5);
        json_data["msg"] = Json::Value("服务器未连接");
        json_root["data"] = json_data;
        poGmClient->Send(json_root.toStyledString().c_str(), json_root.toStyledString().size());
    }
    return TRUE;
}

BOOL CCliProcessor::GetZoneInfoReq(VOID* poReceiver, Json::Value & json_object)
{
    CGMClient * poGmClient	=	(CGMClient*)poReceiver;
    if (NULL == poGmClient)
    {
        return FALSE;
    }
    Json::Value json_root;
    Json::Value json_data;
    json_root["action"] = Json::Value(em_getzoneinfo);

    UINT32	dwNum = 0;
    CPipeChannelMap &mapPipeChannel = ((CGMApplication*)SDGetApp())->GetPipeChannelMap();
    for (CPipeChannelMapItr itr = mapPipeChannel.begin(); itr != mapPipeChannel.end(); itr++)
    {
        CSDServerID oServerID(itr->first);
        if (TYPE_MS_SERVER == oServerID.GetServerType())
        {
            char v_szTemp[32] = {0};
            sprintf(v_szTemp, "area_%0d", ++dwNum);
            json_data[v_szTemp] = Json::Value(oServerID.GetAreaID());
        }
    }
    json_data["num"] = Json::Value(dwNum);
    json_root["data"] = json_data;
    poGmClient->Send(json_root.toStyledString().c_str(), json_root.toStyledString().size());
    return TRUE;
}

BOOL CCliProcessor::DefaultReq(VOID* poReceiver, Json::Value & json_object)
{
    CGMClient * poGmClient	=	(CGMClient*)poReceiver;
    if (NULL == poGmClient)
    {
        return FALSE;
    }

    Json::Value json_root;
    json_root["action"] = Json::Value(0xFF);

    Json::Value json_data;
    json_data["state"] = Json::Value("设备未登录，或者类型不正确");
    json_root["data"] = json_data;

    poGmClient->Send(json_root.toStyledString().c_str(), json_root.toStyledString().size());
    return TRUE;
}

