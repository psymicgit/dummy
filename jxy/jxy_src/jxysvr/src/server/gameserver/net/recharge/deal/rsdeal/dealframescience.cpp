// DealFrame0x001A.cpp: implementation of the CDealFrame0x001A class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframescience.h"
#include <logic/player/player.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrameScience::CDealFrameScience()
{

}

CDealFrameScience::~CDealFrameScience()
{

}

void CDealFrameScience::Release()
{

}

string CDealFrameScience::GetFrameType()
{
    return "science";
}

BOOL CDealFrameScience::Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1, UINT32 dwNum2, UINT32 dwNum3, TCHAR *pszDesc, DT_PLAYER_RES_DATA* pstResInfo1, DT_PLAYER_RES_DATA* pstResInfo2)
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
        return poPlayer->GetDT_PLAYER_BASE_DATA().qwScience >= dwOpValue ? TRUE : FALSE;
    }
    break;
    case RESOPTYPE_DECNUM:
    {
        poPlayer->DecScience(dwOpValue, CRecordMgr::EDST_GETGIVEGM, 0, 0, 0, 0, pszDesc);
    }
    break;
    case RESOPTYPE_ADDNUM:
    {
        poPlayer->AddScience(dwOpValue, CRecordMgr::EDSCT_GETGIVEGM, 0, 0, 0, 0, pszDesc);
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
        pstResInfo1->byResKind = SCIENCE;
        pstResInfo1->qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;
    }

    return TRUE;
}

