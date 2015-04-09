#include "stdafx.h"

#include "javaloader.h"

//tags
static LPCTSTR POS_CODER		= "coder";
static LPCTSTR POS_INCLUDE		= "include";
static LPCTSTR POS_DEFINES		= "defines";
static LPCTSTR POS_MSG			= "message";
static LPCTSTR POS_TYPES		= "types";

static LPCTSTR ELEM_CODER		= "coder";	// attr: package, name, version
static LPCTSTR ATTR_PACKAGE	    = "package";
static LPCTSTR ATTR_NAME		= "name";
static LPCTSTR ATTR_FILE        = "name";
static LPCTSTR ATTR_VER			= "version";

static LPCTSTR ELEM_INCLUDE	    = "include";
static LPCTSTR ELEM_IMPORT	    = "import";

static LPCTSTR ELEM_DEFINES	    = "defines";
static LPCTSTR ELEM_DEFINE		= "define";// attr : name, value
static LPCTSTR ATTR_VALUE		= "value";

static LPCTSTR ELEM_TYPES		= "types";
static LPCTSTR ELEM_TYPE		= "type";// attr : name
static LPCTSTR ELEM_ITEM		= "item";// attr : name, type, count, refer, select
static LPCTSTR ATTR_TYPE		= "type";
static LPCTSTR ATTR_COUNT		= "count";
static LPCTSTR ATTR_REFER		= "refer";
static LPCTSTR ATTR_SELECT      = "select";

static LPCTSTR ELEM_MESSAGES	= "messages";
static LPCTSTR ELEM_MESSAGE		= "message"; // attr : name, id, type
static LPCTSTR ATTR_ID			= "id";

#ifdef _DEBUG 
#define new DEBUG_NEW 
#endif

CJavaLoader::CJavaLoader()
{
    m_poProto      = NULL;
    m_pDocument    = NULL;
    m_pRootElement = NULL;
}

CJavaLoader::~CJavaLoader()
{
    _Clean();
}

void CJavaLoader::_Clean()
{
    if (m_pDocument)
    {
        delete m_pDocument;
	    m_pDocument = NULL;
    }

    if (m_poProto)
    {
        delete m_poProto;
        m_poProto = NULL;
    }

    if (!m_vecIncludes.empty())
    {
        CJavaIncludeVector::iterator guard = m_vecIncludes.begin();

        for (; guard != m_vecIncludes.end(); guard++)
        {
            delete *guard;
        }

        m_vecIncludes.clear();
    }

    if (!m_vecDefines.empty())
    {
        CJavaDefineVector::iterator guard = m_vecDefines.begin();

        for (; guard != m_vecDefines.end(); guard++)
        {
            delete *guard;
        }

        m_vecDefines.clear();
    }

    if (!m_vecMessages.empty())
    {
        CJavaMessageVector::iterator guard = m_vecMessages.begin();

        for (; guard != m_vecMessages.end(); guard++)
        {
            delete *guard;
        }

        m_vecMessages.clear();
    }

    if (!m_vecTypes.empty())
    {
        CJavaTypeVector::iterator guard = m_vecTypes.begin();

        for (; guard != m_vecTypes.end(); guard++)
        {
            delete *guard;
        }

        m_vecTypes.clear();
    }
}

bool CJavaLoader::_LoadJavaProto()
{
    if (NULL == m_pRootElement)
	{
		return false;
	}

    m_poProto = new CJavaProto();

    //
	// coder -> package
    //

	CString strPackage = (char*)m_pRootElement->Attribute(ATTR_PACKAGE);

	if (strPackage.IsEmpty())
	{
		m_strError.Format("CJavaLoader Error : coder -> package is not set!");
		return false;
	}

	m_poProto->SetPackageName(strPackage);

    //
	// coder -> name
    //

	CString strName = (char*)m_pRootElement->Attribute(ATTR_NAME);

	if(strName.IsEmpty())
	{
		m_strError.Format("CJavaLoader Error : coder -> name is not set!");
		return false;
	}

	m_poProto->SetName(strName);

    //
	// coder -> version
    //

	CString strVersion = (char*)m_pRootElement->Attribute(ATTR_VER); 

	if(strVersion.IsEmpty())
	{
		m_strError.Format("CJavaLoader Error : coder -> version is not set!");
		return false;
	}

	if(!IsNumber(strVersion))
	{
		m_strError.Format("CJavaLoader Error : coder -> version must be number", strVersion);
		return false;
	}

	m_poProto->SetVersion(strVersion);

	return true;
}

bool CJavaLoader::Load(CString& strFilePath)
{
    if (!_Load(strFilePath))
    {
        _Clean();
        return false;
    }

    return true;
}

bool CJavaLoader::_LoadInclude()
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
	TiXmlElement* pImportElement = pInclude->FirstChildElement(ELEM_IMPORT);

    // include -> import

	while(pImportElement)
	{

        // import -> name

		strImport = (char*)pImportElement->Attribute(ATTR_FILE);

        // push CJavaInclude to include vector

        CJavaInclude* poJavaInclude = new CJavaInclude();
        poJavaInclude->SetInclude(strImport);
		m_vecIncludes.push_back(poJavaInclude);

		pImportElement = pImportElement ->NextSiblingElement(ELEM_IMPORT);
	}

	return true;
}

bool CJavaLoader::_LoadDefines()
{
	if (NULL == m_pRootElement)
	{
		return false ;
	}

	TiXmlElement* pDefinesElement = m_pRootElement->FirstChildElement(POS_DEFINES);

	if (NULL == pDefinesElement)
	{
		return true ;
	}

	TiXmlElement* pDefineElement = pDefinesElement->FirstChildElement(ELEM_DEFINE);

	while(pDefineElement)
	{
		// define -> name

		CString strName = (char*)pDefineElement->Attribute(ATTR_NAME);

		if(strName.IsEmpty())
		{
			m_strError.Format("CJavaLoader Error : define -> name can not be null !");
			return false;
		}

        // define -> value

		CString strValue = (char*)pDefineElement->Attribute(ATTR_VALUE);

		if(strValue.IsEmpty())
		{
			m_strError.Format("CJavaLoader Error : define -> value can not be null !");
			return false;
		}

        // push CJavaDefine to define vector

		CJavaDefine* pDefine = new CJavaDefine;
		pDefine->SetName(strName);
		pDefine->SetValue(strValue);
        m_vecDefines.push_back(pDefine);

		pDefineElement = pDefineElement->NextSiblingElement(ELEM_DEFINE) ;
	}

	return true;
}

bool CJavaLoader::_LoadMessages()
{
	if (NULL == m_pRootElement)
	{
		return false ;
	}

	TiXmlElement* pMessagesElement = m_pRootElement->FirstChildElement(ELEM_MESSAGES);

	if (NULL == pMessagesElement)
	{
		return true ;
	}

	CString strName;
	CString strID;
	CString strType;

	TiXmlElement* pMessageElement = pMessagesElement->FirstChildElement(ELEM_MESSAGE);

    // messages -> message
    
    while(pMessageElement)
	{
        
        // message -> name

		strName = (char*)pMessageElement->Attribute(ATTR_NAME);

		if(strName.IsEmpty())
		{
			m_strError.Format("CJavaLoader Error : message -> name can not be null !");
			return false;
		}

        // message -> id

		strID = (char*)pMessageElement->Attribute(ATTR_ID);	

		if(strID.IsEmpty())
		{			

			m_strError.Format("CJavaLoader Error : message(%s) -> id can not be null !", strName);
			return false;
		}

        // message -> type

		strType = (char*)pMessageElement->Attribute(ATTR_TYPE);

		if(strType.IsEmpty())
		{
			m_strError.Format("CJavaLoader Error : message(%s) -> type can not be null !", strName);
			return false;
		}

        // push CJavaMessage message vector

		CJavaMessage* pMessage = new CJavaMessage;
		pMessage->SetName(strName);
		pMessage->SetID(strID);
		pMessage->SetType(strType);
        m_vecMessages.push_back(pMessage);

		pMessageElement = pMessageElement ->NextSiblingElement(ELEM_MESSAGE) ;
	}

	return true;
}

bool CJavaLoader::_LoadTypes()
{
	if (NULL == m_pRootElement)
	{
		return false ;
	}

	TiXmlElement* pTypesElement = m_pRootElement->FirstChildElement(POS_TYPES);

	if (NULL == pTypesElement)
	{
		return true ;
	}

	TiXmlElement* pTypeElement = pTypesElement->FirstChildElement(ELEM_TYPE);

    // types -> type

	while(pTypeElement)
	{
		if(false == _LoadAType(pTypeElement))
        {
			return false;
        }

		pTypeElement = pTypeElement->NextSiblingElement(ELEM_TYPE) ;
	}

	return true;
}

bool CJavaLoader::_LoadAType(TiXmlElement* pTypeElement)
{
	CString strName = (char*)pTypeElement->Attribute(ATTR_NAME);

	if(strName.IsEmpty())
	{
		m_strError.Format("CJavaLoader Error : type -> name can not be null !");
		return false;
	}	

    // push CJavaType type vector

	CJavaType* pType = new CJavaType;
    m_vecTypes.push_back(pType);
	pType->SetName(strName);

	CString strItemName;
	CString strType;
	CString strCount;
	CString strRefer;
    CString strSelect;
	TiXmlElement* pItemElement = pTypeElement->FirstChildElement(ELEM_ITEM) ;

    // type -> item

	while(pItemElement)
	{

        // item -> name

		strItemName = (char*)pItemElement->Attribute(ATTR_NAME);

		if(strItemName.IsEmpty())
		{
			m_strError.Format("CJavaLoader Error : item -> name can not be null !");
			return false;
		}

        // item -> type

		strType = (char*)pItemElement->Attribute(ATTR_TYPE);

		if(strType.IsEmpty())
		{
			m_strError.Format("CJavaLoader Error : item -> type can not be null !");
			return false;
		}

        // item -> count

		strCount =(char*)pItemElement->Attribute(ATTR_COUNT);

        // item -> refer

		strRefer = (char*)pItemElement->Attribute(ATTR_REFER);

        // item -> select, reserved

        strSelect = (char*)pItemElement->Attribute(ATTR_SELECT);

		CJavaTypeItem* pItem = new CJavaTypeItem;

		pItem->SetName(strItemName);
		pItem->SetType(strType);
		pItem->SetCount(strCount);
		pItem->SetRefer(strRefer);
        pItem->SetSelect(strSelect);

		CString strUpperType = pItem->GetType();
		strUpperType.MakeUpper();

		if (0 == strUpperType.Compare("BYTE"))
		{
            pItem->SetType(CString("BYTE"));
		}
		else if (0 == strUpperType.Compare("WORD"))
		{
			pItem->SetType(CString("WORD"));
		}
		else if (0 == strUpperType.Compare("DWORD"))
		{
            pItem->SetType(CString("DWORD"));
		}
		else if (0 == strUpperType.Compare("UINT64"))
		{
            pItem->SetType(CString("UINT64"));
		}
		else if (0 == strUpperType.Compare("STRING"))
		{
			pItem->SetType(CString("STRING"));
		}
		else if (0 == strUpperType.Compare("INT"))
		{
			pItem->SetType(CString("INT"));
		}
        
        // push item to type

        try
        {
            pType->InsertItem(pItem);
        }
        catch (std::runtime_error& except)
        {
            m_strError = except.what();
            return false;
        }

		pItemElement = pItemElement->NextSiblingElement(ELEM_ITEM);		
	}
	
	return true;
}

bool CJavaLoader::_Load(CString& strFilePath)
{
    m_pDocument = new TiXmlDocument ;

	if (false == m_pDocument->LoadFile(strFilePath.GetString()))
	{
		m_strError = "CJavaLoader Error : Load file " + strFilePath + "failed!" + " check the xml structure.";
		return false ;
	}

    m_pRootElement = m_pDocument->RootElement() ;

	if (NULL == m_pRootElement)
	{
		return false ;
	}

	if(false == _LoadJavaProto())
    {
		return false;
    }

	if (false == _LoadInclude())
    {
		return false;
    }

	if(false == _LoadDefines())
    {
		return false;
    }

	if(false == _LoadTypes())
    {
		return false;
    }

	if(false == _LoadMessages())
    {
		return false;
    }

	return true;
}
