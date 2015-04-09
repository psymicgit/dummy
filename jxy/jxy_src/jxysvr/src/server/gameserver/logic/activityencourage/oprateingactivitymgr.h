#ifndef _OPRATEINGACTIVITYMGR_H_
#define _OPRATEINGACTIVITYMGR_H_

#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>

#include <list>
#include <logic/base/basepropmgr.h>
#include <db/getoprateingactivitycmd.h>


using namespace SGDP;


class CPlayer;

struct SBattleRcd
{
    SBattleRcd()
    {
        memset(this, 0, sizeof(SBattleRcd));
    };
    ~SBattleRcd() {};
    TCHAR			szBattleKey[MAX_COMMON_TXT_LEN];
    UINT64			qwBattleTime;
    PKT_CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5 stBattleAck;
};

typedef map<UINT32, SBattleRcd> CBattleRcdMap;
typedef CBattleRcdMap::iterator CBattleRcdMapItr;


class COprateingActivityMgr
{
public:
    DECLARE_SINGLETON(COprateingActivityMgr)

    BOOL Init();

    VOID UnInit();

    UINT32 OnEvent();

public:
    VOID SetOprateingActivityData(SOprateingActivityProp stProp);

    VOID GetOprateingActivityData(CPlayer* poPlayer, DT_OPERATING_ACTIVITY_DATA& stOperatingActivityInfo);

	BOOL IsOpenSoulAty(CPlayer* poPlayer);

    BOOL PhpBattle(CPlayer* poPlayer, UINT32 dwActivityVersionID, UINT8 byTownerIdx, UINT8 byInstanceIdx, string& strRet);
    VOID CliBattle(CPlayer* poPlayer, string strBattleKey, PKT_CLIGS_OPERATING_ACTIVITY_BATTLE_ACK5* pstAck);
    VOID CloseActivityOnEnterInstance(CPlayer* poPlayer); //配合客户端进入副本时关闭活动显示
    VOID OpenActivityOnEnterHome(CPlayer* poPlayer); //配合客户端进入家园时活动显示

protected:
    COprateingActivityMgr();
    ~COprateingActivityMgr();

protected:
    BOOL	InitOprateingActivityFromDB();
    VOID	CkTimeOutBattleRcd();
private:
    SOprateingActivityProp		m_stOprateingActivityProp;
    UINT16					    m_wZoneID;
    CBattleRcdMap				m_mapBattleRcd;
	BOOL					    m_bBeginSendFlag;//发送状态
	BOOL					    m_bEndSendFlag;

};
#endif
