#ifndef SDCRYPTODES_H
#define SDCRYPTODES_H
/**
* @file sdcryptodes.h

* @brief ¥¶¿ÌDES±‡Ω‚¬Î
*
**/
#include "sddes.h"

namespace SGDP
{

	//
	// @brief          DES key schedule (56-bit, encryption)
	//
	// @param pCtx     DES context to be initialized
	// @param acKey    8-byte secret key
	//
	VOID DESSetEncKey( SDESContext *pCtx, UCHAR acKey[8] );

	//
	// @brief          DES key schedule (56-bit, decryption)
	//
	// @param pCtx     DES context to be initialized
	// @param acKey    8-byte secret key
	//
	VOID DESSetDecKey( SDESContext *pCtx, UCHAR acKey[8] );

	//
	// @brief		  DES initialization vector generator
	//
	//
	//
	VOID DESRandomKey( SDESBlock *pKey );

	//
	// @brief          DES-ECB block encryption/decryption
	//
	// @param pCtx     DES context
	// @param acInput  64-bit input block
	// @param acOutput 64-bit output block
	//
	VOID DESCryptECB( SDESContext	*pCtx,
		UCHAR acInput[8],
		UCHAR acOutput[8] );

	//
	// @brief          DES-CBC buffer encryption/decryption
	//
	// @param pCtx        DES context
	// @param nMode       DES_ENCRYPT or DES_DECRYPT
	// @param nLength     length of the input data
	// @param pIV         initialization vector (updated after use)
	// @param pacInput    buffer holding the input data
	// @param pacOutput   buffer holding the output data
	//
	VOID DESCryptCBC( SDESContext	*pCtx,
		INT32			nMode,
		INT32			nLength,
		SDESBlock		*pIV,
		UCHAR *pacInput,
		UCHAR *pacOutput );



	//
	// @brief		Generate 3-DES key
	// @param pKey	3DES key
	//
	VOID GenDES3Key( SDES3Block  *pKey );

	//
	// @brief		Generate 3-DES key schedule table for encryption
	// @param pCtx	3-DES Context which stored key schedule table
	// @param pKey  3-DES key
	//
	VOID DES3SetEncKey( SDES3Context *pCtx, SDES3Block *pKey );

	//
	// @brief		Generate 3-DES key schedule table for decryption
	// @param pCtx 	3-DES Context which stored key schedule table
	// @param pKey  3-DES key
	//
	VOID DES3SetDecKey( SDES3Context *pCtx, SDES3Block *pKey );

	//
	// @brief		   Crypto in 3-DES ECB mode
	// @param pCtx	   3-DES context
	// @param acInput  input plain text buffer
	// @param acOutput output cipher text buffer
	//
	VOID DES3CryptECB( SDES3Context *pCtx,
		UCHAR acInput[8],
		UCHAR acOutput[8] );

	//
	// @brief			Crypto in 3-DES CBC mode
	// @param pCtx		3-DES context
	// @param nMode		encrypt/decrypt mode
	// @param nLength	input length
	// @param pIv		3-DES key
	// @param pacInput  input text buffer
	// @param pacOutput output text buffer
	//
	VOID DES3CryptCBC ( SDES3Context *pCtx,
		INT32		nMode,
		INT32		nLength,
		SDES3Block	*pIV,
		UCHAR *pacInput,
		UCHAR *pacOutput );



	//
	// @brief          Checkup routine
	//
	// @return         0 if successful, or 1 if the test failed
	//
	INT32 DESSelfTest( INT32 nVerbose );


}

#endif //SD_CRYPTODES_H

