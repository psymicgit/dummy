#include "rcclient.h"
#include <framework/gsapi.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include <db/settongbupayprocessed.h>
#include <algorithm/sdmd5.h>
#include <framework/gsconfig.h>
#include <net/db/dbpktbuilder.h>
#include <net/ks/ksprocessor.h>
#include <logic/player/player.h>
#include <protocol/server/protocommondata.h>
#include <logic/other/jsonopmgr.h>
#include <logic/chat/chatmgr.h>
#include <logic/chat/wordfilter.h>
#include <logic/activityencourage/activityencouragemgr.h>
#include <logic/item/itemmgr.h>
#include <logic/hero/heropropmgr.h>
#include <db/setmypayprocessed.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <common/client/gmdef.h>
#include <logic/record/recordmgr.h>
#include <json/json.h>
#include <logic/activityencourage/oprateingactivitymgr.h>
#include <logic/awaken/awakenpropmgr2.h>
#include <db/getplayerconsume.h>
#include "common/server/ngstring.h"
#include "logic/vip/vippropmgr.h"
#include "logic/awaken/awaken.h"
#include "logic/bag/bag.h"
#include <db/setcenterpayprocessed.h>

#include <errdef.h>
#include <string>

#include "deal/dealframe01.h"
#include "deal/dealframe02.h"
#include "deal/dealframe03.h"
#include "deal/dealframe04.h"
#include "deal/dealframe05.h"
#include "deal/dealframe06.h"
#include "deal/dealframe07.h"
#include "deal/dealframe08.h"
#include "deal/dealframe09.h"
#include "deal/dealframe10.h"
#include "deal/dealframe11.h"
#include "deal/dealframe12.h"
#include "deal/dealframe13.h"
#include "deal/dealframe14.h"
#include "deal/dealframe15.h"
#include "deal/dealframe16.h"
#include "deal/dealframe17.h"
#include "deal/dealframe18.h"
#include "deal/dealframe19.h"
#include "deal/dealframe20.h"
#include "deal/dealframe21.h"
#include "deal/dealframe22.h"
#include "deal/dealframe23.h"
#include "deal/dealframe24.h"
#include "deal/dealframe25.h"
#include "deal/dealframe26.h"
#include "deal/dealframe27.h"
#include "deal/dealframe28.h"
#include "deal/dealframe29.h"
#include "deal/dealframe30.h"
#include "deal/dealframe31.h"
#include "deal/dealframe32.h"
#include "deal/dealframe33.h"
#include "deal/dealframe34.h"
#include "deal/dealframe35.h"
#include "deal/dealframe36.h"
#include "deal/dealframe37.h"
#include "deal/dealframe38.h"
#include "deal/dealframe39.h"
#include "deal/dealframe40.h"
#include "deal/dealframe41.h"
#include "deal/dealframe42.h"
#include "deal/dealframe43.h"
#include "deal/dealframe44.h"
#include "deal/dealframe45.h"
#include "deal/dealframe46.h"
#include "deal/dealframe47.h"
#include "deal/dealframe48.h"
#include "deal/dealframe49.h"
#include "deal/dealframe50.h"
#include "deal/dealframe51.h"
#include "deal/dealframe52.h"
#include "deal/dealframe53.h"
#include "deal/dealframe54.h"
#include "deal/dealframe55.h"
#include "deal/dealframe56.h"
#include "deal/dealframe57.h"
#include "deal/dealframe58.h"
#include "deal/dealframe59.h"
#include "deal/dealframe60.h"
#include "deal/dealframe61.h"
#include "deal/dealframe62.h"
#include "deal/dealframe63.h"
#include "deal/dealframe64.h"
#include "deal/dealframe65.h"
#include "deal/dealframe66.h"
#include "deal/dealframe67.h"
#include "deal/dealframe68.h"
#include "deal/dealframe69.h"
#include "deal/dealframe75showgvgpair.h"
#include "deal/dealframe76setgvgpair.h"
#include "common/server/utility.h"
#include "deal/dealframepower.h"
#include "deal/dealframeaddjewelry.h"
#include "deal/dealframechangename72.h"
#include "deal/dealframeaddgift73.h"
#include "deal/dealframeoperatingactivity74.h"
//同步账号充值信息
const CHAR*		PSOURCE		= "source";
const CHAR*		PTRADE_NO	= "trade_no";
const CHAR*		PAMOUNT		= "amount";
const CHAR*		PPARTNER		= "partner";
const CHAR*		PSIGN		= "sign";
const CHAR*		PCMD			= "cmd";
const CHAR*		PTBKEY		= "&key=FxAKhtwg0EatlbgZbujg4GCnBTRU#VNo";

//GM内部充值信息
const CHAR*		PSERIALNUMBER		= "serialnumber";
const CHAR*		PACTION			= "action";
const CHAR*		PPLAYERID		= "playerid";
const CHAR*		PPARAM1			= "param1";
const CHAR*		PPARAM2			= "param2";
const CHAR*		PPARAM3			= "param3";
const CHAR*		PPARAM4			= "param4";
const CHAR*		PPARAM5			= "param5";
const CHAR*		DESC				= "desc";
const CHAR*		PGMKEY			= "&key=0z#z#b#094kls#040jkas892#z#z#b#0";
const CHAR*		PTIME			= "time"; //时间为当前秒数

//内部充值信息
const CHAR*		PORDERID		= "orderid";
const CHAR*		PRMB			= "rmb";
const CHAR*		PGIVENGOLD	= "givengold";
const CHAR*		PMYKEY		= "&key=MYAKhtwg0EatlbgZbujg4GCnBTRU#VMY";
const CHAR*		PCOMMENT		= "comment";
const CHAR*		PZONEID		= "zoneid";
const CHAR*		PSANDBOX		= "sandbox";
const CHAR*		PPAYID		= "payid";

const CHAR*		PGOLD		= "gold";
const CHAR*		PVIPLEVEL	= "viplevel";

#define MAX_HTTP_MSG_LEN	2048




/* 16进制字符表 */
static const char c2x_table[] = "0123456789ABCDEF";
/* 字符转16进制 */
static unsigned char *c2x(unsigned char what, unsigned char *where)
{
    *where++ = '%';
    *where++ = c2x_table[what >> 4];
    *where++ = c2x_table[what & 0xf];
    return where;
}
/* 16进制转字符 */
static char x2c(const char *what)
{
    register char digit;

    digit = ((what[0] >= 'A') ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10 : (what[1] - '0'));

    return (digit);
}
/*
%\\^[]`+$,@:;/!#?=&.
不安全字符及特殊字符
*/
static int unsafechar(unsigned char ch)
{
    unsigned char *p = &ch;

    if(*p == ' ' || *p == '%' || *p == '\\' || *p == '^' || *p == '[' || *p == ']' || *p == '`'\
            || *p == '+' || *p == '$' || *p == ',' || *p == '@' || *p == ':' || *p == ';'\
            || *p == '/' || *p == '!' || *p == '#' || *p == '?' || *p == '=' || *p == '&' || *p == '.' || *p > 0x80)
    {
        return(1);
    }
    else
    {
        return(0);
    }
}
/*
URL解码
*/
size_t url_decode(const char *src, const int srcSize, char *dest, const int destSize)
{
    char *cp = dest;
    int srcIdx = 0;
    int destIdx = 0;
    while(*src != '\0')
    {
        if(*src == '+')
        {
            *dest++ = ' ';
            destIdx++;
            if(destIdx >= destSize)
            {
                return destIdx;
            }
        }
        else if(*src == '%')
        {
            int ch;
            ch = x2c(src + 1);
            *dest++ = ch;
            destIdx++;
            if(destIdx >= destSize)
            {
                return destIdx;
            }
            srcIdx += 2;
            if(srcIdx >= srcSize)
            {
                return destIdx;
            }
            src += 2;
        }
        else
        {
            *dest++ = *src;
            destIdx++;
            if(destIdx >= destSize)
            {
                return destIdx;
            }
        }
        srcIdx++;
        if(srcIdx >= srcSize)
        {
            return destIdx;
        }
        src++;
    }
    *dest = '\0';
    return(dest - cp);
}
/*
URL编码
*/
size_t url_encode(const char *src, char *dest)
{
    char *cp = dest;

    while(*src != '\0')
    {
        unsigned char *p = (unsigned char*)src;
        if(*p == ' ')
        {
            *dest++ = '+';
        }
        else if(unsafechar(*p))
        {
            unsigned char w[3] = {'\0'};
            c2x(*p, w);
            *dest = w[0];
            *(dest + 1) = w[1];
            *(dest + 2) = w[2];
            dest += 3;
        }
        else
        {
            *dest++ = *p;
        }
        src++;
    }
    *dest = '\0';

    return(dest - cp);
}



CRCClient::CRCClient():
    m_dwCliSessionID(0),
    m_poConnection(NULL)
{
    m_qwLastTime = SDTimeSecs();
    m_bTimeOutClose = FALSE;
    m_poPacketProcessor = NULL;
}

CRCClient::~CRCClient()
{

}


VOID CRCClient::SetConnection(ISDConnection* poConnection)
{
    m_poConnection = poConnection;
}

bool CRCClient::OnError(INT32 nSDError, INT32 nSysError)
{
    if(NULL != m_poConnection)
    {
        DBG_INFO(_SDT("[%s:%d]: Connection error: IP = %s, Port = %d, SDError: %d, SysError: %d\n"),
                 MSG_MARK, m_poConnection->GetRemoteIPStr(), m_poConnection->GetRemotePort(), nSDError, nSysError);
    }
    return true;
}

VOID CRCClient::OnEstablish()
{
    //CAutoCycle oAutoCycle("OnEstablish");

    gsapi::GetRCClientMgr()->AddCliSession(this);

}


VOID CRCClient::OnRecv(const CHAR* pBuf, UINT32 nLen)
{
    //CAutoCycle oAutoCycle("OnRecv");

    m_qwLastTime = SDTimeSecs();

    if(NULL == pBuf)
    {
        DBG_INFO(_SDT("[%s:%d]: recv http recharge req data is NULL!\n"), MSG_MARK);
        return;
    }
    string strIP;
    if(m_poConnection)
    {
        strIP = m_poConnection->GetRemoteIPStr();
    }
    DBG_INFO(_SDT("[%s:%d]: OnRecv Data: %s\n"), MSG_MARK, pBuf, strIP.c_str());

    if(nLen > MAX_HTTP_MSG_LEN)
    {
        DBG_INFO(_SDT("[%s:%d]: http msg too long: %d\n"), MSG_MARK, nLen);
        Rspone("http msg too long, must be < 2048");
        return;
    }

    string strTmp1 = pBuf;
    INT32 nPos1 = strTmp1.find('/');
    string strRet("{\"status\":\"error_12\"}");
    if(nPos1 < 0)
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 1, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }
    INT32 nPosTmp = strTmp1.find('?', nPos1);
    if((nPosTmp < 0) || (nPosTmp <= nPos1))
    {
        DBG_INFO(_SDT("[%s:%d]: http data invalid 2, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    string strType = strTmp1.substr(nPos1 + 1, nPosTmp - nPos1 - 1);
    //给同步 用的充值接口
    if(0 == strType.compare("pay"))
    {
        DealHttpRechargeReq(pBuf, nLen);
    }
    //给内部用的充值接口
    else if(0 == strType.compare("mypay"))
    {
        DealHttpMyRechargeReq(pBuf, nLen);
    }
    //给内部GM用的充值接口
    else if(0 == strType.compare("gm"))
    {
        DealHttpGmReq(pBuf, nLen);
    }
    //给统一充值中心用的充值接口
    else if(0 == strType.compare("centerpay"))
    {
        DealHttpCenterRechargeReq(pBuf, nLen);
    }
    else if(0 == strType.compare("interface"))
    {
        DealHttpInterface(pBuf, nLen);
    }
    else
    {
        DealHttpGmReq2(pBuf, nLen, strType);
    }
}

VOID CRCClient::OnTerminate()
{
    m_poConnection = NULL;
    //((CGSApplication*)SDGetApp())->GetRCClientMgr()->RemoveCliSession(this);
}

VOID CRCClient::Release()
{
    gsapi::GetRCClientMgr()->ReleaseCliSession(this);
    m_poConnection = NULL;
}

ISDConnection* CRCClient::GetConnection()
{
    return m_poConnection;
}



VOID CRCClient::SetCliSessionID(UINT32 dwCliSessionID)
{
    m_dwCliSessionID = dwCliSessionID;
}

UINT32 CRCClient::GetCliSessionID()
{
    return m_dwCliSessionID;
}


VOID CRCClient::Disconnect()
{
    if (m_poConnection)
    {
        m_poConnection->Disconnect();
        OnTerminate();
    }
}

BOOL CRCClient::Send(const CHAR* pBuf, INT32 nLen)
{
    if (m_poConnection)
    {
        m_poConnection->Send(pBuf, nLen);

        return TRUE;
    }

    return FALSE;
}


BOOL CRCClient::Send(UINT16 wMsgID, const CHAR* pBuf, INT32 nLen)
{
    if (m_poConnection)
    {
        CHAR acNetBuf[PACKET_LENGTH] = {0};
        UINT32 dwOffset = sizeof(SDNetMsgHeader);

        if(nLen > PACKET_LENGTH)
        {
            return FALSE;
        }
        else
        {
            memcpy(acNetBuf + dwOffset, pBuf, nLen);
        }

        SDNetMsgHeader* pHeader = (SDNetMsgHeader*)acNetBuf;
        INT32 nPackageLen = SDBuildNetMsgHeader(pHeader, wMsgID, nLen);
        m_poConnection->Send(acNetBuf, nPackageLen);

        return TRUE;

    }

    return FALSE;
}

BOOL CRCClient::SendMsg(UINT16 wMsgID , const CHAR* pData)
{
    if (NULL == m_poPacketProcessor)
    {
        return FALSE;
    }

    CSDProtocol* pProtocol = m_poPacketProcessor->GetProtocol();
    if ( NULL == pProtocol )
    {
        return FALSE;
    }
    static CHAR acMsgData[PACKET_LENGTH] = {0};
    INT32 nEncodeRet = pProtocol->Encode(wMsgID, (VOID *)pData, acMsgData, PACKET_LENGTH);
    if (nEncodeRet < 0)
    {
        return FALSE;
    }

    return Send(wMsgID, acMsgData, nEncodeRet);
}

VOID CRCClient::SetPacketProcessor(CSDPacketProcessor*    pPacketProcessor)
{
    m_poPacketProcessor = pPacketProcessor;
}


CRCClientMgr::CRCClientMgr()
{
    m_poLock = NULL;
    m_mapCliSession.clear();
    m_itrCurDeal = m_mapCliSession.end();
    m_dwCurClientID = 0;
}

CRCClientMgr::~CRCClientMgr()
{
    if (NULL != m_poLock)
    {
        SDDelete m_poLock;
        m_poLock = NULL;
    }
}

BOOL CRCClientMgr::Init(UINT32 nMaxClient)
{
    m_poLock = SDNew CSDMutex();
    if(NULL == m_poLock)
    {
        //SYS_CRITICAL("[%s: %d]: Create ISDLock failed!", MSG_MARK);
        UnInit();
        return FALSE;
    }
    m_poCliSessionPool = SDNew CSDObjectPool<CRCClient, CSDMutex>(nMaxClient, nMaxClient);
    if (NULL == m_poCliSessionPool)
    {
        //SYS_CRITICAL("[%s: %d]: CliSession pool init failed!", MSG_MARK);
        UnInit();
        return FALSE;
    }

    new CDealFrame01(&m_oManager);
    new CDealFrame02(&m_oManager);
    new CDealFrame03(&m_oManager);
    new CDealFrame04(&m_oManager);
    new CDealFrame05(&m_oManager);
    new CDealFrame06(&m_oManager);
    new CDealFrame07(&m_oManager);
    new CDealFrame08(&m_oManager);
    new CDealFrame09(&m_oManager);
    new CDealFrame10(&m_oManager);
    new CDealFrame11(&m_oManager);
    new CDealFrame12(&m_oManager);
    new CDealFrame13(&m_oManager);
    new CDealFrame14(&m_oManager);
    new CDealFrame15(&m_oManager);
    new CDealFrame16(&m_oManager);
    new CDealFrame17(&m_oManager);
    new CDealFrame18(&m_oManager);
    new CDealFrame19(&m_oManager);
    new CDealFrame20(&m_oManager);
    new CDealFrame21(&m_oManager);
    new CDealFrame22(&m_oManager);
    new CDealFrame23(&m_oManager);
    new CDealFrame24(&m_oManager);
    new CDealFrame25(&m_oManager);
    new CDealFrame26(&m_oManager);
    new CDealFrame27(&m_oManager);
    new CDealFrame28(&m_oManager);
    new CDealFrame29(&m_oManager);
    new CDealFrame30(&m_oManager);
    new CDealFrame31(&m_oManager);
    new CDealFrame32(&m_oManager);
    new CDealFrame33(&m_oManager);
    new CDealFrame34(&m_oManager);
    new CDealFrame35(&m_oManager);
    new CDealFrame36(&m_oManager);
    new CDealFrame37(&m_oManager);
    new CDealFrame38(&m_oManager);
    new CDealFrame39(&m_oManager);
    new CDealFrame40(&m_oManager);
    new CDealFrame41(&m_oManager);
    new CDealFrame42(&m_oManager);
    new CDealFrame43(&m_oManager);
    new CDealFrame44(&m_oManager);
    new CDealFrame45(&m_oManager);
    new CDealFrame46(&m_oManager);
    new CDealFrame47(&m_oManager);
    new CDealFrame48(&m_oManager);
    new CDealFrame49(&m_oManager);
    new CDealFrame50(&m_oManager);
    new CDealFrame51(&m_oManager);
    new CDealFrame52(&m_oManager);
    new CDealFrame53(&m_oManager);
    new CDealFrame54(&m_oManager);
    new CDealFrame55(&m_oManager);
    new CDealFrame56(&m_oManager);
    new CDealFrame57(&m_oManager);
    new CDealFrame58(&m_oManager);
    new CDealFrame59(&m_oManager);
    new CDealFrame60(&m_oManager);
	new CDealFrame61(&m_oManager);
    new CDealFrame62(&m_oManager);
    new CDealFrame63(&m_oManager);    
    new CDealFrame64(&m_oManager);
    new CDealFrame65(&m_oManager);
	new CDealFrame66(&m_oManager);
	new CDealFrame67(&m_oManager);
    new CDealFrame68(&m_oManager);
    new CDealFrame69(&m_oManager);
    new CDealFrame75ShowGvGPair(&m_oManager);
    new CDealFrame76SetGvGPair(&m_oManager);
    
	new DealFramePower(&m_oManager);
	new CDealFrameAddJewelry(&m_oManager);
	new CDealFrameChangeName(&m_oManager);
	new CDealFrameAddGift(&m_oManager);
	new CDealFrameOperatingActivity(&m_oManager);
    return TRUE;
}

VOID CRCClientMgr::UnInit()
{
    if (NULL != m_poCliSessionPool)
    {
        SDDelete m_poCliSessionPool;
        m_poCliSessionPool = NULL;
    }
    if (NULL != m_poLock)
    {
        SDDelete m_poLock;
        m_poLock = NULL;
    }
}

ISDSession* CRCClientMgr::CreateSession(ISDConnection* poConnection)
{
    CRCClient* poSession = m_poCliSessionPool->Alloc();
    if (NULL == poSession)
    {
        return NULL;
    }
    UINT32 dwClientID = GenerateClientID();
    poSession->SetCliSessionID(dwClientID);
    poSession->SetPacketProcessor(&m_oCliProcessor);
    return poSession;
}

VOID CRCClientMgr::ReleaseCliSession(CRCClient* pSession)
{
    m_poCliSessionPool->Free(pSession);
}

UINT32 CRCClientMgr::GenerateClientID()
{
    m_poLock->Lock();
    if (m_dwCurClientID == 0xFFFFFFFF)
    {
        m_dwCurClientID = 0;
    }
    ++m_dwCurClientID;
    m_poLock->Unlock();

    return m_dwCurClientID;
}

UINT32 CRCClientMgr::GetCliSessionCount()
{
    return m_mapCliSession.size();
}

VOID CRCClientMgr::AddCliSession(CRCClient *poSession)
{
    m_mapCliSession[poSession->GetCliSessionID()] = poSession;
}

VOID CRCClientMgr::RemoveCliSession(CRCClient* poSession)
{
    if(NULL == poSession)
    {
        return;
    }
    //长时间未收到包的检查关闭，由CRCClientMgr::CkSessionOnTimer()方法来删除
    if(poSession->GetTimeOutCloseFlag())
    {
        return;
    }

    CRCClientMapItr itr =
        m_mapCliSession.find(poSession->GetCliSessionID());

    if (itr != m_mapCliSession.end())
    {
        if( itr == m_itrCurDeal)
        {
            m_mapCliSession.erase(m_itrCurDeal++);
        }
        else
        {
            m_mapCliSession.erase(itr);
        }
    }
}

CRCClient* CRCClientMgr::FindCliSession(UINT32 dwClientID)
{
    CRCClientMapItr itr
        = m_mapCliSession.find(dwClientID);
    if(itr == m_mapCliSession.end())
        return NULL;

    return itr->second;
}


VOID CRCClientMgr::ChkSessionOnTimer()
{
    //每10秒钟检测一次监听端口是否已经启动
    SGateListenerInfo& stInfo = CGSConfig::Instance()->GetGateListenerInfo();
    ISDListener* poListener = gsapi::GetRechargeListener();
    if (poListener->Start(stInfo.m_strIP.c_str(), stInfo.m_wPort) != TRUE)
    {
        USR_INFO(_SDT("Listener Start Failed 3"));
    }

    for(CRCClientMapItr itr = m_mapCliSession.begin(); itr != m_mapCliSession.end();)
    {
        CRCClient* poCliSession = itr->second;
        if((poCliSession)
                && ((SDTimeSecs() - poCliSession->GetLastTime()) > 30)) // 30秒就断开
        {
            ISDConnection* m_poConnection = poCliSession->GetConnection();
            if(m_poConnection)
            {
                m_poConnection->Disconnect();
            }
            m_mapCliSession.erase(itr++);
        }
        else
        {
            itr++;
        }
    }
}





VOID CRCClient::DealHttpRechargeReq(const CHAR* pBuf, UINT32 nLen)
{
    if(NULL == pBuf)
    {
        DBG_INFO(_SDT("[%s:%d]: recv http recharge req data is NULL!\n"), MSG_MARK);
        return;
    }
    DBG_INFO(_SDT("[%s:%d]: recv http recharge req, data: %s\n"), MSG_MARK, pBuf);

    string strTmp1 = pBuf;
    INT32 nPos1 = strTmp1.find('?');
    string strTmp2;
    string strRet("{\"status\":\"error_12\"}");
    if(nPos1 > 0)
    {
        strTmp2 = strTmp1.substr(nPos1 + 1, strTmp1.length() - nPos1 - 1);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 1, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }
    INT32 nPosTmp = strTmp2.find(PSIGN);
    if(nPosTmp < 0)
    {
        DBG_INFO(_SDT("[%s:%d]: http data invalid 33, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
    }
    INT32 nPos2 = strTmp2.find(' ', nPosTmp + 1);
    if(nPos2 > 0)
    {
        strTmp1 = strTmp2.substr(0, nPos2);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 2, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    INT32 nPos3 = strTmp1.find(PSIGN);
    string strSrc;
    if(nPos3 > 0)
    {
        strSrc = strTmp1.substr(0, nPos3 - 1);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 3, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }
    //替换空格转义，其他转义有需求再修改
    strSrc = ReplaceStr(strSrc, "%20", " ");

    vector<string> vecField;
    vecField = SDSplitStringsA(strTmp1, '&');
    UINT32 dwSize = vecField.size();
    map<string, string> mapField;
    for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        vector<string> vecTmp = SDSplitStringsA(vecField[dwIdx], '=');
        if(2 == vecTmp.size())
        {
            mapField[vecTmp[0]] = vecTmp[1];
        }
    }


    if((mapField[PSOURCE].empty()) ||
            (mapField[PTRADE_NO].empty()) ||
            (mapField[PAMOUNT].empty()) ||
            (mapField[PPARTNER].empty()) ||
            (mapField[PSIGN].empty())
      )
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 4, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    if(0 != mapField[PSOURCE].compare("tongbu"))
    {
        DBG_INFO(_SDT("[%s:%d]: http data is not tongbu, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    //UINT8 abyInput[256] = {0};

    CNGString ssInput;
    UINT8 abyDigest[16] = {0};
    //sprintf((CHAR*)abyInput, "%s%s", strSrc.c_str(), PTBKEY);
    ssInput += strSrc.c_str();
    ssInput += PTBKEY;
    string strInput = ssInput.str();
    SDMD5(abyDigest, (UINT8*)strInput.c_str(), strInput.length());
    CHAR szTemp[32] = {0};
    CHAR szHexDigest[256] = {0};
    for (UINT8 byIdx = 0; byIdx < 16; byIdx++)
    {
        sprintf(szTemp, "%02x", (UINT8)abyDigest[byIdx]);
        strcat(szHexDigest, szTemp);
    }
    if(0 != strcmp(szHexDigest, mapField[PSIGN].c_str()))
    {
        DBG_INFO(_SDT("[%s:%d]: http data sign invalid, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }


    STongBuPayProcessed stInfo;
    stInfo.strTableName	= CGSConfig::Instance()->GetAppPayTableName();
    stInfo.strOrderID		= mapField[PTRADE_NO];
    stInfo.dwSessionID	= m_dwCliSessionID;
    stInfo.strSource		= mapField[PSOURCE];
    stInfo.strPartner		= mapField[PPARTNER];

    //充值的金额（同步回调回的的单位是分）
    stInfo.dwRMB			= SDAtou(mapField[PAMOUNT].c_str());

    //处理结果
    //同时把分转成单位元
    SBuyGoldProp buyInfo;
    if( CVipPropMgr::Instance()->GetBuyPropByRMB(AUTH_TYPE_MAXNET, stInfo.dwRMB / 100, buyInfo ) )
    {
        stInfo.dwGold = buyInfo.dwGold;
    }
    else
    {
        //对于找不到的值，直接把分转换成元宝。一分等于0.1元宝
        stInfo.dwGold = stInfo.dwRMB / 10;
    }

    CDBPktBuilder::Instance()->OnProcessOrderReq(stInfo);

    return;
}

VOID CRCClient::DealHttpMyRechargeReq(const CHAR* pBuf, UINT32 nLen)
{
    if(NULL == pBuf)
    {
        DBG_INFO(_SDT("[%s:%d]: recv http my recharge req data is NULL!\n"), MSG_MARK);
        return;
    }
    DBG_INFO(_SDT("[%s:%d]: recv http my recharge req, data: %s\n"), MSG_MARK, pBuf);

    CHAR* pszBuff = SDNew CHAR[nLen * 3 + 1];
    memset(pszBuff, 0, sizeof(pszBuff));
    url_decode(pBuf, nLen, pszBuff, nLen * 3);

    string strTmp1 = pszBuff;
    SDDelete pszBuff;

    INT32 nPos1 = strTmp1.find('?');
    string strTmp2;
    string strRet("{\"status\":\"error_12\"}");
    if(nPos1 > 0)
    {
        strTmp2 = strTmp1.substr(nPos1 + 1, strTmp1.length() - nPos1 - 1);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 1, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }
    INT32 nPosTmp = strTmp2.find(PSIGN);
    if(nPosTmp < 0)
    {
        DBG_INFO(_SDT("[%s:%d]: http data invalid 33, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
    }
    INT32 nPos2 = strTmp2.find(' ', nPosTmp + 1);
    if(nPos2 > 0)
    {
        strTmp1 = strTmp2.substr(0, nPos2);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 2, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    INT32 nPos3 = strTmp1.find(PSIGN);
    string strSrc;
    if(nPos3 > 0)
    {
        strSrc = strTmp1.substr(0, nPos3 - 1);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 3, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    vector<string> vecField;
    vecField = SDSplitStringsA(strTmp1, '&');
    UINT32 dwSize = vecField.size();
    map<string, string> mapField;
    for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        vector<string> vecTmp = SDSplitStringsA(vecField[dwIdx], '=');
        if(2 == vecTmp.size())
        {
            mapField[vecTmp[0]] = vecTmp[1];
        }
    }

    if((mapField[PRMB].empty()) ||
            (mapField[PORDERID].empty()) ||
            (mapField[PGIVENGOLD].empty()) ||
            (mapField[PPLAYERID].empty()) ||
            (mapField[PSANDBOX].empty()) ||
            (mapField[PCOMMENT].empty())
      )
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 4, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }
    CNGString ssInput;
    UINT8 abyDigest[16] = {0};
    //sprintf((CHAR*)abyInput, "%s%s", strSrc.c_str(), PMYKEY);
    ssInput += strSrc.c_str();
    ssInput += PMYKEY;
    string strInput = ssInput.str();
    SDMD5(abyDigest, (UINT8*)strInput.c_str(), strInput.length());

    CHAR szTemp[32] = {0};
    CHAR szHexDigest[256] = {0};
    for (UINT8 byIdx = 0; byIdx < 16; byIdx++)
    {
        sprintf(szTemp, "%02x", (UINT8)abyDigest[byIdx]);
        strcat(szHexDigest, szTemp);
    }
    if(0 != strcmp(szHexDigest, mapField[PSIGN].c_str()))
    {
        DBG_INFO(_SDT("[%s:%d]: http data sign invalid, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }
    string strSandBox = mapField[PSANDBOX];
    if((0 != strSandBox.compare("0")) && (0 != strSandBox.compare("1")))
    {
        DBG_INFO(_SDT("[%s:%d]: http data sandbox invalid, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    SMyPayProcessed stInfo;
    stInfo.dwPlayerID		= SDAtou(mapField[PPLAYERID].c_str());
    stInfo.dwGivenGold	= SDAtou(mapField[PGIVENGOLD].c_str());
    stInfo.strComment		= mapField[PCOMMENT];
    stInfo.strOrderID		= mapField[PORDERID];
    stInfo.dwRMB			= SDAtou(mapField[PRMB].c_str()) * 100;//单位为元，转为分
    stInfo.bySandBoxFlag	= SDAtou(mapField[PSANDBOX].c_str());
    stInfo.dwSessionID	= m_dwCliSessionID;
    stInfo.wZoneID		= gsapi::GetZoneID();

    if(NULL == CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stInfo.dwPlayerID))
    {
        Rspone("{\"status\":\"error_13\"}", stInfo.strOrderID);
        return;
    }
    CDBPktBuilder::Instance()->OnProcessMyOrderReq(stInfo);

    return;
}


VOID CRCClient::DealHttpCenterRechargeReq(const CHAR* pBuf, UINT32 nLen)
{
    if(NULL == pBuf)
    {
        DBG_INFO(_SDT("[%s:%d]: recv http my recharge req data is NULL!\n"), MSG_MARK);
        return;
    }
    DBG_INFO(_SDT("[%s:%d]: recv http my recharge req, data: %s\n"), MSG_MARK, pBuf);

    CHAR* pszBuff = SDNew CHAR[nLen * 3 + 1];
    memset(pszBuff, 0, sizeof(pszBuff));
    url_decode(pBuf, nLen, pszBuff, nLen * 3);

    string strTmp1 = pszBuff;
    SDDelete pszBuff;

    INT32 nPos1 = strTmp1.find('?');
    string strTmp2;
    string strRet("{\"status\":\"error_12\"}");
    if(nPos1 > 0)
    {
        strTmp2 = strTmp1.substr(nPos1 + 1, strTmp1.length() - nPos1 - 1);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 1, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }
    INT32 nPosTmp = strTmp2.find(PSIGN);
    if(nPosTmp < 0)
    {
        DBG_INFO(_SDT("[%s:%d]: http data invalid 33, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
    }
    INT32 nPos2 = strTmp2.find(' ', nPosTmp + 1);
    if(nPos2 > 0)
    {
        strTmp1 = strTmp2.substr(0, nPos2);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 2, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    INT32 nPos3 = strTmp1.find(PSIGN);
    string strSrc;
    if(nPos3 > 0)
    {
        strSrc = strTmp1.substr(0, nPos3 - 1);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 3, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    vector<string> vecField;
    vecField = SDSplitStringsA(strTmp1, '&');
    UINT32 dwSize = vecField.size();
    map<string, string> mapField;
    for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        vector<string> vecTmp = SDSplitStringsA(vecField[dwIdx], '=');
        if(2 == vecTmp.size())
        {
            mapField[vecTmp[0]] = vecTmp[1];
        }
    }

    if((mapField[PRMB].empty()) ||
            (mapField[PORDERID].empty()) ||
            (mapField[PGOLD].empty()) ||
            (mapField[PPLAYERID].empty()) ||
            (mapField[PSANDBOX].empty()) ||
            (mapField[PVIPLEVEL].empty()) ||
			(mapField[PPAYID].empty())
      )
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 4, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }
    CNGString ssInput;
    UINT8 abyDigest[16] = {0};
    //sprintf((CHAR*)abyInput, "%s%s", strSrc.c_str(), PMYKEY);
    ssInput += strSrc.c_str();
    ssInput += PMYKEY;
    string strInput = ssInput.str();
    SDMD5(abyDigest, (UINT8*)strInput.c_str(), strInput.length());

    CHAR szTemp[32] = {0};
    CHAR szHexDigest[256] = {0};
    for (UINT8 byIdx = 0; byIdx < 16; byIdx++)
    {
        sprintf(szTemp, "%02x", (UINT8)abyDigest[byIdx]);
        strcat(szHexDigest, szTemp);
    }
    if(0 != strcmp(szHexDigest, mapField[PSIGN].c_str()))
    {
        DBG_INFO(_SDT("[%s:%d]: http data sign invalid, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }
    string strSandBox = mapField[PSANDBOX];
    if((0 != strSandBox.compare("0")) && (0 != strSandBox.compare("1")))
    {
        DBG_INFO(_SDT("[%s:%d]: http data sandbox invalid, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    SCenterPayProcessed	stInfo;
    stInfo.dwPlayerID		= SDAtou(mapField[PPLAYERID].c_str());
    stInfo.dwGold			= SDAtou(mapField[PGOLD].c_str());
    stInfo.strComment		= mapField[PCOMMENT];
    stInfo.strOrderID		= mapField[PORDERID];
    stInfo.dwRMB			= SDAtou(mapField[PRMB].c_str()) * 100; //单位为元转成分
    stInfo.bySandBoxFlag	= SDAtou(mapField[PSANDBOX].c_str());
    stInfo.dwSessionID	= m_dwCliSessionID;
    stInfo.wZoneID		= gsapi::GetZoneID();
    stInfo.byVipLevel		= SDAtou(mapField[PVIPLEVEL].c_str());
	stInfo.dwPayID		= SDAtou(mapField[PPAYID].c_str());

    //查找玩家是否存在
    if(NULL == CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(stInfo.dwPlayerID))
    {
        Rspone("{\"status\":\"error_13\"}", stInfo.strOrderID);
        return;
    }

    CDBPktBuilder::Instance()->OnProcessCenterOrderReq(stInfo);

    return;
}


VOID CRCClient::DealHttpGmReq(const CHAR* pBuf, UINT32 nLen)
{
    if(NULL == pBuf)
    {
        DBG_INFO(_SDT("[%s:%d]: recv http gm req data is NULL!\n"), MSG_MARK);
        return;
    }
    DBG_INFO(_SDT("[%s:%d]: recv http gm req, data: %s\n"), MSG_MARK, pBuf);


    string strTmp1 = pBuf;

    INT32 nPos1 = strTmp1.find('?');
    string strTmp2;
    string strRet("{\"status\":\"error_12\"}");
    if(nPos1 > 0)
    {
        strTmp2 = strTmp1.substr(nPos1 + 1, strTmp1.length() - nPos1 - 1);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 1, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }
    INT32 nPosTmp = strTmp2.find(PSIGN);
    if(nPosTmp < 0)
    {
        DBG_INFO(_SDT("[%s:%d]: http data invalid 33, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }
    INT32 nPos2 = strTmp2.find(' ', nPosTmp + 1);
    if(nPos2 > 0)
    {
        strTmp1 = strTmp2.substr(0, nPos2);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 2, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    INT32 nPos3 = strTmp1.find(PSIGN);
    string strSrc;
    if(nPos3 > 0)
    {
        strSrc = strTmp1.substr(0, nPos3 - 1);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 3, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }
    ////替换空格转义，其他转义有需求再修改
    //strSrc = ReplaceStr(strSrc, "%20", " ");

    vector<string> vecField;
    vecField = SDSplitStringsA(strTmp1, '&');
    UINT32 dwSize = vecField.size();
    map<string, string> mapField;
    for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        vector<string> vecTmp = SDSplitStringsA(vecField[dwIdx], '=');
        if(2 == vecTmp.size())
        {
            mapField[vecTmp[0]] = vecTmp[1];
        }
    }

    if((mapField[PSERIALNUMBER].empty()) ||
            (mapField[PACTION].empty()) ||
            (mapField[PPLAYERID].empty()) ||
            (mapField[PTIME].empty()) ||
            (mapField[PSIGN].empty())
      )
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 4, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    CNGString ssInput;
    UINT8 abyDigest[16] = {0};
    //sprintf((CHAR*)abyInput, "%s%s", strSrc.c_str(), PGMKEY);
    ssInput += strSrc.c_str();
    ssInput += PGMKEY;
    string strInput = ssInput.str();
    SDMD5(abyDigest, (UINT8*)strInput.c_str(), strInput.length());



    CHAR szTemp[32] = {0};
    CHAR szHexDigest[256] = {0};
    for (UINT8 byIdx = 0; byIdx < 16; byIdx++)
    {
        sprintf(szTemp, "%02x", (UINT8)abyDigest[byIdx]);
        strcat(szHexDigest, szTemp);
    }
    if(0 != strcmp(szHexDigest, mapField[PSIGN].c_str()))
    {
        DBG_INFO(_SDT("[%s:%d]: http data sign invalid, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    UINT64	qwSerialNumber	= SDAtou64(mapField[PSERIALNUMBER].c_str());
    UINT8	byAction			= (UINT8)(SDAtou(mapField[PACTION].c_str()));
    UINT32	dwAreaID			= 0;
    UINT32	dwPlayerID		= SDAtou(mapField[PPLAYERID].c_str());
    UINT32	dwParam1			= SDAtou(mapField[PPARAM1].c_str());
    UINT64	qwParam2			= SDAtou64(mapField[PPARAM2].c_str());
    UINT64	qwParam3			= SDAtou64(mapField[PPARAM3].c_str());
    tstring	strParam4		= mapField[PPARAM4].c_str();
	tstring	strParam5		= mapField[PPARAM5].c_str();
	tstring	strDesc			= mapField[DESC].c_str();
	
    UINT64 qwTime		= SDAtou64(mapField[PTIME].c_str());
    UINT64 qwCurTime	= SDTimeSecs();
    UINT64 qwTmp		= qwCurTime > qwTime ? qwCurTime - qwTime : qwTime - qwCurTime;
    if(qwTmp > 600) //相差600秒后的失效
    {
        Rspone("{\"status\":\"request overtime!\"}");
        return;
    }

    CRCClientMgr* poRCClientMgr = gsapi::GetRCClientMgr();
    //检查是否重复请求
    if(!poRCClientMgr->AddGmSerial(qwSerialNumber))
    {
        Rspone("{\"status\":\"reduplicative request!\"}");
        return;
    }
	//中文转码
	UrlDecode(strParam4);
	//中文转码
	UrlDecode(strParam5);
    poRCClientMgr->GetManager().Deal(byAction, this, (UINT32)qwSerialNumber, byAction, dwAreaID, dwPlayerID, dwParam1, qwParam2, qwParam3, (TCHAR*)strParam4.c_str(),  (TCHAR*)strParam5.c_str(), (TCHAR*)strDesc.c_str());
    return;
}



VOID CRCClient::DealHttpGmReq2(const CHAR* pBuf, UINT32 nLen, string strCmdInfo)
{
    if(NULL == pBuf)
    {
        DBG_INFO(_SDT("[%s:%d]: recv http gm req data is NULL!\n"), MSG_MARK);
        return;
    }

    if (strCmdInfo.empty())
    {
        DBG_INFO(_SDT("[%s:%d]: recv http gm req cmd is NULL!\n"), MSG_MARK);
        return;
    }
//    DBG_INFO(_SDT("[%s:%d]: recv http gm req, data: %s\n"), MSG_MARK, pBuf);

    CHAR* pszBuff = SDNew CHAR[nLen * 3 + 1];
    memset(pszBuff, 0, sizeof(pszBuff));
    url_decode(pBuf, nLen, pszBuff, nLen * 3);

    string strTmp1 = pszBuff;
    SDDelete pszBuff;
    pszBuff = NULL;
    INT32 nPos1 = strTmp1.find('?');
    string strTmp2;
    string strRet = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);

    if(nPos1 > 0)
    {
        strTmp2 = strTmp1.substr(nPos1 + 1, strTmp1.length() - nPos1 - 1);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 1, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    INT32 nPosTmp = strTmp2.find(PSIGN);

    if(nPosTmp < 0)
    {
        DBG_INFO(_SDT("[%s:%d]: http data invalid 33, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
    }

    INT32 nPos2 = strTmp2.find(' ', nPosTmp + 1);

    if(nPos2 > 0)
    {
        strTmp1 = strTmp2.substr(0, nPos2);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 2, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    INT32 nPos3 = strTmp1.find(PSIGN);
    string strSrc;
    if(nPos3 > 0)
    {
        strSrc = strTmp1.substr(0, nPos3 - 1);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 3, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }
    ////替换空格转义，其他转义有需求再修改
    //strSrc = ReplaceStr(strSrc, "%20", " ");

    vector<string> vecField;
    vecField = SDSplitStringsA(strTmp1, '&');
    UINT32 dwSize = vecField.size();
    map<string, string> mapField;

    for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        vector<string> vecTmp = SDSplitStringsA(vecField[dwIdx], '=');
        if(2 == vecTmp.size())
        {
            mapField[vecTmp[0]] = vecTmp[1];
        }
    }

    if((mapField[PSERIALNUMBER].empty()) ||
            (mapField[PTIME].empty()) ||
            (mapField[PSIGN].empty())
      )
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 4, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    CNGString ssInput;
    UINT8 abyDigest[16] = {0};
    //sprintf((CHAR*)abyInput, "%s%s", strSrc.c_str(), PGMKEY);
    ssInput += strSrc.c_str();
    ssInput += PGMKEY;
    string strInput = ssInput.str();
    SDMD5(abyDigest, (UINT8*)strInput.c_str(), strInput.length());

    CHAR szTemp[32] = {0};
    CHAR szHexDigest[256] = {0};
    for (UINT8 byIdx = 0; byIdx < 16; byIdx++)
    {
        sprintf(szTemp, "%02x", (UINT8)abyDigest[byIdx]);
        strcat(szHexDigest, szTemp);
    }
    if(0 != strcmp(szHexDigest, mapField[PSIGN].c_str()))
    {
        DBG_INFO(_SDT("[%s:%d]: http data sign invalid, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    UINT64	qwSerialNumber	= SDAtou64(mapField[PSERIALNUMBER].c_str());
    UINT64	qwTime			= SDAtou64(mapField[PTIME].c_str());
    UINT64	qwCurTime		= SDTimeSecs();
    UINT64	qwTmp			= qwCurTime > qwTime ? qwCurTime - qwTime : qwTime - qwCurTime;
    UINT32	dwAreaID			= 0;

    if(qwTmp > 600) //相差600秒后的失效
    {
        string strError = GetRsponeResult(ERR_GM_PARM_INFO::ID_OVER_TIME);
        Rspone(strError);
        return;
    }

    CRCClientMgr* poRCClientMgr = gsapi::GetRCClientMgr();

    //检查是否重复请求
    if(!poRCClientMgr->AddGmSerial(qwSerialNumber))
    {
        string strError = GetRsponeResult(ERR_GM_PARM_INFO::ID_REDUPLICATIVE);
        Rspone(strError);
        return;
    }

    poRCClientMgr->GetManager().Deal(this,(UINT32)qwSerialNumber,dwAreaID,0, mapField, strCmdInfo);
    return;
}

VOID CRCClient::DealHttpInterface(const CHAR* pBuf, UINT32 nLen)
{
    if(NULL == pBuf)
    {
        DBG_INFO(_SDT("[%s:%d]: recv http interface req data is NULL!\n"), MSG_MARK);
        return;
    }

    DBG_INFO(_SDT("[%s:%d]: recv http interface req, data: %s\n"), MSG_MARK, pBuf);

    CHAR* pszBuff = SDNew CHAR[nLen * 3 + 1];
    memset(pszBuff, 0, sizeof(pszBuff));
    url_decode(pBuf, nLen, pszBuff, nLen * 3);

    string strTmp1 = pszBuff;
    SDDelete pszBuff;
    pszBuff = NULL;
    INT32 nPos1 = strTmp1.find('?');
    string strTmp2;
    string strRet = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);

    if(nPos1 > 0)
    {
        strTmp2 = strTmp1.substr(nPos1 + 1, strTmp1.length() - nPos1 - 1);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 1, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    INT32 nPos2 = strTmp2.find(' ', 0);

    if(nPos2 > 0)
    {
        strTmp1 = strTmp2.substr(0, nPos2);
    }
    else
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 2, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    vector<string> vecField;
    vecField = SDSplitStringsA(strTmp1, '&');
    UINT32 dwSize = vecField.size();
    map<string, string> mapField;

    for(UINT32 dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        vector<string> vecTmp = SDSplitStringsA(vecField[dwIdx], '=');
        if(2 == vecTmp.size())
        {
            mapField[vecTmp[0]] = vecTmp[1];
        }
    }

    if	(mapField[PCMD].empty())
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 4, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    CRCClientMgr* poRCClientMgr = gsapi::GetRCClientMgr();
    poRCClientMgr->GetManager().Interface(mapField[PCMD], this);
    return;
}

VOID CRCClient::RspPlayerRes(CPlayer* poPlayer)
{
    if(NULL == poPlayer)
    {
        Rspone("{\"status\":\"player not exist!\"}");
        return;
    }
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();

    CNGString ssMsg;
    ssMsg += "PlayerID=";
    ssMsg += poPlayer->GetID() ;
    ssMsg += "\n";

    ssMsg += "Coin=";
    ssMsg += stDT_PLAYER_BASE_DATA.qwCoin;
    ssMsg += "\n";
    ssMsg += "Gold=";
    ssMsg += stDT_PLAYER_BASE_DATA.qwGold;
    ssMsg += "\n";
    ssMsg += "Story=";
    ssMsg += stDT_PLAYER_BASE_DATA.qwStory;
    ssMsg += "\n";
    ssMsg += "PhyStrength=";
    ssMsg += stDT_PLAYER_BASE_DATA.wPhyStrength;
    ssMsg += "\n";
    ssMsg += "Science=";
    ssMsg += stDT_PLAYER_BASE_DATA.qwScience;
    ssMsg += "\n";
    ssMsg += "BlueGas=";
    ssMsg += stDT_PLAYER_BASE_DATA.qwBlueGas;
    ssMsg += "\n";
    ssMsg += "PurpleGas=";
    ssMsg += stDT_PLAYER_BASE_DATA.qwPurpleGas;
    ssMsg += "\n";
    ssMsg += "JingJie=";
    ssMsg += stDT_PLAYER_BASE_DATA.qwJingJie;
    ssMsg += "\n";

    // 卦象
    ssMsg += "Awaken={";
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(poAwaken)
    {
        //todo
        DT_AWAKEN_DATA&	stDT_AWAKEN_DATA = poAwaken->GetDT_AWAKEN_DATA();

        if (0 != stDT_AWAKEN_DATA.stBaseInfo.byLevel)
        {
            ssMsg += "Level=";
            ssMsg += UINT16(stDT_AWAKEN_DATA.stBaseInfo.byLevel);
            ssMsg += ",";
            //ssMsg<<"GoldFlag="<<stDT_AWAKEN_DATA.stBaseInfo.byGoldFlag<<",";
            ssMsg += "StudyExp=";
            ssMsg += stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp;
            ssMsg += ",";
            //ssMsg<<"HaveCallNum="<<stDT_AWAKEN_DATA.stBaseInfo.wHaveCallNum<<",";
            //ssMsg<<"LastCallTime="<<stDT_AWAKEN_DATA.stBaseInfo.qwlastCallTime<<",";
            //ssMsg<<"StudyClip="<<stDT_AWAKEN_DATA.stBaseInfo.qwStudyClip<<",";

            ssMsg += "bagStudyInfo=[";

            for (size_t i = 0; i < MAX_BAG_STUDY_GRID_NUM; ++i)
            {
                if (0 != stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyColorKind)
                {
                    if (0 != i)
                    {
                        ssMsg += ",(";
                        ssMsg += "StudyColorKind=";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyColorKind);
                        ssMsg += ",";
                        ssMsg += "StudyAttrKind1=";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyAttrKind1);
                        ssMsg += ",";
                        ssMsg += "byStudyLevel=";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyLevel);
                        ssMsg += "StudyAttrKind2=";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyAttrKind2);
                        ssMsg += ")";
                    }
                    else
                    {
                        ssMsg += "(";
                        ssMsg += "StudyColorKind=";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyColorKind);
                        ssMsg += ",";
                        ssMsg += "StudyAttrKind1=";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyAttrKind1);
                        ssMsg += ",";
                        ssMsg += "byStudyLevel=";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyLevel); //<<",";
                        ssMsg += "StudyAttrKind2=";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyAttrKind2);
                        ssMsg += ")";
                    }
                }
                else
                {
                    continue;
                }
            }
            ssMsg += "]";
        }
    }
    ssMsg += "}\n";


    // 装备
    ssMsg += "EquipItem={";
    DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI stAllEquip = {0};
    poPlayer->GetAllEquip(stAllEquip);

    for(size_t i = 0; i < stAllEquip.wEquipNum; ++i)
    {
        if (0 != i)
        {
            ssMsg += ",(";

            // 取得装备名
            SEquipProp* pstEquipProp = CEquipPropMgr::Instance()->GetProp(stAllEquip.astEquipInfo[i].wKindID);
            if (pstEquipProp)
            {
                ssMsg += "EquipName=";
                ssMsg += pstEquipProp->szName;
                ssMsg += ",";
            }
            else
            {
                ssMsg += "EquipName=";
                ssMsg += "unknown equip name";
                ssMsg +=  ",";
            }

            ssMsg += "KindID=";
            ssMsg += stAllEquip.astEquipInfo[i].wKindID;
            ssMsg += ",";
            ssMsg += "Idx=";
            ssMsg += stAllEquip.astEquipInfo[i].wIdx;
            ssMsg += ",";
            ssMsg += "DressPos=";
            ssMsg += UINT16(stAllEquip.astEquipInfo[i].byDressPos);
            ssMsg += ",";
            ssMsg += "Level=";
            ssMsg += stAllEquip.astEquipInfo[i].wLevel;
            ssMsg += ",";
            ssMsg += "DressLevel=";
            ssMsg += stAllEquip.astEquipInfo[i].wDressLevel;
            ssMsg += ",";

            ssMsg += ")";
        }
        else
        {
            ssMsg += "(";

            // 取得装备名
            SEquipProp* pstEquipProp = CEquipPropMgr::Instance()->GetProp(stAllEquip.astEquipInfo[i].wKindID);
            if (pstEquipProp)
            {
                ssMsg += "EquipName=";
                ssMsg += pstEquipProp->szName;
                ssMsg += ",";
            }
            else
            {
                ssMsg += "EquipName=";
                ssMsg += "unknown equip name";
                ssMsg += ",";
            }

            ssMsg += "KindID=";
            ssMsg += stAllEquip.astEquipInfo[i].wKindID;
            ssMsg += ",";
            ssMsg += "Idx=";
            ssMsg += stAllEquip.astEquipInfo[i].wIdx;
            ssMsg += ",";
            ssMsg += "DressPos=";
            ssMsg +=  UINT16(stAllEquip.astEquipInfo[i].byDressPos);
            ssMsg += ",";
            ssMsg += "Level=";
            ssMsg +=  stAllEquip.astEquipInfo[i].wLevel;
            ssMsg += ",";
            ssMsg += "DressLevel=";
            ssMsg += stAllEquip.astEquipInfo[i].wDressLevel;
            ssMsg += ",";

            ssMsg += ")";
        }
    }
    ssMsg += "}\n";


    // 物品
    ssMsg += "GoodsItem={";
    DT_ITEM_DATA_LIST_CLI2 stBagItemInfo = {0};
    poPlayer->GetBag().GetBagItem(stBagItemInfo);

    bool firstElem = TRUE;

    for(size_t i = 0; i < stBagItemInfo.byItemNum; ++i)
    {
        if (EIK_GOODS == stBagItemInfo.astItemList[i].byItemType)
        {
            if (!firstElem)
            {
                ssMsg += ",(";

                // 取得物品名
                SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(stBagItemInfo.astItemList[i].stItemInfo.stGoods.wKindID);
                if (pstGoodsProp)
                {
                    ssMsg += "GoodsName=";
                    ssMsg += pstGoodsProp->szName;
                    ssMsg += ",";
                }
                else
                {
                    ssMsg += "GoodsName=";
                    ssMsg += "unknown goods name";
                    ssMsg += ",";
                }

                ssMsg += "KindID=";
                ssMsg += stBagItemInfo.astItemList[i].stItemInfo.stGoods.wKindID;
                ssMsg += ",";
                ssMsg += "GoodsMainKindID=";
                ssMsg += UINT16(stBagItemInfo.astItemList[i].stItemInfo.stGoods.byGoodsMainKindID);
                ssMsg += ",";
                ssMsg += "UseLevel=";
                ssMsg += stBagItemInfo.astItemList[i].stItemInfo.stGoods.wUseLevel;
                ssMsg += ",";
                ssMsg += "PileCount=";
                ssMsg += stBagItemInfo.astItemList[i].stItemInfo.stGoods.wPileCount;
                ssMsg += ",";
                ssMsg += ")";
            }
            else
            {
                ssMsg += "(";

                // 取得物品名
                SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(stBagItemInfo.astItemList[i].stItemInfo.stGoods.wKindID);
                if (pstGoodsProp)
                {
                    ssMsg += "GoodsName=";
                    ssMsg += pstGoodsProp->szName;
                    ssMsg += ",";
                }
                else
                {
                    ssMsg += "GoodsName=";
                    ssMsg += "unknown goods name";
                    ssMsg += ",";
                }

                ssMsg += "KindID=";
                ssMsg += stBagItemInfo.astItemList[i].stItemInfo.stGoods.wKindID;
                ssMsg += ",";
                ssMsg += "GoodsMainKindID=";
                ssMsg += UINT16(stBagItemInfo.astItemList[i].stItemInfo.stGoods.byGoodsMainKindID);
                ssMsg += ",";
                ssMsg += "UseLevel=";
                ssMsg += stBagItemInfo.astItemList[i].stItemInfo.stGoods.wUseLevel;
                ssMsg += ",";
                ssMsg += "PileCount=";
                ssMsg += stBagItemInfo.astItemList[i].stItemInfo.stGoods.wPileCount;
                ssMsg += ",";
                ssMsg += ")";

                firstElem = FALSE;
            }
        }
        else
        {
            continue;
        }
    }
    ssMsg += "}\n";

    if(ssMsg.str().empty())
    {
        Rspone("{\"status\":\"no data!\"}");
    }
    else
    {
        Rspone(ssMsg.str());
    }
}

VOID CRCClient::RspPlayerRes2( CPlayer* poPlayer )
{
    if(NULL == poPlayer)
    {
        return ;
    }
    const DT_PLAYER_BASE_DATA& stDT_PLAYER_BASE_DATA = poPlayer->GetDT_PLAYER_BASE_DATA();

    CNGString ssMsg;
    ssMsg += "\"baseattr\":{\"PlayerID\":\"";
    ssMsg += poPlayer->GetID() ;
    ssMsg += ",";

    ssMsg += "\"Coin\":";
    ssMsg += stDT_PLAYER_BASE_DATA.qwCoin;
    ssMsg += ",";
    ssMsg += "\"Gold\":";
    ssMsg += stDT_PLAYER_BASE_DATA.qwGold;
    ssMsg += ",";
    ssMsg += "\"Story\":";
    ssMsg += stDT_PLAYER_BASE_DATA.qwStory;
    ssMsg += ",";
    ssMsg += "\"PhyStrength\":";
    ssMsg += stDT_PLAYER_BASE_DATA.wPhyStrength;
    ssMsg += ",";
    ssMsg += "\"Science\":";
    ssMsg += stDT_PLAYER_BASE_DATA.qwScience;
    ssMsg += ",";
    ssMsg += "\"BlueGas\":";
    ssMsg += stDT_PLAYER_BASE_DATA.qwBlueGas;
    ssMsg += ",";
    ssMsg += "\"PurpleGas\":";
    ssMsg += stDT_PLAYER_BASE_DATA.qwPurpleGas;
    ssMsg += ",";
    ssMsg += "\"JingJie\":";
    ssMsg += stDT_PLAYER_BASE_DATA.qwJingJie;
    ssMsg += "},";

    // 卦象
    ssMsg += "\"Awaken\"={";
    CAwaken* poAwaken = poPlayer->GetAwaken();
    if(poAwaken)
    {
        //todo
        DT_AWAKEN_DATA&	stDT_AWAKEN_DATA = poAwaken->GetDT_AWAKEN_DATA();

        if (0 != stDT_AWAKEN_DATA.stBaseInfo.byLevel)
        {
            ssMsg += "\"Level\":";
            ssMsg += UINT16(stDT_AWAKEN_DATA.stBaseInfo.byLevel);
            ssMsg += ",";
            //ssMsg<<"GoldFlag="<<stDT_AWAKEN_DATA.stBaseInfo.byGoldFlag<<",";
            ssMsg += "\"StudyExp\":";
            ssMsg += stDT_AWAKEN_DATA.stBaseInfo.qwStudyExp;
            ssMsg += ",";
            //ssMsg<<"HaveCallNum="<<stDT_AWAKEN_DATA.stBaseInfo.wHaveCallNum<<",";
            //ssMsg<<"LastCallTime="<<stDT_AWAKEN_DATA.stBaseInfo.qwlastCallTime<<",";
            //ssMsg<<"StudyClip="<<stDT_AWAKEN_DATA.stBaseInfo.qwStudyClip<<",";

            ssMsg += "bagStudyInfo:[";

            for (size_t i = 0; i < MAX_BAG_STUDY_GRID_NUM; ++i)
            {
                if (0 != stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyColorKind)
                {
                    if (0 != i)
                    {
                        ssMsg += ",{";
                        ssMsg += "\"StudyColorKind\":";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyColorKind);
                        ssMsg += ",";
                        ssMsg += "\"StudyAttrKind1\":";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyAttrKind1);
                        ssMsg += ",";
                        ssMsg += "\"byStudyLevel\":";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyLevel);
                        ssMsg += "\"StudyAttrKind2\":";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyAttrKind2);
                        ssMsg += "}";
                    }
                    else
                    {
                        ssMsg += "{";
                        ssMsg += "\"StudyColorKind\":";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyColorKind);
                        ssMsg += ",";
                        ssMsg += "\"StudyAttrKind1\":";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyAttrKind1);
                        ssMsg += ",";
                        ssMsg += "\"byStudyLevel\":";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyLevel); //<<",";
                        ssMsg += "\"StudyAttrKind2\":";
                        ssMsg += UINT16(stDT_AWAKEN_DATA.astBagStudyInfo[i].byStudyAttrKind2);
                        ssMsg += "}";
                    }
                }
                else
                {
                    continue;
                }
            }
            ssMsg += "]";
        }
    }
    ssMsg += "},";

    // 装备
    ssMsg += "\"EquipItem\":[";
    DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI stAllEquip = {0};
    poPlayer->GetAllEquip(stAllEquip);

    for(size_t i = 0; i < stAllEquip.wEquipNum; ++i)
    {
        if (0 != i)
        {
            ssMsg += ",{";

            // 取得装备名
            SEquipProp* pstEquipProp = CEquipPropMgr::Instance()->GetProp(stAllEquip.astEquipInfo[i].wKindID);
            if (pstEquipProp)
            {
                ssMsg += "\"EquipName\":\"";
                ssMsg += pstEquipProp->szName;
                ssMsg += "\",";
            }
            else
            {
                ssMsg += "\"EquipName\":";
                ssMsg += "\"unknown equip name\"";
                ssMsg +=  ",";
            }

            ssMsg += "\"KindID\":";
            ssMsg += stAllEquip.astEquipInfo[i].wKindID;
            ssMsg += ",";
            ssMsg += "\"Idx\":";
            ssMsg += stAllEquip.astEquipInfo[i].wIdx;
            ssMsg += ",";
            ssMsg += "\"DressPos\":";
            ssMsg += UINT16(stAllEquip.astEquipInfo[i].byDressPos);
            ssMsg += ",";
            ssMsg += "\"Level\":";
            ssMsg += stAllEquip.astEquipInfo[i].wLevel;
            ssMsg += ",";
            ssMsg += "\"DressLevel\":";
            ssMsg += stAllEquip.astEquipInfo[i].wDressLevel;

            ssMsg += "}";
        }
        else
        {
            ssMsg += "{";

            // 取得装备名
            SEquipProp* pstEquipProp = CEquipPropMgr::Instance()->GetProp(stAllEquip.astEquipInfo[i].wKindID);
            if (pstEquipProp)
            {
                ssMsg += "\"EquipName\":\"";
                ssMsg += pstEquipProp->szName;
                ssMsg += "\",";
            }
            else
            {
                ssMsg += "\"EquipName\":";
                ssMsg += "\"unknown equip name\"";
                ssMsg +=  ",";
            }

            ssMsg += "\"KindID\":";
            ssMsg += stAllEquip.astEquipInfo[i].wKindID;
            ssMsg += ",";
            ssMsg += "\"Idx\":";
            ssMsg += stAllEquip.astEquipInfo[i].wIdx;
            ssMsg += ",";
            ssMsg += "\"DressPos\":";
            ssMsg += UINT16(stAllEquip.astEquipInfo[i].byDressPos);
            ssMsg += ",";
            ssMsg += "\"Level\":";
            ssMsg += stAllEquip.astEquipInfo[i].wLevel;
            ssMsg += ",";
            ssMsg += "\"DressLevel\":";
            ssMsg += stAllEquip.astEquipInfo[i].wDressLevel;

            ssMsg += "}";
        }
    }
    ssMsg += "],";


    // 物品
    ssMsg += "\"GoodsItem\":[";
    DT_ITEM_DATA_LIST_CLI2 stBagItemInfo = {0};
    poPlayer->GetBag().GetBagItem(stBagItemInfo);

    bool firstElem = TRUE;

    for(size_t i = 0; i < stBagItemInfo.byItemNum; ++i)
    {
        if (EIK_GOODS == stBagItemInfo.astItemList[i].byItemType)
        {
            if (!firstElem)
            {
                ssMsg += ",{";

                // 取得物品名
                SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(stBagItemInfo.astItemList[i].stItemInfo.stGoods.wKindID);
                if (pstGoodsProp)
                {
                    ssMsg += "\"GoodsName\":\"";
                    ssMsg += pstGoodsProp->szName;
                    ssMsg += "\",";
                }
                else
                {
                    ssMsg += "\"GoodsName\":\"";
                    ssMsg += "\"unknown goods name\"";
                    ssMsg += ",";
                }

                ssMsg += "\"KindID\":";
                ssMsg += stBagItemInfo.astItemList[i].stItemInfo.stGoods.wKindID;
                ssMsg += ",";
                ssMsg += "\"GoodsMainKindID\":";
                ssMsg += UINT16(stBagItemInfo.astItemList[i].stItemInfo.stGoods.byGoodsMainKindID);
                ssMsg += ",";
                ssMsg += "\"UseLevel\":";
                ssMsg += stBagItemInfo.astItemList[i].stItemInfo.stGoods.wUseLevel;
                ssMsg += ",";
                ssMsg += "\"PileCount\":";
                ssMsg += stBagItemInfo.astItemList[i].stItemInfo.stGoods.wPileCount;
                ssMsg += "}";
            }
            else
            {
                ssMsg += "{";

                // 取得物品名
                SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(stBagItemInfo.astItemList[i].stItemInfo.stGoods.wKindID);
                if (pstGoodsProp)
                {
                    ssMsg += "\"GoodsName\":\"";
                    ssMsg += pstGoodsProp->szName;
                    ssMsg += "\",";
                }
                else
                {
                    ssMsg += "\"GoodsName\":\"";
                    ssMsg += "\"unknown goods name\"";
                    ssMsg += ",";
                }

                ssMsg += "\"KindID\":";
                ssMsg += stBagItemInfo.astItemList[i].stItemInfo.stGoods.wKindID;
                ssMsg += ",";
                ssMsg += "\"GoodsMainKindID\":";
                ssMsg += UINT16(stBagItemInfo.astItemList[i].stItemInfo.stGoods.byGoodsMainKindID);
                ssMsg += ",";
                ssMsg += "\"UseLevel\":";
                ssMsg += stBagItemInfo.astItemList[i].stItemInfo.stGoods.wUseLevel;
                ssMsg += ",";
                ssMsg += "\"PileCount\":";
                ssMsg += stBagItemInfo.astItemList[i].stItemInfo.stGoods.wPileCount;
                ssMsg += ",";
                ssMsg += "}";

                firstElem = FALSE;
            }
        }
        else
        {
            continue;
        }
    }

    ssMsg += "]";

	vector<string> vecData;
	vecData.push_back(ssMsg.str());


    string strRetInfo = "";
	strRetInfo = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &vecData);

    Rspone(strRetInfo);
}

string GetACTEItemKindTypeName(UINT8 byKindType)
{
    string strKindTypeName;
    switch(byKindType)
    {
    case ACTE_GOOD:
        strKindTypeName = SDGBK2UTF8("物品");
        break;
    case ACTE_HERO:
        strKindTypeName = SDGBK2UTF8("武将");
        break;
    case ACTE_COIN:
        strKindTypeName = SDGBK2UTF8("铜钱");
        break;
    case ACTE_GOLD:
        strKindTypeName = SDGBK2UTF8("元宝");
        break;
    case ACTE_PHYSTRENGTH:
        strKindTypeName = SDGBK2UTF8("体力");
        break;
    case ACTE_SCIENCE:
        strKindTypeName = SDGBK2UTF8("器魂");
        break;
    case ACTE_STORY:
        strKindTypeName = SDGBK2UTF8("阅历");
        break;
    case ACTE_BLUEGAS:
        strKindTypeName = SDGBK2UTF8("蓝气");
        break;
    case ACTE_PRUPLEGAS:
        strKindTypeName = SDGBK2UTF8("紫气");
        break;
    case ACTE_JINGJIE:
        strKindTypeName = SDGBK2UTF8("境界");
        break;

    default:
        strKindTypeName = "unknown activity encourage item";
        break;
    }

    return strKindTypeName;
}

string GetACTEItemKindValueName(UINT8 byKindType, UINT16 wKindID)
{
    string strKindValueName;

    switch(byKindType)
    {
    case ACTE_GOOD:
    {
        UINT8 byDetaildEIK = CItemMgr::Instance()->GetItemKindBYKindID(wKindID);

        if (EIK_EQUIP == byDetaildEIK)
        {
            SEquipProp* pstEquipProp = CEquipPropMgr::Instance()->GetProp(wKindID);
            if (pstEquipProp)
            {
                strKindValueName = pstEquipProp->szName;
            }
            else
            {
                strKindValueName = "unknown equip";
            }
        }
        else if (EIK_GOODS == byDetaildEIK)
        {
            SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wKindID);
            if (pstGoodsProp)
            {
                strKindValueName = pstGoodsProp->szName;
            }
            else
            {
                strKindValueName = "unknown goods";
            }
        }
        else
        {
            strKindValueName = "unknown ACTE_GOOD item ";
        }
    }
    break;

    case ACTE_HERO:
    {
        SHeroProp* pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(wKindID);

        if (pstHeroProp)
        {
            strKindValueName = pstHeroProp->szName;
        }
        else
        {
            strKindValueName = "unknown hero";
        }
    }
    break;
    default:
        strKindValueName = GetACTEItemKindTypeName(byKindType);
        break;
    }

    return strKindValueName;
}

VOID CRCClient::SendPayRet(string strOrderID, UINT16 wErrCode, CPlayer* poPlayer, UINT32 dwExchangeGold)
{
    string strRet;
    switch(wErrCode)
    {
        //成功
    case 0:
        strRet = "{\"status\":\"success\"}";
        if(poPlayer)
        {
            NtfPlayer(poPlayer, dwExchangeGold);
        }
        break;
        //没有该订单
    case 1:
        strRet = "{\"status\":\"error_2\"}";
        break;
        //已经处理
    case 2:
        strRet = "{\"status\":\"success\"}";
        break;
        //db更新失败
    case 3:
        strRet = "{\"status\":\"error_3\"}";
        break;
        //myrecharge重复订单
    case 5:
        strRet = "{\"status\":\"error_5\"}";
        break;
        //玩家不存在
    case 13:
        strRet = "{\"status\":\"error_13\"}";
        break;
    default:
        strRet = "{\"status\":\"error_11\"}";
        break;
    }

    Rspone(strRet, strOrderID);
}

VOID CRCClient::Rspone(string strRet, string strOrderID)
{
    CSDDateTime oNow = SDNow();
    CHAR szHead[1024] = {0};
    sprintf(szHead, "HTTP/1.1 200 OK\r\n"\
            "Server: HeiHuo HttpServer\r\n"\
            "Date: %s\r\n"\
            "Content-Type: text/html;Charset=utf-8\r\n"\
            "Content-Length: %d\r\n\r\n",//头和正文需有空行
            (SDTimeToString(oNow) + " GMT").c_str(),
            strRet.length()
           );
    string strRsp = szHead + strRet;
    if(Send(strRsp.c_str(), strRsp.length()))
    {
        //避免日志过大
        if(!strOrderID.empty())
        {
            DBG_INFO(_SDT("[%s:%d]: SendPayRet OrderID[%s], Ret: %s, OK\n"), MSG_MARK, strOrderID.c_str(), strRet.c_str());
        }
    }
    else
    {
        //避免日志过大
        if(!strOrderID.empty())
        {
            DBG_INFO(_SDT("[%s:%d]: SendPayRet OrderID[%s], Ret: %s, Faild\n"), MSG_MARK, strOrderID.c_str(), strRet.c_str());
        }
    }
    //休眠，引起线程切换
    SDSleep(0);
    //断开网络连接
    Disconnect();
}

VOID CRCClient::NtfPlayer(CPlayer* poPlayer, UINT32 dwExchangeGold)
{
    if(NULL == poPlayer)
    {
        return;
    }

    //玩家已经正常处理成功，通知玩家
    PKT_CLIGS_EXCHANGE_RESULT stAck;
    stAck.wErrCode = ERR_EXCHANGE_RESULT_ID::ID_SUCCESS;
    stAck.qwGold = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    stAck.dwExchangeGold = dwExchangeGold;
    stAck.byVipLevel = poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel;
    stAck.dwTotalExchangeGold = poPlayer->GetDT_PLAYER_BASE_DATA().dwTotalAmount;

    //检测玩家是否在线,现在检测是否在线好像存在问题
    if (poPlayer->CkLogin())
    {
        if(poPlayer->SendMsg( (const CHAR*)(&stAck), CLIGS_EXCHANGE_RESULT ))
        {
            poPlayer->GetDT_EXCHANGE_RESULT_LOG().byNtfFlag = 0;
            poPlayer->GetDT_EXCHANGE_RESULT_LOG().dwExchangeGold = 0;
        }
        else
        {
            poPlayer->GetDT_EXCHANGE_RESULT_LOG().byNtfFlag = 1;
            poPlayer->GetDT_EXCHANGE_RESULT_LOG().dwExchangeGold = dwExchangeGold;
        }
    }
    else
    {
        poPlayer->SendMsg( (const CHAR*)(&stAck), CLIGS_EXCHANGE_RESULT );
        poPlayer->GetDT_EXCHANGE_RESULT_LOG().byNtfFlag = 1;
        poPlayer->GetDT_EXCHANGE_RESULT_LOG().dwExchangeGold = dwExchangeGold;
    }
}




//TRUE则说明不是重复请求，FALSE为重复请求
BOOL CRCClientMgr::AddGmSerial(UINT64 qwSerialID)
{
    UINT64 qwCurTime = SDTimeSecs();
    C6464MapItr itr = m_mapGmSerial2Time.find(qwSerialID);
    if(itr != m_mapGmSerial2Time.end())
    {
        //3天后才不算重复请求
        if(qwCurTime - itr->second  < 3600 * 24 * 3)
        {
            return FALSE;
        }
    }
    m_mapGmSerial2Time[qwSerialID] = qwCurTime;

    return TRUE;
}

