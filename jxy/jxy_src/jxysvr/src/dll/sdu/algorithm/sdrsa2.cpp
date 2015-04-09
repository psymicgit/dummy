#include <string.h>
#include <assert.h>
#include "sddebug.h"
#include "algorithm/sdrsa2.h"
#if SDU_WITH_OPENSSL
#include <openssl/rsa.h>
#include <openssl/objects.h>

static BIGNUM* NewBigNum();
static VOID FreeBigNum(BIGNUM* &pBigNum);
static VOID CopyBigNum(BIGNUM* pstTo, const BIGNUM* pstFrom);
static UINT32 SerializeBigNum(const BIGNUM* pKey, UINT8* pBuf);
static VOID ReadBigNum(BIGNUM* &pBigNum, UINT8* &p);

namespace SGDP
{
    BOOL  GenerateRsaKey(INT32 nBit, RSAKEY *pPublicKey, RSAKEY *pPrivateKey)
    {
		RSA* pstPublicKey = RSA_new();
		RSA* pstPrivateKey = RSA_new();
		*pPublicKey = pstPublicKey;
		*pPrivateKey = pstPrivateKey;

        RSA* pstRsa = RSA_generate_key(nBit, 65537, NULL, NULL);
        if(NULL == pstRsa)
		{
            return FALSE;
		}

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

        return TRUE;
    }

    VOID  FreePrivateKey(RSAKEY *pPrivateKey)
    {
		assert(*pPrivateKey != NULL);
		RSA* pstPrivateKey = (RSA*)*pPrivateKey;
        FreeBigNum(pstPrivateKey->n);
        FreeBigNum(pstPrivateKey->e);
        FreeBigNum(pstPrivateKey->d);
        FreeBigNum(pstPrivateKey->p);
        FreeBigNum(pstPrivateKey->q);
        FreeBigNum(pstPrivateKey->dmp1);
        FreeBigNum(pstPrivateKey->dmq1);

        RSA_free(pstPrivateKey);

        pstPrivateKey = NULL;
		*pPrivateKey = NULL;
    }

    VOID  FreePublicKey(RSAKEY *pPublicKey)
    {
		assert(*pPublicKey != NULL);
		RSA* pstPublicKey = (RSA*)*pPublicKey;

        FreeBigNum(pstPublicKey->n);
        FreeBigNum(pstPublicKey->e);
        FreeBigNum(pstPublicKey->dmp1);
        FreeBigNum(pstPublicKey->dmq1);

        RSA_free(pstPublicKey);

        pstPublicKey = NULL;
		*pPublicKey = NULL;
    }

    BOOL  SavePublicKey(const RSAKEY PublicKey, const char* pszFile)
    {
		assert(PublicKey != NULL);
		RSA* pstPublicKey = (RSA*)PublicKey;

        FILE* fp = fopen(pszFile, "wb");
        if(fp == NULL)
		{
            return FALSE;
		}

        UINT8 pBuf[65535] = {0};
        UINT8* p = pBuf;
        UINT32 nTotalLen = 0;

        UINT32 nDataLen = SerializeBigNum(pstPublicKey->n, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPublicKey->e, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPublicKey->dmp1, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPublicKey->dmq1, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        if(fwrite(pBuf, sizeof(UINT8), nTotalLen, fp) < (size_t)nDataLen)
        {
            fclose(fp);
            return FALSE;
        }
        fclose(fp);
        return TRUE;
    }

    BOOL  SavePrivateKey(const RSAKEY PrivateKey, const char* pszFile)
    {
		assert(PrivateKey != NULL);
		RSA* pstPrivateKey = (RSA*)PrivateKey;
        FILE* fp = fopen(pszFile, "wb");
        if(fp == NULL)
		{
            return FALSE;
		}

        UINT8 pBuf[65535] = {0};
        UINT8* p = pBuf;
        UINT32 nTotalLen = 0;

        UINT32 nDataLen = SerializeBigNum(pstPrivateKey->n, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPrivateKey->e, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPrivateKey->d, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPrivateKey->p, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPrivateKey->q, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPrivateKey->dmp1, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPrivateKey->dmq1, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        if(fwrite(pBuf, sizeof(UINT8), nTotalLen, fp) < (size_t)nDataLen)
        {
            fclose(fp);
            return FALSE;
        }
        fclose(fp);
        return TRUE;
    }

    BOOL  LoadPublicKey(RSAKEY PublicKey, const char* pszFile)
    {
        assert(PublicKey != NULL);
		RSA* pstPublicKey = (RSA*)PublicKey;

        FILE* fp = fopen(pszFile, "rb");
        if(fp == NULL)
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

    BOOL  LoadPrivateKey(RSAKEY PrivateKey, const char *pszFile)
    {
        assert(PrivateKey != NULL);
		RSA* pstPrivateKey = (RSA*)PrivateKey;

        FILE* fp = fopen(pszFile, "rb");
        if(fp == NULL)
		{
            return FALSE;
		}
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

        return TRUE;
    }

	UINT16  SavePublicKeyToMem(const RSAKEY PublicKey, UINT8* pbyKeyBuf, UINT16 wKeyLen)
    {
		assert(PublicKey != NULL);
		RSA* pstPublicKey = (RSA*)PublicKey;

        UINT8* p = pbyKeyBuf;
        UINT32 nTotalLen = 0;

        UINT32 nDataLen = SerializeBigNum(pstPublicKey->n, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPublicKey->e, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPublicKey->dmp1, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPublicKey->dmq1, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        return nTotalLen;
    }

    UINT16  SavePrivateKeyToMem(const RSAKEY PrivateKey, UINT8* pbyKeyBuf, UINT16 wKeyLen)
    {
		assert(PrivateKey != NULL);
		RSA* pstPrivateKey = (RSA*)PrivateKey;

        UINT8* p = pbyKeyBuf;
        UINT32 nTotalLen = 0;

        UINT32 nDataLen = SerializeBigNum(pstPrivateKey->n, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPrivateKey->e, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPrivateKey->d, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPrivateKey->p, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPrivateKey->q, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPrivateKey->dmp1, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        nDataLen = SerializeBigNum(pstPrivateKey->dmq1, p);
        p += nDataLen;
        nTotalLen += nDataLen;

        return nTotalLen;
    }

    VOID  LoadPublicKeyFromMem(RSAKEY PublicKey, UINT8* pbyKeyBuf, UINT16 wKeyLen)
    {
        assert(PublicKey != NULL);
		RSA* pstPublicKey = (RSA*)PublicKey;

        UINT8* p = pbyKeyBuf;
        pstPublicKey = RSA_new();

        ReadBigNum(pstPublicKey->n, p);
        ReadBigNum(pstPublicKey->e, p);
        ReadBigNum(pstPublicKey->dmp1, p);
        ReadBigNum(pstPublicKey->dmq1, p);
    }

    VOID  LoadPrivateKeyFromMem(RSAKEY PrivateKey, UINT8* pbyKeyBuf, UINT16 wKeyLen)
    {
        assert(PrivateKey != NULL);
		RSA* pstPrivateKey = (RSA*)PrivateKey;

        UINT8* p = pbyKeyBuf;
        pstPrivateKey = RSA_new();

        ReadBigNum(pstPrivateKey->n, p);
        ReadBigNum(pstPrivateKey->e, p);
        ReadBigNum(pstPrivateKey->d, p);
        ReadBigNum(pstPrivateKey->p, p);
        ReadBigNum(pstPrivateKey->q, p);
        ReadBigNum(pstPrivateKey->dmp1, p);
        ReadBigNum(pstPrivateKey->dmq1, p);
    }

    UINT32  PubEncrypt(RSAKEY PublicKey, UINT8* from, size_t nFromSize, UINT8* to)
    {
		assert(PublicKey != NULL);
		RSA* pPubKey = (RSA*)PublicKey;

        UINT32 nRSA_size = RSA_size(pPubKey);
        UINT32 nResultBytes = 0;
        UINT32 nRemain = (UINT32)nFromSize;
        UINT8* pCurPos = from;

        do
        {
            UINT32 nCurSrcSize = nRSA_size - 16 < nRemain ? nRSA_size - 16 : nRemain;
            UINT32 nCurEnc = RSA_public_encrypt(nCurSrcSize, pCurPos, to + nResultBytes, pPubKey, RSA_PKCS1_PADDING);
            pCurPos += nCurSrcSize;
            nRemain -= nCurSrcSize;
            nResultBytes += nCurEnc;
        }
        while(nRemain > 0);

        return nResultBytes;
    }

    UINT32  PriDecrypt(RSAKEY PrivateKey, UINT8* from, size_t nFromSize, UINT8* to)
    {
		assert(PrivateKey != NULL);
		RSA* pPriKey = (RSA*)PrivateKey;

        UINT32 nRSA_size = RSA_size(pPriKey);
        UINT32 nResultBytes = 0;
        UINT32 nRemain = (UINT32)nFromSize;
        UINT8* pCurPos = from;

        do
        {
            UINT32 nCurSrcSize = nRSA_size < nRemain ? nRSA_size : nRemain;
            UINT32 nCurEnc = RSA_private_decrypt(nCurSrcSize, pCurPos, to + nResultBytes, pPriKey, RSA_PKCS1_PADDING);
            pCurPos += nCurSrcSize;
            nRemain -= nCurSrcSize;
            nResultBytes += nCurEnc;
        }
        while(nRemain > 0);

        return nResultBytes;
    }

	BOOL  PriSign(RSAKEY PrivateKey, const UCHAR *msg, UINT32 msglen, UCHAR *sigret, UINT32 *siglen, INT32 type)
	{
		assert(PrivateKey != NULL);
		RSA* pPriKey = (RSA*)PrivateKey;

		//RSA_sign 这里如果使用此函数，可输入msglen不可超过36
		INT32 nRet = RSA_sign_ASN1_OCTET_STRING(type, msg, msglen, sigret, siglen, pPriKey);
		if(nRet != 1)
		{
			return FALSE;
		}
		return TRUE;
	}

	BOOL  PubVerify(RSAKEY PublicKey, const UCHAR *msg, UINT32 msglen, UCHAR *sigbuf, UINT32 siglen, INT32 type)
	{
		assert(PublicKey != NULL);
		RSA* pPubKey = (RSA*)PublicKey;

		//RSA_verify 这里如果使用此函数，可输入msglen不可超过36
		INT32 nRet = RSA_verify_ASN1_OCTET_STRING(type, msg, msglen, sigbuf, siglen, pPubKey);
		if(nRet != 1)
		{
			return FALSE;
		}
		return TRUE;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

BIGNUM * NewBigNum()
{
    BIGNUM * pstRet = SDNew BIGNUM;
	if(NULL == pstRet)
	{
		return NULL;
	}
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
	{
        delete[] pBigNum->d;
	}
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
	if(NULL == pKey)
	{
		return 0;
	}
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

#endif // #if SDU_WITH_OPENSSL 

