///<------------------------------------------------------------------------------
//< @file:   logic\pet\pet.h
//< @author: yewenhui10@qq.com
//< @date:   2014年8月6日 16:42:00
//< @brief:  宠物对象
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------
#ifndef _PET_H_ 
#define _PET_H_

#include <logic/battle/battleattrobj.h>
#include "logic/pet/petdef.h"
#include "logic/pet/petskill.h"

class PetStarCfg;
class CPlayer;
class CPetNeiDan;

class CPet :
	public CBattleAttrObj
{
public:
	CPet();
	~CPet();
	//////////////////////////////////战斗相关////////////////////////////////////////////
	VOID			StartPetBattle();
	UINT32&			GetLastHurt(){ return m_dwLastHurt;}
	CPetSkill*& GetSecondHrut(){ return m_poSecondHurt; }
	BOOL&			GetSkillCritical() { return m_bSkillCritical;}
	///////////////////////////////////////战斗相关////////////////////////////////////////
	BOOL			Init(const DT_PET_DATA& rPetData, CPlayer* pPlayer, const DT_PET_NEIDAN_DATA_LST& stNeiDanData);
	DT_PET_DATA&	GetDT_PET_DATA();
	DT_PET_NEIDAN_DATA_LST& GetDT_NeiDan_Data() { return m_stNeiDanData; }
	const DT_PET_DATA&	GetDT_PET_DATA() const;

	// todo
	VOID			SetPetId(UINT16 usPetId);
	UINT16			GetPetId() const {
		return m_PetData.wPetId;
	}

	VOID			FillDT_PET_CLI_DATA(DT_PET_CLI_DATA& rCliPetData) const;

	// 被吞噬的经验
	UINT32			GetBeAteExp() const;
	// 获取灵力
	UINT32			GetLingli() const;
	// 增加经验
	VOID			AddPetExp(UINT32 unAddPetExp, BOOL bUpStarLv = FALSE);
	// 当前资质
	UINT32			GetCurPsychic() const;
	// 最大资质
	UINT32			GetMaxPsychic() const;
	UINT16			GetLevel() const ;
	// CBattleAttrObj
	virtual inline UINT8 GetFormationIdx();

	const PetStarCfg* GetPetStarCfg() const;
	CPetSkill* GetCurSkill() ;
	BOOL EnterNextSkill();
	//获取宠物评分
	UINT64 GetPetScore() const ;
	//获取宠物档次
	UINT8 GetPetGrade()const ;

	BOOL DressNeiDan(CPetNeiDan* poNeiDan);
	CPetNeiDan* UnDressNeiDan(UINT8 byPosType);
	
	VOID GetNeiDanCli(DT_PET_NEIDAN_CLI& stNeiDanCli);
	VOID GetNeiDanMark(DT_NEIDAN_PET_MARK_LST_DATA& stMark);
protected:
	// CBattleAttrObj
	virtual VOID	InitBattleAttribute();
	VOID InitSkill();
private:
	DT_PET_DATA		m_PetData;
	DT_PET_NEIDAN_DATA_LST m_stNeiDanData;//内丹信息
	map<UINT8, CPetNeiDan*> m_mapNeiDan;//内丹



	UINT32			m_dwLastHurt;//上次造成的伤害
	CPetSkill*		m_poSecondHurt;//二次伤害标识
	BOOL			m_bSkillCritical;//技能标识
	vector<CPetSkill*> m_vecSkill;		//技能数组
	UINT32			m_dwSkillIdx;//技能标识
	UINT32			m_dwRoundIdx;//当前攻击回合
	UINT32			m_dwCurLingMaiNum;//当前激活的灵脉
};

#endif //_PET_H_
