///<------------------------------------------------------------------------------
//< @file:   recharge\deal\dealframeaddjewelry.h
//< @author: Kang_zhiquan
//< @date:   2014Äê5ÔÂ23ÈÕ 9:57:50
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dealframeaddjewelry_h_
#define _dealframeaddjewelry_h_
#include "gmdealframe.h"

class CDealFrameAddJewelry :  public InterfaceDealBase::CGmDealFrame  
{
public:
	CDealFrameAddJewelry();
	virtual ~CDealFrameAddJewelry();
	CDealFrameAddJewelry(InterfaceDealBase::CGmManager * IN_pManager)
	{
		m_pManager=IN_pManager;
		RegisterDealFrame(this);
	}
public:
	virtual UINT16 GetFrameType();
	virtual string GetStrFrameType();
	virtual string GetStrFrameDesc();
	virtual void Interface(CRCClient* poClinet);
	virtual void Deal(CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField);
	virtual void Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5 , TCHAR *pszDesc);
	virtual void Release();
	virtual bool init(InterfaceDealBase::CGmManager * IN_pManager) 
	{
		m_pManager=IN_pManager;
		RegisterDealFrame(this);
		return TRUE;
	}
};
#endif //_dealframeaddjewelry_h_