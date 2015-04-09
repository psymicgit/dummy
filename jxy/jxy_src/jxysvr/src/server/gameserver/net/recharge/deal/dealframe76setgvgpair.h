///<------------------------------------------------------------------------------
//< @file:   recharge\deal\dealframe76setgvgpair.h
//< @author: hongkunan
//< @date:   2014年8月13日 10:35:34
//< @brief:  设置跨服战的16强选手对决名单，仅在大乱斗结束后且16强未开始之间的时间段内本接口有效
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dealframe76setgvgpair_h_
#define _dealframe76setgvgpair_h_

#include "gmdealframe.h"

class CDealFrame76SetGvGPair : public InterfaceDealBase::CGmDealFrame
{
public:
    CDealFrame76SetGvGPair(InterfaceDealBase::CGmManager * IN_pManager)
    {
        m_pManager=IN_pManager;
        RegisterDealFrame(this);
    }

    virtual UINT16 GetFrameType(){ return EGMT_SET_GVG_PAIR; }
    virtual string GetStrFrameType(){ return SET_GVG_PAIR; }
    virtual string GetStrFrameDesc();

    virtual void Interface(CRCClient* poClinet);
    virtual void Deal(CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField);
    virtual void Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5 , TCHAR *pszDesc);

    void SwapFactionPos(CRCClient* poClinet, UINT32 dwZoneID, UINT8 byOldPos, UINT8 byNewPos);
};

#endif //_dealframe76setgvgpair_h_