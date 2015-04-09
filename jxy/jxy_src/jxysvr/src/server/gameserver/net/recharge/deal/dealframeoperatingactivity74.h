///<------------------------------------------------------------------------------
//< @file:   recharge\deal\dealframeoperatingactivity74.h
//< @author: Kang_zhiquan
//< @date:   2014Äê7ÔÂ29ÈÕ 14:10:55
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _dealframeoperatingactivity74_h_
#define _dealframeoperatingactivity74_h_

#include "gmdealframe.h"

class CDealFrameOperatingActivity :  public InterfaceDealBase::CGmDealFrame  
{
public:
	CDealFrameOperatingActivity();
	virtual ~CDealFrameOperatingActivity();
	CDealFrameOperatingActivity(InterfaceDealBase::CGmManager * IN_pManager)
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
#endif //_dealframeoperatingactivity74_h_