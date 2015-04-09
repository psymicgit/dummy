#include <string.h>
#include <assert.h>
#include "algorithm/sdrsahlp.h"
#include "sddebug.h"
#if SDU_WITH_OPENSSL
namespace SGDP
{
    static BIGNUM* NewBigNum();
    static VOID FreeBigNum(BIGNUM* &pBigNum);

    static VOID CopyBigNum(BIGNUM* pstTo, const BIGNUM* pstFrom);
    static UINT32 SerializeBigNum(const BIGNUM* pKey, UINT8* pBuf);
    static VOID ReadBigNum(BIGNUM* &pBigNum, UINT8* &p);
	static VOID ReadBigNum(BIGNUM*& pBigNum, UINT8*& p, INT32 iNumSize /*4 or 8*/ );


    BOOL GenerateRsaKey(INT32 nBit, RSA *pstPublicKey, RSA *pstPrivateKey)
    {
        RSA* pstRsa = RSA_generate_key(nBit, 65537, NULL, NULL);
        if(NULL == pstRsa)
            return false;

        //Copy Public Key
        pstPublicKey->n = NewBigNum();
        CopyBigNum(pstPublicKey->n, pstRsa->n);
        pstPublicKey->e = NewBigNum();
        CopyBigNum(pstPublicKey->e, pstRsa->e);
        pstPublicKey->d = NULL;
        pstPublicKey->p = NULL;
        pstPublicKey->q = NULL;
        pstPublicKey->dmp1 = NewBigNum();
        CopyBigNum(pstPublicKey->dmp1, pstRsa->dmp1);
        pstPublicKey->dmq1 = NewBigNum();
        CopyBigNum(pstPublicKey->dmq1, pstRsa->dmq1);

        //Copy Private Key
        pstPrivateKey->n = NewBigNum();
        CopyBigNum(pstPrivateKey->n, pstRsa->n);
        pstPrivateKey->e = NewBigNum();
        CopyBigNum(pstPrivateKey->e, pstRsa->e);
        pstPrivateKey->d = NewBigNum();
        CopyBigNum(pstPrivateKey->d, pstRsa->d);
        pstPrivateKey->p = NewBigNum();
        CopyBigNum(pstPrivateKey->p, pstRsa->p);
        pstPrivateKey->q = NewBigNum();
        CopyBigNum(pstPrivateKey->q, pstRsa->q);
        pstPrivateKey->dmp1 = NewBigNum();
        CopyBigNum(pstPrivateKey->dmp1, pstRsa->dmp1);
        pstPrivateKey->dmq1 = NewBigNum();
        CopyBigNum(pstPrivateKey->dmq1, pstRsa->dmq1);

        RSA_free(pstRsa);

        return true;
    }

    VOID FreePrivateKey(RSA*& pstPrivateKey)
    {
        assert(pstPrivateKey != NULL);
        FreeBigNum(pstPrivateKey->n);
        FreeBigNum(pstPrivateKey->e);
        FreeBigNum(pstPrivateKey->d);
        FreeBigNum(pstPrivateKey->p);
        FreeBigNum(pstPrivateKey->q);
        FreeBigNum(pstPrivateKey->dmp1);
        FreeBigNum(pstPrivateKey->dmq1);

        RSA_free(pstPrivateKey);

        pstPrivateKey = NULL;
    }

    VOID FreePublicKey(RSA*& pstPublicKey)
    {
        assert(pstPublicKey != NULL);
        FreeBigNum(pstPublicKey->n);
        FreeBigNum(pstPublicKey->e);
        FreeBigNum(pstPublicKey->dmp1);
        FreeBigNum(pstPublicKey->dmq1);

        RSA_free(pstPublicKey);

        pstPublicKey = NULL;
    }

  //  BOOL SavePublicKey(const RSA* pstPublicKey, const char* pszFile)
  //  {
  //      FILE* fp = fopen(pszFile, "wb");
  //      if(fp == NULL)
  //          return false;

  //      UINT8 pBuf[65535] = {0};
  //      UINT8* p = pBuf;
  //      UINT32 nTotalLen = 0;

		//*(UINT32*)p = sizeof(BN_ULONG);
		//p += sizeof(UINT32);

  //      UINT32 nDataLen = SerializeBigNum(pstPublicKey->n, p);
  //      p += nDataLen;
  //      nTotalLen += nDataLen;

  //      nDataLen = SerializeBigNum(pstPublicKey->e, p);
  //      p += nDataLen;
  //      nTotalLen += nDataLen;

  //      nDataLen = SerializeBigNum(pstPublicKey->dmp1, p);
  //      p += nDataLen;
  //      nTotalLen += nDataLen;

  //      nDataLen = SerializeBigNum(pstPublicKey->dmq1, p);
  //      p += nDataLen;
  //      nTotalLen += nDataLen;

  //      if(fwrite(pBuf, sizeof(UINT8), nTotalLen, fp) < (size_t)nDataLen)
  //      {
  //          fclose(fp);
  //          return false;
  //      }
  //      fclose(fp);
  //      return true;
  //  }

  //  BOOL SavePrivateKey(const RSA* pstPrivateKey, const char* pszFile)
  //  {
  //      FILE* fp = fopen(pszFile, "wb");
  //      if(fp == NULL)
  //          return false;

  //      UINT8 pBuf[65535] = {0};
  //      UINT8* p = pBuf;
  //      UINT32 nTotalLen = 0;

		//*(UINT32*)p = sizeof(BN_ULONG);
		//p += sizeof(UINT32);

  //      UINT32 nDataLen = SerializeBigNum(pstPrivateKey->n, p);
  //      p += nDataLen;
  //      nTotalLen += nDataLen;

  //      nDataLen = SerializeBigNum(pstPrivateKey->e, p);
  //      p += nDataLen;
  //      nTotalLen += nDataLen;

  //      nDataLen = SerializeBigNum(pstPrivateKey->d, p);
  //      p += nDataLen;
  //      nTotalLen += nDataLen;

  //      nDataLen = SerializeBigNum(pstPrivateKey->p, p);
  //      p += nDataLen;
  //      nTotalLen += nDataLen;

  //      nDataLen = SerializeBigNum(pstPrivateKey->q, p);
  //      p += nDataLen;
  //      nTotalLen += nDataLen;

  //      nDataLen = SerializeBigNum(pstPrivateKey->dmp1, p);
  //      p += nDataLen;
  //      nTotalLen += nDataLen;

  //      nDataLen = SerializeBigNum(pstPrivateKey->dmq1, p);
  //      p += nDataLen;
  //      nTotalLen += nDataLen;

  //      if(fwrite(pBuf, sizeof(UINT8), nTotalLen, fp) < (size_t)nDataLen)
  //      {
  //          fclose(fp);
  //          return false;
  //      }
  //      fclose(fp);
  //      return true;
  //  }

#define BN2BIN(bignum)	\
		nDataLen = BN_bn2bin( bignum, p + sizeof(UINT32) ); \
		*(UINT32*)p = nDataLen; \
        p += ( nDataLen + sizeof(UINT32) ); \
        nTotalLen += ( nDataLen + sizeof(UINT32) ); 

    BOOL SavePublicKey(const RSA* pstPublicKey, const char* pszFile)
    {
        FILE* fp = fopen(pszFile, "wb");
        if(fp == NULL)
            return false;

        UINT8 pBuf[65535] = {0};
        UINT8* p = pBuf;
        UINT32 nTotalLen = 0;

		*(UINT32*)p = sizeof(BN_ULONG);
		p += sizeof(UINT32);


		UINT32 nDataLen = 0;

		BN2BIN( pstPublicKey->n );
		BN2BIN( pstPublicKey->e );
		BN2BIN( pstPublicKey->dmp1 );
		BN2BIN( pstPublicKey->dmq1 );

        if(fwrite(pBuf, sizeof(UINT8), nTotalLen, fp) < (size_t)nDataLen)
        {
            fclose(fp);
            return false;
        }
        fclose(fp);
        return true;
    }

    BOOL SavePrivateKey(const RSA* pstPrivateKey, const char* pszFile)
    {
        FILE* fp = fopen(pszFile, "wb");
        if(fp == NULL)
            return false;

        UINT8 pBuf[65535] = {0};
        UINT8* p = pBuf;
        UINT32 nTotalLen = 0;

		*(UINT32*)p = sizeof(BN_ULONG);
		p += sizeof(UINT32);

		UINT32 nDataLen = 0;

		BN2BIN( pstPrivateKey->n );
		BN2BIN( pstPrivateKey->e );
		BN2BIN( pstPrivateKey->d );
		BN2BIN( pstPrivateKey->p );
		BN2BIN( pstPrivateKey->q );
		BN2BIN( pstPrivateKey->dmp1 );
		BN2BIN( pstPrivateKey->dmq1 );

        if(fwrite(pBuf, sizeof(UINT8), nTotalLen, fp) < (size_t)nDataLen)
        {
            fclose(fp);
            return false;
        }
        fclose(fp);
        return true;
    }

    // 从文件获取公钥
    BOOL LoadPublicKey(RSA*& pstPublicKey, const CHAR* pszFile)
    {
        SDASSERT(NULL == pstPublicKey);

        FILE* fp = fopen(pszFile, "rb");
        if(NULL == fp)
        {
            return FALSE;
        }
        UINT8 pBuf[65535] = {0};
        UINT8* p = pBuf;
        fread(pBuf, sizeof(UINT8), sizeof(pBuf), fp);
        pstPublicKey = RSA_new();
        ReadBigNum(pstPublicKey->n, p);
        ReadBigNum(pstPublicKey->e, p);
        ReadBigNum(pstPublicKey->dmp1, p);
        ReadBigNum(pstPublicKey->dmq1, p);
        fclose(fp);
        return TRUE;
    }

//     BOOL LoadPrivateKey(RSA*& pstPrivateKey, const char *pszFile)
//     {
//         assert(pstPrivateKey == NULL);
// 
//         FILE* fp = fopen(pszFile, "rb");
//         if(fp == NULL)
//             return false;
//         UINT8 pBuf[65535] = {0};
//         UINT8* p = pBuf;
// 
// 		size_t uReadLen = fread(pBuf, sizeof(UINT8), sizeof(pBuf), fp);
// 
// 		if( uReadLen < 4 )
// 		{
// 			fclose(fp);
// 			return false;
// 		}
// 
// 		UINT32	uType = *(UINT32*)p;
// 		p += sizeof(UINT32);
// 
// 		//if( uType > sizeof(BN_ULONG) ) //32位程序不能读64位的密钥文件
// 		//{
// 		//	fclose(fp);
// 		//	return false;
// 		//}
// 
// 		LoadPrivateKeyFromMem( pstPrivateKey, p, uReadLen, uType );
// 		
//         fclose(fp);
// 
//         return true;
//     }


    BOOL LoadPrivateKey(RSA*& pstPrivateKey, const char *pszFile)
    {
        assert(pstPrivateKey == NULL);

        FILE* fp = fopen(pszFile, "rb");
        if(fp == NULL)
            return false;
        UINT8 pBuf[65535] = {0};
        UINT8* p = pBuf;

        fread(pBuf, sizeof(UINT8), sizeof(pBuf), fp);

        pstPrivateKey = RSA_new();

        ReadBigNum(pstPrivateKey->n, p);
        ReadBigNum(pstPrivateKey->e, p);
        ReadBigNum(pstPrivateKey->d, p);
        ReadBigNum(pstPrivateKey->p, p);
        ReadBigNum(pstPrivateKey->q, p);
        ReadBigNum(pstPrivateKey->dmp1, p);
        ReadBigNum(pstPrivateKey->dmq1, p);

        fclose(fp);

        return true;
    }

// #define BIN2BN(bignum)			\
// 		uLen = *((UINT32*)p);	\
// 		p += sizeof(UINT32);	\
// 		bignum = BN_bin2bn(p, uLen, bignum); \
// 		p += uLen;
// 
//     VOID LoadPublicKeyFromMem(RSA*& pstKey, UINT8* pbyKeyBuf, UINT16 wKeyLen, INT32 iElemSize)
//     {
//         assert(pstKey == NULL);
// 
//         UINT8* p = pbyKeyBuf;
//         pstKey = RSA_new();
// 
// 		UINT32	uLen = 0;
// 	
// 		BIN2BN( pstKey->n );
// 		BIN2BN( pstKey->e );
// 		BIN2BN( pstKey->dmp1 );
// 		BIN2BN( pstKey->dmq1 );
//     }

    // 从内存中载入公钥
    VOID LoadPublicKeyFromMem(RSA*& pstKey, UINT8* pbyKeyBuf, UINT16 wKeyLen)
    {
        SDASSERT(pstKey == NULL);

        UINT8* p = pbyKeyBuf;
        pstKey = RSA_new();
        ReadBigNum(pstKey->n, p);
        ReadBigNum(pstKey->e, p);
        ReadBigNum(pstKey->dmp1, p);
        ReadBigNum(pstKey->dmq1, p);
    }

//     VOID LoadPrivateKeyFromMem(RSA*& pstKey, UINT8* pbyKeyBuf, UINT16 wKeyLen, INT32 iElemSize)
//     {
//         assert(pstKey == NULL);
// 
//         UINT8* p = pbyKeyBuf;
//         pstKey = RSA_new();
// 
// 		UINT32	uLen = 0;
// 	
// 		BIN2BN( pstKey->n );
// 		BIN2BN( pstKey->e );
// 		BIN2BN( pstKey->d );
// 		BIN2BN( pstKey->p );
// 		BIN2BN( pstKey->q );
// 		BIN2BN( pstKey->dmp1 );
// 		BIN2BN( pstKey->dmq1 );
//     }

    VOID LoadPrivateKeyFromMem(RSA*& pstKey, UINT8* pbyKeyBuf, UINT16 wKeyLen)
    {
        assert(pstKey == NULL);

        UINT8* p = pbyKeyBuf;
        pstKey = RSA_new();

        ReadBigNum(pstKey->n, p);
        ReadBigNum(pstKey->e, p);
        ReadBigNum(pstKey->d, p);
        ReadBigNum(pstKey->p, p);
        ReadBigNum(pstKey->q, p);
        ReadBigNum(pstKey->dmp1, p);
        ReadBigNum(pstKey->dmq1, p);
    }


    //VOID LoadPublicKeyFromMem(RSA*& pstKey, UINT8* pbyKeyBuf, UINT16 wKeyLen, INT32 iElemSize)
    //{
    //    assert(pstKey == NULL);

    //    UINT8* p = pbyKeyBuf;
    //    pstKey = RSA_new();

    //    ReadBigNum(pstKey->n, p, iElemSize);
    //    ReadBigNum(pstKey->e, p, iElemSize);
    //    ReadBigNum(pstKey->dmp1, p, iElemSize);
    //    ReadBigNum(pstKey->dmq1, p, iElemSize);
    //}

    //VOID LoadPrivateKeyFromMem(RSA*& pstKey, UINT8* pbyKeyBuf, UINT16 wKeyLen, INT32 iElemSize)
    //{
    //    assert(pstKey == NULL);

    //    UINT8* p = pbyKeyBuf;
    //    pstKey = RSA_new();

    //    ReadBigNum(pstKey->n, p, iElemSize);
    //    ReadBigNum(pstKey->e, p, iElemSize);
    //    ReadBigNum(pstKey->d, p, iElemSize);
    //    ReadBigNum(pstKey->p, p, iElemSize);
    //    ReadBigNum(pstKey->q, p, iElemSize);
    //    ReadBigNum(pstKey->dmp1, p, iElemSize);
    //    ReadBigNum(pstKey->dmq1, p, iElemSize);
    //}

    BIGNUM * NewBigNum()
    {
        BIGNUM *pstRet = SDNew BIGNUM;
        pstRet->d = NULL;
        pstRet->top = 0;
        pstRet->dmax = 0;
        pstRet->neg = 0;
        pstRet->flags = 1;

        return pstRet;
    }

    VOID FreeBigNum(BIGNUM*& pBigNum)
    {
        assert(pBigNum != NULL);
        if(pBigNum->d != NULL)
            delete[] pBigNum->d;
        pBigNum->d = NULL;

        delete pBigNum;
        pBigNum = NULL;
    }

    VOID CopyBigNum(BIGNUM *pstTo, const BIGNUM *pstFrom)
    {
        pstTo->d = SDNew BN_ULONG[pstFrom->dmax];
        memset(pstTo->d, 0, sizeof(BN_ULONG) * pstFrom->dmax);
        memcpy(pstTo->d, pstFrom->d, sizeof(BN_ULONG)*pstFrom->top);
        pstTo->top = pstFrom->top;
        pstTo->dmax = pstFrom->dmax;
        pstTo->neg = pstFrom->neg;
        pstTo->flags = pstFrom->flags;
    }

    UINT32 SerializeBigNum(const BIGNUM* pKey, UINT8* pBuf)
    {
        memset(pBuf, 0, sizeof(pBuf));

        UINT8* p = pBuf;

        *(INT32*)p = pKey->dmax;
        p += sizeof(INT32);

        memcpy(p, pKey->d, pKey->dmax * sizeof(BN_ULONG));
        p += pKey->dmax * sizeof(BN_ULONG);

        *(INT32*)p = pKey->top;
        p += sizeof(INT32);

        *(INT32*)p = pKey->dmax;
        p += sizeof(INT32);

        *(INT32*)p = pKey->neg;
        p += sizeof(INT32);

        *(INT32*)p = pKey->flags;
        p += sizeof(INT32);

        return (p-pBuf);
    }

    VOID ReadBigNum(BIGNUM*& pBigNum, UINT8*& p)
    {
        assert(pBigNum == NULL);

        pBigNum = NewBigNum();

        UINT32 nBigNumSize = *(UINT32*)p;
        p += sizeof(UINT32);

        pBigNum->d = SDNew BN_ULONG[nBigNumSize];
        memset(pBigNum->d, 0, sizeof(BN_ULONG) * nBigNumSize);


        memcpy(pBigNum->d, p, nBigNumSize * sizeof(BN_ULONG));
        p += nBigNumSize * sizeof(BN_ULONG);

        pBigNum->top = *(INT32*)p;
        p += sizeof(INT32);

        pBigNum->dmax = *(INT32*)p;
        p += sizeof(INT32);

        pBigNum->neg = *(INT32*)p;
        p += sizeof(INT32);

        pBigNum->flags = *(INT32*)p;
        p += sizeof(INT32);
    }

    VOID ReadBigNum(BIGNUM*& pBigNum, UINT8*& p, INT32 iNumSize )
    {
        assert(pBigNum == NULL);

        pBigNum = NewBigNum();

		if( iNumSize != 8 ) iNumSize = 4;

        UINT32 nBigNumSize = *(UINT32*)p;
        p += sizeof(UINT32);

        pBigNum->d = SDNew BN_ULONG[nBigNumSize];
        memset(pBigNum->d, 0, sizeof(BN_ULONG) * nBigNumSize);

		if( iNumSize == 4 )
		{
			UINT32* arrData = SDNew UINT32[nBigNumSize];
			memset( arrData, 0, sizeof(UINT32) * nBigNumSize );
			memcpy( arrData, p, nBigNumSize * 4 );
			for( INT32 i = 0; i < (INT32)nBigNumSize; i ++ )
			{
				pBigNum->d[i] = arrData[i];
			}
			p += nBigNumSize * 4;
			delete [] arrData;
		}
		else
		{
			UINT64* arrData = SDNew UINT64[nBigNumSize];
			memset( arrData, 0, sizeof(UINT64) * nBigNumSize );
			memcpy( arrData, p, nBigNumSize * 8 );
			for( INT32 i = 0; i < (INT32)nBigNumSize; i ++ )
			{
				pBigNum->d[i] = (UINT32)arrData[i];
			}
			p += nBigNumSize * 8;
			delete [] arrData;
		}

        pBigNum->top = *(INT32*)p;
        p += sizeof(INT32);

        pBigNum->dmax = *(INT32*)p;
        p += sizeof(INT32);

        pBigNum->neg = *(INT32*)p;
        p += sizeof(INT32);

        pBigNum->flags = *(INT32*)p;
        p += sizeof(INT32);
    }

    UINT32 PubEncrypt(UINT8* from, UINT32 nFromSize, UINT8* to, RSA* pPubKey)
    {
        UINT32 nRSA_size = RSA_size(pPubKey);
        UINT32 nResultBytes = 0;
        UINT32 nRemain = (UINT32)nFromSize;
        UINT8* pCurPos = from;

        do
        {
            UINT32 nCurSrcSize = nRSA_size - 16 < nRemain ? nRSA_size-16 : nRemain;
            UINT32 nCurEnc = RSA_public_encrypt(nCurSrcSize, pCurPos, to+nResultBytes, pPubKey, RSA_PKCS1_PADDING);
            pCurPos += nCurSrcSize;
            nRemain -= nCurSrcSize;
            nResultBytes += nCurEnc;
        }
        while(nRemain > 0);

        return nResultBytes;


        //	INT32 nStep1 = RSA_public_encrypt(nRSA_size, from, to, pPubKey, RSA_NO_PADDING);
        //	INT32 nStep2 = RSA_public_encrypt(nRSA_size, from + nStep1, to + nStep1, pPubKey, RSA_NO_PADDING);

    }
    UINT32 PriDecrypt(UINT8* from, UINT32 nFromSize, UINT8* to, RSA* pPriKey)
    {
        UINT32 nRSA_size = RSA_size(pPriKey);
        UINT32 nResultBytes = 0;
        UINT32 nRemain = (UINT32)nFromSize;
        UINT8* pCurPos = from;

        do
        {
            UINT32 nCurSrcSize = nRSA_size < nRemain ? nRSA_size : nRemain;
            UINT32 nCurEnc = RSA_private_decrypt(nCurSrcSize, pCurPos, to+nResultBytes, pPriKey, RSA_PKCS1_PADDING);
            pCurPos += nCurSrcSize;
            nRemain -= nCurSrcSize;
            nResultBytes += nCurEnc;
        }
        while(nRemain > 0);

        return nResultBytes;
    }

	VOID FreeCrypto()
	{
		CRYPTO_cleanup_all_ex_data();
	}
}
#endif // #if SDU_WITH_OPENSSL 

