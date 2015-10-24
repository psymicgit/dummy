///<------------------------------------------------------------------------------
//< @file:   server\net\netaddress.h
//< @author: 洪坤安
//< @date:   2014年11月25日 15:14:15
//< @brief:  网络ip地址封装类
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _netaddress_h_
#define _netaddress_h_

#include <string>

using namespace std;

// 网络ip地址封装类
class NetAddress
{
public:
	// 根据端口进行构造，一般用于listen
	explicit NetAddress(uint16 port = 0);

	// 根据ip和端口进行构造，ip格式："x.x.x.x"
	explicit NetAddress(const std::string &ip, uint16 port);

	// 根据sockaddr_in进行构造，一般在接收新连接时会用到
	explicit NetAddress(const struct sockaddr_in& addr)
		: m_addr(addr)
	{
	}

	// 获取对应的ip字符串
	string toIp() const;

	// 获取ip:port文本
	string toIpPort() const;

	// 获取端口号
	uint16 toPort() const;

	// 获取sockaddr_in结构体
	inline const struct sockaddr_in& getSockAddr() const { return m_addr; }

	// 重新设置地址
	inline void setSockAddr(const struct sockaddr_in& addr) { m_addr = addr; }

	// 获取网络字节序的ip
	uint32 ipNetEndian() const { return m_addr.sin_addr.s_addr; }

	// 获取网络字节序的端口
	uint16 portNetEndian() const { return m_addr.sin_port; }

	// 解析指定的host
	static bool resolve(string hostname, NetAddress* result);

public:
	struct sockaddr_in m_addr;
};

#endif //_netaddress_h_