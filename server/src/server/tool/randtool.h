///<------------------------------------------------------------------------------
//< @file:   server\tool\randtool.h
//< @author: 洪坤安
//< @date:   2015年3月17日 22:40:38
//< @brief:	 获取随机数
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _randtool_h_
#define _randtool_h_

typedef std::vector<uint32> HitVec;

namespace randtool
{
	// 初始化随机数种子，仅需在启动时调用
	void initSeed();

	// 返回随机数
	uint32 random();

	// 获取随机数，范围是[0, unRange - 1]
	uint32 random(uint32 range);

	// 获取随机数，范围是[unMinRange, unMaxRange - 1]
	uint32 random(uint32 minnum, uint32 maxnum);

	// 必须命中算法，返回命中的概率序号(hitVec中的顺序),没有概率返回0序号
	uint32 rollHit(HitVec &hitVec);

	// 获取一串随机数，基于加密的伪随机数生成器（Cryptographically Secure Pseudo-Random Number Generator – CSPRNG）
	void secureRandom(uint8 nums[], uint32 cnt, int beg, int end);
}

#endif //_randtool_h_