///<------------------------------------------------------------------------------
//< @file:   medal\limittime\bosshurtmedal.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ21ÈÕ 16:29:31
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _bosshurtmedal_h_
#define _bosshurtmedal_h_
#include "logic/medal/medal.h"
class CBossHurtMedal : public CTimeMedal
{
public:
	CBossHurtMedal();
	~CBossHurtMedal();

	virtual BOOL CheckActivate();
};
#endif //_bosshurtmedal_h_