#include "sddes.h"
#include "sdcryptodes.h"
#include <openssl/des.h>

namespace SGDP
{
    VOID CSDDes::DESGenKey( SDESBlock* pIV )
    {
        DESRandomKey( pIV );
    }

    VOID CSDDes::DESGenKey( SDES3Block* pIV )
    {
        GenDES3Key( pIV );
    }

    VOID CSDDes::DESGenEncKeySche( SDESContext *pCtx, SDESBlock& oKey )
    {
        DESSetEncKey( pCtx, oKey.acBlock );
    }

    VOID CSDDes::DESGenEncKeySche( SDES3Context *pCtx, SDES3Block& oKey )
    {
        DES3SetEncKey( pCtx, &oKey );
    }


    VOID CSDDes::DESGenDecKeySche( SDESContext *pCtx, SDESBlock& oKey )
    {
        DESSetDecKey( pCtx, oKey.acBlock );
    }

    VOID CSDDes::DESGenDecKeySche( SDES3Context *pCtx, SDES3Block& oKey )
    {
        DES3SetDecKey( pCtx, &oKey );
    }

    VOID CSDDes::DESEncryptNCBC( SDESContext    *pCtx,
                                 SDESBlock		 *pIV,
                                 INT32		 nLength,
                                 unsigned char *pInput,
                                 unsigned char *pOutput )
    {
        SDESBlock IVBak;
        memcpy(&IVBak, (const VOID*)pIV, sizeof(SDESBlock));
        DESCryptCBC( pCtx, DES_ENCRYPT, nLength, pIV, pInput, pOutput );
        memcpy(pIV, (const VOID*)&IVBak, sizeof(SDESBlock));
    }

    VOID CSDDes::DESEncryptNCBC( SDES3Context   *pCtx,
                                 SDES3Block	 *pIV,
                                 INT32		 nLength,
                                 unsigned char *pInput,
                                 unsigned char *pOutput )
    {
        SDES3Block IVBak;
        memcpy(&IVBak, (const VOID*)pIV, sizeof(SDES3Block));
        DES3CryptCBC( pCtx, DES_ENCRYPT, nLength, pIV, pInput, pOutput );
        memcpy(pIV, (const VOID*)&IVBak, sizeof(SDES3Block));
    }

    VOID CSDDes::DESDecryptNCBC( SDESContext	 *pCtx,
                                 SDESBlock		 *pIV,
                                 INT32		 nLength,
                                 unsigned char *pInput,
                                 unsigned char *pOutput )
    {
        SDESBlock IVBak;
        memcpy(&IVBak, (const VOID*)pIV, sizeof(SDESBlock));
        DESCryptCBC( pCtx, DES_DECRYPT, nLength, pIV, pInput, pOutput );
        memcpy(pIV, (const VOID*)&IVBak, sizeof(SDESBlock));
    }

    VOID CSDDes::DESDecryptNCBC( SDES3Context	 *pCtx,
                                 SDES3Block	 *pIV,
                                 INT32		 nLength,
                                 unsigned char *pInput,
                                 unsigned char *pOutput )
    {
        SDES3Block IVBak;
        memcpy(&IVBak, (const VOID*)pIV, sizeof(SDES3Block));
        DES3CryptCBC( pCtx, DES_DECRYPT, nLength, pIV, pInput, pOutput );
        memcpy(pIV, (const VOID*)&IVBak, sizeof(SDES3Block));
    }

}


