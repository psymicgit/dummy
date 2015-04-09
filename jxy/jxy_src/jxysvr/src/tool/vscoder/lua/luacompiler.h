#pragma once
#include "tinyxml.h"
#include "luamaker.h"
#include "compiler.h"
#include "Parser.h"
#include <vector>
#include <map>
#include <string>

class CLuaCompiler;

typedef std::vector<CLuaCompiler*> CLuaCompilerVec;
class CLuaCompiler  
{
public:
    CLuaCompiler(INT32 nType, INT32 nCharsetNet, INT32 nCharsetHost);
	void SetFilePath(LPCTSTR pszPath);

	bool Compile(LPCTSTR pszFile);
	CString &GetErrMsg(){ return m_strErrMsg; }
	CLuaCompiler();

	virtual ~CLuaCompiler();
    void AddChild(CLuaCompiler* poCompiler);
    CLuaCompilerVec* GetChildren();

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
	CTypeInfoMap		m_oMapTypeInfo;
	int				m_nTypeSeq;
	CString			m_strErrMsg;
	CLuaMaker		m_oLuaMaker;
	CString			m_strFilePath;
    CLuaCompilerVec	m_vecChildren;
    INT32           m_nType;
    INT32           m_nCharsetNet;
    INT32           m_nCharsetHost;
};