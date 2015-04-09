// DealFrame48.cpp: implementation of the CDealFrame48 class.
//
//////////////////////////////////////////////////////////////////////
#include "dealframe48.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playermgr.h"
#include <db/autosqlbuf.h>
#include "logic/item/item.h"
#include "logic/item/equip/equippropmgr.h"
#include "logic/bag/bag.h"
#include "logic/awaken/awaken.h"

#include "rsdeal/dealframebluegas.h"
#include "rsdeal/dealframecoin.h"
#include "rsdeal/dealframeexperience.h"
#include "rsdeal/dealframegold.h"
#include "rsdeal/dealframegold1.h"
#include "rsdeal/dealframejingjie.h"
#include "rsdeal/dealframephystrength.h"
#include "rsdeal/dealframepurplegas.h"
#include "rsdeal/dealframescience.h"
#include "rsdeal/dealframestory.h"
#include "rsdeal/dealframestudy.h"
#include "rsdeal/dealframestudycli.h"
#include "rsdeal/dealframestudyexp.h"
#include "rsdeal/dealframestudynoattr.h"
#include "rsdeal/dealframefactionfunds.h"
#include "rsdeal/dealframedoorstribute.h"
#include "rsdeal/dealframesoul.h"
#include "rsdeal/dealframeitem.h"
#include "rsdeal/dealframepet.h"

#include <logic/player/player.h>
#include "logic/player/playerbasedatacachemgr.h"
#include "logic/soul/playersoul.h"

CDealFrame48::CDealFrame48()
{
    Register();
}

CDealFrame48::~CDealFrame48()
{

}

void CDealFrame48::Release()
{

}

UINT16 CDealFrame48::GetFrameType()
{
    return 48;
}

void CDealFrame48::Register()
{
    InterfaceDealBase::CRsDealFrame *pDealFrame = NULL;
    pDealFrame = new CDealFrameBluegas(&m_oManager);
    pDealFrame = new CDealFrameCoin(&m_oManager);
    pDealFrame = new CDealFrameExperience(&m_oManager);
    pDealFrame = new CDealFrameGold(&m_oManager);
    pDealFrame = new CDealFrameJingJie(&m_oManager);
    pDealFrame = new CDealFramePhystrength(&m_oManager);
    pDealFrame = new CDealFramePurPlegas(&m_oManager);
    pDealFrame = new CDealFrameScience(&m_oManager);
    pDealFrame = new CDealFrameStory(&m_oManager);
    pDealFrame = new CDealFrameStudy(&m_oManager);
    pDealFrame = new CDealFrameStudyCli(&m_oManager);
    pDealFrame = new CDealFrameStudyExp(&m_oManager);
    pDealFrame = new CDealFrameStudyNoAttr(&m_oManager);
    pDealFrame = new CDealFrameItem(&m_oManager);
    pDealFrame = new CDealFrameGold1(&m_oManager);
	pDealFrame = new CDealFrameDoorstribute(&m_oManager);
	pDealFrame = new CDealFrameFactionFunds(&m_oManager);
	pDealFrame = new CDealframeSoul(&m_oManager);
	pDealFrame = new CDealframePet(&m_oManager);
}

BOOL CDealFrame48::CkResInvalid(map< string, vector<string> >& mapRes, string& strFirstInvalid)
{
    for(map< string, vector<string> >::iterator itr = mapRes.begin(); itr != mapRes.end(); itr++)
    {
        UINT16 wItemID = SDAtou(itr->first.c_str());
        if(0 == wItemID)
        {
            continue;
        }
        if(wItemID < EQUIP_END_KINDID)
        {
            if(NULL == CEquipPropMgr::Instance()->GetProp(wItemID))
            {
                strFirstInvalid = itr->first;
                return FALSE;
            }
        }
        else
        {
            if(NULL == CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wItemID))
            {
                strFirstInvalid = itr->first;
                return FALSE;
            }
        }
    }
    return TRUE;
}

void CDealFrame48::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    if((NULL == poClinet) || (NULL == pszParam4) || (NULL == pszParam5))
    {
        return;
    }

    CHAR szRet[512] = {0};
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        poClinet->Rspone("{\"status\":\"Player no in mem!\"}");
        return;
    }

    //建筑是否开放	dwParam1
    //物品背包空闲数	qwParam2
    //卦象背包空闲数	qwParam3
    if((0 != dwParam1) && (!poPlayer->GetBuild((EBuildKind)dwParam1)))
    {
        poClinet->Rspone("{\"status\":\" build no open!\"}");
        return;
    }
    if(poPlayer->GetBag().GetIdleSize() < qwParam2)
    {
        poClinet->Rspone("{\"status\":\" bag idle size not enough!\"}");
        return;
    }
    if(0 != qwParam3)
    {

        CAwaken* poAwaken = poPlayer->GetAwaken();
        if(NULL == poAwaken)
        {
            poClinet->Rspone("{\"status\":\" study un open!\"}");
            return;
        }
        if(poAwaken->GetBagIdleSize() < qwParam3)
        {
            poClinet->Rspone("{\"status\":\" study bag idle size not enough!\"}");
            return;
        }
    }

    //扣除物品解析
    vector<string> vecDecRes;
    map< string, vector<string> > mapDecRes;
    //检测对于没有值的都会传递默认值为0，要过虑这种没用的数据
    if (0 != SDStrcmp(pszParam4, "0"))
    {
        vecDecRes = SDSplitStringsA(pszParam4, ',');
        for(UINT8 byIdx = 0; byIdx < vecDecRes.size(); byIdx++)
        {
            vector<string> vecTmp = SDSplitStringsA(vecDecRes[byIdx], '|');
            if(vecTmp.size() < 2)
            {
                poClinet->Rspone("{\"status\":\"param4 sub param invalid!\"}");
                return;
            }
            mapDecRes[vecTmp[0]] = vecTmp;
        }
    }


    //增加物品解析
    vector<string> vecAddRes;
    map< string, vector<string> > mapAddRes;
    //检测对于没有值的都会传递默认值为0，要过虑这种没用的数据
    if (0 != SDStrcmp(pszParam5, "0"))
    {
        vecAddRes = SDSplitStringsA(pszParam5, ',');

        for(UINT8 byIdx = 0; byIdx < vecAddRes.size(); byIdx++)
        {
            vector<string> vecTmp = SDSplitStringsA(vecAddRes[byIdx], '|');
            if(vecTmp.size() < 2)
            {
                poClinet->Rspone("{\"status\":\"param5 sub param invalid!\"}");
                return;
            }
            mapAddRes[vecTmp[0]] = vecTmp;
        }
    }



    //检查数量是否足够
    for(map< string, vector<string> >::iterator itr = mapDecRes.begin(); itr != mapDecRes.end(); itr++)
    {
        if(!m_oManager.Deal(itr->first, poPlayer, itr->second, RESOPTYPE_CKNUM, 0, 0, 0))
        {
            CHAR szRet[128] = {0};
            sprintf(szRet, "{\"status\":\"%s num not enought!\"}", itr->first.c_str());
            poClinet->Rspone(szRet);
            return;
        }
    }

    //检测添加的数量是否超过限制
    for(map< string, vector<string> >::iterator itr = mapAddRes.begin(); itr != mapAddRes.end(); itr++)
    {
        if(!m_oManager.Deal(itr->first, poPlayer, itr->second, RESOPTYPE_CK_ADD_LIMIT, 0, 0, 0))
        {
            string strErrorMsg = m_oManager.GetAddLimitMsg(itr->first);
            poClinet->Rspone(strErrorMsg);
            return;
        }
    }


    //检查资源的合法性
    string strRet;
    if(!CkResInvalid(mapDecRes, strRet))
    {
        sprintf(szRet, "{\"status\":\"%s invalid!\"}", strRet.c_str());
        poClinet->Rspone(szRet);
        return;
    }

    //检查背包是否能容下
    CBag& oBag = poPlayer->GetBag();
    UINT8 byNeedNum = 0;
    for(map< string, vector<string> >::iterator itr = mapAddRes.begin(); itr != mapAddRes.end(); itr++)
    {
        UINT16 wItemID = SDAtou(itr->first.c_str());
        if(0 != wItemID)
        {
            if(!oBag.GetGoodsItem(wItemID)) //装备或不存在物品占1个格子,装备之能给1个
            {
                byNeedNum++;
            }
        }
    }
    if(oBag.GetIdleSize() < byNeedNum)
    {
        poClinet->Rspone("{\"status\":\" bag full!\"}");
        return;
    }

    //检查卦象背包是否能容下
    CAwaken* poAwaken = poPlayer->GetAwaken();
    map< string, vector<string> >::iterator itr = mapAddRes.find("studynoattr");
    if(itr != mapAddRes.end())
    {
        if(NULL == poAwaken)
        {
            poClinet->Rspone("{\"status\":\" study un open!\"}");
            return;
        }

        if(poAwaken->GetBagIdleSize() < SDAtou(itr->second[1].c_str()))
        {
            poClinet->Rspone("{\"status\":\" study bag full!\"}");
            return;
        }
    }
// 
// 	itr = mapAddRes.find("soul");
// 	if(itr != mapAddRes.end()) {
// 		CPlayerSoul* pPlayerSoul = poPlayer->GetPlayerSoul();
// 		if (NULL == pPlayerSoul) {
// 			poClinet->Rspone("{\"status\":\" soul un open!\"}");
// 			return;
// 		}
// 		
// 		if (itr->second.size() < 3) {
// 			return;
// 		}
// 
// 		unsigned int unNum = SDAtou(itr->second[2].c_str());
// 		if (pPlayerSoul->GetSoulBagLeftSize() < unNum) {
// 			poClinet->Rspone("{\"status\":\" soul bag full!\"}");
// 			return;
// 		}
// 	}


    PKT_CLIGS_PLAYER_RES_NTF stNtf;
    //删除资源
    for(map< string, vector<string> >::iterator itr = mapDecRes.begin(); itr != mapDecRes.end(); itr++)
    {
        DT_PLAYER_RES_DATA& stDT_PLAYER_RES_DATA = stNtf.astResInfo[stNtf.byResNum];
        if(!m_oManager.Deal(itr->first, poPlayer, itr->second, RESOPTYPE_DECNUM, 0, 0, 0, pszDesc, &stDT_PLAYER_RES_DATA))
        {
            SYS_CRITICAL(_SDT("[%s:%d]: dec player[%u] res[%s] value[%u] failed!\n"), MSG_MARK, poPlayer->GetID(), itr->first.c_str(), itr->second[0].c_str()); //异常,继续
        }
        if(0 != stDT_PLAYER_RES_DATA.byResKind)
        {
            stNtf.byResNum++;
        }
    }

    //添加资源
    for(map< string, vector<string> >::iterator itr = mapAddRes.begin() ; itr != mapAddRes.end() && stNtf.byResNum < (MAX_PLAYER_RES_NUM - 1); itr++)
    {
        DT_PLAYER_RES_DATA& stDT_PLAYER_RES_DATA1 = stNtf.astResInfo[stNtf.byResNum];
        DT_PLAYER_RES_DATA& stDT_PLAYER_RES_DATA2 = stNtf.astResInfo[stNtf.byResNum + 1];
        if(!m_oManager.Deal(itr->first, poPlayer, itr->second, RESOPTYPE_ADDNUM, 0, 0, 0, pszDesc, &stDT_PLAYER_RES_DATA1, &stDT_PLAYER_RES_DATA2))
        {
            SYS_CRITICAL(_SDT("[%s:%d]: add player[%u] res[%s] value[%u] failed!\n"), MSG_MARK, poPlayer->GetID(), itr->first.c_str(), itr->second[0].c_str()); //异常,继续
        }
        if(0 != stDT_PLAYER_RES_DATA1.byResKind)
        {
            stNtf.byResNum++;
        }
        if(0 != stDT_PLAYER_RES_DATA2.byResKind)
        {
            stNtf.byResNum++;
        }
    }

    char szRspone[1024] = {0};
	SDStrcpy(szRspone, "{\"status\":\"success!\"}");
    poClinet->Rspone(szRspone);

    //同步资源
    if(0 != stNtf.byResNum)
    {
        stNtf.byResNum = stNtf.byResNum > MAX_PLAYER_RES_NUM ? MAX_PLAYER_RES_NUM : stNtf.byResNum;
        poPlayer->SendMsg((CHAR*)&stNtf, CLIGS_PLAYER_RES_NTF);
    }
}

void CDealFrame48::Deal( CRCClient* poClinet, UINT32 dwSerialNumber, UINT32 dwAreaID, UINT32 dwPlayerID, map<string, string> &mapField )
{
    if(NULL == poClinet)
    {
        return;
    }

    if (mapField[RES_BUILD_TYPE_IS_OPEN_PARAM].empty()
            || mapField[RES_BAG_ELSE_AMOUNT_PARAM].empty()
            || mapField[RES_STUDY_BAG_ELSE_AMOUNT_PARAM].empty()
            || mapField[GET_RES_INFO_VALUE_PARAM].empty()
            || mapField[GIVE_RES_INFO_VALUE_PARAM].empty()
            || mapField[PLAYER_ID].empty())
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
    }

    UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
    UINT32 dwParam1 = SDAtou(mapField[RES_BUILD_TYPE_IS_OPEN_PARAM].c_str());
    UINT32 dwParam2 = SDAtou(mapField[RES_BAG_ELSE_AMOUNT_PARAM].c_str());
    UINT32 dwParam3 = SDAtou(mapField[RES_STUDY_BAG_ELSE_AMOUNT_PARAM].c_str());

    string strParam5 = mapField[GET_RES_INFO_VALUE_PARAM];
    string strParam4 = mapField[GIVE_RES_INFO_VALUE_PARAM];

    const SPlayerBaseData* pPlayerBaseData = CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(unPlayerId);

    if (NULL == pPlayerBaseData)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_NOT_EXIT_ERR);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    CHAR szRet[512] = {0};
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(unPlayerId);

    if(NULL == poPlayer)
    {
        string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PLAYER_IS_NOT_ONLINE);
        poClinet->Rspone(strErrorMsg.c_str());
        return;
    }

    //建筑是否开放	dwParam1
    //物品背包空闲数	qwParam2
    //卦象背包空闲数	qwParam3
    if((0 != dwParam1) && (!poPlayer->GetBuild((EBuildKind)dwParam1)))
    {
        string strError = GetRsponeResult(ERR_GM_PARM_INFO::ID_BUILD_NO_OPEN);
        poClinet->Rspone(strError);
        return;
    }

    if(poPlayer->GetBag().GetIdleSize() < dwParam2)
    {
        string strError = GetRsponeResult(ERR_GM_PARM_INFO::ID_BAG_IS_NOT_ENOUTH);
        poClinet->Rspone(strError);
        return;
    }

    if(0 != dwParam3)
    {

        CAwaken* poAwaken = poPlayer->GetAwaken();

        if(NULL == poAwaken)
        {
            string strError = GetRsponeResult(ERR_GM_PARM_INFO::ID_STUDY_IS_NOT_OPEN);
            poClinet->Rspone(strError);
            return;
        }

        if(poAwaken->GetBagIdleSize() < dwParam3)
        {
            string strError = GetRsponeResult(ERR_GM_PARM_INFO::ID_STUDY_BAG_IS_NOT_ENOUGH);
            poClinet->Rspone(strError);
            return;
        }
    }

    //扣除物品解析
    vector<string> vecDecRes;
    map< string, vector<string> > mapDecRes;
    //检测对于没有值的都会传递默认值为0，要过虑这种没用的数据
    if (0 != SDStrcmp(strParam4.c_str(), "0"))
    {
        vecDecRes = SDSplitStringsA(strParam4.c_str(), ',');
        for(UINT8 byIdx = 0; byIdx < vecDecRes.size(); byIdx++)
        {
            vector<string> vecTmp = SDSplitStringsA(vecDecRes[byIdx], '|');

            if(vecTmp.size() < 2)
            {
                string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
                poClinet->Rspone(strErrorMsg.c_str());
                return;
            }

            mapDecRes[vecTmp[0]] = vecTmp;
        }
    }

    //增加物品解析
    vector<string> vecAddRes;
    map< string, vector<string> > mapAddRes;
    //检测对于没有值的都会传递默认值为0，要过虑这种没用的数据
    if (0 != SDStrcmp(strParam5.c_str(), "0"))
    {
        vecAddRes = SDSplitStringsA(strParam5.c_str(), ',');

        for(UINT8 byIdx = 0; byIdx < vecAddRes.size(); byIdx++)
        {
            vector<string> vecTmp = SDSplitStringsA(vecAddRes[byIdx], '|');

            if(vecTmp.size() < 2)
            {
                string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
                poClinet->Rspone(strErrorMsg.c_str());
                return;
            }

            mapAddRes[vecTmp[0]] = vecTmp;
        }
    }

    //检查数量是否足够
    for(map< string, vector<string> >::iterator itr = mapDecRes.begin(); itr != mapDecRes.end(); itr++)
    {
        if(!m_oManager.Deal(itr->first, poPlayer, itr->second, RESOPTYPE_CKNUM, 0, 0, 0))
        {
            string strError = GetRsponeResult(ERR_GM_PARM_INFO::ID_DEL_ITEM_NO_ENOUGH);
            poClinet->Rspone(strError);
            return;
        }
    }

    //检测添加的数量是否超过限制
    for(map< string, vector<string> >::iterator itr = mapAddRes.begin(); itr != mapAddRes.end(); itr++)
    {
        if(!m_oManager.Deal(itr->first, poPlayer, itr->second, RESOPTYPE_CK_ADD_LIMIT, 0, 0, 0))
        {
            string strErrorMsg = m_oManager.GetAddLimitMsg(itr->first);
            poClinet->Rspone(strErrorMsg);
            return;
        }
    }


    //检查资源的合法性
    string strRet;
    if(!CkResInvalid(mapDecRes, strRet))
    {
        string strError = GetRsponeResult(ERR_GM_PARM_INFO::ID_SOURCE_INVALID);
        poClinet->Rspone(strError);
        return;
    }

    //检查背包是否能容下
    CBag& oBag = poPlayer->GetBag();
    UINT8 byNeedNum = 0;
    for(map< string, vector<string> >::iterator itr = mapAddRes.begin(); itr != mapAddRes.end(); itr++)
    {
        UINT16 wItemID = SDAtou(itr->first.c_str());

        if(0 != wItemID)
        {
            if(!oBag.GetGoodsItem(wItemID)) //装备或不存在物品占1个格子,装备之能给1个
            {
                byNeedNum++;
            }
        }
    }

    if(oBag.GetIdleSize() < byNeedNum)
    {
        string strError = GetRsponeResult(ERR_GM_PARM_INFO::ID_BAG_IS_NOT_ENOUTH);
        poClinet->Rspone(strError);
        return;
    }

    //检查卦象背包是否能容下
    CAwaken* poAwaken = poPlayer->GetAwaken();
    map< string, vector<string> >::iterator itr = mapAddRes.find("studynoattr");
    if(itr != mapAddRes.end())
    {
        if(NULL == poAwaken)
        {
            string strError = GetRsponeResult(ERR_GM_PARM_INFO::ID_STUDY_IS_NOT_OPEN);
            poClinet->Rspone(strError);
            return;
        }

        if(poAwaken->GetBagIdleSize() < SDAtou(itr->second[1].c_str()))
        {
            string strError = GetRsponeResult(ERR_GM_PARM_INFO::ID_STUDY_BAG_IS_NOT_ENOUGH);
            poClinet->Rspone(strError);
            return;
        }
    }


    PKT_CLIGS_PLAYER_RES_NTF stNtf;
    //删除资源
    for(map< string, vector<string> >::iterator itr = mapDecRes.begin(); itr != mapDecRes.end(); itr++)
    {
        DT_PLAYER_RES_DATA& stDT_PLAYER_RES_DATA = stNtf.astResInfo[stNtf.byResNum];
        if(!m_oManager.Deal(itr->first, poPlayer, itr->second, RESOPTYPE_DECNUM, 0, 0, 0, "", &stDT_PLAYER_RES_DATA))
        {
            SYS_CRITICAL(_SDT("[%s:%d]: dec player[%u] res[%s] value[%u] failed!\n"), MSG_MARK, poPlayer->GetID(), itr->first.c_str(), itr->second[0].c_str()); //异常,继续
        }
        if(0 != stDT_PLAYER_RES_DATA.byResKind)
        {
            stNtf.byResNum++;
        }
    }

    //添加资源
    for(map< string, vector<string> >::iterator itr = mapAddRes.begin(); itr != mapAddRes.end(); itr++)
    {
        DT_PLAYER_RES_DATA& stDT_PLAYER_RES_DATA = stNtf.astResInfo[stNtf.byResNum];
        if(!m_oManager.Deal(itr->first, poPlayer, itr->second, RESOPTYPE_ADDNUM, 0, 0, 0, "", &stDT_PLAYER_RES_DATA))
        {
            SYS_CRITICAL(_SDT("[%s:%d]: add player[%u] res[%s] value[%u] failed!\n"), MSG_MARK, poPlayer->GetID(), itr->first.c_str(), itr->second[0].c_str()); //异常,继续
        }
        if(0 != stDT_PLAYER_RES_DATA.byResKind)
        {
            stNtf.byResNum++;
        }
    }

    map<string, string> mapJson;
    mapJson["dispname"] = string(poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName);
    string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &mapJson);
    poClinet->Rspone(strErrorMsg.c_str());

    //同步资源
    if(0 != stNtf.byResNum)
    {
        stNtf.byResNum = stNtf.byResNum > MAX_PLAYER_RES_NUM ? MAX_PLAYER_RES_NUM : stNtf.byResNum;
        poPlayer->SendMsg((CHAR*)&stNtf, CLIGS_PLAYER_RES_NTF);
    }
}

string CDealFrame48::GetStrFrameType()
{
    return GET_GIVE_RES;
}

void CDealFrame48::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string, string> mapField;
    mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;
    mapField[RES_BUILD_TYPE_IS_OPEN_PARAM] = SDGBK2UTF8(RES_BUILD_TYPE_IS_OPEN_GBK) + "|" + GM_PARAM_INT;
    mapField[RES_BAG_ELSE_AMOUNT_PARAM] = SDGBK2UTF8(RES_BAG_ELSE_AMOUNT_GBK) + "|" + GM_PARAM_INT;
    mapField[RES_STUDY_BAG_ELSE_AMOUNT_PARAM] = SDGBK2UTF8(RES_STUDY_BAG_ELSE_AMOUNT_GBK) + "|" + GM_PARAM_INT;
    mapField[GET_RES_INFO_VALUE_PARAM] = SDGBK2UTF8(GET_RES_GBK) + "|" + GM_PARAM_STR;
    mapField[GIVE_RES_INFO_VALUE_PARAM] = SDGBK2UTF8(GIVE_RES_GBK) + "|" + GM_PARAM_STR;

    RsponeMsg(poClinet, mapField);
}

string CDealFrame48::GetStrFrameDesc()
{
    return SDGBK2UTF8(GET_GIVE_RES_DESC);
}

