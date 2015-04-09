
#include "monstermgr.h"
#include "monsterupgradepropmgr.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <logic/instance/instancepropmgr.h>
#include <logic/base/logicfunc.h>

UINT32	CMonsterMgr::m_dwMonsterIDGen = 0;
UINT32	CMonsterMgr::m_dwMonsterGroupIDGen = 0;


IMPLEMENT_SINGLETON(CMonsterMgr)


CMonsterMgr::CMonsterMgr()
{

}


CMonsterMgr::~CMonsterMgr()
{
    for(CID2MonsterGroupMapItr itr = m_mapID2MonsterGroup.begin(); itr != m_mapID2MonsterGroup.end(); itr++)
    {
        m_oMonsterGroupPool.Free(itr->second);
    }
    m_mapID2MonsterGroup.clear();
    m_mapInstanceID2MonsterGroup.clear();

    CMonsterPropMgr::DestroyInstance();
    CMonsterUpgradePropMgr::DestroyInstance();
}

BOOL CMonsterMgr::Init()
{
    //读取配置文件

    // 初始化CMonsterPropMgr管理器
    if(FALSE == CMonsterPropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CMonsterPropMgr::CreateMonster failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CMonsterPropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CMonsterPropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    // 初始化CMonsterUpgradePropMgr管理器
    if(FALSE == CMonsterUpgradePropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CMonsterUpgradePropMgr::CreateMonster failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CMonsterUpgradePropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CMonsterUpgradePropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;

}


VOID CMonsterMgr::UnInit()
{
    RemoveAllMonster();

    //////////////////////////////

    CMonsterPropMgr::Instance()->UnInit();
    CMonsterUpgradePropMgr::Instance()->UnInit();

}

CMonsterGroup*  CMonsterMgr::FindMonsterGroup(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT16 wMonsterIdx)
{
    UINT64 qwMonsterKey = GetMonsterKey(wSceneIdx, byTownIdx, byInstanceIdx, wMonsterIdx);
    CID2MonsterGroupMapItr itr = m_mapID2MonsterGroup.find(qwMonsterKey);
    if(itr != m_mapID2MonsterGroup.end())
    {
        return itr->second;
    }

    if(CreateMonsterGroup(wSceneIdx, byTownIdx, byInstanceIdx))
    {
        itr = m_mapID2MonsterGroup.find(qwMonsterKey);
        if(itr != m_mapID2MonsterGroup.end())
        {
            return itr->second;
        }
    }

    return NULL;
}

CMonsterGroup*		CMonsterMgr::FindMonsterGroup(vector<SSingleMonsterProp> vecMonster)
{
    //根据传递进来的怪物ID组成一个唯一的KEY，最多只支持三只怪物
    UINT64 qwMonsterKey = GetMonsterKey(vecMonster);
    CID2MonsterGroupMapItr itr = m_mapID2MonsterGroup.find(qwMonsterKey);
    if(itr != m_mapID2MonsterGroup.end())
    {
        return itr->second;
    }

    if(CreateMonsterGroup(vecMonster))
    {
        itr = m_mapID2MonsterGroup.find(qwMonsterKey);
        if(itr != m_mapID2MonsterGroup.end())
        {
            return itr->second;
        }
    }

    return NULL;
}


CMonster* CMonsterMgr::CreateMonster()
{
    CMonster* poMonster = m_oMonsterPool.Alloc();
    if(NULL == poMonster)
    {
        return NULL;
    }
    UINT32 dwMonsterID = ++m_dwMonsterIDGen;
    poMonster->SetMemID(dwMonsterID);

    return poMonster;
}

BOOL CMonsterMgr::CreateMonsterGroup(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx)
{
    //已产生，不需再次产生
    UINT64	qwkey = GetInstanceKey(wSceneIdx, byTownIdx, byInstanceIdx);
    CInstanceID2MonsterGroupMapItr itr = m_mapInstanceID2MonsterGroup.find(qwkey);
    if( itr != m_mapInstanceID2MonsterGroup.end())
    {
        return TRUE;
    }

    //查找副本相应的怪物配置
    SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(wSceneIdx, byTownIdx, byInstanceIdx);
    if(NULL == pstInstanceProp)
    {
        return NULL;
    }

    DT_MONSTER_DATA_LIST stDT_MONSTER_DATA_LIST;
    memset(&stDT_MONSTER_DATA_LIST, 0, sizeof(DT_MONSTER_DATA_LIST));
    UINT16 wSize = pstInstanceProp->vecWholeMonsterProp.size();
    //wIdx,怪物序号从1开始
    for(UINT16 wIdx = 1; wIdx <= wSize; wIdx++)
    {
        if(!CreateMonsterGroup(wSceneIdx, byTownIdx, byInstanceIdx, wIdx, stDT_MONSTER_DATA_LIST))
        {
            //log
            return NULL;
        }
    }
    //至少保证能有1波怪
    if(stDT_MONSTER_DATA_LIST.wMonsterNum > 0)
    {
        m_mapInstanceID2MonsterGroup[qwkey] = stDT_MONSTER_DATA_LIST;
        return TRUE;
    }
    else
    {
        return FALSE;
    }

    return TRUE;

}

CMonsterGroup* CMonsterMgr::CreateMonsterGroup(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT16 wMonsterIdx, DT_MONSTER_DATA_LIST& stDT_MONSTER_DATA_LIST)
{
    UINT64 qwMonsterKey = GetMonsterKey(wSceneIdx, byTownIdx, byInstanceIdx, wMonsterIdx);
    CID2MonsterGroupMapItr itr = m_mapID2MonsterGroup.find(qwMonsterKey);
    if(itr != m_mapID2MonsterGroup.end())
    {
        return itr->second;
    }
    CMonsterGroup* poMonsterGroup = m_oMonsterGroupPool.Alloc();
    if(NULL == poMonsterGroup)
    {
        return NULL;
    }
    if(FALSE == poMonsterGroup->Init(wSceneIdx, byTownIdx, byInstanceIdx, wMonsterIdx))
    {
        m_oMonsterGroupPool.Free(poMonsterGroup);
        return NULL;
    }

    UINT32 dwMonsterGroupID = ++m_dwMonsterGroupIDGen;
    poMonsterGroup->SetMemID(dwMonsterGroupID);

    m_mapID2MonsterGroup[GetMonsterKey(wSceneIdx, byTownIdx, byInstanceIdx, wMonsterIdx)] = poMonsterGroup;

    DT_MONSTER_BASE_DATA& stDT_MONSTER_BASE_DATA = stDT_MONSTER_DATA_LIST.astMonsterInfListo[stDT_MONSTER_DATA_LIST.wMonsterNum++];
    stDT_MONSTER_BASE_DATA.wKindID = poMonsterGroup->GetMainMonster()->GetMonsterProp()->wKindID;
    stDT_MONSTER_BASE_DATA.wLevel = poMonsterGroup->GetMainMonster()->GetMonsterUpgradeProp()->wLevel;

    return poMonsterGroup;

}

//按怪物ID创建怪物组
CMonsterGroup*	CMonsterMgr::CreateMonsterGroup(vector<SSingleMonsterProp> vecMonster)
{
    //根据传递进来的怪物ID组成一个唯一的KEY，最多只支持三只怪物
    UINT64 qwMonsterKey = GetMonsterKey(vecMonster);
    CID2MonsterGroupMapItr itr = m_mapID2MonsterGroup.find(qwMonsterKey);
    if(itr != m_mapID2MonsterGroup.end())
    {
        return itr->second;
    }
    CMonsterGroup* poMonsterGroup = m_oMonsterGroupPool.Alloc();
    if(NULL == poMonsterGroup)
    {
        return NULL;
    }

    if(FALSE == poMonsterGroup->Init(vecMonster))
    {
        m_oMonsterGroupPool.Free(poMonsterGroup);
        return NULL;
    }

    UINT32 dwMonsterGroupID = ++m_dwMonsterGroupIDGen;
    poMonsterGroup->SetMemID(dwMonsterGroupID);
    m_mapID2MonsterGroup[qwMonsterKey] = poMonsterGroup;
    return poMonsterGroup;
}

VOID CMonsterMgr::RemoveMonster(CMonster* poMonster)
{
    if(poMonster)
    {
        m_oMonsterPool.Free(poMonster);
    }
}

VOID CMonsterMgr::RemoveMonsterGroup(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT16 wMonsterIdx)
{
    CID2MonsterGroupMapItr itr = m_mapID2MonsterGroup.find(GetMonsterKey(wSceneIdx, byTownIdx, byInstanceIdx, wMonsterIdx));
    if (m_mapID2MonsterGroup.end() != itr)
    {
        CMonsterGroup* poMonsterGroup = itr->second;
        m_oMonsterGroupPool.Free(poMonsterGroup);
        m_mapID2MonsterGroup.erase(itr);
    }
}



UINT64 CMonsterMgr::GetMonsterKey(UINT16 wSceneIdx, UINT16 wTownIdx, UINT16 wInstanceIdx, UINT16 wMonsterIdx)
{
    UINT64 qwKey = GetInstanceKey(wSceneIdx, wTownIdx, wInstanceIdx);
    qwKey	=	qwKey << 16;
    qwKey	+=	wMonsterIdx;
    return qwKey;
}

UINT64 CMonsterMgr::GetShadeMonsterKey( UINT8 byType, UINT32 dwPlayerID)
{
	UINT64 qwKey = 0;
	qwKey += byType;
	qwKey = qwKey << 32;
	qwKey += dwPlayerID;
	return qwKey;
}
UINT64 CMonsterMgr::GetMonsterKey(vector<SSingleMonsterProp> vecMonster)
{
    UINT16 wKindID1 = 0;
    UINT16 wKindID2 = 0;
    UINT16 wKindID3 = 0;
    for (UINT32 i = 0; i < vecMonster.size(); i++)
    {
        if (0 == i)
        {
            wKindID1 = vecMonster[i].wMonsterID;
        }
        else if (1 == i)
        {
            wKindID2 = vecMonster[i].wMonsterID;
        }
        else if (2 == i)
        {
            wKindID3 = vecMonster[i].wMonsterID;
        }
    }
    return GetMonsterKey(wKindID1, wKindID2, wKindID3, 0);
}



UINT64 CMonsterMgr::GetInstanceKey(UINT16 wSceneIdx, UINT16 wTownIdx, UINT16 wInstanceIdx)
{
    return CLogicFunc::Get161616Key(wSceneIdx, wTownIdx, wInstanceIdx);
}

DT_MONSTER_DATA_LIST* CMonsterMgr::GetInstanceMonster(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx)
{
    CInstanceID2MonsterGroupMapItr itr = m_mapInstanceID2MonsterGroup.find(GetInstanceKey(wSceneIdx, byTownIdx, byInstanceIdx));
    if( itr != m_mapInstanceID2MonsterGroup.end())
    {
        return &(itr->second);
    }

    if(CreateMonsterGroup(wSceneIdx, byTownIdx, byInstanceIdx))
    {
        itr = m_mapInstanceID2MonsterGroup.find(GetInstanceKey(wSceneIdx, byTownIdx, byInstanceIdx));
        if( itr != m_mapInstanceID2MonsterGroup.end())
        {
            return &(itr->second);
        }
    }

    return NULL;
}

VOID CMonsterMgr::RemoveAllMonster()
{
    for(CID2MonsterGroupMapItr itrGroup = m_mapID2MonsterGroup.begin(); itrGroup != m_mapID2MonsterGroup.end(); itrGroup++)
    {
        CMonsterGroup* poMonsterGroup = itrGroup->second;
        m_oMonsterGroupPool.Free(poMonsterGroup);
        poMonsterGroup = NULL;

    }
    m_mapID2MonsterGroup.clear();
    m_mapInstanceID2MonsterGroup.clear();
}

