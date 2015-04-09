#include "stdafx.h"

#include "vscoder.h"
#include "ClassMaker.h"
#include "Parser.h"
#include <string>
#include <algorithm>
using namespace std;

static LPCTSTR DEF_CHAR		= "CHAR";
static LPCTSTR DEF_BYTE		= "UINT8";
static LPCTSTR DEF_SHORT		= "INT16";
static LPCTSTR DEF_WORD		= "UINT16";
static LPCTSTR DEF_INT			= "INT32";
static LPCTSTR DEF_DWORD		= "UINT32";
static LPCTSTR DEF_INT64		= "INT64";
static LPCTSTR DEF_UINT64		= "UINT64";
static LPCTSTR DEF_FLOAT		= "float";
static LPCTSTR DEF_STRING		= "CHAR";
static LPCTSTR DEF_WCHAR		= "WCHAR";
static LPCTSTR DEF_WSTRING		= "WCHAR";
static LPCTSTR DEF_TCHAR		= "TCHAR";
static LPCTSTR DEF_TSTRING		= "TCHAR";
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const string CClassMaker::m_strParamName="data";

CClassMaker::CClassMaker()
{
	m_poMainParser		= NULL;
	m_poVecChildParser	= NULL;
	m_strErrMsg = "";
	m_fpCpp		=	NULL;
	m_fpH		=	NULL;

	m_fpCppEx		=	NULL;
	m_fpHEx		=	NULL;

	m_fpCppHandle = NULL;
	m_fpHHandle	  =	NULL;
	m_bJson=FALSE;
	m_bSaveEnDe = FALSE;
}

CClassMaker::~CClassMaker()
{
	if(m_fpH != NULL)
		fclose(m_fpH);

	if(m_fpCpp != NULL)
		fclose(m_fpCpp);

	if(m_fpHEx != NULL)
		fclose(m_fpHEx);

	if(m_fpCppEx != NULL)
		fclose(m_fpCppEx);

	if(m_fpHHandle != NULL)
		fclose(m_fpHHandle);

	if(m_fpCppHandle != NULL)
		fclose(m_fpCppHandle);
}

void CClassMaker::SetInfo(INT32 nType, INT32 nCharsetNet, INT32 nCharsetHost, BOOL bCliEnDe, BOOL bSaveEnDe)
{
	m_nType        = nType;
	m_nCharsetNet  = nCharsetNet;
	m_nCharsetHost = nCharsetHost;
	m_bCliEnDe = bCliEnDe;
	m_bSaveEnDe = bSaveEnDe;
	m_bJson		= m_bSaveEnDe;//
}

bool CClassMaker::Make(CParser* poMainParser, CParserVec* poChildParser, CString strHFilepath, CString strCppFilePath)
{
	m_strErrMsg.Format("Make .cpp and .h failed!");

	m_poMainParser		= poMainParser;
	m_poVecChildParser	= poChildParser;

	int nIndex = strHFilepath.ReverseFind('\\');
	m_strPath =  strHFilepath.Left(nIndex);
	m_strPath += "\\";

	m_strNewNameSpace.Format("%s_VERSION_%d", poMainParser->GetName(), poMainParser->GetVersion());
	m_strNewNameSpace.MakeUpper();

	m_strOldNameSpace.Format("%s_VERSION_%d", poMainParser->GetName(), poMainParser->GetOldVersion());
	m_strOldNameSpace.MakeUpper();

	m_strHandleNameSpace.Format("%s_VERSION_%d_%d", poMainParser->GetName(), poMainParser->GetOldVersion(), poMainParser->GetVersion());
	m_strHandleNameSpace.MakeUpper();


	m_strClassName.Format("C%s", poMainParser->GetName());
	m_strClassNameEx.Format("C%s_%d", poMainParser->GetName(), poMainParser->GetVersion());


	//// 2009-03-27 cwy add
	if (false == _MakeName())
	{
		return false;
	}

	if(false == _OpenFile(strHFilepath, strCppFilePath))
		return false;

	if(false == _WriteHeaderToH(poMainParser->GetName(), poMainParser->GetVersion()))
		return false;

	if(false == _WriteIncludeToH())
		return false;

	if(false == _WriteDefinesToH())
		return false;

	if(false == _WriteTypesToH())
		return false;

	if(false == _WriteMaxEnLenToH())
		return false;

	if(false == _WriteMessageIDToH())
		return false;

	if(false == _WriteCodeFuncToH())
		return false;

	if(false == _WriteClassDefToH())
		return false;

	if(false == _WriteIncludeToCpp())
		return false;

	if(false == _WriteCommonPartToCpp())
		return false;

	if(false == _WriteAllCodeFuncToCpp())
		return false;

	if (!m_bCompare)
	{
		return true;
	}

	if(false == _OpenFileEx())
		return false;

	if(false == _WriteHeaderToHEx(poMainParser->GetName(), poMainParser->GetVersion()))
		return false;

	if(false == _WriteIncludeToHEx())
		return false;


	if(false == _WriteDefinesToHEx())
		return false;

	if(false == _WriteTypesToHEx())
		return false;

	if(false == _WriteClassDefToHEx())
		return false;

	if(false == _WriteTailToHEx())
		return false;

	if(false == _WriteIncludeToCppEx())
		return false;


	if(m_poMainParser->GetOldVersion() == m_poMainParser->GetVersion())
	{
		return true;
	}

	if(false == _WriteCodeFuncToHEx())
		return false;

	if(false == _WriteAllCodeFuncToCppEx())
		return false;



	return true;
}

bool CClassMaker::_OpenFile( CString strHFilepath, CString strCppFilePath)
{
	m_fpCpp = fopen(strCppFilePath, "w");
	if(NULL == m_fpCpp)
	{
		m_strErrMsg.Format("open %s for write failed", strCppFilePath);
		return false;
	}

	m_fpH = fopen(strHFilepath, "w");
	if(NULL == m_fpH)
	{
		m_strErrMsg.Format("open %s for write failed", strHFilepath);
		return false;
	}
	return true;
}


bool CClassMaker::_OpenFileEx()
{
	CString strCppFilePath = "";
	strCppFilePath.Format("%s_%d.cpp", m_poMainParser->GetName(), m_poMainParser->GetVersion());
	strCppFilePath.MakeLower();
	m_fpCppEx = fopen(m_strPath + strCppFilePath, "w");
	if(NULL == m_fpCppEx)
	{
		m_strErrMsg.Format("open %s for write failed", m_strPath + strCppFilePath);
		return false;
	}

	CString strHFilepath = "";
	strHFilepath.Format("%s_%d.h", m_poMainParser->GetName(), m_poMainParser->GetVersion());
	strHFilepath.MakeLower();
	m_fpHEx = fopen(m_strPath + strHFilepath, "w");
	if(NULL == m_fpHEx)
	{
		m_strErrMsg.Format("open %s for write failed", m_strPath + strHFilepath);
		return false;
	}

	if(m_poMainParser->GetOldVersion() == m_poMainParser->GetVersion())
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	strCppFilePath.Format("%s_%d_%d.cpp", m_poMainParser->GetName(), m_poMainParser->GetOldVersion(), m_poMainParser->GetVersion());
	strCppFilePath.MakeLower();
	m_fpCppHandle = fopen(m_strPath + strCppFilePath, "w");
	if(NULL == m_fpCppHandle)
	{
		m_strErrMsg.Format("open %s for write failed", m_strPath + strCppFilePath);
		return false;
	}

	strHFilepath.Format("%s_%d_%d.h", m_poMainParser->GetName(), m_poMainParser->GetOldVersion(), m_poMainParser->GetVersion());
	strHFilepath.MakeLower();
	m_fpHHandle = fopen(m_strPath + strHFilepath, "w");
	if(NULL == m_fpHHandle)
	{
		m_strErrMsg.Format("open %s for write failed", m_strPath + strHFilepath);
		return false;
	}

	return true;
}

bool CClassMaker::_WriteHeaderToH(LPCTSTR pszName, int nVersion)
{
	fprintf(m_fpH,	
		"/*       Version Number: %d          */\n"
		"\n", nVersion);

	fprintf(m_fpH,	
		"#ifndef %s_H_INCLUDE_VERSION_%d\n"
		"#define %s_H_INCLUDE_VERSION_%d\n"
		"\n",
		m_strUpperName, nVersion,
		m_strUpperName, nVersion);
	fprintf(m_fpH, "#include \"netdata.h\"\n");

	if(m_bCliEnDe)
	{
		fprintf( m_fpH, "#include \"protindexparse.h\"\n" );		
	}

	if( m_bJson )
	{
		//fprintf( m_fpCpp, "#include <json/json.h>\n" );
		fprintf( m_fpCpp, "#include <cjson.h>\n" );
	}

	return true;
}


bool CClassMaker::_WriteHeaderToHEx(LPCTSTR pszName, int nVersion)
{
	fprintf(m_fpHEx,
		"/*       Version Number: %d          */\n"
		"\n", nVersion);

	fprintf(m_fpHEx,
		"#ifndef %s_H_INCLUDE_VERSION_%d\n"
		"#define %s_H_INCLUDE_VERSION_%d\n"
		"\n",
		m_strUpperName, nVersion,
		m_strUpperName, nVersion);
	fprintf(m_fpHEx, "#include \"netdata.h\"\n");
	fprintf(m_fpHEx, "#include \"sdtype.h\"\n");
	fprintf(m_fpHEx, "#include \"sdstring.h\"\n");
	fprintf(m_fpHEx, "#include \"protocommondatamgr.h\"\n");
	fprintf(m_fpHEx, "namespace %s\n"
		"{\n",m_strNewNameSpace);
	return true;
}




bool CClassMaker::_WriteIncludeToH()
{
	for(size_t i = 0; i < m_poVecChildParser->size(); i++)
	{
		CParser* poParser = (*m_poVecChildParser)[i];
		int nMinID = poParser->GetMinMsgID();
		if ( m_poMainParser->GetMinMsgID() > nMinID )
		{
			m_poMainParser->SetMinMsgID(nMinID);
		}
		int nMaxID = poParser->GetMaxMsgID();
		if ( m_poMainParser->GetMaxMsgID() < nMaxID )
		{
			m_poMainParser->SetMaxMsgID(nMaxID);
		}
		char szName[MAX_NAME_LEN];
		lstrcpyn(szName, poParser->GetName(), sizeof(szName));
		strlwr(szName);

		fprintf(m_fpH, "#include \"%s.h\"\n", szName);
	}
	fprintf(m_fpH, "#include \"sdtype.h\"\n");
	fprintf(m_fpH, "#include \"sdstring.h\"\n");

	if( m_bJson )
	{
		//fprintf(m_fpH, "#include <json/json.h>\n");
		fprintf(m_fpH, "#include <cjson.h>\n");
		fprintf( m_fpH, "#include <dll/sdframework/sdutil.h>\n" );
	}
	fprintf(m_fpH, "#pragma pack(push, 1)\n");
	fprintf(m_fpH, "\n");


	return true;
}



bool CClassMaker::_WriteIncludeToHEx()
{
	for(size_t i = 0; i < m_poVecChildParser->size(); i++)
	{
		CParser* poParser = (*m_poVecChildParser)[i];
		int nMinID = poParser->GetMinMsgID();
		if ( m_poMainParser->GetMinMsgID() > nMinID )
		{
			m_poMainParser->SetMinMsgID(nMinID);
		}
		int nMaxID = poParser->GetMaxMsgID();
		if ( m_poMainParser->GetMaxMsgID() < nMaxID )
		{
			m_poMainParser->SetMaxMsgID(nMaxID);
		}
		char szName[MAX_NAME_LEN];
		lstrcpyn(szName, poParser->GetName(), sizeof(szName));
		strlwr(szName);

		fprintf(m_fpHEx, "#include \"%s.h\"\n", szName);
	}
	fprintf(m_fpHEx, "\n");
	return true;
}

bool CClassMaker::_WriteDefinesToH()
{
	fprintf(m_fpH, "/*Defines Begin*/\n");

	for(int i = 0; i < m_poMainParser->GetDefineInfoCount(); i++)
	{
		SDefineInfo* pstDefineInfo = m_poMainParser->GetDefineInfoByPos(i);
		fprintf(m_fpH, "#define %-24s %s\n", pstDefineInfo->szName, pstDefineInfo->szValue);
	}

	//// 2009-03-09 cwy modify
	fprintf(m_fpH, "#define %s_MSGID_RANGE %d\n", m_strUpperName, m_poMainParser->GetMsgIDRange());
	fprintf(m_fpH, "#define %s_MSGID_OFFSET %d\n", m_strUpperName, m_poMainParser->GetMinMsgID());
	//// 2009-03-09 cwy modify


	CString strTemp = "ProtoCommonData";
	if (strTemp == m_poMainParser->GetName())
	{
		fprintf(m_fpH, "#define PROTOCOL_VERSION %d\n", m_poMainParser->GetVersion());
	}

	fprintf(m_fpH, "/*Defines End*/\n\n\n");

	//
	if(m_bSaveEnDe)
	{
		fprintf(m_fpH, "\n#define INDEX_NUM_HEAD_SIZE	1	//索引个数头大小\n");
		fprintf(m_fpH, "#define ARRAY_NUM_HEAD_SIZE	2	//数组个数头大小\n");		
		fprintf(m_fpH, "\n"\
			"struct SIndex\n"\
			"{\n"\
			"\tUINT8	byIdx;\n"\
			"\tUINT32	dwContextLen;\n"\
			"};\n\n"
			);	
	}

	return true;
}


bool CClassMaker::_WriteDefinesToHEx()
{
	fprintf(m_fpHEx, "/*Defines Begin*/\n");

	for(int i = 0; i < m_poMainParser->GetDefineInfoCount(); i++)
	{
		SDefineInfo* pstDefineInfo = m_poMainParser->GetDefineInfoByPos(i);
		//fprintf(m_fpH, "#define %-24s %s\n", pstDefineInfo->szName, pstDefineInfo->szValue);

		fprintf(m_fpHEx, "const int  %-24s = %s;\n", pstDefineInfo->szName, pstDefineInfo->szValue);
	}

	fprintf(m_fpHEx, "/*Defines End*/\n\n\n");

	return true;
}


bool CClassMaker::_WriteTypesToH()
{
	fprintf(m_fpH, "/*Define Structs and Unions        Begin*/\n");

	for(int i = 0; i < m_poMainParser->GetTypeInfoCount(); i++)
	{
		STypeInfo* pstTypeInfo = m_poMainParser->GetTypeInfoByPos(i);

		if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
		{
			fprintf(m_fpH, "typedef union tag%s{\n", pstTypeInfo->szName);
		}
		else if(CLASS_TYPE_STRUCT == pstTypeInfo->nClassType)
		{
			fprintf(m_fpH, "typedef struct tag%s{\n", pstTypeInfo->szName);
			//¡£¡£¡£
			if(0 == strncmp(pstTypeInfo->szName, "PKT_", 4))
			{
				fprintf(m_fpH, "\ttag%s() { memset(this, 0, sizeof(tag%s)); }\n", pstTypeInfo->szName, pstTypeInfo->szName);
			}
			//¡£¡£¡£
		}
		else
		{
			ASSERT(false);
			return false;
		}

		CString strDef;
		for(size_t nItem = 0; nItem < pstTypeInfo->oVecItemInfo.size(); nItem++)
		{
			SItemInfo *pstItemInfo = pstTypeInfo->oVecItemInfo[nItem];
			if(false == _GetDef(pstItemInfo, strDef))
				return false;

			if(DATA_STRUCT == pstItemInfo->nDataType || DATA_UNION == pstItemInfo->nDataType)
			{
				if(pstItemInfo->szCount[0] != '\0')
				{
					fprintf(m_fpH, "\t%-16s %s[%s]; //%s\n", strDef, pstItemInfo->szMemberName, pstItemInfo->szCount, pstItemInfo->szComment);
				}
				else
				{
					fprintf(m_fpH, "\t%-16s %s; //%s\n", strDef, pstItemInfo->szMemberName, pstItemInfo->szComment);
				}
			}
			else
			{
				if(pstItemInfo->szCount[0] != '\0')
				{
					if(DATA_STRING == pstItemInfo->nDataType || DATA_WSTRING == pstItemInfo->nDataType || DATA_TSTRING == pstItemInfo->nDataType)
					{
						fprintf(m_fpH, "\t%-16s %s[%s]; //%s\n", strDef, pstItemInfo->szMemberName, pstItemInfo->szCount, pstItemInfo->szComment);
					}
					else
					{
						fprintf(m_fpH, "\t%-16s %s[%s]; //%s\n", strDef, pstItemInfo->szMemberName, pstItemInfo->szCount, pstItemInfo->szComment);
					}
				}
				else
				{
					fprintf(m_fpH, "\t%-16s %s; //%s\n", strDef, pstItemInfo->szMemberName, pstItemInfo->szComment);
				}
			}
		}
		if(CLASS_TYPE_STRUCT == pstTypeInfo->nClassType)
		{
			if ( pstTypeInfo->nIgnored > 0 )
			{
				fprintf(m_fpH, "public:\n");
				for(size_t nItem = 0; nItem < pstTypeInfo->oVecItemInfo.size(); nItem++)
				{
					SItemInfo *pstItemInfo = pstTypeInfo->oVecItemInfo[nItem];
					if(false == _GetDef(pstItemInfo, strDef))
						return false;
					if( pstItemInfo->bIgnored)
					{
						if(pstItemInfo->szCount[0] != '\0')
						{
							fprintf(m_fpH, "    void  Set%s(const %s* p%s,UINT32 nLen)\n", pstItemInfo->szName,strDef,pstItemInfo->szName);
						}
						else
							fprintf(m_fpH, "    void  Set%s(const %s pa%s)\n", pstItemInfo->szName,strDef,pstItemInfo->szName);

						fprintf(m_fpH, "    {\n" );
						if(pstItemInfo->szCount[0] != '\0')
						{
							fprintf(m_fpH, "		memset(%s,0,sizeof(%s)) ; \n", pstItemInfo->szMemberName,pstItemInfo->szMemberName);
							fprintf(m_fpH, "		if( sizeof( %s ) < nLen ) \n", pstItemInfo->szMemberName);
							fprintf(m_fpH, "			nLen = sizeof( %s ) ; \n", pstItemInfo->szMemberName);
							fprintf(m_fpH, "		memcpy(%s,p%s,nLen) ; \n", pstItemInfo->szMemberName,pstItemInfo->szName);
						}
						else
						{
							fprintf(m_fpH, "		%s = pa%s ; \n", pstItemInfo->szMemberName,pstItemInfo->szName);
						}
						fprintf(m_fpH, "		bsIgnored.set(%d);\n", nItem);
						fprintf(m_fpH, "    }\n" );
						fprintf(m_fpH, "    BOOL  Get%s()\n", pstItemInfo->szName);
						fprintf(m_fpH, "    {\n" );
						fprintf(m_fpH, "		return bsIgnored.test(%d);\n", nItem);
						fprintf(m_fpH, "    }\n" );
					}
				}
			}
		}

		//
		if(m_bSaveEnDe)
		{
			if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
			{
				fprintf(m_fpH, "\n\tINT32 EncodeSaveData(const UINT8 bySelectType, CHAR *pBuff, const UINT32 dwBuffLen);\n");
				fprintf(m_fpH, "\tINT32 DecodeSaveData(const UINT8 bySelectType, const CHAR *pBuff, const UINT32 dwBuffLen);\n");	
			}
			else
			{
				fprintf(m_fpH, "\n\tINT32 EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen);\n");
				fprintf(m_fpH, "\tINT32 DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen);\n");	
			}		
		}

		fprintf(m_fpH, "}%s;\n\n", pstTypeInfo->szName);		
	}

	fprintf(m_fpH, "/*Define Structs and Unions        End*/\n\n\n");
	return true;
}

bool CClassMaker::_WriteMaxEnLenToH()
{
	if(!m_bSaveEnDe)
	{
		return true;
	}

	fprintf(m_fpH, "/*Define MaxEnLen MAC Begin*/\n");

	for(int i = 0; i < m_poMainParser->GetTypeInfoCount(); i++)
	{
		STypeInfo* pstTypeInfo = m_poMainParser->GetTypeInfoByPos(i);	
		if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
		{
			fprintf(m_fpH, "#define  MAXENLEN_%s ( 10*sizeof(%s) )\n", pstTypeInfo->szName, pstTypeInfo->szName);	
		}
		else
		{
			CString strMac("#define  MAXENLEN_");
			strMac.AppendFormat("%s	( 1+%d*sizeof(SIndex)", pstTypeInfo->szName, pstTypeInfo->oVecItemInfo.size());
			for(size_t nItem = 0; nItem < pstTypeInfo->oVecItemInfo.size(); nItem++)
			{
				SItemInfo *pstItem = pstTypeInfo->oVecItemInfo[nItem];
				CString strDef;
				_GetDef(pstItem, strDef);
				//简单类型
				if((DATA_STRUCT != pstItem->nDataType) && ('\0' == pstItem->szCount[0]))
				{
					strMac.AppendFormat("+sizeof(%s)", strDef.GetString());
				}
				//简单类型_数组
				else if((DATA_STRUCT != pstItem->nDataType) && ('\0' != pstItem->szCount[0]))
				{
					if(DATA_STRING == pstItem->nDataType)
					{
						strMac.AppendFormat("+2+%s*sizeof(UINT8)", pstItem->szCount);
					}
					else if((DATA_WSTRING == pstItem->nDataType) ||
						(DATA_TSTRING == pstItem->nDataType))
					{
						strMac.AppendFormat("+2+%s*sizeof(UINT16)", pstItem->szCount);
					}
					else
					{
						strMac.AppendFormat("+2+%s*sizeof(%s)", pstItem->szCount, strDef.GetString());
					}				
				}
				//复合类型
				else if((DATA_STRUCT == pstItem->nDataType) && ('\0' == pstItem->szCount[0]))
				{
					strMac.AppendFormat("+MAXENLEN_%s", strDef.GetString());	
				}
				//复合类型_数组
				else if((DATA_STRUCT == pstItem->nDataType) && ('\0' != pstItem->szCount[0]))
				{
					strMac.AppendFormat("+2+%s*MAXENLEN_%s", pstItem->szCount, strDef.GetString());
				}
				else
				{

				}
			}

			strMac.Append(" )");

			fprintf(m_fpH, "%s\n", strMac.GetString());	
		}		
	}

	fprintf(m_fpH, "/*DDefine MaxEnLen MAC End*/\n\n\n");
	return true;
}


bool CClassMaker::_WriteTypesToHEx()
{
	fprintf(m_fpHEx, "/*Define Structs and Unions        Begin*/\n");
	fprintf(m_fpHEx, "#pragma pack(push, 1)\n");
	for(int i = 0; i < m_poMainParser->GetTypeInfoCount(); i++)
	{
		STypeInfo* pstTypeInfo = m_poMainParser->GetTypeInfoByPos(i);

		//fprintf(m_fpH, "// %s\n", pstTypeInfo->szComment);
		if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
		{
			fprintf(m_fpHEx, "typedef union tag%s{\n", pstTypeInfo->szName);
		}
		else if(CLASS_TYPE_STRUCT == pstTypeInfo->nClassType)
		{
			if (0 == strcmp("DT_PLAYER_DATA", pstTypeInfo->szName))
			{
				fprintf(m_fpHEx, "typedef struct tag%s:public IDT_PLAYER_DATA{\n", pstTypeInfo->szName);
				fprintf(m_fpHEx, "#include \"protocommondata.hpp\"\n");
			}
			else
			{
				fprintf(m_fpHEx, "typedef struct tag%s:public IDT_OBJ_DATA{\n", pstTypeInfo->szName);
				//fprintf(m_fpH, "\ttag%s() { memset(this, 0, sizeof(tag%s)); }\n", pstTypeInfo->szName, pstTypeInfo->szName);
			}

			if(0 == strncmp(pstTypeInfo->szName, "PKT_", 4))
			{
				fprintf(m_fpHEx, "\ttag%s() { memset(this, 0, sizeof(tag%s)); }\n", pstTypeInfo->szName, pstTypeInfo->szName);
			}

		}
		else
		{
			ASSERT(false);
			return false;
		}

		CString strDef;
		for(size_t nItem = 0; nItem < pstTypeInfo->oVecItemInfo.size(); nItem++)
		{
			SItemInfo *pstItemInfo = pstTypeInfo->oVecItemInfo[nItem];
			if(false == _GetDef(pstItemInfo, strDef))
				return false;

			if(DATA_STRUCT == pstItemInfo->nDataType || DATA_UNION == pstItemInfo->nDataType)
			{
				if(pstItemInfo->szCount[0] != '\0')
				{
					fprintf(m_fpHEx, "\t%-16s %s[%s]; //%s\n", strDef, pstItemInfo->szMemberName, pstItemInfo->szCount, pstItemInfo->szComment);
				}
				else
				{
					fprintf(m_fpHEx, "\t%-16s %s; //%s\n", strDef, pstItemInfo->szMemberName, pstItemInfo->szComment);
				}
			}
			else
			{
				if(pstItemInfo->szCount[0] != '\0')
				{
					if(DATA_STRING == pstItemInfo->nDataType || DATA_WSTRING == pstItemInfo->nDataType || DATA_TSTRING == pstItemInfo->nDataType)
					{
						fprintf(m_fpHEx, "\t%-16s %s[%s]; //%s\n", strDef, pstItemInfo->szMemberName, pstItemInfo->szCount, pstItemInfo->szComment);
					}
					else
					{
						fprintf(m_fpHEx, "\t%-16s %s[%s]; //%s\n", strDef, pstItemInfo->szMemberName, pstItemInfo->szCount, pstItemInfo->szComment);
					}
				}
				else
				{
					fprintf(m_fpHEx, "\t%-16s %s; //%s\n", strDef, pstItemInfo->szMemberName, pstItemInfo->szComment);
				}
			}
		}
		if(CLASS_TYPE_STRUCT == pstTypeInfo->nClassType)
		{
			if ( pstTypeInfo->nIgnored > 0 )
			{
				fprintf(m_fpHEx, "public:\n");
				for(size_t nItem = 0; nItem < pstTypeInfo->oVecItemInfo.size(); nItem++)
				{
					SItemInfo *pstItemInfo = pstTypeInfo->oVecItemInfo[nItem];
					if(false == _GetDef(pstItemInfo, strDef))
						return false;
					if( pstItemInfo->bIgnored)
					{
						if(pstItemInfo->szCount[0] != '\0')
						{
							fprintf(m_fpHEx, "    void  Set%s(const %s* p%s,UINT32 nLen)\n", pstItemInfo->szName, strDef, pstItemInfo->szName);
						}
						else
						{
							fprintf(m_fpHEx, "    void  Set%s(const %s pa%s)\n", pstItemInfo->szName, strDef, pstItemInfo->szName);
						}
						fprintf(m_fpHEx, "    {\n" );
						if(pstItemInfo->szCount[0] != '\0')
						{
							fprintf(m_fpHEx, "		memset(%s,0,sizeof(%s)) ; \n", pstItemInfo->szMemberName, pstItemInfo->szMemberName);
							fprintf(m_fpHEx, "		if( sizeof( %s ) < nLen ) \n", pstItemInfo->szMemberName);
							fprintf(m_fpHEx, "			nLen = sizeof( %s ) ; \n", pstItemInfo->szMemberName);
							fprintf(m_fpHEx, "		memcpy(%s,p%s,nLen) ; \n", pstItemInfo->szMemberName, pstItemInfo->szName);
						}
						else
						{
							fprintf(m_fpHEx, "		%s = pa%s ; \n", pstItemInfo->szMemberName, pstItemInfo->szName);
						}
						fprintf(m_fpHEx, "		bsIgnored.set(%d);\n", nItem);
						fprintf(m_fpHEx, "    }\n" );
						fprintf(m_fpHEx, "    BOOL  Get%s()\n", pstItemInfo->szName);
						fprintf(m_fpHEx, "    {\n" );
						fprintf(m_fpHEx, "		return bsIgnored.test(%d);\n", nItem);
						fprintf(m_fpHEx, "    }\n" );
					}
				}
			}
		}
		fprintf(m_fpHEx, "}%s;\n\n", pstTypeInfo->szName);
	}
	fprintf(m_fpHEx, "#pragma pack(pop)\n");
	fprintf(m_fpHEx, "/*Define Structs and Unions        End*/\n\n\n");
	return true;
}

bool CClassMaker::_WriteMessageIDToH()
{
	fprintf(m_fpH,	
		"enum EN_%s_MessageID\n"
		"{\n",
		m_poMainParser->GetName());

	for(int i = 0; i < m_poMainParser->GetMessageInfoCount(); i++)
	{
		SMessageInfo* pstMessageInfo = m_poMainParser->GetMessageInfoByPos(i);

		//// 2009-03-27 cwy modify for @001
		if (pstMessageInfo->bHasID)
		{
			fprintf(m_fpH, "\t%-30s  = %-10d  ,\n", pstMessageInfo->szName, pstMessageInfo->nID);
		}
		else
		{
			fprintf(m_fpH, "\t%-30s/*= %-10d*/, \n", pstMessageInfo->szName,pstMessageInfo->nID);
		}
		//// ~
	}

	fprintf(m_fpH, "};\n\n");

	return true;
}

bool CClassMaker::_WriteCodeFuncToH()
{
	fprintf(m_fpH, "/*Encode and Decode Function Declare Begin*/\n");
	for(int i = 0; i < m_poMainParser->GetTypeInfoCount(); i++)
	{
		STypeInfo* pstTypeInfo = m_poMainParser->GetTypeInfoByPos(i);
		if(CLASS_TYPE_STRUCT == pstTypeInfo->nClassType)
		{
			fprintf(m_fpH, "INT32 Encode%s(void *pHost, CNetData* poNetData);\n", pstTypeInfo->szName);
			fprintf(m_fpH, "INT32 Decode%s(void *pHost, CNetData* poNetData);\n", pstTypeInfo->szName);

			if( m_bJson )
			{
				fprintf( m_fpH, "cJSON* GetJson%s( %s &data );\n", pstTypeInfo->szName, pstTypeInfo->szName );
			}
		}
		else if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
		{
			fprintf(m_fpH, "INT32 EncodeUn%s(void *pHost, INT32 iSelect, CNetData* poNetData);\n", pstTypeInfo->szName);
			fprintf(m_fpH, "INT32 DecodeUn%s(void *pHost, INT32 iSelect, CNetData* poNetData);\n", pstTypeInfo->szName);
		}
		else
		{
			ASSERT(false);
			return false;
		}
	}

	fprintf(m_fpH, "/*Encode and Decode Function Declare End*/\n\n");

	return true;
}

bool CClassMaker::_WriteCodeFuncToHEx()
{
	fprintf(m_fpHHandle,
		"#ifndef %s_H_INCLUDE_VERSION_%d_%d\n"
		"#define %s_H_INCLUDE_VERSION_%d_%d\n"
		"\n",
		m_strUpperName, m_poMainParser->GetOldVersion(),m_poMainParser->GetVersion(),
		m_strUpperName, m_poMainParser->GetOldVersion(),m_poMainParser->GetVersion());

	fprintf(m_fpHHandle, "#include \"protocommondatamgr.h\"\n");

	fprintf(m_fpHHandle, "namespace %s\n"
		"{\n",m_strHandleNameSpace);

	fprintf(m_fpHHandle, "/*Equal and Decode Function Declare Begin*/\n");
	for(int i = 0; i < m_poMainParser->GetTypeInfoCount(); i++)
	{
		STypeInfo* pstTypeInfo = m_poMainParser->GetTypeInfoByPos(i);
		if(CLASS_TYPE_STRUCT == pstTypeInfo->nClassType)
		{
			fprintf(m_fpHHandle, "void Equal%s(void *pOut, void *pIn);\n", pstTypeInfo->szName);
		}
		else if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
		{
			fprintf(m_fpHHandle, "void EqualUn%s(void *pOut, INT32 iSelect, void *pIn);\n", pstTypeInfo->szName);
		}
		else
		{
			ASSERT(false);
			return false;
		}
	}
	fprintf(m_fpHHandle, "/*Equal and Decode Function Declare End*/\n\n");


	CString v_strClassName = "";
	v_strClassName.Format("CProtoExchange_%d_%d", m_poMainParser->GetOldVersion(), m_poMainParser->GetVersion());

	fprintf(m_fpHHandle,
		"class %s : public IProtoExchange\n"
		"{\n"
		"public:\n",
		v_strClassName);

	fprintf(m_fpHHandle, "	%s(){}\n", v_strClassName);
	fprintf(m_fpHHandle, "	%s(CProtoCommondataMgr * IN_pManager)\n", v_strClassName);
	fprintf(m_fpHHandle, "	{\n");
	fprintf(m_fpHHandle, "		m_pManager = IN_pManager;\n");
	fprintf(m_fpHHandle, "		RegisterProtoExchange(this);\n");
	fprintf(m_fpHHandle, "	}\n");

	fprintf(m_fpHHandle, "	virtual DWORD GetKey()\n");
	fprintf(m_fpHHandle, "	{\n");
	fprintf(m_fpHHandle, "		return %d;\n", m_poMainParser->GetOldVersion());
	fprintf(m_fpHHandle, "	}\n");

	fprintf(m_fpHHandle, "	virtual DWORD GetValue()\n");
	fprintf(m_fpHHandle, "	{\n");
	fprintf(m_fpHHandle, "		return %d;\n", m_poMainParser->GetVersion());
	fprintf(m_fpHHandle, "	}\n");

	fprintf(m_fpHHandle, "	virtual FrmCallFunc * GetCallFunc()\n");
	fprintf(m_fpHHandle, "	{\n");
	fprintf(m_fpHHandle, "		return &EqualDT_PLAYER_DATA;\n");
	fprintf(m_fpHHandle, "	}\n");

	fprintf(m_fpHHandle, "};\n");
	fprintf(m_fpHHandle, "}\n");
	fprintf(m_fpHHandle, "#endif\n\n");
	return true;
}

bool CClassMaker::_WriteClassDefToH()
{
	m_strEnFuncDef.Format("EnFunc%s", m_strClassName);
	m_strDeFuncDef.Format("DeFunc%s", m_strClassName);

	fprintf(m_fpH, "typedef INT32 (*%s)(void *pHost, CNetData* poNetData);\n", 
		m_strEnFuncDef);
	fprintf(m_fpH, "typedef INT32 (*%s)(void *pHost, CNetData* poNetData);\n\n",
		m_strDeFuncDef);

	fprintf(m_fpH, 
		"class %s : public CSDProtocol\n"
		"{\n"
		"public:\n",
		m_strClassName);

	//Ìí¼Ó¶ÔÏóÊµÀý»¯º¯Êý
	fprintf(m_fpH, "    static %s * Instance();\n",m_strClassName);

	fprintf(m_fpH, "    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);\n");
	fprintf(m_fpH, "    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);\n");
	fprintf(m_fpH, "	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }\n");
	fprintf(m_fpH, "	%s FindEncodeFunc(INT32 iMsgID);\n", m_strEnFuncDef);
	fprintf(m_fpH, "	%s FindDecodeFunc(INT32 iMsgID);\n", m_strDeFuncDef);
	fprintf(m_fpH, "	BOOL Init();\n");
	if(m_bCliEnDe)
	{
		fprintf(m_fpH, "	inline CStructIndexMap& GetStructIndexMap() { return m_mapStructIndex; }\n");
		fprintf(m_fpH, "	inline CUnionIDIndexMap& GetUnionIDIndexMap() { return m_mapUnionIDIndex;};\n\n");	
	}

	fprintf(m_fpH, "\n"
		"private:\n"
		"\t%s();\n"
		"\tvirtual ~%s();\n"
		"	%s m_EncodeFuncArray[%s_MSGID_RANGE];\n"
		"	%s m_DecodeFuncArray[%s_MSGID_RANGE];\n"
		"	CNetData m_oData;\n"
		"	INT32 m_iMaxPkgSize;\n",		
		//m_strEnFuncDef, MAX_MESSAGE_ID,
		//m_strDeFuncDef, MAX_MESSAGE_ID);
		m_strClassName,m_strClassName,m_strEnFuncDef, m_strUpperName, m_strDeFuncDef, m_strUpperName);	//// 2009-03-09 cwy modify

	if(m_bCliEnDe)
	{
		fprintf(m_fpH, "	CStructIndexMap		m_mapStructIndex;\n");
		fprintf(m_fpH, "	CUnionIDIndexMap	m_mapUnionIDIndex;\n");
	}

	fprintf(m_fpH, "\n"
		"};\n"
		"\n"
		"#pragma pack(pop)\n"
		"#endif\n");

	return true;
}

bool CClassMaker::_WriteClassDefToHEx()
{
	fprintf(m_fpHEx,
		"class %s : public IProtoCommondata\n"
		"{\n"
		"public:\n",
		m_strClassNameEx);

	fprintf(m_fpHEx, "	%s(){}\n", m_strClassNameEx);
	fprintf(m_fpHEx, "	%s(CProtoCommondataMgr * IN_pManager)\n", m_strClassNameEx);
	fprintf(m_fpHEx, "	{\n");
	fprintf(m_fpHEx, "		m_pManager = IN_pManager;\n");
	fprintf(m_fpHEx, "		RegisterDealFrame(this);\n");
	fprintf(m_fpHEx, "	}\n");
	fprintf(m_fpHEx, "	virtual DWORD GetKey()\n");
	fprintf(m_fpHEx, "	{\n");
	fprintf(m_fpHEx, "		return %d;\n", m_poMainParser->GetVersion());
	fprintf(m_fpHEx, "	}\n");

	fprintf(m_fpHEx, "	virtual IDT_PLAYER_DATA *GetIDT_PLAYER_DATA()\n");
	fprintf(m_fpHEx, "	{\n");
	fprintf(m_fpHEx, "		return new DT_PLAYER_DATA();\n");
	fprintf(m_fpHEx, "	}\n");

	fprintf(m_fpHEx, "};\n");
	return true;
}

bool CClassMaker::_WriteTailToHEx()
{
	fprintf(m_fpHEx, "}\n");
	fprintf(m_fpHEx, "#endif\n");

	return true;
}

bool CClassMaker::_WriteIncludeToCpp()
{
	char szLowerName[256];
	strcpy(szLowerName, m_poMainParser->GetName());
	strlwr(szLowerName);
	fprintf(m_fpCpp, "#include \"%s.h\"\n", szLowerName);

	if( m_bJson )
	{
		fprintf(m_fpCpp, "#include <dll/sdframework/sdframework.h>\n");
		fprintf( m_fpCpp, "#include <sstream>\n" );
	}

	return true;
}

bool CClassMaker::_WriteIncludeToCppEx()
{
	char szLowerName[256] = {0};
	strcpy(szLowerName, m_poMainParser->GetName());
	strlwr(szLowerName);
	fprintf(m_fpCppEx, "#include \"stdafx.h\"\n\n");
	fprintf(m_fpCppEx, "#include \"%s_%d.h\"\n\n", szLowerName, m_poMainParser->GetVersion());
	return true;
}

bool CClassMaker::_WriteCommonPartToCpp()
{
	fprintf(m_fpCpp, 
		"%s::%s()\n"
		"{\n"
		"    m_vecMsgList.clear() ;\n"
		"	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));\n"
		"	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));\n"
		"	m_iMaxPkgSize = 0;\n"
		"	Init();\n",
		m_strClassName, m_strClassName);//¹¹Ôìº¯ÊýÊµÏÖ

	if(m_bCliEnDe)
	{
		fprintf(m_fpCpp, "\n	InitStructIndex(m_mapStructIndex, \"%s\");\n", m_poMainParser->GetName());
		fprintf(m_fpCpp, "	InitUionIndex(m_mapUnionIDIndex, \"%s\");\n", m_poMainParser->GetName());
	}

	fprintf(m_fpCpp, "}\n\n");	

	//ÓÃÓÚµ¥Ïß³ÌÊµÀý
	fprintf(m_fpCpp,
		"%s * %s::Instance()\n"
		"{\n"
		"   static %s oInst;\n"
		"   return &oInst;\n"
		"}\n\n",
		m_strClassName,m_strClassName,m_strClassName);

	fprintf(m_fpCpp, 
		"%s::~%s()\n"
		"{\n"
		"}\n\n",
		m_strClassName, m_strClassName);

	// set charset information

	fprintf(m_fpCpp,
		"INT32 %s::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)\n"
		"{\n"
		"	m_oData.Prepare(pNet, iNetSize, %d, %d, %d);\n"
		"\n"
		"	%s pfnEncode = FindEncodeFunc(iMsgID);\n"
		"	if(NULL == pfnEncode)\n"
		"		return -1;\n"
		"\n"
		"	return (*pfnEncode)(pHost, &m_oData);\n"
		"}\n\n",
		m_strClassName,
		m_nType,
		m_nCharsetNet,
		m_nCharsetHost,
		m_strEnFuncDef
		);

	fprintf(m_fpCpp,
		"INT32 %s::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)\n"
		"{\n"
		"	m_oData.Prepare(pNet, iNetSize, %d, %d, %d);\n"
		"	if(iHostSize < m_iMaxPkgSize)\n"
		"		return -1;\n"
		"\n"
		"	%s pfnDecode = FindDecodeFunc(iMsgID);\n"
		"	if(NULL == pfnDecode)\n"
		"		return -1;\n"
		"\n"
		"	return (*pfnDecode)(pHost, &m_oData);\n"
		"}\n\n",
		m_strClassName,
		m_nType,
		m_nCharsetNet,
		m_nCharsetHost,
		m_strDeFuncDef
		);

	//// 2009-03-09 cwy modify begin
	fprintf(m_fpCpp,
		"%s %s::FindEncodeFunc(INT32 iMsgID)\n"
		"{\n"
		"	if(iMsgID - %s_MSGID_OFFSET >= %s_MSGID_RANGE)\n"
		"		return NULL;\n"
		"	return m_EncodeFuncArray[iMsgID - %s_MSGID_OFFSET];\n"
		"}\n\n",
		m_strEnFuncDef, m_strClassName, m_strUpperName, m_strUpperName, m_strUpperName);

	fprintf(m_fpCpp,
		"%s %s::FindDecodeFunc(INT32 iMsgID)\n"
		"{\n"
		"	if(iMsgID - %s_MSGID_OFFSET >= %s_MSGID_RANGE)\n"
		"		return NULL;\n"
		"	return m_DecodeFuncArray[iMsgID - %s_MSGID_OFFSET];\n"
		"}\n\n",
		m_strDeFuncDef, m_strClassName, m_strUpperName, m_strUpperName, m_strUpperName);
	//// 2009-03-09 cwy modify end

	fprintf(m_fpCpp,
		"BOOL %s::Init()\n"
		"{\n"
		"	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));\n"
		"	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));\n"
		"\n",
		m_strClassName);

	for(int i = 0; i < m_poMainParser->GetMessageInfoCount(); i++)
	{
		SMessageInfo* pstMessageInfo = m_poMainParser->GetMessageInfoByPos(i);

		//// 2009-03-09 cwy modify begin
		fprintf(m_fpCpp, "	m_EncodeFuncArray[%s - %s_MSGID_OFFSET] = &Encode%s;\n", pstMessageInfo->szName, m_strUpperName, pstMessageInfo->szType);
		fprintf(m_fpCpp, "	m_DecodeFuncArray[%s - %s_MSGID_OFFSET] = &Decode%s;\n", pstMessageInfo->szName, m_strUpperName, pstMessageInfo->szType);
		//// 2009-03-09 cwy modify end
		fprintf(m_fpCpp, "	m_vecMsgList.push_back(%s);\n", pstMessageInfo->szName);

		fprintf(m_fpCpp, "	if( (INT32)sizeof(%s) > m_iMaxPkgSize)\n"
			"		m_iMaxPkgSize = (INT32)sizeof(%s);\n\n", pstMessageInfo->szType, pstMessageInfo->szType);
	}

	// ×¢²á×ÓÎÄ¼þµÄÏûÏ¢µ½Ö÷ÎÄ¼þµÄÀà
	CParserVec::iterator itrParser = m_poVecChildParser->begin();
	for (; itrParser != m_poVecChildParser->end(); itrParser++)
	{
		CParser* poParser = *itrParser;
		for(int i = 0; i < poParser->GetMessageInfoCount(); i++)
		{
			SMessageInfo* pstMessageInfo = poParser->GetMessageInfoByPos(i);

			fprintf(m_fpCpp, "	m_EncodeFuncArray[%s - %s_MSGID_OFFSET] = &Encode%s;\n", pstMessageInfo->szName, m_strUpperName, pstMessageInfo->szType);
			fprintf(m_fpCpp, "	m_DecodeFuncArray[%s - %s_MSGID_OFFSET] = &Decode%s;\n", pstMessageInfo->szName, m_strUpperName, pstMessageInfo->szType);		    
			fprintf(m_fpCpp, "	m_vecMsgList.push_back(%s);\n", pstMessageInfo->szName);
			fprintf(m_fpCpp, "	if( (INT32)sizeof(%s) > m_iMaxPkgSize)\n"
				"		m_iMaxPkgSize = (INT32)sizeof(%s);\n\n", pstMessageInfo->szType, pstMessageInfo->szType);
		}
	}

	fprintf(m_fpCpp,
		"	return true;\n"
		"}\n\n");

	return true;
}



bool CClassMaker::_WriteAllCodeFuncToCpp()
{
	for(int i = 0; i < m_poMainParser->GetTypeInfoCount(); i++)
	{
		STypeInfo* pstTypeInfo = m_poMainParser->GetTypeInfoByPos(i);
		if(CLASS_TYPE_STRUCT == pstTypeInfo->nClassType)
		{
			if(m_bCliEnDe)
			{
				if(false == _WriteCliEncodeFuncToCpp(pstTypeInfo))
					return false;
			}
			else
			{
				if(false == _WriteEncodeFuncToCpp(pstTypeInfo))
					return false;
			}

			if(m_bCliEnDe)
			{
				if(false == _WriteCliDecodeFuncToCpp(pstTypeInfo))
					return false;
			}
			else
			{
				if(false == _WriteDecodeFuncToCpp(pstTypeInfo))
					return false;
			}

			if( m_bJson )
			{
				_WriteGetJsonFuncToCpp( pstTypeInfo );
				//_WriteFromJsonFuncToCpp( pstTypeInfo );
			}

			if(m_bSaveEnDe)
			{
				if(!_WriteSaveEncodeFuncToCpp(pstTypeInfo))
				{
					m_strErrMsg.Format("<type name=%s>\r\n _WriteSaveEncodeFuncToCpp failed!", 
						pstTypeInfo->szName);
					return false;
				}
				if(!_WriteSaveDecodeFuncToCpp(pstTypeInfo))
				{
					m_strErrMsg.Format("<type name=%s>\r\n _WriteSaveDecodeFuncToCpp failed!", 
						pstTypeInfo->szName);
					return false;
				}
			}
		}
		else if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
		{
			if(false == _WriteEncodeUnionToCpp(pstTypeInfo))
				return false;
			if(false == _WriteDecodeUnionToCpp(pstTypeInfo))
				return false;

			if(m_bSaveEnDe)
			{
				if(!_WriteSaveEncodeFuncToCpp(pstTypeInfo))
				{
					m_strErrMsg.Format("<type name=%s>\r\n _WriteSaveEncodeFuncToCpp failed!", 
						pstTypeInfo->szName);
					return false;
				}
				if(!_WriteSaveDecodeFuncToCpp(pstTypeInfo))
				{
					m_strErrMsg.Format("<type name=%s>\r\n _WriteSaveDecodeFuncToCpp failed!", 
						pstTypeInfo->szName);
					return false;
				}
			}
		}
		else
		{
			ASSERT(false);
			return false;
		}
	}
	return true;
}

bool CClassMaker::_WriteAllCodeFuncToCppEx()
{
	char szLowerName[256] = {0};
	strcpy(szLowerName, m_poMainParser->GetName());
	strlwr(szLowerName);
	fprintf(m_fpCppHandle, "#include \"stdafx.h\"\n\n");
	fprintf(m_fpCppHandle, "#include \"%s_%d.h\"\n\n", szLowerName, m_poMainParser->GetOldVersion());
	fprintf(m_fpCppHandle, "#include \"%s_%d.h\"\n\n", szLowerName, m_poMainParser->GetVersion());
	fprintf(m_fpCppHandle, "#include \"%s_%d_%d.h\"\n\n", szLowerName, m_poMainParser->GetOldVersion(), m_poMainParser->GetVersion());

	for(int i = 0; i < m_poMainParser->GetTypeInfoCount(); i++)
	{
		STypeInfo* pstTypeInfo = m_poMainParser->GetTypeInfoByPos(i);
		if(CLASS_TYPE_STRUCT == pstTypeInfo->nClassType)
		{
			if(false == _WriteEqualFuncToCpp(pstTypeInfo))
				return false;
		}
		else if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
		{
			if(false == _WriteEqualUnionToCpp(pstTypeInfo))
				return false;
		}
		else
		{
			ASSERT(false);
			return false;
		}
	}
	return true;
}

bool CClassMaker::_WriteEncodeFuncToCpp(STypeInfo* pstTypeInfo)
{
	fprintf(m_fpCpp,
		"INT32 Encode%s(void *pHost, CNetData* poNetData)\n"
		"{\n"
		"	%s *pstIn = (%s*)pHost;\n"
		"\n",
		pstTypeInfo->szName,
		pstTypeInfo->szName, pstTypeInfo->szName);

	bool bDefI =false;
	if ( pstTypeInfo->nIgnored > 0 )
	{
		int nNum =  pstTypeInfo->nIgnored / 8 ;
		if (pstTypeInfo->nIgnored % 8)
		{
			nNum += 1 ;
		}
		fprintf(m_fpCpp,"	CHAR szTemp[%d] = { 0 };\n" , nNum );
		for ( int n = 0 ; n < nNum ; n ++ )
		{
			fprintf(m_fpCpp,"	bitset<%d> bitT = pstIn->bsIgnored >> %d;\n" ,pstTypeInfo->nIgnored , n * 8  );
			if ( n < nNum - 1 )
			{
				fprintf(m_fpCpp,"	bitT = pstIn->bsIgnored << %d;\n" , (nNum - n - 1) * 8  );
			}
			fprintf(m_fpCpp,"	szTemp[%d] = bitT.to_ulong();\n" , n );
		}
		fprintf(m_fpCpp,"	if(-1 == poNetData->AddString(szTemp, sizeof(szTemp)))\n"  );
		fprintf(m_fpCpp,"		return -1;\n\n"  );			
	}

	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{
		if('\0' != pstTypeInfo->oVecItemInfo[i]->szCount[0] 
		&& DATA_STRING != pstTypeInfo->oVecItemInfo[i]->nDataType
			&& DATA_WSTRING != pstTypeInfo->oVecItemInfo[i]->nDataType
			&& DATA_TSTRING != pstTypeInfo->oVecItemInfo[i]->nDataType)
		{
			if(false == bDefI)
			{
				fprintf(m_fpCpp, "	INT32 i;\n");
				bDefI = true;
			}
		}
		if (pstTypeInfo->oVecItemInfo[i]->bIgnored)
		{
			fprintf(m_fpCpp,"	if(pstIn->Get%s())\n",pstTypeInfo->oVecItemInfo[i]->szName );
			fprintf(m_fpCpp, "	{\n");
			if(false == _WriteEncodeItemToCpp(pstTypeInfo->oVecItemInfo[i],true))
				return false;
			fprintf(m_fpCpp, "	}\n\n");
		}
		else
		{		
			if(false == _WriteEncodeItemToCpp(pstTypeInfo->oVecItemInfo[i]))
				return false;
		}
	}

	fprintf(m_fpCpp,
		"	return poNetData->GetDataLen();\n"
		"}\n\n");

	return true;
}

bool CClassMaker::_WriteGetJsonFuncToCpp(STypeInfo* pstTypeInfo)
{
	/*
	string strData;
	for( UINT32 dwIndex=0; dwIndex<pstTypeInfo->oVecItemInfo.size(); ++dwIndex )
	{
		if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_UNION )
		{
			continue;
		}
		else if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_STRUCT ) 
		{
			if( !string(pstTypeInfo->oVecItemInfo[dwIndex]->szCount).empty() )  
			{//如果是数组
				string strCountName=GetCountStr( pstTypeInfo->oVecItemInfo[dwIndex]->szCount, pstTypeInfo->oVecItemInfo[dwIndex]->szReferMemberName );

				strData+="\tfor( UINT32 dwIndex=0; dwIndex<"+strCountName+ "; ++dwIndex)\n"
					+"\t{\n"+"\troot[\""+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"].append( "+ "GetJson"+pstTypeInfo->oVecItemInfo[dwIndex]->szType
					+"(data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+"[dwIndex]) );\n\t}\n";
			}
			else
			{
				strData+=string("\troot[\"")+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"]="
					+"GetJson"+pstTypeInfo->oVecItemInfo[dwIndex]->szType+"(data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+");\n";
				pstTypeInfo->oVecItemInfo[dwIndex]->szCount;
			}
			pstTypeInfo->oVecItemInfo[dwIndex]->szCount;
		}

		else if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_UINT64 )
		{
			string strSSName=string( "ss" )+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName;
			strData+="\tostringstream "+strSSName+";\n";
			strData+="\t"+strSSName+"<<data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+";\n";

			strData+=string("\troot[\"")+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"]="
				+strSSName+".str()"+";\n";
		}
		else
		{
			if( !string(pstTypeInfo->oVecItemInfo[dwIndex]->szCount).empty() )  
			{//如果是数组
				if( DATA_TSTRING==pstTypeInfo->oVecItemInfo[dwIndex]->nDataType 
					|| DATA_STRING==pstTypeInfo->oVecItemInfo[dwIndex]->nDataType 
					|| DATA_WSTRING==pstTypeInfo->oVecItemInfo[dwIndex]->nDataType )
				{//如果是字符串,则不按数组方式处理
					strData+=string("\troot[\"")+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"]="
						+"data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+";\n";
					continue;
				}
				string strCountName=GetCountStr( pstTypeInfo->oVecItemInfo[dwIndex]->szCount, pstTypeInfo->oVecItemInfo[dwIndex]->szReferMemberName );
				strData+="\tfor( UINT32 dwIndex=0; dwIndex<"+strCountName+ "; ++dwIndex)\n"
					+"\t{\n"+"\troot[\""+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"].append( "+ "data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName
					+"[dwIndex]);\n\t}\n";
			}
			else
			{//非数组
				strData+=string("\troot[\"")+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"]="
					+"data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+";\n";
			}
		}


	}

	fprintf( m_fpCpp, "Json::Value GetJson%s(%s &data)\n"
		"{\n"
		"\tJson::Value root;\n"
		"%s\n"
		"\treturn root;\n"
		"}\n", pstTypeInfo->szName, pstTypeInfo->szName, strData.c_str() );
	*/


	string strData;
	for( UINT32 dwIndex=0; dwIndex<pstTypeInfo->oVecItemInfo.size(); ++dwIndex )
	{
		if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_UNION )
		{
			continue;
		}
		else if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_STRUCT ) 
		{
			if( !string(pstTypeInfo->oVecItemInfo[dwIndex]->szCount).empty() )  
			{//如果是数组
				string strCountName=GetCountStr( pstTypeInfo->oVecItemInfo[dwIndex]->szCount, pstTypeInfo->oVecItemInfo[dwIndex]->szReferMemberName );

				/*
				strData+="\tfor( UINT32 dwIndex=0; dwIndex<"+strCountName+ "; ++dwIndex)\n"
					+"\t{\n"+"\troot[\""+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"].append( "+ "GetJson"+pstTypeInfo->oVecItemInfo[dwIndex]->szType
					+"(data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+"[dwIndex]) );\n\t}\n";
					*/
				strData+=string("\tcJSON *") + pstTypeInfo->oVecItemInfo[dwIndex]->szName + " = cJSON_CreateArray();\n"
					+ "\tcJSON_AddItemToObject(root, \"" + pstTypeInfo->oVecItemInfo[dwIndex]->szName +"\", " + pstTypeInfo->oVecItemInfo[dwIndex]->szName + ");\n"
					+ "\tfor( UINT32 dwIndex=0; dwIndex<"+strCountName+ "; ++dwIndex)\n"
					+ "\t{\n"
					+ "\t\tcJSON_AddItemToObject(" + pstTypeInfo->oVecItemInfo[dwIndex]->szName + ", \"" + " " + "\", GetJson"+pstTypeInfo->oVecItemInfo[dwIndex]->szType
					+ "(data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+"[dwIndex]));\n"
					+ "\t}\n";
			}
			else
			{
				/*
				strData+=string("\troot[\"")+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"]="
					+"GetJson"+pstTypeInfo->oVecItemInfo[dwIndex]->szType+"(data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+");\n";
				pstTypeInfo->oVecItemInfo[dwIndex]->szCount;
				*/
				strData+=string("\tcJSON_AddItemToObject(root, \"")+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\", GetJson"	
					+ pstTypeInfo->oVecItemInfo[dwIndex]->szType+"(data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+"));\n";
			}
			pstTypeInfo->oVecItemInfo[dwIndex]->szCount;
		}

// 		else if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_UINT64 )
// 		{
// 			/*
// 			string strSSName=string( "ss" )+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName;
// 			strData+="\tostringstream "+strSSName+";\n";
// 			strData+="\t"+strSSName+"<<data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+";\n";
// 
// 			strData+=string("\troot[\"")+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"]="
// 				+strSSName+".str()"+";\n";
// 			*/
// 			strData += string("\tcJSON_AddNumberToObject(root,\"") + pstTypeInfo->oVecItemInfo[dwIndex]->szName + "\", " + "(double)(data." + pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName + " ));\n";
// 		}
		else
		{
			if( !string(pstTypeInfo->oVecItemInfo[dwIndex]->szCount).empty() )  
			{//如果是数组
				if( DATA_TSTRING==pstTypeInfo->oVecItemInfo[dwIndex]->nDataType 
					|| DATA_STRING==pstTypeInfo->oVecItemInfo[dwIndex]->nDataType 
					|| DATA_WSTRING==pstTypeInfo->oVecItemInfo[dwIndex]->nDataType )
				{//如果是字符串,则不按数组方式处理
					/*
					strData+=string("\troot[\"")+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"]="
						+"data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+";\n";
						*/
					strData += string("\tcJSON_AddStringToObject(root,\"") + pstTypeInfo->oVecItemInfo[dwIndex]->szName + "\", " + "data." + pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName + " );\n";
					continue;
				}
				/*
				string strCountName=GetCountStr( pstTypeInfo->oVecItemInfo[dwIndex]->szCount, pstTypeInfo->oVecItemInfo[dwIndex]->szReferMemberName );
				strData+="\tfor( UINT32 dwIndex=0; dwIndex<"+strCountName+ "; ++dwIndex)\n"
					+"\t{\n"+"\troot[\""+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"].append( "+ "data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName
					+"[dwIndex]);\n\t}\n";
					*/
				string strCountName=GetCountStr( pstTypeInfo->oVecItemInfo[dwIndex]->szCount, pstTypeInfo->oVecItemInfo[dwIndex]->szReferMemberName );
				strData+=string("\tcJSON *") + pstTypeInfo->oVecItemInfo[dwIndex]->szName + " = cJSON_CreateArray();\n"
					+ "\tcJSON_AddItemToObject(root, \"" + pstTypeInfo->oVecItemInfo[dwIndex]->szName +"\", " + pstTypeInfo->oVecItemInfo[dwIndex]->szName + ");\n"
					+ "\tfor( UINT32 dwIndex=0; dwIndex<"+strCountName+ "; ++dwIndex)\n"
					+ "\t{\n"
					+ "\t\tcJSON_AddNumberToObject(root,\"" +pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"" + ", " + "(double)(data." +pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName
					+ "[dwIndex]));\n"
					+ "\t}\n";
			}
			else
			{//非数组
				/*
				strData+=string("\troot[\"")+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"]="
					+"data."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+";\n";
					*/
				strData += string("\tcJSON_AddNumberToObject(root,\"") + pstTypeInfo->oVecItemInfo[dwIndex]->szName + "\", " + "(double)(data." + pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName + "));\n";
			}
		}


	}

	/*
	fprintf( m_fpCpp, "Json::Value GetJson%s(%s &data)\n"
		"{\n"
		"\tJson::Value root;\n"
		"%s\n"
		"\treturn root;\n"
		"}\n", pstTypeInfo->szName, pstTypeInfo->szName, strData.c_str() );
		*/

	fprintf( m_fpCpp, "cJSON* GetJson%s(%s &data)\n"
		"{\n"
		"\tcJSON *root = cJSON_CreateObject();\n"
		"%s\n"
		"\treturn root;\n"
		"}\n", pstTypeInfo->szName, pstTypeInfo->szName, strData.c_str() );
	
	return true;
}

bool CClassMaker::_WriteFromJsonFuncToCpp(STypeInfo* pstTypeInfo)
{
	string strData;
	for( UINT32 dwIndex=0; dwIndex<pstTypeInfo->oVecItemInfo.size(); ++dwIndex )
	{
		string strRootDes=string("root[\"")+pstTypeInfo->oVecItemInfo[dwIndex]->szName+"\"]";
		if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_UNION )
		{
			continue;
		}
		else if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_UINT64 )
		{
			strData+="\tdata."+string( pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName )+string( "=SDAtou64( " )+strRootDes+".asCString() );\n";
		}
		else
		{
			if( !string(pstTypeInfo->oVecItemInfo[dwIndex]->szCount).empty() )  
			{//如果是数组

				if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_CHAR 
					|| pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_TSTRING
					|| DATA_WSTRING==pstTypeInfo->oVecItemInfo[dwIndex]->nDataType
					|| DATA_STRING==pstTypeInfo->oVecItemInfo[dwIndex]->nDataType )
				{
					strData+=string( "\tif( ")+strRootDes+".type()!=Json::stringValue )\n{\n}";
					strData+=string("\telse{\n\tstrncpy( data.")+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName
						+"," +strRootDes+".asCString(),"+pstTypeInfo->oVecItemInfo[dwIndex]->szCount+ ");\n}\n";
				}
				else if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_BYTE || pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_WORD
					|| pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_DWORD ) 
				{
					string strCountName=GetCountStr( pstTypeInfo->oVecItemInfo[dwIndex]->szCount, pstTypeInfo->oVecItemInfo[dwIndex]->szReferMemberName );
					string strTypeDes=pstTypeInfo->oVecItemInfo[dwIndex]->szType;
					transform( strTypeDes.begin(), strTypeDes.end(), strTypeDes.begin(), toupper );

					strData+="\tfor( UINT32 dwIndex=0; dwIndex<"+strCountName+ "; ++dwIndex)\n"
						+"\t{\n"+"\t\tdata."+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+"[dwIndex]="
						+strTypeDes+"("+strRootDes+"[dwIndex].asUInt() );\n\t}\n";
				}
				else
				{
					string strCountName=GetCountStr( pstTypeInfo->oVecItemInfo[dwIndex]->szCount, pstTypeInfo->oVecItemInfo[dwIndex]->szReferMemberName );
					strData+="\tfor( UINT32 dwIndex=0; dwIndex<"+strCountName+ "; ++dwIndex)\n"
						+"\t{\n"+string("\t\tFromJson")+pstTypeInfo->oVecItemInfo[dwIndex]->szType+"("+strRootDes+"[dwIndex] , data."
						+ pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+"[dwIndex] );\n\t}";
				}
			}
			else
			{//非数组

				if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_BYTE || pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_WORD 
					|| pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_DWORD  )
				{
					strData+=string("\tdata.")+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+"="+strRootDes+".asUInt();\n";
				}
				else if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_SHORT || pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_INT )
				{
					strData+=string("\tdata.")+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+"="+strRootDes+".asInt();\n";
				}
				else if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_CHAR || 
					pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_TSTRING || pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_STRING )
				{
					strData+=string("\tdata.")+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+"="+strRootDes+".asCString();\n";
				}
				else if( pstTypeInfo->oVecItemInfo[dwIndex]->nDataType==DATA_STRUCT )
				{
					/*strData+=string("\tdata.")+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+"=";*/
					strData+=string("\tFromJson")+pstTypeInfo->oVecItemInfo[dwIndex]->szType+"("+strRootDes+" , data."
						+ pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+");\n";
				}
				else
				{
					strData+=string("\tdata.")+pstTypeInfo->oVecItemInfo[dwIndex]->szMemberName+"="+strRootDes+";\n";
				}
			}
		}
	}

	fprintf( m_fpCpp, "BOOL FromJson%s(Json::Value &root, %s &data)\n"
		"{\n"
		"%s\n"
		"\treturn TRUE;\n"
		"}\n", pstTypeInfo->szName, pstTypeInfo->szName, strData.c_str() );

	fprintf( m_fpCpp, "BOOL FromJson%s(const string &strData, %s &data)\n"
		"{\n"
		"\tJson::Value root;\n"
		"\tJson::Reader read;\n"
		"\tif( !read.parse( strData, root ) )\n"
		"\t{\n"
		"\t\tUSR_INFO( _SDT( \"[%%s %%d]json parse failed,data%%s\" ), MSG_MARK, strData.c_str() );\n"
		"\t\treturn FALSE;\n"
		"\t}\n"
		"\treturn FromJson%s(root, data);\n"
		"}\n", pstTypeInfo->szName, pstTypeInfo->szName, pstTypeInfo->szName );
	return true;
}


bool CClassMaker::_WriteSaveEncodeFuncToCpp(STypeInfo* pstTypeInfo)
{
	if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
	{
		fprintf( m_fpCpp, "\n"\
			"INT32 %s::EncodeSaveData(const UINT8 bySelectType, CHAR *pBuff, const UINT32 dwBuffLen)\n"\
			"{\n"\
			"\tif(NULL == pBuff)\n"\
			"\t{\n"\
			"\t\treturn -1;\n"\
			"\t}\n"\
			"\tINT32 nEncodeRet = -1;\n"\
			"\tswitch(bySelectType)\n"\
			"\t{\n",
			pstTypeInfo->szName);

		for( UINT32 dwIndex=0; dwIndex<pstTypeInfo->oVecItemInfo.size(); ++dwIndex )
		{
			SItemInfo* pstItem = pstTypeInfo->oVecItemInfo[dwIndex];
			fprintf( m_fpCpp, "\tcase %s:\n"\
				"\t\t{\n"\
				"\t\t\tnEncodeRet = %s.EncodeSaveData(pBuff, dwBuffLen);\n"\
				"\t\t}\n"\
				"\t\tbreak;\n",
			pstItem->szID,
			pstItem->szMemberName);
		}

		fprintf( m_fpCpp, "\tdefault:\n"\
			"\t\tbreak;\n"\
			"\t}\n\n"\
			"\treturn nEncodeRet;\n"\
			"}\n\n");
	}
	else
	{
		fprintf( m_fpCpp, "\n"\
			"INT32 %s::EncodeSaveData(CHAR *pBuff, const UINT32 dwBuffLen)\n"\
			"{\n"\
			"\tif(NULL == pBuff)\n"\
			"\t{\n"\
			"\t\treturn -1;\n"\
			"\t}\n"\
			"\tUINT8 byIdxNum = %d;\n"\
			"\t*pBuff = byIdxNum;\n"\
			"\tUINT16 wIndexAddrLen = byIdxNum * sizeof(SIndex) + INDEX_NUM_HEAD_SIZE;\n"\
			"\tCHAR *pDataBuff = pBuff + wIndexAddrLen;\n"\
			"\tUINT32 dwOffset = 0;\n"\
			"\tUINT32 dwContextLenTmp = 0;\n"\
			"\tUINT32 dwOffsetTmp = 0;\n"\
			"\tSIndex* pstIndex = (SIndex*)(pBuff + INDEX_NUM_HEAD_SIZE);\n",
			pstTypeInfo->szName,
			pstTypeInfo->oVecItemInfo.size());

		map<UINT8, UINT8> mapConflictIdx;
		for( UINT32 dwIndex=0; dwIndex<pstTypeInfo->oVecItemInfo.size(); ++dwIndex )
		{		
			SItemInfo* pstItem = pstTypeInfo->oVecItemInfo[dwIndex];
			UINT8 byKeyIdx = atoi(pstItem->szKeyIdx);
			if(0 == byKeyIdx)
			{
				m_strErrMsg.Format("<type name=%s>\r\n<item name=%s type=%s>\r\nKeyIdx[%s] invalid!", 
					pstTypeInfo->szName, pstItem->szName, pstItem->szType, pstItem->szKeyIdx);
				return false;
			}
			if(mapConflictIdx.find(byKeyIdx) != mapConflictIdx.end())
			{
				m_strErrMsg.Format("<type name=%s>\r\n<item name=%s type=%s>\r\nKeyIdx[%s] Conflict!", 
					pstTypeInfo->szName, pstItem->szName, pstItem->szType, pstItem->szKeyIdx);
				return false;
			}
			mapConflictIdx[byKeyIdx] = byKeyIdx;

			fprintf( m_fpCpp, "\n\tpstIndex->byIdx = %d;\n",byKeyIdx);

			//简单类型
			if((DATA_STRUCT != pstItem->nDataType) && ('\0' == pstItem->szCount[0]))
			{
				fprintf( m_fpCpp, "\tpstIndex->dwContextLen = sizeof(%s);\n"\
					"\tmemcpy(pDataBuff + dwOffset, &%s, sizeof(%s));\n"\
					"\tdwOffset += pstIndex->dwContextLen;\n",				
					pstItem->szMemberName,
					pstItem->szMemberName,pstItem->szMemberName);
			}
			//简单类型_数组
			else if((DATA_STRUCT != pstItem->nDataType) && ('\0' != pstItem->szCount[0]) && ('\0' == pstItem->szReferMemberName[0]))
			{
				fprintf( m_fpCpp, "\tpstIndex->dwContextLen = sizeof(%s) + ARRAY_NUM_HEAD_SIZE;\n"\
					"\t*(UINT16*)(pDataBuff + dwOffset) = %s;\n"\
					"\tmemcpy(pDataBuff + dwOffset + ARRAY_NUM_HEAD_SIZE, &%s, sizeof(%s));\n"\
					"\tdwOffset += pstIndex->dwContextLen;\n",				
					pstItem->szMemberName,
					pstItem->szCount,
					pstItem->szMemberName,pstItem->szMemberName);	
			}
			//简单类型_数组_refer
			else if((DATA_STRUCT != pstItem->nDataType) && ('\0' != pstItem->szCount[0]) && ('\0' != pstItem->szReferMemberName[0]))
			{
				CString strDef;
				_GetDef(pstItem, strDef);
				fprintf( m_fpCpp, "\tif(%s > %s)\n"\
					"\t{\n"\
					"\t\t%s = %s;\n"\
					"\t}\n"\
					"\tpstIndex->dwContextLen = %s * sizeof(%s) + ARRAY_NUM_HEAD_SIZE;\n"\
					"\t*(UINT16*)(pDataBuff + dwOffset) = %s;\n"\
					"\tmemcpy(pDataBuff + dwOffset + ARRAY_NUM_HEAD_SIZE, &%s, %s * sizeof(%s));\n"\
					"\tdwOffset += pstIndex->dwContextLen;\n",		
					pstItem->szReferMemberName, pstItem->szCount,
					pstItem->szReferMemberName, pstItem->szCount,
					pstItem->szReferMemberName, strDef.GetString(),
					pstItem->szReferMemberName,
					pstItem->szMemberName, pstItem->szReferMemberName, strDef.GetString());			
			}
			//复合类型
			else if(((DATA_STRUCT == pstItem->nDataType) || (DATA_UNION == pstItem->nDataType)) && ('\0' == pstItem->szCount[0]))
			{
				fprintf( m_fpCpp, "\tpstIndex->dwContextLen = %s.EncodeSaveData(pDataBuff + dwOffset, dwBuffLen-wIndexAddrLen-dwOffset);\n"\
					"\tdwOffset += pstIndex->dwContextLen;\n",				
					pstItem->szMemberName);			
			}
			//复合类型_数组
			else if(((DATA_STRUCT == pstItem->nDataType) || (DATA_UNION == pstItem->nDataType)) && ('\0' != pstItem->szCount[0]) && ('\0' == pstItem->szReferMemberName[0]))
			{
				fprintf( m_fpCpp, "\tdwContextLenTmp = 0;\n"\
					"\t*(UINT16*)(pDataBuff + dwOffset) = %s;\n"\
					"\tdwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;\n"\
					"\tfor(UINT16 i = 0; i < %s; i++)\n"\
					"\t{\n"\
					"\t\tdwContextLenTmp += %s[i].EncodeSaveData(pDataBuff + dwOffsetTmp, dwBuffLen-wIndexAddrLen-dwOffsetTmp);\n"\
					"\t\tdwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE+ dwContextLenTmp;\n"\
					"\t}\n"\
					"\tpstIndex->dwContextLen = dwContextLenTmp + ARRAY_NUM_HEAD_SIZE;\n"\
					"\tdwOffset += pstIndex->dwContextLen;\n",
					pstItem->szCount,
					pstItem->szCount,
					pstItem->szMemberName);

			}
			//复合类型_数组_refer
			else if(((DATA_STRUCT == pstItem->nDataType) || (DATA_UNION == pstItem->nDataType)) && ('\0' != pstItem->szCount[0]) && ('\0' != pstItem->szReferMemberName[0]))
			{
				fprintf( m_fpCpp, "\tdwContextLenTmp = 0;\n"\
					"\tif(%s > %s)\n"\
					"\t{\n"\
					"\t\t%s = %s;\n"\
					"\t}\n"\
					"\t*(UINT16*)(pDataBuff + dwOffset) = %s;\n"\
					"\tdwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;\n"\
					"\tfor(UINT16 i = 0; i < %s; i++)\n"\
					"\t{\n"\
					"\t\tdwContextLenTmp += %s[i].EncodeSaveData(pDataBuff + dwOffsetTmp, dwBuffLen-wIndexAddrLen-dwOffsetTmp);\n"\
					"\t\tdwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE+ dwContextLenTmp;\n"\
					"\t}\n"\
					"\tpstIndex->dwContextLen = dwContextLenTmp + ARRAY_NUM_HEAD_SIZE;\n"\
					"\tdwOffset += pstIndex->dwContextLen;\n",		
					pstItem->szReferMemberName, pstItem->szCount,
					pstItem->szReferMemberName, pstItem->szCount,
					pstItem->szReferMemberName,
					pstItem->szReferMemberName,
					pstItem->szMemberName);		
			}
			else
			{
				m_strErrMsg.Format("<type name=%s>\r\n<item name=%s type=%s>\r\n DataType[%s] invalid!", 
					pstTypeInfo->szName, pstItem->szName, pstItem->szType, pstItem->szType);
				return false;
			}

			fprintf( m_fpCpp, "\tpstIndex++;\n",byKeyIdx);
		}

		fprintf( m_fpCpp, "\treturn wIndexAddrLen + dwOffset;\n"\
			"}\n");

	}
	

	return true;
}

bool CClassMaker::_WriteSaveDecodeFuncToCpp(STypeInfo* pstTypeInfo)
{
	if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
	{
		fprintf( m_fpCpp, "\n"\
			"INT32 %s::DecodeSaveData(const UINT8 bySelectType, const CHAR *pBuff, const UINT32 dwBuffLen)\n"\
			"{\n"\
			"\tif(NULL == pBuff)\n"\
			"\t{\n"\
			"\t\treturn -1;\n"\
			"\t}\n"\
			"\tINT32 nEncodeRet = -1;\n"\
			"\tswitch(bySelectType)\n"\
			"\t{\n",
			pstTypeInfo->szName);

		for( UINT32 dwIndex=0; dwIndex<pstTypeInfo->oVecItemInfo.size(); ++dwIndex )
		{
			SItemInfo* pstItem = pstTypeInfo->oVecItemInfo[dwIndex];
			fprintf( m_fpCpp, "\tcase %s:\n"\
				"\t\t{\n"\
				"\t\t\tnEncodeRet = %s.DecodeSaveData(pBuff, dwBuffLen);\n"\
				"\t\t}\n"\
				"\t\tbreak;\n",
				pstItem->szID,
				pstItem->szMemberName);
		}

		fprintf( m_fpCpp, "\tdefault:\n"\
			"\t\tbreak;\n"\
			"\t}\n\n"\
			"\treturn nEncodeRet;\n"\
			"}\n\n");
	}
	else
	{
		fprintf( m_fpCpp, "\n"\
			"INT32 %s::DecodeSaveData(const CHAR *pBuff, const UINT32 dwBuffLen)\n"\
			"{\n"\
			"\tif(NULL == pBuff)\n"\
			"\t{\n"\
			"\t\treturn -1;\n"\
			"\t}\n"\
			"\tUINT8 byIdxNum = (UINT8)(*pBuff);\n"\
			"\tUINT16 wIndexAddrLen = byIdxNum * sizeof(SIndex) + INDEX_NUM_HEAD_SIZE;\n"\
			"\tif(wIndexAddrLen >= dwBuffLen)\n"\
			"\t{\n"\
			"\t\treturn -1;\n"\
			"\t}\n"\
			"\tCHAR *pDataBuff = (CHAR*)pBuff + wIndexAddrLen;\n\n"\
			"\tUINT32 dwOffset = 0;\n"\
			"\tSIndex* pstIndex = (SIndex*)(pBuff + INDEX_NUM_HEAD_SIZE);\n"\
			"\twhile(byIdxNum-- > 0)\n"\
			"\t{\n"
			"\t\tswitch(pstIndex->byIdx)\n"\
			"\t\t{\n",
			pstTypeInfo->szName);

		for( UINT32 dwIndex=0; dwIndex<pstTypeInfo->oVecItemInfo.size(); ++dwIndex )
		{		
			SItemInfo* pstItem = pstTypeInfo->oVecItemInfo[dwIndex];
			fprintf( m_fpCpp, "\t\tcase %s:\n"\
				"\t\t\t{\n",
				pstItem->szKeyIdx);

			//////////////////////////////////////////

			//简单类型
			if((DATA_STRUCT != pstItem->nDataType) && ('\0' == pstItem->szCount[0]))
			{
				fprintf( m_fpCpp, "\t\t\t\tif(sizeof(%s) >= pstIndex->dwContextLen)\n"\
					"\t\t\t\t{\n"\
					"\t\t\t\t\tmemcpy(&%s, (pDataBuff + dwOffset), pstIndex->dwContextLen);\n"\
					"\t\t\t\t}\n"\
					"\t\t\t\telse\n"\
					"\t\t\t\t{\n"\
					"\t\t\t\t\tmemset(&%s, 0, sizeof(%s));\n"\
					"\t\t\t\t}\n",
					pstItem->szMemberName,
					pstItem->szMemberName,
					pstItem->szMemberName, pstItem->szMemberName	);
			}
			//简单类型_数组
			else if((DATA_STRUCT != pstItem->nDataType) && ('\0' != pstItem->szCount[0]) && ('\0' == pstItem->szReferMemberName[0]))
			{
				fprintf( m_fpCpp, "\t\t\t\tUINT16 wArrNum = *(UINT16*)(pDataBuff + dwOffset);\n"\
					"\t\t\t\tUINT32 dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;\n"\
					"\t\t\t\tUINT32 dwContextLen = pstIndex->dwContextLen - ARRAY_NUM_HEAD_SIZE;\n"\
					"\t\t\t\tif((sizeof(%s) >= dwContextLen) && (wArrNum <= %s))\n"\
					"\t\t\t\t{\n"\
					"\t\t\t\t\tmemcpy(&%s, (pDataBuff + dwOffsetTmp), dwContextLen);\n"\
					"\t\t\t\t}\n"\
					"\t\t\t\telse\n"\
					"\t\t\t\t{\n"\
					"\t\t\t\t\tmemset(&%s, 0, sizeof(%s));\n"\
					"\t\t\t\t}\n",
					pstItem->szMemberName, pstItem->szCount,
					pstItem->szMemberName,
					pstItem->szMemberName, pstItem->szMemberName	);
			}
			//简单类型_数组_refer
			else if((DATA_STRUCT != pstItem->nDataType) && ('\0' != pstItem->szCount[0]) && ('\0' != pstItem->szReferMemberName[0]))
			{
				fprintf( m_fpCpp, "\t\t\t\tUINT16 wArrNum = *(UINT16*)(pDataBuff + dwOffset);\n"\
					"\t\t\t\tUINT32 dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;\n"\
					"\t\t\t\tUINT32 dwContextLen = pstIndex->dwContextLen - ARRAY_NUM_HEAD_SIZE;\n"\
					"\t\t\t\tif((sizeof(%s) >= dwContextLen) && (wArrNum == %s) && (wArrNum <= %s))\n"\
					"\t\t\t\t{\n"\
					"\t\t\t\t\tmemcpy(&%s, (pDataBuff + dwOffsetTmp), dwContextLen);\n"\
					"\t\t\t\t}\n"\
					"\t\t\t\telse\n"\
					"\t\t\t\t{\n"\
					"\t\t\t\t\tmemset(&%s, 0, sizeof(%s));\n"\
					"\t\t\t\t}\n",
					pstItem->szMemberName, pstItem->szReferMemberName, pstItem->szCount,
					pstItem->szMemberName,
					pstItem->szMemberName, pstItem->szMemberName	);
			}
			//复合类型
			else if((DATA_STRUCT == pstItem->nDataType) && ('\0' == pstItem->szCount[0]) && ('\0' == pstItem->szReferMemberName[0]))
			{
				fprintf( m_fpCpp, "\t\t\t\t%s.DecodeSaveData(pDataBuff + dwOffset, pstIndex->dwContextLen);\n",
					pstItem->szMemberName);			
			}
			//复合类型_数组
			else if((DATA_STRUCT == pstItem->nDataType) && ('\0' != pstItem->szCount[0]) && ('\0' == pstItem->szReferMemberName[0]))
			{
				fprintf( m_fpCpp, "\t\t\t\tUINT16 wArrNum = *(UINT16*)(pDataBuff + dwOffset);\n"\
					"\t\t\t\tUINT32 dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;\n"\
					"\t\t\t\tUINT32 dwPassContextLen = 0;\n"\
					"\t\t\t\twArrNum = wArrNum <= %s ? wArrNum : %s;\n"\
					"\t\t\t\t\tfor(UINT16 i = 0; i < wArrNum; i++)\n"\
					"\t\t\t\t{\n"\
					"\t\t\t\t\tdwPassContextLen += %s[i].DecodeSaveData(pDataBuff + dwOffsetTmp, pstIndex->dwContextLen - dwPassContextLen);\n"\
					"\t\t\t\t\tif(dwPassContextLen >= pstIndex->dwContextLen)\n"\
					"\t\t\t\t\t{\n"\
					"\t\t\t\t\t\tmemset(%s, 0, sizeof(%s));\n"\
					"\t\t\t\t\t\tbreak;\n"\
					"\t\t\t\t\t}\n"\
					"\t\t\t\t\tdwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE + dwPassContextLen;\n"\
					"\t\t\t\t}\n", 
					pstItem->szCount, pstItem->szCount, 
					pstItem->szMemberName, 
					pstItem->szMemberName, pstItem->szMemberName	);
			}
			//复合类型_数组_refer
			else if((DATA_STRUCT == pstItem->nDataType) && ('\0' != pstItem->szCount[0]) && ('\0' != pstItem->szReferMemberName[0]))
			{
				fprintf( m_fpCpp, "\t\t\t\tUINT16 wArrNum = *(UINT16*)(pDataBuff + dwOffset);\n"\
					"\t\t\t\tUINT32 dwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE;\n"\
					"\t\t\t\tUINT32 dwPassContextLen = 0;\n"\
					"\t\t\t\tif(wArrNum == %s)\n"\
					"\t\t\t\t{\n"\
					"\t\t\t\t\twArrNum = wArrNum <= %s ? wArrNum : %s;\n"\
					"\t\t\t\t\t%s = (%s)wArrNum;\n"\
					"\t\t\t\t\tfor(UINT16 i = 0; i < wArrNum; i++)\n"\
					"\t\t\t\t\t{\n"\
					"\t\t\t\t\t\tdwPassContextLen += %s[i].DecodeSaveData(pDataBuff + dwOffsetTmp, pstIndex->dwContextLen - dwPassContextLen);\n"\
					"\t\t\t\t\t\tif(dwPassContextLen >= pstIndex->dwContextLen)\n"\
					"\t\t\t\t\t\t{\n"\
					"\t\t\t\t\t\t\tmemset(%s, 0, sizeof(%s));\n"\
					"\t\t\t\t\t\t\tbreak;\n"\
					"\t\t\t\t\t\t}\n"\
					"\t\t\t\t\t\tdwOffsetTmp = dwOffset + ARRAY_NUM_HEAD_SIZE + dwPassContextLen;\n"\
					"\t\t\t\t\t}\n"\
					"\t\t\t\t}\n"\
					"\t\t\t\telse\n"\
					"\t\t\t\t{\n"\
					"\t\t\t\t\tmemset(&%s, 0, sizeof(%s));\n"\
					"\t\t\t\t}\n",
					pstItem->szReferMemberName, 
					pstItem->szCount, pstItem->szCount,
					pstItem->szReferMemberName, _GetSimpleDef(pstItem->nReferDataType), 
					pstItem->szMemberName, 
					pstItem->szMemberName, pstItem->szMemberName,
					pstItem->szMemberName, pstItem->szMemberName	);
			}
			else
			{
				m_strErrMsg.Format("<type name=%s>\r\n<item name=%s type=%s>\r\n DataType[%s] invalid!", 
					pstTypeInfo->szName, pstItem->szName, pstItem->szType, pstItem->szType);
				return false;
			}

			/////////////////////////////////////////
			fprintf( m_fpCpp, "\t\t\t}\n"\
				"\t\t\tbreak;\n"
				);
		}

		fprintf( m_fpCpp, "\t\tdefault:\n"\
			"\t\t\tbreak;\n"\
			"\t\t}\n\n"\
			"\t\tdwOffset += pstIndex->dwContextLen;\n"\
			"\t\tpstIndex++;\n"\
			"\t}\n\n"\
			"\treturn wIndexAddrLen + dwOffset;\n"\
			"}\n\n"
			);
	}
	return true;
}



string CClassMaker::GetCountStr( const string &strCountName, const string &strReferName )
{
	if( strReferName!="" )
	{
		return m_strParamName+"."+strReferName;
	}
	else
	{
		return strCountName;
	}
}

bool CClassMaker::_WriteDecodeFuncToCpp(STypeInfo* pstTypeInfo)
{
	fprintf(m_fpCpp,
		"INT32 Decode%s(void *pHost, CNetData* poNetData)\n"
		"{\n"
		"	%s *pstOut = (%s*)pHost;\n"
		"\n",
		pstTypeInfo->szName,
		pstTypeInfo->szName,
		pstTypeInfo->szName, pstTypeInfo->szName);
	if ( pstTypeInfo->nIgnored > 0 )
	{
		int nNum =  pstTypeInfo->nIgnored / 8 ;
		if (pstTypeInfo->nIgnored % 8)
		{
			nNum += 1 ;
		}
		fprintf(m_fpCpp,"	CHAR szTemp[%d] = { 0 };\n" , nNum );
		fprintf(m_fpCpp,"	if(-1 == poNetData->DelString(szTemp, sizeof(szTemp)))\n"  );
		fprintf(m_fpCpp,"		return -1;\n\n"  );		
		fprintf(m_fpCpp,"	for ( INT32 t = 0; t < %d ;  t ++ )\n" ,nNum );		
		fprintf(m_fpCpp,"	{\n"  );	
		for ( int n = 0 ; n < nNum ; n ++ )
		{
			fprintf(m_fpCpp,"		bitset<8> bit((UINT32)szTemp[t]);\n"  );
			fprintf(m_fpCpp,"		for(UINT32 m = 0 ; m < 8; m ++ )\n"  );
			fprintf(m_fpCpp,"			pstOut->bsIgnored.set(t * 8 + m , bit.test(m)) ;\n"  );
		}
		fprintf(m_fpCpp,"	}\n\n"  );		
	}
	bool bDefI = false;
	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{
		if('\0' != pstTypeInfo->oVecItemInfo[i]->szCount[0] 
		&& DATA_STRING != pstTypeInfo->oVecItemInfo[i]->nDataType
			&& DATA_WSTRING != pstTypeInfo->oVecItemInfo[i]->nDataType
			&& DATA_TSTRING != pstTypeInfo->oVecItemInfo[i]->nDataType)
		{
			if(false == bDefI)
			{
				fprintf(m_fpCpp, "	INT32 i;\n");
				bDefI = true;
			}
		}
		if (pstTypeInfo->oVecItemInfo[i]->bIgnored)
		{
			fprintf(m_fpCpp,"	if(pstOut->Get%s())\n",pstTypeInfo->oVecItemInfo[i]->szName );
			fprintf(m_fpCpp, "	{\n");
			if(false == _WriteDecodeItemToCpp(pstTypeInfo->oVecItemInfo[i],true))
				return false;
			fprintf(m_fpCpp, "	}\n\n");
		}
		else
		{
			if(false == _WriteDecodeItemToCpp(pstTypeInfo->oVecItemInfo[i]))
				return false;
		}

	}

	fprintf(m_fpCpp,
		"	return sizeof(*pstOut);\n"
		"}\n\n");

	//fprintf(m_fpCpp,
	//	"	return poNetData->GetDataLen();\n"
	//	"}\n\n");

	return true;
}

bool CClassMaker::_WriteEncodeUnionToCpp(STypeInfo* pstTypeInfo)
{
	fprintf(m_fpCpp,
		"INT32 EncodeUn%s(void *pHost, INT32 iSelect, CNetData* poNetData)\n"
		"{\n"
		"\tswitch(iSelect){\n",
		pstTypeInfo->szName);

	//// 2009-03-11 cwy add
	// for union contains single type
	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{	
		//fprintf(m_fpCpp, "	case %s: return Encode%s(pHost, poNetData);\n", pstTypeInfo->oVecItemInfo[i].szID, pstTypeInfo->oVecItemInfo[i].szType);
		fprintf(m_fpCpp, 
			"\tcase %s:\n", pstTypeInfo->oVecItemInfo[i]->szID);
		if (DATA_STRUCT == pstTypeInfo->oVecItemInfo[i]->nDataType)
		{
			fprintf(m_fpCpp, 
				"\t\treturn Encode%s(pHost, poNetData);\n", pstTypeInfo->oVecItemInfo[i]->szType);
			continue;
		}
		else if (false == _WriteEncodeItemToUnion(pstTypeInfo->oVecItemInfo[i]))
		{

			return false;
		}		
	}
	//// 2009-03-11 cwy end

	fprintf(m_fpCpp,
		"	default: return -1;\n"
		"	}\n"
		"}\n\n");

	return true;
}

bool CClassMaker::_WriteDecodeUnionToCpp(STypeInfo* pstTypeInfo)
{
	fprintf(m_fpCpp,
		"INT32 DecodeUn%s(void *pHost, INT32 iSelect, CNetData* poNetData)\n"
		"{\n"
		"\tswitch(iSelect){\n",
		pstTypeInfo->szName);

	//// 2009-03-11 cwy add
	// for union contains single type
	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{
		fprintf(m_fpCpp, 
			"\tcase %s:\n", pstTypeInfo->oVecItemInfo[i]->szID);

		if (DATA_STRUCT == pstTypeInfo->oVecItemInfo[i]->nDataType)
		{
			fprintf(m_fpCpp, 
				"\t\treturn Decode%s(pHost, poNetData);\n", pstTypeInfo->oVecItemInfo[i]->szType);
			continue;
		}
		else if (false == _WriteDecodeItemToUnion(pstTypeInfo->oVecItemInfo[i]))
		{
			return false;
		}		
	}
	//// 2009-03-11 cwy end

	fprintf(m_fpCpp,
		"	default: return -1;\n"
		"	}\n"
		"}\n\n");

	return true;
}

bool CClassMaker::_WriteEncodeItemToCpp(SItemInfo* pstInfo,bool bIsIngnored)
{
	CString strAddFunc;
	switch(pstInfo->nDataType){
	case DATA_CHAR:
		strAddFunc = "AddChar";
		break;
	case DATA_BYTE:
		strAddFunc = "AddByte";
		break;
	case DATA_SHORT:
		strAddFunc = "AddShort";
		break;
	case DATA_WORD:
		strAddFunc = "AddWord";
		break;
	case DATA_INT:
		strAddFunc = "AddInt";
		break;
	case DATA_DWORD:
		strAddFunc = "AddDword";
		break;
	case DATA_INT64:
		strAddFunc = "AddInt64";
		break;
	case DATA_UINT64:
		strAddFunc = "AddUint64";
		break;
	case DATA_FLOAT:
		strAddFunc = "AddFloat";
		break;
	case DATA_STRING:
		{
			if (bIsIngnored)
			{
				fprintf(m_fpCpp,
					"		if(-1 == poNetData->AddString(pstIn->%s, sizeof(pstIn->%s)))\n"
					"			return -1;\n",
					pstInfo->szMemberName, pstInfo->szMemberName);
			}
			else
			{
				fprintf(m_fpCpp,
					"	if(-1 == poNetData->AddString(pstIn->%s, sizeof(pstIn->%s)))\n"
					"		return -1;\n\n",
					pstInfo->szMemberName, pstInfo->szMemberName);
			}
		}
		return true;

	case DATA_WSTRING:
		{
			if (bIsIngnored)
			{
				fprintf(m_fpCpp,
					"		if(-1 == poNetData->AddWString(pstIn->%s, sizeof(pstIn->%s)))\n"
					"			return -1;\n",
					pstInfo->szMemberName, pstInfo->szMemberName);
			}
			else
			{
				fprintf(m_fpCpp,
					"	if(-1 == poNetData->AddWString(pstIn->%s, sizeof(pstIn->%s)))\n"
					"		return -1;\n\n",
					pstInfo->szMemberName, pstInfo->szMemberName);
			}
		}
		return true;
		break;
	case DATA_TSTRING:
		{
			if (bIsIngnored)
			{
				fprintf(m_fpCpp,
					"		if(-1 == poNetData->AddTString(pstIn->%s, sizeof(pstIn->%s)))\n"
					"			return -1;\n",
					pstInfo->szMemberName, pstInfo->szMemberName);
			}
			else
			{
				fprintf(m_fpCpp,
					"	if(-1 == poNetData->AddTString(pstIn->%s, sizeof(pstIn->%s)))\n"
					"		return -1;\n\n",
					pstInfo->szMemberName, pstInfo->szMemberName);
			}
		}
		return true;
		break;

	case DATA_STRUCT:
		{	
			if(pstInfo->szCount[0] != '\0')
			{
				if('\0' == pstInfo->szReferMemberName[0])
				{
					if (bIsIngnored)
					{
						fprintf(m_fpCpp,
							"		for(i = 0; i < %s; i++)\n"
							"		{\n"
							"			if(-1 == Encode%s(&pstIn->%s[i], poNetData))\n"
							"				return -1;\n"
							"		}\n",
							pstInfo->szCount,
							pstInfo->szType, pstInfo->szMemberName);
					}
					else
					{
						fprintf(m_fpCpp,
							"	for(i = 0; i < %s; i++)\n"
							"	{\n"
							"		if(-1 == Encode%s(&pstIn->%s[i], poNetData))\n"
							"			return -1;\n"
							"	}\n\n",
							pstInfo->szCount,
							pstInfo->szType, pstInfo->szMemberName);
					}
				}
				else
				{
					if (bIsIngnored)
					{
						fprintf(m_fpCpp,
							"		for(i = 0; i < %s; i++)\n"
							"		{\n"
							"			if(i >= (INT32)pstIn->%s)\n"
							"				break;\n"
							"			if(-1 == Encode%s(&pstIn->%s[i], poNetData))\n"
							"				return -1;\n"
							"		}\n",
							pstInfo->szCount,
							pstInfo->szReferMemberName,
							pstInfo->szType, pstInfo->szMemberName);
					}
					else
					{
						fprintf(m_fpCpp,
							"	for(i = 0; i < %s; i++)\n"
							"	{\n"
							"		if(i >= (INT32)pstIn->%s)\n"
							"			break;\n"
							"		if(-1 == Encode%s(&pstIn->%s[i], poNetData))\n"
							"			return -1;\n"
							"	}\n\n",
							pstInfo->szCount,
							pstInfo->szReferMemberName,
							pstInfo->szType, pstInfo->szMemberName);
					}

				}
			}
			else
			{
				if (bIsIngnored)
				{
					fprintf(m_fpCpp,
						"		if(-1 == Encode%s(&pstIn->%s, poNetData))\n"
						"			return -1;\n",
						pstInfo->szType, pstInfo->szMemberName);
				}
				else
				{
					fprintf(m_fpCpp,
						"	if(-1 == Encode%s(&pstIn->%s, poNetData))\n"
						"		return -1;\n\n",
						pstInfo->szType, pstInfo->szMemberName);
				}

			}
		}
		return true;
	case DATA_UNION:
		{
			if (bIsIngnored)
			{
				fprintf(m_fpCpp,
					"		if(-1 == EncodeUn%s(&pstIn->%s, pstIn->%s, poNetData))\n"
					"			return -1;\n",
					pstInfo->szType, pstInfo->szMemberName, pstInfo->szSelect);
			}
			else
			{
				fprintf(m_fpCpp,
					"	if(-1 == EncodeUn%s(&pstIn->%s, pstIn->%s, poNetData))\n"
					"		return -1;\n\n",
					pstInfo->szType, pstInfo->szMemberName, pstInfo->szSelect);
			}


		}
		return true;
	default:
		return false;
	}

	CHAR szBuff[512] = {0};
	if(m_bCliEnDe)
	{
		if(pstInfo->szCount[0] != '\0')
		{
			sprintf(szBuff, "if(-1 == poNetData->%s(pstIn->%s[i], stIndex.dwContextLen))", strAddFunc, pstInfo->szMemberName);
		}
		else
		{
			sprintf(szBuff, "if(-1 == poNetData->%s(pstIn->%s, stIndex.dwContextLen))", strAddFunc, pstInfo->szMemberName);
		}
	}
	else
	{
		if(pstInfo->szCount[0] != '\0')
		{
			sprintf(szBuff, "if(-1 == poNetData->%s(pstIn->%s[i]))", strAddFunc, pstInfo->szMemberName);
		}
		else
		{
			sprintf(szBuff, "if(-1 == poNetData->%s(pstIn->%s))", strAddFunc, pstInfo->szMemberName);
		}		
	}

	if(pstInfo->szCount[0] != '\0')
	{
		if('\0' == pstInfo->szReferMemberName[0])
		{
			if (bIsIngnored)
			{
				fprintf(m_fpCpp,
					"		for(i = 0; i < %s; i++)\n"
					"		{\n"
					"			%s\n"
					"				return -1;\n"
					"		}\n",
					pstInfo->szCount,
					szBuff);
			}
			else
			{
				fprintf(m_fpCpp,
					"	for(i = 0; i < %s; i++)\n"
					"	{\n"
					"		%s\n"
					"			return -1;\n"
					"	}\n\n",
					pstInfo->szCount,
					szBuff);
			}

		}
		else
		{
			if (bIsIngnored)
			{
				fprintf(m_fpCpp,
					"		for(i = 0; i < %s; i++)\n"
					"		{\n"
					"			if(i >= (INT32)pstIn->%s)\n"
					"				break;\n"
					"			%s\n"
					"				return -1;\n"
					"		}\n",
					pstInfo->szCount,
					pstInfo->szReferMemberName,
					szBuff);
			}
			else
			{
				fprintf(m_fpCpp,
					"	for(i = 0; i < %s; i++)\n"
					"	{\n"
					"		if(i >= (INT32)pstIn->%s)\n"
					"			break;\n"
					"		%s\n"
					"			return -1;\n"
					"	}\n\n",
					pstInfo->szCount,
					pstInfo->szReferMemberName,
					szBuff);
			}
		}
	}
	else
	{	
		if (bIsIngnored)
		{
			fprintf(m_fpCpp,
				"		%s\n"
				"			return -1;\n",
				szBuff);
		}
		else
		{
			fprintf(m_fpCpp,
				"	%s\n"
				"		return -1;\n"
				"\n",
				szBuff);
		}


		//// 2009-03-10 cwy add begin
		// for none-follow attribute
		//
		if ('\0' != pstInfo->szNoneFollow[0])
		{		
			char * szID,* szNextID;
			char szTokens[] = " ,;.";
			char szNoneFollow[MAX_NAME_LEN];

			strcpy_s(szNoneFollow, MAX_NAME_LEN, pstInfo->szNoneFollow);
			szID = strtok_s(szNoneFollow, szTokens, &szNextID);

			while (NULL != szID)
			{
				if (bIsIngnored)
				{
					fprintf(m_fpCpp,
						"		if(%s == pstIn->%s)\n"
						"			return poNetData->GetDataLen();\n",
						szID, pstInfo->szMemberName);
				}
				else
				{
					fprintf(m_fpCpp,
						"	if(%s == pstIn->%s)\n"
						"		return poNetData->GetDataLen();\n\n",
						szID, pstInfo->szMemberName);
				}

				szID = strtok_s(NULL, szTokens, &szNextID);
			}
		}
		//// 2009-03-10 cwy add end
	}

	return true;
}

bool CClassMaker::_WriteDecodeItemToCpp(SItemInfo* pstInfo,bool bIsIngnored)
{
	CString strDelFunc;
	switch(pstInfo->nDataType){
	case DATA_CHAR:
		strDelFunc = "DelChar";
		break;
	case DATA_BYTE:
		strDelFunc = "DelByte";
		break;
	case DATA_SHORT:
		strDelFunc = "DelShort";
		break;
	case DATA_WORD:
		strDelFunc = "DelWord";
		break;
	case DATA_INT:
		strDelFunc = "DelInt";
		break;
	case DATA_DWORD:
		strDelFunc = "DelDword";
		break;
	case DATA_INT64:
		strDelFunc = "DelInt64";
		break;
	case DATA_UINT64:
		strDelFunc = "DelUint64";
		break;
	case DATA_FLOAT:
		strDelFunc = "DelFloat";
		break;
	case DATA_STRING:
		{
			if (bIsIngnored)
			{
				fprintf(m_fpCpp,
					"		if(-1 == poNetData->DelString(pstOut->%s, sizeof(pstOut->%s)))\n"
					"			return -1;\n",
					pstInfo->szMemberName, pstInfo->szMemberName);
			}
			else
			{
				fprintf(m_fpCpp,
					"	if(-1 == poNetData->DelString(pstOut->%s, sizeof(pstOut->%s)))\n"
					"		return -1;\n\n",
					pstInfo->szMemberName, pstInfo->szMemberName);
			}

		}
		return true ;
		break;

	case DATA_WSTRING:
		{
			if (bIsIngnored)
			{
				fprintf(m_fpCpp,
					"		if(-1 == poNetData->DelWString(pstOut->%s, sizeof(pstOut->%s)))\n"
					"			return -1;\n",
					pstInfo->szMemberName, pstInfo->szMemberName);
			}
			else
			{
				fprintf(m_fpCpp,
					"	if(-1 == poNetData->DelWString(pstOut->%s, sizeof(pstOut->%s)))\n"
					"		return -1;\n\n",
					pstInfo->szMemberName, pstInfo->szMemberName);
			}

		}
		return true ;
		break;
	case DATA_TSTRING:
		{
			if (bIsIngnored)
			{
				fprintf(m_fpCpp,
					"		if(-1 == poNetData->DelTString(pstOut->%s, sizeof(pstOut->%s)))\n"
					"			return -1;\n",
					pstInfo->szMemberName, pstInfo->szMemberName);
			}
			else
			{
				fprintf(m_fpCpp,
					"	if(-1 == poNetData->DelTString(pstOut->%s, sizeof(pstOut->%s)))\n"
					"		return -1;\n\n",
					pstInfo->szMemberName, pstInfo->szMemberName);
			}

		}
		return true ;
		break;
	case DATA_STRUCT:
		{
			if(pstInfo->szCount[0] != '\0')
			{
				if('\0' == pstInfo->szReferMemberName[0])
				{
					if (bIsIngnored)
					{
						fprintf(m_fpCpp,
							"	for(i = 0; i < %s; i++)\n"
							"	{\n"
							"		if(-1 == Decode%s(&pstOut->%s[i], poNetData))\n"
							"			return -1;\n"
							"	}\n",
							pstInfo->szCount,
							pstInfo->szType, pstInfo->szMemberName);
					}
					else
					{
						fprintf(m_fpCpp,
							"		for(i = 0; i < %s; i++)\n"
							"		{\n"
							"			if(-1 == Decode%s(&pstOut->%s[i], poNetData))\n"
							"				return -1;\n"
							"		}\n\n",
							pstInfo->szCount,
							pstInfo->szType, pstInfo->szMemberName);
					}
				}
				else
				{
					if (bIsIngnored)
					{
						fprintf(m_fpCpp,
							"		for(i = 0; i < %s; i++)\n"
							"		{\n"
							"			if(i >= (INT32)pstOut->%s)\n"
							"				break;\n"
							"			if(-1 == Decode%s(&pstOut->%s[i], poNetData))\n"
							"				return -1;\n"
							"		}\n",
							pstInfo->szCount,
							pstInfo->szReferMemberName,
							pstInfo->szType, pstInfo->szMemberName);
					}
					else
					{
						fprintf(m_fpCpp,
							"	for(i = 0; i < %s; i++)\n"
							"	{\n"
							"		if(i >= (INT32)pstOut->%s)\n"
							"			break;\n"
							"		if(-1 == Decode%s(&pstOut->%s[i], poNetData))\n"
							"			return -1;\n"
							"	}\n\n",
							pstInfo->szCount,
							pstInfo->szReferMemberName,
							pstInfo->szType, pstInfo->szMemberName);
					}

				}
			}
			else
			{
				if (bIsIngnored)
				{
					fprintf(m_fpCpp,
						"		if(-1 == Decode%s(&pstOut->%s, poNetData))\n"
						"			return -1;\n",
						pstInfo->szType, pstInfo->szMemberName);
				}
				else
				{
					fprintf(m_fpCpp,
						"	if(-1 == Decode%s(&pstOut->%s, poNetData))\n"
						"		return -1;\n\n",
						pstInfo->szType, pstInfo->szMemberName);
				}

			}
		}
		return true;
		break;
	case DATA_UNION:
		{
			if (bIsIngnored)
			{
				fprintf(m_fpCpp,
					"		if(-1 == DecodeUn%s(&pstOut->%s, pstOut->%s, poNetData))\n"
					"			return -1;\n",
					pstInfo->szType, pstInfo->szMemberName, pstInfo->szSelect);
			}
			else
			{
				fprintf(m_fpCpp,
					"	if(-1 == DecodeUn%s(&pstOut->%s, pstOut->%s, poNetData))\n"
					"		return -1;\n\n",
					pstInfo->szType, pstInfo->szMemberName, pstInfo->szSelect);
			}


		}
		return true;
		break;
	default:
		return false;
	}

	CHAR szBuff[512] = {0};
	if(m_bCliEnDe)
	{
		if(pstInfo->szCount[0] != '\0')
		{
			sprintf(szBuff, "if(-1 == poNetData->%s(pstOut->%s[i], stIndex.dwContextLen))", strDelFunc, pstInfo->szMemberName);
		}
		else
		{
			sprintf(szBuff, "if(-1 == poNetData->%s(pstOut->%s, stIndex.dwContextLen))", strDelFunc, pstInfo->szMemberName);
		}
	}
	else
	{
		if(pstInfo->szCount[0] != '\0')
		{
			sprintf(szBuff, "if(-1 == poNetData->%s(pstOut->%s[i]))", strDelFunc, pstInfo->szMemberName);
		}
		else
		{
			sprintf(szBuff, "if(-1 == poNetData->%s(pstOut->%s))", strDelFunc, pstInfo->szMemberName);
		}		
	}	

	if(pstInfo->szCount[0] != '\0')
	{
		if('\0' == pstInfo->szReferMemberName[0])
		{
			if (bIsIngnored)
			{
				if(m_bCliEnDe)
				{
					fprintf(m_fpCpp,
						"		for(i = 0; i < %s; i++)\n"
						"		{\n"
						"			%s\n"
						"				return -1;\n"
						"		}\n",
						pstInfo->szCount,
						szBuff
						);
				}
				else
				{
					fprintf(m_fpCpp,
						"		for(i = 0; i < %s; i++)\n"
						"		{\n"
						"			%s\n"
						"				return -1;\n"
						"		}\n",
						pstInfo->szCount,
						szBuff);
				}
			}
			else
			{
				fprintf(m_fpCpp,
					"	for(i = 0; i < %s; i++)\n"
					"	{\n"
					"		%s\n"
					"			return -1;\n"
					"	}\n\n",
					pstInfo->szCount,
					szBuff);
			}

		}
		else
		{
			if (bIsIngnored)
			{
				fprintf(m_fpCpp,
					"		for(i = 0; i < %s; i++)\n"
					"		{\n"
					"			if(i >= (INT32)pstOut->%s)\n"
					"				break;\n"
					"			%s\n"
					"				return -1;\n"
					"		}\n",
					pstInfo->szCount,
					pstInfo->szReferMemberName, 
					szBuff);
			}
			else
			{
				fprintf(m_fpCpp,
					"	for(i = 0; i < %s; i++)\n"
					"	{\n"
					"		if(i >= (INT32)pstOut->%s)\n"
					"			break;\n"
					"		%s\n"
					"			return -1;\n"
					"	}\n\n",
					pstInfo->szCount,
					pstInfo->szReferMemberName, 
					szBuff);
			}
		}
	}
	else
	{
		if (bIsIngnored)
		{
			fprintf(m_fpCpp,
				"		%s\n"
				"			return -1;\n",
				szBuff);
		}
		else
		{
			fprintf(m_fpCpp,
				"	%s\n"
				"		return -1;\n"
				"\n",
				szBuff);
		}


		//// 2009-03-10 cwy add begin

		if ('\0' != pstInfo->szNoneFollow[0])
		{		
			char * szID,* szNextID;
			char szTokens[] = " ,;.";
			char szNoneFollow[MAX_NAME_LEN];

			strcpy_s(szNoneFollow, MAX_NAME_LEN, pstInfo->szNoneFollow);
			szID = strtok_s(szNoneFollow, szTokens, &szNextID);

			while (NULL != szID)
			{
				if (bIsIngnored)
				{
					fprintf(m_fpCpp,
						"		if(%s == pstOut->%s)\n"
						"			return sizeof(*pstOut);\n",
						szID, pstInfo->szMemberName);
					szID = strtok_s(NULL, szTokens, &szNextID);
				}
				else
				{
					fprintf(m_fpCpp,
						"	if(%s == pstOut->%s)\n"
						"		return sizeof(*pstOut);\n\n",
						szID, pstInfo->szMemberName);

					szID = strtok_s(NULL, szTokens, &szNextID);
				}

			}
		}
		//// 2009-03-10 cwy add end
	}

	return true;
}


bool CClassMaker::_WriteCliEncodeFuncToCpp(STypeInfo* pstTypeInfo)
{	
	fprintf(m_fpCpp,
		"INT32 Encode%s(void *pHost, CNetData* poNetData)\n"
		"{\n"
		"	%s *pstIn = (%s*)pHost;\n"
		"	UINT32 dwStructId = %d; \n"
		"	CStructIndexMap& mapStructIndex = %s::Instance()->GetStructIndexMap();\n"
		"	CUnionIDIndexMap& mapUnionIDIndex = %s::Instance()->GetUnionIDIndexMap();\n"
		"	CIndexMap& mapIndex = GetStructIndex(mapStructIndex, dwStructId);\n"
		"	if(0 == mapIndex.size())\n"
		"	{\n"
		"		return -1;\n"
		"	}\n"
		"\n",
		pstTypeInfo->szName,
		pstTypeInfo->szName, pstTypeInfo->szName,
		pstTypeInfo->nStructID,
		m_strClassName,
		m_strClassName);

	bool bDefI =false;
	if ( pstTypeInfo->nIgnored > 0 )
	{
		int nNum =  pstTypeInfo->nIgnored / 8 ;
		if (pstTypeInfo->nIgnored % 8)
		{
			nNum += 1 ;
		}
		fprintf(m_fpCpp,"	CHAR szTemp[%d] = { 0 };\n" , nNum );
		for ( int n = 0 ; n < nNum ; n ++ )
		{
			fprintf(m_fpCpp,"	bitset<%d> bitT = pstIn->bsIgnored >> %d;\n" ,pstTypeInfo->nIgnored , n * 8  );
			if ( n < nNum - 1 )
			{
				fprintf(m_fpCpp,"	bitT = pstIn->bsIgnored << %d;\n" , (nNum - n - 1) * 8  );
			}
			fprintf(m_fpCpp,"	szTemp[%d] = bitT.to_ulong();\n" , n );
		}
		fprintf(m_fpCpp,"	if(-1 == poNetData->AddString(szTemp, sizeof(szTemp)))\n"  );
		fprintf(m_fpCpp,"		return -1;\n\n"  );			
	}

	fprintf(m_fpCpp,
		"	for(CIndexMapItr itr = mapIndex.begin(); itr != mapIndex.end(); itr++) \n"
		"	{ \n"
		"		SIndex& stIndex = itr->second;\n"
		"		switch(stIndex.wIdx)\n"
		"		{\n"
		"		\n"
		);

	map<UINT8, UINT8> mapConflictIdx;
	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{		
		SItemInfo* pstItem = pstTypeInfo->oVecItemInfo[i];
		UINT8 byKeyIdx = atoi(pstItem->szKeyIdx);
		if(0 == byKeyIdx)
		{
			m_strErrMsg.Format("<type name=%s>\r\n<item name=%s type=%s>\r\nKeyIdx[%s] invalid!", 
				pstTypeInfo->szName, pstItem->szName, pstItem->szType, pstItem->szKeyIdx);
			return false;
		}
		if(mapConflictIdx.find(byKeyIdx) != mapConflictIdx.end())
		{
			m_strErrMsg.Format("<type name=%s>\r\n<item name=%s type=%s>\r\nKeyIdx[%s] Conflict!", 
				pstTypeInfo->szName, pstItem->szName, pstItem->szType, pstItem->szKeyIdx);
			return false;
		}
		mapConflictIdx[byKeyIdx] = byKeyIdx;

		fprintf(m_fpCpp,
			"		case %d:\n"
			"			{\n",
			byKeyIdx);

		if('\0' != pstItem->szCount[0] 
		&& DATA_STRING != pstItem->nDataType
			&& DATA_WSTRING != pstItem->nDataType
			&& DATA_TSTRING != pstItem->nDataType)
		{
			//if(false == bDefI)
			//{
			fprintf(m_fpCpp, "		INT32 i;\n");
			//	bDefI = true;
			//}
		}

		if (pstItem->bIgnored)
		{
			fprintf(m_fpCpp,"		if(pstIn->Get%s())\n",pstItem->szName );
			fprintf(m_fpCpp, "		{\n");
			if(false == _WriteEncodeItemToCpp(pstItem,true))
				return false;
			fprintf(m_fpCpp, "		}\n\n");
		}
		else
		{		
			if(false == _WriteEncodeItemToCpp(pstItem))
				return false;
		}

		fprintf(m_fpCpp,
			"			}\n"
			"			break;\n");
	}

	fprintf(m_fpCpp,
		"		default:\n"
		"			INT32 nTmp = GetEncodePassLen(mapStructIndex, mapUnionIDIndex, mapIndex, stIndex);\n"
		"			if(-1 == nTmp)\n"
		"			{\n"
		"				return nTmp;\n"
		"			}\n"
		"			if(-1 == poNetData->PassLen(nTmp))\n"
		"			{\n"
		"				return -1;\n"
		"			}\n"
		"			break;\n"
		"		}\n"
		"	}\n"
		);

	fprintf(m_fpCpp,
		"	return poNetData->GetDataLen();\n"
		"}\n\n");

	return true;
}

bool CClassMaker::_WriteCliDecodeFuncToCpp(STypeInfo* pstTypeInfo)
{
	fprintf(m_fpCpp,
		"INT32 Decode%s(void *pHost, CNetData* poNetData)\n"
		"{\n"
		"	%s *pstOut = (%s*)pHost;\n"
		"	UINT32 dwStructId = %d; \n"
		"	CStructIndexMap& mapStructIndex = %s::Instance()->GetStructIndexMap();\n"
		"	CUnionIDIndexMap& mapUnionIDIndex = %s::Instance()->GetUnionIDIndexMap();\n"
		"	CIndexMap& mapIndex = GetStructIndex(mapStructIndex, dwStructId);\n"
		"	if(0 == mapIndex.size())\n"
		"	{\n"
		"		return -1;\n"
		"	}\n"
		"	CIndexPosMap mapIndexPos;\n"
		"\n",
		pstTypeInfo->szName,
		pstTypeInfo->szName, pstTypeInfo->szName,
		pstTypeInfo->nStructID,
		m_strClassName,
		m_strClassName);

	if ( pstTypeInfo->nIgnored > 0 )
	{
		int nNum =  pstTypeInfo->nIgnored / 8 ;
		if (pstTypeInfo->nIgnored % 8)
		{
			nNum += 1 ;
		}
		fprintf(m_fpCpp,"	CHAR szTemp[%d] = { 0 };\n" , nNum );
		fprintf(m_fpCpp,"	if(-1 == poNetData->DelString(szTemp, sizeof(szTemp)))\n"  );
		fprintf(m_fpCpp,"		return -1;\n\n"  );		
		fprintf(m_fpCpp,"	for ( INT32 t = 0; t < %d ;  t ++ )\n" ,nNum );		
		fprintf(m_fpCpp,"	{\n"  );	
		for ( int n = 0 ; n < nNum ; n ++ )
		{
			fprintf(m_fpCpp,"		bitset<8> bit((UINT32)szTemp[t]);\n"  );
			fprintf(m_fpCpp,"		for(UINT32 m = 0 ; m < 8; m ++ )\n"  );
			fprintf(m_fpCpp,"			pstOut->bsIgnored.set(t * 8 + m , bit.test(m)) ;\n"  );
		}
		fprintf(m_fpCpp,"	}\n\n"  );		
	}

	fprintf(m_fpCpp,
		"	for(CIndexMapItr itr = mapIndex.begin(); itr != mapIndex.end(); itr++) \n"
		"	{ \n"
		"		SIndex& stIndex = itr->second;\n"
		"		switch(stIndex.wIdx)\n"
		"		{\n"
		"		\n"
		);
	bool bDefI = false;
	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{
		if('\0' != pstTypeInfo->oVecItemInfo[i]->szCount[0] 
		&& DATA_STRING != pstTypeInfo->oVecItemInfo[i]->nDataType
			&& DATA_WSTRING != pstTypeInfo->oVecItemInfo[i]->nDataType
			&& DATA_TSTRING != pstTypeInfo->oVecItemInfo[i]->nDataType)
		{
			if(false == bDefI)
			{
				fprintf(m_fpCpp, "		INT32 i;\n");
				bDefI = true;
			}
		}

		fprintf(m_fpCpp,
			"		case %s:\n"
			"			{\n",
			pstTypeInfo->oVecItemInfo[i]->szKeyIdx );

		if (pstTypeInfo->oVecItemInfo[i]->bIgnored)
		{
			fprintf(m_fpCpp,"			if(pstOut->Get%s())\n",pstTypeInfo->oVecItemInfo[i]->szName );
			fprintf(m_fpCpp, "			{\n");
			if(false == _WriteDecodeItemToCpp(pstTypeInfo->oVecItemInfo[i],true))
				return false;
			fprintf(m_fpCpp, "			}\n\n");
		}
		else
		{
			if(false == _WriteDecodeItemToCpp(pstTypeInfo->oVecItemInfo[i]))
				return false;
		}
		fprintf(m_fpCpp,
			"			}\n"
			"			break;\n");

	}

	fprintf(m_fpCpp,
		"		default:\n"
		"			INT32 nTmp = GetDecodePassLen(mapStructIndex, mapUnionIDIndex, dwStructId, mapIndex, stIndex, poNetData->GetData() + poNetData->GetDataLen(), mapIndexPos);\n"
		"			if(-1 == nTmp)\n"
		"			{\n"
		"				return nTmp;\n"
		"			}\n"
		"			if(-1 == poNetData->PassLen(nTmp))\n"
		"			{\n"
		"				return -1;\n"
		"			}\n"
		"			break;\n"
		"		}\n"
		"		mapIndexPos[GetKey(dwStructId, stIndex.wIdx)] = poNetData->GetData() + poNetData->GetDataLen() - stIndex.dwContextLen;\n"
		"	}\n"
		);

	fprintf(m_fpCpp,
		"	return sizeof(*pstOut);\n"
		"}\n\n");

	//fprintf(m_fpCpp,
	//	"	return poNetData->GetDataLen();\n"
	//	"}\n\n");

	return true;
}



bool CClassMaker::_WriteEqualFuncToCpp(STypeInfo* pstTypeInfo)
{
	fprintf(m_fpCppHandle,
		"void %s::Equal%s(void *pOut, void *pIn)\n"
		"{\n",
		m_strHandleNameSpace,
		pstTypeInfo->szName
		);

	fprintf(m_fpCppHandle,
		"\t%s::%s *pstIn = (%s::%s*)pIn;\n",
		m_strOldNameSpace,
		pstTypeInfo->szName,
		m_strOldNameSpace,
		pstTypeInfo->szName
		);

	fprintf(m_fpCppHandle,
		"\t%s::%s *pstOut = (%s::%s*)pOut;\n",
		m_strNewNameSpace,
		pstTypeInfo->szName,
		m_strNewNameSpace,
		pstTypeInfo->szName
		);

	fprintf(m_fpCppHandle, "\n");

	if (0 != strcmp("DT_PLAYER_DATA", pstTypeInfo->szName))
	{
		fprintf(m_fpCppHandle, 
			"\tmemset(pstOut, 0x00, sizeof(%s::%s));\n", 
			m_strNewNameSpace, 
			pstTypeInfo->szName);
	}

	fprintf(m_fpCppHandle, "\n");


	bool bDefI = false;
	if ( pstTypeInfo->nIgnored > 0 )
	{
		int nNum =  pstTypeInfo->nIgnored / 8 ;
		if (pstTypeInfo->nIgnored % 8)
		{
			nNum += 1 ;
		}
		fprintf(m_fpCppHandle, "	CHAR szTemp[%d] = { 0 };\n" , nNum );
		for ( int n = 0 ; n < nNum ; n ++ )
		{
			fprintf(m_fpCppHandle, "	bitset<%d> bitT = pstOut->bsIgnored >> %d;\n" , pstTypeInfo->nIgnored , n * 8  );
			if ( n < nNum - 1 )
			{
				fprintf(m_fpCppHandle, "	bitT = pstOut->bsIgnored << %d;\n" , (nNum - n - 1) * 8  );
			}
			fprintf(m_fpCppHandle, "	szTemp[%d] = bitT.to_ulong();\n" , n );
		}
		fprintf(m_fpCppHandle, "	if(-1 == poNetData->AddString(szTemp, sizeof(szTemp)))\n"  );
		fprintf(m_fpCppHandle, "		return -1;\n\n"  );
	}

	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{
		if('\0' != pstTypeInfo->oVecItemInfo[i]->szCount[0]
		&& DATA_STRING != pstTypeInfo->oVecItemInfo[i]->nDataType
			&& DATA_WSTRING != pstTypeInfo->oVecItemInfo[i]->nDataType
			&& DATA_TSTRING != pstTypeInfo->oVecItemInfo[i]->nDataType)
		{
			if(false == bDefI)
			{
				fprintf(m_fpCppHandle, "	INT32 i = 0;\n");
				fprintf(m_fpCppHandle, "	INT32 nNum = 0;\n");
				bDefI = true;
			}
		}
		if (pstTypeInfo->oVecItemInfo[i]->bIgnored)
		{
			fprintf(m_fpCppHandle, "	if(pstIn->Get%s())\n", pstTypeInfo->oVecItemInfo[i]->szName );
			fprintf(m_fpCppHandle, "	{\n");
			if(false == _WriteEqualItemToCpp(pstTypeInfo->oVecItemInfo[i]))
				return false;
			fprintf(m_fpCppHandle, "	}\n\n");
		}
		else
		{
			if(false == _WriteEqualItemToCpp(pstTypeInfo->oVecItemInfo[i]))
				return false;
		}
	}
	fprintf(m_fpCppHandle, "}\n\n");
	return true;
}


bool CClassMaker::_WriteEqualUnionToCpp(STypeInfo* pstTypeInfo)
{
	fprintf(m_fpCppHandle,
		"void %s::EqualUn%s(void *pOut, INT32 iSelect, void *pIn)\n"
		"{\n",
		m_strHandleNameSpace,
		pstTypeInfo->szName);

	fprintf(m_fpCppHandle,
		"\t%s::%s *pstIn = (%s::%s*)pIn;\n",
		m_strOldNameSpace,
		pstTypeInfo->szName,
		m_strOldNameSpace,
		pstTypeInfo->szName
		);

	fprintf(m_fpCppHandle,
		"\t%s::%s *pstOut = (%s::%s*)pOut;\n",
		m_strNewNameSpace,
		pstTypeInfo->szName,
		m_strNewNameSpace,
		pstTypeInfo->szName
		);
	fprintf(m_fpCppHandle, "\n");
	fprintf(m_fpCppHandle, "\tswitch(iSelect){\n");


	//// 2009-03-11 cwy add
	// for union contains single type
	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{
		//fprintf(m_fpCpp, "	case %s: return Equal%s(pstOut, poNetData);\n", pstTypeInfo->oVecItemInfo[i].szID, pstTypeInfo->oVecItemInfo[i].szType);
		fprintf(m_fpCppHandle,
			"\tcase %s::%s:\n", m_strNewNameSpace, pstTypeInfo->oVecItemInfo[i]->szID);
		if (DATA_STRUCT == pstTypeInfo->oVecItemInfo[i]->nDataType)
		{
			SItemInfo * pItemInfo = pstTypeInfo->oVecItemInfo[i];

			fprintf(m_fpCppHandle, "\t\t%s::Equal%s(&pstOut->%s, &pstIn->%s);\n", m_strHandleNameSpace, pstTypeInfo->oVecItemInfo[i]->szType, pstTypeInfo->oVecItemInfo[i]->szMemberName, pstTypeInfo->oVecItemInfo[i]->szMemberName);
			fprintf(m_fpCppHandle, "\t\tbreak;\n");
			continue;
		}
		else if (false == _WriteEqualItemToUnion(pstTypeInfo->oVecItemInfo[i]))
		{

			return false;
		}
	}
	//// 2009-03-11 cwy end

	fprintf(m_fpCppHandle,
		"	default: \n"
		"		break;\n"
		"	}\n"
		"}\n\n");

	return true;
}


bool CClassMaker::_WriteEqualItemToCpp(SItemInfo* pstInfo)
{
	switch(pstInfo->nDataType)
	{
	case DATA_CHAR:
		break;
	case DATA_BYTE:
		break;
	case DATA_SHORT:
		break;
	case DATA_WORD:
		break;
	case DATA_INT:
		break;
	case DATA_DWORD:
		break;
	case DATA_INT64:
		break;
	case DATA_UINT64:
		break;
	case DATA_FLOAT:
		break;
	case DATA_STRING:
		{
			fprintf(m_fpCppHandle, "	memcpy(pstOut->%s, pstIn->%s, sizeof(pstIn->%s));\n", pstInfo->szMemberName, pstInfo->szMemberName, pstInfo->szMemberName);
		}
		return true;
	case DATA_WSTRING:
		{
			fprintf(m_fpCppHandle, "	memcpy(pstOut->%s, pstIn->%s, sizeof(pstIn->%s));\n", pstInfo->szMemberName, pstInfo->szMemberName, pstInfo->szMemberName);
		}
		return true;
		break;
	case DATA_TSTRING:
		{
			fprintf(m_fpCppHandle, "	memcpy(pstOut->%s, pstIn->%s, sizeof(pstIn->%s));\n", pstInfo->szMemberName, pstInfo->szMemberName, pstInfo->szMemberName);
		}
		return true;
		break;
	case DATA_STRUCT:
		{
			if(pstInfo->szCount[0] != '\0')
			{
				if(IsNumber(pstInfo->szCount))
				{
					fprintf(m_fpCppHandle, "	nNum = %s;\n", pstInfo->szCount);
				}
				else
				{
					fprintf(m_fpCppHandle, "	nNum = %s::%s > %s::%s ? %s::%s : %s::%s;\n", 
						m_strNewNameSpace, pstInfo->szCount,
						m_strOldNameSpace, pstInfo->szCount,
						m_strOldNameSpace, pstInfo->szCount,
						m_strNewNameSpace, pstInfo->szCount
						);
				}

				if('\0' == pstInfo->szReferMemberName[0])
				{
					fprintf(m_fpCppHandle,
						"	for(i = 0; i < nNum; i++)\n"
						"	{\n"
						"		%s::Equal%s(&pstOut->%s[i], &pstIn->%s[i]);\n"
						"	}\n\n",
						m_strHandleNameSpace,
						pstInfo->szType,
						pstInfo->szMemberName,
						pstInfo->szMemberName);
				}
				else
				{
					fprintf(m_fpCppHandle,
						"	for(i = 0; i < nNum; i++)\n"
						"	{\n"
						"		if(i >= (INT32)pstOut->%s)\n"
						"			break;\n"
						"		%s::Equal%s(&pstOut->%s[i], &pstIn->%s[i]);\n"
						"	}\n\n",
						pstInfo->szReferMemberName,
						m_strHandleNameSpace, 
						pstInfo->szType,
						pstInfo->szMemberName,
						pstInfo->szMemberName);
				}
			}
			else
			{
				fprintf(m_fpCppHandle,
					"	%s::Equal%s(&pstOut->%s, &pstIn->%s);\n",
					m_strHandleNameSpace,
					pstInfo->szType,
					pstInfo->szMemberName,
					pstInfo->szMemberName);

			}
		}
		return true;
	case DATA_UNION:
		{
			fprintf(m_fpCppHandle,
				"	%s::EqualUn%s(&pstOut->%s, pstOut->%s, &pstIn->%s);\n",
				m_strHandleNameSpace,
				pstInfo->szType, 
				pstInfo->szMemberName, 
				pstInfo->szSelect, 
				pstInfo->szMemberName);
		}
		return true;
	default:
		return false;
	}

	if(pstInfo->szCount[0] != '\0')
	{
		if(IsNumber(pstInfo->szCount))
		{
			fprintf(m_fpCppHandle, "	nNum = %s;\n", pstInfo->szCount);
		}
		else
		{
			fprintf(m_fpCppHandle, "	nNum = %s::%s > %s::%s ? %s::%s : %s::%s;\n", 
				m_strNewNameSpace, pstInfo->szCount,
				m_strOldNameSpace, pstInfo->szCount,
				m_strOldNameSpace, pstInfo->szCount,
				m_strNewNameSpace, pstInfo->szCount
				);
		}

		if('\0' == pstInfo->szReferMemberName[0])
		{
			fprintf(m_fpCppHandle,
				"	for(i = 0; i < nNum; i++)\n"
				"	{\n"
				"		pstOut->%s[i] = pstIn->%s[i];\n"
				"	}\n\n",
				pstInfo->szMemberName,
				pstInfo->szMemberName);

		}
		else
		{
			fprintf(m_fpCppHandle,
				"	for(i = 0; i < nNum; i++)\n"
				"	{\n"
				"		if(i >= (INT32)pstOut->%s)\n"
				"			break;\n"
				"		pstOut->%s[i] = pstIn->%s[i];\n"
				"	}\n\n",
				pstInfo->szReferMemberName,
				pstInfo->szMemberName,
				pstInfo->szMemberName);
		}
	}
	else
	{
		fprintf(m_fpCppHandle,
			"	pstOut->%s = pstIn->%s;\n",
			pstInfo->szMemberName,
			pstInfo->szMemberName);
	}

	return true;
}



bool CClassMaker::_GetDef(SItemInfo* pstItem, CString &strDef)
{
	if(DATA_CHAR == pstItem->nDataType)
	{
		strDef =  DEF_CHAR;
	}
	else if(DATA_BYTE == pstItem->nDataType)
	{
		strDef = DEF_BYTE;
	}
	else if(DATA_SHORT == pstItem->nDataType)
	{
		strDef = DEF_SHORT;
	}
	else if(DATA_WORD == pstItem->nDataType)
	{
		strDef = DEF_WORD;
	}
	else if(DATA_INT == pstItem->nDataType)
	{
		strDef = DEF_INT;
	}
	else if(DATA_DWORD == pstItem->nDataType)
	{
		strDef = DEF_DWORD;
	}
	else if(DATA_INT64 == pstItem->nDataType)
	{
		strDef = DEF_INT64;
	}
	else if(DATA_UINT64 == pstItem->nDataType)
	{
		strDef = DEF_UINT64;
	}
	else if(DATA_FLOAT == pstItem->nDataType)
	{
		strDef = DEF_FLOAT;
	}
	else if(DATA_STRING == pstItem->nDataType)
	{
		strDef = DEF_STRING;
	}
	else if(DATA_WSTRING == pstItem->nDataType)
	{
		strDef = DEF_WSTRING;
	}
	else if(DATA_TSTRING == pstItem->nDataType)
	{
		strDef = DEF_TSTRING;
	} 
	else
	{
		if(pstItem->nDataType != DATA_STRUCT && pstItem->nDataType != DATA_UNION)
		{
			ASSERT(false);
			return false;
		}
		strDef = pstItem->szType;
	}

	return true;
}


CString CClassMaker::_GetSimpleDef(int nDataType)
{
	if(DATA_CHAR == nDataType)
	{
		return  DEF_CHAR;
	}
	else if(DATA_BYTE == nDataType)
	{
		return DEF_BYTE;
	}
	else if(DATA_SHORT == nDataType)
	{
		return DEF_SHORT;
	}
	else if(DATA_WORD == nDataType)
	{
		return DEF_WORD;
	}
	else if(DATA_INT == nDataType)
	{
		return DEF_INT;
	}
	else if(DATA_DWORD == nDataType)
	{
		return DEF_DWORD;
	}
	else if(DATA_INT64 == nDataType)
	{
		return DEF_INT64;
	}
	else if(DATA_UINT64 == nDataType)
	{
		return DEF_UINT64;
	}
	else if(DATA_FLOAT == nDataType)
	{
		return DEF_FLOAT;
	}
	else if(DATA_STRING == nDataType)
	{
		return DEF_STRING;
	}
	else if(DATA_WSTRING == nDataType)
	{
		return DEF_WSTRING;
	}
	else if(DATA_TSTRING == nDataType)
	{
		return DEF_TSTRING;
	} 
	else
	{
		return "";
	}

	return "";
}

//// 2009-3-11 cwy add 
// for union contains single type member
bool CClassMaker::_WriteEncodeItemToUnion( SItemInfo* pstInfo )
{
	CString strAddFunc;
	CString strType;
	switch(pstInfo->nDataType){
	case DATA_CHAR:
		strAddFunc = "AddChar";
		break;
	case DATA_BYTE:
		strAddFunc = "AddByte";
		break;
	case DATA_SHORT:
		strAddFunc = "AddShort";
		break;
	case DATA_WORD:
		strAddFunc = "AddWord";
		break;
	case DATA_INT:
		strAddFunc = "AddInt";
		break;
	case DATA_DWORD:
		strAddFunc = "AddDword";
		break;
	case DATA_INT64:
		strAddFunc = "AddInt64";
		break;
	case DATA_UINT64:
		strAddFunc = "AddUint64";
		break;
	case DATA_FLOAT:
		strAddFunc = "AddFloat";
		break;
	default:
		return false;
	}

	_GetDef(pstInfo,strType);

	fprintf(m_fpCpp,
		"\t\treturn poNetData->%s(*(%s*)pHost);\n",
		strAddFunc, strType);	
	return true;
}
//// 2009-3-11 cwy add 
// for union contains single type member
bool CClassMaker::_WriteDecodeItemToUnion( SItemInfo* pstInfo )
{
	CString strDelFunc;
	CString strType;

	switch(pstInfo->nDataType){
	case DATA_CHAR:
		strDelFunc = "DelChar";
		break;
	case DATA_BYTE:
		strDelFunc = "DelByte";
		break;
	case DATA_SHORT:
		strDelFunc = "DelShort";
		break;
	case DATA_WORD:
		strDelFunc = "DelWord";
		break;
	case DATA_INT:
		strDelFunc = "DelInt";
		break;
	case DATA_DWORD:
		strDelFunc = "DelDword";
		break;
	case DATA_INT64:
		strDelFunc = "DelInt64";
		break;
	case DATA_UINT64:
		strDelFunc = "DelUint64";
		break;
	case DATA_FLOAT:
		strDelFunc = "DelFloat";
		break;
	default:
		return false;
	}
	_GetDef(pstInfo, strType);
	fprintf(m_fpCpp,
		"\t\treturn poNetData->%s(*(%s*)pHost);\n",
		strDelFunc, strType);	
	return true;
}

//// 2009-3-11 cwy add
// for union contains single type member
bool CClassMaker::_WriteEqualItemToUnion( SItemInfo* pstInfo )
{
	CString strAddFunc;
	CString strType;
	switch(pstInfo->nDataType)
	{
	case DATA_CHAR:
		strAddFunc = "AddChar";
		break;
	case DATA_BYTE:
		strAddFunc = "AddByte";
		break;
	case DATA_SHORT:
		strAddFunc = "AddShort";
		break;
	case DATA_WORD:
		strAddFunc = "AddWord";
		break;
	case DATA_INT:
		strAddFunc = "AddInt";
		break;
	case DATA_DWORD:
		strAddFunc = "AddDword";
		break;
	case DATA_INT64:
		strAddFunc = "AddInt64";
		break;
	case DATA_UINT64:
		strAddFunc = "AddUint64";
		break;
	case DATA_FLOAT:
		strAddFunc = "AddFloat";
		break;
	default:
		return false;
	}

	_GetDef(pstInfo, strType);

	fprintf(m_fpCppHandle,
		"\t\treturn poNetData->%s(*(%s*)pstOut);\n",
		strAddFunc, strType);
	return true;
}

bool CClassMaker::_MakeName()
{
	if (NULL == m_poMainParser)
	{
		return false;
	}
	m_strUpperName = m_poMainParser->GetName();
	m_strUpperName.MakeUpper();
	m_strLowerName = m_poMainParser->GetName();
	m_strLowerName.MakeLower();
	return true;
}
