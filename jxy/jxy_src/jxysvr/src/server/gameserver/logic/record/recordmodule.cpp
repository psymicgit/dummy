#include "recordmodule.h"
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <sdstring.h>
#include "sdtime.h"
using namespace SGDP;

CRecordModule::CRecordModule(ISDDBSession* pDBRecordSession)
{
	m_pDBRecordSession = pDBRecordSession;
	m_dwNextID = 1;
	m_strTableName = "";
}

CRecordModule::~CRecordModule()
{

}

BOOL CRecordModule::AddData( const string &strInfo )
{
	CSDLock<CSDMutex> lock(m_mutex);
	if( m_deqData.size()>ms_nMaxDataNum )
	{
		SYS_CRITICAL(_SDT("[%s: %d]:AddData more large!"), MSG_MARK);
		return FALSE;
	}
	m_deqData.push_back( strInfo );
	return TRUE;
}

deque<string> &CRecordModule::GetQueue()
{
	return m_deqData;
}

VOID CRecordModule::Clear()
{
	m_deqData.clear();
}

CSDMutex &CRecordModule::GetDataMutex()
{
	return m_mutex;
}

//tstring SDTimeToStringNoFiled(CSDDateTime t)
//{
//	TCHAR temp[20];
//	_SDTsnprintf(temp, 20,_SDT("%4d%02d%02d%02d%02d%02d"), t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());
//	return temp;
//}

string CRecordModule::GetRecordModuleName(UINT32& dwNextID)
{
	dwNextID = m_dwNextID;	
	string strError;
	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	CHAR szSql[256] = {0};
	sprintf(szSql, "call GetModuleTableName('%s')", SDTimeToStringNoFiled(SDNow()).c_str());
	INT32 nRet = m_pDBRecordSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError);
	if(SDDB_HAS_RECORDSET == nRet)
	{
		if(pRecordSet->GetRecord())
		{
			m_strTableName = pRecordSet->GetFieldValueByName("CurTableName");
			if(NULL != pRecordSet->GetFieldValueByName("MaxID"))
			{
				dwNextID = SDAtou(pRecordSet->GetFieldValueByName("MaxID")) + 1;
			}
		}
	}
	else
	{
		if(!strError.empty())
		{
			SYS_CRITICAL(_SDT("[%s: %d]:call GetModuleTableName failed, errstr:%s"), MSG_MARK, strError.c_str());
		}
	}
	if( pRecordSet )
	{
		pRecordSet->Release();
	}

	m_dwNextID = dwNextID;

	return m_strTableName;
}