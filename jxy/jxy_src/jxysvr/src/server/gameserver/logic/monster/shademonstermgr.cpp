
#include "shademonstermgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <logic/base/logicfunc.h>
#include <logic/monster/shademonstergroup.h>
#include <logic/monster/monsterpropmgr.h>
IMPLEMENT_SINGLETON(CShadeMonsterMgr)


CShadeMonsterMgr::CShadeMonsterMgr()
{
	m_mapShadeMonster.clear();
}


CShadeMonsterMgr::~CShadeMonsterMgr()
{
	m_mapShadeMonster.clear();
}

BOOL CShadeMonsterMgr::Init()
{
	const CMonsterPropMap& mapProp = CMonsterPropMgr::Instance()->GetShadeMonsterProp();
	for ( CMonsterPropMap::const_iterator itr = mapProp.begin(); itr != mapProp.end(); itr++)
	{
		if (1 == itr->second.byCareerID)//进攻
		{
			if (EC_BLUE == itr->second.byType)
			{
				m_mapShadeMonsterByBlueAttr.insert(make_pair(itr->second.wLevel, &itr->second));
			}
			else if (EC_PURPLE == itr->second.byType)
			{
				m_mapShadeMonsterByPurpleAttr.insert(make_pair(itr->second.wLevel, &itr->second));
			}
			else if (EC_RED == itr->second.byType)
			{
				m_mapShadeMonsterByRedAttr.insert(make_pair(itr->second.wLevel, &itr->second));
			}
			else if (EC_ORANGE == itr->second.byType)
			{
				m_mapShadeMonsterByOrangeAttr.insert(make_pair(itr->second.wLevel, &itr->second));
			}
		}
		else//防守
		{
			if (EC_BLUE == itr->second.byType)
			{
				m_mapShadeMonsterByBlueDefend.insert(make_pair(itr->second.wLevel, &itr->second));
			}
			else if (EC_PURPLE == itr->second.byType)
			{
				m_mapShadeMonsterByPurpleDefend.insert(make_pair(itr->second.wLevel, &itr->second));
			}
			else if (EC_RED == itr->second.byType)
			{
				m_mapShadeMonsterByRedDefend.insert(make_pair(itr->second.wLevel, &itr->second));
			}
			else if (EC_ORANGE == itr->second.byType)
			{
				m_mapShadeMonsterByOrangeDefend.insert(make_pair(itr->second.wLevel, &itr->second));
			}
		}
		
	}

	if (0 == m_mapShadeMonsterByBlueAttr.size() || 0 == m_mapShadeMonsterByBlueDefend.size()
		|| 0 == m_mapShadeMonsterByPurpleAttr.size() || 0 == m_mapShadeMonsterByPurpleDefend.size()
		|| 0 == m_mapShadeMonsterByRedAttr.size() || 0 == m_mapShadeMonsterByRedDefend.size()
		|| 0 == m_mapShadeMonsterByOrangeAttr.size() || 0 == m_mapShadeMonsterByOrangeDefend.size())
	{
		return FALSE;
	}
	return TRUE;

}


VOID CShadeMonsterMgr::UnInit()
{
	RemoveAllShadeMonster();

}

CShadeMonster* CShadeMonsterMgr::CreateShadeMonster()
{
	return m_oShadeMonsterPool.Alloc();
}

VOID CShadeMonsterMgr::RemoveAllShadeMonster()
{

}

VOID CShadeMonsterMgr::RemoveShadeMonster(CShadeMonster* poMonster)
{
	if (NULL == poMonster)
	{
		m_oShadeMonsterPool.Free(poMonster);
	}
}
VOID CShadeMonsterMgr::RemoveShadeMonsterGroup(CShadeMonsterGroup* poMonsterGroup)
{
	if (NULL != poMonsterGroup)
	{
		m_oShadeMonsterGroupPool.Free(poMonsterGroup);
	}
}

CShadeMonsterGroup* CShadeMonsterMgr::CreateShadeMonsterGroup(UINT16 wLevel, UINT8 byType, const vector<UINT16>& vecMonsterID)
{
	CShadeMonsterGroup* poMonsterGroup = CreateShadeMonsterGroup();
	if( !poMonsterGroup->Init(wLevel, byType, vecMonsterID))
	{
		RemoveShadeMonsterGroup(poMonsterGroup);
		return NULL;
	}
	return poMonsterGroup;
}

CShadeMonsterGroup* CShadeMonsterMgr::CreateShadeMonsterGroup()
{
	return m_oShadeMonsterGroupPool.Alloc();
}
UINT8 CShadeMonsterMgr::GetShadeMonsterVec(UINT16 wLevel, UINT8 byType, CVecPShadeMonster& vecMonster)
{
	if ( wLevel == 0)
	{
		return 0;
	}
	//先获取一个进攻怪
	const SMonsterProp* poMonster = NULL;

	//再获取一个防守怪
	poMonster = GetShadeMonsterPropByDefendLevelType(wLevel, byType);
	if (NULL == poMonster )
	{
		return 0;
	}
	vecMonster.push_back(poMonster);

	poMonster = GetShadeMonsterPropByAttrLevelType(wLevel, byType);
	if (NULL == poMonster )
	{
		return 0;
	}
	vecMonster.push_back(poMonster);
	//根据等级品质再获取一个怪
	poMonster = GetShadeMonsterPropByLevelType(wLevel, byType);
	if (NULL == poMonster )
	{
		return 0;
	}
	vecMonster.push_back(poMonster);
	//以第三个确认阵型
	return poMonster->byCareerID;
}

const SMonsterProp* CShadeMonsterMgr::GetShadeMonsterPropByAttrLevelType(UINT16 wLevel, UINT8 byType)
{
	UINT8 byCount = 0;
	CMap32ShadeMonsterItr it;

	switch(byType)
	{
	case EC_BLUE:
		{
			byCount = m_mapShadeMonsterByBlueAttr.count(wLevel);
			while(0 == byCount && wLevel != 0)
			{
				wLevel -= 1;
				byCount = m_mapShadeMonsterByBlueAttr.count(wLevel);	
			}
			it = m_mapShadeMonsterByBlueAttr.find(wLevel);
			byCount = GetRdNum() % byCount;		
		}
		break;
	case EC_PURPLE:
		{
			byCount = m_mapShadeMonsterByPurpleAttr.count(wLevel);
			while(0 == byCount && wLevel != 0)
			{
				wLevel -= 1;
				byCount = m_mapShadeMonsterByPurpleAttr.count(wLevel);	
			}
			it = m_mapShadeMonsterByPurpleAttr.find(wLevel);
			byCount = GetRdNum() % byCount;
		}
		break;
	case EC_RED:
		{
			byCount = m_mapShadeMonsterByRedAttr.count(wLevel);
			while(0 == byCount && wLevel != 0)
			{
				wLevel -= 1;
				byCount = m_mapShadeMonsterByRedAttr.count(wLevel);	
			}
			it = m_mapShadeMonsterByRedAttr.find(wLevel);
			byCount = GetRdNum() % byCount;
		}
		break;
	case EC_ORANGE:
		{
			byCount = m_mapShadeMonsterByOrangeAttr.count(wLevel);
			while(0 == byCount && wLevel != 0)
			{
				wLevel -= 1;
				byCount = m_mapShadeMonsterByOrangeAttr.count(wLevel);	
			}
			it = m_mapShadeMonsterByOrangeAttr.find(wLevel);
			byCount = GetRdNum() % byCount;
		}
		break;
	default:
		return NULL;
	}
	for ( UINT8 byIndex = 0; byIndex < byCount - 1; byIndex++)
	{
		it++;
	}
	return it->second;
}

const SMonsterProp* CShadeMonsterMgr::GetShadeMonsterPropByDefendLevelType(UINT16 wLevel, UINT8 byType)
{
	UINT8 byCount = 0;
	CMap32ShadeMonsterItr it;
	//先获取一个进攻怪
	switch(byType)
	{
	case EC_BLUE:
		{
			byCount = m_mapShadeMonsterByBlueDefend.count(wLevel);
			while(0 == byCount && wLevel != 0)
			{
				wLevel -= 1;
				byCount = m_mapShadeMonsterByBlueDefend.count(wLevel);	
			}
			it = m_mapShadeMonsterByBlueDefend.find(wLevel);
			byCount = GetRdNum() % byCount;		
		}
		break;
	case EC_PURPLE:
		{
			byCount = m_mapShadeMonsterByPurpleDefend.count(wLevel);
			while(0 == byCount && wLevel != 0)
			{
				wLevel -= 1;
				byCount = m_mapShadeMonsterByPurpleDefend.count(wLevel);	
			}
			it = m_mapShadeMonsterByPurpleDefend.find(wLevel);
			byCount = GetRdNum() % byCount;
		}
		break;
	case EC_RED:
		{
			byCount = m_mapShadeMonsterByRedDefend.count(wLevel);
			while(0 == byCount && wLevel != 0)
			{
				wLevel -= 1;
				byCount = m_mapShadeMonsterByRedDefend.count(wLevel);	
			}
			it = m_mapShadeMonsterByRedDefend.find(wLevel);
			byCount = GetRdNum() % byCount;
		}
		break;
	case EC_ORANGE:
		{
			byCount = m_mapShadeMonsterByOrangeDefend.count(wLevel);
			while(0 == byCount && wLevel != 0)
			{
				wLevel -= 1;
				byCount = m_mapShadeMonsterByOrangeDefend.count(wLevel);	
			}
			it = m_mapShadeMonsterByOrangeDefend.find(wLevel);
			byCount = GetRdNum() % byCount;
		}
		break;
	default:
		return NULL;
	}
	for ( UINT8 byIndex = 0; byIndex < byCount - 1; byIndex++)
	{
		it++;
	}
	return it->second;
}

const SMonsterProp* CShadeMonsterMgr::GetShadeMonsterPropByLevelType(UINT16 wLevel, UINT8 byType)
{
	UINT8 byCount = 0;
	CMap32ShadeMonsterItr it;
	//先获取一个进攻怪
	switch(byType)
	{
	case EC_BLUE:
		{
			byCount = m_mapShadeMonsterByBlueDefend.count(wLevel);
			byCount += m_mapShadeMonsterByBlueAttr.count(wLevel);
			while(0 == byCount && wLevel != 0)
			{
				wLevel -= 1;
				byCount = m_mapShadeMonsterByBlueDefend.count(wLevel);
				byCount += m_mapShadeMonsterByBlueAttr.count(wLevel);
			}
			if (0 == byCount )
			{
				return NULL;
			}
			byCount = GetRdNum() % byCount;
			it = m_mapShadeMonsterByBlueDefend.find(wLevel);
			UINT8 byIndex = 0;
			for ( ; (byIndex < byCount - 1) && it != m_mapShadeMonsterByBlueDefend.end(); byIndex++)
			{
				it++;
			}
			it = m_mapShadeMonsterByBlueAttr.find(wLevel);
			for ( ; (byIndex < byCount - 1) && it != m_mapShadeMonsterByBlueAttr.end(); byIndex++)
			{
				it++;
			}		
		}
		break;
	case EC_PURPLE:
		{
			byCount = m_mapShadeMonsterByPurpleDefend.count(wLevel);
			byCount += m_mapShadeMonsterByPurpleAttr.count(wLevel);
			while(0 == byCount && wLevel != 0)
			{
				wLevel -= 1;
				byCount = m_mapShadeMonsterByPurpleDefend.count(wLevel);
				byCount += m_mapShadeMonsterByPurpleAttr.count(wLevel);
			}
			if (0 == byCount )
			{
				return NULL;
			}
			byCount = GetRdNum() % byCount;
			it = m_mapShadeMonsterByPurpleDefend.find(wLevel);
			UINT8 byIndex = 0;
			for ( ; (byIndex < byCount - 1) && it != m_mapShadeMonsterByPurpleDefend.end(); byIndex++)
			{
				it++;
			}
			it = m_mapShadeMonsterByPurpleAttr.find(wLevel);
			for ( ; (byIndex < byCount - 1) && it != m_mapShadeMonsterByPurpleAttr.end(); byIndex++)
			{
				it++;
			}
		}
		break;
	case EC_RED:
		{
			byCount = m_mapShadeMonsterByRedDefend.count(wLevel);
			byCount += m_mapShadeMonsterByRedAttr.count(wLevel);
			while(0 == byCount && wLevel != 0)
			{
				wLevel -= 1;
				byCount = m_mapShadeMonsterByRedDefend.count(wLevel);
				byCount += m_mapShadeMonsterByRedAttr.count(wLevel);
			}
			if (0 == byCount )
			{
				return NULL;
			}
			byCount = GetRdNum() % byCount;
			it = m_mapShadeMonsterByRedDefend.find(wLevel);
			UINT8 byIndex = 0;
			for ( ; (byIndex < byCount - 1) && it != m_mapShadeMonsterByRedDefend.end(); byIndex++)
			{
				it++;
			}
			it = m_mapShadeMonsterByRedAttr.find(wLevel);
			for ( ; (byIndex < byCount - 1) && it != m_mapShadeMonsterByRedAttr.end(); byIndex++)
			{
				it++;
			}
		}
		break;
	case EC_ORANGE:
		{
			byCount = m_mapShadeMonsterByOrangeDefend.count(wLevel);
			byCount += m_mapShadeMonsterByOrangeAttr.count(wLevel);
			while(0 == byCount && wLevel != 0)
			{
				wLevel -= 1;
				byCount = m_mapShadeMonsterByOrangeDefend.count(wLevel);
				byCount += m_mapShadeMonsterByOrangeAttr.count(wLevel);
			}
			if (0 == byCount )
			{
				return NULL;
			}
			byCount = GetRdNum() % byCount;
			it = m_mapShadeMonsterByOrangeDefend.find(wLevel);
			UINT8 byIndex = 0;
			for ( ; (byIndex < byCount - 1) && it != m_mapShadeMonsterByOrangeDefend.end(); byIndex++)
			{
				it++;
			}
			it = m_mapShadeMonsterByOrangeAttr.find(wLevel);
			for ( ; (byIndex < byCount - 1) && it != m_mapShadeMonsterByOrangeAttr.end(); byIndex++)
			{
				it++;
			}
		}
		break;
	default:
		return NULL;
	}
	return it->second;
}

UINT32 CShadeMonsterMgr::GetPowerByProp(const SMonsterProp* poProp)
{
	if (NULL == poProp)
	{
		return 0;
	}
	CShadeMonster* poShadeMonster = CreateShadeMonster();
	if (NULL == poShadeMonster)
	{
		return 0;
	}
	poShadeMonster->Init(0, 0, poProp);
	return poShadeMonster->GetPower();
}