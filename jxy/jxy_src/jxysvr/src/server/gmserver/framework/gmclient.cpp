#include "gmclient.h"
#include "gmapplication.h"
#include "net/cli/cliprocessor.h"
#include <sdutil.h>
#include <common/server/utility.h>
#include <protocol/server/protocligt.h>
#include <net/cli/clipktbuilder.h>
#include <json/json.h>
#include "logic/qdes.h"
#include <algorithm/sdmd5.h>



/*********************************************************************************************
 *函   数   名：随机字符串
 *功        能：
 *输        入：data - 整数(int)类型
 *
 *输        出：
 *返        回：四字符字节指针(字符类型）
 *修 改  记 录：NONE
 *********************************************************************************************/
int Rand_Str(char * str, int len)
{
    int i;
    if (str == NULL)
    {
        return 0;
    }
    srand((UINT32)(time(NULL) + rand())); //初始化随机数的种子
    for (i = 0; i < len; i++)
    {
        str[i] = rand() % 94 + 48; //控制得到的随机数为可显示字符
    }
    return 0;
}


CGMClient::CGMClient():
    m_dwCliSessionID(0),
    m_poConnection(NULL)
{
    m_byLoginFlag = CLIENT_STATE_UNAUTHED;
    m_poPacketProcessor = NULL;
    GetRandStr();
}

CGMClient::~CGMClient()
{

}


VOID CGMClient::SetConnection(ISDConnection* poConnection)
{
    m_poConnection = poConnection;
}

bool CGMClient::OnError(INT32 nSDError, INT32 nSysError)
{
    USR_INFO(_SDT("[%s:%d]: Connection error: IP = %s, Port = %d, SDError: %d, SysError: %d\n"),
             MSG_MARK, m_poConnection->GetRemoteIPStr(), m_poConnection->GetRemotePort(), nSDError, nSysError);

    return true;
}

VOID CGMClient::OnEstablish()
{
    ((CGMApplication*)SDGetApp())->GetClientMgr().AddCliSession(this);

}


VOID CGMClient::OnRecv(const CHAR* pBuf, UINT32 nLen)
{
    BOOL		bProccess = TRUE;
    m_qwLastTime = SDTimeSecs();
    if(m_poPacketProcessor)
    {
        m_poPacketProcessor->ProcessPacket(this, NULL, pBuf, nLen);
    }
}


VOID CGMClient::OnTerminate()
{
    //printf("a new client OnTerminate \n");
    //((CGMApplication*)SDGetApp())->GetClientMgr().ReleaseCliSession(this);
}

VOID CGMClient::Release()
{
    //printf("a new client Release \n");
    ((CGMApplication*)SDGetApp())->GetClientMgr().ReleaseCliSession(this);
}

ISDConnection* CGMClient::GetConnection()
{
    return m_poConnection;
}


VOID CGMClient::SetCliSessionID(UINT32 dwCliSessionID)
{
    m_dwCliSessionID = dwCliSessionID;
}

UINT32 CGMClient::GetCliSessionID()
{
    return m_dwCliSessionID;
}


VOID CGMClient::Disconnect()
{
    if (m_poConnection)
    {
        OnTerminate();
        m_poConnection->Disconnect();
    }
}

BOOL CGMClient::Send(const CHAR* pBuf, INT32 nLen)
{
    if (m_poConnection)
    {
        m_poConnection->Send(pBuf, nLen);
        return TRUE;
    }
    return FALSE;
}

BOOL CGMClient::SendMsg(const CHAR* pData)
{
    return Send(pData, strlen(pData));
}


VOID CGMClient::SetPacketProcessor(CCliProcessor*    pPacketProcessor)
{
    m_poPacketProcessor = pPacketProcessor;
}

char	* CGMClient::GetRandStr()
{
    memset(m_szRandStr, 0x00, sizeof(m_szRandStr));
    char		v_szRand[56] = {0};
    Rand_Str(v_szRand, 16);
    for (int i = 0; i < 16; i++)
    {
        char v_szTemp[16] = {0};
        sprintf(v_szTemp, "%02x", (UINT8)v_szRand[i]);
        strcat(m_szRandStr, v_szTemp);
    }
    return m_szRandStr;
}

BOOL CGMClient::Oauth(char *szUserName, char *ncrypt)
{
    char *	pPwd =   NULL;
    pPwd = ((CGMApplication*)SDGetApp())->GetGmConfig()->GetUserPwd(szUserName);
    if (NULL == pPwd)
    {
        return FALSE;
    }

    //
    UCHAR acInput[256] = {0};
    UCHAR acDigest[16] = {0};
    sprintf((char*)acInput, "%s%s", m_szRandStr, pPwd);
    SDMD5(acDigest, acInput, strlen((char*)acInput));

    char szTemp[32] = {0};
    char szHexDigest[256] = {0};
    for (int i = 0; i < 16; i++)
    {
        sprintf(szTemp, "%02x", (UINT8)acDigest[i]);
        strcat(szHexDigest, szTemp);
    }

    if(0 == memcmp(szHexDigest, ncrypt, strlen(ncrypt)))
    {
        return TRUE;
    }

    return FALSE;
}


CGMClientMgr::CGMClientMgr()
{
    m_poLock = NULL;
    m_mapCliSession.clear();
    m_dwCurClientID = 1;
    m_dwSerialNumber = (UINT32)SDTimeSecs();
}

CGMClientMgr::~CGMClientMgr()
{
    if (NULL != m_poLock)
    {
        SDDelete m_poLock;
        m_poLock = NULL;
    }
}

BOOL CGMClientMgr::Init(UINT32 nMaxClient)
{
    m_poLock = SDNew CSDMutex();
    if(NULL == m_poLock)
    {
        //SYS_CRITICAL("[%s: %d]: Create ISDLock failed!", MSG_MARK);
        UnInit();
        return FALSE;
    }
    m_poCliSessionPool = SDNew CSDObjectPool<CGMClient, CSDMutex>(nMaxClient, nMaxClient);
    if (NULL == m_poCliSessionPool)
    {
        //SYS_CRITICAL("[%s: %d]: CliSession pool init failed!", MSG_MARK);
        UnInit();
        return FALSE;
    }
    return TRUE;
}

VOID CGMClientMgr::UnInit()
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

ISDSession* CGMClientMgr::CreateSession(ISDConnection* poConnection)
{
    CGMClient* poSession = m_poCliSessionPool->Alloc();
    if (NULL == poSession)
    {
        return NULL;
    }
    UINT32 dwClientID = GenerateClientID();
    poSession->SetCliSessionID(dwClientID);
    poSession->SetPacketProcessor(&m_oCliProcessor);
    return poSession;
}

VOID CGMClientMgr::ReleaseCliSession(CGMClient* pSession)
{
    if(pSession)
    {
        EarseCliSerialNumber(pSession);
        m_poCliSessionPool->Free(pSession);
    }
}

UINT32 CGMClientMgr::GenerateClientID()
{
    m_poLock->Lock();
    m_dwCurClientID++;
    m_poLock->Unlock();

    return m_dwCurClientID;
}

UINT32 CGMClientMgr::GetCliSessionCount()
{
    return m_mapCliSession.size();
}

VOID CGMClientMgr::AddCliSession(CGMClient *poSession)
{
    m_mapCliSession[poSession->GetCliSessionID()] = poSession;
    //printf("a new client add map ID: %d address: %0x", poSession->GetCliSessionID(), poSession);
}


CGMClient* CGMClientMgr::FindCliSession(UINT32 dwClientID)
{
    CGMClientMapItr itr = m_mapCliSession.find(dwClientID);
    if(itr == m_mapCliSession.end())
        return NULL;

    return itr->second;
}


VOID CGMClientMgr::ChkSessionOnTimer()
{
    for(CGMClientMapItr itr = m_mapCliSession.begin(); itr != m_mapCliSession.end();)
    {
        CGMClient* poCliSession = itr->second;
        if((poCliSession) && ((SDTimeSecs() - poCliSession->GetLastTime()) > 100)) // 10秒就断开
        {
            EarseCliSerialNumber(poCliSession);
            poCliSession->GetConnection()->Disconnect();
            m_mapCliSession.erase(itr++);
        }
        else
        {
            itr++;
        }
    }
}

UINT32	 CGMClientMgr::SetCliSerialNumber(CGMClient* poSession)
{
    UINT32	dwSerialNumber = 0;
    m_oSerialNumberLock.Lock();
    m_dwSerialNumber++;
    dwSerialNumber = m_dwSerialNumber;
    m_mapCliSerialNumber[dwSerialNumber] = poSession->GetCliSessionID();
    m_oSerialNumberLock.Unlock();

    return dwSerialNumber;
}

CGMClient * CGMClientMgr :: GetCliSerialNumber(UINT32 dwSerialNumber)
{
    UINT32	dwCliSessionID = 0;
    CSerialNumberMapItr itrSerialNumber;
    itrSerialNumber = m_mapCliSerialNumber.find(dwSerialNumber);
    if(itrSerialNumber == m_mapCliSerialNumber.end())
    {
        return NULL;
    }
    dwCliSessionID = itrSerialNumber->second;

    CGMClientMapItr itrClient;
    itrClient = m_mapCliSession.find(dwCliSessionID);
    if (itrClient == m_mapCliSession.end())
    {
        return FALSE;
    }
    return itrClient->second;
}

void	 CGMClientMgr::EarseCliSerialNumber(CGMClient* poSession)
{
    CSerialNumberMapItr itr;
    for(itr = m_mapCliSerialNumber.begin(); itr != m_mapCliSerialNumber.end(); )
    {
        if(itr->second == poSession->GetCliSessionID())
        {
            m_mapCliSerialNumber.erase(itr++);
        }
        else
        {
            itr++;
        }
    }
}
