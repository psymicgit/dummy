

#include "bindmobilehttpcmd.h"
#include <sdutil.h>
#include <json/json.h>
#include "common/server/utility.h"
#include "common/server/ngstring.h"
#include "framework/gtclient.h"
#include "framework/gtapplication.h"
#include "net/cli/clipktbuilder.h"
#include "common/client/errdef.h"
#include "noticemgr.h"




CBindMobileHttpCMD::CBindMobileHttpCMD()
{
    m_strUrl = ((CGTApplication*)SDGetApp())->GetGTConfig()->GetBindMobileUrl();
}

CBindMobileHttpCMD::~CBindMobileHttpCMD()
{

}

BOOL  CBindMobileHttpCMD::Init()
{
    CNGString strTemp;
    char *efc = NULL;
    m_strfields.append("_ch=");
    strTemp = AUTH_TYPE_MAXNET;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&userid=");
    strTemp = stBindInfo.dwUserID;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&phone=");
    TCHAR aszMobile[MAX_MOBILE_LENGTH + 1] = {0};
    SDStrncpy(aszMobile, (TCHAR*)stBindInfo.abyMObile, MAX_MOBILE_LENGTH);
    strTemp = aszMobile;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);


    //临时路径打包上签名
    string	strTempUrl = m_strUrl;
    UINT8	abyDigest[16] = {0};
    string	strInput = m_strfields;
    strInput.append(CENTER_SIGN);
    SDMD5(abyDigest, (UINT8*)strInput.c_str(), strInput.length());
    CHAR szTemp[32] = {0};
    CHAR szHexDigest[256] = {0};
    for (UINT8 byIdx = 0; byIdx < 16; byIdx++)
    {
        sprintf(szTemp, "%02x", (UINT8)abyDigest[byIdx]);
        strcat(szHexDigest, szTemp);
    }
    strTempUrl.append("?_sig=");
    strTemp = szHexDigest;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    strTempUrl.append(efc);
    curl_free(efc);
    curl_easy_setopt(m_pEasyHandle, CURLOPT_URL, strTempUrl.c_str());
    curl_easy_setopt(m_pEasyHandle, CURLOPT_ERRORBUFFER, m_szErrorBuff);
    curl_easy_setopt(m_pEasyHandle, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(m_pEasyHandle, CURLOPT_POST, 1 );
	curl_easy_setopt(m_pEasyHandle, CURLOPT_TIMEOUT, 2);
    curl_easy_setopt(m_pEasyHandle, CURLOPT_POSTFIELDS, m_strfields.c_str());
    return TRUE;
}

BOOL CBindMobileHttpCMD::OnExecuted()
{
    if (FALSE == m_bError)
    {
        USR_INFO( _SDT( "UserID:%u url error, msg:%s" ), stBindInfo.dwUserID, m_szErrorBuff );
        OnBindMobileAck( m_dwTransID, ERR_COMMON::ID_OTHER_ERR);
    }
    else
    {
        ProcessResult();
    }
    return TRUE;
}

VOID CBindMobileHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwTransID = dwTransID;
    stBindInfo = *((SBindInfo*)pUserData);
}


VOID CBindMobileHttpCMD::ProcessResult()
{
    USR_INFO( _SDT( "ack:%s" ), SDUTF82Local(m_strResult.c_str()).c_str());
    Json::Value root;
    Json::Reader reader;
    if( !reader.parse( m_strResult, root ) )
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s,msg:%s" ), SDUTF82Local(m_strResult.c_str()).c_str(), m_szErrorBuff );
        OnBindMobileAck( m_dwTransID, ERR_COMMON::ID_OTHER_ERR);
        return;
    }

    //检测数据个数，如果为零刚为出错，防止崩溃
    if (!root.isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s,msg:%s" ), SDUTF82Local(m_strResult.c_str()).c_str(), m_szErrorBuff );
        OnBindMobileAck( m_dwTransID, ERR_COMMON::ID_OTHER_ERR);
        return;
    }

    //解析返回的状态码
    INT16 wErrCode = 0;
    if (root["errcode"].isInt())
    {
        wErrCode = root["errcode"].asInt();
    }
    else
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s,msg:%s" ), SDUTF82Local(m_strResult.c_str()).c_str(), m_szErrorBuff );
        OnBindMobileAck( m_dwTransID, ERR_COMMON::ID_OTHER_ERR);
        return;
    }

    INT16 wCountDown = 0;
    if (root["datas"].isObject())
    {
        Json::Value datas = root["datas"];
        if (datas["countdown"].isInt())
        {
            wCountDown = datas["countdown"].asInt();
        }
    }
    else
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s,msg:%s" ), SDUTF82Local(m_strResult.c_str()).c_str(), m_szErrorBuff );
        OnBindMobileAck( m_dwTransID, ERR_COMMON::ID_OTHER_ERR);
        return;
    }

    OnBindMobileAck(m_dwTransID, wErrCode, wCountDown);
}




BOOL CBindMobileHttpCMD::OnBindMobileAck (UINT32 dwTransID, UINT16 wErrCode)
{
    //找到client
    CGTClient* poCliSession = ((CGTApplication*)SDGetApp())->GetClientMgr()->FindCliSession(dwTransID);
    if (NULL == poCliSession)
    {
        DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
        return FALSE;
    }

    UINT16 wMsgID = CCliPktBuilder::Instance()->BindMobileAck(wErrCode);

    //发送到客户端
    poCliSession->SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());

    return TRUE;
}


BOOL CBindMobileHttpCMD::OnBindMobileAck( UINT32 dwTransID, UINT16 wErrCode, UINT16 wCountDown)
{
    //找到client
    CGTClient* poCliSession = ((CGTApplication*)SDGetApp())->GetClientMgr()->FindCliSession(dwTransID);
    if (NULL == poCliSession)
    {
        DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
        return FALSE;
    };

    UINT16 wMsgID = CCliPktBuilder::Instance()->BindMobileAck(wErrCode, wCountDown);

    //发送到客户端
    poCliSession->SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());

    return TRUE;
}



