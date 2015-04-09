

#ifndef _BUFF_H_
#define _BUFF_H_

#include <protocol/server/protocommondata.h>
#include <logic/base/baseobj.h>
#include "buffaffect.h"


using namespace std ;


#define  BUFF_INVALID_KINDID  0  //无效buffID


/*************************

攻击1回合逻辑上是我方攻击1次，对方攻击1次方算1回合
buff都是当前回合立即生效，蓄力buff是在攻击之后给的buff，则不影响攻击，则需持续2回合

*************************/

class CBattleAttrObj;

class CBuff: public CBaseObj
{
public:
    CBuff();
    ~CBuff();
public:
    //
    BOOL Init(UINT16 wSkillID, UINT8 byBuffKindID, CBattleAttrObj* poGiver, CBattleAttrObj* poActor, UINT16 wParamValue, UINT8 byRoundNum);
    //效果生效作用,buff都是攻击前生效
    VOID Affect(EBuffAffectPhase ePhase, BOOL bMyAttack, CBattleAttrObj* poAttacker, CBattleAttrObj* poNormalAttackTarget);//poAttacker为攻击者
    VOID EndRound(BOOL bEndBigRound);//回合结束

    UINT16	GetSkillID()
    {
        return m_wSkillID;
    }
    UINT8	GetKindID()
    {
        return m_byBuffKindID;
    }
    UINT8	GetRoundNum()
    {
        return m_byRoundNum;
    }
    CBattleAttrObj* GetActor()
    {
        return m_poActor;
    }
	VOID SetExtValue(UINT32 dwExtValue)
	{
		m_dwExtValue = dwExtValue;
	}

protected:

private:
    ///参数
    UINT16			m_wSkillID;		//释放该buff的技能ID
    UINT8			m_byBuffKindID;	//buff类型
    CBattleAttrObj*	m_poActor;		//buff作用者
    CBattleAttrObj*	m_poGiver;		//buff给予者，	给予者将buff放到作用者身上
    UINT16			m_wParamValue;	//buff参数值
    UINT8			m_byRoundNum;		//持续回合数
    CBuffAffect*		m_poBuffAffect;	//效果作用类
    //
    UINT8			m_byCurRoundIdx; //当前回合数
    BOOL				m_bRoundDecFlag; //回合数--标志

	UINT32		m_dwExtValue;//一定用户定义数据
public:
    VOID		SetMemID(UINT32 dwMemID)
    {
        m_dwMemID = dwMemID;
    }
    UINT32		GetMemID()
    {
        return m_dwMemID;
    }
private:
    UINT32		m_dwMemID;//内存ID，便于管理器map管理

};



#endif //#ifndef _BUFF_H_

