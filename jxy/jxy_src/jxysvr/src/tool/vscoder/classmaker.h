#pragma once
#include "afxtempl.h"
#include "Parser.h"
#include <string>
using namespace std;

#define MAX_MESSAGE_ID	65536

typedef struct __ST_StructInfo
{
	int iType;
	char szCount[128];
	char szType[128];
	char szName[128];
	char szMemberName[128];
	char szReferMemberName[128];
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
    void SetInfo(INT32 nType, INT32 nCharsetNet, INT32 nCharsetHost, BOOL bCliEnDe=FALSE, BOOL bSaveEnDe = FALSE);

protected:
	bool _MakeName();
	bool _OpenFile( CString strHFilepath,CString strCppFilePath);
	bool _OpenFileEx();

	bool _WriteHeaderToH(LPCTSTR pszName, int nVersion);
	bool _WriteHeaderToHEx(LPCTSTR pszName, int nVersion);
	bool _WriteIncludeToH();
	bool _WriteIncludeToHEx();
	bool _WriteDefinesToH();
	bool _WriteDefinesToHEx();
	bool _WriteTypesToH();
	bool _WriteMaxEnLenToH();
	bool _WriteTypesToHEx();
	bool _WriteMessageIDToH();
	bool _WriteCodeFuncToH();
	bool _WriteCodeFuncToHEx();
	bool _WriteClassDefToH();
	bool _WriteClassDefToHEx();
	bool _WriteTailToHEx();

	bool _WriteIncludeToCpp();
	bool _WriteIncludeToCppEx();
	bool _WriteCommonPartToCpp();
	bool _WriteAllCodeFuncToCpp();
	bool _WriteAllCodeFuncToCppEx();
	bool _WriteEncodeFuncToCpp(STypeInfo* pstTypeInfo);
	bool _WriteGetJsonFuncToCpp(STypeInfo* pstTypeInfo);
	bool _WriteFromJsonFuncToCpp(STypeInfo* pstTypeInfo);
	bool _WriteSaveEncodeFuncToCpp(STypeInfo* pstTypeInfo);
	bool _WriteSaveDecodeFuncToCpp(STypeInfo* pstTypeInfo);
	bool _WriteDecodeFuncToCpp(STypeInfo* pstTypeInfo);
	bool _WriteEncodeUnionToCpp(STypeInfo* pstTypeInfo);
	bool _WriteDecodeUnionToCpp(STypeInfo* pstTypeInfo);
	bool _WriteEncodeItemToCpp(SItemInfo* pstInfo,bool bIsIngnored = false );
	bool _WriteDecodeItemToCpp(SItemInfo* pstInfo,bool bIsIngnored = false);	

	bool _WriteCliDecodeFuncToCpp(STypeInfo* pstTypeInfo);
	bool _WriteCliEncodeFuncToCpp(STypeInfo* pstTypeInfo);

	//// add 'bToUnion' parameters to functions, by cwy 2009-03-11 
	bool _WriteEncodeItemToUnion(SItemInfo* pstInfo);
	bool _WriteDecodeItemToUnion(SItemInfo* pstInfo);
	
	
	
	bool _WriteEqualFuncToCpp(STypeInfo* pstTypeInfo);
	bool _WriteEqualUnionToCpp(STypeInfo* pstTypeInfo);
	bool _WriteEqualItemToCpp(SItemInfo* pstInfo);

	//// add 'bToUnion' parameters to functions, by cwy 2009-03-11 
	bool _WriteEqualItemToUnion(SItemInfo* pstInfo);

	bool _GetDef(SItemInfo* pstItem, CString &strDef);

	CString _GetSimpleDef(int nDataType);

private:
	string GetCountStr( const string &strCountName, const string &strReferName );
protected:
	CParser*	m_poMainParser;
	CParserVec*	m_poVecChildParser;
	CString		m_strErrMsg;

	FILE*		m_fpCpp;
	FILE*		m_fpH;

	FILE*		m_fpCppEx;
	FILE*		m_fpHEx;

	FILE*		m_fpCppHandle;
	FILE*		m_fpHHandle;

	CString		m_strClassName;
	CString		m_strClassNameEx;
	CString		m_strEnFuncDef;
	CString		m_strDeFuncDef;
	CString		m_strLowerName;
	CString		m_strUpperName;
	
	CString		m_strNewNameSpace;
	CString		m_strOldNameSpace;
	CString		m_strHandleNameSpace;
	CString		m_strPath;

    INT32       m_nType;
    INT32       m_nCharsetNet;
    INT32       m_nCharsetHost;
	BOOL		m_bJson;
	BOOL		m_bCliEnDe;
	BOOL		m_bSaveEnDe;
	const static string m_strParamName;
};
