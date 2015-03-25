/********************************************************************
            Copyright (c) 2012, IGG.Inc
                   All rights reserved
         
    创建日期：  2012年6月25日 22时37分
    文件名称：  gnu.h
    说    明：  跨平台CRT封装
    
    当前版本：  1.00
    作    者：  Selphen Chu
    概    述：  

*********************************************************************/
#pragma once
#include <string.h>
#include <wchar.h>
#include "gnutypes.h"
#if defined __LINUX__
#include <stdarg.h>
#define WINAPI
#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#define __min(a,b)  (((a) < (b)) ? (a) : (b))

int strcpy_s(char *dest, uintptr_t dest_size, const char *source);
int strncpy_s(char *dest, uintptr_t dest_characters, const char *source, uintptr_t max_characters);

template<uintptr_t dest_size>
int strcpy_s(char (&dest)[dest_size], const char *source) {
	return strcpy_s(dest, dest_size, source);
}

template<uintptr_t dest_size>
int strncpy_s(char (&dest)[dest_size], const char *source, uintptr_t max_characters) {
	return strncpy_s(dest, dest_size, source, max_characters);
}

int wcscpy_s(wchar_t *dest, uintptr_t dest_characters, const wchar_t *source);
int wcsncpy_s(wchar_t *dest, uintptr_t dest_characters, const wchar_t *source, uintptr_t max_characters);

template<uintptr_t dest_characters>
int wcscpy_s(wchar_t (&dest)[dest_characters], const wchar_t *source) {
	return wcscpy_s(dest, dest_characters, source);
}

template<uintptr_t dest_characters>
int wcsncpy_s(wchar_t (&dest)[dest_characters], const wchar_t *source, uintptr_t max_characters) {
	return wcsncpy_s(dest, dest_characters, source, max_characters);
}

int memcpy_s(void *dest, uintptr_t dest_size, const void *source, uintptr_t copy_count);

char * strtok_s(char * _Str, const char * _Delim, char ** _Context);

int sprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, ...);

template<uintptr_t dest_characters>
int sprintf_s(char (&buffer)[dest_characters], const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	return vsnprintf(buffer, dest_characters - 1, format, ap);
}

int swprintf_s(wchar_t *buffer, size_t sizeOfBuffer, const wchar_t *format, ...);

template<uintptr_t dest_characters>
int swprintf_s(wchar_t (&buffer)[dest_characters], const wchar_t *format, ...) {
	va_list ap;
	va_start(ap, format);
	return vswprintf(buffer, dest_characters - 1, format, ap);
}

int stricmp(const char *s1, const char *s2);

#define ZeroMemory(ptr,size) memset((ptr),0,(size))
#define Sleep(t) usleep((t)*1000)
#define _atoi64(x) atoll(x)
#define printf_s printf
#define _stricmp(x, y) stricmp((x), (y))
#define InterlockedIncrement(x) __sync_add_and_fetch((x),1)
#define InterlockedDecrement(x) __sync_sub_and_fetch((x),1)
#define InterlockedCompareExchange(x, y, z) __sync_val_compare_and_swap((x),(z),(y))
#define InterlockedExchangeAdd(x, y) __sync_add_and_fetch((x),(y))
#define InterlockedExchangeSub(x, y) __sync_sub_and_fetch((x),(y))

#define INT64FMT "%lld"
#define UINT64FMT "%llu"
#define XINT64FMT "%llx"

#elif defined WIN32
#define InterlockedExchangeSub(x, y) InterlockedExchangeAdd((x),(-y))

#define INT64FMT "%I64d"
#define UINT64FMT "%I64u"
#define XINT64FMT "%I64x"
#endif

uintptr_t ucs2len(const ucs2_t *s);

int ucs2cpy_s(ucs2_t *dest, uintptr_t dest_characters, const ucs2_t *source, uintptr_t max_copy = 0xFFFFFFFF);
int ucscmp(const ucs2_t * _Str1, const ucs2_t * _Str2);

template<uintptr_t dest_characters>
int ucs2cpy_s(ucs2_t (&dest)[dest_characters], const ucs2_t *source) {
	return ucs2cpy_s(dest, dest_characters, source);
}

int wcs_cpy_to_ucs2(ucs2_t *dest, uintptr_t dest_characters, const wchar_t *source, uintptr_t max_copy = 0xFFFFFFFF);

template<uintptr_t dest_characters>
int wcs_cpy_to_ucs2(ucs2_t (&dest)[dest_characters], const wchar_t *source, uintptr_t max_copy = 0xFFFFFFFF) {
	return wcs_cpy_to_ucs2(dest, dest_characters, source, max_copy);
}

int ucs2_cpy_to_wcs(wchar_t *dest, uintptr_t dest_characters, const ucs2_t *source, uintptr_t max_copy = 0xFFFFFFFF);

template<uintptr_t dest_characters>
int ucs2_cpy_to_wcs(wchar_t (&dest)[dest_characters], const ucs2_t *source, uintptr_t max_copy = 0xFFFFFFFF) {
	return ucs2_cpy_to_wcs(dest, dest_characters, source, max_copy);
}

