#pragma once 
/************************************************************************/
/* class CChecker
	function : check the semantic errors */
/************************************************************************/
#include "asprotocolsource.h"

class CASChecker
{
public:
	CASChecker(){}
	~CASChecker(){}

	bool Check(CASProtocolSource* poProto);
	inline CString GetErrString() { return m_strError;}
protected:
	bool _CheckDefines();
	bool _CheckTypes();
	bool _CheckType(CASType* pType, int nPos);
	bool _CheckMessages();
	bool _SetGenerateClass();

	bool _IsValidType(CASType* pType, int nPos, CASTypeItem* pItem);
	bool _IsValidCount(CString& strCount, CASTypeItem* pItem);
	bool _IsValidRefer(CASType* pType, CASTypeItem* pItem, int nPos);
	void _SetGenerate(CASType* pType);
protected:
	CASProtocolSource* m_poProto;
	CString m_strError;
};