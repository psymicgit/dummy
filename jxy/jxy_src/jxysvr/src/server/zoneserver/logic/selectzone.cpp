
#include "framework/zsapplication.h"
#include "selectzone.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include "framework/usermgr.h"
//#include "common/third/ppclient.h"

#include <fstream>
#include <sstream>
#include <ostream>
using namespace std;


CSelectZoneCMD::CSelectZoneCMD()
{
    memset(&m_stAck, 0, sizeof(m_stAck));
}


CSelectZoneCMD::~CSelectZoneCMD()
{
}

VOID CSelectZoneCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    PKT_CLIGT_SELECT_ZONE_REQ* pstReq = (PKT_CLIGT_SELECT_ZONE_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;
    if (FALSE == SelectInfo(poDBConn, pstReq))
    {
        m_bSuccess = FALSE;
    }
    else
    {
        m_bSuccess = TRUE;
    }
}

VOID CSelectZoneCMD::OnExecuted()
{
    ResponseMsg(CLIGT_SELECT_ZONE_ACK, (CHAR*)(&m_stAck));
}


BOOL CSelectZoneCMD::SelectInfo(SGDP::ISDDBConnection* poDBConn, PKT_CLIGT_SELECT_ZONE_REQ* pstReq)
{
 //   m_stAck.wErrCode = ERR_SELECT_ZONE::ID_SUCCESS;
 //   CHAR szSQL[1024] = {0};
 //   ISDDBRecordSet* pRecordSet = NULL;

 //   pstReq->byPwdLen = pstReq->byPwdLen >= USERNAME_LEN ? (USERNAME_LEN - 1) : pstReq->byPwdLen;
 //   pstReq->abyUserPwd[pstReq->byPwdLen] = '\0';

 //   if (AUTH_TYPE_MAXNET != pstReq->byAuthType)
 //   {
 //       pstReq->abyUserPwd[0] = '\0';
 //   }

 //   //检查参数
 //   if(HaveSqlToken(pstReq->aszDeviceID) ||
 //           HaveSqlToken(pstReq->aszUserName) ||
 //           HaveSqlToken((TCHAR*)pstReq->abyUserPwd) ||
 //           HaveSqlToken(pstReq->aszNickName))
 //   {
 //       m_stAck.wErrCode =  ERR_SELECT_ZONE::ID_INVALID_CHARACTER;
 //       return FALSE;
 //   }

	////字符过虑
	//TCHAR       aszUserName[USERNAME_LEN] = {0}; //用户名
	//MisplacedToken(pstReq->aszUserName, aszUserName);

 //   UINT32	dwUserID =	0;
 //   UINT32	dwErr	=	0;
 //   string	strError;
 //   INT32	nRet		=	0;
 //   CHAR		szErrMsg[128] = {0};

 //   CC_LOOP_DO
 //   //检测是否相等
 //   m_stAck.wErrCode = ERR_SELECT_ZONE::ID_SUCCESS;
 //   if(CUserMgr::Instance()->CkUser(dwUserID, pstReq->byAuthType, pstReq->byAuthMethod, aszUserName, (TCHAR*)pstReq->abyUserPwd, pstReq->aszDeviceID, pstReq->aszNickName) && 0 != dwUserID)
 //   {
 //       break;
 //   }

 //   sprintf(szSQL, "call sp_UserAuthEx(%d, '%s', '%s', '%s', %d, '%s', %u, 1);",
 //           pstReq->byAuthType, pstReq->aszDeviceID, aszUserName,
 //           pstReq->abyUserPwd, pstReq->byAuthMethod, pstReq->aszNickName, pstReq->byBindType);

 //   SYS_CRITICAL(_SDT("%s"), szSQL);

 //   nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, &dwErr, &strError);
 //   if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
 //   {
 //       dwUserID = SDAtou(pRecordSet->GetFieldValueByName("UserID"));
 //       m_stAck.wErrCode = SDAtou(pRecordSet->GetFieldValueByName("ErrCode"));
 //       SDStrcpy(szErrMsg, pRecordSet->GetFieldValueByName("ErrStr"));
 //       switch(m_stAck.wErrCode)
 //       {
 //       case 0:
 //           m_stAck.wErrCode = ERR_SELECT_ZONE::ID_SUCCESS;
 //           break;
 //       case 1:
 //           m_stAck.wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
 //           break;
 //       case 2:
 //           m_stAck.wErrCode = ERR_SELECT_ZONE::ID_INVALID_AUTH_TYPE;
 //           break;
 //       case 3:
 //           m_stAck.wErrCode = ERR_SELECT_ZONE::ID_USERNAME_NOT_EXIST;
 //           break;
 //       case 4:
 //           m_stAck.wErrCode = ERR_SELECT_ZONE::ID_FORBIDLOGIN;
 //           break;
 //       default:
 //           m_stAck.wErrCode = ERR_COMMON::ID_OTHER_ERR;
 //           break;
 //       }
 //   }
 //   else
 //   {
 //       m_stAck.wErrCode = ERR_COMMON::ID_OTHER_ERR;
 //       SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
 //   }
 //   if(NULL != pRecordSet)
 //   {
 //       pRecordSet->Release();
 //       pRecordSet = NULL;
 //   }
 //   //创建用户数据
 //   if(ERR_SELECT_ZONE::ID_SUCCESS == m_stAck.wErrCode && 0 != dwUserID)
 //   {
 //       CUserMgr::Instance()->CreateUser(dwUserID, pstReq->byAuthType, pstReq->byAuthMethod, aszUserName, (CHAR*)pstReq->abyUserPwd, pstReq->aszDeviceID, pstReq->aszNickName);
 //   }
 //   CC_LOOP_WHILE(0);


 //   if(ERR_SELECT_ZONE::ID_SUCCESS != m_stAck.wErrCode)
 //   {
 //       return TRUE;
 //   }
 //   m_stAck.byZoneNum = 0;


 //   //未登录/注册的新用户
 //   if(0 == dwUserID)
 //   {
 //       CZoneInfoVec vecZoneInfo = ((CZSApplication*)SDGetApp())->GetDBMgr()->GetZoneInfo();
 //       //UINT16 wSize = vecZoneInfo.size();
 //       //UINT16 wZoneIdx = wSize > (MAX_ZONE_NUM * (pstReq->wZoneIdx + 1)) ? (MAX_ZONE_NUM * (pstReq->wZoneIdx + 1)) : wSize;
 //       //for(int nIdx = wZoneIdx - 1; ((nIdx >= MAX_ZONE_NUM * pstReq->wZoneIdx) & (m_stAck.byZoneNum < MAX_ZONE_NUM)); nIdx--)
 //       for(int nIdx = vecZoneInfo.size() - 1; m_stAck.byZoneNum < MAX_ZONE_NUM && nIdx >= 0; nIdx--)
 //       {
 //           try
 //           {
 //               DT_ZONE_DATA_INFO &stDT_ZONE_DATA_INFO = vecZoneInfo.at(nIdx);
 //               if(2 == stDT_ZONE_DATA_INFO.byVisible ||
 //                       stDT_ZONE_DATA_INFO.wMinVersionID > pstReq->dwCliVersion ||
 //                       stDT_ZONE_DATA_INFO.wMaxVersionID < pstReq->dwCliVersion)
 //               {
 //                   continue;
 //               }
 //               memcpy(&m_stAck.astZoneList[m_stAck.byZoneNum++], &stDT_ZONE_DATA_INFO.stDT_ZONE_DATA, sizeof(DT_ZONE_DATA));
 //           }
 //           catch (...)
 //           {

 //           }
 //       }

 //       return TRUE;
 //   }

 //   //老用户
 //   sprintf(szSQL, "call sp_SelectZone(%d, %d, %d, %d);", dwUserID, pstReq->wZoneIdx, MAX_ZONE_NUM, pstReq->dwCliVersion);

 //   SYS_CRITICAL(_SDT("%s"), szSQL);

 //   UINT16 wLastZoneID  = 0;
 //   DT_ZONE_DATA* pstDT_ZONE_DATA = NULL;
 //   std::vector<SGDP::ISDDBRecordSet*> vecRs;
 //   nRet = poDBConn->ExecuteSqlRs(szSQL, vecRs);
 //   if(SDDB_HAS_RECORDSET == nRet)
 //   {
 //       UINT16 wSize = vecRs.size();
 //       for(UINT16 wIdx = 0; wIdx < wSize; wIdx++)
 //       {
 //           pRecordSet = vecRs[wIdx];
 //           while(pRecordSet->GetRecord())
 //           {
 //               if(0 == dwUserID)
 //               {
 //                   DT_ZONE_DATA& stDT_ZONE_DATA = m_stAck.astZoneList[m_stAck.byZoneNum++];
 //                   stDT_ZONE_DATA.wID = SDAtou(pRecordSet->GetFieldValueByName("ID"));

 //                   stDT_ZONE_DATA.byDomainLen = pRecordSet->GetFieldLengthByName("DomainName");
 //                   stDT_ZONE_DATA.byDomainLen = stDT_ZONE_DATA.byDomainLen > MAX_DOMAIN_NAME_LEN ? MAX_DOMAIN_NAME_LEN : stDT_ZONE_DATA.byDomainLen;
 //                   memcpy(stDT_ZONE_DATA.abyDomainName, pRecordSet->GetFieldValueByName("DomainName"), stDT_ZONE_DATA.byDomainLen);

 //                   stDT_ZONE_DATA.wZonePort = SDAtou(pRecordSet->GetFieldValueByName("Port"));
 //                   SDStrncpy(stDT_ZONE_DATA.aszZoneName, pRecordSet->GetFieldValueByName("ZoneName"), USERNAME_LEN - 1);
 //                   stDT_ZONE_DATA.byBusyFlag = SDAtou(pRecordSet->GetFieldValueByName("BusyFlag"));
 //                   wLastZoneID = stDT_ZONE_DATA.wID;
 //               }
 //               else
 //               {
 //                   UINT16 wZoneID = SDAtou(pRecordSet->GetFieldValueByName("ID"));
 //                   if((wZoneID != wLastZoneID) || (NULL == pstDT_ZONE_DATA))
 //                   {
 //                       pstDT_ZONE_DATA = &(m_stAck.astZoneList[m_stAck.byZoneNum++]);
 //                       pstDT_ZONE_DATA->wID = wZoneID;
 //                       //pstDT_ZONE_DATA->dwZoneIP = inet_addr(SDUTF82A(pRecordSet->GetFieldValueByName("IP")).c_str());

 //                       pstDT_ZONE_DATA->byDomainLen = pRecordSet->GetFieldLengthByName("DomainName");
 //                       pstDT_ZONE_DATA->byDomainLen = pstDT_ZONE_DATA->byDomainLen > MAX_DOMAIN_NAME_LEN ? MAX_DOMAIN_NAME_LEN : pstDT_ZONE_DATA->byDomainLen;
 //                       memcpy(pstDT_ZONE_DATA->abyDomainName, pRecordSet->GetFieldValueByName("DomainName"), pstDT_ZONE_DATA->byDomainLen);

 //                       pstDT_ZONE_DATA->wZonePort = SDAtou(pRecordSet->GetFieldValueByName("Port"));
 //                       SDStrncpy(pstDT_ZONE_DATA->aszZoneName, pRecordSet->GetFieldValueByName("ZoneName"), USERNAME_LEN - 1);
 //                       pstDT_ZONE_DATA->byBusyFlag = SDAtou(pRecordSet->GetFieldValueByName("BusyFlag"));
 //                   }
 //                   wLastZoneID = pstDT_ZONE_DATA->wID;
 //                   if(pstDT_ZONE_DATA->byPlayerNum >= MAX_PLAYER_NUM_PER_ZONE)
 //                   {
 //                       continue;
 //                   }
 //                   DT_ZONE_PLAYER_DATA* pstDT_ZONE_PLAYER_DATA = &(pstDT_ZONE_DATA->astPlayerInfo[pstDT_ZONE_DATA->byPlayerNum]);
 //                   const TCHAR* pszTmp = pRecordSet->GetFieldValueByName("PlayerID");
 //                   if(pszTmp)
 //                   {
 //                       pstDT_ZONE_PLAYER_DATA->dwPlayerID = SDAtou(pszTmp);
 //                       pstDT_ZONE_DATA->byPlayerNum++;
 //                   }
 //                   pszTmp = pRecordSet->GetFieldValueByName("CareerID");
 //                   if(pszTmp)
 //                   {
 //                       pstDT_ZONE_PLAYER_DATA->byPlayerCareer = SDAtou(pszTmp);
 //                   }
 //                   pszTmp = pRecordSet->GetFieldValueByName("PlayerLevel");
 //                   if(pszTmp)
 //                   {
 //                       pstDT_ZONE_PLAYER_DATA->wPlayerLevel = SDAtou(pszTmp);
 //                   }
 //                   pszTmp = pRecordSet->GetFieldValueByName("PlayerDspName");
 //                   if(pszTmp)
 //                   {
 //                       SDStrncpy(pstDT_ZONE_PLAYER_DATA->aszPlayerDspName, pszTmp, USERNAME_LEN - 1);
 //                   }
 //               }
 //           }
 //       }
 //   }
 //   else
 //   {
 //       m_stAck.wErrCode = ERR_SELECT_ZONE::ID_ZONE_NOT_EXIST_ERR;
 //   }

 //   UINT16 wSize = vecRs.size();
 //   for(UINT16 wIdx = 0; wIdx < wSize; wIdx++)
 //   {
 //       if(NULL != pRecordSet)
 //       {
 //           vecRs[wIdx]->Release();
 //       }
 //   }

 //   //必须要跟数据库的列对齐，如果数据库结构修改，代码也要跟随修改
 //   CSDDateTime dt(SDTimeSecs());

	//char v_szData[1024] = {0};
	//sprintf(v_szData, "(%u, %u, '%s', '%s', '%s', %u)", dwUserID, 0, pstReq->aszDeviceID, m_szParm, dt.ToString().c_str(), 0);
 //   ((CZSApplication*)SDGetApp())->AddData(v_szData);

    return TRUE;
}

