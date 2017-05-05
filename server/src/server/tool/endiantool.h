///<------------------------------------------------------------------------------
//< @file:   server\tool\endiantool.h
//< @author: 洪坤安
//< @date:   2014年11月25日 15:26:49
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _endiantool_h_
#define _endiantool_h_

namespace endiantool
{
	// 主机字节序转换成网络字节序：64位整数
	uint64_t HostToNetwork64(uint64_t host64);

	// 将32位的整数从主机字节顺序转换成网络字节顺序
	uint32 HostToNetwork32(uint32 host32);

	// 将16位的整数从主机字节顺序转换成网络字节顺序
	uint16 HostToNetwork16(uint16 host16);

	// 将64位的整数从网络字节顺序转换成主机字节顺序
	uint64_t NetworkToHost64(uint64_t net64);

	// 将32位的整数从网络字节顺序转换成主机字节顺序
	uint32 NetworkToHost32(uint32 net32);

	// 将16位的整数从网络字节顺序转换成主机字节顺序
	uint16 NetworkToHost16(uint16 net16);
}

#endif //_endiantool_h_