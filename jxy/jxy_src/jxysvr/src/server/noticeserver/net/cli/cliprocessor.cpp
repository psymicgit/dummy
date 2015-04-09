#include "cliprocessor.h"
#include "sdutil.h"
#include "framework/nsapplication.h"
#include "protocol/server/protocliks.h"
#include <common/client/errdef.h>




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
    CKSClient * pKSClient	=	(CKSClient*)poReceiver;
    if (NULL == pKSClient)
    {
        return FALSE;
    }

    Json::Reader reader;
    Json::Value  json_object;
    if (!reader.parse(pszBody, json_object))
    {
        return FALSE;
    }

    if (CLIENT_STATE_AUTHED != pKSClient->GetLoginFlay())
    {
        switch(json_object["action"].asUInt())
        {
        case 0:
            GetOauthStringReq(poReceiver, json_object["data"]);
            break;
        case 1:
            OauthUserInfoReq(poReceiver, json_object["data"]);
            break;
        default:
            break;
        }
    }
    switch(json_object["action"].asUInt())
    {
    case 0:
        GetOauthStringReq(poReceiver, json_object["data"]);
        break;
    case 1:
        OauthUserInfoReq(poReceiver, json_object["data"]);
        break;
    default:
        break;
    }
    return TRUE;
}

BOOL CCliProcessor::GetOauthStringReq(VOID * poReceiver, Json::Value & json_object)
{
    CKSClient * pKSClient	=	(CKSClient*)poReceiver;
	if (NULL == pKSClient)
	{
		return FALSE;
	}

	Json::Value json_root;
	json_root["action"]		= Json::Value(0);

	Json::Value json_data;
	json_root["oauth_string"]	= Json::Value(pKSClient->GetRandStr());
	json_root["data"]			= json_data;

	pKSClient->Send(json_root.toStyledString().c_str(), json_root.toStyledString().size());
    return TRUE;
}

BOOL CCliProcessor::OauthUserInfoReq(VOID * poReceiver, Json::Value & json_object)
{
    CKSClient * poKSClient	=	(CKSClient*)poReceiver;
    if (NULL == poKSClient)
    {
        return FALSE;
    }

	Json::Value json_root;
	Json::Value json_data;
	json_root["action"]		=	Json::Value(1);
	string	strOauthName		=	json_object["oauth_name"].asString();
	string	strOauthPdw		=	json_object["oauth_string"].asString();
	if(poKSClient->Oauth((char*)strOauthName.c_str(), (char*)strOauthPdw.c_str()))
	{
		poKSClient->SetLoginFlag(CLIENT_STATE_AUTHED);
		json_root["oauth_state"] = Json::Value(0);
	}
	else
	{
		poKSClient->SetLoginFlag(CLIENT_STATE_UNAUTHED);
		json_root["oauth_state"] = Json::Value(1);
	}
	json_root["data"] = json_data;
	poKSClient->Send(json_root.toStyledString().c_str(), json_root.toStyledString().size());
    return TRUE;
}




