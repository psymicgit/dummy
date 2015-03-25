///<------------------------------------------------------------------------------
//< @file:   prop\parser\parser.h
//< @author: hongkunan
//< @date:   2014Äê3ÔÂ23ÈÕ 22:21:55
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _parser_h_
#define _parser_h_

class COleSafeArray;
class CWorksheet;

#include "prop_decl.h"

namespace parseutil
{
	bool parse_limits(COleSafeArray &ole_safe_array, propdecl_t &decl);
	bool parse_field(COleSafeArray &ole_safe_array, int row, field_t &field, errvec_t& errvec);
	bool parse_decl(COleSafeArray &ole_safe_array, int row_cnt, int col_cnt, propdecl_t &decl, errvec_t& errvec);
	bool parse_sheet(CWorksheet &sheet, propdecl_t &decl, errvec_t& errvec);
    bool parse_excel(string &xlsx, propunit_t &propunit, errvec_t &errvec);
	
    bool save_excel_as_xml(string &xlsx, string &xml);
};
#endif //_parser_h_