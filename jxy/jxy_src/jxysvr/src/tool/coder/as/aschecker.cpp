#include "stdafx.h"
#include "aschecker.h"

bool CASChecker::Check( CASProtocolSource* poProto )
{
	m_poProto = poProto;
	if (false == _CheckDefines())
	{
		return false;
	}
	if (false == _CheckTypes())
	{
		return false;
	}
	if (false == _CheckMessages())
	{
		return false;
	}
	if (false == _SetGenerateClass())
	{
		return false;
	}
	return true;
}

bool CASChecker::_CheckDefines()
{
	
	int nCount = m_poProto->GetDefineCount();
	for (int i = 0; i < nCount; i++)
	{
		CASDefine* pDefine = m_poProto->GetDefineByPos(i);

		// check value, if is decimal, hex or octal
		if (IsNumber(pDefine->GetValue()) || IsHex(pDefine->GetValue()) || IsOctal(pDefine->GetValue()))
		{
			
			continue;
		}
		else 
		{
			// not a number, check if defined as some one before
			bool bFind = false;
			for (int j = 0; j < i; j++)
			{
				CASDefine* pRightValue = m_poProto->GetDefineByPos(j);
				if (0 == pRightValue->GetValue().Compare(pDefine->GetValue()))
				{
					bFind = true;
					break;
				}
			}
			if (false == bFind)
			{
				m_strError.Format("CASChecker Error : define %s's value is invalid !");
				return false;
			}
		}
	}
	return true;
}

bool CASChecker::_CheckTypes()
{	
	int nCount = m_poProto->GetTypeCount();
	for (int i = 0; i < nCount; i++)
	{
		CASType* pType = m_poProto->GetTypeByPos(i);
		if (false == _CheckType(pType, i))
		{
			return false;
		}
	}
	return true;
}

bool CASChecker::_CheckType(CASType* pType, int nPos)
{
	// mainly check if item type is right, refer is exist, and count is a number or define value
	int nCount = pType->GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		CASTypeItem* pItem = pType->GetItemByPos(i);

		// check item type

		if (!pItem->GetType().IsEmpty())
		{
			if (false == _IsValidType(pType, nPos, pItem))
			{
				return false;
			}
		}
		else
		{
			m_strError.Format("CASChecker Error : type %s, item %s needs a type value !", pType->GetName(), pItem->GetName());
			return false;
		}

		// check count

		if (!pItem->GetCount().IsEmpty())
		{
			// has count value, need check if it is number or defined value
			if (false == _IsValidCount(pItem->GetCount(), pItem))				
			{
				m_strError.Format("CASChecker Error : type %s, item %s has a invalid count value !", pType->GetName(), pItem->GetName());
				return false;
			}			

			// check refer under count

			if (!pItem->GetRefer().IsEmpty())
			{
				if(false == _IsValidRefer(pType, pItem, i))
				{
					// find all the items before, and did not found the name of one item same as this refer value
					m_strError.Format("CASChecker Error : type %s, item %s has a invalid refer value !", pType->GetName(), pItem->GetName());
					return false;
				}					
			}
		}
		else
		{
			// do not has a count value, refer value would be null too
			if (!pItem->GetRefer().IsEmpty())
			{
				m_strError.Format("CASChecker Error : type %s, item %s should not have a refer value !", pType->GetName(), pItem->GetName());
				return false;
			}
		}				
	}
	return true;
}


bool CASChecker::_CheckMessages()
{
	// check if message to its id is one-to-one in each map (sc-map and cs-map)

	// check one id to more message 
	int nSCCount = m_poProto->GetSCMessageCount();
	int nCSCount = m_poProto->GetCSMessageCount();

	int* aSCIDs = new int[nSCCount];
	int* aCSIDs = new int[nCSCount];
	aCSIDs[0] = aSCIDs[0] = -1;

	CASMessage* pSCMessage = NULL;
	for (int i = 0; i < nSCCount; i++)
	{
		pSCMessage = m_poProto->GetSCMessageByPos(i);
		aSCIDs[i] = atoi(pSCMessage->GetID().GetString());	
		if (i > 0)
		{
			for (int j = 0; j < i; j++)
			{
				if (aSCIDs[j] == aSCIDs[i])
				{
					m_strError.Format("CASChecker Error : one message id match two or more message! id = %s", pSCMessage->GetID());
					return false;
				}
			}
		}		
	}

	CASMessage* pCSMessage = NULL;
	for (int i = 0; i < nCSCount; i++)
	{
		pCSMessage = m_poProto->GetCSMessageByPos(i);
		aCSIDs[i] = atoi(pCSMessage->GetID().GetString());
		if (i > 0)
		{
			for (int j = 0; j < i; j++)
			{
				if (aCSIDs[j] == aCSIDs[i])
				{
					m_strError.Format("CASChecker Error : one message id match two or more message! id = %s", pCSMessage->GetID());
					return false;
				}
			}
		}
	}

	// check one message to more id
	if (nCSCount > 0 && nSCCount > 0)
	{
		for (int i = 0; i < nSCCount; i++)
		{
			pSCMessage = m_poProto->GetSCMessageByPos(i);
			pCSMessage = m_poProto->FindCSMessageByName(pSCMessage->GetName());
			if (NULL == pCSMessage)
			{
				continue;
			}
			else
			{
				pCSMessage->SetBoth(true);		// set this to identify one message to write, when write to file	
				if (0 != pCSMessage->GetID().Compare(pSCMessage->GetID()))
				{
					m_strError.Format("CASChecker Error : one message has two or more ids! message = %s",pSCMessage->GetName());
					return false;
				}
			}
		}
	}
	return true;
}

bool CASChecker::_IsValidCount( CString& strCount ,CASTypeItem* pItem)
{
	if (IsNumber(strCount) || IsHex(strCount) || IsOctal(strCount))
	{
		// is number
		pItem->SetLength(strCount);
		return true;
	}
	else if (m_poProto->IsDefineExist(strCount))
	{					
		// is a defined value
		pItem->SetLength(m_poProto->GetClassName() + "." + strCount);
		return true;					
	}
	return false;
}

bool CASChecker::_IsValidRefer( CASType* pType, CASTypeItem* pItem, int nPos )
{
	CASTypeItem* pReferItem = NULL;

	for (int j = 0; j < nPos; j++)
	{
		pReferItem = pType->GetItemByPos(j);
		if (0 == pReferItem->GetName().Compare(pItem->GetRefer()))
		{
			// refer is defined before
			pItem->SetLength(pItem->GetRefer());
			return true;
		}
	}
	return false;	
}

bool CASChecker::_IsValidType( CASType* pType, int nPos, CASTypeItem* pItem )
{
	CString strType = pItem->GetType();
	strType.MakeUpper();

	if (0 == strType.Compare("STRING") || 0 == strType.Compare("BYTE") || 
		0 == strType.Compare("WORD") || 0 == strType.Compare("DWORD") || 
		0 == strType.Compare("UINT64") || 0 == strType.Compare("INT"))
	{
		//could has both count and refer	

		// string need has count
		if (0 == strType.Compare("STRING") && pItem->GetCount().IsEmpty())
		{
			m_strError.Format("CASChecker Error : type %s, item %s needs a count value !", pType->GetName(), pItem->GetName());
			return false;
		}
		// set array feature, if not string type
		if (0 != strType.Compare("STRING") && !pItem->GetCount().IsEmpty())
		{
			pItem->SetVarType("Array");
			pItem->SetArray(true);
		}		
	}
	else	// not one of the simple type, check if it's a struct or struct array
	{
		CASType* pStructType = NULL;
		bool bFind = false;
		for (int j = 0; j < nPos; j++)	// the item type that referred to need be defined before this very type
		{
			pStructType = m_poProto->GetTypeByPos(j);
			if (0 == pStructType->GetName().Compare(pItem->GetType()))
			{
				bFind = true;
				break;
			}
		}
		if (bFind)
		{
			// set IsStruct tag
			pStructType->SetStructType(true);
			if (!pItem->GetCount().IsEmpty())
			{				
				pStructType->SetStructArrayType(true);
			}				
		}
		else
		{
			// not a type defined before or not a type
			m_strError.Format("CASChecker Error : type %s -> item %s, type '%s' not defined", pType->GetName(), pItem->GetName(), pItem->GetType());
			return false;
		}
	}
	return true;
}

bool CASChecker::_SetGenerateClass()
{
	int nSCCount = m_poProto->GetSCMessageCount();
	CASMessage* pSCMessage = NULL;

	for (int i = 0; i < nSCCount; i++)
	{
		pSCMessage = m_poProto->GetSCMessageByPos(i);
		CASType* pType = m_poProto->FindTypeByName(pSCMessage->GetType());
		_SetGenerate(pType);		
	}

	int nCSCount = m_poProto->GetCSMessageCount();
	CASMessage* pCSMessage = NULL;

	for (int i = 0; i < nCSCount; i++)
	{
		pCSMessage = m_poProto->GetCSMessageByPos(i);
		CASType* pType = m_poProto->FindTypeByName(pCSMessage->GetType());
		_SetGenerate(pType);		
	}
	return true;
}

void CASChecker::_SetGenerate( CASType* pType )
{
	int nCount = pType->GetItemCount();
	
	for (int i = 0; i < nCount; i++)
	{
		CASTypeItem* pItem = pType->GetItemByPos(i);

		if (true == m_poProto->IsTypeExist(pItem->GetType()))
		{		
			CASType* pChildType = m_poProto->FindTypeByName(pItem->GetType());
			pChildType->SetGenerateClass(true);
			_SetGenerate(pChildType);			
		}
	}
}