#ifndef _HERO_H_
#define _HERO_H_


#include <sdhashmap.h>
#include <protocol/server/protocommondata.h>
#include <logic/item/itemmgr.h>
#include <logic/battle/battleattrobj.h>
#include <logic/talent/drugmgr.h>
#include <logic/talent/experiencedrugpropmgr.h>
#include <logic/godweapon/godweaponmgr.h>
#include <logic/shmem/shmemmap.h>
#include <logic/monster/playergroup.h>


struct tagHeroUpgradeProp;
typedef tagHeroUpgradeProp SHeroUpgradeProp;

struct tagHeroProp;
typedef tagHeroProp SHeroProp;

using namespace std ;


#define EQUIP_POS_NUM	4 //穿戴位置数

typedef CShmemMap<UINT64, CEquip> CEquipMap; 
//typedef CEquipMap::iterator CEquipMapItr;

typedef CShmemMap<UINT8, CEquip> CEquipPosMap;//key为装备在武将身上的位置
//typedef CEquipPosMap::iterator CEquipPosMapItr;

typedef CShmemMap<UINT16, CDrug> CLevel2DrugMap;//key丹药等级
//typedef CLevel2DrugMap::iterator CLevel2DrugMapItr;
typedef CShmemMap<UINT64, CJewelry> CJewelryMap;
typedef CShmemMap<UINT8, CJewelry> CJewelryPosMap;

class CPlayer;
class CPlayerGroup;

class CSoul;

class CHero:public CBattleAttrObj
{
public:
	CHero();
	~CHero();
public:
	virtual inline UINT8 GetFormationIdx();

	void SetPlayerGroup(CPlayerGroup *poPlayerGroup);

	virtual VOID ResetSpecialEffects();

	//初始化相关对象
	BOOL Init(DT_HERO_DATA* pstDT_HERO_DATA, DT_JEWELRY_DATA_LST& stJewelry, const DT_HERO_SOUL_DATA& rSoulData, CPlayer* poOwner);
	
    //添加经验,若可升级则升级，直到满级
	BOOL AddExperience(UINT32 dwValue);

	//添加经验,若可升级则升级，升级不能超过传入的等级
	BOOL AddExperience(UINT32 dwValue, UINT16 wLevel);

    void AddFlyExp(UINT32 dwExp){ m_stDT_HERO_BASE_DATA.dwFlyExp += dwExp; }

    //赋值升级信息，给客户端用
    void SaveClientUpgradeExp();

    //获取武将等级
    UINT16 GetLevel(){ return m_stDT_HERO_BASE_DATA.wLevel; }

	//设置武将等级
	VOID SetLevel(UINT16 wLevel);

	DT_HERO_BASE_DATA&	GetDT_HERO_BASE_DATA() { return m_stDT_HERO_BASE_DATA; }
	DT_ABSORB_DATA_LST&	GetDT_ABSORB_DATA_LST() { return m_stAbsorbDataLst; }

    BOOL IsRecruit(){ return m_stDT_HERO_BASE_DATA.byRecuitedFlag; }
    BOOL IsUnLockRecruit(){ return m_stDT_HERO_BASE_DATA.byRecuitedFlag; }
	
	//DT_HERO_BASE_DATA_CLI &GetDT_HERO_BASE_DATA_Equip_CLI()	{	return m_stDT_HERO_BASE_DATA_CLI; }
	//DT_HERO_BASE_DATA_CLI &GetDT_HERO_BASE_DATA_Equip_CLI()	{	return m_stDT_HERO_BASE_DATA_CLI; }
	VOID GetDT_HERO_BASE_DATA_CLI(DT_HERO_BASE_DATA_CLI& stDT_HERO_BASE_DATA_CLI);
	UINT32 GetPower();
	VOID GetExperience(UINT32& dwExperience, UINT32& dwUpgradeExperience);
	DT_HERO_UPGRADE_DATA &GetDT_HERO_UPGRADE_DATA(){	return m_stDT_HERO_UPGRADE_DATA; } 
	VOID GetDT_HERO_EMBATLE_DATA(DT_HERO_EMBATLE_DATA& stDT_HERO_EMBATLE_DATA);
	VOID GetDT_NEW_HERO_EMBATLE_DATA(DT_NEW_HERO_EMBATLE_DATA& stDT_NEW_HERO_EMBATLE_DATA);
	VOID InitBattleAttribute();//计算战斗属性
	VOID SoulPotencyAttribute(UINT16 wSoulID, UINT8 byPotencyLevel, UINT8 byPotencySubLevel);
	VOID AddBattleAttrBySoulPotency(UINT8 byPotencyID, UINT32 dwValue);
	VOID GetDT_HERO_BASE_DATA_EQUIP_CLI(DT_HERO_BASE_DATA_CLI& stDT_HERO_BASE_DATA_CLI, BOOL bGetEquipAttr = FALSE);//bGetEquipAttr为FALSE则武将属性不包括装备属性,否则包括所有属性. 打开布阵全属性，其他不包括装备
	CEquipMap&	GetEquipMap() { return m_mapEquip; }		
	VOID GetUpgrageInfo(DT_HERO_UPGRADE_DATA& stDT_HERO_UPGRADE_DATA);//获取升级信息并清除该记录
	VOID GetUpgrageInfo2(DT_HERO_UPGRADE_DATA2& stDT_HERO_UPGRADE_DATA2);//获取升级信息并清除该记录	
	//CPlayer* GetOwner() { return m_poOwner; }
	SHeroUpgradeProp* GetHeroUpgradeProp();
	SHeroProp* GetHeroProp();
	//////////////////////////////////////////////////////
	UINT16	OnDressEquip(UINT16 wDressItemKindID, UINT16 wDressItemIdx); //装备武器,同时更新属性
	UINT16	OnUndressEquip(UINT16 wUndressItemKindID, UINT16 wUndressItemIdx); //卸下武器,同时更新属性
	UINT16	OnReplaceEquip(UINT16 wDressItemKindID, UINT16 wDressItemIdx, UINT16 wUndressItemKindID, UINT16 wUndressItemIdx);//替换武器,同时更新属性
    VOID GetEquip(DT_HERO_EQUIP_DATA_LST_CLI& stEquipDataList);//获取武将的装备
	VOID GetEquip_v410(DT_HERO_EQUIP_DATA_LST_CLI_v410& stEquipDataList);//获取武将的装备
	VOID GetEquipStrengthenCompose(DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI& stEquipDataList);//获取武将的装备

    VOID GetEquipsStrengthenEnchantInfo_v410(DT_EQUIP_STRENGTHEN_ENCHANT_DATA_LST_CLI& stEquipDataList);// 获取武将的装备（含强化和附魔信息）

	VOID GetEnemyEquip(DT_ENEMY_EQUIP_DATA_LST& stEquipList);//
	VOID GetExperienceInfo(DT_EXPERIENCE_DATA& stExperienceInfo);
	VOID DressMakeEquip(CEquip* poNewEquip);//穿戴上由旧装备制造的新装备，清除旧装备记录
	VOID OnDeleteEquip(CEquip* poEquip);//清除装备回调
	VOID OnAddEquip(CEquip* poEquip);//新增装备回调
	VOID OnStrengthen();//强化回调
	CEquip* GetEquipByPos(UINT8 byEquipPos) { return m_mapEquipPos.GetData(byEquipPos); } //获取穿戴位置装备
		
	//////////////////////////////////////////////////////
	CGodWeapon* NewGodweapon(DT_GODWEAPON_DATA* pstDT_GODWEAPON_DATA);
	CGodWeapon* GetGodweapon() { return m_poGodWeapon; }

	BOOL FreeGodWeapon();//释放器魂回调
	VOID GetDT_GODWEAPON_BATTLE_DATA(DT_GODWEAPON_BATTLE_DATA& stGodweaponInfo);//获得神器战斗信息	

	//////////////////////////////////////////////////////
	DT_STUDY_DATA* GetStudyInfo() { return m_astStudy; }
	DT_STUDY_DATA* GetStudy(UINT8 byPos);
	VOID OnUpgradeStudy();//升级心得回调
	VOID OnDressStudy(); //穿戴心得回调
	BOOL CkHaveStudyAttrKind(UINT8 byStudyAttrKind);//检查是否已存在同属性心得
	VOID AddStudyBattleAttr();
	BOOL CkHaveStudyAttrKind2(UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2);//检查是否已存在同属性心得
	VOID AddStudyBattleAttr2();
	VOID AddBattleAttr(UINT8 byAttrKind, UINT32 dwValue, UINT8 byValueType = 1);
	VOID DecBattleAttr(UINT8 byAttrKink, UINT32 dwValue);
    BOOL GetStudyAttrGrid(UINT8 byStudyAttrKind, UINT8& byGridIdx);//获取属性卦象位置，没有返回FALSE
    void AddJewelrySuitAttr(); // 添加饰品套装属性
    void GetAllHeroAddAttr(C3232Map&); // 添加饰品套装属性
	BOOL ReCheckJewelrySuit(); // 重新检查是否组成一个饰品套装
    BOOL IsDressJewelrySuit(); // 返回：身上的饰品是否组成一个饰品套装
	//////////////////////////////////////////////////////

	UINT16 GetTalent();//获取武将所有资质的总和
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	CDrug * GetDrug(UINT8 byDrugLevel);
	UINT16 UseDrug(SDrugProp* pstDrugProp);//使用丹药
	UINT16 UseExperienceDrug(SExperienceDrugProp* pExperienceDrugProp);//使用丹药
	VOID ClrUseDrugCD();
	VOID GetDrug(DT_DRUG_DATA_LST& stDT_DRUG_DATA_LST);//获取武将的丹药
	CLevel2DrugMap& GetDrugMap() { return m_mapLevel2Drug; }
	VOID GetAttrForAfterUseDrug(DT_BATTLE_ATTRIBUTE& stHeroBattleAttribute, UINT16& wHeroTalent);//获取使用丹药后的武将属性	

	//////////////////////////////////////////////////////
	VOID OnRecuit();//招募回调
	VOID OnDisband();//离队回调
	VOID OnVisit(UINT8 byVisitNum);//拜访回调
	VOID OnUp(UINT8 byFormationIdx);//上阵回调
	VOID OnDown();//下阵回调

	//////////////////////////////////////////////////////

	UINT16 OnUpdateAttr( UINT8 byAttrKind, UINT32 dwPerCountAddValue );  //属性升级

	vector< DT_SKILL_DATA_CLI > GetSkillList();

	VOID GetHeroSkill(DT_SKILL_DATA_LST& stDT_SKILL_DATA_LST); 
	VOID GetHeroSkillCli(DT_SKILL_DATA_CLI& stDT_SKILL_DATA_CLI); 
	VOID GetHeroSkillLstCli(DT_SKILL_DATA_LST_CLI& stDT_SKILL_DATA_LST_CLI);
	VOID GetHeroPanelInfo(DT_HERO_PANEL_CLI& stHeroCli);
	//获得属性的级别
	UINT16 GetAttrLevel( UINT8 byAttrKind );

	//////////////////////////////////////////////////////
	VOID OnJingjieQualityUpgrade();//境界进阶回调

	//获取装备的最大镶嵌宝石个数。
	UINT8 GetMaxMosaicGemNum();

	UINT8 GetMaxStudyLevel(); //心得等级

	inline UINT16 GetHeroID(){return m_stDT_HERO_BASE_DATA.wKindID;};
	
	//获得神器增加的资质
	UINT8 GetGodweaponAddTalent();

	UINT16 UpgradeSkillAction(UINT16 wActionID, UINT16 wActionLevelCli);

	void		AddTransHeroID(UINT16 KindID);
	BOOL		GetTransHeroID(UINT16 KindID);
	UINT16	GetAbsorbKindID(){return m_stAbsorbDataLst.wAbsorbKindID;};
	void		SetAbsorbKindID(UINT16 KindID){m_stAbsorbDataLst.wAbsorbKindID = KindID;};
	VOID GetPowerByFormation(UINT8 byFormationID, UINT8 byFormationLevel, UINT32& dwPower, DT_BATTLE_ATTRIBUTE& stAttrBute);
	BOOL IsFormationHero();	//是否是上阵英雄

	const DT_HERO_SOUL_DATA& GetHeroSoulData();

	// 获得装备的魂
	CSoul* GetEquipSoul(UINT16 usSoulID, UINT32 unIndex);
	// 是否身上已经全部穿上魂
	BOOL IsSoulAllEquip();
	// 
	BOOL IsHaveSameSoulType(CSoul* pSoul);
	// 装备魂
	BOOL EquipSoul(CSoul* pSoul);
	// 脱下魂
	BOOL UnDressSoul(CSoul* pSoul);
	// 
	VOID UpdateSoulData(CSoul* pSoul);

    // 是否是主角
    BOOL IsCoachHero(){ return m_stDT_HERO_BASE_DATA.byCoach; }

    // 是否显示可飞升按钮
    BOOL IsShowFly();

    // 是否已飞升
    BOOL IsFly();

    // 重置飞升
    BOOL ResetFly();

    // 获取飞升经验
    UINT32 GetFlyExp(){ return m_stDT_HERO_BASE_DATA.dwFlyExp; }
    
    UINT16 Fly(PKT_CLIGS_FLY_ACK &stAck);

    // 重置伙伴身上的全部技能，慎用
    void ResetSkill();
    void ResetEquipOwner();

//////内部测试
public:
	UINT16 Change2NewCareer(UINT8 byCareerID);
	////////////////获取武将饰品//////////////////////////////////
    VOID GetJeweryCli(DT_HERO_JEWELRY_DATA_LST_CLI& stJewelryDataList);
	VOID GetJewerySuitCli(DT_JEWELRY_SUIT_CLI& stJewelrySuit);
	VOID GetJeweryData(DT_JEWELRY_DATA_LST& stJewelryDataList);
	UINT16 OnDressJewelryAck(PKT_CLIGS_DRESS_JEWELRY_REQ& stReq, PKT_CLIGS_DRESS_JEWELRY_ACK& stAck);
	UINT16 DressJewelry(PKT_CLIGS_DRESS_JEWELRY_REQ& stReq, PKT_CLIGS_DRESS_JEWELRY_ACK& stAck);
	UINT16 GetOffJewelry(PKT_CLIGS_DRESS_JEWELRY_REQ& stReq, PKT_CLIGS_DRESS_JEWELRY_ACK& stAck);
	UINT16 ReplaceJewelry(PKT_CLIGS_DRESS_JEWELRY_REQ& stReq, PKT_CLIGS_DRESS_JEWELRY_ACK& stAck);
	CJewelryMap& GetJewelryMap(){ return m_mapJewelry; }
	BOOL DressJewelry(CJewelry* poJewelry);
	BOOL UnDressJewelry(CJewelry* poJewelry);
	UINT32 GetJewelryAttr(UINT32 dwKey);
	BOOL GetJewelryStrengthInfo(DT_HERO_JEWELRY_STRENGTH_INFO& stIfno);
	CJewelry* GetJewelryByPos(UINT8 byPosID);
	VOID OnDeleteJewelry(CJewelry* poJewelry);//清除回调

    BOOL InitAbsorb(DT_ABSORB_DATA_LST *pstAbsorbDataLst);

protected:	
	BOOL InitEquip(DT_HERO_EQUIP_DATA_LST* pstDT_EQUIP_DATA_LST);
	BOOL InitDrug(DT_DRUG_DATA_LST* pstDrugDataInfo);	
	BOOL InitGodweapon(DT_GODWEAPON_DATA* pstDT_GODWEAPON_DATA);
	BOOL InitSkill(DT_SKILL_DATA_LST& stDT_SKILL_DATA_LST);
	BOOL InitStudy(DT_STUDY_DATA astStudyInfo[MAX_HERO_STUDY_GRID_NUM]);
	BOOL InitJewelry(DT_JEWELRY_DATA_LST& pstDT_JEWELRY_DATA_LST);

	DT_BATTLE_BASE_ATTRIBUTE BattleAttrCountByLevelTalent(); //等级/资质统计战斗属性，根据等级/资质计算出该等级/资质对应的战斗属性
	VOID XiangqianAttrCount( map<UINT32, UINT32> &mapCurAttr ); //计算镶嵌获得的加成
	VOID XiangqianDecAttrCount( map<UINT32, UINT32> &mapCurAttr ); //扣除镶嵌获得的加成
	DT_BATTLE_BASE_ATTRIBUTE BattleAttrCountByEquip(); //装备统计战斗属性，根据装备计算出该装备对应的战斗属性
	VOID Upgrade();	//升级
	VOID SetUpgradeExperience(); //赋值下等级升级经验
	UINT16 GetDrugTalent();//获取丹药提升的资质
	VOID InitCliData(DT_BATTLE_ATTRIBUTE& stEquipAttr);//初始化客户端信息

	UINT32 GetHPAttackPower(); //攻击气血影响战力
	UINT32 GetHidePower(); //隐藏属性影响战力
	UINT32 GetGodweaponPower(); //神器影响战力
	UINT32 GetSkillPower(); //技能影响战力
	UINT32 GetHurtPower();//新增伤害影响战力
private:
	VOID InitExperience();//重新初始化客户端的经验信息

	//获得属性升级后增加的量
	UINT32 GetAttrUpdateAddition( UINT8 byAttr, BOOL bCoachHero );
	VOID ResetJewelryAttr();
	VOID JewelryCountBySelf(map<UINT32, UINT32>& mapAttr);
	VOID AddEquipAttr(UINT8 byPos,  UINT32 dwValue, UINT8 byJewelryExtID);
	VOID AddStudyAttr(UINT8 byPos,  UINT32 dwValue, UINT8 byJewelryExtID);
	UINT32 AddScienceLevel(UINT32 dwLevel);
	VOID SetFormationData(UINT8 byKindID , UINT32 dwValue );
	VOID SetFormationData();
	VOID SetOpenFormationStatu(BOOL bStatu = FALSE);

	VOID InitSoulData();

private:
	C3232Map m_mapJewelryAttr;				//加成指数
	C3232Map				m_mapJewelryValue;//加成值 Key:EBattleAttributeKind, Value:加成值

	DT_HERO_BASE_DATA		m_stDT_HERO_BASE_DATA;		//武将基本信息
	CEquipMap			m_mapEquip;				//武将装备	
	CEquipPosMap			m_mapEquipPos;			//武将身上装备位置对应的装备
	CLevel2DrugMap		m_mapLevel2Drug;			//武将丹药			
	DT_HERO_UPGRADE_DATA	m_stDT_HERO_UPGRADE_DATA;	//升级信息
	UINT64				m_qwUpgradeExperience;		//用于缓存下等级升级经验，避免每次+经验都查询
	UINT16				m_dwDrugTalent;			//所有丹药资质，每次使用丹药后重新计算
	UINT16				m_dwSkillTalent;			//所有技能资质，每次技能变化后重新计算 .(只对主将有效)
	CGodWeapon*			m_poGodWeapon;			//神器,不一定有
	DT_STUDY_DATA			m_astStudy[MAX_HERO_STUDY_GRID_NUM];//穿戴的心得
	DT_ABSORB_DATA_LST	m_stAbsorbDataLst;
	
	CJewelryMap         m_mapJewelry;			//武将饰品	
	CJewelryPosMap		m_mapJewelryPos;        //武将身上饰品位置对应的装备
	////////////////////////////////////////////////
	DT_ATTR_INC_DATA    m_stFormation;//阵法数据
	BOOL m_bIsOpenFormation; //TRUE为打开阵法计算战力数据， FALSE为正常数据

	DT_HERO_SOUL_DATA	m_HeroSoulData; // 英雄装备战魂信息
	map<UINT64, CSoul>	m_mpSoul;

    UINT16 m_wJewelrySuitDressLvl; // 当前身上的饰品所组成套装的穿戴等级，为0时代表不构成套装（每次穿戴或者卸下饰品、精炼饰品，以及玩家上线都会重新检查套装）
    UINT16 m_wJewelrySuitLvl;      // 当前身上的饰品所组成套装的强化等级，注意，套装强化等级可能为0

private:
	CPlayerGroup *m_poPlayerGroup;

};



#endif //#ifndef _HERO_H_

