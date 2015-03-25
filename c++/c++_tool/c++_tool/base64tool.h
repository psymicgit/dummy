/********************************************************************
            Copyright (c) 2011, 欢乐连线工作室
                   All rights reserved
         
    创建日期：  2011年7月15日 11时19分
    文件名称：  Base64Tool.h
    说    明：  base64編碼工具     
    
    当前版本：  1.00
    作    者：  Sizhi Huang
    概    述：  創建    

*********************************************************************/
#pragma once
#include <string>
#include <sstream>
using namespace std;

namespace StringEncode {

	template<unsigned int size>
	const string Base64Encode(const unsigned char (&param)[size]) {
		return Base64Encode(param, size);
	}

	template<unsigned int size>
	const string Base64Encode(const char (&param)[size]) {
		return Base64Encode(param, size);
	}

	const string Base64Encode(const void *src, unsigned int bytes);

	template<unsigned int size>
	int Base64Decode(const string &str, unsigned char (&param)[size]) {
		return Base64Decode(str, param, size);
	}

	template<unsigned int size>
	int Base64Decode(const string &str, char (&param)[size]) {
		return Base64Decode(str, param, size);
	}

	int Base64Decode(const string &str, void *outBuff, unsigned int outSize);

};
