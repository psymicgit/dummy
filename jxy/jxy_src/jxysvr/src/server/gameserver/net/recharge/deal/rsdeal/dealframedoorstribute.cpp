// DealFrame0x001A.cpp: implementation of the CDealFrame0x001A class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframedoorstribute.h"

#include <logic/player/player.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrameDoorstribute::CDealFrameDoorstribute()
{

}

CDealFrameDoorstribute::~CDealFrameDoorstribute()
{

}

void CDealFrameDoorstribute::Release()
{

}

string CDealFrameDoorstribute::GetFrameType()
{
    return "doorstribute";
}

BOOL CDealFrameDoorstribute::Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1, UINT32 dwNum2, UINT32 dwNum3, TCHAR *pszDesc, DT_PLAYER_RES_DATA* pstResInfo1, DT_PLAYER_RES_DATA* pstResInfo2)
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

    CFaction* poFaction = poPlayer->GetFaction();
    if ( NULL == poFaction)
    {
        return FALSE;
    }


    UINT32 dwOpValue = SDAtou(vecOpValue[1].c_str());
    switch(byOpType)
    {
    case RESOPTYPE_CKNUM:
    {
        return poFaction->GetDoorsTribute(poPlayer->GetID()) >= dwOpValue ? TRUE : FALSE;
    }
    break;
    case RESOPTYPE_DECNUM:
    {
        poFaction->DeductDoorsTribute(poPlayer->GetID(), dwOpValue, CRecordMgr::EAT_GM);
    }
    break;
    case RESOPTYPE_ADDNUM:
    {
        poFaction->AddDoorsTribute(poPlayer->GetID(), dwOpValue, CRecordMgr::EAT_GM);
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
    return TRUE;
}

