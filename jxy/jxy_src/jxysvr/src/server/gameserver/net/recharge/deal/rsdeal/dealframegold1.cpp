// DealFrame0x001A.cpp: implementation of the CDealFrame0x001A class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframegold1.h"

#include <logic/player/player.h>
#include "logic/vip/vippropmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrameGold1::CDealFrameGold1()
{

}

CDealFrameGold1::~CDealFrameGold1()
{

}

void CDealFrameGold1::Release()
{

}

string CDealFrameGold1::GetFrameType()
{
    return "gold1";
}

BOOL CDealFrameGold1::Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1, UINT32 dwNum2, UINT32 dwNum3, TCHAR *pszDesc, DT_PLAYER_RES_DATA* pstResInfo1, DT_PLAYER_RES_DATA* pstResInfo2)
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
        return poPlayer->GetDT_PLAYER_BASE_DATA().qwGold >= dwOpValue ? TRUE : FALSE;
    }
    break;
    case RESOPTYPE_DECNUM:
    {
        return FALSE;
    }
    break;
    case RESOPTYPE_ADDNUM:
    {
        UINT32 dwAddRmb = 0;
        dwAddRmb = dwOpValue / 10;
        poPlayer->AddGold(dwOpValue, CRecordMgr::EGGT_GM_FLAGEXCHANGE, 0, 0, 0, 0, pszDesc, 0);
    }
    break;
    case RESOPTYPE_CK_ADD_LIMIT:
    {
        return CkAddLimit(vecOpValue[0], dwOpValue);
    }
    break;
    default:
        return FALSE;
        break;
    }

    if(pstResInfo1)
    {
        pstResInfo1->byResKind = GOLD;
        pstResInfo1->qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;
    }

    return TRUE;
}

