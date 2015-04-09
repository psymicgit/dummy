public:
	CHero* GetCoachHero() { return m_poCoachHero; }
	CHero* GetHero(UINT16 wKindID);
	CHero* NewHero(UINT16 wHeroKindId);//添加武将
	CHero* NewHeroNoBuild(UINT16 wHeroKindId);//添加武将
	BOOL OnUpgrade(UINT16 wLevel);//主将升级回调
	VOID AllotExperience(UINT32 dwExperience);//给玩家不同武将分配经验
	inline UINT16 GetLevel();
	inline UINT16 GetCoachHeroKindID();
	VOID SetCoachHero(CHero* poCoachHero);
	
	CKindID2HeroShmemMap& GeHeroMap() { return m_mapKind2Hero; }
	CKindID2HeroShmemMap& GetAllCarryHeroMap() { return m_mapKind2HeroAllCarry; }
	CFormationHeroShmemMap& GetFormationHeroMap() { return m_mapFormationHero; }
	BOOL GetDT_ENEMY_HERO_DATA(UINT8& byEnemyHeroNum, DT_ENEMY_HERO_DATA astEnemyInfoList[]);
	VOID GetHeroUpgradeInfo(UINT8& byHeroNum, DT_HERO_UPGRADE_DATA astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM]);
	VOID GetMainHeroUpgradeInfo(DT_HERO_UPGRADE_DATA& astHeroUpgradeInfo);
	VOID GetPassInstanceHeroUpgradeInfo(UINT32 dwExp, UINT8& byHeroNum, DT_HERO_UPGRADE_DATA astHeroUpgradeInfo[MAX_FORMATION_IDX_NUM]);
	VOID GetExperienceInfo(DT_EXPERIENCE_DATA& stExperienceInfo);
	BOOL CkCarryHeroFull();//检测是否达到上限携带 , TRUE为达到上限

	/*
	UINT16 OnUpFormation(UINT16 wHeroKindID, UINT8 byFormationIdx);//上阵回调
	UINT16 OnDownFormation(UINT16 wHeroKindID);//下阵回调
	UINT16 OnReplaceFormation(UINT8 byFormationIdx, UINT16 wUpHeroKindID, UINT16 wDownHeroKindID);//替换回调
	UINT16 OnExchangeFormation(UINT8 byFormationIdx, UINT16 wHeroKindID1, UINT16 wHeroKindID2);//交换回调
	*/

	UINT16 GetCarryHeroInfo(UINT16& wHeroNum, DT_HERO_BASE_DATA_CLI astHeroInfoLst[MAX_HERO_NUM], 
		DT_HERO_EQUIP_DATA_LST_CLI *poEquipDataLst=NULL, DT_ATTR_ENABLE_UPDATE_INFO *poAttrEnableUpdateLst=NULL,
		DT_SKILL_DATA_CLI *poSkillInfo=NULL );

	UINT16 GetCarryHeroInfoNew(UINT16& wHeroNum, DT_HERO_PANEL_CLI astHeroInfoLst[MAX_HERO_NUM], 
		DT_HERO_EQUIP_DATA_LST_CLI *poEquipDataLst=NULL, DT_ATTR_ENABLE_UPDATE_INFO *poAttrEnableUpdateLst=NULL,
		DT_SKILL_DATA_LST_CLI *poSkillInfo=NULL, DT_HERO_JEWELRY_DATA_LST_CLI* poJewelryInfo = NULL, DT_HERO_SOUL_DATA_LST_CLI* pSoulInfo = NULL, 
        DT_HERO_SOUL_POTENCY_DATA_LST_CLI* poSoulPotencyInfo = NULL, DT_JEWELRY_SUIT_LIST_CLI *pJewelrySuitInfo = NULL);

	//////////////////////////
    UINT16 Recuit(UINT16 wHeroKindID, DT_HERO_BASE_DATA_CLI& stDT_HERO_BASE_DATA_CLI);//邀请招募武将
	UINT16 UpgradeHero(UINT16 wHeroKindID, PKT_CLIGS_UPGRADE_HERO_ACK&);//进阶武将

    // 是否已达成进阶条件（不检查所需进阶材料）
    BOOL IsUpgradable(UINT16 wHeroKindID);//进阶武将

    // 获取进阶伙伴
    CHero* GetUpgradeHero(UINT16 wHeroKindID);

	UINT16 Visit(UINT16 wHeroKindID, UINT8 bySizeFlag, UINT8 byGoldFlag, UINT8 &byWinFlag, UINT8& byNeedVisitNum, UINT8& byCurVisitNum, UINT16 &wUpgradeVisitNum, UINT32 &dwReturnCoin);//拜访武将
	UINT16 Disbang(UINT16 wHeroKindID);//遣散武将
	UINT16 DelItem(UINT16 wItemKindID);

    VOID   GMDisbang();
    UINT16 GMRecuit(UINT16 wHeroKindID, UINT64 qwIndex = 0);//通过GM邀请招募武将
	UINT16 OpenExperienceTransfer(UINT16 wHeroID, PKT_CLIGS_OPEN_EXPERIENCE_TRANSFER_ACK &stAck);
	UINT16 ExperienceTransfer(UINT16 wHeroID, UINT16 wTransHeroID, UINT8 byGold, DT_HERO_BASE_DATA_CLI &stHeroInfo, DT_HERO_BASE_DATA_CLI &stTransHeroInfo);
	UINT16 GetHeroAllCarry(){return m_mapKind2HeroAllCarry.Size();};

	UINT16 GetPlayerInfoEquip(PKT_CLIGS_PLAYER_INFO_EQUIP_ACK& stAck);
	UINT16 JewelryCompare(PKT_CLIGS_JEWELRY_COMPARE_REQ& stReq, PKT_CLIGS_JEWELRY_COMPARE_ACK& stAck);
	VOID GetHeroPanelLstCli(DT_HERO_PANEL_LST_CLI& stHeroLstCli);

	VOID GetPlayerAllHero(vector<CHero*>& setHero);

/////内部测试
public:
	UINT16 Change2NewCareer(UINT8 byCareerID);
	DT_HERO_BASE_DATA& GetOldCareerBeforeChange() { return m_stOldCareerBeforeChange; }
	UINT16 OpenPlayerInfoSkill(PKT_CLIGS_PLAYER_INFO_SKILL_ACK& stAck);
	VOID GetAllCarryHeroAttr( DT_HERO_ATTR_LST_INFO& stAttrInfo, UINT8* abyJobType);
protected:
	BOOL	InitHero(DT_HERO_DATA_LIST& stHeroData);//初始化武将
private:
	//给后面的数组参数指定填充的索引
	BOOL	FillDataListByIndex( UINT16 wIndex, CHero &oHero, DT_HERO_BASE_DATA_CLI *poBaseDataLst, 
		DT_HERO_EQUIP_DATA_LST_CLI *poEquipDataLst, DT_ATTR_ENABLE_UPDATE_INFO *poAttrEnableUpdateLst,
		DT_SKILL_DATA_CLI *poSkillInfo);

	//给后面的数组参数指定填充的索引
	BOOL	FillDataListByIndexNew( UINT16 wIndex, CHero &oHero, DT_HERO_PANEL_CLI *poBaseDataLst, 
		DT_HERO_EQUIP_DATA_LST_CLI *poEquipDataLst, DT_ATTR_ENABLE_UPDATE_INFO *poAttrEnableUpdateLst,
		DT_SKILL_DATA_LST_CLI *poSkillInfo, DT_HERO_JEWELRY_DATA_LST_CLI* poJewelry, DT_JEWELRY_SUIT_LIST_CLI *pJewelrySuitInfo);

	BOOL	GetHeroData( CHero &oHero, DT_HERO_BASE_DATA_CLI *poBaseData, 
		DT_HERO_EQUIP_DATA_LST_CLI *poEquipData, DT_ATTR_ENABLE_UPDATE_INFO *poAttrEnableUpdateLst,
		DT_SKILL_DATA_CLI *poSkillInfo );

	BOOL	GetHeroDataNew( CHero &oHero, DT_HERO_PANEL_CLI *poBaseData, 
		DT_HERO_EQUIP_DATA_LST_CLI *poEquipData, DT_ATTR_ENABLE_UPDATE_INFO *poAttrEnableUpdateLst,
		DT_SKILL_DATA_LST_CLI *poSkillInfo, DT_HERO_JEWELRY_DATA_LST_CLI* poJewelry, DT_JEWELRY_SUIT_CLI *pJewelrySuit);

	bool FormationHeroSortByCarryIndex( UINT16 byHeroKind1, UINT16 byHeroKind2 );
private:
	CKindID2HeroShmemMap 	m_mapKind2Hero;			//拥有的所有武将(包括在招募馆的),key为武将类型，为了查找,同种类型不可重复创建
	CKindID2HeroShmemMap	m_mapKind2HeroAllCarry;	//所有携带的武将，包括上阵的，不包括在招募馆的
	CFormationHeroShmemMap	m_mapFormationHero;		//所有上阵的武将
	CHero*					m_poCoachHero;			//主将
	DT_HERO_BASE_DATA		m_stOldCareerBeforeChange;//职业转换前的职业信息
	//vector< UINT16 > m_vecCostarIndexByAllCarry; //上阵伙伴在所有携带者中的顺序(只有伙伴，不包括主角)
	