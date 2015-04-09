///<------------------------------------------------------------------------------
//< @file:   medal\limittime\racemedal.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ18ÈÕ 19:54:25
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _racemedal_h_
#define _racemedal_h_
#include "logic/medal/medal.h"


class CRaceMedal : public CTimeMedal
{
public:
	CRaceMedal();
	~CRaceMedal();

	virtual BOOL CheckActivate();
private:

};

#endif //_racemedal_h_