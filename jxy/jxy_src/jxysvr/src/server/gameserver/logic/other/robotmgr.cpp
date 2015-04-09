#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/player/playermgr.h>
#include <logic/player/playergetdatamgr.h>
#include <logic/plunder/plundermgr.h>
#include <logic/race/racepropmgr.h>
#include "robotmgr.h"
#include <logic/player/playergetdatamgr.h>

#include "logic/other/errmsgmgr.h"
#include <protogsdb.h>
#include <logic/plunder/plunderbasepropmgr.h>


IMPLEMENT_SINGLETON(CRobotMgr);

CRobotMgr::CRobotMgr()
{

}

CRobotMgr::~CRobotMgr()
{
}

BOOL CRobotMgr::Init()
{
    InitPlunderRobot();
    InitRaceRobot();

    return TRUE;
}

VOID CRobotMgr::UnInit()
{

}


VOID CRobotMgr::InitPlunderRobot()
{
    //注册机器人
    string strDspName[MAX_ROBOT_NUM];
    strDspName[0] = CMsgDefMgr::Instance()->GetErrMsg("ROBIT_01");
    strDspName[1] = CMsgDefMgr::Instance()->GetErrMsg("ROBIT_02");
    strDspName[2] = CMsgDefMgr::Instance()->GetErrMsg("ROBIT_03");
    strDspName[3] = CMsgDefMgr::Instance()->GetErrMsg("ROBIT_04");
    strDspName[4] = CMsgDefMgr::Instance()->GetErrMsg("ROBIT_05");
    strDspName[5] = CMsgDefMgr::Instance()->GetErrMsg("ROBIT_06");
    strDspName[6] = CMsgDefMgr::Instance()->GetErrMsg("ROBIT_07");
    strDspName[7] = CMsgDefMgr::Instance()->GetErrMsg("ROBIT_08");
    strDspName[8] = CMsgDefMgr::Instance()->GetErrMsg("ROBIT_09");
    strDspName[9] = CMsgDefMgr::Instance()->GetErrMsg("ROBIT_10");
    UINT32 dwPlunderRobotStartID = CPlunderBasePropMgr::Instance()->GetPlunderBaseProp().dwPlunderRobotStartID;
    for(UINT8 byIdx = 0; byIdx < MAX_ROBOT_NUM; byIdx++)
    {
        UINT32 dwID = dwPlunderRobotStartID + byIdx;
        if(NULL == CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwID))
        {
            CPlayerMgr::Instance()->CreateNewPlayerGM(dwID, ERFK_PLUNDER, 1, (char*)strDspName[byIdx].c_str());
            //将机器人读入内存中方可修改功能
            CGetPlayerDataMgr::Instance()->GetPlayerData(dwID, GET_PLAYERDATATYPE_SYNCDATA, dwID);
        }
    }

}

VOID CRobotMgr::InitRaceRobot()
{
    string strDspName = CMsgDefMgr::Instance()->GetErrMsg("ROBIT_11");
    //注册机器人
    UINT32 dwRobotID = CRacePropMgr::Instance()->GetRaceProp().dwRobotID;
    if(NULL == CPlayerBaseDataCacheMgr::Instance()->GetPlayerBaseData(dwRobotID))
    {
        CPlayerMgr::Instance()->CreateNewPlayerGM(dwRobotID, ERFK_RACE, 1, (char*)strDspName.c_str());
        //将机器人读入内存中方可修改功能
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwRobotID, GET_PLAYERDATATYPE_SYNCDATA, dwRobotID);
    }
}


