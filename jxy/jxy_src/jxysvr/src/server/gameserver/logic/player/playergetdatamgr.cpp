#include "playergetdatamgr.h"
#include <net/db/dbpktbuilder.h>

IMPLEMENT_SINGLETON(CGetPlayerDataMgr)

CGetPlayerDataMgr::CGetPlayerDataMgr():
    m_oGetPlayerDataPool(MAX_PLAYER_GET_DATA_POOL_NUM),
    m_oGetPlayerDataPool2(MAX_PLAYER_GET_DATA_POOL_NUM)
{

}

CGetPlayerDataMgr::~CGetPlayerDataMgr()
{

}

BOOL CGetPlayerDataMgr::Init()
{
    return TRUE;
}


VOID CGetPlayerDataMgr::UnInit()
{

}

VOID CGetPlayerDataMgr::GetPlayerData(UINT32 dwTransID, UINT8 byGetKind, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, string strParam, UINT8	byAction, UINT32   dwSerialNumber, string strDesc, CRCClient* poClinet)
{
    SGetPlayerData* pstGetPlayerData = m_oGetPlayerDataPool.Alloc();
    if(NULL == pstGetPlayerData)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Alloc SGetPlayerData failed!"), MSG_MARK);
        return;
    }

    DBG_INFO(_SDT("[%s: %d]: Alloc GetPlayerDataPool address:%0x!"), MSG_MARK, pstGetPlayerData);
    pstGetPlayerData->stGetPlayerDataReq.dwTransID = dwTransID;
    pstGetPlayerData->stGetPlayerDataReq.byGetKind = byGetKind;
    pstGetPlayerData->stGetPlayerDataReq.dwPlayerID = dwPlayerID;
    pstGetPlayerData->stGetPlayerDataReq.byAction = byAction;
    pstGetPlayerData->stGetPlayerDataReq.dwParam1 = dwParam1;
    pstGetPlayerData->stGetPlayerDataReq.qwParam2 = qwParam2;
    pstGetPlayerData->stGetPlayerDataReq.qwParam3 = qwParam3;
    pstGetPlayerData->stGetPlayerDataReq.dwSerialNumber = dwSerialNumber;
    pstGetPlayerData->stGetPlayerDataReq.poClinet = poClinet;
    SDStrncpy(pstGetPlayerData->stGetPlayerDataReq.szParam, strParam.c_str(), strParam.size());
	SDStrncpy(pstGetPlayerData->stGetPlayerDataReq.szDesc, strDesc.c_str(), strDesc.size());
    CDBPktBuilder::Instance()->GetPlayerDataReq(pstGetPlayerData);
}


//VOID CGetPlayerDataMgr::GetPlayerDataOld(UINT32 dwTransID, UINT8 byGetKind, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, string strParam, UINT8	byAction, UINT32   dwSerialNumber)
//{
//    SGetPlayerData* pstGetPlayerData = m_oGetPlayerDataPool.Alloc();
//    if(NULL == pstGetPlayerData)
//    {
//        SYS_CRITICAL(_SDT("[%s: %d]: Alloc SGetPlayerData failed!"), MSG_MARK);
//        return;
//    }
//
//    DBG_INFO(_SDT("[%s: %d]: Alloc GetPlayerDataPool address:%0x!"), MSG_MARK, pstGetPlayerData);
//    pstGetPlayerData->stGetPlayerDataReq.dwTransID = dwTransID;
//    pstGetPlayerData->stGetPlayerDataReq.byGetKind = byGetKind;
//    pstGetPlayerData->stGetPlayerDataReq.dwPlayerID = dwPlayerID;
//    pstGetPlayerData->stGetPlayerDataReq.byAction = byAction;
//    pstGetPlayerData->stGetPlayerDataReq.dwParam1 = dwParam1;
//    pstGetPlayerData->stGetPlayerDataReq.qwParam2 = qwParam2;
//    pstGetPlayerData->stGetPlayerDataReq.dwSerialNumber = dwSerialNumber;
//    pstGetPlayerData->stGetPlayerDataReq.strParam = strParam;
//
//    CDBPktBuilder::Instance()->GetPlayerDataReqOld(pstGetPlayerData);
//}

VOID CGetPlayerDataMgr::OnGetPlayerData(SGetPlayerData* pstGetPlayerData)
{
    if(pstGetPlayerData)
    {
        //delete pstGetPlayerData;
        DBG_INFO(_SDT("[%s: %d]: Free GetPlayerDataPool address:%0x!"), MSG_MARK, pstGetPlayerData);
        m_oGetPlayerDataPool.Free(pstGetPlayerData);
    }
}

VOID CGetPlayerDataMgr::GetPlayerData2(UINT32 dwTransID, UINT8 byGetKind, UINT32 dwPlayerID1, UINT32 dwPlayerID2, UINT32 dwParam1/*=0*/, UINT64 qwParam2/*=0*/ )
{
    SGetPlayerData2* pstGetPlayerData = m_oGetPlayerDataPool2.Alloc();
    if(NULL == pstGetPlayerData)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Alloc SGetPlayerData2 failed!"), MSG_MARK);
        return;
    }

    DBG_INFO(_SDT("[%s: %d]: Alloc GetPlayerDataPool2 address:%0x!"), MSG_MARK, pstGetPlayerData);
    pstGetPlayerData->dwTransID		= dwTransID;
    pstGetPlayerData->byGetKind		= byGetKind;
    pstGetPlayerData->dwPlayerID1		= dwPlayerID1;
    pstGetPlayerData->dwPlayerID2		= dwPlayerID2;
    pstGetPlayerData->dwParam1			= dwParam1;
    pstGetPlayerData->qwParam2			= qwParam2;
    CDBPktBuilder::Instance()->GetPlayerDataReq2(pstGetPlayerData);
}


VOID CGetPlayerDataMgr::OnGetPlayerData2(SGetPlayerData2* pstGetPlayerData)
{
    if(pstGetPlayerData)
    {
        DBG_INFO(_SDT("[%s: %d]: Free GetPlayerDataPool2 address:%0x!"), MSG_MARK, pstGetPlayerData);
        //delete pstGetPlayerData;
        m_oGetPlayerDataPool2.Free(pstGetPlayerData);
    }
}
