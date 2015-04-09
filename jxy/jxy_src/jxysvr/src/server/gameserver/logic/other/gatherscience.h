
#ifndef _GATHER_SCIENCE_H_
#define _GATHER_SCIENCE_H_

#include <protocol/server/protocommondata.h>
#include "protocol/server/protocligs.h"
#include <logic/base/baseobj.h>
#include <common/server/utility.h>
using namespace std ;

class CPlayer;
class CGatherScience:public CBaseObj
{
public:
	CGatherScience();
	~CGatherScience();
public:
	//初始化相关对象
	BOOL Init(DT_GATHER_SCIENCE_DATA &stDT_GATHER_SCIENCE_DATA, CPlayer* poOwner);
	DT_GATHER_SCIENCE_DATA& GetDT_GATHER_SCIENCE_DATA() { return m_stDT_GATHER_SCIENCE_DATA; }


	UINT16	OpenGetherScience(DT_GATHER_SCIENCE_DATA_CLI &stGatherScience);
	UINT16	GetherScience(DT_GATHER_SCIENCE_DATA_CLI &stGatherScience, UINT8 byGatherType);
protected:
	//BOOL		InitGatherScience(DT_GATHER_SCIENCE_DATA& stDT_GATHER_SCIENCE_DATA);//初始化信息

	//DECLARE_DESTORY(GatherScience)

	BOOL		RecovryGatherScience();//恢复数据

	VOID		GetDT_GATHER_SCIENCE_DATA_CLI(DT_GATHER_SCIENCE_DATA_CLI &stDT_GATHER_SCIENCE_DATA_CLI);

private:
	DT_GATHER_SCIENCE_DATA	m_stDT_GATHER_SCIENCE_DATA;

};

DECLARE_CLASS_POOL_MGR(CGatherScience)
#endif //#ifndef 

