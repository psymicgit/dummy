#include "prop_decl.h"
#include "str_util.h"

#include "echoutil.h"
#include <algorithm>

namespace proputil
{
    string& raw_type_2_c_type(enum_raw_field_type fieldtype)
    {
        static string c_types[fieldtype_max] = {
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
        static string xsd_types[fieldtype_max] = {
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

    string raw_types[fieldtype_max] = {
        "",
        "bool",
        "string",
        "byte",
        "uint16",
        "uint",
        "uint64",
        "float",
        "double"
    };

    enum_raw_field_type raw_type_2_enum(const char* self_type)
    {
        if (NULL == self_type){
            return fieldtype_none;
        }

        for(int i = fieldtype_none + 1; i < fieldtype_max; i++){
            if(raw_types[i] == self_type){
                return (enum_raw_field_type)i;
            }
        }

        return fieldtype_none;
    }

    void add_field(propdecl_t &decl, enum_raw_field_type fieldtype, string &fieldname, bool is_unique = true, int64_t minval = 0, int64_t maxval = 0)
    {
        field_t field;
        field.fieldtype = fieldtype;
        field.en_name = fieldname;
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

    void echo_errvec(errvec_t &errs)
    {
        for(size_t i = 0; i < errs.size(); i++){
            string &err = errs[i];
            ECHO_ERR("  %s", err.c_str());
        }
    }
}

string& field_t::get_field_type()
{
    return proputil::raw_types[fieldtype];
}
