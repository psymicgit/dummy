#pragma once
#include "tinyxml.h"
#include "ClassMaker.h"
#include "Parser.h"
#include <vector>
#include <map>
#include <string>

class CCompiler;

typedef std::map<std::string, int> CStrMap;
typedef std::vector<CCompiler*> CCompilerVec;

typedef CMap<CString,LPCTSTR,CString,LPCTSTR> CStringMap;
typedef CMap<CString,LPCTSTR,int,int> CStructNameMap;

class CCompiler  
{
public:
    CCompiler(INT32 nType, INT32 nCharsetNet, INT32 nCharsetHost);
	void SetHFilePath(LPCTSTR pszPath);
	void SetCppFilePath(LPCTSTR pszPath);
	bool Compile(LPCTSTR pszFile);
	CString &GetErrMsg(){ return m_strErrMsg; }
	CCompiler();

	virtual ~CCompiler();
    void AddChild(CCompiler* poCompiler);
    CCompilerVec* GetChildren();

protected:
	bool _Parse(LPCTSTR pszFile);
	bool _Check();
	bool _CheckName();
	bool _CheckTypes();
	bool _CheckStructInfo(STypeInfo* pstTypeInfo);
	bool _CheckUnionInfo(STypeInfo* pstTypeInfo);
	bool _CheckRefer(LPCTSTR pszRefer, std::vector<SItemInfo*>* poVecItem, int nSelf);
	bool _CheckSelect(LPCTSTR pszSelect, std::vector<SItemInfo*>* poVecItem, int nSelf);
	bool _CheckNoneFollow(LPCTSTR pszNoneFollow);
	bool _CheckMessages();
	int _FindDataType(LPCTSTR pszType, int nSeq, CString &strPrefix);

protected:
	CParser			m_oMainParser;
	CParserVec		m_oVecChildParsers;
	CDefineInfoMap	m_oDefineInfoMap;
	CTypeInfoMap	m_oMapTypeInfo;
	int				m_nTypeSeq;
	CString			m_strErrMsg;
	CClassMaker		m_oClassMaker;
	CString			m_strHPilePath;
	CString         m_strCppFilePath;
    CCompilerVec    m_vecChildren;
    INT32           m_nType;
    INT32           m_nCharsetNet;
    INT32           m_nCharsetHost;
};