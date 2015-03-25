#include "ItemProp.h"

#include <iostream>
#include <string.h>
#include <rapidxml_utils.hpp>
#include "tickutil.h"

ItemPropMap g_mapItemProp;
ItemPropVec g_vecItemProp;

ItemProp::ItemProp()
    : KindID(0)
    , Name("")
    , Introduction("")
    , MainKindID(0)
    , UseLevel(0)
    , BuyCoin(0)
    , BuyGold(0)
    , RecoverPrice(0)
    , UseTimesPerDay(0)
    , UseCD(0)
    , Value(0)
    , ComposeCount(0)
    , ComposeKindID(0)
    , Param1(0)
    , Param2(0)
    , Param3(0)
    , Param4(0)
{
}

void LoadItemProp(const char *xml)
{
    g_vecItemProp.clear();
    g_mapItemProp.clear();

    Tick tick_now = tickutil::get_tick();

    rapidxml::file<> fdoc(xml);
    rapidxml::xml_document<> doc;
    doc.parse<0>(fdoc.data());

    rapidxml::xml_node<>* root = doc.first_node();

    int nodecnt = 0;
    for(rapidxml::xml_node<> *node = root->first_node(); node; node = node->next_sibling())
    {
        ++nodecnt;
    }

    g_vecItemProp.reserve(nodecnt);

    for(rapidxml::xml_node<> *node = root->first_node(); node; node = node->next_sibling())
    {
        ItemProp prop;

        rapidxml::xml_attribute<> *attr = node->first_attribute();
        const char* val = NULL;
        
        val = attr->value();
        prop.KindID = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.Name = val; attr = attr->next_attribute();
        val = attr->value();
        prop.Introduction = val; attr = attr->next_attribute();
        val = attr->value();
        prop.MainKindID = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.UseLevel = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.BuyCoin = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.BuyGold = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.RecoverPrice = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.UseTimesPerDay = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.UseCD = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.Value = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.ComposeCount = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.ComposeKindID = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.Param1 = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.Param2 = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.Param3 = atoi(val); attr = attr->next_attribute();
        val = attr->value();
        prop.Param4 = atoi(val); attr = attr->next_attribute();

        uint64_t key = keyutil::Get161616Key(prop.KindID, prop.UseLevel, prop.BuyCoin);
        g_mapItemProp.insert(std::make_pair(key, prop));
        g_vecItemProp.push_back(prop);
    }

    dword passed_ms = tickutil::tick_diff(tick_now);
    double passed_sec = (double)passed_ms / 1000;

    std::cout << "载入<ItemProp.xml>成功, 共耗时<" << passed_sec << ">秒" << std::endl;
}

ItemProp* GetItemProp(word KindID, word UseLevel, dword BuyCoin)
{
    uint64_t key = keyutil::Get161616Key(KindID, UseLevel, BuyCoin);

    ItemPropMap::iterator itr = g_mapItemProp.find(key);
    if(itr == g_mapItemProp.end())
    {
        return NULL;
    }
    
    ItemProp *p = itr->second;
    return p;
}

