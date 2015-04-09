public:	
	inline SBattleEncourageProp& GetBattleEncourage() { return m_stBattleEncourage; }
	VOID GetBattleEncourage(DT_BATTLE_ENCOURAGE_DATA& stEncourageInfo, DT_DATA_AFTER_BATTLE_ENCOURAGE* pstAfterBattleEncourage = NULL);//获取战斗奖励并清除记录	
	VOID ResetBattleAttr(BOOL bResetHero = FALSE);//重置战斗属性(当前值也将赋值为最大值)
	//VOID ResetConvertBattleAttr();//重置换算战斗属性(当前值按百分比换算)，用于战斗过程中强化/布阵
	VOID GetUnreadBattleLogNum(UINT8& byBattleLogTypeNum, DT_UNREAD_BATTLE_LOG_DATA_CLI astUnreadBattleLogInfo[MAX_BATTLE_LOG_TYPE_NUM]);
public:
	//CRace* AddRace(DT_RACE_BASE_DATA& stDT_RACE_BASE_DATA);
	//inline CRace* GetRace() { return m_poRace; }
	UINT32 GetRank() ;

public:
	//CPlunder* NewPlunder();//开放掠夺
	//inline CPlunder* GetPlunder() { return m_poPlunder; }
	inline VOID SetPlunder(CPlunder* poPlunder) { m_poPlunder = poPlunder; } //注意，只能是CPlunder::Init中调用

public:
	
	inline VOID	RecordLastBattleAck5(PKT_CLIGS_BATTLE_ACK5& stBattleAck) { memcpy(&m_stLastBattleAck5, &stBattleAck, sizeof(PKT_CLIGS_BATTLE_ACK5)); }
    inline VOID	RecordLastBattleAck3(PKT_CLIGS_BATTLE_ACK3& stBattleAck) { memcpy(&m_stLastBattleAck3, &stBattleAck, sizeof(PKT_CLIGS_BATTLE_ACK3)); }
	inline VOID	RecordLastBattleAck4(PKT_CLIGS_BATTLE_ACK4& stBattleAck) { memcpy(&m_stLastBattleAck4, &stBattleAck, sizeof(PKT_CLIGS_BATTLE_ACK4)); }
	
	inline PKT_CLIGS_BATTLE_ACK5* GetLastBattleAck5() { return m_stLastBattleAck5.stBattleInfo.byBattleRoundNum != 0 ? &m_stLastBattleAck5 : NULL; }
    inline PKT_CLIGS_BATTLE_ACK3* GetLastBattleAck3() { return m_stLastBattleAck3.stBattleInfo.byBattleRoundNum != 0 ? &m_stLastBattleAck3 : NULL; }
	inline PKT_CLIGS_BATTLE_ACK4* GetLastBattleAck4() { return m_stLastBattleAck4.stBattleInfo.byBattleRoundNum != 0 ? &m_stLastBattleAck4 : NULL; }

	C3232Map& GetJewelryAttr(){ return m_mapJewelryAttr; }
	UINT32 GetJewelryAttr(UINT32 dwKey);

public:
    VOID	InitBattleAttr(BOOL bResetHero);//初始化战斗属性

protected:
	VOID    InitJewelryAttr();
	//BOOL	InitRace(DT_RACE_DATA& stDT_RACE_DATA);//初始化竞技
	//BOOL	InitPlunder(DT_PLUNDER_DATA);
	//BOOL	InitRdChallenge(DT_RANDOM_CHALLENGE_DATA& stDT_RANDOM_CHALLENGE_DATA);
	//BOOL	InitWorship(DT_WORSHIP_DATA_LST& stWorshipData);


protected:

private:
	SBattleEncourageProp	m_stBattleEncourage;	//战斗奖励信息
	//CRace*				m_poRace;			//竞技场信息
	//CPlunder*			m_poPlunder;			//掠夺	
	//CRdChallenge*			m_poRdChallenge;		//随机挑战
	//CWorship*			m_poWorship;			//膜拜
	C3232Map				m_mapJewelryAttr;//加成指数
	PKT_CLIGS_BATTLE_ACK5	m_stLastBattleAck5;//上次的战斗记录
    PKT_CLIGS_BATTLE_ACK3   m_stLastBattleAck3;//上次的战斗记录
	PKT_CLIGS_BATTLE_ACK4   m_stLastBattleAck4;//上次的战斗记录

	DECLARE_BUILD_INIT_DESTORY(Worship, CWorship, DT_WORSHIP_DATA_LST)
	DECLARE_BUILD_INIT_DESTORY(RdChallenge, CRdChallenge, DT_RANDOM_CHALLENGE_DATA)
	DECLARE_BUILD_INIT_DESTORY(Race, CRace, DT_RACE_DATA)
	DECLARE_BUILD_INIT_DESTORY(Plunder, CPlunder, DT_PLUNDER_DATA)

	



	