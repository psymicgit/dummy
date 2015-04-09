// DealFrame53.cpp: implementation of the CDealFrame53 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe53.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playermgr.h"
#include "protocol/server/protocommondata.h"
#include "db/getplayerconsume.h"
#include <framework/gsapi.h>
#include <logic/player/player.h>
#include "logic/player/playerbasedatacachemgr.h"
#include "logic/other/fewparam.h"
#include <common/server/utility.h>
#include "logic/pet/playerpet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame53::CDealFrame53()
{

}

CDealFrame53::~CDealFrame53()
{

}

void CDealFrame53::Release()
{

}

UINT16 CDealFrame53::GetFrameType()
{
    return 53;
}

void CDealFrame53::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    CHAR szRet[512] = {0};
    UINT64		qwStartDateTime = 0;
    CSDDateTime	stDateTime;
    tstring		strDateTime = pszParam4;
    if(SDTimeFromString(strDateTime, stDateTime))
    {
        qwStartDateTime = stDateTime.GetTimeValue();
    }
    else
    {
        qwStartDateTime = SDTimeSecs();
    }

    //检测当前时间是否是今天，如果是，直接从内存获取数据
    CC_LOOP_DO
    CC_LOOP_BREAK(!IsToday(qwStartDateTime))
    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    CC_LOOP_BREAK (NULL == poPlayer)
    poPlayer->CkToday();
    DT_PLAYER_TODAY_CONSUME_DATA & stdata = poPlayer->GetDT_PLAYER_TODAY_CONSUME_DATA();
    CSDDateTime	oDateTime(stdata.qwUTCLastConsumeTime);
    tstring		strLastConsumeTime = SDTimeToString("YYYY-mm-dd", oDateTime);
    SDStrncpy(stdata.aszLastConsumeTime, strLastConsumeTime.c_str(), USERNAME_LEN);
    cJSON *JValue = GetJsonDT_PLAYER_TODAY_CONSUME_DATA(stdata);
    CC_LOOP_BREAK(NULL == JValue)
	CFewParam& oFewParam = poPlayer->GetFewParam();
	cJSON_AddNumberToObject(JValue,"GVEPassNum", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wGVETimes));
	cJSON_AddNumberToObject(JValue,"RaceSuccessNum", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wRaceSuccess));
	cJSON_AddNumberToObject(JValue,"RaceFailedNum", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wRaceFailed));
	cJSON_AddNumberToObject(JValue,"TrialSuccessNum", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wTrialSuccess));
	cJSON_AddNumberToObject(JValue,"TrialFailedNum", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wTrialFailed));
	cJSON_AddNumberToObject(JValue,"InterceptSuccessNum", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wInterceptSuccess));
	cJSON_AddNumberToObject(JValue,"InterceptFailedNum", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wInterceptFailed));
	cJSON_AddNumberToObject(JValue,"OrangeAwakenNum", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wAwakenNum));
	cJSON_AddNumberToObject(JValue,"ResetTowerNum", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wResetTowerNum));
	cJSON_AddNumberToObject(JValue,"HellPassNum", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wHellPassNum));
	cJSON_AddNumberToObject(JValue,"WorldPassNum", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wWorldPassNum));
	cJSON_AddNumberToObject(JValue,"HeavenPassNum", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wHeavenPassNum));
	cJSON_AddNumberToObject(JValue,"XiJingMaster", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wXiJingMaster));
	cJSON_AddNumberToObject(JValue, "ExperienceDrug", (double)(oFewParam.GetDT_FEW_PARAM_DATA().wExperienceDrug));
	CPlayerPet* pPlayerPet = poPlayer->GetPlayerPet();
	if (NULL != pPlayerPet) {
		DT_PLAYER_PET_DATA& rPetData = pPlayerPet->GetDT_PLAYER_PET_DATA();
		UINT32 unProductCnt = 0;
		if (!IsToday(rPetData.qwProductLastTime)) {
			unProductCnt = rPetData.wNormalProductNum + rPetData.wGoodProductNum * 10;
		}
		cJSON_AddNumberToObject(JValue, "PetProductNum", (double)(unProductCnt));
	} else {
		cJSON_AddNumberToObject(JValue,"PetProductNum", (double)(0));
	}

    CHAR* pszValue = cJSON_Print(JValue);
    CC_LOOP_BREAK (NULL == pszValue)
    string strData = pszValue;
    free(pszValue);
    pszValue = NULL;
    cJSON_Delete(JValue);

	vector<string >vecStrData;
	vecStrData.push_back(strData);

	string strResultData = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS,&vecStrData);
    poClinet->Rspone(strResultData.c_str());
    return;
    CC_LOOP_WHILE(0);


    DT_GetPlayerConsume stGetPlayerConsume;
    stGetPlayerConsume.dwPlayerID = dwPlayerID;
    stGetPlayerConsume.strStartDate = pszParam4;
    stGetPlayerConsume.strEndDate = pszParam5;

    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_GET_PLAYER_CONSUME);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return ;
    }
    poCMD->SetUserData((VOID*)&stGetPlayerConsume, poClinet->GetCliSessionID());
    gsapi::GetDBMgr()->AddCommand(poCMD);
}

void CDealFrame53::Deal( CRCClient* poClinet, UINT32 dwSerialNumber, UINT32 dwAreaID, UINT32 dwPlayerID, map<string, string> &mapField )
{
    if (NULL == poClinet)
    {
        return;
    }

    if (mapField[PLAYER_ID].empty() || mapField[START_TIME_PARAM].empty() || mapField[END_TIME_PARAM].empty())
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
    const SPlayerBaseData* poPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(unPlayerId);

    if(NULL == poPlayerBaseData)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    CHAR szRet[512] = {0};
    UINT64		qwStartDateTime = 0;
    CSDDateTime	stDateTime;
    tstring		strDateTime = mapField[START_TIME_PARAM].c_str();

    if(SDTimeFromString(strDateTime, stDateTime))
    {
        qwStartDateTime = stDateTime.GetTimeValue();
    }
    else
    {
        qwStartDateTime = SDTimeSecs();
    }

    //检测当前时间是否是今天，如果是，直接从内存获取数据
    CC_LOOP_DO
    CC_LOOP_BREAK(!IsToday(qwStartDateTime))
    CPlayer *poPlayer = CPlayerMgr::Instance()->FindPlayer(unPlayerId);
    CC_LOOP_BREAK (NULL == poPlayer)
    poPlayer->CkToday();
    cJSON *JValue = GetJsonDT_PLAYER_TODAY_CONSUME_DATA(poPlayer->GetDT_PLAYER_TODAY_CONSUME_DATA());
    CC_LOOP_BREAK(NULL == JValue)
    CHAR* pszValue = cJSON_Print(JValue);
    CC_LOOP_BREAK (NULL == pszValue)
    string strData = pszValue;
    free(pszValue);
    pszValue = NULL;
    cJSON_Delete(JValue);
    poClinet->Rspone(strData.c_str());
    return;
    CC_LOOP_WHILE(0);


    DT_GetPlayerConsume stGetPlayerConsume;
    stGetPlayerConsume.dwPlayerID = unPlayerId;
    stGetPlayerConsume.strStartDate = mapField[START_TIME_PARAM].c_str();;
    stGetPlayerConsume.strEndDate = mapField[END_TIME_PARAM].c_str();;

    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_GET_PLAYER_CONSUME);
    if(NULL == poCMD)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateCmd failed!"), MSG_MARK);
        return ;
    }
    poCMD->SetUserData((VOID*)&stGetPlayerConsume, poClinet->GetCliSessionID());
    gsapi::GetDBMgr()->AddCommand(poCMD);
}

string CDealFrame53::GetStrFrameType()
{
    return GET_CONSUME;
}

void CDealFrame53::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string, string> mapField;
    mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;
    mapField[START_TIME_PARAM] = SDGBK2UTF8(START_TIME_GBK) + "|" + GM_PARAM_DATATIME;
    mapField[END_TIME_PARAM] = SDGBK2UTF8(END_TIME_GBK) + "|" + GM_PARAM_DATATIME;

    RsponeMsg(poClinet, mapField);
}

string CDealFrame53::GetStrFrameDesc()
{
    return SDGBK2UTF8(GET_CONSUME_DESC);
}

