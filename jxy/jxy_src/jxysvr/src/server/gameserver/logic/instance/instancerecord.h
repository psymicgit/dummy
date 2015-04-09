
#ifndef _INSTANCERECORD_H_
#define _INSTANCERECORD_H_

#include <protocol/server/protocommondata.h>
#include <logic/base/baseobj.h>
#include "instancepropmgr.h"

using namespace std ;

class CPlayer;

//¸±±¾¼ÇÂ¼
class CInstanceRecord:public CBaseObj
{
public:
	CInstanceRecord();
	~CInstanceRecord();
public:	
	BOOL Init(DT_INSTANCE_DATA* pstDT_INSTANCE_DATA, CPlayer* poOwner);
	DT_INSTANCE_DATA& GetDT_INSTANCE_DATA() { return m_stDT_INSTANCE_DATA;}
	SInstanceProp* GetInstanceProp();

private:
	DT_INSTANCE_DATA		m_stDT_INSTANCE_DATA;
};



#endif //#ifndef 

