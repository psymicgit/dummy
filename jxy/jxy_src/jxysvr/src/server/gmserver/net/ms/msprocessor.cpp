#include "msprocessor.h"
#include "sdutil.h"
#include "framework/gmapplication.h"
#include "protocol/server/protomsgm.h"
#include <common/client/errdef.h>
#include "framework/gmapplication.h"


extern CSDApplication* SDGetApp();

CMSProcessor::CMSProcessor()
{
    Init();
}

CMSProcessor::~CMSProcessor()
{

}

BOOL CMSProcessor::Init()
{
    RegisterCommand(GMGS_CONTROL_ACK, CMSProcessor::OnControlAck);
    RegisterCommand(GMGS_GET_SERVER_INFO_ACK, CMSProcessor::OnGetServerInfoAck);

    return TRUE;
}

CSDProtocol* CMSProcessor::GetProtocol()
{
    return CProtoMSGM::Instance();
}

BOOL CMSProcessor::OnControlAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32			dwTransID  = pPipeHeader->dwTransID;
    if (sizeof(PKT_GMGS_CONTROL_ACK) != dwLen)
    {
        return FALSE;
    }

    PKT_GMGS_CONTROL_ACK* pstAck = (PKT_GMGS_CONTROL_ACK*)pszBody;
    if (NULL == pstAck)
    {
        return FALSE;
    }

    CGMClient * poKSClient	=	NULL;
    poKSClient = ((CGMApplication*)SDGetApp())->GetClientMgr().GetCliSerialNumber(pstAck->dwSerialNumber);
    if (NULL == poKSClient)
    {
        USR_INFO(_SDT("[%s:%d]: Not Find SerialNumber:%u\n"), MSG_MARK, pstAck->dwSerialNumber);
        return FALSE;
    }
    Json::Value json_root;
    json_root["action"] = Json::Value(4);
    Json::Value json_data;
    json_data["state"] = Json::Value(pstAck->wErrCode);
    json_root["data"] = json_data;

    poKSClient->Send(json_root.toStyledString().c_str(), json_root.toStyledString().size());

    return TRUE;
}

BOOL CMSProcessor::OnGetServerInfoAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32			dwTransID  = pPipeHeader->dwTransID;
    if (sizeof(PKT_GMGS_GET_SERVER_INFO_ACK) != dwLen)
    {
        return FALSE;
    }

    PKT_GMGS_GET_SERVER_INFO_ACK* pstAck = (PKT_GMGS_GET_SERVER_INFO_ACK*)pszBody;
    if (NULL == pstAck)
    {
        return FALSE;
    }

    CGMClient * poKSClient	=	NULL;
    poKSClient = ((CGMApplication*)SDGetApp())->GetClientMgr().GetCliSerialNumber(pstAck->dwSerialNumber);
    if (NULL == poKSClient)
    {
        USR_INFO(_SDT("[%s:%d]: Not Find SerialNumber:%u\n"), MSG_MARK, pstAck->dwSerialNumber);
        return FALSE;
    }

    Json::Value json_data;
    for (UINT32 i = 0; i < pstAck->byGroupNum; i++)
    {
        DT_GROUP_DATA	&stDT_GROUP_DATA = pstAck->astGroupDataList[i];
        Json::Value json_group;
        json_group["servernum"] = Json::Value(stDT_GROUP_DATA.byNum);
        json_group["userflag"] = Json::Value(stDT_GROUP_DATA.byUseFlag);
        for (UINT32 j = 0; j < stDT_GROUP_DATA.byNum; j++)
        {
            Json::Value json_server;
            json_server["keepalive"] = Json::Value(stDT_GROUP_DATA.astServerDataList[j].byKeepAlive);
            json_server["server"] = Json::Value(stDT_GROUP_DATA.astServerDataList[j].aszServer);

            char szTemp[256] = {0};
            sprintf(szTemp, "server_%u", j);
            json_group[szTemp] = json_server;
        }

        char szgroup[256] = {0};
        sprintf(szgroup, "group_%u", i);
        json_data[szgroup] = json_group;
    }
    json_data["state"]	= Json::Value(pstAck->wErrCode);
    json_data["areaid"]	= Json::Value(pstAck->dwAreaID);
    json_data["groupnum"]	= Json::Value(pstAck->byGroupNum);

    Json::Value json_root;
    json_root["action"] = Json::Value(2);
    json_root["data"] = json_data;

    poKSClient->Send(json_root.toStyledString().c_str(), json_root.toStyledString().size());

    return TRUE;

}
