#include "collectgas.h"
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <common/server/utility.h>
#include <db/autosqlbuf.h>
#include "logic/record/recordmgr.h"
#include "logic/task/taskmgr.h"
#include "logic/vip/vippropmgr.h"
#include <logic/lua/luamgr.h>
#include "logic/skill/collectgaspropmgr.h"
#include "logic/instance/instancepropmgr.h"
#include <logic/other/errmsgmgr.h>
#include "logic/instance/instance.h"
#include <sdloggerimpl.h>
#include "logic/medal/playermedal.h"

IMPLEMENT_CLASS_POOL_MGR(CCollectGas);
CCollectGas::CCollectGas()
{
    memset(&m_stCollectData, 0, sizeof(m_stCollectData));
}


CCollectGas::~CCollectGas()
{

}

BOOL CCollectGas::Init(DT_COLLECT_GAS_DATA& stCollectData, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    memcpy(&m_stCollectData, &stCollectData, sizeof(DT_COLLECT_GAS_DATA));

    m_poOwner = poOwner;

    return TRUE;
}


UINT16 CCollectGas::GenGas(UINT8 byFlag,  DT_GEN_GAS_DATA_CLI& DT_GEN_GAS_DATA_CLI)
{
    if(!m_poOwner->GetBuild(EBK_COLLECTGAS))
    {
        return ERR_GEN_GAS_BALL::ID_COLLECT_GAS_UNLOCK;
    }
    Recover();

    //如果当前处理收集状态，直接返回成功
    if (1 == m_stCollectData.byCollectState)
    {
        GetDT_GEN_GAS_DATA_CLI(DT_GEN_GAS_DATA_CLI);
        return ERR_GEN_GAS_BALL::ID_SUCCESS;
    }

    if(0 == CanGenNum())
    {
        GetDT_GEN_GAS_DATA_CLI(DT_GEN_GAS_DATA_CLI);
        return ERR_GEN_GAS_BALL::ID_REACH_MAX_NUM;
    }

    if (0 == byFlag)
    {
        UINT32 dwCostCoin = 0;
        dwCostCoin = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_GENCOIN, m_stCollectData.wLastCoinGenNum, 0);

        if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin < dwCostCoin)
        {
            GetDT_GEN_GAS_DATA_CLI(DT_GEN_GAS_DATA_CLI);
            return ERR_COMMON::ID_COIN_NOT_ENOUGH;
        }
        m_poOwner->DecCoin(dwCostCoin, CRecordMgr::EDCT_GENGAS);
        m_stCollectData.wLastCoinGenNum++;
        m_stCollectData.qwLastCoinGenTime = SDTimeSecs();
		CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
		if ( NULL != poPlayerMedal)
		{
			poPlayerMedal->CheckMedalBySmallType( EMST_GATHER_SCIENCE_GOLD);
		}
    }
    else
    {
        if(!CVipPropMgr::Instance()->CkOpenFunc(EVF_GENGAS, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel))
        {
            return ERR_GEN_GAS_BALL::ID_VIP_NOT_ENOUGH_UNLOCK;
        }

        UINT32 dwCostGold = 0;
        dwCostGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_GENGOLD, m_stCollectData.wLastGoldGenNum, 0);

        if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < dwCostGold)
        {
            GetDT_GEN_GAS_DATA_CLI(DT_GEN_GAS_DATA_CLI);
            return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
        }
        m_poOwner->DecGold(dwCostGold, CRecordMgr::EDCT_GENGAS);
        m_stCollectData.wLastGoldGenNum++;
        m_stCollectData.qwLastGoldGenTime = SDTimeSecs();
		CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
		if ( NULL != poPlayerMedal)
		{
			poPlayerMedal->CheckMedalBySmallType( EMST_GATHER_SCIENCE_GOLD);
		}
    }

    for(UINT8 byIdx = 0; byIdx < MAX_GAS_BALL_NUM; byIdx++)
    {
        CCollectGasPropMgr::Instance()->GenGas(m_poOwner, byFlag, m_stCollectData.astGasBollInfo[byIdx]);
    }
    m_stCollectData.byCollectState = 1; //收集状态,0生成，1收取
    GetDT_GEN_GAS_DATA_CLI(DT_GEN_GAS_DATA_CLI);
    return ERR_GEN_GAS_BALL::ID_SUCCESS;
}


UINT16 CCollectGas::UpGasLevel(UINT8 byFullLevelFlag, DT_GEN_GAS_DATA_CLI& DT_GEN_GAS_DATA_CLI)
{
    if(!m_poOwner->GetBuild(EBK_COLLECTGAS))
    {
        return ERR_UP_GAS_BALL_LEVEL::ID_COLLECT_GAS_UNLOCK;
    }
    Recover();
    if(0 == m_stCollectData.byCollectState)
    {
        GetDT_GEN_GAS_DATA_CLI(DT_GEN_GAS_DATA_CLI);
        return ERR_UP_GAS_BALL_LEVEL::ID_UN_GEN;
    }

    if (m_stCollectData.byLastGoldFullUpNum > 0)
    {
        GetDT_GEN_GAS_DATA_CLI(DT_GEN_GAS_DATA_CLI);
        return ERR_UP_GAS_BALL_LEVEL::ID_COLLECT_GAS_FULL;
    }


    UINT32 dwCostGold = 0;
    if(byFullLevelFlag)
    {
        if(!CVipPropMgr::Instance()->CkOpenFunc(EVF_GENGASFULL, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel))
        {
            return ERR_UP_GAS_BALL_LEVEL::ID_VIP_NOT_ENOUGH_UNLOCK;
        }

        dwCostGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_GENUPGOLDFULL, 0);
    }
    else
    {
        UINT16 wTotalFreeCommonUpGasNum = CLuamgr::Instance()->GetTotalFreeCommonUpGasNum();
        if(m_stCollectData.wLastGoldRandomUpNum < wTotalFreeCommonUpGasNum)
        {
            dwCostGold = 0;
        }
        else
        {
            dwCostGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_GENUPGOLD, m_stCollectData.wLastGoldRandomUpNum);
        }
    }

    if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < dwCostGold)
    {
        GetDT_GEN_GAS_DATA_CLI(DT_GEN_GAS_DATA_CLI);
        return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
    }

    for(UINT8 byIdx = 0; byIdx < MAX_GAS_BALL_NUM; byIdx++)
    {
        CCollectGasPropMgr::Instance()->UpGasBallLevel(m_poOwner, byFullLevelFlag, m_stCollectData.astGasBollInfo[byIdx]);
    }

    m_poOwner->DecGold(dwCostGold, CRecordMgr::EDGT_UPGASLEVEL);
    if(byFullLevelFlag)
    {
        m_stCollectData.byLastGoldFullUpNum++;
        m_stCollectData.qwLastGoldFullUpTime = SDTimeSecs();
    }
    else
    {
        m_stCollectData.wLastGoldRandomUpNum++;
        m_stCollectData.qwLastGoldRandomUpTime = SDTimeSecs();
    }

    GetDT_GEN_GAS_DATA_CLI(DT_GEN_GAS_DATA_CLI);
    return ERR_UP_GAS_BALL_LEVEL::ID_SUCCESS;
}

UINT16 CCollectGas::CollectGas(DT_GEN_GAS_DATA_CLI& DT_GEN_GAS_DATA_CLI)
{
    if(!m_poOwner->GetBuild(EBK_COLLECTGAS))
    {
        return ERR_COLLECT_GAS::ID_COLLECT_GAS_UNLOCK;
    }
    Recover();
    //引导阶段
    if(m_poOwner->CKGuide(EGF_GASCOLLECT))
    {
        UINT32  dwCollectGas = CCollectGasPropMgr::Instance()->GetGuidePurpleGas(m_poOwner);
        m_poOwner->AddScience(dwCollectGas, CRecordMgr::EAST_GAS );
        m_poOwner->AddGuideRecord(EGF_GASCOLLECT);
    }
    else
    {
        if(0 == m_stCollectData.byCollectState)
        {
            GetDT_GEN_GAS_DATA_CLI(DT_GEN_GAS_DATA_CLI);
            return ERR_COLLECT_GAS::ID_UN_GEN;
        }

        for(UINT8 byIdx = 0; byIdx < MAX_GAS_BALL_NUM; byIdx++)
        {
            DT_GAS_BALL_DATA& stDT_GAS_BALL_DATA = m_stCollectData.astGasBollInfo[byIdx];
            UINT32 dwGetGas = CCollectGasPropMgr::Instance()->GetGas(m_poOwner, stDT_GAS_BALL_DATA.byKind, stDT_GAS_BALL_DATA.byLevel);
            m_poOwner->AddScience(dwGetGas, CRecordMgr::EAST_GAS, stDT_GAS_BALL_DATA.byLevel);
        }
    }
    m_stCollectData.byCollectState = 0; //修改状态0生成
    memset(&m_stCollectData.astGasBollInfo, 0x00, sizeof(DT_GAS_BALL_DATA) * MAX_GAS_BALL_NUM);
    GetDT_GEN_GAS_DATA_CLI(DT_GEN_GAS_DATA_CLI);

    m_stCollectData.wLastGoldRandomUpNum = 0;
    m_stCollectData.qwLastGoldRandomUpTime = 0;

    m_stCollectData.byLastGoldFullUpNum = 0;
    m_stCollectData.qwLastGoldFullUpTime = 0;
    return ERR_COLLECT_GAS::ID_SUCCESS;
}

//战胜精英XXX，每天聚魂次数增至XXx次

//已战胜全部精英BOSS
UINT16 CCollectGas::OpenGenGasTab(PKT_CLIGS_OPEN_GEN_GAS_TAB_ACK& stAck)
{
    if(!m_poOwner->GetBuild(EBK_COLLECTGAS))
    {
        return ERR_OPEN_GEN_GAS_TAB::ID_COLLECT_GAS_UNLOCK;
    }

    Recover();
    GetDT_GEN_GAS_DATA_CLI(stAck.stGenGasInfo);

    DT_INSTANCE_ID &stDT_INSTANCE_ID = m_poOwner->GetInstance().GetMaxEliteInstanceID();
    SGasNumProp * poGasNumProp = CCollectGasPropMgr::Instance()->GetNextGenGasNumProp((UINT8)m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, stDT_INSTANCE_ID.byTownIdx, stDT_INSTANCE_ID.byInstanceIdx);
    if (NULL == poGasNumProp)
    {
        SDStrcpy(stAck.aszTipsInfo, CMsgDefMgr::Instance()->GetErrMsg("HAVE_WIN_ALL_ELITE") .c_str());
        return ERR_OPEN_GEN_GAS_TAB::ID_SUCCESS;
    }
    //获取副本信息
    SInstanceProp *poInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(EIT_ELITE, poGasNumProp->byTownIdx, poGasNumProp->byInstanceIdx);
    if (NULL == poInstanceProp)
    {
        SDStrcpy(stAck.aszTipsInfo, CMsgDefMgr::Instance()->GetErrMsg("HAVE_WIN_ALL_ELITE") .c_str());
        SYS_CRITICAL(_SDT("[%s: %d]: OpenGenGasTab configure failed!"), MSG_MARK); //配置出错，当前配置的副本在精武英雄副本中找不到

        return ERR_OPEN_GEN_GAS_TAB::ID_SUCCESS;
    }

    //CHAR	szNum[56] = {0};
    //sprintf(szNum, "%u", poGasNumProp->byValue);
    //SDStrcat(stAck.aszTipsInfo, SDGBK2UTF8("战胜精英").c_str());
    //SDStrcat(stAck.aszTipsInfo, poInstanceProp->strName.c_str());
    //SDStrcat(stAck.aszTipsInfo, SDGBK2UTF8("，每天聚魂次数增至").c_str());
    //SDStrcat(stAck.aszTipsInfo, SDGBK2UTF8(szNum).c_str());
    //SDStrcat(stAck.aszTipsInfo, SDGBK2UTF8("次").c_str());

    vector<string> vecFillMsg;
    CHAR szBuff[32] = {0};
    sprintf(szBuff, "%s", poInstanceProp->strName.c_str());
    vecFillMsg.push_back(szBuff);
    sprintf(szBuff, "%u", poGasNumProp->byValue);
    vecFillMsg.push_back(szBuff);
    SDStrncpy(stAck.aszTipsInfo, CMsgDefMgr::Instance()->GetErrMsg("OPEN_GEN_GAS_PROMPT", &vecFillMsg).c_str(), MAX_GEN_GAS_INFO_LEN - 1);

    return ERR_OPEN_GEN_GAS_TAB::ID_SUCCESS;
}

UINT16 CCollectGas::CanGenNum(/*UINT8 byPurpleFlag*/)
{
    UINT16 wMaxGenNum = GetMaxGenNum();
    if(m_stCollectData.wLastCoinGenNum + m_stCollectData.wLastGoldGenNum >= wMaxGenNum)
    {
        return 0;
    }
    else
    {
        return wMaxGenNum - m_stCollectData.wLastCoinGenNum - m_stCollectData.wLastGoldGenNum;
    }
}

VOID CCollectGas::GetDT_GEN_GAS_DATA_CLI(DT_GEN_GAS_DATA_CLI& DT_GEN_GAS_DATA_CLI)
{
    if (0 == m_stCollectData.byCollectState)
    {
        memset(&m_stCollectData.astGasBollInfo, 0x00, sizeof(DT_GEN_GAS_DATA_CLI.astGasBollInfo));
    }
    DT_GEN_GAS_DATA_CLI.byCollectState = m_stCollectData.byCollectState;
    memcpy(&DT_GEN_GAS_DATA_CLI.astGasBollInfo, &m_stCollectData.astGasBollInfo, sizeof(DT_GEN_GAS_DATA_CLI.astGasBollInfo));
    DT_GEN_GAS_DATA_CLI.wCanGenNum = CanGenNum();
    DT_GEN_GAS_DATA_CLI.wTotalGenNum = GetMaxGenNum();
    DT_GEN_GAS_DATA_CLI.dwGenGasCoin = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_GENCOIN, m_stCollectData.wLastCoinGenNum, 0);
    DT_GEN_GAS_DATA_CLI.dwGenGasGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_GENGOLD, m_stCollectData.wLastGoldGenNum, 0);
    DT_GEN_GAS_DATA_CLI.dwFullLevelUpGasGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_GENUPGOLDFULL, 0, 0);
    DT_GEN_GAS_DATA_CLI.wTotalFreeCommonUpGasNum = CLuamgr::Instance()->GetTotalFreeCommonUpGasNum();
    if(m_stCollectData.wLastGoldRandomUpNum < DT_GEN_GAS_DATA_CLI.wTotalFreeCommonUpGasNum)
    {
        DT_GEN_GAS_DATA_CLI.wCanFreeCommonUpGasNum = DT_GEN_GAS_DATA_CLI.wTotalFreeCommonUpGasNum - m_stCollectData.wLastGoldRandomUpNum;
        DT_GEN_GAS_DATA_CLI.dwCommonUpGasGold = 0;
    }
    else
    {
        DT_GEN_GAS_DATA_CLI.wCanFreeCommonUpGasNum = 0;
        DT_GEN_GAS_DATA_CLI.dwCommonUpGasGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_GENUPGOLD, m_stCollectData.wLastGoldRandomUpNum);
    }
}


UINT16  CCollectGas::GetMaxGenNum()
{
    //return CVipPropMgr::Instance()->GetIncNum( EVINF_GENPURPLEGAS, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel );
    DT_INSTANCE_ID &stDT_INSTANCE_ID = m_poOwner->GetInstance().GetMaxEliteInstanceID();
    return CCollectGasPropMgr::Instance()->GetGenGasNum((UINT8)m_poOwner->GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, stDT_INSTANCE_ID.byTownIdx, stDT_INSTANCE_ID.byInstanceIdx);
}

VOID	 CCollectGas::Recover()
{
    if (!IsToday(m_stCollectData.qwLastCoinGenTime))
    {
        m_stCollectData.wLastCoinGenNum = 0;
        m_stCollectData.qwLastCoinGenTime = 0;
    }

    if (!IsToday(m_stCollectData.qwLastCoinRandomUpTime))
    {
        m_stCollectData.wLastCoinRandomUpNum = 0;
        m_stCollectData.qwLastCoinRandomUpTime = 0;
    }


    if (!IsToday(m_stCollectData.qwLastGoldGenTime))
    {
        m_stCollectData.wLastGoldGenNum = 0;
        m_stCollectData.qwLastGoldGenTime = 0;
    }

    if (!IsToday(m_stCollectData.qwLastGoldRandomUpTime))
    {
        m_stCollectData.wLastGoldRandomUpNum = 0;
        m_stCollectData.qwLastGoldRandomUpTime = 0;
    }

    if (!IsToday(m_stCollectData.qwLastGoldFullUpTime))
    {
        m_stCollectData.byLastGoldFullUpNum = 0;
        m_stCollectData.qwLastGoldFullUpTime = 0;
    }
}

