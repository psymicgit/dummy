// DealFrame35.h: interface for the CDealFrame35 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DEALFRAME35_H_
#define _DEALFRAME35_H_




#include "gmdealframe.h"

class CDealFrame35 : public InterfaceDealBase::CGmDealFrame  
{
public:
	CDealFrame35();
	virtual ~CDealFrame35();
	CDealFrame35(InterfaceDealBase::CGmManager * IN_pManager)
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

#endif // !defined(_DEALFRAME35_H_)
