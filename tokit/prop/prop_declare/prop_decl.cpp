#include "prop_decl.h"
#include "str_util.h"

#include <rapidxml_utils.hpp>
#include <rapidxml.hpp>

#include <echoutil.h>
#include <algorithm>

using namespace rapidxml;

namespace proputil
{
    string& raw_type_2_c_type(enum_raw_field_type fieldtype)
    {
        static string c_types[fieldtype_max] = 
        {
            "",
            "bool",
            "std::string",
            "byte",
            "word",         //"unsigned short",
            "dword",        //"unsigned long",
            "qword",        //"unsigned __int64",
            "float",        //"unsigned __int64",
            "double"        //"unsigned __int64",
        };

        return c_types[fieldtype];
    }

    string& raw_type_2_xsd_type(enum_raw_field_type fieldtype)
    {
        static string xsd_types[fieldtype_max] = 
        {
            "",
            "xs:byte",
            "xs:string",
            "xs:unsignedByte",
            "xs:unsignedShort",
            "xs:unsignedInt",
            "xs:unsignedLong",
            "xs:float",
            "xs:double"
        };

        return xsd_types[fieldtype];
    }

    string raw_types[fieldtype_max] = 
    {
        "",
        "bool",
        "string",
        "byte",
        "uint16",
        "uint32",
        "uint64",
        "float",
        "double"
    };

    enum_raw_field_type raw_type_2_enum(const char* self_type)
    {
        if (NULL == self_type)
        {
            return fieldtype_none;
        }

        for(int i = fieldtype_none + 1; i < fieldtype_max; i++)
        {
            if(raw_types[i] == self_type)
            {
                return (enum_raw_field_type)i;
            }
        }

        return fieldtype_none;
    }

    void add_field(propdecl_t &decl, enum_raw_field_type fieldtype, string &fieldname, bool is_unique = true, int64_t minval = 0, int64_t maxval = 0)
    {
        field_t field;
        field.fieldtype = fieldtype;
        field.fieldname = fieldname;
        field.maxlen = 64;
        field.is_unique = is_unique;
        field.minval = minval;
        field.maxval = maxval;

        decl.fields.push_back(field);
    }

    void add_key(propdecl_t &decl, int keyidx)
    {
        decl.keys.push_back(keyidx);
    }

    field_t& get_key(propdecl_t &decl, int keyidx)
    {
        return decl.fields[keyidx];
    }

    bool parse_field(rapidxml::xml_node<> *field_node, field_t &field, errvec_t& errvec)
    {
        if(NULL == field_node)
        {
            return false;
        }

        if(field_node->type() != rapidxml::node_element)
        {
            errvec.push_back(getmsg("解析字段时发生错误：发现不支持的xml结点%s=%s", field_node->name(), field_node->value()));
            return false;
        }

        rapidxml::xml_attribute<> *type_attr = field_node->first_attribute("type");
        enum_raw_field_type fieldtype = raw_type_2_enum(type_attr->value());
        if(fieldtype == fieldtype_none)
        {
            errvec.push_back(getmsg("解析%s字段时发生错误：不支持%s类型的字段", field_node->name(), type_attr->value()));
            return false;
        }

        field.fieldname = field_node->name();
        field.fieldtype = fieldtype;

        xml_node<> *comment_node = field_node->previous_sibling();
        if(comment_node && comment_node->type() == node_comment)
        {
            field.comment   = comment_node->value();
        }

        xml_attribute<> *min_attr = field_node->first_attribute("min");
        if(min_attr)
        {
            field.minval = atoi(min_attr->value());
        }

        xml_attribute<> *max_attr = field_node->first_attribute("max");
        if(max_attr)
        {
            field.maxval = atoi(max_attr->value());
        }

        return true;
    }

    bool parse_decl(rapidxml::xml_node<> *node, const propunit_t &propunit, propdecl_t &decl, errvec_t& errvec)
    {
        if(NULL == node)
        {
            return false;
        }

        bool succ = true;
        decl.filename = strutil::trim(node->name());

        xml_attribute<> *name_attr = node->first_attribute("name");
        if(NULL == name_attr)
        {
            errvec.push_back(getmsg("错误：未给<%s>结点定义name属性", decl.filename.c_str()));
            succ = false;
        }

        xml_node<> *fields_node = node->first_node("fields");
        if(NULL == fields_node)
        {
            errvec.push_back(getmsg("错误：未给<%s>定义字段", decl.filename.c_str()));
            succ = false;
        }

        decl.propname = name_attr->value();
        decl.fields.reserve(8);

        for(rapidxml::xml_node<> *field_node = fields_node->first_node(); field_node; field_node = field_node->next_sibling())
        {
            if(field_node->type() == rapidxml::node_comment)
            {
                continue;
            }

            field_t filed;
            bool ok = parse_field(field_node, filed, errvec);
            if(false == ok)
            {
                succ = false;
                continue;
            }

            if(decl.getfield(filed.fieldname.c_str()))
            {
                field_t *pre_field = decl.getfield(filed.fieldname.c_str());

                errvec.push_back(getmsg("错误：定义了两个同样地字段<%s>", filed.fieldname.c_str()));
                errvec.push_back(getmsg("       <%-6s = %s>", pre_field->get_field_type().c_str(), pre_field->fieldname.c_str()));
                errvec.push_back(getmsg("       <%-6s = %s>", filed.get_field_type().c_str(), filed.fieldname.c_str()));

                succ = false;
                continue;
            }

            decl.fields.push_back(filed);
        }

        xml_node<> *mincnt_node = node->first_node("mincnt");
        if(mincnt_node)
        {
            decl.mincnt = atoi(mincnt_node->value());
        }

        xml_node<> *maxcnt_node = node->first_node("maxcnt");
        if(mincnt_node)
        {
            decl.maxcnt = atoi(maxcnt_node->value());
        }

        xml_node<> *primarykey_node = node->first_node("primarykey");
        if(primarykey_node)
        {
            keyvec_t &keys = decl.keys;
            for(rapidxml::xml_node<> *key_node = primarykey_node->first_node(); key_node; key_node = key_node->next_sibling())
            {
                int key_idx = decl.getfieldidx(key_node->name());
                if(key_idx < 0)
                {
                    errvec.push_back(getmsg("错误：无法将<%s>作为<%s>的主键，找不到<%s>字段", key_node->name(), decl.filename, key_node->name()));
                    continue;
                }

                keys.push_back(key_idx);
            }

            std::sort(keys.begin(), keys.end());
        }

        xml_node<> *uniques_node = node->first_node("unique");
        if(uniques_node)
        {
            for(rapidxml::xml_node<> *unique_node = uniques_node->first_node(); unique_node; unique_node = unique_node->next_sibling())
            {
                field_t *field = decl.getfield(unique_node->name());
                if(NULL == field)
                {
                    errvec.push_back(getmsg("错误：找不到unique中定义的<%s>字段", unique_node->name()));
                    
                    succ = false;
                    continue;
                }

                field->is_unique = true;
            }
        }

        return succ;
    }

    bool parse(const string &xml, propunit_t &propunit, errvec_t& errvec)
    {
        bool succ = true;
        propunit.filename = strutil::strip_dir(xml);

        static rapidxml::file<> fdoc(xml.c_str());
        // std::cout << fdoc.data() << std::endl;

        rapidxml::xml_document<> doc;
        doc.parse<0>(fdoc.data());

        // 获取根节点
        rapidxml::xml_node<>* root = doc.first_node();
        for(rapidxml::xml_node<> *decl_node = root->first_node(); decl_node; decl_node = decl_node->next_sibling())
        {
            propdecl_t propdecl;

            bool ok = parse_decl(decl_node, propunit, propdecl, errvec);
            if(false == ok)
            {
                succ = false;
                continue;
            }

            if(propunit.find_by_file_name(propdecl.filename.c_str()))
            {
                propdecl_t *findpropdecl = propunit.find_by_file_name(propdecl.filename.c_str());

                errvec.push_back(getmsg("错误：定义了两个同样地文件<%s>", propdecl.filename.c_str()));
                errvec.push_back(getmsg("       <%s = %s>", findpropdecl->filename.c_str(), findpropdecl->propname.c_str()));
                errvec.push_back(getmsg("       <%s = %s>", propdecl.filename.c_str(), propdecl.propname.c_str()));

                succ = false;
                continue;
            }

            if(propunit.find_by_prop_name(propdecl.filename.c_str()))
            {
                propdecl_t *findpropdecl = propunit.find_by_prop_name(propdecl.filename.c_str());

                errvec.push_back(getmsg("错误：定义了两个同样地文件<%s>", propdecl.filename.c_str()));
                errvec.push_back(getmsg("       <%s = %s>", findpropdecl->filename.c_str(), findpropdecl->propname.c_str()));
                errvec.push_back(getmsg("       <%s = %s>", propdecl.filename.c_str(), propdecl.propname.c_str()));

                succ = false;
                continue;
            }

            propunit.propdecls.push_back(propdecl);
        }        

        return succ;
    }
}

string& field_t::get_field_type()
{
    return proputil::raw_types[fieldtype];
}
