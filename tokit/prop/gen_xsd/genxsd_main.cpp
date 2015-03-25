#include <fstream>

#include <prop_decl.h>
#include <file_util.h>
#include <str_util.h>
#include <echoutil.h>

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

        o << "            <xs:attribute name=\"" << field.fieldname << "\" type=\"" << xsd_type << "\"/>" << endl;
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

void gen_xsds(string &xml, string &xsd_dir, string &templet)
{
    propunit_t unit;
    errvec_t errvec;
    proputil::parse(xml, unit, errvec);

    for(propdeclvec_t::iterator itr = unit.propdecls.begin(); itr != unit.propdecls.end(); ++itr)
    {
        propdecl_t &decl = *itr;
        string xsd = xsd_dir + strip_ext(strip_dir(decl.filename)) + ".xsd";

        gen_xsd(decl, xsd);
    }
}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        ECHO_ERR("命令错误，格式应为: ");
        ECHO_ERR("   gen_xsd 结构定义文件的路径 模板文件 生成的xsd文件放在哪个文件夹下");
        ECHO_ERR("   比如: gen_xsd  ../../example.xlsx template/template.xsd ../../");

        return 0;
    }

    string xml_file = argv[1];
    string templet  = argv[2];
    string xsd_dir = argv[3];

    if(false == fileuitl::exist(xml_file))
    {
        ECHO_ERR("错误: %s 路径不存在", xml_file.c_str());
        return 0;
    }

    if(false == fileuitl::exist(templet))
    {
        ECHO_ERR("错误: %s 路径不存在", templet.c_str());
        return 0;
    }

    if(false == fileuitl::exist(xsd_dir))
    {
        ECHO_ERR("错误: %s 路径不存在", xsd_dir.c_str());
        return 0;
    }

    gen_xsds(xml_file, xsd_dir, templet);
    return 0;
}
