// DealFrame0x001A.cpp: implementation of the CDealFrame0x001A class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframestudynoattr.h"
#include "logic/awaken/awakenpropmgr2.h"
#include "logic/awaken/awaken.h"
#include <logic/player/player.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrameStudyNoAttr::CDealFrameStudyNoAttr()
{

}

CDealFrameStudyNoAttr::~CDealFrameStudyNoAttr()
{

}

void CDealFrameStudyNoAttr::Release()
{

}

string CDealFrameStudyNoAttr::GetFrameType()
{
    return "studynoattr";
}

BOOL CDealFrameStudyNoAttr::Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1, UINT32 dwNum2, UINT32 dwNum3, TCHAR *pszDesc, DT_PLAYER_RES_DATA* pstResInfo1, DT_PLAYER_RES_DATA* pstResInfo2)
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
    SAwakenBaseProp2& stProp = CAwakenPropMgr2::Instance()->GetAwakenBaseProp();
    DT_STUDY_DATA stNoAttrStudy = {0};
    stNoAttrStudy.byStudyColorKind = stProp.byCallStudyColorKind;
    stNoAttrStudy.byStudyAttrKind1 = stProp.byCallStudyAttrKind1;
    stNoAttrStudy.byStudyAttrKind2 = stProp.byCallStudyAttrKind2;
    stNoAttrStudy.byStudyLevel		= stProp.byCallStudyLevel;

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
            if((stNoAttrStudy.byStudyColorKind == pstBagStudyInfo[byIdx].byStudyColorKind)
                    && (stNoAttrStudy.byStudyAttrKind1 == pstBagStudyInfo[byIdx].byStudyAttrKind1)
                    && (stNoAttrStudy.byStudyAttrKind2 == pstBagStudyInfo[byIdx].byStudyAttrKind2)
                    && (stNoAttrStudy.byStudyLevel == pstBagStudyInfo[byIdx].byStudyLevel)
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
            if((stNoAttrStudy.byStudyColorKind == pstBagStudyInfo[byIdx].byStudyColorKind)
                    && (stNoAttrStudy.byStudyAttrKind1 == pstBagStudyInfo[byIdx].byStudyAttrKind1)
                    && (stNoAttrStudy.byStudyAttrKind2 == pstBagStudyInfo[byIdx].byStudyAttrKind2)
                    && (stNoAttrStudy.byStudyLevel == pstBagStudyInfo[byIdx].byStudyLevel)
              )
            {
                poAwaken->DelStudyForGM(byIdx, pszDesc);
                dwHaveNum = dwHaveNum + 1;
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
            poAwaken->AddStudyForGm(stNoAttrStudy.byStudyColorKind, stNoAttrStudy.byStudyAttrKind1, stNoAttrStudy.byStudyLevel, stNoAttrStudy.byStudyAttrKind2, pszDesc);
        }
        return TRUE;
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

    //推送，todo
    //if(pstResInfo)
    //{
    //	pstResInfo->byResKind = GOLD;
    //	pstResInfo->qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
    //}

    return FALSE;
}

