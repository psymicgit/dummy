#include "sciencetree.h"

#include <common/client/errdef.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include "logic/hero/hero.h"
#include "logic/science/scienceupdatepropmgr.h"
#include "logic/task/taskmgr.h"


IMPLEMENT_CLASS_POOL_MGR(CScienceTree);
CScienceTree::CScienceTree()
{
    memset(&m_stDT_SCIENCE_TREE_DATA, 0, sizeof(m_stDT_SCIENCE_TREE_DATA));
}

CScienceTree::~CScienceTree()
{

}

BOOL CScienceTree::Init(DT_SCIENCE_TREE_DATA &stDT_SCIENCE_TREE_DATA, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    m_poOwner = poOwner;
    memcpy(&m_stDT_SCIENCE_TREE_DATA, &stDT_SCIENCE_TREE_DATA, sizeof(DT_SCIENCE_TREE_DATA));
    return TRUE;
}


UINT16 CScienceTree::GetScienceTreeBuildInfo( UINT64 &qwCurStory, UINT8 &byAlreadyOpenScienceNum, DT_SCIENCE_ALREADY_OPEN_DATA astAlreadyOpenScienceList[MAX_SCIENCE_KIND_NUM],
        UINT8 &byWillOpenNum, DT_SCIENCE_WILL_OPEN_DATA astWillOpenScienceList[MAX_SCIENCE_KIND_NUM] )
{

    if( NULL == m_poOwner->GetCoachHero() )
    {
        return ERR_OPEN_SCIENCE::ID_SERVER_INNER_ERR;
    }

    qwCurStory = m_poOwner->GetDT_PLAYER_BASE_DATA().qwStory;

    UINT16 wSelfLevel = m_poOwner->GetCoachHero()->GetDT_HERO_BASE_DATA().wLevel;

    vector<UINT8> vecScienceKindList = CScienceUpdatePropMgr::Instance()->GetSortScienceKindList();
    //获得科技列表
    for( UINT32 nKindIndex = 0; nKindIndex < vecScienceKindList.size(); ++nKindIndex )
    {
        BOOL bUnlock = FALSE;
        BOOL bVisible = FALSE;
        UINT16 wScienceLevel = 0;

        if( !GetScienceLevel( vecScienceKindList[nKindIndex], wScienceLevel) )
        {
            SYS_CRITICAL(_SDT("[%s: %d]:GetScienceLevel failed, KindIdx:%d"), MSG_MARK, vecScienceKindList[nKindIndex] );
            continue;
        }
        INT32 nRet = CScienceUpdatePropMgr::Instance()->GetUnlockInfo( vecScienceKindList[nKindIndex], m_poOwner->GetLevel(), bVisible, bUnlock );
        if( CScienceUpdatePropMgr::GUIE_SUCCESS != nRet )
        {
            SYS_CRITICAL(_SDT("[%s: %d]:Get Science[%d] UnlockInfo failed:%d"), MSG_MARK, vecScienceKindList[nKindIndex], nRet );
            continue;
        }

        UINT32 dwCostValue = 0;
        UINT32 dwAddValue = 0;
        if( bUnlock )
        {
            astAlreadyOpenScienceList[byAlreadyOpenScienceNum].byKind = vecScienceKindList[nKindIndex];
            astAlreadyOpenScienceList[byAlreadyOpenScienceNum].wLevel = wScienceLevel;

            if( !GetScienceValue( vecScienceKindList[nKindIndex], astAlreadyOpenScienceList[byAlreadyOpenScienceNum].wLevel,
                                  astAlreadyOpenScienceList[byAlreadyOpenScienceNum].dwCostStory,
                                  astAlreadyOpenScienceList[byAlreadyOpenScienceNum].dwCurValue,
                                  astAlreadyOpenScienceList[byAlreadyOpenScienceNum].dwNextValue ) )
            {
                DBG_INFO(_SDT("[%s: %d]:type:%d inf error,error code:%d"), MSG_MARK, vecScienceKindList[nKindIndex], nRet );
                continue;
            }

            astAlreadyOpenScienceList[byAlreadyOpenScienceNum].wNeedPlayerLevel =
                CScienceUpdatePropMgr::Instance()->GetUpdateNeedLevel( vecScienceKindList[nKindIndex], wScienceLevel + 1 );

            UINT16 wMaxLevel = CScienceUpdatePropMgr::Instance()->GetMaxLevel( vecScienceKindList[nKindIndex] );
            astAlreadyOpenScienceList[byAlreadyOpenScienceNum].byMaxLevel = wMaxLevel == wScienceLevel;

            ++byAlreadyOpenScienceNum;
        }
        else if( bVisible )
        {
            if( astWillOpenScienceList == NULL )
            {
                continue;
            }
            const static INT32 s_nDefaultViewLevel = 1;
            UINT16 wUnlockLevel = 0;
            nRet = CScienceUpdatePropMgr::Instance()->GetInfo( vecScienceKindList[nKindIndex], s_nDefaultViewLevel, dwCostValue, dwAddValue, wUnlockLevel );
            if( CScienceUpdatePropMgr::GUIE_SUCCESS != nRet )
            {
                SYS_CRITICAL(_SDT("[%s: %d]:get science GetInfo failed[%d], kind:%d,Level:%d"), MSG_MARK, nRet, vecScienceKindList[nKindIndex], s_nDefaultViewLevel);
                continue;
            }
            astWillOpenScienceList[byWillOpenNum].byKind = vecScienceKindList[nKindIndex];
            astWillOpenScienceList[byWillOpenNum].dwCostStory = dwCostValue;
            astWillOpenScienceList[byWillOpenNum].dwLv1Value = dwAddValue;
            astWillOpenScienceList[byWillOpenNum].wOpenLevel = wUnlockLevel;
            ++byWillOpenNum;
        }
        else
        {
            //什么都不做
        }
    }

    //获得将要升级的科技列表
    return ERR_OPEN_SCIENCE::ID_SUCCESS;
}

INT32 CScienceTree::UpdateScience( UINT8 byKind, UINT16 wLevel, DT_SCIENCE_ALREADY_OPEN_DATA &stUpdatedData,
                                   DT_POWER_CHANGE_DATA &stPower, UINT64 &qwCurStory )
{
    //如果科技建筑本身未解锁，则不能升级
    if( NULL == m_poOwner->GetBuild( EBK_SCIENCE))
    {
        return ERR_UPGRADE_SCIENCE_ATTREXT::ID_ATTR_NOT_UNLOCK_ERR;
    }

    //判断是否能升级
    UINT32 dwCostValue = 0;
    UINT32 dwAddValue = 0;

    UINT16 wActualLevel = 0;
    qwCurStory = m_poOwner->GetDT_PLAYER_BASE_DATA().qwStory;
    if( !GetScienceLevel( byKind, wActualLevel ) )
    {
        SYS_CRITICAL(_SDT("[%s: %d]:GetScienceLevel faield, kind:%d"), MSG_MARK, byKind );
        return ERR_UPGRADE_SCIENCE_ATTREXT::ID_SCIENCE_NOT_EXIST_ERR;
    }

    INT32 nRet = CScienceUpdatePropMgr::Instance()->GetInfo( byKind, wActualLevel + 1, dwCostValue, dwAddValue );
    // 	if(CGuideMgr::Instance()->CKGuide(GetDT_PLAYER_BASE_DATA().qwGuideRecord, EGF_SCIENCETREE))
    // 	{
    // 		wCostValue = 0; //引导消耗0
    // 	}
    if( CScienceUpdatePropMgr::GUIE_SUCCESS != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:get science unlock info failed, kind:%d,Level:%d, ret:%d"), MSG_MARK, byKind, wLevel, nRet );
        if( CScienceUpdatePropMgr::GUIE_AT_MAXMIUM_LEVEL == nRet )
        {
            return ERR_UPGRADE_SCIENCE_ATTREXT::ID_REACH_MAX_LEVEL_ERR;
        }
        else if( CScienceUpdatePropMgr::GUIE_CANNOT_FIND_SCIENCE == nRet )
        {
            return ERR_UPGRADE_SCIENCE_ATTREXT::ID_SCIENCE_NOT_EXIST_ERR;
        }
        else
        {
            return ERR_UPGRADE_SCIENCE_ATTREXT::ID_SERVER_INNER_ERR;
        }
    }

    if( m_poOwner->GetDT_PLAYER_BASE_DATA().qwStory < dwCostValue )
    {
        return ERR_UPGRADE_SCIENCE_ATTREXT::ID_NOT_ENOUGH_SCIENCE_ERR;
    }

    if( m_poOwner->GetLevel() <= CScienceUpdatePropMgr::Instance()->GetUpdateNeedLevel( byKind , wActualLevel ) )
    {
        return ERR_UPGRADE_SCIENCE_ATTREXT::ID_NOT_ENOUGH_PLAYER_LEVEL_ERR;
    }

    if( wLevel != wActualLevel )
    {
        //当前实际级别与请求级别不同，则不升级
        //DBG_INFO( "发现请求科技升级的级别与实际的级别不同, 玩家ID:%d", m_poOwner->GetDT_PLAYER_BASE_DATA().dwID );

        //如果升级后数据没保存，会与客户端升级页面显示的不一致,或者遭到了外挂也会遇到这种情况
        //(这种情况应该自动转换为级别1的数据查询，因为不可能会显示级别0的数据查询)
        if( wActualLevel == 0 )
        {
            stUpdatedData.wLevel = 1;
        }
        else
        {
            stUpdatedData.wLevel = wActualLevel;
        }

    }
    else
    {
        //当前实际级别与请求级别相同，则进行升级
        switch( byKind )
        {
        case EBAK_HP:
            ++m_stDT_SCIENCE_TREE_DATA.wHPLevel;
            break;
        case EBAK_HITRATE:
            ++m_stDT_SCIENCE_TREE_DATA.wHitLevel;
            break;
        case EBAK_ATTACK:
            ++m_stDT_SCIENCE_TREE_DATA.wAttractLevel;
            break;
        case EBAK_DODGERATE:
            ++m_stDT_SCIENCE_TREE_DATA.wDodgeLevel;
            break;
        case EBAK_CRITRATE:
            ++m_stDT_SCIENCE_TREE_DATA.wCritLevel;
            break;
        case EBAK_DECRITRATE:
            ++m_stDT_SCIENCE_TREE_DATA.wShieldLevel;
            break;

        case EBAK_FIRSTATTACK:
            ++m_stDT_SCIENCE_TREE_DATA.dwFirstAttack;
            break;

        case EBAK_COACHTALENT:
            ++m_stDT_SCIENCE_TREE_DATA.wCoachTalent;
            break;
        case EBAK_HP2:
            ++m_stDT_SCIENCE_TREE_DATA.wHPLevel2;
            break;
        case EBAK_ATTACK2:
            ++m_stDT_SCIENCE_TREE_DATA.wAttractLevel2;
            break;
        default:
            return ERR_UPGRADE_SCIENCE_ATTREXT::ID_ATTR_NOT_EXIST_ERR;
        }
        m_poOwner->DecStory( dwCostValue, CRecordMgr::EDST_UPDATESCIENCE, byKind, wActualLevel + 1 );
        stUpdatedData.wLevel = wActualLevel + 1;
    }

    stUpdatedData.byMaxLevel = stUpdatedData.wLevel == CScienceUpdatePropMgr::Instance()->GetMaxLevel( byKind );
    CTaskMgr::Instance()->OnUpgradeScience(m_poOwner);

    //数据处理
    qwCurStory = m_poOwner->GetDT_PLAYER_BASE_DATA().qwStory;

    stUpdatedData.byKind = byKind;

    stUpdatedData.wNeedPlayerLevel = CScienceUpdatePropMgr::Instance()->GetUpdateNeedLevel( byKind , stUpdatedData.wLevel + 1 );

    stPower.dwOldPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
    OnUpdateScience();
    m_poOwner->ResetBattleAttr(TRUE);
    stPower.dwCurPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;

    if( GetScienceValue( byKind, stUpdatedData.wLevel, stUpdatedData.dwCostStory, stUpdatedData.dwCurValue, stUpdatedData.dwNextValue ) )
    {
        m_poOwner->AddGuideRecord( EGF_SCIENCETREE);
        return ERR_UPGRADE_SCIENCE_ATTREXT::ID_SUCCESS;
    }
    else
    {
        return ERR_UPGRADE_SCIENCE_ATTREXT::ID_SERVER_INNER_ERR;
    }
}

BOOL CScienceTree::GetScienceValue( UINT8 byKind, UINT16 wLevel, UINT32 &dwNextCost, UINT32 &dwCurValue, UINT32 &dwNextValue )
{
    UINT32 dwTmpCost = 0;
    INT32 updatedRet = CScienceUpdatePropMgr::Instance()->GetInfo( byKind, wLevel, dwTmpCost, dwCurValue );
    if( CScienceUpdatePropMgr::GUIE_SUCCESS != updatedRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:get science unlock info failed, kind:%d,Level:%d, ret:%d"), MSG_MARK, byKind, wLevel, updatedRet );
        return FALSE;
    }

    INT32 updatedNextLevelRet = CScienceUpdatePropMgr::Instance()->GetInfo( byKind, wLevel + 1, dwNextCost, dwNextValue );

    if( CScienceUpdatePropMgr::GUIE_SUCCESS == updatedNextLevelRet )
    {
        return TRUE;
    }
    if( CScienceUpdatePropMgr::GUIE_AT_MAXMIUM_LEVEL == updatedNextLevelRet )
    {
        dwNextCost = 0;
        dwNextValue = 0;
        return TRUE;
    }
    else
    {
        SYS_CRITICAL(_SDT("[%s: %d]:get science unlock info failed, kind:%d,Level:%d, ret:%d"), MSG_MARK, byKind, wLevel, updatedNextLevelRet );
        return FALSE;
    }
}

BOOL CScienceTree::GetScienceLevel( UINT8 byKind, UINT16 &wLevel )
{
    //如果科技建筑本身未解锁，则不能升级
    switch( byKind )
    {
    case EBAK_HP:
        wLevel = m_stDT_SCIENCE_TREE_DATA.wHPLevel;
        return TRUE;

    case EBAK_HITRATE:
        wLevel = m_stDT_SCIENCE_TREE_DATA.wHitLevel;
        return TRUE;

    case EBAK_ATTACK:
        wLevel = m_stDT_SCIENCE_TREE_DATA.wAttractLevel;
        return TRUE;

    case EBAK_DODGERATE:
        wLevel = m_stDT_SCIENCE_TREE_DATA.wDodgeLevel;
        return TRUE;

    case EBAK_CRITRATE:
        wLevel = m_stDT_SCIENCE_TREE_DATA.wCritLevel;
        return TRUE;

    case EBAK_DECRITRATE:
        wLevel = m_stDT_SCIENCE_TREE_DATA.wShieldLevel;
        return TRUE;

    case EBAK_FIRSTATTACK:
        wLevel = m_stDT_SCIENCE_TREE_DATA.dwFirstAttack;
        return TRUE;

    case EBAK_COACHTALENT:
        wLevel = m_stDT_SCIENCE_TREE_DATA.wCoachTalent;
        return TRUE;

    case EBAK_HP2:
        wLevel = m_stDT_SCIENCE_TREE_DATA.wHPLevel2;
        return TRUE;

    case EBAK_ATTACK2:
        wLevel = m_stDT_SCIENCE_TREE_DATA.wAttractLevel2;
        return TRUE;
    default:
        return FALSE;
    }
}

VOID CScienceTree::OnUpdateScience()
{
    CKindID2HeroShmemMap&mapKind2HeroAllCarry = m_poOwner->GetAllCarryHeroMap();
    CHero* poHero = mapKind2HeroAllCarry.GetFistData();
    while(poHero)
    {
        poHero->InitBattleAttribute();
        poHero = mapKind2HeroAllCarry.GetNextData();
    }
}