///<------------------------------------------------------------------------------
//< @file:   prop\parser\excel_util.h
//< @author: hongkunan
//< @date:   2014Äê4ÔÂ5ÈÕ 17:59:45
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _excel_util_h_
#define _excel_util_h_

#include <string>

class CApplication;
class CWorkbooks;
class CWorkbook;
class CWorksheet;
class CWorksheets;
class COleSafeArray;

using std::string;

namespace excelutil
{
	bool open_sheets_of_excel(string &xlsx, CApplication&, CWorkbooks&, CWorkbook&, CWorksheets&);

	void pre_load_sheet(CWorksheet &sheet, COleSafeArray &safe_array);

	std::string get_cell_str(COleSafeArray &ole_safe_array, int row, int col);

	int get_cell_int(COleSafeArray &ole_safe_array, int row, int col);

	int get_row_cnt(CWorksheet &sheet);

	int get_col_cnt(CWorksheet &sheet);
}
#endif //_excel_util_h_