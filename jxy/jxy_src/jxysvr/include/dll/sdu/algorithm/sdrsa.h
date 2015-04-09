#ifndef SDRSA_H
#define SDRSA_H
/**
* @file sdrsa.h

* @brief 处理RSA编解码
*
**/

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <time.h>
#include "sdtype.h"



namespace SGDP
{
    /**
    * @defgroup groupcrypto 加解密操作
    * @ingroup  SGDP
    * @{
    */

#define SDCRYPTO_HAVE_ASM   /**<asm compiler supported flag*/

    /**
    *@brief RSA error code macros
    */
#define SDCRYPTO_ERR_RSA_BAD_INPUT_DATA       -0x0400
#define SDCRYPTO_ERR_RSA_INVALID_PADDING      -0x0410
#define SDCRYPTO_ERR_RSA_KEY_GEN_FAILED       -0x0420
#define SDCRYPTO_ERR_RSA_KEY_CHECK_FAILED     -0x0430
#define SDCRYPTO_ERR_RSA_PUBLIC_FAILED        -0x0440
#define SDCRYPTO_ERR_RSA_PRIVATE_FAILED       -0x0450
#define SDCRYPTO_ERR_RSA_VERIFY_FAILED        -0x0460

    /**
    *@brief PKCS#1 constants
    */
#define RSA_RAW         0
#define RSA_MD2         2
#define RSA_MD4         3
#define RSA_MD5         4
#define RSA_SHA1        5
#define RSA_SHA256      6

#define RSA_PUBLIC      0
#define RSA_PRIVATE     1

#define RSA_PKCS_V15    0
#define RSA_PKCS_V21    1

#define RSA_SIGN        1
#define RSA_CRYPT       2

    /**
    * @brief typedef Big Number structure
    * be used to hold big integer more than 100 bits
    * 定义大数结构体
    */
    typedef struct tagBigNum
    {
        INT32		s;			/**<signed flag*/
        UINT32		n;			/**<total limbs number*/
        UINT32*		p;			/**<pointer to limbs*/
    } BigNum;

    /**
    * @brief typedef RSA context structure
    * 定义RSA上下文结构体
    */
    typedef struct tagRSAContext
    {
        INT32 ver;				/**<always 0*/
        INT32 len;				/**<size(N) in chars*/
        BigNum N;				/**<public modulus*/
        BigNum E;				/**<public exponent*/
        BigNum D;				/**<private exponent*/
        BigNum P;				/**<1st prime factor*/
        BigNum Q;				/**<2nd prime factor*/
        BigNum DP;				/**<D % (P - 1)*/
        BigNum DQ;				/**<D % (Q - 1)*/
        BigNum QP;				/**<1 / (Q % P)*/
        BigNum RN;				/**<cached R^2 mod N*/
        BigNum RP;				/**<cached R^2 mod P*/
        BigNum RQ;				/**<cached R^2 mod Q*/
        INT32 padding;			/**<1.5 or OAEP/PSS*/
        INT32 hash_id;			/**<hash identifier*/
        INT32 (*f_rng)(VOID *);	/**<RNG function*/
        VOID *p_rng;			/**<RNG parameter*/
    } SRSAContext;

    /**
    *@brief rsa加解密封装类
    */
    class CSDRsa
    {
    public:

        /**
        * @brief
        * Initialize an RSA context 初始化一个RSA上下文
        * @param pCtx : RSA context to be initialized 将要被初始化的RSA上下文
        * @return VOID
        */
        VOID  RSAContextInit( SRSAContext *pCtx );

        /**
        * @brief
        * Free the components of an RSA key 释放一个RSA上下文
        * @param pCtx : RSA context to be freed 将要被释放的RSA上下文
        * @return VOID
        */
        VOID  RSAContextFree( SRSAContext *pCtx );

        /**
        * @brief
        * Get the length of RSA 获取RSA的长度
        * @param pCtx : RSA context 输入的RSA上下文
        * @return the length of RSA 返回RSA的长度
        */
        INT32  RSANSize( SRSAContext *pCtx );

        /**
        * @brief
        * Get a SDNew RSA context 获取一个新的RSA上下文
        * @return pointer of SDNew RSA context, return NULL if failed
        * 返回一个新的RSA上下文的指针，如果返回NULL，代表失败
        */
        SRSAContext*  RSANew();

        /**
        * @brief
        * Generate an RSA keypair 生成一个RSA公钥密钥对
        * @param pKeyPair : RSA context that will hold the key
        * 将要持有此RSA公钥密钥对的RSA上下文
        * @param nNBits : size of the public key in bits
        * 公钥的bit位数
        * @param nExponent : public exponent 公共指数
        * @return success return true, else false 成功返回true，失败返回false
        */
        BOOL  RSAGenKeyPair(SRSAContext *pKeyPair, INT32 nNBits = 1024, INT32 nExponent = 65537);

        /**
        * @brief
        * Get public key length 获取一个公钥长度
        * @param pPubKey : RSA context that hold the key
        * 持有此RSA公钥的RSA上下文
        * @return the length of public key
        * 此RSA公钥的长度
        */
        INT32  RSAGetPubKeyLen(SRSAContext *pPubKey);

        /**
        * @brief
        * Get private key length 获取密钥的长度
        * @param pPrivKey : RSA context that hold the key
        * 持有此RSA密钥的RSA上下文
        * @return the length of private key
        * 此RSA密钥的长度
        */
        INT32  RSAGetPrivKeyLen(SRSAContext *pPrivKey);

        /**
        * @brief
        * Save public key to file 存储公钥到文件
        * @param pPubKey : RSA context that hold the key
        * 持有此RSA公钥的RSA上下文
        * @param pszFile : file name which save the public key
        * 存储的文件路径
        * @return sucess return true, else false 是否成功，true为成功，false为失败
        */
        BOOL   RSASavePubKey(SRSAContext *pPubKey, const char *pszFile);

        /**
        * @brief
        * Save public key to memory 存储公钥到到内存
        * @param pPubKey : RSA context that hold the key
        * 持有此RSA公钥的RSA上下文
        * @param pBuffer : buffer to save the key
        * 存储此RSA公钥的缓冲区
        * @param nBufLen : buffer length
        * 此缓冲区的长度
        * @return sucess return true, else false 是否成功，true为成功，false为失败
        */
        BOOL   RSASavePubKeyToMem(SRSAContext *pPubKey, UCHAR* pBuffer, INT32 nBufLen);

        /**
        * @brief
        * load public key from file 从文件中载入公钥
        * @param pPubKey : RSA context that hold the key
        * 将要持有此RSA公钥的RSA上下文
        * @param pszFile : file name which save the public key
        * 需要载入公钥的文件路径
        * @return sucess return true, else false 是否成功，true为成功，false为失败
        */
        BOOL   RSALoadPubKey(SRSAContext *&pPubKey, const char *pszFile);

        /**
        * @brief
        * load public key from memory 从内存中载入公钥
        * @param pPubKey : RSA context that will hold the key
        * 将要持有此RSA公钥的RSA上下文
        * @param pBuffer : buffer to save the key
        * 需要载入公钥的缓冲区
        * @param nBufLen : buffer length 缓冲区长度
        * @return sucess return true, else false 是否成功，true为成功，false为失败
        */
        BOOL   RSALoadPubKeyFromMem(SRSAContext *&pPubKey, const UCHAR* pBuffer, INT32 nBufLen);

        /**
        * @brief
        * Save private key to file 存储密钥到文件
        * @param pPrivKey : RSA context that will hold the key
        * 持有此RSA密钥的RSA上下文
        * @param pszFile : file name which save the public key
        * 存储密钥的文件路径
        * @return sucess return true, else false 是否成功，true为成功，false为失败
        */
        BOOL   RSASavePrivKey(SRSAContext *pPrivKey, const char *pszFile);

        /**
        * @brief
        * Save private key to memory 存储密钥到到内存
        * @param pPrivKey : RSA context that will hold the key
        * 持有此RSA密钥的RSA上下文
        * @param pBuffer : buffer to save the key
        * 存储此RSA密钥的缓冲区
        * @param nBufLen : buffer length
        * 此缓冲区的长度
        * @return sucess return true, else false 是否成功，true为成功，false为失败
        */
        BOOL   RSASavePrivKeyToMem(SRSAContext *pPrivKey, UCHAR* pBuffer, INT32 nBufLen);

        /**
        * @brief
        * load private key from file 从文件中载入密钥
        * @param pPrivKey : RSA context that will hold the key
        * 将要持有此RSA密钥的RSA上下文
        * @param pszFile : file name which save the public key
        * 需要载入密钥的文件路径
        * @return sucess return true, else false 是否成功，true为成功，false为失败
        */
        BOOL   RSALoadPrivKey(SRSAContext *&pPrivKey, const char *pszFile);

        /**
        * @brief
        * load private key from memory 从内存中载入密钥
        * @param pPrivKey : RSA context that will hold the key
        * 将要持有此RSA密钥的RSA上下文
        * @param pBuffer : buffer to save the key
        * 需要载入密钥的缓冲区
        * @param nBufLen : buffer length
        * 此缓冲区的长度
        * @return sucess return true, else false 是否成功，true为成功，false为失败
        */
        BOOL   RSALoadPrivKeyFromMem(SRSAContext *&pPrivKey, const UCHAR* pBuffer, INT32 nBufLen);

        /**
        * @brief
        * encrypt from public key 从公钥编码
        * @param pPubKey : RSA context that will hold the key
        * 持有此RSA公钥的RSA上下文
        * @param nInLen : input buffer size 输入缓冲区的大小
        * @param pInput : input buffer 输入的缓冲区
        * @param pOutput : output buffer 输出的缓冲区
        * @return length of encrypt buffer 编码的长度
        */
        INT32  RSAPubEncrypt(SRSAContext *pPubKey,
                                  INT32		  nInLen,
                                  UCHAR *pInput,
                                  UCHAR *pOutput);

        /**
        * @brief
        * decrypt from private key 从密钥解码
        * @param pPrivKey : RSA context that will hold the key
        * 持有此RSA密钥的RSA上下文
        * @param nInLen : input buffer size 输入缓冲区的大小
        * @param pInput : input buffer 输入的缓冲区
        * @param pOutput : output buffer 输出的缓冲区
        * @return length of decrypt buffer 解码的长度
        */
        INT32  RSAPrivDecrypt(SRSAContext *pPrivKey,
                                   INT32		  nInLen,
                                   UCHAR *pInput,
                                   UCHAR *pOutput);

    private:

        /**
        * @brief 读取公钥
        * @param pPubKey : 持有此RSA公钥的RSA上下文
        * @param pBuf    : 输出的缓冲区
        * @param nRet    : 错误码
        * @return TRUE:成功获取 FALSE:出现错误
        */
        BOOL RSAReadPubKeySub(SRSAContext* pPubKey, UCHAR* pBuf, INT32 nRet);

        /**
        * @brief 读取密钥
        * @param pPrivKey : 持有此RSA密钥的RSA上下文
        * @param pBuf    : 输出的缓冲区
        * @param nRet    : 错误码
        * @return TRUE:成功获取 FALSE:出现错误
        */
        BOOL RSAReadPrivKeySub(SRSAContext* pPrivKey, UCHAR* pBuf, INT32 nRet);

        /**
        * @brief 写入公钥
        * @param pPubKey : 持有此RSA公钥的RSA上下文
        * @param pBuf    : 输入的缓冲区
        * @param nRet    : 错误码
        * @return TRUE:成功写入 FALSE:出现错误
        */
        BOOL RSAWritePubKeySub(SRSAContext* pPubKey, UCHAR* pBuf, INT32 nRet);

        /**
        * @brief 写入密钥
        * @param pPrivKey : 持有此RSA密钥的RSA上下文
        * @param pBuf    : 输入的缓冲区
        * @param nRet    : 错误码
        * @return TRUE:成功写入 FALSE:出现错误
        */
        BOOL RSAWritePrivKeySub(SRSAContext* pPrivKey, UCHAR* pBuf, INT32 nRet);

        /**
        * @brief 获取公钥的长度
        * @param pPubKey : 持有RSA公钥的RSA上下文
        * @return 公钥的长度
        */
        INT32 RSAGetPubKeyLenSub(SRSAContext *pPubKey);

        /**
        * @brief 获取密钥的长度
        * @param pPrivKey : 持有RSA密钥的RSA上下文
        * @return 密钥的长度
        */
        INT32 RSAGetPrivKeyLenSub(SRSAContext *pPrivKey);
        //
    };


    /** @} */

}


#endif //SD_RSA_H


