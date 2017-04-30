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

	int rand_int_all()
	{
		return rand();
	}

	// 获取随机数，范围是[0, n - 1]
	int rand_int(int n)
	{
		return rand() % n;
	}

	int rand_int_between(int a, int b)
	{
		if ( a >= b ) {
			return b;
		}

		return rand() % (b - a) + a;
	}

	float rand_float(float n)
	{
		return ((float)rand() / RAND_MAX) * n;
	}

	float rand_float_between(float a, float b)
	{
		return a + rand_float(b - a);
	}

	int rollHit(HitVec &hitVec)
	{
		if (hitVec.empty()) {
			return 0;
		}

		int totalWeight = 0;
		for (size_t i = 0; i < hitVec.size(); i++) {
			totalWeight += hitVec[i];
		}

		int hit = rand_int(totalWeight);

		totalWeight = 0;

		for (size_t i = 0; i < hitVec.size(); i++) {
			totalWeight += hitVec[i];

			if(hit < totalWeight) {
				return i;
			}
		}

		return 0;
	}

	void secureRandom(uint8 nums[], int cnt, int beg, int end)
	{
		int range = end - beg;

#ifdef WIN
		HCRYPTPROV  hCryptProv = 0;
		CryptGenRandom(hCryptProv, cnt, nums);

		for (int i = 0; i < cnt; i++) {
			nums[i] = rand();

			nums[i] = nums[i] % range + beg;
		}
#else
		for (int i = 0; i < cnt; i++) {
			nums[i] = rand() % range + beg;
		}
#endif
	}
}