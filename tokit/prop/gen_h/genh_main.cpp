#include <fstream>

#include <prop_decl.h>
#include <file_util.h>
#include <str_util.h>
#include <echoutil.h>

string gen_struct_fields(propdecl_t &decl)
{
    string struct_fields;
    for(fieldvec_t::iterator itr = decl.fields.begin(); itr != decl.fields.end(); ++itr)
    {
        field_t &field = *itr;
        struct_fields += "    ";
        struct_fields += proputil::raw_type_2_c_type(field.fieldtype);
        struct_fields += " ";
        struct_fields += field.fieldname;
        struct_fields += ";\n";
    }

    return struct_fields;
}

string gen_find_func(propdecl_t &decl)
{
    size_t n_key = decl.keys.size();
    if(0 == n_key)
    {
        return "";
    }

    // 拼接出函数参数，比如 = const char* name, int Id
    string key = "";
    for(keyvec_t::iterator itr = decl.keys.begin(); itr != decl.keys.end(); ++itr)
    {
        int keyidx = *itr;

        field_t &field = decl.fields[keyidx];

        string paramtype = (field.fieldtype == fieldtype_string ? "const char *" : proputil::raw_type_2_c_type(field.fieldtype) + " ");
        key += paramtype + field.fieldname + ", ";
    }

    // 把最后的", "移掉
    key.erase(key.begin() + key.size() - 2, key.end());

    string findfunc = 
        "%prop%* Get%prop%(%key%);\n"
        ;

    strutil::replace(findfunc, "%prop%", decl.propname.c_str());
    strutil::replace(findfunc, "%key%",  key.c_str());

    return findfunc;
}

string gen_typedef_map(propdecl_t &decl)
{
    size_t n_key = decl.keys.size();
    if(0 == n_key)
    {
        return "";
    }

    bool is_key_has_str = false;
    for (keyvec_t::iterator itr = decl.keys.begin(); itr != decl.keys.end(); ++itr)
    {
        int keyidx = *itr;
        field_t &key = decl.fields[keyidx];

        if(key.fieldtype == fieldtype_string)
        {
            is_key_has_str = true;
            break;
        }
    }

    string typedef_map = "";
    if(is_key_has_str)
    {
        typedef_map = "typedef std::map<std::string, %prop%*> %prop%Map;";
    }
    else
    {
        switch(n_key)
        {
        case 1:
            typedef_map = "typedef std::map<int, %prop%*> %prop%Map;";
            break;

        case 2:
        case 3:
        case 4:
            typedef_map = "typedef std::map<uint64_t, %prop%*> %prop%Map;";
            break;

        default:
            break;
        }
    }
    
    return typedef_map;
}

void gen_h(propdecl_t &decl, string &xxx_h, string &buf)
{
    ofstream o(xxx_h);
    string out = buf;

    string struct_fields = gen_struct_fields(decl);
    string typedef_map = gen_typedef_map(decl);
    string find_func = gen_find_func(decl);

    strutil::replace(out, "%fields%", struct_fields.c_str());
    strutil::replace(out, "%typedef_map%", typedef_map.c_str());
    strutil::replace(out, "%find_func%", find_func.c_str());
    strutil::replace(out, "%prop%", decl.propname.c_str());

    o << out;
    o.close();
}

void gen_hs(string &xml, string &h_dir, string &h_templet)
{
    propunit_t unit;
    errvec_t errvec;
    proputil::parse(xml, unit, errvec);

    string contents = fileuitl::get_whole_file_str(h_templet);
    if(contents.empty())
    {
        return;
    }

    for(propdeclvec_t::iterator itr = unit.propdecls.begin(); itr != unit.propdecls.end(); ++itr)
    {
        propdecl_t &decl = *itr;
        string xxx_h = h_dir + strip_ext(strip_dir(decl.propname)) + ".h";

        gen_h(decl, xxx_h, contents);
    }
}

int main(int argc, char **argv)
{
    if(argc != 4)
    {
        ECHO_ERR("命令错误，格式应为: ");
        ECHO_ERR("   gen_h 结构定义文件的路径 模板文件 生成的.h文件放在哪个文件夹下");
        ECHO_ERR("   比如: gen_h  ../../example.xlsx template/c_template.h ../../");

        return 0;
    }

    string xml_file = argv[1];
    string templet  = argv[2];
    string h_dir    = argv[3];

    if(false == fileuitl::exist(xml_file))
    {
        ECHO_ERR("参数错误: %s 路径不存在", xml_file.c_str());
        return 0;
    }

    if(false == fileuitl::exist(templet))
    {
        ECHO_ERR("参数错误: %s 路径不存在", templet.c_str());
        return 0;
    }

    if(false == fileuitl::exist(h_dir))
    {
        ECHO_ERR("错误: %s 路径不存在", h_dir.c_str());
        return 0;
    }

    gen_hs(xml_file, h_dir, templet);
    return 0;
}
