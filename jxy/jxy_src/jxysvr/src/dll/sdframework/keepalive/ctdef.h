#ifndef CTDEF_H_
#define CTDEF_H_


#include <sdprocess.h>

using namespace SGDP;

#pragma pack(push, 1)

//进程信息
typedef struct tagProcessDaemon
{
	SDPROCESSID	pid;
	time_t 		starttimer;
	time_t 		lasttimer;
	int 			useFlag; //使用标志
	int 			alive;
}SProcessDaemon;

#pragma pack(pop)
#endif