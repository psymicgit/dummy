//
//  MGEncrypt.h
//  MyGame
//
//  Created by  on 12-3-11.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef MyGame_MGEncrypt_h
#define MyGame_MGEncrypt_h

#include "math.h"
#include <assert.h>

class MGEncrypt
{
public:
    MGEncrypt();
    ~MGEncrypt();
    void setEncryptKey(const unsigned char * pEncryptKey, unsigned int keyLength);
    void setKey(const unsigned char * pEncryptKey, unsigned int keyLength);
    void encryptionDatas(unsigned char * pDatas, int dataSize);
    bool decryptionDatas(unsigned char * pDatas, int dataSize);	
	unsigned int getKeyLength() { return m_nKeyLength; }
	void clear() { m_nKeyLength = 0;}
private:
    unsigned char * m_pEncryptKey;
    unsigned int  m_nKeyLength;
};
unsigned char shiftRight(unsigned char val ,unsigned char bits);
unsigned char shiftLeft(unsigned char val, unsigned char bits);
bool RandomNumbercode(unsigned char* RandomArray,unsigned int size);

#define ENCRYPT_HEAD_LEN 2  //加解密头大小，保存使用的密钥偏移量和使用的密钥字节数
#define ENCRYPT_TAIL_LEN 4  //加解密尾大小，保存检验和

#endif
