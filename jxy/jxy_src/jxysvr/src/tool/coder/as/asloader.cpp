#include "stdafx.h"
#include "asloader.h"


//tags
static LPCTSTR POS_CODER		= "coder";
static LPCTSTR POS_INCLUDE		= "include";
static LPCTSTR POS_DEFINES		= "defines";
static LPCTSTR POS_MSG			= "message";
static LPCTSTR POS_TYPES		= "types";

static LPCTSTR ELEM_CODER		= "coder";	// attr: package, name, version
static LPCTSTR ATTR_PACKAGE	= "package";
static LPCTSTR ATTR_NAME		= "name";
static LPCTSTR ATTR_VER			= "version";

static LPCTSTR ELEM_INCLUDE	= "include";
static LPCTSTR ELEM_IMPORT	= "import";

static LPCTSTR ELEM_DEFINES	= "defines";
static LPCTSTR ELEM_DEFINE		= "define";// attr : name, value
static LPCTSTR ATTR_VALUE		= "value";

static LPCTSTR ELEM_TYPES		= "types";
static LPCTSTR ELEM_TYPE		= "type";// attr : name
static LPCTSTR ELEM_ITEM		= "item";// attr : name, type, count, refer
static LPCTSTR ATTR_TYPE		= "type";
static LPCTSTR ATTR_COUNT		= "count";
static LPCTSTR ATTR_REFER		= "refer";

static LPCTSTR ELEM_MESSAGES	= "messages";
static LPCTSTR ELEM_MESSAGE		= "message"; // attr : name, id, type, info
static LPCTSTR ATTR_ID				= "id";
static LPCTSTR ATTR_DIR				= "dir";
static LPCTSTR ATTR_INFO			= "info";



bool CASLoader::Load( CString& strFilePath, CASProtocolSource* poProto )
{
	m_poProto = poProto;

	if (m_pDocument)
	{
		delete m_pDocument ;
		m_pDocument = NULL ;
	}
	m_pDocument = new TiXmlDocument ;
	if (false == m_pDocument->LoadFile(strFilePath.GetString()))
	{
		delete m_pDocument ;
		m_pDocument = NULL ;
		m_strError = "CASLoader Error : Load file " + strFilePath + "failed!" + " check the xml structure.";
		return false ;
	}
	m_pRootElement = m_pDocument->RootElement() ;
	if (NULL == m_pRootElement)
	{
		delete m_pDocument ;
		m_pDocument = NULL ;
		return false ;
	}
	if(false == _SavePos())
		return false;

	if(false == _LoadCoder())
		return false;

	if (false == _LoadInclude())
		return false;

	if(false == _LoadDefines())
		return false;

	if(false == _LoadTypes())
		return false;

	if(false == _LoadMessages())
		return false;

	return true;
}

bool CASLoader::_SavePos()
{
	if (m_pDocument)
	{
		m_pDocument->SaveFile() ;
	}
	return true;
}

bool CASLoader::_LoadCoder()
{
	if (NULL == m_pRootElement)
	{
		return false ;
	}

	// coder -> package
	CString strPackage = (char*)m_pRootElement->Attribute(ATTR_PACKAGE);
	if (strPackage.IsEmpty())
	{
		m_strError.Format("CASLoader Error : coder -> package is not set!");
		return false;
	}
	m_poProto->SetPackageName(strPackage);

	// coder -> name
	CString strName = (char*)m_pRootElement->Attribute(ATTR_NAME);
	if(strName.IsEmpty())
	{
		m_strError.Format("CASLoader Error : coder -> name is not set!");
		return false;
	}
	m_poProto->SetClassName(strName);

	// coder -> version
	CString strVersion = (char*)m_pRootElement->Attribute(ATTR_VER); 
	if(strVersion.IsEmpty())
	{
		m_strError.Format("CASLoader Error : coder -> version is not set!");
		return false;
	}
	if(!IsNumber(strVersion))
	{
		m_strError.Format("CASLoader Error : coder -> version must be number", strVersion);
		return false;
	}
	m_poProto->SetVersion(strVersion);

	return true;
}

bool CASLoader::_LoadDefines()
{
	if (NULL == m_pRootElement)
	{
		return false ;
	}
	TiXmlElement* pDefinesElement = m_pRootElement->FirstChildElement(POS_DEFINES) ;
	if (NULL == pDefinesElement)
	{
		return true ;
	}
	TiXmlElement* pDefineElement = pDefinesElement->FirstChildElement(ELEM_DEFINE) ;

	while(pDefineElement)
	{
		// define -> name
		CString strName = (char*)pDefineElement->Attribute(ATTR_NAME);
		if(strName.IsEmpty())
		{
			m_strError.Format("CASLoader Error : define -> name can not be null !");
			return false;
		}

		CString strValue = (char*)pDefineElement->Attribute(ATTR_VALUE);
		if(strValue.IsEmpty())
		{
			m_strError.Format("CASLoader Error : define -> value can not be null !");
			return false;
		}

		CASDefine* pDefine = new CASDefine;
		pDefine->SetName(strName);
		pDefine->SetValue(strValue);
		if (false == m_poProto->InsertDefine(pDefine))
		{
			m_strError = m_poProto->GetErrString();
			return false;
		}
		pDefineElement = pDefineElement->NextSiblingElement(ELEM_DEFINE) ;
	}

	return true;
}

bool CASLoader::_LoadTypes()
{
	if (NULL == m_pRootElement)
	{
		return false ;
	}
	TiXmlElement* pTypesElement = m_pRootElement->FirstChildElement(POS_TYPES) ;
	if (NULL == pTypesElement)
	{
		return true ;
	}
	TiXmlElement* pTypeElement = pTypesElement->FirstChildElement(ELEM_TYPE) ;
	while(pTypeElement)
	{
		if(false == _LoadAType(pTypeElement))
			return false;
		pTypeElement = pTypeElement->NextSiblingElement(ELEM_TYPE) ;
	}

	return true;
	
}

bool CASLoader::_LoadAType(TiXmlElement* pTypeElement)
{
	CString strName = (char*)pTypeElement->Attribute(ATTR_NAME);
	if(strName.IsEmpty())
	{
		m_strError.Format("CASLoader Error : type -> name can not be null !");
		return false;
	}	

	CASType* pType = new CASType;
	pType->SetName(strName);

	CString strItemName;
	CString strType;
	CString strCount;
	CString strRefer;
	TiXmlElement* pItemElement = pTypeElement->FirstChildElement(ELEM_ITEM) ;
	while(pItemElement)
	{
		strItemName = (char*)pItemElement->Attribute(ATTR_NAME);
		if(strItemName.IsEmpty())
		{
			m_strError.Format("CASLoader Error : item -> name can not be null !");
			return false;
		}

		strType = (char*)pItemElement->Attribute(ATTR_TYPE);
		if(strType.IsEmpty())
		{
			m_strError.Format("CASLoader Error : item -> type can not be null !");
			return false;
		}

		strCount =(char*)pItemElement->Attribute(ATTR_COUNT) ;
		strRefer = (char*)pItemElement->Attribute(ATTR_REFER);

		CASTypeItem* pItem = new CASTypeItem;

		pItem->SetName(strItemName);
		pItem->SetType(strType);
		pItem->SetCount(strCount);
		pItem->SetRefer(strRefer);

		// Set Var type and Conv_ type (Conv_ = Add_)
		CString strUpperType = pItem->GetType();
		strUpperType.MakeUpper();
		if (0 == strUpperType.Compare("BYTE"))
		{
			pItem->SetVarType("int");
			pItem->SetConvType("BYTE");
			pItem->SetItemType(EI_SimpleType);
		}
		else if (0 == strUpperType.Compare("WORD"))
		{
			pItem->SetVarType("int");
			pItem->SetConvType("WORD");
			pItem->SetItemType(EI_SimpleType);
		}
		else if (0 == strUpperType.Compare("DWORD"))// ? dword -> uint ?
		{
			pItem->SetVarType("int");
			pItem->SetConvType("DWORD");
			pItem->SetItemType(EI_SimpleType);
		}
		else if (0 == strUpperType.Compare("UINT64"))
		{
			pItem->SetVarType("uint");
			pItem->SetConvType("DWORD");
			pItem->SetItemType(EI_SimpleType);
		}
		else if (0 == strUpperType.Compare("STRING"))
		{
			pItem->SetVarType("String");
			pItem->SetConvType("STR");
			pItem->SetItemType(EI_SimpleType);
		}
		else if (0 == strUpperType.Compare("INT"))
		{
			pItem->SetVarType("int");
			pItem->SetConvType("DWORD");
			pItem->SetItemType(EI_SimpleType);
		}
		else
		{
			if (!pItem->GetCount().IsEmpty())
			{
				CString strTemp = pItem->GetType() + "List";
				pItem->SetVarType(strTemp);
				pItem->SetConvType(strTemp);
				pItem->SetItemType(EI_StructListType);
			}
			else
			{
				pItem->SetVarType(pItem->GetType());
				pItem->SetConvType(pItem->GetType());	
				pItem->SetItemType(EI_StructType);
			}
		}

		if (false == pType->InsertItem(pItem))
		{
			m_strError.Format("CASLoader Error : type %s has same item name !", pType->GetName());
			return false;
		}
		pItemElement = pItemElement->NextSiblingElement(ELEM_ITEM) ;
		
	}
	if (false ==m_poProto->InsertType(pType))
	{
		m_strError = m_poProto->GetErrString();
		return false;
	}
	
	return true;
}

bool CASLoader::_LoadMessages()
{
	if (NULL == m_pRootElement)
	{
		return false ;
	}
	TiXmlElement* pMessagesElement = m_pRootElement->FirstChildElement(ELEM_MESSAGES) ;
	if (NULL == pMessagesElement)
	{
		return true ;
	}

	CString strName;
	CString strID;
	CString strType;
	CString strDir;
	CString strInfo;
	TiXmlElement* pMessageElement = pMessagesElement->FirstChildElement(ELEM_MESSAGE) ;
	while(pMessageElement)
	{
		strName = (char*)pMessageElement->Attribute(ATTR_NAME);
		if(strName.IsEmpty())
		{
			m_strError.Format("CASLoader Error : message -> name can not be null !");
			return false;
		}

		strID = (char*)pMessageElement->Attribute(ATTR_ID);		
		if(strID.IsEmpty())
		{			

			m_strError.Format("CASLoader Error : message(%s) -> id can not be null !", strName);
			return false;
		}

		strType = (char*)pMessageElement->Attribute(ATTR_TYPE);
		if(strType.IsEmpty())
		{
			m_strError.Format("CASLoader Error : message(%s) -> type can not be null !", strName);
			return false;
		}

		strDir = (char*)pMessageElement->Attribute(ATTR_DIR);
		if(strDir.IsEmpty())
		{
			m_strError.Format("CASLoader Error : message(%s) -> dir can not be null !", strName);
			return false;
		}	

		strInfo = (char*)pMessageElement->Attribute(ATTR_INFO);

		CASMessage* pMessage = new CASMessage;
		pMessage->SetName(strName);
		pMessage->SetID(strID);
		pMessage->SetType(strType);
		pMessage->SetDir(strDir.MakeLower());
		pMessage->SetInfo(strInfo);

		if (0 == pMessage->GetDir().Compare("cs"))
		{
			if(false == m_poProto->InsertCSMessage(pMessage))
			{
				m_strError = m_poProto->GetErrString();
				return false;
			}
		}
		else if (0 == pMessage->GetDir().Compare("sc"))
		{
			if(false == m_poProto->InsertSCMessage(pMessage))
			{
				m_strError = m_poProto->GetErrString();
				return false;
			}
		}
		else
		{
			m_strError.Format("CASLoader Error : message(%s) -> dir should be 'cs' or 'sc' not %s", strName, strDir);
			return false;
		}

		pMessageElement = pMessageElement ->NextSiblingElement(ELEM_MESSAGE) ;
	}

	return true;
}

bool CASLoader::_LoadInclude()
{
	if (NULL == m_pRootElement)
	{
		return true ;
	}
	TiXmlElement* pInclude = m_pRootElement->FirstChildElement(POS_INCLUDE);
	if (NULL == pInclude)
	{
		return true ;
	}

	CString strImport;
	TiXmlElement* pImportElement = pInclude->FirstChildElement(ELEM_IMPORT) ;
	while(pImportElement)
	{
		strImport = (char*)pImportElement->Attribute(ATTR_NAME);
		m_poProto->InsertImport(strImport);
		pImportElement = pImportElement ->NextSiblingElement(ELEM_IMPORT) ;
	}
	return true;
}