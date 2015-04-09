

#include "basehttpcmd.h"
#include <sdutil.h>
#include <sdtime.h>

#include "common/server/utility.h"
#include "httpmgr.h"
#include "framework/gtclient.h"
#include "framework/gtapplication.h"

CBaseHttpCMD::CBaseHttpCMD()
{
    m_bError = FALSE;
    m_dwTransID = 0;
    m_strResult = "";
    m_strUrl = "";
    m_strfields = "";
    m_qwLastAddTime = SDTimeSecs();

    memset(m_szErrorBuff, 0x00, sizeof(m_szErrorBuff));
    m_pEasyHandle = curl_easy_init();
}

CBaseHttpCMD::~CBaseHttpCMD()
{
    if (m_pEasyHandle)
    {
        curl_easy_cleanup(m_pEasyHandle);
    }
}

VOID  CBaseHttpCMD::Release()
{
    CHttpMgr::Instance()->FreeCmd(this);
}

string CBaseHttpCMD::UrlSig()
{
    //地址
    string	strUrl = m_strUrl;
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
    strUrl.append("?_sig=");
    strInput = szHexDigest;
    char *efc = NULL;
    efc = curl_easy_escape(m_pEasyHandle, strInput.c_str(), 0);
    strUrl.append(efc);
    curl_free(efc);

    return strUrl;
}

VOID CBaseHttpCMD::ResponseMsg(UINT16 wMsgID, const CHAR* pBuf, BOOL bDisconnect)
{
    CGTClient* poGTClient = ((CGTApplication*)SDGetApp())->GetClientMgr()->FindCliSession(m_dwTransID);
    if(poGTClient)
    {
        poGTClient->SendMsg(wMsgID, pBuf);
        if (!bDisconnect)
        {
            return;
        }
        //引起线程切换，保证数据能够正常发送出去
        SDSleep(0);
        poGTClient->Disconnect();//返回区数据后断开链接
    }
}



