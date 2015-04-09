
#ifndef _RDCHALLENGEPROPMGR_H_
#define _RDCHALLENGEPROPMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>



class CPlayer;
class CRdChallengePropMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CRdChallengePropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_RDCHALLENGE ; }

    BOOL Init();

    VOID UnInit();
public:
	UINT16 GetMaxChallengePoint() { return m_wMaxChallengePoint; }
	UINT16 GetRdChallengeRefreshCD() { return  m_wRdChallengeRefreshCD; }
	UINT16 GetRdChallengeCD() { return  m_wRdChallengeCD; }
	UINT32 GetRefreshCoin() { return m_dwRefreshCoin; }
	UINT16 GetClrRdChallengeCDGold() { return m_wClrRdChallengeCDGold; }
public:
    CRdChallengePropMgr();
    virtual ~CRdChallengePropMgr();
protected:

    BOOL LoadFromDB();

private:
	UINT16	m_wMaxChallengePoint; //最大挑战点
    UINT16	m_wRdChallengeRefreshCD;//体力恢复1点所需时间(单位秒)
	UINT16	m_wRdChallengeCD;//挑战CD
	UINT32	m_dwRefreshCoin;//刷新铜钱
	UINT16	m_wClrRdChallengeCDGold;//清除挑战CD元宝
};

#endif // 
