#include "plundercapture.h"
#include "plunder.h"
#include "plunderbasepropmgr.h"
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <db/autosqlbuf.h>
#include "logic/record/recordmgr.h"



CPlunderCapture::CPlunderCapture()
{
    memset(&m_stDT_CAPTURE_DATA, 0, sizeof(m_stDT_CAPTURE_DATA));
    m_dwCaptureKeepSecond = CPlunderBasePropMgr::Instance()->GetPlunderBaseProp().dwCaptureKeepSecond;
    m_dwCaptureWorkSecond = CPlunderBasePropMgr::Instance()->GetPlunderBaseProp().dwCaptureWorkSecond;
    //m_dwWorkCoinPerSec = CPlunderBasePropMgr::Instance()->GetPlunderBaseProp().dwWorkCoinPerSec;
    //m_dwWorkSciencePerSec = CPlunderBasePropMgr::Instance()->GetPlunderBaseProp().dwWorkSciencePerSec;
    m_poPlunder = NULL;

}


CPlunderCapture::~CPlunderCapture()
{

}

BOOL CPlunderCapture::Init(const DT_CAPTURE_DATA* pstDT_CAPTURE_DATA, CPlayer* poOwner, CPlunder* poPlunder)
{
    if((NULL == pstDT_CAPTURE_DATA) || (NULL == poOwner) || (NULL == poPlunder))
    {
        return FALSE;
    }
    memcpy(&m_stDT_CAPTURE_DATA, pstDT_CAPTURE_DATA, sizeof(DT_CAPTURE_DATA));

    m_poOwner = poOwner;
    m_poPlunder = poPlunder;

    return TRUE;
}


VOID CPlunderCapture::GetDT_CAPTURE_DATA_CLI(DT_CAPTURE_DATA_CLI& stDT_CAPTURE_DATA_CLI)
{
    stDT_CAPTURE_DATA_CLI.dwCapturePlayerID = m_stDT_CAPTURE_DATA.dwCapturePlayerID;
    stDT_CAPTURE_DATA_CLI.dwCapturePlayerID = m_stDT_CAPTURE_DATA.dwCapturePlayerID;
    stDT_CAPTURE_DATA_CLI.byCaptureGridIdx = m_stDT_CAPTURE_DATA.byCaptureGridIdx;
    INT64 dnGap = SDTimeSecs() - m_stDT_CAPTURE_DATA.qwCaptureTime;
    stDT_CAPTURE_DATA_CLI.dwReleaseCountdown = (UINT32)(m_dwCaptureKeepSecond >= dnGap ? m_dwCaptureKeepSecond - dnGap : 0);
    const SPlayerBaseData* pstProp = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_stDT_CAPTURE_DATA.dwCapturePlayerID);
    if(pstProp)
    {
        SDStrcpy(stDT_CAPTURE_DATA_CLI.aszCaptureDispName, pstProp->aszDispName);
        stDT_CAPTURE_DATA_CLI.wCaptureCareerID = pstProp->wMainHeroKindID;
        stDT_CAPTURE_DATA_CLI.wCaptureLevel = pstProp->wLevel;
		if(ERFK_PLUNDER == pstProp->byRobotFunc)
		{
			stDT_CAPTURE_DATA_CLI.wCaptureLevel = m_poOwner->GetLevel();
		}
    }

    GetDT_CAPTURE_WORK_DATA_CLI(stDT_CAPTURE_DATA_CLI.stWorkInfo);
}

VOID CPlunderCapture::GetDT_CAPTURE_WORK_DATA_CLI(DT_CAPTURE_WORK_DATA_CLI& stDT_CAPTURE_WORK_DATA_CLI)
{
	if (NULL == m_poOwner)
	{
		return;
	}

    UINT64 qnGap = SDTimeSecs() - m_stDT_CAPTURE_DATA.qwCaptureTime;
    if(qnGap > m_dwCaptureWorkSecond)
    {
        stDT_CAPTURE_WORK_DATA_CLI.dwWorkCountdown = 0;
    }
    else if(qnGap > 0)
    {
        stDT_CAPTURE_WORK_DATA_CLI.dwWorkCountdown = m_dwCaptureWorkSecond - (UINT32)qnGap;
    }
    //异常
    else
    {
        stDT_CAPTURE_WORK_DATA_CLI.dwWorkCountdown = m_dwCaptureWorkSecond;
    }

    stDT_CAPTURE_WORK_DATA_CLI.byIncomeType = m_stDT_CAPTURE_DATA.byIncomeType;

	const SPlayerBaseData* pstCaptureBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_stDT_CAPTURE_DATA.dwCapturePlayerID);	

	UINT16 wCaptureLevel = 0;
	if(pstCaptureBaseData)
	{
		wCaptureLevel = pstCaptureBaseData->wLevel;
	}
	else
	{
		wCaptureLevel = m_poOwner->GetLevel();//异常，取玩家等级
	}

	//检测是否是机器人
	if(pstCaptureBaseData && ERFK_PLUNDER == pstCaptureBaseData->byRobotFunc)
	{
		wCaptureLevel = m_poOwner->GetLevel();
	}

    //获取总时间
    UINT32 dwCollectIncome;
    stDT_CAPTURE_WORK_DATA_CLI.dwCaptureWorkSecond = CPlunderBasePropMgr::Instance()->GetPlunderBaseProp().dwCaptureWorkSecond;
    switch(m_stDT_CAPTURE_DATA.byIncomeType)
    {
        //获取总收益
    case EPWT_COIN:
        dwCollectIncome = (UINT32)(CPlunderBasePropMgr::Instance()->GetWorkCoin(wCaptureLevel));
        break;
    case EPWT_SCIENCE:
        dwCollectIncome = (UINT32)(CPlunderBasePropMgr::Instance()->GetWorkScience(wCaptureLevel));
        break;
    default:
        dwCollectIncome = 0;
        break;
    }
    stDT_CAPTURE_WORK_DATA_CLI.dwCollectIncome = dwCollectIncome;

    if(m_stDT_CAPTURE_DATA.qwLastCollectTime < m_stDT_CAPTURE_DATA.qwCaptureTime)//异常
    {
        m_stDT_CAPTURE_DATA.qwLastCollectTime = m_stDT_CAPTURE_DATA.qwCaptureTime;
    }

    //剩下可收益时间
    UINT32 dwUnCollectTime =  (UINT32)(m_stDT_CAPTURE_DATA.qwLastCollectTime - m_stDT_CAPTURE_DATA.qwCaptureTime);
    dwUnCollectTime = dwUnCollectTime >= m_dwCaptureWorkSecond ? 0 : m_dwCaptureWorkSecond - dwUnCollectTime;

    //已过收益时间
    UINT32 dwPassTime =  (UINT32)(SDTimeSecs() - m_stDT_CAPTURE_DATA.qwLastCollectTime);
    dwPassTime = dwPassTime < 0 ? 0 : dwPassTime;
    //已过可收益时间
    UINT32 dwPassCollectTime = dwPassTime > dwUnCollectTime ? dwUnCollectTime : dwPassTime;
	
    stDT_CAPTURE_WORK_DATA_CLI.dwUncollectIncome  = (UINT32)((FLOAT)dwPassCollectTime /  (FLOAT)m_dwCaptureWorkSecond * (FLOAT)dwCollectIncome);
    stDT_CAPTURE_WORK_DATA_CLI.dwAllCollectIncome = (UINT32)((FLOAT)dwUnCollectTime /  (FLOAT)m_dwCaptureWorkSecond * (FLOAT)dwCollectIncome);
}


/*
//检测是否需释放
BOOL CPlunderCapture::CkNeedRelease()
{
	INT64 dnGap = SDTimeSecs() - m_stDT_CAPTURE_DATA. qwCaptureTime;
	if(dnGap >= (INT64)m_dwCaptureKeepSecond)
	{
		return TRUE;
	}

	return FALSE;
}
*/

UINT16 CPlunderCapture::SetWorkType(UINT8 byIncomeType, DT_CAPTURE_WORK_DATA_CLI& stWorkInfo)
{
	//可重复设置
    //if(EPWT_UNSET != m_stDT_CAPTURE_DATA.byIncomeType)
    //{
    //    return ERR_SET_WORK_TYPE::ID_HAVE_SET;
    //}
	if((byIncomeType != EPWT_COIN) && (byIncomeType != EPWT_SCIENCE))
	{
		return ERR_SET_WORK_TYPE::ID_INVALID_TYPE;
	}

    m_stDT_CAPTURE_DATA.byIncomeType = byIncomeType;
    GetDT_CAPTURE_WORK_DATA_CLI(stWorkInfo);

    return ERR_SET_WORK_TYPE::ID_SUCCESS;
}

UINT16 CPlunderCapture::CollectIncome(UINT32& dwIncomeValue, DT_CAPTURE_WORK_DATA_CLI& stWorkInfo, UINT8 &byHalved)
{
    if(EPWT_UNSET == m_stDT_CAPTURE_DATA.byIncomeType)
    {
        GetDT_CAPTURE_WORK_DATA_CLI(stWorkInfo);
        return ERR_COLLECT_INCOME::ID_WORK_TYPE_NOT_SET;
    }

    if(m_stDT_CAPTURE_DATA.qwLastCollectTime < m_stDT_CAPTURE_DATA.qwCaptureTime)//异常
    {
        m_stDT_CAPTURE_DATA.qwLastCollectTime = m_stDT_CAPTURE_DATA.qwCaptureTime;
    }
	
	if(0 == dwIncomeValue)
	{
		GetDT_CAPTURE_WORK_DATA_CLI(stWorkInfo);
		return ERR_COLLECT_INCOME::ID_SUCCESS;
	}

	const SPlayerBaseData* pstCaptureBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_stDT_CAPTURE_DATA.dwCapturePlayerID);
	UINT16 wCaptureLevel = 0;
	if(pstCaptureBaseData)
	{
		wCaptureLevel = pstCaptureBaseData->wLevel;
	}
	else
	{
		wCaptureLevel = m_poOwner->GetLevel();//异常，取玩家等级
	}
	//检测是否是机器人
	if(pstCaptureBaseData && ERFK_PLUNDER == pstCaptureBaseData->byRobotFunc)
	{
		wCaptureLevel = m_poOwner->GetLevel();
	}

    //剩下可收益时间
    UINT32 dwOddCollectTime =  (UINT32)(m_stDT_CAPTURE_DATA.qwLastCollectTime - m_stDT_CAPTURE_DATA.qwCaptureTime);
    dwOddCollectTime = dwOddCollectTime >= m_dwCaptureWorkSecond ? 0 : m_dwCaptureWorkSecond - dwOddCollectTime;

    //已过收益时间
    UINT32 dwPassTime = (UINT32)(SDTimeSecs() - m_stDT_CAPTURE_DATA.qwLastCollectTime);
    dwPassTime = dwPassTime < 0 ? 0 : dwPassTime;

    //已过可收益时间
    UINT32 dwPassCollectTime = dwPassTime > dwOddCollectTime ? dwOddCollectTime : dwPassTime;

    UINT32 dwCaptureOwnerPlayerID = m_poPlunder->GetDT_PLUNDER_BASE_DATA().dwCaptureOwnerPlayerID;
    UINT32 dwCanCollectIncomeValue = 0;


    UINT64 qwCollectIncome = 0;
    switch(m_stDT_CAPTURE_DATA.byIncomeType)
    {
    case EPWT_COIN:
    {
        qwCollectIncome = CPlunderBasePropMgr::Instance()->GetWorkCoin(wCaptureLevel);
		if(0 != dwCaptureOwnerPlayerID)//有俘虏主，收益80%
		{
			byHalved = 1;
			qwCollectIncome = qwCollectIncome *8 / 10;
		}
        dwCanCollectIncomeValue = (UINT32)(dwPassCollectTime * qwCollectIncome / m_dwCaptureWorkSecond);
        if(dwIncomeValue < dwCanCollectIncomeValue)
        {
            //UINT32 dwOddSec = (dwCanCollectIncomeValue - dwIncomeValue) * dwCollectIncome / m_dwCaptureWorkSecond;
			UINT32 dwOddSec = (dwCanCollectIncomeValue - dwIncomeValue) * m_dwCaptureWorkSecond / (UINT32)qwCollectIncome;
            m_stDT_CAPTURE_DATA.qwLastCollectTime = SDTimeSecs() - dwOddSec; 
        }
        else
        {
            dwIncomeValue = dwCanCollectIncomeValue;
            m_stDT_CAPTURE_DATA.qwLastCollectTime = SDTimeSecs();
        }
		//if(0 != dwCaptureOwnerPlayerID)//有俘虏主，收益减半
		//{
		//	dwIncomeValue = dwIncomeValue / 2;
		//}

        //m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin += dwIncomeValue;
		m_poOwner->AddCoin(dwIncomeValue, CRecordMgr::EACT_PLUNDER);
    }
    break;
    case EPWT_SCIENCE:
    {
		qwCollectIncome = CPlunderBasePropMgr::Instance()->GetWorkScience(wCaptureLevel);
		if(0 != dwCaptureOwnerPlayerID)//有俘虏主，收益80%
		{
			byHalved = 1;
			qwCollectIncome = qwCollectIncome  *8 / 10;
		}
        dwCanCollectIncomeValue = (UINT32)(dwPassCollectTime * qwCollectIncome / m_dwCaptureWorkSecond);
        if(dwIncomeValue < dwCanCollectIncomeValue)
        {
			//UINT32 dwOddSec = (dwCanCollectIncomeValue - dwIncomeValue) * dwCollectIncome / m_dwCaptureWorkSecond;
			UINT32 dwOddSec = (dwCanCollectIncomeValue - dwIncomeValue) * m_dwCaptureWorkSecond / (UINT32)qwCollectIncome;
			m_stDT_CAPTURE_DATA.qwLastCollectTime = SDTimeSecs() - dwOddSec; 
        }
        else
        {
            dwIncomeValue = dwCanCollectIncomeValue;
            m_stDT_CAPTURE_DATA.qwLastCollectTime = SDTimeSecs();
        }
        //if(0 != dwCaptureOwnerPlayerID)//有俘虏主，收益减半
        //{
        //    dwIncomeValue = dwIncomeValue / 2;
        //}
        
		//m_poOwner->GetDT_PLAYER_BASE_DATA().qwScience += dwIncomeValue;
		m_poOwner->AddScience(dwIncomeValue, CRecordMgr::EAST_PLUNDER);
    }
    break;
    default:
    {
		byHalved = 0;
        dwCanCollectIncomeValue = 0;
    }
    break;
    }

    GetDT_CAPTURE_WORK_DATA_CLI(stWorkInfo);

    return ERR_COLLECT_INCOME::ID_SUCCESS;
}

UINT16 CPlunderCapture::BleedCapture(UINT8& byIncomeType, UINT32& dwIncomeValue)
{
    if(EPWT_UNSET == m_stDT_CAPTURE_DATA.byIncomeType)
    {
        return ERR_BLEED_CAPTURE::ID_WORK_TYPE_NOT_SET;
    }

    if(m_stDT_CAPTURE_DATA.qwLastCollectTime < m_stDT_CAPTURE_DATA.qwCaptureTime)//异常
    {
        m_stDT_CAPTURE_DATA.qwLastCollectTime = m_stDT_CAPTURE_DATA.qwCaptureTime;
    }
    //剩下可收益时间
    UINT32 dwOddCollectTime =  (UINT32)(m_stDT_CAPTURE_DATA.qwLastCollectTime - m_stDT_CAPTURE_DATA.qwCaptureTime);
    dwOddCollectTime = dwOddCollectTime >= m_dwCaptureWorkSecond ? 0 : m_dwCaptureWorkSecond - dwOddCollectTime;
    UINT32 dwCostGold = 0;
    if(0 != dwOddCollectTime)
    {
        //判断所需元宝
        UINT32 dwSecs = CPlunderBasePropMgr::Instance()->GetPlunderBaseProp().byBleedPerGoldClrMin * 60;
        dwCostGold = dwOddCollectTime / dwSecs;
        if(0 != dwOddCollectTime % dwSecs)
        {
            dwCostGold++;
        }
    }
    if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < dwCostGold)
    {
        return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
    }

	const SPlayerBaseData* pstCaptureBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(m_stDT_CAPTURE_DATA.dwCapturePlayerID);	
	UINT16 wCaptureLevel = 0;
	if(pstCaptureBaseData)
	{
		wCaptureLevel = pstCaptureBaseData->wLevel;
	}
	else
	{
		wCaptureLevel = m_poOwner->GetLevel();//异常，取玩家等级
	}

	//检测是否是机器人
	if(pstCaptureBaseData && ERFK_PLUNDER == pstCaptureBaseData->byRobotFunc)
	{
		wCaptureLevel = m_poOwner->GetLevel();
	}

    //获取总时间
    UINT64 qwCollectIncome = 0;
    UINT32 dwCanCollectIncomeValue = 0;
    switch(m_stDT_CAPTURE_DATA.byIncomeType)
    {
    case EPWT_COIN:
    {
        qwCollectIncome = CPlunderBasePropMgr::Instance()->GetWorkCoin(wCaptureLevel);
        dwCanCollectIncomeValue = (UINT32)(dwOddCollectTime * qwCollectIncome / m_dwCaptureWorkSecond);
        //m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin += dwCanCollectIncomeValue;
		m_poOwner->AddCoin(dwCanCollectIncomeValue, CRecordMgr::EACT_PLUNDER);
    }
    break;
    case EPWT_SCIENCE:
    {
        qwCollectIncome = CPlunderBasePropMgr::Instance()->GetWorkScience(wCaptureLevel);
        dwCanCollectIncomeValue = (UINT32)(dwOddCollectTime * qwCollectIncome /  m_dwCaptureWorkSecond);
        //m_poOwner->GetDT_PLAYER_BASE_DATA().qwScience += dwCanCollectIncomeValue;
		m_poOwner->AddScience(dwCanCollectIncomeValue, CRecordMgr::EAST_PLUNDER);
    }
    break;
    default:
    {
        dwCanCollectIncomeValue = 0;
    }
    break;
    }
    m_stDT_CAPTURE_DATA.qwLastCollectTime = SDTimeSecs();
    //m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold -= dwCostGold;
	m_poOwner->DecGold(dwCostGold, CRecordMgr::EDGT_BLEEDCAPTURE, dwOddCollectTime);

    byIncomeType = m_stDT_CAPTURE_DATA.byIncomeType;
    dwIncomeValue = dwCanCollectIncomeValue;

    return ERR_BLEED_CAPTURE::ID_SUCCESS;
}