///<------------------------------------------------------------------------------
//< @file:   logic\http\getmaxplayerid_addgift.cpp
//< @author: Kang_zhiquan
//< @date:   2014年7月28日 16:36:8
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "getmaxplayerid_addgift.h"
#include "logic/http/urlpropmgr.h"
#include "framework/gsconfig.h"
#include "framework/gsapi.h"

CGetMaxPlayerID_AddGift::CGetMaxPlayerID_AddGift()
{
	m_strUrl = CUrlPropMgr::Instance()->GetUrlGetMaxPlyaerID();
}

CGetMaxPlayerID_AddGift::~CGetMaxPlayerID_AddGift()
{

}

BOOL CGetMaxPlayerID_AddGift::Init()
{
	CNGString strTemp;
	char *efc = NULL;

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

    //数据执行完后回调
BOOL CGetMaxPlayerID_AddGift::OnExecuted()
{
	if (FALSE == m_bError)
	{
		USR_INFO("[%s:%u] strResult = %s", MSG_MARK, m_strResult.c_str());
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
	UINT32 dwIOSMaxPlayerID = 0;
	if (!root["ios"].isInt())
	{
		SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
		return FALSE;
	}
	dwIOSMaxPlayerID = root["ios"].asInt();

	UINT32 dwANDMaxPlayerID = 0;
	if (!root["and"].isInt())
	{
		SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
		return FALSE;
	}
	dwANDMaxPlayerID = root["and"].asInt();
	
	if ( SINGLE_IOS == m_stGift.bySelectType)
	{
		m_stGift.dwStartPlayerID = 0;
		m_stGift.dwEndPlayerID = dwIOSMaxPlayerID;
	}
	else if(SINGLE_ANDROID == m_stGift.bySelectType)
	{
		m_stGift.dwStartPlayerID = BEGIN_AND_PLAYERID;
		m_stGift.dwEndPlayerID = dwANDMaxPlayerID;
	}
	else
	{
		USR_INFO("[%s:%d] SelectType failed", MSG_MARK);
		return FALSE;
	}
	CBaseDBCMD* poDB = gsapi::GetDBMgr()->CreateCmd(EDBCT_ADD_PLAYER_GIFT);
	if(NULL == poDB)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
		return FALSE;
	}
	poDB->SetUserData(&m_stGift);
	gsapi::GetDBMgr()->AddPayCommand(poDB);
	
	return TRUE;
}

    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
VOID CGetMaxPlayerID_AddGift::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
	m_stGift = *(SActivityGift*)pUserData;
}