#include "db_ado_rs.h"
#include <string>
#include "sdmemorypool.h"

#if (defined(WIN32) || defined(WIN64))

static SGDP::CSDVarMemoryPool g_ADORsPool;
static volatile bool g_bPoolInit = false;
static SGDP::CSDMutex g_ADORsPoolLock;

CADORecordSet::CADORecordSet() : 
m_rsPtr("ADODB.Recordset"),
m_bFirstRecord(true)
{
	if(!g_bPoolInit)
	{
		g_ADORsPoolLock.Lock();
		if(!g_bPoolInit)
		{
			g_ADORsPool.Create();
			g_bPoolInit = true;
		}
		g_ADORsPoolLock.Unlock();
	}
}

CADORecordSet::~CADORecordSet()
{
	ClearDatas();
	if(NULL != m_rsPtr)
	{
		m_rsPtr->Close();
	}
}

UINT32 CADORecordSet::GetRecordCount()
{
	if(NULL == m_rsPtr)
	{
		return 0;
	}
	long RecordCount = m_rsPtr->GetRecordCount();
	return RecordCount;
}

UINT32 CADORecordSet::GetFieldCount()
{
	if(NULL == m_rsPtr)
	{
		return 0;
	}
	return m_rsPtr->Fields->GetCount();
}

bool CADORecordSet::GetRecord()
{
	if(NULL == m_rsPtr)
	{
		return false;
	}
	
	ClearDatas();

	if(m_bFirstRecord)
	{
		m_bFirstRecord = false;
	}
	else
	{
		m_rsPtr->MoveNext();
	}
	return !m_rsPtr->adoEOF;
}

const CHAR * CADORecordSet::GetFieldValue(UINT32 dwIndex)
{
	if(NULL == m_rsPtr)
	{
		return NULL;
	}
	_variant_t value = m_rsPtr->Fields->GetItem((long)dwIndex)->Value;

	CHAR * p = NULL;
	//二进制数组
	if(((value.vt) & VT_ARRAY) != 0)
	{
		SAFEARRAY * a = value.parray;
		UINT alen = a->rgsabound[0].cElements;
		g_ADORsPoolLock.Lock();
		p = (CHAR *)g_ADORsPool.Malloc(alen);
		g_ADORsPoolLock.Unlock();
		memcpy(p, a->pvData, alen);
	}
	//NULL
	else if(value.vt == VT_NULL)
	{
		return NULL;
	}
	//varchar和其他
	else
	{
		_bstr_t bstr = _bstr_t(&value);
		int length = SysStringByteLen(bstr.GetBSTR());
		g_ADORsPoolLock.Lock();
		p = (CHAR *)g_ADORsPool.Malloc(length + 1);
		g_ADORsPoolLock.Unlock();

		memcpy(p, (const char *)bstr, length);
		p[length] = '\0';
	}
	m_Datas.push_back(p);
	
	return p;
}

INT32 CADORecordSet::GetFieldLength(UINT32 dwIndex)
{
	if(NULL == m_rsPtr)
	{
		return 0;
	}
	_variant_t value = m_rsPtr->Fields->GetItem((long)dwIndex)->Value;
	//二进制数组
	if(((value.vt) & VT_ARRAY) != 0)
	{
		SAFEARRAY * a = value.parray;
		return a->rgsabound[0].cElements;
	}
	//NULL
	else if(value.vt == VT_NULL)
	{
		return 0;
	}
	//varchar及其他
	else
	{
		_bstr_t bstr = _bstr_t(&value);
		return strlen((const char *)bstr);
	}
}

void CADORecordSet::Release()
{
	ClearDatas();
	if(NULL != m_rsPtr)
	{
		m_rsPtr->Close();
		m_rsPtr = NULL;
	}
	delete this;
}

const CHAR * CADORecordSet::GetFieldValueByName(const CHAR *pszFieldName)
{
	if(NULL == m_rsPtr)
	{
		return NULL;
	}
	_variant_t value = m_rsPtr->Fields->GetItem(pszFieldName)->Value;
	CHAR * p = NULL;
	//二进制数组
	if(((value.vt) & VT_ARRAY) != 0)
	{
		SAFEARRAY * a = value.parray;
		UINT alen = a->rgsabound[0].cElements;
		g_ADORsPoolLock.Lock();
		p = (CHAR *)g_ADORsPool.Malloc(alen);
		g_ADORsPoolLock.Unlock();
		memcpy(p, a->pvData, alen);
	}
	//NULL
	else if(value.vt == VT_NULL)
	{
		return NULL;
	}
	//varchar及其他
	else
	{
		_bstr_t bstr = _bstr_t(&value);
		int length = SysStringByteLen(bstr.GetBSTR());
		g_ADORsPoolLock.Lock();
		p = (CHAR *)g_ADORsPool.Malloc(length + 1);
		g_ADORsPoolLock.Unlock();

		memcpy(p, (const char *)bstr, length);
		p[length] = '\0';
	}
	m_Datas.push_back(p);
	return p;
}

INT32 CADORecordSet::GetFieldLengthByName(const CHAR *pszFieldName)
{
	if(NULL == m_rsPtr)
	{
		return 0;
	}
	_variant_t value = m_rsPtr->Fields->GetItem(pszFieldName)->Value;
	//二进制数组
	if(((value.vt) & VT_ARRAY) != 0)
	{
		int i = value.vt & VT_ARRAY;
		SAFEARRAY * a = value.parray;
		return a->rgsabound[0].cElements;
	}
	//NULL
	else if(value.vt == VT_NULL)
	{
		return 0;
	}
	//varchar及其他
	else
	{
		_bstr_t bstr = _bstr_t(&value);
		return strlen((const char *)bstr);
	}
}

void CADORecordSet::ClearDatas()
{
	std::list<CHAR *>::iterator itr = m_Datas.begin();
	while(itr != m_Datas.end())
	{
		CHAR * p = *itr;
		g_ADORsPoolLock.Lock();
		g_ADORsPool.Free(p);
		g_ADORsPoolLock.Unlock();
		m_Datas.erase(itr);
		itr = m_Datas.begin();
	}
}

#endif

