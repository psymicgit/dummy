#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include "xiangqianmgr.h"
#include <sdutil.h>
#include <logic/record/recordmgr.h>
#include <logic/item/goods/goodspropmgr.h>
#include <logic/item/equip/equip.h>
#include <logic/item/itemmgr.h>
#include <logic/bag/bag.h>
#include <logic/hero/hero.h>
#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include <errdef.h>
#include <dll/sdframework/sdloggerimpl.h>
#include <logic/player/player.h>

#define SQL_READ_XIANGQIAN_STRING "select * from xiangqianprop"
#define SQL_READ_XIANGQIAN_ITEM_STRING "select * from xiangqianitemprop"

IMPLEMENT_SINGLETON_PROPMGR(CXiangqianMgr);

CXiangqianMgr::CXiangqianMgr()
{

}
CXiangqianMgr::~CXiangqianMgr()
{

}

BOOL CXiangqianMgr::Init()
{
    return LoadFromDB();
}

VOID CXiangqianMgr::UnInit()
{
    m_vecUnlockLevel.clear();
}

BOOL CXiangqianMgr::LoadFromDB()
{
    {
        SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
        if(NULL == pDBSession)
        {
            return FALSE;
        }
        SGDP::ISDDBRecordSet* pUnlockRes = NULL;
        UINT32 dwErrID = 0;
        string strErr;
        INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("xiangqianprop").c_str(), &pUnlockRes, &dwErrID,  &strErr);
        if(SDDB_HAS_RECORDSET != nRet)
        {
            SYS_CRITICAL( _SDT("[%s: %d]: has no data! :%s"), MSG_MARK, strErr.c_str() );
            return FALSE;
        }

        // 打印查询的表结果
        while(pUnlockRes->GetRecord())// 当有行结果时
        {
            m_vecUnlockLevel.push_back( SGDP::SDAtou(pUnlockRes->GetFieldValueByName("UnlockLevel")) );
        }

        pUnlockRes->Release();
        pUnlockRes = NULL;
    }

    {
        SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
        if(NULL == pDBSession)
        {
            return FALSE;
        }
        SGDP::ISDDBRecordSet* pXiangqianDesRes = NULL;
        UINT32 dwErrID = 0;
        string strErr;
        INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("xiangqianequipdes").c_str(), &pXiangqianDesRes, &dwErrID,  &strErr);
        if(SDDB_HAS_RECORDSET != nRet)
        {
            SYS_CRITICAL( _SDT("[%s: %d]: has no data!:%s"), MSG_MARK, strErr.c_str() );
            return FALSE;
        }

        // 打印查询的表结果
        if(pXiangqianDesRes->GetRecord())// 当有行结果时
        {
            m_xiangqianEuipDes.strEquipPos1Des = pXiangqianDesRes->GetFieldValueByName("Des1");
            m_xiangqianEuipDes.strEquipPos2Des = pXiangqianDesRes->GetFieldValueByName("Des2");
            m_xiangqianEuipDes.strEquipPos3Des = pXiangqianDesRes->GetFieldValueByName("Des3");
            m_xiangqianEuipDes.strEquipPos4Des = pXiangqianDesRes->GetFieldValueByName("Des4");
        }

        pXiangqianDesRes->Release();
        pXiangqianDesRes = NULL;
    }


    return TRUE;
}

BOOL CXiangqianMgr::IsUnlock( UINT8 byIndex, UINT16 wLevel )
{
    if( byIndex >= m_vecUnlockLevel.size() )
    {
        return FALSE;
    }

    return m_vecUnlockLevel[byIndex] <= wLevel;
}

UINT32 CXiangqianMgr::GetUnlockNum( UINT16 wLevel )
{
    UINT32 dwNum = 0;
    for( UINT32 dwIndex = 0; dwIndex < m_vecUnlockLevel.size(); ++dwIndex )
    {
        if( m_vecUnlockLevel[dwIndex] > wLevel )
        {
            break;
        }
        ++dwNum;
    }

    return dwNum;
}

BOOL CXiangqianMgr::GetAddValue( UINT16 wItemID, UINT32 &dwAddValue, UINT32 &dwAddPercentValue, UINT32 &dwType )
{
    SGoodsProp *pProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey( wItemID );
    if( NULL == pProp )
    {
        return FALSE;
    }

    if( pProp->dwParam1 == EBAK_HP )
    {
        dwAddPercentValue = pProp->dwValue;
    }
    else if( pProp->dwParam1 == EBAK_ATTACK )
    {
        dwAddPercentValue = pProp->dwValue;
    }
    else
    {
        dwAddValue = pProp->dwValue;
    }

    dwType = pProp->dwParam1;

    return TRUE;
}
BOOL CXiangqianMgr::IsEquipCanXiangqian( UINT16 wItemID, CEquip* poEquip )
{
    if(NULL == poEquip)
    {
        return FALSE;
    }
    if(0 == wItemID)
    {
        return FALSE;
    }
    SGoodsProp *pstXiangQianProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey( wItemID );
    if( NULL == pstXiangQianProp )
    {
        return FALSE;
    }

    SEquipProp* poEquipProp = poEquip->GetEquipProp();
    if (NULL == poEquipProp)
    {
        return ERR_XIANGQIAN::ID_GOODS_NOT_EXIST;
    }

    //该宝石是否可以镶嵌到该装备上
    if( poEquipProp->byPosKindID == pstXiangQianProp->dwParam2 || poEquipProp->byPosKindID == pstXiangQianProp->dwParam3 )
    {
        return TRUE;
    }

    return FALSE;
}

UINT16 CXiangqianMgr::OnXiangqian( DT_EQUIP_DATA_CLI &equipData, CPlayer *poPlayer, UINT16 wEquipID,
                                   UINT16 wEquipIdx, UINT16 wItemID, UINT8 byXiangqianPos, UINT16 wHeroID )
{
    //判断是否解锁
    CBuild *pBuild = poPlayer->GetBuild( EBK_XIANGQIAN );
    if( NULL == pBuild )
    {
        return ERR_XIANGQIAN::ID_BUILD_UNLOCK;
    }
    //判断装备是否存在
    CEquip *pEquip = poPlayer->GetEquip( wEquipID, wEquipIdx );
    if( NULL == pEquip )
    {
        return ERR_XIANGQIAN::ID_EQUIP_NOT_EXIST;
    }

    //判断物品是否存在
    CGoods *pGoods = poPlayer->GetBag().GetGoodsItem( wItemID );
    if( NULL == pGoods )
    {
        return ERR_XIANGQIAN::ID_GOODS_NOT_EXIST;
    }

    SEquipProp* poEquipProp = pEquip->GetEquipProp();
    if (NULL == poEquipProp)
    {
        return ERR_XIANGQIAN::ID_GOODS_NOT_EXIST;
    }

    //判断是否可镶嵌
    if( !IsUnlock( byXiangqianPos, poEquipProp->wDressLevel ) )
    {
        return ERR_XIANGQIAN::ID_ALREADY_XIANGQIAN_THIS_TYPE;
    }

    //判断类型是否可镶嵌到改装备
    if( !IsEquipCanXiangqian( wItemID, pEquip ) )
    {
        return ERR_XIANGQIAN::ID_EQUIP_POS_CANNOT_XIANGQIAN;
    }

    //判断镶嵌位置是否有效
    if( byXiangqianPos >= MAX_XIANGQIAN_NUM )
    {
        return ERR_XIANGQIAN::ID_NOT_EXIST_HOLE;
    }

    //判断是否有空位,若原位置有宝石则卸下
    if( pEquip->GetDT_EQUIP_DATA().astXiangqianList[byXiangqianPos].wItemID != 0 )
    {
        ECreateItemRet ret;
        CItemMgr::Instance()->CreateItemIntoBag( poPlayer, pEquip->GetDT_EQUIP_DATA().astXiangqianList[byXiangqianPos].wItemID,
                1, ret, 0, CRecordMgr::EAIT_REPLACEXIANGQIAN );
        if( ret != ECIR_SUCCESS )
        {
            SYS_CRITICAL( _SDT("[%s: %d]: CXiangqianMgr::CreateItemIntoBag failed, PlayerID:%u,ret:%d" ), MSG_MARK, poPlayer->GetID(), ret );
            return ERR_XIANGQIAN::ID_XIANGQIAN_GEM_CHANGE;
        }
        pEquip->GetDT_EQUIP_DATA().astXiangqianList[byXiangqianPos].wItemID = 0;
    }
    //若存在相同类型宝石则卸下
    DT_XIANGQIAN_DATA* pstDT_XIANGQIAN_DATA = pEquip->GetDT_EQUIP_DATA().astXiangqianList;
    for(UINT8 byIdx = 0; byIdx < MAX_XIANGQIAN_NUM; byIdx++, pstDT_XIANGQIAN_DATA++)
    {
        UINT16 wGemItemID = pstDT_XIANGQIAN_DATA->wItemID;
        if(0 == wItemID)
        {
            continue;
        }
        if(wItemID == wGemItemID)
        {
            return FALSE;
        }
        if(byXiangqianPos == byIdx)
        {
            continue;
        }
        SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey( wGemItemID );
        if((pstGoodsProp) && (pstGoodsProp->dwParam1 == pGoods->GetGoodsProp()->dwParam1)) //同个装备不能镶嵌同个类型的宝石
        {
            ECreateItemRet ret;
            CItemMgr::Instance()->CreateItemIntoBag( poPlayer, pEquip->GetDT_EQUIP_DATA().astXiangqianList[byIdx].wItemID,
                    1, ret, 0, CRecordMgr::EAIT_REPLACEXIANGQIAN );
            if( ret != ECIR_SUCCESS )
            {
                SYS_CRITICAL( _SDT("[%s: %d]: CXiangqianMgr::CreateItemIntoBag failed, PlayerID:%u,ret:%d" ), MSG_MARK, poPlayer->GetID(), ret );
                return ERR_XIANGQIAN::ID_XIANGQIAN_GEM_CHANGE;
            }
            pEquip->GetDT_EQUIP_DATA().astXiangqianList[byIdx].wItemID = 0;
            break;
        }
    }

    //镶嵌
    pEquip->Xiangqian( wItemID, byXiangqianPos );

    CHero *pEquipHero = pEquip->GetHero();
    if( pEquipHero != NULL )
    {
        pEquipHero->InitBattleAttribute();
        poPlayer->ResetBattleAttr();
    }

    pEquip->GetDT_EQUIP_DATA_CLI( equipData );
    return ERR_XIANGQIAN::ID_SUCCESS;
}

VOID CXiangqianMgr::GetXiangqianEquipDes( char *pszDes1, char *pszDes2, char *pszDes3, char *pszDes4 )
{
    strcpy( pszDes1, m_xiangqianEuipDes.strEquipPos1Des.c_str() );
    strcpy( pszDes2, m_xiangqianEuipDes.strEquipPos2Des.c_str() );
    strcpy( pszDes3, m_xiangqianEuipDes.strEquipPos3Des.c_str() );
    strcpy( pszDes4, m_xiangqianEuipDes.strEquipPos4Des.c_str() );
}

vector<UINT16> CXiangqianMgr::GetXiangqianPosList( UINT16 wItemID )
{
    vector<UINT16> vecRet;

    SGoodsProp *pProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey( wItemID );
    if( NULL == pProp )
    {
        return vecRet;
    }

    if( pProp->dwParam2 != 0 )
    {
        vecRet.push_back( pProp->dwParam2 );
    }

    if( pProp->dwParam3 != 0 )
    {
        vecRet.push_back( pProp->dwParam3 );
    }

    return vecRet;

}