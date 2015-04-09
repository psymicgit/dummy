#ifndef SDSTRENCRYPT_H
#define SDSTRENCRYPT_H
/**
* @file sdstrencrypt.h
* @brief 字符串加密解密系列
*
**/
#include "sdtype.h"
#include <string>


namespace SGDP
{
    /**
    * @defgroup groupencrypt 字符串加密
    * @ingroup  SGDP
    * @{
    */

    /**
    *@brief 字符串加密处理类
    */
    class CSDStrEncrypt
    {
    public:

		/**
		* @brief 构造函数
		*/
        CSDStrEncrypt() {}

		/**
		* @brief 析构函数
		*/
        ~CSDStrEncrypt() {}
        static const BYTE CODE_KEY = 0x96;	/**<加密对应的key*/

        //对每个字节高低4bit互换，与KEY进行异或运算

        /**
        * @brief static, encode a string 对字符串进行加密
        * @param sContent : string to be encoded 要加密的字符串
        * @return VOID
        */
        static VOID  Encode(std::string& sContent);

        /**
        * @brief static, encode a string 对字符串进行加密
        * @param pBuf : pointer to the buf to be encode 要加密的缓冲区
        * @param nLen : buf len 缓冲区长度
        * @return VOID
        */
        static VOID  Encode(BYTE *pBuf, INT32 nLen);

        /**
        * @brief static, decode a string 对字符串进行解密
        * @param sContent : string to be decode 要解密的字符串
        * @return VOID
        */
        static VOID  Decode(std::string &sContent);

        /**
        * @brief static, decode a string 对字符串进行解密
        * @param pBuf : pointer to the buf to be encode 要解密的缓冲区
        * @param nLen : buf len 缓冲区长度
        * @return VOID
        */
        static VOID  Decode(BYTE *pBuf, INT32 nLen);
    };

    /** @} */

} // namespace SGDP



#endif //


