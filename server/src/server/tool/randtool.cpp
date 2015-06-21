///<------------------------------------------------------------------------------
//< @file:   server\tool\randtool.cpp
//< @author: 洪坤安
//< @date:   2015年3月16日 23:50:51
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "randtool.h"

#ifdef WIN
	#include <WinCrypt.h>
#else

#endif

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

	void secureRandom(uint8 nums[], uint32 cnt, int beg, int end)
	{
		int range = end - beg;

#ifdef WIN
		HCRYPTPROV  hCryptProv = 0;
		CryptGenRandom(hCryptProv, cnt, nums);

		for (uint32 i = 0; i < cnt; i++) {
			nums[i] = random();

			nums[i] = nums[i] % range + beg;
		}
#else
		for (uint32 i = 0; i < cnt; i++) {
			nums[i] = random() % range + beg;
		}
#endif
	}
}