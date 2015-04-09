

//  Defines the entry point for the console application.
//  时间：2010-12-28。
//

#ifndef _SGDP_FRAMEWORK_LOGGER_H_
#define _SGDP_FRAMEWORK_LOGGER_H_

#include "sdlogger.h"
#include "sdstring.h"

#include <sdmutex.h>
#include <sdobjectpool.h>

#ifndef MAX_PATH
#define MAX_PATH   260
#endif
namespace SGDP {

    enum EExeLogLvl
    {
        LOGLVL_SYS_CRITICAL = 0,  ///< 系统级致命错误日志
        LOGLVL_SYS_IMPORTANT,     ///< 系统级重要日志
        LOGLVL_SYS_INFO,          ///< 系统级信息日志
        LOGLVL_USR_CRITICAL,      ///< 用户级致命错误日志
        LOGLVL_USR_IMPORTANT,     ///< 用户级重要日志
        LOGLVL_USR_INFO,          ///< 用户级信息日志
        LOGLVL_DBG_INFO,          ///< 用户级调试日志
        LOGLVL_MAX,               ///< 最大允许的日志级别
    };

	//异常错误日志，用于记录异常情况
    #define SYS_CRITICAL    if(CSDLoggerImpl::NeedLog(LOGLVL_SYS_CRITICAL))CSDLoggerImpl::SysCritical   ///< 系统级致命错误日志
    //#define SYS_IMPORTANT   if(CSDLoggerImpl::NeedLog(LOGLVL_SYS_IMPORTANT))CSDLoggerImpl::SysImportant ///< 系统级重要日志
    //#define SYS_INFO        if(CSDLoggerImpl::NeedLog(LOGLVL_SYS_INFO))CSDLoggerImpl::SysInfo           ///< 系统级信息日志
    //#define USR_CRITICAL    if(CSDLoggerImpl::NeedLog(LOGLVL_USR_CRITICAL))CSDLoggerImpl::UsrCritical   ///< 用户级致命错误日志
    //#define USR_IMPORTANT   if(CSDLoggerImpl::NeedLog(LOGLVL_USR_IMPORTANT))CSDLoggerImpl::UsrImportant ///< 用户级重要日志
	//用户日志，用于追踪核实用户操作信息
    #define USR_INFO        if(CSDLoggerImpl::NeedLog(LOGLVL_USR_INFO))CSDLoggerImpl::UsrInfo           ///< 用户级信息日志
	//调试日志，用于定位bug
    #define DBG_INFO        if(CSDLoggerImpl::NeedLog(LOGLVL_DBG_INFO))CSDLoggerImpl::DbgInfo           ///< 调试信息日志

	//#define	PRINT_ERR_INFO		if(CSDLoggerImpl::NeedLog(LOGLVL_SYS_CRITICAL))CSDLoggerImpl::SysCritical(_SDT("[%s: %d]: "), MSG_MARK);

	#define MAX_LOG_DATA_LEN 8192

	typedef struct _tagLOGDATA
	{
		_tagLOGDATA()
		{
			Init();
		}
		void Init()
		{
			memset(this, 0x00, sizeof(_tagLOGDATA));
		}
		TCHAR	szData[MAX_LOG_DATA_LEN];
	}CLOGDATA, *pLOGDATA;

    /**
    * @brief CSDLoggerImpl类
    * 底层框架日志实现处理类，统一提供日志api。主要提供以下几大功能：
    * 1、文件日志。
    * 2、udp日志备份
    * 3、tcp日志备份
    * 4、可以自由开启管理日志级别
    * 5、为日志信息加上模块类别前缀，标识日志信息的分类
    */
    class CSDLoggerImpl : public ISDLogger
    {
        /**
	    * @brief 构造函数
	    */
        CSDLoggerImpl();

        /**
	    * @brief 析构函数
	    */
        virtual ~CSDLoggerImpl();
    public:
        /**
	    * @brief 静态函数，初始化应用程序日志配置相关信息
	    * @param pszConfig : 主配置文档路径，utf8格式的xml文件
	    * @return BOOL型数据，TRUE代表初始化成功；FALSE代表初始化失败
	    */
        static BOOL Init(const TCHAR *pszConfig);

        /**
	    * @brief 静态函数，初始化应用程序日志配置相关信息
	    * @return BOOL型数据，TRUE代表初始化成功；FALSE代表初始化失败
	    */
        static BOOL Reload();

        /**
	    * @brief 释放底层框架向系统申请的资源
	    */
        static VOID UnInit();

        /**
	    * @brief 静态函数，判断指定日志级别是否需要处理
	    * @param nLevel : 日志级别
	    * @return BOOL型数据，TRUE代表需要处理；FALSE代表不需要处理
	    */
        static BOOL NeedLog(EExeLogLvl nLevel);

        /**
	    * @brief 静态函数，记录系统级致命错误日志
	    * @param format : 日志信息格式
	    */
        static VOID SysCritical(const TCHAR *format, ...);

        /**
	    * @brief 静态函数，记录系统级重要日志
	    * @param format : 日志信息格式
	    */
        static VOID SysImportant(const TCHAR *format, ...);

        /**
	    * @brief 静态函数，记录系统级信息日志
	    * @param format : 日志信息格式
	    */
        static VOID SysInfo(const TCHAR *format, ...);
        
        /**
	    * @brief 静态函数，记录用户级致命错误日志
	    * @param format : 日志信息格式
	    */
        static VOID UsrCritical(const TCHAR *format, ...);

        /**
	    * @brief 静态函数，记录用户级重要日志
	    * @param format : 日志信息格式
	    */
        static VOID UsrImportant(const TCHAR *format, ...);

        /**
	    * @brief 静态函数，记录用户级信息日志
	    * @param format : 日志信息格式
	    */
        static VOID UsrInfo(const TCHAR *format, ...);

        /**
	    * @brief 静态函数，记录调试信息日志
	    * @param format : 日志信息格式
	    */
        static VOID DbgInfo(const TCHAR *format, ...);

        /**
	    * @brief 未加分类头部标识信息的日志
	    * @param format : 日志信息格式
	    */
        static VOID NoHeadLog(const TCHAR *format, ...);

        /**
	    * @brief 开/关指定日志级别
	    * @param nIndex : 日志级别
        * @param bVal : TRUE代表开启指定的日志级别，FALSE代表关闭指定的日志级别
	    */
        static VOID SetLogLevel(INT32 nIndex, BOOL bVal);

        /**
	    * @brief 静态函数，判断指定的日志级别是否需要记录
	    * @param nLevel : 日志级别
	    * @return BOOL型数据，TRUE代表需要处理；FALSE代表不需要处理
	    */
        static BOOL GetLevelValue(INT32 nIndex);

        /**
	    * @brief 静态函数，判断是否需要进行文件日志操作
	    * @return BOOL型数据，TRUE代表需要处理；FALSE代表不需要处理
	    */
        static BOOL IsLogToFile() { return m_bLogToFile; }
        
        /**
	    * @brief 静态函数，判断是否需要进行Debug日志操作
	    * @return BOOL型数据，TRUE代表需要处理；FALSE代表不需要处理
	    */
        static BOOL IsLogToDebugger() { return m_bLogToDebugger; }
        
        /**
	    * @brief 静态函数，判断是否需要进行udp日志信息备份操作
	    * @return BOOL型数据，TRUE代表需要处理；FALSE代表不需要处理
	    */
        static BOOL IsLogToUdp() { return m_bLogToUdp; }
        
        /**
	    * @brief 静态函数，判断是否需要进行tcp日志信息备份操作
	    * @return BOOL型数据，TRUE代表需要处理；FALSE代表不需要处理
	    */
        static BOOL IsLogToTcp() { return m_bLogToTcp; }

        /**
	    * @brief 静态函数，设定是否需要进行文件日志信息操作
	    * @param bLog : 是否开启文件日志
	    */
        static VOID SetLogToFile(BOOL bLog) { m_bLogToFile = bLog; }

        /**
	    * @brief 静态函数，设定是否需要进行Debug日志信息操作
	    * @param bLog : 是否开启Debug日志
	    */
        static VOID SetLogToDebugger(BOOL bLog) { m_bLogToDebugger = bLog; }

        /**
	    * @brief 静态函数，设定是否需要进行Udp日志信息备份操作
	    * @param bLog : 是否开启Udp日志信息备份
	    */
        static VOID SetLogToUdp(BOOL bLog) { m_bLogToUdp = bLog; }
        
        /**
	    * @brief 静态函数，设定是否需要进行tcp日志信息备份操作
	    * @param bLog : 是否开启tcp日志信息备份
	    */
        static VOID SetLogToTcp(BOOL bLog) { m_bLogToTcp = bLog; }

        /**
	    * @brief 以字符串形式记录日志
	    * @param pszLog : 需要记录的日志信息
        * @return bool型数据，TRUE代表需要处理；FALSE代表不需要处理
	    */
        virtual bool SDAPI LogText(const TCHAR* pszLog);

        /**
	    * @brief 以二进制形式记录日志
	    * @param pLog : 需要记录的日志信息内存指针
	    * @param dwLen : 需要记录的日志信息内存长度区间
        * @return bool型数据，TRUE代表需要处理；FALSE代表不需要处理
	    */
        virtual bool SDAPI LogBinary(const UINT8* pLog, UINT32 dwLen);

    protected:

        /**
	    * @brief 静态函数，类的内部处理函数，不提供外部使用，
        *                  根据用户提供的格式信息、日志级别，记录日志
	    * @param nLevel : 指定的日志级别
        * @param format : 日志格式信息
        * @param argptr : 形参列表
	    */
        static VOID _Log(EExeLogLvl nLevel, const TCHAR *format, va_list argptr);
    protected:

        static BOOL     m_abLogLevel[LOGLVL_MAX];   ///< 对应日志级别是否开启标识数组
        static TCHAR *  m_aszLogPrefix[LOGLVL_MAX]; ///< 对应日志级别前缀标识数组
        static CHAR     m_szConfig[MAX_PATH];       ///< 对应日志信息配置管理文档路径
        static TCHAR    m_szSysLogFileName[MAX_PATH];  ///< 文件日志的文件名
		static TCHAR    m_szUsrLogFileName[MAX_PATH];  ///< 文件日志的文件名
		static TCHAR    m_szDbgLogFileName[MAX_PATH];  ///< 文件日志的文件名

        static ISDFileLogger   *	m_pSYSFileLogger;      ///< 文件日志句柄指针
		static ISDFileLogger   *	m_pUsrFileLogger;      ///< 文件日志句柄指针
		static ISDFileLogger   *	m_pDbgFileLogger;      ///< 文件日志句柄指针
        static BOOL				m_bLogToFile;       ///< 是否开启文件日志标识
        static BOOL				m_bLogToDebugger;    ///< 是否开启debug日志标识

        static ISDUdpLogger   *m_pUdpLogger;        ///< Udp日志备份句柄指针
        static BOOL			m_bLogToUdp;                ///< 是否开启Udp日志备份标识

        static ISDTcpLogger   *m_pTcpLogger;        ///< tcp日志备份句柄指针
        static BOOL			m_bLogToTcp;          ///< 是否开启tcp日志备份标识

		static CSDObjectPool<CLOGDATA, CSDMutex> m_oLogDataPool; //日志信息结构体
    };

    /**
    * @brief CSDComLoggerImpl类
    * 底层框架日志实现处理类，统一提供日志api。
    * 为日志信息加上前缀，标识日志信息的分类
    */
    class CSDComLoggerImpl : public ISDLogger
    {
    public:
        /**
	    * @brief 构造函数
	    */
        CSDComLoggerImpl();
        ~CSDComLoggerImpl();
        /**
	    * @brief 静态函数，指定日志模块类别的前缀，比如：[sdnet]、[sdpipe]
	    * @param pszConfig : 主配置文档路径，utf8格式的xml文件
	    * @return BOOL型数据，TRUE代表初始化成功；FALSE代表初始化失败
	    */
        VOID Init(const TCHAR* pszPrefix);

        /**
	    * @brief 以字符串形式记录日志
	    * @param pszLog : 需要记录的日志信息
        * @return bool型数据，TRUE代表需要处理；FALSE代表不需要处理
	    */
        virtual bool SDAPI LogText(const TCHAR *pszLog);
      
        /**
	    * @brief 以二进制形式记录日志
	    * @param pLog : 需要记录的日志信息内存指针
	    * @param dwLen : 需要记录的日志信息内存长度区间
        * @return bool型数据，TRUE代表需要处理；FALSE代表不需要处理
	    */
        virtual bool SDAPI LogBinary(const UINT8 *pLog, UINT32 dwLen);

    protected:
        TCHAR	m_szPrefix[16];      ///< 指定日志前缀的内容
    };

}

#endif

