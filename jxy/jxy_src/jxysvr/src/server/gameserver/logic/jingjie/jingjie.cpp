#include "jingjie.h"
#include "jingjiepropmgr.h"
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <common/server/utility.h>
#include <db/autosqlbuf.h>
#include "logic/record/recordmgr.h"
#include "logic/task/taskmgr.h"
#include "logic/vip/vipextdata.h"
#include <logic/lua/luamgr.h>
#include <logic/hero/hero.h>
#include "logic/hero/heropropmgr.h"
#include "logic/skill/skill.h"
#include "logic/skill/skillpropmgr.h"

IMPLEMENT_CLASS_POOL_MGR(CJingJie)

CJingJie::CJingJie()
{
    memset(&m_stDT_JINGJIE_DATA, 0, sizeof(m_stDT_JINGJIE_DATA));
    m_mapSkillActionData.clear();
}


CJingJie::~CJingJie()
{

}

BOOL CJingJie::Init(DT_JINGJIE_DATA& stJingJieData, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    memcpy(&m_stDT_JINGJIE_DATA, &stJingJieData, sizeof(DT_JINGJIE_DATA));
    DT_SKILL_ACTION_DATA_LIST& stSkillActionData = m_stDT_JINGJIE_DATA.stSkillActionData;
    UINT8 byActionNum = stSkillActionData.byActionNum;
    for(UINT8 byIdx = 0; byIdx < byActionNum; byIdx++)
    {
        m_mapSkillActionData[stSkillActionData.astActionInfo[byIdx].wID] = &stSkillActionData.astActionInfo[byIdx];
    }


    //todo, 判断补全已解锁招式及效果

    m_poOwner = poOwner;

    return TRUE;
}

BOOL	CJingJie::SetJingJieLevel(UINT16 wLevel, UINT16 wSubLevel)
{
    if (wLevel > CJingJiePropMgr::Instance()->GetMaxQuality())
    {
        return FALSE;
    }

    if (wSubLevel > CJingJiePropMgr::Instance()->GetSubLevelNum())
    {
        return FALSE;
    }

    DT_JINGJIE_BASE_DATA& stBaseData = m_stDT_JINGJIE_DATA.stBaseData;
    stBaseData.wLevel = wLevel;
    stBaseData.wSubLevel = wSubLevel;
    return TRUE;
}



UINT16 CJingJie::Upgrade(UINT16 wSubLevel, DT_JINGJIE_DATA_CLI& stDT_JINGJIE_CLI)
{
    DT_JINGJIE_BASE_DATA& stBaseData = m_stDT_JINGJIE_DATA.stBaseData;

    memcpy(&stDT_JINGJIE_CLI.stBaseData, &stBaseData, sizeof(stBaseData));

    UINT16 wSubLevelNum = CJingJiePropMgr::Instance()->GetSubLevelNum();
    UINT16 wMaxQualityLevel = CJingJiePropMgr::Instance()->GetMaxQuality();
    if((stBaseData.wLevel > wMaxQualityLevel) ||
            ((stBaseData.wLevel == wMaxQualityLevel) && (stBaseData.wSubLevel >= wSubLevelNum)))
    {
        return ERR_UPGRADE_JINGJIE::ID_MAX_QUALITY_LEVEL;
    }

    //请求等级小于当前等级，直接返回成功
    if(wSubLevel <= stBaseData.wSubLevel)
    {
        return ERR_UPGRADE_JINGJIE::ID_SUCCESS;
    }

    if(wSubLevel > CJingJiePropMgr::Instance()->GetSubLevelNum())
    {
        return ERR_UPGRADE_JINGJIE::ID_LEVEL_INVALID;
    }

    if(wSubLevel == stBaseData.wSubLevel + 1)
    {
        UINT32 dwCostGas = CJingJiePropMgr::Instance()->GetJingJieSubLevelUpgradeGas( m_poOwner->GetCoachCareerID(), stBaseData.wLevel, wSubLevel);
        //引导阶段，消耗0
        if(m_poOwner->CKGuide(EGF_JINGJIEUPGRADE))
        {
            //消耗境界点
            dwCostGas = dwCostGas > (UINT32)(m_poOwner->GetDT_PLAYER_BASE_DATA().qwJingJie) ? (UINT32)(m_poOwner->GetDT_PLAYER_BASE_DATA().qwJingJie) : dwCostGas;
            //dwCostGas = dwCostGas > (UINT32)(m_poOwner->GetDT_PLAYER_BASE_DATA().qwBlueGas) ? (UINT32)(m_poOwner->GetDT_PLAYER_BASE_DATA().qwBlueGas) : dwCostGas;
        }
        else
        {
            //消耗境界点
            //if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwBlueGas < dwCostGas)
            if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwJingJie < dwCostGas)
            {
                return ERR_UPGRADE_JINGJIE::ID_NOT_ENOUGH_JINGJIE;
                /*if(m_poOwner->GetBuild(EBK_COLLECTGAS))
                {
                	return ERR_UPGRADE_JINGJIE::ID_NOT_ENOUGH_BLUEGAS1;
                }
                else
                {
                	return ERR_UPGRADE_JINGJIE::ID_NOT_ENOUGH_BLUEGAS2;
                }*/
            }
        }

        if(0 != dwCostGas)
        {
            //消耗境界点
            //m_poOwner->DecBlueGas(dwCostGas, CRecordMgr::EDBGT_UPGRADESKILLACTION);
            m_poOwner->DecJingJie(dwCostGas, CRecordMgr::EDJT_UPJINGJIE, stBaseData.byCareerID, stBaseData.wLevel, stBaseData.wSubLevel);
        }

        //升级
        stBaseData.wSubLevel++;
        if((stBaseData.wSubLevel >= wSubLevelNum) && (stBaseData.wLevel < wMaxQualityLevel)) //升阶
        {
            stBaseData.wLevel++;
            stBaseData.wSubLevel = 0;
        }
        m_poOwner->AddGuideRecord( EGF_JINGJIEUPGRADE);
        OnJingjieQualityUpgrade();
        //重置玩家属性
        m_poOwner->ResetBattleAttr();

    }
    else
    {
        return ERR_UPGRADE_JINGJIE::ID_LEVEL_INVALID;
    }
    CTaskMgr::Instance()->OnUpgradeJingJie(m_poOwner, stBaseData.wLevel, stBaseData.wSubLevel);
    GetDT_JINGJIE_CLI(stDT_JINGJIE_CLI);
    return ERR_UPGRADE_JINGJIE::ID_SUCCESS;
}

const SAllAttrProp* CJingJie::GetAllIncAttr()
{
    return CJingJiePropMgr::Instance()->GetAllIncAttr(m_poOwner->GetCoachCareerID(), m_stDT_JINGJIE_DATA.stBaseData.wLevel, m_stDT_JINGJIE_DATA.stBaseData.wSubLevel);
}


UINT16 CJingJie::OpenJingJieTab(PKT_CLIGS_OPEN_JINGJIE_TAB_ACK& stAck)
{
    DT_JINGJIE_BASE_DATA& stBaseData = m_stDT_JINGJIE_DATA.stBaseData;
    GetDT_JINGJIE_CLI(stAck.stJingJieInfo);
    stAck.qwCurJingJie = m_poOwner->GetDT_PLAYER_BASE_DATA().qwJingJie;
    stAck.wMaxQualityLevel = CJingJiePropMgr::Instance()->GetMaxQuality();

    return ERR_OPEN_JINGJIE_TAB::ID_SUCCESS;
}


VOID CJingJie::GetDT_JINGJIE_CLI(DT_JINGJIE_DATA_CLI& stDT_JINGJIE_CLI)
{
    //stDT_JINGJIE_CLI.byUnlockActionLevelPerJingJieLevel = CJingJiePropMgr::Instance()->GetUnlockActionLevelPerJingJieLevel();
    DT_JINGJIE_BASE_DATA& stBaseData = m_stDT_JINGJIE_DATA.stBaseData;
    memcpy(&stDT_JINGJIE_CLI.stBaseData, &stBaseData, sizeof(DT_JINGJIE_BASE_DATA));
    CJingJiePropMgr::Instance()->GetJingJiePerLevelData(m_poOwner->GetCoachCareerID(), stBaseData.wLevel, stDT_JINGJIE_CLI.stJingJiePerLevelData);
    CJingJiePropMgr::Instance()->GetAllIncAttr(m_poOwner->GetCoachCareerID(), stBaseData.wLevel, stBaseData.wSubLevel, stDT_JINGJIE_CLI.byIncAttrNum, stDT_JINGJIE_CLI.astIncAttrInfo);
    stDT_JINGJIE_CLI.wCurCoachTalent = m_poOwner->GetCoachHero()->GetTalent();
}






UINT16 CJingJie::GetJingjieQualityLevel()
{
    return m_stDT_JINGJIE_DATA.stBaseData.wLevel;
}

VOID CJingJie::OnJingjieQualityUpgrade()
{
    if(m_poOwner->GetCoachHero())
    {
        m_poOwner->GetCoachHero()->OnJingjieQualityUpgrade();
    }
}

UINT16 CJingJie::GetJingJieLevel()
{
    return m_stDT_JINGJIE_DATA.stBaseData.wLevel;
    return 0;
}




