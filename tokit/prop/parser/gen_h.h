///<------------------------------------------------------------------------------
//< @file:   prop\parser\gen_h.h
//< @author: hongkunan
//< @date:   2014年4月5日 23:2:6
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gen_h_h_
#define _gen_h_h_

#include <string>

using std::string;

namespace cpputil
{
	bool gen_hs(string &excel, string &h_dir, string &h_templet/* 用于生成h的模板文件 */);
}

#endif //_gen_h_h_