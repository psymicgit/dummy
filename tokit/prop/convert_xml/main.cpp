#include <fstream>
#include <iostream>


#include <prop_decl.h>
#include <file_util.h>
#include <str_util.h>
#include <echoutil.h>

#include <rapidxml_utils.hpp>

// 这个是解析旧的用xml定义配置数据结构的
typedef unsigned __int64 QWORD;

struct element_t
{
    element_t()
    {
        memset(this, 0, sizeof(element_t));
    }

    char *buf;
    int len;
};

struct elements_t
{
    element_t *elems;
};

void buf_into(char buf_beg[], int &pos, char* attrname, char* attrval, propdecl_t &decl)
{
    field_t *field = decl.getfield(attrname);
    if(NULL == field)
    {
        return;
    }

    char *buf = buf_beg + pos;


    char name[] = "afsddsfa";
    switch (field->fieldtype)
    {
    case fieldtype_bool:
    {
        bool v = (attrval[0] != '0');
        memcpy(buf, &v, sizeof(bool));
        pos += sizeof(bool);
        break;
    }
    case fieldtype_string:
    {
        memset(buf, 0, field->maxlen);
        memcpy(buf, attrval, strlen(attrval));
        pos += field->maxlen;
        break;
    }
    case fieldtype_byte:
    {
        byte v = atoi(attrval);
        memcpy(buf, &v, sizeof(byte));
        pos += sizeof(byte);
        break;
    }
    case fieldtype_word:
    {
        WORD v = atoi(attrval);
        memcpy(buf, &v, sizeof(WORD));
        pos += sizeof(WORD);
        break;
    }
    case fieldtype_dword:
    {
        DWORD v = atoi(attrval);
        memcpy(buf, &v, sizeof(DWORD));
        pos += sizeof(DWORD);
        break;
    }
    case fieldtype_qword:
    {
        QWORD v = atoi(attrval);
        memcpy(buf, &v, sizeof(QWORD));
        pos += sizeof(QWORD);
        break;
    }
    case fieldtype_float:
    {
        float v = (float)atof(attrval);
        memcpy(buf, &v, sizeof(float));
        pos += sizeof(float);
        break;
    }
    case fieldtype_double:
    {
        double v = atof(attrval);
        memcpy(buf, &v, sizeof(double));
        pos += sizeof(double);
        break;
    }
    }
}

void readxml(string &xml, propdecl_t &decl)
{
    rapidxml::file<> fdoc(xml.c_str());
    std::cout << fdoc.data() << std::endl;

    rapidxml::xml_document<> doc;
    doc.parse<0>(fdoc.data());
    std::cout<<doc.name()<<std::endl;

    // 获取根节点
    rapidxml::xml_node<>* root = doc.first_node();
    std::cout<<root->name()<<std::endl;
    
    char buf[102400] = {0};
    int pos = 0;

    char *buf_pivot = buf;

    for(rapidxml::xml_node<> *node = root->first_node(); node; node = node->next_sibling())
    {
        std::cout << node->name() << ": ";
        for(rapidxml::xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
        {
            std::cout << attr->name() << "='" << attr->value() << "' ";
            buf_into(buf_pivot, pos, attr->name(), attr->value(), decl);
        }

        std::cout << std::endl;
    }
}

void writeout(string &out, char buf[])
{
    ofstream o(out);
    o.write(buf, 10240);
}

void convert(string &xml, string &out)
{
    propdecl_t decl;
    proputil::parse(xml, decl);;

    char buf[10240] = {0};
    readxml(xml, decl);
    writeout(out, buf);
}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        ECHO_ERR("命令错误，格式应为: ");
        ECHO_ERR("   convert_xml xml文件的路径 生成的文件");
        ECHO_ERR("   比如: convert_xml ../../example.xml example.h");

        return 0;
    }

    string xml_file = argv[1];
    string converted_file = argv[2];

    if(false == fileuitl::exist(xml_file))
    {
        ECHO_ERR("参数错误: %s 路径不存在", xml_file.c_str());
        return 0;
    }

    convert(xml_file, converted_file);
    system("pause");
    return 0;
}
