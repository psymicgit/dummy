
#include "dbmgr.h"
#include <framework/zsapplication.h>
#include <logic/getversionnoticecmd.h>
#include <logic/getversionnoticecmd2.h>
#include <logic/getzoneinfocmd.h>
#include <logic/selectzone.h>
#include <logic/synczonenotice.h>
#include <logic/getuserinfo.h>
#include <logic/register.h>
#include <logic/userdeviceinfo.h>
#include <logic/unbinddevice.h>
#include <logic/changepwdcmd.h>
#include <logic/forbidlogin.h>
#include <logic/forbidregistered.h>
#include <logic/resetpasswdmobile.h>
#include <logic/resetpasswdnewpasswd.h>
#include <logic/setaccounttype.h>
#include <logic/startserver.h>

extern CSDApplication* SDGetApp();

CDBMgr::CDBMgr() :
    m_pDBModule(NULL),
    m_pDBSession(NULL)
{

}

CDBMgr::~CDBMgr()
{
}

BOOL CDBMgr::Init()
{
    SGDP::SDDBAccount account;
    if(!((CZSApplication*)SDGetApp())->GetZSConfig()->GetDBAccount(account))
    {
        SYS_CRITICAL(_SDT("[%s: %d]:GetDBAccount failed"), MSG_MARK);
        return FALSE;
    }
    m_pDBModule = SGDP::SDDBGetModule(&(SGDP::SGDP_VERSION));
    if(NULL == m_pDBModule)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:SDDBGetModule failed"), MSG_MARK);
        return FALSE;
    }
    m_pDBSession = m_pDBModule->GetDBSession(&account,
                   ((CZSApplication*)SDGetApp())->GetZSConfig()->GetPoolCoreSize(),
                   ((CZSApplication*)SDGetApp())->GetZSConfig()->GetPoolMaxSize());
    if(NULL == m_pDBSession)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:GetDBSession failed"), MSG_MARK);
        return FALSE;
    }
    return TRUE;
}

VOID CDBMgr::Release()
{
    if(NULL != m_pDBModule)
    {
        if(NULL != m_pDBSession)
        {
            m_pDBModule->Close(m_pDBSession);
            m_pDBSession = NULL;
        }
        m_pDBModule->Release();
        m_pDBModule = NULL;
    }
}

SGDP::ISDDBSession* CDBMgr::GetDBSession()
{
    return m_pDBSession;
}

BOOL CDBMgr::Run()
{
    UINT32 dwCount = 100;
    BOOL bRet = m_pDBSession->Run(dwCount);
    return bRet;
}

BOOL CDBMgr::AddCommand(CBaseDBCMD* poDBCommand)
{
    return m_pDBSession->AddDBCommand(poDBCommand);
}


CBaseDBCMD*	CDBMgr::CreateCmd(EDBCmdType eDbCmdType)
{
    CBaseDBCMD* poBaseDBCMD = NULL;
    switch(eDbCmdType)
    {
    case EDBCT_SELECT_ZONE:
        poBaseDBCMD = m_oSelectZoneCMDPool.Alloc();
        break;
    case EDBCT_GET_ZONE_INFO:
        poBaseDBCMD = m_oGetZoneInfoCMDPool.Alloc();
        break;
    case EDBCT_GET_VERSION_NOTICE:
        poBaseDBCMD = m_oGetVersionNoticeCMDPool.Alloc();
        break;
    case EDBCT_GET_VERSION_NOTICE2:
        poBaseDBCMD = m_oGetVersionNoticeCMD2Pool.Alloc();
        break;
    case EDBCT_SYNC_ZONENOTICE:
        poBaseDBCMD = m_oSyncZoneNoticeCMDPool.Alloc();
        break;
    case EDBCT_GET_USERINFO:
        poBaseDBCMD = m_oGetUserInfoCMDPool.Alloc();
        break;
    case EDBCT_REGISTER_USERNAME:
        poBaseDBCMD = m_oRegisterCMDPool.Alloc();
        break;
    case EDBCT_USER_DEVICE_INFO:
        poBaseDBCMD = m_oUserDeviceInfoCMDPool.Alloc();
        break;
    case EDBCT_UNBIND_DEVICE:
        poBaseDBCMD = m_oUnBindDeviceCMDPool.Alloc();
        break;
    case EDBCT_CHANGE_PWD:
        poBaseDBCMD =  m_oChangePwdCMDPool.Alloc();
        break;
    case EDBCT_FORBID_LOGIN:
        poBaseDBCMD =  m_oForbidLoginCMDPool.Alloc();
        break;
	case EDBCT_FORBID_REGISTERED:
		poBaseDBCMD =  m_oForbidRegisteredCMDPool.Alloc();
		break;
	case EDBCT_RESETPASSWDMOBILE:
		poBaseDBCMD = m_oResetPasswdMobileCMDPool.Alloc();
		break;
	case EDBCT_RESETPASSWDNEWPASSWD:
		poBaseDBCMD =  m_oResetPasswdNewPassCMDPool.Alloc();
		break;
	case EDBCT_SETACCOUNTTYPE:
		poBaseDBCMD =  m_oSetAccountTypeCMDPool.Alloc();
		break;
	case EDBCT_STARTSERVICE:
		poBaseDBCMD =  m_oStartServerCMDPool.Alloc();
		break;
    default:
        break;
    }

	SYS_CRITICAL(_SDT("[%s: %d]:CreateCmd %0x Type:%u"), MSG_MARK, poBaseDBCMD, eDbCmdType);

    return poBaseDBCMD;
}

VOID CDBMgr::FreeCmd(CBaseDBCMD* poBaseDBCMD)
{
    if(NULL == poBaseDBCMD)
    {
        return;
    }

	SYS_CRITICAL(_SDT("[%s: %d]:FreeCmd %0x Type:%u"), MSG_MARK, poBaseDBCMD, poBaseDBCMD->GetDBCmdType());

    switch(poBaseDBCMD->GetDBCmdType())
    {
    case EDBCT_SELECT_ZONE:
        m_oSelectZoneCMDPool.Free((CSelectZoneCMD*)poBaseDBCMD);
        break;
    case EDBCT_GET_ZONE_INFO:
        m_oGetZoneInfoCMDPool.Free((CGetZoneInfoCMD*)poBaseDBCMD);
        break;
    case EDBCT_GET_VERSION_NOTICE:
        m_oGetVersionNoticeCMDPool.Free((CGetVersionNoticeCMD*)poBaseDBCMD);
        break;
    case EDBCT_GET_VERSION_NOTICE2:
        m_oGetVersionNoticeCMD2Pool.Free((CGetVersionNoticeCMD2*)poBaseDBCMD);
        break;
    case EDBCT_SYNC_ZONENOTICE:
        m_oSyncZoneNoticeCMDPool.Free((CSyncZoneNoticeCMD*)poBaseDBCMD);
        break;
    case EDBCT_GET_USERINFO:
        m_oGetUserInfoCMDPool.Free((CGetUserInfoCMD*)poBaseDBCMD);
        break;
    case EDBCT_REGISTER_USERNAME:
        m_oRegisterCMDPool.Free((CRegisterCMD*)poBaseDBCMD);
        break;
    case EDBCT_USER_DEVICE_INFO:
        m_oUserDeviceInfoCMDPool.Free((CUserDeviceInfoCMD*)poBaseDBCMD);
        break;
    case EDBCT_UNBIND_DEVICE:
        m_oUnBindDeviceCMDPool.Free((CUnBindDeviceCMD*)poBaseDBCMD);
        break;
    case EDBCT_CHANGE_PWD:
        m_oChangePwdCMDPool.Free((CChangePwdCMD*)poBaseDBCMD);
        break;
    case EDBCT_FORBID_LOGIN:
        m_oForbidLoginCMDPool.Free((CForbidLoginCMD*)poBaseDBCMD);
        break;
	case EDBCT_FORBID_REGISTERED:
		m_oForbidRegisteredCMDPool.Free((CForbidRegisteredCMD*)poBaseDBCMD);
		break;
	case EDBCT_RESETPASSWDMOBILE:
		m_oResetPasswdMobileCMDPool.Free((CResetPasswdMobileCMD*)poBaseDBCMD);
		break;
	case EDBCT_RESETPASSWDNEWPASSWD:
		m_oResetPasswdNewPassCMDPool.Free((CResetPasswdNewPassCMD*)poBaseDBCMD);
		break;
	case EDBCT_SETACCOUNTTYPE:
		m_oSetAccountTypeCMDPool.Free((CSetAccountTypeCMD*)poBaseDBCMD);
		break;
	case EDBCT_STARTSERVICE:
		m_oStartServerCMDPool.Free((CStartServerCMD*)poBaseDBCMD);
		break;
    default:
        break;
    }
}

VOID CDBMgr::SyncZoneInfo(CZoneInfoVec& vecZoneInfo)
{
    if(0 == vecZoneInfo.size())
    {
        return;
    }

    m_vecZoneInfo.clear();
    UINT16 wSize = vecZoneInfo.size();
    for(UINT16 wIdx = 0; wIdx < wSize; wIdx++)
    {
        m_vecZoneInfo.push_back(vecZoneInfo[wIdx]);
    }
}

VOID CDBMgr::SyncVersionNotice(SVersionNoticeInfo& stVersionNoticeInfo)
{
    memcpy(&m_stVersionNoticeInfo, &stVersionNoticeInfo, sizeof(stVersionNoticeInfo));
}

VOID CDBMgr::SyncVersionNotice2(SVersionNoticeInfo2& stVersionNoticeInfo2)
{
    memcpy(&m_stVersionNoticeInfo2, &stVersionNoticeInfo2, sizeof(m_stVersionNoticeInfo2));
}

VOID CDBMgr::SyncResVer(CVerID2CliVerInfoMap& mapVerID2CliVerInfo)
{
    m_mapVerID2CliVerInfo = mapVerID2CliVerInfo;
}

VOID CDBMgr::GetCliVerInfo(UINT32 dwCliVer, UINT32& dwResVer, UINT32& dwUIVer)
{
    CVerID2CliVerInfoMapItr itr = m_mapVerID2CliVerInfo.find(dwCliVer);
    if(itr != m_mapVerID2CliVerInfo.end())
    {
        dwResVer = itr->second.dwResVer;
        dwUIVer = itr->second.dwUIVer;
    }
    else
    {
        dwResVer = 0;
        dwUIVer = 0;
    }
}
