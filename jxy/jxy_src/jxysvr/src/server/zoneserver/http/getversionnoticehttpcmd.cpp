

#include "getversionnoticehttpcmd.h"
#include <sdutil.h>
#include <json/json.h>
#include "common/server/utility.h"
#include "common/server/ngstring.h"
#include "net/cli/clipktbuilder.h"
#include "common/client/errdef.h"




CGetVersionNoticeHttpCMD::CGetVersionNoticeHttpCMD()
{
    memset(&stReq, 0x00, sizeof(stReq));
    memset(&stAck, 0x00, sizeof(stAck));
    m_strUrl = ((CZSApplication*)SDGetApp())->GetZSConfig()->Getversionnotice();
}

CGetVersionNoticeHttpCMD::~CGetVersionNoticeHttpCMD()
{

}


BOOL  CGetVersionNoticeHttpCMD::Init()
{
    CNGString strTemp;
    char *efc = NULL;
    m_strfields.append("_ch=");
    strTemp = AUTH_TYPE_MAXNET;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&cliversion=");
    strTemp = stReq.dwCliVersion;
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

BOOL CGetVersionNoticeHttpCMD::OnExecuted()
{
	if (FALSE == m_bError)
	{
		ResponseMsg(CLIZS_GET_VERSION_NOTICE_ACK2, (CHAR*)(&stAck));
	}
	else
	{
		if(ProcessResult())
		{
			return TRUE;
		}
		memset(&stAck, 0, sizeof(stAck));
		ResponseMsg(CLIZS_GET_VERSION_NOTICE_ACK2, (CHAR*)(&stAck));
	}
    return TRUE;
}

VOID CGetVersionNoticeHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwTransID = dwTransID;
    memcpy(&stReq, pUserData, sizeof(stReq));
}


BOOL CGetVersionNoticeHttpCMD::ProcessResult()
{
	DBG_INFO( _SDT( "[%s: %d]: Result(%s)" ), MSG_MARK, m_strResult.c_str() );
    Json::Value root;
    Json::Reader reader;
    if( !reader.parse( m_strResult, root ) )
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        //OnLoginExAck(	m_dwTransID, stLoginInfo.dwPlayerID, ERR_LOGIN::ID_LS_SERVER_NOT_STARTUP);
        return FALSE;
    }

    //检测数据个数，如果为零刚为出错，防止崩溃
    if (!root.isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        //OnLoginExAck( m_dwTransID, stLoginInfo.dwPlayerID, ERR_LOGIN::ID_LS_SERVER_NOT_STARTUP);
        return FALSE;
    }

    //解析返回的状态码
    if (!root["errcode"].isInt())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }

    if (!root["datas"].isObject())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }

    Json::Value datas = root["datas"];
    if (!datas["versioninfo"].isObject())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    Json::Value versioninfo = datas["versioninfo"];
    if (!versioninfo["versionid"].isInt())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    stAck.stVersionInfo.dwVersionID = versioninfo["versionid"].asInt();

    if (!versioninfo["forceupdateflag"].isInt())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    stAck.stVersionInfo.byForceUpdateFlag = versioninfo["forceupdateflag"].asInt();

    if (!versioninfo["resver"].isInt())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    stAck.stVersionInfo.dwResVer = versioninfo["resver"].asInt();

    if (!versioninfo["uiver"].isInt())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    stAck.stVersionInfo.dwUIVer = versioninfo["uiver"].asInt();

    if (!versioninfo["updatecontent"].isString())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    SDStrncpy((char*)stAck.stVersionInfo.aszUpdateContent, versioninfo["updatecontent"].asCString(), MAX_CONTENT_SIZE - 1);
    if (!versioninfo["updateurl"].isString())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    SDStrncpy(stAck.stVersionInfo.aszUpdateURL, versioninfo["updateurl"].asCString(), MAX_CONTENT_SIZE - 1);

    if (!datas["notice"].isObject())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    Json::Value notice = datas["notice"];
    if (!notice["noticeid"].isInt())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    stAck.stNoticeInfo.dwNoticeID = notice["noticeid"].asInt();

    if (!notice["forcepopflag"].isInt())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    stAck.stNoticeInfo.byForcePopFlag = notice["forcepopflag"].asInt();
    if (!notice["noticecontent"].isString())
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    SDStrncpy(stAck.stNoticeInfo.aszNoticeContent, notice["noticecontent"].asCString(), MAX_CONTENT_SIZE - 1);
    ResponseMsg(CLIZS_GET_VERSION_NOTICE_ACK2, (CHAR*)(&stAck));
    return TRUE;
}

