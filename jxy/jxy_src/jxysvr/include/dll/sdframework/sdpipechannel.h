


#ifndef _SGDP_PIPE_CHANNEL_H_
#define _SGDP_PIPE_CHANNEL_H_

#include "sdtype.h"
#include "sdnet.h"
#include "sdpipe.h"
#include "sdstring.h"
#include "sdpacketprocessor.h"
#include "netdata.h"

namespace SGDP{
    class CSDPacketProcessor;
    /**
    * @brief CSDPipeChannel 服务器间互连通道管理类
    *
    */
    class CSDPipeChannel : public ISDPipeSink
    {
    public:
        /**
	    * @brief 构造函数
	    */
        CSDPipeChannel();
        
        /**
	    * @brief 析构函数
	    */
        ~CSDPipeChannel();

        /**
	    * @brief 设定sdpipe的操作句柄指针，以备，发送数据使用，以及将该类关联到通信底层
	    * @param pPipe 网络通信层句柄指针
	    */
        VOID            SetPipe(const ISDPipe* pPipe);

        /**
	    * @brief 获取PipeChannel的对方的ServerID值
	    * @return 返回PipeChannel的对方的ServerID值
	    */
        UINT32			GetPipeID() const;
        
        /**
	    * @brief 获取PipeChannel的对方服务器类型
	    * @return 返回PipeChannel的对方服务器类型
	    */
        UINT8			GetSvrType() const;

        /**
	    * @brief 获取PipeChannel的对方本地IP地址
	    * @return 返回PipeChannel的对方本地IP地址字符串
	    */
        TCHAR*			GetLocalIPStr() const;
        
        /**
	    * @brief 获取PipeChannel的对方本地IP地址
	    * @return 返回PipeChannel的对方本地IP地址整型数据
	    */
        UINT32			GetLocalIP() const; 
        
        /**
	    * @brief 获取PipeChannel的对方本地IP地址连接端口
	    * @return 返回PipeChannel的对方本地IP地址连接端口
	    */
        UINT16          GetLocalPort() const;
        
        /**
	    * @brief 获取PipeChannel的对方远程IP地址
	    * @return 返回PipeChannel的对方远程IP地址字符串
	    */
        tstring			GetRemoteIPStr() const;

        /**
	    * @brief 获取PipeChannel的对方远程IP地址
	    * @return 返回PipeChannel的对方远程IP地址整型数据
	    */
        UINT32			GetRemoteIP() const; 
        
        /**
	    * @brief 获取PipeChannel的对方远程IP地址连接端口
	    * @return 返回PipeChannel的对方远程IP地址连接端口
	    */
        UINT16          GetRemotePort() const;
        
        /**
	    * @brief 设定PipeChannel协议处理关联协议族
	    * @param poPacketProcessor PipeChannel协议处理关联协议族指针
	    */
        VOID            SetPacketProcessor(CSDPacketProcessor* poPacketProcessor);

        /**
	    * @brief 发送传输数据
	    * @param pszData: 需要传输的数据内存指针
        * @param dwLen: 需要传输的数据内存长度
	    * @return BOOL型数据，TRUE代表发送成功；FALSE代表发送失败。
	    */
        BOOL            Send( const CHAR* pszData, const UINT32 dwLen);

        /**
	    * @brief 发送传输消息数据，与Send不同的是，SendMsg将执行Encode操作，封装报文
	    * @param dwTransID: 报文处理的CUser对象对应的ClientID
        * @param dwMsgID: 发送消息的MessageID
	    * @param pData: 需要传输的数据内存指针
	    * @return BOOL型数据，TRUE代表发送成功；FALSE代表发送失败。
	    */
        BOOL            SendMsg( UINT32 dwTransID, const UINT32 dwMsgID ,const CHAR* pData);
        
        /**
	    * @brief 服务器连接成功建立，系统内部自动回调
	    */
        virtual VOID    OnEstablish();
    protected:    
        /**
	    * @brief 服务器连接成功建立，系统内部自动回调
	    */
        virtual VOID    OnTerminate();

        /**
	    * @brief 服务器连接出现异常错误。
        * @param nErrCode: 异常错误的错误码
	    */
        virtual VOID    OnError(const INT32 nErrCode);
    protected:
        CSDPacketProcessor*  m_poPacketProcessor; ///< 处理函数集指针

    private:      
        /**
	    * @brief 服务器连接状态变迁回调函数。
        * @param wBusinessID: 业务类型码
        * @param nErrCode: 异常错误的错误码
	    */
        virtual VOID SDAPI   OnReport(UINT16 wBusinessID, INT32 nErrCode);

        /**
	    * @brief 收到来自服务器传输过来的报文数据。
        * @param wBusinessID: 业务类型码
        * @param pData: 收到的数据内存指针
        * @param pData: 收到的数据内存长度
	    */
        virtual VOID SDAPI   OnRecv(UINT16 wBusinessID, const CHAR *pData, UINT32 dwLen);

        ISDPipe*			 m_poPipe;            ///< 该网络连接对应的sdpipe层网络句柄指针
        UINT8                m_byServerType;      ///< 该网络连接对应的服务器类型
        static UINT32        m_dwIndex;           ///< 网络连接静态索引·
    };

    /**
    * @brief 服务器之间的多重互连的状态管理回调接口，以及PipeChannel的集中管理器
    *
    */
    class CSDPipeChannelMgr : public ISDPipeReporter
    {
    public:
        typedef map<UINT32, CSDPipeChannel*>            CPipeChannelMap;
        typedef map<UINT32, CSDPipeChannel*>::iterator  CPipeChannelMapItr;
      
        /**
	    * @brief 构造函数
	    */
        CSDPipeChannelMgr();
        
        /**
	    * @brief 析构函数
	    */
        ~CSDPipeChannelMgr();

	    /**
	    * @brief 类成员数据初始化接口
	    * @param poPipeModule : 框架应用层启用的pipe模块句柄指针
	    */
        VOID               Init(ISDPipeModule* poPipeModule);

        /**
	    * @brief 释放底层框架向系统申请的资源
	    */
        VOID               UnInit();

        /**
	    * @brief 根据ServerID，找到对应的服务器连接
	    * @param dwID : 需要查找的服务器连接的ServerID
        * @return 如果返回值为空，则代表指定的服务器连接不存在，否则，返回的是对应的网络层连接PipeChannel指针。
	    */
        CSDPipeChannel*	   FindPipeChannel(const UINT32 dwID);

        /**
	    * @brief 向连接管理器中添加一个网络连接句柄指针，以PipeChannel对应的ServerID为key。
	    * @param poPipeChannel : 需要添加到管理器中的网络连接句柄指针
	    */
        VOID               AddPipeChannel ( CSDPipeChannel* poPipeChannel );

        /**
	    * @brief 从管理器中根据指定的ServerID，删除对应的网络连接指针对象
	    * @param dwID : 需要添加到管理器中的网络连接句柄指针
	    */
        VOID               RemovePipeChannel(const UINT32 dwID);

        /**
	    * @brief 获得连接管理器map引用对象
        * @return 返回连接管理器map引用对象
	    */
        CPipeChannelMap&   GetPipeChannelMap();

        /**
	    * @brief 将数据向同类型网络层连接广播
	    * @param dwTranID : 报文处理的CUser对象对应的ClientID
        * @param wMsgID : 发送消息的MessageID
	    * @param pData: 需要传输的数据内存指针
	    * @param byType: 需要广播数据的服务器类型
	    */
        VOID               BroadcastToSameTypeServers(
                                                      const UINT32 dwTranID,
                                                      const UINT16 wMsgID,
                                                      const CHAR*  pData,                                                      
                                                      const UINT8 byType);

    protected:
        /**
	    * @brief 服务器连接状态变迁回调函数。
        * @param dwID: 对应服务器的ServerID
        * @param nErrCode: 异常错误的错误码
	    */
        virtual VOID SDAPI OnReport(INT32 nErrCode, UINT32 dwID);
       
        /**
	    * @brief 服务器连接成功建立，系统回调
        * @param dwPipeID: 对应服务器的ServerID
	    */
        virtual VOID _OnConnected(UINT32 dwPipeID);

        /**
	    * @brief 服务器连接断开，系统回调
        * @param dwPipeID: 对应服务器的ServerID
	    */
        virtual VOID _OnDisconnected(UINT32 dwPipeID);
    private:
        CPipeChannelMap     m_mapPipeChannel;    ///< 服务器网络连接map管理器
        ISDPipeModule*      m_poPipeModule;      ///< application层开启的pipe模块句柄指针
    };

}
#endif

