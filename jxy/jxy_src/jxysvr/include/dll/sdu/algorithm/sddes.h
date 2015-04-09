
#ifndef SDDES_H
#define SDDES_H
/**
* @file sddes.h

* @brief 处理DES编解码
*
**/
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <time.h>
#include "sdtype.h"
#include <openssl/des.h>
namespace SGDP
{
    /**
    * @defgroup groupcrypto 加解密操作
    * @ingroup  SGDP
    * @{
    */

    /**
    * @brief DES enc/dec type macros
    * DES编码/解码宏
    */
#define DES_ENCRYPT  1
#define DES_DECRYPT  0

    /**
    * @brief typedef DES context structure
    * key schedule table
    * 定义DES上下文结构体
    * 关键调度表
    */
    typedef struct tagDESContext
    {
        INT32		nEnDecMode;			/**<encrypt/decrypt*/
        UINT32      aSubKey[32];		/**<DES sub-keys*/
    } SDESContext;

    /**
    * @brief typedef DES block structure
    * 64bits key (equal to IV)
    * 定义DES块结构
    * 64位key（等于IV）
    */
    typedef struct tagDESBlock
    {
        UCHAR acBlock[8];
    } SDESBlock;

    /**
    * @brief typedef 3-DES context structure
    * key schedule table
    * 定义3－DES上下文结构体
    * 关键调度表
    */
    typedef struct tagDES3Context
    {
        INT32		nEnDecMode;			/**<encrypt/decrypt*/
        UINT32      aSubKey[96];		/**<DES sub-keys*/
    } SDES3Context;

    /**
    * @brief typedef 3-DES block structure
    * 192bits key (equal to IV)
    * 定义3DES块结构体
    * 192位key（等于IV）
    */
    typedef struct tagDES3Block
    {
        UCHAR acBlock[24];
    } SDES3Block;


    typedef struct tagDesKey
    {
        DES_cblock          stDesKey;
        DES_key_schedule    stKeySche;
    }SDDES_KEY;
    /**
    * @brief des 加解密封装类，支持3－DES和DES两种方式
    */
    class CSDDes
    {
    public:
        /**
        * @brief
        * initialize des block 初始化DES块
        * @param pIV : [in/out]DES block structure to be initialized
        * 输入输出参数，将被初始化的DES块结构
        * @return VOID
        */
        VOID  DESGenKey(SDESBlock *pIV);

        /**
        * @brief
        * initialize des-3 block 初始化3－DES块
        * @param pIV : [in/out]DES-3 block structure to be initialized
        * 输入输出参数，将被初始化的3－DES块结构
        * @return VOID
        */
        VOID  DESGenKey(SDES3Block *pIV);

        /**
        * @brief
        * Generate DES encrypt key 生成DES编码键
        * @param pCtx : [in/out]DES context to be initialized
        * 输入输出参数，将被初始化的DES上下文
        * @param oKey : [in]des block [输入参数]DES块
        * @return VOID
        */
        VOID  DESGenEncKeySche(SDESContext *pCtx, SDESBlock &oKey);

        /**
        * @brief
        * Generate DES-3 encrypt key 生成3－DES编码键
        * @param pCtx : [in/out]DES-3 context to be initialized
        * [输入输出参数]将被初始化的DES上下文
        * @param oKey : [in]des block [输入参数]DES块
        * @return VOID
        */
        VOID  DESGenEncKeySche(SDES3Context *pCtx, SDES3Block &oKey);

        /**
        * @brief
        * Generate DES decrypt key 生成DES解码键
        * @param pCtx : [in/out]DES context to be initialized
        * [输入输出参数]将被初始化的DES上下文
        * @param oKey : [in]des block [输入参数]DES块
        * @return VOID
        */
        VOID  DESGenDecKeySche(SDESContext *pCtx, SDESBlock &oKey);

        /**
        * @brief
        * Generate DES-3 decrypt key 生成3－DES解码键
        * @param pCtx : [in/out]DES-3 context to be initialized
        * [输入输出参数]将被初始化的3－DES上下文
        * @param oKey : [in]des block [输入参数]DES块
        * @return VOID
        */
        VOID  DESGenDecKeySche(SDES3Context *pCtx, SDES3Block &oKey);

        /**
        * @brief
        * DES buffer encryption DES缓冲编码
        * @param pCtx : [in]DES context [输入参数]DES上下文
        * @param pIV :[in] DES block [输入参数]DES块
        * @param nInLen : [in]input buffer len [输入参数]输入缓冲区长度
        * @param pInput : [in]input buffer [输入参数]输入缓冲区
        * @param pOutput : [in/out]output buffer [输入输出参数]输出缓冲区
        * @return VOID
        */
        VOID  DESEncryptNCBC(SDESContext *pCtx,
                                  SDESBlock *pIV,
                                  INT32 nInLen,
                                  UCHAR *pInput,
                                  UCHAR *pOutput);

        /**
        * @brief
        * DES-3 buffer encryption 3－DES缓冲编码
        * @param pCtx : [in]DES-3 context [输入参数]3－DES上下文
        * @param pIV : [in]DES-3 block[输入参数] 3－DES块
        * @param nInLen : [in]input buffer len [输入参数]输入缓冲区长度
        * @param pInput : [in]input buffer [输入参数]输入缓冲区
        * @param pOutput : [in/out]output buffer [输入输出参数]输出缓冲区
        * @return VOID
        */
        VOID  DESEncryptNCBC(SDES3Context *pCtx,
                                  SDES3Block *pIV,
                                  INT32 nInLen,
                                  UCHAR *pInput,
                                  UCHAR *pOutput);

        /**
        * @brief
        * DES buffer decryption DES缓冲解码
        * @param pCtx : [in]DES context [输入参数]DES上下文
        * @param pIV : [in]DES block [输入参数]DES块
        * @param nInLen : [in]input buffer len [输入参数]输入缓冲区长度
        * @param pInput : [in]input buffer [输入参数]输入缓冲区
        * @param pOutput : [in/out]output buffer [输入输出参数]输出缓冲区
        * @return VOID
        */
        VOID  DESDecryptNCBC(SDESContext *pCtx,
                                  SDESBlock *pIV,
                                  INT32  nInLen,
                                  UCHAR *pInput,
                                  UCHAR *pOutput);

        /**
        * @brief
        * DES-3 buffer decryption 3－DES缓冲解码
        * @param pCtx : [in]DES-3 context [输入参数]3－DES上下文
        * @param pIV : [in]DES-3 block [输入参数]3－DES块
        * @param nInLen : [in]input buffer len [输入参数]输入缓冲长度
        * @param pInput : [in]input buffer [输入参数]输入缓冲区
        * @param pOutput : [in/out]output buffer [输入输出参数]输出缓冲区
        * @return VOID
        */
        VOID  DESDecryptNCBC(SDES3Context *pCtx,
                                  SDES3Block *pIV,
                                  INT32 nInLen,
                                  UCHAR *pInput,
                                  UCHAR *pOutput);



    };

}

#endif //SD_DES_H


