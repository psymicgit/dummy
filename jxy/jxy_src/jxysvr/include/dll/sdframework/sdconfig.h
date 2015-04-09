

#ifndef __SGDP_CONFIG_XML_H_
#define __SGDP_CONFIG_XML_H_

#include <string>
#include <string.h>
#include "sdxmlfile.h"
#include "sdtype.h"

using namespace std;

namespace SGDP {  
    /**
    * @brief CSDConfig类
    * 底层框架主配置文档处理类。
    */
    class CSDConfig  
    {
    public:
        /**
	    * @brief 构造函数
	    */
	    CSDConfig();
    
        /**
	    * @brief 析构函数
	    */
	    ~CSDConfig();

        /**
	    * @brief 释放底层框架向系统申请的资源
	    */
	    virtual VOID        UnInit();

	    /**
	    * @brief 底层框架数据、对象、参数初始化函数
	    * @param pszConfigFilename : 主配置文档路径，utf8格式的xml文件
	    * @return BOOL型数据，TRUE代表初始化成功；FALSE代表初始化失败
	    */
	    virtual BOOL        Init(const TCHAR* pszFilename);
	    
        /**
	    * @brief 获取服务器名称
	    * @return 返回服务器名称
	    */
        std::string         GetServerName();
        
        /**
	    * @brief 获取pipe连接黑白名单配置文档路径
	    * @return 返回pipe连接黑白名单配置文档路径
	    */
        std::string         GetIpListFile();

        /**
	    * @brief 获取pipe连接配置文档路径
	    * @return 返回pipe连接配置文档路径
	    */
        std::string         GetPipeFile();

        /**
	    * @brief 获取服务器ServerID
	    * @return 返回服务器ServerID
	    */
	    UINT32              GetServerID();

        /**
	    * @brief 获取PipeModel中run函数设定的参数
	    * @return 返回PipeModel中run函数设定的参数
	    */
        UINT32              GetPipeModuleCount();

        /**
	    * @brief 获取NetModel中run函数设定的参数
	    * @return 返回NetModel中run函数设定的参数
	    */
        UINT32              GetNetModuleCount();

        /**
	    * @brief 获取是否在控制台显示信息的状态标识
	    * @return 返回是否在控制台显示信息的状态标识，TRUE代表需要显示在控制台；FALSE标识不显示
	    */
        BOOL                GetShowConnectionStatus();

        /**
	    * @brief 设定是否在控制台显示信息的状态标识
	    * @param bShowConnectionStatus 返回是否在控制台显示信息的状态标识
	    */
        VOID                SetShowConnectionStatus(const BOOL bShowConnectionStatus);
      
        /**
	    * @brief 获取NetModel中接收缓冲区大小
	    * @return 返回NetModel中接收缓冲区大小
	    */
        UINT32              GetRecvBufferSize();

        /**
	    * @brief 获取NetModel中发送缓冲区大小
	    * @return 返回NetModel中发送缓冲区大小
	    */
        UINT32              GetSendBufferSize();

        /**
	    * @brief 获取NetModel中侦听的IP地址
	    * @return 返回NetModel中侦听的IP地址
	    */
        std::string         GetListenIp();

        /**
	    * @brief 获取NetModel中侦听的端口
	    * @return 返回NetModel中侦听的端口
	    */
        UINT16              GetListenPort();

        /**
	    * @brief 获取最大允许连接用户数目
	    * @return 返回最大允许连接用户数目
	    */
        UINT32              GetMaxUsers();

		UINT32				GetNetLevel();
		UINT32				GetPipeLevel();
		UINT32				GetDbLevel();

    protected:
	    tstring             m_strFilename;           ///< 主配置文档路径
	    std::string         m_strServerName;         ///< 服务器名称
        std::string         m_strPipeFile;           ///< pipe连接配置文档路径
        std::string         m_strIpList;             ///< pipe连接黑白版配置文档路径
        std::string         m_strListenIp;           ///< 侦听的IP地址
        UINT16              m_wPort;                 ///< 侦听的端口
	    UINT32              m_dwServerID;            ///< 服务器的ServerID
        UINT32              m_dwNetModuleCount;      ///< NetModel运行函数Run的参数
        UINT32              m_dwPipeModuleCount;     ///< PipeModel运行函数Run的参数
        UINT32              m_dwRecvBufferSize;      ///< 接收缓冲区大小
        UINT32              m_dwSendBufferSize;      ///< 发送缓冲区大小
        BOOL                m_bShowConnectionStatus; ///< 控制台是否显示信息标识
        UINT32				m_dwMaxUsers;            ///< 最大允许连接用户数目
		UINT32				m_dwNetLevel;
		UINT32				m_dwPipeLevel;
		UINT32				m_dwDbLevel;
        
    };
}

#endif

