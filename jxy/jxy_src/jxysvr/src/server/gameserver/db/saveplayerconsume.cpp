#include "saveplayerconsume.h"
#include <db/autosqlbuf.h>
#include "dbcommoperate.h"
#include <sdloggerimpl.h>

CSavePlayerConsume::CSavePlayerConsume()
{
    memset(&m_stDT_PLAYER_TODAY_CONSUME_DATA, 0x00, sizeof(DT_PLAYER_TODAY_CONSUME_DATA));
}

CSavePlayerConsume::~CSavePlayerConsume()
{

}

VOID SDAPI CSavePlayerConsume::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    Execute(poDBConn);
}

VOID SDAPI CSavePlayerConsume::OnExecuted()
{

}

BOOL CSavePlayerConsume::Execute(SGDP::ISDDBConnection* poDBConn)
{
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    if((NULL == pszSql))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }


    CAutoSqlBuf oSqlValues;
    CAutoSqlBuf oAutoTmp;
    CHAR* pszTmp = oAutoTmp.GetSqlBuf();
    CHAR* pszSqlValues = oSqlValues.GetSqlBuf();

    if((NULL == pszSql) || (NULL == pszTmp) || (NULL == pszSqlValues))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    if((NULL == pszSql) || (NULL == pszTmp) || (NULL == pszSqlValues))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    CSDDateTime oDateTime(m_stDT_PLAYER_TODAY_CONSUME_DATA.qwUTCLastConsumeTime);
    tstring strCreateTime = SDTimeToString("YYYY-mm-dd", oDateTime);
    sprintf(pszSql, "insert into player_consume (");
    INSERTINTOSIMPLEFIELD1(PlayerID, "%u", m_stDT_PLAYER_TODAY_CONSUME_DATA.dwID);
    INSERTINTOSIMPLEFIELD2(CreateTime, "%s", strCreateTime.c_str());

    INSERTINTOSIMPLEFIELD2(AddGold, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddGold);
    INSERTINTOSIMPLEFIELD2(DecGold, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecGold);
    INSERTINTOSIMPLEFIELD2(AfterGold, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterGold);

    INSERTINTOSIMPLEFIELD2(AddCoin, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddCoin);
    INSERTINTOSIMPLEFIELD2(DecCoin, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecCoin);
    INSERTINTOSIMPLEFIELD2(AfterCoin, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterCoin);

    INSERTINTOSIMPLEFIELD2(AddPhystrength, "%u", m_stDT_PLAYER_TODAY_CONSUME_DATA.wAddPhyStrength);
    INSERTINTOSIMPLEFIELD2(DecPhystrength, "%u", m_stDT_PLAYER_TODAY_CONSUME_DATA.wDecPhyStrength);
    INSERTINTOSIMPLEFIELD2(AfterPhystrength, "%u", m_stDT_PLAYER_TODAY_CONSUME_DATA.wAfterPhyStrength);

    INSERTINTOSIMPLEFIELD2(AddScience, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddScience);
    INSERTINTOSIMPLEFIELD2(DecScience, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecScience);
    INSERTINTOSIMPLEFIELD2(AfterScience, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterScience);

    INSERTINTOSIMPLEFIELD2(AddStory, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddStory);
    INSERTINTOSIMPLEFIELD2(DecStory, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecStory);
    INSERTINTOSIMPLEFIELD2(AfterStory, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterStory);

    INSERTINTOSIMPLEFIELD2(AddJingJie, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddJingJie);
    INSERTINTOSIMPLEFIELD2(DecJingJie, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecJingJie);
    INSERTINTOSIMPLEFIELD2(AfterJingJie, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterJingJie);

    INSERTINTOSIMPLEFIELD2(AddBlueGas, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddBlueGas);
    INSERTINTOSIMPLEFIELD2(DecBlueGas, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecBlueGas);
    INSERTINTOSIMPLEFIELD2(AfterBlueGas, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterBlueGas);

    INSERTINTOSIMPLEFIELD2(AddPurpleGas, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddPurpleGas);
    INSERTINTOSIMPLEFIELD2(DecPurpleGas, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecPurpleGas);
    INSERTINTOSIMPLEFIELD2(AfterPurpleGas, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterPurpleGas);

    INSERTINTOSIMPLEFIELD2(AddStudyExp, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddStudyExp);
    INSERTINTOSIMPLEFIELD2(DecStudyExp, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecStudyExp);
    INSERTINTOSIMPLEFIELD2(AfterStudyExp, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterStudyExp);

    INSERTINTOSIMPLEFIELD2(AddAmount, "%u", m_stDT_PLAYER_TODAY_CONSUME_DATA.dwAddAmount);
    INSERTINTOSIMPLEFIELD2(AfterAmount, "%u", m_stDT_PLAYER_TODAY_CONSUME_DATA.dwAfterAmount);

    INSERTINTOSIMPLEFIELD2(AddExperience, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddExperience);
    INSERTINTOSIMPLEFIELD2(AfterExperience, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterExperience);

    INSERTINTOSIMPLEFIELD2(AddLevel, "%u", m_stDT_PLAYER_TODAY_CONSUME_DATA.wAddLevel);
    INSERTINTOSIMPLEFIELD2(AfterLevel, "%u", m_stDT_PLAYER_TODAY_CONSUME_DATA.wAfterLevel);

    INSERTINTOSIMPLEFIELD2(AddVipLevel, "%d", m_stDT_PLAYER_TODAY_CONSUME_DATA.byAddVipLevel);
    INSERTINTOSIMPLEFIELD2(AfterVipLevel, "%d", m_stDT_PLAYER_TODAY_CONSUME_DATA.byAfterVipLevel);

    INSERTINTOSIMPLEFIELD2(AddTotalRMB, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddTotalRMB);
    INSERTINTOSIMPLEFIELD2(AfterTotalRMB, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterTotalRMB);

    INSERTINTOSIMPLEFIELD2(AddMedal, "%u", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddMedal);
    INSERTINTOSIMPLEFIELD2(DecMedal, "%u", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecMedal);
    INSERTINTOSIMPLEFIELD2(AfterMedal, "%u", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterMedal);

    INSERTINTOSIMPLEFIELD3(UpdateTime, now());

	INSERTINTOSIMPLEFIELD2(AddDoorsTribute, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddDoorsTribute);
	INSERTINTOSIMPLEFIELD2(DecDoorsTribute, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwDecDoorsTribute);
	INSERTINTOSIMPLEFIELD2(AfterDoorsTribute, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAfterDoorsTribute);

    INSERTINTOSIMPLEFIELD2(RedStudyNum, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddRedStudyNum);
    INSERTINTOSIMPLEFIELD2(TaijiStudyNum, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwAddTaijiStudyNum);

    INSERTINTOSIMPLEFIELD2(BuyPhyStrengthNum, "%llu", m_stDT_PLAYER_TODAY_CONSUME_DATA.qwBuyPhyStrengthNum);
    strcat(pszSql, ") values (");
    strcat(pszSql, pszTmp);
    strcat(pszSql, ") on duplicate key update \
				   player_consume.AddGold = values(player_consume.AddGold) \
				   ,player_consume.DecGold = values(player_consume.DecGold) \
				   ,player_consume.AfterGold = values(player_consume.AfterGold) \
				   ,player_consume.AddCoin = values(player_consume.AddCoin) \
				   ,player_consume.DecCoin = values(player_consume.DecCoin) \
				   ,player_consume.AfterCoin = values(player_consume.AfterCoin) \
				   ,player_consume.AddPhystrength = values(player_consume.AddPhystrength) \
				   ,player_consume.DecPhystrength = values(player_consume.DecPhystrength) \
				   ,player_consume.AfterPhystrength = values(player_consume.AfterPhystrength) \
				   ,player_consume.AddScience = values(player_consume.AddScience) \
				   ,player_consume.DecScience = values(player_consume.DecScience) \
				   ,player_consume.AfterScience = values(player_consume.AfterScience) \
				   ,player_consume.AddStory = values(player_consume.AddStory) \
				   ,player_consume.DecStory = values(player_consume.DecStory) \
				   ,player_consume.AfterStory = values(player_consume.AfterStory) \
				   ,player_consume.AddJingJie = values(player_consume.AddJingJie) \
				   ,player_consume.DecJingJie = values(player_consume.DecJingJie) \
				   ,player_consume.AfterJingJie = values(player_consume.AfterJingJie) \
				   ,player_consume.AddBlueGas = values(player_consume.AddBlueGas) \
				   ,player_consume.DecBlueGas = values(player_consume.DecBlueGas) \
				   ,player_consume.AfterBlueGas = values(player_consume.AfterBlueGas) \
				   ,player_consume.AddPurpleGas = values(player_consume.AddPurpleGas) \
				   ,player_consume.DecPurpleGas = values(player_consume.DecPurpleGas) \
				   ,player_consume.AfterPurpleGas = values(player_consume.AfterPurpleGas) \
				   ,player_consume.AddStudyExp = values(player_consume.AddStudyExp) \
				   ,player_consume.DecStudyExp = values(player_consume.DecStudyExp) \
				   ,player_consume.AfterStudyExp = values(player_consume.AfterStudyExp) \
				   ,player_consume.AddAmount = values(player_consume.AddAmount) \
				   ,player_consume.AfterAmount = values(player_consume.AfterAmount) \
				   ,player_consume.AddExperience = values(player_consume.AddExperience) \
				   ,player_consume.AfterExperience = values(player_consume.AfterExperience) \
				   ,player_consume.AddLevel = values(player_consume.AddLevel) \
				   ,player_consume.AfterLevel = values(player_consume.AfterLevel) \
				   ,player_consume.AddVipLevel = values(player_consume.AddVipLevel) \
				   ,player_consume.AfterVipLevel = values(player_consume.AfterVipLevel) \
				   ,player_consume.AddMedal = values(player_consume.AddMedal) \
				   ,player_consume.DecMedal = values(player_consume.DecMedal) \
				   ,player_consume.AfterMedal = values(player_consume.AfterMedal) \
				   ,player_consume.AddTotalRMB = values(player_consume.AddTotalRMB) \
				   ,player_consume.AfterTotalRMB = values(player_consume.AfterTotalRMB) \
				   ,player_consume.UpdateTime = values(player_consume.UpdateTime) \
				   ,player_consume.AddDoorsTribute = values(player_consume.AddDoorsTribute) \
				   ,player_consume.DecDoorsTribute = values(player_consume.DecDoorsTribute) \
				   ,player_consume.AfterDoorsTribute = values(player_consume.AfterDoorsTribute) \
                   ,player_consume.RedStudyNum = values(player_consume.RedStudyNum) \
				   ,player_consume.TaijiStudyNum = values(player_consume.TaijiStudyNum) \
                   ,player_consume.BuyPhyStrengthNum = values(player_consume.BuyPhyStrengthNum)");
    UINT32 dwErr;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(pszSql, NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, dwErr, strError.c_str(), pszSql);
        m_bSuccess = FALSE;
        return FALSE;
    }
    return TRUE;
}

VOID CSavePlayerConsume::SetUserData(VOID* pUserData)
{
    memcpy(&m_stDT_PLAYER_TODAY_CONSUME_DATA, (void*)pUserData, sizeof(DT_PLAYER_TODAY_CONSUME_DATA));
}


