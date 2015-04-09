//parser.h
//
// Modify Record
// @001 : 为生成的enum类型的MessageID提供默认值的支持。 
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <vector>
#include <map>
#include "tinyxml.h"

#define MAX_NAME_LEN	128

enum EClassType
{
	CLASS_TYPE_UNKNOWN = 0,
	CLASS_TYPE_STRUCT,
	CLASS_TYPE_UNION,
};

enum EDataType
{
	DATA_UNKNOWN = 0,
	DATA_CHAR,
	DATA_BYTE,
	DATA_SHORT,
	DATA_WORD,
	DATA_INT,
	DATA_DWORD,
	DATA_INT64,
	DATA_UINT64,
	DATA_FLOAT,
	DATA_STRING,
	DATA_STRUCT,
	DATA_UNION,
    DATA_WSTRING,
    DATA_TSTRING,
};

struct SIncludeInfo
{
	char	szFile[MAX_NAME_LEN];
};

struct SDefineInfo
{
	char	szName[MAX_NAME_LEN];
	char	szValue[MAX_NAME_LEN];
};

struct SItemInfo
{
	SItemInfo()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(SItemInfo));
	}
	int		nDataType;
	char	szName[MAX_NAME_LEN];
	char	szCount[MAX_NAME_LEN];
	char	szType[MAX_NAME_LEN];
	char	szMemberName[MAX_NAME_LEN];
	char	szRefer[MAX_NAME_LEN];
	char	szSelect[MAX_NAME_LEN];
	char	szID[MAX_NAME_LEN];
	char	szNoneFollow[MAX_NAME_LEN];
	bool    bIgnored ;
	char		szComment[MAX_NAME_LEN * 10];
};

struct STypeInfo
{
	char		szComment[MAX_NAME_LEN * 10];
	char		szName[MAX_NAME_LEN];
	int		nClassType;
	int		nSeq;
	int     nIgnored ;
	std::vector<SItemInfo*>	oVecItemInfo;
public:
	STypeInfo()
	{
		memset(szComment,0,MAX_NAME_LEN) ;
		memset(szName,0,MAX_NAME_LEN) ;
		nClassType = 0 ;
		nSeq = 0 ;
		nIgnored = 0 ;
		oVecItemInfo.clear() ;
	}
	~STypeInfo()
	{

	}
};

struct SMessageInfo
{
	int		nID;
	char	szName[MAX_NAME_LEN];
	char	szType[MAX_NAME_LEN];
	//// 2009-03-27 cwy add for @001
	bool	bHasID;
};

class CParser;

typedef std::vector<SIncludeInfo*> CIncludeInfoVec;
typedef std::vector<SDefineInfo*> CDefineInfoVec;
typedef std::vector<STypeInfo*> CTypeInfoVec;
typedef std::vector<SMessageInfo*> CMessageInfoVec;
typedef std::map<std::string, SDefineInfo*> CDefineInfoMap;
typedef std::map<std::string, STypeInfo*> CTypeInfoMap;
typedef std::vector<CParser*> CParserVec;

class CParser
{
public:
	CParser();
	~CParser();

	bool Parse(LPCTSTR pszXml);
	CString& GetErrMsg() { return m_strErrMsg; }

	const char* GetName() { return m_szName; }
	int GetVersion() { return m_nVersion; }

	SIncludeInfo* FindIncludeInfo(const char* pszFile);
	int GetIncludeInfoCount();
	SIncludeInfo* GetIncludeInfoByPos(int nPos);
	bool AddIncludeInfo(SIncludeInfo* pstInfo);

	SDefineInfo* FindDefineInfo(const char* pszName);
	int GetDefineInfoCount();
	SDefineInfo* GetDefineInfoByPos(int nPos);
	bool AddDefineInfo(SDefineInfo* pstInfo);

	STypeInfo* FindTypeInfo(const char* pszName);
	int GetTypeInfoCount();
	STypeInfo* GetTypeInfoByPos(int nPos);
	bool AddTypeInfo(STypeInfo* pstInfo);

	SMessageInfo* FindMessageInfo(const char* pszName);
	int GetMessageInfoCount();
	SMessageInfo* GetMessageInfoByPos(int nPos);
	bool AddMessageInfo(SMessageInfo* pstInfo);

	void SetMinMsgID(int nMsgID);
	int GetMinMsgID();
	void SetMaxMsgID(int nMsgID);
	int GetMaxMsgID();
	void SetMsgIDRange(int nRange);
	int GetMsgIDRange();

	void  Release() ;
protected:
	bool _SavePos();
	bool _ParseCoder();
	bool _ParseIncludes();
	bool _ParseDefines();
	bool _ParseMessages();
	bool _ParseTypes();
	bool _ParseAType(TiXmlElement* pTypeElement, CString strComment);
    VOID _MakeMsgRange();
protected:
	char				m_szName[MAX_NAME_LEN];
	int				m_nVersion ;
	CIncludeInfoVec	m_oVecIncludeInfo ;
	CDefineInfoVec	m_oVecDefineInfo ;
	CTypeInfoVec		m_oVecTypeInfo ;
	CMessageInfoVec	m_oVecMessageInfo ;
	CString			m_strErrMsg ;
	TiXmlDocument*	m_pDocument ;
	TiXmlElement*		m_pRootElement ;

	int				m_nMinID;
	int				m_nMaxID;
	int				m_nMsgIDRange;
};
