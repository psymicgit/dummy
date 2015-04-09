#include "getuserexpandhttpcmd.h"
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

CGetUserExpandHttpCMD::CGetUserExpandHttpCMD()
{
    m_strUrl = ((CGTApplication*)SDGetApp())->GetGTConfig()->GetUserExpandUrl();
}

CGetUserExpandHttpCMD::~CGetUserExpandHttpCMD()
{

}

BOOL CGetUserExpandHttpCMD::Init()
{
    CNGString strTemp;
    char *efc = NULL;
    m_strfields.append("userid=");
    strTemp = m_stGetUserExpandInfo.dwUserID;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&_tag=");
    strTemp = 1;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&_ch=");
    strTemp = AUTH_TYPE_MAXNET;
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

VOID CGetUserExpandHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwTransID = dwTransID;
    m_stGetUserExpandInfo = *((SGetUserExpandInfo*)pUserData);
}

BOOL CGetUserExpandHttpCMD::OnExecuted()
{
    if ( FALSE == m_bError)
    {
        USR_INFO( _SDT( "Player:%u url error, msg:%s" ), m_stGetUserExpandInfo.dwPlayerID, m_szErrorBuff );
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

    Json::Value datas;
    if (root["datas"].isObject())
    {
        datas = root["datas"];
    }
    else
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), SDUTF82Local(m_strResult.c_str()).c_str() );
        return SendMessage(ERR_COMMON::ID_OTHER_ERR, NULL);
    }

    string strPhone;
    if (datas["phone"].isString())
    {
        strPhone = datas["phone"].asCString();
    }
    else
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), SDUTF82Local(m_strResult.c_str()).c_str() );
        return SendMessage(ERR_COMMON::ID_OTHER_ERR, NULL);
    }
    DT_EXPAND_FUNC_PARAM_LST stExpandLst = {0};
    stExpandLst.byExpandNum = 1;
    stExpandLst.astExpandFuncParaInfo[0].byExpandType = 1;
    stExpandLst.astExpandFuncParaInfo[0].byExpandLen = strPhone.size();
    memcpy(stExpandLst.astExpandFuncParaInfo[0].abyExpandData, strPhone.c_str(), strPhone.size());

    return SendMessage(wErrCode, &stExpandLst);

}

BOOL CGetUserExpandHttpCMD::SendMessage(UINT16 wErrcode, DT_EXPAND_FUNC_PARAM_LST* stExpandLst)
{
    CGTClient* poCliSession = ((CGTApplication*)SDGetApp())->GetClientMgr()->FindCliSession(m_dwTransID);
    if (NULL == poCliSession)
    {
        DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
        return FALSE;
    }


    UINT16 wMsgID = CCliPktBuilder::Instance()->GetUserExpandAck(wErrcode, stExpandLst);

    poCliSession->SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());

    return TRUE;
}