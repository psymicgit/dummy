
#include "playergroupmgr.h"

#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <logic/base/logicfunc.h>




IMPLEMENT_SINGLETON(CPlayerGroupMgr)


CPlayerGroupMgr::CPlayerGroupMgr()
{

}


CPlayerGroupMgr::~CPlayerGroupMgr()
{

}

BOOL CPlayerGroupMgr::Init()
{
    //∂¡»°≈‰÷√Œƒº˛
    return TRUE;
}


VOID CPlayerGroupMgr::UnInit()
{

}



CPlayerGroup* CPlayerGroupMgr::CreatePlayerGroup(vector<CHero*> vecHero)
{
    CPlayerGroup* poPlayerGroup = m_oPlayerGroupPool.Alloc();
    if(NULL == poPlayerGroup)
    {
        return NULL;
    }
	poPlayerGroup->Init(vecHero);
    return poPlayerGroup;
}


VOID CPlayerGroupMgr::RemovePlayerGroup(CPlayerGroup* poPlayerGroup)
{
    if(poPlayerGroup)
    {
        m_oPlayerGroupPool.Free(poPlayerGroup);
		poPlayerGroup = NULL;
    }
}

