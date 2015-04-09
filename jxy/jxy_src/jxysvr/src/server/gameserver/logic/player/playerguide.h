public:
	//检测是否需做引导，若是返回TRUE,并做需引导记录
	BOOL	CKGuide(EGuideFunction eGuideFunction);//dwGuideRecord前4位为引导记录，后4位为已开启但为完成的引导

	//添加已引导记录，并清除开启引导记录
	VOID	AddGuideRecord(EGuideFunction eGuideFunction);

protected:
	VOID	InitmAllGuildFunction();

protected:
	static UINT8	m_abyAllGuildFunction[MAX_GUIDE_NUM];//所有的引导功能