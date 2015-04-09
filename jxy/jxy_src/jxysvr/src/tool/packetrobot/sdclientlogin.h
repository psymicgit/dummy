
#ifndef _SDCLIENTLOGIN_H_
#define _SDCLIENTLOGIN_H_

/**
 * @file 客户端登录模块
 *
 * @author 技术中心服务器引擎部
 */
#include <sdtype.h>

/**
 * @brief PT验证登录
 * @param pUserName 帐号
 * @param pPassword 密码
 * @param pNet 报文缓冲区地址
 * @param iNetSize 报文缓冲区大小
 * @return 成功返回登录报文大小，失败返回负数
 */
INT32 PTLogin(CHAR* pUserName, CHAR* pPassword, CHAR* pNet, INT32 iNetSize);

/**
 * @brief 协议编码
 * @param iMsgID 协议编号
 * @param pHost 待编码的数据
 * @param pNet 编码后的数据
 * @param iNetSize 输出缓冲区长度
 * @return 返回编码后的数据长度
 */
INT32 Encode(INT32 iMsgID, CHAR* pHost, CHAR* pNet, INT32 iNetSize);

/**
 * @brief 协议解码
 * @param iMsgID 协议编号
 * @param pHost 待编码的数据
 * @param pNet 编码后的数据
 * @param iNetSize 输出缓冲区长度
 * @return 返回编码后的数据长度
 */
INT32 Decode(INT32 iMsgID, CHAR* pNet, INT32 iNetSize, CHAR* pHost, INT32 iHostSize);

/**
 * @brief 获取报文数据长度
 * @param pBuf 报文缓冲区地址
 * @return 报文内容大小，错误返回-1
 */
INT32 GetDataLength(CHAR *pBuf);

/**
 * @brief 获取报文协议号
 * @param pBuf 报文缓冲区地址
 * @return 报文协议号，错误返回-1
 */
INT32 GetMsgID(CHAR *pBuf);

/**
 * @brief 获取报文头长度
 * @return 报文头长度
 */
INT32 GetHeaderLen();

#endif // #ifndef _SDCLIENTLOGIN_H_

