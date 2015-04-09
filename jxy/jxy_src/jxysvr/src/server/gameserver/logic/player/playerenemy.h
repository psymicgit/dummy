public:
	CEnemy* AddEnemy(UINT32 dwEnemyID);//添加仇敌，若已存在，则交战次数++	
	UINT16 GetEnemyInfo(UINT16 wStartIdx, UINT16& wTotalNum, UINT16& wCurNum, DT_ENEMY_DATA_CLI astEnemyInfo[MAX_GET_ENEMY_NUM]);
	CEnemy* GetEnemy(UINT32 dwEnemyID);
	UINT16 DelEnemy(UINT32 dwEnemyID, UINT16& wStartIdx,UINT16& wTotalNum, UINT16& wCurNum, DT_ENEMY_DATA_CLI astEnemyInfo[MAX_GET_ENEMY_NUM]);
	vector<CEnemy *> GetEnemyList();
protected:
	BOOL InitEnemy(DT_ENEMY_DATA_LIST& stEnemyData);//初始化仇敌
	BOOL  OnDelEnemy(UINT32 dwEnemyID);//清除仇敌
private:
	CShmemMap<UINT32, CEnemy>		m_mapEnemy;	//key为PlayerID
	//map<UINT64, CEnemy*>			m_mapTimeEnemy;//key为挑战时间，若重复+1
	vector<CEnemy*>					m_vecEnemy;//
	
	