

#include "levelhttpcmd.h"
#include <sdutil.h>
#include <json/json.h>
#include "common/server/utility.h"
#include "common/server/ngstring.h"
#include <db/autosqlbuf.h>
#include "common/client/errdef.h"
#include "urlpropmgr.h"


CLevelHttpCMD::CLevelHttpCMD()
{
    m_strUrl = CUrlPropMgr::Instance()->GetUrlLevel();
}

CLevelHttpCMD::~CLevelHttpCMD()
{

}

BOOL CLevelHttpCMD::Init()
{
    CNGString strTemp;
    char *efc = NULL;
    m_strfields.append("playerid=");
    strTemp = stLevelInfo.dwPlayerID;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&level=");
    strTemp = stLevelInfo.wLevel;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

	m_strfields.append("&deviceid=");
	strTemp = stLevelInfo.strDeviceID;
	efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
	m_strfields.append(efc);
	curl_free(efc);

	m_strfields.append("&_ch=");
	strTemp = stLevelInfo.byAuthType;
	efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
	m_strfields.append(efc);
	curl_free(efc);

	m_strfields.append("&ip=");
	strTemp = stLevelInfo.strIp;
	efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
	m_strfields.append(efc);
	curl_free(efc);
	
    //地址
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
    curl_easy_setopt(m_pEasyHandle, CURLOPT_POSTFIELDS, m_strfields.c_str());
    return TRUE;
}

BOOL CLevelHttpCMD::OnExecuted()
{
    if (FALSE == m_bError)
    {
        return FALSE;
    }

    Json::Value root;
    Json::Reader reader;
    if( !reader.parse( m_strResult, root ) )
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }

    //检测数据个数，如果为零刚为出错，防止崩溃
    if (!root.isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }

    //解析返回的状态码
    INT32 nErrCode = 0;
    if (!root["errcode"].isInt())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    nErrCode = root["errcode"].asInt();
	if (0 != nErrCode)
	{
		SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
		return FALSE;
	}
	return TRUE;
}

VOID CLevelHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwTransID = dwTransID;
    stLevelInfo = *((SLevelInfo*)pUserData);
}