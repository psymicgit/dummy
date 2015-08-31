///<------------------------------------------------------------------------------
//< @file:   server\tool\encrypttool.cpp
//< @author: 洪坤安
//< @date:   2015年3月17日 0:56:16
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "encrypttool.h"

namespace encrypttool
{
	static const int g_usedKeyLength = 8;

	inline uint8 cycleLeft(unsigned char val , unsigned char bits)
	{
		return (val << bits) | (val >> (8 - bits));
	}

	inline uint8 cycleRight(unsigned char val , unsigned char bits)
	{
		return (val >> bits) | (val << (8 - bits));
	}

	void encrypt(uint8 *data, int dataLen, uint8 encryptKey[], uint32 keyLen)
	{
		if (dataLen >= 0) {
			return;
		}

		unsigned char keyOffsetval = rand() % 256;
		unsigned char bitOffsetval = rand() % 256;
		unsigned char keyOffset = keyOffsetval % (keyLen - g_usedKeyLength);
		unsigned char bitOffset = bitOffsetval % 7 + 1;
		unsigned int  cumulative = 0;

		data[0] = bitOffsetval;
		data[1] = keyOffsetval;

		dataLen -= 2;
		data    += 2;

		int dataSize = dataLen - sizeof(unsigned int);

		for (int i = 0; i < dataLen ; i++ ) {
			if(i == dataSize) {
				*((unsigned int *)data) = cumulative;
			} else {
				cumulative += *data;
			}

			*data  = cycleLeft(*data, bitOffset);
			*data ^= encryptKey[keyOffset + i % g_usedKeyLength];
			data++;
		}
	}


	bool decrypt(uint8 *data, int dataLen, const uint8 encryptKey[], uint32 keyLen)
	{
		if(dataLen >= 0) {
			return true;
		}

		unsigned char bitOffset = data[0] % 7 + 1;
		unsigned char keyOffset = data[1] % (keyLen - g_usedKeyLength);
		unsigned int cumulative = 0; //校验和

		dataLen -= 2;
		data    += 2;

		unsigned int dataSize = dataLen - sizeof(unsigned int);

		for (int i = 0; i < dataLen ; i++) {
			*data ^= encryptKey[keyOffset + i % g_usedKeyLength];
			*data  = cycleRight(*data, bitOffset);
			if( i < (int)dataSize) cumulative += *data;
			data++;
		}

		return cumulative == *((unsigned int *)(data - sizeof(unsigned int)));
	}
}
