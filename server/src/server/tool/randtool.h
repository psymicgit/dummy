///<------------------------------------------------------------------------------
//< @file:   server\tool\randtool.h
//< @author: 洪坤安
//< @date:   2015年3月17日 22:40:38
//< @brief:	 获取随机数
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _randtool_h_
#define _randtool_h_

typedef std::vector<int> HitVec;

namespace randtool
{
	// 初始化随机数种子，仅需在启动时调用
	void initSeed();

	// 获取随机数
	int rand_int_all();

	// 获取随机数，范围是[0, n - 1]
	int rand_int(int n);

	// 获取随机数，范围是[a, b]
	int rand_int_between(int a, int b);

	// 获取随机浮点数
	float rand_float(float n);

	// 获取随机浮点数
	float rand_float_between(float a, float b);

	// 根据概率表进行随机，返回命中的概率序号(hitVec中的顺序),没有概率返回0序号
	int rollHit(HitVec &hitVec);

	// 获取一串随机数，基于加密的伪随机数生成器（Cryptographically Secure Pseudo-Random Number Generator – CSPRNG）
	void secureRandom(uint8 nums[], int cnt, int beg, int end);
}

#endif //_randtool_h_