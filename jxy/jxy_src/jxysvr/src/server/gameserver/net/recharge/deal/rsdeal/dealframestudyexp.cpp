// DealFrame0x001A.cpp: implementation of the CDealFrame0x001A class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframestudyexp.h"

#include "logic/awaken/awaken.h"
#include <logic/player/player.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrameStudyExp::CDealFrameStudyExp()
{

}

CDealFrameStudyExp::~CDealFrameStudyExp()
{

}

void CDealFrameStudyExp::Release()
{

}

string CDealFrameStudyExp::GetFrameType()
{
    return "studyexp";
}

BOOL CDealFrameStudyExp::Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1, UINT32 dwNum2, UINT32 dwNum3, TCHAR *pszDesc, DT_PLAYER_RES_DATA* pstResInfo1, DT_PLAYER_RES_DATA* pstResInfo2)
{
    if(NULL == poPlayer)
    {
        return FALSE;
    }
    //参数出错
    if (vecOpValue.size() < 2)
    {
        return FALSE;
    }
    //todo记录log
    UINT32 dwOpValue = SDAtou(vecOpValue[1].c_str());
    switch(byOpType)
    {
    case RESOPTYPE_CKNUM:
    {
        CAwaken* poAwaken = poPlayer->GetAwaken();
        if(NULL == poAwaken)
        {
            return FALSE;
        }
        DT_AWAKEN_BASE_DATA& stBase = poAwaken->GetDT_AWAKEN_DATA().stBaseInfo;
        return stBase.qwStudyExp >= dwOpValue ? TRUE : FALSE;
    }
    break;
    case RESOPTYPE_DECNUM:
    {
        CAwaken* poAwaken = poPlayer->GetAwaken();
        if(NULL == poAwaken)
        {
            return TRUE;
        }
        DT_AWAKEN_BASE_DATA& stBase = poAwaken->GetDT_AWAKEN_DATA().stBaseInfo;
        stBase.qwStudyExp = stBase.qwStudyExp >= dwOpValue ? stBase.qwStudyExp - dwOpValue  : 0;
    }
    break;
    case RESOPTYPE_ADDNUM:
    {
        CAwaken* poAwaken = poPlayer->GetAwaken();
        if(NULL == poAwaken)
        {
            return TRUE;
        }
        poAwaken->GetDT_AWAKEN_DATA().stBaseInfo.qwStudyExp += dwOpValue;
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

    //推送，todo
    //if(pstResInfo)
    //{
    //	pstResInfo->byResKind = GOLD;
    //	pstResInfo->qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
    //}

    return TRUE;
}

