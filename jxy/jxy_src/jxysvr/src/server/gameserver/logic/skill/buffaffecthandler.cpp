#include "buffaffecthandler.h"
#include "buff.h"

VOID CBuffAffectHandler::Init()
{
	m_mapAffectOprator[CBuffAffect_BloodthirstySingle::Instance()->GetBuffState()] = CBuffAffect_BloodthirstySingle::Instance();     
	m_mapAffectOprator[CBuffAffect_BloodthirstyAll::Instance()->GetBuffState()] = CBuffAffect_BloodthirstyAll::Instance();     
	m_mapAffectOprator[CBuffAffect_NoSkill::Instance()->GetBuffState()] = CBuffAffect_NoSkill::Instance();     
	m_mapAffectOprator[CBuffAffect_Poisoning::Instance()->GetBuffState()] = CBuffAffect_Poisoning::Instance();     
	m_mapAffectOprator[CBuffAffect_DecAttack::Instance()->GetBuffState()] = CBuffAffect_DecAttack::Instance();     
	m_mapAffectOprator[CBuffAffect_Tear::Instance()->GetBuffState()] = CBuffAffect_Tear::Instance(); 
	m_mapAffectOprator[CBuffAffect_Static::Instance()->GetBuffState()] = CBuffAffect_Static::Instance();  	
	m_mapAffectOprator[CBuffAffect_DecHurtSingle::Instance()->GetBuffState()] = CBuffAffect_DecHurtSingle::Instance();  	
	m_mapAffectOprator[CBuffAffect_DecHurtAll::Instance()->GetBuffState()] = CBuffAffect_DecHurtAll::Instance();  		
	m_mapAffectOprator[CBuffAffect_DefendPercent::Instance()->GetBuffState()] = CBuffAffect_DefendPercent::Instance();  	
	m_mapAffectOprator[CBuffAffect_Frost::Instance()->GetBuffState()] = CBuffAffect_Frost::Instance();  	
	m_mapAffectOprator[CBuffAffect_Chaos::Instance()->GetBuffState()] = CBuffAffect_Chaos::Instance();  	
	m_mapAffectOprator[CBuffAffect_Firing::Instance()->GetBuffState()] = CBuffAffect_Firing::Instance();  	
	m_mapAffectOprator[CBuffAffect_Disturb::Instance()->GetBuffState()] = CBuffAffect_Disturb::Instance();  	
	m_mapAffectOprator[CBuffAffect_NotAnger::Instance()->GetBuffState()] = CBuffAffect_NotAnger::Instance();  	
	m_mapAffectOprator[CBuffAffect_Sleep::Instance()->GetBuffState()] = CBuffAffect_Sleep::Instance();  
	m_mapAffectOprator[CBuffAffect_AddBlood::Instance()->GetBuffState()] = CBuffAffect_AddBlood::Instance();  
	m_mapAffectOprator[CBuffAffect_Dreamland::Instance()->GetBuffState()] = CBuffAffect_Dreamland::Instance();
  	
}

CBuffAffect* CBuffAffectHandler::GetBuffAffect(UINT8 byBuffKind)
{
	CBuffAffectOpratorMapItr itr = m_mapAffectOprator.find(byBuffKind);
	if( itr != m_mapAffectOprator.end())
	{
		return itr->second;
	}

	return NULL;
}