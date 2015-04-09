
#include "base64.h"

#ifndef UINT_MAX
#define UINT_MAX      0xffffffff
#endif

static const UINT8 map2[] =
{
    0x3e, 0xff, 0xff, 0xff, 0x3f, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11,
    0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1a, 0x1b,
    0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
    0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
    0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33
};

INT32 Base64Decode(UINT8 * out, const CHAR *in, INT32 out_length)
{
    INT32 i, v;
    UINT8 *dst = out;

    v = 0;
    for (i = 0; in[i] && in[i] != '='; i++) {
        UINT32 index= in[i]-43;
        if (index>=(sizeof(map2)/sizeof(map2[0])) || map2[index] == 0xff)
            return -1;
        v = (v << 6) + map2[index];
        if (i & 3) {
            if (dst - out < out_length) {
                *dst++ = v >> (6 - 2 * (i & 3));
            }
        }
    }

    return (dst - out);
}


CHAR *Base64Encode(CHAR * buf, INT32 buf_len, const UINT8 * src, INT32 len)
{
    static const CHAR b64[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    CHAR *ret, *dst;
    unsigned i_bits = 0;
    INT32 i_shift = 0;
    INT32 bytes_remaining = len;

    if (len >= UINT_MAX / 4 ||
        buf_len < len * 4 / 3 + 12)
        return NULL;
    ret = dst = buf;
    while (bytes_remaining) {
        i_bits = (i_bits << 8) + *src++;
        bytes_remaining--;
        i_shift += 8;

        do {
            *dst++ = b64[(i_bits << 6 >> i_shift) & 0x3f];
            i_shift -= 6;
        } while (i_shift > 6 || (bytes_remaining == 0 && i_shift > 0));
    }
    while ((dst - ret) & 3)
        *dst++ = '=';
    *dst = '\0';

    return ret;
}
    
CHAR *Base64Encode2(const CHAR *p, CHAR *buf, INT32 len) 
{    
    CHAR al[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    CHAR *s = buf;
        
    while(*p) 
    {
        if (s >= buf+len-4)
        {
            printf("base64enc: buffer overflow\n");
            break ;
        }
        *(s++) = al[(*p >> 2) & 0x3F];
        *(s++) = al[((*p << 4) & 0x30) | ((*(p+1) >> 4) & 0x0F)];
        *s = *(s+1) = '=';
        *(s+2) = 0;
        if (! *(++p)) break;
        *(s++) = al[((*p << 2) & 0x3C) | ((*(p+1) >> 6) & 0x03)];
        if (! *(++p)) break;
        *(s++) = al[*(p++) & 0x3F];
    }
        
    return buf;
}
CHAR GetBase64Value(CHAR ch)
{
    if ((ch >= 'A') && (ch <= 'Z')) 
        return ch - 'A'; 
    if ((ch >= 'a') && (ch <= 'z')) 
        return ch - 'a' + 26; 
    if ((ch >= '0') && (ch <= '9')) 
        return ch - '0' + 52; 
    switch (ch) 
    { 
    case '+': 
        return 62; 
    case '/': 
        return 63; 
    case '=': /* base64 padding */ 
        return 0; 
    default: 
        return 0; 
    } 
}
    
//进行base64解码输入应该是4的倍数(根据mime标准)
//如果不是4倍数返回错误
//注意 如果是最后一个字符 那么长度不准备 可能会多1 
//返回buf长度
INT32 Base64Decode2(CHAR *buf,CHAR*text,INT32 size) 
{
    UCHAR chunk[4];
    INT32 parsenum=0;
        
    if(size%4) return -1;
    while(size>0)
    {
        chunk[0] = GetBase64Value(text[0]); 
        chunk[1] = GetBase64Value(text[1]); 
        chunk[2] = GetBase64Value(text[2]); 
        chunk[3] = GetBase64Value(text[3]); 
            
        *buf++ = (chunk[0] << 2) | (chunk[1] >> 4); 
        *buf++ = (chunk[1] << 4) | (chunk[2] >> 2); 
        *buf++ = (chunk[2] << 6) | (chunk[3]);
            
        text+=4;
        size-=4;
        parsenum+=3;
    }
    return parsenum;
} 


