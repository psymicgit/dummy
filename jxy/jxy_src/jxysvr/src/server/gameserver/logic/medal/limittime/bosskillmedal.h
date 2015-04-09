///<------------------------------------------------------------------------------
//< @file:   medal\limittime\bosskillmedal.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ21ÈÕ 16:29:37
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _bosskillmedal_h_
#define _bosskillmedal_h_
#include "logic/medal/medal.h"
class CBossKillMedal : public CTimeMedal
{
public:
	CBossKillMedal();
	~CBossKillMedal();

	virtual BOOL CheckActivate();
};
#endif //_bosskillmedal_h_