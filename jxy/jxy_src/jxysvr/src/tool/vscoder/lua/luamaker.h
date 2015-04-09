#pragma once
#include "afxtempl.h"
#include "Parser.h"
#include "classmaker.h"
#include <string>
using namespace std;

#define MAX_MESSAGE_ID	65536

class CLuaMaker  
{
public:
	CLuaMaker();
	virtual ~CLuaMaker();

	bool Make(CParser* poMainParser, CParserVec* poChildParser, CString strFilepath);
	CString &GetErrMsg(){ return m_strErrMsg; }
	void SetInfo(INT32 nType, INT32 nCharsetNet, INT32 nCharsetHost);

protected:
	bool _MakeName();
	bool _OpenFile( CString strFilepath);
	bool _WriteHeaderToLua(LPCTSTR pszName, int nVersion);
	bool _WriteIncludeToLua();
	bool _WriteDefinesToLua();
	bool _WriteTypesToLua();
	bool _WriteMessageIDToLua();
	bool _WriteAllCodeFuncToLua();
	bool _WriteCodeMapFuncToLua();

	////////////////////////////////////////////
	bool _WriteEncodeFuncToLua(STypeInfo* pstTypeInfoe);
	bool _WriteDecodeFuncToLua(STypeInfo* pstTypeInfo);
	bool _WriteEncodeUnionToLua(STypeInfo* pstTypeInfo);
	bool _WriteDecodeUnionToLua(STypeInfo* pstTypeInfo);
	bool _WriteEncodeItemToLua(SItemInfo* pstInfo, char* pszStructName);
	bool _WriteDecodeItemToLua(SItemInfo* pstInfo, char* pszStructName);

	bool _WriteEncodeItemToUnion(SItemInfo* pstInfo, char* pszStructName);
	bool _WriteDecodeItemToUnion(SItemInfo* pstInfo, char* pszStructName);

protected:
	CParser*	m_poMainParser;
	CParserVec*	m_poVecChildParser;
	CString		m_strErrMsg;

	FILE*		m_fpLua;
	CString		m_strClassName;
	CString		m_strEnFuncDef;
	CString		m_strDeFuncDef;
	CString		m_strLowerName;
	CString		m_strUpperName;

	INT32       m_nType;
	INT32       m_nCharsetNet;
	INT32       m_nCharsetHost;
};


