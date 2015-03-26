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
#include <string.h>
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
    const char  sign_public_key_str[] = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDQl074HcPTQ+cOyoUaL4cznY8yt5WBg76oIUiaqO+eOG0a31J08e9jqU1kEs9Ik2yjU+VVMQdZpYEoewg/XmKhtj4GvvClvkvmMxZUXTP0pWLG8EIX0seHEt+9oECwmVsfJyVvXl2yAc0FKLRCmGfjBko38YRa0NJ4iW871toPnQIDAQAB";
    const char sign_private_key_str[] = "MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBANCXTvgdw9ND5w7KhRovhzOdjzK3lYGDvqghSJqo7544bRrfUnTx72OpTWQSz0iTbKNT5VUxB1mlgSh7CD9eYqG2Pga+8KW+S+YzFlRdM/SlYsbwQhfSx4cS372gQLCZWx8nJW9eXbIBzQUotEKYZ+MGSjfxhFrQ0niJbzvW2g+dAgMBAAECgYBbhxlCrCTiTfhUOC9Bdzt3PZq5IW873mhJdBlcq+ZA4feWBAo1gHePnplz4QENkorn/Ac0Y4gbIRFUhu/uhmb2TZ0OeoaLEYpgXQya6v6YsUrtc5SNNnl5YyWD1Il0J8x5qo69Tx1zJ2K3JJsBX6XqLvr/KwryCz4nt0gFclQMLQJBAOxdnAJSA1t0YAnLTcFbtBOTn+oBG+U42k4GfV77wP0gIdl1Hj60FcYSMpnwm1Z0c71EkiIRaz59fgy6HoFB2q8CQQDh6xApLDNNz5xi4a1+G1wRw3vkrwaZ9/iZ4PFT6G1gxqtgXKV/9aZ7L4jGBxZpz/O94R7oBuFhPll4MkgbMR1zAkBe3UcjC0V7rB5rb+Q6KZMBlGtHIMDQY9wCGsqwl3wiW4YaX84OvnpKr70NWasOrNhS6zV3ZORVBUrU0tIxcHvTAkEAmx+UkTDQaXVMpHjhb93JckkQIQ4tMjuab86M5liKAhFoS1pSKXK7RYp2F8N2GGG5s3+IgTl4OBVfybB84m7vEQJBAMLq5hGVV3LJHPA+gnxmZF0n4kkwNlDyCdWGBR7BZ9e2aSMdCa6/tXgnRlkaXpZTxxXnxGG5DGcCRe/WI9p2dEc=";
    const char      sign_plain_text[] = "dealseq=20130222105228127&fee=0.01&notify_data=V/5/99ubmARZ0uOT/KOBrOdns/91mm23mAGDvfvgJjhG36/R82QaaEQrD7+vIR7850Hx03wZr7QcdIy2CGiEB+p5lgKDjU8FAQAjwLxqIWNT0T8ugdwGiMI3pC/SKlY3I0mKtoN78YNIgFLWA5QM0xSWvjls5p7hLbs4cZz6Oe4=&orderid=130222-23-10523450458&payresult=0&subject=充值100金&uid=放着我来&v=1";

    char *errmsg = NULL;
    const char *encrypted_text = "V/5/99ubmARZ0uOT/KOBrOdns/91mm23mAGDvfvgJjhG36/R82QaaEQrD7+vIR7850Hx03wZr7QcdIy2CGiEB+p5lgKDjU8FAQAjwLxqIWNT0T8ugdwGiMI3pC/SKlY3I0mKtoN78YNIgFLWA5QM0xSWvjls5p7hLbs4cZz6Oe4=";


    int encrypted_len = strlen(encrypted_text);
    int sign_plain_len = strlen(sign_plain_text);

    size_t outlen = 0;
    const unsigned char *out = base64_decode((unsigned char*)encrypted_text, encrypted_len, &outlen);

    encrypted_text = (char*)out;
    encrypted_len = outlen;

    int decrypt_len = 0;
    char *decrypt_text = public_key_decrypt(encrypted_text, encrypted_len, public_key_str, &decrypt_len, &errmsg);
    if(NULL == decrypt_text)
    {
        printf(errmsg);
        return;
    }

    unsigned int sign_len = 0;
    char *sign_text = private_key_sign(sign_plain_text, sign_plain_len, sign_private_key_str, &sign_len, &errmsg);
    if(NULL == sign_text)
    {
        printf(errmsg);
        return;
    }

    bool verify_ok = public_key_verify(sign_plain_text, sign_plain_len, sign_text, sign_len, sign_public_key_str, &errmsg);
    if(false == verify_ok)
    {
        printf(errmsg);
        return;
    }
}

int main1(int argc, char *argv[])
{
    // online();
    kuaiyong_test();
    return 0;
}
