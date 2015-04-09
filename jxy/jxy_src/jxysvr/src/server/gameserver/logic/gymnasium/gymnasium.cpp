#include "gymnasium.h"
#include "gymnasiumpropmgr.h"
#include <logic/faction/factionmgr.h>
#include <logic/player/playermgr.h>
#include <logic/other/errmsgmgr.h>

BOOL CGymnasium::Init(DT_GYMNASIUM_DATA &stDT_GYMNASIUM_SKILL_DATA, CPlayer* poOwner)
{
	m_poOwner = poOwner;
	UINT8 bySkillNum = stDT_GYMNASIUM_SKILL_DATA.bySkillNum;
	for (UINT8 byIndex = 0; byIndex < bySkillNum; byIndex++)
	{
		DT_GYMNASIUM_SKILL_INFO& stInfo = stDT_GYMNASIUM_SKILL_DATA.astGymnasiumSkillInfo[byIndex];
		m_mapMySkill[stInfo.bySkillID] = stInfo;
	}
	GetFaction();
	return TRUE;
}

DT_GYMNASIUM_DATA& CGymnasium::GetDT_GYMNASIUM_SKILL_DATA()
{
	m_stGymnasiumSkillData.bySkillNum = m_mapMySkill.size();
	UINT8 byIndex = 0;
	for (CMapMySkillItr it = m_mapMySkill.begin(); it != m_mapMySkill.end(); it++, byIndex++)
	{
		m_stGymnasiumSkillData.astGymnasiumSkillInfo[byIndex] = it->second;
	}
	return m_stGymnasiumSkillData;
}

CFaction* CGymnasium::GetFaction()
{
	UINT32 dwFactionID = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, m_poOwner->GetID());
	if ( !bRet )
	{
		return NULL;
	}
	return CFactionMgr::Instance()->GetFaction(dwFactionID);
}

UINT32 CGymnasium::GetFactionID()
{
	UINT32 dwFactionID = 0;
	BOOL bRet = CFaction::CkFactionPlayerData(dwFactionID, m_poOwner->GetID());
	if ( !bRet )
	{
		return NULL;
	}
	return dwFactionID;
}

UINT16 CGymnasium::OpenGymnasium( DT_GYMNASIUM_CLI_INFO& stGymnasiumInfo, UINT8& bySkillNum,DT_SKILL_CLI_INFO* astSkillInfo, UINT32 adwSkillSumDoorsTribute[MAX_GYMNASIUM_SKILL_NUM] )
{
	CFaction* poFaction = GetFaction();
	//检验门派
	if (NULL == poFaction)
	{
		return ERR_OPEN_GYMNASIUM::ID_FACTION_NOT_EXITS;
	}
	
	DT_FACTION_PLAYER_DATA* pstData = poFaction->GetDT_Faction_Player_Data(m_poOwner->GetID());
	if (NULL == pstData)
	{
		return ERR_OPEN_GYMNASIUM::ID_FACTION_NOT_EXITS;
	}

	BOOL bRet = IsOpen();
	if (!bRet)
	{
		return ERR_OPEN_GYMNASIUM::ID_GYMNASIUM_NOT_EXIST;
	}
	//赋值武堂等级
	GetGymnasiumInfo(stGymnasiumInfo);
	//武堂开放
	if ( 0 == stGymnasiumInfo.byGymnasiumLevel)
	{
		return ERR_OPEN_GYMNASIUM::ID_GYMNASIUM_NOT_EXIST;
	}
	//赋值技能信息
	GetSkillInfo(bySkillNum, astSkillInfo, adwSkillSumDoorsTribute);
	if ( ESM_YES == pstData->byNewSkillFlag)
	{
		//新技能提示去除
		pstData->byNewSkillFlag = ESM_NO;
		//发送提示推送
		PKT_CLIGS_FACTION_PROMPT_NTF stNtf;
		memset(&stNtf, 0, sizeof(PKT_CLIGS_FACTION_PROMPT_NTF));
		GetPrompt(stNtf.stFactionPrompt);
		m_poOwner->SendMsg((TCHAR*)&stNtf, CLIGS_FACTION_PROMPT_NTF);
	}
	return ERR_OPEN_GYMNASIUM::ID_SUCCESS;
}

UINT16 CGymnasium::LearnGymnasiumSkill( PKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK& stAck )
{
	CFaction* poFaction = GetFaction();
	if (poFaction == NULL)
	{
		return ERR_LEARN_GYMNASIUM_SKILL::ID_FACTION_NOT_EXIST;
	}

	BOOL bRet = IsOpen();
	if (!bRet)
	{
		return ERR_LEARN_GYMNASIUM_SKILL::ID_GYMNASIUM_NOT_EXIST;
	}

	UINT16 wRet = GetAfterLearnSkillInfo(stAck.stSkillInfo, stAck.dwLeftDoorsTribute);
	if (wRet != ERR_LEARN_GYMNASIUM_SKILL::ID_SUCCESS)
	{
		return wRet;
	}

	//战力信息 
	stAck.stPowerInfo.dwOldPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	m_poOwner->ResetBattleAttr(TRUE);
	stAck.stPowerInfo.dwCurPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	SSkillProp stSkillProp;
	CGymnasiumPropMgr::Instance()->GetSkillInfoProp(stSkillProp, stAck.stSkillInfo.bySkillID, stAck.stSkillInfo.bySkillLevel);
	stAck.dwSumDoorsTribute = stSkillProp.dwSumDoorsTribute;
	return ERR_LEARN_GYMNASIUM_SKILL::ID_SUCCESS;
}

UINT16 CGymnasium::LearnGymnasiumSkill( PKT_CLIGS_LEARN_GYMNASIUM_SKILL_ACK2& stAck , UINT8 byCurLevel)
{
	CFaction* poFaction = GetFaction();
	if (poFaction == NULL)
	{
		return ERR_LEARN_GYMNASIUM_SKILL::ID_FACTION_NOT_EXIST;
	}

	BOOL bRet = IsOpen();
	if (!bRet)
	{
		return ERR_LEARN_GYMNASIUM_SKILL::ID_GYMNASIUM_NOT_EXIST;
	}
	
	
	const CMapFactionSkill& mapFactionSkill = poFaction->GetFactionSkillMap();
	CMapFactionSkillconstItr factionItr = mapFactionSkill.find(stAck.stSkillInfo.bySkillID);
	CMapMySkillConstItr myItr = m_mapMySkill.find(stAck.stSkillInfo.bySkillID);
	if ( factionItr == mapFactionSkill.end())
	{
		return ERR_LEARN_GYMNASIUM_SKILL::ID_SKILL_NOT_EXIST;
	}

	UINT32 dwLearnLevel = 0;
	if ( myItr != m_mapMySkill.end())
	{
		dwLearnLevel = myItr->second.byCurSkillLevel;
	}

	bRet = TRUE;
	if ( byCurLevel != dwLearnLevel)
	{
		bRet = FALSE;
	}
	
	
	if (bRet)
	{
		UINT16 wRet = GetAfterLearnSkillInfo(stAck.stSkillInfo, stAck.dwLeftDoorsTribute);
		if (wRet != ERR_LEARN_GYMNASIUM_SKILL::ID_SUCCESS)
		{
			return wRet;
		}
	}
	else
	{
		GetSingleSkillInfo(stAck.stSkillInfo, stAck.stSkillInfo.bySkillID);
	}

	//战力信息 
	stAck.stPowerInfo.dwOldPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	m_poOwner->ResetBattleAttr(TRUE);
	stAck.stPowerInfo.dwCurPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	SSkillProp stSkillProp;
	CGymnasiumPropMgr::Instance()->GetSkillInfoProp(stSkillProp, stAck.stSkillInfo.bySkillID, stAck.stSkillInfo.bySkillLevel);
	stAck.dwSumDoorsTribute = stSkillProp.dwSumDoorsTribute;
	return ERR_LEARN_GYMNASIUM_SKILL::ID_SUCCESS;
}

UINT16 CGymnasium::OpenGymnasiumUpgrade( DT_UPGRADE_REQUIRE_INFO_CLI& stUpgradeRequireInfo )
{
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		return ERR_OPEN_GYMNASIUM_UPGRADE::ID_FACTION_NOT_EXIST;
	}

	BOOL bRet = IsOpen();
	if (!bRet)
	{
		return ERR_OPEN_GYMNASIUM_UPGRADE::ID_GYMNASIUM_NOT_EXIST;
	}

	UINT32 dwGymnasiumLevel = poFaction->GetFactionBuildLevel(EFB_Gymnasium);

	UINT16 wRet = GetGymnasiumUpgradeInfo(stUpgradeRequireInfo, dwGymnasiumLevel);
	stUpgradeRequireInfo.byBuildLevel = poFaction->GetFactionBuildLevel(EFB_Gymnasium);
	bRet = CFactionMgr::Instance()->CheckPlayerAuthority(m_poOwner->GetID(), EFA_Build);
	stUpgradeRequireInfo.byIsUpgradeLimit = bRet ? ESM_YES : ESM_NO;
	if ( ERR_OPEN_GYMNASIUM_UPGRADE::ID_SUCCESS != wRet)
	{
		return wRet;
	}
	return ERR_OPEN_GYMNASIUM_UPGRADE::ID_SUCCESS;
}

UINT16 CGymnasium::GymnasiumUpgrade( PKT_CLIGS_GYMNASIUM_UPGRADE_ACK& stAck )
{
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		return ERR_GYMNASIUM_UPGRADE::ID_FACTION_NOT_EXIST;
	}
	UINT8 bRet = CFactionMgr::Instance()->CheckPlayerAuthority(m_poOwner->GetID(), EFA_Build);
	if ( 0 == bRet )
	{
		return ERR_GYMNASIUM_UPGRADE::ID_LIMIT_NOT_ENOUGH;
	}
	
	UINT32 dwBeferSkillNum = poFaction->GetFactionSkillMap().size();

	DT_UPGRADE_REQUIRE_INFO_CLI stUpgradeRequireInfoCli = {0};
	UINT16 wRet = OpenGymnasiumUpgrade( stUpgradeRequireInfoCli );
	//错误码匹配
	switch (wRet)
	{
	case ERR_OPEN_GYMNASIUM_UPGRADE::ID_FACTION_NOT_EXIST:
		{
			return ERR_GYMNASIUM_UPGRADE::ID_FACTION_NOT_EXIST;
		}
		break;
	case ERR_OPEN_GYMNASIUM_UPGRADE::ID_GYMNASIUM_LEVEL_IS_FULL:
		{
			return ERR_GYMNASIUM_UPGRADE::ID_GYMNASIUM_LEVEL_IS_FULL;
		}
		break;
	case ERR_OPEN_GYMNASIUM_UPGRADE::ID_GYMNASIUM_NOT_EXIST:
		{
			return ERR_GYMNASIUM_UPGRADE::ID_GYMNASIUM_NOT_EXIST;
		}

	default:
		//无操作
		break;
	}

	//检验门派等级

	if ( poFaction->GetDT_FACTION_BASE_DATA().dwFactionLevel < stUpgradeRequireInfoCli.byUnitedGrade)
	{
		return ERR_GYMNASIUM_UPGRADE::ID_FACTION_LEVEL_NOT_ENOUGH;
	}
	if ( poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds < stUpgradeRequireInfoCli.dwConsumptionMoney && 0 <= stUpgradeRequireInfoCli.dwConsumptionMoney)
	{
		return ERR_GYMNASIUM_UPGRADE::ID_FACTION_FUNDS_NOT_ENOUGH;
	}

	//确认升级
	//武堂等级上升
	poFaction->GetDT_FACTION_BASE_DATA().dwGymnasiumLevel += 1;
	//减去门派资金
	poFaction->DecFactionFunds(stUpgradeRequireInfoCli.dwConsumptionMoney, CRecordMgr::EDFF_UPGRADE_GYMNASIUM);
	//更新技能信息
	poFaction->SetFactionSkillInfo();
	//日志
	DT_FACTION_LOG_DATA stFactionLogData;
	stFactionLogData.dwPlayerID1 = m_poOwner->GetID();
	DT_FACTION_PLAYER_CLI stFactionPlayerCli = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli, m_poOwner->GetID());
	stFactionLogData.byPlayerJobType1 = stFactionPlayerCli.byJobType;
	stFactionLogData.byAction = EFLID_UpgradeBuild;
	stFactionLogData.qwLogTime = SGDP::SDTimeSecs();
	stFactionLogData.adwParamList[0] = stUpgradeRequireInfoCli.dwConsumptionMoney;;//需要资金
	stFactionLogData.adwParamList[1] = EFB_Gymnasium;//建筑ID
	stFactionLogData.adwParamList[2] = poFaction->GetDT_FACTION_BASE_DATA().dwGymnasiumLevel;//提升后的等级
	poFaction->AddFactionLogData(stFactionLogData);

	DT_GYMNASIUM_CLI_INFO stGymnasiumInfo = {0};
	OpenGymnasium(stGymnasiumInfo, stAck.bySkillNum, stAck.astAfterUpgradeInfo, stAck.adwSkillSumDoorsTribute);
	wRet = OpenGymnasiumUpgrade( stAck.stUpgradeRequireInfo );
	if ( ERR_OPEN_GYMNASIUM_UPGRADE::ID_GYMNASIUM_LEVEL_IS_FULL == wRet)
	{
		stAck.byFullLevelMark = ESM_YES;
	}
	else
	{
		stAck.byFullLevelMark = ESM_NO;
	}
	UINT32 dwAfterSkillNum = poFaction->GetFactionSkillMap().size();
	if ( dwBeferSkillNum != dwAfterSkillNum)
	{
		poFaction->SetAllNewSkillFlag();
	}
	stAck.byGymnasiumLevel = poFaction->GetDT_FACTION_BASE_DATA().dwGymnasiumLevel;
	stAck.dwFactionFunds = poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds;

	return ERR_GYMNASIUM_UPGRADE::ID_SUCCESS;
}

UINT32 CGymnasium::GetGymnasiumAttrLevel(UINT8 byKindID)
{
	GetFaction();
	CMapMySkillItr it = m_mapMySkill.find(byKindID);
	if ( it != m_mapMySkill.end())
	{
		return it->second.byCurSkillLevel;
	}
	return 0;
}

VOID CGymnasium::GetGymnasiumInfo(DT_GYMNASIUM_CLI_INFO& stGymnasiumInfo)
{
	CFaction* poFaction = GetFaction();
	UINT32 dwFactionID = GetFactionID();
	if (NULL == poFaction)
	{
		return ;
	}
	if (0 == dwFactionID)
	{
		return ;
	}
	stGymnasiumInfo.byGymnasiumLevel = static_cast<UINT8>(CFactionMgr::Instance()->GetFactionBuildLevel(dwFactionID, EFB_Gymnasium));

	DT_FACTION_PLAYER_CLI stDT_FACTION_PLAYER_CLI = {0};
	if ( NULL == poFaction)
	{
		return ;
	}

	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stDT_FACTION_PLAYER_CLI, m_poOwner->GetID());
	stGymnasiumInfo.dwDoorsTribute = stDT_FACTION_PLAYER_CLI.dwDoorsTributeValues;
	//权限
	em_Faction_Job emTmp = static_cast<em_Faction_Job>(stDT_FACTION_PLAYER_CLI.byJobType);

	BOOL bRet = CFactorAuthorityPropMgr::Instance()->CkJobAuthority(emTmp, EFA_Build);

	stGymnasiumInfo.byUpgradeLimit = bRet ? ESM_YES : ESM_NO;  
}

VOID CGymnasium::GetSkillInfo(UINT8& bySkillNum,DT_SKILL_CLI_INFO* astSkillInfo, UINT32 adwSkillSumDoorsTribute[MAX_GYMNASIUM_SKILL_NUM])
{
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return ;
	}

	const CMapFactionSkill& mapFactionSkill = poFaction->GetFactionSkillMap();
	CMapFactionSkill::const_iterator it = mapFactionSkill.begin();
	vector<DT_SKILL_CLI_INFO> vecSkillInfo;
	UINT8 byIndex = 0;
	for ( ; it != mapFactionSkill.end() && byIndex < MAX_GYMNASIUM_SKILL_NUM; it++ )
	{
		GetSingleSkillInfo(astSkillInfo[byIndex], it->second.bySkillID);
		if ( 5 == astSkillInfo[byIndex].bySkillFlag)//未解锁
		{
			vecSkillInfo.push_back(astSkillInfo[byIndex]);
		}
		else
		{
			byIndex++;
		}
	}
	vector<DT_SKILL_CLI_INFO>::iterator vecItr = vecSkillInfo.begin();
	for ( ; vecItr != vecSkillInfo.end() && byIndex < MAX_GYMNASIUM_SKILL_NUM; vecItr++)
	{
		astSkillInfo[byIndex++] = *vecItr;
	}
	bySkillNum = byIndex;
	
	//赋值对累积门贡的要求 
	for ( byIndex = 0; byIndex < bySkillNum; byIndex++)
	{
		SSkillProp stProp;
		BOOL bRet = CGymnasiumPropMgr::Instance()->GetSkillInfoProp(stProp, astSkillInfo[byIndex].bySkillID, astSkillInfo[byIndex].bySkillLevel);
		if ( !bRet )
		{
			continue;
		}
		adwSkillSumDoorsTribute[byIndex] = stProp.dwSumDoorsTribute;
	}
}

VOID CGymnasium::GetSingleSkillInfo(DT_SKILL_CLI_INFO& stSkillInfo, UINT8 bySkillID)
{
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return ;
	}
	const CMapFactionSkill& mapFactionSkill = poFaction->GetFactionSkillMap();

	CMapFactionSkill::const_iterator it = mapFactionSkill.find(bySkillID);
	if (it == mapFactionSkill.end())
	{
		return ;
	}

	CMapMySkillItr myItr = m_mapMySkill.find(bySkillID);
	if (myItr == m_mapMySkill.end())
	{
		DT_GYMNASIUM_SKILL_INFO stInfo;
		stInfo.bySkillID = bySkillID;
		stInfo.byCurSkillLevel = 0;
		m_mapMySkill[bySkillID] = stInfo;
	}
	myItr = m_mapMySkill.find(bySkillID);

	SDStrncpy(stSkillInfo.aszGymnasiumSkillName, it->second.aszSkillName, SKILL_NAME_LEN - 1);
	stSkillInfo.bySkillID = bySkillID;
	stSkillInfo.byCurMaxSkillLevel = it->second.bySkillCurMax;
	stSkillInfo.bySkillLevel = myItr->second.byCurSkillLevel;

	//当技能未解锁时
	if (it->second.byUnlockFlag == ESM_NO)
	{
		stSkillInfo.bySkillFlag = EGSF_Unlock;
		stSkillInfo.dwGymnasiumLevelRequire = CGymnasiumPropMgr::Instance()->GetUnlockSkillLevel(bySkillID);
		return;
	}

	//对学习门贡 与 技能信息赋值
	GetInfoBySkillProp(stSkillInfo);

	//技能达到真正满级
	if( CGymnasiumPropMgr::Instance()->GetMaxSkillLevel() <= myItr->second.byCurSkillLevel)
	{
		stSkillInfo.bySkillFlag = EGSF_SumMax;
		return;
	}

	//技能达到当前满级
	if (it->second.bySkillCurMax <= myItr->second.byCurSkillLevel)
	{
		stSkillInfo.bySkillFlag = EGSF_CurMax;
		stSkillInfo.dwGymnasiumLevelRequire = CGymnasiumPropMgr::Instance()->GetRequireGymnasiumLevel(myItr->second.byCurSkillLevel);
		return ;
	}

	DT_FACTION_PLAYER_CLI stFactionCli = {0};
	poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionCli, m_poOwner->GetID());
	
	SSkillProp stProp;
	BOOL bRet = CGymnasiumPropMgr::Instance()->GetSkillInfoProp(stProp, bySkillID, myItr->second.byCurSkillLevel);
	if ( !bRet )
	{
		return ;
	}

	if ( m_poOwner->GetCliVer() > 302)//302版本之后才会有累积门贡不足的情况
	{
		if (stFactionCli.qwDoorsTributeSum < stProp.dwSumDoorsTribute)
		{
			stSkillInfo.bySkillFlag = EGSF_SumDoorsTribute; //累积门贡不足
			return ;
		}
	}

	if ( stFactionCli.dwDoorsTributeValues < stProp.LearnDoorsTribute && 0 <= stProp.LearnDoorsTribute )
	{
		stSkillInfo.bySkillFlag = EGSF_DoorsTribute;//门贡不足
		return;
	}

	stSkillInfo.bySkillFlag = EGSF_Learn;

}

VOID CGymnasium::GetSingleSkillInfoByScience(DT_SKILL_CLI_INFO& stSkillInfo, UINT8 bySkillID)
{

	CMapMySkillItr myItr = m_mapMySkill.find(bySkillID);
	SSkillProp stProp;
	BOOL bRet = CGymnasiumPropMgr::Instance()->GetSkillInfoProp(stProp, bySkillID, myItr->second.byCurSkillLevel);
	if ( !bRet )
	{
		return ;
	}

	stSkillInfo.bySkillID = bySkillID;
	stSkillInfo.bySkillLevel = myItr->second.byCurSkillLevel;
	GetInfoBySkillProp(stSkillInfo);
	TCHAR* aszSkillName = CGymnasiumPropMgr::Instance()->GetSkillName(bySkillID);
	if ( NULL != aszSkillName)
	{
		SDStrncpy(stSkillInfo.aszGymnasiumSkillName, aszSkillName, SKILL_NAME_LEN - 1);
	}
	stSkillInfo.bySkillFlag = 1;
	stSkillInfo.dwGymnasiumLevelRequire = 0;
}

VOID CGymnasium::GetInfoBySkillProp(DT_SKILL_CLI_INFO& stSkillInfo)
{
	SSkillProp stSkillProp;
	BOOL bRet = CGymnasiumPropMgr::Instance()->GetSkillInfoProp(stSkillProp, stSkillInfo.bySkillID, stSkillInfo.bySkillLevel);
	if ( FALSE == bRet)
	{
		return;
	}
	stSkillInfo.dwCostDoorsTribute = stSkillProp.LearnDoorsTribute;		//学习消耗门贡
	
	vector<string> vecFillMsg;
	CHAR szBuff[32] = {0};
	sprintf(szBuff, "%u", stSkillProp.SkillValue);
	vecFillMsg.push_back(szBuff);
	string strMsg; 
	switch (stSkillInfo.bySkillID)
	{
	case EBAK_HP:
		{
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("SKILL_MSG_HP", &vecFillMsg);
		}
		break;
	case EBAK_ATTACK:
		{
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("SKILL_MSG_ATTCAK", &vecFillMsg);
		}
		break;
	case EBAK_CRITRATE:
		{
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("SKILL_MSG_CRITRATE", &vecFillMsg);
		}
		break;
	case EBAK_DECHURTRATE:
		{
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("SKILL_MSG_DECHURTRATE", &vecFillMsg);
		}
		break;
	case EBAK_DECRITRATE:
		{
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("SKILL_MSG_DECRITRATE", &vecFillMsg);
		}
		break;
	case EBAK_DODGERATE:
		{
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("SKILL_MSG_DODGERATE", &vecFillMsg);
		}
		break;
	case EBAK_HITRATE:
		{
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("SKILL_MSG_HITRATE", &vecFillMsg);
		}
		break;
	case EBAK_INCHURTRATE:
		{
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("SKILL_MSG_INCHURTRATE", &vecFillMsg);
		}
		break;
	/*case EBAK_HP:
		{
			strMsg = CMsgDefMgr::Instance()->GetErrMsg("SKILL_MSG_DEFENSE", &vecFillMsg);
		}*/
	}
	SDStrncpy(stSkillInfo.aszSkillMsg, strMsg.c_str(), SKILL_MSG_LEN-1);
}

UINT16 CGymnasium::GetAfterLearnSkillInfo(DT_SKILL_CLI_INFO& stSkillInfo, UINT32& dwLeftDoorsTribute)
{
	UINT8 bySkillID = stSkillInfo.bySkillID;
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		return ERR_LEARN_GYMNASIUM_SKILL::ID_FACTION_NOT_EXIST;
	}

	DT_SKILL_CLI_INFO stInfo;
	GetSingleSkillInfo(stInfo, stSkillInfo.bySkillID);

	if( EGSF_Learn == stInfo.bySkillFlag )
	{
		//扣除
		poFaction->DeductDoorsTribute(m_poOwner->GetID(), stInfo.dwCostDoorsTribute, CRecordMgr::EDDT_LEARN_SKILL);

		DT_FACTION_PLAYER_CLI stFactionPlayerCli = {0};
		poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stFactionPlayerCli, m_poOwner->GetID());
		//赋值剩余门贡
		dwLeftDoorsTribute = stFactionPlayerCli.dwDoorsTributeValues;
		//技能等级+1
		CMapMySkillItr myItr = m_mapMySkill.find(stSkillInfo.bySkillID);
		if ( myItr != m_mapMySkill.end())
		{
			myItr->second.byCurSkillLevel += 1;
		}

		GetSingleSkillInfo(stSkillInfo, stSkillInfo.bySkillID);
	}
	else if ( EGSF_SumMax == stInfo.bySkillFlag )
	{
		return ERR_LEARN_GYMNASIUM_SKILL::ID_IS_FULL_LEVEL;
	}
	else if ( EGSF_DoorsTribute == stInfo.bySkillFlag)
	{
		return ERR_LEARN_GYMNASIUM_SKILL::ID_DOORSTRIBUTE_NOT_ENOUGH;
	}
	else if ( EGSF_Unlock == stInfo.bySkillFlag )
	{
		return ERR_LEARN_GYMNASIUM_SKILL::ID_SKILL_NOT_UNLOCK;
	}
	else if (EGSF_CurMax == stInfo.bySkillFlag)
	{
		return ERR_LEARN_GYMNASIUM_SKILL::ID_GYMNASIUM_LEVEL_NOT_ENOUGH;
	}
	else if ( EGSF_SumDoorsTribute == stInfo.bySkillFlag)
	{
		return ERR_LEARN_GYMNASIUM_SKILL::ID_SUM_DOORSTRINUTE_NOT_ENOUGH;
	}
	
	return ERR_LEARN_GYMNASIUM_SKILL::ID_SUCCESS;

}

UINT16 CGymnasium::GetGymnasiumUpgradeInfo(DT_UPGRADE_REQUIRE_INFO_CLI& stUpgradeRequireInfo, UINT32 dwGymnasiumLevel)
{
	SGymnasiumUpgradeProp stGymnasiumUpgradeProp = {0};
	BOOL bRet = CGymnasiumPropMgr::Instance()->GetGymnasiumUpgradeProp(stGymnasiumUpgradeProp, dwGymnasiumLevel);
	if ( !bRet)
	{
		return ERR_OPEN_GYMNASIUM_UPGRADE::ID_GYMNASIUM_LEVEL_IS_FULL;
	}
	//获取门派资金
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		return ERR_OPEN_GYMNASIUM_UPGRADE::ID_FACTION_NOT_EXIST;
	}

	stUpgradeRequireInfo.byUnitedGrade = poFaction->GetFactionBuildLevel(EFB_Gymnasium) + 1;
	stUpgradeRequireInfo.dwConsumptionMoney = stGymnasiumUpgradeProp.dwNeedFactionFunds;
	if ( poFaction->GetDT_FACTION_BASE_DATA().dwFactionLevel < stUpgradeRequireInfo.byUnitedGrade)
	{
		stUpgradeRequireInfo.byFactionIsEnough = ESM_NO;
	}
	else
	{
		stUpgradeRequireInfo.byFactionIsEnough = ESM_YES;
	}

	UINT32 dwFactionMoney = poFaction->GetDT_FACTION_BASE_DATA().dwFactionFunds;
	if (dwFactionMoney < stUpgradeRequireInfo.dwConsumptionMoney)
	{
		stUpgradeRequireInfo.byFactionFundsEnough = 0;
	}
	else
	{
		stUpgradeRequireInfo.byFactionFundsEnough = 1;
	}

	SGymnasiumUpgradeProp stNextProp;
	bRet = CGymnasiumPropMgr::Instance()->GetGymnasiumUpgradeProp(stNextProp, dwGymnasiumLevel + 1);
	if ( !bRet)
	{
		return ERR_OPEN_GYMNASIUM_UPGRADE::ID_GYMNASIUM_LEVEL_IS_FULL;
	}
	//技能等级上限+N
	UINT32 dwPropNum = stNextProp.byCurMaxLevel - stGymnasiumUpgradeProp.byCurMaxLevel;
	vector<string> vecFillMsg;
	CHAR szBuff[32] = {0};
	sprintf(szBuff, "%u", dwPropNum);
	vecFillMsg.push_back(szBuff);
	string strMsg = CMsgDefMgr::Instance()->GetErrMsg("GYMNASIUM_UPGRADE_SKILL_MAX", &vecFillMsg);
	SDStrncpy(stUpgradeRequireInfo.astUpgradeInfoList[0].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN-1);

	stUpgradeRequireInfo.byInfoListNum = 1;

	/*const UnlockSkillMap& mapUnlockInfo = CGymnasiumPropMgr::Instance()->GetSkillUnLock();
	pair<UnlockSkillMapIrt, UnlockSkillMapIrt> pairItr = mapUnlockInfo.equal_range(dwGymnasiumLevel + 1);
	for ( UnlockSkillMapIrt unlockItr = pairItr.first; unlockItr != pairItr.second; unlockItr++)
	{
		vecFillMsg.clear();
		vecFillMsg.push_back(unlockItr->second.SKillName);
		strMsg = CMsgDefMgr::Instance()->GetErrMsg("GYMNASIUM_UPGRADE_UNLOCK", &vecFillMsg);
		SDStrncpy(stUpgradeRequireInfo.astUpgradeInfoList[stUpgradeRequireInfo.byInfoListNum].aszUpgradeMsg, strMsg.c_str(), FACTION_UPGRADE_MSG_LEN-1);
		stUpgradeRequireInfo.byInfoListNum += 1;
	}*/

	return ERR_OPEN_GYMNASIUM_UPGRADE::ID_SUCCESS;

}

BOOL CGymnasium::IsOpen()
{
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return FALSE;
	}
	if( 0 == poFaction->GetDT_FACTION_BASE_DATA().dwGymnasiumLevel)
	{
		return FALSE;
	}
	return TRUE;
}

VOID CGymnasium::GetPrompt(DT_FUNC_COMMON_PROMPT& stPrompt)
{
	CFaction* poFaction = GetFaction();
	if (NULL == poFaction)
	{
		return ;
	}
	DT_FACTION_PLAYER_DATA* stData = poFaction->GetDT_Faction_Player_Data(m_poOwner->GetID());
	if (NULL == stData)
	{
		return ;
	}
	stPrompt.byBuildKindID = EFB_Gymnasium;
	if (stData->byNewSkillFlag)
	{
		stPrompt.byFuncFlag = EFPID_NewSkill;//新技能
		return ;
	}

	//可学习
	if (poFaction->IsCanLearnSkill(m_poOwner->GetID()))
	{
		stPrompt.byFuncFlag = EFPID_CanLearn;
	}
}

UINT16 CGymnasium::OpenFactionSkill(PKT_CLIGS_OPEN_FACTION_SKILL_ACK& stAck)
{
	BOOL bFaction = FALSE;
	UINT32 dwGymnasiumLevel = 0;
	CFaction* poFaction = GetFaction();
	if ( NULL == poFaction)
	{
		bFaction = FALSE;
	}
	else
	{
		bFaction = TRUE;
		dwGymnasiumLevel = poFaction->GetFactionBuildLevel(EFB_Gymnasium);
	}

	UINT8 bySkillNum = 0;
	CMapMySkillItr itr = m_mapMySkill.begin();
	for ( ; itr != m_mapMySkill.end() && bySkillNum < MAX_GYMNASIUM_SKILL_NUM; itr++)
	{
		if(0 == itr->second.byCurSkillLevel)
		{
			continue;
		}
		else
		{
			GetSingleSkillInfoByScience(stAck.astSkillInfo[bySkillNum++], itr->second.bySkillID);
		}
	}
	stAck.bySkillNum = bySkillNum;
	BOOL bSkill = FALSE;
	if ( 0 == bySkillNum)
	{
		bSkill = FALSE;
	}
	else
	{
		bSkill = TRUE;
	}
	
	// <!--状态（0正常，1未加入门派未学习技能，2 加入门派，武堂开启，未学习技能，3加入门派，武堂未开启, 4未加入门派有技能）-->
	if ( !bFaction && !bSkill)
	{
		stAck.byStatu = 1;
	}
	else if( (bFaction && (0 != dwGymnasiumLevel)) && !bSkill)
	{
		stAck.byStatu = 2;
	}
	else if( bFaction && (0 == dwGymnasiumLevel))
	{
		stAck.byStatu = 3;
	}
	else if ( !bFaction && bSkill)
	{
		stAck.byStatu = 4;
	}
	else
	{
		stAck.byStatu = 0;
	}
	

	return ERR_OPEN_FACTION_SKILL::ID_SUCCESS;
}