// DealFrame60.h: interface for the CDealFrame60 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DEALFRAME60_H_
#define _DEALFRAME60_H_




#include "gmdealframe.h"

class CDealFrame60 : public InterfaceDealBase::CGmDealFrame  
{
public:
	CDealFrame60();
	virtual ~CDealFrame60();
	CDealFrame60(InterfaceDealBase::CGmManager * IN_pManager)
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
protected:
	void DealCmd(CRCClient* poClinet,UINT32 unPlayerId,UINT8 unType,string strFileName);
};

#endif // !defined(_DEALFRAME60_H_)
