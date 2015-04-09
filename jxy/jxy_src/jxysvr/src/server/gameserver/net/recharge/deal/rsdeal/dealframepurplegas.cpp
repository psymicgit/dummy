// DealFrame0x001A.cpp: implementation of the CDealFrame0x001A class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframepurplegas.h"

#include <logic/player/player.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFramePurPlegas::CDealFramePurPlegas()
{

}

CDealFramePurPlegas::~CDealFramePurPlegas()
{

}

void CDealFramePurPlegas::Release()
{

}

string CDealFramePurPlegas::GetFrameType()
{
    return "purplegas";
}

BOOL CDealFramePurPlegas::Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1, UINT32 dwNum2, UINT32 dwNum3, TCHAR *pszDesc, DT_PLAYER_RES_DATA* pstResInfo1, DT_PLAYER_RES_DATA* pstResInfo2)
{
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    //²ÎÊý³ö´í
    if (vecOpValue.size() < 2)
    {
        return FALSE;
    }

    UINT32 dwOpValue = SDAtou(vecOpValue[1].c_str());
    switch(byOpType)
    {
    case RESOPTYPE_CKNUM:
    {
        return poPlayer->GetDT_PLAYER_BASE_DATA().qwPurpleGas >= dwOpValue ? TRUE : FALSE;
    }
    break;
    case RESOPTYPE_DECNUM:
    {
        poPlayer->DecPurpleGas(dwOpValue, CRecordMgr::EDPGT_GETGIVEGM, 0, 0, 0, 0, pszDesc);
    }
    break;
    case RESOPTYPE_ADDNUM:
    {
        poPlayer->AddPurpleGas(dwOpValue, CRecordMgr::EAPGT_GETGIVEGM, 0, 0, 0, 0, pszDesc);
    }
    break;
	case RESOPTYPE_CK_ADD_LIMIT:
	{
		return CkAddLimit(vecOpValue[0],dwOpValue);
	}
	break;
    default:
        return FALSE;
        break;
    }

    if(pstResInfo1)
    {
        pstResInfo1->byResKind = PURPLEGAS;
        pstResInfo1->qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwPurpleGas;
    }

    return TRUE;
}

