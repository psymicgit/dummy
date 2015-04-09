
#ifndef _DB_MANAGER_H_
#define _DB_MANAGER_H_

#pragma once

#include <sddb.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <sdobjectpool.h>
#include <protocol/server/protoclizs.h>
#include <vector>
#include <map>

using namespace std;
using namespace SGDP;

//DB命令类型
enum EDBCmdType
{
    EDBCT_SELECT_ZONE = 1,//选区
    EDBCT_GET_ZONE_INFO = 4,//获取区信息
    EDBCT_GET_VERSION_NOTICE = 5,//获取版本及公告信息
    EDBCT_SYNC_ZONENOTICE = 6,//同步区及公告信息
    EDBCT_GET_USERINFO = 7,//获取用户信息
    EDBCT_REGISTER_USERNAME = 8,//注册用户信息
    EDBCT_USER_DEVICE_INFO = 9,//设备信息
    EDBCT_UNBIND_DEVICE = 10,//设备信息
    EDBCT_GET_VERSION_NOTICE2 = 11,//获取版本及公告信息
    EDBCT_CHANGE_PWD	=	12,//更改密码
    EDBCT_FORBID_LOGIN =	13, //禁止登录
    EDBCT_FORBID_REGISTERED	=	14,//禁止注册
    EDBCT_RESETPASSWDMOBILE	=	15,//重置密码验证手机号
    EDBCT_RESETPASSWDNEWPASSWD	=	16,//重置密码 设置新密码
    EDBCT_SETACCOUNTTYPE		=	17,//设置账号类型
    EDBCT_STARTSERVICE		=	18,//设置开服时间
};


class CBaseDBCMD;
class CSelectZoneCMD;
class CGetZoneInfoCMD;
class CGetVersionNoticeCMD;
class CGetVersionNoticeCMD2;
class CSyncZoneNoticeCMD;
class CGetUserInfoCMD;
class CRegisterCMD;
class CUserDeviceInfoCMD;
class CUnBindDeviceCMD;
class CChangePwdCMD;
class CForbidLoginCMD;
class CForbidRegisteredCMD;
class CResetPasswdMobileCMD;
class CResetPasswdNewPassCMD;
class CSetAccountTypeCMD;
class CStartServerCMD;


typedef struct _tagDT_ZONE_DATA_INFO
{
    UINT8		byVisible;
	UINT16		wMinVersionID;
	UINT16		wMaxVersionID;
    DT_ZONE_DATA stDT_ZONE_DATA;
    _tagDT_ZONE_DATA_INFO()
    {
        Init();
    }
    void Init()
    {
        memset(this, 0x00, sizeof(_tagDT_ZONE_DATA_INFO));
    }
} DT_ZONE_DATA_INFO;

typedef vector<DT_ZONE_DATA_INFO> CZoneInfoVec;
typedef CZoneInfoVec::iterator CZoneInfoVecItr;

struct SVersionNoticeInfo
{
    UINT32				dwMinUnupdateVersionID;
    DT_VERSION_DATA		stVersionInfo; //版本信息
    DT_NOTICE_DATA		stNoticeInfo; //公告信息
};

struct SVersionNoticeInfo2
{
    UINT32				dwMinUnupdateVersionID;
    DT_VERSION_DATA2		stVersionInfo; //版本信息
    DT_NOTICE_DATA		stNoticeInfo; //公告信息
};

//客户端版本资源配置
struct SCliVerInfoProp
{
    UINT32 dwCliVer;
    UINT32 dwResVer;
    UINT32 dwUIVer;
};

typedef map<UINT32, SCliVerInfoProp> CVerID2CliVerInfoMap;
typedef CVerID2CliVerInfoMap::iterator CVerID2CliVerInfoMapItr;

class CDBMgr
{
public:
    CDBMgr();
    ~CDBMgr();

    BOOL Init();
    VOID Release();
    BOOL Run();
    BOOL AddCommand(CBaseDBCMD* poDBCommand);
    UINT32 GetDBCommandCount();
    SGDP::ISDDBSession* GetDBSession();
    /////////////////
    CBaseDBCMD*	CreateCmd(EDBCmdType eDbCmdType);
    VOID		FreeCmd(CBaseDBCMD* poBaseDBCMD);

    /////////////////
    VOID SyncZoneInfo(CZoneInfoVec& vecZoneInfo);
    VOID SyncVersionNotice(SVersionNoticeInfo& stVersionNoticeInfo);
    VOID SyncVersionNotice2(SVersionNoticeInfo2& stVersionNoticeInfo2);
    VOID SyncResVer(CVerID2CliVerInfoMap& mapVerID2CliVerInfo);

    CZoneInfoVec GetZoneInfo()
    {
        return m_vecZoneInfo;
    }

    SVersionNoticeInfo GetVersionNotice()
    {
        return m_stVersionNoticeInfo;
    }
    SVersionNoticeInfo2 GetVersionNotice2()
    {
        return m_stVersionNoticeInfo2;
    }
    VOID GetCliVerInfo(UINT32 dwCliVer, UINT32& dwResVer, UINT32& dwUIVer);

private:
    CDBMgr(const CDBMgr& rhs);
    CDBMgr& operator = (const CDBMgr& rhs);

private:
    SGDP::ISDDBModule*  m_pDBModule;
    SGDP::ISDDBSession* m_pDBSession;
    CSDObjectPool<CSelectZoneCMD, CSDMutex>			m_oSelectZoneCMDPool;
    CSDObjectPool<CGetZoneInfoCMD, CSDMutex>			m_oGetZoneInfoCMDPool;
    CSDObjectPool<CGetVersionNoticeCMD, CSDMutex>	m_oGetVersionNoticeCMDPool;
    CSDObjectPool<CGetVersionNoticeCMD2, CSDMutex>	m_oGetVersionNoticeCMD2Pool;
    CSDObjectPool<CSyncZoneNoticeCMD, CSDMutex>		m_oSyncZoneNoticeCMDPool;
    CSDObjectPool<CGetUserInfoCMD, CSDMutex>			m_oGetUserInfoCMDPool;
    CSDObjectPool<CRegisterCMD, CSDMutex>			m_oRegisterCMDPool;
    CSDObjectPool<CUserDeviceInfoCMD, CSDMutex>		m_oUserDeviceInfoCMDPool;
    CSDObjectPool<CUnBindDeviceCMD, CSDMutex>		m_oUnBindDeviceCMDPool;
    CSDObjectPool<CChangePwdCMD, CSDMutex>			m_oChangePwdCMDPool;
    CSDObjectPool<CForbidLoginCMD, CSDMutex>			m_oForbidLoginCMDPool;
    CSDObjectPool<CForbidRegisteredCMD, CSDMutex>	m_oForbidRegisteredCMDPool;
    CSDObjectPool<CResetPasswdMobileCMD, CSDMutex>	m_oResetPasswdMobileCMDPool;
    CSDObjectPool<CResetPasswdNewPassCMD, CSDMutex>	m_oResetPasswdNewPassCMDPool;
    CSDObjectPool<CSetAccountTypeCMD, CSDMutex>		m_oSetAccountTypeCMDPool;
	CSDObjectPool<CStartServerCMD, CSDMutex>			m_oStartServerCMDPool;

    CZoneInfoVec									m_vecZoneInfo;
    SVersionNoticeInfo							m_stVersionNoticeInfo;
    SVersionNoticeInfo2							m_stVersionNoticeInfo2;
    CVerID2CliVerInfoMap							m_mapVerID2CliVerInfo;
};

#endif

