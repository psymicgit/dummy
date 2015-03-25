///<------------------------------------------------------------------------------
//< @file:   server\basic\buffer.cpp
//< @author: 洪坤安
//< @date:   2015年2月26日 18:33:12
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "buffer.h"

const char Buffer::kCRLF[] = "\r\n";

const size_t Buffer::kCheapPrepend = 8;
const size_t Buffer::kInitialSize = 1024;