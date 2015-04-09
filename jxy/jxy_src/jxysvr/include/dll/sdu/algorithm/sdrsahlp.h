
#ifndef SDRSAHLP_H
#define SDRSAHLP_H
/**
* @file sdrsahlp.h
* @brief 处理RSA编解码
*
**/
#include "config.h"
#include "sdtype.h"

#if SDU_WITH_OPENSSL

#include <openssl/rsa.h>
#include <openssl/crypto.h>
namespace SGDP
{
    /**
    * @defgroup groupcrypto 加解密操作
    * @ingroup  SGDP
    * @{
    */

    /**
    *@brief RSA辅助操作，必须包含openssl库
    */

    /**
    * @brief
    * 产生rsa key
    * @param nBit : 密钥的位数
    * @param pstPublicKey : 生成的公钥
    * @param pstPrivateKey : 生成的私钥
    * @return 成功返回true，失败返回false
    */
    BOOL GenerateRsaKey(INT32 nBit, RSA* pstPublicKey, RSA* pstPrivateKey);

    /**
    * @brief
    * 保存公钥到文件
    * @param pstPublicKey : 对应的公钥指针
    * @param pszFile : 保存的文件名
    * @return 成功返回true，失败返回false
    */
    BOOL SavePublicKey(const RSA* pstPublicKey, const CHAR* pszFile);

    /**
    * @brief
    * 保存私钥到文件
    * @param pstPrivateKey : 对应的私钥指针
    * @param pszFile : 保存的文件名
    * @return 成功返回true，失败返回false
    */
    BOOL SavePrivateKey(const RSA* pstPrivateKey, const CHAR* pszFile);

    /**
     * @brief
     * 从文件中载入公钥
     * @param pstPublicKey : 对应的公钥指针
     * @param pszFile : 公钥对应的文件名
     * @return 成功返回TRUE，失败返回FALSE
     */
    BOOL LoadPublicKey(RSA*& pstPublicKey, const CHAR* pszFile);

    /**
    * @brief
    * 从文件中载入公钥
    * @param pstPrivateKey : 对应的公钥指针
    * @param pszFile : 公钥对应的文件名
    * @return 成功返回true，失败返回false
    */
    BOOL LoadPrivateKey(RSA*& pstPrivateKey, const CHAR* pszFile);

    /**
    * @brief
    * 从内存中载入公钥
    * @param pstKey : 公钥指针
    * @param pbyKeyBuf : 内存的地址
    * @param wKeyLen : 内存的长度
	* @param iElemSize : BN_ULONG类型的大小
    * @return VOID
    */
    // VOID LoadPublicKeyFromMem(RSA* &pstKey, UINT8* pbyKeyBuf, UINT16 wKeyLen, INT32 iElemSize);

    /**
     * @brief
     * 从内存中载入公钥
     * @param pstKey : 公钥指针
     * @param pbyKeyBuf : 内存的地址
     * @param wKeyLen : 内存的长度
     */
    VOID LoadPublicKeyFromMem(RSA* &pstKey, UINT8* pbyKeyBuf, UINT16 wKeyLen);

    /**
    * @brief
    * 从内存中载入私钥
    * @param pstKey : 私钥指针
    * @param pbyKeyBuf : 内存的地址
    * @param wKeyLen : 内存的长度
	* @param iElemSize : BN_ULONG类型的大小
    * @return VOID
    */
    VOID LoadPrivateKeyFromMem(RSA* &pstKey, UINT8* pbyKeyBuf, UINT16 wKeyLen, INT32 iElemSize );

    /**
    * @brief
    * 对数据进行加密
    * @param from : 需要解密的缓冲区地址
    * @param dwFromSize : 加密缓冲区长度
    * @param to : 加密数据存储位置
    * @param pPubKey : 加密用的公钥
    * @return VOID
    */
    UINT32 PubEncrypt(UINT8* from, UINT32 dwFromSize, UINT8* to, RSA* pPubKey);

    /**
    * @brief
    * 对数据进行解密
    * @param from : 需要解密的缓冲区地址
    * @param dwFromSize : 需要解密的缓冲区长度
    * @param to : 解密数据存储位置
    * @param pPriKey : 解密用的私钥
    * @return VOID
    */
    UINT32 PriDecrypt(UINT8* from, UINT32 dwFromSize, UINT8* to, RSA* pPriKey);

    /**
    * @brief
    * 释放公钥资源
    * @param pstPublicKey : 公钥指针
    * @return VOID
    */
    VOID FreePublicKey(RSA* &pstPublicKey);

    /**
    * @brief
    * 释放私钥资源
    * @param pstPrivateKey : 私钥指针
    * @return VOID
    */
    VOID FreePrivateKey(RSA* &pstPrivateKey);

	/**
    * @brief
    * 释放Crypto加密模式全局内存
    * @return VOID
    */
    VOID FreeCrypto();

    /** @} */

}

#endif // #if SDU_WITH_OPENSSL 

#endif /*#ifndef RSAHLP_H__ */


