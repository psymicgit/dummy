#ifndef SD_BIGNUM_H
#define SD_BIGNUM_H
/**
* @file sdbignum.h

* @brief 处理大数的函数系列
*
**/
#include "sdrsa.h"
#include "sdbignummul.h"

//#ifndef WIN64
namespace SGDP
{

// typedef error number
    enum eBigNumErrNo
    {
        SDCRYPTO_BNERR_FILE_IO_ERROR	 = -0x0002,
        SDCRYPTO_BNERR_BAD_INPUT_DATA    = -0x0004,
        SDCRYPTO_BNERR_INVALID_CHARACTER = -0x0006,
        SDCRYPTO_BNERR_BUFFER_TOO_SMALL  = -0x0008,
        SDCRYPTO_BNERR_NEGATIVE_VALUE    = -0x000A,
        SDCRYPTO_BNERR_DIVISION_BY_ZERO  = -0x000C,
        SDCRYPTO_BNERR_NOT_ACCEPTABLE    = -0x000E
    };

// typedef check return macro
#define BN_CHK(f)  if ( ( nRet = f ) != 0 ) \
	{						\
	goto cleanup;		\
		}


#ifdef __cplusplus
    extern "C" {
#endif

        //
        // @brief		Initialize one or more BigNum
        //
        VOID BNInit( BigNum *X, ...);

        //
        // @brief		 Free one or more BigNum
        //
        VOID BNFree( BigNum *X, ... );

        //
        // @brief          Enlarge to the specified number of limbs
        //
        // @return         0 if successful,
        //                 1 if memory allocation failed
        //
        INT32 BNGrow( BigNum *X, INT32 nLimbs );

        //
        // @brief          Copy the contents of Y into X
        //
        // @return         0 if successful,
        //                 1 if memory allocation failed
        //
        INT32 BNCopy( BigNum *X, BigNum *Y );

        //
        // @brief           Swap the contents of X and Y
        //
        VOID BNSwap( BigNum *X, BigNum *Y );

        //
        // @brief           Set value from integer
        //
        // @return          0 if successful,
        //                  1 if memory allocation failed
        //
        INT32 BNSetValue( BigNum *X, INT32 nZ );

        //
        // @brief           Return the number of least significant bits
        //
        INT32 BNGetLSB( BigNum *X );

        //
        // @brief           Return the number of most significant bits
        //
        INT32 BNGetMSB( BigNum *X );

        //
        // @brief           Return the total size in bytes
        //
        INT32 BNGetTotalBytes( BigNum *X );

        //
        // @brief			Convert an ASCII character to digit value
        //
        // @param d			destination digital value
        // @param nRadix	input numeric base
        // @param c			charactor
        //
        // @return		    0 if successful, or a SDCRYPTO_BNERR_XXX error code
        //
        INT32 BNGetDigit( UINT32 *d, INT32 nRadix, CHAR c );

        //
        // @brief            Import from an ASCII string
        //
        // @param X          destination BigNum
        // @param nRadix     input numeric base
        // @param s          null-terminated string buffer
        //
        // @return           0 if successful, or a SDCRYPTO_BNERR_XXX error code
        //
        INT32 BNReadString( BigNum *X, INT32 nRadix, CHAR *s );

        //
        // @brief			Helper to write the digits high-order first
        //
        // @param X
        // @param nRadix
        // @param p
        //
        // @return
        //
        INT32 BNWriteHlp( BigNum *X, INT32 nRadix, CHAR **p );

        //
        // @brief            Export into an ASCII string
        //
        // @param X          source BigNum
        // @param nRadix     output numeric base
        // @param s          string buffer
        // @param slen       string buffer size
        //
        // @return           0 if successful, or a SDCRYPTO_BNERR_XXX error code
        //
        // @note             Call this function with *slen = 0 to obtain the
        //                   minimum required buffer size in *slen.
        //
        INT32 BNWriteString( BigNum *X, INT32 nRadix, CHAR *s, INT32 *sLen );

        //
        // @brief            Read X from an opened file
        //
        // @param X          destination mpi
        // @param nRadix     input numeric base
        // @param fpIn       input file handle
        //
        // @return           0 if successful, or a SDCRYPTO_BNERR_XXX error code
        //
        INT32 BNReadFile( BigNum *X, INT32 nRadix, FILE *fpIn );

        //
        // @brief            Write X into an opened file, or stdout
        //
        // @param p          prefix, can be NULL
        // @param X          source mpi
        // @param nRadix     output numeric base
        // @param fpOut      output file handle
        //
        // @return           0 if successful, or a SDCRYPTO_BNERR_XXX error code
        //
        // @note             Set fpOut == NULL to print X on the console.
        //
        INT32 BNWriteFile( CHAR *p, BigNum *X, INT32 nRadix, FILE *fpOut );

        //
        // @brief            Import X from unsigned binary data, big endian
        //
        // @param X          destination mpi
        // @param pBuf       input buffer
        // @param nBufLen    input buffer size
        //
        // @return           0 if successful,
        //                   1 if memory allocation failed
        //
        INT32 BNReadBinary( BigNum *X, UCHAR *pBuf, INT32 nBufLen );

        //
        // @brief           Export X INT32o unsigned binary data, big endian
        //
        // @param X         source mpi
        // @param pBuf      output buffer
        // @param nBufLen   output buffer size
        //
        // @return          0 if successful,
        //                  SDCRYPTO_BNERR_BUFFER_TOO_SMALL if buf isn't large enough
        //
        // @note            Call this function with *buflen = 0 to obtain the
        //                  minimum required buffer size in *buflen.
        //
        INT32 BNWriteBinary( BigNum *X, UCHAR *pBuf, INT32 nBufLen );

        //
        // @brief           Left-shift: X <<= nCount
        //
        // @return          0 if successful,
        //                  1 if memory allocation failed
        //
        INT32 BNShiftLeft( BigNum *X, INT32 nCount );

        //
        // @brief           Right-shift: X >>= nCount
        //
        // @return          0 if successful,
        //                  1 if memory allocation failed
        //
        INT32 BNShiftRight( BigNum *X, INT32 nCount );

        //
        // @brief           Compare unsigned values
        //
        // @return         1 if |X| is greater than |Y|,
        //                -1 if |X| is lesser  than |Y| or
        //                 0 if |X| is equal to |Y|
        //
        INT32 BNCmpAbs( BigNum *X, BigNum *Y );

        //
        // @brief           Compare signed values
        //
        // @return         1 if X is greater than Y,
        //                -1 if X is lesser  than Y or
        //                 0 if X is equal to Y
        //
        INT32 BNCmpMpi( BigNum *X, BigNum *Y );

        //
        // @brief           Compare signed values
        //
        // @return         1 if X is greater than z,
        //                -1 if X is lesser  than z or
        //                 0 if X is equal to z
        //
        INT32 BNCmpInt( BigNum *X, INT32 nZ );

        //
        // @brief           Unsigned addition: X = |A| + |B|
        //
        // @return         0 if successful,
        //                 1 if memory allocation failed
        //
        INT32 BNAddAbs( BigNum *X, BigNum *A, BigNum *B );

        //
        // @brief          Helper for mpi substraction
        //
        VOID BNSubHlp( INT32 n, UINT32 *s, UINT32 *d );

        //
        // @brief           Unsigned substraction: X = |A| - |B|
        //
        // @return         0 if successful,
        //                 SDCRYPTO_BNERR_NEGATIVE_VALUE if B is greater than A
        //
        INT32 BNSubAbs( BigNum *X, BigNum *A, BigNum *B );

        //
        // @brief           Signed addition: X = A + B
        //
        // @return         0 if successful,
        //                 1 if memory allocation failed
        //
        INT32 BNAddMpi( BigNum *X, BigNum *A, BigNum *B );

        //
        // @brief           Signed substraction: X = A - B
        //
        // @return         0 if successful,
        //                 1 if memory allocation failed
        //
        INT32 BNSubMpi( BigNum *X, BigNum *A, BigNum *B );

        //
        // @brief           Signed addition: X = A + b
        //
        // @return         0 if successful,
        //                 1 if memory allocation failed
        //
        INT32 BNAddInt( BigNum *X, BigNum *A, INT32 b );

        //
        // @brief           Signed substraction: X = A - b
        //
        // @return         0 if successful,
        //                 1 if memory allocation failed
        //
        INT32 BNSubInt( BigNum *X, BigNum *A, INT32 b );

        //
        // @brief          Helper for mpi multiplication
        //
        VOID BNMulHlp( INT32 i, UINT32 *s, UINT32 *d, UINT32 b );

        //
        // @brief           Baseline multiplication: X = A * B
        //
        // @return         0 if successful,
        //                 1 if memory allocation failed
        //
        INT32 BNMulMpi( BigNum *X, BigNum *A, BigNum *B );

        //
        // @brief           Baseline multiplication: X = A * b
        //
        // @return         0 if successful,
        //                 1 if memory allocation failed
        //
        INT32 BNMulInt( BigNum *X, BigNum *A, UINT32 b );

        //
        // @brief           Division by mpi: A = Q * B + R
        //
        // @return         0 if successful,
        //                 1 if memory allocation failed,
        //                 SDCRYPTO_BNERR_DIVISION_BY_ZERO if B == 0
        //
        // @note           Either Q or R can be NULL.
        //
        INT32 BNDivMpi( BigNum *Q, BigNum *R, BigNum *A, BigNum *B );

        //
        // @brief           Division by INT32: A = Q * b + R
        //
        // @return         0 if successful,
        //                 1 if memory allocation failed,
        //                 SDCRYPTO_BNERR_DIVISION_BY_ZERO if b == 0
        //
        // @note           Either Q or R can be NULL.
        //
        INT32 BNDivInt( BigNum *Q, BigNum *R, BigNum *A, INT32 b );

        //
        // @brief           Modulo: R = A mod B
        //
        // @return         0 if successful,
        //                1 if memory allocation failed,
        //                SDCRYPTO_BNERR_DIVISION_BY_ZERO if B == 0
        //
        INT32 BNModMpi( BigNum *R, BigNum *A, BigNum *B );

        //
        // @brief           Modulo: r = A mod b
        //
        // @return         0 if successful,
        //                1 if memory allocation failed,
        //                SDCRYPTO_BNERR_DIVISION_BY_ZERO if b == 0
        //
        INT32 BNModInt( UINT32 *r, BigNum *A, INT32 b );

        //
        // @brief			Fast Montgomery initialization (thanks to Tom St Denis)
        //
        VOID BNMontgInit( UINT32 *mm, BigNum *N );

        //
        // @brief			Montgomery multiplication: A = A * B * R^-1 mod N  (HAC 14.36)
        //
        VOID BNMontMul( BigNum *A, BigNum *B, BigNum *N, UINT32 mm, BigNum *T );

        //
        // @brief			Montgomery reduction: A = A * R^-1 mod N
        //
        VOID BNMontRed( BigNum *A, BigNum *N, UINT32 mm, BigNum *T );

        //
        // @brief           Sliding-window exponentiation: X = A^E mod N
        //
        // @return         0 if successful,
        //                1 if memory allocation failed,
        //                SDCRYPTO_BNERR_BAD_INPUT_DATA if N is negative or even
        //
        // @note           _RR is used to avoid re-computing R*R mod N across
        //                multiple calls, which speeds up things a bit. It can
        //                be set to NULL if the extra performance is unneeded.
        //
        INT32 BNExpMod( BigNum *X, BigNum *A, BigNum *E, BigNum *N, BigNum *_RR );

        //
        // @brief           Greatest common divisor: G = gcd(A, B)
        //
        // @return         0 if successful,
        //                 1 if memory allocation failed
        //
        INT32 BNGcd( BigNum *G, BigNum *A, BigNum *B );

        //
        // @brief           Modular inverse: X = A^-1 mod N
        //
        // @return         0 if successful,
        //                 1 if memory allocation failed,
        //                 SDCRYPTO_BNERR_BAD_INPUT_DATA if N is negative or nil
        //                 SDCRYPTO_BNERR_NOT_ACCEPTABLE if A has no inverse mod N
        //
        INT32 BNInvMod( BigNum *X, BigNum *A, BigNum *N );

        //
        // @brief           Miller-Rabin primality test
        //
        // @return         0 if successful (probably prime),
        //                 1 if memory allocation failed,
        //                 SDCRYPTO_BNERR_NOT_ACCEPTABLE if X is not prime
        //
        INT32 BNIsPrime( BigNum *X, INT32 (*fpRng)(VOID *), VOID *pRng );

        //
        // @brief           Prime number generation
        //
        // @param X        destination mpi
        // @param nBits    required size of X in bits
        // @param nDhFlag  if 1, then (X-1)/2 will be prime too
        // @param fpRng    RNG function
        // @param pRng     RNG parameter
        //
        // @return         0 if successful (probably prime),
        //                 1 if memory allocation failed,
        //                 SDCRYPTO_BNERR_BAD_INPUT_DATA if nBits is < 3
        //
        INT32 BNGenPrime( BigNum *X, INT32 nBits, INT32 nDhFlag,
                          INT32 (*fpRng)(VOID *), VOID *pRng );

        //
        // @brief           Checkup routine
        //
        // @return         0 if successful, or 1 if the test failed
        //
        INT32 BNSelfTest( INT32 nVerbose );


    };

}
//#endif // 

#endif //SD_BIGNUM_H

