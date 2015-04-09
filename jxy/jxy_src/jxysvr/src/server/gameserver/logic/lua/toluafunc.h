#ifndef _TOLUAFUNC_H_
#define _TOLUAFUNC_H_

#include <sdtype.h>

class CPlayer;

UINT64 GetTimeValue(const CHAR* pszDateTime); //日期格式字符串如 2013/11/28 17:23:37

UINT32 GetRank(CPlayer* poPlayer); //获取排名，0表示没有排名

INT32 LuaRandomReseed(UINT32 dwRange); //重置种子并随机

#endif

