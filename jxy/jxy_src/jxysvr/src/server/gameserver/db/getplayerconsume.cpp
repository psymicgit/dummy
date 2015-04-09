#include "getplayerconsume.h"
#include "dbcommoperate.h"
#include <framework/gsapi.h>
#include <net/recharge/rcclient.h>

#include <logic/player/player.h>
#include <logic/player/playermgr.h>





CGetPlayerConsume::CGetPlayerConsume()
{
    memset(&m_stDT_PLAYER_TODAY_CONSUME_DATA, 0x00, sizeof(DT_PLAYER_TODAY_CONSUME_DATA));
}

CGetPlayerConsume::~CGetPlayerConsume()
{

}

#define GETSQLDEFAULTVALUE(FieldName) (pRecordSet->GetFieldValueByName(FieldName) != NULL ? pRecordSet->GetFieldValueByName(FieldName) : "0")

VOID SDAPI CGetPlayerConsume::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    //获取旧消费记录
    CHAR		szSQL[256] = {0};
    sprintf(szSQL, "select * from player_consume where PlayerID = %u and CreateTime = '%s'", m_stGetPlayerConsume.dwPlayerID, m_stGetPlayerConsume.strStartDate.c_str());
    ISDDBRecordSet* pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, 0, &strError );
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        m_stDT_PLAYER_TODAY_CONSUME_DATA.dwID = m_stGetPlayerConsume.dwPlayerID;
        if (pRecordSet->GetFieldValueByName("CreateTime") != NULL)
        {
            GetDateTime2Stamp(m_stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime, "CreateTime");
        }
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddGold = SDAtou64( GETSQLDEFAULTVALUE("AddGold"));
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecGold = SDAtou64( GETSQLDEFAULTVALUE("DecGold"));
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterGold = SDAtou64( GETSQLDEFAULTVALUE("AfterGold"));

        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddCoin = SDAtou64( GETSQLDEFAULTVALUE("AddCoin") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecCoin = SDAtou64( GETSQLDEFAULTVALUE("DecCoin") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterCoin = SDAtou64( GETSQLDEFAULTVALUE("AfterCoin") );

        m_stDT_PLAYER_TODAY_CONSUME_DATA.wAddPhyStrength = SDAtou( GETSQLDEFAULTVALUE("AddPhystrength") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.wDecPhyStrength = SDAtou( GETSQLDEFAULTVALUE("DecPhystrength") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.wAfterPhyStrength = SDAtou( GETSQLDEFAULTVALUE("AfterPhystrength") );
																				
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddScience = SDAtou64( GETSQLDEFAULTVALUE("AddScience") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecScience = SDAtou64( GETSQLDEFAULTVALUE("DecScience") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterScience = SDAtou64( GETSQLDEFAULTVALUE("AfterScience") );

        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddStory = SDAtou64( GETSQLDEFAULTVALUE("AddStory") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecStory = SDAtou64( GETSQLDEFAULTVALUE("DecStory") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterStory = SDAtou64( GETSQLDEFAULTVALUE("AfterStory") );

        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddJingJie = SDAtou64( GETSQLDEFAULTVALUE("AddJingJie") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecJingJie = SDAtou64( GETSQLDEFAULTVALUE("DecJingJie") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterJingJie = SDAtou64( GETSQLDEFAULTVALUE("AfterJingJie") );

        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddBlueGas = SDAtou64( GETSQLDEFAULTVALUE("AddBlueGas") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecBlueGas = SDAtou64( GETSQLDEFAULTVALUE("DecBlueGas") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterBlueGas = SDAtou64( GETSQLDEFAULTVALUE("AfterBlueGas") );

        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddPurpleGas = SDAtou64( GETSQLDEFAULTVALUE("AddPurpleGas") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecPurpleGas = SDAtou64( GETSQLDEFAULTVALUE("DecPurpleGas") );
		m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterPurpleGas = SDAtou64( GETSQLDEFAULTVALUE("AfterPurpleGas") );

        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddStudyExp = SDAtou64( GETSQLDEFAULTVALUE("AddStudyExp") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecStudyExp = SDAtou64( GETSQLDEFAULTVALUE("DecStudyExp") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterStudyExp = SDAtou64( GETSQLDEFAULTVALUE("AfterStudyExp") );

        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddExperience = SDAtou64( GETSQLDEFAULTVALUE("AddExperience") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterExperience = SDAtou64( GETSQLDEFAULTVALUE("AfterExperience") );

        m_stDT_PLAYER_TODAY_CONSUME_DATA.wAddLevel = SDAtou( GETSQLDEFAULTVALUE("AddLevel") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.wAfterLevel = SDAtou( GETSQLDEFAULTVALUE("AfterLevel") );

        m_stDT_PLAYER_TODAY_CONSUME_DATA.byAddVipLevel = SDAtou( GETSQLDEFAULTVALUE("AddVipLevel") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.byAfterVipLevel = SDAtou( GETSQLDEFAULTVALUE("AfterVipLevel") );

		m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddMedal = SDAtou( GETSQLDEFAULTVALUE("AddMedal") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecMedal = SDAtou( GETSQLDEFAULTVALUE("DecMedal") );
		m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterMedal = SDAtou( GETSQLDEFAULTVALUE("AfterMedal") );

        m_stDT_PLAYER_TODAY_CONSUME_DATA.dwAddAmount = SDAtou( GETSQLDEFAULTVALUE("AddAmount") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.dwAfterAmount = SDAtou( GETSQLDEFAULTVALUE("AfterAmount") );

        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddTotalRMB = SDAtou( GETSQLDEFAULTVALUE("AddTotalRMB") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterTotalRMB = SDAtou( GETSQLDEFAULTVALUE("AfterTotalRMB") );

		m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddDoorsTribute = SDAtou( GETSQLDEFAULTVALUE("AddDoorsTribute") );
		m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecDoorsTribute = SDAtou( GETSQLDEFAULTVALUE("DecDoorsTribute") );
		m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterDoorsTribute = SDAtou( GETSQLDEFAULTVALUE("AfterDoorsTribute") );

        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddRedStudyNum = SDAtou( GETSQLDEFAULTVALUE("RedStudyNum") );
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddTaijiStudyNum = SDAtou( GETSQLDEFAULTVALUE("TaijiStudyNum") );

        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwBuyPhyStrengthNum = SDAtou( GETSQLDEFAULTVALUE("BuyPhyStrengthNum") );
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

}

VOID SDAPI CGetPlayerConsume::OnExecuted()
{
    CRCClient* poClinet = gsapi::GetRCClientMgr()->FindCliSession(m_dwTransID);
    if (NULL == poClinet)
    {
        return;
    }
    string strData = "";

    UINT64 qwStarDateTime = 0;
    UINT64 qwEndDateTime = 0;
    CSDDateTime stDateTime;
    if(SDTimeFromString(m_stGetPlayerConsume.strStartDate, stDateTime))
    {
        qwStarDateTime = stDateTime.GetTimeValue();
    }
    else
    {
        qwStarDateTime = SDTimeSecs();
    }

    if(SDTimeFromString(m_stGetPlayerConsume.strEndDate, stDateTime))
    {
        qwEndDateTime = stDateTime.GetTimeValue();
    }
    else
    {
        qwEndDateTime = SDTimeSecs();
    }

    if (0 == m_stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime)
    {
        m_stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime = stDateTime.GetTimeValue();
    }

    if (0 == m_stDT_PLAYER_TODAY_CONSUME_DATA.dwID)
    {
        m_stDT_PLAYER_TODAY_CONSUME_DATA.dwID = m_stGetPlayerConsume.dwPlayerID;
    }

    CSDDateTime	oDateTime(m_stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime);
    tstring		strLastConsumeTime = SDTimeToString("YYYY-mm-dd", oDateTime);
    SDStrncpy(m_stDT_PLAYER_TODAY_CONSUME_DATA.aszLastConsumeTime, strLastConsumeTime.c_str(), USERNAME_LEN);
    cJSON *JValue = GetJsonDT_PLAYER_TODAY_CONSUME_DATA(m_stDT_PLAYER_TODAY_CONSUME_DATA);
    if(NULL == JValue)
    {
        return;
    }
    CHAR* pszValue = cJSON_Print(JValue);
    if(NULL == pszValue)
    {
        return;
    }
    strData += pszValue;
    free(pszValue);
    pszValue = NULL;
    cJSON_Delete(JValue);

	vector<string> vecStrData;
	vecStrData.push_back(strData);
	string strResultData = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS,&vecStrData);
    poClinet->Rspone(strResultData.c_str());
}

VOID CGetPlayerConsume::SetUserData(VOID* pUserData)
{
    m_stGetPlayerConsume = *(DT_GetPlayerConsume*)pUserData;
}


