//
//  MGEncrypt.cpp
//  MyGame
//
//  Created by  on 12-3-11.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "mgencrypt.h"

static const int UsedKeyLength = 8;

MGEncrypt::MGEncrypt()
{
     m_pEncryptKey = NULL;
     m_nKeyLength = 0;
     time_t t = time(NULL);
     srand((unsigned int)t);
}

MGEncrypt::~MGEncrypt()
{
     if(m_pEncryptKey) 
     {
        free(m_pEncryptKey);
        m_pEncryptKey = NULL;
     }
}

unsigned char shiftRight(unsigned char val ,unsigned char bits)
{
     for (int i =0 ; i< bits; i++)
     {
        val = (val  << 7) | (val >> 1);
     }
     return val;
}

unsigned char shiftLeft(unsigned char val, unsigned char bits)
{
     for (int i =0 ; i < bits; i++)
     {
        val = (val >> 7) | (val<<1);
     }
     return val;
}

void MGEncrypt::setEncryptKey(const unsigned char * pEncryptKey, unsigned int keyLength)
{
     if( !pEncryptKey )
     {
        if (m_pEncryptKey) 
        {
           free(m_pEncryptKey);
           m_pEncryptKey=NULL;
        }
        m_nKeyLength = 0;
        return;
     }

     if(!m_pEncryptKey)
     {
        m_pEncryptKey = (unsigned char *)malloc(keyLength);
     }
     else if(keyLength != m_nKeyLength)
     {
        realloc(m_pEncryptKey,keyLength);
     }

     for(unsigned int i = 0; i < keyLength; i++)
     {
        m_pEncryptKey[i] = shiftRight(pEncryptKey[i], i % 7 + 1);
        int s = m_pEncryptKey[i];
     }

     m_nKeyLength = keyLength;
     //memcpy(m_pEncryptKey,pEncryptKey,keyLength);
}


void MGEncrypt::setKey(const unsigned char * pEncryptKey, unsigned int keyLength)
{
    if( !pEncryptKey )
    {
      if (m_pEncryptKey) 
      {
         free(m_pEncryptKey);
         m_pEncryptKey=NULL;
      }
      m_nKeyLength = 0;
      return;
    }

    if(!m_pEncryptKey)
    {
       m_pEncryptKey = (unsigned char *)malloc(keyLength);
    }
    else if(keyLength != m_nKeyLength)
    {
       realloc(m_pEncryptKey,keyLength);
    }
    m_nKeyLength = keyLength;
    memcpy(m_pEncryptKey,pEncryptKey,keyLength);
}

void MGEncrypt::encryptionDatas(unsigned char * pDatas, int dataSize)
{

    unsigned char keyOffsetval = rand() % 256;
    unsigned char bitOffsetval = rand() % 256;
    unsigned char keyOffset = keyOffsetval % (m_nKeyLength - UsedKeyLength);
    unsigned char bitOffset = bitOffsetval % 7 + 1;
    unsigned int  cumulative = 0; 
    
    assert(m_pEncryptKey);
    assert(pDatas);

    pDatas[0] = bitOffsetval;
    pDatas[1] = keyOffsetval;
    dataSize -= 2;  // 
    pDatas   += 2;  //

    unsigned int dataLen = dataSize - sizeof(unsigned int);

    for (int i = 0; i< dataSize ; i++ ) 
    {
        if(i == dataLen)
        {
           *((unsigned int *)pDatas) = cumulative;
        }
        else
        {
            cumulative += *pDatas;
        }
        *pDatas  = shiftLeft(*pDatas, bitOffset);
        *pDatas ^= m_pEncryptKey[keyOffset + i % UsedKeyLength]; 
        pDatas++;
    }
}


bool MGEncrypt::decryptionDatas(unsigned char * pDatas, int dataSize)
{
    unsigned char bitOffset = pDatas[0] % 7 + 1;
    unsigned char keyOffset = pDatas[1] % (m_nKeyLength - UsedKeyLength);
    unsigned int cumulative = 0; //校验和

    assert(m_pEncryptKey);
    assert(pDatas);

    dataSize -= 2;
    pDatas   += 2;

    unsigned int dataLen = dataSize - sizeof(unsigned int);

    for (int i = 0; i < dataSize ; i++)
    {
        *pDatas ^= m_pEncryptKey[keyOffset + i % UsedKeyLength];
        *pDatas  = shiftRight(*pDatas, bitOffset);
        if( i < dataLen ) cumulative += *pDatas;
        pDatas++;
    }
    return cumulative == *((unsigned int *)(pDatas - sizeof(unsigned int)));
}

inline bool RandomNumbercode(unsigned char* RandomArray,unsigned int size)
{
	time_t t;
	srand((unsigned)time(&t));
	if(RandomArray==NULL)
	{
		return false;
	}
	for(unsigned int i=0;i<size;i++)
	{
		RandomArray[i]=rand()%10+48;
	}
	return true;
}

