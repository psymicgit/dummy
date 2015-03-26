///<------------------------------------------------------------------------------
//< @file:   zlib.h
//< @date:   2014年2月27日 10:18:08
//< @brief:  zlib操作函数
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "zlibpkg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <stdarg.h>

#include <zlib.h>

using namespace std;

typedef unsigned char byte;
typedef int errno_t;

char zlib_err[1024] = "";
size_t err_len = sizeof(zlib_err);

void get_err(char buf[], const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
}

// gz压缩
int gzcompress(const char *uncompr, unsigned int uncompr_len, char *compr, unsigned int *compr_len, char **err_msg)
{
    *err_msg = zlib_err;
    zlib_err[0] = '\0';
    get_err(zlib_err, "failed to gzcompress %s\n", uncompr);

    z_stream c_stream;
    int err = 0;

    if(uncompr && uncompr_len > 0) {
        c_stream.zalloc = NULL;
        c_stream.zfree = NULL;
        c_stream.opaque = NULL;
        //只有设置为MAX_WBITS + 16才能在在压缩文本中带header和trailer
        if(deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
            MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) return -1;
        c_stream.next_in  = (Bytef*)uncompr;
        c_stream.avail_in  = uncompr_len;
        c_stream.next_out = (Bytef*)compr;
        c_stream.avail_out  = *compr_len;
        while(c_stream.avail_in != 0 && c_stream.total_out < *compr_len) {
            if(deflate(&c_stream, Z_NO_FLUSH) != Z_OK) return -1;
        }
        if(c_stream.avail_in != 0) return c_stream.avail_in;
        for(;;) {
            if((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END) break;
            if(err != Z_OK) return -1;
        }
        if(deflateEnd(&c_stream) != Z_OK) return -1;
        *compr_len = c_stream.total_out;
        *err_msg = NULL;
        return 0;
    }
    return -1;
}

// gz解压
int gzdecompress(const char *compr, unsigned int compr_len, char *uncompr, unsigned int *uncompr_len, char **err_msg)
{
    *err_msg = zlib_err;
    zlib_err[0] = '\0';
    get_err(zlib_err, "failed to gzdecompress %s\n", compr);

    int err = 0;
    z_stream d_stream = {0}; /* decompression stream */
    static char dummy_head[2] = {
        0x8 + 0x7 * 0x10,
        (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };
    d_stream.zalloc = NULL;
    d_stream.zfree = NULL;
    d_stream.opaque = NULL;
    d_stream.next_in  = (Bytef*)compr;
    d_stream.avail_in = 0;
    d_stream.next_out = (Bytef*)uncompr;
    //只有设置为MAX_WBITS + 16才能在解压带header和trailer的文本
    if(inflateInit2(&d_stream, MAX_WBITS + 16) != Z_OK) return -1;
    //if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
    while(d_stream.total_out < *uncompr_len && d_stream.total_in < compr_len) {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
        if(err != Z_OK) {
            if(err == Z_DATA_ERROR) {
                d_stream.next_in = (Bytef*) dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) {
                    return -1;
                }
            } else return -1;
        }
    }
    if(inflateEnd(&d_stream) != Z_OK) return -1;
    *uncompr_len = d_stream.total_out;

    // *err_msg = NULL;
    return 0;
}


// htttpgz解压
int httpgzdecompress(const char *httpcompr, unsigned int httpcompr_len, char *uncompr, unsigned int *uncompr_len, char **err_msg)
{
    *err_msg = zlib_err;
    zlib_err[0] = '\0';
    get_err(zlib_err, "failed to gzdecompress %s\n", httpcompr);

    int err = 0;
    z_stream d_stream = {0}; /* decompression stream */
    static char dummy_head[2] = 
    {
        0x8 + 0x7 * 0x10,
        (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in  = (Bytef*)httpcompr;
    d_stream.avail_in = 0;
    d_stream.next_out = (Bytef*)uncompr;
    if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
    while (d_stream.total_out < *uncompr_len && d_stream.total_in < httpcompr_len) {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
        if(err != Z_OK )
        {
            if(err == Z_DATA_ERROR)
            {
                d_stream.next_in = (Bytef*)dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) 
                {
                    get_err(zlib_err, "failed to gzcompress %s\n", httpcompr);
                    return -1;
                }
            }
            else return -1;
        }
    }
    if(inflateEnd(&d_stream) != Z_OK) return -1;
    *uncompr_len = d_stream.total_out;
    *err_msg = NULL;
    return 0;
}
