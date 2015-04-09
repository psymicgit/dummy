#include "gatherscience.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include "logic/lua/luamgr.h"
#include "logic/vip/vippropmgr.h"


enum em_GSIP
{
	GSIP_DOWN = 1, 			//向下
	GSIP_EQUIT = 2, 			//相等
	GSIP_UP = 3, 				//向上
};

IMPLEMENT_CLASS_POOL_MGR(CGatherScience)


CGatherScience::CGatherScience()
{
    memset(&m_stDT_GATHER_SCIENCE_DATA, 0 , sizeof(m_stDT_GATHER_SCIENCE_DATA));
}

CGatherScience::~CGatherScience()
{

}

BOOL CGatherScience::Init(DT_GATHER_SCIENCE_DATA &stDT_GATHER_SCIENCE_DATA, CPlayer* poOwner)
{
	m_poOwner = poOwner;
    memcpy(&m_stDT_GATHER_SCIENCE_DATA, &stDT_GATHER_SCIENCE_DATA, sizeof(DT_GATHER_SCIENCE_DATA));
	//恢复数据
	RecovryGatherScience();
	//重置倍率
	m_stDT_GATHER_SCIENCE_DATA.byIncrease = 0;
    return TRUE;
}

//恢复数据
BOOL	CGatherScience::RecovryGatherScience()
{
	if (!IsToday(m_stDT_GATHER_SCIENCE_DATA.qwCoinLastTime))
	{
		m_stDT_GATHER_SCIENCE_DATA.wCoinNum = 0;
		m_stDT_GATHER_SCIENCE_DATA.qwCoinLastTime = 0;
	}

	if (!IsToday(m_stDT_GATHER_SCIENCE_DATA.qwGoldLastTime))
	{
		m_stDT_GATHER_SCIENCE_DATA.wGoldNum = 0;
		m_stDT_GATHER_SCIENCE_DATA.qwGoldLastTime = 0;
	}
	return TRUE;
}


UINT16 CGatherScience::OpenGetherScience(DT_GATHER_SCIENCE_DATA_CLI &stGatherScience)
{
	//恢复数据
	RecovryGatherScience();
	//打包相关属性
	GetDT_GATHER_SCIENCE_DATA_CLI(stGatherScience);
	//打开面板重置倍率
	m_stDT_GATHER_SCIENCE_DATA.byIncrease = 0;
	return ERR_OPEN_GATHER_SCIENCE::ID_SUCCESS;
}

UINT16 CGatherScience::GetherScience(DT_GATHER_SCIENCE_DATA_CLI &stGatherScience, UINT8 byGatherType)
{

	RecovryGatherScience();

	//铜钱聚魂
	if (0 == byGatherType)
	{
		UINT16 wCoinMaxNum = CVipPropMgr::Instance()->GetIncNum(EVINF_GATHER_SCIENCE_COIN, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
		if (wCoinMaxNum <= m_stDT_GATHER_SCIENCE_DATA.wCoinNum)
		{
			return ERR_GATHER_SCIENCE::ID_VIP_NOT_LEVEL ;
		}

		UINT32 dwCoin = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_GATHER_SCIENCE_COIN, m_stDT_GATHER_SCIENCE_DATA.wCoinNum, 0);
		if (dwCoin > m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin)
		{
			return ERR_GATHER_SCIENCE::ID_COIN_NOT_ENOUGH ;
		}
		m_poOwner->DecCoin(dwCoin, CRecordMgr::EDCT_GATHERSCIENCE);

		UINT32 dwCoinScience = CLuamgr::Instance()->GetFuncObtain(m_poOwner, EOF_GATHER_SCIENCE_COIN, m_stDT_GATHER_SCIENCE_DATA.wCoinNum);
		UINT8 byIncrease = m_stDT_GATHER_SCIENCE_DATA.byIncrease >= MAX_INCREASE ? MAX_INCREASE : m_stDT_GATHER_SCIENCE_DATA.byIncrease;
		dwCoinScience *=  CLuamgr::Instance()->GetGatherScienceIncrease(byIncrease + 1);
		m_poOwner->AddScience(dwCoinScience, CRecordMgr::EAST_GATHERSCIENCE);
		m_stDT_GATHER_SCIENCE_DATA.wCoinNum++;
		m_stDT_GATHER_SCIENCE_DATA.qwCoinLastTime = SDTimeSecs();
	}
	else
	{
		UINT16 wGoldMaxNum = CVipPropMgr::Instance()->GetIncNum(EVINF_GATHER_SCIENCE_GOLD, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
		if (wGoldMaxNum <= m_stDT_GATHER_SCIENCE_DATA.wGoldNum)
		{
			return ERR_GATHER_SCIENCE::ID_VIP_NOT_LEVEL ;
		}

		UINT32 dwGold = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_GATHER_SCIENCE_GOLD, m_stDT_GATHER_SCIENCE_DATA.wGoldNum, 0);
		if (dwGold > m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold)
		{
			return ERR_GATHER_SCIENCE::ID_GOLD_NOT_ENOUGH ;
		}

		m_poOwner->DecGold(dwGold, CRecordMgr::EDGT_GATHER_SCIENCE);

		UINT32 dwGoldScience = CLuamgr::Instance()->GetFuncObtain(m_poOwner, EOF_GATHER_SCIENCE_GOLD, m_stDT_GATHER_SCIENCE_DATA.wGoldNum);
		UINT8 byIncrease = m_stDT_GATHER_SCIENCE_DATA.byIncrease >= MAX_INCREASE ? MAX_INCREASE : m_stDT_GATHER_SCIENCE_DATA.byIncrease;
		dwGoldScience *= CLuamgr::Instance()->GetGatherScienceIncrease(byIncrease + 1);
		m_poOwner->AddScience(dwGoldScience, CRecordMgr::EAST_GATHERSCIENCE);
		m_stDT_GATHER_SCIENCE_DATA.wGoldNum++;
		m_stDT_GATHER_SCIENCE_DATA.qwGoldLastTime = SDTimeSecs();
	}

	//计算随机值
	CRandomVec vecRandom;
	vecRandom.push_back(CLuamgr::Instance()->GetGatherScienceIncreasePer(m_stDT_GATHER_SCIENCE_DATA.byIncrease + 1, GSIP_DOWN));
	vecRandom.push_back(CLuamgr::Instance()->GetGatherScienceIncreasePer(m_stDT_GATHER_SCIENCE_DATA.byIncrease + 1, GSIP_EQUIT));
	vecRandom.push_back(CLuamgr::Instance()->GetGatherScienceIncreasePer(m_stDT_GATHER_SCIENCE_DATA.byIncrease + 1, GSIP_UP));

	//随机
	INT32 nRandomHitIdx = GetRandomIdxMustHit(vecRandom);
	if(RANDOM_UNHIT == nRandomHitIdx)
	{
		RETURN_OTHER_ERR;
	}

	//重新计算倍率
	switch(nRandomHitIdx)
	{
	case 0:
		if (0 == m_stDT_GATHER_SCIENCE_DATA.byIncrease)
		{
			m_stDT_GATHER_SCIENCE_DATA.byIncrease = 0;
		}
		else
		{
			m_stDT_GATHER_SCIENCE_DATA.byIncrease--;;
		}
		break;
	case 1:
		//m_stDT_GATHER_SCIENCE_DATA.byIncrease;
		//还是当前的倍率不变
		break;
	case 2:
		if (m_stDT_GATHER_SCIENCE_DATA.byIncrease >= MAX_INCREASE - 1)
		{
			m_stDT_GATHER_SCIENCE_DATA.byIncrease = 0;
		}
		else
		{
			m_stDT_GATHER_SCIENCE_DATA.byIncrease++;
		}
		break;
	default:
		m_stDT_GATHER_SCIENCE_DATA.byIncrease = 0;
		break;
	}
	GetDT_GATHER_SCIENCE_DATA_CLI(stGatherScience);
	return ERR_GATHER_SCIENCE::ID_SUCCESS;
}

VOID		CGatherScience::GetDT_GATHER_SCIENCE_DATA_CLI(DT_GATHER_SCIENCE_DATA_CLI &stDT_GATHER_SCIENCE_DATA_CLI)
{
	stDT_GATHER_SCIENCE_DATA_CLI.qwCoinScience = CLuamgr::Instance()->GetFuncObtain(m_poOwner, EOF_GATHER_SCIENCE_COIN, m_stDT_GATHER_SCIENCE_DATA.wCoinNum);
	stDT_GATHER_SCIENCE_DATA_CLI.wCoinMaxNum = CVipPropMgr::Instance()->GetIncNum(EVINF_GATHER_SCIENCE_COIN, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
	stDT_GATHER_SCIENCE_DATA_CLI.wCoinNum = m_stDT_GATHER_SCIENCE_DATA.wCoinNum;
	stDT_GATHER_SCIENCE_DATA_CLI.dwCoinValues = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_GATHER_SCIENCE_COIN, m_stDT_GATHER_SCIENCE_DATA.wCoinNum, 0);

	stDT_GATHER_SCIENCE_DATA_CLI.qwGoldScience = CLuamgr::Instance()->GetFuncObtain(m_poOwner, EOF_GATHER_SCIENCE_GOLD, stDT_GATHER_SCIENCE_DATA_CLI.wGoldNum);
	stDT_GATHER_SCIENCE_DATA_CLI.wGoldMaxNum = CVipPropMgr::Instance()->GetIncNum(EVINF_GATHER_SCIENCE_GOLD, m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel);
	stDT_GATHER_SCIENCE_DATA_CLI.wGoldNum = m_stDT_GATHER_SCIENCE_DATA.wGoldNum;
	stDT_GATHER_SCIENCE_DATA_CLI.dwGoldValues = CLuamgr::Instance()->GetFuncCost(m_poOwner, EGCF_GATHER_SCIENCE_GOLD, stDT_GATHER_SCIENCE_DATA_CLI.wGoldNum, 0);

	for (UINT32 i = 0; i < MAX_INCREASE; i++)
	{
		stDT_GATHER_SCIENCE_DATA_CLI.abyIncrease[i] = CLuamgr::Instance()->GetGatherScienceIncrease(i + 1);
	}
	stDT_GATHER_SCIENCE_DATA_CLI.byIncrease = m_stDT_GATHER_SCIENCE_DATA.byIncrease;
}
