///<------------------------------------------------------------------------------
//< @file:   server\tool\randtool.cpp
//< @author: 洪坤安
//< @date:   2015年3月16日 23:50:51
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "randtool.h"

namespace randtool
{
	void initSeed()
	{
		srand((unsigned)time(NULL));
	}

	uint32 random()
	{
		return rand();
	}

	uint32 random(uint32 range)
	{
		return rand() % range;
	}

	uint32 random(uint32 minnum, uint32 maxnum)
	{
		if ( minnum >= maxnum ) {
			return maxnum;
		}

		return rand() % (maxnum - minnum) + minnum;
	}

	uint32 rollHit(HitVec &hitVec)
	{
		if (hitVec.empty()) {
			return 0;
		}

		uint32 totalWeight = 0;
		for (size_t i = 0; i < hitVec.size(); i++) {
			totalWeight += hitVec[i];
		}

		uint32 hit = random(totalWeight);

		totalWeight = 0;

		for (size_t i = 0; i < hitVec.size(); i++) {
			totalWeight += hitVec[i];

			if(hit < totalWeight) {
				return i;
			}
		}

		return 0;
	}
}