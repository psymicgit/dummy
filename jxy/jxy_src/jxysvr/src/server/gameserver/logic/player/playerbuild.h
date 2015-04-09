public:
	CBuild* NewBuild(UINT8 byKindID);//添加建筑	
	CBuild* GetBuild(EBuildKind eBuildKind);
	VOID CkRegUserName();//检测是否需要注册用户名
	VOID GetBuildList(UINT16& wBuildNum , DT_BUILD_DATA_CLI astBuildInfo[MAX_BUILD_NUM]);
public:
    UINT16	GetOpenCruitAck(PKT_CLIGS_OPEN_RECRUIT_ACK& stAck);//获取打开招募所信息
	UINT16	GetOpenCruitAck_v410(PKT_CLIGS_OPEN_RECRUIT_ACK_v410& stAck);//获取打开招募所信息
	UINT16	GetOpenRaceBuildAck(PKT_CLIGS_OPEN_RACE_BUILD_ACK& stAck);//获取打开竞技场信息
	UINT16	GetOpenSimthyAck(PKT_CLIGS_OPEN_SMITHY_ACK& stAck);//获取打开铁匠铺信息
    UINT16  GetOpenSmithyAck_v410(PKT_CLIGS_OPEN_SMITHY_ACK_v410& stAck);

	UINT16 OpenMainTown(UINT16& wCurLevel,UINT8& byCanCrowFlag, UINT32& dwCrownCoin, UINT32& dwCrownGold, UINT32& dwCrownJingJie,
		UINT32& dwLevyCountDown, UINT32& dwLevyCoin, UINT32& dwLevyJingJie, UINT8& byLevyNum, UINT8 &byTotalLevyNum); //打开主城
	UINT16 Crown(UINT8& byCanCrowFlag);//收获
	UINT16 Levy(UINT32& dwLevyCountDown,UINT8& byLevyNum);//征收

	UINT16 OpenDogFeces(UINT8 byType, UINT8 &byNum, DT_DOGFECES_BUILD_TIP astDogFecesBuildTip[MAX_DOGFECES_BUILD_TIP_NUM], DT_FUNC_PROMPT_DATA &stPromptInfo);
    UINT16 OpenSmithyIcon(UINT8 &byNum, DT_DOGFECES_BUILD_TIP astDogFecesBuildTip[MAX_DOGFECES_BUILD_TIP_NUM], DT_FUNC_PROMPT_DATA &stPromptInfo);

	CShmemMap<UINT8, CBuild>& GetBuildMap(){ return m_mapKind2Build; }
protected:
	BOOL	InitBuild(DT_BUILD_DATA_LIST& stBuildData);//初始化建筑,必须先初始化建筑，其他模块根据建筑判断是否开放
	VOID	AddBuildFunctionGuide(UINT8 byKindID);//添加建筑功能引导
	UINT32	GetLevyCountDown();//获取下次征收倒计时
	BOOL	GetDT_RECRUIT_HERO_DATA(CHero* poHero, DT_RECRUIT_HERO_DATA& stDT_RECRUIT_HERO_DATA);
    BOOL	GetDT_RECRUIT_HERO_DATA_v410(CHero* poHero, DT_RECRUIT_HERO_DATA_v410&);

    BOOL	GetDT_RECRUIT_HERO_DATA_OF_GM(CHero* poHero, DT_RECRUIT_HERO_DATA& stDT_RECRUIT_HERO_DATA);
    BOOL	GetDT_RECRUIT_HERO_DATA_OF_GM_v410(CHero* poHero, DT_RECRUIT_HERO_DATA_v410& stDT_RECRUIT_HERO_DATA);

private:
	CShmemMap<UINT8, CBuild>	m_mapKind2Build;		//拥有的建筑,key为建筑类型，为了查找,同种类型不可重复创建	
	//Value1 ,对于主城为收获时间
	//Value2 ,对于主城为征收时间
	//Value3 ,对于主城为征收的次数
	SMaintownLevyProp*	m_pstMaintownLevyProp;