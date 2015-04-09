#pragma once 
#include "tinyxml.h"
#include "asprotocolsource.h"
/************************************************************************/
/* class CLoader                                                                     
	function : load protocol definitions from a specify file, check the syntax error*/
/************************************************************************/
class CASLoader
{
public:
	CASLoader()
	{
		m_pRootElement = NULL ;
		m_pDocument = NULL ;
	}
	~CASLoader()
	{
		if (m_pDocument)
		{
			delete m_pDocument ;
		}
	}
	bool Load(CString& strFilePath, CASProtocolSource* poProto);
	inline CString GetErrString() { return m_strError;}
protected:
	bool _SavePos();
	bool _LoadCoder();
	bool _LoadInclude();
	bool _LoadDefines();
	bool _LoadTypes();
	bool _LoadAType(TiXmlElement* pTypeElement);
	bool _LoadMessages();
protected:
	CASProtocolSource* m_poProto;
	CString m_strError;
	TiXmlDocument*	m_pDocument ;
	TiXmlElement*   m_pRootElement ;
};