#include <string>
#include <iostream>
#include <vector>
#include <wtypes.h>
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <map>
#include <stdio.h>
#include <sdxmlfile.h>
#include <sdhashmap.h>
//#include "errdef.h"

using namespace std;
using namespace SGDP;

struct SErrProp
{
	int nID;
	string strVarname; 
	string strMsg;	
	map<string, string> mapType2Msg;
};


typedef vector<SErrProp> CErrPropVec;
typedef CErrPropVec::iterator CErrPropVecItr;
struct SErrNamespace
{
	string strNamespace;
	CErrPropVec vecErrProp;
};

struct SErr
{
	string strOutFileName;
	vector<string> vecMsgTypes;
	vector<SErrNamespace> vecErrNamespace;
};

static SErr stErr;

BOOL ReadMessage(CSDXMLNode rootNode, string strNamespace, CErrPropVec& vecErrProp)
{
	CSDXMLNode xmlNode = rootNode["message"];
	if(xmlNode == NULL)
	{
		return FALSE;
	}
	int nLastID = -1;
	vector<string>& vecMsgTypes = stErr.vecMsgTypes;
	while(xmlNode != NULL)
	{
		SErrProp stErrProp;
		stErrProp.nID = xmlNode("id").AsInteger();
		stErrProp.strVarname = SDUTF82Local(xmlNode("varname").AsString().c_str());
		stErrProp.strMsg = SDUTF82Local(xmlNode("msg").AsString().c_str());
		for(vector<string>::iterator itr = vecMsgTypes.begin(); itr != vecMsgTypes.end(); itr++)
		{
			string strType = string("msg_") + *itr;
			string strTypeMsg = SDUTF82Local(xmlNode(strType.c_str()).AsString().c_str());
			if(strTypeMsg.empty()) //使用默认的
			{
				stErrProp.mapType2Msg[strType] = stErrProp.strMsg;
			}
			else
			{
				stErrProp.mapType2Msg[strType] = strTypeMsg;
			}
		}

		if((stErrProp.nID != nLastID+1) && (0 != strNamespace.compare("ERR_COMMON")))
		{
			cout<<"namespace "<<strNamespace<<" ID["<<stErrProp.nID<<"] turn invalid"<<endl;
			return FALSE;
		}

		vecErrProp.push_back(stErrProp);
		nLastID = stErrProp.nID;
		
		xmlNode = xmlNode.Sibling("message");
	}

	return TRUE;
}

BOOL LoadFromXML(string strFile)
{
	CSDXMLFile xmlFile;

	if(FALSE == xmlFile.Load(strFile.c_str()))
	{
		cout<<"parse xml failed"<<endl;
		return FALSE;
	}	

	CSDXMLNode rootNode = xmlFile.GetRootNode();
	CSDXMLNode xmlNode = rootNode["output"];
	if(xmlNode == NULL)
	{
		cout<<"output is invalid"<<endl;
		return FALSE;
	}
	stErr.strOutFileName = xmlNode("filename").AsString("errdef.h");

	xmlNode = rootNode["msgtype"];
	if(xmlNode == NULL)
	{
		cout<<"msgtype is invalid"<<endl;
		return FALSE;
	}
	CHAR szTmp[128] = {0};
	for(UINT8 byIdx = 1; byIdx <= 255; byIdx++)
	{
		sprintf(szTmp, "type%u", byIdx);
		string strType = xmlNode(szTmp).AsString("");
		if(strType.empty())
		{
			break;
		}
		stErr.vecMsgTypes.push_back(strType);
	}

	CSDXMLNode xmlNamespaceNode = rootNode["namespace"];
	while(xmlNamespaceNode != NULL)
	{
		SErrNamespace stErrNamespace;
		stErrNamespace.strNamespace = xmlNamespaceNode("name").AsString();
		if(!ReadMessage(xmlNamespaceNode, stErrNamespace.strNamespace, stErrNamespace.vecErrProp))
		{
			cout<<"ReadMessage failed"<<endl;
			return FALSE;
		}
		stErr.vecErrNamespace.push_back(stErrNamespace);

		xmlNamespaceNode = xmlNamespaceNode.Sibling("namespace");
	}


	return TRUE;
}

BOOL WriteErrFile(BOOL bUtf8 = FALSE)
{
	FILE* pf = fopen(stErr.strOutFileName.c_str(), "w");
	if(NULL == pf)
	{
		cout<<"fopen "<<stErr.strOutFileName.c_str()<<"failed!"<<endl;
		return FALSE;
	}

	FILE* pfLua = fopen("errdef.lua", "w");
	if(NULL == pfLua)
	{
		cout<<"fopen "<<"errdef.lua"<<"failed!"<<endl;
		return FALSE;
	}

	vector<FILE*> vecTypeMsgFile;

	vector<string>& vecMsgTypes = stErr.vecMsgTypes;
	for(vector<string>::iterator itr = vecMsgTypes.begin(); itr != vecMsgTypes.end(); itr++)
	{
		string strFile = string("errdef_") + *itr + string(".json");
		FILE* pfTmp = fopen(strFile.c_str(), "w");
		if(NULL == pfTmp)
		{
			cout<<"fopen "<<strFile<<"failed!"<<endl;
			return FALSE;
		}
		vecTypeMsgFile.push_back(pfTmp);
	}
	UINT8 byMsgTypesSize = vecMsgTypes.size();
	for(UINT8 byIdx = 0; byIdx < byMsgTypesSize; byIdx++)
	{
		fprintf(vecTypeMsgFile[byIdx], "{\n");
	}

	char szBuff[1024] = {0};
		
	/*
	if(bUtf8)
	{
		//写utf8 bom头
		CHAR szBOM[4] = {(CHAR)0xEF, (CHAR)0xBB, (CHAR)0xBF, 0};  
		fwrite(szBOM,1,4,pf) ;
	}
	*/

	fprintf(pf, "#ifndef __ERRDEF_H__\n#define __ERRDEF_H__\n\n");

	map<string, UINT32> mapWriteNum;
	UINT16 wSize = stErr.vecErrNamespace.size();
	for(UINT16 wIdx = 0; wIdx < wSize; wIdx++)
	{
		SErrNamespace& stErrNamespace = stErr.vecErrNamespace[wIdx];

		fprintf(pf, "namespace %s \n{\n", stErrNamespace.strNamespace.c_str());		
		fprintf(pfLua, "%s=\n{\n",
			stErrNamespace.strNamespace.c_str());
		

		//写单个
		for(CErrPropVecItr itr = stErrNamespace.vecErrProp.begin(); itr != stErrNamespace.vecErrProp.end(); itr++)
		{
			SErrProp& stSErrProp = *itr;
			if(bUtf8)
			{
				fprintf(pf, "\tstatic const unsigned int ID_%s = %d;\n", stSErrProp.strVarname.c_str(), stSErrProp.nID);
				sprintf(szBuff, "\t//static const char* STR_%s = \"%s\";\n\n", stSErrProp.strVarname.c_str(), stSErrProp.strMsg.c_str());
				fprintf(pf, "%s", SDLocal2UTF8(szBuff).c_str());
			}
			else
			{
				fprintf(pf, "\tstatic const unsigned int ID_%s = %d;\n", stSErrProp.strVarname.c_str(), stSErrProp.nID);
				fprintf(pf, "\tstatic const char* STR_%s = \"%s\";\n\n", stSErrProp.strVarname.c_str(), stSErrProp.strMsg.c_str());
			}			
			
			sprintf(szBuff, "\t%s=%d, [%d]='%s', \n", stSErrProp.strVarname.c_str(), stSErrProp.nID, stSErrProp.nID, stSErrProp.strMsg.c_str());
			fprintf(pfLua, "%s", SDLocal2UTF8(szBuff).c_str());			
		}
		fprintf(pf, "\tstatic const char* STR_NAMESPACE = \"%s\";\n", stErrNamespace.strNamespace.c_str()); //客户端需要
		fprintf(pf, "\tstatic void func(){ const char * psz = STR_NAMESPACE; psz = psz;}\n\n");//客户端需要


		//已经不用
		
		//写错误信息数组		
		if(!bUtf8)
		{
			fprintf(pf, "\tstatic const char* ErrMsg[] = {\n");		
			BOOL bFirst = TRUE;
			for(CErrPropVecItr itr = stErrNamespace.vecErrProp.begin(); itr != stErrNamespace.vecErrProp.end(); itr++)
			{
				SErrProp& stSErrProp = *itr;
				if(bFirst)
				{
					fprintf(pf, "\tSTR_%s\n", stSErrProp.strVarname.c_str());
					bFirst = FALSE;
				}
				else
				{
					fprintf(pf, "\t,STR_%s\n", stSErrProp.strVarname.c_str());
				}

			}		
			fprintf(pf, "\t};\n");
		}



		fprintf(pf,"}\n\n");

		fprintf(pfLua, "}\n\n");

		////////////////////////////////////////////
		//写资源文件
		for(UINT8 byIdx = 0; byIdx < byMsgTypesSize; byIdx++)
		{			
			for(CErrPropVecItr itr = stErrNamespace.vecErrProp.begin(); itr != stErrNamespace.vecErrProp.end(); itr++)
			{
				SErrProp& stSErrProp = *itr;
				if(0 != mapWriteNum[vecMsgTypes[byIdx]])
				{
					fprintf(vecTypeMsgFile[byIdx], ",");
				}
				if(bUtf8)
				{
					fprintf(vecTypeMsgFile[byIdx], "%s_%u:'%s'\n", SDLocal2UTF8(stErrNamespace.strNamespace.c_str()).c_str(), stSErrProp.nID, SDLocal2UTF8(stSErrProp.strMsg.c_str()).c_str());
				}
				else
				{
					fprintf(vecTypeMsgFile[byIdx], "%s_%u:'%s'\n", SDLocal2UTF8(stErrNamespace.strNamespace.c_str()).c_str(), stSErrProp.nID, SDLocal2UTF8(stSErrProp.strMsg.c_str()).c_str());
				}
				mapWriteNum[vecMsgTypes[byIdx]]++;
			}			
		}

	}
	
	//写尾
	fprintf(pf, "#endif\n");
	fclose(pf);


	for(UINT8 byIdx = 0; byIdx < byMsgTypesSize; byIdx++)
	{
		fprintf(vecTypeMsgFile[byIdx], "}\n");
	}

	return TRUE;
}

int main(int argc, char** argv)
{
	if(argc !=3 )
	{
		cout<<"Usage:"<<argv[0]<<" [xml file path] "<<" [utf flag]"<<endl;
		return -1;
	}

	if(!LoadFromXML(argv[1]))
	{
		cout<<"LoadFromXML failed"<<endl;
		//getchar();
		return FALSE;
	}

	BOOL bUtf8 = FALSE;
	if(0 == strcmp("1",argv[2]))
	{
		bUtf8 = TRUE;
	}
	if(!WriteErrFile(bUtf8))
	{
		cout<<"WriteErrFile failed"<<endl;
		//getchar();
		return FALSE;
	}

	cout<<"gen file success!"<<endl;

	//getchar();
	return 0;
}
