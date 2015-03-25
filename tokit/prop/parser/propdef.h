///<------------------------------------------------------------------------------
//< @file:   prop\parser\propdef.h
//< @author: hongkunan
//< @date:   2014年4月4日 22:37:44
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _propdef_h_
#define _propdef_h_

typedef _Longlong int64_t;

using namespace std;

enum excel_row
{
    excel_row_none         = 0,
    excel_row_prop_limit   = 2,
    excel_row_prop_declare = 6,

	excel_row_at_least_max,
};

enum excel_prop_limit_col
{
    field_limit_col_none  = 0,
    field_limit_col_file_name  = 1,
    field_limit_col_min_cnt    = 2,
    field_limit_col_max_cnt    = 3,
};

enum excel_prop_declare_col
{
    field_declare_col_cn_name    = 1,
    field_declare_col_en_name    = 2,
    field_declare_col_field_type = 3,
    field_declare_col_min_val    = 4,
    field_declare_col_max_val    = 5,
    field_declare_col_is_primary = 6,
	field_declare_col_is_unique  = 7,
	field_declare_col_comment    = 8,

    field_declare_col_max
};

enum enum_raw_field_type
{
    fieldtype_none   = 0, 
    fieldtype_bool   = 1, // bool类型
    fieldtype_string = 2, // 字符串
    fieldtype_byte   = 3, // 单字节整数
    fieldtype_word   = 4, // 双字节整数
    fieldtype_dword  = 5, // 四字节整数
    fieldtype_qword  = 6, // 八字节整数
    fieldtype_float  = 7, // 浮点数
    fieldtype_double = 8, // 双精度浮点数
    fieldtype_max,
};

#endif //_propdef_h_