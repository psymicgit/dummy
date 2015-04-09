#pragma once 

/************************************************************************/
/* class CMaker
	function : generate three files of the protocol*/
/************************************************************************/
#include "asprotocolsource.h"

class CASMaker
{
public:
	CASMaker(){}
	~CASMaker(){}
	void SetOutPutFile(CString& strOutPath){m_strPath = strOutPath;}
	bool Make(CASProtocolSource* poProto);
	inline CString GetErrString(){ return m_strError;}
protected:
	bool _MakeDefinitionFile();
	bool _MakeFuncDeclareFile();
	bool _MakeHandlerFile();
	bool _MakeStructFiles();
	bool _MakeStructListFiles();
	bool _MakeAStructFile(CASType* pType);
	bool _MakeAListFile(CASType* pType);
	void _WriteImportPart(FILE* fp);

protected:
	CASProtocolSource* m_poProto;
	CString m_strError;
	CString m_strPath;
};