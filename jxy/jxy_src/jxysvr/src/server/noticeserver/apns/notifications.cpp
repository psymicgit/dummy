#include "notifications.h"
#include "common/server/utility.h"
#include <framework/nsapplication.h>

#define MAX_NOTIFY_NODE_NUM 100000

extern CSDApplication* SDGetApp();


CNotifications::CNotifications()
{
    m_dwIdentifier = 0;
}

CNotifications::~CNotifications()
{

}

bool CNotifications::SSL_PushNotice(const char *pToken, const char *pMsg, const SPlayerNotifyInfo &stNoticeInfo)
{
    int			v_len = 0;
    char			v_szData[2048] = {0};
    pPUSHDATA	v_pPUSHDATA = (pPUSHDATA)v_szData;
    v_pPUSHDATA->Init();
    v_pPUSHDATA->SetToken(pToken);
    v_len	= v_pPUSHDATA->SetPayLoad(pMsg, "default", 1, stNoticeInfo.dwPlayerID, stNoticeInfo.byZoneID, stNoticeInfo.byCareerID, stNoticeInfo.byAuthType, stNoticeInfo.byAction);
    return m_oSSLComm.ssl_write(v_szData, v_len) ? true : false;
}

bool CNotifications::SSL_PushNoticeEx(const char *pToken, const char *pMsg, const SPlayerNotifyInfo &stNoticeInfo)
{
    //十六进制转ASCII
    char szToken[256] = {0};
    int  nToken = HexToAscii((unsigned char*)szToken, (unsigned char*)pToken, MAX_TOKEN_NUM);
    if(-1 == nToken)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: 进六进制解码出错。"), MSG_MARK, stNoticeInfo.dwPlayerID);
        return false;
    }

    pNOTIFY_NODE	v_pNotifyNode = m_oNotifyNodePool.Alloc();
    if (NULL == v_pNotifyNode)
    {
        return false;
    }
    v_pNotifyNode->Init();
    pPUSHDATAEX v_pPUSHDATAEX = (pPUSHDATAEX)v_pNotifyNode->szBuf;
    v_pPUSHDATAEX->Init();
    v_pPUSHDATAEX->SetToken(szToken);
    v_pPUSHDATAEX->SetIdentifier(++m_dwIdentifier);
    v_pPUSHDATAEX->SetExpiry(UINT32(time(NULL)) + 86400);
    v_pNotifyNode->nLen	= v_pPUSHDATAEX->SetPayLoad(pMsg, "default", 1, stNoticeInfo.dwPlayerID, stNoticeInfo.byZoneID, stNoticeInfo.byCareerID, stNoticeInfo.byAuthType, stNoticeInfo.byAction);

    m_oMutex.Lock();
    //检测队列个数,如果超过个数的话，把前面清空掉一些。
    if (m_oNotifyNodeList.size() > MAX_NOTIFY_NODE_NUM)
    {
		pNOTIFY_NODE	v_pTemp = NULL;
        UINT32 nLoop = 1000;
        while(--nLoop > 0)
        {
            v_pTemp = m_oNotifyNodeList.front();
            m_oNotifyNodeList.pop_front();
            if (NULL != v_pTemp)
            {
                m_oNotifyNodePool.Free(v_pTemp);
            }
            v_pTemp = NULL;
        }
    }
    m_oNotifyNodeList.push_back(v_pNotifyNode);
    m_oMutex.Unlock();
    m_oCondition.Signal();

    return true;
}


void CNotifications::Read()
{
    while(!m_bShutDown)
    {
        //有应答
        char	  v_szData[2048] = {0};
        memset(v_szData, 0x00, sizeof(v_szData));
        int ret = m_oSSLComm.ssl_read(v_szData, (int)sizeof(v_szData));
        if (ret <= 0)
        {
            //读错误，可能存在多种情况，一种是网络未连接，第二是网络无事件
            SDSleep(5000);
            continue;
        }
        pPUSHDATAEXRESPONCE v_pResponce = (pPUSHDATAEXRESPONCE)v_szData;
        if (NULL == v_pResponce)
        {
            continue;
        }
        SYS_CRITICAL(_SDT("[%s:%d] SSL_read Responce (ret = %d) (Status = %s) (Identifier = %u)\n"), MSG_MARK, ret, ERR_RESPONCE::GetErrorInfo(v_pResponce->byStatus), v_pResponce->GetIdentifier());
    }
}

void CNotifications::Write()
{
    pNOTIFY_NODE	v_pNotifyNode = NULL;
    while (!m_bShutDown)
    {
        m_oMutex.Lock();
        //如果等待队列为0并且不销毁线程池，则处于阻塞状态
        while (!m_oNotifyNodeList.size() && !m_bShutDown)
        {
            m_oCondition.Wait(m_oMutex);
        }

        if (m_bShutDown)
        {
            m_oMutex.Unlock();
            break;
        }
        v_pNotifyNode = m_oNotifyNodeList.front();
        m_oNotifyNodeList.pop_front();
        m_oMutex.Unlock();


        if (NULL == v_pNotifyNode)
        {
            continue;
        }
        int ret = m_oSSLComm.ssl_write((const char*)v_pNotifyNode->szBuf, v_pNotifyNode->nLen);
        if (ret <= 0)
        {
            SYS_CRITICAL(_SDT("[%s:%d] SSL_write (ret = %d) \n"), MSG_MARK, ret);
        }
        else
        {
            SYS_CRITICAL(_SDT("[%s:%d] SSL_write (ret = %d) \n"), MSG_MARK, ret);
        }
        if (NULL != v_pNotifyNode)
        {
            m_oNotifyNodePool.Free(v_pNotifyNode);
            v_pNotifyNode = NULL;
        }
    }
}