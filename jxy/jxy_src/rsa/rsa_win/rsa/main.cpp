#include <string>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include "base64.h"

#include <stdio.h>

using namespace std;

typedef unsigned char byte;


#include <stdio.h>
#include <stdlib.h>

#include <openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#include <openssl/bio.h>
#include <fstream>
#include <iostream>
#include <string>

#include "rsa.h"

using namespace std;
// #pragma comment(lib, "libeay32.lib")
// #pragma comment(lib, "ssleay32.lib")

const char  public_key_str[] = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC+WNnrVVHQEcE3VwiuOPdAMZ5NGgs4Dikn6vd+XTURtAwn/86jd7En86F4s61pq7zjABNsVka8lpeq0Dq9XbXOQAPHJBt4PyEKA9EJ9XMeGF63UeJYPmN5VgrGtLJdPoFLY5AdTiNf3v6+CHyIoDs0NWsw3OSbostMbgwm7Z5A+wIDAQAB";
const char private_key_str[] = "MIICeAIBADANBgkqhkiG9w0BAQEFAASCAmIwggJeAgEAAoGBAL5Y2etVUdARwTdXCK4490Axnk0aCzgOKSfq935dNRG0DCf/zqN3sSfzoXizrWmrvOMAE2xWRryWl6rQOr1dtc5AA8ckG3g/IQoD0Qn1cx4YXrdR4lg+Y3lWCsa0sl0+gUtjkB1OI1/e/r4IfIigOzQ1azDc5Juiy0xuDCbtnkD7AgMBAAECgYEAjuu39HODkm9CXIMFCYvMWm7nJY1Ajz5SuJ48Zq0lg71kw0PXq/XbFtxfiXmf45AHq4oLC3Vcd427I45mDHmoX4ApaaKozk/+QOGdBuM1mattfqX2D/MDqbUNa2yVo1lN9/4KtWf9+vKIusL70+iYTOuNAw946M/9cN9qBxZfkcECQQDvcz5ncjfLA9fsqYwhHtAWf7+5RuUifZyJn4fvgpqjGth5qP+1zzE+xo+5mQA79w+8iXpz0CIx+zMR+MtgUxotAkEAy4DK7pdvBXdhSZDpFLuJZINPj60baV/IRsi4NG3l59kPwXn41MizY3pT9EqBKySGVzOgXtzT9TZ6WVBLxrGIxwJAbmnpwLG+YHLpQkv8PdmDgpxtv/Wc+waJBDz35c2HmOZsI811/FQKFTRiWebfYMdQDlQmjvjOi9WVexgKT+SCEQJBAItIOBx6qvLr1Ttbohudi5iuwuqMSWGiu9mukVzZBuzbRIxgZe2E/DqOxB1ETQnxMr1GSXID5FJFmhG9AlAYvIsCQQDEVQ1hO9q2nOeAkD5w6ly3G72uI1jBduzRLyCLUte+QmNKA+0b2LoIyCzgZk9ghlIU3m86TLpVNxGEyha4uLId";
// const char encrypted_text[]  = "V/5/99ubmARZ0uOT/KOBrOdns/91mm23mAGDvfvgJjhG36/R82QaaEQrD7+vIR7850Hx03wZr7QcdIy2CGiEB+p5lgKDjU8FAQAjwLxqIWNT0T8ugdwGiMI3pC/SKlY3I0mKtoN78YNIgFLWA5QM0xSWvjls5p7hLbs4cZz6Oe4=";

const char      plain_text[] = "dealseq=20130222156627158&fee=0.01&payresult=0";

void kuaiyong_my_test()
{
    char *errmsg = NULL;

    int encrypt_len = 0;
    char *encrypt_text = private_key_encrypt_from_file(plain_text, strlen(plain_text), "private.pem", &encrypt_len, &errmsg);
    if(NULL == encrypt_text)
    {
        printf(errmsg);
        return;
    }

    int encrypt_len_from_buf = 0;
    char *encrypt_text_from_buf = private_key_encrypt(plain_text, strlen(plain_text), private_key_str, &encrypt_len_from_buf, &errmsg);
    if(NULL == encrypt_text_from_buf)
    {
        printf(errmsg);
        return;
    }

    int decrypt_len = 0;
    char *decrypt_text = public_key_decrypt_from_file(encrypt_text, encrypt_len, "public.pem", &decrypt_len, &errmsg);
    if(NULL == decrypt_text)
    {
        printf(errmsg);
        // return;
    }

    int decrypt_len_from_buf = 0;
    char *decrypt_text_from_buf = public_key_decrypt(encrypt_text_from_buf, encrypt_len_from_buf, public_key_str, &decrypt_len_from_buf, &errmsg);
    if(NULL == decrypt_text_from_buf)
    {
        printf(errmsg);
        // return;
    }

    unsigned int sign_len = 0;
    char *sign_text = private_key_sign(plain_text, strlen(plain_text), private_key_str, &sign_len, &errmsg);
    if(NULL == sign_text || 0 == sign_len)
    {
        return;
    }

    bool verify_ok = public_key_verify(plain_text, strlen(plain_text), sign_text, sign_len, public_key_str, &errmsg);
    if(false == verify_ok)
    {
        return;
    }
}

void kuaiyong_test()
{
    const char  sign_public_key_str[] = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCmreYIkPwVovKR8rLHWlFVw7YDfm9uQOJKL89Smt6ypXGVdrAKKl0wNYc3/jecAoPi2ylChfa2iRu5gunJyNmpWZzlCNRIau55fxGW0XEu553IiprOZcaw5OuYGlf60ga8QT6qToP0/dpiL/ZbmNUO9kUhosIjEu22uFgR+5cYyQIDAQAB";
    const char      sign_plain_text[] = "notifyId=GC2014012016284531500023878004&partnerOrder=andoppo_1390206441_100063666_4647&productName=x3330¸öÔª±¦&productDesc=&price=29800&count=1&attach=100063666";
    const char     base64_sign_text[] = "gCVbSDFnGVMSx+5GPLRF2ppD9OLxV3zj2v6ImHfhALxFIwPkac5mMHzr6Yx/1GX/X27Y2mk4y21nCeGcO6H1QlF1Jdr4Xz0P+VAMATjcVXklm85nfjvIFIrAHQtefp3qq0EEDZMYUVRJ2TnKLa+eb6JGq3ehyoct29QFKOqfUqo=";

    int sign_plain_len = strlen(sign_plain_text);
    size_t base64_sign_len = strlen(base64_sign_text);

    size_t sign_len = 0;
    const unsigned char *sign_text = base64_decode((unsigned char*)base64_sign_text, base64_sign_len, &sign_len);

    char *errmsg = NULL;
    bool verify_ok = public_key_verify(sign_plain_text, sign_plain_len, (char*)sign_text, sign_len, sign_public_key_str, &errmsg);
    if(false == verify_ok)
    {
        printf(errmsg);
        return;
    }
}

int main(int argc, char *argv[])
{
    // online();
    kuaiyong_test();
    return 0;
}
