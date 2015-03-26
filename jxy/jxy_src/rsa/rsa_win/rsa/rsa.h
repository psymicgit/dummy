///<------------------------------------------------------------------------------
//< @file:   rsa.h
//< @author: hongkunan
//< @date:   2014年1月7日 11:21:8
//< @brief:  针对快用平台的rsa操作函数（对其余平台可能不适用，比如，其他平台可能采用pkcs#1或者pkcs#8来加解密，验证签名时可能采用sha1/sha/md5等签名算法，）
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _rsa_h_
#define _rsa_h_

/****************rsa加密解密************************************************/

/**
  * 利用公钥解密密文
  * @encrypted_text : 秘文
  * @encrypted_len  : 秘文长度
  * @pub_key        : 公钥，这个公钥须是没有空符号和换行符的，比如: 
  *                       MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC+WNnrVVHQEcE3VwiuOPdAMZ5NGgs4Dikn6vd+XTURtAwn/86jd7En86F4s61pq7zjABNsVka8lpeq0Dq9XbXOQAPHJBt4PyEKA9EJ9XMeGF63UeJYPmN5VgrGtLJdPoFLY5AdTiNf3v6+CHyIoDs0NWsw3OSbostMbgwm7Z5A+wIDAQAB
  * @decrypt_len    : 返回解密结果的长度，解密失败为0
  * @err_msg        : 失败时返回的错误信息
  * @return         ：返回解密后的明文，解密失败返回NULL
  */
char* public_key_decrypt(const char* encrypted_text, int encrypted_len, const char* pub_key, int *decrypt_len, char **err_msg);

/**
  * 利用公钥pem解密密文
  * @encrypted_text : 秘文
  * @encrypted_len  : 秘文长度
  * @pub_key_pem    : 公钥，这个公钥必须得是openssl支持的pem格式，比如: 
  *                       -----BEGIN PUBLIC KEY-----      
  *                       MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC+WNnrVVHQEcE3VwiuOPdAMZ5N      
  *                       Ggs4Dikn6vd+XTURtAwn/86jd7En86F4s61pq7zjABNsVka8lpeq0Dq9XbXOQAPH      
  *                       JBt4PyEKA9EJ9XMeGF63UeJYPmN5VgrGtLJdPoFLY5AdTiNf3v6+CHyIoDs0NWsw      
  *                       3OSbostMbgwm7Z5A+wIDAQAB      
  *                       -----END PUBLIC KEY-----      
  * @decrypt_len    : 返回解密结果的长度，解密失败为0
  * @err_msg        : 失败时返回的错误信息
  * @return         ：返回解密后的明文，解密失败返回NULL
  */
char* public_key_decrypt_by_pem(const char* encrypted_text, int encrypted_len, const char* pub_key_pem, int *decrypt_len, char **err_msg);

/**
  * 利用公钥文件解密密文
  * @encrypted_text : 秘文
  * @encrypted_len  : 秘文长度
  * @pub_key        : 存放公钥的文件地址，如:"publickey.pem"，文件里存放的是pem格式的公钥
  * @decrypt_len    : 返回解密结果的长度，解密失败为0
  * @err_msg        : 失败时返回的错误信息
  * @return         ：返回解密后的明文，解密失败返回NULL
  */
char* public_key_decrypt_from_file(const char* encrypted_text, int encrypted_len, const char* pub_key_file, int *decrypt_len, char **err_msg);

/**
  * 利用私钥加密明文
  * @plain_text  : 明文
  * @plain_len   : 明文长度
  * @pri_key     : 私钥，这个私钥是没有空符号和换行符的，比如：
  *                    MIICeAIBADANBgkqhkiG9w0BAQEFAASCAmIwggJeAgEAAoGBAL5Y2etVUdARwTdXCK4490Axnk0aCzgOKSfq935dNRG0DCf/zqN3sSfzoXizrWmrvOMAE2xWRryWl6rQOr1dtc5AA8ckG3g/IQoD0Qn1cx4YXrdR4lg+Y3lWCsa0sl0+gUtjkB1OI1/e/r4IfIigOzQ1azDc5Juiy0xuDCbtnkD7AgMBAAECgYEAjuu39HODkm9CXIMFCYvMWm7nJY1Ajz5SuJ48Zq0lg71kw0PXq/XbFtxfiXmf45AHq4oLC3Vcd427I45mDHmoX4ApaaKozk/+QOGdBuM1mattfqX2D/MDqbUNa2yVo1lN9/4KtWf9+vKIusL70+iYTOuNAw946M/9cN9qBxZfkcECQQDvcz5ncjfLA9fsqYwhHtAWf7+5RuUifZyJn4fvgpqjGth5qP+1zzE+xo+5mQA79w+8iXpz0CIx+zMR+MtgUxotAkEAy4DK7pdvBXdhSZDpFLuJZINPj60baV/IRsi4NG3l59kPwXn41MizY3pT9EqBKySGVzOgXtzT9TZ6WVBLxrGIxwJAbmnpwLG+YHLpQkv8PdmDgpxtv/Wc+waJBDz35c2HmOZsI811/FQKFTRiWebfYMdQDlQmjvjOi9WVexgKT+SCEQJBAItIOBx6qvLr1Ttbohudi5iuwuqMSWGiu9mukVzZBuzbRIxgZe2E/DqOxB1ETQnxMr1GSXID5FJFmhG9AlAYvIsCQQDEVQ1hO9q2nOeAkD5w6ly3G72uI1jBduzRLyCLUte+QmNKA+0b2LoIyCzgZk9ghlIU3m86TLpVNxGEyha4uLId
  * @encrypt_len : 返回加密结果的长度，加密失败为0
  * @err_msg     : 失败时返回的错误信息
  * @return      ：返回加密后的秘文，加密失败返回NULL
  */
char* private_key_encrypt(const char* plain_text, int plain_len, const char* pri_key, int *encrypt_len, char **err_msg);

/**
  * 利用私钥pem加密明文
  * @plain_text  : 明文
  * @plain_len   : 明文长度
  * @pri_key_pem : 私钥，这个私钥必须得是openssl支持的pem格式，比如: 
  *                    -----BEGIN RSA PRIVATE KEY-----
  *                    MIICeAIBADANBgkqhkiG9w0BAQEFAASCAmIwggJeAgEAAoGBAL5Y2etVUdARwTdX
  *                    CK4490Axnk0aCzgOKSfq935dNRG0DCf/zqN3sSfzoXizrWmrvOMAE2xWRryWl6rQ
  *                    Or1dtc5AA8ckG3g/IQoD0Qn1cx4YXrdR4lg+Y3lWCsa0sl0+gUtjkB1OI1/e/r4I
  *                    fIigOzQ1azDc5Juiy0xuDCbtnkD7AgMBAAECgYEAjuu39HODkm9CXIMFCYvMWm7n
  *                    JY1Ajz5SuJ48Zq0lg71kw0PXq/XbFtxfiXmf45AHq4oLC3Vcd427I45mDHmoX4Ap
  *                    aaKozk/+QOGdBuM1mattfqX2D/MDqbUNa2yVo1lN9/4KtWf9+vKIusL70+iYTOuN
  *                    Aw946M/9cN9qBxZfkcECQQDvcz5ncjfLA9fsqYwhHtAWf7+5RuUifZyJn4fvgpqj
  *                    Gth5qP+1zzE+xo+5mQA79w+8iXpz0CIx+zMR+MtgUxotAkEAy4DK7pdvBXdhSZDp
  *                    FLuJZINPj60baV/IRsi4NG3l59kPwXn41MizY3pT9EqBKySGVzOgXtzT9TZ6WVBL
  *                    xrGIxwJAbmnpwLG+YHLpQkv8PdmDgpxtv/Wc+waJBDz35c2HmOZsI811/FQKFTRi
  *                    WebfYMdQDlQmjvjOi9WVexgKT+SCEQJBAItIOBx6qvLr1Ttbohudi5iuwuqMSWGi
  *                    u9mukVzZBuzbRIxgZe2E/DqOxB1ETQnxMr1GSXID5FJFmhG9AlAYvIsCQQDEVQ1h
  *                    O9q2nOeAkD5w6ly3G72uI1jBduzRLyCLUte+QmNKA+0b2LoIyCzgZk9ghlIU3m86
  *                    TLpVNxGEyha4uLId
  *                    -----END RSA PRIVATE KEY-----   
  * @encrypt_len : 返回加密结果的长度，加密失败为0
  * @err_msg     : 失败时返回的错误信息
  * @return      ：返回加密后的秘文，加密失败返回NULL
  */
char* private_key_encrypt_pem(const char* plain_text, int plain_len, const char* pri_key_pem, int *encrypt_len, char **err_msg);


/**
  * 利用私钥文件加密明文
  * @plain_text  : 明文
  * @plain_len   : 明文长度
  * @pri_key     : 存放私钥的文件地址，如:"privatekey.pem"，文件里存放的是pem格式的私钥
  * @encrypt_len : 返回加密结果的长度，加密失败为0
  * @err_msg     : 失败时返回的错误信息
  * @return      ：返回加密后的秘文，加密失败返回NULL
  */
char* private_key_encrypt_from_file(const char* plain_text, int plain_len, const char* pri_key_file, int *encrypt_len, char **err_msg);

/****************rsa加密解密************************************************/


/****************rsa验签************************/

/**
  * 利用私钥进行rsa签名（采用SHA1算法），返回签名
  * @plain_text: 明文
  * @plain_len : 明文长度
  * @pri_key   : 私钥，这个私钥是没有空符号和换行符的，比如：
  *                    MIICeAIBADANBgkqhkiG9w0BAQEFAASCAmIwggJeAgEAAoGBAL5Y2etVUdARwTdXCK4490Axnk0aCzgOKSfq935dNRG0DCf/zqN3sSfzoXizrWmrvOMAE2xWRryWl6rQOr1dtc5AA8ckG3g/IQoD0Qn1cx4YXrdR4lg+Y3lWCsa0sl0+gUtjkB1OI1/e/r4IfIigOzQ1azDc5Juiy0xuDCbtnkD7AgMBAAECgYEAjuu39HODkm9CXIMFCYvMWm7nJY1Ajz5SuJ48Zq0lg71kw0PXq/XbFtxfiXmf45AHq4oLC3Vcd427I45mDHmoX4ApaaKozk/+QOGdBuM1mattfqX2D/MDqbUNa2yVo1lN9/4KtWf9+vKIusL70+iYTOuNAw946M/9cN9qBxZfkcECQQDvcz5ncjfLA9fsqYwhHtAWf7+5RuUifZyJn4fvgpqjGth5qP+1zzE+xo+5mQA79w+8iXpz0CIx+zMR+MtgUxotAkEAy4DK7pdvBXdhSZDpFLuJZINPj60baV/IRsi4NG3l59kPwXn41MizY3pT9EqBKySGVzOgXtzT9TZ6WVBLxrGIxwJAbmnpwLG+YHLpQkv8PdmDgpxtv/Wc+waJBDz35c2HmOZsI811/FQKFTRiWebfYMdQDlQmjvjOi9WVexgKT+SCEQJBAItIOBx6qvLr1Ttbohudi5iuwuqMSWGiu9mukVzZBuzbRIxgZe2E/DqOxB1ETQnxMr1GSXID5FJFmhG9AlAYvIsCQQDEVQ1hO9q2nOeAkD5w6ly3G72uI1jBduzRLyCLUte+QmNKA+0b2LoIyCzgZk9ghlIU3m86TLpVNxGEyha4uLId
  * @sign_len  : 返回的签名长度，失败返回0
  * @err_msg   : 失败时返回的错误信息
  * @return    : 返回签名，失败返回NULL
  */
char* private_key_sign(const char* plain_text, int plain_len, const char* pri_key, unsigned int *sign_len, char **err_msg);

/**
  * 利用私钥pem进行rsa签名（采用SHA1算法），返回签名
  * @plain_text  : 明文
  * @plain_len   : 明文长度
  * @pri_key_pem : 私钥，这个私钥必须得是openssl支持的pem格式，比如: 
  *                    -----BEGIN RSA PRIVATE KEY-----
  *                    MIICeAIBADANBgkqhkiG9w0BAQEFAASCAmIwggJeAgEAAoGBAL5Y2etVUdARwTdX
  *                    CK4490Axnk0aCzgOKSfq935dNRG0DCf/zqN3sSfzoXizrWmrvOMAE2xWRryWl6rQ
  *                    Or1dtc5AA8ckG3g/IQoD0Qn1cx4YXrdR4lg+Y3lWCsa0sl0+gUtjkB1OI1/e/r4I
  *                    fIigOzQ1azDc5Juiy0xuDCbtnkD7AgMBAAECgYEAjuu39HODkm9CXIMFCYvMWm7n
  *                    JY1Ajz5SuJ48Zq0lg71kw0PXq/XbFtxfiXmf45AHq4oLC3Vcd427I45mDHmoX4Ap
  *                    aaKozk/+QOGdBuM1mattfqX2D/MDqbUNa2yVo1lN9/4KtWf9+vKIusL70+iYTOuN
  *                    Aw946M/9cN9qBxZfkcECQQDvcz5ncjfLA9fsqYwhHtAWf7+5RuUifZyJn4fvgpqj
  *                    Gth5qP+1zzE+xo+5mQA79w+8iXpz0CIx+zMR+MtgUxotAkEAy4DK7pdvBXdhSZDp
  *                    FLuJZINPj60baV/IRsi4NG3l59kPwXn41MizY3pT9EqBKySGVzOgXtzT9TZ6WVBL
  *                    xrGIxwJAbmnpwLG+YHLpQkv8PdmDgpxtv/Wc+waJBDz35c2HmOZsI811/FQKFTRi
  *                    WebfYMdQDlQmjvjOi9WVexgKT+SCEQJBAItIOBx6qvLr1Ttbohudi5iuwuqMSWGi
  *                    u9mukVzZBuzbRIxgZe2E/DqOxB1ETQnxMr1GSXID5FJFmhG9AlAYvIsCQQDEVQ1h
  *                    O9q2nOeAkD5w6ly3G72uI1jBduzRLyCLUte+QmNKA+0b2LoIyCzgZk9ghlIU3m86
  *                    TLpVNxGEyha4uLId
  *                    -----END RSA PRIVATE KEY-----
  * @sign_len    : 返回的签名长度，失败返回0
  * @err_msg     : 失败时返回的错误信息
  * @return      : 返回签名，失败返回NULL
  */
char* private_key_sign_pem(const char* plain_text, int plain_len, const char* pri_key_pem, unsigned int *sign_len, char **err_msg);

/**
  * 利用公钥进行rsa验签(采用SHA1算法)，并返回验证结果：验证通过true/验证失败false
  * @plain_text: 明文
  * @plain_len : 明文长度
  * @sign_text : 签名
  * @sign_len  : 签名长度
  * @pub_key   : 公钥，这个公钥是没有空符号和换行符的，比如: 
  *                       MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC+WNnrVVHQEcE3VwiuOPdAMZ5NGgs4Dikn6vd+XTURtAwn/86jd7En86F4s61pq7zjABNsVka8lpeq0Dq9XbXOQAPHJBt4PyEKA9EJ9XMeGF63UeJYPmN5VgrGtLJdPoFLY5AdTiNf3v6+CHyIoDs0NWsw3OSbostMbgwm7Z5A+wIDAQAB
  * @err_msg   : 失败时返回的错误信息
  * @return    ：返回验证结果：验证通过true/验证失败false
  */
bool public_key_verify(const char* plain_text, int plain_len, const char* sign_text, int sign_len, const char* pub_key, char **err_msg);

/**
  * 利用公钥pem进行rsa验签(采用SHA1算法)，并返回验证结果：验证通过true/验证失败false
  * @plain_text  : 明文
  * @plain_len   : 明文长度
  * @sign_text   : 签名
  * @sign_len    : 签名长度
  * @pub_key_pem : 公钥，这个公钥必须得是openssl支持的pem格式，比如: 
  *                    -----BEGIN PUBLIC KEY-----      
  *                    MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC+WNnrVVHQEcE3VwiuOPdAMZ5N      
  *                    Ggs4Dikn6vd+XTURtAwn/86jd7En86F4s61pq7zjABNsVka8lpeq0Dq9XbXOQAPH      
  *                    JBt4PyEKA9EJ9XMeGF63UeJYPmN5VgrGtLJdPoFLY5AdTiNf3v6+CHyIoDs0NWsw      
  *                    3OSbostMbgwm7Z5A+wIDAQAB      
  *                    -----END PUBLIC KEY-----      
  * @err_msg     : 失败时返回的错误信息
  * @return      ：返回验证结果：验证通过true/验证失败false
  */
bool public_key_verify_pem(const char* plain_text, int plain_len, const char* sign_text, int sign_len, const char* pub_key_pem, char **err_msg);

/****************rsa验签************************/


/* 
附注：

通过分析快用平台提供的公钥和私钥范例，可以大致得知，快用平台是通过openssl生成rsa密钥的，但由于我们获取到的公钥被快用平台做了转换，所以，这里得反向转换公钥为openssl支持的格式

1. 反向转换公钥

    快用平台的提供的公钥：
        MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC+WNnrVVHQEcE3VwiuOPdAMZ5NGgs4Dikn6vd+XTURtAwn/86jd7En86F4s61pq7zjABNsVka8lpeq0Dq9XbXOQAPHJBt4PyEKA9EJ9XMeGF63UeJYPmN5VgrGtLJdPoFLY5AdTiNf3v6+CHyIoDs0NWsw3OSbostMbgwm7Z5A+wIDAQAB;

    而符合openssl格式的公钥一般是这样的（可以自己用openssl工具生成对照）：
        -----BEGIN PUBLIC KEY-----
        MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC+WNnrVVHQEcE3VwiuOPdAMZ5N
        Ggs4Dikn6vd+XTURtAwn/86jd7En86F4s61pq7zjABNsVka8lpeq0Dq9XbXOQAPH
        JBt4PyEKA9EJ9XMeGF63UeJYPmN5VgrGtLJdPoFLY5AdTiNf3v6+CHyIoDs0NWsw
        3OSbostMbgwm7Z5A+wIDAQAB
        -----END PUBLIC KEY-----

    所以，应对快用平台的传来的公钥串作如下处理：
    1. 每隔64个字符插入\r\n
    2. 头部加上-----BEGIN PUBLIC KEY-----\r\n
    3. 尾部加上\r\n-----END PUBLIC KEY-----\r\n

2. 关于私钥（我们这边是用不到私钥的，但这里也一并列出私钥相关内容）

    快用平台那边的私钥应该是这个样子的：
        -----BEGIN RSA PRIVATE KEY-----
        MIICeAIBADANBgkqhkiG9w0BAQEFAASCAmIwggJeAgEAAoGBAL5Y2etVUdARwTdX
        CK4490Axnk0aCzgOKSfq935dNRG0DCf/zqN3sSfzoXizrWmrvOMAE2xWRryWl6rQ
        Or1dtc5AA8ckG3g/IQoD0Qn1cx4YXrdR4lg+Y3lWCsa0sl0+gUtjkB1OI1/e/r4I
        fIigOzQ1azDc5Juiy0xuDCbtnkD7AgMBAAECgYEAjuu39HODkm9CXIMFCYvMWm7n
        JY1Ajz5SuJ48Zq0lg71kw0PXq/XbFtxfiXmf45AHq4oLC3Vcd427I45mDHmoX4Ap
        aaKozk/+QOGdBuM1mattfqX2D/MDqbUNa2yVo1lN9/4KtWf9+vKIusL70+iYTOuN
        Aw946M/9cN9qBxZfkcECQQDvcz5ncjfLA9fsqYwhHtAWf7+5RuUifZyJn4fvgpqj
        Gth5qP+1zzE+xo+5mQA79w+8iXpz0CIx+zMR+MtgUxotAkEAy4DK7pdvBXdhSZDp
        FLuJZINPj60baV/IRsi4NG3l59kPwXn41MizY3pT9EqBKySGVzOgXtzT9TZ6WVBL
        xrGIxwJAbmnpwLG+YHLpQkv8PdmDgpxtv/Wc+waJBDz35c2HmOZsI811/FQKFTRi
        WebfYMdQDlQmjvjOi9WVexgKT+SCEQJBAItIOBx6qvLr1Ttbohudi5iuwuqMSWGi
        u9mukVzZBuzbRIxgZe2E/DqOxB1ETQnxMr1GSXID5FJFmhG9AlAYvIsCQQDEVQ1h
        O9q2nOeAkD5w6ly3G72uI1jBduzRLyCLUte+QmNKA+0b2LoIyCzgZk9ghlIU3m86
        TLpVNxGEyha4uLId
        -----END RSA PRIVATE KEY-----    

*/

#endif //_rsa_h_