#include "luamgr.h"
#include "toluafunc.h"
#include <sddir.h>
#include <iostream>
#include <sdfile.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <tolua++.h>
#include <logic/player/player.h>
#include <logic/vip/vippropmgr.h>
#include <errdef.h>
#include <framework/gsapi.h>
#include <db/dbmgr.h>
#include <utility.h>


//#include "logic/lua/tclass.h"
//#include "netdata.h"

using namespace std;
using namespace SGDP;

TOLUA_API int tolua_toluainterface_open (lua_State* tolua_S);
//TOLUA_API int tolua_tclasslua_open (lua_State* tolua_S);
//TOLUA_API int tolua_netdataLua_open (lua_State* tolua_S);

IMPLEMENT_SINGLETON_PROPMGR(CLuamgr);

CLuamgr::CLuamgr(): m_oLuaState(TRUE)
{

}
CLuamgr::~CLuamgr()
{

}

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

BOOL CLuamgr::InitLua()
{
    tolua_toluainterface_open(m_oLuaState->GetCState());

    //////////////////////////////////////////////////////////////////////////

  //  int a =  sizeof(wchar_t);
  //  luaopen_base(m_oLuaState->GetCState());
  //  tolua_tclasslua_open( m_oLuaState->GetCState() );
  //  tolua_netdataLua_open( m_oLuaState->GetCState() );

  //  int  nStatus = 0;
  //  vector<string> vecStr;
  //  char szDir[1024] = {0};
  //  sprintf( szDir, "%slua", (const CHAR*)SDGetModulePath());
  //  ListFile( szDir, ".lua", TRUE, vecStr );
  //  for( INT32 nIndex = 0; nIndex < INT32( vecStr.size() ); ++nIndex )
  //  {
  //      nStatus = m_oLuaState->DoFile(vecStr[nIndex].c_str());
  //  }
  //  vecStr.clear();

  //  CHAR		 szNetData[65535] = {0};
  //  CNetData  oNetData;
  //  try
  //  {
  //      //PKT_CLIGS_ENTERGS_REQ stReq;
  //      //oNetData.Prepare(szNetData, sizeof(szNetData), 0, 0, 0);
  //      //LuaFunction<UINT16> ProtoCliGS(m_oLuaState->GetGlobal("ProtoCliGS"));
  //      //ProtoCliGS(CLIGS_ENTERGS_REQ, &oNetData);
		//CMessage oMessage;
		//LuaFunction<UINT16> TestNetData(m_oLuaState->GetGlobal("TestNetData"));
		//TestNetData(CLIGS_ENTERGS_REQ, &oMessage);
  //  }
  //  catch(LuaException e)
  //  {
  //      SYS_CRITICAL(_SDT("[%s: %d]: InitLua failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
  //  }
    //////////////////////////////////////////////////////////////////////////
    return TRUE;
}

BOOL CLuamgr::LoadLuaScript()
{

    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("luafile", "", "select Common1, Common2, Common3, Common4, Common5, Calculate, Activity from ").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    INT32 nStatus = 0;
    if(pRes->GetRecord())
    {
        if((0 == nStatus) && (nStatus = m_oLuaState->DoString(pRes->GetFieldValueByName("Common1")))) {};
        if((0 == nStatus) && (nStatus = m_oLuaState->DoString(pRes->GetFieldValueByName("Common2")))) {};
        if((0 == nStatus) && (nStatus = m_oLuaState->DoString(pRes->GetFieldValueByName("Common3")))) {};
        if((0 == nStatus) && (nStatus = m_oLuaState->DoString(pRes->GetFieldValueByName("Common4")))) {};
        if((0 == nStatus) && (nStatus = m_oLuaState->DoString(pRes->GetFieldValueByName("Common5")))) {};
        if((0 == nStatus) && (nStatus = m_oLuaState->DoString(pRes->GetFieldValueByName("Calculate")))) {};
        if((0 == nStatus) && (nStatus = m_oLuaState->DoString(pRes->GetFieldValueByName("Activity")))) {};
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    if(0 != nStatus)
    {
        LuaStackObject luaError = m_oLuaState->StackTop();
        const char* pszErrorMsg = luaError.GetString();
        SYS_CRITICAL(_SDT("[%s: %d]: require Calculate failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

BOOL CLuamgr::InitLuaActivity()
{
    m_mapActivityInfo.clear();

    LuaObject activityInfo = m_oLuaState->GetGlobal("activityInfo");
    if(activityInfo.IsNil())
    {
        return FALSE;
    }

    UINT8 byIdx = 0;
    do
    {
        SActivityInfo stActivityInfo;
        LuaObject objRow = activityInfo[++byIdx];
        if(objRow.IsNil())
        {
            break;
        }
        LuaObject ID = objRow["ID"];
        if(TRUE == ID.IsNil() || FALSE == ID.IsConvertibleToInteger())
        {
            SYS_CRITICAL(_SDT("[%s: %d]: ID is nill, row %d!"), MSG_MARK, byIdx);
            return FALSE;
        }
        stActivityInfo.dwID = ID.ToInteger();

        LuaObject DspDate = objRow["DspDate"];
        if(TRUE == DspDate.IsNil() || FALSE == DspDate.IsConvertibleToString())
        {
            SYS_CRITICAL(_SDT("[%s: %d]: DspDate is nill, row %d!"), MSG_MARK, byIdx);
            return FALSE;
        }
        stActivityInfo.qwDspDate = 0;
        string strDspDate = DspDate.ToString();
        if(0 != strDspDate.compare("0"))
        {
            stActivityInfo.qwDspDate = GetTimeValue(strDspDate.c_str());
        }

        LuaObject StartDate = objRow["StartDate"];
        if(TRUE == StartDate.IsNil() || FALSE == StartDate.IsConvertibleToString())
        {
            SYS_CRITICAL(_SDT("[%s: %d]: StartDate is nill, row %d!"), MSG_MARK, byIdx);
            return FALSE;
        }
        stActivityInfo.qwStartDate = 0;
        string strStartDate = StartDate.ToString();
        if(0 != strStartDate.compare("0"))
        {
            stActivityInfo.qwStartDate = GetTimeValue(strStartDate.c_str());
        }

        LuaObject EndDate = objRow["EndDate"];
        if(TRUE == EndDate.IsNil() || FALSE == EndDate.IsConvertibleToString())
        {
            SYS_CRITICAL(_SDT("[%s: %d]: EndDate is nill, row %d!"), MSG_MARK, byIdx);
            return FALSE;
        }
        stActivityInfo.qwEndDate = 0;
        string strEndDate = EndDate.ToString();
        if(0 != strEndDate.compare("0"))
        {
            stActivityInfo.qwEndDate = GetTimeValue(strEndDate.c_str());
        }

        LuaObject Name = objRow["Name"];
        if(TRUE == Name.IsNil() || FALSE == Name.IsConvertibleToString())
        {
            SYS_CRITICAL(_SDT("[%s: %d]: Name is nill, row %d!"), MSG_MARK, byIdx);
            return FALSE;
        }
        stActivityInfo.strName = Name.ToString();

        m_mapActivityInfo[stActivityInfo.dwID] = stActivityInfo;

    }
    while (1);


    return TRUE;
}



BOOL CLuamgr::Init()
{
    if(!InitLua())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitLua failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadLuaScript())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadLuaScript failed!"), MSG_MARK);
        return FALSE;
    }

    if(!InitLuaActivity())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitActivityEvent failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

VOID CLuamgr::UnInit()
{
    m_mapActivityInfo.clear();
}



//强化铜钱
UINT32 CLuamgr::GetStrengthenCoin(UINT16 wStrenthenLevel, UINT16 wDressLevel)
{
    UINT32 dwCoin = 0;
    try
    {
        LuaFunction<UINT32> LuaGetStrengthenCoin(m_oLuaState->GetGlobal("GetStrengthenCoin"));
        dwCoin = LuaGetStrengthenCoin(wStrenthenLevel, wDressLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetStrengthenCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwCoin = 0;
    }

    return dwCoin;
}

UINT16 CLuamgr::GetEnchantStoneID(UINT16 wEnchantLevel)
{
    UINT16 wStoneID = 0;
    try{
        LuaFunction<UINT16> LuaGetEnchantStoneID(m_oLuaState->GetGlobal("GetEnchantStoneID"));
        wStoneID = LuaGetEnchantStoneID(wEnchantLevel);
    }catch(LuaException e){
        SYS_CRITICAL(_SDT("[%s: %d]: GetEnchantStoneID failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        wStoneID = 0;
    }

    return wStoneID;
}

UINT32 CLuamgr::GetEnchantStoneCost(UINT16 wEnchantLevel, UINT16 wDressLevel)
{
    UINT16 wCostStone = 0;
    try{
        LuaFunction<UINT16> LuaGetEnchantStoneCost(m_oLuaState->GetGlobal("GetEnchantStoneCost"));
        wCostStone = LuaGetEnchantStoneCost(wEnchantLevel, wDressLevel);
    }catch(LuaException e){
        SYS_CRITICAL(_SDT("[%s: %d]: GetEnchantStoneID failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        wCostStone = 0;
    }

    return wCostStone;
}

//俘虏工作生产铜币
UINT32 CLuamgr::GetCaptureWorkCoin(UINT16 wCaptureLevel)
{
    UINT32 dwCoin = 0;
    try
    {
        LuaFunction<UINT32> LuaGetCaptureWorkCoin = m_oLuaState->GetGlobal("GetCaptureWorkCoin");
        dwCoin = LuaGetCaptureWorkCoin(wCaptureLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetCaptureWorkCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwCoin = 0;
    }

    return dwCoin;
}

//俘虏工作生产器魂
UINT32 CLuamgr::GetCaptureWorkScience(UINT16 wCaptureLevel)
{
    UINT32 dwScience = 0;
    try
    {
        LuaFunction<UINT32> LuaGetCaptureWorkScience = m_oLuaState->GetGlobal("GetCaptureWorkScience");
        dwScience = LuaGetCaptureWorkScience(wCaptureLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetCaptureWorkScience failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwScience = 0;
    }

    return dwScience;
}

//排名奖励——铜币
UINT32 CLuamgr::GetRankEncourageCoin(UINT32 dwRank)
{
    UINT32 dwCoin = 0;
    try
    {
        LuaFunction<UINT32> LuaGetRankEncourageCoin = m_oLuaState->GetGlobal("GetRankEncourageCoin");
        dwCoin = LuaGetRankEncourageCoin(dwRank);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetRankEncourageCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwCoin = 0;
    }

    return dwCoin;
}

//排名奖励——器魂
UINT32 CLuamgr::GetRankEncourageScience(UINT32 dwRank)
{
    UINT32 dwScience = 0;
    try
    {
        LuaFunction<UINT32> LuaGetRankEncourageScience = m_oLuaState->GetGlobal("GetRankEncourageScience");
        dwScience = LuaGetRankEncourageScience(dwRank);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetRankEncourageScience failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwScience = 0;
    }

    return dwScience;
}

//排名奖励——境界点
UINT32 CLuamgr::GetRankEncourageJingJie(UINT32 dwRank)
{
    UINT32 dwJingJie = 0;
    try
    {
        LuaFunction<UINT32> LuaGetRankEncourageJingJie = m_oLuaState->GetGlobal("GetRankEncourageJingJie");
        dwJingJie = LuaGetRankEncourageJingJie(dwRank);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetRankEncourageJingJie failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwJingJie = 0;
    }

    return dwJingJie;
}

//挑战奖励——铜币
UINT32 CLuamgr::GetChallengeEncourageCoin(UINT16 wPlayerLevel, BOOL bWinFlag)
{
    UINT32 dwCoin = 0;
    try
    {
        LuaFunction<UINT32> LuaGetRankEncourageCoin = m_oLuaState->GetGlobal("GetChallengeEncourageCoin");
        dwCoin = LuaGetRankEncourageCoin(wPlayerLevel, bWinFlag);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetChallengeEncourageCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwCoin = 0;
    }

    return dwCoin;
}

//挑战奖励——器魂
UINT32 CLuamgr::GetChallengeEncourageScience(UINT16 wPlayerLevel, BOOL bWinFlag)
{
    UINT32 dwScience = 0;
    try
    {
        LuaFunction<UINT32> LuaGetChallengeEncourageScience = m_oLuaState->GetGlobal("GetChallengeEncourageScience");
        dwScience = LuaGetChallengeEncourageScience(wPlayerLevel, bWinFlag);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetChallengeEncourageScience failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwScience = 0;
    }

    return dwScience;
}

//挑战奖励——境界
UINT32 CLuamgr::GetChallengeEncourageJingJie(UINT16 wPlayerLevel, BOOL bWinFlag)
{
    UINT32 dwJingJie = 0;
    try
    {
        LuaFunction<UINT32> LuaGetChallengeEncourageJingJie = m_oLuaState->GetGlobal("GetChallengeEncourageJingJie");
        dwJingJie = LuaGetChallengeEncourageJingJie(wPlayerLevel, bWinFlag);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetChallengeEncourageJingJie failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwJingJie = 0;
    }

    return dwJingJie;
}


//摇钱树产出——铜币
UINT32 CLuamgr::GetMoneyTreeProduceCoin(UINT16 wPlayerLevel)
{
    UINT32 dwCoin = 0;
    try
    {
        LuaFunction<UINT32> LuaGetMoneyTreeProduceCoin = m_oLuaState->GetGlobal("GetMoneyTreeProduceCoin");
        dwCoin = LuaGetMoneyTreeProduceCoin(wPlayerLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetMoneyTreeProduceCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwCoin = 0;
    }

    return dwCoin;
}

//属性升级——增加的属性值
DOUBLE CLuamgr::GetAttrAddValue(UINT16 wTalent, UINT16 wHeroLevel, UINT8 byAttrType, UINT8 byCareerType)
{
    DOUBLE dwAddValue = 0;
    try
    {
        LuaFunction<DOUBLE> LuaGetAttrAddValue = m_oLuaState->GetGlobal("GetAttrAddValue");
        dwAddValue = LuaGetAttrAddValue(wTalent, wHeroLevel, byAttrType, byCareerType);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetAttrAddValue failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwAddValue = 0;
    }

    return dwAddValue;
}

//属性升级——消耗铜币
UINT32 CLuamgr::GetUpdateAttrCostCoin(UINT16 wTalent, UINT16 wAttrLevel, BOOL bCoachFlag)
{
    UINT32 dwCoin = 0;
    try
    {
        LuaFunction<UINT32> LuaGetUpdateAttrCostCoin = m_oLuaState->GetGlobal("GetUpdateAttrCostCoin");
        dwCoin = LuaGetUpdateAttrCostCoin(wTalent, wAttrLevel, bCoachFlag);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetUpdateAttrCostCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwCoin = 0;
    }

    return dwCoin;
}

//悟道召唤次数
//UINT16 CLuamgr::GetAwakenCallNum()
//{
//    UINT16 wNum = 1;
//    try
//    {
//        LuaFunction<UINT32> LuaGetAwakenCallNum = m_oLuaState->GetGlobal("GetAwakenCallNum");
//        wNum = LuaGetAwakenCallNum();
//    }
//    catch(LuaException e)
//    {
//        SYS_CRITICAL(_SDT("[%s: %d]: GetAwakenCallNum failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
//        wNum = 1;
//    }
//
//    return wNum;
//}

//境界蓝气转换紫气比例(多少蓝气换1紫气)
UINT16 CLuamgr::GetBlueNumToPerPurple()
{
    UINT16 wNum = 100;
    try
    {
        LuaFunction<UINT32> LuaGetBlueNumToPerPurple = m_oLuaState->GetGlobal("GetBlueNumToPerPurple");
        wNum = LuaGetBlueNumToPerPurple();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBlueNumToPerPurple failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        wNum = 100;
    }

    return wNum;
}


//每次点蓝气值
UINT32 CLuamgr::GetClickBlueValue()
{
    UINT32 dwNum = 1000;
    try
    {
        LuaFunction<UINT32> LuaGetClickBlueValue = m_oLuaState->GetGlobal("GetClickBlueValue");
        dwNum = LuaGetClickBlueValue();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetClickBlueValue failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwNum = 1000;
    }

    return dwNum;
}

//获得主城收获铜钱
UINT32 CLuamgr::GetMaintownCrownCoin(UINT16 wPlayerLevel)
{
    UINT32 dwValue = 1000;
    try
    {
        LuaFunction<UINT32> LuaGetMaintownCrownCoin = m_oLuaState->GetGlobal("GetMaintownCrownCoin");
        dwValue = LuaGetMaintownCrownCoin(wPlayerLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetMaintownCrownCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwValue = 1000;
    }

    return dwValue;
}

//获得主城收获元宝
UINT32 CLuamgr::GetMaintownCrownGold(UINT16 wPlayerLevel)
{
    UINT32 dwValue = 1000;
    try
    {
        LuaFunction<UINT32> LuaGetMaintownCrownGold = m_oLuaState->GetGlobal("GetMaintownCrownGold");
        dwValue = LuaGetMaintownCrownGold(wPlayerLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetMaintownCrownGold failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwValue = 1000;
    }

    return dwValue;
}

//获得主城收获境界点
UINT32 CLuamgr::GetMaintownCrownJingJie(UINT16 wPlayerLevel)
{
    UINT32 dwJingJie = 1000;
    try
    {
        LuaFunction<UINT32> LuaGetMaintownCrownJingJie = m_oLuaState->GetGlobal("GetMaintownCrownJingJie");
        dwJingJie = LuaGetMaintownCrownJingJie(wPlayerLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetMaintownCrownJingJie failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwJingJie = 1000;
    }

    return dwJingJie;
}

//获得主城征收铜钱
UINT32 CLuamgr::GetMaintownLevyCoin(UINT16 wPlayerLevel)
{
    UINT32 dwValue = 1000;
    try
    {
        LuaFunction<UINT32> LuaGetMaintownLevyCoin = m_oLuaState->GetGlobal("GetMaintownLevyCoin");
        dwValue = LuaGetMaintownLevyCoin(wPlayerLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetMaintownLevyCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwValue = 1000;
    }

    return dwValue;
}

//获得征收境界点
UINT32 CLuamgr::GetMaintownLevyJingJie(UINT16 wPlayerLevel)
{
    UINT32 dwJingJie = 1000;
    try
    {
        LuaFunction<UINT32> LuaGetMaintownLevyJingJie = m_oLuaState->GetGlobal("GetMaintownLevyJingJie");
        dwJingJie = LuaGetMaintownLevyJingJie(wPlayerLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetMaintownLevyJingJie failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwJingJie = 1000;
    }

    return dwJingJie;
}

//查找该礼包是否需提示
BOOL CLuamgr::CkBoxGoodsPrompt(UINT16 wBoxGoodsID)
{
    BOOL bPrompt = FALSE;
    try
    {
        LuaFunction<UINT32> LuaGetBoxGoodsPrompt = m_oLuaState->GetGlobal("GetBoxGoodsPrompt");
        bPrompt = (1 == LuaGetBoxGoodsPrompt(wBoxGoodsID)) ? TRUE : FALSE;
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBoxGoodsPrompt failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        bPrompt = FALSE;
    }

    return bPrompt;
}


/*
//初始化活动事件
BOOL CLuamgr::InitActivityEvent()
{
	UINT8 byIdx = 0;
	for( byIdx = 0; byIdx < EAE_MAX; byIdx++)
	{
		m_astrLuaFuncName[byIdx] = "";
	}

	LuaObject activityInfo = m_oLuaState->GetGlobal("activityInfo");
	if(activityInfo.IsNil())
	{
		return FALSE;
	}

	byIdx = 0;
	do
	{
		LuaObject obj = activityInfo[++byIdx];
		if(obj.IsNil())
		{
			break;
		}
		LuaObject obj1 = obj.GetByIndex(1);
		if(TRUE == obj1.IsNil() ||
			FALSE == obj1.IsConvertibleToInteger())
			break;

		LuaObject obj2 = obj.GetByIndex(2);
		if(TRUE == obj2.IsNil() ||
			FALSE == obj2.IsConvertibleToString())
			break;

		UINT16 wEventID = obj1.ToInteger();
		if(wEventID > EAE_MAX)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: EventID[%d] > EAE_MAX[%d]!"), MSG_MARK, wEventID, EAE_MAX);
			continue;
		}
		string strFunc = obj2.ToString();

		m_astrLuaFuncName[wEventID] = strFunc;

	} while (1);


	return TRUE;
}
*/

/*

//触发活动事件回调
VOID CLuamgr::OnTriggerActivityEvent(EActivityEvent eActivityEvent)
{
	string strFunc = m_astrLuaFuncName[eActivityEvent];
	if(strFunc.empty())
	{
		return;
	}
	try
	{
		LuaFunction<VOID> ActivityLuaFunc = m_oLuaState->GetGlobal(strFunc.c_str());
		ActivityLuaFunc();
	}
	catch(LuaException e)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: Execute ActivityLuaFunc failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
	}
}
*/

BOOL CLuamgr::CkHaveActivity(UINT32 dwId)
{
    if(m_mapActivityInfo.find(dwId) != m_mapActivityInfo.end())
    {
        return TRUE;
    }

    return FALSE;
}

UINT16 CLuamgr::CkFinishActivity(CPlayer* poPlayer, UINT32 dwId)
{
    UINT16 wErrCode = 0;
    try
    {
        LuaFunction<UINT16> LuaCkFinishActivity = m_oLuaState->GetGlobal("CkFinishActivity");
        wErrCode = LuaCkFinishActivity(poPlayer, dwId);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CkFinishActivity failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        SET_OTHER_ERR(wErrCode)
    }

    return wErrCode;
}

UINT32 CLuamgr::GetFuncCost(CPlayer* poPlayer, ECostFunc func, INT32 nCount, INT32 nIdx)
{
    if (CVipPropMgr::Instance()->CkCostVIP(func))
    {
        //检测当前VIP功能的开放等级
        int byVipLevel = 0;
        for ( byVipLevel = 0; byVipLevel <= CVipPropMgr::Instance()->GetMaxVipLevel(); byVipLevel++)
        {
            if (CVipPropMgr::Instance()->GetIncNum(func, byVipLevel) > 0)
            {
                break;
            }
        }

        //检测玩家的VIP等级是否达到开放等级，如果未达到，直接返回开放等级的值。
        if (poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel < byVipLevel)
        {
            return GetCostByFunc(byVipLevel, func, nCount, nIdx);
        }

        //检测当前VIP功能是否开放
        for ( byVipLevel; byVipLevel <= poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel; byVipLevel++)
        {
            if (CVipPropMgr::Instance()->GetIncNum(func, byVipLevel) > nCount)
            {
                return GetCostByFunc(byVipLevel, func, nCount, nIdx);
            }
        }

        //对于没有找到的，直接下一阶的数据
        if (poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel < CVipPropMgr::Instance()->GetMaxVipLevel())
        {
            return GetCostByFunc(poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel + 1, func, nCount, nIdx);
        }
        else
        {
            return GetCostByFunc(CVipPropMgr::Instance()->GetMaxVipLevel(), func, nCount, nIdx);
        }

        SYS_CRITICAL(_SDT("[%s: %d]: GetFuncCost failed, playerid:%u, func:%u, count:%u, Idx:%u!"), MSG_MARK, poPlayer->GetID(), func, nCount, nIdx);
    }
    else
    {
        return GetCostByFunc(0, func, nCount, nIdx);
    }
    return 0;
}

//获得功能的聚魂数
UINT32 CLuamgr::GetFuncObtain(CPlayer* poPlayer, EObtainFunc func, INT32 nCount, INT32 nIdx)
{
    return GetObtainByFunc(poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, (UINT8)poPlayer->GetLevel(), func, nCount, nIdx);
}



//获得功能的元宝消耗
UINT32 CLuamgr::GetCostByFunc(UINT8 byVipLevel, ECostFunc func, INT32 nCount, INT32 nIdx)
{
    UINT32 nRet = 0;
    try
    {
        LuaFunction<UINT32> GetCostByFunc = m_oLuaState->GetGlobal("GetCostByFunc");
        nRet = GetCostByFunc(byVipLevel, func, nCount, nIdx);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetCostByFunc failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        nRet = 0;
    }
    return nRet;
}

//获得功能的聚魂数
UINT32 CLuamgr::GetObtainByFunc(UINT8 byVipLevel, UINT8 byLevel, EObtainFunc func, INT32 nCount, INT32 nIdx)
{
    UINT32 nRet = 0;
    try
    {
        LuaFunction<UINT32> GetObtainByFunc = m_oLuaState->GetGlobal("GetObtainByFunc");
        nRet = GetObtainByFunc(byVipLevel, byLevel, func, nCount, nIdx);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetScienceCostByFunc failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        nRet = 0;
    }
    return nRet;
}


UINT32 CLuamgr::GetPassIncGenGasNumTime()
{
    UINT32 dwTime = 0;
    try
    {
        LuaFunction<UINT32> LuaGetPassIncGenGasNumTime = m_oLuaState->GetGlobal("GetPassIncGenGasNumTime");
        dwTime = LuaGetPassIncGenGasNumTime(GetDayPassSecond());
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetPassIncGenGasNumTime failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwTime = 0;
    }

    return dwTime;
}


UINT32 CLuamgr::GetBossBTopEncCoin(UINT16 wPlayerLevel, UINT16 wTop)
{
    UINT32 nRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetBossBTopEncCoin = m_oLuaState->GetGlobal("GetBossBTopEncCoin");
        nRet = LuaGetBossBTopEncCoin(wPlayerLevel, wTop);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBossBTopEncCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        nRet = 0;
    }

    return nRet;
}

UINT32 CLuamgr::GetBossBTopEncJingJie(UINT16 wPlayerLevel, UINT16 wTop)
{
    UINT32 nRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetBossBTopEncJingJie = m_oLuaState->GetGlobal("GetBossBTopEncJingJie");
        nRet = LuaGetBossBTopEncJingJie(wPlayerLevel, wTop);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBossBTopEncJingJie failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        nRet = 0;
    }

    return nRet;
}

UINT32 CLuamgr::GetBossBKillEncCoin(UINT16 wBossLevel)
{
    UINT32 nRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetBossBKillEncCoin = m_oLuaState->GetGlobal("GetBossBKillEncCoin");
        nRet = LuaGetBossBKillEncCoin(wBossLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBossBKillEncCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        nRet = 0;
    }

    return nRet;
}

UINT32 CLuamgr::GetBossBKillEncJingJie(UINT16 wBossLevel)
{
    UINT32 nRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetBossBKillEncJingJie = m_oLuaState->GetGlobal("GetBossBKillEncJingJie");
        nRet = LuaGetBossBKillEncJingJie(wBossLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBossBKillEncJingJie failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        nRet = 0;
    }

    return nRet;
}

UINT32 CLuamgr::GetBossBHurtEncCoin(UINT16 wPlayerLevel, UINT32 dwHurtValue)
{
    UINT32 nRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetBossBHurtEncCoin = m_oLuaState->GetGlobal("GetBossBHurtEncCoin");
        nRet = LuaGetBossBHurtEncCoin(wPlayerLevel, dwHurtValue);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBossBHurtEncCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        nRet = 0;
    }

    return nRet;
}

UINT32 CLuamgr::GetBossBHurtEncJingJie(UINT16 wPlayerLevel, UINT32 dwHurtValue)
{
    UINT32 nRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetBossBHurtEncJingJie = m_oLuaState->GetGlobal("GetBossBHurtEncJingJie");
        nRet = LuaGetBossBHurtEncJingJie(wPlayerLevel, dwHurtValue);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBossBHurtEncJingJie failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        nRet = 0;
    }

    return nRet;
}

UINT32 CLuamgr::GetBossBHeroHurtEncCoin()
{
    UINT32 nRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetBossBHeroHurtEncCoin = m_oLuaState->GetGlobal("GetBossBHeroHurtEncCoin");
        nRet = LuaGetBossBHeroHurtEncCoin();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBossBHeroHurtEncCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        nRet = 0;
    }

    return nRet;
}

UINT32 CLuamgr::GetBossBHeroHurtEncJingJie()
{
    UINT32 nRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetBossBHeroHurtEncJingJie = m_oLuaState->GetGlobal("GetBossBHeroHurtEncJingJie");
        nRet = LuaGetBossBHeroHurtEncJingJie();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBossBHeroHurtEncJingJie failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        nRet = 0;
    }

    return nRet;
}

UINT8 CLuamgr::GetInspireRate(UINT8 byGoldInspireFlag, UINT16 wIncPowerRate)
{
    UINT8 byRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetInspireRate = m_oLuaState->GetGlobal("GetInspireRate");
        byRet = LuaGetInspireRate(byGoldInspireFlag, wIncPowerRate);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetlInspireRate failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        byRet = 0;
    }

    return byRet;
}

UINT16 CLuamgr::GetIncRateByStory()
{
    UINT16 wRet = 10;
    try
    {
        LuaFunction<UINT32> LuaGetIncRateByStory = m_oLuaState->GetGlobal("GetIncRateByStory");
        wRet = LuaGetIncRateByStory();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetIncRateByStory failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        wRet = 0;
    }

    return wRet;
}

UINT16 CLuamgr::GetIncRateByGold()
{
    UINT16 wRet = 10;
    try
    {
        LuaFunction<UINT32> LuaGetIncRateByGold = m_oLuaState->GetGlobal("GetIncRateByGold");
        wRet = LuaGetIncRateByGold();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetIncRateByGold failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        wRet = 0;
    }

    return wRet;
}


UINT32 CLuamgr::GetRdchallengeEncJingJie(UINT8 byRet, UINT16 wMyLevel, UINT16 wEnemyLevel)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetRdchallengeEncJingJie = m_oLuaState->GetGlobal("GetRdchallengeEncJingJie");
        dwRet = LuaGetRdchallengeEncJingJie(byRet, wMyLevel, wEnemyLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetRdchallengeEncJingJie failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

UINT32 CLuamgr::GetRdchallengeEncPurpleGas(UINT8 byRet, UINT16 wMyLevel, UINT16 wEnemyLevel)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetRdchallengeEncPurpleGas = m_oLuaState->GetGlobal("GetRdchallengeEncPurpleGas");
        dwRet = LuaGetRdchallengeEncPurpleGas(byRet, wMyLevel, wEnemyLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetRdchallengeEncJPurpleGas failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

UINT32 CLuamgr::GetMeltGold(UINT32 dwScience)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetMeltGold = m_oLuaState->GetGlobal("GetMeltGold");
        dwRet = LuaGetMeltGold(dwScience);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetMeltGold failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

//获得魂魂倍率
UINT32 CLuamgr::GetGatherScienceIncrease(INT32 nIdx)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetGatherScienceIncrease = m_oLuaState->GetGlobal("GetGatherScienceIncrease");
        dwRet = LuaGetGatherScienceIncrease(nIdx);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetGatherScienceIncrease failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

//获得魂魂倍率百分比
UINT32 CLuamgr::GetGatherScienceIncreasePer(INT32 nIdx, INT32 nFunc)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetGatherScienceIncreasePer = m_oLuaState->GetGlobal("GetGatherScienceIncreasePer");
        dwRet = LuaGetGatherScienceIncreasePer(nIdx, nFunc);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetGatherScienceIncreasePer failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

// 根据当前爬塔层数来获取当前求仙露的最大次数
UINT32 CLuamgr::GetBegBlueMaxTimes(UINT32 dwClimbTowerLayer)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetBegBlueMaxTimes = m_oLuaState->GetGlobal("GetBegBlueMaxTimes");
        dwRet = LuaGetBegBlueMaxTimes(dwClimbTowerLayer);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBegBlueMaxTimes failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

// 获取求仙露每天免费改运次数
UINT32 CLuamgr::GetFreeImproveLuckTimes()
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetFreeImproveLuckTimes = m_oLuaState->GetGlobal("GetFreeImproveLuckTimes");
        dwRet = LuaGetFreeImproveLuckTimes();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetFreeImproveLuckTimes failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

// 获取下一个爬塔提示层
UINT32 CLuamgr::GetClimbTowerPrompt(UINT32 dwClimbTowerLayer)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetClimbTowerPrompt = m_oLuaState->GetGlobal("GetClimbTowerPrompt");
        dwRet = LuaGetClimbTowerPrompt(dwClimbTowerLayer);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetClimbTowerPrompt failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

// 获取求仙露是否忽略倍率
UINT8  CLuamgr::GetBegBlueIsIgnoreXnMultiple()
{
    UINT8 byRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetBegBlueIsIgnoreXnMultiple = m_oLuaState->GetGlobal("GetBegBlueIsIgnoreXnMultiple");
        byRet = LuaGetBegBlueIsIgnoreXnMultiple();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBegBlueIsIgnoreXnMultiple failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        byRet = 0;
    }

    return byRet;
}


//获得聚气的次数
UINT32 CLuamgr::GetGenGasNum(UINT8 byVipLevel, UINT8 byLevel, UINT8 byTownIdx, UINT8 byInstanceIdx)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetGenGasNum = m_oLuaState->GetGlobal("GetGenGasNum");
        dwRet = LuaGetGenGasNum(byVipLevel, byLevel, byTownIdx, byInstanceIdx);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetGenGasNum failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

UINT16 CLuamgr::GetTotalFreeCommonUpGasNum()
{
    UINT16 wRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetTotalFreeCommonUpGasNum = m_oLuaState->GetGlobal("GetTotalFreeCommonUpGasNum");
        wRet = LuaGetTotalFreeCommonUpGasNum();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetGenGasNum failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        wRet = 0;
    }
    return wRet;
}


UINT32 CLuamgr::GetBossBHurtValueByPower(UINT32 dwPower)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetBossBHurtValueByPower = m_oLuaState->GetGlobal("GetBossBHurtValueByPower");
        dwRet = LuaGetBossBHurtValueByPower(dwPower);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetBossBHurtValueByPower failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = dwPower / 10;
    }
    return dwRet;
}

//筋斗云费用
UINT32 CLuamgr::GetFindMonsterGold()
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetGoldByFindMonster = m_oLuaState->GetGlobal("GetGoldByFindMonster");
        dwRet = LuaGetGoldByFindMonster();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetGoldByFindMonster failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }
    return dwRet;
}
//获取免费更新妖怪次数
UINT32 CLuamgr::GetFreeRefreshNum()
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetFreeRefreshNum = m_oLuaState->GetGlobal("GetFreeRefreshNum");
        dwRet = LuaGetFreeRefreshNum();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetFreeRefreshNum failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }
    return dwRet;
}


UINT32 CLuamgr::GetCoinByBadWine()
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetCoinByBadWine = m_oLuaState->GetGlobal("GetCoinByBadWine");
        dwRet = LuaGetCoinByBadWine();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetCoinByBadWine failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }
    return dwRet;
}

UINT32 CLuamgr::GetDoorsTributeIntegral()
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetDoorsTributeIntegral = m_oLuaState->GetGlobal("GetDoorsTributeIntegral");
        dwRet = LuaGetDoorsTributeIntegral();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetDoorsTributeIntegral failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }
    return dwRet;
}
//获取捐献一次消耗铜钱
UINT32 CLuamgr::GetCostCoinByDonate(UINT32 dwFactionLevel)
{

    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetCostCoinByDonate = m_oLuaState->GetGlobal("GetCostCoinByDonate");
        dwRet = LuaGetCostCoinByDonate(dwFactionLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetCostCoinByDonate failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }
    return dwRet;
}
//获取捐献一次消耗元宝
UINT32 CLuamgr::GetCostGoldByDonate(UINT32 dwFactionLevel)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetCostGoldByDonate = m_oLuaState->GetGlobal("GetCostGoldByDonate");
        dwRet = LuaGetCostGoldByDonate(dwFactionLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetCostGoldByDonate failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }
    return dwRet;
}
//捐献一次铜钱增加资金数
UINT32 CLuamgr::GetFundsByDonateCoin(UINT32 dwFactionLevel)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetFundsByDonateCoin = m_oLuaState->GetGlobal("GetFundsByDonateCoin");
        dwRet = LuaGetFundsByDonateCoin(dwFactionLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetFundsByDonateCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }
    return dwRet;
}
//捐献一次元宝增加资金数
UINT32 CLuamgr::GetFundsByDonateGold(UINT32 dwFactionLevel)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetFundsByDonateGold = m_oLuaState->GetGlobal("GetFundsByDonateGold");
        dwRet = LuaGetFundsByDonateGold(dwFactionLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetFundsByDonateGold failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }
    return dwRet;
}

//捐献一次铜钱增加门贡
UINT32 CLuamgr::GetDoorsTributeByDonateCoin(UINT32 dwFactionLevel)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetDoorsTributeByDonateCoin = m_oLuaState->GetGlobal("GetDoorsTributeByDonateCoin");
        dwRet = LuaGetDoorsTributeByDonateCoin(dwFactionLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetDoorsTributeByDonateCoin failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }
    return dwRet;
}
//捐献一次元宝增加门贡
UINT32 CLuamgr::GetDoorsTributeByDonateGold(UINT32 dwFactionLevel)
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetDoorsTributeByDonateGold = m_oLuaState->GetGlobal("GetDoorsTributeByDonateGold");
        dwRet = LuaGetDoorsTributeByDonateGold(dwFactionLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetDoorsTributeByDonateGold failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }
    return dwRet;
}

//根据人物等级获取试炼组队的最低等级
UINT32 CLuamgr::GetCourageGroupLevel( UINT32 dwLevel )
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetCourageGroupLevel = m_oLuaState->GetGlobal("GetCourageGroupLevel");
        dwRet = LuaGetCourageGroupLevel(dwLevel);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetCourageGroupLevel failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

UINT32 CLuamgr::GetTotalFreeCourageTimes()
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetTotalFreeCourageTimes = m_oLuaState->GetGlobal("GetTotalFreeCourageTimes");
        dwRet = LuaGetTotalFreeCourageTimes();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetTotalFreeCourageTimes failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

UINT32 CLuamgr::GetTotalHelpCourageTimes()
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetTotalHelpCourageTimes = m_oLuaState->GetGlobal("GetTotalHelpCourageTimes");
        dwRet = LuaGetTotalHelpCourageTimes();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetTotalHelpCourageTimes failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

UINT32 CLuamgr::GetAddCourageTimes()
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetAddCourageTimes = m_oLuaState->GetGlobal("GetAddCourageTimes");
        dwRet = LuaGetAddCourageTimes();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetAddCourageTimes failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

//当前VIP可购买的试炼次数
UINT32 CLuamgr::GetVipBuyCourageTimes(UINT8 byVip )
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetVipBuyCourageTimes = m_oLuaState->GetGlobal("GetVipBuyCourageTimes");
        dwRet = LuaGetVipBuyCourageTimes(byVip);
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetCourageGroupLevel failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

UINT32 CLuamgr::GetGoldModeMulti()
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetGoldModeMulti = m_oLuaState->GetGlobal("GetGoldModeMulti");
        dwRet = LuaGetGoldModeMulti();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetGoldModeMulti failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

UINT32 CLuamgr::GetJoinFactionInterval()
{
    UINT32 dwRet = 0;
    try
    {
        LuaFunction<UINT32> LuaGetJoinFactionInterval = m_oLuaState->GetGlobal("GetJoinFactionInterval");
        dwRet = LuaGetJoinFactionInterval();
    }
    catch(LuaException e)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetJoinFactionInterval failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
        dwRet = 0;
    }

    return dwRet;
}

UINT32 CLuamgr::GetNormalFindBackDoorsTribute(UINT32 dwLeftDoorsTribute)
{
	UINT32 dwRet = 0;
	try
	{
		LuaFunction<UINT32> LuaGetNormalFindBackDoorsTribute = m_oLuaState->GetGlobal("GetNormalFindBackDoorsTribute");
		dwRet = LuaGetNormalFindBackDoorsTribute( dwLeftDoorsTribute );
	}
	catch(LuaException e)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: GetJoinFactionInterval failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
		dwRet = 0;
	}

	return dwRet;
}

UINT32 CLuamgr::GetPerfectFindBackDoorsTribute(UINT32 dwLeftDoorsTribute)
{
	UINT32 dwRet = 0;
	try
	{
		LuaFunction<UINT32> LuaGetPerfectFindBackDoorsTribute = m_oLuaState->GetGlobal("GetPerfectFindBackDoorsTribute");
		dwRet = LuaGetPerfectFindBackDoorsTribute( dwLeftDoorsTribute );
	}
	catch(LuaException e)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: GetPerfectFindBackDoorsTribute failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
		dwRet = 0;
	}

	return dwRet;
}

UINT32 CLuamgr::GetMaxExprience(UINT32 dwVipLevel, UINT32 dwFactionLevel)
{
	UINT32 dwRet = 0;
	try
	{
		LuaFunction<UINT32> LuaGetMaxExprience = m_oLuaState->GetGlobal("GetMaxExprience");
		dwRet = LuaGetMaxExprience( dwVipLevel, dwFactionLevel );
	}
	catch(LuaException e)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LuaGetMaxExprience failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
		dwRet = 0;
	}

	return dwRet;
}

UINT32 CLuamgr::GetExprienceBase(UINT32 dwPlayerLevel, UINT32 dwFactionLevel)
{
	UINT32 dwRet = 0;
	try
	{
		LuaFunction<UINT32> LuaGetExprienceBase = m_oLuaState->GetGlobal("GetExprienceAddBase");
		dwRet = LuaGetExprienceBase( dwPlayerLevel, dwFactionLevel );
	}
	catch(LuaException e)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LuaGetExprienceBase failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
		dwRet = 0;
	}

	return dwRet;
}

//生成饰品等级
UINT32 CLuamgr::GenerateJewelryLevel(UINT16 wFirstLevel, UINT16 wSecondLevel, UINT16 wThirdLevel, UINT16 wFourthLevel)
{
	UINT32 dwRet = 0;
	try
	{
		LuaFunction<UINT32> LuaGenerateJewelryLevel = m_oLuaState->GetGlobal("GenerateJewelryLevel");
		dwRet = LuaGenerateJewelryLevel( wFirstLevel, wSecondLevel, wThirdLevel, wFourthLevel );
	}
	catch(LuaException e)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LuaGenerateJewelryLevel failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
		dwRet = 0;
	}

	return dwRet;
}
//生成饰品品质
UINT8 CLuamgr::GenerateJewelryColor(UINT8 byColor1, UINT8 byColor2, UINT8 byColor3, UINT8 byColor4)
{
	UINT8 byRet = 0;
	try
	{
		LuaFunction<UINT32> LuaGenerateJewelryColor = m_oLuaState->GetGlobal("GenerateJewelryColor");
		byRet = LuaGenerateJewelryColor( byColor1, byColor2, byColor3, byColor4 );
	}
	catch(LuaException e)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LuaGenerateJewelryColor failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
		byRet = 0;
	}

	return byRet;
}

UINT32 CLuamgr::GenerateJewelryCost(UINT16 wDressLevel, UINT8 byColor)
{
	UINT32 dwRet = 0;
	try
	{
		LuaFunction<UINT32> LuaGenerateJewelryCost = m_oLuaState->GetGlobal("GenerateJewelryCost");
		dwRet = LuaGenerateJewelryCost( wDressLevel, byColor );
	}
	catch(LuaException e)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LuaGenerateJewelryCost failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
		dwRet = 0;
	}

	return dwRet;
}

UINT8 CLuamgr::GetProcuctSoulCostType(UINT8 ucProductyType, UINT8 ucSearchType, UINT32 unNum) {
	UINT32 dwRet = 0;
	try {
		LuaFunction<UINT32> luaFunc = m_oLuaState->GetGlobal("GetProcuctSoulCostType");
		dwRet = luaFunc(ucProductyType, ucSearchType, unNum);
	} catch(LuaException e) {
		SYS_CRITICAL(_SDT("[%s: %d]: GetProcuctSoulCostType failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
		dwRet = 0;
	}
	return dwRet;
}

UINT32 CLuamgr::GetProcuctSoulCostNum(UINT8 ucProductyType, UINT8 ucSearchType, UINT32 unNum) {
	UINT32 dwRet = 0;
	try {
		LuaFunction<UINT32> luaFunc = m_oLuaState->GetGlobal("GetProductSoulCostNum");
		dwRet = luaFunc(ucProductyType, ucSearchType, unNum);
	} catch(LuaException e) {
		SYS_CRITICAL(_SDT("[%s: %d]: GetProcuctSoulCostNum failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
		dwRet = 0;
	}
	return dwRet;
}

UINT32 CLuamgr::GetSoulRefleshCost(UINT8 ucLockNum) {
	UINT32 dwRet = 0;
	try {
		LuaFunction<UINT32> luaFunc = m_oLuaState->GetGlobal("GetSoulRefleshCost");
		dwRet = luaFunc(ucLockNum);
	} catch(LuaException e) {
		SYS_CRITICAL(_SDT("[%s: %d]: GetSoulRefleshCost failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
		dwRet = 0;
	}
	return dwRet;
}

UINT32 CLuamgr::GetSoulDevourCost(UINT8 ucDevourCost) {
	UINT32 dwRet = 0;
	try {
		LuaFunction<UINT32> luaFunc = m_oLuaState->GetGlobal("GetSoulDevourCost");
		dwRet = luaFunc(ucDevourCost);
	} catch(LuaException e) {
		SYS_CRITICAL(_SDT("[%s: %d]: GetSoulDevourCost failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
		dwRet = 0;
	}
	return dwRet;
}

UINT32 CLuamgr::GetSkillNumByPlayerLevel(UINT8 bySkillType, UINT16 wPlayerLevel)
{
	UINT32 dwRet = 0;
	try {
		LuaFunction<UINT32> luaFunc = m_oLuaState->GetGlobal("GetSkillNumByPlayerLevel");
		dwRet = luaFunc(bySkillType, wPlayerLevel);
	} catch(LuaException e) {
		SYS_CRITICAL(_SDT("[%s: %d]: GetSoulDevourCost failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
		dwRet = 0;
	}
	return dwRet;
}

UINT32 CLuamgr::GetPetFeedVIPCostGold(UINT8 ucVipLevel, UINT8 ucVipPetFeedCnt) {
	UINT32 dwRet = 0;
	try {
		LuaFunction<UINT32> luaFunc = m_oLuaState->GetGlobal("GetPetFeedVIPCostGold");
		dwRet = luaFunc(ucVipLevel, ucVipPetFeedCnt);
	} catch(LuaException e) {
		SYS_CRITICAL(_SDT("[%s: %d]: GetPetFeedVIPCostGold failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
		dwRet = 0;
	}
	return dwRet;
}

// UINT32 CLuamgr::RefinePetGetPetType(UINT8 ucPetType1, UINT8 ucPetType2) {
// 	UINT32 dwRet = 0;
// 	try {
// 		LuaFunction<UINT32> luaFunc = m_oLuaState->GetGlobal("RefinePetGetPetType");
// 		dwRet = luaFunc(ucPetType1, ucPetType2);
// 	} catch(LuaException e) {
// 		SYS_CRITICAL(_SDT("[%s: %d]: RefinePetGetPetType failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
// 		dwRet = 0;
// 	}
// 	return dwRet;
// }
// 
// UINT32 CLuamgr::RefinePetGetPetQuality(UINT8 ucPetQuality1, UINT8 ucPetQuality2) {
// 	UINT32 dwRet = 0;
// 	try {
// 		LuaFunction<UINT32> luaFunc = m_oLuaState->GetGlobal("RefinePetGetPetQuality");
// 		dwRet = luaFunc(ucPetQuality1, ucPetQuality2);
// 	} catch(LuaException e) {
// 		SYS_CRITICAL(_SDT("[%s: %d]: RefinePetGetPetQuality failed, lua err msg:%s!"), MSG_MARK, e.GetErrorMessage());
// 		dwRet = 0;
// 	}
// 	return dwRet;
// }
