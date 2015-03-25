#ifndef GNUTYPES_H
#define	GNUTYPES_H
#pragma once
#include <string>
#if defined __LINUX__
/* Types for `void *' pointers.  */  
#include <limits.h>
#if ULONG_MAX == 0xffffffffffffffff

#ifndef __intptr_t_defined  
typedef long int        intptr_t;  
#define __intptr_t_defined  
#endif  
typedef unsigned long int   uintptr_t;  

typedef unsigned int ULONG;

#else  


#ifndef __intptr_t_defined  
typedef int         intptr_t;  
#define __intptr_t_defined  
#endif  
typedef unsigned int        uintptr_t;  
typedef unsigned long ULONG;
#endif

typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef int INT32;
typedef unsigned int UINT32;
typedef long long LONGLONG;
typedef long long __int64;
typedef unsigned long long ULONGLONG;
typedef bool BOOL;


// Windows types wrapper
typedef int INT;
typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef short SHORT;
typedef unsigned char BYTE;
typedef int LONG;
//typedef unsigned int ULONG;

typedef float FLOAT;
typedef void *LPVOID;
typedef void *HMODULE;


// UCS2(2 bytes of unicode) type define
typedef unsigned short ucs2_t;
typedef unsigned short WCHAR;

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#elif defined WIN32
// UCS2(2 bytes of unicode) type define
typedef wchar_t ucs2_t;
#endif 

namespace std {
	typedef basic_string<ucs2_t, char_traits<ucs2_t>, allocator<ucs2_t> > _ustring;
}

#define _64BIT_BYTES_ 8
template <typename _Base_T>
struct _64BIT_BYTES_T 
{
	signed int	val[2];
	_64BIT_BYTES_T () {
		memset(this, 0, sizeof(val));
	}

	template <typename rightT>
	_64BIT_BYTES_T(const _64BIT_BYTES_T<rightT>  &right) {
		memcpy(this, &right, sizeof(val));
		//*(_Base_T*)this = (rightT)right;
	}

	_64BIT_BYTES_T(const _Base_T  &right) {
		memcpy(this, &right, sizeof(val));
		//*(_Base_T*)this = right;
	}

	operator long long() const {
		//return (long long )*(_Base_T*)this;
		long long llRet;
		memcpy(&llRet, this, sizeof(llRet));
		return llRet;
	}

	operator unsigned long long() const {
		unsigned long long llRet;
		memcpy(&llRet, this, sizeof(llRet));
		return llRet;
	}

	operator double() const {
		double llRet;
		memcpy(&llRet, this, sizeof(llRet));
		return llRet;
	}

	operator unsigned int() const {
		return (unsigned int)*(_Base_T*)this;
	}
	operator int() const {
		return (int)*(_Base_T*)this;
	}

	operator unsigned long() const {
		return (unsigned long)*(_Base_T*)this;
	}
	operator long() const {
		return (long)*(_Base_T*)this;
	}

	template <typename rightT>
	_64BIT_BYTES_T & operator=(const _64BIT_BYTES_T<rightT>  &right) {
		return *(_64BIT_BYTES_T*)&(*(_Base_T*)this = (rightT)right);
	}

	template <typename rightT>
	_64BIT_BYTES_T & operator+=(const _64BIT_BYTES_T<rightT>  &right) {
		return *(_64BIT_BYTES_T*)&(*(_Base_T*)this += (rightT)right);
	}

	template <typename rightT>
	_64BIT_BYTES_T & operator-=(const _64BIT_BYTES_T<rightT>  &right) {
		return *(_64BIT_BYTES_T*)&(*(_Base_T*)this -= (rightT)right);
	}

	template <typename rightT>
	bool operator==(const _64BIT_BYTES_T<rightT>  &right) const {
		return *(_Base_T*)this == (rightT)right;
	}

	template <typename rightT>
	bool operator!=(const _64BIT_BYTES_T<rightT>  &right) const {
		return !operator==(right);
	}

	template <typename rightT>
	bool operator>(const _64BIT_BYTES_T<rightT>  &right) const {
		return *(_Base_T*)this > (rightT)right;
	}
	template <typename rightT>
	bool operator>=(const _64BIT_BYTES_T<rightT>  &right) const {
		return *(_Base_T*)this >= (rightT)right;
	}
	template <typename rightT>
	bool operator<(const _64BIT_BYTES_T<rightT>  &right) const {
		return *(_Base_T*)this < (rightT)right;
	}
	template <typename rightT>
	bool operator<=(const _64BIT_BYTES_T<rightT>  &right) const {
		return *(_Base_T*)this <= (rightT)right;
	}

	template <typename rightT>
	friend _64BIT_BYTES_T operator -(const _64BIT_BYTES_T &c1, const _64BIT_BYTES_T<rightT> &c2) {
		return (_Base_T)c1 - (rightT)c2;
	}

	_Base_T & operator=(const _Base_T  &right) {
		return *(_Base_T*)this = right;
	}

	_Base_T & operator+=(const _Base_T  &right) {
		return *(_Base_T*)this += right;
	}

	_Base_T & operator-=(const _Base_T  &right) {
		return *(_Base_T*)this -= right;
	}

	bool operator==(const _Base_T  &right) const {
		return *(_Base_T*)this == right;
	}

	bool operator!=(const _Base_T  &right) const {
		return !operator==(right);
	}

	bool operator>(const _Base_T  &right) const {
		return *(_Base_T*)this > right;
	}

	bool operator>=(const _Base_T  &right) const {
		return *(_Base_T*)this >= right;
	}

	bool operator<(const _Base_T  &right) const {
		return *(_Base_T*)this < right;
	}

	bool operator<=(const _Base_T  &right) const {
		return *(_Base_T*)this <= right;
	}

	_64BIT_BYTES_T& operator++() {
		++(*(_Base_T*)this);
		return *this;
	}

	_64BIT_BYTES_T& operator++(int) {
		return *(_64BIT_BYTES_T*)&((*(_Base_T*)this)++);
	}

	_64BIT_BYTES_T& operator--() {
		--(*(_Base_T*)this);
		return *this;
	}

	_64BIT_BYTES_T& operator--(int) {
		return *(_64BIT_BYTES_T*)((*(_Base_T*)this)--);
	}

	bool operator==(int right) const {
		return *(_Base_T*)this == right;
	}

	bool operator!=(int right) const {
		return *(_Base_T*)this != right;
	}

	bool operator>(int right) const {
		return *(_Base_T*)this > right;
	}
	bool operator>=(int right) const {
		return *(_Base_T*)this >= right;
	}
	bool operator<(int right) const {
		return *(_Base_T*)this < right;
	}
	bool operator<=(int right) const {
		return *(_Base_T*)this <= right;
	}

};

typedef _64BIT_BYTES_T<double> _xdouble_t;
typedef _64BIT_BYTES_T<long long> _xlong_long_t;
typedef _64BIT_BYTES_T<unsigned long long> _xu_long_long_t;

namespace std {
	typedef _xlong_long_t time64_t;
}
using std::time64_t;

#endif
