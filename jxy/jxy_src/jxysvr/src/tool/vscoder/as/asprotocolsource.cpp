#include "stdafx.h"
#include "asprotocolsource.h"

//////////////////////////////////////////////////////////////////////////

bool CASType::InsertItem( CASTypeItem* pTypeItem )
{
	if (NULL == pTypeItem)
	{
		return false;
	}
	if(m_mapTypeItems.end() != m_mapTypeItems.find(pTypeItem->GetName())) 
	{
		return false;
	}
	m_mapTypeItems[pTypeItem->GetName()] = pTypeItem;
	m_vecTypeItems.push_back(pTypeItem);
	return true;
}

CASTypeItem* CASType::GetItem( CString& strName )
{
	CItemMap::iterator it = m_mapTypeItems.find(strName);
	if (m_mapTypeItems.end() != it)
	{
		return it->second;
	}
	return NULL;
}

CASTypeItem* CASType::GetItemByPos( int nPos )
{
	if (nPos >= 0 && nPos <= (int)m_vecTypeItems.size())
	{
		return m_vecTypeItems[nPos];
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

bool CASProtocolSource::InsertDefine( CASDefine* pDefine )
{
	if (NULL == pDefine)
	{
		return false;
	}
	if (m_mapDefines.end() != m_mapDefines.find(string(pDefine->GetName().GetBuffer())))
	{
		m_strError.Format("CASProtocolSource Error : define -> name %s has already defined", pDefine->GetName());
		return false;
	}
	m_mapDefines[string(pDefine->GetName().GetBuffer())] = pDefine;
	m_vecDefines.push_back(pDefine);
	return true;
}

bool CASProtocolSource::InsertType( CASType* pType )
{
	if (NULL == pType)
	{
		return false;
	}
	if (m_mapTypes.end() != m_mapTypes.find(string(pType->GetName().GetBuffer())))
	{
		m_strError.Format("CASProtocolSource Error : type -> name %s has already defined", pType->GetName());
		return false;
	}
	m_mapTypes[string(pType->GetName().GetBuffer())] = pType;	
	m_vecTypes.push_back(pType);
	return true;
}

bool CASProtocolSource::InsertCSMessage( CASMessage* pMessage )
{
	if (NULL == pMessage)
	{
		return false;
	}
	if (m_mapCSMessages.end() != m_mapCSMessages.find(string(pMessage->GetName().GetBuffer())))
	{
		m_strError.Format("CASProtocolSource Error : type -> name %s has already defined", pMessage->GetName());
		return false;
	}
	m_mapCSMessages[string(pMessage->GetName().GetBuffer())] = pMessage;
	m_vecCSMessages.push_back(pMessage);
	return true;
}

bool CASProtocolSource::InsertSCMessage( CASMessage* pMessage )
{
	if (NULL == pMessage)
	{
		return false;
	}
	if (m_mapSCMessages.end() != m_mapSCMessages.find(string(pMessage->GetName().GetBuffer())))
	{
		m_strError.Format("CASProtocolSource Error : type -> name %s has already defined", pMessage->GetName());
		return false;
	}
	m_mapSCMessages[string(pMessage->GetName().GetBuffer())] = pMessage;
	m_vecSCMessages.push_back(pMessage);
	return true;
}

int CASProtocolSource::GetDefineCount()
{
	return (int)m_vecDefines.size();
}

int CASProtocolSource::GetTypeCount()
{
	return (int)m_vecTypes.size();
}

int CASProtocolSource::GetCSMessageCount()
{
	return (int)m_vecCSMessages.size();
}

int CASProtocolSource::GetSCMessageCount()
{
	return (int)m_vecSCMessages.size();
}

CASDefine* CASProtocolSource::GetDefineByPos( int nPos )
{
	if (nPos >=0 && nPos <= (int)m_vecDefines.size())
	{
		return m_vecDefines[nPos];
	}
	return NULL;
	
}

CASType* CASProtocolSource::GetTypeByPos( int nPos )
{
	if (nPos >=0 && nPos <= (int)m_vecTypes.size())
	{
		return m_vecTypes[nPos];
	}
	return NULL;
}

CASMessage* CASProtocolSource::GetCSMessageByPos( int nPos )
{
	if (nPos >=0 && nPos <= (int)m_vecCSMessages.size())
	{
		return m_vecCSMessages[nPos];
	}
	return NULL;
}

CASMessage* CASProtocolSource::GetSCMessageByPos( int nPos )
{
	if (nPos >=0 && nPos <= (int)m_vecSCMessages.size())
	{
		return m_vecSCMessages[nPos];
	}
	return NULL;
}

CASDefine* CASProtocolSource::FindDefineByName( CString& strName )
{
	CDefineMap::iterator it = m_mapDefines.find(string(strName.GetBuffer()));
	if (m_mapDefines.end() != it)
	{
		return it->second;
	}
	return NULL;
}

CASType* CASProtocolSource::FindTypeByName( CString& strName )// in map
{
	CTypeMap::iterator it = m_mapTypes.find(string(strName.GetBuffer()));
	if (m_mapTypes.end() != it)
	{
		return it->second;
	}
	return NULL;
}

CASMessage* CASProtocolSource::FindCSMessageByName( CString& strName )
{
	CMessageMap::iterator it = m_mapCSMessages.find(string(strName.GetBuffer()));
	if (m_mapCSMessages.end() != it)
	{
		return it->second;
	}
	return NULL;
}

CASMessage* CASProtocolSource::FindSCMessageByName( CString& strName )
{
	CMessageMap::iterator it = m_mapSCMessages.find(string(strName.GetBuffer()));
	if (m_mapSCMessages.end() != it)
	{
		return it->second;
	}
	return NULL;
}

bool CASProtocolSource::IsDefineExist( CString& strName )
{
	CDefineMap::iterator it = m_mapDefines.find(string(strName.GetBuffer()));
	if (m_mapDefines.end() != it)
	{
		return true;
	}
	return false;
}

bool CASProtocolSource::IsTypeExist( CString& strName )
{
	CTypeMap::iterator it = m_mapTypes.find(string(strName.GetBuffer()));
	if (m_mapTypes.end() != it)
	{
		return true;
	}
	return false;
}

bool CASProtocolSource::IsSCMessageExist( CString& strName )
{
	CMessageMap::iterator it = m_mapCSMessages.find(string(strName.GetBuffer()));
	if (m_mapCSMessages.end() != it)
	{
		return true;
	}
	return false;
}

bool CASProtocolSource::IsCSMessageExist( CString& strName )
{
	CMessageMap::iterator it = m_mapSCMessages.find(string(strName.GetBuffer()));
	if (m_mapSCMessages.end() != it)
	{
		return true;
	}
	return false;
}

bool CASProtocolSource::InsertImport( CString strImport )
{
	int nCount = (int)m_vecInclude.size();
	for (int i = 0; i < nCount; i++)
	{
		if (0 == strImport.Compare(m_vecInclude[i]))
		{
			return false;
		}
	}
	m_vecInclude.push_back(strImport);
	return true;
}

int CASProtocolSource::GetImportCount()
{
	return (int)m_vecInclude.size();
}

CString CASProtocolSource::GetImportByPos( int nPos )
{
	int nCount = (int)m_vecInclude.size();
	if (nPos >= 0 && nPos <=nCount)
	{
		return m_vecInclude[nPos];
	}
	return "";
}