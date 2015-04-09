#ifndef _UPDATE_ATTR_H
#define _UPDATE_ATTR_H

#include <sdtype.h>

struct tagDT_HERO_UPDATE_ATTR_DATA;
typedef tagDT_HERO_UPDATE_ATTR_DATA DT_HERO_UPDATE_ATTR_DATA;

class CPlayer;

class CUpdateAttr
{
public:
	CUpdateAttr();
	~CUpdateAttr();
public:
	//初始化相关对象
	BOOL Init(DT_HERO_UPDATE_ATTR_DATA* pstDT_UPDATE_ATTR, CPlayer* poOwner);
private:
	CPlayer*				m_poOwner;
};

#endif