///<------------------------------------------------------------------------------
//< @file:   server\net\imsghandler.h
//< @author: 洪坤安
//< @date:   2015年3月17日 23:37:25
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _imsghandler_h_
#define _imsghandler_h_

// 消息处理器
// （这里之所以提取出IMsgHandler接口是为了防止模板文件互相包含导致编译不过）
template <typename LinkType>
class IMsgHandler
{
public:
	virtual ~IMsgHandler() {}

	virtual void clear() = 0;
};

#endif //_imsghandler_h_