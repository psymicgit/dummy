

#include "namehttpcmd.h"
#include <sdutil.h>
#include <json/json.h>
#include "common/server/utility.h"
#include "common/server/ngstring.h"
#include <db/autosqlbuf.h>
#include "common/client/errdef.h"
#include "urlpropmgr.h"
#include "logic/record/recordmgr.h"
#include "logic/player/playermgr.h"



CNameHttpCMD::CNameHttpCMD()
{
    m_strUrl = CUrlPropMgr::Instance()->GetUrlName();
}

CNameHttpCMD::~CNameHttpCMD()
{

}

BOOL CNameHttpCMD::Init()
{
    CNGString strTemp;
    char *efc = NULL;
    m_strfields.append("playerid=");
    strTemp = stNameInfo.dwPlayerID;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&name=");
    strTemp = stNameInfo.strName;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

	m_strfields.append("&_ch=");
	strTemp = stNameInfo.byAuthType;
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

BOOL CNameHttpCMD::OnExecuted()
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

    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(stNameInfo.dwPlayerID);
    if(NULL == poPlayer)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: player[playerid = %u] try to reg dispname[%s] fail!"), MSG_MARK, stNameInfo.dwPlayerID, stNameInfo.strName.c_str());
        return FALSE;
    }

    // 检测玩家是否是因为合区导致的昵称冲突而重新注册的，是的话要记录日志
    BOOL bNeedReRegDspName = (get_bit(poPlayer->GetExtData(), em_EDIT_NAME));
    if(bNeedReRegDspName)
    {
        //修改标志位
        clr_bit(poPlayer->GetExtData(), em_EDIT_NAME);

        CHAR szComment[256] = {0};
        SDSnprintf(szComment, sizeof(szComment) - 1, "[%s] - [%s]", stNameInfo.strOldName.c_str(), stNameInfo.strName.c_str());

        CRecordMgr::Instance()->RecordInfo(stNameInfo.dwPlayerID, ERM_PLAYER_OPERATE, CRecordMgr::EPO_RE_REG_NICK_NAME, 1, 1, 
            poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 0, 0, 0, 0, szComment);
    }    

	return TRUE;
}

VOID CNameHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwTransID = dwTransID;
    stNameInfo = *((SNameInfo*)pUserData);
}