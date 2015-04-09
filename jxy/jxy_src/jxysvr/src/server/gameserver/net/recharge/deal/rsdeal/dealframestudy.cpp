// DealFrame0x001A.cpp: implementation of the CDealFrame0x001A class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframestudy.h"

#include "logic/awaken/awaken.h"
#include <logic/player/player.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrameStudy::CDealFrameStudy()
{

}

CDealFrameStudy::~CDealFrameStudy()
{

}

void CDealFrameStudy::Release()
{

}

string CDealFrameStudy::GetFrameType()
{
    return "study";
}

BOOL CDealFrameStudy::Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1, UINT32 dwNum2, UINT32 dwNum3, TCHAR *pszDesc, DT_PLAYER_RES_DATA* pstResInfo1, DT_PLAYER_RES_DATA* pstResInfo2)
{
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    if (vecOpValue.size() < 5)
    {
        return FALSE;
    }

    UINT8 byStudyColorKind		=	SDAtou(vecOpValue[1].c_str());
    UINT8 byStudyAttrKind1		=	SDAtou(vecOpValue[2].c_str());
    UINT8 byStudyAttrKind2		=	SDAtou(vecOpValue[3].c_str());
    UINT8 dwOpValue			=	SDAtou(vecOpValue[4].c_str());

    switch(byOpType)
    {
    case RESOPTYPE_CKNUM:
    {
        CAwaken* poAwaken = poPlayer->GetAwaken();
        if(NULL == poAwaken)
        {
            return FALSE;
        }

        UINT32 dwHaveNum = 0;
        DT_STUDY_DATA* pstBagStudyInfo = poAwaken->GetDT_AWAKEN_DATA().astBagStudyInfo;
        for(UINT8 byIdx = 0; byIdx < MAX_BAG_STUDY_GRID_NUM; byIdx++)
        {
            if((byStudyColorKind == pstBagStudyInfo[byIdx].byStudyColorKind)
                    && (byStudyAttrKind1 == pstBagStudyInfo[byIdx].byStudyAttrKind1)
                    && (byStudyAttrKind2 == pstBagStudyInfo[byIdx].byStudyAttrKind2)
              )
            {
                if(++dwHaveNum >= dwOpValue)
                {
                    return TRUE;
                }
            }
        }

        return FALSE;
    }
    break;
    case RESOPTYPE_DECNUM:
    {
        CAwaken* poAwaken = poPlayer->GetAwaken();
        if(NULL == poAwaken)
        {
            return FALSE;
        }

        UINT32 dwHaveNum = 0;
        DT_STUDY_DATA* pstBagStudyInfo = poAwaken->GetDT_AWAKEN_DATA().astBagStudyInfo;
        for(UINT8 byIdx = 0; ((byIdx < MAX_BAG_STUDY_GRID_NUM) && (dwHaveNum < dwOpValue)); byIdx++)
        {
            if((byStudyColorKind == pstBagStudyInfo[byIdx].byStudyColorKind)
                    && (byStudyAttrKind1 == pstBagStudyInfo[byIdx].byStudyAttrKind1)
                    && (byStudyAttrKind2 == pstBagStudyInfo[byIdx].byStudyAttrKind2)
              )
            {
                poAwaken->DelStudyForGM(byIdx);
				dwHaveNum++;
            }
        }

        return FALSE;
    }
    break;
    case RESOPTYPE_ADDNUM:
    {
        CAwaken* poAwaken = poPlayer->GetAwaken();
        if(NULL == poAwaken)
        {
            return TRUE;
        }
        UINT8 byHaveNum = (UINT8)dwOpValue > MAX_BAG_STUDY_GRID_NUM ?  MAX_BAG_STUDY_GRID_NUM :  (UINT8)dwOpValue;
        for(UINT8 byIdx = 0; byIdx < byHaveNum; byIdx++)
        {
            poAwaken->AddStudyForGm(byStudyColorKind, byStudyAttrKind1, 1, byStudyAttrKind2);
        }
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

    //ÍÆËÍ£¬todo
    //if(pstResInfo)
    //{
    //	pstResInfo->byResKind = GOLD;
    //	pstResInfo->qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
    //}

    return FALSE;
}

