#include "registerhttpcmd.h"
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

CRegisterHttpCMD::CRegisterHttpCMD()
{
	m_strUrl = ((CGTApplication*)SDGetApp())->GetGTConfig()->GetRegisterurl();
}

CRegisterHttpCMD::~CRegisterHttpCMD()
{

}
BOOL CRegisterHttpCMD::Init()
{
	CNGString strTemp;
	char *efc = NULL;
	m_strfields.append("deviceid=");
	strTemp = m_stRegisterInfo.strDeviceid;
	efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
	m_strfields.append(efc);
	curl_free(efc);

	m_strfields.append("&username=");
	strTemp = m_stRegisterInfo.strUserName;
	efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
	m_strfields.append(efc);
	curl_free(efc);

	m_strfields.append("&userpwd=");
	strTemp = m_stRegisterInfo.strUserPwd;
	efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
	m_strfields.append(efc);
	curl_free(efc);

	m_strfields.append("&userid=");
	strTemp = m_stRegisterInfo.dwUserID;
	efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
	m_strfields.append(efc);
	curl_free(efc);

	m_strfields.append("&_tag=");
	strTemp = 1;
	efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
	m_strfields.append(efc);
	curl_free(efc);

	m_strfields.append("&_ch=");
	strTemp = m_stRegisterInfo.byAuthType == 0 ? AUTH_TYPE_MAXNET : m_stRegisterInfo.byAuthType;
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

BOOL CRegisterHttpCMD::OnExecuted()
{
	if (FALSE == m_bError)
	{
		USR_INFO( _SDT( "Player:%u url error, msg:%s" ), m_stRegisterInfo.dwPlayerID, m_szErrorBuff );
		return SendMessage(ERR_COMMON::ID_OTHER_ERR, NULL);
	}
	USR_INFO( _SDT( "ack:%s" ), SDUTF82Local(m_strResult.c_str()).c_str());
	Json::Value root;
	Json::Reader reader;

	if( !reader.parse( m_strResult, root ) )
	{
		SYS_CRITICAL( _SDT( "json parse error, result:%s" ), SDUTF82Local(m_strResult.c_str()).c_str() );
		return SendMessage(ERR_COMMON::ID_OTHER_ERR, NULL);
	}

	//检测数据个数，如果为零刚为出错，防止崩溃
	if (!root.isObject())
	{
		SYS_CRITICAL( _SDT( "json size error, result:%s" ), SDUTF82Local(m_strResult.c_str()).c_str() );
		return SendMessage(ERR_COMMON::ID_OTHER_ERR, NULL);
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
		return SendMessage(ERR_COMMON::ID_OTHER_ERR, NULL);
	}

	if ( ERR_REGISTER_USERNAME::ID_SUCCESS != wErrCode)
	{
		return SendMessage(wErrCode, NULL);
	}



	string strUserName;
	Json::Value jsDatas;
	if (root["datas"].isObject())
	{
		jsDatas = root["datas"];
	}
	else
	{
		SYS_CRITICAL( _SDT( "json parse error, result:%s" ), SDUTF82Local(m_strResult.c_str()).c_str() );
		return SendMessage(ERR_COMMON::ID_OTHER_ERR, NULL);
	}

	if (jsDatas["username"].isString())
	{
		strUserName = jsDatas["username"].asCString();
	}
	else
	{
		SYS_CRITICAL( _SDT( "json parse error, result:%s" ), SDUTF82Local(m_strResult.c_str()).c_str() );
		return SendMessage(ERR_COMMON::ID_OTHER_ERR, NULL);
	}

	return SendMessage(wErrCode, strUserName.c_str());
}

VOID CRegisterHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
	m_dwTransID = dwTransID;
	m_stRegisterInfo = *((SResgisterInfo*)pUserData);
}

BOOL CRegisterHttpCMD::SendMessage(UINT16 wErrCode, const TCHAR* aszUserName)
{
	CGTClient* poCliSession = ((CGTApplication*)SDGetApp())->GetClientMgr()->FindCliSession(m_dwTransID);
	if (NULL == poCliSession)
	{
		DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
		return FALSE;
	}

	UINT16 wMsgID = CCliPktBuilder::Instance()->RegisterUserNameAck( wErrCode, aszUserName);
	
	poCliSession->SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());
	//只有在认证失败时才断开
	//poCliSession->Disconnect();//断开连接

	return TRUE;
}