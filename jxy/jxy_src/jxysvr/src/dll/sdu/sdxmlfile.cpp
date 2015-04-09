#include "sdxmlfile.h"

using namespace SGDP;

namespace SGDP
{
    //CSDXMLNode
    CSDXMLNode::CSDXMLNode()
    {
        m_pElement = NULL;
    }

    CSDXMLNode::CSDXMLNode(const CSDXMLNode& xmlNode)
    {
        m_pElement = xmlNode.m_pElement;
    }

    CSDXMLNode CSDXMLNode::operator [] (const CHAR* pszNodeName)
    {
        if (NULL == m_pElement)
        {
            CSDXMLNode tempNode;
            return tempNode;
        }
        CSDXMLNode tempNode;
        //这里应该分析pszNode，如"node1/node2/node3"
        std::string str = pszNodeName;
        std::vector<std::string> vNodeStr = SDSplitStringsA(str,'/');
        TiXmlElement* tempElement = m_pElement->FirstChildElement(vNodeStr[0].c_str());
        if(tempElement != NULL)
        {
            for(INT32 i=1;i<(INT32)vNodeStr.size();i++)
            {
                tempElement = tempElement->FirstChildElement(vNodeStr[i].c_str());
                if(tempElement == NULL)
                {
                    break;
                }
            }
            tempNode.m_pElement = tempElement;
        }
        return tempNode;
    }

    CSDXMLNode::CXMLValue CSDXMLNode::operator ()(const CHAR* pszAttrName)
    {
        if (NULL == m_pElement)
        {
            return CSDXMLNode::CXMLValue();
        }
        return CSDXMLNode::CXMLValue(m_pElement->Attribute(pszAttrName));
    }

    BOOL CSDXMLNode::operator != ( const CSDXMLNode* node )
    {
		if(node == NULL)
		{
			return (m_pElement !=  NULL);
		}
		else
		{
			return (m_pElement != node->m_pElement);
		}
    }

    BOOL CSDXMLNode::operator == ( const CSDXMLNode* node )
    {
		if(node == NULL)
		{
			return (m_pElement ==  NULL);
		}
		else
		{
			return (m_pElement == node->m_pElement);
		}
    }

    std::string CSDXMLNode::AsString(const CHAR* pszString)
    {
        if (NULL == m_pElement)
        {
            return pszString;
        }
        const CHAR* pszValue = m_pElement->GetText();
        if( pszValue==NULL || strlen(pszValue)==0 )
            return pszString;
        return pszValue;
    }


    INT32 CSDXMLNode::AsInteger(INT32 nInteger)
    {
        if (NULL == m_pElement)
        {
            return nInteger;
        }
        const CHAR* pszValue = m_pElement->GetText();
        if( pszValue==NULL || strlen(pszValue)==0 )
            return nInteger;
        return SDAtoi(pszValue);
    }

    FLOAT CSDXMLNode::AsFloat(FLOAT fFloat)
    {
        if (NULL == m_pElement)
        {
            return fFloat;
        }
        const CHAR* pszValue = m_pElement->GetText();
        if( pszValue==NULL || strlen(pszValue)==0 )
            return fFloat;
        return (FLOAT)atof(pszValue);
    }

    BOOL CSDXMLNode::AsBoolean(BOOL bBoolean)
    {
        if (NULL == m_pElement)
        {
            return bBoolean;
        }
        const CHAR* pszValue = m_pElement->GetText();
        if( pszValue==NULL || strlen(pszValue)==0 )
            return bBoolean;
        if(SDStrcmp(pszValue,"true")==0)
            return TRUE;
        else if(SDStrcmp(pszValue,"false")==0)
            return FALSE;
        else
        {
            return FALSE;
        }
    }

    DOUBLE CSDXMLNode::AsDouble(DOUBLE dDouble)
    {
        if (NULL == m_pElement)
        {
            return dDouble;
        }
        const CHAR* pszValue = m_pElement->GetText();
        if( pszValue==NULL || strlen(pszValue)==0 )
            return dDouble;
        return atof(pszValue);
    }

    CSDXMLNode CSDXMLNode::Sibling()
    {
        CSDXMLNode tempNode;
        tempNode.m_pElement = m_pElement->NextSiblingElement();
        return tempNode;
    }

    CSDXMLNode CSDXMLNode::Sibling(const CHAR* szNext)
    {
        CSDXMLNode tempNode;
        tempNode.m_pElement = m_pElement->NextSiblingElement(szNext);
        return tempNode;
    }

    //CXMLValue
    CSDXMLNode::CXMLValue::CXMLValue()
    {
        m_pszValue = NULL;
    }

    CSDXMLNode::CXMLValue::CXMLValue(const CHAR* pszValue)
    {
        m_pszValue = pszValue;
    }

    CSDXMLNode::CXMLValue::CXMLValue(const CXMLValue& v)
    {
        this->m_pszValue = v.m_pszValue;
    }

    std::string CSDXMLNode::CXMLValue::AsString(const CHAR* pszString)
    {
        //if( m_pszValue==NULL || strlen(m_pszValue)==0 )
		if(m_pszValue==NULL)
            return pszString;
        return m_pszValue;
    }

    INT32 CSDXMLNode::CXMLValue::AsInteger(INT32 nInteger)
    {
        if( m_pszValue==NULL || strlen(m_pszValue)==0 )
            return nInteger;
        return SDAtoi(m_pszValue);
    }

    FLOAT CSDXMLNode::CXMLValue::AsFloat(FLOAT fFloat)
    {
        if( m_pszValue==NULL || strlen(m_pszValue)==0 )
            return fFloat;
        return (FLOAT)atof(m_pszValue);
    }

    BOOL CSDXMLNode::CXMLValue::AsBoolean(BOOL bBoolean)
    {
        if( m_pszValue==NULL || strlen(m_pszValue)==0 )
            return bBoolean;
        if(SDStrcmp(m_pszValue,"true")==0)
            return TRUE;
        else if(SDStrcmp(m_pszValue,"false")==0)
            return FALSE;
        else
        {
            return FALSE;
        }
    }

    DOUBLE CSDXMLNode::CXMLValue::AsDouble(DOUBLE dDouble)
    {
        if( m_pszValue==NULL || strlen(m_pszValue)==0 )
            return dDouble;
        return atof(m_pszValue);
    }

    //CSDXMLFile
    CSDXMLFile::CSDXMLFile()
    {
        m_pDocument = NULL;
    }

    CSDXMLFile::~CSDXMLFile()
    {
        UnLoad();
    }

    CSDXMLFile::CSDXMLFile(const CHAR* pszXmlFile)
    {
        Load(pszXmlFile);
    }

    BOOL CSDXMLFile::Load(const CHAR* pszXmlFile)
    {
		if(NULL == pszXmlFile)
		{
			return FALSE;
		}
        UnLoad();
        m_pDocument = SDNew TiXmlDocument();
        if(pszXmlFile!=NULL)
        {
            if(m_pDocument->LoadFile(pszXmlFile))
            {
                return TRUE;
            }
        }
        UnLoad();
        return FALSE;
    }

	BOOL CSDXMLFile::LoadData(const CHAR* pszData, const UINT32 dwDataLen)
	{
		if(NULL == pszData)
		{
			return FALSE;
		}
		UnLoad();
		m_pDocument = SDNew TiXmlDocument();
		if(m_pDocument->LoadData(pszData, dwDataLen))
		{
			return TRUE;
		}		
		UnLoad();
		return FALSE;
	}

    VOID CSDXMLFile::UnLoad()
    {
        if(m_pDocument != NULL)
        {
            SDDelete m_pDocument;
            m_pDocument = NULL;
        }
    }

    CSDXMLNode CSDXMLFile::GetRootNode()
    {
        SetElement(m_pDocument->RootElement());
        return *this;
    }
    
    BOOL CSDXMLFile::IsValid()
    {
        return m_pDocument ? TRUE : FALSE;
    }
}

