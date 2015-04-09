#include "stdafx.h"
#include "vscoder.h"
#include "Compiler.h"
#pragma pack(push, 1)

static LPCTSTR TYPE_CHAR		= "char";
static LPCTSTR TYPE_BYTE		= "byte";
static LPCTSTR TYPE_SHORT		= "short";
static LPCTSTR TYPE_WORD		= "word";
static LPCTSTR TYPE_INT			= "int";
static LPCTSTR TYPE_DWORD		= "dword";
static LPCTSTR TYPE_INT64		= "int64";
static LPCTSTR TYPE_UINT64		= "uint64";
static LPCTSTR TYPE_FLOAT		= "float";
static LPCTSTR TYPE_STRING		= "string";
static LPCTSTR TYPE_WSTRING		= "wstring";
static LPCTSTR TYPE_TSTRING		= "tstring";
static LPCTSTR CLASS_UNION		= "union";
static LPCTSTR CLASS_STRUCT		= "struct";

static LPCTSTR PREFIX_CHAR		= "ch";
static LPCTSTR PREFIX_BYTE		= "by";
static LPCTSTR PREFIX_SHORT		= "sh";
static LPCTSTR PREFIX_WORD		= "w";
static LPCTSTR PREFIX_INT		= "n";
static LPCTSTR PREFIX_DWORD		= "dw";
static LPCTSTR PREFIX_INT64		= "ll";
static LPCTSTR PREFIX_UINT64	= "qw";
static LPCTSTR PREFIX_FLOAT		= "f";
static LPCTSTR PREFIX_STRING	= "sz";
static LPCTSTR PREFIX_WSTRING	= "wz";
static LPCTSTR PREFIX_TSTRING	= "sz";
static LPCTSTR PREFIX_ARRAY		= "a";
static LPCTSTR PREFIX_STRUCT	= "st";

struct SIndex
{
	SIndex() { memset(this, 0, sizeof(SIndex)); }
	UINT8	wIdx;			
	UINT32	dwStructIdx;	
	UINT8	byReferIdx;		
	UINT8	bySelectIdx;	
	UINT8	byStringFlag;
	UINT16	wMaxArrNum;		
	UINT32	dwContextLen;	
};

struct SIndexHead
{
	SIndexHead() { memset(this, 0, sizeof(SIndexHead)); }
	UINT32	dwStructId ;
	UINT8	byIndexNum ;
};


struct SUnionIndex
{
	SUnionIndex() { memset(this, 0, sizeof(SUnionIndex)); }		
	UINT8	wSelectValue;	
	UINT8	wIdx;	
	UINT32	dwStructIdx;	
	UINT32	dwContextLen;	
};


CCompiler::CCompiler(INT32 nType, INT32 nCharsetNet, INT32 nCharsetHost)
{
	m_strErrMsg = "";
	m_nTypeSeq = 0;
    m_nType = nType;
    m_nCharsetNet = nCharsetNet;
    m_nCharsetHost = nCharsetHost;
	m_bCliEnDe = FALSE;
}

CCompiler::~CCompiler()
{
	for(size_t i = 0; i < m_oVecChildParsers.size(); i++)
	{
		delete m_oVecChildParsers[i];
	}
	m_oVecChildParsers.clear();
}

void CCompiler::AddChild(CCompiler* poCompiler)
{
    if (NULL != poCompiler)
    {
        m_vecChildren.push_back(poCompiler);
    }
}

CCompilerVec* CCompiler::GetChildren()
{
    return &m_vecChildren;
}

bool CCompiler::Compile(LPCTSTR pszFile, BOOL bCliEnDe, BOOL bSaveEnDe)
{
	m_strErrMsg.Format("Compile Failed......Unknown Error");

	m_bCliEnDe = bCliEnDe;

	if(false == _Parse(pszFile, bCliEnDe))
		return false;

	if(false == _Check())
		return false;

	if(bCliEnDe)
	{
		if(!_WriteCliIndex())
		{
			return false;
		}
	}

    m_oClassMaker.SetInfo(m_nType, m_nCharsetNet, m_nCharsetHost, bCliEnDe, bSaveEnDe);
	if(false == m_oClassMaker.Make(&m_oMainParser, &m_oVecChildParsers, m_strHPilePath,m_strCppFilePath))
	{
		m_strErrMsg = m_oClassMaker.GetErrMsg();
		return false;
	}
	m_strErrMsg.Format("%s Compile OK", pszFile);

	return true;

	//if(false == _SavePos())
	//	return false;

	//if(false == m_oClassMaker.Init())
	//	return false;

	//m_fpCpp = NULL;
	//m_fpH = NULL;
	//
	//if(false == _ProcFileHeader())
	//	return false;

	//if(false == _ProcDefines())
	//	return false;

	//if(false == _ProcTypes())
	//	return false;

	//if(false == _ProcMessages())
	//	return false;

	//if(false == m_oClassMaker.MakeEndPart())
	//	return false;

	//m_oClassMaker.SaveToHpp(m_fpH);
	//m_oClassMaker.SaveToCpp(m_fpCpp);
	//
	//fprintf(m_fpH, "#endif\n");

	//m_strErrMsg.Format("Compile OK");

	//_CloseFile();
	//
	//return true;
}

bool CCompiler::_Parse(LPCTSTR pszFile, BOOL bCliEnDe)
{
	if(false == m_oMainParser.Parse(pszFile, &m_oMainParser, bCliEnDe))
	{
		m_strErrMsg.Format("Parse %s failed:\r\n %s", pszFile, m_oMainParser.GetErrMsg());
		return false;
	}

	char szDir[MAX_PATH];
	lstrcpyn(szDir, pszFile, sizeof(szDir));
	char* p = szDir;
	while(*p != '\0')
	{
		if(*p == '/')
			*p = '\\';
		p++;
	}
	p = strrchr(szDir, '\\');
	if(p != NULL)
		*(p+1) = '\0';
	else
		szDir[0] = '\0';

	for(int i = 0; i < m_oMainParser.GetIncludeInfoCount(); i++)
	{
		SIncludeInfo* pstInfo = m_oMainParser.GetIncludeInfoByPos(i);
		ASSERT(pstInfo != NULL);

		char szFile[MAX_PATH];
		sprintf(szFile, "%s%s", szDir, pstInfo->szFile);

		CParser* poParser = new CParser;
		if(false == poParser->Parse(szFile, &m_oMainParser, bCliEnDe))
		{
			m_strErrMsg.Format("Parse %s failed:\r\n %s", szFile, poParser->GetErrMsg());
			delete poParser;
			return false;
		}

		m_oVecChildParsers.push_back(poParser);
	}

	return true;
}

bool CCompiler::_WriteCliIndex()
{
	int nIndex = m_strHPilePath.ReverseFind('\\');
	CString strPath =  m_strHPilePath.Left(nIndex);
	strPath += "\\";

	char szStructIndexFile[512] = {0};
	sprintf(szStructIndexFile, "%s%s_StructIndex.jj", strPath, m_oMainParser.GetName());
	FILE* pfStructIndexFile = fopen(szStructIndexFile, "wb+");
	if(NULL == pfStructIndexFile)
	{
		m_strErrMsg.Format("open '%s' failed!", szStructIndexFile);
		return false;
	}
	char szUnionIndexFile[512] = {0};
	sprintf(szUnionIndexFile, "%s%s_UnionIndex.jj", strPath, m_oMainParser.GetName());
	FILE* pfUnionIndexFile = fopen(szUnionIndexFile, "wb+");
	if(NULL == pfUnionIndexFile)
	{
		m_strErrMsg.Format("open '%s' failed!", szUnionIndexFile);
		return false;
	}

	if(!_WriteIndex(&m_oMainParser, pfStructIndexFile, pfUnionIndexFile))
	{
		return false;
	}

	for(size_t i = 0; i < m_oVecChildParsers.size(); i++)
	{
		if(!_WriteIndex(m_oVecChildParsers[i], pfStructIndexFile, pfUnionIndexFile))
		{
			return false;
		}
	}

	fclose(pfStructIndexFile);
	fclose(pfUnionIndexFile);

	return true;
}

bool CCompiler::_WriteIndex(CParser* poParser, FILE* pfStructIndexFile, FILE* pfUnionIndexFile)
{
	if(NULL == poParser)
	{
		m_strErrMsg.Format("poParser is NULL!");
		return false;
	}
	CTypeInfoVec& oVecTypeInfo = poParser->GetTypeInfoVec();
	for(size_t j = 0; j < oVecTypeInfo.size(); j++)
	{
		STypeInfo* pstTypeInfo = oVecTypeInfo[j];

		std::vector<SItemInfo*>& oVecItemInfo = pstTypeInfo->oVecItemInfo;

		SIndexHead stIndexHead;
		stIndexHead.dwStructId = pstTypeInfo->nStructID;
		stIndexHead.byIndexNum = oVecItemInfo.size();
		int nLen = fwrite((VOID*)(&stIndexHead), 1, sizeof(stIndexHead), pfStructIndexFile);
		if(nLen != sizeof(stIndexHead))
		{
			m_strErrMsg.Format("write index file error!");
			return false;
		}
		//fflush(pfStructIndexFile);//fortest

		for(size_t k = 0; k < oVecItemInfo.size(); k++)
		{
			SItemInfo* pstItemInfo = oVecItemInfo[k];
			if(NULL == pstItemInfo)
			{
				m_strErrMsg.Format("pstItemInfo is NULL!");
				return false;
			}
			SIndex stIndex;
			stIndex.wIdx = atoi(pstItemInfo->szKeyIdx);
			stIndex.dwStructIdx = pstItemInfo->nStructID;
			stIndex.byReferIdx = atoi(pstItemInfo->szReferKeyIdx);
			stIndex.bySelectIdx = atoi(pstItemInfo->szSelectKeyIdx);
			if('\0' != pstItemInfo->szCount[0])
			{
				SDefineInfo* pstDefineInfo = m_oMainParser.FindDefineInfoFromAll(pstItemInfo->szCount);
				if(pstDefineInfo)
				{
					stIndex.wMaxArrNum = atoi(pstDefineInfo->szValue);
				}
				else
				{
					stIndex.wMaxArrNum = atoi(pstItemInfo->szCount);
				}
			}
			else
			{
				stIndex.wMaxArrNum = 1;
			}

			switch(pstItemInfo->nDataType)
			{
			case DATA_CHAR:
			case DATA_BYTE:
				stIndex.dwContextLen = 1;
				break;
			case DATA_SHORT:
			case DATA_WORD:
				stIndex.dwContextLen = 2;
				break;
			case DATA_INT:
			case DATA_DWORD:
			case DATA_FLOAT:
				stIndex.dwContextLen = 4;
				break;
			case DATA_INT64:
			case DATA_UINT64:
				stIndex.dwContextLen = 8;
				break;
			case DATA_STRING:
			case DATA_TSTRING:
				stIndex.byStringFlag = 1;
				stIndex.dwContextLen = 2 + stIndex.wMaxArrNum;
				break;
			case DATA_STRUCT:
			case DATA_UNION:
				stIndex.dwContextLen = 0;
				break;
			case DATA_WSTRING:
				stIndex.byStringFlag = 1;
				stIndex.dwContextLen = 2 + stIndex.wMaxArrNum * 2;
				break;
			default:
				stIndex.wMaxArrNum = 0;
				stIndex.dwContextLen = 0;
				break;
			}
			nLen = fwrite((VOID*)(&stIndex), 1, sizeof(stIndex), pfStructIndexFile);
			if(nLen != sizeof(stIndex))
			{
				m_strErrMsg.Format("write index file error!");
				return false;
			}
			//fflush(pfStructIndexFile);//fortest
		}


		if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
		{
			std::vector<SItemInfo*>& oVecItemInfo = pstTypeInfo->oVecItemInfo;

			SIndexHead stIndexHead;
			stIndexHead.dwStructId = pstTypeInfo->nStructID;
			stIndexHead.byIndexNum = oVecItemInfo.size();
			fwrite((VOID*)(&stIndexHead), sizeof(stIndexHead), 1, pfUnionIndexFile);

			for(size_t k = 0; k < oVecItemInfo.size(); k++)
			{
				SItemInfo* pstItemInfo = oVecItemInfo[k];
				if(NULL == pstItemInfo)
				{
					m_strErrMsg.Format("pstItemInfo is NULL!");
					return false;
				}

				SUnionIndex stIndex;
				stIndex.wIdx = atoi(pstItemInfo->szKeyIdx);
				stIndex.dwStructIdx = pstItemInfo->nStructID;
				stIndex.wSelectValue = pstItemInfo->nSelectValue;
				stIndex.dwContextLen = 0;//					
				nLen = fwrite((VOID*)(&stIndex), 1, sizeof(stIndex), pfUnionIndexFile);
				if(nLen != sizeof(stIndex))
				{
					m_strErrMsg.Format("write index file error!");
					return false;
				}
			}
		}
	}

	return true;
}

bool CCompiler::_Check()
{
	m_oVecChildParsers.push_back(&m_oMainParser);
	if(false == _CheckName())
	{
		m_oVecChildParsers.pop_back();
		return false;
	}

	if(false == _CheckTypes())
	{
		m_oVecChildParsers.pop_back();
		return false;
	}

	if(false == _CheckMessages())
	{
		m_oVecChildParsers.pop_back();
		return false;
	}

	m_oVecChildParsers.pop_back();
	return true;
}

bool CCompiler::_CheckName()
{
	int i;
	CStrMap oMapName;
	for(int nParser = 0; nParser < (int)m_oVecChildParsers.size(); nParser++)
	{
		CParser* poParser = m_oVecChildParsers[nParser];
		for(i = 0; i < poParser->GetDefineInfoCount(); i++)
		{
			SDefineInfo* pstDefineInfo = poParser->GetDefineInfoByPos(i);
			if(oMapName.find(pstDefineInfo->szName) != oMapName.end())
			{
				m_strErrMsg.Format("%s error: <define name=%s> already defined", poParser->GetName(), pstDefineInfo->szName);
				return false;
			}
			oMapName[pstDefineInfo->szName] = 0;
			m_oDefineInfoMap[pstDefineInfo->szName] = pstDefineInfo;
		}

		for(i = 0; i < poParser->GetTypeInfoCount(); i++)
		{
			STypeInfo* pstTypeInfo = poParser->GetTypeInfoByPos(i);
			if(oMapName.find(pstTypeInfo->szName) != oMapName.end())
			{
				m_strErrMsg.Format("%s error: <type name=%s> already defined", poParser->GetName(), pstTypeInfo->szName);
				return false;
			}
			oMapName[pstTypeInfo->szName] = 0;

			pstTypeInfo->nSeq = ++m_nTypeSeq;
			m_oMapTypeInfo[pstTypeInfo->szName] = pstTypeInfo;
		}

		for(i = 0; i < poParser->GetMessageInfoCount(); i++)
		{
			SMessageInfo* pstMessageInfo = poParser->GetMessageInfoByPos(i);
			if(oMapName.find(pstMessageInfo->szName) != oMapName.end())
			{
				m_strErrMsg.Format("%s error: <message name=%s> already defined", poParser->GetName(), pstMessageInfo->szName);
				return false;
			}
			oMapName[pstMessageInfo->szName] = 0;
		}
	}
	return true;
}

bool CCompiler::_CheckTypes()
{
	CString strPreFix;
	CString strMemberName;

	for(int i = 0; i < m_oMainParser.GetTypeInfoCount(); i++)
	{
		STypeInfo* pstTypeInfo = m_oMainParser.GetTypeInfoByPos(i);
		for(size_t nItem = 0; nItem < pstTypeInfo->oVecItemInfo.size(); nItem++)
		{
			SItemInfo* pstItem = pstTypeInfo->oVecItemInfo[nItem];
			pstItem->nDataType = _FindDataType(pstItem->szType, pstTypeInfo->nSeq, strPreFix);

			if(DATA_UNKNOWN == pstItem->nDataType)
			{
				m_strErrMsg.Format("error: <item name=%s type=%s>, type is not defined", pstItem->szName, pstItem->szType);
				return false;
			}

			if( (DATA_STRING == pstItem->nDataType) && ('\0' == pstItem->szCount[0]) )
			{
				m_strErrMsg.Format("<type name=%s> item %s is string, but count not define", 
					pstTypeInfo->szName, pstItem->szName);
				return false;
			}

			if(DATA_STRUCT == pstItem->nDataType || DATA_UNION == pstItem->nDataType)
			{
				if(pstItem->szCount[0] != '\0')
				{
					strMemberName.Format("%s%s%s", PREFIX_ARRAY, PREFIX_STRUCT, pstItem->szName);
				}
				else
				{
					strMemberName.Format("%s%s", PREFIX_STRUCT, pstItem->szName);
				}
				if(m_bCliEnDe)
				{
					STypeInfo* pstTypeInfo = m_oMainParser.FindTypeInfoFromAll(pstItem->szType);
					if(NULL == pstTypeInfo)
					{
						m_strErrMsg.Format("<type name=%s> item %s type %s not define", 
							pstTypeInfo->szName, pstItem->szType);
						return false;
					}
					pstItem->nStructID = pstTypeInfo->nStructID;
				}

			}
			else
			{
				if(pstItem->szCount[0] != '\0')
				{
					if(DATA_STRING == pstItem->nDataType)
					{
						strMemberName = strPreFix + pstItem->szName;
					}
					else
					{
						strMemberName = PREFIX_ARRAY + strPreFix + pstItem->szName;
					}
				}
				else
				{
					strMemberName = strPreFix + pstItem->szName;
				}
			}

			lstrcpyn(pstItem->szMemberName, strMemberName, sizeof(pstItem->szMemberName));
			if('\0' != pstItem->szRefer[0])
			{
				std::map<string, SItemInfo*>::iterator itr = pstTypeInfo->oMapItemInfo.find(pstItem->szRefer);				
				if(itr == pstTypeInfo->oMapItemInfo.end())
				{
					m_strErrMsg.Format("<type name=%s>\r\n<item name=%s type=%s>\r\nrefer[%s] not found!", 
						pstTypeInfo->szName, pstItem->szName, pstItem->szType, pstItem->szRefer);
					return false;
				}
				lstrcpyn(pstItem->szReferMemberName, itr->second->szMemberName, sizeof(pstItem->szReferMemberName));
				lstrcpyn(pstItem->szReferKeyIdx, itr->second->szKeyIdx, sizeof(pstItem->szReferKeyIdx));				
				pstItem->nReferDataType = itr->second->nDataType;

			}		
			if('\0' != pstItem->szSelect[0])
			{
				std::map<string, SItemInfo*>::iterator itr = pstTypeInfo->oMapItemInfo.find(pstItem->szSelect);				
				if(itr == pstTypeInfo->oMapItemInfo.end())
				{
					m_strErrMsg.Format("<type name=%s>\r\n<item name=%s type=%s>\r\nselect[%s] not found!", 
						pstTypeInfo->szName, pstItem->szName, pstItem->szType, pstItem->szSelect);
					return false;
				}
				lstrcpyn(pstItem->szSelectKeyIdx, itr->second->szKeyIdx, sizeof(pstItem->szSelectKeyIdx));				
				pstItem->nSelectDataType = itr->second->nDataType;

			}	
			if('\0' != pstItem->szID[0])
			{
				SDefineInfo* pstDefineInfo = m_oMainParser.FindDefineInfoFromAll(pstItem->szID);
				if(NULL == pstDefineInfo)
				{
					m_strErrMsg.Format("<type name=%s>\r\n<item name=%s type=%s>\r\nselect id[%s] not found!", 
						pstTypeInfo->szName, pstItem->szName, pstItem->szType, pstItem->szID);
					return false;
				}
				pstItem->nSelectValue = atoi(pstDefineInfo->szValue);

			}
		}

		if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
		{
			if(false == _CheckUnionInfo(pstTypeInfo))
				return false;
		}
		else if(CLASS_TYPE_STRUCT == pstTypeInfo->nClassType)
		{
			if(false == _CheckStructInfo(pstTypeInfo))
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

bool CCompiler::_CheckStructInfo(STypeInfo* pstTypeInfo)
{
	//refer, select
	CString strTmp;
	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{
		SItemInfo* pstItem = pstTypeInfo->oVecItemInfo[i];

		//Check if count is available
		if('\0' != pstItem->szCount[0])
		{
			if(DATA_UNION == pstItem->nDataType)
			{
				m_strErrMsg.Format("Error:<type name=%s>\r\n<item name=%s>\r\n"
					"union can be an array",
					pstTypeInfo->szName, pstItem->szName);
				return false;
			}

			if(false == IsNumber(pstItem->szCount))
			{
				if(m_oDefineInfoMap.find(pstItem->szCount) == m_oDefineInfoMap.end())
				{
					m_strErrMsg.Format("Error:<type name=%s>\r\n<item name=%s count=%s>\r\n"
						"%s is not define", pstTypeInfo->szName, pstItem->szName, pstItem->szCount, pstItem->szCount);
					return false;
				}
			}
		}

		//check refer
		if(false == _CheckRefer(pstItem->szRefer, &(pstTypeInfo->oVecItemInfo), i))
		{
			m_strErrMsg.Format("Error:<type name=%s>\r\n<item name=%s refer=%s>\r\n"
				"refer %s is not define", pstTypeInfo->szName, pstItem->szName, pstItem->szReferMemberName, pstItem->szReferMemberName);
			return false;
		}

		//check select
		if(DATA_UNION == pstItem->nDataType)
		{
			if('\0' == pstItem->szSelect[0])
			{
				m_strErrMsg.Format("Error:<type name=%s>\r\n<item name=%s type=%s>\r\n"
					"union must have select", pstTypeInfo->szName, pstItem->szName, pstItem->szType);
				return false;
			}

			if(false == _CheckSelect(pstItem->szSelect, &(pstTypeInfo->oVecItemInfo), i))
			{
				m_strErrMsg.Format("Error:<type name=%s>\r\n<item name=%s type=%s>\r\n"
					"union select %s is not valid", 
					pstTypeInfo->szName, pstItem->szName, pstItem->szSelect, pstItem->szSelect);
				return false;
			}
		}


		//// 2009-03-10 cwy add begin
		// check none-follow
		if ('\0' != pstItem->szNoneFollow[0])
		{
			if (DATA_UNION == pstItem->nDataType ||
				DATA_STRUCT == pstItem->nDataType ||
				DATA_STRING == pstItem->nDataType ||
                DATA_WSTRING == pstItem->nDataType ||
				DATA_UNKNOWN == pstItem->nDataType)
			{
				m_strErrMsg.Format("Error:<type name=%s>\r\n"
					"	<item name=\"%s\" type=\"%s\" none-follow=\"%s\">\r\n"
					"none-follow must used with a single type.",
					pstTypeInfo->szName, pstItem->szName, pstItem->szType, pstItem->szNoneFollow);
				return false;
			}
			if ('\0' != pstItem->szCount[0])
			{
				m_strErrMsg.Format("Error:<type name=%s>\r\n"
					"	<item name=\"%s\" type=\"%s\" none-follow=\"%s\">\r\n"
					"none-follow must refers to a value, not an array.",
					pstTypeInfo->szName, pstItem->szName, pstItem->szType, pstItem->szNoneFollow);
				return false;
			}
			if (false == _CheckNoneFollow(pstItem->szNoneFollow))
			{
				m_strErrMsg.Format("Error:<type name=%s>\r\n"
					"	<item name=\"%s\" type=\"%s\" none-follow=\"%s\">\r\n"
					"none-follow is not valid, check values or macro definitions", 
					pstTypeInfo->szName, pstItem->szName, pstItem->szType, pstItem->szNoneFollow);
				return false;
			}
		}
	}

	return true;
}

bool CCompiler::_CheckUnionInfo(STypeInfo* pstTypeInfo)
{
	for(size_t i = 0; i < pstTypeInfo->oVecItemInfo.size(); i++)
	{
		SItemInfo* pstInfo = pstTypeInfo->oVecItemInfo[i];

		//// 2009-03-11 cwy modify begin

		if(DATA_UNION == pstInfo->nDataType)
		{
			m_strErrMsg.Format("<type name=%s>\r\n<item name=%s type=%s>\r\nin union, type can not be union", 
				pstTypeInfo->szName, pstInfo->szName, pstInfo->szType);
			return false;
			
		}

		if ('\0' != pstInfo->szCount[0])
		{
			m_strErrMsg.Format("<type name=%s>\r\n<item name=%s type=%s>\r\nin union, type can not be an array or string", 
				pstTypeInfo->szName, pstInfo->szName, pstInfo->szType);
			return false;
		}

		//// 2009-03-11 cwy modify end

		if('\0' == pstInfo->szID[0])
		{
			m_strErrMsg.Format("<type name=%s>\r\n<item name=%s type=%s>\r\n in union must have a id",
				pstTypeInfo->szName, pstInfo->szName, pstInfo->szType);
			return false;
		}

		if(false == IsNumber(pstInfo->szID))
		{
			if(m_oDefineInfoMap.find(pstInfo->szID) == m_oDefineInfoMap.end())
			{
				m_strErrMsg.Format("<item name=%s id=%s> id not define", pstInfo->szName, pstInfo->szID);
				return false;
			}
		}
	}

#pragma message ("注意：union 没有检查名称是否重复，没有检查id是否重复")

	return true;
}

bool CCompiler::_CheckRefer(LPCTSTR pszRefer, std::vector<SItemInfo*>* poVecItem, int nSelf)
{
	if('\0' == pszRefer[0])
		return true;

	for(size_t i = 0; i < poVecItem->size(); i++)
	{
		SItemInfo *pstInfo = poVecItem->at(i);

		if(i == nSelf)
			continue;

		if(0 == strcmp(pszRefer, pstInfo->szName))
		{
			//refer只能为数值类型
			if(DATA_UNION == pstInfo->nDataType 
				|| DATA_STRUCT == pstInfo->nDataType
				|| DATA_STRING == pstInfo->nDataType
                || DATA_WSTRING == pstInfo->nDataType
				|| DATA_UNKNOWN == pstInfo->nDataType)
			{
				return false;
			}

			//refer的目标不能是数组
			if('\0' != pstInfo->szCount[0])
				return false;

			strcpy(poVecItem->at(nSelf)->szReferMemberName, pstInfo->szMemberName);

			return true;
		}
	}

	return false;
}

bool CCompiler::_CheckSelect(LPCTSTR pszSelect, std::vector<SItemInfo*>* poVecItem, int nSelf)
{
	for(size_t i = 0; i < poVecItem->size(); i++)
	{
		SItemInfo* pstInfo = poVecItem->at(i);

		if(i == nSelf)
			continue;

		if(0 == strcmp(pstInfo->szName, pszSelect))
		{
			//select 只能为数值
			if(DATA_UNION == pstInfo->nDataType 
				|| DATA_STRUCT == pstInfo->nDataType
				|| DATA_STRING == pstInfo->nDataType
                || DATA_WSTRING == pstInfo->nDataType
				|| DATA_UNKNOWN == pstInfo->nDataType)
			{
				return false;
			}

			//select的目标不能为数组
			if('\0' != pstInfo->szCount[0])
				return false;

			strcpy(poVecItem->at(nSelf)->szSelect, pstInfo->szMemberName);

			return true;
		}
	}

	return false;
}


//// 2009-03-10 cwy add function
bool CCompiler::_CheckNoneFollow( LPCTSTR pszNoneFollow)
{
	char * szID,* szNextID;
	char szTokens[] = " ,;.";
	bool bFound = false;
	int nDefines = m_oMainParser.GetDefineInfoCount();
	int i = 0;
	char szNoneFollow[MAX_NAME_LEN];

	strcpy_s(szNoneFollow, MAX_NAME_LEN, pszNoneFollow);

	szID = strtok_s(szNoneFollow, szTokens, &szNextID);

	while (NULL != szID)
	{
		if(false == IsNumber(szNoneFollow))
		{
			if(m_oDefineInfoMap.find(szNoneFollow) == m_oDefineInfoMap.end())
			{				
				return false;
			}
		}
		szID = strtok_s(NULL, szTokens, &szNextID);
		
	}
	return true;
}

bool CCompiler::_CheckMessages()
{
	int nMin = 0;
	int nMax = 0;
	int nCurrentID = 0; // 记录当前的MessageID值，用于生成没有定义ID的Msg的ID
	bool bFirst = true;
	for(int i = 0; i < m_oMainParser.GetMessageInfoCount(); i++)
	{
		SMessageInfo* pstMessageInfo = m_oMainParser.GetMessageInfoByPos(i);

		CTypeInfoMap::iterator it = m_oMapTypeInfo.find(pstMessageInfo->szType);
		if(it == m_oMapTypeInfo.end())
		{
			m_strErrMsg.Format("Error: <message name=%s type=%s>\r\n %s not define",
				pstMessageInfo->szName, pstMessageInfo->szType, pstMessageInfo->szType);
			return false;
		}
		STypeInfo* pstTypeInfo = it->second;
		if(CLASS_TYPE_STRUCT != pstTypeInfo->nClassType)
		{
			m_strErrMsg.Format("Error: <message name=%s type=%s>\r\n %s must be struct",
				pstMessageInfo->szName, pstMessageInfo->szType, pstMessageInfo->szType);
			return false;
		}
		if (bFirst)
		{
			//// 2009-03-27 cwy add and modify for @001
			if (false == pstMessageInfo->bHasID)
			{						
				pstMessageInfo->nID = 0;				
			}
			nCurrentID = pstMessageInfo->nID;
			nMin = pstMessageInfo->nID;
			nMax = pstMessageInfo->nID;			
			bFirst = false;
			//// ~
		}

		//// 2009-03-27 cwy add and modify for @001
		if (false == pstMessageInfo->bHasID)
		{			
			pstMessageInfo->nID = nCurrentID + 1;			
		}
		nCurrentID = pstMessageInfo->nID;
		//// ~

		if (nMin > pstMessageInfo->nID)
		{
			nMin = pstMessageInfo->nID;
		}
		if (nMax < pstMessageInfo->nID)
		{
			nMax = pstMessageInfo->nID;
		}
	}
	m_oMainParser.SetMinMsgID(nMin);
	m_oMainParser.SetMaxMsgID(nMax);
	m_oMainParser.SetMsgIDRange(nMax - nMin + 1);
    for (DWORD n = 0; n < m_oVecChildParsers.size(); n ++)
    {
        CParser* poParser = m_oVecChildParsers[n];
        if (poParser)
        {
            if (poParser->GetMaxMsgID()>nMax)
            {
                nMax = poParser->GetMaxMsgID();
                
            }
            if(poParser->GetMinMsgID()< nMin)
            {
                nMin = poParser->GetMinMsgID();

            }
        }
    }
    m_oMainParser.SetMinMsgID(nMin);
    m_oMainParser.SetMaxMsgID(nMax);
    m_oMainParser.SetMsgIDRange(nMax - nMin + 1);
	return true;
}

int CCompiler::_FindDataType(LPCTSTR pszType, int nSeq, CString &strPrefix)
{
	if(0 == strcmp(pszType, TYPE_CHAR))
	{
		strPrefix = PREFIX_CHAR;
		return DATA_CHAR;
	}
	else if(0 == strcmp(pszType, TYPE_BYTE))
	{
		strPrefix = PREFIX_BYTE;
		return DATA_BYTE;
	}
	else if(0 == strcmp(pszType, TYPE_SHORT))
	{
		strPrefix = PREFIX_SHORT;
		return DATA_SHORT;
	}
	else if(0 == strcmp(pszType, TYPE_WORD))
	{
		strPrefix = PREFIX_WORD;
		return DATA_WORD;
	}
	else if(0 == strcmp(pszType, TYPE_INT))
	{
		strPrefix = PREFIX_INT;
		return DATA_INT;
	}
	else if(0 == strcmp(pszType, TYPE_DWORD))
	{
		strPrefix = PREFIX_DWORD;
		return DATA_DWORD;
	}
	else if(0 == strcmp(pszType, TYPE_INT64))
	{
		strPrefix = PREFIX_INT64;
		return DATA_INT64;
	}
	else if(0 == strcmp(pszType, TYPE_UINT64))
	{
		strPrefix = PREFIX_UINT64;
		return DATA_UINT64;
	}
	else if(0 == strcmp(pszType, TYPE_FLOAT))
	{
		strPrefix = PREFIX_FLOAT;
		return DATA_FLOAT;
	}
	else if(0 == strcmp(pszType, TYPE_STRING))
	{
		strPrefix = PREFIX_STRING;
		return DATA_STRING;
	}
    else if(0 == strcmp(pszType, TYPE_WSTRING))
    {
        strPrefix = PREFIX_WSTRING;
        return DATA_WSTRING;
    }	
    else if(0 == strcmp(pszType, TYPE_TSTRING))
    {
        strPrefix = PREFIX_TSTRING;
        return DATA_TSTRING;
    }	
    else
	{
		CTypeInfoMap::iterator it = m_oMapTypeInfo.find(pszType);
		if(it == m_oMapTypeInfo.end())
			return DATA_UNKNOWN;

		STypeInfo* pstTypeInfo = it->second;
		
		//fortest
		//// 定义顺序必须在之前
		//if(pstTypeInfo->nSeq >= nSeq)
		//	return DATA_UNKNOWN;

		strPrefix = PREFIX_STRUCT;

		if(CLASS_TYPE_STRUCT == pstTypeInfo->nClassType)
		{
			return DATA_STRUCT;
		}
		else if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
		{
			return DATA_UNION;
		}
		else
		{
			return DATA_UNKNOWN;
		}
	}
}

void CCompiler::SetHFilePath(LPCTSTR pszPath)
{
	m_strHPilePath = pszPath;
}

void CCompiler::SetCppFilePath(LPCTSTR pszPath)
{
	m_strCppFilePath = pszPath;
}

#pragma pack(pop)
