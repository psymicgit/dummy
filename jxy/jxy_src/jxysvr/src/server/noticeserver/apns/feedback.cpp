#include "feedback.h"
#include "logic/basedbccmd.h"
#include "common/server/utility.h"
#include <framework/nsapplication.h>




extern CSDApplication* SDGetApp();

CFeedBack::CFeedBack()
{
    m_nProtelLen = 0;
    memset(m_szProtelData, 0x00, sizeof(m_szProtelData));
}

CFeedBack::~CFeedBack()
{

}

void CFeedBack::Read()
{
	char	 v_szData[MAX_FEED_BACK_DATA] = {0};
    while(!m_bShutDown)
    {
        memset(v_szData, 0x00, sizeof(v_szData));
        int ret = m_oSSLComm.ssl_read(v_szData, MAX_FEED_BACK_DATA);
        if (ret <= 0)
        {
			m_nProtelLen = 0;
			memset(m_szProtelData, 0x00, sizeof(m_szProtelData));
			SDSleep(5000);
            continue;
        }
        HandleData(v_szData, ret);
    }
}

void CFeedBack::Write()
{

}

void CFeedBack::HandleData(const char * szData, const int nLen)
{
    memcpy(m_szProtelData + m_nProtelLen, szData, nLen);
    m_nProtelLen += nLen;

    int			v_nLen = 0;
    pResponce	v_pResponce = NULL;
    while (1)
    {
        if (v_nLen <= m_nProtelLen)
        {
            v_pResponce = (pResponce)(m_szProtelData + v_nLen);
            v_nLen += sizeof(CResponce);
            CBaseDBCMD* poCMD = ((CNSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_SAVE_FEED_BACK);
            if(NULL == poCMD)
            {
                continue;
            }
            poCMD->SetUserData((VOID*)v_pResponce, sizeof(CResponce), 0);
            ((CNSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
        }
        else
        {
            m_nProtelLen -= v_nLen;
            memmove(m_szProtelData, m_szProtelData + v_nLen, m_nProtelLen);
        }
    }
}