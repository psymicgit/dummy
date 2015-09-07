///<------------------------------------------------------------------------------
//< @file:   server\tool\sockettool.h
//< @author: 洪坤安
//< @date:   2014年11月25日 14:4:34
//< @brief:  操作socket
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _sockettool_h_
#define _sockettool_h_

#include "net/net.h"

class NetAddress;

namespace socktool
{
	// 申请新的socket
	socket_t createSocket();

	// 关闭socket
	void closeSocket(socket_t);

	// 设置socket为非阻塞
	bool setNonBlocking(socket_t);

	// 关闭Nagle算法
	bool setTcpNoDelay(socket_t);

	// 设置端口重用，使得处于TIME_WAIT状态下的socket可以重复绑定使用
	// (一般来说，一个端口释放后会等待两分钟（即TIME_WAIT状态的时间）之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用，否则将提示"地址正在使用中"
	// 服务端程序应该调用bind()之前设置SO_REUSEADDR套接字选项。TCP中先调用close()的一方会进入TIME_WAIT状态)
	void setReuseAddr(socket_t, bool on);

	// 如该连接在指定秒内没有任何数据往来,则进行探测
	void setKeepAlive(socket_t, bool on, int keepAliveTime);

	// windows下默认发送缓冲区大小 = 8192，linux下默认发送缓冲区大小 = 171480
	void setSendBufSize(socket_t, int size);

	// windows下默认接收缓冲区大小 = 8192，linux下默认接收缓冲区大小 = 87380
	void setRecvBufSize(socket_t, int size);

	// 设置延迟关闭socket的时间，等待套接字发送缓冲区中的数据发送完成（慎用，不要依赖这个选项，最好结合业务和场景自行处理）
	void setLinger(socket_t, int waitTime);

	// 获取socket的错误码并置错误码为0
	int getSocketError(socket_t sockfd);

	// 打印出错误号对应的错误文本
	const char* getErrorMsg(int err);

	// 获取系统保存的错误号
	int geterrno();

	// 清空系统保存的错误号
	void clearerrno();

	// 获取本端地址
	struct sockaddr_in getLocalAddr(int sockfd);

	// 获取对端地址
	struct sockaddr_in getPeerAddr(int sockfd);

	// 为套接字命名：调用bind()函数之后，为socket()函数创建的套接字关联一个相应地址，发送到这个地址的数据可以通过该套接字读取与使用。
	bool bindAddress(socket_t sockfd, const NetAddress& localaddr);

	// 监听socket
	bool listen(socket_t);
}

#endif //_sockettool_h_