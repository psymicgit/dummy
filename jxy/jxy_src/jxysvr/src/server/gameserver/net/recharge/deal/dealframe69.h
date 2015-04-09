// DealFrame69.h: interface for the CDealFrame69 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DEALFRAME69_H_
#define _DEALFRAME69_H_




#include "gmdealframe.h"

class CDealFrame69 : public InterfaceDealBase::CGmDealFrame  
{
public:
    CDealFrame69();
    virtual ~CDealFrame69();
    CDealFrame69(InterfaceDealBase::CGmManager * IN_pManager)
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

#endif // !defined(_DEALFRAME58_H_)
