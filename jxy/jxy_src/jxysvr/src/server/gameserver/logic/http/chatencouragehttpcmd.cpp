

#include "chatencouragehttpcmd.h"
#include <sdutil.h>
#include <json/json.h>
#include "common/server/utility.h"
#include "common/server/ngstring.h"
#include <db/autosqlbuf.h>
#include "common/client/errdef.h"
#include "urlpropmgr.h"
#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include "logic/other/errmsgmgr.h"
#include "logic/item/itemmgr.h"
#include "net/gt/gtpktbuilder.h"
#include "logic/player/player.h"


CChatEncourageHttpCMD::CChatEncourageHttpCMD()
{
    m_strUrl = CUrlPropMgr::Instance()->GetUrlChatEncourage();
    memset(&stAck, 0x00, sizeof(PKT_CLIGS_CHAT_ACK));
    memset(&stReq, 0x00, sizeof(PKT_CLIGS_CHAT_REQ));
}

CChatEncourageHttpCMD::~CChatEncourageHttpCMD()
{

}

BOOL CChatEncourageHttpCMD::Init()
{
    stReq.abyChatContent[stReq.dwChatLen] = '\0';
    CNGString strTemp;
    char *efc = NULL;
    m_strfields.append("playerid=");
    strTemp = m_dwTransID;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

    m_strfields.append("&cdkey=");
    strTemp = (char*)stReq.abyChatContent;
    efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
    m_strfields.append(efc);
    curl_free(efc);

	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(m_dwTransID);
	if ( NULL != poPlayer)
	{
		m_strfields.append("&_ch=");
		strTemp = poPlayer->GetAuthType();
		efc = curl_easy_escape(m_pEasyHandle, strTemp.c_str(), 0);
		m_strfields.append(efc);
		curl_free(efc);
	}
	

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

BOOL CChatEncourageHttpCMD::OnExecuted()
{
    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(m_dwTransID);
    if (NULL == poPlayer)
    {
        return FALSE;
    }
    if (FALSE == m_bError)
    {
        //把这种数据再次投递到队列中去
        SYS_CRITICAL( _SDT( "request http:[%s] timeout" ), m_strUrl.c_str() );
        return FALSE;
    }
    ProcessResult(poPlayer);
    return TRUE;
}

BOOL CChatEncourageHttpCMD::ProcessResult(CPlayer *poPlayer)
{
    DBG_INFO( _SDT( "json, result:%s" ), m_strResult.c_str() );

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
    INT16 wErrCode = 0;
    if (!root["errcode"].isInt())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    wErrCode = root["errcode"].asInt();

    //检测数据个数，如果为零刚为出错，防止崩溃
    if (!root["datas"].isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }
    Json::Value datas = root["datas"];

    if (!datas.isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), m_strResult.c_str() );
        return FALSE;
    }

    string strTitle = "";
    if (datas["title"].isString())
    {
        strTitle = datas["title"].asString();
    }

    string strdesc = "";
    if (datas["desc"].isString())
    {
        strdesc = datas["desc"].asString();
    }

    string strItems = "";
    if (datas["items"].isString())
    {
        strItems = datas["items"].asString();
    }

    vector<string> vecItems;
    vecItems = SDSplitStringsA(strItems, ',');

    UINT32 dwCoin = 0;
    if (datas["coin"].isInt())
    {
        dwCoin = datas["coin"].asInt();
    }

    UINT32 dwGold = 0;
    if (datas["gold"].isInt())
    {
        dwGold = datas["gold"].asInt();
    }
    UINT32 dwPhystrength = 0;
    if (datas["phystrength"].isInt())
    {
        dwPhystrength = datas["phystrength"].asInt();
    }
    UINT32 dwScience = 0;
    if (datas["science"].isInt())
    {
        dwScience = datas["science"].asInt();
    }
    UINT32 dwStory = 0;
    if (datas["story"].isInt())
    {
        dwStory = datas["story"].asInt();
    }
    UINT32 dwBluegas = 0;
    if (datas["bluegas"].isInt())
    {
        dwBluegas = datas["bluegas"].asInt();
    }
    UINT32 dwPrupleGas = 0;
    if (datas["purplegas"].isInt())
    {
        dwPrupleGas = datas["purplegas"].asInt();
    }
    UINT32 dwJingJie = 0;
    if (datas["jingjie"].isInt())
    {
        dwJingJie = datas["jingjie"].asInt();
    }
    UINT32 dwPlayerID = 0;
    if (datas["playerid"].isInt())
    {
        dwPlayerID = datas["playerid"].asInt();
    }

    vector<string> vecStr;
    vecStr.push_back(strTitle);
    stAck.stChatData.dwSerialNumber = 0;
    stAck.stChatData.dwPlayerID = 0;
    stAck.stChatData.byChatType = ECT_SYS;
    stAck.stChatData.qwChatTime = SDTimeSecs();
    //0 成功， 1 不存在， 2 CDkey已被他人使用，3：已领取过，请务再次领取， 4：内部服务器出错
    if (1 == wErrCode)
    {
        stAck.stChatData.dwChatLen  = strdesc.size() > MAX_CHAT_CONTENT_NUM ? MAX_CHAT_CONTENT_NUM - 1 : strdesc.size();
        SDStrncpy((char*)stAck.stChatData.abyChatContent, strdesc.c_str(), stAck.stChatData.dwChatLen);
        ResponseMsg(CLIGS_CHAT_ACK, (CHAR*)&stAck);
        return TRUE;
    }
    else if (2 == wErrCode && m_dwTransID != dwPlayerID)
    {
        stAck.stChatData.dwChatLen  = strdesc.size() > MAX_CHAT_CONTENT_NUM ? MAX_CHAT_CONTENT_NUM - 1 : strdesc.size();
        SDStrncpy((char*)stAck.stChatData.abyChatContent, strdesc.c_str(), stAck.stChatData.dwChatLen);
        ResponseMsg(CLIGS_CHAT_ACK, (CHAR*)&stAck);
        return TRUE;
    }
    else if (3 <= wErrCode)
    {
        stAck.stChatData.dwChatLen  = strdesc.size() > MAX_CHAT_CONTENT_NUM ? MAX_CHAT_CONTENT_NUM - 1 : strdesc.size();
        SDStrncpy((char*)stAck.stChatData.abyChatContent, strdesc.c_str(), stAck.stChatData.dwChatLen);
        ResponseMsg(CLIGS_CHAT_ACK, (CHAR*)&stAck);
        return TRUE;
    }
	

    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

	wErrCode = 99;
    //////////////////////////////////////////////////////////////////////////
    char szSql[1024] = {0};
    sprintf(szSql, "call sp_AddCdKey('%s', %u)", (CHAR*)stReq.abyChatContent, m_dwTransID);
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );
    if(SDDB_HAS_RECORDSET == nRet && pRecordSet->GetRecord())
    {
        wErrCode = SGDP::SDAtou(pRecordSet->GetFieldValueByName("ErrCode"));
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

	//只有返回0，才可以领取
    if (0 == wErrCode)
    {
        for(UINT32 dwIdx = 0; dwIdx < vecItems.size(); dwIdx++)
        {
            ECreateItemRet ItemRet = ECIR_SUCCESS;
            CItem* poItem = CItemMgr::Instance()->CreateItemIntoBag(poPlayer, SGDP::SDAtou(vecItems[dwIdx].c_str()), 1, ItemRet, 0, CRecordMgr::EAIT_ENCOURAGE);
            if(NULL == poItem)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: CreateItem failed[%d], ItemKindID=%d !"), MSG_MARK, ItemRet, SGDP::SDAtou(vecItems[dwIdx].c_str()));
            }
        }

        PKT_CLIGS_PLAYER_RES_NTF stNtf;
        memset(&stNtf, 0x00, sizeof(PKT_CLIGS_PLAYER_RES_NTF));
        poPlayer->AddCoin(dwCoin, CRecordMgr::EACT_ENCOURAGE, 0, 0, 0, 0, (CHAR*)stReq.abyChatContent);
        stNtf.astResInfo[stNtf.byResNum].byResKind = COIN;
        stNtf.astResInfo[stNtf.byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
        stNtf.byResNum++;

        poPlayer->AddGold(dwGold, CRecordMgr::EGGT_ENCOURAGE, 0, 0, 0, 0, (CHAR*)stReq.abyChatContent);
        stNtf.astResInfo[stNtf.byResNum].byResKind = GOLD;
        stNtf.astResInfo[stNtf.byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
        stNtf.byResNum++;

        poPlayer->AddPhyStrength(dwPhystrength, CRecordMgr::EAPST_ENCOURAGE, 0, 0, 0, 0, (CHAR*)stReq.abyChatContent);
        stNtf.astResInfo[stNtf.byResNum].byResKind = PHYSTRENGTH;
        stNtf.astResInfo[stNtf.byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().wPhyStrength;
        stNtf.byResNum++;

        poPlayer->AddScience(dwScience, CRecordMgr::EAST_ENCOURAGE, 0, 0, 0, 0, (CHAR*)stReq.abyChatContent);
        stNtf.astResInfo[stNtf.byResNum].byResKind = SCIENCE;
        stNtf.astResInfo[stNtf.byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
        stNtf.byResNum++;

        poPlayer->AddStory(dwStory, CRecordMgr::EASTT_ENCOURAGE, 0, 0, 0, 0, (CHAR*)stReq.abyChatContent);
        stNtf.astResInfo[stNtf.byResNum].byResKind = STORY;
        stNtf.astResInfo[stNtf.byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;
        stNtf.byResNum++;

        poPlayer->AddBlueGas(dwBluegas, CRecordMgr::EABGT_ENCOURAGE, 0, 0, 0, 0, (CHAR*)stReq.abyChatContent);
        stNtf.astResInfo[stNtf.byResNum].byResKind = BLUEGAS;
        stNtf.astResInfo[stNtf.byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas;
        stNtf.byResNum++;

        poPlayer->AddPurpleGas(dwPrupleGas, CRecordMgr::EAPGT_ENCOURAGE, 0, 0, 0, 0, (CHAR*)stReq.abyChatContent);
        stNtf.astResInfo[stNtf.byResNum].byResKind = PURPLEGAS;
        stNtf.astResInfo[stNtf.byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwPurpleGas;
        stNtf.byResNum++;

        poPlayer->AddJingJie(dwJingJie, CRecordMgr::EAJT_ENCOURAGE, 0, 0, 0, 0, (CHAR*)stReq.abyChatContent);
        stNtf.astResInfo[stNtf.byResNum].byResKind = JINGJIE;
        stNtf.astResInfo[stNtf.byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
        stNtf.byResNum++;

        //同步资源
        poPlayer->SendMsg((CHAR*)&stNtf, CLIGS_PLAYER_RES_NTF);
    }

	stAck.stChatData.dwChatLen  = strdesc.size() > MAX_CHAT_CONTENT_NUM ? MAX_CHAT_CONTENT_NUM - 1 : strdesc.size();
	SDStrncpy((char*)stAck.stChatData.abyChatContent, strdesc.c_str(), stAck.stChatData.dwChatLen);
	ResponseMsg(CLIGS_CHAT_ACK, (CHAR*)&stAck);
	return TRUE;
}



VOID CChatEncourageHttpCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwTransID = dwTransID;
    memcpy(&stReq, pUserData, sizeof(stReq));
}