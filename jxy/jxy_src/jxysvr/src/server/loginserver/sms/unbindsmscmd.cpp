
#include "framework/lsapplication.h"
#include "unbindsmscmd.h"
#include <sdutil.h>
#include "sms/smsusermgr.h"
#include "smsmgr.h"
#include "common/server/utility.h"




CUnBindSmsCMD::CUnBindSmsCMD()
{

}

CUnBindSmsCMD::~CUnBindSmsCMD()
{

}

VOID CUnBindSmsCMD::OnExecuted(void)
{
	m_stAck.wCountdown = MAX_MOBILE_TIME_OUT;
	ResponseMsg(CLILS_UNBIND_MOBILE_ACK, (CHAR*)(&m_stAck));
}

//数据执行
VOID CUnBindSmsCMD::OnExecute(void)
{
    PKT_CLILS_UNBIND_MOBILE_REQ *pReq = (PKT_CLILS_UNBIND_MOBILE_REQ*)m_pUserData;

    INT32 dwRand = Random(100000, 1000000);
    char abyVerCode[MAX_MOBILE_LENGTH] = {0}; //验证码
    sprintf(abyVerCode, "%4u", dwRand);

    CURLcode	code;
    CURL*	pEasyHandle;
    pEasyHandle = curl_easy_init();
    if( NULL == pEasyHandle )
    {
        SYS_CRITICAL( _SDT( "[%s: %d]:curl_easy_init failed!" ), MSG_MARK );
        return;
    }

    char *efc = NULL;
    std::string strfields("sdk=");
    efc = curl_easy_escape(pEasyHandle, SDK, 0);
    strfields.append(efc);
    curl_free(efc);

    strfields.append("&code=");
    efc = curl_easy_escape(pEasyHandle, CODE, 0);
    strfields.append(efc);
    curl_free(efc);

    strfields.append("&phones=");
    efc = curl_easy_escape(pEasyHandle, (CHAR*)m_szParm, 0);
    strfields.append(efc);
    curl_free(efc);

	CHAR		szMsg[256] = {0};
	sprintf(szMsg, "解除绑定验证码: %s - 《囧西游》", abyVerCode);

    strfields.append("&msg=");
    efc = curl_easy_escape(pEasyHandle, SDGBK2UTF8(szMsg).c_str(), 0);
    strfields.append(efc);
    curl_free(efc);


    strfields.append("&resulttype=");
    efc = curl_easy_escape(pEasyHandle, RESULTTYPE, 0);
    strfields.append(efc);
    curl_free(efc);

    strfields.append("&subcode=");
    efc = curl_easy_escape(pEasyHandle, SUBCODE, 0);
    strfields.append(efc);
    curl_free(efc);

    curl_easy_setopt(pEasyHandle, CURLOPT_POST, 1 );
    curl_easy_setopt(pEasyHandle, CURLOPT_POSTFIELDS, strfields.c_str());
    curl_easy_setopt(pEasyHandle , CURLOPT_WRITEFUNCTION, OnRecv);
    curl_easy_setopt(pEasyHandle , CURLOPT_WRITEDATA,  &m_strNetData);
    curl_easy_setopt(pEasyHandle , CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(pEasyHandle, CURLOPT_TIMEOUT, 2);
    // 执行数据请求
    curl_easy_setopt(pEasyHandle , CURLOPT_URL, ACTION_MT);
    code = curl_easy_perform(pEasyHandle);
    curl_easy_cleanup(pEasyHandle);
    if (SDGBK2UTF8(RESULTSTR) == m_strNetData)
    {
        m_stAck.wErrCode = ERR_BIND_MOBILE::ID_SUCCESS;
        CSmsUserMgr::Instance()->CreateUser(pReq->dwUserID, (CHAR*)m_szParm, em_UNBIND, abyVerCode);
    }
    else
    {
        m_stAck.wErrCode = ERR_BIND_MOBILE::ID_SEND_FAILD;
    }
}



