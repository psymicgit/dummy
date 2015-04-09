

#ifndef _SGDP_PACKET_PROCESSOR_H_
#define _SGDP_PACKET_PROCESSOR_H_
#include "sdtype.h"
#include <map>
using namespace std;

namespace SGDP {
    class CSDPipeChannel;
    class CSDProtocol;
    typedef BOOL (*PPROCESSFUNC)(VOID* poReceiver, VOID* pHeader, const CHAR* pBody, UINT32 dwBodyLen);

#define COMMAND_COUNT      5000
#define COMMAND_STARTINDEX 1

    /**
    * @brief 网络命令的Map映射关系类
    *
    */
    class CCommandMap
    {
    public:
        /**
	    * @brief 构造函数
	    */
	    CCommandMap();
        
        /**
	    * @brief 析构函数
	    */
	    ~CCommandMap();
	    
        /**
	    * @brief 删除所有的命令
	    * @return VOID
	    */
	    VOID  Reset();

	    /**
	    * @brief 添加命令与处理命令函数的对应关系
	    * @param wMsgID 消息的ID
	    * @param pFunc 消息对应的处理函数
	    * @param bForce 是否强制执行
	    * @return BOOL 是否添加成功,如果bForce为FALSE,原来也存在此消息,则添加不成功
	    */
	    BOOL AddCommand(UINT16 wMsgID, PPROCESSFUNC pFunc, BOOL bForce = FALSE);

	    /**
	    * @brief 查找某个消息对应的处理函数
	    * @param wMsgID 消息的ID
	    * @return 返回对应的处理函数
	    *
	    */
	    PPROCESSFUNC FindCommand(UINT16 wMsgID);

	    /**
	    * @brief 删除某个消息对应的处理函数
	    * @param wMsgID 消息的ID
	    * @return 返回删除的处理函数
	    *
	    */
	    PPROCESSFUNC RemoveCommand(UINT16 wMsgID);

	    /**
	    * @brief 修改消息处理函数数组区间，本函数需要在AddCommand、RemoveCommand之前调用，
                 否则，会清除之前加的command函数。
	    * @param wArrayStart 起始消息的ID
                 wArraySize  允许的写入的消息区间大小
	    * @return 无
	    *
	    */
        VOID         InitCommandSize(const UINT16 wArrayStart,const UINT16 wArraySize);

    private:
	    PPROCESSFUNC* m_poFuncArray;  ///< 处理函数集指针
        UINT16 m_wArraySize;          ///< 处理函数容量
        UINT16 m_wArrayStart;         ///< 处理函数起始MsgID
        UINT16 m_wArrayEnd;           ///< 处理函数最大MsgID
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * @brief 网络数据包处理句柄,用于处理网络数据包
    *
    */
    class CSDPacketProcessor 
    {
    public:
        /**
	    * @brief 构造函数
	    */
	    CSDPacketProcessor();
        
        /**
	    * @brief 析构函数
	    */
	    ~CSDPacketProcessor();

	    /**
	    * @brief 初始化此句柄
	    * @return 是否初始化成功,TRUE代表成功,FALSE代表失败
	    */
        virtual BOOL Init(){return TRUE;};

	    /**
	    * @brief 释放此句柄
	    * @return VOID
	    */
        virtual VOID UnInit() {};

	    /**
	    * @brief 根据wBusinessID获取对应的协议处理类
	    * @param wBusinessID 协议命令的消息ID号
	    * @return 处理该协议命令的处理函数
	    */
        virtual CSDProtocol* GetProtocol() = 0;

	    /**
	    * @brief 根据协议命令的MessageID获取对应的处理函数
	    * @param wMsgID 协议命令的消息ID号
	    * @return 处理该协议命令的处理函数
	    */
	    PPROCESSFUNC SDAPI LookupProcessFunc(UINT16 wMsgID);

	    /**
	    * @brief 处理协议命令数据包的处理函数,此函数能够根据协议命令的类型自动选择处理函数处理
	    * 此函数包装了LookupProcessFunc函数
	    * @param poReceiver 接收此协议命令的会话
	    * @param pHeader 协议命令的消息头
	    * @param pBody 协议命令的数据包内容
	    * @param dwBodyLen 协议命令的数据包内容长度
	    * @return 是否处理成功.TRUE为处理成功,FALSE为处理失败
	    */
	    BOOL ProcessPacket(VOID* poReceiver, VOID* pHeader, const CHAR* pBody, UINT32 dwBodyLen);

	    /**
	    * @brief 注册协议命令及其对应的处理函数,没有注册的协议命令将被忽略
	    * @param wMsgID 协议命令的消息ID号
	    * @param pFunc 对应的处理函数
	    * @return VOID
	    */
	    BOOL SDAPI RegisterCommand(UINT16 wMsgID, PPROCESSFUNC pFunc, BOOL bForce = TRUE);

	    /**
	    * @brief 注册协议命令及其对应的处理函数,没有注册的协议命令将被忽略
	    * @param wMsgID 协议命令的消息ID号
	    * @param pFunc 对应的处理函数
	    * @return VOID
	    */
	    BOOL SDAPI UnRegisterCommand(UINT16 wMsgID);
	    /**
	    * @brief 修改消息处理函数数组区间，本函数需要在AddCommand、RemoveCommand之前调用，
                 否则，会清除之前加的command函数。
	    * @param wArrayStart 起始消息的ID
                 wArraySize  允许的写入的消息区间大小
	    * @return 无
	    *
	    */
        VOID         InitCommandSize(const UINT16 wArrayStart,const UINT16 wArraySize);
    private:	    
	    CCommandMap m_mapCommand;     ///< 所有的命令的存放映射
    };
}
#endif //#ifndef _SGDP_PACKET_PROCESSOR_H_

