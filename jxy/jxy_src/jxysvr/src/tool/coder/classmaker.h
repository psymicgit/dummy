#pragma once
#include "afxtempl.h"
#include "Parser.h"

#define MAX_MESSAGE_ID	65536

typedef struct __ST_StructInfo
{
	int iType;
	char szCount[128];
	char szType[128];
	char szName[128];
	char szMemberName[128];
	char szRefer[128];
	char szSelect[128];
	char szID[128];
}ST_STRUCTINFO;

typedef struct __ST_MessageInfo
{
	char szName[128];
	char szType[128];
}ST_MESSAGEINFO;

typedef CArray<ST_STRUCTINFO, ST_STRUCTINFO> CStructInfoArray;
typedef CArray<ST_MESSAGEINFO, ST_MESSAGEINFO> CMessageInfoArray;

class CClassMaker  
{
public:
	CClassMaker();
	virtual ~CClassMaker();

	bool Make(CParser* poMainParser, CParserVec* poChildParser, CString strHFilepath,CString strCppFilePath);
	CString &GetErrMsg(){ return m_strErrMsg; }
    void SetInfo(INT32 nType, INT32 nCharsetNet, INT32 nCharsetHost);

protected:
	bool _MakeName();
	bool _OpenFile( CString strHFilepath,CString strCppFilePath);
	bool _WriteHeaderToH(LPCTSTR pszName, int nVersion);
	bool _WriteIncludeToH();
	bool _WriteDefinesToH();
	bool _WriteTypesToH();
	bool _WriteMessageIDToH();
	bool _WriteCodeFuncToH();
	bool _WriteClassDefToH();
	bool _WriteIncludeToCpp();
	bool _WriteCommonPartToCpp();
	bool _WriteAllCodeFuncToCpp();
	bool _WriteEncodeFuncToCpp(STypeInfo* pstTypeInfo);
	bool _WriteDecodeFuncToCpp(STypeInfo* pstTypeInfo);
	bool _WriteEncodeUnionToCpp(STypeInfo* pstTypeInfo);
	bool _WriteDecodeUnionToCpp(STypeInfo* pstTypeInfo);
	bool _WriteEncodeItemToCpp(SItemInfo* pstInfo,bool bIsIngnored = false );
	bool _WriteDecodeItemToCpp(SItemInfo* pstInfo,bool bIsIngnored = false);

	//// add 'bToUnion' parameters to functions, by cwy 2009-03-11 
	bool _WriteEncodeItemToUnion(SItemInfo* pstInfo);
	bool _WriteDecodeItemToUnion(SItemInfo* pstInfo);

	bool _GetDef(SItemInfo* pstItem, CString &strDef);


protected:
	CParser*	m_poMainParser;
	CParserVec*	m_poVecChildParser;
	CString		m_strErrMsg;

	FILE*		m_fpCpp;
	FILE*		m_fpH;
	CString		m_strClassName;
	CString		m_strEnFuncDef;
	CString		m_strDeFuncDef;
	CString		m_strLowerName;
	CString		m_strUpperName;

    INT32       m_nType;
    INT32       m_nCharsetNet;
    INT32       m_nCharsetHost;
};
