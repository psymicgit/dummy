#include "ksclient.h"
#include "nsapplication.h"
#include "net/cli/cliprocessor.h"
#include <sdutil.h>
#include <common/server/utility.h>
#include <protocol/server/protocligt.h>
#include <net/cli/clipktbuilder.h>
#include <json/json.h>



/*********************************************************************************************
 *函   数   名：随机字符串
 *功        能：
 *输        入：data - 整数(int)类型
 *
 *输        出：
 *返        回：四字符字节指针(字符类型）
 *修 改  记 录：NONE
 *********************************************************************************************/
int Rand_Str(char * str, int nLen)
{
    int i;
    if (str == NULL)
    {
        return 0;
    }
    srand((UINT32)(time(NULL) + rand())); //初始化随机数的种子
    for (i = 0; i < nLen; i++)
    {
        str[i] = rand() % 94 + 32; //控制得到的随机数为可显示字符
    }
    return 0;
}


CKSClient::CKSClient():
    m_dwCliSessionID(0),
    m_poConnection(NULL)
{
	m_byLoginFlag = CLIENT_STATE_UNAUTHED;
    m_poPacketProcessor = NULL;
}

CKSClient::~CKSClient()
{

}


VOID CKSClient::SetConnection(ISDConnection* poConnection)
{
    m_poConnection = poConnection;
}

bool CKSClient::OnError(INT32 nSDError, INT32 nSysError)
{
    USR_INFO(_SDT("[%s:%d]: Connection error: IP = %s, Port = %d, SDError: %d, SysError: %d\n"),
                 MSG_MARK, m_poConnection->GetRemoteIPStr(), m_poConnection->GetRemotePort(), nSDError, nSysError);

    return true;
}

VOID CKSClient::OnEstablish()
{

}


VOID CKSClient::OnRecv(const CHAR* pBuf, UINT32 nLen)
{
    BOOL		bProccess = TRUE;
    m_qwLastTime = SDTimeSecs();
    if(m_poPacketProcessor)
    {
		m_poPacketProcessor->ProcessPacket(this, NULL, pBuf, nLen);
    }
}


VOID CKSClient::OnTerminate()
{
    //((CNSApplication*)SDGetApp())->GetClientMgr().ReleaseCliSession(this);
}

VOID CKSClient::Release()
{
    ((CNSApplication*)SDGetApp())->GetClientMgr().ReleaseCliSession(this);
}

ISDConnection* CKSClient::GetConnection()
{
    return m_poConnection;
}


VOID CKSClient::SetCliSessionID(UINT32 dwCliSessionID)
{
    m_dwCliSessionID = dwCliSessionID;
}

UINT32 CKSClient::GetCliSessionID()
{
    return m_dwCliSessionID;
}


VOID CKSClient::Disconnect()
{
    if (m_poConnection)
    {
        OnTerminate();
        m_poConnection->Disconnect();
    }
}

BOOL CKSClient::Send(const CHAR* pBuf, INT32 nLen)
{
    if (m_poConnection)
    {
        m_poConnection->Send(pBuf, nLen);
        return TRUE;
    }
    return FALSE;
}

BOOL CKSClient::SendMsg(const CHAR* pData)
{
    return Send(pData, strlen(pData));
}


VOID CKSClient::SetPacketProcessor(CCliProcessor*    pPacketProcessor)
{
    m_poPacketProcessor = pPacketProcessor;
}

char	* CKSClient::GetRandStr()
{
	memset(m_szRandStr, 0x00, sizeof(m_szRandStr));
	Rand_Str(m_szRandStr, sizeof(m_szRandStr));
	return m_szRandStr;
}

BOOL CKSClient::Oauth(char *szUserName, char *ncrypt)
{
	unsigned char key8[9];
	memset(key8, 0x00, 9);

	//char *	pKey =   NULL;
	//pKey = ((CNSApplication*)SDGetApp())->GetKSConfig()->GetUserPwd(szUserName);
	//if (NULL == pKey)
	//{
	//	return FALSE;
	//}
	///*拷贝密码*/
	//memcpy(key8, pKey, 8);

	///*保存明文*/
	//char s_ncrypt[9];
	///*保存密文*/
	//char d_ncrypt[9];
	///*密文清零*/
	//memset(d_ncrypt, 0, 9);
	///*明文清零*/
	//memset(s_ncrypt, 0, 9);
	///*拷贝密文*/
	//memcpy(d_ncrypt, ncrypt, 8);
	///*解密密文*/
	//_QDES((char*) key8, (char*) d_ncrypt, (char*) s_ncrypt);

	//if(0 == memcmp(m_szRandStr, s_ncrypt, strlen(m_szRandStr)))
	//{
	//	return TRUE;
	//}
	return FALSE;
}


CKSClientMgr::CKSClientMgr()
{
    m_poLock = NULL;
    m_mapCliSession.clear();
    m_dwCurClientID = 1;
	m_dwSerialNumber = 1;
}

CKSClientMgr::~CKSClientMgr()
{
    if (NULL != m_poLock)
    {
        SDDelete m_poLock;
        m_poLock = NULL;
    }
}

BOOL CKSClientMgr::Init(UINT32 nMaxClient)
{
    m_poLock = SDNew CSDMutex();
    if(NULL == m_poLock)
    {
        //SYS_CRITICAL("[%s: %d]: Create ISDLock failed!", MSG_MARK);
        UnInit();
        return FALSE;
    }
    m_poCliSessionPool = SDNew CSDObjectPool<CKSClient, CSDMutex>(nMaxClient, nMaxClient);
    if (NULL == m_poCliSessionPool)
    {
        //SYS_CRITICAL("[%s: %d]: CliSession pool init failed!", MSG_MARK);
        UnInit();
        return FALSE;
    }
    return TRUE;
}

VOID CKSClientMgr::UnInit()
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

ISDSession* CKSClientMgr::CreateSession(ISDConnection* poConnection)
{
    CKSClient* poSession = m_poCliSessionPool->Alloc();
    if (NULL == poSession)
    {
        return NULL;
    }
    UINT32 dwClientID = GenerateClientID();
    poSession->SetCliSessionID(dwClientID);
    poSession->SetPacketProcessor(&m_oCliProcessor);
    return poSession;
}

VOID CKSClientMgr::ReleaseCliSession(CKSClient* pSession)
{
    if(pSession)
    {
		EarseCliSerialNumber(pSession);
        m_poCliSessionPool->Free(pSession);
    }
}

UINT32 CKSClientMgr::GenerateClientID()
{
    m_poLock->Lock();
    m_dwCurClientID++;
    m_poLock->Unlock();

    return m_dwCurClientID;
}

UINT32 CKSClientMgr::GetCliSessionCount()
{
    return m_mapCliSession.size();
}

VOID CKSClientMgr::AddCliSession(CKSClient *poSession)
{
    m_mapCliSession[poSession->GetCliSessionID()] = poSession;
}


CKSClient* CKSClientMgr::FindCliSession(UINT32 dwClientID)
{
    CKSClientMapItr itr = m_mapCliSession.find(dwClientID);
    if(itr == m_mapCliSession.end())
        return NULL;

    return itr->second;
}


VOID CKSClientMgr::ChkSessionOnTimer()
{
    for(CKSClientMapItr itr = m_mapCliSession.begin(); itr != m_mapCliSession.end();)
    {
        CKSClient* poCliSession = itr->second;
        if((poCliSession) && ((SDTimeSecs() - poCliSession->GetLastTime()) > 10)) // 10秒就断开
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

UINT32	 CKSClientMgr::SetCliSerialNumber(CKSClient* poSession)
{
	m_dwSerialNumber++;
	m_mapCliSerialNumber[m_dwSerialNumber] = poSession;
	return m_dwSerialNumber;
}

CKSClient * CKSClientMgr :: GetCliSerialNumber(UINT32 dwSerialNumber)
{
	CKSClientMapItr itr = m_mapCliSerialNumber.find(dwSerialNumber);
	if(itr == m_mapCliSerialNumber.end())
		return NULL;
	CKSClient *poKSClient =  itr->second;
	m_mapCliSerialNumber.erase(itr);
	return poKSClient;
}

void	 CKSClientMgr::EarseCliSerialNumber(CKSClient* poSession)
{
	for(CKSClientMapItr itr = m_mapCliSerialNumber.begin(); itr != m_mapCliSerialNumber.end();)
	{
		if(itr->second == poSession)
		{
			m_mapCliSerialNumber.erase(itr++);
		}
		else
		{
			itr++;
		}
	}
}
