///<------------------------------------------------------------------------------
//< @file:   prop\parser\parser.cpp
//< @author: hongkunan
//< @date:   2014年3月23日 22:22:33
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "parser.h"
#include "prop_decl.h"
#include "excel_util.h"
#include "str_util.h"
#include "echoutil.h"
#include <algorithm>

#include "CApplication.h"  
#include "CRange.h"  
#include "CWorkbook.h"  
#include "CWorkbooks.h"  
#include "CWorksheet.h"
#include "CWorksheets.h"

#include <fstream>

using namespace excelutil;

namespace parseutil
{
	bool parse_limits(COleSafeArray &ole_safe_array, propdecl_t &decl)
	{
		decl.propname = get_cell_str(ole_safe_array, excel_row_prop_limit, field_limit_col_file_name);
		decl.mincnt = get_cell_int(ole_safe_array, excel_row_prop_limit, field_limit_col_min_cnt);
		decl.maxcnt = get_cell_int(ole_safe_array, excel_row_prop_limit, field_limit_col_max_cnt);

		return true;
	}

	bool parse_field(COleSafeArray &ole_safe_array, int row, field_t &field, errvec_t& errvec)
	{
		field.cn_name = strutil::trim(get_cell_str(ole_safe_array, row, field_declare_col_cn_name));
		if(field.cn_name.empty())
		{
			errvec.push_back(getmsg("解析第<%s>行时发生错误：中文名不允许为空", row));
			return false;
		}

		field.en_name = strutil::trim(get_cell_str(ole_safe_array, row, field_declare_col_en_name));
		if(field.en_name.empty())
		{
			errvec.push_back(getmsg("解析<%s>字段时发生错误：英文名不允许为空", field.cn_name.c_str()));
			return false;
		}

		std::string fieldtype = strutil::trim(get_cell_str(ole_safe_array, row, field_declare_col_field_type));
		if(fieldtype.empty())
		{
			return false;
		}

		field.fieldtype = proputil::raw_type_2_enum(fieldtype.c_str());
		if(fieldtype_none == field.fieldtype)
		{
			errvec.push_back(getmsg("解析<%s>字段时发生错误：不支持<%s>类型的字段", field.cn_name.c_str(), fieldtype.c_str()));
			return false;
		}

		field.minval    = get_cell_int(ole_safe_array, row, field_declare_col_min_val);
		field.maxval    = get_cell_int(ole_safe_array, row, field_declare_col_max_val);
		field.is_unique = (1 == get_cell_int(ole_safe_array, row, field_declare_col_is_unique));

		field.comment	= get_cell_str(ole_safe_array, row, field_declare_col_comment);
		return true;
	}

	bool parse_decl(COleSafeArray &ole_safe_array, int row_cnt, int col_cnt, propdecl_t &decl, errvec_t& errvec)
	{
		bool succ = true;

		field_t field;
		for(int row = excel_row_prop_declare; row <= row_cnt; row++)
		{
			bool ok = parse_field(ole_safe_array, row, field, errvec);
			if(false == ok)
			{
				succ = false;
				continue;
			}

			if(decl.getfield(field.en_name.c_str()))
			{
				field_t *pre_field = decl.getfield(field.en_name.c_str());

				errvec.push_back(getmsg("错误：定义了两个同样地字段<%s>", field.en_name.c_str()));
				errvec.push_back(getmsg("       <%-6s = %s>", pre_field->get_field_type().c_str(), pre_field->en_name.c_str()));
				errvec.push_back(getmsg("       <%-6s = %s>", field.get_field_type().c_str(), field.en_name.c_str()));

				succ = false;
				continue;
			}

			bool is_primary = (1 == get_cell_int(ole_safe_array, row, field_declare_col_is_primary));
			if(is_primary)
			{
				decl.keys.push_back(decl.fields.size());
			}

			decl.fields.push_back(field);
		}

		if(decl.fields.empty())
		{
			errvec.push_back(getmsg("错误：未给<%s>定义字段", decl.filename.c_str()));
		}

		std::sort(decl.keys.begin(), decl.keys.end());

		return true;
	}

	bool parse_sheet(CWorksheet &sheet, propdecl_t &decl, errvec_t& errvec)
	{
		COleSafeArray ole_safe_array;
        ole_safe_array.Clear();
		pre_load_sheet(sheet, ole_safe_array);

		int row_cnt = get_row_cnt(sheet);
		int col_cnt = get_col_cnt(sheet);

		if(row_cnt < excel_row_at_least_max || col_cnt < field_declare_col_max - 1)
		{
			errvec.push_back(getmsg("分析<%s>失败，文档结构混乱", sheet.get_Name().GetString()));
			return false;
		}

		decl.filename = sheet.get_Name().GetString();

		bool succ = parse_limits(ole_safe_array, decl);
		succ &= parse_decl(ole_safe_array, row_cnt, col_cnt, decl, errvec);

		return succ;
	}

    string get_sheet_str(CWorksheet &sheet)
    {
        COleSafeArray ole_safe_array;
        ole_safe_array.Clear();
        pre_load_sheet(sheet, ole_safe_array);

        int row_cnt = get_row_cnt(sheet);
        int col_cnt = get_col_cnt(sheet);

        string ret;

        char buf[128] = {0};

        string text = "<sheet>";
        for(int r = 1; r <= row_cnt; r++){
            string row = "    <row";

            for(int c = 1; c <= col_cnt; c++){
                string cell = get_cell_str(ole_safe_array, r, c);
                sprintf_s(buf, " cell%d=\"%s\"", c, cell.c_str());

                row += buf;
            }

            row += "/>\n";
            text += row;
        }

        text += "</sheet>";
        return text;
    }

	bool parse_excel(string &xlsx, propunit_t &propunit, errvec_t &errvec)
	{
		CApplication excel_app;
		CWorkbooks books;
		CWorkbook book;
		CRange range;
		CWorksheets sheets;

		bool succ = excelutil::open_sheets_of_excel(xlsx, excel_app, books, book, sheets);
		if(false == succ)
		{
			return false;
		}

		for(int i = 0;i < sheets.get_Count(); i++)
		{   
			propdecl_t propdecl;
			CWorksheet sheet = sheets.get_Item(COleVariant((long)(i+1)));   

			bool ok = parse_sheet(sheet, propdecl, errvec);
			sheet.ReleaseDispatch();

			if(false == ok)
			{
				succ = false;
				continue;
			}

			propdecl_t *conflict_by_file = propunit.find_by_file_name(propdecl.filename.c_str());
			propdecl_t *conflict_by_prop = propunit.find_by_prop_name(propdecl.propname.c_str());

			if(conflict_by_file)
			{
				errvec.push_back(getmsg("错误：定义了两个同样地文件<%s>", propdecl.filename.c_str()));
				errvec.push_back(getmsg("       <%s = %s>", conflict_by_file->filename.c_str(), conflict_by_file->propname.c_str()));
				errvec.push_back(getmsg("       <%s = %s>", propdecl.filename.c_str(), propdecl.propname.c_str()));

				succ = false;
				continue;
			}

			if(conflict_by_prop)
			{
				errvec.push_back(getmsg("错误：定义了两个同样地文件<%s>", propdecl.filename.c_str()));
				errvec.push_back(getmsg("       <%s = %s>", conflict_by_prop->filename.c_str(), conflict_by_prop->propname.c_str()));
				errvec.push_back(getmsg("       <%s = %s>", propdecl.filename.c_str(), propdecl.propname.c_str()));

				succ = false;
				continue;
			}

			propunit.propdecls.push_back(propdecl);
		}

		/*释放资源*/
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();
		books.ReleaseDispatch();
		excel_app.Quit();
		excel_app.ReleaseDispatch();

		return succ;
	}

    bool save_excel_as_xml(string &xlsx, string &xml)
    {
        CApplication excel_app;
        CWorkbooks books;
        CWorkbook book;
        CRange range;
        CWorksheets sheets;

        bool succ = excelutil::open_sheets_of_excel(xlsx, excel_app, books, book, sheets);
        if(false == succ)
        {
            return false;
        }

        string text;
        for(int i = 0;i < sheets.get_Count(); i++)
        {   
            propdecl_t propdecl;
            CWorksheet sheet = sheets.get_Item(COleVariant((long)(i+1)));   

            string sheet_text = get_sheet_str(sheet);
            sheet.ReleaseDispatch();

            text += sheet_text;
        }

        /*释放资源*/
        sheets.ReleaseDispatch();
        book.ReleaseDispatch();
        books.ReleaseDispatch();
        excel_app.Quit();
        excel_app.ReleaseDispatch();

        ofstream o(xml);
        o << text;
        o.close();
        return succ;
    }
};