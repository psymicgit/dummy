#include "stdafx.h"
#include "asmaker.h"
#include <stdio.h>
bool CASMaker::Make( CASProtocolSource* poProto )
{
	m_poProto = poProto;
	if (false == _MakeDefinitionFile())
	{
		return false;
	}
	if (false == _MakeFuncDeclareFile())
	{
		return false;
	}
	if (false == _MakeHandlerFile())
	{
		return false;
	}
	if (false == _MakeStructFiles())
	{
		return false;
	}
	if (false == _MakeStructListFiles())
	{
		return false;
	}
	return true;
}

bool CASMaker::_MakeDefinitionFile()
{
	CString strDefFile = m_strPath + "\\" + m_poProto->GetClassName() + ".as";
	FILE* fp = fopen(strDefFile.GetString(),"w");
	if (NULL == fp)
	{
		m_strError.Format("CASMaker Error : Create %s file failed", strDefFile.GetString());
		return false;
	}

	// write codes to file
	fprintf(fp, "package %s\n", m_poProto->GetPackageName());
	fprintf(fp,"{\n ");

	// write import part;
	_WriteImportPart(fp);

	fprintf(fp,"\t//protocol\n");
	fprintf(fp,"\tpublic final class %s\n", m_poProto->GetClassName());
	fprintf(fp,"\t{\n");
	fprintf(fp,"\t\t//constants\n");

	int nDefineCount = m_poProto->GetDefineCount();
	CASDefine* pDefine = NULL;
	for (int i = 0; i < nDefineCount; i++)
	{
		pDefine = m_poProto->GetDefineByPos(i);
		fprintf(fp,"\t\tpublic static const %-25s = %s;\n", pDefine->GetName() + ":int", pDefine->GetValue());
	}
	fprintf(fp,"\t\t//message\n");

	int nSCMessageCount = m_poProto->GetSCMessageCount();
	CASMessage* pMessage = NULL;
	for (int i = 0; i < nSCMessageCount; i++)
	{
		pMessage = m_poProto->GetSCMessageByPos(i);
		fprintf(fp,"\t\tpublic static var %-25s = %s;\n", pMessage->GetName()+":int", pMessage->GetID());
		if (!pMessage->GetInfo().IsEmpty())
		{
			fprintf(fp,"\t//%s\n", pMessage->GetInfo());
		}
	}
	int nCSMessageCount = m_poProto->GetCSMessageCount();
	for (int i = 0; i < nCSMessageCount; i++)
	{
		pMessage = m_poProto->GetCSMessageByPos(i);
		if (!pMessage->IsBoth())	// check cs-message's 'both' tag, which is set during checking if message has more id
		{
			fprintf(fp,"\t\tpublic static var %-25s = %s;\n", pMessage->GetName()+ ":int", pMessage->GetID());
			if (!pMessage->GetInfo().IsEmpty())
			{
				fprintf(fp,"\t//%s\n", pMessage->GetInfo());
			}
		}
	}
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n");
	fclose(fp);
	return true;
}

bool CASMaker::_MakeFuncDeclareFile()
{
	CString strFDeclareFile = m_strPath + "\\I"+m_poProto->GetClassName()+"MsgProc"+".as";
	FILE* fp = fopen(strFDeclareFile.GetString(),"w");
	if (NULL == fp)
	{
		m_strError.Format("CASMaker Error : Create %s file failed", strFDeclareFile.GetString());
		return false;
	}
	// write codes to file
	fprintf(fp, "package %s\n", m_poProto->GetPackageName());
	fprintf(fp, "{\n");

	// write import part;
	_WriteImportPart(fp);

	fprintf(fp, "\tpublic class I%sMsgProc\n", m_poProto->GetClassName());
	fprintf(fp, "\t{\n");

	CASMessage* pCSMessage = NULL;
	CASType* pType = NULL;
	CASTypeItem* pItem = NULL;
	int nItemCount = 0;
	int nSCCount = m_poProto->GetSCMessageCount();
	
	for (int i = 0; i < nSCCount; i++)
	{
		pCSMessage = m_poProto->GetSCMessageByPos(i);
		fprintf(fp, "\t\tpublic function %s_Proc(", pCSMessage->GetName());
		pType = m_poProto->FindTypeByName(pCSMessage->GetType());

		//write function params
		nItemCount = pType->GetItemCount();		
		for (int j = 0; j < nItemCount; j++)
		{
			if (j > 0)
			{
				fprintf(fp, ", ");
			}
			pItem = pType->GetItemByPos(j);
			fprintf(fp, "%s%s:%s", 
				 EI_SimpleType == pItem->GetItemType()?"n":"",
				pItem->GetName(), pItem->GetVarType());	
		}
		fprintf(fp, "):void{ trace(\"%s\");}\n",pCSMessage->GetName());
	}		
	fprintf(fp, "\t}\n");
	fprintf(fp, "}\n");
	fclose(fp);
	return true;
}

bool CASMaker::_MakeHandlerFile()
{
	CString strHandlerFile = m_strPath + "\\Handler_"+m_poProto->GetClassName()+".as";
	FILE* fp = fopen(strHandlerFile.GetString(),"w");
	if (NULL == fp)
	{
		m_strError.Format("CASMaker Error : Create %s file failed", strHandlerFile.GetString());
		return false;
	}

	fprintf(fp, "package %s\n", m_poProto->GetPackageName());
	fprintf(fp, "{\n");

	// write import part;
	_WriteImportPart(fp);

	fprintf(fp, "\tpublic class Handler_%s extends Handler_Base\n", m_poProto->GetClassName());
	fprintf(fp, "\t{\n"); 
	fprintf(fp, "\t\tpublic var MsgProc:I%sMsgProc;\n", m_poProto->GetClassName());
	// constructor
	fprintf(fp, "\n\t\t//constructor\n");
	fprintf(fp, "\t\tpublic function Handler_%s (worldSock:SDTcpSocket, iMsgProc:I%sMsgProc)\n", m_poProto->GetClassName(), m_poProto->GetClassName());
	fprintf(fp, "\t\t{\n");
	fprintf(fp, "\t\t\tsuper(worldSock);\n");
	fprintf(fp, "\t\t\tMsgProc= iMsgProc;\n");
	fprintf(fp, "\n\t\t\t//Only Register Decode Functions\n");

	// register(SC)
	int nSCCount = m_poProto->GetSCMessageCount();
	CASMessage* pMessage = NULL;
	for (int i = 0; i < nSCCount; i++)
	{
		pMessage = m_poProto->GetSCMessageByPos(i);
		fprintf(fp, "\t\t\tRegisterHandler(%s.%s, %s_Decode);\n",m_poProto->GetClassName(), pMessage->GetName(), pMessage->GetName());
	}
	fprintf(fp, "\t\t}\n");

	// decode loop
	CASType* pType = NULL;
	for (int i = 0; i < nSCCount; i++)
	{	
		pMessage = m_poProto->GetSCMessageByPos(i);

		fprintf(fp, "\t\t//decode\n");
		fprintf(fp, "\t\tprivate function %s_Decode(pack:WorldPacket):void\n", pMessage->GetName());
		fprintf(fp, "\t\t{\n");
		//fprintf(fp, "\t\t\tvar %-31s = pack.Conv_WORD();\n", "nCmd:int");

		// get type, for iterate each item
		pType = m_poProto->FindTypeByName(pMessage->GetType());
		int nItemCount = pType->GetItemCount();
		CASTypeItem* pItem = NULL;
		CString strParams;

		// write each var's definition

		// item define
		for (int j = 0; j <nItemCount; j++)
		{										
			pItem = pType->GetItemByPos(j);
			//  simple ones first		
			if (EI_SimpleType == pItem->GetItemType())
			{
				if (pItem->IsArray())
				{
					fprintf(fp, "\t\t\tvar n%-30s:Array = pack.Conv_%s_Array(%s);\n", 
						pItem->GetName(), pItem->GetConvType(), pItem->GetRefer().IsEmpty()?pItem->GetLength():"n"+pItem->GetRefer());
				}
				else
				{
					// string or single simple type
					fprintf(fp, "\t\t\tvar n%-30s = pack.Conv_%s(", pItem->GetName() +  ":" + pItem->GetVarType(),
						pItem->GetConvType());
					if (!pItem->GetRefer().IsEmpty())
					{				
						fprintf(fp, "n%s);\n", pItem->GetRefer());
					}
					else if (!pItem->GetCount().IsEmpty())
					{				
						fprintf(fp, "%s);\n", pItem->GetLength());
					}					
					else
					{
						fprintf(fp,");\n");
					}				
				}
			}
			else
			{
				// item define, struct and struct lists	
				fprintf(fp, "\t\t\tvar n%-30s = %s.CreateFromBuffer(pack", pItem->GetName()+":"+pItem->GetVarType(),
					pItem->GetConvType());
				if (EI_StructType == pItem->GetItemType())
				{				
					fprintf(fp, ");\n");
				}
				else if (EI_StructListType == pItem->GetItemType())
				{
					CString strCount = pItem->GetCount();
					if (m_poProto->IsDefineExist(strCount))
					{
						strCount = m_poProto->GetClassName() + "." + strCount;
					}
					fprintf(fp, ", %s%s);\n", strCount, pItem->GetRefer().IsEmpty()?"":", n"+pItem->GetRefer());
				}					
				else
				{
					//fprintf(fp,");\n");
				}		
			}
			
		}
		fprintf(fp, "\n");

		// call decode function
		for (int j = 0; j <nItemCount; j++)
		{			
			// prepare params					
			pItem = pType->GetItemByPos(j);
			if (j > 0)
			{
				strParams += ", ";
			}
			strParams += "n" + pItem->GetName();
		}
		fprintf(fp, "\t\t\tMsgProc.%s_Proc(%s);\n",pMessage->GetName(), strParams);
		fprintf(fp, "\t\t}\n\n");
	}
	fprintf(fp, "\n");

	// encode loop
	int nCSCount = m_poProto->GetCSMessageCount();
	for (int i = 0; i < nCSCount; i++)
	{
		pMessage = m_poProto->GetCSMessageByPos(i);

		fprintf(fp, "\t\t//encode\n");
		fprintf(fp, "\t\tpublic function %s_Encode(", pMessage->GetType());

		// function params

		pType = m_poProto->FindTypeByName(pMessage->GetType());
		int nItemCount = pType->GetItemCount();
		CASTypeItem* pItem = NULL;
		for (int j = 0; j < nItemCount; j++)
		{
			pItem = pType->GetItemByPos(j);
			if (j > 0)
			{
				fprintf(fp,", ");
			}
			fprintf(fp,"%s:%s", pItem->GetName(), pItem->GetVarType());
		}
		fprintf(fp, "):void\n");
		fprintf(fp, "\t\t{\n");

		// function contents

		// SET_COMMAND
		fprintf(fp, "\t\t\tSET_COMMAND(%s.%s);\n", m_poProto->GetClassName(), pMessage->GetName());

		//	vars
		for (int k = 0; k < nItemCount; k++)
		{			
			pItem = pType->GetItemByPos(k);

			// simple types
			if (EI_SimpleType == pItem->GetItemType())
			{
				if (pItem->IsArray())
				{
					fprintf(fp, "\t\t\toutpack.Add_%s_Array(%s, %s%s);\n", pItem->GetConvType(), 
						pItem->GetName(), pItem->GetCount(), 
						pItem->GetRefer().IsEmpty()?"":", n"+ pItem->GetRefer());
				}
				else
				{
					fprintf(fp, "\t\t\toutpack.Add_%s(%s);\n", pItem->GetConvType(), pItem->GetName());		
				}
			}			
			else
			{
				// struct and struct list
				fprintf(fp, "\t\t\t%s.ToBuffer(GetPacket()", pItem->GetName());

				if (EI_StructType == pItem->GetItemType())
				{				
					fprintf(fp, ");\n");
				}
				else if (EI_StructListType == pItem->GetItemType())
				{
					if (!pItem->GetRefer().IsEmpty())
					{
						fprintf(fp, ", %s);\n", pItem->GetRefer());
					}
					else
					{
						fprintf(fp, ", %s);\n", pItem->GetCount());
					}
				}	
			}			
		}	
		fprintf(fp, "\n");		
		fprintf(fp, "\n\t\t\tSEND_SERVER();\n");
		fprintf(fp, "\t\t}\n\n");
	}
	fprintf(fp, "\t}\n");
	fprintf(fp, "}\n");
	fclose(fp);
	return true;
}

bool CASMaker::_MakeStructFiles()
{
	int nTypeCount = m_poProto->GetTypeCount();
	CASType * pType = NULL;
	for (int i = 0; i < nTypeCount; i++)
	{
		pType = m_poProto->GetTypeByPos(i);
		if (pType->IsStructType() && pType->IsGenerateClass())
		{
			if ( false == _MakeAStructFile(pType))
			{
				return false;
			}		
		}
	}
	return true;
}

bool CASMaker::_MakeStructListFiles()
{
	int nTypeCount = m_poProto->GetTypeCount();
	CASType * pType = NULL;
	for (int i = 0; i < nTypeCount; i++)
	{
		pType = m_poProto->GetTypeByPos(i);
		if (pType->IsStructArrayType() && pType->IsGenerateClass())
		{
			if ( false == _MakeAListFile(pType))
			{
				return false;
			}		
		}
	}
	return true;
}

bool CASMaker::_MakeAStructFile( CASType* pType )
{
	CString strFile = m_strPath + "\\" + pType->GetName() + ".as";
	FILE* fp = fopen(strFile.GetString(), "w");
	if (NULL == fp)
	{
		m_strError.Format("CASMaker Error : Create file %s failed!", strFile);
		return false;
	}

	// package begin
	fprintf(fp, "package %s\n", m_poProto->GetPackageName());
	fprintf(fp, "{\n");
	fprintf(fp, "\t// Description\n");

	// write import part;
	_WriteImportPart(fp);

	// class begin
	fprintf(fp, "\tpublic class %s\n", pType->GetName());
	fprintf(fp, "\t{\n");

	// class variables
	int nItemCount = pType->GetItemCount();
	CASTypeItem* pItem = NULL;
	for (int i = 0; i < nItemCount; i++)
	{
		pItem = pType->GetItemByPos(i);
		
		if (EI_SimpleType == pItem->GetItemType())
		{
			if (pItem->IsArray())
			{
				fprintf(fp, "\t\tpublic var %s:Array = new Array(%s);\n", 
					pItem->GetName(), pItem->GetLength());				
			}
			else
			{
				fprintf(fp, "\t\tpublic var %s:%s;\n", pItem->GetName(), pItem->GetVarType());
			}						
		}
		else if (EI_StructType == pItem->GetItemType())
		{
			fprintf(fp, "\t\tpublic var %s:%s = new %s;\n", pItem->GetName(), 
				pItem->GetVarType(), pItem->GetVarType());		
		}
		else if (EI_StructListType == pItem->GetItemType())
		{
			fprintf(fp, "\t\tpublic var %s:%s = new %s(%s);\n", pItem->GetName(), 
				pItem->GetVarType(), pItem->GetVarType(), pItem->GetLength());	
		}
	}
	fprintf(fp, "\n");
	
	// class member functions

	// function : CreateFromBuffer
	fprintf(fp, "\t\t//static creating\n");
	fprintf(fp, "\t\tstatic public function CreateFromBuffer(pack:WorldPacket):%s\n", pType->GetName());
	fprintf(fp, "\t\t{\n");
	fprintf(fp, "\t\t\tvar ret:%s = new %s;\n", pType->GetName(), pType->GetName());
	fprintf(fp, "\t\t\tret.FromBuffer(pack);\n");
	fprintf(fp, "\t\t\treturn ret;\n");
	fprintf(fp, "\t\t}\n");
	fprintf(fp, "\n");

	// function : FromBuffer
	fprintf(fp, "\t\t//serializing\n");
	fprintf(fp, "\t\tpublic function FromBuffer(pack:WorldPacket):void\n");
	fprintf(fp, "\t\t{\n");
	for (int i = 0; i < nItemCount; i++)
	{
		pItem = pType->GetItemByPos(i);
		if (EI_SimpleType == pItem->GetItemType())
		{
			if (pItem->IsArray())
			{				
				fprintf(fp, "\t\t\tfor (var i:int = 0; i < %s; i++)\n", pItem->GetLength());
				fprintf(fp, "\t\t\t{\n");
				fprintf(fp, "\t\t\t\t%s[i] = pack.Conv_%s();\n", pItem->GetName(), pItem->GetConvType());
				fprintf(fp, "\t\t\t}\n");
				fprintf(fp, "\t\t\t\n");
			}
			else
			{
				// is simple type, or string
				fprintf(fp, "\t\t\t%-15s = pack.Conv_%s(%s);\n", 
					pItem->GetName(), pItem->GetConvType(), pItem->GetLength().IsEmpty()?"":pItem->GetLength());						
			}								
		}
		else if (EI_StructType == pItem->GetItemType())
		{
			fprintf(fp, "\t\t\t%s.FromBuffer(pack);\n", pItem->GetName());
		}
		else if (EI_StructListType == pItem->GetItemType())
		{
			fprintf(fp, "\t\t\t%s.FromBuffer(pack, %s);\n", pItem->GetName(), pItem->GetLength());			
		}
	}	
	fprintf(fp, "\t\t}\n");
	fprintf(fp, "\n");

	// function : ToBuffer
	fprintf(fp, "\t\t//\n");
	fprintf(fp, "\t\tpublic function ToBuffer(pack:WorldPacket):void\n");
	fprintf(fp, "\t\t{\n");
	for (int i = 0; i < nItemCount; i++)
	{
		pItem = pType->GetItemByPos(i);
		if (EI_SimpleType == pItem->GetItemType())
		{
			if (pItem->IsArray())
			{				
				fprintf(fp, "\t\t\tfor (var i:int = 0; i < %s; i++)\n", pItem->GetLength());
				fprintf(fp, "\t\t\t{\n");
				fprintf(fp, "\t\t\t\tpack.Add_%s(%s[i]);\n", pItem->GetConvType(), pItem->GetName());
				fprintf(fp, "\t\t\t}\n");
				fprintf(fp, "\t\t\t\n");
			}
			else
			{
				// is simple type, or string
				if (0 == pItem->GetConvType().Compare("STR"))
				{
					fprintf(fp, "\t\t\tpack.Add_%s(%s, %s);\n", 
						pItem->GetConvType(), pItem->GetName(), pItem->GetLength().IsEmpty()?"":pItem->GetLength());		
				}
				else
				{
					fprintf(fp, "\t\t\tpack.Add_%s(%s);\n", 
						 pItem->GetConvType(), pItem->GetName());		
				}
			}								
		}
		else if (EI_StructType == pItem->GetItemType())
		{
			fprintf(fp, "\t\t\t%s.ToBuffer(pack);\n", pItem->GetName());
		}
		else if (EI_StructListType == pItem->GetItemType())
		{
			if (!pItem->GetRefer().IsEmpty())
			{
				fprintf(fp, "\t\t\t%s.ToBuffer(pack, %s);\n", pItem->GetName(), pItem->GetRefer());
			}
			else
			{
				CString strCount = pItem->GetCount();
				if (m_poProto->IsDefineExist(strCount))
				{
					strCount = m_poProto->GetClassName() + "." + strCount;
				}
				fprintf(fp, "\t\t\t%s.ToBuffer(pack, %s);\n", pItem->GetName(), strCount);
			}	
		}
	}	
	fprintf(fp, "\t\t}\n");
	fprintf(fp, "\n");

	// function : Clone
	fprintf(fp, "\t\t//clone object\n");
	fprintf(fp, "\t\tpublic function Clone(from:%s):void\n", pType->GetName());
	fprintf(fp, "\t\t{\n");
	for (int i = 0; i < nItemCount; i++)
	{
		pItem = pType->GetItemByPos(i);
		fprintf(fp, "\t\t\t%-15s =	from.%s;\n",  pItem->GetName(), pItem->GetName());
	}	
	fprintf(fp, "\t\t}\n");
	fprintf(fp, "\n");

	// class end
	fprintf(fp, "\t}\n");
	fprintf(fp, "\t\n");
	fprintf(fp, "}\n");

	fclose(fp);
	return true;
}

bool CASMaker::_MakeAListFile( CASType* pType )
{
	CString strClassName = pType->GetName() + "List";
	CString strFile = m_strPath + "\\" + strClassName + ".as";

	FILE* fp = fopen(strFile.GetString(), "w");
	if (NULL == fp)
	{
		m_strError.Format("CASMaker Error : Create file %s failed!", strFile);
		return false;
	}

	// package begin
	fprintf(fp, "package %s\n", m_poProto->GetPackageName());
	fprintf(fp, "{\n");
	fprintf(fp, "\t// Description\n");

	// write import part;
	_WriteImportPart(fp);

	// class begin
	fprintf(fp, "\tpublic final class %s\n", strClassName);
	fprintf(fp, "\t{\n");

	// class variables
	fprintf(fp, "\t\tprivate var m_%s_Arr:Array;\n\n", pType->GetName());

	// class member functions 

	// function : ***List
	fprintf(fp, "\t\tpublic function %sList(max:int)\n", pType->GetName());
	fprintf(fp, "\t\t{\n");
	fprintf(fp, "\t\t\tm_%s_Arr = new Array(max);\n", pType->GetName());
	fprintf(fp, "\t\t}\n");
	fprintf(fp, "\n");

	// function : CreateFromBuffer
	fprintf(fp, "\t\tpublic static function CreateFromBuffer(pack:WorldPacket, count:uint, refer:int=-1):%s\n", strClassName);
	fprintf(fp, "\t\t{\n");
	fprintf(fp, "\t\t\tvar ret:%sList = new %sList(count);\n", pType->GetName(), pType->GetName());
	fprintf(fp, "\t\t\tret.FromBuffer(pack, refer==-1?count:refer);\n");
	fprintf(fp, "\t\t\treturn ret;\n");
	fprintf(fp, "\t\t}\n");
	fprintf(fp, "\n");

	// function : FromBuffer
	fprintf(fp, "\t\tpublic function FromBuffer(pack:WorldPacket, count:uint):void\n");
	fprintf(fp, "\t\t{\n");
	fprintf(fp, "\t\t\tif(count <= m_%s_Arr.length)\n", pType->GetName());
	fprintf(fp, "\t\t\t{\n");
	fprintf(fp, "\t\t\t\tfor( var i:int = 0; i < count; i++ )\n");
	fprintf(fp, "\t\t\t\t{\n");
	fprintf(fp, "\t\t\t\t\tvar entry:%s = new %s;\n", pType->GetName(), pType->GetName());
	fprintf(fp, "\t\t\t\t\tentry.FromBuffer(pack);\n");
	fprintf(fp, "\t\t\t\t\tm_%s_Arr[i]=entry;\n", pType->GetName());
	fprintf(fp, "\t\t\t\t}\n");
	fprintf(fp, "\t\t\t}\n");
	fprintf(fp, "\t\t}\n");
	fprintf(fp, "\n");

	// function : ToBuffer
	fprintf(fp, "\t\tpublic function ToBuffer(pack:WorldPacket, count:uint):void\n");
	fprintf(fp, "\t\t{\n");
	fprintf(fp, "\t\t\tif(count < m_%s_Arr.length)\n", pType->GetName());
	fprintf(fp, "\t\t\t{\n");
	fprintf(fp, "\t\t\t\tfor( var i:int = 0; i < count; i++ )\n");
	fprintf(fp, "\t\t\t\t{\n");
	fprintf(fp, "\t\t\t\t\t%s(m_%s_Arr[i]).ToBuffer(pack);\n", pType->GetName(), pType->GetName());	
	fprintf(fp, "\t\t\t\t}\n");
	fprintf(fp, "\t\t\t}\n");
	fprintf(fp, "\t\t}\n");
	fprintf(fp, "\n");
	
	// function :GetAt
	fprintf(fp, "\t\tpublic function GetAt(idx:int):%s\n",pType->GetName());
	fprintf(fp, "\t\t{\n");
	fprintf(fp, "\t\t\treturn m_%s_Arr[idx];\n",pType->GetName());	
	fprintf(fp, "\t\t}\n");
	fprintf(fp, "\n");
	
	// function GetLength
	fprintf(fp, "\t\tpublic function GetLength():int\n");
	fprintf(fp, "\t\t{\n");
	fprintf(fp, "\t\t\treturn m_%s_Arr.length;\n",pType->GetName());	
	fprintf(fp, "\t\t}\n");
	fprintf(fp, "\n");

	// class end
	fprintf(fp, "\t}\n");
	fprintf(fp, "\t\n");
	fprintf(fp, "}\n");

	fclose(fp);
	return true;
}

void CASMaker::_WriteImportPart( FILE* fp )
{
	int nCount = m_poProto->GetImportCount();
	for (int i = 0; i < nCount; i++)
	{
		fprintf(fp, "\timport %s;\n", m_poProto->GetImportByPos(i));
	}
	fprintf(fp, "\n");
}

