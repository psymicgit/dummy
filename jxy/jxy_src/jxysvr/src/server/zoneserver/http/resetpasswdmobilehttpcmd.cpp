

#include "resetpasswdmobilehttpcmd.h"
#include <sdutil.h>
#include <json/json.h>
#include "common/server/utility.h"
#include "common/server/ngstring.h"
#include "net/cli/clipktbuilder.h"
#include "common/client/errdef.h"




CResetPasswdMobileHttpCMD::CResetPasswdMobileHttpCMD()
{
    memset(&stReq, 0x00, sizeof(stReq));
    memset(&stAck, 0x00, sizeof(stAck));
    m_strUrl = ((CZSApplication*)SDGetApp())->GetZSConfig()->Getfindpwd_sendsmscode();
}

CResetPasswdMobileHttpCMD::~CResetPasswdMobileHttpCMD()
{

}

BOOL  CResetPasswdMobileHttpCMD::Init()
{
    CNGString strTemp;
    char *efc = NULL;
    m_strfields.append("_ch=");
    strTemp = AUTH_TYPE_MAXNET;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&mobile=");
    strTemp = (char*)stReq.abyMObile;
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

BOOL CResetPasswdMobileHttpCMD::OnExecuted()
{
    if (FALSE == m_bError)
    {
        stAck.wErrCode = ERR_RESET_PASSWD_MOBILE::ID_SUCCESS;
        ResponseMsg(CLIZS_RESET_PASSWD_MOBILE_ACK, (CHAR*)(&stAck));
    }
    else
    {
        ProcessResult();
    }
    return TRUE;
}


VOID CResetPasswdMobileHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwTransID = dwTransID;
    memcpy(&stReq, pUserData, sizeof(stReq));
}




VOID CResetPasswdMobileHttpCMD::ProcessResult()
{
    DBG_INFO( _SDT( "[%s: %d]: Result(%s)" ), MSG_MARK, m_strResult.c_str() );

    Json::Value root;
    Json::Reader reader;
    if( !reader.parse( m_strResult, root ) )
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        //OnLoginExAck(	m_dwTransID, stLoginInfo.dwPlayerID, ERR_LOGIN::ID_LS_SERVER_NOT_STARTUP);
        return;
    }

    //检测数据个数，如果为零刚为出错，防止崩溃
    if (!root.isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        //OnLoginExAck( m_dwTransID, stLoginInfo.dwPlayerID, ERR_LOGIN::ID_LS_SERVER_NOT_STARTUP);
        return;
    }

    //解析返回的状态码
    if (!root["errcode"].isInt())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return;
    }
    stAck.wErrCode = root["errcode"].asInt();


    if (ERR_RESET_PASSWD_MOBILE::ID_SUCCESS != stAck.wErrCode)
    {
        ResponseMsg(CLIZS_RESET_PASSWD_MOBILE_ACK, (CHAR*)(&stAck));
        return;
    }

    if (!root["datas"].isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        return;
    }
    Json::Value datas = root["datas"];

    if (!datas["token"].isString())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return;
    }
    SDStrncpy((char*)stAck.abyToken, datas["token"].asCString(), MAX_RESET_PASSWD_TOKEN_NUM);
    //解析返回的状态码
    if (!datas["countdown"].isInt())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return;
    }
    stAck.wCountdown = datas["countdown"].asInt();
    ResponseMsg(CLIZS_RESET_PASSWD_MOBILE_ACK, (CHAR*)(&stAck));
}
