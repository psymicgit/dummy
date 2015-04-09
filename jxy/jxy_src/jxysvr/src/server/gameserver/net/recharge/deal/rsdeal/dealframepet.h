/********************************************************************
	@file:   dealframepet.cpp
	@author: yewenhui@qq.com
	@date:   2014年9月15日 14点53分
	@brief:  gm宠物
*********************************************************************/
#pragma once
#include "rsdealframe.h"

class CDealframePet
	: public InterfaceDealBase::CRsDealFrame
{
public:
	CDealframePet(InterfaceDealBase::CRsManager * IN_pManager) {
		m_pManager = IN_pManager;
		RegisterDealFrame(this);
	}
	virtual ~CDealframePet();

	virtual bool init(InterfaceDealBase::CRsManager * IN_pManager) {
		m_pManager=IN_pManager;
		RegisterDealFrame(this);
		return TRUE;
	}
	virtual void Release() {}

	virtual string GetFrameType();
	virtual BOOL Deal(CPlayer* pPlayer, vector<string> vecOpValue, UINT8 byOpType
		, UINT32 dwNum1 = 0, UINT32 dwNum2 = 0, UINT32 dwNum3 = 0
		, TCHAR *pszDesc = NULL, DT_PLAYER_RES_DATA* pstResInfo1 = NULL, DT_PLAYER_RES_DATA* pstResInfo2 = NULL);
};
