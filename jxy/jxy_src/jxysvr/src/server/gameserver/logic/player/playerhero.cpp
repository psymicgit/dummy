#include "player.h"
#include "playerbasedatacachemgr.h"
#include <common/client/errdef.h>
#include <net/gt/gtpktbuilder.h>
#include <protocol/server/protogsls.h>
#include "logic/player/playermgr.h"
#include "logic/player/playersavemgr.h"
#include "logic/other/singleprammgr.h"
#include <framework/gsapi.h>
#include <json/json.h>
#include <logic/vip/vippropmgr.h>
#include <logic/record/recordmgr.h>
#include <logic/other/singleprammgr.h>
#include <logic/base/basedefine.h>
#include <logic/http/httpmgr.h>
#include <logic/http/levelhttpcmd.h>
#include <logic/hero/heropropmgr.h>
#include <logic/updateattr/updateattrpropmgr.h>
#include <logic/task/taskmgr.h>
#include <logic/hero/heromgr.h>
#include <logic/formation/formation.h>
#include "logic/build/build.h"
#include "logic/other/state.h"
#include "logic/jingjie/jingjie.h"
#include "logic/other/xiangqianmgr.h"
#include "logic/item/jewelry/jewelrypropmgr.h"
#include "logic/item/equip/equipmakemgr.h"
#include "../soul/soul.h"
#include "logic/battle/battlemgr.h"
CHero* CPlayer::GetHero(UINT16 wKindID)
{
    return m_mapKind2Hero.GetData(wKindID);
}

UINT16 CPlayer::GetLevel()
{
    if(NULL == m_poCoachHero) return 1;
    return m_poCoachHero->GetDT_HERO_BASE_DATA().wLevel;
}
UINT16 CPlayer::GetCoachHeroKindID()
{
    if(NULL == m_poCoachHero) return 0;
    return m_poCoachHero->GetDT_HERO_BASE_DATA().wKindID;
}

VOID CPlayer::SetCoachHero(CHero* poCoachHero)
{
    m_poCoachHero = poCoachHero;
}

CHero* CPlayer::NewHero(UINT16 wHeroKindId)
{
    //赋值新增属性
    DT_HERO_DATA stDT_HERO_DATA;
    memset(&stDT_HERO_DATA, 0, sizeof(stDT_HERO_DATA));
    stDT_HERO_DATA.stHeroBaseData.wLevel = 1;//新增武将1级
    stDT_HERO_DATA.stHeroBaseData.byPos = EHP_RECRUIT;//新增武将放在招募馆中
    stDT_HERO_DATA.stHeroBaseData.byCoach = 0;
    stDT_HERO_DATA.stHeroBaseData.byFormationIdx = 0;
    stDT_HERO_DATA.stHeroBaseData.wTalent = 0;
    stDT_HERO_DATA.stHeroBaseData.wKindID = wHeroKindId;
    //新增武将的扩展属性为0,不需赋值
	DT_JEWELRY_DATA_LST stJewelryLst = {0};
	DT_HERO_SOUL_DATA rSoulData;
	memset(&rSoulData, 0, sizeof(DT_HERO_SOUL_DATA));

    //判断是否存在该类型武将，若存在，则配置有问题
    if(NULL != m_mapKind2Hero.GetData(wHeroKindId))
    {
        USR_INFO(_SDT("[%s: %d]: CPlayer::AddHero failed, rebuild %d !"), MSG_MARK, wHeroKindId);
        return NULL;
    }

    SHeroProp *pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(wHeroKindId);
    if (NULL == pstHeroProp){
        return NULL;
    }

    /*
    // 如果已有对应的进阶伙伴，则不再创建该伙伴
    if (pstHeroProp->wUpgradeToHeroID){
        if (GetHero(pstHeroProp->wUpgradeToHeroID)){
            return NULL;
        }
    }
    */

    CHero* poHero = CHeroMgr::Instance()->CreateHero(m_dwPlayerID, wHeroKindId);
    if(NULL == poHero)
    {
        return NULL;
    }
    
    if(FALSE == poHero->Init(&stDT_HERO_DATA, stJewelryLst, rSoulData, this))
    {
        CHeroMgr::Instance()->RemoveHero(m_dwPlayerID, wHeroKindId);
        return NULL;
    }
    //poHero->SetObjDataSaveType(EODST_INSERT);
    //poHero->SetSaveState(ESS_WANT_SAVE);//数据修改，则需保存

    //新增武将添加到招募馆中
    CBuild* poBuild = GetBuild(EBK_RECRUIT);
    if(NULL == poBuild)
    {
        CHeroMgr::Instance()->RemoveHero(m_dwPlayerID, wHeroKindId);
        return NULL;
    }

    //若没有开阵型，则开阵型
    if(NULL == GetBuild(EBK_FROMATION))
    {
        NewBuild(EBK_FROMATION);
    }
    
    if (pstHeroProp->bIsLegendHero){
        if (pstHeroProp->wUpgradeFromHeroID){
            CHero *poUpgradeFromHero = GetHero(pstHeroProp->wUpgradeFromHeroID);
            if (poUpgradeFromHero){
                poBuild->GetDT_BUILD_BASE_DATA().qwValue1 = 1; //对于招募馆，则表示是否存在新伙伴可进阶
            }
        }
    }

    poBuild->GetDT_BUILD_BASE_DATA().byValue3 = 1;//对于招募馆，则表示是否存在新伙伴可招募
    //m_poMinUpgradeGodweapon = 0;

    m_mapKind2Hero.AddData(poHero->GetDT_HERO_BASE_DATA().wKindID, poHero);

    return poHero;
}

CHero* CPlayer::NewHeroNoBuild(UINT16 wHeroKindId)
{
    //赋值新增属性
    DT_HERO_DATA stDT_HERO_DATA;
    memset(&stDT_HERO_DATA, 0, sizeof(stDT_HERO_DATA));
    stDT_HERO_DATA.stHeroBaseData.wLevel = 1;//新增武将1级
    stDT_HERO_DATA.stHeroBaseData.byPos = EHP_CARRY;//新增武将放在招募馆中
    stDT_HERO_DATA.stHeroBaseData.byCoach = 0;
    stDT_HERO_DATA.stHeroBaseData.byFormationIdx = 0;
    stDT_HERO_DATA.stHeroBaseData.wTalent = 0;
    stDT_HERO_DATA.stHeroBaseData.wKindID = wHeroKindId;
    //新增武将的扩展属性为0,不需赋值
	DT_JEWELRY_DATA_LST stJewelryLst = {0};
	DT_HERO_SOUL_DATA rSoulData = {0};
    //判断是否存在该类型武将，若存在，则配置有问题
    CHero* poHero = m_mapKind2Hero.GetData(wHeroKindId);
    if(NULL != poHero)
    {
        return poHero;
    }

    poHero = CHeroMgr::Instance()->CreateHero(m_dwPlayerID, wHeroKindId);
    if(NULL == poHero)
    {
        return NULL;
    }
    if(FALSE == poHero->Init(&stDT_HERO_DATA, stJewelryLst, rSoulData, this))
    {
        CHeroMgr::Instance()->RemoveHero(m_dwPlayerID, wHeroKindId);
        return NULL;
    }

    //若没有开阵型，则开阵型
    if(NULL == GetBuild(EBK_FROMATION))
    {
        NewBuild(EBK_FROMATION);
    }

    m_mapKind2Hero.AddData(poHero->GetDT_HERO_BASE_DATA().wKindID, poHero);

    return poHero;
}

BOOL CPlayer::OnUpgrade(UINT16 wLevel)
{
    if (FALSE == m_poState)
    {
        return FALSE;
    }


    if(wLevel > CSinglePramMgr::Instance()->GetMaxPlayerLevel())
        return FALSE;
    //任务回调
    CTaskMgr::Instance()->CkWantCheckAccept(this);

    CPlayerBaseDataCacheMgr::Instance()->OnLevelChange(m_dwPlayerID, wLevel);

    //重新计算属性
    ResetBattleAttr();
    //
    m_poState->OnMainUpgrade(wLevel, m_stMaxBattleAttribute.qwHP, m_stCurBattleAttribute.qwHP);

    //向GT通知玩家升级
    PKT_GSLS_PLAYER_UPGRADE_NTF stNtf;
    stNtf.dwPlayerID = m_dwPlayerID;
    stNtf.wLevel = wLevel;
    SendMsg((CHAR*)(&stNtf), GSLS_PLAYER_UPGRADE_NTF);

    SLevelInfo stLevelInfo;
    stLevelInfo.dwPlayerID = m_dwPlayerID;
    stLevelInfo.wLevel = wLevel;
    CUser *poUser = GetUser();
    if (NULL != poUser)
    {
        stLevelInfo.strDeviceID = poUser->GetDeviceID();
        stLevelInfo.strIp = SDInetNtoa(poUser->GetLoginIP());
		stLevelInfo.byAuthType = poUser->GetAuthType();
    }

    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_LEVEL);
    if (NULL == poHttpCmd)
    {
        SYS_CRITICAL( _SDT("[%s:%d] player:%u in memory"), MSG_MARK, m_dwPlayerID );
        return TRUE;
    }
    poHttpCmd->SetUserData(&stLevelInfo, sizeof(SLevelInfo), 0);
    poHttpCmd->Init();
    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        SYS_CRITICAL( _SDT("[%s:%d] can't AddCommand player:%u in memory"), MSG_MARK, stLevelInfo.dwPlayerID );
    }
    return TRUE;


    //m_stDT_PLAYER_TODAY_CONSUME_DATA.wAddLevel += (GetLevel() - m_stDT_PLAYER_TODAY_CONSUME_DATA.byAfterVipLevel);
    //m_stDT_PLAYER_TODAY_CONSUME_DATA.wAfterLevel = GetLevel();


    //记录玩家升级时的所有信息
    //CPlayer *pPlayer = CPlayerMgr::Instance()->FindPlayer( m_dwPlayerID );

    //if( NULL == pPlayer )
    //{
    //   //记录
    //    SYS_CRITICAL( _SDT("[%s: %d]: Player[%u] not found!"), MSG_MARK, m_dwPlayerID );
    //    return FALSE;
    //}

//     BOOL bSucceed = CPlayerSaveMgr::Instance()->SavePlayerData( pPlayer, ESDBT_RECORD );
//     if( bSucceed )
//     {
//         DBG_INFO( _SDT( "准备插入玩家升级数据成功!ID:%d Level:%d" ), m_dwPlayerID, wLevel );
//     }
//     else
//     {
//         USR_IMPORTANT( _SDT( "[%s: %d]: 准备插入玩家升级数据失败!ID:%d Level:%d" ), MSG_MARK, m_dwPlayerID, wLevel );
//     }

    return TRUE;
}


//给玩家的上阵武将经验
VOID CPlayer::AllotExperience(UINT32 dwExperience)
{
    if(0 == dwExperience)
    {
        return;
    }

    CHero* poCoachHero = GetCoachHero();
	UINT32 dwAddExperience = 0;
    map<UINT8, CHero*>& mapHeroData = m_mapFormationHero.GetDataMap();
    for(map<UINT8, CHero*>::iterator itr = mapHeroData.begin(); itr != mapHeroData.end(); itr++)
    {
        CHero* poHero = itr->second;
        if(NULL == poHero)
        {
            continue;
        }
		
		dwAddExperience = ((100 + poHero->GetJewelryAttr(EJEA_INSTANCE_EXPERIENCE)) *  dwExperience / 100);

        if (poCoachHero == poHero)
        {
            poHero->AddExperience(dwAddExperience * CSinglePramMgr::Instance()->GetCoachExpRate() / 100);
        }
        else
        {
            poHero->AddExperience(dwAddExperience, poCoachHero->GetDT_HERO_BASE_DATA().wLevel);
        }
    }

}


BOOL CPlayer::InitHero(DT_HERO_DATA_LIST& stHeroData)
{
    UINT16 wHeroNum = stHeroData.wHeroNum;
    //不可能没有武将
    if(0 == wHeroNum)
    {
        return FALSE;
    }

    for(UINT16 wIdx = 0; wIdx < wHeroNum && wIdx < MAX_HERO_NUM; wIdx++)
    {
        DT_HERO_DATA* pstDT_HERO_DATA = &(stHeroData.astHeroInfoLst[wIdx]);
        CHero* poHero = CHeroMgr::Instance()->CreateHero(m_dwPlayerID, pstDT_HERO_DATA->stHeroBaseData.wKindID);
        if(NULL == poHero)
        {
            return FALSE;
        }
        if(FALSE == poHero->Init(pstDT_HERO_DATA, stHeroData.astjewelryInfoLst[wIdx], stHeroData.astSoulInfoLst[wIdx], this))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: hero init failed!"), MSG_MARK);
            CHeroMgr::Instance()->RemoveHero(m_dwPlayerID, pstDT_HERO_DATA->stHeroBaseData.wKindID);
            continue;//跳过有问题数据
        }
        if(COACHHERO == pstDT_HERO_DATA->stHeroBaseData.byCoach)
        {
            m_poCoachHero = poHero;
        }
        //招募馆的武将不添加到携带表中
        if(EHP_RECRUIT == poHero->GetDT_HERO_BASE_DATA().byPos)
        {

        }
        else
        {
            m_mapKind2HeroAllCarry.AddData(pstDT_HERO_DATA->stHeroBaseData.wKindID, poHero);
        }
        m_mapKind2Hero.AddData(poHero->GetDT_HERO_BASE_DATA().wKindID, poHero);
    }
    if(NULL == m_poCoachHero)
    {
        return FALSE;
    }

    //若携带伙伴个数大于配置，则多余的回招募馆
    INT8 cDownNum = m_mapKind2HeroAllCarry.Size() - m_poInstance->GetCanCarryNum();
    CHero* poHero = m_mapKind2HeroAllCarry.GetFistData();
    while((cDownNum > 0) && (poHero))
    {
        if((poHero) && (EHP_BATTLE != poHero->GetDT_HERO_BASE_DATA().byPos))
        {
            Disbang(poHero->GetDT_HERO_BASE_DATA().wKindID);
            cDownNum--;
        }
        poHero = m_mapKind2HeroAllCarry.GetNextData();
    }

    return TRUE;
}

VOID CPlayer::GetHeroUpgradeInfo(UINT8& byHeroNum, DT_HERO_UPGRADE_DATA astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM])
{
    byHeroNum = 0;
    for(UINT8 byIdx = 1; byIdx <= MAX_FORMATION_IDX_NUM; byIdx++)
    {
        CBattleAttrObj* poObj = GetFormationObj(byIdx);
        CHero* poHero = NULL;
        if(poObj)
        {
            poHero = dynamic_cast<CHero*>(poObj);
            if(poHero)
            {
                poHero->GetUpgrageInfo(astHeroUpgradeInfo[byHeroNum++]);
            }
        }
    }
}

VOID CPlayer::GetMainHeroUpgradeInfo(DT_HERO_UPGRADE_DATA& astHeroUpgradeInfo)
{
    memset(&astHeroUpgradeInfo, 0, sizeof(DT_HERO_UPGRADE_DATA));
    if(m_poCoachHero)
    {
        m_poCoachHero->GetUpgrageInfo(astHeroUpgradeInfo);
    }
}

VOID CPlayer::GetPassInstanceHeroUpgradeInfo(UINT32 dwExp, UINT8& byHeroNum, DT_HERO_UPGRADE_DATA astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM])
{
    byHeroNum = 0;
    for(UINT8 byIdx = 1; byIdx <= MAX_FORMATION_IDX_NUM; byIdx++)
    {
        CBattleAttrObj* poObj = GetFormationObj(byIdx);
        CHero* poHero = NULL;
        if(poObj)
        {
            poHero = dynamic_cast<CHero*>(poObj);
            if(poHero)
            {
                DT_HERO_UPGRADE_DATA& stDT_HERO_UPGRADE_DATA = astHeroUpgradeInfo[byHeroNum++];
                poHero->GetUpgrageInfo(stDT_HERO_UPGRADE_DATA);
                stDT_HERO_UPGRADE_DATA.dwEncExperience = dwExp;
                if(stDT_HERO_UPGRADE_DATA.wHeroKindID == GetCoachHeroKindID())
                {
                    stDT_HERO_UPGRADE_DATA.dwEncExperience = dwExp * CSinglePramMgr::Instance()->GetCoachExpRate() / 100;;
                }
                if(0 == stDT_HERO_UPGRADE_DATA.byUpgradeFlag)
                {
                    UINT32 dwLastExp = (UINT32)(poHero->GetDT_HERO_BASE_DATA().qwExperience - dwExp);
                    SHeroUpgradeProp* pstProp = CHeroPropMgr::Instance()->GetHeroUpgradeProp(stDT_HERO_UPGRADE_DATA.wLevel);
                    if(pstProp)
                    {
                        if(dwLastExp < pstProp->qwExperience)
                        {
                            stDT_HERO_UPGRADE_DATA.byUpgradeFlag = 1;
                        }
                    }
                }
            }
        }
    }

}


VOID CPlayer::GetExperienceInfo(DT_EXPERIENCE_DATA& stExperienceInfo)
{
    if(m_poCoachHero)
    {
        m_poCoachHero->GetExperienceInfo(stExperienceInfo);
    }
}

//检测是否达到上限携带
BOOL CPlayer::CkCarryHeroFull()
{
    //携带上限包括上阵的
    //if((m_mapKind2HeroAllCarry.Size() - m_mapFormationHero.Size()) >= m_poInstance->GetCanCarryNum())
    if(m_mapKind2HeroAllCarry.Size() >= m_poInstance->GetCanCarryNum())
    {
        return TRUE;
    }

    return FALSE;
}

UINT16 CPlayer::GetCarryHeroInfoNew(UINT16& wHeroNum, 
	DT_HERO_PANEL_CLI astHeroInfoLst[MAX_HERO_NUM], 
	DT_HERO_EQUIP_DATA_LST_CLI *poEquipDataLst , 
	DT_ATTR_ENABLE_UPDATE_INFO *poAttrEnableUpdateLst ,
	DT_SKILL_DATA_LST_CLI *poSkillInfo , 
	DT_HERO_JEWELRY_DATA_LST_CLI* poJewelryInfo ,
	DT_HERO_SOUL_DATA_LST_CLI* pSoulInfo,
    DT_HERO_SOUL_POTENCY_DATA_LST_CLI* poSoulPotencyInfo,
    DT_JEWELRY_SUIT_LIST_CLI *pJewelrySuitInfo)
{
	GetFormation().CheckFormation();
	
	vector<CHero*> setHero;
	GetPlayerAllHero(setHero);
	
	size_t nIndex = 0;
	for (vector<CHero*>::iterator itor = setHero.begin(), end = setHero.end()
		; end != itor
		; ++ itor)
	{
		CHero* poHero = (*itor);
		if (NULL != poHero) {
			FillDataListByIndexNew(nIndex, *poHero, astHeroInfoLst, poEquipDataLst, poAttrEnableUpdateLst, poSkillInfo, poJewelryInfo, pJewelrySuitInfo);
			if ( NULL != pSoulInfo)
			{
				pSoulInfo[nIndex].wSoulEquipNum = poHero->GetHeroSoulData().wSoulNum;
				if ( NULL != poSoulPotencyInfo)
				{
					poSoulPotencyInfo[nIndex].wSoulEquipNum = pSoulInfo[nIndex].wSoulEquipNum;
				}
				
				for (size_t n = 0; n < pSoulInfo[nIndex].wSoulEquipNum && n < MAX_SOUL_EQUIP_NUM; ++n) {
					CSoul soul;
					soul.Init(this, poHero->GetHeroSoulData().astSoulLstData[n], poHero->GetHeroSoulData().astSoulPotencyLstData[n], poHero);
					soul.GetSoulCli(pSoulInfo[nIndex].astSoulEquipLstData[n]);
					if ( NULL != poSoulPotencyInfo)
					{
						soul.GetSoulPotencyCli(poSoulPotencyInfo[nIndex].astSoulEquipLstData[n]);
					}
				}
			}
		}
		++ nIndex;
	}

	wHeroNum = nIndex;

    if (pJewelrySuitInfo){
        pJewelrySuitInfo->wHeroNum = wHeroNum;
    }

	return ERR_GET_HERO_LIST::ID_SUCCESS;
}

UINT16 CPlayer::GetCarryHeroInfo(UINT16& wHeroNum, DT_HERO_BASE_DATA_CLI astHeroInfoLst[MAX_HERO_NUM],
                                 DT_HERO_EQUIP_DATA_LST_CLI *poEquipDataLst/*=NULL*/, DT_ATTR_ENABLE_UPDATE_INFO *poAttrEnableUpdateLst/*=NULL*/,
                                 DT_SKILL_DATA_CLI *poSkillInfo/*=NULL*/  )
{

	GetFormation().CheckFormation();
    map<UINT16, UINT16> mapAlreadyIn;

    INT32 nFillIndex = 0;
//   //先传回上阵的
//   UINT32 nResult = m_mapFormationHero.Size();

    //先写入主角的信息
    FillDataListByIndex( nFillIndex++, *m_poCoachHero, astHeroInfoLst, poEquipDataLst, poAttrEnableUpdateLst, poSkillInfo);
    mapAlreadyIn[m_poCoachHero->GetDT_HERO_BASE_DATA().wKindID] = 1;

    for(CHero* poHero = m_mapFormationHero.GetFistData(); poHero != NULL; poHero = m_mapFormationHero.GetNextData())
    {
        if( NULL == poHero )
        {
            //找不到指定类型的伙伴是有可能的，因为玩家身上未必携带该伙伴
            continue;
        }

        //如果已经添加存在，则不添加
        map<UINT16, UINT16>::iterator itr = mapAlreadyIn.find(poHero->GetDT_HERO_BASE_DATA().wKindID);
        if( itr != mapAlreadyIn.end() )
        {
            continue;
        }

        FillDataListByIndex( nFillIndex++, *poHero, astHeroInfoLst, poEquipDataLst, poAttrEnableUpdateLst, poSkillInfo);
        mapAlreadyIn[poHero->GetDT_HERO_BASE_DATA().wKindID] = 1;

        //ssLog << poHero->GetDT_HERO_BASE_DATA().wKindID;
    }

    //DBG_INFO( ssLog.str().c_str() );


    //包括主角自己
    wHeroNum = m_mapKind2HeroAllCarry.Size();

    vector<UINT8> vecPos;
    for( std::size_t nHeroIndex = 0; nHeroIndex < m_mapKind2HeroAllCarry.Size(); ++nHeroIndex )
    {
        CHero* poHero = m_mapKind2HeroAllCarry.GetDataByIndex( nHeroIndex );

        if( NULL == poHero )
        {
            //找不到指定类型的伙伴是有可能的，因为玩家身上未必携带该伙伴
            continue;
        }

        //如果已经添加存在，则不添加
        map<UINT16, UINT16>::iterator itr = mapAlreadyIn.find(poHero->GetDT_HERO_BASE_DATA().wKindID);
        if( itr != mapAlreadyIn.end() )
        {
            continue;
        }

        FillDataListByIndex( nFillIndex++, *poHero, astHeroInfoLst, poEquipDataLst, poAttrEnableUpdateLst, poSkillInfo );

        mapAlreadyIn[poHero->GetDT_HERO_BASE_DATA().wKindID] = 1;
    }
    return ERR_GET_HERO_LIST::ID_SUCCESS;
}

bool CPlayer::FormationHeroSortByCarryIndex( UINT16 byHeroKind1, UINT16 byHeroKind2 )
{
    return m_mapKind2HeroAllCarry.GetDataIndexByKey( byHeroKind1 ) < m_mapKind2HeroAllCarry.GetDataIndexByKey( byHeroKind2 );
}

BOOL CPlayer::FillDataListByIndex( UINT16 wIndex, CHero &oHero, DT_HERO_BASE_DATA_CLI *poBaseDataLst,
                                   DT_HERO_EQUIP_DATA_LST_CLI *poEquipDataLst, DT_ATTR_ENABLE_UPDATE_INFO *poAttrEnableUpdateLst,
                                   DT_SKILL_DATA_CLI *poSkillInfo)
{
    return GetHeroData( oHero, &poBaseDataLst[wIndex], poEquipDataLst ? &poEquipDataLst[wIndex] : NULL,
                        poAttrEnableUpdateLst ? &poAttrEnableUpdateLst[wIndex] : NULL,
                        poSkillInfo ? &poSkillInfo[wIndex] : NULL );
}

BOOL CPlayer::GetHeroData( CHero &oHero, DT_HERO_BASE_DATA_CLI *poBaseData,
                           DT_HERO_EQUIP_DATA_LST_CLI *poEquipData, DT_ATTR_ENABLE_UPDATE_INFO *poAttrEnableUpdateLst,
                           DT_SKILL_DATA_CLI *poSkillInfo)
{
    if( NULL != poBaseData )
    {
        oHero.GetDT_HERO_BASE_DATA_EQUIP_CLI(*poBaseData);
    }

    if( NULL != poEquipData )
    {
        oHero.GetEquip( *poEquipData );
    }

    if( NULL != poAttrEnableUpdateLst )
    {
        //如果当前值小于最大值，那么可升级
        poAttrEnableUpdateLst->byEnableAttrAttackUpdate = ( oHero.GetDT_HERO_BASE_DATA().stUpdateAttrInfo.wAttrAttack
                < CUpdateAttrPropMgr::Instance()->GetTotalCount( oHero.GetDT_HERO_BASE_DATA().wLevel ) );
        poAttrEnableUpdateLst->byEnableAttrHPUpdate = ( oHero.GetDT_HERO_BASE_DATA().stUpdateAttrInfo.wAttrHP
                < CUpdateAttrPropMgr::Instance()->GetTotalCount( oHero.GetDT_HERO_BASE_DATA().wLevel ));
    }

    if( NULL != poSkillInfo )
    {
        oHero.GetHeroSkillCli(*poSkillInfo);
    }

    return TRUE;
}

//给后面的数组参数指定填充的索引
BOOL CPlayer::FillDataListByIndexNew( UINT16 wIndex, CHero &oHero, DT_HERO_PANEL_CLI *poBaseDataLst, 
	DT_HERO_EQUIP_DATA_LST_CLI *poEquipDataLst, DT_ATTR_ENABLE_UPDATE_INFO *poAttrEnableUpdateLst,
	DT_SKILL_DATA_LST_CLI *poSkillInfo, DT_HERO_JEWELRY_DATA_LST_CLI* poJewelry, DT_JEWELRY_SUIT_LIST_CLI *pJewelrySuit)
{
	return GetHeroDataNew( oHero, &poBaseDataLst[wIndex], poEquipDataLst ? &poEquipDataLst[wIndex] : NULL,
		poAttrEnableUpdateLst ? &poAttrEnableUpdateLst[wIndex] : NULL,
		poSkillInfo ? &poSkillInfo[wIndex] : NULL, poJewelry ? &poJewelry[wIndex] : NULL, pJewelrySuit ? &pJewelrySuit->astSuitList[wIndex] : NULL);
}

BOOL CPlayer::GetHeroDataNew( CHero &oHero, DT_HERO_PANEL_CLI *poBaseData, 
	DT_HERO_EQUIP_DATA_LST_CLI *poEquipData, DT_ATTR_ENABLE_UPDATE_INFO *poAttrEnableUpdateLst,
	DT_SKILL_DATA_LST_CLI *poSkillInfo, DT_HERO_JEWELRY_DATA_LST_CLI* poJewelry, DT_JEWELRY_SUIT_CLI *pJewelrySuit)
{
	if( NULL != poBaseData )
	{
		oHero.GetHeroPanelInfo(*poBaseData);
	}

	if( NULL != poEquipData )
	{
		oHero.GetEquip( *poEquipData );
	}

	if( NULL != poAttrEnableUpdateLst )
	{
		//如果当前值小于最大值，那么可升级
		poAttrEnableUpdateLst->byEnableAttrAttackUpdate = ( oHero.GetDT_HERO_BASE_DATA().stUpdateAttrInfo.wAttrAttack
			< CUpdateAttrPropMgr::Instance()->GetTotalCount( oHero.GetDT_HERO_BASE_DATA().wLevel ) );
		poAttrEnableUpdateLst->byEnableAttrHPUpdate = ( oHero.GetDT_HERO_BASE_DATA().stUpdateAttrInfo.wAttrHP
			< CUpdateAttrPropMgr::Instance()->GetTotalCount( oHero.GetDT_HERO_BASE_DATA().wLevel ));
	}

	if( NULL != poSkillInfo )
	{
		poSkillInfo->bySkillNum = 1;
		oHero.GetHeroSkillLstCli(*poSkillInfo);
	}

	if (NULL != poJewelry)
	{
		oHero.GetJeweryCli(*poJewelry);
	}

    if (pJewelrySuit){
        oHero.GetJewerySuitCli(*pJewelrySuit);
    }

	return TRUE;
}

UINT16 CPlayer::Recuit(UINT16 wHeroKindID, DT_HERO_BASE_DATA_CLI& stDT_HERO_BASE_DATA_CLI)
{
    CBuild* poBuild = GetBuild(EBK_RECRUIT);
    if(NULL == poBuild)
    {
        return ERR_RECUIT_HERO::ID_RECUIT_NOT_EXIST_ERR;
    }

    CHero* poHero = m_mapKind2Hero.GetData(wHeroKindID);
    if(NULL == poHero)
    {
        return ERR_RECUIT_HERO::ID_HERO_NOT_EXIST_ERR;
    }
    UINT16 wJingjieQualityLevel = 0;
    if (m_poJingJie)
    {
        wJingjieQualityLevel = m_poJingJie->GetJingjieQualityLevel();
    }

    BOOL bAlreadyRecuit = FALSE;
    if( EHP_RECRUIT != poHero->GetDT_HERO_BASE_DATA().byPos )
    {
        bAlreadyRecuit = TRUE;
    }

    if( poHero->GetDT_HERO_BASE_DATA().byCoach )
    {
        //若主角则返回相应错误
        return ERR_RECUIT_HERO::ID_CANNOT_RECUIT_COACH;
    }

    //如果已经携带则不判断可携带数目
    if( !bAlreadyRecuit )
    {
        //判断可携带数目
        DT_INSTANCE_ID& stInstanceID = m_poInstance->GetMaxCommonInstanceID();
        SHeroUnlockProp* pstHeroUnlockProp = CInstancePropMgr::Instance()->GetUnlockHero(stInstanceID.wSceneIdx, stInstanceID.byTownIdx, stInstanceID.byInstanceIdx);
        if(NULL == pstHeroUnlockProp)
        {
            RETURN_OTHER_ERR;
        }
        if(CkCarryHeroFull())
        {
            return ERR_RECUIT_HERO::ID_UPPER_LIMIT_ERR;
        }
    }


    SHeroProp* pstHeroProp = poHero->GetHeroProp();
    if(NULL == pstHeroProp)
    {
        RETURN_OTHER_ERR;
    }

    if (CKGuide( EGF_RECUIT))
    {
        poHero->GetDT_HERO_BASE_DATA().byVisitNum = pstHeroProp->byVisitNum;
    }
    
    //没有招募过，则判断拜访次数/等级/境界是否已经足够
    if(0 == poHero->GetDT_HERO_BASE_DATA().byRecuitedFlag)
    {
        if (pstHeroProp->bIsLegendHero && pstHeroProp->wUpgradeFromHeroID > 0){
            return ERR_RECUIT_HERO::ID_MUST_UPGRADE;
        }

        if(poHero->GetDT_HERO_BASE_DATA().byVisitNum < pstHeroProp->byVisitNum){
            // 如果不是外传英雄，则需要摇骰子提升好感度
            if (!pstHeroProp->bIsLegendHero){
                return ERR_RECUIT_HERO::ID_NOT_REACH_VISIT_NUM;
            }
        }else if (wJingjieQualityLevel < pstHeroProp->wRequireJingjieLevel){
            return ERR_RECUIT_HERO::ID_NOT_ENOUGH_JINGJIE_LEVEL;
        }else if(GetLevel() < pstHeroProp->wRecuitLevel){
            return ERR_RECUIT_HERO::ID_NOT_ENOUGH_PLAYER_LEVEL;
        }

        if (pstHeroProp->bIsLegendHero){
            DT_MATERIAL_LIST stMaterials;
            CEquipMakeMgr::TakeMaterialListCli(*this, pstHeroProp->vecCostMaterial, stMaterials);

            for(UINT16 i = 0; i < stMaterials.byMaterialCnt; i++){
                DT_MATERIAL &stMaterial = stMaterials.astMaterialList[i];
                if (stMaterial.wCurNum < stMaterial.wRequiredNum){
                    return ERR_RECUIT_HERO::ID_MATERIAL_NOT_ENOUGH;
                }
            }

            BOOL bCostSucc = CEquipMakeMgr::CostMaterialList(*this, stMaterials, CRecordMgr::EDIT_RECRUIT_LEGEND_HERO);
            if (!bCostSucc){
                return ERR_RECUIT_HERO::ID_MATERIAL_NOT_ENOUGH;
            }

            CRecordMgr::Instance()->RecordInfo(GetPlayerID(), ERM_FLY, CRecordMgr::EF_RECRUIT_HERO, poHero->GetHeroID(), poHero->GetDT_HERO_BASE_DATA().byRecuitedFlag,
                poHero->GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, 0, 0, 0, 0);
        }
    }


    if( !bAlreadyRecuit )
    {
        //招募,携带
        m_mapKind2HeroAllCarry.AddData(wHeroKindID, poHero);
        poHero->OnRecuit();

        CTaskMgr::Instance()->OnRecruitHero(this, wHeroKindID);

        CTaskMgr::Instance()->OnRecruitHeroNum(this, wHeroKindID);
    }
    poHero->InitBattleAttribute();
    //返回信息
    poHero->GetDT_HERO_BASE_DATA_EQUIP_CLI(stDT_HERO_BASE_DATA_CLI);

    //记录招募引导
    AddGuideRecord( EGF_RECUIT);
    //判断是否需布阵
    CKGuide( EGF_EMBATTLE);
    //3个武将时2次布阵引导
    if(m_mapKind2HeroAllCarry.Size() == 3)
    {
        //判断是否需2次布阵引导
        CKGuide( EGF_SECOND_EMBATTLE);
    }

    return ERR_RECUIT_HERO::ID_SUCCESS;
}

BOOL CPlayer::IsUpgradable(UINT16 wHeroKindID)
{
    CHero *poUpgradeHero = GetUpgradeHero(wHeroKindID);
    if (NULL == poUpgradeHero){
        return FALSE;
    }

    if (poUpgradeHero->IsRecruit()){
        return FALSE;
    }

    return TRUE;

    /*
    SInstanceID &stNeedPassInstance = pstUpgradeHeroProp->stNeedPassInstanceID;
    if (!m_poInstance->CkHaveRecord(stNeedPassInstance.wSceneIdx, stNeedPassInstance.byTownIdx, stNeedPassInstance.byInstanceIdx)){
        return FALSE;
    }

    return TRUE;
    */
}

CHero* CPlayer::GetUpgradeHero(UINT16 wHeroKindID)
{
    if (!IsFly()){
        return NULL;
    }

    SHeroProp* pstProp = CHeroPropMgr::Instance()->GetHeroProp(wHeroKindID);
    if (NULL == pstProp || 0 == pstProp->wUpgradeToHeroID){
        return NULL;
    }

    SHeroProp* pstUpgradeHeroProp = CHeroPropMgr::Instance()->GetHeroProp(pstProp->wUpgradeToHeroID);
    if (NULL == pstUpgradeHeroProp){
        return NULL;
    }

    return GetHero(pstProp->wUpgradeToHeroID);
}

UINT16 CPlayer::UpgradeHero(UINT16 wHeroKindID, PKT_CLIGS_UPGRADE_HERO_ACK &stAck)
{
    if (!IsFly()){
        return ERR_UPGRADE_HERO::ID_NOT_FLY;
    }

    // 检测招募馆是否已解锁
    CBuild* poBuild = GetBuild(EBK_RECRUIT);
    if(NULL == poBuild){
        return ERR_UPGRADE_HERO::ID_IS_UNOPEN;
    }

    CHero *poHero = GetHero(wHeroKindID);
    if (NULL == poHero){
        return ERR_UPGRADE_HERO::ID_NOT_FOUND_HERO;
    }

    // 检测是否已被招募：已被招募才能进阶
    if (!poHero->IsRecruit()){
        return ERR_UPGRADE_HERO::ID_NOT_UPGRADABLE;
    }

    SHeroProp* pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(wHeroKindID);
    if (NULL == pstHeroProp){
        return ERR_UPGRADE_HERO::ID_NOT_FOUND_HERO;
    }

    SHeroProp* pstUpgradeHeroProp = CHeroPropMgr::Instance()->GetHeroProp(pstHeroProp->wUpgradeToHeroID);
    if (NULL == pstUpgradeHeroProp){
        return ERR_UPGRADE_HERO::ID_NOT_UPGRADABLE;
    }

    // 获取对应的进阶伙伴
    CHero *poUpgradeHero = GetHero(pstHeroProp->wUpgradeToHeroID);
    if (NULL == poUpgradeHero){
        return ERR_UPGRADE_HERO::ID_NOT_PASS_OUTBOUND;
    }

    if (poUpgradeHero->IsRecruit()){
        return ERR_UPGRADE_HERO::ID_SUCCESS;
    }

    // 检测进阶材料信息
    DT_MATERIAL_LIST stMaterialsCli;
    CEquipMakeMgr::TakeMaterialListCli(*this, pstUpgradeHeroProp->vecCostMaterial, stMaterialsCli);

    for (UINT16 i = 0; i < stMaterialsCli.byMaterialCnt; i++){
        DT_MATERIAL &stMaterial = stMaterialsCli.astMaterialList[i];
        if (stMaterial.wCurNum < stMaterial.wRequiredNum){
            return ERR_UPGRADE_HERO::ID_MATERIAL_NOT_ENOUGH;
        }
    }

    BOOL bCostSucc = CEquipMakeMgr::CostMaterialList(*this, stMaterialsCli, CRecordMgr::EDIT_UPGRADE_HERO);
    if (!bCostSucc){
        return ERR_UPGRADE_HERO::ID_MATERIAL_NOT_ENOUGH;
    }

    // 开始进阶
    stAck.stPowerChange.dwOldPower = GetDT_PLAYER_BASE_DATA().dwPower;

    // 进阶步骤1. 从已解锁伙伴列表中移除旧伙伴
    CHeroMgr::Instance()->SwapHero(m_dwPlayerID, pstHeroProp->wKindID, pstUpgradeHeroProp->wKindID);
    CHeroMgr::Instance()->RemoveHero(m_dwPlayerID, pstHeroProp->wKindID);

    poHero->GetDT_HERO_BASE_DATA().wKindID = pstUpgradeHeroProp->wKindID;
    poHero->InitBattleAttribute();

    m_mapKind2Hero.DelData(pstHeroProp->wKindID);
    m_mapKind2Hero.DelData(pstUpgradeHeroProp->wKindID);
    m_mapKind2Hero.AddData(poHero->GetHeroID(), poHero);

    // 进阶步骤2. 如果旧伙伴已上阵，则替换成进阶伙伴
    if (m_mapKind2HeroAllCarry.GetData(pstHeroProp->wKindID)){
        m_mapKind2HeroAllCarry.DelData(pstHeroProp->wKindID);
        m_mapKind2HeroAllCarry.AddData(poHero->GetHeroID(), poHero);
    }

    if (m_mapFormationHero.GetData(pstHeroProp->wKindID)){
        m_mapFormationHero.DelData(pstHeroProp->wKindID);
        m_mapFormationHero.AddData(poHero->GetHeroID(), poHero);
    }

    poHero->ResetSkill();
    poHero->ResetEquipOwner();

    GetDT_RECRUIT_HERO_DATA_v410(poHero, stAck.stUpgradeHero);

    // 重新创建原来的伙伴，因为如果直接删除原来的伙伴，那么数据库中旧的伙伴信息就不会被更新，下次上线新旧伙伴可能出现阵位重复等冲突
    UINT8 byNewHeroFlag = poBuild->GetDT_BUILD_BASE_DATA().byValue3;
    NewHero(wHeroKindID);
    poBuild->GetDT_BUILD_BASE_DATA().byValue3 = byNewHeroFlag;

    CGodWeapon *poGodWeapon = poHero->GetGodweapon();
    if (poGodWeapon){
        // poGodWeapon->GetDT_GODWEAPON_DATA().wHeroKindID = poHero->GetHeroID();

        DT_GODWEAPON_DATA_CLI stGodweaponInfo;
        memset(&stGodweaponInfo, 0, sizeof(stGodweaponInfo));

        CGodweaponMgr::Instance()->FreeGodweapon(this, poHero->GetHeroID(), stGodweaponInfo, TRUE);
    }

    poBuild->GetDT_BUILD_BASE_DATA().qwValue1 = 0;

    // 更新酒馆标识：如果仍有伙伴可进阶，则酒馆建筑上方提示：可进阶
    for (CHero *poOwnHero = m_mapKind2Hero.GetFistData(); poOwnHero; poOwnHero = m_mapKind2Hero.GetNextData()){
        if (IsUpgradable(poOwnHero->GetHeroID())){
            poBuild->GetDT_BUILD_BASE_DATA().qwValue1 = 1;
            break;
        }
    }

    InitBattleAttr(TRUE);
    stAck.stPowerChange.dwCurPower = GetDT_PLAYER_BASE_DATA().dwPower;
    stAck.wHeroID = wHeroKindID;

    GetRecruitPrompt(stAck.stPromptInfo.astBuildPromptInfo[stAck.stPromptInfo.byBuildNum++]);

    CRecordMgr::Instance()->RecordInfo(GetPlayerID(), ERM_FLY, CRecordMgr::EF_UPGRADE_HERO, pstHeroProp->wKindID, pstUpgradeHeroProp->wKindID,
        poHero->GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, stAck.stPowerChange.dwOldPower, stAck.stPowerChange.dwCurPower, 0, 0);

    return ERR_UPGRADE_HERO::ID_SUCCESS;
}

UINT16 CPlayer::Disbang(UINT16 wHeroKindID)
{
    CBuild* poBuild = GetBuild(EBK_RECRUIT);
    if(NULL == poBuild)
    {
        return ERR_DISBAND_HERO::ID_RECUIT_NOT_EXIST_ERR;
    }

    CHero* poHero = GetHero(wHeroKindID);
    if(NULL == poHero)
    {
        return 	ERR_DISBAND_HERO::ID_HERO_NOT_EXIST_ERR;
    }

    if( EHP_RECRUIT == poHero->GetDT_HERO_BASE_DATA().byPos )
    {
        return ERR_DISBAND_HERO::ID_SUCCESS;
    }

    if(poHero == GetCoachHero())
    {
        return 	ERR_DISBAND_HERO::ID_COACH_CANNOT_DISBAND_ERR;
    }

    //清除携带/上阵
    m_mapKind2HeroAllCarry.DelData(wHeroKindID);
    m_mapFormationHero.DelData(poHero->GetDT_HERO_BASE_DATA().byFormationIdx);

    //vector< UINT16 >::iterator itrPos = std::find( m_vecCostarIndexByAllCarry.begin(), m_vecCostarIndexByAllCarry.end(), wHeroKindID );
    //if( itrPos != m_vecCostarIndexByAllCarry.end() )
    //{
    //    m_vecCostarIndexByAllCarry.erase( itrPos );
    //}


    m_apoFormationObj[poHero->GetDT_HERO_BASE_DATA().byFormationIdx] = NULL;
    if (m_poFormation)
    {
        m_poFormation->OnDisband(wHeroKindID);
    }

    poHero->OnDisband();

    //加入招募馆中
    poHero->GetDT_HERO_BASE_DATA().byPos = EHP_RECRUIT;


    //重新计算属性
    ResetBattleAttr();

    //poHero->SetSaveState(ESS_WANT_SAVE);

    return ERR_DISBAND_HERO::ID_SUCCESS;
}


//拜访武将
UINT16 CPlayer::Visit(UINT16 wHeroKindID, UINT8 bySizeFlag, UINT8 byGoldFlag, UINT8 &byWinFlag, UINT8& byNeedVisitNum, UINT8& byCurVisitNum, UINT16 &wUpgradeVisitNum, UINT32 &dwReturnCoin)
{
    CBuild* poBuild = GetBuild(EBK_RECRUIT);
    if(NULL == poBuild)
    {
        return ERR_VISIT_HERO::ID_RECUIT_NOT_EXIST_ERR;
    }

    CHero* poHero = m_mapKind2Hero.GetData(wHeroKindID);
    if(NULL == poHero)
    {
        return ERR_VISIT_HERO::ID_HERO_NOT_EXIST;
    }

    if( poHero->GetDT_HERO_BASE_DATA().byRecuitedFlag )
    {
        return ERR_VISIT_HERO::ID_ALREADY_RECUIT;
    }

    SHeroProp*	pstHeroProp = poHero->GetHeroProp();
    if(NULL == pstHeroProp)
    {
        //异常
        SYS_CRITICAL(_SDT("[%s: %d]: pstHeroProp is NULL!"), MSG_MARK);
        RETURN_OTHER_ERR;
    }
    UINT16 wJingjieQualityLevel = 0;
    if (m_poJingJie)
    {
        wJingjieQualityLevel = m_poJingJie->GetJingjieQualityLevel();
    }

    if (wJingjieQualityLevel < pstHeroProp->wRequireJingjieLevel)
    {
        return ERR_VISIT_HERO::ID_NOT_ENOUGH_JINGJIE_LEVEL;
    }
    if(GetLevel() < pstHeroProp->wRecuitLevel)
    {
        return ERR_RECUIT_HERO::ID_NOT_ENOUGH_PLAYER_LEVEL;
    }

    byNeedVisitNum  = pstHeroProp->byVisitNum;

    if(!byGoldFlag && m_stDT_PLAYER_BASE_DATA.qwCoin < pstHeroProp->dwVisitCoin)
    {
        return ERR_COMMON::ID_COIN_NOT_ENOUGH;
    }

    if((byGoldFlag) && (!CVipPropMgr::Instance()->CkOpenFunc(EVF_VISITGOLD, m_stDT_PLAYER_BASE_DATA.byVipLevel)))
    {
        return ERR_COMMON::ID_VIP_LEVEL_NOT_REACH;
    }

    if(byGoldFlag && m_stDT_PLAYER_BASE_DATA.qwGold < pstHeroProp->wVisitGold)
    {
        return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
    }

    if(poHero->GetDT_HERO_BASE_DATA().byVisitNum >= pstHeroProp->byVisitNum)
    {
        poHero->GetDT_HERO_BASE_DATA().byVisitNum = pstHeroProp->byVisitNum;
        byCurVisitNum = pstHeroProp->byVisitNum;
        return ERR_VISIT_HERO::ID_SUCCESS;
    }

    wUpgradeVisitNum = 0;
    byWinFlag = 0;
    if (byGoldFlag)
    {
        CRandomVec vecRandom;
        vecRandom.push_back(pstHeroProp->wGoldLeopardProbability);
        vecRandom.push_back(100 - pstHeroProp->wGoldLeopardProbability);
        //随机取一个，随机命中，有可能失败
        INT32 nRandomHitIdx = GetRandomIdxMustHit(vecRandom);	//命中率
        if (0 == nRandomHitIdx)
        {
            byWinFlag = 2;
            wUpgradeVisitNum = (UINT8)CSinglePramMgr::Instance()->GetRecruitFavorability();
        }
        else
        {
            byWinFlag = 1;
            wUpgradeVisitNum = 1;
        }
    }
    else
    {
        //////////////////////////////////////////////////////////////////////////
        //获取生成的等级概率

        CRandomVec vecRandom;
        vecRandom.push_back(pstHeroProp->wProbability);
        vecRandom.push_back(100 - pstHeroProp->wProbability);
        //随机取一个，随机命中，有可能失败
        INT32 nRandomHitIdx = GetRandomIdxMustHit(vecRandom);	//命中率
        if (0 == nRandomHitIdx)
        {
            CRandomVec vecRandom;
            vecRandom.push_back(pstHeroProp->wCoinLeopardProbability);
            vecRandom.push_back(100 - pstHeroProp->wCoinLeopardProbability);
            //随机取一个，随机命中，有可能失败
            INT32 nRandomHitIdx = GetRandomIdxMustHit(vecRandom);	//命中率
            if (0 == nRandomHitIdx)
            {
                byWinFlag = 2;
                wUpgradeVisitNum = (UINT8)CSinglePramMgr::Instance()->GetRecruitFavorability();
            }
            else
            {
                byWinFlag = 1;
                wUpgradeVisitNum = 1;
            }
        }
        else
        {
            byWinFlag = 0;
        }

    }
    //计算剩下多少次可招募
    UINT8 byLeftVisitNum =  pstHeroProp->byVisitNum - poHero->GetDT_HERO_BASE_DATA().byVisitNum;
    wUpgradeVisitNum = wUpgradeVisitNum > byLeftVisitNum ? byLeftVisitNum : wUpgradeVisitNum;
    if (byWinFlag)
    {
        poHero->OnVisit((UINT8)wUpgradeVisitNum);
    }

    if (byGoldFlag)
    {
        DecGold( pstHeroProp->wVisitGold, CRecordMgr::EDGT_VISITHERO, pstHeroProp->wKindID, poHero->GetDT_HERO_BASE_DATA().byVisitNum);
    }
    else
    {
        //对于赢取的话，直接扣铜钱，或者失败，要返回一定的比例。
        if (byWinFlag)
        {
            DecCoin(pstHeroProp->dwVisitCoin, CRecordMgr::EDCT_VISITHERO, pstHeroProp->wKindID, poHero->GetDT_HERO_BASE_DATA().byVisitNum);
        }
        else
        {
            dwReturnCoin = (pstHeroProp->dwVisitCoin * CSinglePramMgr::Instance()->GetRecruitReturnCoinPercentage()) / 100;
            DecCoin(dwReturnCoin, CRecordMgr::EDCT_VISITHERO, pstHeroProp->wKindID, poHero->GetDT_HERO_BASE_DATA().byVisitNum);
        }
    }

    byCurVisitNum = poHero->GetDT_HERO_BASE_DATA().byVisitNum;
    return ERR_VISIT_HERO::ID_SUCCESS;
}

//测试用，删除宝石
UINT16 CPlayer::DelItem(UINT16 wItemKindID)
{
    UINT16 wTotal = 0;
    CHero* poHero = m_mapKind2Hero.GetFistData();
    do
    {
        if (NULL == poHero)
        {
            break;
        }
        CEquipMap &	mapEquip = poHero->GetEquipMap();
        CEquip*		poEquip = mapEquip.GetFistData();
        while(poEquip)
        {


            DT_EQUIP_DATA & stDT_EQUIP_DATA = poEquip->GetDT_EQUIP_DATA();
            for (int i = 0; i < MAX_XIANGQIAN_NUM; i++)
            {
                if(stDT_EQUIP_DATA.astXiangqianList[i].wItemID >= wItemKindID)
                {
                    wTotal++;
                    stDT_EQUIP_DATA.astXiangqianList[i].wItemID = 0;
                }
            }
            poEquip = mapEquip.GetNextData();
        }
    }
    while (poHero = m_mapKind2Hero.GetNextData());
    return wTotal;
}

UINT16 CPlayer::OpenExperienceTransfer(UINT16 wHeroID, PKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_ACK &stAck)
{
    if(NULL == GetBuild(EBK_EXPTRAN))
    {
        return ERR_OPEN_EXPERIENCE_TRANSFER::ID_BUILD_NOT_EXIT;
    }

    CHero *poHero = GetHero(wHeroID);
    if (NULL == poHero)
    {
        return ERR_OPEN_EXPERIENCE_TRANSFER::ID_HERO_NOT_EXIT;
    }
    stAck.byFreePercentage = (UINT8)CSinglePramMgr::Instance()->GetHeroTransFreePercentage();
    stAck.byGoldPercentage = (UINT8)CSinglePramMgr::Instance()->GetHeroTransGoldPercentage();
    stAck.wGold = CSinglePramMgr::Instance()->GetTransGold();

    poHero->GetDT_HERO_BASE_DATA_EQUIP_CLI(stAck.stHeroInfo);

    CHero * poTransHero = m_mapKind2HeroAllCarry.GetFistData();
    while(poTransHero)
    {
        //去掉武将自己以及玩家主角信息
        if (wHeroID == poTransHero->GetHeroID() || poTransHero->GetHeroID() == m_poCoachHero->GetHeroID())
        {
            poTransHero = m_mapKind2HeroAllCarry.GetNextData();
            continue;
        }

        BOOL bIsFly = poHero->IsFly();

        //获取武将基本信息
        poTransHero->GetDT_HERO_BASE_DATA_EQUIP_CLI(stAck.astHeroInfoLst[stAck.wHeroNum]);

        UINT64 qwExperience = 0;
        //武将达到最大等级，经验还会往上涨，要过虑掉
        if (poTransHero->GetDT_HERO_BASE_DATA().wLevel == CSinglePramMgr::Instance()->GetMaxPlayerLevel())
        {
            SHeroUpgradeProp *poHeroUpgradeProp = CHeroPropMgr::Instance()->GetHeroUpgradeProp(poTransHero->GetDT_HERO_BASE_DATA().wLevel);
            if(NULL != poHeroUpgradeProp)
            {
                qwExperience = poHeroUpgradeProp->qwExperience;
            }
        }
        else
        {
            qwExperience =  poTransHero->GetDT_HERO_BASE_DATA().qwExperience;
        }

        qwExperience =  qwExperience * CSinglePramMgr::Instance()->GetHeroTransFreePercentage() / 100;
        qwExperience += poHero->GetDT_HERO_BASE_DATA().qwExperience;

        DT_HERO_UPGRADE_LEVEL &stUpgradeLevel = stAck.astUpgradeHeroInfoLst[stAck.wHeroNum];

        stUpgradeLevel.byFreeLevel = (UINT8)CHeroPropMgr::Instance()->GetLevel(qwExperience);

        if (stUpgradeLevel.byFreeLevel > GetLevel()){
            stUpgradeLevel.byFreeLevel = (UINT8)GetLevel();
        }

        qwExperience = 0;
        //武将达到最大等级，经验还会往上涨，要过虑掉
        if (poTransHero->GetDT_HERO_BASE_DATA().wLevel == CSinglePramMgr::Instance()->GetMaxPlayerLevel())
        {
            SHeroUpgradeProp *poHeroUpgradeProp = CHeroPropMgr::Instance()->GetHeroUpgradeProp(poTransHero->GetDT_HERO_BASE_DATA().wLevel);
            if(NULL != poHeroUpgradeProp)
            {
                qwExperience = poHeroUpgradeProp->qwExperience;
            }
        }
        else
        {
            qwExperience =  poTransHero->GetDT_HERO_BASE_DATA().qwExperience;
        }
        qwExperience =  qwExperience * CSinglePramMgr::Instance()->GetHeroTransGoldPercentage() / 100;
        qwExperience += poHero->GetDT_HERO_BASE_DATA().qwExperience;

        stUpgradeLevel.byGoldLevel = (UINT8)CHeroPropMgr::Instance()->GetLevel(qwExperience);

        if (stUpgradeLevel.byGoldLevel > GetLevel()){
            stUpgradeLevel.byGoldLevel = (UINT8)GetLevel();
        }

        stAck.wHeroNum++;
        poTransHero = m_mapKind2HeroAllCarry.GetNextData();
    }
    return ERR_OPEN_EXPERIENCE_TRANSFER::ID_SUCCESS;
}

UINT16 CPlayer::ExperienceTransfer(UINT16 wHeroID, UINT16 wTransHeroID, UINT8 byGold, DT_HERO_BASE_DATA_CLI &stHeroInfo, DT_HERO_BASE_DATA_CLI &stTransHeroInfo)
{
    if(NULL == GetBuild(EBK_EXPTRAN))
    {
        return ERR_OPEN_EXPERIENCE_TRANSFER::ID_BUILD_NOT_EXIT;
    }

    if (NULL == m_poCoachHero)
    {
        RETURN_OTHER_ERR;
    }

    if (wHeroID == wTransHeroID)
    {
        return ERR_EXPERIENCE_TRANSFER::ID_TRANS_HERO_EQUIL;
    }

    if (wHeroID == m_poCoachHero->GetHeroID())
    {
        return ERR_EXPERIENCE_TRANSFER::ID_TRANS_HERO_COACH;
    }

    if (wTransHeroID == m_poCoachHero->GetHeroID())
    {
        return ERR_EXPERIENCE_TRANSFER::ID_TRANS_HERO_COACH;
    }

    CHero *poHero = GetHero(wHeroID);
    if (NULL == poHero)
    {
        return ERR_EXPERIENCE_TRANSFER::ID_HERO_NOT_EXIT;
    }

    SHeroProp* pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(wHeroID);
    if(NULL == pstHeroProp)
    {
        return ERR_EXPERIENCE_TRANSFER::ID_HERO_NOT_EXIT;
    }

    if(poHero->GetDT_ABSORB_DATA_LST().byAbsorbNum > pstHeroProp->wMaxAbsorbNum)
    {
        return ERR_EXPERIENCE_TRANSFER::ID_TRANS_HERO_OVER_STEP;
    }
    //要吸收的武将
    CHero * poTransHero = GetHero(wTransHeroID);
    if (NULL == poTransHero)
    {
        return ERR_EXPERIENCE_TRANSFER::ID_HERO_NOT_EXIT;
    }

    if(poTransHero->GetAbsorbKindID() > 0)
    {
        return ERR_EXPERIENCE_TRANSFER::ID_HERO_TWICE_ERR;
    }

    if (poTransHero->GetTransHeroID(wHeroID))
    {
        return ERR_EXPERIENCE_TRANSFER::ID_HERO_MUTUAL_TRANSFER;
    }

    UINT64 qwExperience = 0;
    if (byGold > 0)
    {
        if (CSinglePramMgr::Instance()->GetTransGold() > GetDT_PLAYER_BASE_DATA().qwGold)
        {
            return ERR_EXPERIENCE_TRANSFER::ID_GOLD_NOT_ENOUGH;
        }
        DecGold(CSinglePramMgr::Instance()->GetTransGold(), CRecordMgr::EDGT_EXPTRANS);
        //武将达到最大等级，经验还会往上涨，要过虑掉
        if (poTransHero->GetDT_HERO_BASE_DATA().wLevel == CSinglePramMgr::Instance()->GetMaxPlayerLevel())
        {
            SHeroUpgradeProp *poHeroUpgradeProp = CHeroPropMgr::Instance()->GetHeroUpgradeProp(poTransHero->GetDT_HERO_BASE_DATA().wLevel);
            if(NULL != poHeroUpgradeProp)
            {
                qwExperience = poHeroUpgradeProp->qwExperience;
            }
        }
        else
        {
            qwExperience =  poTransHero->GetDT_HERO_BASE_DATA().qwExperience;
        }
        qwExperience =  qwExperience * CSinglePramMgr::Instance()->GetHeroTransGoldPercentage() / 100;
        //qwExperience += poHero->GetDT_HERO_BASE_DATA().dwExperience;
    }
    else
    {
        //武将达到最大等级，经验还会往上涨，要过虑掉
        if (poTransHero->GetDT_HERO_BASE_DATA().wLevel == CSinglePramMgr::Instance()->GetMaxPlayerLevel())
        {
            SHeroUpgradeProp *poHeroUpgradeProp = CHeroPropMgr::Instance()->GetHeroUpgradeProp(poTransHero->GetDT_HERO_BASE_DATA().wLevel);
            if(NULL != poHeroUpgradeProp)
            {
                qwExperience = poHeroUpgradeProp->qwExperience;
            }
        }
        else
        {
            qwExperience =  poTransHero->GetDT_HERO_BASE_DATA().qwExperience;
        }
        qwExperience =  qwExperience * CSinglePramMgr::Instance()->GetHeroTransFreePercentage() / 100;
        //qwExperience += poHero->GetDT_HERO_BASE_DATA().dwExperience;
    }

    SHeroUpgradeProp *poFlyLevelUpgradeProp = CHeroPropMgr::Instance()->GetHeroUpgradeProp(FLY_LEVEL);
    if(poFlyLevelUpgradeProp){
        UINT64 qwFlyLevelExp = poFlyLevelUpgradeProp->qwExperience;

        UINT64 qwFinalExp = poHero->GetDT_HERO_BASE_DATA().qwExperience + qwExperience;
        if (qwFinalExp > qwFlyLevelExp){
            UINT32 dwFlyExp = (UINT32)(qwFinalExp - qwFlyLevelExp);
            poHero->AddFlyExp(dwFlyExp);
        }
    }

    poHero->AddExperience((UINT32)qwExperience, m_poCoachHero->GetDT_HERO_BASE_DATA().wLevel);
    poTransHero->SetAbsorbKindID(wHeroID);
    poHero->AddTransHeroID(wTransHeroID);

    poHero->GetDT_HERO_BASE_DATA_EQUIP_CLI(stHeroInfo);
    poTransHero->GetDT_HERO_BASE_DATA_EQUIP_CLI(stTransHeroInfo);
    ResetBattleAttr();//玩家属性重置
    return ERR_EXPERIENCE_TRANSFER::ID_SUCCESS;
}

UINT16 CPlayer::Change2NewCareer(UINT8 byCareerID)
{
    UINT8 byOldCareerID = (UINT8)(GetCoachHeroKindID());
    if(byOldCareerID == byCareerID)
    {
        return ERR_CHANGE_CAREER_INFO::ID_SUCCESS;
    }

    if(byCareerID >= 10)
    {
        return ERR_CHANGE_CAREER_INFO::ID_CAREERID_NOT_EXIST;
    }

    SHeroProp* pstProp = CHeroPropMgr::Instance()->GetHeroProp(byCareerID);
    if(NULL == pstProp)
    {
        return ERR_CHANGE_CAREER_INFO::ID_CAREERID_NOT_EXIST;
    }

    DT_HERO_BASE_DATA stDT_HERO_BASE_DATATmp = m_poCoachHero->GetDT_HERO_BASE_DATA();
    UINT16 wErrCode = m_poCoachHero->Change2NewCareer(byCareerID);
    if(0 != wErrCode)
    {
        return wErrCode;
    }

    //修改hero管理
    m_mapKind2Hero.DelData(byOldCareerID);
    m_mapKind2HeroAllCarry.DelData(byOldCareerID);
    m_mapKind2HeroAllCarry.AddData(byCareerID, m_poCoachHero);
    m_mapKind2Hero.AddData(byCareerID, m_poCoachHero);



    //修改整型中的位置
    if (m_poFormation)
    {
        DT_NEW_FORMATION_DATA& stFormationData = m_poFormation->GetDT_FORMATION_DATA();
        for(UINT8 byFormationKindNum = 0; byFormationKindNum < stFormationData.stFormationList.byFormationKindNum; byFormationKindNum++)
        {
            DT_FORMATION_IDX_DATA& stDT_FORMATION_IDX_DATA = stFormationData.stFormationList.astFormationInfo[byFormationKindNum];
            for(UINT8 byOpenIdxNum = 0; byOpenIdxNum < stDT_FORMATION_IDX_DATA.byOpenIdxNum; byOpenIdxNum++)
            {
                DT_FORMATION_HERO_DATA& stDT_FORMATION_HERO_DATA = stDT_FORMATION_IDX_DATA.astOpenIdxInfo[byOpenIdxNum];
                if(stDT_FORMATION_HERO_DATA.wHeroID == byOldCareerID)
                {
                    stDT_FORMATION_HERO_DATA.wHeroID = byCareerID;
                }
            }
        }
        m_mapFormationHero.Clear();
        InitFormation(stFormationData);
    }

    //修改基本信息中的职业
    const SPlayerBaseData* pstPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_dwPlayerID);
    if(pstPlayerBaseData)
    {
        (const_cast<SPlayerBaseData*>(pstPlayerBaseData))->wMainHeroKindID = byCareerID;
    }

    m_stOldCareerBeforeChange = stDT_HERO_BASE_DATATmp;
    m_stOldCareerBeforeChange.byCoach = 2;//表示转职业

    ResetBattleAttr(FALSE);

    return ERR_CHANGE_CAREER_INFO::ID_SUCCESS;;
}


UINT16 CPlayer::GMRecuit( UINT16 wHeroKindID, UINT64 qwIndex)
{
    CBuild* poBuild = GetBuild(EBK_RECRUIT);
    if(NULL == poBuild)
    {
        return ERR_GM_PARM_INFO::ID_RECUIT_NOT_EXIST_ERR;
    }

    BOOL	bNew = FALSE;
    CHero* poHero = m_mapKind2Hero.GetData(wHeroKindID);
    if(NULL == poHero)
    {
        //选创建武将类型
        bNew = TRUE;
        poHero = NewHeroNoBuild(wHeroKindID);
        poHero->GetDT_HERO_BASE_DATA().byPos = EHP_RECRUIT;
    }
    UINT16 wJingjieQualityLevel = 0;
    if (m_poJingJie)
    {
        wJingjieQualityLevel = m_poJingJie->GetJingjieQualityLevel();
    }

    BOOL bAlreadyRecuit = FALSE;
    if( EHP_RECRUIT != poHero->GetDT_HERO_BASE_DATA().byPos )
    {
        bAlreadyRecuit = TRUE;
    }

    if( poHero->GetDT_HERO_BASE_DATA().byCoach )
    {
        //若主角则返回相应错误
        return ERR_GM_PARM_INFO::ID_CANNOT_RECUIT_COACH;
    }

    //如果已经携带则不判断可携带数目
    if( !bAlreadyRecuit )
    {
        //判断可携带数目
        DT_INSTANCE_ID& stInstanceID = m_poInstance->GetMaxCommonInstanceID();
        SHeroUnlockProp* pstHeroUnlockProp = CInstancePropMgr::Instance()->GetUnlockHero(stInstanceID.wSceneIdx, stInstanceID.byTownIdx, stInstanceID.byInstanceIdx);
        if(NULL == pstHeroUnlockProp)
        {
            RETURN_OTHER_ERR;
        }
        if(CkCarryHeroFull())
        {
            if (qwIndex == 0)
            {
                //如果伙伴上限了就直接给他摇点满
                SHeroProp* pstHeroProp = poHero->GetHeroProp();
                if(NULL != pstHeroProp)
                {
                     poHero->GetDT_HERO_BASE_DATA().byVisitNum = pstHeroProp->byVisitNum;
                }
                return ERR_GM_PARM_INFO::ID_UPPER_LIMIT_ERR;
            }
            else
            {
                GMDisbang();
            }
        }
    }

    SHeroProp* pstHeroProp = poHero->GetHeroProp();
    if(NULL == pstHeroProp)
    {
        RETURN_OTHER_ERR;
    }

    poHero->GetDT_HERO_BASE_DATA().byVisitNum = pstHeroProp->byVisitNum;
    
    if( !bAlreadyRecuit )
    {
        //招募,携带
        m_mapKind2HeroAllCarry.AddData(wHeroKindID, poHero);
        poHero->OnRecuit();

        CTaskMgr::Instance()->OnRecruitHero(this, wHeroKindID);

        CTaskMgr::Instance()->OnRecruitHeroNum(this, wHeroKindID);
    }

    poHero->InitBattleAttribute();


    //记录招募引导
    AddGuideRecord( EGF_RECUIT);
    //判断是否需布阵
    CKGuide( EGF_EMBATTLE);
    //3个武将时2次布阵引导
    if(m_mapKind2HeroAllCarry.Size() == 3)
    {
        //判断是否需2次布阵引导
        CKGuide( EGF_SECOND_EMBATTLE);
    }

    return ERR_GM_PARM_INFO::ID_SUCCESS;
}

VOID CPlayer::GMDisbang()
{
    map<UINT16, UINT16> mapAlreadyIn;
    //先写入主角的信息
    mapAlreadyIn[m_poCoachHero->GetDT_HERO_BASE_DATA().wKindID] = 1;

    for(CHero* poHero = m_mapFormationHero.GetFistData(); poHero != NULL; poHero = m_mapFormationHero.GetNextData())
    {
        if( NULL == poHero )
        {
            //找不到指定类型的伙伴是有可能的，因为玩家身上未必携带该伙伴
            continue;
        }

        //如果已经添加存在，则不添加
        map<UINT16, UINT16>::iterator itr = mapAlreadyIn.find(poHero->GetDT_HERO_BASE_DATA().wKindID);
        if( itr != mapAlreadyIn.end() )
        {
            continue;
        }

        mapAlreadyIn[poHero->GetDT_HERO_BASE_DATA().wKindID] = 1;
    }

    //包括主角自己
    UINT16 wHeroNum = m_mapKind2HeroAllCarry.Size();

    map<UINT16, UINT16> mapNotAlready;
    for( std::size_t nHeroIndex = 0; nHeroIndex < m_mapKind2HeroAllCarry.Size(); ++nHeroIndex )
    {
        CHero* poHero = m_mapKind2HeroAllCarry.GetDataByIndex( nHeroIndex );

        if( NULL == poHero )
        {
            //找不到指定类型的伙伴是有可能的，因为玩家身上未必携带该伙伴
            continue;
        }

        //如果已经添加存在，则不添加
        map<UINT16, UINT16>::iterator itr = mapAlreadyIn.find(poHero->GetDT_HERO_BASE_DATA().wKindID);
        if( itr != mapAlreadyIn.end() )
        {
            continue;
        }

        mapNotAlready[poHero->GetDT_HERO_BASE_DATA().wLevel] = poHero->GetDT_HERO_BASE_DATA().wKindID;
    }

    UINT16 wKindID = 0;

    map<UINT16, UINT16>::iterator itr = mapNotAlready.begin();
    if (itr != mapNotAlready.end())
    {
        wKindID = itr->second;
    }
    CHero* poHeroNew = GetHero(wKindID);
    if(NULL == poHeroNew)
    {
        return;
    }

    //清除携带/上阵
    m_mapKind2HeroAllCarry.DelData(wKindID);
    m_mapFormationHero.DelData(poHeroNew->GetDT_HERO_BASE_DATA().byFormationIdx);

    m_apoFormationObj[poHeroNew->GetDT_HERO_BASE_DATA().byFormationIdx] = NULL;
    if (m_poFormation)
    {
        m_poFormation->OnDisband(wKindID);
    }

    poHeroNew->OnDisband();

    //加入招募馆中
    poHeroNew->GetDT_HERO_BASE_DATA().byPos = EHP_RECRUIT;

    //重新计算属性
    ResetBattleAttr();

}

UINT16 CPlayer::OpenPlayerInfoSkill(PKT_CLIGS_PLAYER_INFO_SKILL_ACK& stAck)
{
	UINT8 byHeroNum = 0;
	set<CHero*> setHero;
	CHero* poHero = GetCoachHero();
	if ( NULL == poHero)
	{
		return ERR_PLAYER_INFO_SKILL::ID_COACH_HERO_LOST;
	}
	stAck.astSkillInfo[byHeroNum].bySkillNum = 1;
	poHero->GetHeroSkillLstCli(stAck.astSkillInfo[byHeroNum]);
	setHero.insert(poHero);
	byHeroNum++;

	poHero = m_mapFormationHero.GetFistData();
	while ( poHero)
	{
		if (setHero.end() != setHero.find(poHero))
		{
			poHero = m_mapFormationHero.GetNextData();
			continue;
		}
		stAck.astSkillInfo[byHeroNum].bySkillNum = 1;
		poHero->GetHeroSkillLstCli(stAck.astSkillInfo[byHeroNum]);
		setHero.insert(poHero);
		byHeroNum++;
		poHero = m_mapFormationHero.GetNextData();
	}

	poHero = m_mapKind2HeroAllCarry.GetFistData();
	while ( poHero)
	{
		if (setHero.end() != setHero.find(poHero))
		{
			poHero = m_mapKind2HeroAllCarry.GetNextData();
			continue;
		}
		stAck.astSkillInfo[byHeroNum].bySkillNum = 1;
		poHero->GetHeroSkillLstCli(stAck.astSkillInfo[byHeroNum]);
		setHero.insert(poHero);
		byHeroNum++;
		poHero = m_mapKind2HeroAllCarry.GetNextData();
	}

	stAck.byHeroNum = byHeroNum;
	return ERR_PLAYER_INFO_SKILL::ID_SUCCESS;
}

VOID CPlayer::GetAllCarryHeroAttr(DT_HERO_ATTR_LST_INFO& stAttrInfo, UINT8* abyJobType)
{
	vector<CHero*> vecHero;
	GetPlayerAllHero(vecHero);

	UINT32 unIndex = 0;
	for (vector<CHero*>::iterator itor = vecHero.begin(), end = vecHero.end()
		; end != itor
		; ++ itor)
	{
		CHero* pHero = *(itor);
		if (NULL != pHero) {
			stAttrInfo.awHeroID[unIndex] = pHero->GetHeroID();
			stAttrInfo.adwPower[unIndex] = pHero->GetPower();
			stAttrInfo.astBaseAttr[unIndex] = CBattleMgr::Instance()->ConvertAttribute2ToBase(pHero->GetDT_BATTLE_ATTRIBUTE());
			stAttrInfo.astHideAttr[unIndex] = pHero->GetDT_HIDE_BATTLE_ATTRIBUTE();
			stAttrInfo.awTalent[unIndex] = pHero->GetTalent();
			stAttrInfo.awLevel[unIndex] = pHero->GetDT_HERO_BASE_DATA().wLevel;
			SHeroProp* pHeroProp = CHeroPropMgr::Instance()->GetHeroProp(pHero->GetHeroID());
			if (NULL != pHeroProp) {
				abyJobType[unIndex] = pHeroProp->byCareerID;
			} else {
				abyJobType[unIndex] = 1;
			}
		}

		++unIndex;
	}

	stAttrInfo.wHeroNum = unIndex;
}

UINT16 CPlayer::GetPlayerInfoEquip(PKT_CLIGS_PLAYER_INFO_EQUIP_ACK& stAck)
{
	set<CHero*> setHero;
	CHero* poHero = GetCoachHero();
	UINT8 byHeroNum = 0;
	if(NULL == poHero)
	{
		return ERR_PLAYER_INFO_EQUIP::ID_COACH_HERO_LOST;
	}
	poHero->GetEquip( stAck.astEquipDataList[byHeroNum]);
	poHero->GetDT_HERO_BASE_DATA_CLI(stAck.astHeroBaseData[byHeroNum]);
	byHeroNum++;
	setHero.insert(poHero);

	poHero = m_mapFormationHero.GetFistData();
	while( poHero )
	{
		if(setHero.find(poHero) != setHero.end() )
		{
			poHero = m_mapFormationHero.GetNextData();
			continue;
		}
		poHero->GetEquip(stAck.astEquipDataList[byHeroNum]);
		poHero->GetDT_HERO_BASE_DATA_CLI(stAck.astHeroBaseData[byHeroNum]);
		byHeroNum++;
		setHero.insert(poHero);
		poHero = m_mapFormationHero.GetNextData();
	}

	poHero = m_mapKind2HeroAllCarry.GetFistData();
	while( poHero )
	{
		if(setHero.find(poHero) != setHero.end() )
		{
			poHero = m_mapKind2HeroAllCarry.GetNextData();
			continue;
		}
		poHero->GetEquip(stAck.astEquipDataList[byHeroNum]);
		poHero->GetDT_HERO_BASE_DATA_CLI(stAck.astHeroBaseData[byHeroNum]);
		byHeroNum++;
		setHero.insert(poHero);
		poHero = m_mapKind2HeroAllCarry.GetNextData();
	}
	stAck.byHeroNum = byHeroNum;
	stAck.dwPlayerID = m_stDT_PLAYER_BASE_DATA.dwID;
	stAck.byVipLevel = m_stDT_PLAYER_BASE_DATA.byVipLevel;

	CXiangqianMgr::Instance()->GetXiangqianEquipDes( stAck.stEquipXiangqianDes.aszEquipPos1Des, stAck.stEquipXiangqianDes.aszEquipPos2Des,
		stAck.stEquipXiangqianDes.aszEquipPos3Des, stAck.stEquipXiangqianDes.aszEquipPos4Des );
	return ERR_PLAYER_INFO_EQUIP::ID_SUCCESS;
}

UINT16 CPlayer::JewelryCompare(PKT_CLIGS_JEWELRY_COMPARE_REQ& stReq, PKT_CLIGS_JEWELRY_COMPARE_ACK& stAck)
{
	CJewelry* poJewelry = GetJewelry( stReq.wJewelryID, stReq.wJewelryIdx);
	if ( NULL == poJewelry)
	{
		return ERR_JEWELRY_COMPARE::ID_NOT_FIND_JEWELRY;
	}
	CJewelry* poLowJewelry = NULL;
	UINT8 byPosID = poJewelry->GetDT_JEWELRY_DATA().byJewelryDressPos;
	poJewelry->GetJewelryDataCli(stAck.stCompareJewelryInfo);

	CHero* poHero = GetCoachHero();
	if ( NULL == poHero)
	{
		return ERR_JEWELRY_COMPARE::ID_COACH_HERO_LOST;
	}
	CJewelry* poCurJewelry = poHero->GetJewelryByPos(byPosID);
	if ( NULL != poCurJewelry)
	{
		poLowJewelry = poCurJewelry;
	}
	set<CHero*> setHero;
	setHero.insert(poHero);
	poHero = m_mapFormationHero.GetFistData();
	while(poHero)
	{
		if ( setHero.end() != setHero.find(poHero))
		{
			poHero = m_mapFormationHero.GetNextData();
			continue;
		}
		setHero.insert(poHero);
		poCurJewelry = poHero->GetJewelryByPos(byPosID);
		poLowJewelry = CJewelryPropMgr::Instance()->CompareJewelry(poLowJewelry, poCurJewelry);
		
		poHero = m_mapFormationHero.GetNextData();
	}


	if ( NULL != poLowJewelry)
	{
		poLowJewelry->GetJewelryDataCli(stAck.stDressJewelryInfo);
	}

	return ERR_JEWELRY_COMPARE::ID_SUCCESS;
}

VOID CPlayer::GetHeroPanelLstCli(DT_HERO_PANEL_LST_CLI& stHeroLstCli)
{
	CHero* poHero = GetCoachHero();
	if ( NULL == poHero)
	{
		return ;
	}
	set<CHero*> setHero;
	UINT8 byHeroNum = 0;
	poHero->GetHeroPanelInfo(stHeroLstCli.astHeroPanelInfo[byHeroNum++]);
	setHero.insert(poHero);
	poHero = m_mapFormationHero.GetFistData();
	while(poHero)
	{
		if ( setHero.end() != setHero.find(poHero))
		{
			poHero = m_mapFormationHero.GetNextData();
			continue;
		}
		poHero->GetHeroPanelInfo( stHeroLstCli.astHeroPanelInfo[byHeroNum++] );
		setHero.insert(poHero);
		poHero = m_mapFormationHero.GetNextData();
	}

	poHero = m_mapKind2HeroAllCarry.GetFistData();

	while(poHero)
	{
		if ( setHero.end() != setHero.find(poHero))
		{
			poHero = m_mapKind2HeroAllCarry.GetNextData();
			continue;
		}
		poHero->GetHeroPanelInfo(stHeroLstCli.astHeroPanelInfo[byHeroNum++]);
		setHero.insert(poHero);
		poHero = m_mapKind2HeroAllCarry.GetNextData();
	}

	stHeroLstCli.wHeroNum = byHeroNum;
}
//set会进行排序，不能按指定顺序下发，用vector比较好
VOID CPlayer::GetPlayerAllHero(vector<CHero*>& vecHero) {
	vecHero.clear();
	CHero* poHero = GetCoachHero();
	if (NULL != poHero) {
		vecHero.push_back(poHero);
	}

	poHero = m_mapFormationHero.GetFistData();
	while (NULL != poHero) {
		if (find(vecHero.begin(), vecHero.end(), poHero) != vecHero.end()) {
			poHero = m_mapFormationHero.GetNextData();
			continue;
		}
		vecHero.push_back(poHero);
		poHero = m_mapFormationHero.GetNextData();
	}

	poHero = m_mapKind2HeroAllCarry.GetFistData();
	while (NULL != poHero) {
		if (find(vecHero.begin(), vecHero.end(), poHero) != vecHero.end()) {
			poHero = m_mapKind2HeroAllCarry.GetNextData();
			continue;
		}

		vecHero.push_back(poHero);
		poHero = m_mapKind2HeroAllCarry.GetNextData();
	}
}
