// DealFrame38.cpp: implementation of the CDealFrame38 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe38.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/activityencourage/activityencouragemgr.h"


extern  string GetACTEItemKindTypeName(UINT8 byKindType);
extern  string GetACTEItemKindValueName(UINT8 byKindType, UINT16 wKindID);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame38::CDealFrame38()
{

}

CDealFrame38::~CDealFrame38()
{

}

void CDealFrame38::Release()
{

}

UINT16 CDealFrame38::GetFrameType()
{
    return 38;
}

void CDealFrame38::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    //if((NULL == poClinet) || (NULL == pszParam4))
    //{
    //    return;
    //}
    //CActivityEncouragePropMap& mapActivityEncouragePropMap = CActivityEncourageMgr::Instance()->GetNorActivityEncouragePropMap();
    ////CActivityEncouragePropMapItr iter

    //CNGString ssMsg;
    //for (CActivityEncouragePropMapItr iter = mapActivityEncouragePropMap.begin(); iter != mapActivityEncouragePropMap.end(); ++iter)
    //{
    //    //UINT16 wActivityID = iter->first;
    //    //SActivityEncouragePro stActivityEncouragePro = iter->second;

    //    if (iter != mapActivityEncouragePropMap.begin())
    //    {
    //        ssMsg += "|{";
    //        ssMsg += "ActivityID=";
    //        ssMsg += iter->second.dwActivityID;
    //        ssMsg += ",";
    //        ssMsg += ",";
    //        ssMsg += "IsNeedCdKey=";
    //        ssMsg += iter->second.byNeedCdKey;
    //        ssMsg += ",";
    //        ssMsg += "ActivityName=";
    //        ssMsg += iter->second.szActivityName;
    //        ssMsg += ",";
    //        ssMsg += "ActivityDesc=";
    //        ssMsg += iter->second.szActivityDesc;
    //        ssMsg += ",";

    //        //
    //        ssMsg += "EncourageItem=[";
    //        for (CJsonResItemVectorItr eiIter = iter->second.vecActivityEncourageItem.begin(); eiIter != iter->second.vecActivityEncourageItem.end(); ++eiIter)
    //        {
    //            if (eiIter != iter->second.vecActivityEncourageItem.begin())
    //            {
    //                ssMsg += ",(";
    //                ssMsg += "KindType=";
    //                ssMsg += UINT16(eiIter->byKindType);
    //                ssMsg += ",";
    //                ssMsg += "KindTypeName=";
    //                ssMsg += GetACTEItemKindTypeName(eiIter->byKindType);
    //                ssMsg += ",";
    //                ssMsg += "KindValue=";
    //                ssMsg += eiIter->dwKindValue;
    //                ssMsg += ",";
    //                ssMsg += "KindValueName=";
    //                ssMsg += GetACTEItemKindValueName(eiIter->byKindType, UINT16(eiIter->dwKindValue));
    //                ssMsg += ",";
    //                ssMsg += "Num=";
    //                ssMsg += UINT16(eiIter->byNum);
    //                ssMsg += ",";
    //                ssMsg += ")";
    //            }
    //            else
    //            {
    //                ssMsg += "(";
    //                ssMsg += "KindType=";
    //                ssMsg += UINT16(eiIter->byKindType);
    //                ssMsg += ",";
    //                ssMsg += "KindTypeName=";
    //                ssMsg += GetACTEItemKindTypeName(eiIter->byKindType);
    //                ssMsg += ",";
    //                ssMsg += "KindValue=";
    //                ssMsg += eiIter->dwKindValue;
    //                ssMsg += ",";
    //                ssMsg += "KindValueName=";
    //                ssMsg += GetACTEItemKindValueName(eiIter->byKindType, UINT16(eiIter->dwKindValue));
    //                ssMsg += ",";
    //                ssMsg += "Num=";
    //                ssMsg += UINT16(eiIter->byNum);
    //                ssMsg += ",";
    //                ssMsg += ")";
    //            }
    //        }
    //        ssMsg += "],";

    //        //
    //        ssMsg += "EncouragePlayer=[";
    //        for (CActivityEncouragePlayerMapItr epIter = iter->second.mapActivityEncourageStartPlayerID.begin(); epIter != iter->second.mapActivityEncourageStartPlayerID.end(); ++epIter)
    //        {
    //            if (epIter != iter->second.mapActivityEncourageStartPlayerID.begin())
    //            {
    //                ssMsg += ",(";
    //                ssMsg += "StartPlayerID=";
    //                ssMsg += epIter->first;
    //                ssMsg += ",";
    //                ssMsg += "EndPlayerID=";
    //                ssMsg += epIter->second;
    //                ssMsg += ")";
    //            }
    //            else
    //            {
    //                ssMsg += "(";
    //                ssMsg += "StartPlayerID=";
    //                ssMsg += epIter->first;
    //                ssMsg += ",";
    //                ssMsg += "EndPlayerID=";
    //                ssMsg += epIter->second;
    //                ssMsg += ")";
    //            }
    //        }
    //        ssMsg += "]";
    //        ssMsg += "}";
    //    }
    //    else
    //    {
    //        ssMsg += "{";
    //        ssMsg += "ActivityID=";
    //        ssMsg += iter->second.dwActivityID;
    //        ssMsg += ",";
    //        ssMsg += ",";
    //        ssMsg += "IsNeedCdKey=";
    //        ssMsg += iter->second.byNeedCdKey;
    //        ssMsg += ",";
    //        ssMsg += "ActivityName=";
    //        ssMsg += iter->second.szActivityName;
    //        ssMsg += ",";
    //        ssMsg += "ActivityDesc=";
    //        ssMsg += iter->second.szActivityDesc;
    //        ssMsg += ",";

    //        //
    //        ssMsg += "EncourageItem=[";
    //        for (CJsonResItemVectorItr eiIter = iter->second.vecActivityEncourageItem.begin(); eiIter != iter->second.vecActivityEncourageItem.end(); ++eiIter)
    //        {
    //            if (eiIter != iter->second.vecActivityEncourageItem.begin())
    //            {
    //                ssMsg += ",(";
    //                ssMsg += "KindType=";
    //                ssMsg += UINT16(eiIter->byKindType);
    //                ssMsg += ",";
    //                ssMsg += "KindTypeName=";
    //                ssMsg += GetACTEItemKindTypeName(eiIter->byKindType);
    //                ssMsg += ",";
    //                ssMsg += "KindValue=";
    //                ssMsg += eiIter->dwKindValue;
    //                ssMsg += ",";
    //                ssMsg += "KindValueName=";
    //                ssMsg += GetACTEItemKindValueName(eiIter->byKindType, UINT16(eiIter->dwKindValue));
    //                ssMsg += ",";
    //                ssMsg += "Num=";
    //                ssMsg += UINT16(eiIter->byNum);
    //                ssMsg += ",";
    //                ssMsg += ")";
    //            }
    //            else
    //            {
    //                ssMsg += "(";
    //                ssMsg += "KindType=";
    //                ssMsg += UINT16(eiIter->byKindType);
    //                ssMsg += ",";
    //                ssMsg += "KindTypeName=";
    //                ssMsg += GetACTEItemKindTypeName(eiIter->byKindType);
    //                ssMsg += ",";
    //                ssMsg += "KindValue=";
    //                ssMsg += eiIter->dwKindValue;
    //                ssMsg += ",";
    //                ssMsg += "KindValueName=";
    //                ssMsg += GetACTEItemKindValueName(eiIter->byKindType, UINT16(eiIter->dwKindValue));
    //                ssMsg += ",";
    //                ssMsg += "Num=";
    //                ssMsg += UINT16(eiIter->byNum);
    //                ssMsg += ",";
    //                ssMsg += ")";
    //            }
    //        }
    //        ssMsg += "],";

    //        //
    //        ssMsg += "EncouragePlayer=[";
    //        for (CActivityEncouragePlayerMapItr epIter = iter->second.mapActivityEncourageStartPlayerID.begin(); epIter != iter->second.mapActivityEncourageStartPlayerID.end(); ++epIter)
    //        {
    //            if (epIter != iter->second.mapActivityEncourageStartPlayerID.begin())
    //            {
    //                ssMsg += ",(";
    //                ssMsg += "StartPlayerID=";
    //                ssMsg += epIter->first;
    //                ssMsg += ",";
    //                ssMsg += "EndPlayerID=";
    //                ssMsg += epIter->second;
    //                ssMsg += ")";
    //            }
    //            else
    //            {
    //                ssMsg += "(";
    //                ssMsg += "StartPlayerID=";
    //                ssMsg += epIter->first;
    //                ssMsg += ",";
    //                ssMsg += "EndPlayerID=";
    //                ssMsg += epIter->second;
    //                ssMsg += ")";
    //            }
    //        }
    //        ssMsg += "]";
    //        ssMsg += "}";
    //    }
    //}

    //if(ssMsg.str().empty())
    //{
    //    poClinet->Rspone("{\"status\":\"no data!\"}");
    //}
    //else
    //{
    //    poClinet->Rspone(ssMsg.str());
    //}
}

void CDealFrame38::Deal( CRCClient* poClinet, UINT32 dwSerialNumber, UINT32 dwAreaID, UINT32 dwPlayerID, map<string, string> &mapField )
{
    if(NULL == poClinet)
    {
        return;
    }

    //CActivityEncouragePropMap& mapActivityEncouragePropMap = CActivityEncourageMgr::Instance()->GetNorActivityEncouragePropMap();
    //if	(mapActivityEncouragePropMap.size() == 0)
    //{
    //    string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_NOT_DATA);
    //    poClinet->Rspone(strErrorMsg.c_str());
    //    return;
    //}
    //vector<string>jsonData;
    //for (CActivityEncouragePropMapItr iter = mapActivityEncouragePropMap.begin(); iter != mapActivityEncouragePropMap.end(); ++iter)
    //{
    //    CNGString ssMsg = "";
    //    ssMsg += "{";
    //    ssMsg += "\"ActivityID\":";
    //    ssMsg += iter->second.dwActivityID;
    //    ssMsg += ",";
    //    ssMsg += ",";
    //    ssMsg += "\"IsNeedCdKey\":";
    //    ssMsg += iter->second.byNeedCdKey;
    //    ssMsg += ",";
    //    ssMsg += "\"ActivityName\":\"";
    //    ssMsg += iter->second.szActivityName;
    //    ssMsg += "\",";
    //    ssMsg += "\"ActivityDesc\":\"";
    //    ssMsg += iter->second.szActivityDesc;
    //    ssMsg += "\",";

    //    //
    //    ssMsg += "\"EncourageItem\":[";

    //    for (CJsonResItemVectorItr eiIter = iter->second.vecActivityEncourageItem.begin(); eiIter != iter->second.vecActivityEncourageItem.end(); ++eiIter)
    //    {
    //        if (eiIter != iter->second.vecActivityEncourageItem.begin())
    //        {
    //            ssMsg += ",{";
    //        }
    //        else
    //        {
    //            ssMsg += "{";
    //        }

    //        ssMsg += "\"KindType\":";
    //        ssMsg += UINT16(eiIter->byKindType);
    //        ssMsg += ",";
    //        ssMsg += "\"KindTypeName\":\"";
    //        ssMsg += GetACTEItemKindTypeName(eiIter->byKindType);
    //        ssMsg += "\",";
    //        ssMsg += "\"KindValue\":";
    //        ssMsg += eiIter->dwKindValue;
    //        ssMsg += ",";
    //        ssMsg += "\"KindValueName\":\"";
    //        ssMsg += GetACTEItemKindValueName(eiIter->byKindType, UINT16(eiIter->dwKindValue));
    //        ssMsg += "\",";
    //        ssMsg += "\"Num\":";
    //        ssMsg += UINT16(eiIter->byNum);
    //        ssMsg += "}";
    //    }
    //    ssMsg += "],";

    //    //
    //    ssMsg += "EncouragePlayer=[";
    //    for (CActivityEncouragePlayerMapItr epIter = iter->second.mapActivityEncourageStartPlayerID.begin(); epIter != iter->second.mapActivityEncourageStartPlayerID.end(); ++epIter)
    //    {
    //        if (epIter != iter->second.mapActivityEncourageStartPlayerID.begin())
    //        {
    //            ssMsg += ",{";
    //        }
    //        else
    //        {
    //            ssMsg += "{";
    //        }

    //        ssMsg += "\"StartPlayerID\":";
    //        ssMsg += epIter->first;
    //        ssMsg += ",";
    //        ssMsg += "\"EndPlayerID\":";
    //        ssMsg += epIter->second;
    //        ssMsg += "}";
    //    }
    //    ssMsg += "]";
    //    ssMsg += "}";
    //    jsonData.push_back(ssMsg.str());

    //}
    //string strRetInfo = "";
    //strRetInfo = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &jsonData);
    //poClinet->Rspone(strRetInfo.c_str());
}

string CDealFrame38::GetStrFrameType()
{
    return GET_ACTIVITY_ENCOURAGE;
}

void CDealFrame38::Interface( CRCClient* poClinet )
{
    if (NULL == poClinet)
    {
        return;
    }

    map<string, string> mapField;
    RsponeMsg(poClinet, mapField);
}

string CDealFrame38::GetStrFrameDesc()
{
    return SDGBK2UTF8(GET_ACTIVITY_ENCOURAGE_DESC);
}

