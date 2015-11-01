///<------------------------------------------------------------------------------
//< @file:   server\tool\encrypttool.h
//< @author: 洪坤安
//< @date:   2015年3月17日 0:56:2
//< @brief:	 加解密工具类
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _encrypttool_h_
#define _encrypttool_h_

// 加解密工具类
namespace encrypttool
{
	// 异或方式加密（很原始的加密方式，容易被破解）
	void xor_encrypt(uint8 *data, int dataLen, uint8 encryptKey[], uint32 keyLen);

	// 异或方式解密（很原始的解密方式，容易被破解）
	bool xor_decrypt(uint8 *data, int dataLen, const uint8 encryptKey[], uint32 keyLen);
}

#endif //_encrypttool_h_