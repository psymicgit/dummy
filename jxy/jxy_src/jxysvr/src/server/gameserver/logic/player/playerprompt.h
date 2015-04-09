public:
	VOID  GetPrompt(DT_FUNC_PROMPT_DATA& stPromptInfo);
	VOID  GetSideActiviPrompt(DT_ACTIVI_SIDE_PROMPT_DATA_LST& stPromptInfo);

	VOID GetMaintownPrompt1(DT_FUNC_COMMON_PROMPT& stMaintownPrompt);//获取主城收获提示
	VOID GetMaintownPrompt2(DT_FUNC_COMMON_PROMPT& stMaintownPrompt);//获取主城提示提示
    VOID GetSmithyPrompt(DT_FUNC_COMMON_PROMPT& stSmithyPrompt);//获取铁匠铺提示
    VOID GetStrengthenPrompt(DT_FUNC_COMMON_PROMPT& stStrengthenPrompt);//获取铁匠铺强化提示
    VOID GetEnchantPrompt(DT_FUNC_COMMON_PROMPT& stEnchantPrompt);//获取铁匠铺附魔提示
	VOID GetEquipMakePrompt(DT_FUNC_COMMON_PROMPT& stEquipMakePrompt);//获取铁匠铺装备制作提示
	VOID GetRecruitPrompt(DT_FUNC_COMMON_PROMPT& stRecruitPrompt); //获取招募馆提示信息
	VOID GetPVPPrompt(DT_FUNC_COMMON_PROMPT& stPvpPrompt); //比武提示信息
	VOID GetGodweaponPrompt(DT_FUNC_COMMON_PROMPT& stGodweaponPrompt); //神器提示信息	
	VOID GetMoneyTreePrompt(DT_FUNC_COMMON_PROMPT& stMoneyTreePrompt); //招财树提示信息	
	VOID GetCropPrompt(DT_FUNC_COMMON_PROMPT& stDT_CROP_PROMPT); //种植提示信息
	VOID GetSciencePrompt(DT_FUNC_COMMON_PROMPT& stDT_SCIENCE_PROMPT); //科技提示信息	
	VOID GetPlunderPrompt(DT_FUNC_COMMON_PROMPT& stPlunderPrompt); //掠夺提示信息
	VOID GetEmprisePrompt(DT_FUNC_COMMON_PROMPT& stPlunderPrompt); //冒险提示信息
	VOID GetJingJiePrompt(DT_FUNC_COMMON_PROMPT& stJingJiePrompt); //境界提示信息
	VOID GetActivityCenterPrompt(DT_FUNC_COMMON_PROMPT& stPrompt); //活动中心提示信息
	VOID GetActivityEncouragePrompt(DT_FUNC_COMMON_PROMPT& stPrompt); //活动领取奖励提示信息

	VOID GetRacePrompt(DT_FUNC_COMMON_PROMPT& stPvpPrompt); //比武提示信息
	VOID GetRdChallengePrompt(DT_FUNC_COMMON_PROMPT& stPvpPrompt); //比武提示信息
	
	VOID GetElitePrompt(DT_FUNC_COMMON_PROMPT& stPlunderPrompt); //精英提示信息
	VOID GetSoulInstancePrompt(DT_FUNC_COMMON_PROMPT& stPlunderPrompt);	//战魂副本提示信息
	VOID GetClimbTowerPrompt(DT_FUNC_COMMON_PROMPT& stPlunderPrompt); //爬塔提示信息
	VOID GetYabiaoPrompt(DT_FUNC_COMMON_PROMPT& stPlunderPrompt); //押镖提示信息
	VOID GetFactionPrompt(DT_FUNC_COMMON_PROMPT& stFactionPrompt);//门派提示信息
	VOID GetJewelryPrompt(DT_FUNC_COMMON_PROMPT& stFactionPrompt);//饰品提示信息
	/////////////////////////////////////活动侧边提示/////////////////////////////////////////
	VOID GetHotSpringPrompt(DT_ACTIVI_SIDE_PROMPT_DATA_LST& stHotSpringPrompt);//温泉提示信息
	VOID GetShadePrompt(DT_ACTIVI_SIDE_PROMPT_DATA_LST& stShadePrompt);//暗部提示信息
	//////////////
	BOOL HaveBetterHero();//是否有更强伙伴
	VOID GetNewEquipPrompt(DT_NEW_EQUIP_PROMPT_DATA& stNewEquipPrompt); //获取新装备更强提示信息
	UINT16 GetBoxGoodsPrompt();//获取可使用宝箱ID


	VOID GetCoinResourcePrompt(DT_RESOURCES_PROMPT_DATA &stRsPromptData);
	VOID GetStoryResourcePrompt(DT_RESOURCES_PROMPT_DATA &stRsPromptData);
	VOID GetScienceResourcePrompt(DT_RESOURCES_PROMPT_DATA &stRsPromptData);
	VOID GetJingJieResourcePrompt(DT_RESOURCES_PROMPT_DATA &stRsPromptData);

	//////////////////////////////////////////////////////////////////////////
	VOID GetMainCoinResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);
	VOID GetCropCoinResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);
	VOID GetMoneyTreeCoinResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);
	VOID GetSellCoinResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);
	VOID GetMonsterCoinResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);
	VOID GetBuddhistSutrasCoinResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);

	//////////////////////////////////////////////////////////////////////////
	VOID GetCropStoryResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);

	//////////////////////////////////////////////////////////////////////////
	VOID GetMeltScienceResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);
	VOID GetCropScienceResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);

	//////////////////////////////////////////////////////////////////////////
	VOID GetFirstBossJingJieResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);
	VOID GetBuddhistSutrasJingJieResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);
	VOID GetRaceJingJieResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);
	VOID GetCropJingJieResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);
	VOID GetBaishengJingJieResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);
	VOID GetShangXiangJingJieResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt);

	UINT16 SetIgnorePrompt(UINT8 byActiviID, PKT_CLIGS_PROMPT_IGNORE_ACK& stAck);
protected:	
	BOOL IsScienceCanGet( UINT16 wAppointScienceLevel, UINT8 byScienceKind ); //是否指定科技属性能升到指定的级别

	BOOL CkCanPromote(CEquip* poEquip, DT_NEW_EQUIP_PROMPT_DATA& stNewEquipPrompt,  BOOL bJunior);//装备是否可提升伙伴
	BOOL CkCanPromote(CHero* poHero, CEquip* poEquip, DT_NEW_EQUIP_PROMPT_DATA& stNewEquipPrompt);//装备是否可提升伙伴
private:
	vector<CEquip*>				m_vecNewEquip;//

	const static INT32 ms_nPromotYabiaoCanYabiao=1;
	const static INT32 ms_nPromotYabiaoRunning=2;
	const static INT32 ms_nPromotYabiaoSucceed=3;
	const static INT32 ms_nPromotYabiaoCanJiebiao=4;
	


