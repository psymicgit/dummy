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

	unsigned char shiftRight(unsigned char val , unsigned char bits)
	{
		for (int i = 0 ; i < bits; i++) {
			val = (val  << 7) | (val >> 1);
		}
		return val;
	}

	unsigned char shiftLeft(unsigned char val, unsigned char bits)
	{
		for (int i = 0 ; i < bits; i++) {
			val = (val >> 7) | (val << 1);
		}

		return val;
	}

	void encrypt(uint8 *data, int dataLen, uint8 encryptKey[], uint32 keyLen)
	{
		unsigned char keyOffsetval = rand() % 256;
		unsigned char bitOffsetval = rand() % 256;
		unsigned char keyOffset = keyOffsetval % (keyLen - g_usedKeyLength);
		unsigned char bitOffset = bitOffsetval % 7 + 1;
		unsigned int  cumulative = 0;

		data[0] = bitOffsetval;
		data[1] = keyOffsetval;

		dataLen -= 2;
		data    += 2;

		unsigned int dataSize = dataLen - sizeof(unsigned int);

		for (int i = 0; i < dataLen ; i++ ) {
			if(i == dataSize) {
				*((unsigned int *)data) = cumulative;
			}
			else {
				cumulative += *data;
			}
			*data  = shiftLeft(*data, bitOffset);
			*data ^= encryptKey[keyOffset + i % g_usedKeyLength];
			data++;
		}
	}


	bool decrypt(uint8 *data, int dataLen, uint8 encryptKey[], uint32 keyLen)
	{
		unsigned char bitOffset = data[0] % 7 + 1;
		unsigned char keyOffset = data[1] % (keyLen - g_usedKeyLength);
		unsigned int cumulative = 0; //校验和

		dataLen -= 2;
		data    += 2;

		unsigned int dataSize = dataLen - sizeof(unsigned int);

		for (int i = 0; i < dataLen ; i++) {
			*data ^= encryptKey[keyOffset + i % g_usedKeyLength];
			*data  = shiftRight(*data, bitOffset);
			if( i < (int)dataSize) cumulative += *data;
			data++;
		}

		return cumulative == *((unsigned int *)(data - sizeof(unsigned int)));
	}
}
