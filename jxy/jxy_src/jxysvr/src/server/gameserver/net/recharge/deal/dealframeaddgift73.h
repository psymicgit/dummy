///<------------------------------------------------------------------------------
//< @file:   recharge\deal\dealframeaddgift73.h
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ9ÈÕ 19:43:19
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dealframeaddgift73_h_
#define _dealframeaddgift73_h_
#include "gmdealframe.h"

class CDealFrameAddGift :  public InterfaceDealBase::CGmDealFrame  
{
public:
	CDealFrameAddGift();
	virtual ~CDealFrameAddGift();
	CDealFrameAddGift(InterfaceDealBase::CGmManager * IN_pManager)
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

#endif //_dealframeaddgift73_h_