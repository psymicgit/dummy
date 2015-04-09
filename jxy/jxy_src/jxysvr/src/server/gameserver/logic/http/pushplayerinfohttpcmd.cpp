
//< @file:   logic\http\pushplayerinfohttpcmd.cpp
//< @author: hongkunan
//< @date:   2014年4月1日 9:28:41
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "pushplayerinfohttpcmd.h"
#include <json/json.h>
#include "urlpropmgr.h"

CPushPlayerInfoHttpCMD::CPushPlayerInfoHttpCMD()
{
    m_strUrl = CUrlPropMgr::Instance()->GetUrlPushPlayerInfo();
}

CPushPlayerInfoHttpCMD::~CPushPlayerInfoHttpCMD()
{

}

void CPushPlayerInfoHttpCMD::EscapeFields()
{
    CNGString strTemp;
    char *szEscape = NULL;
    m_strfields.append("playerid=");
    strTemp = m_stPlayerInfo.dwPlayerID;
    szEscape = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(szEscape);
    curl_free(szEscape);

    m_strfields.append("&level=");
    strTemp = m_stPlayerInfo.wLevel;
    szEscape = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(szEscape);
    curl_free(szEscape);

    m_strfields.append("&careerid=");
    strTemp = m_stPlayerInfo.wCoachHeroID;
    szEscape = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(szEscape);
    curl_free(szEscape);

	m_strfields.append("&viplevel=");
	strTemp = m_stPlayerInfo.byVipLv;
	szEscape = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
	m_strfields.append(szEscape);
	curl_free(szEscape);

	m_strfields.append("&_ch=");
	strTemp = m_stPlayerInfo.byAuthType;
	szEscape = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
	m_strfields.append(szEscape);
	curl_free(szEscape);

    m_strfields.append("&name=");
    szEscape = curl_easy_escape(m_pEasyHandle, m_stPlayerInfo.strNickName.c_str(), 0);
    if(NULL == szEscape)
    {
        SYS_CRITICAL( _SDT( "[%s: %d]: curl_easy_escape playerid[%u] nickname [%s] failed"), MSG_MARK, 
            m_stPlayerInfo.dwPlayerID, m_stPlayerInfo.strNickName.c_str());
    }

    m_strfields.append(szEscape);
    curl_free(szEscape);
}

void CPushPlayerInfoHttpCMD::EscapeUrl(CHAR szUrl[], UINT16 wLen)
{
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

    char *szEscapeHex = curl_easy_escape(m_pEasyHandle, szHexDigest, 0);;
    SDSnprintf(szUrl, wLen, "%s?_sig=%s", m_strUrl.c_str(), szEscapeHex);

    curl_free(szEscapeHex);
}

BOOL CPushPlayerInfoHttpCMD::Init()
{
    
    CHAR szUrl[1024] = {0};
    EscapeFields();
    EscapeUrl(szUrl, sizeof(szUrl) - 1);

    curl_easy_setopt(m_pEasyHandle, CURLOPT_URL, szUrl);
    curl_easy_setopt(m_pEasyHandle, CURLOPT_ERRORBUFFER, m_szErrorBuff);
    curl_easy_setopt(m_pEasyHandle, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(m_pEasyHandle, CURLOPT_POST, 1 );
    curl_easy_setopt(m_pEasyHandle, CURLOPT_POSTFIELDS, m_strfields.c_str());
    return TRUE;
}

BOOL CPushPlayerInfoHttpCMD::OnExecuted()
{
    if (FALSE == m_bError)
    {
        return FALSE;
    }

    Json::Value root;
    Json::Reader reader;
    if( !reader.parse( m_strResult, root ) )
    {
        SYS_CRITICAL( _SDT( "[%s: %d]: CPushPlayerInfoHttpCMD::OnExecuted parse return json msg [%s] error1" ), MSG_MARK, m_strResult.c_str() );
        return FALSE;
    }

    //检测数据个数，如果为零则为出错，防止崩溃
    if (!root.isObject())
    {
        SYS_CRITICAL( _SDT( "[%s: %d]: CPushPlayerInfoHttpCMD::OnExecuted json size error, return json msg = %s" ), MSG_MARK, m_strResult.c_str() );
        return FALSE;
    }

    //解析返回的状态码
    INT32 nErrCode = 0;
    if (!root["errcode"].isInt())
    {
        SYS_CRITICAL( _SDT( "[%s: %d]: CPushPlayerInfoHttpCMD::OnExecuted json error could not find errcode in ret json [%s]" ), MSG_MARK, m_strResult.c_str() );
        return FALSE;
    }

    // 检测返回的错误码是否为0，0代表用户中心成功更新了玩家信息
    nErrCode = root["errcode"].asInt();
    if (0 != nErrCode)
    {
        SYS_CRITICAL( _SDT( "[%s: %d]: CPushPlayerInfoHttpCMD::OnExecuted push player info failed, return code = %u, return json msg = %s" ), MSG_MARK, nErrCode, m_strResult.c_str() );
        return FALSE;
    }

    return TRUE;
}

VOID CPushPlayerInfoHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_stPlayerInfo = *((SPlayerBaseInfo*)pUserData);
}