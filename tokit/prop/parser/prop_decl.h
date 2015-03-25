///<------------------------------------------------------------------------------
//< @file:   prop\parser\prop_decl.h
//< @author: hongkunan
//< @date:   2014年4月5日 16:47:39
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _prop_decl_h_
#define _prop_decl_h_

#include <string>
#include <vector>

#include "propdef.h"

typedef _Longlong int64_t;

using namespace std;

// 成员描述
struct field_t
{
    field_t()
        : fieldtype(fieldtype_none)
        , maxlen(0)
        , minval(0)
        , maxval(0)
        , is_unique(false)
    {
    }

    string& get_field_type();

    enum_raw_field_type fieldtype; // 数据类型
	string cn_name;         // 中文字段名称
    string en_name;         // 英文字段名称
    string comment;           // 注释
    int    maxlen;               // 最大长度，仅当类型为字符串时有效

    int64_t minval;    // 最小值，如果是字符串则为最小长度
    int64_t maxval;    // 最大值，如果是字符串则为最大长度
    bool    is_unique; // 每条记录中的本字段值是否必须都不一样
};

typedef std::vector<field_t> fieldvec_t;
typedef std::vector<int> keyvec_t;

// 配置数据的描述
struct propdecl_t
{
    propdecl_t()
        : mincnt(0)
        , maxcnt(0)
    {
    }

    string filename;   // 配置结构定义文件的名称
    string propname;   // 配置结构体的名称  

    fieldvec_t fields; // 各成员
    keyvec_t keys;     // 主键索引列表（某个字段如果是主键，则将其索引加入到这个列表）

    size_t mincnt; // 至少有几条数据
    size_t maxcnt; // 最多有几条数据

    bool exist(const char* fieldname)
    {
        return (NULL != getfield(fieldname));
    }

    // 根据字段名获取字段
    field_t* getfield(const char* fieldname)
    {
        for(fieldvec_t::iterator itr = fields.begin(); itr != fields.end(); ++itr)
        {
            field_t &field = *itr;
            if(field.en_name == fieldname)
            {
                return &field;
            }
        }

        return NULL;
    }

    int getfieldidx(const char* fieldname)
    {
        int ret = -1;

        int i = 0;
        for(fieldvec_t::iterator itr = fields.begin(); itr != fields.end(); ++itr, ++i)
        {
            field_t &field = *itr;
            if(field.en_name == fieldname)
            {
                ret = i;
                break;
            }
        }

        return ret;
    }
};

typedef std::vector<propdecl_t> propdeclvec_t;
struct propunit_t
{
    propdecl_t* find_by_file_name(const char* filename)
    {
        if(NULL == filename)
        {
            return NULL;
        }

        for(propdeclvec_t::iterator itr = propdecls.begin(); itr != propdecls.end(); ++itr)
        {
            propdecl_t &propdecl = *itr;
            if(propdecl.filename == filename)
            {
                return &propdecl;
            }
        }

        return NULL;
    }

    propdecl_t* find_by_prop_name(const char* propname)
    {
        if(NULL == propname)
        {
            return NULL;
        }

        for(propdeclvec_t::iterator itr = propdecls.begin(); itr != propdecls.end(); ++itr)
        {
            propdecl_t &propdecl = *itr;
            if(propdecl.propname == propname)
            {
                return &propdecl;
            }
        }

        return NULL;
    }

    string filename;
    propdeclvec_t propdecls;
};

typedef std::vector<string> errvec_t;
namespace proputil
{
    string& raw_type_2_c_type(enum_raw_field_type);

    string& raw_type_2_xsd_type(enum_raw_field_type);

    field_t& get_key(propdecl_t &decl, int keyidx);

	enum_raw_field_type raw_type_2_enum(const char* self_type);

    void echo_errvec(errvec_t&);
}

#endif //_prop_decl_h_