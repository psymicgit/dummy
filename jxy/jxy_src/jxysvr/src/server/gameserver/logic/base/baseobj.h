
#ifndef _BASEOBJ_H_
#define _BASEOBJ_H_

#include <sdtype.h>

class CPlayer;
class CBaseObj
{
public:
	CBaseObj() { m_poOwner = NULL; };
	~CBaseObj() { m_poOwner = NULL; };
public:
	CPlayer* GetOwner() { return m_poOwner; }
	VOID	 SetOwner(CPlayer* poPlayer)
	{ 
		m_poOwner = poPlayer; 
	}
protected:	
	CPlayer*			m_poOwner;
};


#endif

