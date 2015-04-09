#include <json/json.h>
#include "protocommondata.h"
#include <dll/sdframework/sdframework.h>
#include <sstream>

CProtoCommonData::CProtoCommonData()
{
    m_vecMsgList.clear() ;
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	m_iMaxPkgSize = 0;
	Init();
}

CProtoCommonData * CProtoCommonData::Instance()
{
   static CProtoCommonData oInst;
   return &oInst;
}

CProtoCommonData::~CProtoCommonData()
{
}

INT32 CProtoCommonData::Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);

	EnFuncCProtoCommonData pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

INT32 CProtoCommonData::Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize)
{
	m_oData.Prepare(pNet, iNetSize, 1, 3, 1);
	if(iHostSize < m_iMaxPkgSize)
		return -1;

	DeFuncCProtoCommonData pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCProtoCommonData CProtoCommonData::FindEncodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOCOMMONDATA_MSGID_OFFSET >= PROTOCOMMONDATA_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - PROTOCOMMONDATA_MSGID_OFFSET];
}

DeFuncCProtoCommonData CProtoCommonData::FindDecodeFunc(INT32 iMsgID)
{
	if(iMsgID - PROTOCOMMONDATA_MSGID_OFFSET >= PROTOCOMMONDATA_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - PROTOCOMMONDATA_MSGID_OFFSET];
}

BOOL CProtoCommonData::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	return true;
}

INT32 EncodeDT_PLAYER_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_BASE_DATA *pstIn = (DT_PLAYER_BASE_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwID))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszDispName, sizeof(pstIn->aszDispName)))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwCoin))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwGold))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwStory))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wPhyStrength))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwScience))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwGuideRecord))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byRobotFunc))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPower))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwFirstAttack))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwTodayFirstLoginTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLAYER_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_BASE_DATA *pstOut = (DT_PLAYER_BASE_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwID))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszDispName, sizeof(pstOut->aszDispName)))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwCoin))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwGold))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwStory))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wPhyStrength))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwScience))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwGuideRecord))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byRobotFunc))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPower))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwFirstAttack))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwTodayFirstLoginTime))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_PLAYER_BASE_DATA(DT_PLAYER_BASE_DATA &data)
{
	Json::Value root;
	root["ID"]=data.dwID;
	root["DispName"]=data.aszDispName;
	ostringstream ssqwCoin;
	ssqwCoin<<data.qwCoin;
	root["Coin"]=ssqwCoin.str();
	ostringstream ssqwGold;
	ssqwGold<<data.qwGold;
	root["Gold"]=ssqwGold.str();
	ostringstream ssqwStory;
	ssqwStory<<data.qwStory;
	root["Story"]=ssqwStory.str();
	root["PhyStrength"]=data.wPhyStrength;
	ostringstream ssqwScience;
	ssqwScience<<data.qwScience;
	root["Science"]=ssqwScience.str();
	ostringstream ssqwGuideRecord;
	ssqwGuideRecord<<data.qwGuideRecord;
	root["GuideRecord"]=ssqwGuideRecord.str();
	root["RobotFunc"]=data.byRobotFunc;
	root["Power"]=data.dwPower;
	root["FirstAttack"]=data.dwFirstAttack;
	ostringstream ssqwTodayFirstLoginTime;
	ssqwTodayFirstLoginTime<<data.qwTodayFirstLoginTime;
	root["TodayFirstLoginTime"]=ssqwTodayFirstLoginTime.str();

	return root;
}
BOOL FromJsonDT_PLAYER_BASE_DATA(Json::Value &root, DT_PLAYER_BASE_DATA &data)
{
	data.dwID=root["ID"].asUInt();
	strncpy( data.aszDispName,root["DispName"].asCString(),USERNAME_LEN);
	data.qwCoin=SDAtou64( root["Coin"].asCString() );
	data.qwGold=SDAtou64( root["Gold"].asCString() );
	data.qwStory=SDAtou64( root["Story"].asCString() );
	data.wPhyStrength=root["PhyStrength"].asUInt();
	data.qwScience=SDAtou64( root["Science"].asCString() );
	data.qwGuideRecord=SDAtou64( root["GuideRecord"].asCString() );
	data.byRobotFunc=root["RobotFunc"].asUInt();
	data.dwPower=root["Power"].asUInt();
	data.dwFirstAttack=root["FirstAttack"].asUInt();
	data.qwTodayFirstLoginTime=SDAtou64( root["TodayFirstLoginTime"].asCString() );

	return TRUE;
}
BOOL FromJsonDT_PLAYER_BASE_DATA(const string &strData, DT_PLAYER_BASE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_PLAYER_BASE_DATA(root, data);
}
INT32 EncodeDT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_ATTRIBUTE *pstIn = (DT_BATTLE_ATTRIBUTE*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwHP))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwAttack))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_ATTRIBUTE *pstOut = (DT_BATTLE_ATTRIBUTE*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwHP))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwAttack))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_BATTLE_ATTRIBUTE(DT_BATTLE_ATTRIBUTE &data)
{
	Json::Value root;
	root["HP"]=data.dwHP;
	root["Attack"]=data.dwAttack;

	return root;
}
BOOL FromJsonDT_BATTLE_ATTRIBUTE(Json::Value &root, DT_BATTLE_ATTRIBUTE &data)
{
	data.dwHP=root["HP"].asUInt();
	data.dwAttack=root["Attack"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_BATTLE_ATTRIBUTE(const string &strData, DT_BATTLE_ATTRIBUTE &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_BATTLE_ATTRIBUTE(root, data);
}
INT32 EncodeDT_HIDE_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData)
{
	DT_HIDE_BATTLE_ATTRIBUTE *pstIn = (DT_HIDE_BATTLE_ATTRIBUTE*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wHitRate))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wDodgeRate))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCritRate))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wDeCritRate))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wAngryValue))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HIDE_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData)
{
	DT_HIDE_BATTLE_ATTRIBUTE *pstOut = (DT_HIDE_BATTLE_ATTRIBUTE*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wHitRate))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wDodgeRate))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCritRate))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wDeCritRate))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wAngryValue))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_HIDE_BATTLE_ATTRIBUTE(DT_HIDE_BATTLE_ATTRIBUTE &data)
{
	Json::Value root;
	root["HitRate"]=data.wHitRate;
	root["DodgeRate"]=data.wDodgeRate;
	root["CritRate"]=data.wCritRate;
	root["DeCritRate"]=data.wDeCritRate;
	root["AngryValue"]=data.wAngryValue;

	return root;
}
BOOL FromJsonDT_HIDE_BATTLE_ATTRIBUTE(Json::Value &root, DT_HIDE_BATTLE_ATTRIBUTE &data)
{
	data.wHitRate=root["HitRate"].asUInt();
	data.wDodgeRate=root["DodgeRate"].asUInt();
	data.wCritRate=root["CritRate"].asUInt();
	data.wDeCritRate=root["DeCritRate"].asUInt();
	data.wAngryValue=root["AngryValue"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_HIDE_BATTLE_ATTRIBUTE(const string &strData, DT_HIDE_BATTLE_ATTRIBUTE &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_HIDE_BATTLE_ATTRIBUTE(root, data);
}
INT32 EncodeDT_EXT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData)
{
	DT_EXT_BATTLE_ATTRIBUTE *pstIn = (DT_EXT_BATTLE_ATTRIBUTE*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wIncHurtRate))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wDecHurtRate))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wOddAngry))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwFirstAttack))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCoachTalent))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_EXT_BATTLE_ATTRIBUTE(void *pHost, CNetData* poNetData)
{
	DT_EXT_BATTLE_ATTRIBUTE *pstOut = (DT_EXT_BATTLE_ATTRIBUTE*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wIncHurtRate))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wDecHurtRate))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wOddAngry))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwFirstAttack))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCoachTalent))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_EXT_BATTLE_ATTRIBUTE(DT_EXT_BATTLE_ATTRIBUTE &data)
{
	Json::Value root;
	root["IncHurtRate"]=data.wIncHurtRate;
	root["DecHurtRate"]=data.wDecHurtRate;
	root["OddAngry"]=data.wOddAngry;
	root["FirstAttack"]=data.dwFirstAttack;
	root["CoachTalent"]=data.wCoachTalent;

	return root;
}
BOOL FromJsonDT_EXT_BATTLE_ATTRIBUTE(Json::Value &root, DT_EXT_BATTLE_ATTRIBUTE &data)
{
	data.wIncHurtRate=root["IncHurtRate"].asUInt();
	data.wDecHurtRate=root["DecHurtRate"].asUInt();
	data.wOddAngry=root["OddAngry"].asUInt();
	data.dwFirstAttack=root["FirstAttack"].asUInt();
	data.wCoachTalent=root["CoachTalent"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_EXT_BATTLE_ATTRIBUTE(const string &strData, DT_EXT_BATTLE_ATTRIBUTE &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_EXT_BATTLE_ATTRIBUTE(root, data);
}
INT32 EncodeDT_HERO_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_UPDATE_ATTR_DATA *pstIn = (DT_HERO_UPDATE_ATTR_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wAttrHP))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wAttrAttack))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HERO_UPDATE_ATTR_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_UPDATE_ATTR_DATA *pstOut = (DT_HERO_UPDATE_ATTR_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wAttrHP))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wAttrAttack))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_HERO_UPDATE_ATTR_DATA(DT_HERO_UPDATE_ATTR_DATA &data)
{
	Json::Value root;
	root["AttrHP"]=data.wAttrHP;
	root["AttrAttack"]=data.wAttrAttack;

	return root;
}
BOOL FromJsonDT_HERO_UPDATE_ATTR_DATA(Json::Value &root, DT_HERO_UPDATE_ATTR_DATA &data)
{
	data.wAttrHP=root["AttrHP"].asUInt();
	data.wAttrAttack=root["AttrAttack"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_HERO_UPDATE_ATTR_DATA(const string &strData, DT_HERO_UPDATE_ATTR_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_HERO_UPDATE_ATTR_DATA(root, data);
}
INT32 EncodeDT_HERO_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_BASE_DATA *pstIn = (DT_HERO_BASE_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byCoach))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPos))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wTalent))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwExperience))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byRecuitedFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byVisitNum))
		return -1;

	if(-1 == EncodeDT_BATTLE_ATTRIBUTE(&pstIn->stBattleAttribute, poNetData))
		return -1;

	if(-1 == EncodeDT_HIDE_BATTLE_ATTRIBUTE(&pstIn->stHideBattleAttr, poNetData))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLastUseDrugLevel))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastUseDrugTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUseExperienceDrugPerDay))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastUseExperienceDrugTime))
		return -1;

	if(-1 == EncodeDT_HERO_UPDATE_ATTR_DATA(&pstIn->stUpdateAttrInfo, poNetData))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wSelectSkillID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wSelectSkillLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HERO_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_BASE_DATA *pstOut = (DT_HERO_BASE_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byCoach))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPos))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wTalent))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwExperience))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byRecuitedFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byVisitNum))
		return -1;

	if(-1 == DecodeDT_BATTLE_ATTRIBUTE(&pstOut->stBattleAttribute, poNetData))
		return -1;

	if(-1 == DecodeDT_HIDE_BATTLE_ATTRIBUTE(&pstOut->stHideBattleAttr, poNetData))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLastUseDrugLevel))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastUseDrugTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUseExperienceDrugPerDay))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastUseExperienceDrugTime))
		return -1;

	if(-1 == DecodeDT_HERO_UPDATE_ATTR_DATA(&pstOut->stUpdateAttrInfo, poNetData))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wSelectSkillID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wSelectSkillLevel))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_HERO_BASE_DATA(DT_HERO_BASE_DATA &data)
{
	Json::Value root;
	root["KindID"]=data.wKindID;
	root["Coach"]=data.byCoach;
	root["Pos"]=data.byPos;
	root["FormationIdx"]=data.byFormationIdx;
	root["Level"]=data.wLevel;
	root["Talent"]=data.wTalent;
	root["Experience"]=data.dwExperience;
	root["RecuitedFlag"]=data.byRecuitedFlag;
	root["VisitNum"]=data.byVisitNum;
	root["BattleAttribute"]=GetJsonDT_BATTLE_ATTRIBUTE(data.stBattleAttribute);
	root["HideBattleAttr"]=GetJsonDT_HIDE_BATTLE_ATTRIBUTE(data.stHideBattleAttr);
	root["LastUseDrugLevel"]=data.byLastUseDrugLevel;
	ostringstream ssqwLastUseDrugTime;
	ssqwLastUseDrugTime<<data.qwLastUseDrugTime;
	root["LastUseDrugTime"]=ssqwLastUseDrugTime.str();
	root["UseExperienceDrugPerDay"]=data.byUseExperienceDrugPerDay;
	ostringstream ssqwLastUseExperienceDrugTime;
	ssqwLastUseExperienceDrugTime<<data.qwLastUseExperienceDrugTime;
	root["LastUseExperienceDrugTime"]=ssqwLastUseExperienceDrugTime.str();
	root["UpdateAttrInfo"]=GetJsonDT_HERO_UPDATE_ATTR_DATA(data.stUpdateAttrInfo);
	root["SelectSkillID"]=data.wSelectSkillID;
	root["SelectSkillLevel"]=data.wSelectSkillLevel;

	return root;
}
BOOL FromJsonDT_HERO_BASE_DATA(Json::Value &root, DT_HERO_BASE_DATA &data)
{
	data.wKindID=root["KindID"].asUInt();
	data.byCoach=root["Coach"].asUInt();
	data.byPos=root["Pos"].asUInt();
	data.byFormationIdx=root["FormationIdx"].asUInt();
	data.wLevel=root["Level"].asUInt();
	data.wTalent=root["Talent"].asUInt();
	data.dwExperience=root["Experience"].asUInt();
	data.byRecuitedFlag=root["RecuitedFlag"].asUInt();
	data.byVisitNum=root["VisitNum"].asUInt();
	FromJsonDT_BATTLE_ATTRIBUTE(root["BattleAttribute"] , data.stBattleAttribute);
	FromJsonDT_HIDE_BATTLE_ATTRIBUTE(root["HideBattleAttr"] , data.stHideBattleAttr);
	data.byLastUseDrugLevel=root["LastUseDrugLevel"].asUInt();
	data.qwLastUseDrugTime=SDAtou64( root["LastUseDrugTime"].asCString() );
	data.byUseExperienceDrugPerDay=root["UseExperienceDrugPerDay"].asUInt();
	data.qwLastUseExperienceDrugTime=SDAtou64( root["LastUseExperienceDrugTime"].asCString() );
	FromJsonDT_HERO_UPDATE_ATTR_DATA(root["UpdateAttrInfo"] , data.stUpdateAttrInfo);
	data.wSelectSkillID=root["SelectSkillID"].asUInt();
	data.wSelectSkillLevel=root["SelectSkillLevel"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_HERO_BASE_DATA(const string &strData, DT_HERO_BASE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_HERO_BASE_DATA(root, data);
}
INT32 EncodeDT_EQUIP_DATA(void *pHost, CNetData* poNetData)
{
	DT_EQUIP_DATA *pstIn = (DT_EQUIP_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHeroKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wBagPos))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwItemNewTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_EQUIP_DATA(void *pHost, CNetData* poNetData)
{
	DT_EQUIP_DATA *pstOut = (DT_EQUIP_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHeroKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wBagPos))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwItemNewTime))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_EQUIP_DATA(DT_EQUIP_DATA &data)
{
	Json::Value root;
	root["KindID"]=data.wKindID;
	root["Idx"]=data.wIdx;
	root["HeroKindID"]=data.wHeroKindID;
	root["Level"]=data.wLevel;
	root["BagPos"]=data.wBagPos;
	ostringstream ssqwItemNewTime;
	ssqwItemNewTime<<data.qwItemNewTime;
	root["ItemNewTime"]=ssqwItemNewTime.str();

	return root;
}
BOOL FromJsonDT_EQUIP_DATA(Json::Value &root, DT_EQUIP_DATA &data)
{
	data.wKindID=root["KindID"].asUInt();
	data.wIdx=root["Idx"].asUInt();
	data.wHeroKindID=root["HeroKindID"].asUInt();
	data.wLevel=root["Level"].asUInt();
	data.wBagPos=root["BagPos"].asUInt();
	data.qwItemNewTime=SDAtou64( root["ItemNewTime"].asCString() );

	return TRUE;
}
BOOL FromJsonDT_EQUIP_DATA(const string &strData, DT_EQUIP_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_EQUIP_DATA(root, data);
}
INT32 EncodeDT_HERO_EQUIP_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_HERO_EQUIP_DATA_LST *pstIn = (DT_HERO_EQUIP_DATA_LST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEquipNum))
		return -1;

	INT32 i;
	for(i = 0; i < EQUIP_POS_NUM; i++)
	{
		if(i >= (INT32)pstIn->wEquipNum)
			break;
		if(-1 == EncodeDT_EQUIP_DATA(&pstIn->astEquipList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HERO_EQUIP_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_HERO_EQUIP_DATA_LST *pstOut = (DT_HERO_EQUIP_DATA_LST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEquipNum))
		return -1;

	INT32 i;
	for(i = 0; i < EQUIP_POS_NUM; i++)
	{
		if(i >= (INT32)pstOut->wEquipNum)
			break;
		if(-1 == DecodeDT_EQUIP_DATA(&pstOut->astEquipList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_HERO_EQUIP_DATA_LST(DT_HERO_EQUIP_DATA_LST &data)
{
	Json::Value root;
	root["EquipNum"]=data.wEquipNum;
	for( UINT32 dwIndex=0; dwIndex<data.wEquipNum; ++dwIndex)
	{
	root["EquipList"].append( GetJsonDT_EQUIP_DATA(data.astEquipList[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_HERO_EQUIP_DATA_LST(Json::Value &root, DT_HERO_EQUIP_DATA_LST &data)
{
	data.wEquipNum=root["EquipNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.wEquipNum; ++dwIndex)
	{
		FromJsonDT_EQUIP_DATA(root["EquipList"][dwIndex] , data.astEquipList[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_HERO_EQUIP_DATA_LST(const string &strData, DT_HERO_EQUIP_DATA_LST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_HERO_EQUIP_DATA_LST(root, data);
}
INT32 EncodeDT_BAG_EQUIP_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_BAG_EQUIP_DATA_LST *pstIn = (DT_BAG_EQUIP_DATA_LST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEquipNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BAG_OPEN_NUM; i++)
	{
		if(i >= (INT32)pstIn->wEquipNum)
			break;
		if(-1 == EncodeDT_EQUIP_DATA(&pstIn->astEquipList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BAG_EQUIP_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_BAG_EQUIP_DATA_LST *pstOut = (DT_BAG_EQUIP_DATA_LST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEquipNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BAG_OPEN_NUM; i++)
	{
		if(i >= (INT32)pstOut->wEquipNum)
			break;
		if(-1 == DecodeDT_EQUIP_DATA(&pstOut->astEquipList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_BAG_EQUIP_DATA_LST(DT_BAG_EQUIP_DATA_LST &data)
{
	Json::Value root;
	root["EquipNum"]=data.wEquipNum;
	for( UINT32 dwIndex=0; dwIndex<data.wEquipNum; ++dwIndex)
	{
	root["EquipList"].append( GetJsonDT_EQUIP_DATA(data.astEquipList[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_BAG_EQUIP_DATA_LST(Json::Value &root, DT_BAG_EQUIP_DATA_LST &data)
{
	data.wEquipNum=root["EquipNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.wEquipNum; ++dwIndex)
	{
		FromJsonDT_EQUIP_DATA(root["EquipList"][dwIndex] , data.astEquipList[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_BAG_EQUIP_DATA_LST(const string &strData, DT_BAG_EQUIP_DATA_LST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_BAG_EQUIP_DATA_LST(root, data);
}
INT32 EncodeDT_GOODS_DATA(void *pHost, CNetData* poNetData)
{
	DT_GOODS_DATA *pstIn = (DT_GOODS_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wPileCount))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastUseTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastUseNum))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wBagPos))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwItemNewTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_GOODS_DATA(void *pHost, CNetData* poNetData)
{
	DT_GOODS_DATA *pstOut = (DT_GOODS_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wPileCount))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastUseTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastUseNum))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wBagPos))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwItemNewTime))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_GOODS_DATA(DT_GOODS_DATA &data)
{
	Json::Value root;
	root["KindID"]=data.wKindID;
	root["PileCount"]=data.wPileCount;
	ostringstream ssqwLastUseTime;
	ssqwLastUseTime<<data.qwLastUseTime;
	root["LastUseTime"]=ssqwLastUseTime.str();
	root["LastUseNum"]=data.wLastUseNum;
	root["BagPos"]=data.wBagPos;
	ostringstream ssqwItemNewTime;
	ssqwItemNewTime<<data.qwItemNewTime;
	root["ItemNewTime"]=ssqwItemNewTime.str();

	return root;
}
BOOL FromJsonDT_GOODS_DATA(Json::Value &root, DT_GOODS_DATA &data)
{
	data.wKindID=root["KindID"].asUInt();
	data.wPileCount=root["PileCount"].asUInt();
	data.qwLastUseTime=SDAtou64( root["LastUseTime"].asCString() );
	data.wLastUseNum=root["LastUseNum"].asUInt();
	data.wBagPos=root["BagPos"].asUInt();
	data.qwItemNewTime=SDAtou64( root["ItemNewTime"].asCString() );

	return TRUE;
}
BOOL FromJsonDT_GOODS_DATA(const string &strData, DT_GOODS_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_GOODS_DATA(root, data);
}
INT32 EncodeDT_BAG_GOODS_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_BAG_GOODS_DATA_LST *pstIn = (DT_BAG_GOODS_DATA_LST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wGoodsNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BAG_OPEN_NUM; i++)
	{
		if(i >= (INT32)pstIn->wGoodsNum)
			break;
		if(-1 == EncodeDT_GOODS_DATA(&pstIn->astGoodsList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BAG_GOODS_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_BAG_GOODS_DATA_LST *pstOut = (DT_BAG_GOODS_DATA_LST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wGoodsNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BAG_OPEN_NUM; i++)
	{
		if(i >= (INT32)pstOut->wGoodsNum)
			break;
		if(-1 == DecodeDT_GOODS_DATA(&pstOut->astGoodsList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_BAG_GOODS_DATA_LST(DT_BAG_GOODS_DATA_LST &data)
{
	Json::Value root;
	root["GoodsNum"]=data.wGoodsNum;
	for( UINT32 dwIndex=0; dwIndex<data.wGoodsNum; ++dwIndex)
	{
	root["GoodsList"].append( GetJsonDT_GOODS_DATA(data.astGoodsList[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_BAG_GOODS_DATA_LST(Json::Value &root, DT_BAG_GOODS_DATA_LST &data)
{
	data.wGoodsNum=root["GoodsNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.wGoodsNum; ++dwIndex)
	{
		FromJsonDT_GOODS_DATA(root["GoodsList"][dwIndex] , data.astGoodsList[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_BAG_GOODS_DATA_LST(const string &strData, DT_BAG_GOODS_DATA_LST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_BAG_GOODS_DATA_LST(root, data);
}
INT32 EncodeDT_DRUG_DATA(void *pHost, CNetData* poNetData)
{
	DT_DRUG_DATA *pstIn = (DT_DRUG_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNum))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHeroKindID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_DRUG_DATA(void *pHost, CNetData* poNetData)
{
	DT_DRUG_DATA *pstOut = (DT_DRUG_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNum))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHeroKindID))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_DRUG_DATA(DT_DRUG_DATA &data)
{
	Json::Value root;
	root["Level"]=data.byLevel;
	root["Num"]=data.byNum;
	root["HeroKindID"]=data.wHeroKindID;

	return root;
}
BOOL FromJsonDT_DRUG_DATA(Json::Value &root, DT_DRUG_DATA &data)
{
	data.byLevel=root["Level"].asUInt();
	data.byNum=root["Num"].asUInt();
	data.wHeroKindID=root["HeroKindID"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_DRUG_DATA(const string &strData, DT_DRUG_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_DRUG_DATA(root, data);
}
INT32 EncodeDT_DRUG_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_DRUG_DATA_LST *pstIn = (DT_DRUG_DATA_LST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byDrugLevelNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_DRUG_LEVEL; i++)
	{
		if(i >= (INT32)pstIn->byDrugLevelNum)
			break;
		if(-1 == EncodeDT_DRUG_DATA(&pstIn->astDrugList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_DRUG_DATA_LST(void *pHost, CNetData* poNetData)
{
	DT_DRUG_DATA_LST *pstOut = (DT_DRUG_DATA_LST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byDrugLevelNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_DRUG_LEVEL; i++)
	{
		if(i >= (INT32)pstOut->byDrugLevelNum)
			break;
		if(-1 == DecodeDT_DRUG_DATA(&pstOut->astDrugList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_DRUG_DATA_LST(DT_DRUG_DATA_LST &data)
{
	Json::Value root;
	root["DrugLevelNum"]=data.byDrugLevelNum;
	for( UINT32 dwIndex=0; dwIndex<data.byDrugLevelNum; ++dwIndex)
	{
	root["DrugList"].append( GetJsonDT_DRUG_DATA(data.astDrugList[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_DRUG_DATA_LST(Json::Value &root, DT_DRUG_DATA_LST &data)
{
	data.byDrugLevelNum=root["DrugLevelNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byDrugLevelNum; ++dwIndex)
	{
		FromJsonDT_DRUG_DATA(root["DrugList"][dwIndex] , data.astDrugList[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_DRUG_DATA_LST(const string &strData, DT_DRUG_DATA_LST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_DRUG_DATA_LST(root, data);
}
INT32 EncodeDT_GODWEAPON_DATA(void *pHost, CNetData* poNetData)
{
	DT_GODWEAPON_DATA *pstIn = (DT_GODWEAPON_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wQuality))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHeroKindID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_GODWEAPON_DATA(void *pHost, CNetData* poNetData)
{
	DT_GODWEAPON_DATA *pstOut = (DT_GODWEAPON_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wQuality))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHeroKindID))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_GODWEAPON_DATA(DT_GODWEAPON_DATA &data)
{
	Json::Value root;
	root["HaveDataFlag"]=data.byHaveDataFlag;
	root["Quality"]=data.wQuality;
	root["Level"]=data.wLevel;
	root["HeroKindID"]=data.wHeroKindID;

	return root;
}
BOOL FromJsonDT_GODWEAPON_DATA(Json::Value &root, DT_GODWEAPON_DATA &data)
{
	data.byHaveDataFlag=root["HaveDataFlag"].asUInt();
	data.wQuality=root["Quality"].asUInt();
	data.wLevel=root["Level"].asUInt();
	data.wHeroKindID=root["HeroKindID"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_GODWEAPON_DATA(const string &strData, DT_GODWEAPON_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_GODWEAPON_DATA(root, data);
}
INT32 EncodeDT_SKILL_DATA(void *pHost, CNetData* poNetData)
{
	DT_SKILL_DATA *pstIn = (DT_SKILL_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTrainNum))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->bySlotIdx))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SKILL_DATA(void *pHost, CNetData* poNetData)
{
	DT_SKILL_DATA *pstOut = (DT_SKILL_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTrainNum))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->bySlotIdx))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_SKILL_DATA(DT_SKILL_DATA &data)
{
	Json::Value root;
	root["ID"]=data.wID;
	root["Level"]=data.wLevel;
	root["TrainNum"]=data.byTrainNum;
	root["SlotIdx"]=data.bySlotIdx;

	return root;
}
BOOL FromJsonDT_SKILL_DATA(Json::Value &root, DT_SKILL_DATA &data)
{
	data.wID=root["ID"].asUInt();
	data.wLevel=root["Level"].asUInt();
	data.byTrainNum=root["TrainNum"].asUInt();
	data.bySlotIdx=root["SlotIdx"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_SKILL_DATA(const string &strData, DT_SKILL_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_SKILL_DATA(root, data);
}
INT32 EncodeDT_SKILL_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_SKILL_DATA_LIST *pstIn = (DT_SKILL_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->bySkillNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SKILL_NUM; i++)
	{
		if(i >= (INT32)pstIn->bySkillNum)
			break;
		if(-1 == EncodeDT_SKILL_DATA(&pstIn->astSkillInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SKILL_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_SKILL_DATA_LIST *pstOut = (DT_SKILL_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->bySkillNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SKILL_NUM; i++)
	{
		if(i >= (INT32)pstOut->bySkillNum)
			break;
		if(-1 == DecodeDT_SKILL_DATA(&pstOut->astSkillInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_SKILL_DATA_LIST(DT_SKILL_DATA_LIST &data)
{
	Json::Value root;
	root["SkillNum"]=data.bySkillNum;
	for( UINT32 dwIndex=0; dwIndex<data.bySkillNum; ++dwIndex)
	{
	root["SkillInfo"].append( GetJsonDT_SKILL_DATA(data.astSkillInfo[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_SKILL_DATA_LIST(Json::Value &root, DT_SKILL_DATA_LIST &data)
{
	data.bySkillNum=root["SkillNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.bySkillNum; ++dwIndex)
	{
		FromJsonDT_SKILL_DATA(root["SkillInfo"][dwIndex] , data.astSkillInfo[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_SKILL_DATA_LIST(const string &strData, DT_SKILL_DATA_LIST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_SKILL_DATA_LIST(root, data);
}
INT32 EncodeDT_HERO_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_DATA *pstIn = (DT_HERO_DATA*)pHost;

	if(-1 == EncodeDT_HERO_BASE_DATA(&pstIn->stHeroBaseData, poNetData))
		return -1;

	if(-1 == EncodeDT_HERO_EQUIP_DATA_LST(&pstIn->stEquipDataInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_DRUG_DATA_LST(&pstIn->stDrugDataInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_GODWEAPON_DATA(&pstIn->stGodweaponInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_SKILL_DATA_LIST(&pstIn->stSkillData, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HERO_DATA(void *pHost, CNetData* poNetData)
{
	DT_HERO_DATA *pstOut = (DT_HERO_DATA*)pHost;

	if(-1 == DecodeDT_HERO_BASE_DATA(&pstOut->stHeroBaseData, poNetData))
		return -1;

	if(-1 == DecodeDT_HERO_EQUIP_DATA_LST(&pstOut->stEquipDataInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_DRUG_DATA_LST(&pstOut->stDrugDataInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_GODWEAPON_DATA(&pstOut->stGodweaponInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_SKILL_DATA_LIST(&pstOut->stSkillData, poNetData))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_HERO_DATA(DT_HERO_DATA &data)
{
	Json::Value root;
	root["HeroBaseData"]=GetJsonDT_HERO_BASE_DATA(data.stHeroBaseData);
	root["EquipDataInfo"]=GetJsonDT_HERO_EQUIP_DATA_LST(data.stEquipDataInfo);
	root["DrugDataInfo"]=GetJsonDT_DRUG_DATA_LST(data.stDrugDataInfo);
	root["GodweaponInfo"]=GetJsonDT_GODWEAPON_DATA(data.stGodweaponInfo);
	root["SkillData"]=GetJsonDT_SKILL_DATA_LIST(data.stSkillData);

	return root;
}
BOOL FromJsonDT_HERO_DATA(Json::Value &root, DT_HERO_DATA &data)
{
	FromJsonDT_HERO_BASE_DATA(root["HeroBaseData"] , data.stHeroBaseData);
	FromJsonDT_HERO_EQUIP_DATA_LST(root["EquipDataInfo"] , data.stEquipDataInfo);
	FromJsonDT_DRUG_DATA_LST(root["DrugDataInfo"] , data.stDrugDataInfo);
	FromJsonDT_GODWEAPON_DATA(root["GodweaponInfo"] , data.stGodweaponInfo);
	FromJsonDT_SKILL_DATA_LIST(root["SkillData"] , data.stSkillData);

	return TRUE;
}
BOOL FromJsonDT_HERO_DATA(const string &strData, DT_HERO_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_HERO_DATA(root, data);
}
INT32 EncodeDT_HERO_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_HERO_DATA_LIST *pstIn = (DT_HERO_DATA_LIST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wHeroNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_HERO_NUM; i++)
	{
		if(i >= (INT32)pstIn->wHeroNum)
			break;
		if(-1 == EncodeDT_HERO_DATA(&pstIn->astHeroInfoLst[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HERO_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_HERO_DATA_LIST *pstOut = (DT_HERO_DATA_LIST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wHeroNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_HERO_NUM; i++)
	{
		if(i >= (INT32)pstOut->wHeroNum)
			break;
		if(-1 == DecodeDT_HERO_DATA(&pstOut->astHeroInfoLst[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_HERO_DATA_LIST(DT_HERO_DATA_LIST &data)
{
	Json::Value root;
	root["HeroNum"]=data.wHeroNum;
	for( UINT32 dwIndex=0; dwIndex<data.wHeroNum; ++dwIndex)
	{
	root["HeroInfoLst"].append( GetJsonDT_HERO_DATA(data.astHeroInfoLst[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_HERO_DATA_LIST(Json::Value &root, DT_HERO_DATA_LIST &data)
{
	data.wHeroNum=root["HeroNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.wHeroNum; ++dwIndex)
	{
		FromJsonDT_HERO_DATA(root["HeroInfoLst"][dwIndex] , data.astHeroInfoLst[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_HERO_DATA_LIST(const string &strData, DT_HERO_DATA_LIST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_HERO_DATA_LIST(root, data);
}
INT32 EncodeDT_BAG_DATA(void *pHost, CNetData* poNetData)
{
	DT_BAG_DATA *pstIn = (DT_BAG_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byOpenNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BAG_DATA(void *pHost, CNetData* poNetData)
{
	DT_BAG_DATA *pstOut = (DT_BAG_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byOpenNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_BAG_DATA(DT_BAG_DATA &data)
{
	Json::Value root;
	root["OpenNum"]=data.byOpenNum;

	return root;
}
BOOL FromJsonDT_BAG_DATA(Json::Value &root, DT_BAG_DATA &data)
{
	data.byOpenNum=root["OpenNum"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_BAG_DATA(const string &strData, DT_BAG_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_BAG_DATA(root, data);
}
INT32 EncodeDT_BUILD_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BUILD_BASE_DATA *pstIn = (DT_BUILD_BASE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwValue1))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwValue2))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byValue3))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->bySort))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byHaveOpenFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BUILD_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BUILD_BASE_DATA *pstOut = (DT_BUILD_BASE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwValue1))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwValue2))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byValue3))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->bySort))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byHaveOpenFlag))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_BUILD_BASE_DATA(DT_BUILD_BASE_DATA &data)
{
	Json::Value root;
	root["KindID"]=data.byKindID;
	root["Level"]=data.wLevel;
	ostringstream ssqwValue1;
	ssqwValue1<<data.qwValue1;
	root["Value1"]=ssqwValue1.str();
	ostringstream ssqwValue2;
	ssqwValue2<<data.qwValue2;
	root["Value2"]=ssqwValue2.str();
	root["Value3"]=data.byValue3;
	root["Sort"]=data.bySort;
	root["HaveOpenFlag"]=data.byHaveOpenFlag;

	return root;
}
BOOL FromJsonDT_BUILD_BASE_DATA(Json::Value &root, DT_BUILD_BASE_DATA &data)
{
	data.byKindID=root["KindID"].asUInt();
	data.wLevel=root["Level"].asUInt();
	data.qwValue1=SDAtou64( root["Value1"].asCString() );
	data.qwValue2=SDAtou64( root["Value2"].asCString() );
	data.byValue3=root["Value3"].asUInt();
	data.bySort=root["Sort"].asUInt();
	data.byHaveOpenFlag=root["HaveOpenFlag"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_BUILD_BASE_DATA(const string &strData, DT_BUILD_BASE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_BUILD_BASE_DATA(root, data);
}
INT32 EncodeDT_BUILD_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_BUILD_DATA_LIST *pstIn = (DT_BUILD_DATA_LIST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wBuildNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BUILD_NUM; i++)
	{
		if(i >= (INT32)pstIn->wBuildNum)
			break;
		if(-1 == EncodeDT_BUILD_BASE_DATA(&pstIn->astBuildInfoLst[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BUILD_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_BUILD_DATA_LIST *pstOut = (DT_BUILD_DATA_LIST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wBuildNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BUILD_NUM; i++)
	{
		if(i >= (INT32)pstOut->wBuildNum)
			break;
		if(-1 == DecodeDT_BUILD_BASE_DATA(&pstOut->astBuildInfoLst[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_BUILD_DATA_LIST(DT_BUILD_DATA_LIST &data)
{
	Json::Value root;
	root["BuildNum"]=data.wBuildNum;
	for( UINT32 dwIndex=0; dwIndex<data.wBuildNum; ++dwIndex)
	{
	root["BuildInfoLst"].append( GetJsonDT_BUILD_BASE_DATA(data.astBuildInfoLst[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_BUILD_DATA_LIST(Json::Value &root, DT_BUILD_DATA_LIST &data)
{
	data.wBuildNum=root["BuildNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.wBuildNum; ++dwIndex)
	{
		FromJsonDT_BUILD_BASE_DATA(root["BuildInfoLst"][dwIndex] , data.astBuildInfoLst[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_BUILD_DATA_LIST(const string &strData, DT_BUILD_DATA_LIST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_BUILD_DATA_LIST(root, data);
}
INT32 EncodeDT_INSTANCE_ID(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_ID *pstIn = (DT_INSTANCE_ID*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_INSTANCE_ID(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_ID *pstOut = (DT_INSTANCE_ID*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_INSTANCE_ID(DT_INSTANCE_ID &data)
{
	Json::Value root;
	root["SceneIdx"]=data.wSceneIdx;
	root["TownIdx"]=data.byTownIdx;
	root["InstanceIdx"]=data.byInstanceIdx;

	return root;
}
BOOL FromJsonDT_INSTANCE_ID(Json::Value &root, DT_INSTANCE_ID &data)
{
	data.wSceneIdx=root["SceneIdx"].asUInt();
	data.byTownIdx=root["TownIdx"].asUInt();
	data.byInstanceIdx=root["InstanceIdx"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_INSTANCE_ID(const string &strData, DT_INSTANCE_ID &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_INSTANCE_ID(root, data);
}
INT32 EncodeDT_INSTANCE_DATA(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_DATA *pstIn = (DT_INSTANCE_DATA*)pHost;

	if(-1 == EncodeDT_INSTANCE_ID(&pstIn->stInstanceID, poNetData))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byScore))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wPassTimes))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastPassTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_INSTANCE_DATA(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_DATA *pstOut = (DT_INSTANCE_DATA*)pHost;

	if(-1 == DecodeDT_INSTANCE_ID(&pstOut->stInstanceID, poNetData))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byScore))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wPassTimes))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastPassTime))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_INSTANCE_DATA(DT_INSTANCE_DATA &data)
{
	Json::Value root;
	root["InstanceID"]=GetJsonDT_INSTANCE_ID(data.stInstanceID);
	root["Score"]=data.byScore;
	root["PassTimes"]=data.wPassTimes;
	ostringstream ssqwLastPassTime;
	ssqwLastPassTime<<data.qwLastPassTime;
	root["LastPassTime"]=ssqwLastPassTime.str();

	return root;
}
BOOL FromJsonDT_INSTANCE_DATA(Json::Value &root, DT_INSTANCE_DATA &data)
{
	FromJsonDT_INSTANCE_ID(root["InstanceID"] , data.stInstanceID);
	data.byScore=root["Score"].asUInt();
	data.wPassTimes=root["PassTimes"].asUInt();
	data.qwLastPassTime=SDAtou64( root["LastPassTime"].asCString() );

	return TRUE;
}
BOOL FromJsonDT_INSTANCE_DATA(const string &strData, DT_INSTANCE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_INSTANCE_DATA(root, data);
}
INT32 EncodeDT_INSTANCE_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_DATA_LIST *pstIn = (DT_INSTANCE_DATA_LIST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wInstanceNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_INSTANCE_PER_SCENE_NUM; i++)
	{
		if(i >= (INT32)pstIn->wInstanceNum)
			break;
		if(-1 == EncodeDT_INSTANCE_DATA(&pstIn->astInstanceInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_INSTANCE_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_DATA_LIST *pstOut = (DT_INSTANCE_DATA_LIST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wInstanceNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_INSTANCE_PER_SCENE_NUM; i++)
	{
		if(i >= (INT32)pstOut->wInstanceNum)
			break;
		if(-1 == DecodeDT_INSTANCE_DATA(&pstOut->astInstanceInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_INSTANCE_DATA_LIST(DT_INSTANCE_DATA_LIST &data)
{
	Json::Value root;
	root["SceneIdx"]=data.wSceneIdx;
	root["InstanceNum"]=data.wInstanceNum;
	for( UINT32 dwIndex=0; dwIndex<data.wInstanceNum; ++dwIndex)
	{
	root["InstanceInfo"].append( GetJsonDT_INSTANCE_DATA(data.astInstanceInfo[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_INSTANCE_DATA_LIST(Json::Value &root, DT_INSTANCE_DATA_LIST &data)
{
	data.wSceneIdx=root["SceneIdx"].asUInt();
	data.wInstanceNum=root["InstanceNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.wInstanceNum; ++dwIndex)
	{
		FromJsonDT_INSTANCE_DATA(root["InstanceInfo"][dwIndex] , data.astInstanceInfo[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_INSTANCE_DATA_LIST(const string &strData, DT_INSTANCE_DATA_LIST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_INSTANCE_DATA_LIST(root, data);
}
INT32 EncodeDT_SCIENCE_ATTREXT(void *pHost, CNetData* poNetData)
{
	DT_SCIENCE_ATTREXT *pstIn = (DT_SCIENCE_ATTREXT*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAttrKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wAttrLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SCIENCE_ATTREXT(void *pHost, CNetData* poNetData)
{
	DT_SCIENCE_ATTREXT *pstOut = (DT_SCIENCE_ATTREXT*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAttrKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wAttrLevel))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_SCIENCE_ATTREXT(DT_SCIENCE_ATTREXT &data)
{
	Json::Value root;
	root["AttrKindID"]=data.byAttrKindID;
	root["AttrLevel"]=data.wAttrLevel;

	return root;
}
BOOL FromJsonDT_SCIENCE_ATTREXT(Json::Value &root, DT_SCIENCE_ATTREXT &data)
{
	data.byAttrKindID=root["AttrKindID"].asUInt();
	data.wAttrLevel=root["AttrLevel"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_SCIENCE_ATTREXT(const string &strData, DT_SCIENCE_ATTREXT &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_SCIENCE_ATTREXT(root, data);
}
INT32 EncodeDT_SCIENCE_ATTREXT_LIST(void *pHost, CNetData* poNetData)
{
	DT_SCIENCE_ATTREXT_LIST *pstIn = (DT_SCIENCE_ATTREXT_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAttrNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SCIENCE_ATTR_NUM; i++)
	{
		if(i >= (INT32)pstIn->byAttrNum)
			break;
		if(-1 == EncodeDT_SCIENCE_ATTREXT(&pstIn->astBuildInfoLst[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SCIENCE_ATTREXT_LIST(void *pHost, CNetData* poNetData)
{
	DT_SCIENCE_ATTREXT_LIST *pstOut = (DT_SCIENCE_ATTREXT_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAttrNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SCIENCE_ATTR_NUM; i++)
	{
		if(i >= (INT32)pstOut->byAttrNum)
			break;
		if(-1 == DecodeDT_SCIENCE_ATTREXT(&pstOut->astBuildInfoLst[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_SCIENCE_ATTREXT_LIST(DT_SCIENCE_ATTREXT_LIST &data)
{
	Json::Value root;
	root["AttrNum"]=data.byAttrNum;
	for( UINT32 dwIndex=0; dwIndex<data.byAttrNum; ++dwIndex)
	{
	root["BuildInfoLst"].append( GetJsonDT_SCIENCE_ATTREXT(data.astBuildInfoLst[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_SCIENCE_ATTREXT_LIST(Json::Value &root, DT_SCIENCE_ATTREXT_LIST &data)
{
	data.byAttrNum=root["AttrNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byAttrNum; ++dwIndex)
	{
		FromJsonDT_SCIENCE_ATTREXT(root["BuildInfoLst"][dwIndex] , data.astBuildInfoLst[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_SCIENCE_ATTREXT_LIST(const string &strData, DT_SCIENCE_ATTREXT_LIST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_SCIENCE_ATTREXT_LIST(root, data);
}
INT32 EncodeDT_CROWN_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROWN_TASK_CONDITION *pstIn = (DT_CROWN_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wCrownTimes))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CROWN_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROWN_TASK_CONDITION *pstOut = (DT_CROWN_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wCrownTimes))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_CROWN_TASK_CONDITION(DT_CROWN_TASK_CONDITION &data)
{
	Json::Value root;
	root["CrownTimes"]=data.wCrownTimes;

	return root;
}
BOOL FromJsonDT_CROWN_TASK_CONDITION(Json::Value &root, DT_CROWN_TASK_CONDITION &data)
{
	data.wCrownTimes=root["CrownTimes"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_CROWN_TASK_CONDITION(const string &strData, DT_CROWN_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_CROWN_TASK_CONDITION(root, data);
}
INT32 EncodeDT_BUILDUPGRADE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_BUILDUPGRADE_TASK_CONDITION *pstIn = (DT_BUILDUPGRADE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byBuildKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BUILDUPGRADE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_BUILDUPGRADE_TASK_CONDITION *pstOut = (DT_BUILDUPGRADE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byBuildKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_BUILDUPGRADE_TASK_CONDITION(DT_BUILDUPGRADE_TASK_CONDITION &data)
{
	Json::Value root;
	root["BuildKindID"]=data.byBuildKindID;
	root["Level"]=data.wLevel;

	return root;
}
BOOL FromJsonDT_BUILDUPGRADE_TASK_CONDITION(Json::Value &root, DT_BUILDUPGRADE_TASK_CONDITION &data)
{
	data.byBuildKindID=root["BuildKindID"].asUInt();
	data.wLevel=root["Level"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_BUILDUPGRADE_TASK_CONDITION(const string &strData, DT_BUILDUPGRADE_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_BUILDUPGRADE_TASK_CONDITION(root, data);
}
INT32 EncodeDT_RECRUITHERO_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_RECRUITHERO_TASK_CONDITION *pstIn = (DT_RECRUITHERO_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wRecruitHeroKindID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_RECRUITHERO_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_RECRUITHERO_TASK_CONDITION *pstOut = (DT_RECRUITHERO_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wRecruitHeroKindID))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_RECRUITHERO_TASK_CONDITION(DT_RECRUITHERO_TASK_CONDITION &data)
{
	Json::Value root;
	root["RecruitHeroKindID"]=data.wRecruitHeroKindID;

	return root;
}
BOOL FromJsonDT_RECRUITHERO_TASK_CONDITION(Json::Value &root, DT_RECRUITHERO_TASK_CONDITION &data)
{
	data.wRecruitHeroKindID=root["RecruitHeroKindID"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_RECRUITHERO_TASK_CONDITION(const string &strData, DT_RECRUITHERO_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_RECRUITHERO_TASK_CONDITION(root, data);
}
INT32 EncodeDT_CROWN_HEROCOLOR_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROWN_HEROCOLOR_CONDITION *pstIn = (DT_CROWN_HEROCOLOR_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wHeroColor))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CROWN_HEROCOLOR_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROWN_HEROCOLOR_CONDITION *pstOut = (DT_CROWN_HEROCOLOR_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wHeroColor))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_CROWN_HEROCOLOR_CONDITION(DT_CROWN_HEROCOLOR_CONDITION &data)
{
	Json::Value root;
	root["HeroColor"]=data.wHeroColor;
	root["Num"]=data.byNum;

	return root;
}
BOOL FromJsonDT_CROWN_HEROCOLOR_CONDITION(Json::Value &root, DT_CROWN_HEROCOLOR_CONDITION &data)
{
	data.wHeroColor=root["HeroColor"].asUInt();
	data.byNum=root["Num"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_CROWN_HEROCOLOR_CONDITION(const string &strData, DT_CROWN_HEROCOLOR_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_CROWN_HEROCOLOR_CONDITION(root, data);
}
INT32 EncodeDT_CROWN_HEROUPGRADE_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROWN_HEROUPGRADE_CONDITION *pstIn = (DT_CROWN_HEROUPGRADE_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wHeroLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CROWN_HEROUPGRADE_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROWN_HEROUPGRADE_CONDITION *pstOut = (DT_CROWN_HEROUPGRADE_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wHeroLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_CROWN_HEROUPGRADE_CONDITION(DT_CROWN_HEROUPGRADE_CONDITION &data)
{
	Json::Value root;
	root["HeroLevel"]=data.wHeroLevel;
	root["Num"]=data.byNum;

	return root;
}
BOOL FromJsonDT_CROWN_HEROUPGRADE_CONDITION(Json::Value &root, DT_CROWN_HEROUPGRADE_CONDITION &data)
{
	data.wHeroLevel=root["HeroLevel"].asUInt();
	data.byNum=root["Num"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_CROWN_HEROUPGRADE_CONDITION(const string &strData, DT_CROWN_HEROUPGRADE_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_CROWN_HEROUPGRADE_CONDITION(root, data);
}
INT32 EncodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_FIRSTPASSINSTANCE_TASK_CONDITION *pstIn = (DT_FIRSTPASSINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_FIRSTPASSINSTANCE_TASK_CONDITION *pstOut = (DT_FIRSTPASSINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_FIRSTPASSINSTANCE_TASK_CONDITION(DT_FIRSTPASSINSTANCE_TASK_CONDITION &data)
{
	Json::Value root;
	root["SceneIdx"]=data.wSceneIdx;
	root["TownIdx"]=data.byTownIdx;
	root["InstanceIdx"]=data.byInstanceIdx;

	return root;
}
BOOL FromJsonDT_FIRSTPASSINSTANCE_TASK_CONDITION(Json::Value &root, DT_FIRSTPASSINSTANCE_TASK_CONDITION &data)
{
	data.wSceneIdx=root["SceneIdx"].asUInt();
	data.byTownIdx=root["TownIdx"].asUInt();
	data.byInstanceIdx=root["InstanceIdx"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_FIRSTPASSINSTANCE_TASK_CONDITION(const string &strData, DT_FIRSTPASSINSTANCE_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_FIRSTPASSINSTANCE_TASK_CONDITION(root, data);
}
INT32 EncodeDT_SCOREPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_SCOREPASSINSTANCE_TASK_CONDITION *pstIn = (DT_SCOREPASSINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byScore))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SCOREPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_SCOREPASSINSTANCE_TASK_CONDITION *pstOut = (DT_SCOREPASSINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byScore))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_SCOREPASSINSTANCE_TASK_CONDITION(DT_SCOREPASSINSTANCE_TASK_CONDITION &data)
{
	Json::Value root;
	root["SceneIdx"]=data.wSceneIdx;
	root["TownIdx"]=data.byTownIdx;
	root["InstanceIdx"]=data.byInstanceIdx;
	root["Score"]=data.byScore;

	return root;
}
BOOL FromJsonDT_SCOREPASSINSTANCE_TASK_CONDITION(Json::Value &root, DT_SCOREPASSINSTANCE_TASK_CONDITION &data)
{
	data.wSceneIdx=root["SceneIdx"].asUInt();
	data.byTownIdx=root["TownIdx"].asUInt();
	data.byInstanceIdx=root["InstanceIdx"].asUInt();
	data.byScore=root["Score"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_SCOREPASSINSTANCE_TASK_CONDITION(const string &strData, DT_SCOREPASSINSTANCE_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_SCOREPASSINSTANCE_TASK_CONDITION(root, data);
}
INT32 EncodeDT_TIMESPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_TIMESPASSINSTANCE_TASK_CONDITION *pstIn = (DT_TIMESPASSINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTimes))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_TIMESPASSINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_TIMESPASSINSTANCE_TASK_CONDITION *pstOut = (DT_TIMESPASSINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTimes))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_TIMESPASSINSTANCE_TASK_CONDITION(DT_TIMESPASSINSTANCE_TASK_CONDITION &data)
{
	Json::Value root;
	root["SceneIdx"]=data.wSceneIdx;
	root["TownIdx"]=data.byTownIdx;
	root["InstanceIdx"]=data.byInstanceIdx;
	root["Times"]=data.byTimes;

	return root;
}
BOOL FromJsonDT_TIMESPASSINSTANCE_TASK_CONDITION(Json::Value &root, DT_TIMESPASSINSTANCE_TASK_CONDITION &data)
{
	data.wSceneIdx=root["SceneIdx"].asUInt();
	data.byTownIdx=root["TownIdx"].asUInt();
	data.byInstanceIdx=root["InstanceIdx"].asUInt();
	data.byTimes=root["Times"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_TIMESPASSINSTANCE_TASK_CONDITION(const string &strData, DT_TIMESPASSINSTANCE_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_TIMESPASSINSTANCE_TASK_CONDITION(root, data);
}
INT32 EncodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_KILLMONSTERINSTANCE_TASK_CONDITION *pstIn = (DT_KILLMONSTERINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wMonsterKindID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byKillNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_KILLMONSTERINSTANCE_TASK_CONDITION *pstOut = (DT_KILLMONSTERINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wMonsterKindID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byKillNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_KILLMONSTERINSTANCE_TASK_CONDITION(DT_KILLMONSTERINSTANCE_TASK_CONDITION &data)
{
	Json::Value root;
	root["SceneIdx"]=data.wSceneIdx;
	root["TownIdx"]=data.byTownIdx;
	root["InstanceIdx"]=data.byInstanceIdx;
	root["MonsterKindID"]=data.wMonsterKindID;
	root["KillNum"]=data.byKillNum;

	return root;
}
BOOL FromJsonDT_KILLMONSTERINSTANCE_TASK_CONDITION(Json::Value &root, DT_KILLMONSTERINSTANCE_TASK_CONDITION &data)
{
	data.wSceneIdx=root["SceneIdx"].asUInt();
	data.byTownIdx=root["TownIdx"].asUInt();
	data.byInstanceIdx=root["InstanceIdx"].asUInt();
	data.wMonsterKindID=root["MonsterKindID"].asUInt();
	data.byKillNum=root["KillNum"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_KILLMONSTERINSTANCE_TASK_CONDITION(const string &strData, DT_KILLMONSTERINSTANCE_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_KILLMONSTERINSTANCE_TASK_CONDITION(root, data);
}
INT32 EncodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION *pstIn = (DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEquipKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION *pstOut = (DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEquipKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION &data)
{
	Json::Value root;
	root["EquipKindID"]=data.wEquipKindID;
	root["Level"]=data.wLevel;

	return root;
}
BOOL FromJsonDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(Json::Value &root, DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION &data)
{
	data.wEquipKindID=root["EquipKindID"].asUInt();
	data.wLevel=root["Level"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(const string &strData, DT_STRENGTHENEQUIPLEVEL_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(root, data);
}
INT32 EncodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHENEQUIPNUM_TASK_CONDITION *pstIn = (DT_STRENGTHENEQUIPNUM_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEquipKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHENEQUIPNUM_TASK_CONDITION *pstOut = (DT_STRENGTHENEQUIPNUM_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEquipKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_STRENGTHENEQUIPNUM_TASK_CONDITION(DT_STRENGTHENEQUIPNUM_TASK_CONDITION &data)
{
	Json::Value root;
	root["EquipKindID"]=data.wEquipKindID;
	root["Num"]=data.wNum;

	return root;
}
BOOL FromJsonDT_STRENGTHENEQUIPNUM_TASK_CONDITION(Json::Value &root, DT_STRENGTHENEQUIPNUM_TASK_CONDITION &data)
{
	data.wEquipKindID=root["EquipKindID"].asUInt();
	data.wNum=root["Num"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_STRENGTHENEQUIPNUM_TASK_CONDITION(const string &strData, DT_STRENGTHENEQUIPNUM_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_STRENGTHENEQUIPNUM_TASK_CONDITION(root, data);
}
INT32 EncodeDT_ITEMCOLLECT_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_ITEMCOLLECT_TASK_CONDITION *pstIn = (DT_ITEMCOLLECT_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wItemKindNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ITEM_KIND_COLLECT_NUM; i++)
	{
		if(i >= (INT32)pstIn->wItemKindNum)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyItemCollectInfo[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ITEMCOLLECT_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_ITEMCOLLECT_TASK_CONDITION *pstOut = (DT_ITEMCOLLECT_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wItemKindNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ITEM_KIND_COLLECT_NUM; i++)
	{
		if(i >= (INT32)pstOut->wItemKindNum)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyItemCollectInfo[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_ITEMCOLLECT_TASK_CONDITION(DT_ITEMCOLLECT_TASK_CONDITION &data)
{
	Json::Value root;
	root["ItemKindNum"]=data.wItemKindNum;
	for( UINT32 dwIndex=0; dwIndex<data.wItemKindNum; ++dwIndex)
	{
	root["ItemCollectInfo"].append( data.abyItemCollectInfo[dwIndex]);
	}

	return root;
}
BOOL FromJsonDT_ITEMCOLLECT_TASK_CONDITION(Json::Value &root, DT_ITEMCOLLECT_TASK_CONDITION &data)
{
	data.wItemKindNum=root["ItemKindNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.wItemKindNum; ++dwIndex)
	{
		data.abyItemCollectInfo[dwIndex]=BYTE(root["ItemCollectInfo"][dwIndex].asUInt() );
	}

	return TRUE;
}
BOOL FromJsonDT_ITEMCOLLECT_TASK_CONDITION(const string &strData, DT_ITEMCOLLECT_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_ITEMCOLLECT_TASK_CONDITION(root, data);
}
INT32 EncodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_ACTIVATEGODWEAPON_TASK_CONDITION *pstIn = (DT_ACTIVATEGODWEAPON_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wHeroKindID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_ACTIVATEGODWEAPON_TASK_CONDITION *pstOut = (DT_ACTIVATEGODWEAPON_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wHeroKindID))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_ACTIVATEGODWEAPON_TASK_CONDITION(DT_ACTIVATEGODWEAPON_TASK_CONDITION &data)
{
	Json::Value root;
	root["HeroKindID"]=data.wHeroKindID;

	return root;
}
BOOL FromJsonDT_ACTIVATEGODWEAPON_TASK_CONDITION(Json::Value &root, DT_ACTIVATEGODWEAPON_TASK_CONDITION &data)
{
	data.wHeroKindID=root["HeroKindID"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_ACTIVATEGODWEAPON_TASK_CONDITION(const string &strData, DT_ACTIVATEGODWEAPON_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_ACTIVATEGODWEAPON_TASK_CONDITION(root, data);
}
INT32 EncodeDT_UPGRADEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEGODWEAPON_TASK_CONDITION *pstIn = (DT_UPGRADEGODWEAPON_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wHeroKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wQuality))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_UPGRADEGODWEAPON_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEGODWEAPON_TASK_CONDITION *pstOut = (DT_UPGRADEGODWEAPON_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wHeroKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wQuality))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_UPGRADEGODWEAPON_TASK_CONDITION(DT_UPGRADEGODWEAPON_TASK_CONDITION &data)
{
	Json::Value root;
	root["HeroKindID"]=data.wHeroKindID;
	root["Quality"]=data.wQuality;
	root["Level"]=data.wLevel;

	return root;
}
BOOL FromJsonDT_UPGRADEGODWEAPON_TASK_CONDITION(Json::Value &root, DT_UPGRADEGODWEAPON_TASK_CONDITION &data)
{
	data.wHeroKindID=root["HeroKindID"].asUInt();
	data.wQuality=root["Quality"].asUInt();
	data.wLevel=root["Level"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_UPGRADEGODWEAPON_TASK_CONDITION(const string &strData, DT_UPGRADEGODWEAPON_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_UPGRADEGODWEAPON_TASK_CONDITION(root, data);
}
INT32 EncodeDT_UPGRADEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEDRUG_TASK_CONDITION *pstIn = (DT_UPGRADEDRUG_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wUpgradeNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_UPGRADEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEDRUG_TASK_CONDITION *pstOut = (DT_UPGRADEDRUG_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wUpgradeNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_UPGRADEDRUG_TASK_CONDITION(DT_UPGRADEDRUG_TASK_CONDITION &data)
{
	Json::Value root;
	root["UpgradeNum"]=data.wUpgradeNum;

	return root;
}
BOOL FromJsonDT_UPGRADEDRUG_TASK_CONDITION(Json::Value &root, DT_UPGRADEDRUG_TASK_CONDITION &data)
{
	data.wUpgradeNum=root["UpgradeNum"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_UPGRADEDRUG_TASK_CONDITION(const string &strData, DT_UPGRADEDRUG_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_UPGRADEDRUG_TASK_CONDITION(root, data);
}
INT32 EncodeDT_CHALLENGE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CHALLENGE_TASK_CONDITION *pstIn = (DT_CHALLENGE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wChallengeNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CHALLENGE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CHALLENGE_TASK_CONDITION *pstOut = (DT_CHALLENGE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wChallengeNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_CHALLENGE_TASK_CONDITION(DT_CHALLENGE_TASK_CONDITION &data)
{
	Json::Value root;
	root["ChallengeNum"]=data.wChallengeNum;

	return root;
}
BOOL FromJsonDT_CHALLENGE_TASK_CONDITION(Json::Value &root, DT_CHALLENGE_TASK_CONDITION &data)
{
	data.wChallengeNum=root["ChallengeNum"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_CHALLENGE_TASK_CONDITION(const string &strData, DT_CHALLENGE_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_CHALLENGE_TASK_CONDITION(root, data);
}
INT32 EncodeDT_PLUNDER_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_TASK_CONDITION *pstIn = (DT_PLUNDER_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wPlunderNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLUNDER_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_TASK_CONDITION *pstOut = (DT_PLUNDER_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wPlunderNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_PLUNDER_TASK_CONDITION(DT_PLUNDER_TASK_CONDITION &data)
{
	Json::Value root;
	root["PlunderNum"]=data.wPlunderNum;

	return root;
}
BOOL FromJsonDT_PLUNDER_TASK_CONDITION(Json::Value &root, DT_PLUNDER_TASK_CONDITION &data)
{
	data.wPlunderNum=root["PlunderNum"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_PLUNDER_TASK_CONDITION(const string &strData, DT_PLUNDER_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_PLUNDER_TASK_CONDITION(root, data);
}
INT32 EncodeDT_DRESSSKILL_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_DRESSSKILL_TASK_CONDITION *pstIn = (DT_DRESSSKILL_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSkillID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byDressFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_DRESSSKILL_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_DRESSSKILL_TASK_CONDITION *pstOut = (DT_DRESSSKILL_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSkillID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byDressFlag))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_DRESSSKILL_TASK_CONDITION(DT_DRESSSKILL_TASK_CONDITION &data)
{
	Json::Value root;
	root["SkillID"]=data.wSkillID;
	root["DressFlag"]=data.byDressFlag;

	return root;
}
BOOL FromJsonDT_DRESSSKILL_TASK_CONDITION(Json::Value &root, DT_DRESSSKILL_TASK_CONDITION &data)
{
	data.wSkillID=root["SkillID"].asUInt();
	data.byDressFlag=root["DressFlag"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_DRESSSKILL_TASK_CONDITION(const string &strData, DT_DRESSSKILL_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_DRESSSKILL_TASK_CONDITION(root, data);
}
INT32 EncodeDT_DRESSEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_DRESSEQUIP_TASK_CONDITION *pstIn = (DT_DRESSEQUIP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEquipID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byDressFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_DRESSEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_DRESSEQUIP_TASK_CONDITION *pstOut = (DT_DRESSEQUIP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEquipID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byDressFlag))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_DRESSEQUIP_TASK_CONDITION(DT_DRESSEQUIP_TASK_CONDITION &data)
{
	Json::Value root;
	root["EquipID"]=data.wEquipID;
	root["DressFlag"]=data.byDressFlag;

	return root;
}
BOOL FromJsonDT_DRESSEQUIP_TASK_CONDITION(Json::Value &root, DT_DRESSEQUIP_TASK_CONDITION &data)
{
	data.wEquipID=root["EquipID"].asUInt();
	data.byDressFlag=root["DressFlag"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_DRESSEQUIP_TASK_CONDITION(const string &strData, DT_DRESSEQUIP_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_DRESSEQUIP_TASK_CONDITION(root, data);
}
INT32 EncodeDT_UPGRADESKILL_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADESKILL_TASK_CONDITION *pstIn = (DT_UPGRADESKILL_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSkillID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wUpgradeNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_UPGRADESKILL_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADESKILL_TASK_CONDITION *pstOut = (DT_UPGRADESKILL_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSkillID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wUpgradeNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_UPGRADESKILL_TASK_CONDITION(DT_UPGRADESKILL_TASK_CONDITION &data)
{
	Json::Value root;
	root["SkillID"]=data.wSkillID;
	root["UpgradeNum"]=data.wUpgradeNum;

	return root;
}
BOOL FromJsonDT_UPGRADESKILL_TASK_CONDITION(Json::Value &root, DT_UPGRADESKILL_TASK_CONDITION &data)
{
	data.wSkillID=root["SkillID"].asUInt();
	data.wUpgradeNum=root["UpgradeNum"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_UPGRADESKILL_TASK_CONDITION(const string &strData, DT_UPGRADESKILL_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_UPGRADESKILL_TASK_CONDITION(root, data);
}
INT32 EncodeDT_UPGRADEEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEEQUIP_TASK_CONDITION *pstIn = (DT_UPGRADEEQUIP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wEquipID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUpgradeFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_UPGRADEEQUIP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADEEQUIP_TASK_CONDITION *pstOut = (DT_UPGRADEEQUIP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wEquipID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUpgradeFlag))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_UPGRADEEQUIP_TASK_CONDITION(DT_UPGRADEEQUIP_TASK_CONDITION &data)
{
	Json::Value root;
	root["EquipID"]=data.wEquipID;
	root["UpgradeFlag"]=data.byUpgradeFlag;

	return root;
}
BOOL FromJsonDT_UPGRADEEQUIP_TASK_CONDITION(Json::Value &root, DT_UPGRADEEQUIP_TASK_CONDITION &data)
{
	data.wEquipID=root["EquipID"].asUInt();
	data.byUpgradeFlag=root["UpgradeFlag"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_UPGRADEEQUIP_TASK_CONDITION(const string &strData, DT_UPGRADEEQUIP_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_UPGRADEEQUIP_TASK_CONDITION(root, data);
}
INT32 EncodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_USEEXPRIENCEDRUG_TASK_CONDITION *pstIn = (DT_USEEXPRIENCEDRUG_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_USEEXPRIENCEDRUG_TASK_CONDITION *pstOut = (DT_USEEXPRIENCEDRUG_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_USEEXPRIENCEDRUG_TASK_CONDITION(DT_USEEXPRIENCEDRUG_TASK_CONDITION &data)
{
	Json::Value root;
	root["Num"]=data.wNum;

	return root;
}
BOOL FromJsonDT_USEEXPRIENCEDRUG_TASK_CONDITION(Json::Value &root, DT_USEEXPRIENCEDRUG_TASK_CONDITION &data)
{
	data.wNum=root["Num"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_USEEXPRIENCEDRUG_TASK_CONDITION(const string &strData, DT_USEEXPRIENCEDRUG_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_USEEXPRIENCEDRUG_TASK_CONDITION(root, data);
}
INT32 EncodeDT_MULT_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_MULT_TASK_CONDITION *pstIn = (DT_MULT_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_MULT_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_MULT_TASK_CONDITION *pstOut = (DT_MULT_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_MULT_TASK_CONDITION(DT_MULT_TASK_CONDITION &data)
{
	Json::Value root;
	root["Num"]=data.wNum;

	return root;
}
BOOL FromJsonDT_MULT_TASK_CONDITION(Json::Value &root, DT_MULT_TASK_CONDITION &data)
{
	data.wNum=root["Num"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_MULT_TASK_CONDITION(const string &strData, DT_MULT_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_MULT_TASK_CONDITION(root, data);
}
INT32 EncodeDT_COMPOSEGOOD_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_COMPOSEGOOD_TASK_CONDITION *pstIn = (DT_COMPOSEGOOD_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wMaterialID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wMaterialNum))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_COMPOSEGOOD_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_COMPOSEGOOD_TASK_CONDITION *pstOut = (DT_COMPOSEGOOD_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wMaterialID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wMaterialNum))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_COMPOSEGOOD_TASK_CONDITION(DT_COMPOSEGOOD_TASK_CONDITION &data)
{
	Json::Value root;
	root["MaterialID"]=data.wMaterialID;
	root["MaterialNum"]=data.wMaterialNum;
	root["KindID"]=data.wKindID;
	root["Num"]=data.wNum;

	return root;
}
BOOL FromJsonDT_COMPOSEGOOD_TASK_CONDITION(Json::Value &root, DT_COMPOSEGOOD_TASK_CONDITION &data)
{
	data.wMaterialID=root["MaterialID"].asUInt();
	data.wMaterialNum=root["MaterialNum"].asUInt();
	data.wKindID=root["KindID"].asUInt();
	data.wNum=root["Num"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_COMPOSEGOOD_TASK_CONDITION(const string &strData, DT_COMPOSEGOOD_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_COMPOSEGOOD_TASK_CONDITION(root, data);
}
INT32 EncodeDT_ELITEINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_ELITEINSTANCE_TASK_CONDITION *pstIn = (DT_ELITEINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUpgradeFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ELITEINSTANCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_ELITEINSTANCE_TASK_CONDITION *pstOut = (DT_ELITEINSTANCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUpgradeFlag))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_ELITEINSTANCE_TASK_CONDITION(DT_ELITEINSTANCE_TASK_CONDITION &data)
{
	Json::Value root;
	root["SceneIdx"]=data.wSceneIdx;
	root["TownIdx"]=data.byTownIdx;
	root["InstanceIdx"]=data.byInstanceIdx;
	root["UpgradeFlag"]=data.byUpgradeFlag;

	return root;
}
BOOL FromJsonDT_ELITEINSTANCE_TASK_CONDITION(Json::Value &root, DT_ELITEINSTANCE_TASK_CONDITION &data)
{
	data.wSceneIdx=root["SceneIdx"].asUInt();
	data.byTownIdx=root["TownIdx"].asUInt();
	data.byInstanceIdx=root["InstanceIdx"].asUInt();
	data.byUpgradeFlag=root["UpgradeFlag"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_ELITEINSTANCE_TASK_CONDITION(const string &strData, DT_ELITEINSTANCE_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_ELITEINSTANCE_TASK_CONDITION(root, data);
}
INT32 EncodeDT_UPGRADSCIENCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADSCIENCE_TASK_CONDITION *pstIn = (DT_UPGRADSCIENCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_UPGRADSCIENCE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_UPGRADSCIENCE_TASK_CONDITION *pstOut = (DT_UPGRADSCIENCE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_UPGRADSCIENCE_TASK_CONDITION(DT_UPGRADSCIENCE_TASK_CONDITION &data)
{
	Json::Value root;
	root["Num"]=data.wNum;

	return root;
}
BOOL FromJsonDT_UPGRADSCIENCE_TASK_CONDITION(Json::Value &root, DT_UPGRADSCIENCE_TASK_CONDITION &data)
{
	data.wNum=root["Num"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_UPGRADSCIENCE_TASK_CONDITION(const string &strData, DT_UPGRADSCIENCE_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_UPGRADSCIENCE_TASK_CONDITION(root, data);
}
INT32 EncodeDT_STRENGTHATTACK_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHATTACK_TASK_CONDITION *pstIn = (DT_STRENGTHATTACK_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STRENGTHATTACK_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHATTACK_TASK_CONDITION *pstOut = (DT_STRENGTHATTACK_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_STRENGTHATTACK_TASK_CONDITION(DT_STRENGTHATTACK_TASK_CONDITION &data)
{
	Json::Value root;
	root["Num"]=data.wNum;

	return root;
}
BOOL FromJsonDT_STRENGTHATTACK_TASK_CONDITION(Json::Value &root, DT_STRENGTHATTACK_TASK_CONDITION &data)
{
	data.wNum=root["Num"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_STRENGTHATTACK_TASK_CONDITION(const string &strData, DT_STRENGTHATTACK_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_STRENGTHATTACK_TASK_CONDITION(root, data);
}
INT32 EncodeDT_STRENGTHLIFE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHLIFE_TASK_CONDITION *pstIn = (DT_STRENGTHLIFE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STRENGTHLIFE_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_STRENGTHLIFE_TASK_CONDITION *pstOut = (DT_STRENGTHLIFE_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_STRENGTHLIFE_TASK_CONDITION(DT_STRENGTHLIFE_TASK_CONDITION &data)
{
	Json::Value root;
	root["Num"]=data.wNum;

	return root;
}
BOOL FromJsonDT_STRENGTHLIFE_TASK_CONDITION(Json::Value &root, DT_STRENGTHLIFE_TASK_CONDITION &data)
{
	data.wNum=root["Num"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_STRENGTHLIFE_TASK_CONDITION(const string &strData, DT_STRENGTHLIFE_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_STRENGTHLIFE_TASK_CONDITION(root, data);
}
INT32 EncodeDT_CROP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROP_TASK_CONDITION *pstIn = (DT_CROP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CROP_TASK_CONDITION(void *pHost, CNetData* poNetData)
{
	DT_CROP_TASK_CONDITION *pstOut = (DT_CROP_TASK_CONDITION*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_CROP_TASK_CONDITION(DT_CROP_TASK_CONDITION &data)
{
	Json::Value root;
	root["Num"]=data.wNum;

	return root;
}
BOOL FromJsonDT_CROP_TASK_CONDITION(Json::Value &root, DT_CROP_TASK_CONDITION &data)
{
	data.wNum=root["Num"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_CROP_TASK_CONDITION(const string &strData, DT_CROP_TASK_CONDITION &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_CROP_TASK_CONDITION(root, data);
}
INT32 EncodeUnDT_TASK_CONDITION(void *pHost, INT32 iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case CROWN:
		return EncodeDT_CROWN_TASK_CONDITION(pHost, poNetData);
	case RECRUITHERO:
		return EncodeDT_RECRUITHERO_TASK_CONDITION(pHost, poNetData);
	case FIRSTPASSINSTANCE:
		return EncodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(pHost, poNetData);
	case SCOREPASSINSTANCE:
		return EncodeDT_SCOREPASSINSTANCE_TASK_CONDITION(pHost, poNetData);
	case TIMESPASSINSTANCE:
		return EncodeDT_TIMESPASSINSTANCE_TASK_CONDITION(pHost, poNetData);
	case KILLMONSTERINSTANCE:
		return EncodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(pHost, poNetData);
	case STRENGTHENEQUIPLEVEL:
		return EncodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(pHost, poNetData);
	case STRENGTHENEQUIPNUM:
		return EncodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(pHost, poNetData);
	case ACTIVATEGODWEAPON:
		return EncodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(pHost, poNetData);
	case UPGRADEGODWEAPON:
		return EncodeDT_UPGRADEGODWEAPON_TASK_CONDITION(pHost, poNetData);
	case CHALLENGE:
		return EncodeDT_CHALLENGE_TASK_CONDITION(pHost, poNetData);
	case PLUNDER:
		return EncodeDT_PLUNDER_TASK_CONDITION(pHost, poNetData);
	case DRESSEQUIP:
		return EncodeDT_DRESSEQUIP_TASK_CONDITION(pHost, poNetData);
	case USEEXPRIENCEDRUG:
		return EncodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(pHost, poNetData);
	case COMPOSEGOOD:
		return EncodeDT_COMPOSEGOOD_TASK_CONDITION(pHost, poNetData);
	case ELITEINSTANCE:
		return EncodeDT_ELITEINSTANCE_TASK_CONDITION(pHost, poNetData);
	case UPGRADSCIENCE:
		return EncodeDT_UPGRADSCIENCE_TASK_CONDITION(pHost, poNetData);
	case STRENGTHATTACK:
		return EncodeDT_STRENGTHATTACK_TASK_CONDITION(pHost, poNetData);
	case STRENGTHLIFE:
		return EncodeDT_STRENGTHLIFE_TASK_CONDITION(pHost, poNetData);
	case CROP:
		return EncodeDT_CROP_TASK_CONDITION(pHost, poNetData);
	case MULT:
		return EncodeDT_MULT_TASK_CONDITION(pHost, poNetData);
	default: return -1;
	}
}

INT32 DecodeUnDT_TASK_CONDITION(void *pHost, INT32 iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case CROWN:
		return DecodeDT_CROWN_TASK_CONDITION(pHost, poNetData);
	case RECRUITHERO:
		return DecodeDT_RECRUITHERO_TASK_CONDITION(pHost, poNetData);
	case FIRSTPASSINSTANCE:
		return DecodeDT_FIRSTPASSINSTANCE_TASK_CONDITION(pHost, poNetData);
	case SCOREPASSINSTANCE:
		return DecodeDT_SCOREPASSINSTANCE_TASK_CONDITION(pHost, poNetData);
	case TIMESPASSINSTANCE:
		return DecodeDT_TIMESPASSINSTANCE_TASK_CONDITION(pHost, poNetData);
	case KILLMONSTERINSTANCE:
		return DecodeDT_KILLMONSTERINSTANCE_TASK_CONDITION(pHost, poNetData);
	case STRENGTHENEQUIPLEVEL:
		return DecodeDT_STRENGTHENEQUIPLEVEL_TASK_CONDITION(pHost, poNetData);
	case STRENGTHENEQUIPNUM:
		return DecodeDT_STRENGTHENEQUIPNUM_TASK_CONDITION(pHost, poNetData);
	case ACTIVATEGODWEAPON:
		return DecodeDT_ACTIVATEGODWEAPON_TASK_CONDITION(pHost, poNetData);
	case UPGRADEGODWEAPON:
		return DecodeDT_UPGRADEGODWEAPON_TASK_CONDITION(pHost, poNetData);
	case CHALLENGE:
		return DecodeDT_CHALLENGE_TASK_CONDITION(pHost, poNetData);
	case PLUNDER:
		return DecodeDT_PLUNDER_TASK_CONDITION(pHost, poNetData);
	case DRESSEQUIP:
		return DecodeDT_DRESSEQUIP_TASK_CONDITION(pHost, poNetData);
	case USEEXPRIENCEDRUG:
		return DecodeDT_USEEXPRIENCEDRUG_TASK_CONDITION(pHost, poNetData);
	case COMPOSEGOOD:
		return DecodeDT_COMPOSEGOOD_TASK_CONDITION(pHost, poNetData);
	case ELITEINSTANCE:
		return DecodeDT_ELITEINSTANCE_TASK_CONDITION(pHost, poNetData);
	case UPGRADSCIENCE:
		return DecodeDT_UPGRADSCIENCE_TASK_CONDITION(pHost, poNetData);
	case STRENGTHATTACK:
		return DecodeDT_STRENGTHATTACK_TASK_CONDITION(pHost, poNetData);
	case STRENGTHLIFE:
		return DecodeDT_STRENGTHLIFE_TASK_CONDITION(pHost, poNetData);
	case CROP:
		return DecodeDT_CROP_TASK_CONDITION(pHost, poNetData);
	case MULT:
		return DecodeDT_MULT_TASK_CONDITION(pHost, poNetData);
	default: return -1;
	}
}

INT32 EncodeDT_TASK_DATA(void *pHost, CNetData* poNetData)
{
	DT_TASK_DATA *pstIn = (DT_TASK_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byTaskLine))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwTaskIdx))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwStateUpdateTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTaskState))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTaskType))
		return -1;

	if(-1 == EncodeUnDT_TASK_CONDITION(&pstIn->stTaskConditionInfo, pstIn->byTaskType, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_TASK_DATA(void *pHost, CNetData* poNetData)
{
	DT_TASK_DATA *pstOut = (DT_TASK_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byTaskLine))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwTaskIdx))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwStateUpdateTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTaskState))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTaskType))
		return -1;

	if(-1 == DecodeUnDT_TASK_CONDITION(&pstOut->stTaskConditionInfo, pstOut->byTaskType, poNetData))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_TASK_DATA(DT_TASK_DATA &data)
{
	Json::Value root;
	root["TaskLine"]=data.byTaskLine;
	root["TaskIdx"]=data.dwTaskIdx;
	ostringstream ssqwStateUpdateTime;
	ssqwStateUpdateTime<<data.qwStateUpdateTime;
	root["StateUpdateTime"]=ssqwStateUpdateTime.str();
	root["TaskState"]=data.byTaskState;
	root["TaskType"]=data.byTaskType;

	return root;
}
BOOL FromJsonDT_TASK_DATA(Json::Value &root, DT_TASK_DATA &data)
{
	data.byTaskLine=root["TaskLine"].asUInt();
	data.dwTaskIdx=root["TaskIdx"].asUInt();
	data.qwStateUpdateTime=SDAtou64( root["StateUpdateTime"].asCString() );
	data.byTaskState=root["TaskState"].asUInt();
	data.byTaskType=root["TaskType"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_TASK_DATA(const string &strData, DT_TASK_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_TASK_DATA(root, data);
}
INT32 EncodeDT_TASK_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_TASK_DATA_LIST *pstIn = (DT_TASK_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byTaskNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_TASK_LINE_NUM; i++)
	{
		if(i >= (INT32)pstIn->byTaskNum)
			break;
		if(-1 == EncodeDT_TASK_DATA(&pstIn->astTaskInfoList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_TASK_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_TASK_DATA_LIST *pstOut = (DT_TASK_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byTaskNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_TASK_LINE_NUM; i++)
	{
		if(i >= (INT32)pstOut->byTaskNum)
			break;
		if(-1 == DecodeDT_TASK_DATA(&pstOut->astTaskInfoList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_TASK_DATA_LIST(DT_TASK_DATA_LIST &data)
{
	Json::Value root;
	root["TaskNum"]=data.byTaskNum;
	for( UINT32 dwIndex=0; dwIndex<data.byTaskNum; ++dwIndex)
	{
	root["TaskInfoList"].append( GetJsonDT_TASK_DATA(data.astTaskInfoList[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_TASK_DATA_LIST(Json::Value &root, DT_TASK_DATA_LIST &data)
{
	data.byTaskNum=root["TaskNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byTaskNum; ++dwIndex)
	{
		FromJsonDT_TASK_DATA(root["TaskInfoList"][dwIndex] , data.astTaskInfoList[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_TASK_DATA_LIST(const string &strData, DT_TASK_DATA_LIST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_TASK_DATA_LIST(root, data);
}
INT32 EncodeDT_RACE_DATA(void *pHost, CNetData* poNetData)
{
	DT_RACE_DATA *pstIn = (DT_RACE_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwRank))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wContiWin))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwRecvEncourageTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastChallTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byChallTimes))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byFailedFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUnreadBattleLogNum))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwRecvEncourageRank))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_RACE_DATA(void *pHost, CNetData* poNetData)
{
	DT_RACE_DATA *pstOut = (DT_RACE_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwRank))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wContiWin))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwRecvEncourageTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastChallTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byChallTimes))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byFailedFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUnreadBattleLogNum))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwRecvEncourageRank))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_RACE_DATA(DT_RACE_DATA &data)
{
	Json::Value root;
	root["Rank"]=data.dwRank;
	root["ContiWin"]=data.wContiWin;
	ostringstream ssqwRecvEncourageTime;
	ssqwRecvEncourageTime<<data.qwRecvEncourageTime;
	root["RecvEncourageTime"]=ssqwRecvEncourageTime.str();
	ostringstream ssqwLastChallTime;
	ssqwLastChallTime<<data.qwLastChallTime;
	root["LastChallTime"]=ssqwLastChallTime.str();
	root["ChallTimes"]=data.byChallTimes;
	root["FailedFlag"]=data.byFailedFlag;
	root["UnreadBattleLogNum"]=data.byUnreadBattleLogNum;
	root["RecvEncourageRank"]=data.dwRecvEncourageRank;

	return root;
}
BOOL FromJsonDT_RACE_DATA(Json::Value &root, DT_RACE_DATA &data)
{
	data.dwRank=root["Rank"].asUInt();
	data.wContiWin=root["ContiWin"].asUInt();
	data.qwRecvEncourageTime=SDAtou64( root["RecvEncourageTime"].asCString() );
	data.qwLastChallTime=SDAtou64( root["LastChallTime"].asCString() );
	data.byChallTimes=root["ChallTimes"].asUInt();
	data.byFailedFlag=root["FailedFlag"].asUInt();
	data.byUnreadBattleLogNum=root["UnreadBattleLogNum"].asUInt();
	data.dwRecvEncourageRank=root["RecvEncourageRank"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_RACE_DATA(const string &strData, DT_RACE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_RACE_DATA(root, data);
}
INT32 EncodeDT_LOCALE_HOME(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_HOME *pstIn = (DT_LOCALE_HOME*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byExt))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LOCALE_HOME(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_HOME *pstOut = (DT_LOCALE_HOME*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byExt))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_LOCALE_HOME(DT_LOCALE_HOME &data)
{
	Json::Value root;
	root["Ext"]=data.byExt;

	return root;
}
BOOL FromJsonDT_LOCALE_HOME(Json::Value &root, DT_LOCALE_HOME &data)
{
	data.byExt=root["Ext"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_LOCALE_HOME(const string &strData, DT_LOCALE_HOME &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_LOCALE_HOME(root, data);
}
INT32 EncodeDT_LOCALE_INSTANCE(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_INSTANCE *pstIn = (DT_LOCALE_INSTANCE*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBattleIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->bySuccessFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LOCALE_INSTANCE(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_INSTANCE *pstOut = (DT_LOCALE_INSTANCE*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBattleIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->bySuccessFlag))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_LOCALE_INSTANCE(DT_LOCALE_INSTANCE &data)
{
	Json::Value root;
	root["SceneIdx"]=data.wSceneIdx;
	root["TownIdx"]=data.byTownIdx;
	root["InstanceIdx"]=data.byInstanceIdx;
	root["BattleIdx"]=data.byBattleIdx;
	root["SuccessFlag"]=data.bySuccessFlag;

	return root;
}
BOOL FromJsonDT_LOCALE_INSTANCE(Json::Value &root, DT_LOCALE_INSTANCE &data)
{
	data.wSceneIdx=root["SceneIdx"].asUInt();
	data.byTownIdx=root["TownIdx"].asUInt();
	data.byInstanceIdx=root["InstanceIdx"].asUInt();
	data.byBattleIdx=root["BattleIdx"].asUInt();
	data.bySuccessFlag=root["SuccessFlag"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_LOCALE_INSTANCE(const string &strData, DT_LOCALE_INSTANCE &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_LOCALE_INSTANCE(root, data);
}
INT32 EncodeDT_LOCALE_ONHOOK(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_ONHOOK *pstIn = (DT_LOCALE_ONHOOK*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wSceneIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTownIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byInstanceIdx))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LOCALE_ONHOOK(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_ONHOOK *pstOut = (DT_LOCALE_ONHOOK*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wSceneIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTownIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byInstanceIdx))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_LOCALE_ONHOOK(DT_LOCALE_ONHOOK &data)
{
	Json::Value root;
	root["SceneIdx"]=data.wSceneIdx;
	root["TownIdx"]=data.byTownIdx;
	root["InstanceIdx"]=data.byInstanceIdx;

	return root;
}
BOOL FromJsonDT_LOCALE_ONHOOK(Json::Value &root, DT_LOCALE_ONHOOK &data)
{
	data.wSceneIdx=root["SceneIdx"].asUInt();
	data.byTownIdx=root["TownIdx"].asUInt();
	data.byInstanceIdx=root["InstanceIdx"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_LOCALE_ONHOOK(const string &strData, DT_LOCALE_ONHOOK &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_LOCALE_ONHOOK(root, data);
}
INT32 EncodeUnDT_LOCALE_DATA_UNION(void *pHost, INT32 iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case EPL_HOME:
		return EncodeDT_LOCALE_HOME(pHost, poNetData);
	case EPL_INSTANCE:
		return EncodeDT_LOCALE_INSTANCE(pHost, poNetData);
	case EPL_ONHOOK:
		return EncodeDT_LOCALE_ONHOOK(pHost, poNetData);
	default: return -1;
	}
}

INT32 DecodeUnDT_LOCALE_DATA_UNION(void *pHost, INT32 iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case EPL_HOME:
		return DecodeDT_LOCALE_HOME(pHost, poNetData);
	case EPL_INSTANCE:
		return DecodeDT_LOCALE_INSTANCE(pHost, poNetData);
	case EPL_ONHOOK:
		return DecodeDT_LOCALE_ONHOOK(pHost, poNetData);
	default: return -1;
	}
}

INT32 EncodeDT_LOCALE_DATA(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_DATA *pstIn = (DT_LOCALE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byLocaleType))
		return -1;

	if(-1 == EncodeUnDT_LOCALE_DATA_UNION(&pstIn->stLocaleInfo, pstIn->byLocaleType, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LOCALE_DATA(void *pHost, CNetData* poNetData)
{
	DT_LOCALE_DATA *pstOut = (DT_LOCALE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byLocaleType))
		return -1;

	if(-1 == DecodeUnDT_LOCALE_DATA_UNION(&pstOut->stLocaleInfo, pstOut->byLocaleType, poNetData))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_LOCALE_DATA(DT_LOCALE_DATA &data)
{
	Json::Value root;
	root["LocaleType"]=data.byLocaleType;

	return root;
}
BOOL FromJsonDT_LOCALE_DATA(Json::Value &root, DT_LOCALE_DATA &data)
{
	data.byLocaleType=root["LocaleType"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_LOCALE_DATA(const string &strData, DT_LOCALE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_LOCALE_DATA(root, data);
}
INT32 EncodeUnlockScienceInfo(void *pHost, CNetData* poNetData)
{
	UnlockScienceInfo *pstIn = (UnlockScienceInfo*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byUnlockNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SCIENCE_ATTR_NUM; i++)
	{
		if(i >= (INT32)pstIn->byUnlockNum)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyUnlockScienceID[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeUnlockScienceInfo(void *pHost, CNetData* poNetData)
{
	UnlockScienceInfo *pstOut = (UnlockScienceInfo*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byUnlockNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_SCIENCE_ATTR_NUM; i++)
	{
		if(i >= (INT32)pstOut->byUnlockNum)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyUnlockScienceID[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonUnlockScienceInfo(UnlockScienceInfo &data)
{
	Json::Value root;
	root["UnlockNum"]=data.byUnlockNum;
	for( UINT32 dwIndex=0; dwIndex<data.byUnlockNum; ++dwIndex)
	{
	root["UnlockScienceID"].append( data.abyUnlockScienceID[dwIndex]);
	}

	return root;
}
BOOL FromJsonUnlockScienceInfo(Json::Value &root, UnlockScienceInfo &data)
{
	data.byUnlockNum=root["UnlockNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byUnlockNum; ++dwIndex)
	{
		data.abyUnlockScienceID[dwIndex]=BYTE(root["UnlockScienceID"][dwIndex].asUInt() );
	}

	return TRUE;
}
BOOL FromJsonUnlockScienceInfo(const string &strData, UnlockScienceInfo &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonUnlockScienceInfo(root, data);
}
INT32 EncodeDT_STATE_DATA(void *pHost, CNetData* poNetData)
{
	DT_STATE_DATA *pstIn = (DT_STATE_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwInstanceMAXHP))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwInstanceCurHP))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wDoubleExpNum))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byOpenBoxFlag))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ENCOURAGE_BOX_GROUP_NUM; i++)
	{
		if(-1 == poNetData->AddByte(pstIn->abyHitItemIdx[i]))
			return -1;
	}

	for(i = 0; i < MAX_UNLOCK_HERO_NUM; i++)
	{
		if(-1 == poNetData->AddWord(pstIn->awUnlockHeroID[i]))
			return -1;
	}

	if(-1 == poNetData->AddWord(pstIn->wUnlockSkillID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wUnlockEquipLevel))
		return -1;

	if(-1 == EncodeUnlockScienceInfo(&pstIn->stUnlockScienceList, poNetData))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwRegisterDispNameTime))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUnlockElite))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STATE_DATA(void *pHost, CNetData* poNetData)
{
	DT_STATE_DATA *pstOut = (DT_STATE_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwInstanceMAXHP))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwInstanceCurHP))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wDoubleExpNum))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byOpenBoxFlag))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ENCOURAGE_BOX_GROUP_NUM; i++)
	{
		if(-1 == poNetData->DelByte(pstOut->abyHitItemIdx[i]))
			return -1;
	}

	for(i = 0; i < MAX_UNLOCK_HERO_NUM; i++)
	{
		if(-1 == poNetData->DelWord(pstOut->awUnlockHeroID[i]))
			return -1;
	}

	if(-1 == poNetData->DelWord(pstOut->wUnlockSkillID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wUnlockEquipLevel))
		return -1;

	if(-1 == DecodeUnlockScienceInfo(&pstOut->stUnlockScienceList, poNetData))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwRegisterDispNameTime))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUnlockElite))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_STATE_DATA(DT_STATE_DATA &data)
{
	Json::Value root;
	root["InstanceMAXHP"]=data.dwInstanceMAXHP;
	root["InstanceCurHP"]=data.dwInstanceCurHP;
	root["DoubleExpNum"]=data.wDoubleExpNum;
	root["OpenBoxFlag"]=data.byOpenBoxFlag;
	for( UINT32 dwIndex=0; dwIndex<MAX_ENCOURAGE_BOX_GROUP_NUM; ++dwIndex)
	{
	root["HitItemIdx"].append( data.abyHitItemIdx[dwIndex]);
	}
	for( UINT32 dwIndex=0; dwIndex<MAX_UNLOCK_HERO_NUM; ++dwIndex)
	{
	root["UnlockHeroID"].append( data.awUnlockHeroID[dwIndex]);
	}
	root["UnlockSkillID"]=data.wUnlockSkillID;
	root["UnlockEquipLevel"]=data.wUnlockEquipLevel;
	root["UnlockScienceList"]=GetJsonUnlockScienceInfo(data.stUnlockScienceList);
	ostringstream ssqwRegisterDispNameTime;
	ssqwRegisterDispNameTime<<data.qwRegisterDispNameTime;
	root["RegisterDispNameTime"]=ssqwRegisterDispNameTime.str();
	root["UnlockElite"]=data.dwUnlockElite;

	return root;
}
BOOL FromJsonDT_STATE_DATA(Json::Value &root, DT_STATE_DATA &data)
{
	data.dwInstanceMAXHP=root["InstanceMAXHP"].asUInt();
	data.dwInstanceCurHP=root["InstanceCurHP"].asUInt();
	data.wDoubleExpNum=root["DoubleExpNum"].asUInt();
	data.byOpenBoxFlag=root["OpenBoxFlag"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<MAX_ENCOURAGE_BOX_GROUP_NUM; ++dwIndex)
	{
		data.abyHitItemIdx[dwIndex]=BYTE(root["HitItemIdx"][dwIndex].asUInt() );
	}
	for( UINT32 dwIndex=0; dwIndex<MAX_UNLOCK_HERO_NUM; ++dwIndex)
	{
		data.awUnlockHeroID[dwIndex]=WORD(root["UnlockHeroID"][dwIndex].asUInt() );
	}
	data.wUnlockSkillID=root["UnlockSkillID"].asUInt();
	data.wUnlockEquipLevel=root["UnlockEquipLevel"].asUInt();
	FromJsonUnlockScienceInfo(root["UnlockScienceList"] , data.stUnlockScienceList);
	data.qwRegisterDispNameTime=SDAtou64( root["RegisterDispNameTime"].asCString() );
	data.dwUnlockElite=root["UnlockElite"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_STATE_DATA(const string &strData, DT_STATE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_STATE_DATA(root, data);
}
INT32 EncodeDT_MONSTER_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_MONSTER_BASE_DATA *pstIn = (DT_MONSTER_BASE_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_MONSTER_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_MONSTER_BASE_DATA *pstOut = (DT_MONSTER_BASE_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_MONSTER_BASE_DATA(DT_MONSTER_BASE_DATA &data)
{
	Json::Value root;
	root["KindID"]=data.wKindID;
	root["Level"]=data.wLevel;

	return root;
}
BOOL FromJsonDT_MONSTER_BASE_DATA(Json::Value &root, DT_MONSTER_BASE_DATA &data)
{
	data.wKindID=root["KindID"].asUInt();
	data.wLevel=root["Level"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_MONSTER_BASE_DATA(const string &strData, DT_MONSTER_BASE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_MONSTER_BASE_DATA(root, data);
}
INT32 EncodeDT_MONSTER_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_MONSTER_DATA_LIST *pstIn = (DT_MONSTER_DATA_LIST*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wMonsterNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_INSTANCE_MONSTER_NUM; i++)
	{
		if(i >= (INT32)pstIn->wMonsterNum)
			break;
		if(-1 == EncodeDT_MONSTER_BASE_DATA(&pstIn->astMonsterInfListo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_MONSTER_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_MONSTER_DATA_LIST *pstOut = (DT_MONSTER_DATA_LIST*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wMonsterNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_INSTANCE_MONSTER_NUM; i++)
	{
		if(i >= (INT32)pstOut->wMonsterNum)
			break;
		if(-1 == DecodeDT_MONSTER_BASE_DATA(&pstOut->astMonsterInfListo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_MONSTER_DATA_LIST(DT_MONSTER_DATA_LIST &data)
{
	Json::Value root;
	root["MonsterNum"]=data.wMonsterNum;
	for( UINT32 dwIndex=0; dwIndex<data.wMonsterNum; ++dwIndex)
	{
	root["MonsterInfListo"].append( GetJsonDT_MONSTER_BASE_DATA(data.astMonsterInfListo[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_MONSTER_DATA_LIST(Json::Value &root, DT_MONSTER_DATA_LIST &data)
{
	data.wMonsterNum=root["MonsterNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.wMonsterNum; ++dwIndex)
	{
		FromJsonDT_MONSTER_BASE_DATA(root["MonsterInfListo"][dwIndex] , data.astMonsterInfListo[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_MONSTER_DATA_LIST(const string &strData, DT_MONSTER_DATA_LIST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_MONSTER_DATA_LIST(root, data);
}
INT32 EncodeDT_BATTLE_OBJ_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_OBJ_DATA *pstIn = (DT_BATTLE_OBJ_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byCareerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byFomationIdx))
		return -1;

	if(-1 == EncodeDT_BATTLE_ATTRIBUTE(&pstIn->stBattleAttr, poNetData))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wSkillID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wSkillLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wNeedAngry))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wInitAngry))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byHPGridNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BATTLE_OBJ_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_OBJ_DATA *pstOut = (DT_BATTLE_OBJ_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byCareerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byFomationIdx))
		return -1;

	if(-1 == DecodeDT_BATTLE_ATTRIBUTE(&pstOut->stBattleAttr, poNetData))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wSkillID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wSkillLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wNeedAngry))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wInitAngry))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byHPGridNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_BATTLE_OBJ_DATA(DT_BATTLE_OBJ_DATA &data)
{
	Json::Value root;
	root["KindID"]=data.wKindID;
	root["CareerID"]=data.byCareerID;
	root["Level"]=data.wLevel;
	root["FomationIdx"]=data.byFomationIdx;
	root["BattleAttr"]=GetJsonDT_BATTLE_ATTRIBUTE(data.stBattleAttr);
	root["SkillID"]=data.wSkillID;
	root["SkillLevel"]=data.wSkillLevel;
	root["NeedAngry"]=data.wNeedAngry;
	root["InitAngry"]=data.wInitAngry;
	root["HPGridNum"]=data.byHPGridNum;

	return root;
}
BOOL FromJsonDT_BATTLE_OBJ_DATA(Json::Value &root, DT_BATTLE_OBJ_DATA &data)
{
	data.wKindID=root["KindID"].asUInt();
	data.byCareerID=root["CareerID"].asUInt();
	data.wLevel=root["Level"].asUInt();
	data.byFomationIdx=root["FomationIdx"].asUInt();
	FromJsonDT_BATTLE_ATTRIBUTE(root["BattleAttr"] , data.stBattleAttr);
	data.wSkillID=root["SkillID"].asUInt();
	data.wSkillLevel=root["SkillLevel"].asUInt();
	data.wNeedAngry=root["NeedAngry"].asUInt();
	data.wInitAngry=root["InitAngry"].asUInt();
	data.byHPGridNum=root["HPGridNum"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_BATTLE_OBJ_DATA(const string &strData, DT_BATTLE_OBJ_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_BATTLE_OBJ_DATA(root, data);
}
INT32 EncodeDT_BATTLE_OBJ_GROUP_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_OBJ_GROUP_DATA *pstIn = (DT_BATTLE_OBJ_GROUP_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byObjNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstIn->byObjNum)
			break;
		if(-1 == EncodeDT_BATTLE_OBJ_DATA(&pstIn->astBattleObjInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BATTLE_OBJ_GROUP_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_OBJ_GROUP_DATA *pstOut = (DT_BATTLE_OBJ_GROUP_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byObjNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstOut->byObjNum)
			break;
		if(-1 == DecodeDT_BATTLE_OBJ_DATA(&pstOut->astBattleObjInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_BATTLE_OBJ_GROUP_DATA(DT_BATTLE_OBJ_GROUP_DATA &data)
{
	Json::Value root;
	root["ObjNum"]=data.byObjNum;
	for( UINT32 dwIndex=0; dwIndex<data.byObjNum; ++dwIndex)
	{
	root["BattleObjInfo"].append( GetJsonDT_BATTLE_OBJ_DATA(data.astBattleObjInfo[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_BATTLE_OBJ_GROUP_DATA(Json::Value &root, DT_BATTLE_OBJ_GROUP_DATA &data)
{
	data.byObjNum=root["ObjNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byObjNum; ++dwIndex)
	{
		FromJsonDT_BATTLE_OBJ_DATA(root["BattleObjInfo"][dwIndex] , data.astBattleObjInfo[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_BATTLE_OBJ_GROUP_DATA(const string &strData, DT_BATTLE_OBJ_GROUP_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_BATTLE_OBJ_GROUP_DATA(root, data);
}
INT32 EncodeDT_ITEM_DATA(void *pHost, CNetData* poNetData)
{
	DT_ITEM_DATA *pstIn = (DT_ITEM_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKindID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ITEM_DATA(void *pHost, CNetData* poNetData)
{
	DT_ITEM_DATA *pstOut = (DT_ITEM_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKindID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_ITEM_DATA(DT_ITEM_DATA &data)
{
	Json::Value root;
	root["KindID"]=data.wKindID;
	root["Num"]=data.wNum;

	return root;
}
BOOL FromJsonDT_ITEM_DATA(Json::Value &root, DT_ITEM_DATA &data)
{
	data.wKindID=root["KindID"].asUInt();
	data.wNum=root["Num"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_ITEM_DATA(const string &strData, DT_ITEM_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_ITEM_DATA(root, data);
}
INT32 EncodeDT_BUFF_DATA(void *pHost, CNetData* poNetData)
{
	DT_BUFF_DATA *pstIn = (DT_BUFF_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wAddDelFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBuffID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BUFF_DATA(void *pHost, CNetData* poNetData)
{
	DT_BUFF_DATA *pstOut = (DT_BUFF_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wAddDelFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBuffID))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_BUFF_DATA(DT_BUFF_DATA &data)
{
	Json::Value root;
	root["FormationIdx"]=data.byFormationIdx;
	root["AddDelFlag"]=data.wAddDelFlag;
	root["BuffID"]=data.byBuffID;

	return root;
}
BOOL FromJsonDT_BUFF_DATA(Json::Value &root, DT_BUFF_DATA &data)
{
	data.byFormationIdx=root["FormationIdx"].asUInt();
	data.wAddDelFlag=root["AddDelFlag"].asUInt();
	data.byBuffID=root["BuffID"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_BUFF_DATA(const string &strData, DT_BUFF_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_BUFF_DATA(root, data);
}
INT32 EncodeDT_HP_DATA(void *pHost, CNetData* poNetData)
{
	DT_HP_DATA *pstIn = (DT_HP_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddInt(pstIn->nHP))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_HP_DATA(void *pHost, CNetData* poNetData)
{
	DT_HP_DATA *pstOut = (DT_HP_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelInt(pstOut->nHP))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_HP_DATA(DT_HP_DATA &data)
{
	Json::Value root;
	root["FormationIdx"]=data.byFormationIdx;
	root["HP"]=data.nHP;

	return root;
}
BOOL FromJsonDT_HP_DATA(Json::Value &root, DT_HP_DATA &data)
{
	data.byFormationIdx=root["FormationIdx"].asUInt();
	data.nHP=root["HP"].asInt();

	return TRUE;
}
BOOL FromJsonDT_HP_DATA(const string &strData, DT_HP_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_HP_DATA(root, data);
}
INT32 EncodeDT_ANGRY_DATA(void *pHost, CNetData* poNetData)
{
	DT_ANGRY_DATA *pstIn = (DT_ANGRY_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddShort(pstIn->shAngry))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ANGRY_DATA(void *pHost, CNetData* poNetData)
{
	DT_ANGRY_DATA *pstOut = (DT_ANGRY_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelShort(pstOut->shAngry))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_ANGRY_DATA(DT_ANGRY_DATA &data)
{
	Json::Value root;
	root["FormationIdx"]=data.byFormationIdx;
	root["Angry"]=data.shAngry;

	return root;
}
BOOL FromJsonDT_ANGRY_DATA(Json::Value &root, DT_ANGRY_DATA &data)
{
	data.byFormationIdx=root["FormationIdx"].asUInt();
	data.shAngry=root["Angry"].asInt();

	return TRUE;
}
BOOL FromJsonDT_ANGRY_DATA(const string &strData, DT_ANGRY_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_ANGRY_DATA(root, data);
}
INT32 EncodeDT_PARRY_DATA(void *pHost, CNetData* poNetData)
{
	DT_PARRY_DATA *pstIn = (DT_PARRY_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byParryFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PARRY_DATA(void *pHost, CNetData* poNetData)
{
	DT_PARRY_DATA *pstOut = (DT_PARRY_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byParryFlag))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_PARRY_DATA(DT_PARRY_DATA &data)
{
	Json::Value root;
	root["FormationIdx"]=data.byFormationIdx;
	root["ParryFlag"]=data.byParryFlag;

	return root;
}
BOOL FromJsonDT_PARRY_DATA(Json::Value &root, DT_PARRY_DATA &data)
{
	data.byFormationIdx=root["FormationIdx"].asUInt();
	data.byParryFlag=root["ParryFlag"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_PARRY_DATA(const string &strData, DT_PARRY_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_PARRY_DATA(root, data);
}
INT32 EncodeDT_SKILL_EFFECT_ID_DATA(void *pHost, CNetData* poNetData)
{
	DT_SKILL_EFFECT_ID_DATA *pstIn = (DT_SKILL_EFFECT_ID_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wEffectID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SKILL_EFFECT_ID_DATA(void *pHost, CNetData* poNetData)
{
	DT_SKILL_EFFECT_ID_DATA *pstOut = (DT_SKILL_EFFECT_ID_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wEffectID))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_SKILL_EFFECT_ID_DATA(DT_SKILL_EFFECT_ID_DATA &data)
{
	Json::Value root;
	root["FormationIdx"]=data.byFormationIdx;
	root["EffectID"]=data.wEffectID;

	return root;
}
BOOL FromJsonDT_SKILL_EFFECT_ID_DATA(Json::Value &root, DT_SKILL_EFFECT_ID_DATA &data)
{
	data.byFormationIdx=root["FormationIdx"].asUInt();
	data.wEffectID=root["EffectID"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_SKILL_EFFECT_ID_DATA(const string &strData, DT_SKILL_EFFECT_ID_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_SKILL_EFFECT_ID_DATA(root, data);
}
INT32 EncodeDT_ATTACK_OBJ_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_OBJ_DATA *pstIn = (DT_ATTACK_OBJ_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byBuffChangeNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BUFF_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstIn->byBuffChangeNum)
			break;
		if(-1 == EncodeDT_BUFF_DATA(&pstIn->astBuffChangeInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->AddByte(pstIn->byHpChangeNum))
		return -1;

	for(i = 0; i < MAX_HP_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstIn->byHpChangeNum)
			break;
		if(-1 == EncodeDT_HP_DATA(&pstIn->astHpChangeInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->AddByte(pstIn->byAngryChangeNum))
		return -1;

	for(i = 0; i < MAX_ANGRY_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstIn->byAngryChangeNum)
			break;
		if(-1 == EncodeDT_ANGRY_DATA(&pstIn->astAngryChangeInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->AddByte(pstIn->bySkillEffectIDChangeNum))
		return -1;

	for(i = 0; i < MAX_BUFF_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstIn->bySkillEffectIDChangeNum)
			break;
		if(-1 == EncodeDT_SKILL_EFFECT_ID_DATA(&pstIn->astSkillEffectIDChangeInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ATTACK_OBJ_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_OBJ_DATA *pstOut = (DT_ATTACK_OBJ_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byBuffChangeNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BUFF_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstOut->byBuffChangeNum)
			break;
		if(-1 == DecodeDT_BUFF_DATA(&pstOut->astBuffChangeInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->DelByte(pstOut->byHpChangeNum))
		return -1;

	for(i = 0; i < MAX_HP_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstOut->byHpChangeNum)
			break;
		if(-1 == DecodeDT_HP_DATA(&pstOut->astHpChangeInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->DelByte(pstOut->byAngryChangeNum))
		return -1;

	for(i = 0; i < MAX_ANGRY_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstOut->byAngryChangeNum)
			break;
		if(-1 == DecodeDT_ANGRY_DATA(&pstOut->astAngryChangeInfo[i], poNetData))
			return -1;
	}

	if(-1 == poNetData->DelByte(pstOut->bySkillEffectIDChangeNum))
		return -1;

	for(i = 0; i < MAX_BUFF_CHANGE_TIMES_PER_ROUND; i++)
	{
		if(i >= (INT32)pstOut->bySkillEffectIDChangeNum)
			break;
		if(-1 == DecodeDT_SKILL_EFFECT_ID_DATA(&pstOut->astSkillEffectIDChangeInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_ATTACK_OBJ_DATA(DT_ATTACK_OBJ_DATA &data)
{
	Json::Value root;
	root["BuffChangeNum"]=data.byBuffChangeNum;
	for( UINT32 dwIndex=0; dwIndex<data.byBuffChangeNum; ++dwIndex)
	{
	root["BuffChangeInfo"].append( GetJsonDT_BUFF_DATA(data.astBuffChangeInfo[dwIndex]) );
	}
	root["HpChangeNum"]=data.byHpChangeNum;
	for( UINT32 dwIndex=0; dwIndex<data.byHpChangeNum; ++dwIndex)
	{
	root["HpChangeInfo"].append( GetJsonDT_HP_DATA(data.astHpChangeInfo[dwIndex]) );
	}
	root["AngryChangeNum"]=data.byAngryChangeNum;
	for( UINT32 dwIndex=0; dwIndex<data.byAngryChangeNum; ++dwIndex)
	{
	root["AngryChangeInfo"].append( GetJsonDT_ANGRY_DATA(data.astAngryChangeInfo[dwIndex]) );
	}
	root["SkillEffectIDChangeNum"]=data.bySkillEffectIDChangeNum;
	for( UINT32 dwIndex=0; dwIndex<data.bySkillEffectIDChangeNum; ++dwIndex)
	{
	root["SkillEffectIDChangeInfo"].append( GetJsonDT_SKILL_EFFECT_ID_DATA(data.astSkillEffectIDChangeInfo[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_ATTACK_OBJ_DATA(Json::Value &root, DT_ATTACK_OBJ_DATA &data)
{
	data.byBuffChangeNum=root["BuffChangeNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byBuffChangeNum; ++dwIndex)
	{
		FromJsonDT_BUFF_DATA(root["BuffChangeInfo"][dwIndex] , data.astBuffChangeInfo[dwIndex] );
	}	data.byHpChangeNum=root["HpChangeNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byHpChangeNum; ++dwIndex)
	{
		FromJsonDT_HP_DATA(root["HpChangeInfo"][dwIndex] , data.astHpChangeInfo[dwIndex] );
	}	data.byAngryChangeNum=root["AngryChangeNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byAngryChangeNum; ++dwIndex)
	{
		FromJsonDT_ANGRY_DATA(root["AngryChangeInfo"][dwIndex] , data.astAngryChangeInfo[dwIndex] );
	}	data.bySkillEffectIDChangeNum=root["SkillEffectIDChangeNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.bySkillEffectIDChangeNum; ++dwIndex)
	{
		FromJsonDT_SKILL_EFFECT_ID_DATA(root["SkillEffectIDChangeInfo"][dwIndex] , data.astSkillEffectIDChangeInfo[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_ATTACK_OBJ_DATA(const string &strData, DT_ATTACK_OBJ_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_ATTACK_OBJ_DATA(root, data);
}
INT32 EncodeDT_ATTACK_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_DATA *pstIn = (DT_ATTACK_DATA*)pHost;

	if(-1 == EncodeDT_ATTACK_OBJ_DATA(&pstIn->stMyselfObjInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_ATTACK_OBJ_DATA(&pstIn->stEnemyObjInfo, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ATTACK_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_DATA *pstOut = (DT_ATTACK_DATA*)pHost;

	if(-1 == DecodeDT_ATTACK_OBJ_DATA(&pstOut->stMyselfObjInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_ATTACK_OBJ_DATA(&pstOut->stEnemyObjInfo, poNetData))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_ATTACK_DATA(DT_ATTACK_DATA &data)
{
	Json::Value root;
	root["MyselfObjInfo"]=GetJsonDT_ATTACK_OBJ_DATA(data.stMyselfObjInfo);
	root["EnemyObjInfo"]=GetJsonDT_ATTACK_OBJ_DATA(data.stEnemyObjInfo);

	return root;
}
BOOL FromJsonDT_ATTACK_DATA(Json::Value &root, DT_ATTACK_DATA &data)
{
	FromJsonDT_ATTACK_OBJ_DATA(root["MyselfObjInfo"] , data.stMyselfObjInfo);
	FromJsonDT_ATTACK_OBJ_DATA(root["EnemyObjInfo"] , data.stEnemyObjInfo);

	return TRUE;
}
BOOL FromJsonDT_ATTACK_DATA(const string &strData, DT_ATTACK_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_ATTACK_DATA(root, data);
}
INT32 EncodeDT_ATTACK_ALL_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_ALL_DATA *pstIn = (DT_ATTACK_ALL_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAttackerIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wAttackSkillID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wAttackSkillLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byCritFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBeAttackerIdxNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstIn->byBeAttackerIdxNum)
			break;
		if(-1 == EncodeDT_PARRY_DATA(&pstIn->astBeAttackerIdxInfo[i], poNetData))
			return -1;
	}

	if(-1 == EncodeDT_ATTACK_DATA(&pstIn->stBeforeAttack, poNetData))
		return -1;

	if(-1 == EncodeDT_ATTACK_DATA(&pstIn->stAfterAttack, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ATTACK_ALL_DATA(void *pHost, CNetData* poNetData)
{
	DT_ATTACK_ALL_DATA *pstOut = (DT_ATTACK_ALL_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAttackerIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wAttackSkillID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wAttackSkillLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byCritFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBeAttackerIdxNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstOut->byBeAttackerIdxNum)
			break;
		if(-1 == DecodeDT_PARRY_DATA(&pstOut->astBeAttackerIdxInfo[i], poNetData))
			return -1;
	}

	if(-1 == DecodeDT_ATTACK_DATA(&pstOut->stBeforeAttack, poNetData))
		return -1;

	if(-1 == DecodeDT_ATTACK_DATA(&pstOut->stAfterAttack, poNetData))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_ATTACK_ALL_DATA(DT_ATTACK_ALL_DATA &data)
{
	Json::Value root;
	root["AttackerIdx"]=data.byAttackerIdx;
	root["AttackSkillID"]=data.wAttackSkillID;
	root["AttackSkillLevel"]=data.wAttackSkillLevel;
	root["CritFlag"]=data.byCritFlag;
	root["BeAttackerIdxNum"]=data.byBeAttackerIdxNum;
	for( UINT32 dwIndex=0; dwIndex<data.byBeAttackerIdxNum; ++dwIndex)
	{
	root["BeAttackerIdxInfo"].append( GetJsonDT_PARRY_DATA(data.astBeAttackerIdxInfo[dwIndex]) );
	}
	root["BeforeAttack"]=GetJsonDT_ATTACK_DATA(data.stBeforeAttack);
	root["AfterAttack"]=GetJsonDT_ATTACK_DATA(data.stAfterAttack);

	return root;
}
BOOL FromJsonDT_ATTACK_ALL_DATA(Json::Value &root, DT_ATTACK_ALL_DATA &data)
{
	data.byAttackerIdx=root["AttackerIdx"].asUInt();
	data.wAttackSkillID=root["AttackSkillID"].asUInt();
	data.wAttackSkillLevel=root["AttackSkillLevel"].asUInt();
	data.byCritFlag=root["CritFlag"].asUInt();
	data.byBeAttackerIdxNum=root["BeAttackerIdxNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byBeAttackerIdxNum; ++dwIndex)
	{
		FromJsonDT_PARRY_DATA(root["BeAttackerIdxInfo"][dwIndex] , data.astBeAttackerIdxInfo[dwIndex] );
	}	FromJsonDT_ATTACK_DATA(root["BeforeAttack"] , data.stBeforeAttack);
	FromJsonDT_ATTACK_DATA(root["AfterAttack"] , data.stAfterAttack);

	return TRUE;
}
BOOL FromJsonDT_ATTACK_ALL_DATA(const string &strData, DT_ATTACK_ALL_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_ATTACK_ALL_DATA(root, data);
}
INT32 EncodeDT_BATTLE_ANGRY_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_ANGRY_DATA *pstIn = (DT_BATTLE_ANGRY_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->bySlot))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCurAngry))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BATTLE_ANGRY_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_ANGRY_DATA *pstOut = (DT_BATTLE_ANGRY_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->bySlot))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCurAngry))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_BATTLE_ANGRY_DATA(DT_BATTLE_ANGRY_DATA &data)
{
	Json::Value root;
	root["Slot"]=data.bySlot;
	root["CurAngry"]=data.wCurAngry;

	return root;
}
BOOL FromJsonDT_BATTLE_ANGRY_DATA(Json::Value &root, DT_BATTLE_ANGRY_DATA &data)
{
	data.bySlot=root["Slot"].asUInt();
	data.wCurAngry=root["CurAngry"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_BATTLE_ANGRY_DATA(const string &strData, DT_BATTLE_ANGRY_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_BATTLE_ANGRY_DATA(root, data);
}
INT32 EncodeDT_BATTLE_ROUND_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_ROUND_DATA *pstIn = (DT_BATTLE_ROUND_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byAttackerFlag))
		return -1;

	if(-1 == EncodeDT_ATTACK_ALL_DATA(&pstIn->stAttackData, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BATTLE_ROUND_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_ROUND_DATA *pstOut = (DT_BATTLE_ROUND_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byAttackerFlag))
		return -1;

	if(-1 == DecodeDT_ATTACK_ALL_DATA(&pstOut->stAttackData, poNetData))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_BATTLE_ROUND_DATA(DT_BATTLE_ROUND_DATA &data)
{
	Json::Value root;
	root["AttackerFlag"]=data.byAttackerFlag;
	root["AttackData"]=GetJsonDT_ATTACK_ALL_DATA(data.stAttackData);

	return root;
}
BOOL FromJsonDT_BATTLE_ROUND_DATA(Json::Value &root, DT_BATTLE_ROUND_DATA &data)
{
	data.byAttackerFlag=root["AttackerFlag"].asUInt();
	FromJsonDT_ATTACK_ALL_DATA(root["AttackData"] , data.stAttackData);

	return TRUE;
}
BOOL FromJsonDT_BATTLE_ROUND_DATA(const string &strData, DT_BATTLE_ROUND_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_BATTLE_ROUND_DATA(root, data);
}
INT32 EncodeDT_BATTLE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_DATA *pstIn = (DT_BATTLE_DATA*)pHost;

	if(-1 == EncodeDT_BATTLE_OBJ_GROUP_DATA(&pstIn->stMyselfInfo, poNetData))
		return -1;

	if(-1 == EncodeDT_BATTLE_OBJ_GROUP_DATA(&pstIn->stEnemyInfo, poNetData))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBattleResult))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byScore))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBattleRoundNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BATTLE_ROUND_NUM; i++)
	{
		if(i >= (INT32)pstIn->byBattleRoundNum)
			break;
		if(-1 == EncodeDT_BATTLE_ROUND_DATA(&pstIn->astBattleRoundInfo[i], poNetData))
			return -1;
	}

	if(-1 == EncodeDT_BATTLE_ATTRIBUTE(&pstIn->stMyselfInfoAfterBattle, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_BATTLE_DATA(void *pHost, CNetData* poNetData)
{
	DT_BATTLE_DATA *pstOut = (DT_BATTLE_DATA*)pHost;

	if(-1 == DecodeDT_BATTLE_OBJ_GROUP_DATA(&pstOut->stMyselfInfo, poNetData))
		return -1;

	if(-1 == DecodeDT_BATTLE_OBJ_GROUP_DATA(&pstOut->stEnemyInfo, poNetData))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBattleResult))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byScore))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBattleRoundNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_BATTLE_ROUND_NUM; i++)
	{
		if(i >= (INT32)pstOut->byBattleRoundNum)
			break;
		if(-1 == DecodeDT_BATTLE_ROUND_DATA(&pstOut->astBattleRoundInfo[i], poNetData))
			return -1;
	}

	if(-1 == DecodeDT_BATTLE_ATTRIBUTE(&pstOut->stMyselfInfoAfterBattle, poNetData))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_BATTLE_DATA(DT_BATTLE_DATA &data)
{
	Json::Value root;
	root["MyselfInfo"]=GetJsonDT_BATTLE_OBJ_GROUP_DATA(data.stMyselfInfo);
	root["EnemyInfo"]=GetJsonDT_BATTLE_OBJ_GROUP_DATA(data.stEnemyInfo);
	root["BattleResult"]=data.byBattleResult;
	root["Score"]=data.byScore;
	root["BattleRoundNum"]=data.byBattleRoundNum;
	for( UINT32 dwIndex=0; dwIndex<data.byBattleRoundNum; ++dwIndex)
	{
	root["BattleRoundInfo"].append( GetJsonDT_BATTLE_ROUND_DATA(data.astBattleRoundInfo[dwIndex]) );
	}
	root["MyselfInfoAfterBattle"]=GetJsonDT_BATTLE_ATTRIBUTE(data.stMyselfInfoAfterBattle);

	return root;
}
BOOL FromJsonDT_BATTLE_DATA(Json::Value &root, DT_BATTLE_DATA &data)
{
	FromJsonDT_BATTLE_OBJ_GROUP_DATA(root["MyselfInfo"] , data.stMyselfInfo);
	FromJsonDT_BATTLE_OBJ_GROUP_DATA(root["EnemyInfo"] , data.stEnemyInfo);
	data.byBattleResult=root["BattleResult"].asUInt();
	data.byScore=root["Score"].asUInt();
	data.byBattleRoundNum=root["BattleRoundNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byBattleRoundNum; ++dwIndex)
	{
		FromJsonDT_BATTLE_ROUND_DATA(root["BattleRoundInfo"][dwIndex] , data.astBattleRoundInfo[dwIndex] );
	}	FromJsonDT_BATTLE_ATTRIBUTE(root["MyselfInfoAfterBattle"] , data.stMyselfInfoAfterBattle);

	return TRUE;
}
BOOL FromJsonDT_BATTLE_DATA(const string &strData, DT_BATTLE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_BATTLE_DATA(root, data);
}
INT32 EncodeDT_ONLINE_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData)
{
	DT_ONLINE_ENCOURAGE_RECORD_DATA *pstIn = (DT_ONLINE_ENCOURAGE_RECORD_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byRecvDays))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastRecvTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLastRecvIndex))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ONLINE_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData)
{
	DT_ONLINE_ENCOURAGE_RECORD_DATA *pstOut = (DT_ONLINE_ENCOURAGE_RECORD_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byRecvDays))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastRecvTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLastRecvIndex))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_ONLINE_ENCOURAGE_RECORD_DATA(DT_ONLINE_ENCOURAGE_RECORD_DATA &data)
{
	Json::Value root;
	root["RecvDays"]=data.byRecvDays;
	ostringstream ssqwLastRecvTime;
	ssqwLastRecvTime<<data.qwLastRecvTime;
	root["LastRecvTime"]=ssqwLastRecvTime.str();
	root["LastRecvIndex"]=data.byLastRecvIndex;

	return root;
}
BOOL FromJsonDT_ONLINE_ENCOURAGE_RECORD_DATA(Json::Value &root, DT_ONLINE_ENCOURAGE_RECORD_DATA &data)
{
	data.byRecvDays=root["RecvDays"].asUInt();
	data.qwLastRecvTime=SDAtou64( root["LastRecvTime"].asCString() );
	data.byLastRecvIndex=root["LastRecvIndex"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_ONLINE_ENCOURAGE_RECORD_DATA(const string &strData, DT_ONLINE_ENCOURAGE_RECORD_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_ONLINE_ENCOURAGE_RECORD_DATA(root, data);
}
INT32 EncodeDT_LOGIN_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData)
{
	DT_LOGIN_ENCOURAGE_RECORD_DATA *pstIn = (DT_LOGIN_ENCOURAGE_RECORD_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wKeepDays))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLastRecvNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastRecvTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLastSelectIndex))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byStateFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_LOGIN_ENCOURAGE_RECORD_DATA(void *pHost, CNetData* poNetData)
{
	DT_LOGIN_ENCOURAGE_RECORD_DATA *pstOut = (DT_LOGIN_ENCOURAGE_RECORD_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wKeepDays))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLastRecvNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastRecvTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLastSelectIndex))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byStateFlag))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_LOGIN_ENCOURAGE_RECORD_DATA(DT_LOGIN_ENCOURAGE_RECORD_DATA &data)
{
	Json::Value root;
	root["KeepDays"]=data.wKeepDays;
	root["LastRecvNum"]=data.byLastRecvNum;
	ostringstream ssqwLastRecvTime;
	ssqwLastRecvTime<<data.qwLastRecvTime;
	root["LastRecvTime"]=ssqwLastRecvTime.str();
	root["LastSelectIndex"]=data.byLastSelectIndex;
	root["StateFlag"]=data.byStateFlag;

	return root;
}
BOOL FromJsonDT_LOGIN_ENCOURAGE_RECORD_DATA(Json::Value &root, DT_LOGIN_ENCOURAGE_RECORD_DATA &data)
{
	data.wKeepDays=root["KeepDays"].asUInt();
	data.byLastRecvNum=root["LastRecvNum"].asUInt();
	data.qwLastRecvTime=SDAtou64( root["LastRecvTime"].asCString() );
	data.byLastSelectIndex=root["LastSelectIndex"].asUInt();
	data.byStateFlag=root["StateFlag"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_LOGIN_ENCOURAGE_RECORD_DATA(const string &strData, DT_LOGIN_ENCOURAGE_RECORD_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_LOGIN_ENCOURAGE_RECORD_DATA(root, data);
}
INT32 EncodeDT_RACE_BATTLELOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_RACE_BATTLELOG_DATA *pstIn = (DT_RACE_BATTLELOG_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byChallengeFlag))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwChallengeTime))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwEnemyPlayerID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byResult))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwBeginChallengeRank))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwAfterChallengeRank))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNewFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_RACE_BATTLELOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_RACE_BATTLELOG_DATA *pstOut = (DT_RACE_BATTLELOG_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byChallengeFlag))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwChallengeTime))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwEnemyPlayerID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byResult))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwBeginChallengeRank))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwAfterChallengeRank))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNewFlag))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_RACE_BATTLELOG_DATA(DT_RACE_BATTLELOG_DATA &data)
{
	Json::Value root;
	root["ChallengeFlag"]=data.byChallengeFlag;
	ostringstream ssqwChallengeTime;
	ssqwChallengeTime<<data.qwChallengeTime;
	root["ChallengeTime"]=ssqwChallengeTime.str();
	root["EnemyPlayerID"]=data.dwEnemyPlayerID;
	root["Result"]=data.byResult;
	root["BeginChallengeRank"]=data.dwBeginChallengeRank;
	root["AfterChallengeRank"]=data.dwAfterChallengeRank;
	root["NewFlag"]=data.byNewFlag;

	return root;
}
BOOL FromJsonDT_RACE_BATTLELOG_DATA(Json::Value &root, DT_RACE_BATTLELOG_DATA &data)
{
	data.byChallengeFlag=root["ChallengeFlag"].asUInt();
	data.qwChallengeTime=SDAtou64( root["ChallengeTime"].asCString() );
	data.dwEnemyPlayerID=root["EnemyPlayerID"].asUInt();
	data.byResult=root["Result"].asUInt();
	data.dwBeginChallengeRank=root["BeginChallengeRank"].asUInt();
	data.dwAfterChallengeRank=root["AfterChallengeRank"].asUInt();
	data.byNewFlag=root["NewFlag"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_RACE_BATTLELOG_DATA(const string &strData, DT_RACE_BATTLELOG_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_RACE_BATTLELOG_DATA(root, data);
}
INT32 EncodeDT_RACE_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_RACE_BATTLELOG_DATA_LIST *pstIn = (DT_RACE_BATTLELOG_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byBattleLogNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_RACE_BATTLELOG_NUM; i++)
	{
		if(i >= (INT32)pstIn->byBattleLogNum)
			break;
		if(-1 == EncodeDT_RACE_BATTLELOG_DATA(&pstIn->astBattleLogInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_RACE_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_RACE_BATTLELOG_DATA_LIST *pstOut = (DT_RACE_BATTLELOG_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byBattleLogNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_RACE_BATTLELOG_NUM; i++)
	{
		if(i >= (INT32)pstOut->byBattleLogNum)
			break;
		if(-1 == DecodeDT_RACE_BATTLELOG_DATA(&pstOut->astBattleLogInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_RACE_BATTLELOG_DATA_LIST(DT_RACE_BATTLELOG_DATA_LIST &data)
{
	Json::Value root;
	root["BattleLogNum"]=data.byBattleLogNum;
	for( UINT32 dwIndex=0; dwIndex<data.byBattleLogNum; ++dwIndex)
	{
	root["BattleLogInfo"].append( GetJsonDT_RACE_BATTLELOG_DATA(data.astBattleLogInfo[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_RACE_BATTLELOG_DATA_LIST(Json::Value &root, DT_RACE_BATTLELOG_DATA_LIST &data)
{
	data.byBattleLogNum=root["BattleLogNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byBattleLogNum; ++dwIndex)
	{
		FromJsonDT_RACE_BATTLELOG_DATA(root["BattleLogInfo"][dwIndex] , data.astBattleLogInfo[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_RACE_BATTLELOG_DATA_LIST(const string &strData, DT_RACE_BATTLELOG_DATA_LIST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_RACE_BATTLELOG_DATA_LIST(root, data);
}
INT32 EncodeDT_CAPTURE_DATA(void *pHost, CNetData* poNetData)
{
	DT_CAPTURE_DATA *pstIn = (DT_CAPTURE_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwCapturePlayerID))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwCaptureTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byCaptureGridIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byIncomeType))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastCollectTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CAPTURE_DATA(void *pHost, CNetData* poNetData)
{
	DT_CAPTURE_DATA *pstOut = (DT_CAPTURE_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwCapturePlayerID))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwCaptureTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byCaptureGridIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byIncomeType))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastCollectTime))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_CAPTURE_DATA(DT_CAPTURE_DATA &data)
{
	Json::Value root;
	root["CapturePlayerID"]=data.dwCapturePlayerID;
	ostringstream ssqwCaptureTime;
	ssqwCaptureTime<<data.qwCaptureTime;
	root["CaptureTime"]=ssqwCaptureTime.str();
	root["CaptureGridIdx"]=data.byCaptureGridIdx;
	root["IncomeType"]=data.byIncomeType;
	ostringstream ssqwLastCollectTime;
	ssqwLastCollectTime<<data.qwLastCollectTime;
	root["LastCollectTime"]=ssqwLastCollectTime.str();

	return root;
}
BOOL FromJsonDT_CAPTURE_DATA(Json::Value &root, DT_CAPTURE_DATA &data)
{
	data.dwCapturePlayerID=root["CapturePlayerID"].asUInt();
	data.qwCaptureTime=SDAtou64( root["CaptureTime"].asCString() );
	data.byCaptureGridIdx=root["CaptureGridIdx"].asUInt();
	data.byIncomeType=root["IncomeType"].asUInt();
	data.qwLastCollectTime=SDAtou64( root["LastCollectTime"].asCString() );

	return TRUE;
}
BOOL FromJsonDT_CAPTURE_DATA(const string &strData, DT_CAPTURE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_CAPTURE_DATA(root, data);
}
INT32 EncodeDT_CAPTURE_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_CAPTURE_DATA_LIST *pstIn = (DT_CAPTURE_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byCaptureNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_CAPTURE_NUM; i++)
	{
		if(i >= (INT32)pstIn->byCaptureNum)
			break;
		if(-1 == EncodeDT_CAPTURE_DATA(&pstIn->astCaptureInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CAPTURE_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_CAPTURE_DATA_LIST *pstOut = (DT_CAPTURE_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byCaptureNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_CAPTURE_NUM; i++)
	{
		if(i >= (INT32)pstOut->byCaptureNum)
			break;
		if(-1 == DecodeDT_CAPTURE_DATA(&pstOut->astCaptureInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_CAPTURE_DATA_LIST(DT_CAPTURE_DATA_LIST &data)
{
	Json::Value root;
	root["CaptureNum"]=data.byCaptureNum;
	for( UINT32 dwIndex=0; dwIndex<data.byCaptureNum; ++dwIndex)
	{
	root["CaptureInfo"].append( GetJsonDT_CAPTURE_DATA(data.astCaptureInfo[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_CAPTURE_DATA_LIST(Json::Value &root, DT_CAPTURE_DATA_LIST &data)
{
	data.byCaptureNum=root["CaptureNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byCaptureNum; ++dwIndex)
	{
		FromJsonDT_CAPTURE_DATA(root["CaptureInfo"][dwIndex] , data.astCaptureInfo[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_CAPTURE_DATA_LIST(const string &strData, DT_CAPTURE_DATA_LIST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_CAPTURE_DATA_LIST(root, data);
}
INT32 EncodeDT_PLUNDER_BATTLELOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_BATTLELOG_DATA *pstIn = (DT_PLUNDER_BATTLELOG_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byPlunderFlag))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwPlunderTime))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwActivePlayerID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwPassivePlayerID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwCapturePlayerID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byResult))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwPlunderCoin))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwPlunderScience))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byNewFlag))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLUNDER_BATTLELOG_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_BATTLELOG_DATA *pstOut = (DT_PLUNDER_BATTLELOG_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byPlunderFlag))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwPlunderTime))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwActivePlayerID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwPassivePlayerID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwCapturePlayerID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byResult))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwPlunderCoin))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwPlunderScience))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byNewFlag))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_PLUNDER_BATTLELOG_DATA(DT_PLUNDER_BATTLELOG_DATA &data)
{
	Json::Value root;
	root["PlunderFlag"]=data.byPlunderFlag;
	ostringstream ssqwPlunderTime;
	ssqwPlunderTime<<data.qwPlunderTime;
	root["PlunderTime"]=ssqwPlunderTime.str();
	root["ActivePlayerID"]=data.dwActivePlayerID;
	root["PassivePlayerID"]=data.dwPassivePlayerID;
	root["CapturePlayerID"]=data.dwCapturePlayerID;
	root["Result"]=data.byResult;
	ostringstream ssqwPlunderCoin;
	ssqwPlunderCoin<<data.qwPlunderCoin;
	root["PlunderCoin"]=ssqwPlunderCoin.str();
	ostringstream ssqwPlunderScience;
	ssqwPlunderScience<<data.qwPlunderScience;
	root["PlunderScience"]=ssqwPlunderScience.str();
	root["NewFlag"]=data.byNewFlag;

	return root;
}
BOOL FromJsonDT_PLUNDER_BATTLELOG_DATA(Json::Value &root, DT_PLUNDER_BATTLELOG_DATA &data)
{
	data.byPlunderFlag=root["PlunderFlag"].asUInt();
	data.qwPlunderTime=SDAtou64( root["PlunderTime"].asCString() );
	data.dwActivePlayerID=root["ActivePlayerID"].asUInt();
	data.dwPassivePlayerID=root["PassivePlayerID"].asUInt();
	data.dwCapturePlayerID=root["CapturePlayerID"].asUInt();
	data.byResult=root["Result"].asUInt();
	data.qwPlunderCoin=SDAtou64( root["PlunderCoin"].asCString() );
	data.qwPlunderScience=SDAtou64( root["PlunderScience"].asCString() );
	data.byNewFlag=root["NewFlag"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_PLUNDER_BATTLELOG_DATA(const string &strData, DT_PLUNDER_BATTLELOG_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_PLUNDER_BATTLELOG_DATA(root, data);
}
INT32 EncodeDT_PLUNDER_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_BATTLELOG_DATA_LIST *pstIn = (DT_PLUNDER_BATTLELOG_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byBattleLogNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_PLUNDER_BATTLELOG_NUM; i++)
	{
		if(i >= (INT32)pstIn->byBattleLogNum)
			break;
		if(-1 == EncodeDT_PLUNDER_BATTLELOG_DATA(&pstIn->astBattleLogInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLUNDER_BATTLELOG_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_BATTLELOG_DATA_LIST *pstOut = (DT_PLUNDER_BATTLELOG_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byBattleLogNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_PLUNDER_BATTLELOG_NUM; i++)
	{
		if(i >= (INT32)pstOut->byBattleLogNum)
			break;
		if(-1 == DecodeDT_PLUNDER_BATTLELOG_DATA(&pstOut->astBattleLogInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_PLUNDER_BATTLELOG_DATA_LIST(DT_PLUNDER_BATTLELOG_DATA_LIST &data)
{
	Json::Value root;
	root["BattleLogNum"]=data.byBattleLogNum;
	for( UINT32 dwIndex=0; dwIndex<data.byBattleLogNum; ++dwIndex)
	{
	root["BattleLogInfo"].append( GetJsonDT_PLUNDER_BATTLELOG_DATA(data.astBattleLogInfo[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_PLUNDER_BATTLELOG_DATA_LIST(Json::Value &root, DT_PLUNDER_BATTLELOG_DATA_LIST &data)
{
	data.byBattleLogNum=root["BattleLogNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byBattleLogNum; ++dwIndex)
	{
		FromJsonDT_PLUNDER_BATTLELOG_DATA(root["BattleLogInfo"][dwIndex] , data.astBattleLogInfo[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_PLUNDER_BATTLELOG_DATA_LIST(const string &strData, DT_PLUNDER_BATTLELOG_DATA_LIST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_PLUNDER_BATTLELOG_DATA_LIST(root, data);
}
INT32 EncodeDT_PLUNDER_GRID_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_GRID_DATA *pstIn = (DT_PLUNDER_GRID_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byIncomeType))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwIncomeValue))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLUNDER_GRID_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_GRID_DATA *pstOut = (DT_PLUNDER_GRID_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byIncomeType))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwIncomeValue))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_PLUNDER_GRID_DATA(DT_PLUNDER_GRID_DATA &data)
{
	Json::Value root;
	root["IncomeType"]=data.byIncomeType;
	root["IncomeValue"]=data.dwIncomeValue;

	return root;
}
BOOL FromJsonDT_PLUNDER_GRID_DATA(Json::Value &root, DT_PLUNDER_GRID_DATA &data)
{
	data.byIncomeType=root["IncomeType"].asUInt();
	data.dwIncomeValue=root["IncomeValue"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_PLUNDER_GRID_DATA(const string &strData, DT_PLUNDER_GRID_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_PLUNDER_GRID_DATA(root, data);
}
INT32 EncodeDT_PLUNDER_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_BASE_DATA *pstIn = (DT_PLUNDER_BASE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwCaptureOwnerPlayerID))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwBeCaptureTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wExtPlunderNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastPlunderTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastPlunderNum))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUnreadBattleLogNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastDriveAwayTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastDriveAwayNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_CAPTURE_NUM; i++)
	{
		if(-1 == EncodeDT_PLUNDER_GRID_DATA(&pstIn->astCaptureGridInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLUNDER_BASE_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLUNDER_BASE_DATA *pstOut = (DT_PLUNDER_BASE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwCaptureOwnerPlayerID))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwBeCaptureTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wExtPlunderNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastPlunderTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastPlunderNum))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUnreadBattleLogNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastDriveAwayTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastDriveAwayNum))
		return -1;

	INT32 i;
		for(i = 0; i < MAX_CAPTURE_NUM; i++)
		{
			if(-1 == DecodeDT_PLUNDER_GRID_DATA(&pstOut->astCaptureGridInfo[i], poNetData))
				return -1;
		}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_PLUNDER_BASE_DATA(DT_PLUNDER_BASE_DATA &data)
{
	Json::Value root;
	root["HaveDataFlag"]=data.byHaveDataFlag;
	root["CaptureOwnerPlayerID"]=data.dwCaptureOwnerPlayerID;
	ostringstream ssqwBeCaptureTime;
	ssqwBeCaptureTime<<data.qwBeCaptureTime;
	root["BeCaptureTime"]=ssqwBeCaptureTime.str();
	root["ExtPlunderNum"]=data.wExtPlunderNum;
	ostringstream ssqwLastPlunderTime;
	ssqwLastPlunderTime<<data.qwLastPlunderTime;
	root["LastPlunderTime"]=ssqwLastPlunderTime.str();
	root["LastPlunderNum"]=data.wLastPlunderNum;
	root["UnreadBattleLogNum"]=data.byUnreadBattleLogNum;
	ostringstream ssqwLastDriveAwayTime;
	ssqwLastDriveAwayTime<<data.qwLastDriveAwayTime;
	root["LastDriveAwayTime"]=ssqwLastDriveAwayTime.str();
	root["LastDriveAwayNum"]=data.wLastDriveAwayNum;
	for( UINT32 dwIndex=0; dwIndex<MAX_CAPTURE_NUM; ++dwIndex)
	{
	root["CaptureGridInfo"].append( GetJsonDT_PLUNDER_GRID_DATA(data.astCaptureGridInfo[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_PLUNDER_BASE_DATA(Json::Value &root, DT_PLUNDER_BASE_DATA &data)
{
	data.byHaveDataFlag=root["HaveDataFlag"].asUInt();
	data.dwCaptureOwnerPlayerID=root["CaptureOwnerPlayerID"].asUInt();
	data.qwBeCaptureTime=SDAtou64( root["BeCaptureTime"].asCString() );
	data.wExtPlunderNum=root["ExtPlunderNum"].asUInt();
	data.qwLastPlunderTime=SDAtou64( root["LastPlunderTime"].asCString() );
	data.wLastPlunderNum=root["LastPlunderNum"].asUInt();
	data.byUnreadBattleLogNum=root["UnreadBattleLogNum"].asUInt();
	data.qwLastDriveAwayTime=SDAtou64( root["LastDriveAwayTime"].asCString() );
	data.wLastDriveAwayNum=root["LastDriveAwayNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<MAX_CAPTURE_NUM; ++dwIndex)
	{
		FromJsonDT_PLUNDER_GRID_DATA(root["CaptureGridInfo"][dwIndex] , data.astCaptureGridInfo[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_PLUNDER_BASE_DATA(const string &strData, DT_PLUNDER_BASE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_PLUNDER_BASE_DATA(root, data);
}
INT32 EncodeDT_ENEMY_DATA(void *pHost, CNetData* poNetData)
{
	DT_ENEMY_DATA *pstIn = (DT_ENEMY_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwEnemyPlayerID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wBattleNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwAddTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ENEMY_DATA(void *pHost, CNetData* poNetData)
{
	DT_ENEMY_DATA *pstOut = (DT_ENEMY_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwEnemyPlayerID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wBattleNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwAddTime))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_ENEMY_DATA(DT_ENEMY_DATA &data)
{
	Json::Value root;
	root["EnemyPlayerID"]=data.dwEnemyPlayerID;
	root["BattleNum"]=data.wBattleNum;
	ostringstream ssqwAddTime;
	ssqwAddTime<<data.qwAddTime;
	root["AddTime"]=ssqwAddTime.str();

	return root;
}
BOOL FromJsonDT_ENEMY_DATA(Json::Value &root, DT_ENEMY_DATA &data)
{
	data.dwEnemyPlayerID=root["EnemyPlayerID"].asUInt();
	data.wBattleNum=root["BattleNum"].asUInt();
	data.qwAddTime=SDAtou64( root["AddTime"].asCString() );

	return TRUE;
}
BOOL FromJsonDT_ENEMY_DATA(const string &strData, DT_ENEMY_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_ENEMY_DATA(root, data);
}
INT32 EncodeDT_ENEMY_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_ENEMY_DATA_LIST *pstIn = (DT_ENEMY_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byEnemyNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ENEMY_NUM; i++)
	{
		if(i >= (INT32)pstIn->byEnemyNum)
			break;
		if(-1 == EncodeDT_ENEMY_DATA(&pstIn->astEnemyInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ENEMY_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_ENEMY_DATA_LIST *pstOut = (DT_ENEMY_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byEnemyNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ENEMY_NUM; i++)
	{
		if(i >= (INT32)pstOut->byEnemyNum)
			break;
		if(-1 == DecodeDT_ENEMY_DATA(&pstOut->astEnemyInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_ENEMY_DATA_LIST(DT_ENEMY_DATA_LIST &data)
{
	Json::Value root;
	root["EnemyNum"]=data.byEnemyNum;
	for( UINT32 dwIndex=0; dwIndex<data.byEnemyNum; ++dwIndex)
	{
	root["EnemyInfo"].append( GetJsonDT_ENEMY_DATA(data.astEnemyInfo[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_ENEMY_DATA_LIST(Json::Value &root, DT_ENEMY_DATA_LIST &data)
{
	data.byEnemyNum=root["EnemyNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byEnemyNum; ++dwIndex)
	{
		FromJsonDT_ENEMY_DATA(root["EnemyInfo"][dwIndex] , data.astEnemyInfo[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_ENEMY_DATA_LIST(const string &strData, DT_ENEMY_DATA_LIST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_ENEMY_DATA_LIST(root, data);
}
INT32 EncodeDT_MONEY_TREE_DATA(void *pHost, CNetData* poNetData)
{
	DT_MONEY_TREE_DATA *pstIn = (DT_MONEY_TREE_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wRockNum))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwRockTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byMoodValue))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwMoodStartTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_MONEY_TREE_DATA(void *pHost, CNetData* poNetData)
{
	DT_MONEY_TREE_DATA *pstOut = (DT_MONEY_TREE_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byHaveDataFlag))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wRockNum))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwRockTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byMoodValue))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwMoodStartTime))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_MONEY_TREE_DATA(DT_MONEY_TREE_DATA &data)
{
	Json::Value root;
	root["HaveDataFlag"]=data.byHaveDataFlag;
	root["RockNum"]=data.wRockNum;
	ostringstream ssqwRockTime;
	ssqwRockTime<<data.qwRockTime;
	root["RockTime"]=ssqwRockTime.str();
	root["MoodValue"]=data.byMoodValue;
	ostringstream ssqwMoodStartTime;
	ssqwMoodStartTime<<data.qwMoodStartTime;
	root["MoodStartTime"]=ssqwMoodStartTime.str();

	return root;
}
BOOL FromJsonDT_MONEY_TREE_DATA(Json::Value &root, DT_MONEY_TREE_DATA &data)
{
	data.byHaveDataFlag=root["HaveDataFlag"].asUInt();
	data.wRockNum=root["RockNum"].asUInt();
	data.qwRockTime=SDAtou64( root["RockTime"].asCString() );
	data.byMoodValue=root["MoodValue"].asUInt();
	data.qwMoodStartTime=SDAtou64( root["MoodStartTime"].asCString() );

	return TRUE;
}
BOOL FromJsonDT_MONEY_TREE_DATA(const string &strData, DT_MONEY_TREE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_MONEY_TREE_DATA(root, data);
}
INT32 EncodeDT_STUDY_DATA(void *pHost, CNetData* poNetData)
{
	DT_STUDY_DATA *pstIn = (DT_STUDY_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byGridIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHeroKindID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byKind))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byColor))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLevel))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STUDY_DATA(void *pHost, CNetData* poNetData)
{
	DT_STUDY_DATA *pstOut = (DT_STUDY_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byGridIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHeroKindID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byKind))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byColor))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLevel))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_STUDY_DATA(DT_STUDY_DATA &data)
{
	Json::Value root;
	root["ID"]=data.wID;
	root["GridIdx"]=data.byGridIdx;
	root["HeroKindID"]=data.wHeroKindID;
	root["Kind"]=data.byKind;
	root["Color"]=data.byColor;
	root["Level"]=data.byLevel;

	return root;
}
BOOL FromJsonDT_STUDY_DATA(Json::Value &root, DT_STUDY_DATA &data)
{
	data.wID=root["ID"].asUInt();
	data.byGridIdx=root["GridIdx"].asUInt();
	data.wHeroKindID=root["HeroKindID"].asUInt();
	data.byKind=root["Kind"].asUInt();
	data.byColor=root["Color"].asUInt();
	data.byLevel=root["Level"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_STUDY_DATA(const string &strData, DT_STUDY_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_STUDY_DATA(root, data);
}
INT32 EncodeDT_STUDY_CLIP_DATA(void *pHost, CNetData* poNetData)
{
	DT_STUDY_CLIP_DATA *pstIn = (DT_STUDY_CLIP_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byGridIdx))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPickupFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byColor))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_STUDY_CLIP_DATA(void *pHost, CNetData* poNetData)
{
	DT_STUDY_CLIP_DATA *pstOut = (DT_STUDY_CLIP_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byGridIdx))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPickupFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byColor))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_STUDY_CLIP_DATA(DT_STUDY_CLIP_DATA &data)
{
	Json::Value root;
	root["ID"]=data.wID;
	root["GridIdx"]=data.byGridIdx;
	root["PickupFlag"]=data.byPickupFlag;
	root["Color"]=data.byColor;
	root["Num"]=data.wNum;

	return root;
}
BOOL FromJsonDT_STUDY_CLIP_DATA(Json::Value &root, DT_STUDY_CLIP_DATA &data)
{
	data.wID=root["ID"].asUInt();
	data.byGridIdx=root["GridIdx"].asUInt();
	data.byPickupFlag=root["PickupFlag"].asUInt();
	data.byColor=root["Color"].asUInt();
	data.wNum=root["Num"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_STUDY_CLIP_DATA(const string &strData, DT_STUDY_CLIP_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_STUDY_CLIP_DATA(root, data);
}
INT32 EncodeDT_AWAKEN_DATA(void *pHost, CNetData* poNetData)
{
	DT_AWAKEN_DATA *pstIn = (DT_AWAKEN_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAuto2ExpFlag))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwStudyExp))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_AWAKEN_DATA(void *pHost, CNetData* poNetData)
{
	DT_AWAKEN_DATA *pstOut = (DT_AWAKEN_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAuto2ExpFlag))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwStudyExp))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_AWAKEN_DATA(DT_AWAKEN_DATA &data)
{
	Json::Value root;
	root["Level"]=data.byLevel;
	root["Auto2ExpFlag"]=data.byAuto2ExpFlag;
	ostringstream ssqwStudyExp;
	ssqwStudyExp<<data.qwStudyExp;
	root["StudyExp"]=ssqwStudyExp.str();

	return root;
}
BOOL FromJsonDT_AWAKEN_DATA(Json::Value &root, DT_AWAKEN_DATA &data)
{
	data.byLevel=root["Level"].asUInt();
	data.byAuto2ExpFlag=root["Auto2ExpFlag"].asUInt();
	data.qwStudyExp=SDAtou64( root["StudyExp"].asCString() );

	return TRUE;
}
BOOL FromJsonDT_AWAKEN_DATA(const string &strData, DT_AWAKEN_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_AWAKEN_DATA(root, data);
}
INT32 EncodeDT_INSTANCE_BATTLE_DATA(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_BATTLE_DATA *pstIn = (DT_INSTANCE_BATTLE_DATA*)pHost;

	if(-1 == EncodeDT_INSTANCE_ID(&pstIn->stInstanceID, poNetData))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byBattleIndex))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwBattleTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_INSTANCE_BATTLE_DATA(void *pHost, CNetData* poNetData)
{
	DT_INSTANCE_BATTLE_DATA *pstOut = (DT_INSTANCE_BATTLE_DATA*)pHost;

	if(-1 == DecodeDT_INSTANCE_ID(&pstOut->stInstanceID, poNetData))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byBattleIndex))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwBattleTime))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_INSTANCE_BATTLE_DATA(DT_INSTANCE_BATTLE_DATA &data)
{
	Json::Value root;
	root["InstanceID"]=GetJsonDT_INSTANCE_ID(data.stInstanceID);
	root["BattleIndex"]=data.byBattleIndex;
	ostringstream ssqwBattleTime;
	ssqwBattleTime<<data.qwBattleTime;
	root["BattleTime"]=ssqwBattleTime.str();

	return root;
}
BOOL FromJsonDT_INSTANCE_BATTLE_DATA(Json::Value &root, DT_INSTANCE_BATTLE_DATA &data)
{
	FromJsonDT_INSTANCE_ID(root["InstanceID"] , data.stInstanceID);
	data.byBattleIndex=root["BattleIndex"].asUInt();
	data.qwBattleTime=SDAtou64( root["BattleTime"].asCString() );

	return TRUE;
}
BOOL FromJsonDT_INSTANCE_BATTLE_DATA(const string &strData, DT_INSTANCE_BATTLE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_INSTANCE_BATTLE_DATA(root, data);
}
INT32 EncodeDT_PHYSTRENGTH_DATA(void *pHost, CNetData* poNetData)
{
	DT_PHYSTRENGTH_DATA *pstIn = (DT_PHYSTRENGTH_DATA*)pHost;

	if(-1 == poNetData->AddUint64(pstIn->qwLastPhyStrengthRefreshTime))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastBuyPhyStrengthTime))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLastBuyPhyStrengthNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PHYSTRENGTH_DATA(void *pHost, CNetData* poNetData)
{
	DT_PHYSTRENGTH_DATA *pstOut = (DT_PHYSTRENGTH_DATA*)pHost;

	if(-1 == poNetData->DelUint64(pstOut->qwLastPhyStrengthRefreshTime))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastBuyPhyStrengthTime))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLastBuyPhyStrengthNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_PHYSTRENGTH_DATA(DT_PHYSTRENGTH_DATA &data)
{
	Json::Value root;
	ostringstream ssqwLastPhyStrengthRefreshTime;
	ssqwLastPhyStrengthRefreshTime<<data.qwLastPhyStrengthRefreshTime;
	root["LastPhyStrengthRefreshTime"]=ssqwLastPhyStrengthRefreshTime.str();
	ostringstream ssqwLastBuyPhyStrengthTime;
	ssqwLastBuyPhyStrengthTime<<data.qwLastBuyPhyStrengthTime;
	root["LastBuyPhyStrengthTime"]=ssqwLastBuyPhyStrengthTime.str();
	root["LastBuyPhyStrengthNum"]=data.byLastBuyPhyStrengthNum;

	return root;
}
BOOL FromJsonDT_PHYSTRENGTH_DATA(Json::Value &root, DT_PHYSTRENGTH_DATA &data)
{
	data.qwLastPhyStrengthRefreshTime=SDAtou64( root["LastPhyStrengthRefreshTime"].asCString() );
	data.qwLastBuyPhyStrengthTime=SDAtou64( root["LastBuyPhyStrengthTime"].asCString() );
	data.byLastBuyPhyStrengthNum=root["LastBuyPhyStrengthNum"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_PHYSTRENGTH_DATA(const string &strData, DT_PHYSTRENGTH_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_PHYSTRENGTH_DATA(root, data);
}
INT32 EncodeDT_ONHOOK_RESULT(void *pHost, CNetData* poNetData)
{
	DT_ONHOOK_RESULT *pstIn = (DT_ONHOOK_RESULT*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byCurRunTimes))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byAllRunTimes))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwStory))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwExperience))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwCoin))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwGold))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byItemNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ENCOURAGE_ITEM_KIND_NUM; i++)
	{
		if(i >= (INT32)pstIn->byItemNum)
			break;
		if(-1 == EncodeDT_ITEM_DATA(&pstIn->astItemInfoList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ONHOOK_RESULT(void *pHost, CNetData* poNetData)
{
	DT_ONHOOK_RESULT *pstOut = (DT_ONHOOK_RESULT*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byCurRunTimes))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byAllRunTimes))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwStory))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwExperience))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwCoin))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwGold))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byItemNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_ENCOURAGE_ITEM_KIND_NUM; i++)
	{
		if(i >= (INT32)pstOut->byItemNum)
			break;
		if(-1 == DecodeDT_ITEM_DATA(&pstOut->astItemInfoList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_ONHOOK_RESULT(DT_ONHOOK_RESULT &data)
{
	Json::Value root;
	root["CurRunTimes"]=data.byCurRunTimes;
	root["AllRunTimes"]=data.byAllRunTimes;
	root["Story"]=data.dwStory;
	root["Experience"]=data.dwExperience;
	root["Coin"]=data.dwCoin;
	root["Gold"]=data.dwGold;
	root["ItemNum"]=data.byItemNum;
	for( UINT32 dwIndex=0; dwIndex<data.byItemNum; ++dwIndex)
	{
	root["ItemInfoList"].append( GetJsonDT_ITEM_DATA(data.astItemInfoList[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_ONHOOK_RESULT(Json::Value &root, DT_ONHOOK_RESULT &data)
{
	data.byCurRunTimes=root["CurRunTimes"].asUInt();
	data.byAllRunTimes=root["AllRunTimes"].asUInt();
	data.dwStory=root["Story"].asUInt();
	data.dwExperience=root["Experience"].asUInt();
	data.dwCoin=root["Coin"].asUInt();
	data.dwGold=root["Gold"].asUInt();
	data.byItemNum=root["ItemNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byItemNum; ++dwIndex)
	{
		FromJsonDT_ITEM_DATA(root["ItemInfoList"][dwIndex] , data.astItemInfoList[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_ONHOOK_RESULT(const string &strData, DT_ONHOOK_RESULT &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_ONHOOK_RESULT(root, data);
}
INT32 EncodeDT_ONHOOK_RCD_DATA(void *pHost, CNetData* poNetData)
{
	DT_ONHOOK_RCD_DATA *pstIn = (DT_ONHOOK_RCD_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byOnhookFlag))
		return -1;

	if(-1 == EncodeDT_ONHOOK_RESULT(&pstIn->stOnhookResult, poNetData))
		return -1;

	if(-1 == EncodeDT_INSTANCE_ID(&pstIn->stOnhookInstanceID, poNetData))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastOnhookResultReqTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_ONHOOK_RCD_DATA(void *pHost, CNetData* poNetData)
{
	DT_ONHOOK_RCD_DATA *pstOut = (DT_ONHOOK_RCD_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byOnhookFlag))
		return -1;

	if(-1 == DecodeDT_ONHOOK_RESULT(&pstOut->stOnhookResult, poNetData))
		return -1;

	if(-1 == DecodeDT_INSTANCE_ID(&pstOut->stOnhookInstanceID, poNetData))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastOnhookResultReqTime))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_ONHOOK_RCD_DATA(DT_ONHOOK_RCD_DATA &data)
{
	Json::Value root;
	root["OnhookFlag"]=data.byOnhookFlag;
	root["OnhookResult"]=GetJsonDT_ONHOOK_RESULT(data.stOnhookResult);
	root["OnhookInstanceID"]=GetJsonDT_INSTANCE_ID(data.stOnhookInstanceID);
	ostringstream ssqwLastOnhookResultReqTime;
	ssqwLastOnhookResultReqTime<<data.qwLastOnhookResultReqTime;
	root["LastOnhookResultReqTime"]=ssqwLastOnhookResultReqTime.str();

	return root;
}
BOOL FromJsonDT_ONHOOK_RCD_DATA(Json::Value &root, DT_ONHOOK_RCD_DATA &data)
{
	data.byOnhookFlag=root["OnhookFlag"].asUInt();
	FromJsonDT_ONHOOK_RESULT(root["OnhookResult"] , data.stOnhookResult);
	FromJsonDT_INSTANCE_ID(root["OnhookInstanceID"] , data.stOnhookInstanceID);
	data.qwLastOnhookResultReqTime=SDAtou64( root["LastOnhookResultReqTime"].asCString() );

	return TRUE;
}
BOOL FromJsonDT_ONHOOK_RCD_DATA(const string &strData, DT_ONHOOK_RCD_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_ONHOOK_RCD_DATA(root, data);
}
INT32 EncodeDT_FEW_PARAM_DATA(void *pHost, CNetData* poNetData)
{
	DT_FEW_PARAM_DATA *pstIn = (DT_FEW_PARAM_DATA*)pHost;

	if(-1 == poNetData->AddUint64(pstIn->qwLastStrengthenTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastStrengthenCD))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastRefreshEliteTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLastRefreshEliteNum))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FEW_PARAM_DATA(void *pHost, CNetData* poNetData)
{
	DT_FEW_PARAM_DATA *pstOut = (DT_FEW_PARAM_DATA*)pHost;

	if(-1 == poNetData->DelUint64(pstOut->qwLastStrengthenTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastStrengthenCD))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastRefreshEliteTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLastRefreshEliteNum))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_FEW_PARAM_DATA(DT_FEW_PARAM_DATA &data)
{
	Json::Value root;
	ostringstream ssqwLastStrengthenTime;
	ssqwLastStrengthenTime<<data.qwLastStrengthenTime;
	root["LastStrengthenTime"]=ssqwLastStrengthenTime.str();
	root["LastStrengthenCD"]=data.wLastStrengthenCD;
	ostringstream ssqwLastRefreshEliteTime;
	ssqwLastRefreshEliteTime<<data.qwLastRefreshEliteTime;
	root["LastRefreshEliteTime"]=ssqwLastRefreshEliteTime.str();
	root["LastRefreshEliteNum"]=data.wLastRefreshEliteNum;

	return root;
}
BOOL FromJsonDT_FEW_PARAM_DATA(Json::Value &root, DT_FEW_PARAM_DATA &data)
{
	data.qwLastStrengthenTime=SDAtou64( root["LastStrengthenTime"].asCString() );
	data.wLastStrengthenCD=root["LastStrengthenCD"].asUInt();
	data.qwLastRefreshEliteTime=SDAtou64( root["LastRefreshEliteTime"].asCString() );
	data.wLastRefreshEliteNum=root["LastRefreshEliteNum"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_FEW_PARAM_DATA(const string &strData, DT_FEW_PARAM_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_FEW_PARAM_DATA(root, data);
}
INT32 EncodeDT_CROP_INFO_DATA(void *pHost, CNetData* poNetData)
{
	DT_CROP_INFO_DATA *pstIn = (DT_CROP_INFO_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byOpen))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCropKindID))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwLastCropTime))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CROP_INFO_DATA(void *pHost, CNetData* poNetData)
{
	DT_CROP_INFO_DATA *pstOut = (DT_CROP_INFO_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byOpen))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCropKindID))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwLastCropTime))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_CROP_INFO_DATA(DT_CROP_INFO_DATA &data)
{
	Json::Value root;
	root["Open"]=data.byOpen;
	root["CropKindID"]=data.wCropKindID;
	ostringstream ssqwLastCropTime;
	ssqwLastCropTime<<data.qwLastCropTime;
	root["LastCropTime"]=ssqwLastCropTime.str();

	return root;
}
BOOL FromJsonDT_CROP_INFO_DATA(Json::Value &root, DT_CROP_INFO_DATA &data)
{
	data.byOpen=root["Open"].asUInt();
	data.wCropKindID=root["CropKindID"].asUInt();
	data.qwLastCropTime=SDAtou64( root["LastCropTime"].asCString() );

	return TRUE;
}
BOOL FromJsonDT_CROP_INFO_DATA(const string &strData, DT_CROP_INFO_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_CROP_INFO_DATA(root, data);
}
INT32 EncodeDT_CROP_DATA(void *pHost, CNetData* poNetData)
{
	DT_CROP_DATA *pstIn = (DT_CROP_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byHaveDataFlag))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_CROP_NUM; i++)
	{
		if(-1 == EncodeDT_CROP_INFO_DATA(&pstIn->astCropInfoList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_CROP_DATA(void *pHost, CNetData* poNetData)
{
	DT_CROP_DATA *pstOut = (DT_CROP_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byHaveDataFlag))
		return -1;

	INT32 i;
		for(i = 0; i < MAX_CROP_NUM; i++)
		{
			if(-1 == DecodeDT_CROP_INFO_DATA(&pstOut->astCropInfoList[i], poNetData))
				return -1;
		}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_CROP_DATA(DT_CROP_DATA &data)
{
	Json::Value root;
	root["HaveDataFlag"]=data.byHaveDataFlag;
	for( UINT32 dwIndex=0; dwIndex<MAX_CROP_NUM; ++dwIndex)
	{
	root["CropInfoList"].append( GetJsonDT_CROP_INFO_DATA(data.astCropInfoList[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_CROP_DATA(Json::Value &root, DT_CROP_DATA &data)
{
	data.byHaveDataFlag=root["HaveDataFlag"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<MAX_CROP_NUM; ++dwIndex)
	{
		FromJsonDT_CROP_INFO_DATA(root["CropInfoList"][dwIndex] , data.astCropInfoList[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_CROP_DATA(const string &strData, DT_CROP_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_CROP_DATA(root, data);
}
INT32 EncodeDT_SCIENCE_TREE_DATA(void *pHost, CNetData* poNetData)
{
	DT_SCIENCE_TREE_DATA *pstIn = (DT_SCIENCE_TREE_DATA*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wAttractLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHPLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCritLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wShieldLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHitLevel))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wDodgeLevel))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwFirstAttack))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wCoachTalent))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_SCIENCE_TREE_DATA(void *pHost, CNetData* poNetData)
{
	DT_SCIENCE_TREE_DATA *pstOut = (DT_SCIENCE_TREE_DATA*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wAttractLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHPLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCritLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wShieldLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHitLevel))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wDodgeLevel))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwFirstAttack))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wCoachTalent))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_SCIENCE_TREE_DATA(DT_SCIENCE_TREE_DATA &data)
{
	Json::Value root;
	root["AttractLevel"]=data.wAttractLevel;
	root["HPLevel"]=data.wHPLevel;
	root["CritLevel"]=data.wCritLevel;
	root["ShieldLevel"]=data.wShieldLevel;
	root["HitLevel"]=data.wHitLevel;
	root["DodgeLevel"]=data.wDodgeLevel;
	root["FirstAttack"]=data.dwFirstAttack;
	root["CoachTalent"]=data.wCoachTalent;

	return root;
}
BOOL FromJsonDT_SCIENCE_TREE_DATA(Json::Value &root, DT_SCIENCE_TREE_DATA &data)
{
	data.wAttractLevel=root["AttractLevel"].asUInt();
	data.wHPLevel=root["HPLevel"].asUInt();
	data.wCritLevel=root["CritLevel"].asUInt();
	data.wShieldLevel=root["ShieldLevel"].asUInt();
	data.wHitLevel=root["HitLevel"].asUInt();
	data.wDodgeLevel=root["DodgeLevel"].asUInt();
	data.dwFirstAttack=root["FirstAttack"].asUInt();
	data.wCoachTalent=root["CoachTalent"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_SCIENCE_TREE_DATA(const string &strData, DT_SCIENCE_TREE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_SCIENCE_TREE_DATA(root, data);
}
INT32 EncodeDT_FORMATION_HERO_DATA(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_HERO_DATA *pstIn = (DT_FORMATION_HERO_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationIdx))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wHeroID))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FORMATION_HERO_DATA(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_HERO_DATA *pstOut = (DT_FORMATION_HERO_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationIdx))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wHeroID))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_FORMATION_HERO_DATA(DT_FORMATION_HERO_DATA &data)
{
	Json::Value root;
	root["FormationIdx"]=data.byFormationIdx;
	root["HeroID"]=data.wHeroID;

	return root;
}
BOOL FromJsonDT_FORMATION_HERO_DATA(Json::Value &root, DT_FORMATION_HERO_DATA &data)
{
	data.byFormationIdx=root["FormationIdx"].asUInt();
	data.wHeroID=root["HeroID"].asUInt();

	return TRUE;
}
BOOL FromJsonDT_FORMATION_HERO_DATA(const string &strData, DT_FORMATION_HERO_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_FORMATION_HERO_DATA(root, data);
}
INT32 EncodeDT_FORMATION_IDX_DATA(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_IDX_DATA *pstIn = (DT_FORMATION_IDX_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationKind))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byOpenIdxNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstIn->byOpenIdxNum)
			break;
		if(-1 == EncodeDT_FORMATION_HERO_DATA(&pstIn->astOpenIdxInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FORMATION_IDX_DATA(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_IDX_DATA *pstOut = (DT_FORMATION_IDX_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationKind))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byOpenIdxNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_IDX_NUM; i++)
	{
		if(i >= (INT32)pstOut->byOpenIdxNum)
			break;
		if(-1 == DecodeDT_FORMATION_HERO_DATA(&pstOut->astOpenIdxInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_FORMATION_IDX_DATA(DT_FORMATION_IDX_DATA &data)
{
	Json::Value root;
	root["FormationKind"]=data.byFormationKind;
	root["OpenIdxNum"]=data.byOpenIdxNum;
	for( UINT32 dwIndex=0; dwIndex<data.byOpenIdxNum; ++dwIndex)
	{
	root["OpenIdxInfo"].append( GetJsonDT_FORMATION_HERO_DATA(data.astOpenIdxInfo[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_FORMATION_IDX_DATA(Json::Value &root, DT_FORMATION_IDX_DATA &data)
{
	data.byFormationKind=root["FormationKind"].asUInt();
	data.byOpenIdxNum=root["OpenIdxNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byOpenIdxNum; ++dwIndex)
	{
		FromJsonDT_FORMATION_HERO_DATA(root["OpenIdxInfo"][dwIndex] , data.astOpenIdxInfo[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_FORMATION_IDX_DATA(const string &strData, DT_FORMATION_IDX_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_FORMATION_IDX_DATA(root, data);
}
INT32 EncodeDT_FORMATION_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_DATA_LIST *pstIn = (DT_FORMATION_DATA_LIST*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFormationKindNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_NUM; i++)
	{
		if(i >= (INT32)pstIn->byFormationKindNum)
			break;
		if(-1 == EncodeDT_FORMATION_IDX_DATA(&pstIn->astFormationInfo[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FORMATION_DATA_LIST(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_DATA_LIST *pstOut = (DT_FORMATION_DATA_LIST*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFormationKindNum))
		return -1;

	INT32 i;
	for(i = 0; i < MAX_FORMATION_NUM; i++)
	{
		if(i >= (INT32)pstOut->byFormationKindNum)
			break;
		if(-1 == DecodeDT_FORMATION_IDX_DATA(&pstOut->astFormationInfo[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_FORMATION_DATA_LIST(DT_FORMATION_DATA_LIST &data)
{
	Json::Value root;
	root["FormationKindNum"]=data.byFormationKindNum;
	for( UINT32 dwIndex=0; dwIndex<data.byFormationKindNum; ++dwIndex)
	{
	root["FormationInfo"].append( GetJsonDT_FORMATION_IDX_DATA(data.astFormationInfo[dwIndex]) );
	}

	return root;
}
BOOL FromJsonDT_FORMATION_DATA_LIST(Json::Value &root, DT_FORMATION_DATA_LIST &data)
{
	data.byFormationKindNum=root["FormationKindNum"].asUInt();
	for( UINT32 dwIndex=0; dwIndex<data.byFormationKindNum; ++dwIndex)
	{
		FromJsonDT_FORMATION_IDX_DATA(root["FormationInfo"][dwIndex] , data.astFormationInfo[dwIndex] );
	}
	return TRUE;
}
BOOL FromJsonDT_FORMATION_DATA_LIST(const string &strData, DT_FORMATION_DATA_LIST &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_FORMATION_DATA_LIST(root, data);
}
INT32 EncodeDT_FORMATION_DATA(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_DATA *pstIn = (DT_FORMATION_DATA*)pHost;

	if(-1 == poNetData->AddByte(pstIn->bySelectIdx))
		return -1;

	if(-1 == EncodeDT_FORMATION_DATA_LIST(&pstIn->stFormationList, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_FORMATION_DATA(void *pHost, CNetData* poNetData)
{
	DT_FORMATION_DATA *pstOut = (DT_FORMATION_DATA*)pHost;

	if(-1 == poNetData->DelByte(pstOut->bySelectIdx))
		return -1;

	if(-1 == DecodeDT_FORMATION_DATA_LIST(&pstOut->stFormationList, poNetData))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_FORMATION_DATA(DT_FORMATION_DATA &data)
{
	Json::Value root;
	root["SelectIdx"]=data.bySelectIdx;
	root["FormationList"]=GetJsonDT_FORMATION_DATA_LIST(data.stFormationList);

	return root;
}
BOOL FromJsonDT_FORMATION_DATA(Json::Value &root, DT_FORMATION_DATA &data)
{
	data.bySelectIdx=root["SelectIdx"].asUInt();
	FromJsonDT_FORMATION_DATA_LIST(root["FormationList"] , data.stFormationList);

	return TRUE;
}
BOOL FromJsonDT_FORMATION_DATA(const string &strData, DT_FORMATION_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_FORMATION_DATA(root, data);
}
INT32 EncodeDT_VERSION_DATA(void *pHost, CNetData* poNetData)
{
	DT_VERSION_DATA *pstIn = (DT_VERSION_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwVersionID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byForceUpdateFlag))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUpdateContent, sizeof(pstIn->aszUpdateContent)))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszUpdateURL, sizeof(pstIn->aszUpdateURL)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_VERSION_DATA(void *pHost, CNetData* poNetData)
{
	DT_VERSION_DATA *pstOut = (DT_VERSION_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwVersionID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byForceUpdateFlag))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUpdateContent, sizeof(pstOut->aszUpdateContent)))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszUpdateURL, sizeof(pstOut->aszUpdateURL)))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_VERSION_DATA(DT_VERSION_DATA &data)
{
	Json::Value root;
	root["VersionID"]=data.dwVersionID;
	root["ForceUpdateFlag"]=data.byForceUpdateFlag;
	root["UpdateContent"]=data.aszUpdateContent;
	root["UpdateURL"]=data.aszUpdateURL;

	return root;
}
BOOL FromJsonDT_VERSION_DATA(Json::Value &root, DT_VERSION_DATA &data)
{
	data.dwVersionID=root["VersionID"].asUInt();
	data.byForceUpdateFlag=root["ForceUpdateFlag"].asUInt();
	strncpy( data.aszUpdateContent,root["UpdateContent"].asCString(),MAX_CONTENT_SIZE);
	strncpy( data.aszUpdateURL,root["UpdateURL"].asCString(),MAX_CONTENT_SIZE);

	return TRUE;
}
BOOL FromJsonDT_VERSION_DATA(const string &strData, DT_VERSION_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_VERSION_DATA(root, data);
}
INT32 EncodeDT_NOTICE_DATA(void *pHost, CNetData* poNetData)
{
	DT_NOTICE_DATA *pstIn = (DT_NOTICE_DATA*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwNoticeID))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byForcePopFlag))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byUrlFlag))
		return -1;

	if(-1 == poNetData->AddTString(pstIn->aszNoticeContent, sizeof(pstIn->aszNoticeContent)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_NOTICE_DATA(void *pHost, CNetData* poNetData)
{
	DT_NOTICE_DATA *pstOut = (DT_NOTICE_DATA*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwNoticeID))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byForcePopFlag))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byUrlFlag))
		return -1;

	if(-1 == poNetData->DelTString(pstOut->aszNoticeContent, sizeof(pstOut->aszNoticeContent)))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_NOTICE_DATA(DT_NOTICE_DATA &data)
{
	Json::Value root;
	root["NoticeID"]=data.dwNoticeID;
	root["ForcePopFlag"]=data.byForcePopFlag;
	root["UrlFlag"]=data.byUrlFlag;
	root["NoticeContent"]=data.aszNoticeContent;

	return root;
}
BOOL FromJsonDT_NOTICE_DATA(Json::Value &root, DT_NOTICE_DATA &data)
{
	data.dwNoticeID=root["NoticeID"].asUInt();
	data.byForcePopFlag=root["ForcePopFlag"].asUInt();
	data.byUrlFlag=root["UrlFlag"].asUInt();
	strncpy( data.aszNoticeContent,root["NoticeContent"].asCString(),MAX_CONTENT_SIZE);

	return TRUE;
}
BOOL FromJsonDT_NOTICE_DATA(const string &strData, DT_NOTICE_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_NOTICE_DATA(root, data);
}
INT32 EncodeDT_PLAYER_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_DATA *pstIn = (DT_PLAYER_DATA*)pHost;

	if(-1 == EncodeDT_PLAYER_BASE_DATA(&pstIn->stBaseData, poNetData))
		return -1;

	if(-1 == EncodeDT_HERO_DATA_LIST(&pstIn->stHeroData, poNetData))
		return -1;

	if(-1 == EncodeDT_BAG_DATA(&pstIn->stBagData, poNetData))
		return -1;

	if(-1 == EncodeDT_BAG_EQUIP_DATA_LST(&pstIn->stBagEquipData, poNetData))
		return -1;

	if(-1 == EncodeDT_BAG_GOODS_DATA_LST(&pstIn->stBagGoodsData, poNetData))
		return -1;

	if(-1 == EncodeDT_BUILD_DATA_LIST(&pstIn->stBuildData, poNetData))
		return -1;

	if(-1 == EncodeDT_TASK_DATA_LIST(&pstIn->stTaskData, poNetData))
		return -1;

	if(-1 == EncodeDT_RACE_DATA(&pstIn->stRaceData, poNetData))
		return -1;

	if(-1 == EncodeDT_RACE_BATTLELOG_DATA_LIST(&pstIn->stRaceBattleLog, poNetData))
		return -1;

	if(-1 == EncodeDT_LOCALE_DATA(&pstIn->stLocaleData, poNetData))
		return -1;

	if(-1 == EncodeDT_STATE_DATA(&pstIn->stCurState, poNetData))
		return -1;

	if(-1 == EncodeDT_ONLINE_ENCOURAGE_RECORD_DATA(&pstIn->stOnlineEncRecdData, poNetData))
		return -1;

	if(-1 == EncodeDT_LOGIN_ENCOURAGE_RECORD_DATA(&pstIn->stLoginEncRecdData, poNetData))
		return -1;

	if(-1 == EncodeDT_INSTANCE_DATA_LIST(&pstIn->stCommonInstanceData, poNetData))
		return -1;

	if(-1 == EncodeDT_INSTANCE_DATA_LIST(&pstIn->stEliteInstanceData, poNetData))
		return -1;

	if(-1 == EncodeDT_PLUNDER_BASE_DATA(&pstIn->stPlunderBaseData, poNetData))
		return -1;

	if(-1 == EncodeDT_PLUNDER_BATTLELOG_DATA_LIST(&pstIn->stPlunderBattleLog, poNetData))
		return -1;

	if(-1 == EncodeDT_CAPTURE_DATA_LIST(&pstIn->stCaptureData, poNetData))
		return -1;

	if(-1 == EncodeDT_ENEMY_DATA_LIST(&pstIn->stEnemyData, poNetData))
		return -1;

	if(-1 == EncodeDT_MONEY_TREE_DATA(&pstIn->stMoneyTreeData, poNetData))
		return -1;

	if(-1 == EncodeDT_ONHOOK_RCD_DATA(&pstIn->stOnhookRcdData, poNetData))
		return -1;

	if(-1 == EncodeDT_INSTANCE_BATTLE_DATA(&pstIn->stCommonBattleRcd, poNetData))
		return -1;

	if(-1 == EncodeDT_INSTANCE_BATTLE_DATA(&pstIn->stEliteBattleRcd, poNetData))
		return -1;

	if(-1 == EncodeDT_SKILL_DATA_LIST(&pstIn->stSkillData, poNetData))
		return -1;

	if(-1 == EncodeDT_PHYSTRENGTH_DATA(&pstIn->stPhystrengthData, poNetData))
		return -1;

	if(-1 == EncodeDT_FEW_PARAM_DATA(&pstIn->stFewParamData, poNetData))
		return -1;

	if(-1 == EncodeDT_CROP_DATA(&pstIn->stCropData, poNetData))
		return -1;

	if(-1 == EncodeDT_SCIENCE_TREE_DATA(&pstIn->stScienceTree, poNetData))
		return -1;

	if(-1 == EncodeDT_FORMATION_DATA(&pstIn->stFormationInfo, poNetData))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szLastOrderID, sizeof(pstIn->szLastOrderID)))
		return -1;

	return poNetData->GetDataLen();
}

INT32 DecodeDT_PLAYER_DATA(void *pHost, CNetData* poNetData)
{
	DT_PLAYER_DATA *pstOut = (DT_PLAYER_DATA*)pHost;

	if(-1 == DecodeDT_PLAYER_BASE_DATA(&pstOut->stBaseData, poNetData))
		return -1;

	if(-1 == DecodeDT_HERO_DATA_LIST(&pstOut->stHeroData, poNetData))
		return -1;

	if(-1 == DecodeDT_BAG_DATA(&pstOut->stBagData, poNetData))
		return -1;

	if(-1 == DecodeDT_BAG_EQUIP_DATA_LST(&pstOut->stBagEquipData, poNetData))
		return -1;

	if(-1 == DecodeDT_BAG_GOODS_DATA_LST(&pstOut->stBagGoodsData, poNetData))
		return -1;

	if(-1 == DecodeDT_BUILD_DATA_LIST(&pstOut->stBuildData, poNetData))
		return -1;

	if(-1 == DecodeDT_TASK_DATA_LIST(&pstOut->stTaskData, poNetData))
		return -1;

	if(-1 == DecodeDT_RACE_DATA(&pstOut->stRaceData, poNetData))
		return -1;

	if(-1 == DecodeDT_RACE_BATTLELOG_DATA_LIST(&pstOut->stRaceBattleLog, poNetData))
		return -1;

	if(-1 == DecodeDT_LOCALE_DATA(&pstOut->stLocaleData, poNetData))
		return -1;

	if(-1 == DecodeDT_STATE_DATA(&pstOut->stCurState, poNetData))
		return -1;

	if(-1 == DecodeDT_ONLINE_ENCOURAGE_RECORD_DATA(&pstOut->stOnlineEncRecdData, poNetData))
		return -1;

	if(-1 == DecodeDT_LOGIN_ENCOURAGE_RECORD_DATA(&pstOut->stLoginEncRecdData, poNetData))
		return -1;

	if(-1 == DecodeDT_INSTANCE_DATA_LIST(&pstOut->stCommonInstanceData, poNetData))
		return -1;

	if(-1 == DecodeDT_INSTANCE_DATA_LIST(&pstOut->stEliteInstanceData, poNetData))
		return -1;

	if(-1 == DecodeDT_PLUNDER_BASE_DATA(&pstOut->stPlunderBaseData, poNetData))
		return -1;

	if(-1 == DecodeDT_PLUNDER_BATTLELOG_DATA_LIST(&pstOut->stPlunderBattleLog, poNetData))
		return -1;

	if(-1 == DecodeDT_CAPTURE_DATA_LIST(&pstOut->stCaptureData, poNetData))
		return -1;

	if(-1 == DecodeDT_ENEMY_DATA_LIST(&pstOut->stEnemyData, poNetData))
		return -1;

	if(-1 == DecodeDT_MONEY_TREE_DATA(&pstOut->stMoneyTreeData, poNetData))
		return -1;

	if(-1 == DecodeDT_ONHOOK_RCD_DATA(&pstOut->stOnhookRcdData, poNetData))
		return -1;

	if(-1 == DecodeDT_INSTANCE_BATTLE_DATA(&pstOut->stCommonBattleRcd, poNetData))
		return -1;

	if(-1 == DecodeDT_INSTANCE_BATTLE_DATA(&pstOut->stEliteBattleRcd, poNetData))
		return -1;

	if(-1 == DecodeDT_SKILL_DATA_LIST(&pstOut->stSkillData, poNetData))
		return -1;

	if(-1 == DecodeDT_PHYSTRENGTH_DATA(&pstOut->stPhystrengthData, poNetData))
		return -1;

	if(-1 == DecodeDT_FEW_PARAM_DATA(&pstOut->stFewParamData, poNetData))
		return -1;

	if(-1 == DecodeDT_CROP_DATA(&pstOut->stCropData, poNetData))
		return -1;

	if(-1 == DecodeDT_SCIENCE_TREE_DATA(&pstOut->stScienceTree, poNetData))
		return -1;

	if(-1 == DecodeDT_FORMATION_DATA(&pstOut->stFormationInfo, poNetData))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szLastOrderID, sizeof(pstOut->szLastOrderID)))
		return -1;

	return sizeof(*pstOut);
}

Json::Value GetJsonDT_PLAYER_DATA(DT_PLAYER_DATA &data)
{
	Json::Value root;
	root["BaseData"]=GetJsonDT_PLAYER_BASE_DATA(data.stBaseData);
	root["HeroData"]=GetJsonDT_HERO_DATA_LIST(data.stHeroData);
	root["BagData"]=GetJsonDT_BAG_DATA(data.stBagData);
	root["BagEquipData"]=GetJsonDT_BAG_EQUIP_DATA_LST(data.stBagEquipData);
	root["BagGoodsData"]=GetJsonDT_BAG_GOODS_DATA_LST(data.stBagGoodsData);
	root["BuildData"]=GetJsonDT_BUILD_DATA_LIST(data.stBuildData);
	root["TaskData"]=GetJsonDT_TASK_DATA_LIST(data.stTaskData);
	root["RaceData"]=GetJsonDT_RACE_DATA(data.stRaceData);
	root["RaceBattleLog"]=GetJsonDT_RACE_BATTLELOG_DATA_LIST(data.stRaceBattleLog);
	root["LocaleData"]=GetJsonDT_LOCALE_DATA(data.stLocaleData);
	root["CurState"]=GetJsonDT_STATE_DATA(data.stCurState);
	root["OnlineEncRecdData"]=GetJsonDT_ONLINE_ENCOURAGE_RECORD_DATA(data.stOnlineEncRecdData);
	root["LoginEncRecdData"]=GetJsonDT_LOGIN_ENCOURAGE_RECORD_DATA(data.stLoginEncRecdData);
	root["CommonInstanceData"]=GetJsonDT_INSTANCE_DATA_LIST(data.stCommonInstanceData);
	root["EliteInstanceData"]=GetJsonDT_INSTANCE_DATA_LIST(data.stEliteInstanceData);
	root["PlunderBaseData"]=GetJsonDT_PLUNDER_BASE_DATA(data.stPlunderBaseData);
	root["PlunderBattleLog"]=GetJsonDT_PLUNDER_BATTLELOG_DATA_LIST(data.stPlunderBattleLog);
	root["CaptureData"]=GetJsonDT_CAPTURE_DATA_LIST(data.stCaptureData);
	root["EnemyData"]=GetJsonDT_ENEMY_DATA_LIST(data.stEnemyData);
	root["MoneyTreeData"]=GetJsonDT_MONEY_TREE_DATA(data.stMoneyTreeData);
	root["OnhookRcdData"]=GetJsonDT_ONHOOK_RCD_DATA(data.stOnhookRcdData);
	root["CommonBattleRcd"]=GetJsonDT_INSTANCE_BATTLE_DATA(data.stCommonBattleRcd);
	root["EliteBattleRcd"]=GetJsonDT_INSTANCE_BATTLE_DATA(data.stEliteBattleRcd);
	root["SkillData"]=GetJsonDT_SKILL_DATA_LIST(data.stSkillData);
	root["PhystrengthData"]=GetJsonDT_PHYSTRENGTH_DATA(data.stPhystrengthData);
	root["FewParamData"]=GetJsonDT_FEW_PARAM_DATA(data.stFewParamData);
	root["CropData"]=GetJsonDT_CROP_DATA(data.stCropData);
	root["ScienceTree"]=GetJsonDT_SCIENCE_TREE_DATA(data.stScienceTree);
	root["FormationInfo"]=GetJsonDT_FORMATION_DATA(data.stFormationInfo);
	root["LastOrderID"]=data.szLastOrderID;

	return root;
}
BOOL FromJsonDT_PLAYER_DATA(Json::Value &root, DT_PLAYER_DATA &data)
{
	FromJsonDT_PLAYER_BASE_DATA(root["BaseData"] , data.stBaseData);
	FromJsonDT_HERO_DATA_LIST(root["HeroData"] , data.stHeroData);
	FromJsonDT_BAG_DATA(root["BagData"] , data.stBagData);
	FromJsonDT_BAG_EQUIP_DATA_LST(root["BagEquipData"] , data.stBagEquipData);
	FromJsonDT_BAG_GOODS_DATA_LST(root["BagGoodsData"] , data.stBagGoodsData);
	FromJsonDT_BUILD_DATA_LIST(root["BuildData"] , data.stBuildData);
	FromJsonDT_TASK_DATA_LIST(root["TaskData"] , data.stTaskData);
	FromJsonDT_RACE_DATA(root["RaceData"] , data.stRaceData);
	FromJsonDT_RACE_BATTLELOG_DATA_LIST(root["RaceBattleLog"] , data.stRaceBattleLog);
	FromJsonDT_LOCALE_DATA(root["LocaleData"] , data.stLocaleData);
	FromJsonDT_STATE_DATA(root["CurState"] , data.stCurState);
	FromJsonDT_ONLINE_ENCOURAGE_RECORD_DATA(root["OnlineEncRecdData"] , data.stOnlineEncRecdData);
	FromJsonDT_LOGIN_ENCOURAGE_RECORD_DATA(root["LoginEncRecdData"] , data.stLoginEncRecdData);
	FromJsonDT_INSTANCE_DATA_LIST(root["CommonInstanceData"] , data.stCommonInstanceData);
	FromJsonDT_INSTANCE_DATA_LIST(root["EliteInstanceData"] , data.stEliteInstanceData);
	FromJsonDT_PLUNDER_BASE_DATA(root["PlunderBaseData"] , data.stPlunderBaseData);
	FromJsonDT_PLUNDER_BATTLELOG_DATA_LIST(root["PlunderBattleLog"] , data.stPlunderBattleLog);
	FromJsonDT_CAPTURE_DATA_LIST(root["CaptureData"] , data.stCaptureData);
	FromJsonDT_ENEMY_DATA_LIST(root["EnemyData"] , data.stEnemyData);
	FromJsonDT_MONEY_TREE_DATA(root["MoneyTreeData"] , data.stMoneyTreeData);
	FromJsonDT_ONHOOK_RCD_DATA(root["OnhookRcdData"] , data.stOnhookRcdData);
	FromJsonDT_INSTANCE_BATTLE_DATA(root["CommonBattleRcd"] , data.stCommonBattleRcd);
	FromJsonDT_INSTANCE_BATTLE_DATA(root["EliteBattleRcd"] , data.stEliteBattleRcd);
	FromJsonDT_SKILL_DATA_LIST(root["SkillData"] , data.stSkillData);
	FromJsonDT_PHYSTRENGTH_DATA(root["PhystrengthData"] , data.stPhystrengthData);
	FromJsonDT_FEW_PARAM_DATA(root["FewParamData"] , data.stFewParamData);
	FromJsonDT_CROP_DATA(root["CropData"] , data.stCropData);
	FromJsonDT_SCIENCE_TREE_DATA(root["ScienceTree"] , data.stScienceTree);
	FromJsonDT_FORMATION_DATA(root["FormationInfo"] , data.stFormationInfo);
	strncpy( data.szLastOrderID,root["LastOrderID"].asCString(),MAX_ORDER_ID_LEN);

	return TRUE;
}
BOOL FromJsonDT_PLAYER_DATA(const string &strData, DT_PLAYER_DATA &data)
{
	Json::Value root;
	Json::Reader read;
	if( !read.parse( strData, root ) )
	{
		USR_IMPORTANT( _SDT( "json解析数据失败,数据%s" ), strData.c_str() );
		return FALSE;
	}
	return FromJsonDT_PLAYER_DATA(root, data);
}
