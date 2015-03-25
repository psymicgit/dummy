#include "gnu.h"

#if defined __LINUX__
int strcpy_s(char *dest, uintptr_t dest_size, const char *source){
	if (strlen(source) < dest_size) {
		strcpy(dest, source); 
		return 0;
	} else {
		memcpy(dest, source, dest_size);
		dest[dest_size - 1] = 0;
		return -1;
	}
}

int strncpy_s(char *dest, uintptr_t dest_size, const char *source, uintptr_t max_characters){
	uintptr_t len = __min(strlen(source), __min(dest_size-1, max_characters));
	strncpy(dest, source, len);
	dest[len] = '\0';
	return 0;
}

int wcscpy_s(wchar_t *dest, uintptr_t dest_characters, const wchar_t *source){
	if (wcslen(source) < dest_characters) {
		wcscpy(dest, source);
		return 0;
	} else {
		memcpy(dest, source, dest_characters);
		dest[dest_characters - 1] = 0;
		return -1;
	}
}

int wcsncpy_s(wchar_t *dest, uintptr_t dest_characters, const wchar_t *source, uintptr_t max_characters) {
	uintptr_t len = __min(wcslen(source), __min(dest_characters-1, max_characters));
	wcsncpy(dest, source, len);
	dest[len] = '\0';
	return 0;
}

int memcpy_s(void *dest, uintptr_t dest_size, const void *source, uintptr_t copy_count){
	if (copy_count >= dest_size) {
		memcpy(dest, source, dest_size);
		return 0;
	} else {
		memcpy(dest, source, copy_count);
		return -1;
	}
}
#define ZeroMemory(ptr,size) memset((ptr),0,(size))
#define Sleep(t) usleep((t)*1000)

char * strtok_s(char * _Str, const char * _Delim, char ** _Context) {
	return strtok_r(_Str, _Delim, _Context);
}

int sprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	return vsnprintf(buffer, sizeOfBuffer - 1, format, ap);
}

int swprintf_s(wchar_t *buffer, size_t sizeOfBuffer, const wchar_t *format, ...) {
	va_list ap;
	va_start(ap, format);
	return vswprintf(buffer, sizeOfBuffer - 1, format, ap);
}

int stricmp(const char *s1, const char *s2) {
	return strcasecmp(s1, s2);
}

#endif

uintptr_t ucs2len(const ucs2_t *s) {
#if defined __LINUX__
	uintptr_t len = 0;
	while (0 != *s) {
		++s;
		++len;
	}
	return len;
#elif defined WIN32
	return wcslen(s);
#endif
}

int ucs2cpy_s(ucs2_t *dest, uintptr_t dest_characters, const ucs2_t *source, uintptr_t max_copy /*= 0xFFFFFFFF*/) {
#if defined __LINUX__
	uintptr_t sourcelen = ucs2len(source);
	uintptr_t finallen = sourcelen >= dest_characters ? dest_characters - 1 : sourcelen;
	memcpy(dest, source, finallen * sizeof(ucs2_t));
	dest[finallen] = 0;
	return 0;
#elif defined WIN32
	return wcsncpy_s(dest, dest_characters, source, max_copy);
#endif
}

int ucscmp(const ucs2_t * _Str1, const ucs2_t * _Str2) {
#if defined __LINUX__
	while (*_Str1 && *_Str2 && (*_Str1 == *_Str2)) {  
		++_Str1;  
		++_Str2;
	}  
	return *_Str1 - *_Str2;  
#elif defined WIN32
	return wcscmp(_Str1, _Str2);
#endif
}

int wcs_cpy_to_ucs2(ucs2_t *dest, uintptr_t dest_characters, const wchar_t *source, uintptr_t max_copy) {
#if defined __LINUX__
	uintptr_t sourcelen = wcslen(source);
	uintptr_t i = 0;
	for (i = 0; i < sourcelen && i < dest_characters - 1 && i < max_copy; ++i) {
		dest[i] = (ucs2_t)source[i];
	}
	dest[i] = 0;
	return 0;
#elif WIN32
	return wcsncpy_s(dest, dest_characters, source, max_copy);
#endif
}
int ucs2_cpy_to_wcs(wchar_t *dest, uintptr_t dest_characters, const ucs2_t *source, uintptr_t max_copy) {
#if defined __LINUX__
	uintptr_t sourcelen = ucs2len(source);
	uintptr_t i = 0;
	for (i = 0; i < sourcelen && i < dest_characters - 1 && i < max_copy; ++i) {
		dest[i] = (wchar_t)source[i];
	}
	dest[i] = 0;
	return 0;
#elif defined WIN32
	return wcsncpy_s(dest, dest_characters, source, max_copy);
#endif
}

