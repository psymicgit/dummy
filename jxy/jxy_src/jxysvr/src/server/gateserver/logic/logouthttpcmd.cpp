#include "logouthttpcmd.h"
#include <sdutil.h>
#include <json/json.h>
#include "common/server/utility.h"
#include "common/server/ngstring.h"
#include "framework/gtclient.h"
#include "framework/gtapplication.h"
#include "net/cli/clipktbuilder.h"
#include "common/client/errdef.h"
#include <string.h>
using std::string;

CLogoutHttpCMD::CLogoutHttpCMD()
{
    m_strUrl = ((CGTApplication*)SDGetApp())->GetGTConfig()->GetLogoutUrl();
}

CLogoutHttpCMD::~CLogoutHttpCMD()
{

}

BOOL CLogoutHttpCMD::Init()
{
    CNGString strTemp;
    char *efc = NULL;
    m_strfields.append("_ch=");
    strTemp = m_stLogoutInfo.byAuthType == 0 ? AUTH_TYPE_MAXNET : m_stLogoutInfo.byAuthType;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&deviceid=");
    strTemp = m_stLogoutInfo.strDeviceID;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&userid=");
    strTemp = m_stLogoutInfo.dwUserID;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&playerid=");
    strTemp = m_stLogoutInfo.dwPlayerID;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&_tag=");
    strTemp = 1;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    string strTempUrl = UrlSig();
    curl_easy_setopt(m_pEasyHandle, CURLOPT_URL, strTempUrl.c_str());
    curl_easy_setopt(m_pEasyHandle, CURLOPT_ERRORBUFFER, m_szErrorBuff);
    curl_easy_setopt(m_pEasyHandle, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(m_pEasyHandle, CURLOPT_POST, 1 );
	curl_easy_setopt(m_pEasyHandle, CURLOPT_TIMEOUT, 2);
    curl_easy_setopt(m_pEasyHandle, CURLOPT_POSTFIELDS, m_strfields.c_str());
    return TRUE;
}


VOID CLogoutHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwTransID = dwTransID;
    m_stLogoutInfo = *((SLogoutInfo*)pUserData);
}

BOOL CLogoutHttpCMD::OnExecuted()
{
    if ( FALSE == m_bError)
    {
        USR_INFO( _SDT( "Player:%u url error, msg:%s" ), m_stLogoutInfo.dwPlayerID, m_szErrorBuff );
        return SendMessage(ERR_COMMON::ID_OTHER_ERR);
    }
    USR_INFO( _SDT( "ack:%s" ), SDUTF82Local(SDUTF82Local(m_strResult.c_str()).c_str()).c_str());
    Json::Value root;
    Json::Reader reader;

    if( !reader.parse( m_strResult, root ) )
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), SDUTF82Local(m_strResult.c_str()).c_str() );
        return SendMessage(ERR_COMMON::ID_OTHER_ERR);
    }

    //检测数据个数，如果为零刚为出错，防止崩溃
    if (!root.isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), SDUTF82Local(m_strResult.c_str()).c_str() );
        return SendMessage(ERR_COMMON::ID_OTHER_ERR);
    }

    //解析返回的状态码
    INT16 wErrCode = 0;
    if (root["errcode"].isInt())
    {
        wErrCode = root["errcode"].asInt();
    }
    else
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), SDUTF82Local(m_strResult.c_str()).c_str() );
        return SendMessage(ERR_COMMON::ID_OTHER_ERR);
    }

    return SendMessage(wErrCode);
}

BOOL CLogoutHttpCMD::SendMessage(UINT16 wErrCode)
{
    CGTClient* poCliSession = ((CGTApplication*)SDGetApp())->GetClientMgr()->FindCliSession(m_dwTransID);
    if (NULL == poCliSession)
    {
        DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
        return FALSE;
    }

    UINT16 wMsgID = CCliPktBuilder::Instance()->LogoutAck(wErrCode);

    poCliSession->SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());

    if ( ERR_LOGOUT::ID_SUCCESS == wMsgID)
    {
        poCliSession->Disconnect();//断开连接
    }


    return TRUE;
}
