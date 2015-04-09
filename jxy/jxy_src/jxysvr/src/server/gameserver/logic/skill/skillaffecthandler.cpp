#include "skillaffecthandler.h"
#include "buff.h"

VOID CSkillOnceAffectHandler::Init()
{
	m_mapAffectOprator[CSkillOnceAffect_Hurt::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_Hurt::Instance();        	
	m_mapAffectOprator[CSkillOnceAffect_SingleCure::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_SingleCure::Instance();        	
	m_mapAffectOprator[CSkillOnceAffect_WholeCure::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_WholeCure::Instance();        	
	m_mapAffectOprator[CSkillOnceAffect_DecAngry::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_DecAngry::Instance();
	m_mapAffectOprator[CSkillOnceAffect_SecondAttack::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_SecondAttack::Instance();
	m_mapAffectOprator[CSkillOnceAffect_Critial::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_Critial::Instance();
	m_mapAffectOprator[CSkillOnceAffect_Water::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_Water::Instance();
	m_mapAffectOprator[CSkillOnceAffect_Soil::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_Soil::Instance();
	m_mapAffectOprator[CSkillOnceAffect_Ice::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_Ice::Instance();
	m_mapAffectOprator[CSkillOnceAffect_Sand::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_Sand::Instance();
	m_mapAffectOprator[CSkillOnceAffect_Spark::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_Spark::Instance();
	m_mapAffectOprator[CSkillOnceAffect_Haunting::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_Haunting::Instance();
	m_mapAffectOprator[CSkillOnceAffect_Drame::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_Drame::Instance();
	m_mapAffectOprator[CSkillOnceAffect_Soul::Instance()->GetSkillEffectKind()] = CSkillOnceAffect_Soul::Instance();
  
}

CSkillOnceAffect* CSkillOnceAffectHandler::GetSkillAffect(UINT8 bySkillEffectKindID)
{
	CSkillAffectOpratorMapItr itr = m_mapAffectOprator.find(bySkillEffectKindID);
	if( itr != m_mapAffectOprator.end())
	{
		return itr->second;
	}

	return NULL;
}