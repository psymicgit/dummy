/********************************************************************
	@file:   dealframesoul.h
	@author: yewenhui@qq.com
	@date:   2014年7月8日 9点59分
	@brief:  gm
*********************************************************************/
#pragma once
#include "rsdealframe.h"

class CDealframeSoul
	: public InterfaceDealBase::CRsDealFrame
{
public:	
	CDealframeSoul(InterfaceDealBase::CRsManager * IN_pManager) {
		m_pManager = IN_pManager;
		RegisterDealFrame(this);
	}
	virtual ~CDealframeSoul();

	virtual bool init(InterfaceDealBase::CRsManager * IN_pManager) {
		m_pManager=IN_pManager;
		RegisterDealFrame(this);
		return TRUE;
	}
	virtual void Release() {}

	virtual string GetFrameType();
	virtual BOOL Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType
		, UINT32 dwNum1 = 0, UINT32 dwNum2 = 0, UINT32 dwNum3 = 0
		, TCHAR *pszDesc = NULL, DT_PLAYER_RES_DATA* pstResInfo1 = NULL, DT_PLAYER_RES_DATA* pstResInfo2 = NULL);
	
};
