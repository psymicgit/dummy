#include "jsonopmgr.h"
#include <sdlock.h>
#include <net/recharge/rcclient.h>
#include <framework/gsapi.h>


IMPLEMENT_SINGLETON(CJsonOpMgr);

CJsonOpMgr::CJsonOpMgr()
{
    m_bStop = FALSE;    
}

CJsonOpMgr::~CJsonOpMgr()
{

}

BOOL CJsonOpMgr::Init()
{
    Start();
    return TRUE;
}

VOID CJsonOpMgr::UnInit()
{
    Stop();
    Wait();
}

VOID CJsonOpMgr::Stop()
{
    m_bStop = TRUE;
}


VOID  CJsonOpMgr::ThrdProc()
{ 
    while(!m_bStop)
    {
		if(0 == m_deqJsonParsePlayerData.size())
		{
			SDSleep(100);
			continue;
		}
		CSDLock<CSDMutex> lock(m_oJsonParsePlayerDataMutex);
		SDT_PLAYER_DATA_TRANSID& stData = m_deqJsonParsePlayerData.back();
		SJVALUE_TRANSID stSJVALUE_TRANSID;
		stSJVALUE_TRANSID.dwTransID = stData.dwTransID;
		cJSON* JValue = GetJsonDT_PLAYER_DATA(stData.stDT_PLAYER_DATA);
		if(NULL == JValue)
		{
			m_deqJsonParsePlayerData.pop_back();
			continue;
		}
		CHAR* pszValue = cJSON_Print(JValue);
		if (NULL == pszValue)
		{
			m_deqJsonParsePlayerData.pop_back();
			continue;
		}
		stSJVALUE_TRANSID.JValue = pszValue;
		free(pszValue);
		pszValue = NULL;
		cJSON_Delete(JValue);
		m_deqJsonParsePlayerData.pop_back();
		{
			CSDLock<CSDMutex> lock(m_oParsedPlayerJsonDataMutex);
			m_deqParsedPlayerJsonData.push_front(stSJVALUE_TRANSID);
		}
    }

}

VOID CJsonOpMgr::Run()
{
	CSDLock<CSDMutex> lock(m_oParsedPlayerJsonDataMutex);
	if(0 == m_deqParsedPlayerJsonData.size())
	{
		return;
	}
	SJVALUE_TRANSID stSJVALUE_TRANSID = m_deqParsedPlayerJsonData.back();
	m_deqParsedPlayerJsonData.pop_back();
	CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(stSJVALUE_TRANSID.dwTransID);
	if(NULL == poClinet)
	{
		SYS_CRITICAL(_SDT("[%s:%d]: CJsonOpMgr Run find CRCClient[%d] failed!"), MSG_MARK, stSJVALUE_TRANSID.dwTransID);
		return;
	}

	poClinet->Rspone(stSJVALUE_TRANSID.JValue);	
}

VOID CJsonOpMgr::AddJsonParsePlayerData(const DT_PLAYER_DATA& stDT_PLAYER_DATA, UINT32 dwTransID)
{
	CSDLock<CSDMutex> lock(m_oJsonParsePlayerDataMutex);
	SDT_PLAYER_DATA_TRANSID stData = {0};
	memcpy(&stData.stDT_PLAYER_DATA, &stDT_PLAYER_DATA, sizeof(DT_PLAYER_DATA));
	stData.dwTransID = dwTransID;
	m_deqJsonParsePlayerData.push_front(stData);
}
