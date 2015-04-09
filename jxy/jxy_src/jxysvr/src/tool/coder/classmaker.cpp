#include "stdafx.h"

#include "Coder.h"
#include "ClassMaker.h"
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

CClassMaker::CClassMaker()
{
	m_poMainParser		= NULL;
	m_poVecChildParser	= NULL;
	m_strErrMsg = "";
	m_fpCpp		= NULL;
	m_fpH		= NULL;
}

CClassMaker::~CClassMaker()
{
	if(m_fpH != NULL)
		fclose(m_fpH);

	if(m_fpCpp != NULL)
		fclose(m_fpCpp);
}

void CClassMaker::SetInfo(INT32 nType, INT32 nCharsetNet, INT32 nCharsetHost)
{
    m_nType        = nType;
    m_nCharsetNet  = nCharsetNet;
    m_nCharsetHost = nCharsetHost;
}

bool CClassMaker::Make(CParser* poMainParser, CParserVec* poChildParser, CString strHFilepath,CString strCppFilePath)
{
	m_strErrMsg.Format("Make .cpp and .h failed!");

	m_poMainParser		= poMainParser;
	m_poVecChildParser	= poChildParser;
	
	m_strClassName.Format("C%s", poMainParser->GetName());

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
	return true;
}

bool CClassMaker::_OpenFile( CString strHFilepath,CString strCppFilePath)
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
	fprintf(m_fpH, "#pragma pack(push, 1)\n");
	fprintf(m_fpH, "\n");

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

	fprintf(m_fpH, "/*Defines End*/\n\n\n");

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
			//。。。
			if(0 == strncmp(pstTypeInfo->szName, "PKT_", 4))
			{
				fprintf(m_fpH, "\ttag%s() { memset(this, 0, sizeof(tag%s)); }\n", pstTypeInfo->szName, pstTypeInfo->szName);
			}
			//。。。
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
		fprintf(m_fpH, "}%s;\n\n", pstTypeInfo->szName);
	}

	fprintf(m_fpH, "/*Define Structs and Unions        End*/\n\n\n");
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

    //添加对象实例化函数
    fprintf(m_fpH, "    static %s * Instance();\n",m_strClassName);

	fprintf(m_fpH, "    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);\n");
	fprintf(m_fpH, "    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);\n");
    fprintf(m_fpH, "	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }\n");
	fprintf(m_fpH, "	%s FindEncodeFunc(INT32 iMsgID);\n", m_strEnFuncDef);
	fprintf(m_fpH, "	%s FindDecodeFunc(INT32 iMsgID);\n", m_strDeFuncDef);
	fprintf(m_fpH, "	BOOL Init();\n");
	fprintf(m_fpH, "\n"
		"private:\n"
		"\t%s();\n"
		"\tvirtual ~%s();\n"
		"	%s m_EncodeFuncArray[%s_MSGID_RANGE];\n"
		"	%s m_DecodeFuncArray[%s_MSGID_RANGE];\n"
		"	CNetData m_oData;\n"
        "	INT32 m_iMaxPkgSize;\n"	
		"};\n"
		"\n"
		"#pragma pack(pop)\n"
		"#endif\n",		
		//m_strEnFuncDef, MAX_MESSAGE_ID,
		//m_strDeFuncDef, MAX_MESSAGE_ID);
		m_strClassName,m_strClassName,m_strEnFuncDef, m_strUpperName, m_strDeFuncDef, m_strUpperName);	//// 2009-03-09 cwy modify
	return true;
}

bool CClassMaker::_WriteIncludeToCpp()
{
	char szLowerName[256];
	strcpy(szLowerName, m_poMainParser->GetName());
	strlwr(szLowerName);
	fprintf(m_fpCpp, "#include \"%s.h\"\n\n", szLowerName);
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
		"	Init();\n"
		"}\n\n", 
		m_strClassName, m_strClassName);//构造函数实现

    //用于单线程实例
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

    // 注册子文件的消息到主文件的类
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
			if(false == _WriteEncodeFuncToCpp(pstTypeInfo))
				return false;
			if(false == _WriteDecodeFuncToCpp(pstTypeInfo))
				return false;
		}
		else if(CLASS_TYPE_UNION == pstTypeInfo->nClassType)
		{
			if(false == _WriteEncodeUnionToCpp(pstTypeInfo))
				return false;
			if(false == _WriteDecodeUnionToCpp(pstTypeInfo))
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
				if('\0' == pstInfo->szRefer[0])
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
							pstInfo->szRefer,
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
							pstInfo->szRefer,
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

	if(pstInfo->szCount[0] != '\0')
	{
		if('\0' == pstInfo->szRefer[0])
		{
			if (bIsIngnored)
			{
				fprintf(m_fpCpp,
					"		for(i = 0; i < %s; i++)\n"
					"		{\n"
					"			if(-1 == poNetData->%s(pstIn->%s[i]))\n"
					"				return -1;\n"
					"		}\n",
					pstInfo->szCount,
					strAddFunc, pstInfo->szMemberName);
			}
			else
			{
				fprintf(m_fpCpp,
					"	for(i = 0; i < %s; i++)\n"
					"	{\n"
					"		if(-1 == poNetData->%s(pstIn->%s[i]))\n"
					"			return -1;\n"
					"	}\n\n",
					pstInfo->szCount,
					strAddFunc, pstInfo->szMemberName);
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
					"			if(-1 == poNetData->%s(pstIn->%s[i]))\n"
					"				return -1;\n"
					"		}\n",
					pstInfo->szCount,
					pstInfo->szRefer,
					strAddFunc, pstInfo->szMemberName);
			}
			else
			{
				fprintf(m_fpCpp,
					"	for(i = 0; i < %s; i++)\n"
					"	{\n"
					"		if(i >= (INT32)pstIn->%s)\n"
					"			break;\n"
					"		if(-1 == poNetData->%s(pstIn->%s[i]))\n"
					"			return -1;\n"
					"	}\n\n",
					pstInfo->szCount,
					pstInfo->szRefer,
					strAddFunc, pstInfo->szMemberName);
			}
		}
	}
	else
	{	
		if (bIsIngnored)
		{
			fprintf(m_fpCpp,
				"		if(-1 == poNetData->%s(pstIn->%s))\n"
				"			return -1;\n",
				strAddFunc, pstInfo->szMemberName);
		}
		else
		{
			fprintf(m_fpCpp,
				"	if(-1 == poNetData->%s(pstIn->%s))\n"
				"		return -1;\n"
				"\n",
				strAddFunc, pstInfo->szMemberName);
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
				if('\0' == pstInfo->szRefer[0])
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
							pstInfo->szRefer,
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
							pstInfo->szRefer,
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

	if(pstInfo->szCount[0] != '\0')
	{
		if('\0' == pstInfo->szRefer[0])
		{
			if (bIsIngnored)
			{
				fprintf(m_fpCpp,
					"		for(i = 0; i < %s; i++)\n"
					"		{\n"
					"			if(-1 == poNetData->%s(pstOut->%s[i]))\n"
					"				return -1;\n"
					"		}\n",
					pstInfo->szCount,
					strDelFunc, pstInfo->szMemberName);
			}
			else
			{
				fprintf(m_fpCpp,
					"	for(i = 0; i < %s; i++)\n"
					"	{\n"
					"		if(-1 == poNetData->%s(pstOut->%s[i]))\n"
					"			return -1;\n"
					"	}\n\n",
					pstInfo->szCount,
					strDelFunc, pstInfo->szMemberName);
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
					"			if(-1 == poNetData->%s(pstOut->%s[i]))\n"
					"				return -1;\n"
					"		}\n",
					pstInfo->szCount,
					pstInfo->szRefer,
					strDelFunc, pstInfo->szMemberName);
			}
			else
			{
				fprintf(m_fpCpp,
					"	for(i = 0; i < %s; i++)\n"
					"	{\n"
					"		if(i >= (INT32)pstOut->%s)\n"
					"			break;\n"
					"		if(-1 == poNetData->%s(pstOut->%s[i]))\n"
					"			return -1;\n"
					"	}\n\n",
					pstInfo->szCount,
					pstInfo->szRefer,
					strDelFunc, pstInfo->szMemberName);
			}
		}
	}
	else
	{
		if (bIsIngnored)
		{
			fprintf(m_fpCpp,
				"		if(-1 == poNetData->%s(pstOut->%s))\n"
				"			return -1;\n",
				strDelFunc, pstInfo->szMemberName);
		}
		else
		{
			fprintf(m_fpCpp,
				"	if(-1 == poNetData->%s(pstOut->%s))\n"
				"		return -1;\n"
				"\n",
				strDelFunc, pstInfo->szMemberName);
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
