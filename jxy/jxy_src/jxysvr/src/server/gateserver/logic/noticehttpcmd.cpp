

#include "noticehttpcmd.h"
#include <sdutil.h>
#include <json/json.h>
#include "common/server/utility.h"
#include "common/server/ngstring.h"
#include "framework/gtclient.h"
#include "framework/gtapplication.h"
#include "net/cli/clipktbuilder.h"
#include "common/client/errdef.h"
#include "noticemgr.h"




CNoticeHttpCMD::CNoticeHttpCMD()
{
    m_strUrl = ((CGTApplication*)SDGetApp())->GetGTConfig()->GetNoticeUrl();
}

CNoticeHttpCMD::~CNoticeHttpCMD()
{

}

BOOL  CNoticeHttpCMD::Init()
{
    CNGString strTemp;
    char *efc = NULL;

    m_strfields.append("zoneid=");
    strTemp = stNoticeInfo.wZoneID;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&_tag=");
    strTemp = 1;
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

BOOL CNoticeHttpCMD::OnExecuted()
{
    if (TRUE == m_bError)
    {
        ProcessResult();
    }
    else
    {
        SYS_CRITICAL( _SDT( "get notice info" ));
    }
    return TRUE;
}

VOID CNoticeHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwTransID = dwTransID;
    stNoticeInfo = *((SNoticeInfo*)pUserData);
}


VOID CNoticeHttpCMD::ProcessResult()
{
    USR_INFO( _SDT( "ack:%s" ), SDUTF82Local(m_strResult.c_str()).c_str());

    Json::Value root;
    Json::Reader reader;
    if( !reader.parse( m_strResult, root ) )
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), SDUTF82Local(m_strResult.c_str()).c_str() );
        return;
    }

    //检测数据个数，如果为零刚为出错，防止崩溃
    if (!root.isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), SDUTF82Local(m_strResult.c_str()).c_str() );
        return;
    }

    //解析返回的状态码
    INT16 wErrCode = 0;
    if (!root["errcode"].isInt())
    {
        return;
    }
    wErrCode = root["errcode"].asInt();
    if (0 != wErrCode)
    {
        return;
    }

    {
		if (!root["datas"].isArray())
		{
			return;
		}

        MAP_DT_NOTICE_DATA mapDT_NOTICE_DATA;
        Json::Value datas = root["datas"];
        for (UINT32 idx = 0; idx < datas.size(); idx++)
        {
            Json::Value data = datas[idx];
            if (!data.isObject())
            {
                continue;
            }
            UINT8 byAuthType = 0;
            if (!data["authtype"].isInt())
            {
                continue;
            }
            byAuthType = data["authtype"].asInt();

            //通知
            DT_NOTICE_DATA	stNoticeInfo;
            memset(&stNoticeInfo, 0x00, sizeof(DT_NOTICE_DATA));
            if (!data["noticeid"].isInt())
            {
                continue;
            }
            stNoticeInfo.dwNoticeID  = data["noticeid"].asInt();
            if (!data["forcepopflag"].isInt())
            {
                continue;
            }
            stNoticeInfo.byForcePopFlag  = data["forcepopflag"].asInt();
            if (!data["urlflag"].isInt())
            {
                continue;
            }
            stNoticeInfo.byUrlFlag  = data["urlflag"].asInt();
            if (!data["noticecontent"].isString())
            {
                continue;
            }
            SDStrncpy(stNoticeInfo.aszNoticeContent, data["noticecontent"].asCString(), MAX_CONTENT_SIZE);
            mapDT_NOTICE_DATA[byAuthType] = stNoticeInfo;
        }
        if (mapDT_NOTICE_DATA.size())
        {
            CNoticeMgr::Instance()->GetMAP_DT_NOTICE_DATA() = mapDT_NOTICE_DATA;
        }
    }

    {
		if (!root["dataexs"].isArray())
		{
			return;
		}

        MAP_DT_TIME_OUT mapMAP_DT_TIME_OUT;
        Json::Value dataexs = root["dataexs"];
        for (UINT32 idx = 0; idx < dataexs.size(); idx++)
        {
            Json::Value data = dataexs[idx];
            if (!data.isObject())
            {
                continue;
            }
            UINT8 byAuthType = 0;
            if (!data["authtype"].isInt())
            {
                continue;
            }
            byAuthType = data["authtype"].asInt();

            //通知
            DT_TIME_OUT	stDT_TIME_OUT;
            if (!data["noticecontent"].isString())
            {
                continue;
            }
            SDStrncpy(stDT_TIME_OUT.aszNoticeContent, data["noticecontent"].asCString(), MAX_CONTENT_SIZE);
            mapMAP_DT_TIME_OUT[byAuthType] = stDT_TIME_OUT;
        }
        if (mapMAP_DT_TIME_OUT.size())
        {
            CNoticeMgr::Instance()->GetMAP_DT_TIME_OUT() = mapMAP_DT_TIME_OUT;
        }
    }
}







