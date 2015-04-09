///<------------------------------------------------------------------------------
//< @file:   medal\resource\phystrengthmedal.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ23ÈÕ 15:31:15
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _phystrengthmedal_h_
#define _phystrengthmedal_h_
#include "logic/medal/medal.h"

class CPhystrengthMedal : public CNoneTimeMedal
{
public:
	BOOL CheckActivate();
	virtual VOID GetMedalCli(DT_MEDAL_DATA_CLI& stCli);
};
#endif //_phystrengthmedal_h_