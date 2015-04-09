#include "playerslogmgr.h"

#include "logic/player/playermgr.h"
#include "sdloggerimpl.h"
#include "db/basedbccmd.h"
#include "db/dbmgr.h"
#include "db/dbcommoperate.h"
#include "framework/gsapi.h"
#include "logic/other/errmsgmgr.h"

IMPLEMENT_SINGLETON(CPlayerLogMgr)
#define  SAVE_LOG_TIME 60
CPlayerLogMgr::CPlayerLogMgr()
{
	m_mapDBLog.clear();
	m_mapWillSaveLog.clear();
	m_qwSaveTime = 0;
}

CPlayerLogMgr::~CPlayerLogMgr()
{
	m_mapDBLog.clear();
	m_mapWillSaveLog.clear();
	m_qwSaveTime = 0;
}

BOOL CPlayerLogMgr::Init()
{
	if ( !LoadFromDB())
	{
		return FALSE;
	}
	
	return TRUE;
}
VOID CPlayerLogMgr::AddBattleLog(UINT32 dwPlayerID, UINT32 dwEnemyPlayerID, EBattleLogType eLogType, UINT8 bySmallLogType, const vector<UINT64>& vec64, const vector<string>& vecStr)
{
	UINT64 qwKey = CLogicFunc::Get3232Key(dwPlayerID, eLogType);
	DT_BATTLE_LOG_MSG_DATA stData = {0};
    stData.dwPlayerID = dwPlayerID;
	stData.byLogBigType = eLogType;
	stData.byLogSmallType = bySmallLogType;
	stData.qwLogTime = SGDP::SDTimeSecs();
	UINT8 byIdx = 0;
	for ( vector<UINT64>::const_iterator itr = vec64.begin(); itr != vec64.end() && byIdx < MAX_BATTLE_LOG_PARAM_STR; itr++, byIdx++)
	{
		stData.aqwParamLst[byIdx] = *itr;
	}
	stData.byParamNum = byIdx;
	byIdx = 0;
	for ( vector<string>::const_iterator itr = vecStr.begin(); itr != vecStr.end() && byIdx < MAX_BATTLE_LOG_PARAM_STR; itr++, byIdx++)
	{
		SDStrncpy(stData.astStrLst[byIdx].aszTstringMsg, (*itr).c_str(), NORMAL_MSG_LEN - 1);
	}
	stData.byStrNum = byIdx;
	m_mapWillSaveLog.insert(make_pair(qwKey, stData));

	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if ( (NULL != poPlayer) && (GetCliVersion() <= poPlayer->GetCliVer()))
	{
		PKT_CLIGS_UNREAD_BATTLE_NTF stNtf;
		GetUnReadLogNum(dwPlayerID, stNtf);
		poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_UNREAD_BATTLE_NTF);
	}
}

VOID CPlayerLogMgr::GetUnReadLogNum(UINT32 dwPlayerID, PKT_CLIGS_UNREAD_BATTLE_NTF& stNtf)
{
	UINT8 byLogNum = 0;
	for ( UINT8 byIdx = EBLT_GVG; byIdx < EBLT_MAX; byIdx++)
	{
		UINT8 byUnReadNum = m_mapWillSaveLog.count(CLogicFunc::Get3232Key(dwPlayerID, byIdx)) + m_mapDBLog.count(CLogicFunc::Get3232Key(dwPlayerID, byIdx));
		if ( 0 != byUnReadNum)
		{
			stNtf.astLogInfo[byLogNum].byLogType = byIdx;
			stNtf.astLogInfo[byLogNum].byUnReadNum = byUnReadNum;
			byLogNum++;
		}
	}

	stNtf.byLogNum = byLogNum;

}
//无用服务器下发文字无法变色
//BOOL CPlayerLogMgr::AddGvGBattleLog(UINT8 bySmallLogType, const vector<UINT64>& vec64, const vector<string>& vecStr, string& strMsg)
//{
//	TCHAR aszBuf[256] = {0};
//	switch(bySmallLogType)
//	{
//	case EGLT_CONTINUE_KILL://连斩战报
//		{
//			if( 4 != vec64.size())
//			{
//				return FALSE;
//			}
//			vector<string> vecMsg;
//			if ( GVG_CHAOS_WAR == vec64[0])// 本服第二轮（门派大乱斗）（不可押注）
//			{
//				vecMsg.push_back(CMsgDefMgr::Instance()->GetErrMsg("GVG_CHAOS_WAR"));
//			}
//			else if ( GVG_16_TO_8 == vec64[0])// 双服16进8
//			{
//				vecMsg.push_back(CMsgDefMgr::Instance()->GetErrMsg("GVG_16_TO_8"));
//			}
//			else if ( GVG_8_TO_4 == vec64[0])// 双服8进4
//			{
//				vecMsg.push_back(CMsgDefMgr::Instance()->GetErrMsg("GVG_8_TO_4"));
//			}
//			else if ( GVG_FINAL_WAR == vec64[0])// 四服总决赛
//			{
//				vecMsg.push_back(CMsgDefMgr::Instance()->GetErrMsg("GVG_FINAL_WAR"));
//			}
//			else
//			{
//				return FALSE;
//			}
//			SDSprintf(aszBuf, "%u", vec64[1]);
//			vecMsg.push_back(aszBuf);
//			SDSprintf(aszBuf, "%u", vec64[2]);
//			vecMsg.push_back(aszBuf);
//			SDSprintf(aszBuf, "%u", vec64[3]);
//			vecMsg.push_back(aszBuf);
//
//			strMsg = CMsgDefMgr::Instance()->GetErrMsg("EGLT_CONTINUE_KILL", &vecMsg);
//		}
//		break;
//	case EGLT_GVG_FACTION_BATTLE://门派战斗战报
//		{
//			if( vec64.size() < 3 || 1 < vecStr.size())
//			{
//				return FALSE;
//			}
//			vector<string> vecMsg;
//			if ( GVG_CHAOS_WAR == vec64[0])// 本服第二轮（门派大乱斗）（不可押注）
//			{
//				vecMsg.push_back(CMsgDefMgr::Instance()->GetErrMsg("GVG_CHAOS_WAR"));
//			}
//			else if ( GVG_16_TO_8 == vec64[0])// 双服16进8
//			{
//				vecMsg.push_back(CMsgDefMgr::Instance()->GetErrMsg("GVG_16_TO_8"));
//			}
//			else if ( GVG_8_TO_4 == vec64[0])// 双服8进4
//			{
//				vecMsg.push_back(CMsgDefMgr::Instance()->GetErrMsg("GVG_8_TO_4"));
//			}
//			else if ( GVG_FINAL_WAR == vec64[0])// 四服总决赛
//			{
//				vecMsg.push_back(CMsgDefMgr::Instance()->GetErrMsg("GVG_FINAL_WAR"));
//			}
//			else
//			{
//				return FALSE;
//			}
//			SDSprintf(aszBuf, "%u", vec64[1]);
//			vecMsg.push_back(aszBuf);
//			if ( EGLR_WIN == vec64[2]) // 成功
//			{   
//				if ( 1 != vecStr.size())
//				{
//					return FALSE;
//				}
//				vecMsg.push_back(vecStr[0]);
//				strMsg = CMsgDefMgr::Instance()->GetErrMsg("EGLT_GVG_FACTION_BATTLE_WIN", &vecMsg);
//			}
//			else if (EGLR_LOSE == vec64[2])//失败
//			{
//				if ( 1 != vecStr.size())
//				{
//					return FALSE;
//				}
//				vecMsg.push_back(vecStr[0]);
//				strMsg = CMsgDefMgr::Instance()->GetErrMsg("EGLT_GVG_FACTION_BATTLE_LOSE", &vecMsg);
//			}
//			else if ( EGLR_NO_ENEMY == vec64[2]) // 轮空
//			{
//				if ( 4 != vec64.size())
//				{
//					return FALSE;
//				}
//				if ( ESM_YES == vec64[3])
//				{
//					strMsg = CMsgDefMgr::Instance()->GetErrMsg("EGLT_GVG_FACTION_BATTLE_EMPTY_NEXT", &vecMsg);
//				}
//				else if ( ESM_NO == vec64[3])
//				{
//					strMsg = CMsgDefMgr::Instance()->GetErrMsg("EGLT_GVG_FACTION_BATTLE_EMPTY_FOUR", &vecMsg);
//				}
//			}
//		}
//		break;
//	case EGLT_GVG_GAMBLE://押注
//		{
//			if ( 1 != vec64.size())
//			{
//				return FALSE;
//			}
//			vector<string> vecMsg;
//			SDSprintf(aszBuf, "%u", vec64[0]);
//			vecMsg.push_back(aszBuf);
//			strMsg = CMsgDefMgr::Instance()->GetErrMsg("EGLT_GVG_GAMBLE", &vecMsg);
//		}
//		break;
//	case EGLT_GVG_ALL_HONOUR://全服荣誉
//		{
//			if ( 2 != vecStr.size())
//			{
//				return FALSE;
//			}
//			vector<string> vecMsg;
//			vecMsg.push_back(vecStr[0]);
//			vecMsg.push_back(vecStr[1]);
//			strMsg = CMsgDefMgr::Instance()->GetErrMsg("EGLT_GVG_ALL_HONOUR", &vecMsg);
//		}
//		break;
//	default:
//		return FALSE;
//	}
//
//	return TRUE;
//}

//获取未读信息个数
UINT32 CPlayerLogMgr::GetUnReadLogNum(UINT32 dwPlayerID, UINT8 byLogType)
{
	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if ( NULL == poPlayer)
	{
		return 0;
	}
	if ( poPlayer->GetCliVer() < GetCliVersion() )
	{
		return 0;
	}
	UINT64 qwKey = CLogicFunc::Get3232Key(dwPlayerID, byLogType);
	UINT32 dwNum = m_mapWillSaveLog.count(qwKey) + m_mapDBLog.count(qwKey);
	return dwNum;
}
//获取未读信息内容
BOOL CPlayerLogMgr::GetUnReadLogMsg(UINT32 dwPlayerID, UINT8 byLogType, DT_BATTLE_LOG_MSG_LST_CLI& stData)
{
	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if ( NULL == poPlayer)
	{
		return FALSE;
	}
	if ( poPlayer->GetCliVer() < GetCliVersion() )
	{
		return FALSE;
	}
	UINT64 qwKey = CLogicFunc::Get3232Key(dwPlayerID, byLogType);
	UINT32 dwLogNum = GetUnReadLogNum(dwPlayerID, byLogType);
	if ( 0 == dwLogNum)
	{
		return FALSE;
	}
	UINT8 byCurLogNum = 0;
	UINT8 byLogNum = static_cast<UINT8>(dwLogNum - 1);
	if ( MAX_BATTLE_LOG_NUM <= byLogNum)
	{
		byLogNum = MAX_BATTLE_LOG_NUM - 1;
	}
	pair<CMultiMapBattleLogItr, CMultiMapBattleLogItr> pItr = m_mapDBLog.equal_range(qwKey);
	for (CMultiMapBattleLogItr itr = pItr.first; itr != pItr.second && byCurLogNum < MAX_BATTLE_LOG_NUM; )
	{
		stData.astLogData[byLogNum].byLogType = itr->second.byLogSmallType;
		stData.astLogData[byLogNum].qwLogTime = itr->second.qwLogTime;
		stData.astLogData[byLogNum].byParamNum = itr->second.byParamNum;
		stData.astLogData[byLogNum].byStrNum = itr->second.byStrNum;
		memcpy(stData.astLogData[byLogNum].aqwParamLst, itr->second.aqwParamLst, sizeof(itr->second.aqwParamLst));
		memcpy(stData.astLogData[byLogNum].astStrLst, itr->second.astStrLst, sizeof(itr->second.astStrLst));
		byLogNum--;
		byCurLogNum++;
		m_mapDBLog.erase(itr++);
	}
	pItr = m_mapWillSaveLog.equal_range(qwKey);
	for (CMultiMapBattleLogItr itr = pItr.first; itr != pItr.second && byCurLogNum < MAX_BATTLE_LOG_NUM; )
	{
		stData.astLogData[byLogNum].byLogType = itr->second.byLogSmallType;
		stData.astLogData[byLogNum].qwLogTime = itr->second.qwLogTime;
		stData.astLogData[byLogNum].byParamNum = itr->second.byParamNum;
		stData.astLogData[byLogNum].byStrNum = itr->second.byStrNum;
		memcpy(stData.astLogData[byLogNum].aqwParamLst, itr->second.aqwParamLst, sizeof(itr->second.aqwParamLst));
		memcpy(stData.astLogData[byLogNum].astStrLst, itr->second.astStrLst, sizeof(itr->second.astStrLst));
		byLogNum--;
		byCurLogNum++;
		m_mapWillSaveLog.erase(itr++);
	}
	
	stData.byLogNum = byCurLogNum;

	if ( 0 != stData.byLogNum )
	{
		CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_DEL_PLAYER_LOG);
		if ( NULL == poCMD)
		{
			USR_INFO("[%s:%d] CreateCmd Del EDBCT_Del_PLAYER_LOG failed PlayerID = %lu, LogType = %u", MSG_MARK, dwPlayerID, byLogType);
			return TRUE;
		}
		poCMD->SetUserData(&qwKey);
		gsapi::GetDBMgr()->AddCommand(poCMD);
	}
	return TRUE;
}

BOOL CPlayerLogMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}

	CHAR szSql[1024] = {0};
	sprintf(szSql, "select * from players_log order by PlayerID, ModuleID, LogTime;");

	UINT32 dwLen = 0;
	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	string strError;
	INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );
	if(SDDB_HAS_RECORDSET == nRet)
	{
		while (pRecordSet->GetRecord())
		{
			DT_BATTLE_LOG_MSG_DATA stData = {0};
			UINT32 dwPlayerID = SDAtou(pRecordSet->GetFieldValueByName("PlayerID"));
			UINT8 byModuleID = SDAtou(pRecordSet->GetFieldValueByName("ModuleID"));
			dwLen = pRecordSet->GetFieldLengthByName("Value");
			if (0 != dwLen)
			{
				if( -1 == stData.DecodeSaveData(pRecordSet->GetFieldValueByName("Value"), dwLen ) )
				{
					SYS_CRITICAL( _SDT("[%s: %d] DT_BATTLE_LOG_MSG_DATA.DecodeSaveData Failed!"), MSG_MARK);
					return FALSE;
				}
			}
			m_mapDBLog.insert(make_pair(CLogicFunc::Get3232Key(dwPlayerID, byModuleID), stData));
		}
	}

	if ( pRecordSet)
	{
		pRecordSet->Release();
		pRecordSet = NULL;
	}
	return TRUE;
}

VOID CPlayerLogMgr::Run()
{
	UINT64 qwDiffTime = SDTimeSecs() - m_qwSaveTime;
	if ( SAVE_LOG_TIME < qwDiffTime )
	{
		UINT8 byIdx = 0;
		DT_BATTLE_LOG_MSG_LST_DATA stData = {0};
		for ( CMultiMapBattleLogItr itr = m_mapWillSaveLog.begin(); itr != m_mapWillSaveLog.end() && byIdx < MAX_BATTLE_LOG_NUM; byIdx++)
		{
			stData.astLogData[byIdx] = itr->second;
			m_mapDBLog.insert(make_pair(itr->first, itr->second));
			m_mapWillSaveLog.erase(itr++);
		}

		stData.byLogNum = byIdx;
		if ( 0 != byIdx )
		{
			CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_ADD_PLAYER_LOG);
			if ( NULL == poCMD)
			{
				USR_INFO("[%s:%d] CreateCmd Add EDBCT_ADD_PLAYER_LOG failed", MSG_MARK);;
				return ;
			}
			poCMD->SetUserData(&stData);
			gsapi::GetDBMgr()->AddCommand(poCMD);
		}
		m_qwSaveTime = SDTimeSecs();
	}
}
