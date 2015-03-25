///<------------------------------------------------------------------------------
//< @file:   prop\parser\gen_xsd.cpp
//< @author: hongkunan
//< @date:   2014年4月5日 17:39:38
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "gen_xsd.h"

#include <fstream>

#include "prop_decl.h"
#include "file_util.h"
#include "str_util.h"
#include "echoutil.h"
#include "parser.h"

namespace genxsdutil{

void gen_head(ofstream &o, propdecl_t &decl)
{
	o << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
}

void gen_elem(ofstream &o, propdecl_t &decl)
{
	o << "<xs:element name=\"" << decl.propname << "s\">" << std::endl;
	o << "  <xs:complexType>" << endl;
	o << "    <xs:sequence minOccurs=\"0\" maxOccurs=\"unbounded\">" << endl;
	o << "      <xs:element name=\"" << decl.propname << "\">" << std::endl;
	o << "        <xs:complexType>" << endl;
	o << "          <xs:sequence>" << endl;
	for(fieldvec_t::iterator itr = decl.fields.begin(); itr != decl.fields.end(); ++itr)
	{
		field_t &field = *itr;
		string& xsd_type = proputil::raw_type_2_xsd_type(field.fieldtype);

		//o << "            <xs:element name=\"" << field.fieldname << "\" type=\"" << xsd_type << "\"/>" << endl;
	}
	o << "          </xs:sequence>" << endl;

	for(fieldvec_t::iterator itr = decl.fields.begin(); itr != decl.fields.end(); ++itr)
	{
		field_t &field = *itr;
		string& xsd_type = proputil::raw_type_2_xsd_type(field.fieldtype);

		o << "            <xs:attribute name=\"" << field.en_name << "\" type=\"" << xsd_type << "\"/>" << endl;
	}

	o << "        </xs:complexType>" << endl;    
	o << "      </xs:element>" << endl;
	o << "    </xs:sequence>" << endl;
	o << "  </xs:complexType>" << endl;    
	o << "</xs:element>" << endl;
}

void gen_body(ofstream &o, propdecl_t &decl)
{
	o << "<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\">" << endl;
	gen_elem(o, decl);
	o << "</xs:schema>" << endl;
}

void gen_xsd(propdecl_t &decl, string &xsd)
{
	ofstream o(xsd);

	gen_head(o, decl);
	gen_body(o, decl);
}

bool gen_xsds(string &excel, string &xsd_dir)
{
	if(false == fileuitl::exist(excel))
	{
		ECHO_ERR("错误: %s 路径不存在", excel.c_str());
		return false;
	}

	if(false == fileuitl::exist(xsd_dir))
	{
		ECHO_ERR("错误: %s 路径不存在", xsd_dir.c_str());
		return false;
	}

	propunit_t unit;
	errvec_t errvec;

    bool ok = parseutil::parse_excel(excel, unit, errvec);
    if (!ok){
        ECHO_ERR("错误: %s 解析文件失败", xsd_dir.c_str());
        proputil::echo_errvec(errvec);
        return false;
    }
	
	// proputil::parse(excel, unit, errvec);

	for(propdeclvec_t::iterator itr = unit.propdecls.begin(); itr != unit.propdecls.end(); ++itr)
	{
		propdecl_t &decl = *itr;
		string xsd = xsd_dir + "\\" + strip_ext(strip_dir(decl.filename)) + ".xsd";

		gen_xsd(decl, xsd);
	}

	return true;
}
}