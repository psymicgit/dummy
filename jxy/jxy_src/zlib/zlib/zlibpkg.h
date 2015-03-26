///<------------------------------------------------------------------------------
//< @file:   zlib.h
//< @date:   2014年2月27日 10:17:08
//< @brief:  zlib操作函数
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _zlibpkg_h_
#define _zlibpkg_h_

/****************zlib压缩和解压************************************************/

/**
  * gzip压缩
  * @uncompr     : 原串
  * @uncompr_len : 原串长度
  * @compr       : 压缩串
  * @compr_len   : 压缩串长度
  * @err_msg     : 失败时返回的错误信息
  * @return      ：成功返回0，否则表示失败
  */
int gzcompress(const char *uncompr, unsigned int uncompr_len, char *compr, unsigned int *compr_len, char **err_msg);

/**
  * gzip解压
  * @compr       : 压缩串
  * @compr_len   : 压缩串长度
  * @uncompr     : 原串
  * @uncompr_len : 原串长度
  * @err_msg     : 失败时返回的错误信息
  * @return      : 成功返回0，否则表示失败
  */
int gzdecompress(const char *compr, unsigned int compr_len, char *uncompr, unsigned int *uncompr_len, char **err_msg);

/**
  * htttpgzip解压
  * @compr       : 压缩串
  * @compr_len   : 压缩串长度
  * @uncompr     : 原串
  * @uncompr_len : 原串长度
  * @err_msg     : 失败时返回的错误信息
  * @return      : 成功返回0，否则表示失败
  */
int httpgzdecompress(const char *compr, unsigned int compr_len, char *uncompr, unsigned int *uncompr_len, char **err_msg);

/****************zlib压缩和解压************************************************/

#endif //_zlibpkg_h_