public:
	 VOID SetPower(UINT32 dwPower);
	 VOID SetFirstAttack(UINT32 dwFirstAttack);
	 VOID SetTodayFirstLoginTime(UINT64 qwTodayFirstLoginTime);
	 VOID SetBagOpenNum(UINT8 byOpenNum);

	 VOID AddCoin(UINT32 dwCoin, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 //dwRMB 金额单位为分
	 VOID AddGold(UINT32 dwGold, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="", UINT32 dwRMB = 0);
	 VOID AddStory(UINT32 dwStory, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 VOID AddPhyStrength(UINT16 wPhyStrength, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 VOID AddScience(UINT32 dwScience, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 VOID AddBlueGas(UINT32 dwGas, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 VOID AddPurpleGas(UINT32 dwGas, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 VOID AddJingJie(UINT32 dwJingJie, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
     VOID AddMedal(UINT32 dwMedal, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment=""); // 勇气勋章
	 VOID AddTotalAmount( UINT32 dwAmount );

	 VOID SetJingJie(UINT32 dwJingJie, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");

	 VOID DecCoin(UINT32 dwCoin, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 VOID DecGold(UINT32 dwGold, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 VOID DecStory(UINT32 dwStory, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 VOID DecPhyStrength(UINT16 wPhyStrength, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 VOID DecScience(UINT32 dwScience, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 VOID DecBlueGas(UINT32 dwGas, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 VOID DecPurpleGas(UINT32 dwGas, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 VOID DecJingJie(UINT32 dwJingJie, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
     VOID DecMedal(UINT32 dwMedal, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment=""); // 勇气勋章

	 VOID AddStudyCli(UINT32 dwCli, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");
	 VOID DecStudyCli(UINT32 dwCli, UINT16 wOpType, UINT64 qwParam1=0, UINT64 qwParam2=0, UINT64 qwParam3=0, UINT64 qwParam4=0, string strComment="");

	 

     UINT64 GetMedal(); // 获取勇气勋章

	 //只给GM工具使用
	 VOID SetVipLevel( UINT8 byVipLevel, UINT32 dwTotalAmount = 0);

	 VOID DecPayByGM(UINT32 dwDecRMB);

	 void CkToday();
	 
	 VOID AddFlowerNum( UINT32 dwFlowerNum );

