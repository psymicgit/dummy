

#include "reportuserdeviceinfohttpcmd.h"
#include <sdutil.h>
#include <json/json.h>
#include "common/server/utility.h"
#include "common/server/ngstring.h"
#include "net/cli/clipktbuilder.h"
#include "common/client/errdef.h"




CReportUserDeviceInfoHttpCMD::CReportUserDeviceInfoHttpCMD()
{
    memset(&stReq, 0x00, sizeof(stReq));
    memset(&stAck, 0x00, sizeof(stAck));
    m_strUrl = ((CZSApplication*)SDGetApp())->GetZSConfig()->Getreportuserdeviceinfo();
}

CReportUserDeviceInfoHttpCMD::~CReportUserDeviceInfoHttpCMD()
{

}

BOOL  CReportUserDeviceInfoHttpCMD::Init()
{
    CNGString strTemp;
    char *efc = NULL;

    m_strfields.append("_ch=");
    strTemp = AUTH_TYPE_MAXNET;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&deviceid=");
    strTemp = stReq.aszDeviceID;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&platform=");
    strTemp = stReq.aszPlatform;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&_gv=");
    strTemp = stReq.aszGameVersion;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&ostype=");
    strTemp = stReq.aszOSType;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&osversion=");
    strTemp = stReq.aszOSVersion;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&hardwareversion=");
    strTemp = stReq.aszHardwareVersion;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);


    //个数（1个字节）+ [参数1（1个字节） + 参数1内容（n个字节）]
    CHAR		szMac[256] = {0};
    CHAR		szAdid[256] = {0};
    UINT32	dwIndex = 0;
    //获取扩展参数的个数
    UINT8	byExtNum = stReq.aszExtInfo[dwIndex++];
    for(UINT8 i = 0; i < byExtNum; i++)
    {
        //扩展参数的长度占一个字节
        UINT8 byExtLen = stReq.aszExtInfo[dwIndex++];
        switch (i)
        {
        case 0:
            SDStrncpy(szMac, stReq.aszExtInfo + dwIndex, byExtLen);
            break;
        case 1:
            SDStrncpy(szAdid, stReq.aszExtInfo + dwIndex, byExtLen);
            break;
        default:
            break;
        }
        dwIndex += byExtLen;
    }

    m_strfields.append("&mac=");
    strTemp = szMac;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&adid=");
    strTemp = szAdid;
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

BOOL CReportUserDeviceInfoHttpCMD::OnExecuted()
{
    if (FALSE == m_bError)
    {
        stAck.wErrCode = ERR_REPORT_USER_DEVICE_INFO::ID_SUCCESS;
        ResponseMsg(CLIZS_REPORT_USER_DEVICE_INFO_ACK, (CHAR*)(&stAck));
    }
    else
    {
        ProcessResult();
    }
    return TRUE;
}

VOID CReportUserDeviceInfoHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwTransID = dwTransID;
    memcpy(&stReq, pUserData, sizeof(stReq));
}




VOID CReportUserDeviceInfoHttpCMD::ProcessResult()
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
    ResponseMsg(CLIZS_REPORT_USER_DEVICE_INFO_ACK, (CHAR*)(&stAck));
}


