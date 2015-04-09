#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/build/buildmgr.h>
#include "updateattrpropmgr.h"
#include <logic/lua/luamgr.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <framework/gsapi.h>

#include <db/dbmgr.h>

using namespace SGDP;

IMPLEMENT_SINGLETON_PROPMGR(CUpdateAttrPropMgr)

CUpdateAttrPropMgr::CUpdateAttrPropMgr()
{

}

CUpdateAttrPropMgr::~CUpdateAttrPropMgr()
{

}

BOOL CUpdateAttrPropMgr::Init()
{
    if( !InitData() )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitData failed!"), MSG_MARK);
        return FALSE;
    }
    return TRUE;
}
VOID CUpdateAttrPropMgr::UnInit()
{

}

BOOL CUpdateAttrPropMgr::GetUpdateAttrNeedInfo( BOOL bLeader, UINT8 byAttrType, UINT8 byJobType, UINT16 wAttrLevel, UINT32 dwTalent, SUpdateData &data, UINT16 wLevel )
{
    if( dwTalent == 0 )
    {
        USR_INFO(_SDT("[%s: %d]: dwTalent param 0 error!"), MSG_MARK);
        return FALSE;
    }

    data.dwPerCountAddValue = GetAttrPerAddValue( byAttrType, byJobType, wLevel, dwTalent );

    //以下消耗根据策划公式计算，所以直接写入数字

	data.stCost.dwCostCoinValue = Round(CLuamgr::Instance()->GetUpdateAttrCostCoin(dwTalent, wAttrLevel, bLeader));   

    //铜币5000转化为1物品，四舍五入
    data.stCost.wCostItemValue = Round( data.stCost.dwCostCoinValue / 5000.0f );

    //至少值1
    if( data.stCost.wCostItemValue == 0 )
    {
        data.stCost.wCostItemValue = 1;
    }

    return TRUE;
}

UINT16 CUpdateAttrPropMgr::GetTotalCount( UINT16 wLevel )
{
    return wLevel * m_oParam.wUpdateLevelAddCount;
}

BOOL CUpdateAttrPropMgr::InitData()
{
	//return TRUE;//已使用脚本

    CSqlite3RecordSet recordUpdateAttrRes;

    string strSql = "select * from updateattr";
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("updateattr").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
    while(pRes->GetRecord())// 当有行结果时
    {
  //      m_oParam.fTalentAttackFactor = FLOAT( SGDP::SDStrtod(recordUpdateAttrRes.GetFieldValueByName("TalentAttackFactor").c_str(), NULL) );
  //      m_oParam.fTalentHPFactor = FLOAT( SGDP::SDStrtod(recordUpdateAttrRes.GetFieldValueByName("TalentHPFactor").c_str(), NULL) );
  //      m_oParam.fHPScaleToAttack = FLOAT( SGDP::SDStrtod(recordUpdateAttrRes.GetFieldValueByName("HPScaleToAttack").c_str(), NULL) );
  //      m_oParam.fH_AttackFactor = FLOAT( SGDP::SDStrtod(recordUpdateAttrRes.GetFieldValueByName("H_AttackFactor").c_str(), NULL) );
  //      m_oParam.fH_HPFactor = FLOAT( SGDP::SDStrtod(recordUpdateAttrRes.GetFieldValueByName("H_HPFactor").c_str(), NULL) );
		//m_oParam.fA_AttackFactor = FLOAT( SGDP::SDStrtod(recordUpdateAttrRes.GetFieldValueByName("A_AttackFactor").c_str(), NULL) );
		//m_oParam.fA_HPFactor = FLOAT( SGDP::SDStrtod(recordUpdateAttrRes.GetFieldValueByName("A_HPFactor").c_str(), NULL) );
		m_oParam.wStrengthScaleToNormal = SGDP::SDAtou(pRes->GetFieldValueByName("StrengthScaleToNormal"));
		m_oParam.wUpdateLevelAddCount = SGDP::SDAtou(pRes->GetFieldValueByName("UpdateLevelAddCount"));
    }

    return TRUE;
}

DOUBLE CUpdateAttrPropMgr::GetAttrAddValue( UINT32 dwTalent, UINT16 wLevel, UINT8 byAttrType, UINT8 byJobType, BOOL bIncludeDefaultAttr )
{
	if (wLevel < 1)
	{
		return 0;
	}

	if( bIncludeDefaultAttr )
	{
		--wLevel;
	}
	
	return CLuamgr::Instance()->GetAttrAddValue(dwTalent, wLevel, byAttrType, byJobType);

}

UINT32 CUpdateAttrPropMgr::GetAttrPerAddValue( UINT8 byAttrType, UINT8 byJobType, UINT16 wLevel, UINT32 dwTalent )
{
    //四舍五入
	DOUBLE dRet=GetAttrAddValue( dwTalent, wLevel, byAttrType, byJobType, FALSE );
    return Round( dRet * m_oParam.wStrengthScaleToNormal / (m_oParam.wUpdateLevelAddCount * wLevel) );
}