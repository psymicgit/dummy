///<------------------------------------------------------------------------------
//< @file:   pkconfig.h
//< @author: hongkunan
//< @date:   2014年1月2日 16:8:55
//< @brief:  跨服战服务器的配置文件解析器
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _pkconfig_h_
#define _pkconfig_h_

#include <sdutil.h>
#include <sdframework.h>
#include <sdsingleton.h>
#include <sdxmlfile.h>
#include <sddb.h>

using namespace std;
using namespace SGDP;

// 跨服战服务器的配置文件
class CPKConfig
{
public:
    DECLARE_SINGLETON(CPKConfig);

public:
    BOOL Init(const TCHAR* pszConfig);
    VOID UnInit();


public:

    /**
    * @brief 获取数据库连接信息
    * @param pszHost : 数据库服务器IP地址
    * @param wPort : 数据库服务器端口
    * @param pszLoginName : 数据库帐号
    * @param pszPassword : 数据库密码
    * @param pszDBName : 数据库名称
    * @param pszCharacterSet : 字符集
    * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL GetDBConfig(CHAR* pszHost, UINT16 &wPort, CHAR* pszLoginName, CHAR* pszPassword, CHAR* pszDBName, CHAR* pszCharacterSet, CHAR* pszUnixSocket);

    /**
     * @brief 获取配置数据库文件地址
     * @param pszPath : 配置数据库文件地址缓冲区
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL    GetConfigDataPath(CHAR* pszPath);

    tstring  GetConfigFile();

    BOOL	GetCfgDBAccount( SGDP::SDDBAccount& DBAccount );
    UINT32	GetCfgPoolCoreSize(){ return 1; }
    UINT32	GetCfgPoolMaxSize(){ return 1; }

    string GetSpeCfgTablePrefix()
    {
        return m_strSpeCfgTablePrefix;
    }

    UINT64 &GetServerOpentTime()
    {
        return m_qwServerOpentTime;
    }

protected:
    CPKConfig();
    ~CPKConfig();

    BOOL _LoadCfg(CSDXMLFile* poConfig);
    BOOL _LoadGateListenerInfo(CSDXMLFile* poSDXMLFile);

private:
    BOOL CopyDBAccountInfo( SGDP::SDDBAccount &DBAccountDst, const SGDP::SDDBAccount &DBAccountSrc );
    BOOL LoadDBConnInfo( CSDXMLNode &oNode, SGDP::SDDBAccount &oDBAccount );

private:
    CHAR m_szDBConfigPath[MAX_PATH];
    CHAR m_szConfigFile[MAX_PATH];

    //app充值的数据库相关信息
    SGDP::SDDBAccount  m_oPayDBAccount; //采集数据库的账号信息
    UINT32	m_dwPayPoolCoreSize;
    UINT32  m_dwPayPoolMaxSize;

    SGDP::SDDBAccount  m_oCfgDBAccount; //配置数据库的账号信息

    string	m_strSpeCfgTablePrefix;//特指配置表前缀
    UINT64	m_qwServerOpentTime;//开服时间
};

#endif //_pkconfig_h_