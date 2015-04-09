// DealFrame.cpp: implementation of the CGmDealFrame class.
//
//////////////////////////////////////////////////////////////////////


#include <assert.h>
#include "gmdealframe.h"
#include <sdstring.h>
#include "net/recharge/rcclient.h"

using namespace SGDP;

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace InterfaceDealBase
{
CGmDealFrame::CGmDealFrame()
{
    m_pManager = NULL;
}

CGmDealFrame::CGmDealFrame(CGmManager * IN_pManager): m_pManager(IN_pManager)
{

}

CGmDealFrame::~CGmDealFrame()
{
    Release();
}

bool CGmDealFrame::init(CGmManager * IN_pManager)
{
    m_pManager = IN_pManager;
    RegisterDealFrame(this);
    return TRUE;
}

void CGmDealFrame::Release()
{

}

//注册处理类型与对象指针的对应关系
void CGmDealFrame::RegisterDealFrame(CGmDealFrame *v_pDealFrame)
{
    if(m_pManager)
        m_pManager->RegisterDealFrame(v_pDealFrame);
}




void CGmDealFrame::RsponeMsg( CRCClient* poClient, map<string, string> &mapFiled)
{
    if (NULL == poClient)
    {
        return;
    }

    vector<string>jsonData;
    for (iter = mapFiled.begin(); iter != mapFiled.end(); iter++)
    {
        vector<string> vecInfo = SDSplitStringsA(iter->second.c_str(), '|');
        if (vecInfo.size() != 2)
        {
            return;
        }
        CHAR szRet[2048] = {0};
        SDStrcat(szRet, "{");
        SDStrcat(szRet, "\"");
        SDStrcat(szRet, iter->first.c_str());
        SDStrcat(szRet, "\"");
        SDStrcat(szRet, ":");
        SDStrcat(szRet, "\"");
        SDStrcat(szRet, vecInfo[0].c_str());
        SDStrcat(szRet, "\"");
        SDStrcat(szRet, ",\"type\":\"");
        SDStrcat(szRet, vecInfo[1].c_str());
        SDStrcat(szRet, "\"");
        SDStrcat(szRet, "}");
        jsonData.push_back(szRet);
    }
    poClient->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &jsonData));
}


CGmManager::CGmManager()
{
    m_pInstance = this;
}


CGmManager::~CGmManager()
{
    Release();
}


//获取本对象实例
CGmManager* CGmManager::instance()
{
    return m_pInstance;
}


void CGmManager::Release()
{
    try
    {
        for(MapTaskIter itr = m_mapTask.begin(); itr != m_mapTask.end(); itr++ )
        {
            itr->second->Release();
            delete itr->second;
        }
        m_mapTask.clear();
    }
    catch(...)
    {
        return ;
    }
    return ;
}


//注册处理类型与对象指针的对应关系
void CGmManager::RegisterDealFrame(CGmDealFrame * IN_pDealFrame)
{
    assert(m_mapTask.find(IN_pDealFrame->GetFrameType()) == m_mapTask.end());
    assert(m_mapStrTask.find(IN_pDealFrame->GetStrFrameType()) == m_mapStrTask.end());
    m_mapTask.insert(std::make_pair(IN_pDealFrame->GetFrameType(), IN_pDealFrame));
    m_mapStrTask.insert(std::make_pair(IN_pDealFrame->GetStrFrameType(), IN_pDealFrame));
}

//获取对象指针
CGmDealFrame * CGmManager::GetInstanceByType(UINT16 wProto)
{
    MapTaskIter         iter;
    iter = m_mapTask.find(wProto);
    if( m_mapTask.end() == iter )
    {
        return NULL;
    }
    return iter->second;
}

CGmDealFrame * CGmManager::GetInstanceByType(string strProto)
{
    MapStrTaskIter         iter;
    iter = m_mapStrTask.find(strProto);
    if( m_mapStrTask.end() == iter )
    {
        return NULL;
    }
    return iter->second;
}


void CGmManager::Deal( UINT16 wProto, CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
    InterfaceDealBase::CGmDealFrame *pDealFrame = GetInstanceByType(wProto);
    if(pDealFrame)
        pDealFrame->Deal(poClinet, dwSerialNumber, byAction, dwAreaID, dwPlayerID, dwParam1, qwParam2, qwParam3, pszParam4, pszParam5, pszDesc);
}

void CGmManager::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT32 dwAreaID, UINT32 dwPlayerID, map<string, string> &mapField, string strCmdInfo)
{
    InterfaceDealBase::CGmDealFrame *pDealFrame = GetInstanceByType(strCmdInfo.c_str());

    if(pDealFrame)
    {
        pDealFrame->Deal(poClinet, dwSerialNumber, dwAreaID, dwPlayerID, mapField);
    }
}

string CGmManager::GetGmInterfaceCmd(const string strProto)
{
    string strCmd = "";
    return strCmd;
}

void CGmManager::Interface(string strProto, CRCClient* poClinet)
{
    InterfaceDealBase::CGmDealFrame *pDealFrame = GetInstanceByType(strProto);

    if(pDealFrame)
    {
        pDealFrame->Interface(poClinet);
    }
    else
    {

        if (0 != strProto.compare("all"))
        {
            poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_FIND_NO));
            return;
        }
        vector<string>jsonData;
        MapTaskIter     iter;
        for (iter = m_mapTask.begin(); iter != m_mapTask.end(); iter++)
        {
            char	v_stData[1024] = {0};
            SDStrcat(v_stData, "{");
            char v_szTemp[56] = {0};
            sprintf(v_szTemp, "%u", iter->first);
            SDStrcat(v_stData, "\"");
            SDStrcat(v_stData, v_szTemp);
            SDStrcat(v_stData, "\"");
            SDStrcat(v_stData, ":");
            SDStrcat(v_stData, "\"");
            SDStrcat(v_stData, iter->second->GetStrFrameType().c_str());
            SDStrcat(v_stData, "\"");
            SDStrcat(v_stData, ",");
            SDStrcat(v_stData, "\"");
            SDStrcat(v_stData, "desc\":\"");
            SDStrcat(v_stData, iter->second->GetStrFrameDesc().c_str());
            SDStrcat(v_stData, "\"");
            SDStrcat(v_stData, "}");
            jsonData.push_back(v_stData);
        }
        poClinet->Rspone(GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS, &jsonData));
    }
}

}
