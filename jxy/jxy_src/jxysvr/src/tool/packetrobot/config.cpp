#include "config.h"
#include <tinyxml.h>
#include <sdstring.h>
//#include <include/sdu/sdmemdefines.h>

// 服务器IP
CHAR g_acServerIP[32];
// 服务器端口
UINT32 g_dwServerPort;
// 心跳包频率
UINT32 g_dwTick;
// 报文链表
std::list<MsgInfo> g_lsMessages;


CHAR* TrimString(CHAR* _Input)
{
	CHAR* czRet = _Input;
	CHAR* szTemp = _Input;
	while(*szTemp == ' ')
	{
		if(*szTemp == '\0')
		{
			czRet = szTemp;
			return czRet;
		}
		szTemp++;
	}
	czRet = szTemp;
	INT32 i = strlen(szTemp);
	szTemp = szTemp + i - 1;
	while((*szTemp == ' ') || (10 == *szTemp ) || (13 == *szTemp))
	{
		*szTemp = '\0';
		szTemp--;
	}
	return czRet;
}

// 读取本地配置文件
BOOL LoadFromXML(CHAR* _Path)
{
	BOOL bResult = FALSE;
	TiXmlDocument* pDocument = SDNew TiXmlDocument;
	bResult = pDocument->LoadFile(_Path);
	if(!bResult)
	{
		SDDelete pDocument;
		pDocument = NULL;
		return FALSE;
	}
	TiXmlElement* pRootElement = pDocument->RootElement();
	if(!pRootElement)
	{
		SDDelete pDocument;
		pDocument = NULL;
		return FALSE;
	}
	TiXmlElement* poPropertyElement = pRootElement->FirstChildElement("server");
	if(NULL != poPropertyElement)
	{
		const CHAR* pszValue = NULL;
		pszValue = poPropertyElement->Attribute("ip");
		if(NULL == pszValue)
		{
			return FALSE;
		}
		else
		{
			SGDP::SDStrncpy(g_acServerIP, pszValue, 32);
		}

		pszValue = poPropertyElement->Attribute("port");
		if(NULL == pszValue)
		{
			return FALSE;
		}
		else
		{
			g_dwServerPort = SGDP::SDAtou(pszValue);
		}
	}
	poPropertyElement = pRootElement->FirstChildElement("heartbeat");
	if(NULL != poPropertyElement)
	{
		const CHAR* pszValue = NULL;
		pszValue = poPropertyElement->Attribute("tick");
		if(NULL == pszValue)
		{
			return FALSE;
		}
		else
		{
			g_dwTick = SGDP::SDAtou(pszValue);
		}
	}
	poPropertyElement = pRootElement->FirstChildElement("msg");
	while(NULL != poPropertyElement)
	{
		const CHAR* pszValue = NULL;
		MsgInfo msgInfo;
		pszValue = poPropertyElement->Attribute("caption");
		if(NULL == pszValue)
		{
			return FALSE;
		}
		else
		{
			SGDP::SDStrncpy(msgInfo.szCaption, pszValue, STRING_SIZE_DEFAULT);
		}
		pszValue = poPropertyElement->Attribute("describe");
		if(NULL == pszValue)
		{
			return FALSE;
		}
		else
		{
			SGDP::SDStrncpy(msgInfo.szDescribe, pszValue, STRING_SIZE_DEFAULT);
		}
		pszValue = poPropertyElement->Attribute("note");
		if(NULL == pszValue)
		{
			return FALSE;
		}
		else
		{
			SGDP::SDStrncpy(msgInfo.szNote, pszValue, STRING_SIZE_DEFAULT);
		}
		// 输入对象
		TiXmlElement* poInputElement = poPropertyElement->FirstChildElement("input");
		while(NULL != poInputElement)
		{
			InputInfo inputInfo;
			pszValue = poInputElement->Attribute("id");
			if(NULL == pszValue)
			{
				return FALSE;
			}
			else
			{
				inputInfo.nCtrlId = SGDP::SDAtou(pszValue);
			}
			pszValue = poInputElement->Attribute("caption");
			if(NULL == pszValue)
			{
				return FALSE;
			}
			else
			{
				SGDP::SDStrncpy(inputInfo.szCaption, pszValue, STRING_SIZE_DEFAULT);
			}
			pszValue = poInputElement->Attribute("value");
			if(NULL == pszValue)
			{
				SGDP::SDStrncpy(inputInfo.szValue, "", STRING_SIZE_DEFAULT);
			}
			else
			{
				SGDP::SDStrncpy(inputInfo.szValue, pszValue, STRING_SIZE_DEFAULT);
			}
			// 插入链表
			msgInfo.lsInputs.push_back(inputInfo);
			// 
			poInputElement = poInputElement->NextSiblingElement("input");
		}
		// 插入链表
		g_lsMessages.push_back(msgInfo);
		// 枚举下一个兄弟节点
		poPropertyElement = poPropertyElement->NextSiblingElement("msg");
	}

	SDDelete pDocument;
	pDocument = NULL;
	return TRUE;
}

