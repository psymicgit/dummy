public:
	CTask* GetTask(UINT8 byLineIdx);

	CLineIdx2TaskShmemMap& GetGiveTask(){ return m_mapGiveTask; };//获取给予未接受的任务
	CLineIdx2TaskShmemMap& GetAcceptTask() { return m_mapAcceptTask; } ;//获取接受未完成的任务
	CLineIdx2TaskShmemMap& GetEncourageTask(){ return m_mapEncourageTask; };//获取完成未领取奖励的任务
	CLineIdx2TaskShmemMap& GetFinishTask(){ return m_mapFinishTask; };//获取已领取奖励已结束的任务
	CLineIdx2TaskShmemMap& GetAllTask() { return m_mapAllTask; } //获取所有的任务
	CTask* GetTaskByLine(UINT8 byTaskLine);

	CLineIdx2TaskShmemMap& GetChangeTask() { return m_mapChangeTask; }	
	VOID RecordChangeTask(CTask* poTask);//记录变化的任务
	BOOL CkNeedTaskBaseInfo(CTask* poTask);//检查是否是领取奖励任务且需下发任务基本信息	

	VOID	GetDT_TASK_DATA_LIST_CLI(DT_TASK_DATA_LIST_CLI& stTaskInfoList, BOOL bLogin = FALSE);	
	VOID	GetDT_TASK_DATA_LIST_CLI_Change(DT_TASK_DATA_LIST_CLI& stTaskInfoList);	
	VOID	GetDT_TASK_COND_RECORD_LIST(DT_TASK_COND_RECORD_LIST& stTaskRecordList);	

	UINT8	GetMaxUnlockCommonInstanceID(DT_INSTANCE_ID& stDT_INSTANCE_ID, BOOL bAcceptTask = FALSE);//返回值为当前已领取奖励任务是否新解锁副本
	VOID	SetLastMainLineTaskProp(UINT32 dwTaskIdx) { m_dwLastAcceptMainLineTaskIdx = dwTaskIdx;};//上一个接受的主线任务，若当前主线任务为接受状态，则为该任务

	BOOL CkTaskFinish(UINT8 byTaskLine, UINT32 dwTaskIdx);//检查任务是否已经完成，已完成返回TRUE

	VOID GetUnlockNewInstance(UINT8& byUnlockNewInstanceFlag, UINT8& byHaveNewInstanceFlag, DT_INSTANCE_ID& stUnlockCommonInstanceID, DT_INSTANCE_DATA_CLI& stNewCommonInstanceID);//获取解锁新副本信息.方法在GetDT_TASK_DATA_LIST_CLI/GetDT_TASK_DATA_LIST_CLI_Change之前调用
protected:
	BOOL	InitTask(DT_TASK_DATA_LIST& stTaskData);//初始化任务
	CTask*	InitTask(DT_TASK_DATA* pstDT_TASK_DATA);
	VOID	GetDT_TASK_DATA_CLI(CTask* poTask, DT_TASK_DATA_CLI& stDT_TASK_DATA_CLI, BOOL bLogin = FALSE);
private:
	CLineIdx2TaskShmemMap	m_mapAllTask;			//所有任务
	CLineIdx2TaskShmemMap	m_mapGiveTask;			//给予未接受的任务
	CLineIdx2TaskShmemMap	m_mapAcceptTask;		//接受未完成的任务	
	CLineIdx2TaskShmemMap	m_mapEncourageTask;		//完成未领取奖励的任务
	CLineIdx2TaskShmemMap	m_mapChangeTask;		//变化的任务，所有变化的任务(包含m_mapNBIEncourageTask,不包含给予的任务)，每次返回给客户端后清除
	CLineIdx2TaskShmemMap	m_mapNBIEncourageTask;	//变化的任务，需要基本信息的奖励状态任务(用于刚接受的任务立即满足完成条件的任务)	
	CLineIdx2TaskShmemMap	m_mapFinishTask;		//已结束任务
	UINT32					m_dwLastAcceptMainLineTaskIdx;	//上一个接受的主线任务(用于判断解锁副本)，若当前主线任务为接受状态，则为该任务。
