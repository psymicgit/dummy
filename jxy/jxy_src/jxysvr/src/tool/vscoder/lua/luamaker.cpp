#include "stdafx.h"

#include "VSCoder.h"
#include "luamaker.h"
#include "Parser.h"

static LPCTSTR DEF_CHAR			= "CHAR";
static LPCTSTR DEF_BYTE			= "UINT8";
static LPCTSTR DEF_SHORT		= "INT16";
static LPCTSTR DEF_WORD			= "UINT16";
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

CLuaMaker::CLuaMaker()
{
	m_poMainParser		= NULL;
	m_poVecChildParser	= NULL;
	m_strErrMsg = "";
	m_fpLua		= NULL;
}

CLuaMaker::~CLuaMaker()
{
	if(m_fpLua != NULL)
		fclose(m_fpLua);
}

void CLuaMaker::SetInfo(INT32 nType, INT32 nCharsetNet, INT32 nCharsetHost)
{
	m_nType        = nType;
	m_nCharsetNet  = nCharsetNet;
	m_nCharsetHost = nCharsetHost;
}

bool CLuaMaker::Make(CParser* poMainParser, CParserVec* poChildParser, CString strFilepath)
{
	m_strErrMsg.Format("lua file failed!");

	m_poMainParser		= poMainParser;
	m_poVecChildParser	= poChildParser;

	m_strClassName.Format("C%s", poMainParser->GetName());

	//// 2009-03-27 cwy add
	if (false == _MakeName())
	{
		return false;
	}

	if(false == _OpenFile(strFilepath))
		return false;

	if(false == _WriteHeaderToLua(poMainParser->GetName(), poMainParser->GetVersion()))
		return false;

	if(false == _WriteIncludeToLua())
		return false;

	if(false == _WriteDefinesToLua())
		return false;

	if(false == _WriteTypesToLua())
		return false;

	if(false == _WriteMessageIDToLua())
		return false;

	if(false == _WriteAllCodeFuncToLua())
		return false;

	if(false == _WriteCodeMapFuncToLua())
		return false;

	return true;
}

bool CLuaMaker::_OpenFile( CString strFilepath)
{
	strFilepath.AppendFormat("%s.lua", m_strLowerName);
	m_fpLua = fopen(strFilepath, "w");
	if(NULL == m_fpLua)
	{
		m_strErrMsg.Format("open %s for write failed", strFilepath);
		return false;
	}

	return true;
}

bool CLuaMaker::_WriteHeaderToLua(LPCTSTR pszName, int nVersion)
{
	//fprintf(m_fpLua, "module (..., package.seeall)\n\n");

	return true;
}

bool CLuaMaker::_WriteIncludeToLua()
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

		//fprintf(m_fpLua, "require \"network/protocol/lua/%s\"\n", szName);
		fprintf(m_fpLua, "req(\"network/protocol/lua/%s\")\n", szName);
	}
	fprintf(m_fpLua, "\n\n");

	return true;
}

bool CLuaMaker::_WriteDefinesToLua()
{
	fprintf(m_fpLua, "-----------------\n"\
		"-- MAC begin --\n"\
		"-----------------\n\n");

	for(int i = 0; i < m_poMainParser->GetDefineInfoCount(); i++)
	{
		SDefineInfo* pstDefineInfo = m_poMainParser->GetDefineInfoByPos(i);
		fprintf(m_fpLua, "%-24s =%s\n", pstDefineInfo->szName, pstDefineInfo->szValue);
	}

	fprintf(m_fpLua, "-----------------\n"\
		"-- MAC end --\n"\
		"-----------------\n\n");

	return true;
}

bool CLuaMaker::_WriteTypesToLua()
{
	fprintf(m_fpLua, "-----------------\n"\
		"-- struct begin --\n"\
		"-----------------\n\n  --[[ ");

	for(int i = 0; i < m_poMainParser->GetTypeInfoCount(); i++)
	{
		STypeInfo* pstTypeInfo = m_poMainParser->GetTypeInfoByPos(i);

		if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
		{
			fprintf(m_fpLua, "\ntag%s = { --union\n", pstTypeInfo->szName);
		}
		else if(CLASS_TYPE_STRUCT == pstTypeInfo->nClassType)
		{
			fprintf(m_fpLua, "\ntag%s = {\n", pstTypeInfo->szName);
		}
		else
		{
			ASSERT(false);
			return false;
		}

		for(size_t nItem = 0; nItem < pstTypeInfo->oVecItemInfo.size(); nItem++)
		{
			SItemInfo *pstItemInfo = pstTypeInfo->oVecItemInfo[nItem];

			if(DATA_STRUCT == pstItemInfo->nDataType || DATA_UNION == pstItemInfo->nDataType)
			{
				if(pstItemInfo->szCount[0] != '\0')
				{
					if('\0' == pstItemInfo->szRefer[0])
					{
						fprintf(m_fpLua, "\t%-16s ={}, -- type[%s], array max count[%s], Comment[%s]\n", 
							pstItemInfo->szName, 
							pstItemInfo->szType,
							pstItemInfo->szCount, 
							pstItemInfo->szComment);
					}
					else
					{
						fprintf(m_fpLua, "\t%-16s ={}, -- type[%s], array max count[%s], array refer count[%s], Comment[%s]\n", 
							pstItemInfo->szName, 
							pstItemInfo->szType,
							pstItemInfo->szCount, 
							pstItemInfo->szRefer,
							pstItemInfo->szComment);
					}
				}
				else
				{
					if('\0' == pstItemInfo->szRefer[0])
					{
						fprintf(m_fpLua, "\t%-16s ={}, -- type[%s], Comment[%s]\n", 
							pstItemInfo->szName, 
							pstItemInfo->szType,
							pstItemInfo->szComment);
					}
					else
					{
						fprintf(m_fpLua, "\t%-16s ={}, -- type[%s], Comment[%s]\n", 
							pstItemInfo->szName, 
							pstItemInfo->szType,
							pstItemInfo->szComment);
					}
				}
			}
			else
			{
				if(pstItemInfo->szCount[0] != '\0')
				{
					if('\0' == pstItemInfo->szRefer[0])
					{
						if((DATA_STRING == pstItemInfo->nDataType) || (DATA_TSTRING == pstItemInfo->nDataType))
						{
							fprintf(m_fpLua, "\t%-16s =\"\", -- type[%s], array max count[%s], Comment[%s]\n", 
								pstItemInfo->szName, 
								pstItemInfo->szType,
								pstItemInfo->szCount, 
								pstItemInfo->szComment);
						}
						else
						{
							fprintf(m_fpLua, "\t%-16s ={}, -- type[%s], array max count[%s], Comment[%s]\n", 
								pstItemInfo->szName, 
								pstItemInfo->szType,
								pstItemInfo->szCount, 
								pstItemInfo->szComment);
						}
					}
					else
					{
						fprintf(m_fpLua, "\t%-16s ={}, -- type[%s], array max count[%s], array refer count[%s], Comment[%s]\n", 
							pstItemInfo->szName, 
							pstItemInfo->szType,
							pstItemInfo->szCount, 
							pstItemInfo->szRefer,
							pstItemInfo->szComment);
					}
				}
				else
				{
					if(DATA_STRING == pstItemInfo->nDataType || DATA_WSTRING == pstItemInfo->nDataType || DATA_TSTRING == pstItemInfo->nDataType)
					{
						fprintf(m_fpLua, "\t%-16s ="", -- type[%s], Comment[%s]\n", 
							pstItemInfo->szName, 
							pstItemInfo->szType,
							pstItemInfo->szComment);
					}
					else
					{
						fprintf(m_fpLua, "\t%-16s =0, -- type[%s], Comment[%s]\n", 
							pstItemInfo->szName, 
							pstItemInfo->szType,
							pstItemInfo->szComment);
					}
				}
			}
		}

		fprintf(m_fpLua, "}\n", pstTypeInfo->szName);
	}

	fprintf(m_fpLua, "--]]\n-----------------\n"\
		"-- struct end --\n"\
		"-----------------\n\n");
	return true;
}

bool CLuaMaker::_WriteMessageIDToLua()
{
	fprintf(m_fpLua, "-----------------\n"\
		"-- MsgID begin --\n"\
		"-----------------\n\n");

	for(int i = 0; i < m_poMainParser->GetMessageInfoCount(); i++)
	{
		SMessageInfo* pstMessageInfo = m_poMainParser->GetMessageInfoByPos(i);

		if (pstMessageInfo->bHasID)
		{
			fprintf(m_fpLua, "\t%-30s  = %-10d  \n", pstMessageInfo->szName, pstMessageInfo->nID);
		}
	}

	fprintf(m_fpLua, "-----------------\n"\
		"-- MsgID end --\n"\
		"-----------------\n\n");

	return true;
}


bool CLuaMaker::_WriteAllCodeFuncToLua()
{
	fprintf(m_fpLua, "-----------------\n"\
		"-- func begin --\n"\
		"-----------------\n\n");

	for(int i = 0; i < m_poMainParser->GetTypeInfoCount(); i++)
	{
		STypeInfo* pstTypeInfo = m_poMainParser->GetTypeInfoByPos(i);
		if(CLASS_TYPE_STRUCT == pstTypeInfo->nClassType)
		{
			if(false == _WriteEncodeFuncToLua(pstTypeInfo))
				return false;
			if(false == _WriteDecodeFuncToLua(pstTypeInfo))
				return false;
		}
		else if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
		{
			if(false == _WriteEncodeUnionToLua(pstTypeInfo))
				return false;
			if(false == _WriteDecodeUnionToLua(pstTypeInfo))
				return false;
		}
		else
		{
			ASSERT(false);
			return false;
		}
	}

	fprintf(m_fpLua, "-----------------\n"\
		"-- func end --\n"\
		"-----------------\n\n");

	return true;
}

bool CLuaMaker::_WriteEncodeFuncToLua(STypeInfo* pstTypeInfo)
{
	fprintf(m_fpLua,
		"function Encode_%s(NetMsg, %s)\n",
		pstTypeInfo->szName,  pstTypeInfo->szName);

	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{
		if(false == _WriteEncodeItemToLua(pstTypeInfo->oVecItemInfo[i], pstTypeInfo->szName))
			return false;
	}

	fprintf(m_fpLua,"	return NetMsg;\n"\
		"end\n");

	return true;
}

bool CLuaMaker::_WriteDecodeFuncToLua(STypeInfo* pstTypeInfo)
{
	fprintf(m_fpLua,
		"function Decode_%s(NetMsg, %s)\n",
		pstTypeInfo->szName,  pstTypeInfo->szName);

	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{		
		if(false == _WriteDecodeItemToLua(pstTypeInfo->oVecItemInfo[i], pstTypeInfo->szName))
			return false;
	}

	fprintf(m_fpLua,"	return %s;\n"\
		"end\n", pstTypeInfo->szName);

	return true;
}

bool CLuaMaker::_WriteEncodeUnionToLua(STypeInfo* pstTypeInfo)
{
	fprintf(m_fpLua,
		"function EncodeUn_%s(NetMsg, %s, Select)\n",
		pstTypeInfo->szName,  pstTypeInfo->szName);

	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{	
		fprintf(m_fpLua, 
			"	if Select == %s then\n", pstTypeInfo->oVecItemInfo[i]->szID);
		if (DATA_STRUCT == pstTypeInfo->oVecItemInfo[i]->nDataType)
		{
			fprintf(m_fpLua, 
				"		Encode_%s(NetMsg, %s.%s);\n"\
				"	end\n",
				pstTypeInfo->oVecItemInfo[i]->szType, pstTypeInfo->szName, pstTypeInfo->oVecItemInfo[i]->szName);
		}
		else if (false == _WriteEncodeItemToUnion(pstTypeInfo->oVecItemInfo[i], pstTypeInfo->szName))
		{
			return false;
		}		
	}

	fprintf(m_fpLua,"	return NetMsg;\n"\
		"end\n");

	return true;
}

bool CLuaMaker::_WriteDecodeUnionToLua(STypeInfo* pstTypeInfo)
{
	fprintf(m_fpLua,
		"function DecodeUn_%s(NetMsg, %s, Select)\n",
		pstTypeInfo->szName, pstTypeInfo->szName);

	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{	
		fprintf(m_fpLua, 
			"	if Select == %s then\n", pstTypeInfo->oVecItemInfo[i]->szID);
		if (DATA_STRUCT == pstTypeInfo->oVecItemInfo[i]->nDataType)
		{
			fprintf(m_fpLua, 
				"		%s.%s = Decode_%s(NetMsg, %s.%s);\n"\
				"	end\n",
				pstTypeInfo->szName, pstTypeInfo->oVecItemInfo[i]->szName, pstTypeInfo->oVecItemInfo[i]->szType, pstTypeInfo->szName, pstTypeInfo->oVecItemInfo[i]->szName);
		}
		else if (false == _WriteDecodeItemToUnion(pstTypeInfo->oVecItemInfo[i], pstTypeInfo->szName))
		{
			return false;
		}	
	}

	fprintf(m_fpLua,"	return %s;\n"\
		"end\n", pstTypeInfo->szName);

	return true;
}

bool CLuaMaker::_WriteEncodeItemToLua(SItemInfo* pstInfo, char* pszStructName)
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
	case DATA_TSTRING:
		{
			fprintf(m_fpLua,
				"	AddString(NetMsg, %s.%s);\n",
				pszStructName, pstInfo->szName);
		}
		return true;

	case DATA_WSTRING:
		return false;
		break;
	case DATA_STRUCT:
		{	
			if(pstInfo->szCount[0] != '\0')
			{
				if('\0' == pstInfo->szRefer[0])
				{
					fprintf(m_fpLua,
						"	for i=1, %s do\n"\
						"		Encode_%s(NetMsg, %s.%s[i]);\n"\
						"	end\n",
						pstInfo->szCount,
						pstInfo->szType, pszStructName, pstInfo->szName);
				}
				else
				{
					fprintf(m_fpLua,
						"	for i=1, %s.%s do\n"\
						"		Encode_%s(NetMsg, %s.%s[i]);\n"\
						"	end\n",
						pszStructName, pstInfo->szRefer,
						pstInfo->szType, pszStructName, pstInfo->szName);
				}
			}
			else
			{
				fprintf(m_fpLua,
					"	Encode_%s(NetMsg, %s.%s);\n",
					pstInfo->szType, pszStructName, pstInfo->szName);
			}
		}
		return true;
	case DATA_UNION:
		{
			fprintf(m_fpLua,
				"	EncodeUn_%s(NetMsg, %s.%s, %s);\n",
				pstInfo->szType, pszStructName, pstInfo->szName, pstInfo->szSelect);
		}
		return true;
	default:
		return false;
	}

	if(pstInfo->szCount[0] != '\0')
	{
		if('\0' == pstInfo->szRefer[0])
		{
			fprintf(m_fpLua,
				"	for i=1, %s do\n"\
				"		%s(NetMsg, %s.%s[i]);\n"\
				"	end\n",
				pstInfo->szCount,
				strAddFunc, pszStructName, pstInfo->szName);
		}
		else
		{
			fprintf(m_fpLua,
				"	for i=1, %s.%s do\n"\
				"		%s(NetMsg, %s.%s[i]);\n"\
				"	end\n",
				pszStructName, pstInfo->szRefer,
				strAddFunc, pszStructName, pstInfo->szName);
		}
	}
	else
	{	
		fprintf(m_fpLua,
			"	%s(NetMsg, %s.%s);\n",
			strAddFunc, pszStructName, pstInfo->szName);
	}

	return true;
}

bool CLuaMaker::_WriteDecodeItemToLua(SItemInfo* pstInfo, char* pszStructName)
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
	case DATA_TSTRING:
		{
			fprintf(m_fpLua,
				"	%s.%s = DelString(NetMsg);\n"\
				"	if -1 == CkDecode(NetMsg) then\n"\
				"		return nil;\n"\
				"	end\n",
				pszStructName, pstInfo->szName);
		}
		return true;

	case DATA_WSTRING:
		return false;
		break;
	case DATA_STRUCT:
		{	
			if(pstInfo->szCount[0] != '\0')
			{
				if('\0' == pstInfo->szRefer[0])
				{
					fprintf(m_fpLua,
						"	%s.%s = {};\n"\
						"	for i=1, %s do\n"\
						"		%s.%s[i] = {};\n"
						"		%s.%s[i] = Decode_%s(NetMsg, %s.%s[i]);\n"\
						"		if -1 == CkDecode(NetMsg) then\n"\
						"			return nil;\n"\
						"		end\n"\
						"	end\n",
						pszStructName, pstInfo->szName,
						pstInfo->szCount,
						pszStructName, pstInfo->szName,
						pszStructName, pstInfo->szName, pstInfo->szType, pszStructName, pstInfo->szName);
				}
				else
				{
					fprintf(m_fpLua,
						"	%s.%s = {};\n"\
						"	for i=1, %s.%s do\n"\
						"		%s.%s[i] = {};\n"
						"		%s.%s[i] = Decode_%s(NetMsg, %s.%s[i]);\n"\
						"		if -1 == CkDecode(NetMsg) then\n"\
						"			return nil;\n"\
						"		end\n"\
						"	end\n",
						pszStructName, pstInfo->szName,
						pszStructName, pstInfo->szRefer,
						pszStructName, pstInfo->szName,
						pszStructName, pstInfo->szName, pstInfo->szType, pszStructName, pstInfo->szName);
				}
			}
			else
			{
				fprintf(m_fpLua,
					"	%s.%s = Decode_%s(NetMsg, %s.%s);\n",
					pszStructName, pstInfo->szName, pstInfo->szType, pszStructName, pstInfo->szName);
			}
		}
		return true;
	case DATA_UNION:
		{
			fprintf(m_fpLua,
				"	%s.%s = DecodeUn_%s(NetMsg, %s.%s, %s);\n",
				pszStructName, pstInfo->szName, pstInfo->szType, pstInfo->szSelect, pszStructName, pstInfo->szName);
		}
		return true;
	default:
		return false;
	}

	if(pstInfo->szCount[0] != '\0')
	{
		if('\0' == pstInfo->szRefer[0])
		{
			fprintf(m_fpLua,
				"	for i=1, %s do\n"\
				"		%s.%s[i] = %s(NetMsg);\n"\
				"		if -1 == CkDecode(NetMsg) then\n"\
				"			return nil;\n"\
				"		end\n"\
				"	end\n",
				pstInfo->szCount,
				pszStructName, pstInfo->szName, strDelFunc);
		}
		else
		{
			fprintf(m_fpLua,
				"	for i=1, %s.%s do\n"\
				"		%s.%s[i] = %s(NetMsg);\n"\
				"		if -1 == CkDecode(NetMsg) then\n"\
				"			return nil;\n"\
				"		end\n"\
				"	end\n",
				pszStructName, pstInfo->szRefer,
				pszStructName, pstInfo->szName, strDelFunc);
		}
	}
	else
	{
		fprintf(m_fpLua,
			"	%s.%s = %s(NetMsg);\n"\
			"	if -1 == CkDecode(NetMsg) then\n"\
			"		return nil;\n"\
			"	end\n",
			pszStructName, pstInfo->szName, strDelFunc);
	}

	return true;
}


bool CLuaMaker::_WriteEncodeItemToUnion( SItemInfo* pstInfo, char* pszStructName)
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

	fprintf(m_fpLua,
		"	%s(NetMsg, %s.%s%s);\n",
		strAddFunc, pszStructName, pstInfo->szName);

	return true;
}

bool CLuaMaker::_WriteDecodeItemToUnion( SItemInfo* pstInfo, char* pszStructName)
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

	fprintf(m_fpLua,
		"	%s.%s = %s(NetMsg);\n"\
		"	if -1 == CkDecode(NetMsg) then\n"\
		"		return nil;\n"\
		"	end\n",
		pszStructName, pstInfo->szName, strDelFunc);

	return true;
}

bool CLuaMaker::_MakeName()
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


bool CLuaMaker::_WriteCodeMapFuncToLua()
{
	fprintf(m_fpLua, "-----------------\n"\
		"-- funcmap begin --\n"\
		"-----------------\n\n");

	fprintf(m_fpLua, "MsgEncodeFuncMap = \n"\
		"{\n");

	for(int i = 0; i < m_poMainParser->GetMessageInfoCount(); i++)
	{
		SMessageInfo* pstMessageInfo = m_poMainParser->GetMessageInfoByPos(i);
		fprintf(m_fpLua, "	func%d=Encode_%s, \n", pstMessageInfo->nID, pstMessageInfo->szType);
	}

	fprintf(m_fpLua, "}\n\n");


	fprintf(m_fpLua, "MsgDecodeFuncMap = \n"\
		"{\n");

	for(int i = 0; i < m_poMainParser->GetMessageInfoCount(); i++)
	{
		SMessageInfo* pstMessageInfo = m_poMainParser->GetMessageInfoByPos(i);
		fprintf(m_fpLua, "	func%d=Decode_%s, \n", pstMessageInfo->nID, pstMessageInfo->szType);
	}

	fprintf(m_fpLua, "}\n\n");

	fprintf(m_fpLua, "function %s_Encode(MsgID, DTMsg)\n"\
		"	Key = string.format(\"func%%d\",MsgID)\n"\
		"	EncodeFunc = MsgEncodeFuncMap[Key]\n"\
		"	if nil == EncodeFunc then\n"\
		"		return nil;\n"\
		"	end\n"\
		"	NetMsg = GetNetMsg();\n"\
		"	return EncodeFunc(NetMsg, DTMsg);\n"\
		"end\n\n"\
		"function %s_Decode(MsgID, NetMsg)\n"\
		"	Key = string.format(\"func%%d\",MsgID)\n"\
		"	DecodeFunc = MsgDecodeFuncMap[Key]\n"\
		"	if nil == DecodeFunc then\n"\
		"		return nil;\n"\
		"	end\n"\
		"	MsgStruct={};\n"\
		"	return DecodeFunc(NetMsg, MsgStruct);\n"\
		"end\n\n", m_poMainParser->GetName(), m_poMainParser->GetName());


		fprintf(m_fpLua, "-----------------\n"\
		"-- funcmap end --\n"\
		"-----------------\n\n");

		return true;
}

