
#ifndef _CLIPKTPARSER_H_
#define _CLIPKTPARSER_H_

/**
 * @file 报文校验模块
 *
 * 用于校验报文完整性与合法性，目前作用是拼包
 *
 * @author 技术中心服务器引擎部
 */
#include "sdnet.h"

/**
 * @brief 报文合法性和完整性校验
 */
class CCliPacketParser : public SGDP::ISDPacketParser
{
private:
    CCliPacketParser();

    virtual ~CCliPacketParser();

public:
    /**
     * @brief 获取实例
     * @return 报文校验模块对象指针
     */
    static CCliPacketParser* Instance();

    /**
     * @brief 解析报文
     * @param pBuf 接收到的报文地址
     * @param pBuf 接收到的字节数
     * @return 接收到的实际大小
     */
    virtual INT32 SDAPI ParsePacket(const CHAR *pBuf, UINT32 dwLen);
};

#endif // #ifndef _CLIPKTPARSER_H_

