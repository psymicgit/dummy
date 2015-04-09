// DealFrame0x001A.cpp: implementation of the CDealFrame0x001A class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframeexperience.h"

#include <logic/player/player.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrameExperience::CDealFrameExperience()
{

}

CDealFrameExperience::~CDealFrameExperience()
{

}

void CDealFrameExperience::Release()
{

}

string CDealFrameExperience::GetFrameType()
{
    return "experience";
}

BOOL CDealFrameExperience::Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1, UINT32 dwNum2, UINT32 dwNum3, TCHAR *pszDesc, DT_PLAYER_RES_DATA* pstResInfo1, DT_PLAYER_RES_DATA* pstResInfo2)
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

    UINT32 dwOpValue	= SDAtou(vecOpValue[1].c_str());
    switch(byOpType)
    {
    case RESOPTYPE_CKNUM:
    {
        return TRUE;
    }
    break;
    case RESOPTYPE_DECNUM:
    {
        return TRUE;
    }
    break;
    case RESOPTYPE_ADDNUM:
    {
        poPlayer->AllotExperience(dwOpValue);
        return TRUE;
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

    if(pstResInfo1 && pstResInfo2)
    {
        DT_EXPERIENCE_DATA stExperienceInfo;
        pstResInfo1->byResKind = EXPERIENCE;
        pstResInfo2->byResKind = UPGRADEEXPERIENCE;
        poPlayer->GetExperienceInfo(stExperienceInfo);
        pstResInfo1->qwResValue = stExperienceInfo.dwExperience;
        pstResInfo2->qwResValue = stExperienceInfo.dwUpgradeExperience;
    }
    return TRUE;
}

