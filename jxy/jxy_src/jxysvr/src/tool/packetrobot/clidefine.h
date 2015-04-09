#ifndef _CLIDEFINE_H_
#define _CLIDEFINE_H_

#include <sdbase.h>

/// 发送缓冲区大小
#define SD_CLIENT_SENDBUFFER    200 * 1024
/// 报文最大长度
#define PACKAGE_LENGTH          1024 * 1024

//网络包头起始标志
#define PKTHEADMAK				0xCCDD

/**
 * @brief 网络层报文头
 */
#pragma pack(push, 1)
struct STNetMsgHeader
{
	UINT16	wMark;			///< 包头标示
    UINT16	wMsgID;         ///< 消息号
    UINT32	dwDataLen;      ///< 数据包长度
};
#pragma pack(pop)

/// 报文内容最大长度
#define PACKAGE_CONTENT_LENGTH  PACKAGE_LENGTH - sizeof(STNetMsgHeader)

/// 客户端版本
const SGDP::SSGDPVersion SDCLIENT_MODULE_VERSION = {3, 2, 0, 0};

#endif // #ifndef _CLIDEFINE_H_

