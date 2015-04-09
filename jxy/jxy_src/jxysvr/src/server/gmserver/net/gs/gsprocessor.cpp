#include "gsprocessor.h"
#include "sdutil.h"
#include "framework/gmapplication.h"
#include "protocol/server/protogmgs.h"
#include <common/client/errdef.h>
#include "framework/gmapplication.h"
#include "logic//basedbccmd.h"


extern CSDApplication* SDGetApp();

CGSProcessor::CGSProcessor()
{
    Init();
}

CGSProcessor::~CGSProcessor()
{

}

BOOL CGSProcessor::Init()
{
    RegisterCommand(GMGS_PARM_INFO_ACK, CGSProcessor::OnParmInfoAck);
    return TRUE;
}

CSDProtocol* CGSProcessor::GetProtocol()
{
    return CProtoGMGS::Instance();
}

BOOL CGSProcessor::OnParmInfoAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32			dwTransID  = pPipeHeader->dwTransID;
    if (sizeof(PKT_GMGS_PARM_INFO_ACK) != dwLen)
    {
        return FALSE;
    }

    PKT_GMGS_PARM_INFO_ACK* pstAck = (PKT_GMGS_PARM_INFO_ACK*)pszBody;
    if (NULL == pstAck)
    {
        return FALSE;
    }

	//CBaseDBCMD* poCMD = ((CGMApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_RECORD_INFO);
	//if (NULL == poCMD)
	//{
	//	return FALSE;
	//}
	//poCMD->SetUserData((VOID*)pstAck, sizeof(PKT_GMGS_PARM_INFO_ACK), ERI_UPDATE);
	//((CGMApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);


    CGMClient * poKSClient	=	NULL;
    poKSClient = ((CGMApplication*)SDGetApp())->GetClientMgr().GetCliSerialNumber(pstAck->dwSerialNumber);
    if (NULL == poKSClient)
    {
		USR_INFO(_SDT("[%s:%d]: Not Find SerialNumber:%u\n"), MSG_MARK, pstAck->dwSerialNumber);
        return FALSE;
    }
    Json::Value json_root;
    json_root["action"] = Json::Value(3);

    Json::Value json_data;
	json_data["playerid"] = Json::Value(pstAck->dwPlayerID);
    json_data["state"] = Json::Value(pstAck->wErrCode);
    json_root["data"] = json_data;

    if (poKSClient->Send(json_root.toStyledString().c_str(), json_root.toStyledString().size()))
    {
		USR_INFO(_SDT("[%s:%d]: Send OK!\n"), MSG_MARK);
    }
	else
	{
		USR_INFO(_SDT("[%s:%d]: Send Faild!\n"), MSG_MARK);
	}
	SDSleep(10);
    return TRUE;
}
