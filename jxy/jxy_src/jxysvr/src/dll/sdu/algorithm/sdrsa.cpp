#include "sdrsa.h"
#include "sdcryptorsa.h"

//#ifndef WIN64 

namespace SGDP
{

////////////////////////////////////////////////////////////////////////////////
// typedef check return macro
#define BN_SERIAL_CHK(f)  if ( ( nRet = f ) != 0 ) \
	{						   \
	goto ERROR_HANDLE;	   \
}


////////////////////////////////////////////////////////////////////////////////

    static int RandWrapper( VOID *rng_state )
    {
        if( rng_state != NULL )
            rng_state  = NULL;

        srand((unsigned)time(NULL) + rand());
        return( rand() );
    }

////////////////////////////////////////////////////////////////////////////////

    VOID CSDRsa::RSAContextInit( SRSAContext *pCtx )
    {
        RSAInit( pCtx, 0, RSA_PKCS_V15, RandWrapper, NULL );
    }

    VOID CSDRsa::RSAContextFree( SRSAContext *pCtx )
    {
        RSAFree(pCtx);
    }

    INT32 CSDRsa::RSANSize(SRSAContext *pCtx)
    {
        return pCtx->len;
    }

    SRSAContext*  CSDRsa::RSANew()
    {
        SRSAContext* pRSAContext = ( SRSAContext* )malloc( sizeof(SRSAContext) );
        if ( NULL != pRSAContext )
        {
            RSAContextInit( pRSAContext );
            return pRSAContext;
        }
        else
        {
            return NULL;
        }
    }

    BOOL  CSDRsa::RSAGenKeyPair( SRSAContext *pCtx, INT32 nBits, INT32 nExponent )
    {
        INT32 nRet = RSAGenKey( pCtx, nBits, nExponent );
        if ( 0 == nRet )
        {
            return TRUE;
        }
        else
        {
            // write logger here!nRet was the error code
            return FALSE;
        }
    }

    INT32 CSDRsa::RSAGetPubKeyLen(SRSAContext *pPubKey)
    {
        return RSAGetPubKeyLenSub(pPubKey) + 3*sizeof(INT32);
    }

    INT32 CSDRsa::RSAGetPubKeyLenSub(SRSAContext *pPubKey)
    {
        INT32 nLen = BNGetTotalBytes( &(pPubKey->N) );
        nLen += BNGetTotalBytes( &(pPubKey->E) );
        return nLen;
    }

    INT32  CSDRsa::RSAGetPrivKeyLen(SRSAContext* pPrivKey)
    {
        return RSAGetPrivKeyLenSub(pPrivKey) + 9*sizeof(INT32);
    }

    INT32 CSDRsa::RSAGetPrivKeyLenSub(SRSAContext *pPrivKey)
    {
        INT32 nLen = BNGetTotalBytes( &(pPrivKey->N) );
        nLen += BNGetTotalBytes( &(pPrivKey->E) );
        nLen += BNGetTotalBytes( &(pPrivKey->D) );
        nLen += BNGetTotalBytes( &(pPrivKey->P) );
        nLen += BNGetTotalBytes( &(pPrivKey->Q) );
        nLen += BNGetTotalBytes( &(pPrivKey->DP) );
        nLen += BNGetTotalBytes( &(pPrivKey->DQ) );
        nLen += BNGetTotalBytes( &(pPrivKey->QP) );
        return nLen;
    }

    BOOL CSDRsa::RSAReadPubKeySub(SRSAContext* pPubKey, UCHAR* pBuf, INT32 nRet)
    {
        INT32 nLenBNUM = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNReadBinary(&pPubKey->N, pBuf, nLenBNUM) );
        pBuf += nLenBNUM;

        nLenBNUM = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNReadBinary(&pPubKey->E, pBuf, nLenBNUM) );
        pBuf += nLenBNUM;

        pPubKey->len = ( BNGetMSB( &pPubKey->N ) + 7 ) >> 3;

        return true;

ERROR_HANDLE:
        nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
        return  false;
    }

    BOOL CSDRsa::RSAReadPrivKeySub(SRSAContext* pPrivKey, UCHAR* pBuf, INT32 nRet)
    {
        INT32 nLenBNUM = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNReadBinary(&pPrivKey->N, pBuf, nLenBNUM) );
        pBuf += nLenBNUM;

        nLenBNUM = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNReadBinary(&pPrivKey->E, pBuf, nLenBNUM) );
        pBuf += nLenBNUM;

        nLenBNUM = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNReadBinary(&pPrivKey->D, pBuf, nLenBNUM) );
        pBuf += nLenBNUM;

        nLenBNUM = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNReadBinary(&pPrivKey->P, pBuf, nLenBNUM) );
        pBuf += nLenBNUM;

        nLenBNUM = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNReadBinary(&pPrivKey->Q, pBuf, nLenBNUM) );
        pBuf += nLenBNUM;

        nLenBNUM = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNReadBinary(&pPrivKey->DP, pBuf, nLenBNUM) );
        pBuf += nLenBNUM;

        nLenBNUM = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNReadBinary(&pPrivKey->DQ, pBuf, nLenBNUM) );
        pBuf += nLenBNUM;

        nLenBNUM = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNReadBinary(&pPrivKey->QP, pBuf, nLenBNUM) );
        pBuf += nLenBNUM;

        pPrivKey->len = ( BNGetMSB( &pPrivKey->N ) + 7 ) >> 3;

        return true;

ERROR_HANDLE:
        nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
        return  false;
    }

    BOOL CSDRsa::RSAWritePubKeySub(SRSAContext* pPubKey, UCHAR* pBuf, INT32 nRet)
    {
        INT32 nHeaderLen = RSAGetPubKeyLenSub(pPubKey) + 3*sizeof(INT32);
        memcpy(pBuf, &nHeaderLen, sizeof(INT32));
        pBuf += sizeof(INT32);

        INT32 nLenBNUM = BNGetTotalBytes(&pPubKey->N);
        memcpy(pBuf, &nLenBNUM, sizeof(INT32));
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNWriteBinary( &(pPubKey->N), pBuf, nLenBNUM) );
        pBuf += nLenBNUM;

        nLenBNUM = BNGetTotalBytes(&pPubKey->E);
        memcpy(pBuf, &nLenBNUM, sizeof(INT32));
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNWriteBinary( &(pPubKey->E), pBuf, nLenBNUM));
        pBuf += nLenBNUM;

        return true;

ERROR_HANDLE:
        nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
        return  false;
    }

    BOOL CSDRsa::RSAWritePrivKeySub(SRSAContext* pPrivKey, UCHAR* pBuf, INT32 nRet)
    {
        INT32 nHeaderLen = RSAGetPrivKeyLenSub(pPrivKey) + 9*sizeof(INT32);
        memcpy(pBuf, &nHeaderLen, sizeof(INT32));
        pBuf += sizeof(INT32);

        INT32 nLenBNUM = BNGetTotalBytes(&pPrivKey->N);
        memcpy(pBuf, &nLenBNUM, sizeof(INT32));
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNWriteBinary( &(pPrivKey->N), pBuf, nLenBNUM) );
        pBuf += nLenBNUM;

        nLenBNUM = BNGetTotalBytes(&pPrivKey->E);
        memcpy(pBuf, &nLenBNUM, sizeof(INT32));
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNWriteBinary( &(pPrivKey->E), pBuf, nLenBNUM));
        pBuf += nLenBNUM;

        nLenBNUM = BNGetTotalBytes(&pPrivKey->D);
        memcpy(pBuf, &nLenBNUM, sizeof(INT32));
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNWriteBinary( &(pPrivKey->D), pBuf, nLenBNUM));
        pBuf += nLenBNUM;

        nLenBNUM = BNGetTotalBytes(&pPrivKey->P);
        memcpy(pBuf, &nLenBNUM, sizeof(INT32));
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNWriteBinary( &(pPrivKey->P), pBuf, nLenBNUM));
        pBuf += nLenBNUM;

        nLenBNUM = BNGetTotalBytes(&pPrivKey->Q);
        memcpy(pBuf, &nLenBNUM, sizeof(INT32));
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNWriteBinary( &(pPrivKey->Q), pBuf, nLenBNUM));
        pBuf += nLenBNUM;

        nLenBNUM = BNGetTotalBytes(&pPrivKey->DP);
        memcpy(pBuf, &nLenBNUM, sizeof(INT32));
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNWriteBinary( &(pPrivKey->DP), pBuf, nLenBNUM));
        pBuf += nLenBNUM;

        nLenBNUM = BNGetTotalBytes(&pPrivKey->DQ);
        memcpy(pBuf, &nLenBNUM, sizeof(INT32));
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNWriteBinary( &(pPrivKey->DQ), pBuf, nLenBNUM));
        pBuf += nLenBNUM;

        nLenBNUM = BNGetTotalBytes(&pPrivKey->QP);
        memcpy(pBuf, &nLenBNUM, sizeof(INT32));
        pBuf += sizeof(INT32);
        BN_SERIAL_CHK( BNWriteBinary( &(pPrivKey->QP), pBuf, nLenBNUM));
        pBuf += nLenBNUM;

        return true;

ERROR_HANDLE:
        nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
        return  false;
    }

    BOOL CSDRsa::RSASavePubKey(SRSAContext *pPubKey, const char *pszFile)
    {
        INT32 nRet = -1;
        UCHAR acBuf[2048] = {0};
        UCHAR* pBuf = acBuf;

        INT32 nHeaderLen = 0;

        FILE* fp = fopen( pszFile, "wb" );
        if ( fp == NULL )
        {
            nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
            goto ERROR_HANDLE;
        }

        nHeaderLen = RSAGetPubKeyLenSub(pPubKey) + 3*sizeof(INT32);

        if(!RSAWritePubKeySub(pPubKey, pBuf, nRet))
            goto ERROR_HANDLE;

        if(fwrite(acBuf, 1, nHeaderLen, fp) != nHeaderLen )
        {
            nRet = SDCRYPTO_BNERR_FILE_IO_ERROR;
            goto ERROR_HANDLE;
        }

        fclose( fp );
        return true;

ERROR_HANDLE:
        fclose( fp );
        // write logger here!nRet was the error code
        return false;
    }

    BOOL CSDRsa::RSASavePubKeyToMem(SRSAContext *pPubKey, UCHAR* pBuffer, INT32 nBufLen)
    {
        if (NULL ==  pBuffer || NULL == pPubKey ||
                nBufLen < RSAGetPubKeyLen(pPubKey))
        {
            return false;
        }

        INT32 nRet = -1;
        return RSAWritePubKeySub(pPubKey, pBuffer, nRet);
    }


    BOOL CSDRsa::RSALoadPubKey(SRSAContext *&pPubKey, const char *pszFile)
    {
        INT32 nRet = -1;

        INT32 nHeaderLen = 0;

        UCHAR  keyBuf[2048] = {0};
        UCHAR* pBuf = keyBuf;

        INT32 nFileLen = 0;

        FILE* fp = fopen( pszFile, "rb" );
        if (fp == NULL)
        {
            nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
            goto ERROR_HANDLE;
        }

        pPubKey = RSANew();



        fseek(fp, 0, SEEK_END);
        nFileLen = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        if(fread(pBuf, 1, nFileLen, fp) != nFileLen)
        {
            nRet = SDCRYPTO_BNERR_FILE_IO_ERROR;
            goto ERROR_HANDLE;
        }

        fclose( fp );

        nHeaderLen = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        if(nFileLen != nHeaderLen)
        {
            nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
            goto ERROR_HANDLE;
        }

        if(!RSAReadPubKeySub(pPubKey, pBuf, nRet))
            goto ERROR_HANDLE;

        return true;

ERROR_HANDLE:
        if (NULL != fp)
        {
            fclose( fp );
        }
        // write logger here!nRet was the error code
        return false;
    }

    BOOL CSDRsa::RSALoadPubKeyFromMem(SRSAContext *&pPubKey, const UCHAR* pBuffer, INT32 nBufLen)
    {
        INT32 nRet = -1;

        UCHAR* pBuf = (UCHAR*)pBuffer;

        INT32 nHeaderLen = 0;

        if( pBuffer == NULL )
        {
            nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
            goto ERROR_HANDLE;
        }

        pPubKey = RSANew();

        nHeaderLen = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        if(nBufLen < nHeaderLen)
        {
            nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
            goto ERROR_HANDLE;
        }

        if(!RSAReadPubKeySub(pPubKey, pBuf, nRet))
            goto ERROR_HANDLE;

        return true;

ERROR_HANDLE:
        return false;
    }


    BOOL CSDRsa::RSASavePrivKey(SRSAContext *pPrivKey, const char *pszFile)
    {
        FILE* fp = fopen( pszFile, "wb" );
        if (fp == NULL)
        {
            return false;
        }

        INT32 nRet = -1;

        UCHAR acBuf[2048] = {0};
        UCHAR* pBuf = acBuf;

        INT32 nHeaderLen = RSAGetPrivKeyLenSub(pPrivKey) + 9*sizeof(INT32);

        if(!RSAWritePrivKeySub(pPrivKey, pBuf, nRet))
        {
            goto ERROR_HANDLE;
        }

        if(fwrite(acBuf, 1, nHeaderLen, fp) != nHeaderLen )
        {
            nRet = SDCRYPTO_BNERR_FILE_IO_ERROR;
            goto ERROR_HANDLE;
        }

        fclose( fp );
        return true;


ERROR_HANDLE:

        fclose(fp);
        // write logger here!nRet was the error code

        return false;
    }

    BOOL CSDRsa::RSASavePrivKeyToMem(SRSAContext *pPrivKey, UCHAR* pBuffer, INT32 nBufLen)
    {
        INT32 nRet = -1;
        INT32 nHeaderLen = 0;
        UCHAR* pBuf = pBuffer;

        if( pBuffer == NULL )
        {
            nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
            goto ERROR_HANDLE;
        }

        pPrivKey = RSANew();

        nHeaderLen = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        if(nBufLen < nHeaderLen)
        {
            nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
            goto ERROR_HANDLE;
        }

        if(!RSAReadPrivKeySub(pPrivKey, pBuf, nRet))
            goto ERROR_HANDLE;

        return true;

ERROR_HANDLE:
        return false;
    }

    BOOL CSDRsa::RSALoadPrivKey(SRSAContext *&pPrivKey, const char *pszFile)
    {
        INT32 nRet = -1;

        FILE* fp = fopen( pszFile, "rb" );
        if (fp == NULL)
        {
            return false;
        }

        pPrivKey = RSANew();

        UCHAR keyBuf[2048] = {0};
        UCHAR* pBuf = keyBuf;

        INT32 nHeaderLen = 0;

        fseek(fp, 0, SEEK_END);
        INT32 nFileLen = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        if(fread(pBuf, 1, nFileLen, fp) != nFileLen)
        {
            nRet = SDCRYPTO_BNERR_FILE_IO_ERROR;
            fclose(fp);
            goto ERROR_HANDLE;
        }

        fclose( fp );

        nHeaderLen = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        if(nFileLen != nHeaderLen)
        {
            nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
            goto ERROR_HANDLE;
        }

        RSAReadPrivKeySub(pPrivKey, pBuf, nRet);

        return true;
ERROR_HANDLE:
        fclose( fp );
        // write logger here!nRet was the error code

        return false;
    }

    BOOL CSDRsa::RSALoadPrivKeyFromMem(SRSAContext *&pPrivKey, const UCHAR* pBuffer, INT32 nBufLen)
    {
        INT32 nRet = -1;

        UCHAR* pBuf = (UCHAR*)pBuffer;
        INT32 nHeaderLen = 0;

        if( pBuffer == NULL )
        {
            nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
            goto ERROR_HANDLE;
        }

        pPrivKey = RSANew();

        nHeaderLen = *(INT32*)(pBuf);
        pBuf += sizeof(INT32);
        if(nBufLen < nHeaderLen)
        {
            nRet = SDCRYPTO_BNERR_BAD_INPUT_DATA;
            goto ERROR_HANDLE;
        }

        if(RSAReadPrivKeySub(pPrivKey, (UCHAR*)pBuf, nRet))
            goto ERROR_HANDLE;

ERROR_HANDLE:
        return false;
    }

    INT32  CSDRsa::RSAPubEncrypt( SRSAContext	*pCtx,
                                 INT32			nInLen,
                                 UCHAR *pInput,
                                 UCHAR *pOutput )
    {
        UINT32 nRemain = (UINT32)nInLen;
        UINT32 nNSize  = RSANSize(pCtx);
        UCHAR* pCurInPos  = pInput;
        UCHAR* pCurOutPos = pOutput;
        INT32 nOutLen = 0;

        do
        {
            UINT32 nCurEncSize = ( nNSize - 16 ) < nRemain ? ( nNSize - 16 ) : nRemain;

            INT32 nRet = RSAPkcs1Encrypt( pCtx, RSA_PUBLIC, nCurEncSize, pCurInPos, pCurOutPos );
            if ( 0 != nRet )
            {
                // write logger here! nRet was the error code
                return -1;
            }
            else
            {
                nRemain	   -= nCurEncSize;
                pCurInPos  += nCurEncSize;
                pCurOutPos += nNSize;
                nOutLen    += nNSize;
            }
        }
        while ( nRemain > 0 );

        return nOutLen;
    }

    INT32  CSDRsa::RSAPrivDecrypt( SRSAContext	   *pCtx,
                                  INT32         nInLen,
                                  UCHAR *pInput,
                                  UCHAR *pOutput )
    {
        UINT32 nRemain = (UINT32)nInLen;
        UINT32 nNSize = RSANSize(pCtx);
        UCHAR* pCurInPos  = pInput;
        UCHAR* pCurOutPos = pOutput;

        INT32 nOutLenSingle = 0;
        INT32 nOutLenTotal  = 0;
        do
        {
            UINT32 nCurDecSize = nNSize < nRemain ? nNSize : nRemain;

            INT32 nRet = RSAPkcs1Decrypt( pCtx, RSA_PRIVATE, &nOutLenSingle, pCurInPos, pCurOutPos);
            if ( 0 != nRet )
            {
                // write logger here! nRet was the error code
                return -1;
            }
            else
            {
                nRemain      -= nCurDecSize;
                pCurInPos    += nCurDecSize;
                pCurOutPos   += nOutLenSingle;
                nOutLenTotal += nOutLenSingle;
            }
        }
        while(  nRemain > 0 );

        return nOutLenTotal;
    }

}
//#endif //#ifndef WIN64 

