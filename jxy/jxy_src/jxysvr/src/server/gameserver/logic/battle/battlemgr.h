
#ifndef _BATTLEMGR_H_
#define _BATTLEMGR_H_


#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdmutex.h>
#include <protocol/server/protocommondata.h>
#include "battleobj.h"
#include <logic/skill/buff.h>


using namespace SGDP;

#define MAX_DEFENSE	5000 //最大防御力

//攻击者标志
enum EAttackerFlag
{
	EAF_MYSELF = 0,//我方
	EAF_ENEMY = 1,//敌方
};


//回合结果
enum ERoundResult
{	
	ERR_CONTINUE = 0, //继续该回合的下阶段
	ERR_SKIP = 1, //跳过该回合
	ERR_DEAD = 2, //对方死亡
};

class CPlayer;
class CMonsterGroup;
class CSpecialEffect;
/**
* @brief 战斗管理类
*/
class CBattleMgr
{
public:
    DECLARE_SINGLETON(CBattleMgr)

    /**
    * @brief
    * 初始化函数,读取配置文件等
    * @return 执行结果，成功返回TRUE，失败返回FALSE
    */
    BOOL Init();

    /**
    * @brief
    * 卸载函数
    */
    VOID UnInit();


    /**
    * @brief
    * 战斗
	* poMyself	:自己
	* poEnemy	:敌人(怪物)
	* stDT_BATTLE_DATA	:战斗数据
    * @return 执行结果，成功返回TRUE，失败返回FALSE
    */
    BOOL Battle(
        CBattleObj* poMyself, 
        CBattleObj* poEnemy, 
        DT_BATTLE_DATA2& stDT_BATTLE_DATA, 
        BOOL bPVP = FALSE, 
        DT_BATTLE_OBJ_GROUP_HP *pstAfterMyselfInfo = NULL, 
        BOOL bChallengFirst = FALSE
    );

        /**
    * @brief
    * 战斗
	* poMyself	:自己
	* poEnemy	:敌人(怪物)
	* stDT_BATTLE_DATA	:战斗数据
    * @return 执行结果，成功返回TRUE，失败返回FALSE
    * pstAfterMyselfInfo : 自己战斗完后信息
    //////////以下数据针对跨服战////////////
    * pstAfterMyselfInfo2 ：自己战斗完后数据，包括血量和KindID
    * pstAfterEnemyInfo2：　敌人战斗完后数据，包括血量和KindID
    * byRound : 攻击回合, 0为默认战斗方式 1为自己还有残血 2为敌人还有残血
    * pstNewMyselfInfo2 ：自己在第一次战斗后的数据 用来赋值给第二次战斗
    * pstNewEnemyInfo2 ：敌人在第一次战斗后的数据 用来赋值给第二次战斗
    */
    BOOL Battle2(
        CBattleObj* poMyself, 
        CBattleObj* poEnemy, 
        DT_BATTLE_DATA3& stDT_BATTLE_DATA, 
        BOOL bPVP = FALSE, 
        BOOL bChallengFirst = FALSE, 

        DT_BATTLE_OBJ_GROUP_HP *pstAfterMyselfHP = NULL, 
        DT_BATTLE_OBJ_GROUP_HP *pstAfterEnemyHP = NULL,
        DT_BATTLE_OBJ_GROUP_HP *pstInitMyselfHP = NULL, 
        DT_BATTLE_OBJ_GROUP_HP *pstInitEnemyHP = NULL
    );

	/**
    * @brief
    * 战斗
	* poMyself	:自己
	* poEnemy	:敌人(怪物)
	* stDT_BATTLE_DATA	:战斗数据
    * @return 执行结果，成功返回TRUE，失败返回FALSE
    * pstAfterMyselfInfo : 自己战斗完后信息
    //////////以下数据针对跨服战////////////
    * pstAfterMyselfHP ：存储左方战斗完后的数据
    * pstAfterEnemyHP：　存储右方战斗完后的数据
    * pstInitMyselfHP ： 左方初始化时的血量
    * pstInitEnemyHP ：  右方初始化时的血量
    * byMyselfWeakenPercent: 左方强化的比例，如，30表示玩家全属性（攻防+4隐藏属性）变为原来的30%，130代表全属性变为原来的130%
    * byEnemyWeakenPercent:  右方强化的比例
    */
    BOOL Battle3(
        CBattleObj* poMyself, 
        CBattleObj* poEnemy, 
        DT_BATTLE_DATA4& stDT_BATTLE_DATA, 
        BOOL bPVP = FALSE, 
        BOOL bChallengFirst = FALSE, 

        DT_BATTLE_OBJ_GROUP_HP *pstAfterMyselfHP = NULL, 
        DT_BATTLE_OBJ_GROUP_HP *pstAfterEnemyHP = NULL,
        DT_BATTLE_OBJ_GROUP_HP *pstInitMyselfHP = NULL, 
        DT_BATTLE_OBJ_GROUP_HP *pstInitEnemyHP = NULL,
        UINT8 byMyselfStrongPercent = 100,
        UINT8 byEnemyStrongPercent  = 100
    );

		/**
    * @brief
    * 战斗
	* poMyself	:自己
	* poEnemy	:敌人(怪物)
	* stDT_BATTLE_DATA	:战斗数据
    * @return 执行结果，成功返回TRUE，失败返回FALSE
    * pstAfterMyselfInfo : 自己战斗完后信息
    //////////以下数据针对跨服战////////////
    * pstAfterMyselfHP ：存储左方战斗完后的数据
    * pstAfterEnemyHP：　存储右方战斗完后的数据
    * pstInitMyselfHP ： 左方初始化时的血量
    * pstInitEnemyHP ：  右方初始化时的血量
    * byMyselfWeakenPercent: 左方强化的比例，如，30表示玩家全属性（攻防+4隐藏属性）变为原来的30%，130代表全属性变为原来的130%
    * byEnemyWeakenPercent:  右方强化的比例
    */
    BOOL Battle4(
        CBattleObj* poMyself, 
        CBattleObj* poEnemy, 
        DT_BATTLE_DATA5& stDT_BATTLE_DATA, 
        BOOL bPVP = FALSE, 
        BOOL bChallengFirst = FALSE, 

        DT_BATTLE_OBJ_GROUP_HP *pstAfterMyselfHP = NULL, 
        DT_BATTLE_OBJ_GROUP_HP *pstAfterEnemyHP = NULL,
        DT_BATTLE_OBJ_GROUP_HP *pstInitMyselfHP = NULL, 
        DT_BATTLE_OBJ_GROUP_HP *pstInitEnemyHP = NULL,
        UINT8 byMyselfStrongPercent = 100,
        UINT8 byEnemyStrongPercent  = 100
    );
	//转换版本
	//从4转换为2
	VOID Convert5To3(const DT_BATTLE_DATA5& stData4, DT_BATTLE_DATA3& stData3);
	VOID Convert5To4(const DT_BATTLE_DATA5& stData4, DT_BATTLE_DATA4& stData3);
	VOID Convert4To3(const DT_BATTLE_DATA4& stData4, DT_BATTLE_DATA3& stData3);
	VOID Convert4To2(const DT_BATTLE_DATA4& stData4, DT_BATTLE_DATA2& stData2);
	VOID Convert2To4(const DT_BATTLE_DATA2& stData2, DT_BATTLE_DATA4& stData4);
	VOID Convert3To4(const DT_BATTLE_DATA3& stData3, DT_BATTLE_DATA4& stData4);
	VOID ReplaceBattleData(const vector<UINT16>& vecMonsterID, DT_BATTLE_DATA2& stDT_BATTLE_DATA);
  
	UINT32 CountHurt(CBattleAttrObj* poCurAttacker, CBattleAttrObj* poBeattacker, UINT8* byAttackSpecialEffectFlag, UINT8* byEnemySpecialEffectFlag);
	UINT32 SecondCountHurt(CBattleAttrObj* poCurAttacker, CBattleAttrObj* poBeattacker, CSpecialEffect* byAttackType, UINT8* byAttackSpecialEffectFlag = NULL, UINT8* byEnemySpecialEffectFlag = NULL);
	VOID BattleData221(DT_BATTLE_DATA2& stBattleInfo2, DT_BATTLE_DATA& stBattleInfo1);

	UINT16 BattleMyself(CPlayer* poMyself, UINT8 byCareerID, DT_BATTLE_DATA5& stDT_BATTLE_DATA, DT_BATTLE_OBJ_GROUP_HP *pstAfterMyselfInfo = NULL);

	DT_BATTLE_ATTRIBUTE& NewToOldBattleAttribute(const DT_BATTLE_BASE_ATTRIBUTE& stAttr2);
	DT_BATTLE_ATTRIBUTE& NewToOldBattleAttribute(const DT_BATTLE_ATTRIBUTE2& stAttr2);
	DT_BATTLE_ATTRIBUTE2 ConvertAttribute2ToBase(const DT_BATTLE_BASE_ATTRIBUTE& stAttr);
	VOID VerifyBattle(const DT_BATTLE_DATA5& stBattleData4);
protected:
    /**
    * @brief
    * 战斗一回合
	* poAttacker	:攻击者
	* poBeattacker	:被攻击者
	* bAttackerFlag :攻击者标志，FALSE为挑战者
	* stDT_BATTLE_ROUND_DATA:回合记录
    * @return 回合结果
    */

	ERoundResult OnceRound3(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag& eAttackerFlag, DT_BATTLE_ROUND_DATA4& stBattleRoundInfo, CBattleAttrObj*& poEnemy); 

	ERoundResult OnceRound4(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag& eAttackerFlag, DT_BATTLE_ROUND_DATA5& stBattleRoundInfo, CBattleAttrObj*& poEnemy); 
	//两次攻击
	ERoundResult SecondAttack(CBattleObj* poAttacker, CBattleAttrObj* poObj, CBattleObj* poBeattacker, CBattleAttrObj* poEnemyObj, EAttackerFlag& eAttackFlag, CSpecialEffect* byAttackType, DT_BATTLE_ROUND_DATA4& stBattleInfo);
	//两次攻击
	ERoundResult SecondAttack(CBattleObj* poAttacker, CBattleAttrObj* poObj, CBattleObj* poBeattacker, CBattleAttrObj* poEnemyObj, EAttackerFlag& eAttackFlag, CSpecialEffect* byAttackType, DT_BATTLE_ROUND_DATA5& stBattleInfo);
	//宠物攻击
	ERoundResult PetBattle(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag& eAttackerFlag, DT_BATTLE_ROUND_DATA5& stBattleRoundInfo, vector<CBattleAttrObj*>& vecTargetObj);
	//宠物二次攻击
	ERoundResult PetSecondBattle(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag& eAttackerFlag, DT_BATTLE_ROUND_DATA5& stBattleRoundInfo, vector<CBattleAttrObj*>& vecTargetObj);
	//宠物伤害计算
	VOID PetCountHurt(CPet* poAttack, CBattleAttrObj* poBeattacker);

	VOID BuffAffect(EBuffAffectPhase eBuffAffectPhase, CBattleObj* poAttacker, CBattleObj* poBeattacker);
	

	VOID BuffRoundDec(CBattleObj* poBattleObj, BOOL bEndBigRound = FALSE);//buff回合减少

	ERoundResult ActionJudge(CBattleObj* poAttacker);

	VOID CountAttackBackHurt(CBattleAttrObj* poAttacker, CBattleAttrObj* poBeattacker);

	BOOL JudgeDead(CBattleObj* poAttacker, CBattleObj* poBeattacker);

	VOID GetAttackData(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag eAttackerFlag, BOOL bBeforeAttack, DT_ATTACK_DATA& stDT_ATTACK_DATA);

    VOID GetAttackData2(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag eAttackerFlag, BOOL bBeforeAttack, DT_ATTACK_DATA2& stDT_ATTACK_DATA);

	VOID GetAttackData3(CBattleObj* poAttacker, CBattleObj* poBeattacker, EAttackerFlag eAttackerFlag, BOOL bBeforeAttack, DT_ATTACK_DATA3& stDT_ATTACK_DATA);

	VOID CkGodweaponAttack(CBattleAttrObj* poAttacker, CBattleAttrObj* poBeattacker, DT_GODWEAPON_ATTACK_DATA& stGodweaponAttackInfo);//检查神器攻击及攻击数值
	VOID CkGodweaponAttack(CBattleAttrObj* poAttacker, CBattleAttrObj* poBeattacker, DT_GODWEAPON_ATTACK_DATA2& stGodweaponAttackInfo);//检查神器攻击及攻击数值

	//////////////////////////////////////////////////////////////////////////////////////////

	VOID BuffAffect(EBuffAffectPhase eBuffAffectPhase, BOOL bMyAttack, CBattleAttrObj* poAttrObj, CBattleAttrObj* poAttacker, CBattleAttrObj* poNormalAttackTarget);	
	VOID SetChangeHP(UINT8& byNum, INT32 anHpChangeInfo[], vector<INT32>& vecHP, BOOL bReverse = FALSE);
	INT32 GetTotalChangeHP(vector<INT32>& vecHP);
	VOID GetAttackData(CBattleAttrObj* poObj, BOOL bBeforeAttack, DT_ATTACK_OBJ_DATA& stDT_ATTACK_OBJ_DATA);
    VOID GetAttackData2(CBattleAttrObj* poObj, BOOL bBeforeAttack, DT_ATTACK_OBJ_DATA2& stDT_ATTACK_OBJ_DATA);
	VOID GetAttackData3(CBattleAttrObj* poObj, BOOL bBeforeAttack, DT_ATTACK_OBJ_DATA3& stDT_ATTACK_OBJ_DATA);

protected:
	CBattleMgr();
	~CBattleMgr();
private:

};




#endif //#ifndef

