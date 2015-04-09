///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\rockmoneymedal.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ26ÈÕ 21:44:36
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _rockmoneymedal_h_
#define _rockmoneymedal_h_
#include "logic/medal/medal.h"

class CRockMoneyMedal : public CNoneTimeMedal
{
public:
	BOOL CheckActivate();
	virtual VOID GetMedalCli(DT_MEDAL_DATA_CLI& stCli);
};
#endif //_rockmoneymedal_h_