#include "burstprop.h"


SBurst::~SBurst()
{
	for(vector<SProp*>::iterator itr = vecProp.begin(); itr != vecProp.end(); itr++)
	{
		SDDelete *itr;
		*itr = NULL;
	}
	vecProp.clear();
}


EBattleAttributeKind GetBattleAttrKindByStr(string strAttr)
{
	static CStr2BattleAttrKindMap	m_mapStr2BattleAttrKindMap;
	static BOOL bInit = FALSE;
	if(FALSE == bInit)
	{
		m_mapStr2BattleAttrKindMap["HP"] = EBAK_HP;
		m_mapStr2BattleAttrKindMap["Attack"] = EBAK_ATTACK;

		bInit = TRUE;
	}

	CStr2BattleAttrKindMapItr itr = m_mapStr2BattleAttrKindMap.find(strAttr.c_str());
	if(itr != m_mapStr2BattleAttrKindMap.end())
	{
		return itr->second;
	}
	return EBAK_INVALID;
}



BOOL ReadRandomBurst(EBurstKind eBurstKind, SBurst* pstBurst, CSDXMLNode xmlNode)
{
	SDTRY_BEGIN
	if(xmlNode == NULL)
	{
		return FALSE;
	}

	pstBurst->eBurstKind = eBurstKind;
	pstBurst->bRandom = TRUE;

	CSDXMLNode xmlNodeProp = xmlNode["Prop"];
	while(xmlNodeProp != NULL)
	{
		SProp* pstProp = new SProp();
		switch(eBurstKind)
		{
		case EBKIND_COIN:
		case EBKIND_GOLD:
		case EBKIND_EXPERIENCE:
		case EBKIND_PHYSTRENGTH:
		case EBKIND_SCIENCE:
			{
				pstProp->wMin = xmlNodeProp("Min").AsInteger();
				pstProp->dwMax = xmlNodeProp("Max").AsInteger();		
				if(pstProp->wMin > pstProp->wMin)
				{
					pstProp->wMin = pstProp->dwMax;
				}
			}
			break;
		case EBKIND_ITEM:
			{
				pstProp->wKindID = xmlNodeProp("KindID").AsInteger();
				pstProp->wNum = xmlNodeProp("Num").AsInteger();
			}
			break;
		case EBKIND_ATTRIBUTE:
			{
				pstProp->wKindID = GetBattleAttrKindByStr(xmlNodeProp("KindID").AsString());
				if(EBAK_INVALID == pstProp->wKindID)
				{
					SDDelete pstProp;
					pstProp = NULL;
					return FALSE;
				}
				pstProp->nNum = xmlNodeProp("Num").AsInteger();
			}
			break;
		}
		pstProp->wRate = xmlNodeProp("Rate").AsInteger();
		pstProp->wRate = pstProp->wRate > 100 ? 100 : pstProp->wRate;
		UINT8 byFirstFalg = xmlNodeProp("FirstFalg").AsInteger();
		if(1 == byFirstFalg)
		{
			pstProp->bFirstFalg = TRUE;
		}
		pstBurst->vecProp.push_back(pstProp);

		xmlNodeProp = xmlNodeProp.Sibling("Prop");
	}
	return TRUE;
	SDTRY_END
		return FALSE;
}

SEncourageProp::~SEncourageProp()
{
	CBurstVecItr itr = vecBurst.begin();
	for(itr = vecBurst.begin(); itr != vecBurst.end(); itr++)
	{
		SDDelete *itr;
		 *itr = NULL;
	}
	vecBurst.clear();

	for(itr = vecBurstItemGroup.begin(); itr != vecBurstItemGroup.end(); itr++)
	{
		SDDelete *itr;
		*itr = NULL;
	}
	vecBurstItemGroup.clear();
}


BOOL ReadEncourageItemGroup(CBurstVec& vecBurst, CSDXMLNode xmlNode)
{
	if(xmlNode == NULL)
	{
		return TRUE;
	}
	CHAR* pszReadItemName = "Group";
	CSDXMLNode xmlNodeProp = xmlNode[pszReadItemName];
	while(xmlNodeProp != NULL)
	{
		SBurst* pstBurst = SDNew SBurst();		
		pstBurst->eBurstKind = EBKIND_ITEM;

		UINT16 wKindID = xmlNodeProp("KindID").AsInteger();
		UINT32 nNum = xmlNodeProp("Num").AsInteger();
		if(0 != wKindID)
		{
			pstBurst->bRandom = FALSE;			
			SProp* pstProp = SDNew SProp();
			pstProp->wKindID = wKindID;
			pstProp->nNum = nNum;
			pstBurst->vecProp.push_back(pstProp);
			vecBurst.push_back(pstBurst);
			xmlNodeProp = xmlNodeProp.Sibling(pszReadItemName);
			continue;
		}

		if(!ReadRandomBurst(EBKIND_ITEM,pstBurst, xmlNodeProp))
		{
			SDDelete pstBurst;
			pstBurst = NULL;
			return FALSE;
		}
		vecBurst.push_back(pstBurst);

		xmlNodeProp = xmlNodeProp.Sibling(pszReadItemName);
	}

	return TRUE;
}


BOOL ReadBurstItem(EBurstKind eBurstKind, CBurstVec& vecBurst, CSDXMLNode xmlNode)
{
	if(xmlNode == NULL)
	{
		return TRUE;
	}
	CHAR* pszReadItemName = NULL;
	switch(eBurstKind)
	{
	case EBKIND_COIN:
		pszReadItemName = "Coin";
		break;
	case EBKIND_GOLD:
		pszReadItemName = "Gold";
		break;
	case EBKIND_EXPERIENCE:
		pszReadItemName = "Experience";
		break;
	case EBKIND_PHYSTRENGTH:
		pszReadItemName = "PhyStrength";
		break;	
	case EBKIND_ITEM:
		pszReadItemName = "Item";
		break;
	case EBKIND_ATTRIBUTE:
		pszReadItemName = "Attribute";
		break;
	case EBKIND_SCIENCE:
		pszReadItemName = "Science";
		break;
	default:
		return FALSE;		
	}

	CSDXMLNode xmlNodeProp = xmlNode[pszReadItemName];
	while(xmlNodeProp != NULL)
	{
		SBurst* pstBurst = SDNew SBurst();
		pstBurst->eBurstKind = eBurstKind;

		switch(eBurstKind)
		{
		case EBKIND_COIN:
		case EBKIND_GOLD:
		case EBKIND_EXPERIENCE:
		case EBKIND_PHYSTRENGTH:
		case EBKIND_SCIENCE:
			{
				UINT32 dwValue = xmlNodeProp("Value").AsInteger();
				if(0 != dwValue)
				{			
					pstBurst->bRandom = FALSE;
					SProp* pstProp = SDNew SProp();
					pstProp->dwMax = dwValue;
					pstBurst->vecProp.push_back(pstProp);
					vecBurst.push_back(pstBurst);
					xmlNodeProp = xmlNodeProp.Sibling(pszReadItemName);
					continue;
				}
			}
			break;
		case EBKIND_ITEM:
			{
				UINT16 wKindID = xmlNodeProp("KindID").AsInteger();
				INT32 nNum = xmlNodeProp("Num").AsInteger();
				if((0 != wKindID) && (0 != nNum))
				{			
					pstBurst->bRandom = FALSE;
					SProp* pstProp = SDNew SProp();
					pstProp->wKindID = eBurstKind;
					pstProp->nNum = nNum;
					pstBurst->vecProp.push_back(pstProp);
					vecBurst.push_back(pstBurst);
					xmlNodeProp = xmlNodeProp.Sibling(pszReadItemName);
					continue;
				}
			}
			break;
		case EBKIND_ATTRIBUTE:
			{
				UINT16 wKindID = GetBattleAttrKindByStr(xmlNodeProp("KindID").AsString());
				UINT32 nNum = xmlNodeProp("Num").AsInteger();
				if((0 != wKindID) && (0 != nNum))
				{			
					pstBurst->bRandom = FALSE;
					SProp* pstProp = SDNew SProp();
					pstProp->wKindID = eBurstKind;
					pstProp->nNum = nNum;
					pstBurst->vecProp.push_back(pstProp);
					vecBurst.push_back(pstBurst);
					xmlNodeProp = xmlNodeProp.Sibling(pszReadItemName);
					continue;
				}
			}
			break;	
		}
		if(!ReadRandomBurst(eBurstKind,pstBurst, xmlNodeProp))
		{
			SDDelete pstBurst;
			pstBurst = NULL;
			return FALSE;
		}
		vecBurst.push_back(pstBurst);

		xmlNodeProp = xmlNodeProp.Sibling(pszReadItemName);
	}

	return TRUE;
}


//读取奖励
BOOL ReadEncourage(SEncourageProp* pstEncourageProp, CSDXMLNode xmlNode)
{
	if(xmlNode == NULL)
	{
		return FALSE;
	}

	//钱币
	if(!ReadBurstItem(EBKIND_COIN, pstEncourageProp->vecBurst, xmlNode))
	{
		return FALSE;
	}
	//元宝
	if(!ReadBurstItem(EBKIND_GOLD, pstEncourageProp->vecBurst, xmlNode))
	{
		return FALSE;
	}
	//经验
	if(!ReadBurstItem(EBKIND_EXPERIENCE, pstEncourageProp->vecBurst, xmlNode))
	{
		return FALSE;
	}
	//体力
	if(!ReadBurstItem(EBKIND_PHYSTRENGTH, pstEncourageProp->vecBurst, xmlNode))
	{
		return FALSE;
	}
	//科技点
	if(!ReadBurstItem(EBKIND_SCIENCE, pstEncourageProp->vecBurst, xmlNode))
	{
		return FALSE;
	}
	
	//道具
	CSDXMLNode xmlNodeProp = xmlNode["Item"];	
	if(!ReadEncourageItemGroup(pstEncourageProp->vecBurstItemGroup, xmlNode["Item"]))
	{
		return FALSE;
	}

	return TRUE;
}
