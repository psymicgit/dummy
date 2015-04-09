
#ifndef _COMMON_H_
#define _COMMON_H_

#include <sdtype.h>

// ---------------- 函  数 ----------------
#ifndef UNICODE
// asc字符串转utf-8字符串
INT32 ASC2UTF(TCHAR* _Output, TCHAR* _Input);

// utf-8字符串转asc字符串
INT32 UTF2ASC(TCHAR* _Output, TCHAR* _Input);
#endif

// 获取当前时间字符串
VOID TimeString(CHAR* _Output);

// 写入文件
INT32 WriteLog(CHAR* _Data);

// 整型数组转字符串
template<typename T>
VOID ArrayToString(T* _Data, UINT32 _Size, CHAR* _Output)
{
	if(NULL == _Data || NULL == _Output)
		return;
	strcpy(_Output, "{");
	for(UINT32 i = 0; i < _Size; i++)
	{
		if(i == _Size - 1)
		{
			sprintf(_Output, "%s%d}", _Output, *(_Data + i));
		}
		else
		{
			sprintf(_Output, "%s%d, ", _Output, *(_Data + i));
		}
	}
}

/**
 * @brief 字符串转数组
 * @param szInput : 待解析的字符串
 * @param cFilter : 过滤字符
 * @param acArray : 数值型数组
 * @param nArrayCount : 数组大小
 * @param nRetCount : 分析的个数
 * @return 转换结果
 */

template<typename T>
BOOL StringToArray(const CHAR* szInput, const CHAR cFilter, T acArray[], INT32 nArrayCount, UINT16& wRetCount )
{
	CHAR* pszTemp = SDNew CHAR[strlen(szInput) + 1];
	strcpy(pszTemp, szInput);
	CHAR* pszBegin = pszTemp;
	CHAR* pszEnd = NULL;
	wRetCount = 0;
	memset(acArray, 0, sizeof(T) * nArrayCount);
	do
	{
		if('\0' == *pszBegin)
		{
			break;
		}
		pszEnd = strchr(pszBegin, cFilter);
		if(pszEnd != NULL)
		{
			*pszEnd = '\0';
			if(nArrayCount > wRetCount)
			{
				acArray[wRetCount++] = SDAtoi(pszBegin);
			}
			else
			{
				SDDelete[] pszTemp;
				return FALSE;
			}
			pszBegin = pszEnd + 1;
		}
		else
		{
			if(nArrayCount > wRetCount)
			{
				acArray[wRetCount++] = SDAtoi(pszBegin);
			}
			break;
		}
	}
	while(TRUE);
	SDDelete[] pszTemp;
	return TRUE;
}



#endif

