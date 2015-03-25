#include <fstream>
#include <iostream>

#include <prop_decl.h>
#include <file_util.h>
#include <str_util.h>
#include <echoutil.h>
#include <set>
#include <map>

#include <rapidxml_utils.hpp>

using namespace echoutil;

typedef unsigned __int64 QWORD;

struct item_t
{
    item_t()
        : name(0)
        , val(0)
    {
    }

    char *name;
    char *val;
};

typedef std::vector<item_t> itemvec_t;
struct row_t
{
    row_t()
        : row(0)
    {

    }

    itemvec_t items;
    size_t row;      // 所处的行数
};

typedef std::vector<row_t> rowvec_t;
struct chart_t
{
    rowvec_t rows;
};

typedef std::vector<string> errvec_t;

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

void readxml(string &xml, propdecl_t &decl, chart_t &table)
{
    static rapidxml::file<> fdoc(xml.c_str());
    // std::cout << fdoc.data() << std::endl;

    rapidxml::xml_document<> doc;
    doc.parse<0>(fdoc.data());
    // std::cout<<doc.name()<<std::endl;

    // 获取根节点
    rapidxml::xml_node<>* root = doc.first_node();
    // std::cout<<root->name()<<std::endl;

    table.rows.reserve(512);
    
    size_t rowidx = 1;
    for(rapidxml::xml_node<> *node = root->first_node(); node; node = node->next_sibling(), ++rowidx)
    {
        // std::cout << node->name() << ": ";
        
        row_t row;
        for(rapidxml::xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
        {
            item_t item;
            item.name = attr->name();  // 字段名，只拷贝指针
            item.val  = attr->value(); // 字段值，只拷贝指针

            row.items.push_back(item);
            // std::cout << attr->name() << "='" << attr->value() << "' ";
        }

        row.row = rowidx;
        table.rows.push_back(row);
        // std::cout << std::endl;
    }
}

typedef std::map<string/*字段值*/, row_t*> itemmap_t;
typedef std::map<string/*键值*/  , row_t*> keymap_t;

void check_field_unique(propdecl_t &decl, chart_t &table, size_t field_idx, errvec_t &errvec)
{
    if(field_idx < 0 || field_idx > decl.fields.size() - 1)
    {
        errvec.push_back(getmsg("错误：检测时出现异常错误，索引超出"));
        return;
    }

    field_t &field = decl.fields[field_idx];
    if(false == field.is_unique)
    {
        return;
    }

    itemmap_t itemmap;
    for (rowvec_t::iterator itr = table.rows.begin(); itr != table.rows.end(); ++itr)
    {
        row_t &row = *itr;
        item_t &item = row.items[field_idx];

        itemmap_t::iterator inneritr = itemmap.find(item.val);
        if(inneritr != itemmap.end())
        {
            row_t *existed_item_row = inneritr->second;

            errvec.push_back(getmsg("错误：发现第%d行与第%d行的<%s> = <%s>重复", row.row, existed_item_row->row, item.name, item.val));
            continue;
        }

        itemmap[item.val] = &row;
    }
}

void check_field_min_max_val(propdecl_t &decl, chart_t &table, size_t field_idx, errvec_t &errvec)
{
    if(field_idx < 0 || field_idx > decl.fields.size() - 1)
    {
        errvec.push_back(getmsg("错误：检测时出现异常错误，索引超出"));
        return;
    }

    field_t &field = decl.fields[field_idx];
    if(fieldtype_string == field.fieldtype)
    {
        return;
    }

    for (rowvec_t::iterator itr = table.rows.begin(); itr != table.rows.end(); ++itr)
    {
        row_t &row = *itr;
        item_t &item = row.items[field_idx];

        int64_t val = _atoi64(item.val);
        if(val < field.minval)        
        {
            errvec.push_back(getmsg("错误：发现第%d行的<%s> = <%s>, 值至少应为<%d>", row.row, item.name, item.val, field.minval));
            continue;
        }

        if(field.maxval != 0 && val > field.maxval)
        {
            errvec.push_back(getmsg("错误：发现第%d行的<%s> = <%s>, 值最大应为<%d>", row.row, item.name, item.val, field.maxval));
            continue;
        }
    }
}

void check_min_max_cnt(propdecl_t &decl, chart_t &table, errvec_t &errvec)
{
    if(table.rows.size() < decl.mincnt)
    {
        errvec.push_back(getmsg("错误：%s表只有%d条数据，至少应有%d条数据", decl.propname.c_str(), table.rows.size(), decl.mincnt));
        return;
    }

    if(table.rows.size() > decl.maxcnt)
    {
        errvec.push_back(getmsg("检测到%s表有%d条数据，最多只应该有%d条数据", decl.propname.c_str(), table.rows.size(), decl.maxcnt));
        return;
    }
}

void check_unique(propdecl_t &decl, chart_t &table, errvec_t &errvec)
{
    for(size_t i = 0; i < decl.fields.size(); ++i)
    {
        check_field_unique(decl, table, i, errvec);
    }
}

void check_min_max_val(propdecl_t &decl, chart_t &table, errvec_t &errvec)
{
    for(size_t i = 0; i < decl.fields.size(); ++i)
    {
        check_field_min_max_val(decl, table, i, errvec);
    }
}

void check_primary_key(propdecl_t &decl, chart_t &table, errvec_t &errvec)
{
    keymap_t keymap;
    for (rowvec_t::iterator itr = table.rows.begin(); itr != table.rows.end(); ++itr)
    {
        row_t &row = *itr;

        string key = "";
        for(size_t i = 0; i < decl.keys.size(); i++)
        {
            item_t &item = row.items[i];
            key += item.val;
        }

        keymap_t::iterator keyitr = keymap.find(key);
        if(keyitr != keymap.end())
        {
            row_t *existed_key_row = keyitr->second;

            string err = getmsg("错误：发现第%d行与第%d行的主键重复: <", row.row, existed_key_row->row);

            for(size_t i = 0; i < decl.keys.size(); i++)
            {
                item_t &item = row.items[i];
                err += getmsg("%s = %s", item.name, item.val);

                if(i != (decl.keys.size() - 1))
                {
                    err += ", ";
                }
            }

            err += ">";
            errvec.push_back(err);
        }
    }
}

bool check_decl(propdecl_t &decl, chart_t &table, errvec_t &errvec)
{
    check_min_max_cnt(decl, table, errvec);
    check_unique(decl, table, errvec);
    check_min_max_val(decl, table, errvec);
    check_primary_key(decl, table, errvec);

    return errvec.empty();
}

bool check_xml(string &xml, propdecl_t &decl)
{
    if(false == fileuitl::exist(xml))
    {
        ECHO_ERR("错误：找不到<%s>文件:", xml.c_str());
        return false;
    }

    chart_t chart;
    readxml(xml, decl, chart);

    errvec_t errvec;
    bool ok = check_decl(decl, chart, errvec);
    if(ok)
    {
        ECHO_OK("检测<%s>文件通过", xml.c_str());
    }
    else
    {
        ECHO_ERR("检测<%s>文件不通过:", xml.c_str());
        for(errvec_t::iterator itr = errvec.begin(); itr != errvec.end(); ++itr)
        {
            string &err = *itr;
            ECHO_ERR("  %s", err.c_str());
        }
    }

    return true;
}

void check(string &xml, string &datadir)
{
    propunit_t unit;
    errvec_t errvec;
    proputil::parse(xml, unit, errvec);

    for(propdeclvec_t::iterator itr = unit.propdecls.begin(); itr != unit.propdecls.end(); ++itr)
    {
        propdecl_t &decl = *itr;
        string datapath = datadir + decl.propname + ".xml";

        check_xml(datapath, decl);
    }
}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        ECHO_WARN("命令错误，格式应为: ");
        ECHO_WARN("   exam xlsx结构定义文件 xml数据文件的路径");
        ECHO_WARN("   比如: exam.exe ../../itemdecl.xml ../../");

        return 0;
    }

    string xml = argv[1];
    string datadir = argv[2];

    if(false == fileuitl::exist(xml))
    {
        ECHO_ERR("错误: %s 路径不存在", xml.c_str());
        return 0;
    }

    if(false == fileuitl::exist(datadir))
    {
        ECHO_ERR("错误: %s 路径不存在", datadir.c_str());
        return 0;
    }

    check(xml, datadir);
    return 0;
}
