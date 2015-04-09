#ifndef __SDRSA_H__
#define __SDRSA_H__

/**
* @file sdrsahlp.h

* @brief 处理RSA编解码
*
**/
#include "config.h"
#include "sdtype.h"

#if SDU_WITH_OPENSSL

namespace SGDP
{
	typedef VOID* RSAKEY;

	/**
    * @defgroup groupcrypto 加解密操作
    * @ingroup  SGDP
    * @{
    */

    /**
    *@brief RSA辅助操作，必须包含openssl库
    */

    /**
    * @brief 产生rsa key
    * @param nBit : 密钥的位数
    * @param pPublicKey : 生成的公钥
    * @param pPrivateKey : 生成的私钥
    * @return 成功返回true，失败返回false
    */
    BOOL  GenerateRsaKey(INT32 nBit, RSAKEY *pPublicKey, RSAKEY *pPrivateKey);

    /**
    * @brief 保存公钥到文件
    * @param PublicKey : 对应的公钥指针
    * @param pszFile : 保存的文件名
    * @return 成功返回true，失败返回false
    */
    BOOL  SavePublicKey(const RSAKEY PublicKey, const CHAR *pszFile);

    /**
    * @brief 保存私钥到文件
    * @param PrivateKey : 对应的私钥指针
    * @param pszFile : 保存的文件名
    * @return 成功返回true，失败返回false
    */
    BOOL  SavePrivateKey(const RSAKEY PrivateKey, const CHAR* pszFile);

    /**
    * @brief 从文件中载入公钥
    * @param pPublicKey : 对应的公钥指针
    * @param pszFile : 公钥对应的文件名
    * @return 成功返回true，失败返回false
    */
    BOOL  LoadPublicKey(RSAKEY* pPublicKey, const CHAR* pszFile);

    /**
    * @brief 从文件中载入公钥
    * @param pPrivateKey : 对应的公钥指针
    * @param pszFile : 公钥对应的文件名
    * @return 成功返回true，失败返回false
    */
    BOOL  LoadPrivateKey(RSAKEY* pPrivateKey, const CHAR* pszFile);

	/**
    * @brief 保存公钥到内存
    * @param PublicKey : 对应的公钥指针
	* @param pbyKeyBuf : 内存的地址
    * @param wKeyLen : 内存的长度
    * @return 成功返回true，失败返回false
    */
    UINT16  SavePublicKeyToMem(const RSAKEY PublicKey, UINT8 *pbyKeyBuf, UINT16 wKeyLen);

    /**
    * @brief 保存私钥到内存
    * @param PrivateKey : 对应的私钥指针
	* @param pbyKeyBuf : 内存的地址
    * @param wKeyLen : 内存的长度
    * @return 成功返回true，失败返回false
    */
    UINT16  SavePrivateKeyToMem(const RSAKEY PrivateKey, UINT8 *pbyKeyBuf, UINT16 wKeyLen);

    /**
    * @brief 从内存中载入公钥
    * @param PublicKey : 公钥指针
    * @param pbyKeyBuf : 内存的地址
    * @param wKeyLen : 内存的长度
    * @return VOID
    */
    VOID  LoadPublicKeyFromMem(RSAKEY PublicKey, UINT8 *pbyKeyBuf, UINT16 wKeyLen);

    /**
    * @brief 从内存中载入私钥
    * @param PrivateKey : 私钥指针
    * @param pbyKeyBuf : 内存的地址
    * @param wKeyLen : 内存的长度
    * @return VOID
    */
    VOID  LoadPrivateKeyFromMem(RSAKEY PrivateKey, UINT8 *pbyKeyBuf, UINT16 wKeyLen);

    /**
    * @brief 对数据进行加密
    * @param PublicKey : 加密用的公钥
    * @param from : 需要解密的缓冲区地址
    * @param dwFromSize : 加密缓冲区长度
    * @param to : 加密数据存储位置
    * @return 返回的加密后的长度
    */
    UINT32  PubEncrypt(RSAKEY PublicKey, UINT8 *from, UINT32 dwFromSize, UINT8 *to);

    /**
    * @brief 对数据进行解密
    * @param PrivateKey : 解密用的私钥
    * @param from : 需要解密的缓冲区地址
    * @param dwFromSize : 需要解密的缓冲区长度
    * @param to : 解密数据存储位置
    * @return 返回的解密后的长度
    */
    UINT32  PriDecrypt(RSAKEY PrivateKey, UINT8 *from, UINT32 dwFromSize, UINT8 *to);

	enum ERsaSign {
		NID_UNDEF = 0,
		NID_MD5 = 4,
		NID_SHA = 41,
		NID_SHA1 = 64,
		NID_MD5_SHA = 114,
	};

	/**
	* @brief 用密钥获取信息的签名，需要注意的是，msglen最好不要超过100
    * @param PrivateKey : 签名的密钥
	* @param msg : 需要签名的字节串
	* @param msglen : 字节串长度
	* @param sigret : [输入输出参数]返回的签名
	* @param siglen : [输入输出参数]返回的签名长度
    * @param type : 使用摘要的算法
	* @return 是否获取签名成功，TRUE为成功，FALSE为失败
	*/
	BOOL  PriSign(RSAKEY PrivateKey, const UCHAR *msg, UINT32 msglen, UCHAR *sigret, UINT32 *siglen, INT32 type = NID_MD5);

	/**
	* @brief 用公钥验证签名，需要注意的是，msglen最好不要超过100
    * @param PublicKey : 验证的公钥
	* @param msg : 需要验证的字符串
	* @param msglen : 字节串长度
	* @param sigbuf : [输入输出参数]返回的签名
	* @param siglen : [输入输出参数]返回的签名长度
    * @param type : 使用摘要的算法
	* @return 是否验证成功，TRUE为成功，FALSE为失败
	*/
	BOOL  PubVerify(RSAKEY PublicKey, const UCHAR *msg, UINT32 msglen, UCHAR *sigbuf, UINT32 siglen, INT32 type = NID_MD5);

    /**
    * @brief 释放公钥资源
    * @param pstPublicKey : 公钥指针
    * @return VOID
    */
    VOID  FreePublicKey(RSAKEY* pPublicKey);

    /**
    * @brief 释放私钥资源
    * @param pstPrivateKey : 私钥指针
    * @return VOID
    */
    VOID  FreePrivateKey(RSAKEY* pPrivateKey);


    /** @} */
}

#endif // #if SDU_WITH_OPENSSL

#endif


