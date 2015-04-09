// DealFrame36.h: interface for the CDealFrame36 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DEALFRAME36_H_
#define _DEALFRAME36_H_




#include "gmdealframe.h"

class CDealFrame36 : public InterfaceDealBase::CGmDealFrame  
{
public:
	CDealFrame36();
	virtual ~CDealFrame36();
	CDealFrame36(InterfaceDealBase::CGmManager * IN_pManager)
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

#endif // !defined(_DEALFRAME36_H_)
