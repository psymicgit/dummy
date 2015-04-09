#include "begblue.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>
//#include "errdef.h"
//#include "player.h"
//
//#include "logic/other/singleprammgr.h"
//#include <framework/gsapi.h>
//#include "logic/record/recordmgr.h"
#include "logic/lua/luamgr.h"
//#include <sdloggerimpl.h>
#include <logic/other/errmsgmgr.h>
#include <logic/begbluegas/begbluepropmgr.h>
#include "logic/medal/playermedal.h"

const int MAX_BEGBLUE_SMILE_CNT = 6;
const int MAX_BEGBLUE_XN_MULTIPLE = 6;
const int MAX_CLIMB_TOWER_LAYER = 100;

IMPLEMENT_CLASS_POOL_MGR(CBegBlue)
CBegBlue::CBegBlue()
{
    memset(&m_stDT_BEGBLUE_DATA, 0 , sizeof(m_stDT_BEGBLUE_DATA));
}

CBegBlue::~CBegBlue()
{

}

BOOL CBegBlue::Init(DT_BEGBLUE_DATA &stDT_BEGBLUE_DATA, CPlayer* poOwner)
{
    m_poOwner = poOwner;
    memcpy(&m_stDT_BEGBLUE_DATA, &stDT_BEGBLUE_DATA, sizeof(DT_BEGBLUE_DATA));
    ChkAndSetBegBlueInitVal();
    return TRUE;
}

// 打开仙露阁
UINT16 CBegBlue::OpenBluePavilion(DT_BLUE_PAVILION_DATA_CLI &stBluePavilion, UINT8 &byIsIgnoreXnMultiple, DT_BLUE_ENCOURAGEINFO_CLI &stBlueEncourage)
{
    stBluePavilion.byBluePavilionType = FOR_TO_COMFIRM_BEGBLUE;
    CBuild* pBuild = m_poOwner->GetBuild(EBK_BEGBLUE);
    if ( NULL == pBuild)
    {
        return ERR_OPEN_BLUE_PAVILION::ID_BUILD_NOT_EXIST;
    }

    ChkAndSetBegBlueInitVal();

    // update m_stDT_BEGBLUE_DATA
    m_stDT_BEGBLUE_DATA.qwBegBlueLastTime = SGDP::SDTimeSecs();
    GetBegBlueNewestVal(m_stDT_BEGBLUE_DATA.dwMaxBegTimes, m_stDT_BEGBLUE_DATA.aszClimbTowerPrompt);

    m_stDT_BEGBLUE_DATA.byIsIgnoreXnMultiple = CLuamgr::Instance()->GetBegBlueIsIgnoreXnMultiple();

    // 测试:清空祈求结果
    //m_stDT_BEGBLUE_DATA.byIsHaveToComfirmBegBlueInfo = 0;
    //memset(&m_stDT_BEGBLUE_DATA.stBegBlueRes, 0, sizeof(DT_BEGBLUE_RES));

    // assemble stBluePavilion
    //获得奖励信息提示
    if (m_poOwner->GetCliVer() > CLIENT_VER)
    {
        GetEncourageInfo(stBlueEncourage);
    }

    byIsIgnoreXnMultiple = m_stDT_BEGBLUE_DATA.byIsIgnoreXnMultiple;
    if (m_stDT_BEGBLUE_DATA.byIsHaveToComfirmBegBlueInfo)
    {
        // 1获取求仙露信息
        // 2获取改运信息
        // 3获取求得的资源信息
        // 4获取爬塔提示

        stBluePavilion.byBluePavilionType = FOR_TO_COMFIRM_BEGBLUE;
        GetBegBlueInfo(stBluePavilion.stBluePavilionUnion.stBegBlueResult.stBegBlueInfo);
        GetImproveLuckInfo(stBluePavilion.stBluePavilionUnion.stBegBlueResult.stImproveLuckInfo);
        GetBegBlueResCli(stBluePavilion.stBluePavilionUnion.stBegBlueResult.stBegBlueResCli);
        GetClimbPrompt(stBluePavilion.aszClimbTowerPrompt);
    }
    else
    {
        // 1获取求仙露信息
        // 2获取爬塔提示
        stBluePavilion.byBluePavilionType = FOR_NEW_BEGBLUE;
        GetBegBlueInfo(stBluePavilion.stBluePavilionUnion.stBegBlueInfo);
        GetClimbPrompt(stBluePavilion.aszClimbTowerPrompt);
    }

    return ERR_OPEN_BLUE_PAVILION::ID_SUCCESS;
}



// 求仙露
UINT16 CBegBlue::BegBlue(DT_BEGBLUE_RESULT_CLI &stBegBlueResult)
{
    CBuild* pBuild = m_poOwner->GetBuild(EBK_BEGBLUE);

    if ( NULL == pBuild)
    {
        return ERR_BEGBLUE::ID_BUILD_NOT_EXIST;
    }
    ChkAndSetBegBlueInitVal();
    // update m_stDT_BEGBLUE_DATA
    m_stDT_BEGBLUE_DATA.qwBegBlueLastTime = SGDP::SDTimeSecs();
    //GetBegBlueNewestVal(m_stDT_BEGBLUE_DATA.dwMaxBegTimes, m_stDT_BEGBLUE_DATA.aszClimbTowerPrompt);

    m_stDT_BEGBLUE_DATA.dwFreeImproveLuckMaxTimes = CLuamgr::Instance()->GetFreeImproveLuckTimes();
    m_stDT_BEGBLUE_DATA.dwUsedFreeImproveLuckTimes = 0;

    // 如果有还未待确认的求仙露结果
    if (m_stDT_BEGBLUE_DATA.byIsHaveToComfirmBegBlueInfo)
    {
        GetBegBlueInfo(stBegBlueResult.stBegBlueInfo);
        GetImproveLuckInfo(stBegBlueResult.stImproveLuckInfo);
        GetBegBlueResCli(stBegBlueResult.stBegBlueResCli);

        return ERR_BEGBLUE::ID_SUCCESS;
    }

    // 是否有求仙露机会
    if ((m_stDT_BEGBLUE_DATA.dwMaxBegTimes <= m_stDT_BEGBLUE_DATA.dwUsedBegTimes))
    {
        return ERR_BEGBLUE::ID_CURR_BEGBLUE_USEUP;
    }

    m_stDT_BEGBLUE_DATA.dwUsedBegTimes = m_stDT_BEGBLUE_DATA.dwUsedBegTimes + 1;

    // 是否还有免费改运机会
    /*
    if (m_stDT_BEGBLUE_DATA.dwFreeImproveLuckMaxTimes > m_stDT_BEGBLUE_DATA.dwUsedFreeImproveLuckTimes)
    {
        m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx       = 0;
        m_stDT_BEGBLUE_DATA.dwNextImproveLuckGold      = 0;
    }
    else
    {
        m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx  = 1;
        m_stDT_BEGBLUE_DATA.dwNextImproveLuckGold = GetNextImproveLuckGold(m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx);
    }
    */

    m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx  = 0;
    m_stDT_BEGBLUE_DATA.dwNextImproveLuckGold = 0;

    m_stDT_BEGBLUE_DATA.byIsHaveToComfirmBegBlueInfo = 1;

    // 生产求仙露结果
    GetBegBlueRes(m_stDT_BEGBLUE_DATA.stBegBlueRes);

    // assemble stBegBlueResult
    GetBegBlueInfo(stBegBlueResult.stBegBlueInfo);
    GetImproveLuckInfo(stBegBlueResult.stImproveLuckInfo);
    GetBegBlueResCli(stBegBlueResult.stBegBlueResCli);

    return ERR_BEGBLUE::ID_SUCCESS;
}

// 改运
UINT16 CBegBlue::ImproveLuck(DT_BEGBLUE_RES_DATA_CLI &stBegBlueResCli, DT_IMPROVELUCK_INFO_CLI &stImproveLuckInfo)
{
    CBuild* pBuild = m_poOwner->GetBuild(EBK_BEGBLUE);

    if ( NULL == pBuild)
    {
        return ERR_IMPROVE_LUCK::ID_BUILD_NOT_EXIST;
    }
    ChkAndSetBegBlueInitVal();
    //  update m_stDT_BEGBLUE_DATA
    m_stDT_BEGBLUE_DATA.qwBegBlueLastTime = SGDP::SDTimeSecs();
    //GetBegBlueNewestVal(m_stDT_BEGBLUE_DATA.dwMaxBegTimes, m_stDT_BEGBLUE_DATA.aszClimbTowerPrompt);

    if (!m_stDT_BEGBLUE_DATA.byIsHaveToComfirmBegBlueInfo)
    {
        // 无求仙露结果
        return ERR_IMPROVE_LUCK::ID_NO_BEGBLUE_RES;
    }

    UINT16 wXnMultiple = 0;
    if (m_poOwner->GetCliVer() > CLIENT_VER)
    {
        wXnMultiple = 1;
    }
    else
    {
        wXnMultiple = 6;
    }
    if (MAX_BEGBLUE_SMILE_CNT == m_stDT_BEGBLUE_DATA.stBegBlueRes.wSmileNum && wXnMultiple == m_stDT_BEGBLUE_DATA.stBegBlueRes.wXnMultiple)
    {
        // assemble stBegBlueResCli and stImproveLuckInfo
        GetImproveLuckInfo(stImproveLuckInfo);
        GetBegBlueResCli(stBegBlueResCli);

        return ERR_IMPROVE_LUCK::ID_SUCCESS;
    }

    if (m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < m_stDT_BEGBLUE_DATA.dwNextImproveLuckGold)
    {
        return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
    }

    // 前往改运,刷新祈求结果
    RefreshBegBlueRes(m_stDT_BEGBLUE_DATA.stBegBlueRes);

    if (m_stDT_BEGBLUE_DATA.dwFreeImproveLuckMaxTimes > m_stDT_BEGBLUE_DATA.dwUsedFreeImproveLuckTimes)
    {
        m_stDT_BEGBLUE_DATA.dwUsedFreeImproveLuckTimes = m_stDT_BEGBLUE_DATA.dwUsedFreeImproveLuckTimes + 1;
        m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx       = 0;
        m_stDT_BEGBLUE_DATA.dwNextImproveLuckGold      = 0;

        if (m_stDT_BEGBLUE_DATA.dwFreeImproveLuckMaxTimes == m_stDT_BEGBLUE_DATA.dwUsedFreeImproveLuckTimes)
        {
            // m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx = m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx + 1;
            m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx = m_stDT_BEGBLUE_DATA.dwFreeImproveLuckMaxTimes + 1;
            m_stDT_BEGBLUE_DATA.dwNextImproveLuckGold = GetNextImproveLuckGold(m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx);
        }
    }
    else
    {
        // 扣除改运花费的元宝
        m_poOwner->DecGold(m_stDT_BEGBLUE_DATA.dwNextImproveLuckGold, CRecordMgr::EDGT_BEG_BLUE_IMPRV_LUCK, m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx);

        // 下次改运的元宝
        m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx = m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx + 1;
        m_stDT_BEGBLUE_DATA.dwNextImproveLuckGold = GetNextImproveLuckGold(m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx);
    }


    // assemble stBegBlueResCli and stImproveLuckInfo
    GetImproveLuckInfo(stImproveLuckInfo);
    GetBegBlueResCli(stBegBlueResCli);

    return ERR_IMPROVE_LUCK::ID_SUCCESS;
}

// 确认求仙露
UINT16 CBegBlue::ComfirmBegBlue(DT_BEGBLUE_INFO_CLI &stBegBlueInfo ,DT_BLUE_ENCOURAGEINFO_CLI &stBlueEncourage)
{

    CBuild* pBuild = m_poOwner->GetBuild(EBK_BEGBLUE);

    if ( NULL == pBuild)
    {
        return ERR_COMFIRM_BEGBLUE::ID_BUILD_NOT_EXIST;
    }
    // 更新求仙露时间, 重置元宝改运序号和下次花费的元宝, 更新玩家的仙露和阅历, 确认标识和求得的资源置0
    // 拼装返回客户端的数据


    ChkAndSetBegBlueInitVal();

    //  update m_stDT_BEGBLUE_DATA
    m_stDT_BEGBLUE_DATA.qwBegBlueLastTime = SGDP::SDTimeSecs();
    //GetBegBlueNewestVal(m_stDT_BEGBLUE_DATA.dwMaxBegTimes, m_stDT_BEGBLUE_DATA.aszClimbTowerPrompt);

    if (!m_stDT_BEGBLUE_DATA.byIsHaveToComfirmBegBlueInfo)
    {
        // 无效的确认请求
        return ERR_COMFIRM_BEGBLUE::ID_NO_TO_COMFIRM_BEGBLUE;
    }

    m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx  = 0;
    m_stDT_BEGBLUE_DATA.dwNextImproveLuckGold = 0;

    UINT32 dwToAddBlueGas = 0;
    UINT32 dwToAddStory   = 0;

    if(m_stDT_BEGBLUE_DATA.byIsIgnoreXnMultiple)
    {
        dwToAddBlueGas = m_stDT_BEGBLUE_DATA.stBegBlueRes.dwBlueUnit;
        dwToAddStory   = m_stDT_BEGBLUE_DATA.stBegBlueRes.dwStoryUnit;
    }
    else
    {
        dwToAddBlueGas = m_stDT_BEGBLUE_DATA.stBegBlueRes.dwBlueUnit * m_stDT_BEGBLUE_DATA.stBegBlueRes.wXnMultiple;
        dwToAddStory   = m_stDT_BEGBLUE_DATA.stBegBlueRes.dwStoryUnit * m_stDT_BEGBLUE_DATA.stBegBlueRes.wXnMultiple;
    }

    //UINT32 dwToAddBlueGas = m_stDT_BEGBLUE_DATA.stBegBlueRes.dwBlueUnit * m_stDT_BEGBLUE_DATA.stBegBlueRes.wXnMultiple;
    //UINT32 dwToAddStory   = m_stDT_BEGBLUE_DATA.stBegBlueRes.dwStoryUnit * m_stDT_BEGBLUE_DATA.stBegBlueRes.wXnMultiple;

    // 更新玩家仙露和阅历
    m_poOwner->AddBlueGas(dwToAddBlueGas, CRecordMgr::EABGT_BEG_BLUE_GAS);
    m_poOwner->AddStory(dwToAddStory, CRecordMgr::EASTT_BEG_BLUE_GAS);

    //获得奖励信息提示
    if (m_poOwner->GetCliVer() > CLIENT_VER)
    {
        //累计笑脸个数
        m_stDT_BEGBLUE_DATA.wSmileNum += m_stDT_BEGBLUE_DATA.stBegBlueRes.wSmileNum;
		CPlayerMedal* poPlayrMedal = m_poOwner->GetPlayerMedal();
		if ( NULL != poPlayrMedal)
		{
			poPlayrMedal->AddSmileNum(m_stDT_BEGBLUE_DATA.stBegBlueRes.wSmileNum);
			poPlayrMedal->CheckMedalBySmallType(EMST_SMILE);
		}
        GetEncourageInfo(stBlueEncourage);
    }
    // 清空祈求结果
    m_stDT_BEGBLUE_DATA.byIsHaveToComfirmBegBlueInfo = 0;
    memset(&m_stDT_BEGBLUE_DATA.stBegBlueRes, 0, sizeof(DT_BEGBLUE_RES));

    // assemble stBegBlueInfo
    GetBegBlueInfo(stBegBlueInfo);

    return ERR_COMFIRM_BEGBLUE::ID_SUCCESS;
}

// 对求仙露对象数据进行过期检查和设置初值
BOOL CBegBlue::ChkAndSetBegBlueInitVal()
{
    if (!IsToday(m_stDT_BEGBLUE_DATA.qwBegBlueLastTime))
    {
        m_stDT_BEGBLUE_DATA.qwBegBlueLastTime = SGDP::SDTimeSecs();

        GetBegBlueInitVal(m_stDT_BEGBLUE_DATA.dwMaxBegTimes,
                          m_stDT_BEGBLUE_DATA.dwFreeImproveLuckMaxTimes,
                          m_stDT_BEGBLUE_DATA.aszClimbTowerPrompt);

        m_stDT_BEGBLUE_DATA.dwUsedBegTimes = 0;

        //清除累计笑脸个数
        m_stDT_BEGBLUE_DATA.wSmileNum = 0;
        memset(&m_stDT_BEGBLUE_DATA.stDtConsumeSmile,0,sizeof(DT_CONSUME_BEGBLUE_SMILE_LST));
        //m_stDT_BEGBLUE_DATA.dwUsedFreeImproveLuckTimes = 0;

        //m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx  = 0;
        //m_stDT_BEGBLUE_DATA.dwNextImproveLuckGold = 0;

        // 如果没有待确认结果
        if (!m_stDT_BEGBLUE_DATA.byIsHaveToComfirmBegBlueInfo)
        {
            memset(&m_stDT_BEGBLUE_DATA.stBegBlueRes, 0, sizeof(DT_BEGBLUE_RES));
        }
    }

    return TRUE;
}



// 获取每天求仙露的初值
BOOL CBegBlue::GetBegBlueInitVal(UINT32& dwMaxBegTimes,
                                 UINT32& dwFreeImproveLuckMaxTimes,
                                 TCHAR aszClimbTowerPrompt[BEGBLUE_CLIMB_TOWER_PROMPT])
{
    CInstance &oInstance = m_poOwner->GetInstance();
    // 1 求仙露上限
    UINT32 dwClimbRowerLayer = oInstance.GetMaxPassClimbTower();
    dwMaxBegTimes = CLuamgr::Instance()->GetBegBlueMaxTimes(dwClimbRowerLayer);

    // 2 免费改运上限
    //dwFreeImproveLuckMaxTimes = CLuamgr::Instance()->GetFreeImproveLuckTimes();


    // 3 构造爬塔提示

    // 再爬n层提示
    //char szClimbTowerPrompt[BEGBLUE_CLIMB_TOWER_PROMPT] = {0};
    if (MAX_CLIMB_TOWER_LAYER == dwClimbRowerLayer)
    {
        std::vector<string> vecFillMsg;
        CHAR szBuff[11] = {0};
        sprintf(szBuff, "%u", dwClimbRowerLayer);
        vecFillMsg.push_back(szBuff);
        memset(aszClimbTowerPrompt, 0, BEGBLUE_CLIMB_TOWER_PROMPT);
        SGDP::SDStrncpy(aszClimbTowerPrompt, CMsgDefMgr::Instance()->GetErrMsg("CLIMB_TOWER_FINISH_PROMPT", &vecFillMsg).c_str(), BEGBLUE_CLIMB_TOWER_PROMPT - 1);

        //sprintf(szClimbTowerPrompt,"恭喜你顺利闯过%u层通天塔，你可以通天了:)", dwClimbRowerLayer);
    }
    else
    {
        UINT32 dwMoreClimbLayer = CLuamgr::Instance()->GetClimbTowerPrompt(dwClimbRowerLayer);
        // 再爬n层提示后的求仙露次数
        UINT32 dwNextMaxBegTimes = CLuamgr::Instance()->GetBegBlueMaxTimes(dwClimbRowerLayer + dwMoreClimbLayer);

        vector<string> vecFillMsg;
        CHAR szBuff[11] = {0};
        sprintf(szBuff, "%u", dwClimbRowerLayer + dwMoreClimbLayer);
        vecFillMsg.push_back(szBuff);
        sprintf(szBuff, "%u", dwNextMaxBegTimes);
        vecFillMsg.push_back(szBuff);
        memset(aszClimbTowerPrompt, 0, BEGBLUE_CLIMB_TOWER_PROMPT);
        SGDP::SDStrncpy(aszClimbTowerPrompt, CMsgDefMgr::Instance()->GetErrMsg("CLIMB_TOWER_PROMPT", &vecFillMsg).c_str(), BEGBLUE_CLIMB_TOWER_PROMPT - 1);

        //sprintf(szClimbTowerPrompt,"闯过%u层通天塔，每天求仙露次数增至%u次", dwClimbRowerLayer + dwMoreClimbLayer, dwNextMaxBegTimes);
    }

    return TRUE;
}



// 获取求仙露最新的限制值和爬塔提示
BOOL CBegBlue::GetBegBlueNewestVal(UINT32& dwMaxBegTimes, TCHAR aszClimbTowerPrompt[BEGBLUE_CLIMB_TOWER_PROMPT])
{
    CInstance &oInstance = m_poOwner->GetInstance();
    // 1 求仙露上限
    UINT32 dwClimbRowerLayer = oInstance.GetMaxPassClimbTower();
    dwMaxBegTimes = CLuamgr::Instance()->GetBegBlueMaxTimes(dwClimbRowerLayer);

    // 2 构造爬塔提示

    // 再爬n层提示
    //char szClimbTowerPrompt[BEGBLUE_CLIMB_TOWER_PROMPT] = {0};
    if (MAX_CLIMB_TOWER_LAYER == dwClimbRowerLayer)
    {
        std::vector<string> vecFillMsg;
        CHAR szBuff[11] = {0};
        sprintf(szBuff, "%u", dwClimbRowerLayer);
        vecFillMsg.push_back(szBuff);
        memset(aszClimbTowerPrompt, 0, BEGBLUE_CLIMB_TOWER_PROMPT);
        SGDP::SDStrncpy(aszClimbTowerPrompt, CMsgDefMgr::Instance()->GetErrMsg("CLIMB_TOWER_FINISH_PROMPT", &vecFillMsg).c_str(), BEGBLUE_CLIMB_TOWER_PROMPT - 1);

        //sprintf(szClimbTowerPrompt,"恭喜你顺利闯过%u层通天塔，你可以通天了:)", dwClimbRowerLayer);
    }
    else
    {
        UINT32 dwMoreClimbLayer = CLuamgr::Instance()->GetClimbTowerPrompt(dwClimbRowerLayer);
        // 再爬n层提示后的求仙露次数
        UINT32 dwNextMaxBegTimes = CLuamgr::Instance()->GetBegBlueMaxTimes(dwClimbRowerLayer + dwMoreClimbLayer);

        vector<string> vecFillMsg;
        CHAR szBuff[11] = {0};
        sprintf(szBuff, "%u", dwClimbRowerLayer + dwMoreClimbLayer);
        vecFillMsg.push_back(szBuff);
        sprintf(szBuff, "%u", dwNextMaxBegTimes);
        vecFillMsg.push_back(szBuff);
        memset(aszClimbTowerPrompt, 0, BEGBLUE_CLIMB_TOWER_PROMPT);
        SGDP::SDStrncpy(aszClimbTowerPrompt, CMsgDefMgr::Instance()->GetErrMsg("CLIMB_TOWER_PROMPT", &vecFillMsg).c_str(), BEGBLUE_CLIMB_TOWER_PROMPT - 1);

        //sprintf(szClimbTowerPrompt,"闯过%u层通天塔，每天求仙露次数增至%u次", dwClimbRowerLayer + dwMoreClimbLayer, dwNextMaxBegTimes);
    }

    return TRUE;
}

UINT32 CBegBlue::GetNextImproveLuckGold(UINT32 dwGoldImproveLuckIdx)
{
    return CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_BEG_BLUE_IMPRV_LUCK, 0, dwGoldImproveLuckIdx);
}

BOOL CBegBlue::GetBegBlueRes(DT_BEGBLUE_RES &stBegBlueRes)
{
    BOOL bRet = CBegBluePropMgr::Instance()->GetBegBlueRes(0, stBegBlueRes.wSmileNum, stBegBlueRes.wXnMultiple,
                stBegBlueRes.dwBlueUnit, stBegBlueRes.dwStoryUnit, stBegBlueRes.aszLuckPrompt, m_poOwner->GetCliVer());

    if (!bRet)
    {
        return FALSE;
    }

    stBegBlueRes.qwCreateTime = SGDP::SDTimeSecs();
    stBegBlueRes.qwUpdateTime = SGDP::SDTimeSecs();

    return TRUE;
}

BOOL CBegBlue::RefreshBegBlueRes(DT_BEGBLUE_RES &stBegBlueRes)
{
    UINT16 wCurrSmile = stBegBlueRes.wSmileNum;

    /*
    if (0 == wCurrSmile)
    {
        wCurrSmile = 1;
    }
    */

    BOOL bRet = CBegBluePropMgr::Instance()->GetBegBlueRes(wCurrSmile, stBegBlueRes.wSmileNum, stBegBlueRes.wXnMultiple,
                stBegBlueRes.dwBlueUnit, stBegBlueRes.dwStoryUnit, stBegBlueRes.aszLuckPrompt,m_poOwner->GetCliVer());

    if (!bRet)
    {
        return FALSE;
    }

    stBegBlueRes.qwUpdateTime = SGDP::SDTimeSecs();
    return TRUE;
}

// ...

VOID CBegBlue::GetBegBlueInfo(DT_BEGBLUE_INFO_CLI &stBegBlueInfo)
{
    stBegBlueInfo.dwMaxBegTimes  = m_stDT_BEGBLUE_DATA.dwMaxBegTimes;

    stBegBlueInfo.dwLeftBegTimes = (m_stDT_BEGBLUE_DATA.dwMaxBegTimes <= m_stDT_BEGBLUE_DATA.dwUsedBegTimes) ? 0 : (m_stDT_BEGBLUE_DATA.dwMaxBegTimes - m_stDT_BEGBLUE_DATA.dwUsedBegTimes);

    return;
}

VOID CBegBlue::GetImproveLuckInfo(DT_IMPROVELUCK_INFO_CLI &stImproveLuckInfo)
{
    stImproveLuckInfo.dwFreeImproveLuckMaxTimes  = m_stDT_BEGBLUE_DATA.dwFreeImproveLuckMaxTimes;
    stImproveLuckInfo.dwLeftFreeImproveLuckTimes = (m_stDT_BEGBLUE_DATA.dwFreeImproveLuckMaxTimes <= m_stDT_BEGBLUE_DATA.dwUsedFreeImproveLuckTimes) ? 0 : (m_stDT_BEGBLUE_DATA.dwFreeImproveLuckMaxTimes - m_stDT_BEGBLUE_DATA.dwUsedFreeImproveLuckTimes);
    stImproveLuckInfo.dwGoldImproveLuckIdx       = m_stDT_BEGBLUE_DATA.dwGoldImproveLuckIdx;
    stImproveLuckInfo.dwNextImproveLuckGold      = m_stDT_BEGBLUE_DATA.dwNextImproveLuckGold;

    return;
}

VOID CBegBlue::GetBegBlueResCli(DT_BEGBLUE_RES_DATA_CLI &stBegBlueResCli)
{
    stBegBlueResCli.wSmileNum   = m_stDT_BEGBLUE_DATA.stBegBlueRes.wSmileNum;
    stBegBlueResCli.dwBlueUnit  = m_stDT_BEGBLUE_DATA.stBegBlueRes.dwBlueUnit;
    stBegBlueResCli.dwStoryUnit = m_stDT_BEGBLUE_DATA.stBegBlueRes.dwStoryUnit;
    stBegBlueResCli.wXnMultiple = m_stDT_BEGBLUE_DATA.stBegBlueRes.wXnMultiple;

    //SGDP::SDStrcpy(stBegBlueResCli.aszLuckPrompt, m_stDT_BEGBLUE_DATA.stBegBlueRes.aszLuckPrompt);

    SGDP::SDStrcpy(stBegBlueResCli.aszLuckPrompt, CMsgDefMgr::Instance()->GetErrMsg(m_stDT_BEGBLUE_DATA.stBegBlueRes.aszLuckPrompt) .c_str());

    return;
}

VOID CBegBlue::GetClimbPrompt(TCHAR aszClimbTowerPrompt[BEGBLUE_CLIMB_TOWER_PROMPT])
{
    SGDP::SDStrcpy(aszClimbTowerPrompt, m_stDT_BEGBLUE_DATA.aszClimbTowerPrompt);
    return;
}

VOID CBegBlue::GetEncourageInfo( DT_BLUE_ENCOURAGEINFO_CLI &stBlueEncourage )
{
    //今日累计获得笑脸个数
    stBlueEncourage.wSmileNum = m_stDT_BEGBLUE_DATA.wSmileNum;
    //今日已兑换笑脸次数
    UINT16 wAllTimes = m_stDT_BEGBLUE_DATA.stDtConsumeSmile.wConsumeTimes;

    UINT16 wSmileNum = m_stDT_BEGBLUE_DATA.wSmileNum ;//> m_stDT_BEGBLUE_DATA.wConsumeSmileNum ? m_stDT_BEGBLUE_DATA.wSmileNum - m_stDT_BEGBLUE_DATA.wConsumeSmileNum : 0;
    CBegBlueCouragePropMap m_mapBuleEncourage = CBegBluePropMgr::Instance()->GetEncourageProp();
    CBegBlueCouragePropMapItr itr_reverse = m_mapBuleEncourage.begin();
    for(;itr_reverse != m_mapBuleEncourage.end();++itr_reverse)
    {
        //是否达到兑换条件
        if (itr_reverse->first > wSmileNum)
        {
            continue;
        }
        UINT8 byHaveData = 1;
        for (UINT16 wIndex = 0; wIndex < wAllTimes; wIndex++)
        {
            //验证是否已经兑换了此条件的奖励
            if(m_stDT_BEGBLUE_DATA.stDtConsumeSmile.awConsumeSmileNum[wIndex] == itr_reverse->first)
            {
                byHaveData = 0;
                break;
            }
        } 
        if (byHaveData == 1)
        {
            stBlueEncourage.byGetAward = 1;
            stBlueEncourage.stEncourageData.dwBlue = itr_reverse->second.dwBlue;
            stBlueEncourage.stEncourageData.wSmileNum = itr_reverse->second.dwSmileNum;

            return;
        }
    }
    //获取下一个奖励提示
    CBegBlueCouragePropMapItr itr = m_mapBuleEncourage.begin();
    while(itr != m_mapBuleEncourage.end())
    {
        if (itr->first > wSmileNum)
        {
            break;
        }
        itr++;
    }
    if (itr != m_mapBuleEncourage.end())
    {     
        UINT32 dwNum = itr->first > wSmileNum ? itr->first - wSmileNum : 0;
        stBlueEncourage.byGetAward = 0;
        stBlueEncourage.stEncourageData.dwBlue = itr->second.dwBlue;
        stBlueEncourage.stEncourageData.wSmileNum = dwNum;
        return;
    }

    //奖励已经达上限
    stBlueEncourage.byGetAward = 2;
}

UINT16 CBegBlue::GetBlueEncourage( DT_BLUE_ENCOURAGEINFO_CLI &stBlueEncourage , UINT32 &dwBlue)
{

    CBuild* pBuild = m_poOwner->GetBuild(EBK_BEGBLUE);

    if ( NULL == pBuild)
    {
        return ERR_GET_BLUE_ENCOURAGE::ID_BUILD_NOT_EXIST;
    }
    if (m_poOwner->GetCliVer() <= CLIENT_VER)
    {
        return ERR_GET_BLUE_ENCOURAGE::ID_SUCCESS;
    }
    //获取今日累计获得笑脸个数
    UINT16 wSmileNum = m_stDT_BEGBLUE_DATA.wSmileNum;//> m_stDT_BEGBLUE_DATA.wConsumeSmileNum ? m_stDT_BEGBLUE_DATA.wSmileNum - m_stDT_BEGBLUE_DATA.wConsumeSmileNum : 0;
    //今日已兑换笑脸次数
    UINT16 wAllTimes = m_stDT_BEGBLUE_DATA.stDtConsumeSmile.wConsumeTimes;

    CBegBlueCouragePropMap m_mapBuleEncourage = CBegBluePropMgr::Instance()->GetEncourageProp();
    CBegBlueCouragePropMapItr itr_reverse = m_mapBuleEncourage.begin();
    CBegBlueReverseCouragePropItr itr = m_mapBuleEncourage.rbegin();

    for(;itr_reverse != m_mapBuleEncourage.end();++itr_reverse)
    {
        //是否达到兑换条件
        if (itr_reverse->first > wSmileNum)
        {
            continue;
        }
        UINT8 byHaveData = 1;
        for (UINT16 wIndex = 0; wIndex < wAllTimes; wIndex++)
        {
            //验证是否已经兑换了此条件的奖励
            if(m_stDT_BEGBLUE_DATA.stDtConsumeSmile.awConsumeSmileNum[wIndex] == itr_reverse->first)
            {
                byHaveData = 0;
                break;
            }
        }        
        if (byHaveData == 1)
        {
            break;
        }
    }
    if (itr_reverse == m_mapBuleEncourage.end())
    {
        if (itr != m_mapBuleEncourage.rend() && itr->first < wSmileNum)
        {
            return ERR_GET_BLUE_ENCOURAGE::ID_NO_BLUE;
        }
        return ERR_GET_BLUE_ENCOURAGE::ID_NO_SMILE_NUM;
    }

    // 更新玩家仙露
    m_poOwner->AddBlueGas(itr_reverse->second.dwBlue, CRecordMgr::EABGT_BEG_BLUE_GAS);

    dwBlue = itr_reverse->second.dwBlue;
    //累计消耗笑脸个数   
    m_stDT_BEGBLUE_DATA.stDtConsumeSmile.awConsumeSmileNum[m_stDT_BEGBLUE_DATA.stDtConsumeSmile.wConsumeTimes++] = itr_reverse->first;

    GetEncourageInfo(stBlueEncourage);

    return ERR_GET_BLUE_ENCOURAGE::ID_SUCCESS;
}






