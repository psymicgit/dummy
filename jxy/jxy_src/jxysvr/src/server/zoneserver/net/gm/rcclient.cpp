#include "rcclient.h"
#include <sdutil.h>
#include <common/server/utility.h>
#include <algorithm/sdmd5.h>
#include <protocol/server/protocommondata.h>
#include <framework/zsapplication.h>
#include <common/client/commondef.h>
#include <logic/changepwdcmd.h>
#include <logic/forbidlogin.h>
#include <logic/forbidlogin.h>
#include <logic/forbidregistered.h>
#include <logic/setaccounttype.h>
#include <logic/startserver.h>
#include <common/client/gmdef.h>


const CHAR*		PSOURCE		= "source";
const CHAR*		PTRADE_NO	= "trade_no";
const CHAR*		PAMOUNT		= "amount";
const CHAR*		PPARTNER	= "partner";
const CHAR*		PSIGN		= "sign";
const CHAR*		PTBKEY		= "&key=FxAKhtwg0EatlbgZbujg4GCnBTRU#VNo";


const CHAR*		PSERIALNUMBER		= "serialnumber";
const CHAR*		PACTION			= "action";
const CHAR*		PPLAYERID		= "playerid";
const CHAR*		PPARAM1			= "param1";
const CHAR*		PPARAM2			= "param2";
const CHAR*		PPARAM3			= "param3";
const CHAR*		PPARAM4			= "param4";
const CHAR*		PPARAM5			= "param5";
const CHAR*		PGMKEY			= "&key=0z#z#b#094kls#040jkas892#z#z#b#0";


const CHAR*		PORDERID		= "orderid";
const CHAR*		PRMB			= "rmb";
const CHAR*		PGIVENGOLD	= "givengold";
const CHAR*		PMYKEY		= "&key=MYAKhtwg0EatlbgZbujg4GCnBTRU#VMY";
const CHAR*		PCOMMENT		= "comment";
const CHAR*		PZONEID		= "zoneid";



/** 16进制字符表 */
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
    ((CZSApplication*)SDGetApp())->GetRCClientMgr()->AddCliSession(this);

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
    DBG_INFO(_SDT("[%s:%d]: OnRecv Data: %s\n"), MSG_MARK, pBuf);

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
    }

    string strType = strTmp1.substr(nPos1 + 1, nPosTmp - nPos1 - 1);
    if(0 == strType.compare("gm"))
    {
        DealHttpGmReq(pBuf, nLen);
    }
    else
    {
        CHAR szRet[128] = {0};
        sprintf(szRet, "{\"status\":\"error_1\"}");
        Rspone(szRet);
    }
}

VOID CRCClient::OnTerminate()
{
    m_poConnection = NULL;
    //((CZSApplication*)SDGetApp())->GetRCClientMgr()->RemoveCliSession(this);
}

VOID CRCClient::Release()
{
    ((CZSApplication*)SDGetApp())->GetRCClientMgr()->ReleaseCliSession(this);
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

    CRCClientMapItr itr = m_mapCliSession.find(poSession->GetCliSessionID());
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
    CRCClientMapItr itr = m_mapCliSession.find(dwClientID);
    if(itr == m_mapCliSession.end())
        return NULL;

    return itr->second;
}


VOID CRCClientMgr::ChkSessionOnTimer()
{
    for(CRCClientMapItr itr = m_mapCliSession.begin(); itr != m_mapCliSession.end();)
    {
        CRCClient* poCliSession = itr->second;
        if((poCliSession) && ((SDTimeSecs() - poCliSession->GetLastTime()) > 30)) // 30秒就断开
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

VOID CRCClient::DealHttpGmReq(const CHAR* pBuf, UINT32 nLen)
{
    if(NULL == pBuf)
    {
        DBG_INFO(_SDT("[%s:%d]: recv http gm req data is NULL!\n"), MSG_MARK);
        return;
    }
    DBG_INFO(_SDT("[%s:%d]: recv http gm req, data: %s\n"), MSG_MARK, pBuf);

    CHAR* pszBuff = SDNew CHAR[nLen * 3 + 1];
    memset(pszBuff, 0, sizeof(pszBuff));
    url_decode(pBuf, nLen, pszBuff, nLen * 3);

    string strTmp1 = pszBuff;
    SDDelete pszBuff;
    pszBuff = NULL;
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
            (mapField[PSIGN].empty())
      )
    {
        //无效
        DBG_INFO(_SDT("[%s:%d]: http data invalid 4, data: %s\n"), MSG_MARK, pBuf);
        Rspone(strRet, "");
        return;
    }

    UINT8 abyInput[256] = {0};
    UINT8 abyDigest[16] = {0};
    sprintf((CHAR*)abyInput, "%s%s", strSrc.c_str(), PGMKEY);
    SDMD5(abyDigest, abyInput, strlen((CHAR*)abyInput));
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

    UINT32	dwSerialNumber = SDAtou(mapField[PSERIALNUMBER].c_str());
    UINT8	byAction		= (UINT8)(SDAtou(mapField[PACTION].c_str()));
    UINT32	dwAreaID		= 0;
    UINT32	dwPlayerID	= SDAtou(mapField[PPLAYERID].c_str());
    UINT32	dwParam1		= SDAtou(mapField[PPARAM1].c_str());
    UINT64	qwParam2		= SDAtou64(mapField[PPARAM2].c_str());
    UINT64	qwParam3		= SDAtou64(mapField[PPARAM3].c_str());
    tstring strParam4		= mapField[PPARAM4].c_str();
    tstring strParam5		= mapField[PPARAM5].c_str();
 
    return;
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
        DBG_INFO(_SDT("[%s:%d]: SendPayRet OrderID[%s], Ret: %s, OK\n"), MSG_MARK, strOrderID.c_str(), strRet.c_str());
    }
    else
    {
        DBG_INFO(_SDT("[%s:%d]: SendPayRet OrderID[%s], Ret: %s, Faild\n"), MSG_MARK, strOrderID.c_str(), strRet.c_str());
    }
    //休眠，引起线程切换
    SDSleep(0);
    //断开网络连接
    Disconnect();
}


