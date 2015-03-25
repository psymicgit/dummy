///<------------------------------------------------------------------------------
//< @file:   prop\parser\gen_cpp.cpp
//< @author: hongkunan
//< @date:   2014年4月5日 23:13:52
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "file_util.h"
#include "str_util.h"
#include "echoutil.h"
#include "prop_decl.h"
#include "parser.h"

#include <fstream>

using namespace fileuitl;
using namespace std;

typedef std::vector<string> propvec_t;

namespace cpputil
{

	void gen_includes(ofstream &o, propdecl_t &decl)
	{
		o << "#include \"" << decl.propname << ".h\"" << std::endl;
		o << endl;

		o << "#include <iostream>" << std::endl;
		o << "#include <string.h>" << std::endl;
		o << "#include <rapidxml_utils.hpp>" << std::endl;
		o << "#include \"tickutil.h\"" << std::endl;
		o << "#include \"str_util.h\"" << std::endl;

		o << endl;
	}

	void gen_struct_and_map(ofstream &o, propdecl_t &decl)
	{
		string structs_maps = 
			"%prop%Vec g_vec%prop%;\n"
			"%prop%Map g_map%prop%;\n"
			;

		o << strutil::replace(string(structs_maps), "%prop%", decl.propname.c_str());
	}

	void gen_ctor(ofstream &o, propdecl_t &decl)
	{
		string ctor = 
			"\n%prop%::%prop%()"
			;       

		o << strutil::replace(ctor, "%prop%", decl.propname.c_str());

		for(size_t i = 0; i != decl.fields.size(); ++i)
		{
			field_t &field = decl.fields[i];

			i ? o << "\n    , " :
				o << "\n    : ";

			field.fieldtype == fieldtype_string ?
				o << field.en_name << "(\"\")" :
			o << field.en_name << "(0)";
		}

		o << "\n{";
		o << "\n}";
		o << endl;
	}

	// 生成读取属性的语句
	string gen_load_attr_statement(field_t &field)
	{
		static string loads[fieldtype_max] =
		{
			"",
			"prop.%field% = (val[0] != 0);",
			"prop.%field% = val;",
			"prop.%field% = atoi(val);",
			"prop.%field% = atoi(val);",
			"prop.%field% = atoi(val);",
			"prop.%field% = atoi(val);",
			"prop.%field% = atoi(val);",
			"prop.%field% = atoi(val);",
		};

		string load_stat = loads[field.fieldtype];
		load_stat = strutil::replace(load_stat, "%field%", field.en_name.c_str());

		return load_stat;
	}

	// splice the key string
	// eg: string key = tostr(prop.type) + tostr(prop.id) + prop.name;
	// eg: string key = tostr(prop.type) + prop.name;
	// eg: string &key = prop.name;
	string splice_str_key(fieldvec_t keys)
	{
		string ret = "string key = ";
		if(keys.size() == 1)
		{
			ret = "std::string &key = ";
		}
		else
		{
			ret = "std::string key = ";
		}

		for (fieldvec_t::iterator itr = keys.begin(); itr != keys.end(); ++itr)
		{
			field_t &key = *itr;

			if(key.fieldtype == fieldtype_string)
			{
				ret += key.en_name;
			}
			else
			{
				ret += "strutil::tostr(";
				ret += key.en_name;
				ret += ")";
			}

			ret += " + ";
		}

		ret.erase(ret.end() - 3, ret.end());

		ret += ";";
		return ret;
	}

	// splice the key string
	// eg: string key = tostr(prop.type) + tostr(prop.id) + prop.name;
	// eg: string key = tostr(prop.type) + prop.name;
	// eg: string &key = prop.name;
	string splice_int_key(fieldvec_t &keys)
	{
		size_t n_key = keys.size();
		if(0 == n_key)
		{
			return "";
		}

		string ret = "";
		switch(n_key)
		{
			// 1个主键
		case 1:
			{
				field_t &key  = keys[0];
				ret = proputil::raw_type_2_c_type(key.fieldtype) + " key = " + key.en_name  + ";";
				break;
			}
			// 2个主键
		case 2:
			ret = "uint64_t key = keyutil::Get3232Key(";
			ret += keys[0].en_name;
			ret += ", ";
			ret += keys[1].en_name;
			ret += ");";

			break;

			// 3个主键
		case 3:
			ret = "uint64_t key = keyutil::Get161616Key(";
			ret += keys[0].en_name;
			ret += ", ";
			ret += keys[1].en_name;
			ret += ", ";
			ret += keys[2].en_name;
			ret += ");";
			break;

			// 4个主键
		case 4:
			ret = "uint64_t key = keyutil::Get16161616Key(";
			ret += keys[0].en_name;
			ret += ", ";
			ret += keys[1].en_name;
			ret += ", ";
			ret += keys[2].en_name;
			ret += ", ";
			ret += keys[3].en_name;
			ret += ");";

			break;
		}

		return ret;
	}

	string gen_map_insert(propdecl_t &decl)
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

		fieldvec_t keys;
		for (keyvec_t::iterator itr = decl.keys.begin(); itr != decl.keys.end(); ++itr)
		{
			int keyidx = *itr;
			field_t key = decl.fields[keyidx]; // deep copy

			key.en_name = "prop." + key.en_name;
			keys.push_back(key);
		}

		string map_key = "\n        ";
		is_key_has_str ?
			map_key += splice_str_key(keys) :
		map_key += splice_int_key(keys);

		string map_insert = 
			"\n        g_map%prop%[key] = &g_vec%prop%.back();"
			;

		strutil::replace(map_insert, "%prop%", decl.propname.c_str());
		return map_key + map_insert;
	}

	void gen_load_func(ofstream &o, propdecl_t &decl)
	{
		string loadfunc = 
			"\nvoid Load%prop%(const char *xml)"
			"\n{"
			"\n    g_vec%prop%.clear();"
			"\n    g_map%prop%.clear();"
			"\n"
			"\n    Tick tick_now = tickutil::get_tick();"
			"\n"
			"\n    rapidxml::file<> fdoc(xml);"
			"\n    rapidxml::xml_document<> doc;"
			"\n    doc.parse<0>(fdoc.data());"
			"\n"
			"\n    rapidxml::xml_node<>* root = doc.first_node();"
			"\n"
			"\n    int nodecnt = 0;"
			"\n    for(rapidxml::xml_node<> *node = root->first_node(); node; node = node->next_sibling())"
			"\n    {"
			"\n        ++nodecnt;"
			"\n    }"
			"\n"
			"\n    g_vec%prop%.reserve(nodecnt);"
			"\n"
			"\n    for(rapidxml::xml_node<> *node = root->first_node(); node; node = node->next_sibling())"
			"\n    {"
			"\n        %prop% prop;"
			"\n"
			"\n        rapidxml::xml_attribute<> *attr = node->first_attribute();"
			"\n        const char* val = NULL;"
			"\n        "
			;

		o << strutil::replace(string(loadfunc), "%prop%", decl.propname.c_str()) << endl;

		fieldvec_t &fields = decl.fields;
		for(fieldvec_t::iterator itr = fields.begin(); itr != fields.end(); ++itr)
		{
			field_t &field = *itr;

			o << "        val = attr->value();" << endl;
			o << "        " << gen_load_attr_statement(field);
			o << " attr = attr->next_attribute();" << endl;
		}

		string vec_push = 
			"\n        g_vec%prop%.push_back(prop);"
			;

		string map_insert = gen_map_insert(decl);
		string endfunc = 
			"\n    }"
			"\n"
			"\n    dword passed_ms = tickutil::tick_diff(tick_now);"
			"\n    double passed_sec = (double)passed_ms / 1000;"
			"\n"
			"\n    std::cout << \"载入<%prop%.xml>成功, 共耗时<\" << passed_sec << \">秒\" << std::endl;"
			"\n}"
			"\n"
			;

		o << strutil::replace(vec_push, "%prop%", decl.propname.c_str());
		o << endl;
		o << map_insert;
		o << strutil::replace(endfunc, "%prop%", decl.propname.c_str()) << endl;
	}

	void gen_find_func(ofstream &o, propdecl_t &decl)
	{
		size_t n_key = decl.keys.size();
		if(0 == n_key)
		{
			return;
		}

		// 生成传入函数的参数，
		// 比如：ItemProp* GetItemProp(int itemtype, int itemgrade)其中的int itemtype, int itemgrade
		string funckey = "";
		for(keyvec_t::iterator itr = decl.keys.begin(); itr != decl.keys.end(); ++itr)
		{
			int keyidx = *itr;

			field_t &field = decl.fields[keyidx];
			funckey += (field.fieldtype == fieldtype_string ? "const " : "");
			funckey += proputil::raw_type_2_c_type(field.fieldtype) + " ";
			funckey += (field.fieldtype == fieldtype_string ? "&" : "");
			funckey += (field.en_name + ", ");
		}

		// 把最后的", "移掉
		funckey.erase(funckey.begin() + funckey.size() - 2, funckey.end());

		// map的key
		// 
		string map_key = "";
		switch(n_key)
		{
			// 单主键
		case 1:
			{
				field_t &field = decl.fields[decl.keys[0]];
				map_key = field.en_name;
				break;
			}

			// 多主键
		case 2:
		case 3:
			map_key = "key";
			break;
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

		fieldvec_t keys;
		for (keyvec_t::iterator itr = decl.keys.begin(); itr != decl.keys.end(); ++itr)
		{
			int keyidx = *itr;
			field_t key = decl.fields[keyidx]; // deep copy

			key.en_name = key.en_name;
			keys.push_back(key);
		}

		string get_key = "";

		is_key_has_str ?
			get_key = "    " + splice_str_key(keys) + "\n" :
		get_key = "    " + splice_int_key(keys) + "\n";

		string findfunc = 
			"%prop%* Get%prop%(%funckey%)\n"
			"{\n"
			"%get_key%"
			"    %prop%Map::iterator itr = g_map%prop%.find(%mapkey%);\n"
			"    if(itr == g_map%prop%.end())\n"
			"    {\n"
			"        return NULL;\n"
			"    }\n"
			"    \n"
			"    %prop% *p = itr->second;\n"
			"    return p;\n"
			"}\n"
			;

		strutil::replace(findfunc, "%prop%",    decl.propname.c_str());
		strutil::replace(findfunc, "%funckey%", funckey.c_str());
		strutil::replace(findfunc, "%get_key%", get_key.c_str());
		strutil::replace(findfunc, "%mapkey%",  map_key.c_str());    

		o << findfunc;
		o << endl;
	}

	void gen_cpp(propdecl_t &decl, string &xxx_cpp)
	{
		ofstream o(xxx_cpp);

		gen_includes(o, decl);
		gen_struct_and_map(o, decl);
		gen_ctor(o, decl);
		gen_load_func(o, decl);
		gen_find_func(o, decl);

		o.close();
	}

	bool gen_cpps(string &excel, string &cpp_dir, string &cpp_templet)
	{
		if(false == fileuitl::exist(excel))
		{
			ECHO_ERR("参数错误: 找不到excel结构定义文件<%s>", excel.c_str());
			return false;
		}

		if(false == fileuitl::exist(cpp_dir))
		{
			ECHO_ERR("参数错误: 找不到指定的生成源文件的路径<%s>", cpp_dir.c_str());
			return false;
		}

		if(false == fileuitl::exist(cpp_templet))
		{
			ECHO_ERR("参数错误: 找不到cpp模板文件<%s>", cpp_templet.c_str());
			return false;
		}

		propunit_t unit;
		errvec_t errvec;
		parseutil::parse_excel(excel, unit, errvec);

		for(propdeclvec_t::iterator itr = unit.propdecls.begin(); itr != unit.propdecls.end(); ++itr)
		{
			propdecl_t &decl = *itr;
			string xxx_cpp = cpp_dir + "\\" + strip_ext(strip_dir(decl.propname)) + ".cpp";

			gen_cpp(decl, xxx_cpp);
		}

		return true;
	}
}