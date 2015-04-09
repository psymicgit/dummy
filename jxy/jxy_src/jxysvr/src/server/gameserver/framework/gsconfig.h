


#ifndef _GAME_SERVER_SETTING_H_
#define _GAME_SERVER_SETTING_H_

#include <sdutil.h>
#include <sdframework.h>
#include <sdsingleton.h>
#include <sdxmlfile.h>
#include <sddb.h>
using namespace std;
using namespace SGDP;

#define PAY_TYPE_TONGBU	0	//充值为同步
#define PAY_TYPE_APPSTORE 1	//充值为appstore

#define IOS_AND_BOUND 100
#define IOS_DEFAULT_NOTICE 3
#define AND_DEFAULT_NOTICE 101

#define IOS_AND_ANDROID  0
#define SINGLE_IOS 1
#define SINGLE_ANDROID 2
#define BEGIN_MERGE_ZONE 146
#define BEGIN_AND_PLAYERID 100000000

struct SGateListenerInfo
{
    string					m_strIP;
    UINT16					m_wPort;
    UINT32					m_dwRecvBuf;
    UINT32					m_dwSendBuf;
};

class CGSConfig
{
public:
    DECLARE_SINGLETON(CGSConfig);

public:
    BOOL    Init(const TCHAR* pszConfig);
    VOID    UnInit();

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
    BOOL    GetDBConfig(CHAR* pszHost, UINT16 &wPort, CHAR* pszLoginName, CHAR* pszPassword, CHAR* pszDBName, CHAR* pszCharacterSet, CHAR* pszUnixSocket);

    /**
     * @brief 获取配置数据库文件地址
     * @param pszPath : 配置数据库文件地址缓冲区
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL    GetConfigDataPath(CHAR* pszPath);

    tstring  GetConfigFile();

    // 是否是跨服战服务器，否的话则为游戏服务器
    BOOL GetIsPKServer(){ return m_bPKServer; }

    UINT32	GetOnlineSaveTimeInterval()
    {
        return m_dwOnlineSaveTimeInterval;
    }
    UINT32	GetOfflineSaveCkInterval()
    {
        return m_dwOfflineSaveCkInterval;
    }
    UINT32	GetOfflineCacheTime()
    {
        return m_dwOfflineCacheTime;
    }
    UINT32	GetPvpSaveCkInterval()
    {
        return m_dwPvpSaveCkInterval;
    }
    UINT32	GetPvpSaveNum()
    {
        return m_dwPvpSaveNum;
    }

    BOOL	GetDBAccount( SGDP::SDDBAccount& DBAccount );
    UINT32	GetPoolCoreSize();
    UINT32	GetPoolMaxSize();

    BOOL	GetRecordDBAccount( SGDP::SDDBAccount& DBAccount );
    UINT32	GetRecordPoolCoreSize();
    UINT32	GetRecordPoolMaxSize();

    BOOL	GetPayDBAccount( SGDP::SDDBAccount& DBAccount );
    UINT32	GetPayPoolCoreSize();
    UINT32	GetPayPoolMaxSize();

    BOOL	GetCfgDBAccount( SGDP::SDDBAccount& DBAccount );
    UINT32	GetCfgPoolCoreSize()
    {
        return 1;
    }
    UINT32	GetCfgPoolMaxSize()
    {
        return 1;
    }

    UINT32 GetTimerNotice()
    {
        return m_dwTimerNotice;
    };
    UINT32 GetChatSyncTime()
    {
        return m_dwChatSyncTime;
    };
    UINT32 GetChatSendNotice()
    {
        return m_dwChatSendNotice;
    };
    UINT32 GetSavePlayerInterval()
    {
        return m_dwPlayerSaveInterval;
    };
    UINT32 GetFillPlayerQueueInterval()
    {
        return m_dwFillePlayerQueueInterval;
    };
    std::string GetRechargeIP()
    {
        return m_strRechargeIP;
    };
    UINT32 GetRechargePort()
    {
        return m_dwRechargePort;
    };
    std::string GetAppPayUrl()
    {
        return m_strAppPayUrl;
    };
    std::string GetAppPayTableName()
    {
        return m_strAppPayTableName;
    };
    INT32  GetPayType()
    {
        return m_nPayType;
    };

    SGateListenerInfo& GetGateListenerInfo()
    {
        return m_stRechargeListenerInfo;
    }

    string GetSpeCfgTablePrefix()
    {
        return m_strSpeCfgTablePrefix;
    }

    UINT64 &GetServerOpentTime()
    {
        return m_qwServerOpentTime;
    }

protected:
    CGSConfig();
    ~CGSConfig();

    BOOL    _LoadCfg(CSDXMLFile* poConfig);
    BOOL	_LoadGateListenerInfo(CSDXMLFile* poSDXMLFile);

private:
    BOOL	CopyDBAccountInfo( SGDP::SDDBAccount &DBAccountDst, const SGDP::SDDBAccount &DBAccountSrc );
    BOOL	LoadDBConnInfo( CSDXMLNode &oNode, SGDP::SDDBAccount &oDBAccount );
private:

    CHAR    m_szDBConfigPath[MAX_PATH];
    CHAR    m_szConfigFile[MAX_PATH];
    SGDP::SDDBAccount  m_oDBAccount;
    UINT32  m_dwPoolCoreSize;
    UINT32  m_dwPoolMaxSize;

    BOOL    m_bPKServer;       // 是否是跨服战服务器，否的话则为游戏服务器
    
    //采集数据库的相关信息

    SGDP::SDDBAccount  m_oRecordDBAccount; //采集数据库的账号信息
    UINT32	m_dwRecordPoolCoreSize;
    UINT32  m_dwRecordPoolMaxSize;

    //app充值的数据库相关信息
    SGDP::SDDBAccount  m_oPayDBAccount; //采集数据库的账号信息
    UINT32	m_dwPayPoolCoreSize;
    UINT32  m_dwPayPoolMaxSize;

    SGDP::SDDBAccount  m_oCfgDBAccount; //配置数据库的账号信息

    //定时器配置
    UINT32	m_dwOnlineSaveTimeInterval;//登录玩家db定时保存间隔,单位秒
    UINT32	m_dwOfflineSaveCkInterval;//非登录玩家保存/检测间隔,单位秒
    UINT32	m_dwOfflineCacheTime;//非登录玩家内存中保持时间,单位秒
    UINT32	m_dwPvpSaveCkInterval;//pvp信息保存间隔,单位秒
    UINT32	m_dwPvpSaveNum;//pvp每次保存个数

    UINT32	m_dwTimerNotice;
    UINT32	m_dwChatSyncTime;
    UINT32	m_dwChatSendNotice;

    UINT32  m_dwPlayerSaveInterval; //玩家保存的间隔以毫秒为单位
    UINT32  m_dwFillePlayerQueueInterval; //保存玩家数据的队列时间间隔，以毫秒为单位
    std::string  m_strRechargeIP; //充值IP
    UINT32  m_dwRechargePort; //充值接口
    std::string  m_strAppPayUrl; //苹果充值URL
    std::string  m_strAppPayTableName; //苹果充值数据库的表名
    INT32	m_nPayType; //充值类型

    SGateListenerInfo	m_stRechargeListenerInfo; //同步充值端口

    string	m_strSpeCfgTablePrefix;//特指配置表前缀
    UINT64	m_qwServerOpentTime;//开服时间
};

#endif // #ifndef _GAME_SERVER_SETTING_H_

