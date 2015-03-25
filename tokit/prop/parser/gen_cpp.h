///<------------------------------------------------------------------------------
//< @file:   prop\parser\gen_cpp.h
//< @author: hongkunan
//< @date:   2014Äê4ÔÂ5ÈÕ 23:13:43
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gen_cpp_h_
#define _gen_cpp_h_

#include <string>

using std::string;

namespace cpputil
{
	bool gen_cpps(string &excel, string &cpp_dir, string &cpp_templet);
}

#endif //_gen_cpp_h_