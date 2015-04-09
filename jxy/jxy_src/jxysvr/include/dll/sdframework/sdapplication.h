

//  Defines the entry point for the console application.
//  时间：2010-12-28。
//

#ifndef _SGDP_APPLICATION_H_
#define _SGDP_APPLICATION_H_

#include <list>
#include <vector>
#include <string>
#include <map>
#include <sdserverid.h>
#include <sdshmem.h>
#include "sdpipechannel.h"
#include "sdconfig.h"
#include "sdloggerimpl.h"
//#include "sdconsole.h"
#include "sdu.h"
#include "netdata.h"
#include "sdcommandline.h"


namespace SGDP {

    /**
    * @brief CSDApplication类
    * 底层框架类，管理应用程序入口。主要集中管理：基础配置文件读取
    *                                             日志管理
    *                                             服务器之间的网络控制管理入口
    *                                             命令行控制台
    */

    class CSDApplication
    {    
    public:


    public:
	    /**
	    * @brief 构造函数
	    */
        CSDApplication(void);

        /**
	    * @brief 析构函数
	    */
        ~CSDApplication(void);

	    /**
	    * @brief 底层框架数据、对象、参数初始化函数
	    * @param pszConfigFilename : 主配置文档路径，utf8格式的xml文件
	    * @return BOOL型数据，TRUE代表初始化成功；FALSE代表初始化失败
	    */
        virtual BOOL        Init(const TCHAR* pszConfigFilename);
        
        /**
	    * @brief 释放底层框架向系统申请的资源
	    */
        virtual VOID        UnInit();

		/**
	    * @brief 释放日志模块申请的资源
	    */
        virtual VOID        UnInitLog();

		/**
	    * @brief 释放控制台模块申请的资源
	    */
		virtual VOID        UnInitConsole();

        /**
	    * @brief 主线程运行主体函数	  
	    * @return BOOL型数据，TRUE代表当前运行过程执行了具体任务；
                              FALSE代表当前运行过程未执行具体任务
	    */
        virtual BOOL        Run();
       	
        /**
	    * @brief 设定控制台固定显示区域最大行数
	    * @param nCount : 指定的控制台固定区域显示行数	   
	    */
        VOID                SetFixedLineCount(INT32 nCount);

        /**
	    * @brief 在控制台滚动区域输出日志、运行态信息
	    * @param format : 输出数据的格式字符串	   
	    */
        VOID                PrintScreen(const TCHAR *format, ...);

        /**
	    * @brief 更新控制台指定行的信息数据
        * @param wLine :  指定需要更新的行数
	    * @param format : 输出数据的格式字符串	   
	    */
        VOID                UpdateScreen(const UINT16 wLine , const TCHAR *format, ...);
        
        /**
	    * @brief 获取网络层句柄指针
        * @return ISDNet指针，返回底层框架使用的网络层句柄指针
	    */
        ISDNet*             GetNetModule();

        /**
	    * @brief 获取应用框架底层服务器之间互连的管理器
        * @return CSDPipeChannelMgr指针，返回应用框架底层服务器之间互连的管理器句柄指针
	    */
        CSDPipeChannelMgr*  GetPipeChannelMgr();

        /**
	    * @brief 获取应用框架主配置文件管理器指针
        * @return CSDConfig指针，返回应用框架主配置文件管理器指针
	    */
        CSDConfig*          GetConfig();

        /**
	    * @brief 获取服务器之间互连信息引用
        * @return CPipeInfoVect对象，返回服务器之间互连信息引用
	    */
        //CPipeInfoVect*      GetPipeVect();
        
        /**
	    * @brief 获取命令行处理类指针
        * @return CSDCommandLine指针，返回命令行处理类指针
	    */
        CSDCommandLine*     GetCmdLine();

        /**
	    * @brief 中止应用程序的运行
	    */
        VOID                Terminate();

        /**
	    * @brief 查询应用程序是否已经被中止运行。
        * @return BOOL型数据，TRUE 代表应用程序已中止。FALSE 代表应用程序未中止。
	    */
        BOOL                IsTerminated();


		/**
	    * @brief 获得该Server的ServerID
        * @return ServerID
	    */
		UINT32				GetLocalID();

		/**
	    * @brief 获得该Server的ZoneID
        * @return ZoneID
	    */
		UINT16				GetZoneID();

        /**
	    * @brief 更新控制台服务器互连状态信息
        * @param dwID :  服务器的ServerID
	    * @param dwIP :  服务器的IP地址	   
	    * @param bConnect :  连接状态，TRUE代表连接成功，FALSE代表连接断开。	 
	    */
        virtual VOID		UpdateSvrStatus(UINT32 dwID,UINT32 dwIP, BOOL bConnect);
        
        /**
	    * @brief 程序启动时，将启动参数赋给基础框架的m_oSDCommandLine
        * @param argc :  参数数目
	    * @param argv :  参数字符串数组	    
	    */
        virtual VOID        OnSetCmdLine(INT32 argc, TCHAR** argv);

        /**
	    * @brief 处理用户从控制台输入的命令，是纯虚函数，需要用户自己实现内部功能。
        * @param argc :  参数数目
	    * @param argv :  参数字符串数组	    
	    */
        virtual VOID        OnHandleUserCmd(const TCHAR* szCmd) ;

        /**
	    * @brief 服务器连接成功建立，留给用户进行进行CSDPipeChannel与CSDProcessor关联的接口。
        * @param dwPipeID :  建立连接的服务器的ServerID。
        * @return CSDPipeChannel指针，由应用层创建，并返回给底层进行关联。   
	    */
        virtual CSDPipeChannel* OnPipeChannelConnected(UINT32 dwPipeID) = 0;
        
        /**
	    * @brief 服务器连接断开，底层将相关CSDPipeChannel指针传给应用层，应用层进行相关资源清理
        * @param poChannel : 连接断开的PipeChannel指针
	    */
        virtual VOID        OnPipeChannelDisconnected(CSDPipeChannel* poChannel) = 0;

    private:
        /**
	    * @brief 框架底层内部使用组件初始化。
        * @return BOOL型数据，TRUE代表组件初始化成功，FALSE代表初始化失败。   
	    */
        virtual BOOL		_InitComponent();
        
        /**
	    * @brief 释放框架底层内部使用组件的系统资源。
	    */
        virtual VOID		_UnInitComponent();

        /**
	    * @brief 更新服务器互连状态信息。
	    */
        virtual VOID        _UpdateSvrStatus();

        /**
	    * @brief 从配置文档中读取服务器互连信息。
        * @param strPipeFile : 互连信息配置文档路径
	    */
        VOID                _InitPipeInfoList(const string strPipeFile);
        
        /**
	    * @brief 删除所有服务器互连状态信息。
	    */
        VOID                _DeleteAllPipeInfo();

        /**
	    * @brief 根据ServerID查找服务器互连信息结构体
        * @param dwServerID : 指定的服务器ServerID
        * @return SPipeInfo指针，根据ServerID索引到底服务器互连信息结构体
	    */
        SPipeInfo*          _FindPipeInfo(UINT32 dwServerID);
        
/*        ISDConsole*         m_poConsole;          ///< 控制台指针
        INT32               m_nMaxFixedAreaLines; ///< 控制台固定输出区域最大行数     */    
        ISDNet*				m_poNetModule;        ///< sdnet网络层句柄指针
        ISDPipeModule*		m_poPipeModule;       ///< sdpipe网络层句柄指针
        CSDPipeChannelMgr		m_oPipeChannelMgr;    ///< 服务器互连管理器
        BOOL					m_bTerminated;        ///< 标识服务器主线程是否已退出
        tstring				m_strConfigFilename;  ///< 主配置文件路径
     
        CSDConfig			m_oConfig;            ///< 主配置文档管理对象
        CSDComLoggerImpl		m_oNetLogger;         ///< 网络层日志处理对象
        CSDComLoggerImpl		m_oPipeLogger;        ///< sdpipe日志处理对象
		CSDComLoggerImpl		m_oDBLogger;        ///< sdpipe日志处理对象

		SShmem				m_shmemPipeInfo;		///< 服务器互连列表共享内存
        //SAllPipeInfo*			m_pstAllPipeInfo;		///< 服务器互连列表
		vector<SPipeInfo>	m_vecPipeInfo;			//服务器互连列表	
        CSDCommandLine		m_oSDCommandLine;     ///< 程序主入口输入命令行参数处理对象
    };
}

/**
* @brief 获取应用程序全局唯一句柄指针，应用程序可以通过这个api在程序任何地方很方便的获取程序主入口。
*/
extern  CSDApplication* SDGetApp();

#endif

