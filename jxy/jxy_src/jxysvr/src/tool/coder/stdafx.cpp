#include "stdafx.h"

bool IsNumber(const char *pszIn)
{
	const char *ptr = pszIn;
	while(*ptr)
	{
		if(*ptr < '0' || *ptr > '9' )
			return false;

		ptr++;
	}

	return true;
}

bool IsHex(const char *pszIn)	//16进制
{
	const char* ptr = pszIn;

	if (strlen(ptr) > 2 && ptr[0] == '0' && (ptr[1]=='x' || ptr[1] == 'X'))
	{
		ptr = ptr + 2;
		while (*ptr)
		{
			if ((*ptr>= '0' && *ptr<= '9') || (*ptr>='a' && *ptr<='f') || (*ptr>='A' && *ptr<='F'))
			{
				ptr++;
				continue;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	return false;
}
bool IsOctal(const char *pszIn)	//8进制
{
	const char* ptr = pszIn;

	if (strlen(ptr) > 1 && ptr[0] == '0')
	{
		ptr = ptr + 2;
		while (*ptr)
		{
			if ((*ptr>= '0' && *ptr<= '7'))
			{
				ptr++;
				continue;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	return false;
}


void ConvertUtf8ToGBK(CString& strUtf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);

	strUtf8 = szGBK;
	delete[] szGBK;
	delete[] wszGBK;
}


void ConvertGBKToUtf8(CString& strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL, 0);
	unsigned short * wszUtf8 = new unsigned short[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = new char[len + 1];
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL, NULL);

	strGBK = szUtf8;
	delete[] szUtf8;
	delete[] wszUtf8;
}

INT32 g_nCharsetHost = 0;
