///<------------------------------------------------------------------------------
//< @file:   recharge\deal\dealframe75showgvgpair.h
//< @author: hongkunan
//< @date:   2014年8月13日 10:35:41
//< @brief:  显示跨服战的16强选手对决名单
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dealframe75showgvgpair_h_
#define _dealframe75showgvgpair_h_

#include "gmdealframe.h"

class CDealFrame75ShowGvGPair : public InterfaceDealBase::CGmDealFrame
{
public:
    CDealFrame75ShowGvGPair(InterfaceDealBase::CGmManager * IN_pManager)
    {
        m_pManager=IN_pManager;
        RegisterDealFrame(this);
    }

    virtual UINT16 GetFrameType(){ return EGMT_SHOW_GVG_PAIR; }
    virtual string GetStrFrameType(){ return SHOW_GVG_PAIR; }
    virtual string GetStrFrameDesc();

    virtual void Interface(CRCClient* poClinet);
    virtual void Deal(CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField);
    virtual void Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5 , TCHAR *pszDesc);

    void ShowGvGPair(CRCClient* poClinet, UINT32 dwZoneID);
};

#endif //_dealframe75showgvgpair_h_