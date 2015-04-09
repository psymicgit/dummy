
#ifndef _ROBOTMGR_H_
#define _ROBOTMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>


class CRobotMgr
{
public:

    DECLARE_SINGLETON(CRobotMgr);

public:

    BOOL Init();

    VOID UnInit();
public:

protected:
    CRobotMgr();
    virtual ~CRobotMgr();
protected:
	VOID	InitPlunderRobot();
	VOID	InitRaceRobot();
protected:

    //BOOL LoadFromDB();

private:

};

#endif // 
