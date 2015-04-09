#include "dealframepower.h"
#include "gmdef.h"
#include <sstream> 
#include "logic/player/playerbasedatacachemgr.h"
#include "net/recharge/rcclient.h"
DealFramePower::DealFramePower(InterfaceDealBase::CGmManager* pGmManager)
{
	init(pGmManager);
}

DealFramePower::~DealFramePower() {

}

void DealFramePower::Deal(CRCClient* poClinet, UINT32 dwSerialNumber
	, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID
	, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3
	, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc) 
{
	if(NULL == poClinet) {
		return;
	}

	const CPlayerBaseDataVec& rVecPlayerData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseDatas();
	VecPowerRankData vPowerRankData;
	PlayerPowerRankData rankPowerData;
	for (size_t n = 0; n < rVecPlayerData.size(); ++n) {
		SDStrncpy(rankPowerData.szPlayerName, rVecPlayerData[n]->aszDispName, USERNAME_LEN-1);
		rankPowerData.unPlayerID = rVecPlayerData[n]->dwID;
		rankPowerData.unPower = rVecPlayerData[n]->dwPower;
		vPowerRankData.push_back(rankPowerData);
	}

	std::sort(vPowerRankData.begin(), vPowerRankData.end(), SortPlayerPowerRank());

	vector<string> jsonData;
	std::ostringstream oss;
	for (size_t n = 0; n < vPowerRankData.size() && n < eZoneRankNum; ++n) {
		oss.str("");
		oss << "{\"rank\":" << (n + 1);
		oss << "," << "\"playerID\":" << vPowerRankData[n].unPlayerID;
		// oss << "," << "\"playername\":" << "\"" << vPowerRankData[n].szPlayerName << "\"";
		oss << "," << "\"power\":" << vPowerRankData[n].unPower;
		oss << "}";
		jsonData.push_back(oss.str());
	}

	string strMsg = "";
	strMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &jsonData);
	poClinet->Rspone(strMsg);
}

void DealFramePower::Deal(CRCClient* poClinet, UINT32 dwSerialNumber
	, UINT32 dwAreaID,UINT32 dwPlayerID, map<string,string> &mapField)
{
	if (NULL == poClinet) {
		return;
	}

}

void DealFramePower::Interface(CRCClient* poClinet) {
	if (NULL == poClinet) {
		return;
	}
	map<string,string> mapField;
	mapField["rank"] =  SDGBK2UTF8(POWER_RANK_GBK) + "|" + GM_PARAM_INT;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID)+ "|" + GM_PARAM_PLAYER_ID;
	mapField["power"] = SDGBK2UTF8(POWER_GBK) + "|" + GM_PARAM_INT;
	RsponeMsg(poClinet, mapField);
}

UINT16 DealFramePower::GetFrameType() {
	return EGMT_GET_POWER_RANK;
}

string DealFramePower::GetStrFrameType() {
	return CHANGE_POWER_RANK;
}

string DealFramePower::GetStrFrameDesc() {
	return SDGBK2UTF8(GET_POWER_RANK_DESC);
}

void DealFramePower::Release() {
	m_pManager = NULL;
}

bool DealFramePower::init(InterfaceDealBase::CGmManager * IN_pManager) {
	m_pManager = IN_pManager;
	RegisterDealFrame(this);
	return TRUE;
}
