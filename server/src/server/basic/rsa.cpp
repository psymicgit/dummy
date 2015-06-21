///<------------------------------------------------------------------------------
//< @file:   rsa.cpp
//< @author: hongkunan
//< @date:   2014年1月7日 11:21:22
//< @brief:  本文件采用openssl中的rsa库，版本为openssl 1.0.1e
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "rsa.h"


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdio.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>  
#include <openssl/ossl_typ.h>
#include <openssl/err.h>

using namespace std;

typedef unsigned char byte;
typedef int errno_t;

char err_buf[1024];
char rsa_err[1024] = "";
size_t err_len = sizeof(rsa_err);

byte encrypt_buf[1024] = "";
byte decrypt_buf[1024] = "";

byte buf[1024] = "";

void get_err(char buf[], const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
}

// 针对openssl的rsa工具方法，这些方法仅在openssl的rsa模块源码基础上做了一些小调整
namespace openssl_rsa
{
    // 本函数从openssl源码中crypto/pem/pem_all.c中第162行拷贝而来
    RSA *pkey_get_rsa(EVP_PKEY *key, RSA **rsa)
    {
        RSA *rtmp;
        if(!key) return NULL;
        rtmp = EVP_PKEY_get1_RSA(key);
        EVP_PKEY_free(key);
        if(!rtmp) return NULL;
        if(rsa) {
            RSA_free(*rsa);
            *rsa = rtmp;
        }
        return rtmp;
    }

    // 根据给定的公钥获取RSA结构
    // 
    // 本函数从openssl源码中crypto/pem/pem_all.c中第252行:
    //         IMPLEMENT_PEM_rw(RSA_PUBKEY, RSA, PEM_STRING_PUBLIC, RSA_PUBKEY)
    // 的定义修改而来，由从pem文件读取改为从内存中读取
    RSA* get_rsa_from_public_key(const char* public_key)
    {
        RSA* rsa  = NULL;
        BIO *bio  = NULL;
        void *ret = NULL;

        if ((bio=BIO_new(BIO_s_mem())) == NULL)
        {
            PEMerr(PEM_F_PEM_ASN1_READ,ERR_R_BUF_LIB);
            return NULL;
        }

        BIO_puts(bio, public_key);

        ret=PEM_ASN1_read_bio((d2i_of_void *)d2i_RSA_PUBKEY,"PUBLIC KEY",bio,(void **)rsa,NULL, NULL);
        BIO_free(bio);

        return (RSA*)ret;
    }

    // 根据给定的私钥获取RSA结构
    // 
    // 本函数从openssl源码中crypto/pem/pem_all.c中第186行:
    //         RSA *PEM_read_RSAPrivateKey(FILE *fp, RSA **rsa, pem_password_cb *cb, void *u)
    // 的函数修改而来，由从pem文件读取改为从内存中读取
    RSA* get_rsa_from_private_key(const char* private_key)
    {
        RSA* rsa        = NULL;

        BIO *bio        = NULL;
        EVP_PKEY *pktmp = NULL;

        if ((bio=BIO_new(BIO_s_mem())) == NULL)
        {
            PEMerr(PEM_F_PEM_READ_PRIVATEKEY,ERR_R_BUF_LIB);
            return(0);
        }
        BIO_puts(bio, private_key);

        pktmp=PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
        BIO_free(bio);

        return pkey_get_rsa(pktmp, &rsa);
    }
}

namespace kuaiyong
{
    // 把快用平台的公钥转化成openssl支持的格式，详见头文件中的附注
    std::string convert_public_key(const char public_key[], int len)
    {
        string key;

        int cnt = 0;
        for(int i = 0; i < len; i++)
        {
            if(cnt < 64)
            {
                key += public_key[i];
                ++cnt;
            }
            else
            {
                key += "\r\n";
                key += public_key[i];
                cnt = 1;
            }
        }

        key = "-----BEGIN PUBLIC KEY-----\r\n" + key;
        key += "\r\n-----END PUBLIC KEY-----\r\n";
        return key;
    }

    // 把快用平台的私钥转化成openssl支持的格式，详见头文件中的附注
    std::string convert_private_key(const char public_key[], int len)
    {
        string key;

        int cnt = 0;
        for(int i = 0; i < len; i++)
        {
            if(cnt < 64)
            {
                key += public_key[i];
                ++cnt;
            }
            else
            {
                key += "\r\n";
                key += public_key[i];
                cnt = 1;
            }
        }

        key = "-----BEGIN RSA PRIVATE KEY-----\r\n" + key;
        key += "\r\n-----END RSA PRIVATE KEY-----\r\n";
        return key;
    }
}

// 利用公钥解密密文
char* public_key_decrypt(const char* encrypted_text, int encrypted_len, const char* pub_key, int *decrypt_len, char **err_msg)
{
    *err_msg = rsa_err;
    rsa_err[0] = '\0';

    // 把公钥转换成pem格式
    string pub_key_pem = kuaiyong::convert_public_key(pub_key, strlen(pub_key));
    return public_key_decrypt_by_pem(encrypted_text, encrypted_len, pub_key_pem.c_str(), decrypt_len, err_msg);
}

// 利用公钥pem解密密文
char* public_key_decrypt_by_pem(const char* encrypted_text, int encrypted_len, const char* pub_key_pem, int *decrypt_len, char **err_msg)
{
    *err_msg = rsa_err;
    rsa_err[0] = '\0';

    // 根据公钥获取rsa结构
    RSA* rsa = openssl_rsa::get_rsa_from_public_key(pub_key_pem);
    if(NULL == rsa)
    {
        get_err(rsa_err, "fail: given public key is invalid: %s!\n", pub_key_pem);
        return NULL;
    }

    // 用公钥加密明文
    int len = RSA_public_decrypt(encrypted_len, (unsigned char*)encrypted_text, (unsigned char*)decrypt_buf, rsa, RSA_PKCS1_PADDING);
    if(-1 == len){
        get_err(rsa_err, "failed to decrypt! please recheck the decrypted text and the format of %s\n", pub_key_pem);
        return NULL;
    }

    decrypt_buf[len] = '\0';

    *decrypt_len = len;
    return (char*)decrypt_buf;
}

// 利用公钥文件解密密文
char* public_key_decrypt_from_file(const char* encrypted_text, int encrypted_len, const char* pub_key_file, int *decrypt_len, char **err_msg)
{
    *err_msg = rsa_err;
    rsa_err[0] = '\0';
    decrypt_buf[0] = '\0';

    // 打开公钥文件
    FILE* pub_fp = fopen(pub_key_file, "r");
	if(NULL == pub_fp)
	{
        get_err(rsa_err, "failed to open public key file %s: %s!\n", pub_key_file, strerror(errno));
        return NULL;
	}

    // 从文件中读取公钥
    RSA* rsa = PEM_read_RSA_PUBKEY(pub_fp, NULL, NULL, NULL);
    if(NULL == rsa)
    {
        get_err(rsa_err, "fail: unable to read public key from %s!\n", pub_key_file);
        return NULL;
    }

    // 用公钥加密
    int len = RSA_public_decrypt(encrypted_len, (unsigned char*)encrypted_text, (unsigned char*)decrypt_buf, rsa, RSA_PKCS1_PADDING);
    if(-1 == len){
        get_err(rsa_err, "failed to decrypt! please recheck the decrypted text and the format of %s\n", pub_key_file);
        return NULL;
    }

    fclose(pub_fp);

    decrypt_buf[len] = '\0';

    *decrypt_len = len;
    return (char*)decrypt_buf;
}

// 利用私钥加密明文
char* private_key_encrypt(const char* plain_text, int plain_len, const char* pri_key, int *encrypt_len, char **err_msg)
{
    // 对私钥进行转换
    string pri_key_pem = kuaiyong::convert_private_key(pri_key, strlen(pri_key));
    return private_key_encrypt_pem(plain_text, plain_len, pri_key_pem.c_str(), encrypt_len, err_msg);
}

char* private_key_encrypt_pem(const char* plain_text, int plain_len, const char* pri_key_pem, int *encrypt_len, char **err_msg)
{
    *err_msg = rsa_err;
    rsa_err[0] = '\0';
    encrypt_buf[0] = '\0';
    *encrypt_len = 0;

    // 根据私钥获取rsa结构
    RSA* rsa = openssl_rsa::get_rsa_from_private_key(pri_key_pem);
    if(NULL == rsa)
    {
        get_err(rsa_err, "fail: given private key is invalid!\n");
        return NULL;
    }

    // 用rsa加密的明文长度不能太长
    int rsa_size  = RSA_size(rsa);
    if(plain_len >= rsa_size - 41)
    {
        get_err(rsa_err, "failed to encrypt: length of plain text(= %d) is too long, must be less than rsa_size(=%d) - 41 = %d\n", plain_len, rsa_size, rsa_size - 41);
        return NULL;
    }

    // 用私钥解密
    int len = RSA_private_encrypt(plain_len, (unsigned char*)plain_text, (unsigned char*)encrypt_buf, rsa, RSA_PKCS1_PADDING);
    if(-1 == len){
        get_err(rsa_err, "failed to encrypt! please recheck the plain text and the format of %s\n", pri_key_pem);
        return NULL;
    }

    encrypt_buf[len] = '\0';

    *encrypt_len = len;
    return (char*)encrypt_buf;
}

// 利用私钥文件加密明文
char* private_key_encrypt_from_file(const char* plain_text, int plain_len, const char* pri_key_file, int *encrypt_len, char **err_msg)
{
    *err_msg = rsa_err;
    rsa_err[0] = '\0';

    // 打开私钥文件
    FILE* priv_fp = fopen(pri_key_file, "r");
	if(NULL == priv_fp)
	{
        get_err(rsa_err, "failed to open private key file %s: %s!\n", pri_key_file, strerror(errno));
        return NULL;
	}

    // 从文件中读取私钥
    RSA *rsa = PEM_read_RSAPrivateKey(priv_fp, NULL, NULL, NULL);
    if(NULL == rsa){
        get_err(rsa_err, "fail: unable to read private key from %s!\n", pri_key_file);
        return NULL; 
    }

    int rsa_size  = RSA_size(rsa);
    if(plain_len >= rsa_size - 41)
    {
        get_err(rsa_err, "failed to encrypt: length of plain text(= %d) is too long, must be less than rsa_size(=%d) - 41 = %d\n", plain_len, rsa_size, rsa_size - 41);
        return NULL;
    }

    // 用私钥解密
    int len = RSA_private_encrypt(plain_len, (unsigned char*)plain_text, (unsigned char*)encrypt_buf, rsa, RSA_PKCS1_PADDING);
    if(-1 == len){
        get_err(rsa_err, "failed to encrypt! please recheck the plain text and the format of %s\n", pri_key_file);
        return NULL;
    }

    fclose(priv_fp);

    encrypt_buf[len] = '\0';

    *encrypt_len = len;
    return (char*)encrypt_buf;
}

// 利用私钥进行rsa签名（采用SHA1算法），返回签名
char* private_key_sign(const char* plain_text, int plain_len, const char* pri_key, unsigned int *sign_len, char **err_msg)
{
    // 对私钥进行转换
    string pri_key_pem = kuaiyong::convert_private_key(pri_key, strlen(pri_key));
    return private_key_sign_pem(plain_text, plain_len, pri_key_pem.c_str(), sign_len, err_msg);
}

// 利用私钥pem进行rsa签名（采用SHA1算法），返回签名
char* private_key_sign_pem(const char* plain_text, int plain_len, const char* pri_key_pem, unsigned int *sign_len, char **err_msg)
{
    *err_msg = rsa_err;
    rsa_err[0] = '\0';
    encrypt_buf[0] = '\0';
    *sign_len = 0;

    // 根据私钥获取rsa结构
    RSA* rsa = openssl_rsa::get_rsa_from_private_key(pri_key_pem);
    if(NULL == rsa)
    {
        get_err(rsa_err, "fail: given private key is invalid!\n");
        return NULL;
    }

    // 先对明文做sha1摘要
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, plain_text, plain_len);
    SHA1_Final(buf, &ctx);

    // 采用RSA对sha1摘要进行签名
    int ret = RSA_sign(NID_sha1, (unsigned char*)buf, SHA_DIGEST_LENGTH, encrypt_buf, sign_len, rsa);
    if(ret != 1)
    {
        get_err(rsa_err, "fail: can not sign with given private key!\n");
        return NULL;
    }

	encrypt_buf[*sign_len] = '\0';
    return (char*)encrypt_buf;
}

// 利用公钥进行rsa验签(采用SHA1算法)，并返回验证结果：验证通过true/验证失败false
bool public_key_verify(const char* plain_text, int plain_len, const char* sign_text, int sign_len, const char* pub_key, char **err_msg)
{
    // 对公钥进行转换
    string pub_key_pem = kuaiyong::convert_public_key(pub_key, strlen(pub_key));
    return public_key_verify_pem(plain_text, plain_len, sign_text, sign_len, pub_key_pem.c_str(), err_msg);
}

// 利用公钥pem进行rsa验签(采用SHA1算法)，并返回验证结果：验证通过true/验证失败false
bool public_key_verify_pem(const char* plain_text, int plain_len, const char* sign_text, int sign_len, const char* pub_key_pem, char **err_msg)
{
    *err_msg = rsa_err;
    rsa_err[0] = '\0';

    // 根据公钥获取rsa结构
    RSA* rsa = openssl_rsa::get_rsa_from_public_key(pub_key_pem);
    if(NULL == rsa)
    {
        get_err(rsa_err, "fail: given public key is invalid: %s!\n", pub_key_pem);
        return NULL;
    }

    // 先对明文做sha1摘要
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, plain_text, plain_len);
    SHA1_Final(buf, &ctx);

    // 采用RSA对sha1摘要进行验签
    int ret = RSA_verify(NID_sha1, (unsigned char*)buf, SHA_DIGEST_LENGTH, (unsigned char*)sign_text, sign_len, rsa);
    return (ret == 1);
}
