#include "stdafx.h"
#include "parser.h"
#include "paraminfo.h"

static LPCTSTR POS_CODER		= "coder";
static LPCTSTR POS_INCLUDES		= "includes";
static LPCTSTR POS_DEFINES		= "defines";
static LPCTSTR POS_MSG			= "message";
static LPCTSTR POS_TYPES		= "types";

static LPCTSTR CLASS_UNION		= "union";
static LPCTSTR CLASS_STRUCT		= "struct";

static LPCTSTR ELEM_CODER		= "coder";
static LPCTSTR ATTR_NAME		= "name";
static LPCTSTR ATTR_SID			= "sid";
static LPCTSTR ATTR_VER			= "version";
static LPCTSTR ATTR_OLDVER		= "oldversion";
static LPCTSTR ELEM_DEFINES		= "defines";
static LPCTSTR ELEM_DEFINE		= "define";
static LPCTSTR ELEM_INCLUDES	= "includes";
static LPCTSTR ELEM_INCLUDE		= "include";
static LPCTSTR ATTR_VALUE		= "value";
static LPCTSTR ELEM_MESSAGES	= "messages";
static LPCTSTR ELEM_MESSAGE		= "message";
static LPCTSTR ATTR_ID			= "id";
static LPCTSTR ELEM_TYPES		= "types";
static LPCTSTR ELEM_TYPE		= "type";
static LPCTSTR ELEM_ITEM		= "item";
static LPCTSTR ATTR_TYPE		= "type";
static LPCTSTR ATTR_COUNT		= "count";
static LPCTSTR ATTR_REFER		= "refer";
static LPCTSTR ATTR_SELECT		= "select";
static LPCTSTR ATTR_CLASS		= "class";
static LPCTSTR ATTR_FILE		= "file";
static LPCTSTR ATTR_NONEFOLLOW	= "none-follow";
static LPCTSTR ATTR_IGNORE		= "ignore";
static LPCTSTR ATTR_KEYIDX		= "keyidx";


CParser::CParser()
{
    m_szName[0] = '\0';
    m_nVersion = 0;
	m_nOldVersion = 0;
    m_strErrMsg = "";
    m_nMinID = 0;
    m_nMaxID = 0;
    m_nMsgIDRange = 0;
    m_pDocument = NULL ;
    m_pRootElement = NULL ;
	m_poMainParser = NULL;
}

CParser::~CParser()
{
    Release() ;
}

void CParser::Release()
{
    if (m_pDocument)
    {
        delete m_pDocument ;
        m_pDocument = NULL ;
    }

}

CString CParser::GetNameFromConfig(const char* pszFileName)
{
    TiXmlDocument* pParseDocument = new TiXmlDocument ;
    if (pParseDocument->LoadFile(pszFileName))
    {
        TiXmlElement* pParseElement = pParseDocument->RootElement() ;
        if (pParseElement)
        {
            char* cpName = (char*)pParseElement->Attribute("name") ;
            if (cpName)
            {
                return cpName ;
            }
        }
    }

    delete pParseDocument ;
    return "";
}

bool CParser::Parse(LPCTSTR pszXml, CParser* poMainParser, BOOL bCliEnDe)
{
	m_bCliEnDe = bCliEnDe;
	m_poMainParser = poMainParser;

    m_strErrMsg.Format("Parse %s Failed......Unknown Error", pszXml);
    Release() ;
    m_pDocument = new TiXmlDocument ;
    if(false == m_pDocument->LoadFile(pszXml))
    {
        m_strErrMsg.Format( "Load XML File %s failed, error:%s", pszXml, m_pDocument->ErrorDesc() );
        return false;
    }
    m_pRootElement = m_pDocument->RootElement() ;
    if(false == _SavePos())
        return false;

    if(false == _ParseCoder())
        return false;

    if(false == _ParseIncludes())
        return false;

    if(false == _ParseDefines())
        return false;

    if(false == _ParseTypes())
        return false;
	////fortest
	//if(false == _SavePos())
	//	return false;

    if(false == _ParseMessages())
        return false;
    _MakeMsgRange();
    return true;
}

bool CParser::_SavePos()
{
    if (m_pDocument)
    {
        m_pDocument->SaveFile() ;
    }
    return true;
}

bool CParser::_ParseCoder()
{
    if(NULL == m_pRootElement)
        return false;

    CString strName = m_pRootElement->Attribute(ATTR_NAME) == NULL ? "" : m_pRootElement->Attribute(ATTR_NAME);
    if(strName.IsEmpty())
    {
        m_strErrMsg.Format("Can not find <%s %s=\"\">", ELEM_CODER, ATTR_NAME);
        return false;
    }
    lstrcpyn(m_szName, strName, sizeof(m_szName));

    CString strVersion = m_pRootElement->Attribute(ATTR_VER) == NULL ? "" : m_pRootElement->Attribute(ATTR_VER);
    if(strVersion.IsEmpty())
    {
        m_strErrMsg.Format("Can not find <%s %s=\"\">", ELEM_CODER, ATTR_VER);
        return false;
    }
    if(!IsNumber(strVersion))
    {
        m_strErrMsg.Format("<coder version=\"%s\" > must be number", strVersion);
        return false;
    }

	CString strOldVersion = m_pRootElement->Attribute(ATTR_OLDVER) == NULL ? "0" : m_pRootElement->Attribute(ATTR_OLDVER);
	
	if(strOldVersion.IsEmpty())
	{
		m_strErrMsg.Format("Can not find <%s %s=\"\">", ELEM_CODER, ATTR_OLDVER);
		return false;
	}

	if(!IsNumber(strOldVersion))
	{
		m_strErrMsg.Format("<coder version=\"%s\" > must be number", strVersion);
		return false;
	}

    m_nVersion = atoi(strVersion);
	m_nOldVersion = atoi(strOldVersion);
    return true;
}

bool CParser::_ParseIncludes()
{
    //没有<includes> 返回true，表示没有include
    if (NULL == m_pRootElement)
    {
        return false ;
    }
    TiXmlElement* pIncludesElement = m_pRootElement->FirstChildElement(POS_INCLUDES) ;
    if (pIncludesElement)
    {
        TiXmlElement* pIncludeElement = pIncludesElement->FirstChildElement(ELEM_INCLUDE) ;
        while (pIncludeElement)
        {
            CString strName = pIncludeElement->Attribute(ATTR_FILE) == NULL ? "" : pIncludeElement->Attribute(ATTR_FILE);
            if(strName.IsEmpty())
            {
                m_strErrMsg.Format("element <include> attribute \"file\" is NULL");
                return false;
            }

            SIncludeInfo* pstIncludeInfo = new SIncludeInfo;
            memset(pstIncludeInfo, 0, sizeof(SIncludeInfo)) ;
            lstrcpyn(pstIncludeInfo->szFile, strName, sizeof(pstIncludeInfo->szFile));

            if(false == AddIncludeInfo(pstIncludeInfo))
                return false;
            pIncludeElement = pIncludeElement->NextSiblingElement(ELEM_INCLUDE) ;
        }
    }
    return true;
}

bool CParser::_ParseDefines()
{
    //没有Define 返回true，表示没有语法错误
    if (NULL == m_pRootElement)
    {
        return false ;
    }
    TiXmlElement* pDefinesElement = m_pRootElement->FirstChildElement(POS_DEFINES) ;
    if (pDefinesElement)
    {
        TiXmlElement* pDefineElement = pDefinesElement->FirstChildElement(ELEM_DEFINE) ;
        while (pDefineElement)
        {
            char* cpName = (char*)pDefineElement->Attribute(ATTR_NAME);
            if (NULL == cpName)
            {
                m_strErrMsg.Format("element \"define\" attribute \"name\" is NULL");
                return false;
            }
            char* cpValue = (char*)pDefineElement->Attribute(ATTR_VALUE);
            if (NULL == cpValue)
            {
                m_strErrMsg.Format("element <define> attribute \"value\" is NULL");
                return false;
            }

            SDefineInfo* pstDefineInfo = new SDefineInfo ;
            memset(pstDefineInfo, 0, sizeof(SDefineInfo)) ;
            lstrcpyn(pstDefineInfo->szName, cpName, sizeof(pstDefineInfo->szName));
            lstrcpyn(pstDefineInfo->szValue, cpValue, sizeof(pstDefineInfo->szValue));

            if(false == AddDefineInfo(pstDefineInfo))
                return false;
			if(m_poMainParser)
			{
				if(false == m_poMainParser->AddDefineInfo2All(pstDefineInfo))
					return false;
			}

            pDefineElement = pDefineElement->NextSiblingElement(ELEM_DEFINE) ;
        }
    }
    return true;
}





bool CParser::_ParseTypes()
{
    if (NULL == m_pRootElement)
    {
        return false ;
    }
    TiXmlElement* pTypesElement = m_pRootElement->FirstChildElement(POS_TYPES) ;
    if (pTypesElement)
    {
		CString strComment = "";
		TiXmlComment *pTiXmlComment = NULL;
		TiXmlNode *pTiXmlNode = pTypesElement->FirstChild();
		if (NULL != pTiXmlNode)
		{
			pTiXmlComment = pTiXmlNode->ToComment();
		}
		if (NULL != pTiXmlComment)
		{
			strComment = pTiXmlComment->Value();
		}
		if(em_GBK == g_emCharSetHost)
		{
			ConvertUtf8ToGBK(strComment);
		}

		strComment.TrimLeft();
		strComment.TrimRight();


        TiXmlElement* pTypeElement = pTypesElement->FirstChildElement(ELEM_TYPE) ;
		UINT32 unKeyIndex = 1;
		while (pTypeElement)
        {
            if (false == _ParseAType(pTypeElement, strComment, unKeyIndex++))
            {
                return false ;
            }
            pTypeElement = pTypeElement->NextSiblingElement(ELEM_TYPE) ;
        }
    }

    return true;
}



VOID CParser::_MakeMsgRange()
{
    int nMin = 0;
    int nMax = 0;
    int nCurrentID = 0; // 记录当前的MessageID值，用于生成没有定义ID的Msg的ID
    bool bFirst = true;
    for(int i = 0; i < GetMessageInfoCount(); i++)
    {
        SMessageInfo* pstMessageInfo = GetMessageInfoByPos(i);
        if (bFirst)
        {
            //// 2009-03-27 cwy add and modify for @001
            if (false == pstMessageInfo->bHasID)
            {
                pstMessageInfo->nID = 0;
            }
            nCurrentID = pstMessageInfo->nID;
            nMin = pstMessageInfo->nID;
            nMax = pstMessageInfo->nID;
            bFirst = false;
            //// ~
        }

        //// 2009-03-27 cwy add and modify for @001
        if (false == pstMessageInfo->bHasID)
        {
            pstMessageInfo->nID = nCurrentID + 1;
        }
        nCurrentID = pstMessageInfo->nID;
        //// ~

        if (nMin > pstMessageInfo->nID)
        {
            nMin = pstMessageInfo->nID;
        }
        if (nMax < pstMessageInfo->nID)
        {
            nMax = pstMessageInfo->nID;
        }
    }
    SetMinMsgID(nMin);
    SetMaxMsgID(nMax);
    SetMsgIDRange(nMax - nMin + 1);
}


bool CParser::_ParseAType(TiXmlElement* pTypeElement, CString strComment, UINT32 unKeyIndex)
{
    STypeInfo *pstTypeInfo = new STypeInfo;
    char* cpName = (char*)pTypeElement->Attribute(ATTR_NAME) ;
    if (NULL == cpName)
    {
        m_strErrMsg.Format("element <type> attribute \"name\" is NULL");
        return false;
    }
    lstrcpyn(pstTypeInfo->szComment, strComment.GetBuffer(0), strComment.GetLength() + 1);
    lstrcpyn(pstTypeInfo->szName, cpName, sizeof(pstTypeInfo->szName));

	if(FindTypeInfo(cpName))
	{
		m_strErrMsg.Format("type %s is defined twice or more", cpName);
		return false;
	}

	if(m_bCliEnDe) {	
		static INT32 nSid = 1 + CCoderParamInfo::Instance().m_nSidOffset;
		pstTypeInfo->nStructID = nSid++;
	}

    pstTypeInfo->nSeq = 0;

    char* cpClass = (char*)pTypeElement->Attribute(ATTR_CLASS) ;
    if(cpClass && strcmp(cpClass, CLASS_UNION) ==  0)
    {
        pstTypeInfo->nClassType = CLASS_TYPE_UNION;
    }
    else
    {
        pstTypeInfo->nClassType = CLASS_TYPE_STRUCT;
    }

    CString strItemName;
    CString strType;
    CString strCount;
    CString strRefer;
    CString strSelect;
	CString strKeyIdx;
	strKeyIdx.Format("%u", unKeyIndex);
    CString strID;
    CString strPreFix;
    CString strDef;
    CString strMemberName;
    //// 2009-03-10 cwy add
    CString strNoneFollow;
    CString strIgnore;

    TiXmlNode *		pTiXmlNode = NULL;
    TiXmlElement*	pItemElement = NULL;
    TiXmlComment	*pTiXmlComment = NULL;
    pTiXmlNode = pTypeElement->FirstChild();
    while(pTiXmlNode)
    {
		while(NULL == pItemElement && NULL != pTiXmlNode)
		{
			switch (pTiXmlNode->Type())
			{
			case TiXmlNode::ELEMENT:
				pItemElement = (TiXmlElement*)pTiXmlNode;
				break;
			case TiXmlNode::COMMENT:
				pTiXmlComment = (TiXmlComment*)pTiXmlNode;
				break;
			default:
				break;
			}
			pTiXmlNode = pTiXmlNode->NextSibling();
		}
		if (NULL != pTiXmlComment)
		{
			strComment = pTiXmlComment->Value();
			if(em_GBK == g_emCharSetHost)
			{
				ConvertUtf8ToGBK(strComment);
			}
			strComment.TrimLeft();
			strComment.TrimRight();
		}

		if( NULL==pItemElement )
		{
			break;
		}
        strItemName = pItemElement->Attribute(ATTR_NAME) == NULL ? "" : pItemElement->Attribute(ATTR_NAME);
        if(strItemName.IsEmpty())
        {
            m_strErrMsg.Format("Type %s: item name is NULL", cpName);
            return false;
        }
        strType = pItemElement->Attribute(ATTR_TYPE) == NULL ? "" : pItemElement->Attribute(ATTR_TYPE);
        if(strType.IsEmpty())
        {
            m_strErrMsg.Format("Type %s: item type is NULL", cpName);
            return false;
        }

        strCount = pItemElement->Attribute(ATTR_COUNT) == NULL ? "" : pItemElement->Attribute(ATTR_COUNT);
        strRefer = pItemElement->Attribute(ATTR_REFER) == NULL ? "" : pItemElement->Attribute(ATTR_REFER);
        strSelect = pItemElement->Attribute(ATTR_SELECT) == NULL ? "" : pItemElement->Attribute(ATTR_SELECT);
        strID = pItemElement->Attribute(ATTR_ID) == NULL ? "" : pItemElement->Attribute(ATTR_ID);
        strIgnore = pItemElement->Attribute(ATTR_IGNORE) == NULL ? "" : pItemElement->Attribute(ATTR_IGNORE);
		if (NULL != pItemElement->Attribute(ATTR_KEYIDX)) {
			strKeyIdx = pItemElement->Attribute(ATTR_KEYIDX);
		}

        //// 2009-03-10 cwy add
        strNoneFollow = pItemElement->Attribute(ATTR_NONEFOLLOW) == NULL ? "" : pItemElement->Attribute(ATTR_NONEFOLLOW);
        SItemInfo *pstItemInfo = new SItemInfo;
        memset(pstItemInfo, 0, sizeof(SItemInfo)) ;
        pstItemInfo->nDataType = DATA_UNKNOWN;
        lstrcpyn(pstItemInfo->szCount, strCount, sizeof(pstItemInfo->szCount));
        lstrcpyn(pstItemInfo->szType, strType, sizeof(pstItemInfo->szType));
        lstrcpyn(pstItemInfo->szName, strItemName, sizeof(pstItemInfo->szName));
        lstrcpyn(pstItemInfo->szMemberName, strMemberName, sizeof(pstItemInfo->szMemberName));
        lstrcpyn(pstItemInfo->szRefer, strRefer, sizeof(pstItemInfo->szRefer));		
        lstrcpyn(pstItemInfo->szSelect, strSelect, sizeof(pstItemInfo->szSelect));
		lstrcpyn(pstItemInfo->szKeyIdx, strKeyIdx, sizeof(pstItemInfo->szKeyIdx));
        lstrcpyn(pstItemInfo->szID, strID, sizeof(pstItemInfo->szID));

        //// 2009-03-10 cwy add
        lstrcpyn(pstItemInfo->szNoneFollow, strNoneFollow, sizeof(pstItemInfo->szNoneFollow));

		lstrcpyn(pstItemInfo->szComment, strComment.GetBuffer(0), strComment.GetLength() + 1);


        if (strIgnore == "1")
        {
            pstItemInfo->bIgnored = true ;
            pstTypeInfo->nIgnored ++ ;
        }
        pstTypeInfo->oVecItemInfo.push_back(pstItemInfo);
		pstTypeInfo->oMapItemInfo[pstItemInfo->szName] = pstItemInfo;
		pItemElement = NULL;
		pTiXmlComment = NULL;
    }

    if(!AddTypeInfo(pstTypeInfo))
	{
		return false;
	}
	if(m_poMainParser)
	{
		if(!m_poMainParser->AddTypeInfo2All(pstTypeInfo))
		{
			return false;
		}
	}
    return true;
}

bool CParser::_ParseMessages()
{
    if (NULL == m_pRootElement)
    {
        return false ;
    }
    TiXmlElement* pMessagesElement = m_pRootElement->FirstChildElement(ELEM_MESSAGES) ;
    if (pMessagesElement)
    {
        TiXmlElement* pMessageElement = pMessagesElement->FirstChildElement(ELEM_MESSAGE) ;
        while(pMessageElement)
        {
            CString strName = pMessageElement->Attribute(ATTR_NAME) == NULL ? "" : pMessageElement->Attribute(ATTR_NAME);
            if(strName.IsEmpty())
            {
                m_strErrMsg.Format("element <message> attribute \"name\" is NULL");
                return false;
            }

            CString strID = pMessageElement->Attribute(ATTR_ID) == NULL ? "" : pMessageElement->Attribute(ATTR_ID);

            //// 2009-03-27 cwy modify, @001
            bool bHasID = true;
            if(strID.IsEmpty())
            {
                //m_strErrMsg.Format("element <message> attribute \"id\" is NULL");
                //return false;
                bHasID = false;
            }
            else if(false == IsNumber(strID))
            {
                m_strErrMsg.Format("message %s error: id %s must be number", strName, strID);
                return false;
            }

            CString strType = pMessageElement->Attribute(ATTR_TYPE) == NULL ? "" : pMessageElement->Attribute(ATTR_TYPE);
            if(strType.IsEmpty())
            {
                m_strErrMsg.Format("message %s error: type is NULL", strName);
                return false;
            }

            SMessageInfo* pstMessageInfo = new SMessageInfo;
            memset(pstMessageInfo, 0, sizeof(SMessageInfo)) ;

            //// 2009-03-27 cwy add for @001
            if (false == bHasID)
            {
                pstMessageInfo->bHasID = false;
                pstMessageInfo->nID = 0;
            }
            else
            {
                pstMessageInfo->bHasID = true;
                pstMessageInfo->nID = atoi(strID);
            }
            //// ~

            lstrcpyn(pstMessageInfo->szName, strName, sizeof(pstMessageInfo->szName));
            lstrcpyn(pstMessageInfo->szType, strType, sizeof(pstMessageInfo->szType));

            AddMessageInfo(pstMessageInfo);
            pMessageElement = pMessageElement->NextSiblingElement(ELEM_MESSAGE) ;
        }
    }
    return true;
}

SIncludeInfo* CParser::FindIncludeInfo(const char* pszFile)
{
    for(size_t i = 0; i < m_oVecIncludeInfo.size(); i++)
    {
        SIncludeInfo* pstInfo = m_oVecIncludeInfo[i];
        if(0 == stricmp(pszFile, pstInfo->szFile))
            return pstInfo;
    }

    return NULL;
}

int CParser::GetIncludeInfoCount()
{
    return (int)m_oVecIncludeInfo.size();
}

SIncludeInfo* CParser::GetIncludeInfoByPos(int nPos)
{
    if(nPos < 0 || nPos >= (int)m_oVecIncludeInfo.size())
        return NULL;

    return m_oVecIncludeInfo[nPos];
}

bool CParser::AddIncludeInfo(SIncludeInfo* pstInfo)
{
    if(FindIncludeInfo(pstInfo->szFile) != NULL)
    {
        m_strErrMsg.Format("<include file=%s > already include", pstInfo->szFile);
        return false;
    }

    m_oVecIncludeInfo.push_back(pstInfo);
    return true;
}

SDefineInfo* CParser::FindDefineInfo(const char* pszName)
{
    for(size_t i = 0; i < m_oVecDefineInfo.size(); i++)
    {
        SDefineInfo* pstInfo = m_oVecDefineInfo[i];
        if(strcmp(pstInfo->szName, pszName) == 0)
            return pstInfo;
    }

    return NULL;
}

int CParser::GetDefineInfoCount()
{
    return (int)m_oVecDefineInfo.size();
}

SDefineInfo* CParser::GetDefineInfoByPos(int nPos)
{
    if(nPos < 0 || nPos >= (int)m_oVecDefineInfo.size())
        return NULL;

    return m_oVecDefineInfo[nPos];
}

bool CParser::AddDefineInfo(SDefineInfo* pstInfo)
{
    if(FindDefineInfo(pstInfo->szName) != NULL)
    {
        m_strErrMsg.Format("<define name=%s > already exists", pstInfo->szName);
        return false;
    }

    m_oVecDefineInfo.push_back(pstInfo);

    return true;
}

bool CParser::AddDefineInfo2All(SDefineInfo* pstInfo)
{
	if(m_mapAllDefineInfo.find(pstInfo->szName) != m_mapAllDefineInfo.end())
	{
		m_strErrMsg.Format("<define name=%s > already exists", pstInfo->szName);
		return false;
	}

	m_mapAllDefineInfo[pstInfo->szName] = pstInfo;

	return true;
}


SDefineInfo* CParser::FindDefineInfoFromAll(const char* pszName)
{
	CDefineInfoMapItr itr = m_mapAllDefineInfo.find(pszName);
	if(itr != m_mapAllDefineInfo.end())
	{
		return itr->second;
	}

	return NULL;
}

STypeInfo* CParser::FindTypeInfo(const char* pszName)
{
    for(size_t i = 0; i < m_oVecTypeInfo.size(); i++)
    {
        STypeInfo* pstInfo = m_oVecTypeInfo[i];
        if(0 == strcmp(pstInfo->szName, pszName))
            return pstInfo;
    }

    return NULL;
}

int CParser::GetTypeInfoCount()
{
    return (int)m_oVecTypeInfo.size();
}

STypeInfo* CParser::GetTypeInfoByPos(int nPos)
{
    if(nPos < 0 || nPos >= (int)m_oVecTypeInfo.size())
        return NULL;

    return m_oVecTypeInfo[nPos];
}

bool CParser::AddTypeInfo(STypeInfo* pstInfo)
{
    if(FindTypeInfo(pstInfo->szName) != NULL)
    {
        m_strErrMsg.Format("<type name=%s > already exists", pstInfo->szName);
        return false;
    }

    m_oVecTypeInfo.push_back(pstInfo);
    return true;
}

bool CParser::AddTypeInfo2All(STypeInfo* pstInfo)
{
	if(m_mapAllTypeInfo.find(pstInfo->szName) != m_mapAllTypeInfo.end())
	{
		m_strErrMsg.Format("<type name=%s > already exists", pstInfo->szName);
		return false;
	}

	m_mapAllTypeInfo[pstInfo->szName] = pstInfo;
	return true;
}

STypeInfo* CParser::FindTypeInfoFromAll(const char* pszName)
{
	CTypeInfoMapItr itr = m_mapAllTypeInfo.find((pszName));
	if(itr != m_mapAllTypeInfo.end())
	{
		return itr->second;
	}
	
	return NULL;
}


SMessageInfo* CParser::FindMessageInfo(const char* pszName)
{
    for(size_t i = 0; i < m_oVecMessageInfo.size(); i++)
    {
        SMessageInfo* pstInfo = m_oVecMessageInfo[i];
        if(0 == strcmp(pstInfo->szName, pszName))
            return pstInfo;
    }

    return NULL;
}

int CParser::GetMessageInfoCount()
{
    return (int)m_oVecMessageInfo.size();
}

SMessageInfo* CParser::GetMessageInfoByPos(int nPos)
{
    if(nPos < 0 || nPos >= (int)m_oVecMessageInfo.size())
        return NULL;

    return m_oVecMessageInfo[nPos];
}

bool CParser::AddMessageInfo(SMessageInfo* pstInfo)
{
    m_oVecMessageInfo.push_back(pstInfo);
    return true;
}

int CParser::GetMinMsgID()
{
    return m_nMinID;
}

int CParser::GetMaxMsgID()
{
    return m_nMaxID;
}

int CParser::GetMsgIDRange()
{
    return m_nMsgIDRange;
}

void CParser::SetMinMsgID( int nMsgID )
{
    m_nMinID = nMsgID;
}

void CParser::SetMaxMsgID( int nMsgID )
{
    m_nMaxID = nMsgID;
}

void CParser::SetMsgIDRange( int nRange )
{
    m_nMsgIDRange = nRange;
}
