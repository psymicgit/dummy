
#include "buff.h"
#include "buffaffecthandler.h"
#include <logic/battle/battleattrobj.h>


CBuff::CBuff()
{
    m_wSkillID = 0;
    m_byBuffKindID	= 0;
    m_poActor		= NULL;
	m_poGiver		= NULL;
    m_wParamValue	= 0;
    m_byRoundNum	= 0;
    m_poBuffAffect	= NULL;
    m_byCurRoundIdx = 0;
    m_bRoundDecFlag = FALSE;
	m_dwExtValue = 0;
}

CBuff::~CBuff()
{

}

BOOL CBuff::Init(UINT16 wSkillID, UINT8 byBuffKindID, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, UINT8 byRoundNum)
{
    if(byRoundNum < 1)
    {
        return FALSE;
    }
    m_wSkillID = wSkillID;
    m_byBuffKindID = byBuffKindID;
    m_poActor = poActor;
	m_poGiver = poGiver;
    m_wParamValue = wParamValue;
    m_byRoundNum = byRoundNum;
    if(0 == m_byRoundNum)//表示永久直到战斗结束
    {
        m_byRoundNum = 200;
    }

    m_poBuffAffect = CBuffAffectHandler::Instance()->GetBuffAffect(m_byBuffKindID);
    if (NULL == m_poBuffAffect)
    {
        return FALSE;
    }
    //作用到自身的buff都是下次轮到自己时才有机会生效，则++
    if(EBT_MYSELF == m_poBuffAffect->GetBuffTarget())
    {
        m_byRoundNum++;
    }

    return TRUE;
}



VOID CBuff::Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poNormalAttackTarget)
{
    //回合数已到
    if(0 == m_byRoundNum)
    {
        return;
    }

    //效果, 攻击者的回合 则回合--
	if(poAttacker == m_poActor)
	{
		m_byRoundNum--;
		m_bRoundDecFlag = TRUE;
	}

	////效果,发生作用则回合--
    //if((m_poBuffAffect->Affect(ePhase, bMyAttack, poAttacker, m_poGiver, m_poActor, m_wParamValue, poNormalAttackTarget) && )
    //{
    //    m_byRoundNum--;
    //    m_bRoundDecFlag = TRUE;
    //}

	m_poBuffAffect->Affect(ePhase, bMyAttack, poAttacker, m_poGiver, m_poActor, m_wParamValue, poNormalAttackTarget, m_dwExtValue);
}

//回合结束
VOID CBuff::EndRound(BOOL bEndBigRound)
{
    //全是单体回合，大回合去掉
    /*
    //回合结束回合,若之前未--回合则--
    if(!m_bRoundDecFlag)
    {
    	if(!m_poBuffAffect->BigRound())
    	{
    		m_byRoundNum--;
    	}
    	else if((m_poBuffAffect->BigRound()) && (bEndBigRound))
    	{
    		m_byRoundNum--;
    	}
    }

    m_bRoundDecFlag = FALSE;
    */
}

