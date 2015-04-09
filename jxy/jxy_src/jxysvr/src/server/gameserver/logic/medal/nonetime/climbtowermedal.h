///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\climbtower.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ25ÈÕ 11:37:36
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _climbtowermedal_h_
#define _climbtowermedal_h_
#include "logic/medal/medal.h"

class CClimbTowerMedal : public CNoneTimeMedal
{
public:
	VOID GetMedalCli(DT_MEDAL_DATA_CLI& stCli);
	virtual BOOL CheckActivate();
};
#endif //_climbtower_h_