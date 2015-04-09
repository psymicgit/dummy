// DealFrameCoin.h: interface for the CDealFrameCoin class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DEALFRAMEGOLD1_H_
#define _DEALFRAMEGOLD1_H_



#include "rsdealframe.h"

class CDealFrameGold1 : public InterfaceDealBase::CRsDealFrame  
{
public:
	CDealFrameGold1();
	virtual ~CDealFrameGold1();
	CDealFrameGold1(InterfaceDealBase::CRsManager * IN_pManager)
	{
		m_pManager=IN_pManager;
		RegisterDealFrame(this);
	}
public:
	string GetFrameType();
	BOOL Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1 = 0, UINT32 dwNum2 = 0, UINT32 dwNum3 = 0, TCHAR *pszDesc = NULL, DT_PLAYER_RES_DATA* pstResInfo1 = NULL, DT_PLAYER_RES_DATA* pstResInfo2 = NULL);
	void Release();
	bool init(InterfaceDealBase::CRsManager * IN_pManager) 
	{
		m_pManager=IN_pManager;
		RegisterDealFrame(this);
		return TRUE;
	}
};

#endif // !defined(_DEALFRAMECOIN_H_)
