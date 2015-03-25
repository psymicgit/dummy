#include "propapi.h"

#include <iostream>
#include <xdsapi.h>
#include <xdsentry.h>
#include <tickutil.h>

#include "ActivityProp.h"
#include "AwardProp.h"
#include "BagProp.h"
#include "EquipMasterProp.h"
#include "ItemProp.h"
#include "NickFilterProp.h"

namespace propapi
{
    void loadprop(char *xds, char *dsd, void **prop, int &cnt)
    {
        /*
        xdsentry *entry = xdsapi::loadxds("powerups.xds", "powerups.dsd");
        if(NULL == entry)
        {
            return;
        }

        if(entry->isempty())
        {
            return;
        }

        *prop = entry->m_elements;
        cnt  = entry->m_elemcnt;
        */
    }

    void load_gamesvr()
    {
        const string prop_dir = "../../svr_data/xml/";

        Tick tick_now = tickutil::get_tick();

        LoadActivityProp((prop_dir + "ActivityProp.xml").c_str());
        LoadAwardProp((prop_dir + "AwardProp.xml").c_str());
        LoadBagProp((prop_dir + "BagProp.xml").c_str());
        LoadEquipMasterProp((prop_dir + "EquipMasterProp.xml").c_str());
        LoadItemProp((prop_dir + "ItemProp.xml").c_str());
        LoadActivityProp((prop_dir + "ActivityProp.xml").c_str());
        LoadAwardProp((prop_dir + "AwardProp.xml").c_str());
        LoadBagProp((prop_dir + "BagProp.xml").c_str());
        LoadEquipMasterProp((prop_dir + "EquipMasterProp.xml").c_str());
        LoadItemProp((prop_dir + "ItemProp.xml").c_str());
        LoadActivityProp((prop_dir + "ActivityProp.xml").c_str());
        LoadAwardProp((prop_dir + "AwardProp.xml").c_str());
        LoadBagProp((prop_dir + "BagProp.xml").c_str());
        LoadEquipMasterProp((prop_dir + "EquipMasterProp.xml").c_str());
        LoadItemProp((prop_dir + "ItemProp.xml").c_str());
        LoadActivityProp((prop_dir + "ActivityProp.xml").c_str());
        LoadAwardProp((prop_dir + "AwardProp.xml").c_str());
        LoadBagProp((prop_dir + "BagProp.xml").c_str());
        LoadEquipMasterProp((prop_dir + "EquipMasterProp.xml").c_str());
        LoadItemProp((prop_dir + "ItemProp.xml").c_str());
        LoadActivityProp((prop_dir + "ActivityProp.xml").c_str());
        LoadAwardProp((prop_dir + "AwardProp.xml").c_str());
        LoadBagProp((prop_dir + "BagProp.xml").c_str());
        LoadEquipMasterProp((prop_dir + "EquipMasterProp.xml").c_str());
        LoadItemProp((prop_dir + "ItemProp.xml").c_str());
        LoadActivityProp((prop_dir + "ActivityProp.xml").c_str());
        LoadAwardProp((prop_dir + "AwardProp.xml").c_str());
        LoadBagProp((prop_dir + "BagProp.xml").c_str());
        LoadEquipMasterProp((prop_dir + "EquipMasterProp.xml").c_str());
        LoadItemProp((prop_dir + "ItemProp.xml").c_str());
        LoadItemProp((prop_dir + "ItemProp.xml").c_str());
        LoadItemProp((prop_dir + "ItemProp.xml").c_str());
        LoadItemProp((prop_dir + "ItemProp.xml").c_str());
        LoadItemProp((prop_dir + "ItemProp.xml").c_str());
        LoadItemProp((prop_dir + "ItemProp.xml").c_str());
        LoadItemProp((prop_dir + "ItemProp.xml").c_str());
        LoadBagProp((prop_dir + "BagProp.xml").c_str());
        LoadBagProp((prop_dir + "BagProp.xml").c_str());
        LoadBagProp((prop_dir + "BagProp.xml").c_str());


        dword passed_ms = tickutil::tick_diff(tick_now);
        double passed_sec = (double)passed_ms / 1000;

        std::cout << "π≤∫ƒ ±<" << passed_sec << ">√Î" << std::endl;
    }
}