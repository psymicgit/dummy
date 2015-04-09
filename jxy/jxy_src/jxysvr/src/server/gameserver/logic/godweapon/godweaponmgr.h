
#ifndef _GODWEAPONMGR_H_
#define _GODWEAPONMGR_H_

#include "godweapon.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>

using namespace SGDP;
using namespace std;


typedef HashMap<UINT64, CGodWeapon*> CID2GodWeaponMap;
typedef CID2GodWeaponMap::iterator CID2GodWeaponMapItr;

class CPlayer;
class CGodweaponMgr
{
public:
    DECLARE_SINGLETON(CGodweaponMgr)

    BOOL Init();

    VOID UnInit();

public:

    CGodWeapon*		CreateGodWeapon(UINT32 dwPlayerID, UINT16 wHeroKindID);

    VOID			RemoveGodWeapon(UINT32 dwPlayerID, UINT16 wHeroKindID);

public:
    UINT16			OpenGodweapon(CPlayer* poPlayer, UINT16& wGodweaponNum, DT_GODWEAPON_DATA_CLI astGodweaponLst[MAX_HERO_NUM], UINT16& wUpgradeQualityItemNum, UINT32& dwUpgradeQualityItemPrice, 
		UINT16& wUpgradeQualityItemID, UINT32& dwActiveCoin, TCHAR aszExtContext[MAX_COMMON_TXT_LEN]);
	UINT16			OpenGodWeaponByPlayerInfo(CPlayer* poPlayer, UINT16& wGodweaponNum, DT_GODWEAPON_DATA_CLI astGodweaponLst[MAX_HERO_NUM], UINT8 byLockStatus[MAX_HERO_NUM]);
    UINT16			UpgradeGodweapon(CPlayer* poPlayer, UINT16 wHeroKindID,	UINT16 wQuality, UINT16 wLevel, DT_GODWEAPON_DATA_CLI& stGodweaponInfo, UINT16& wUpgradeQualityItemNum);
    UINT16			FreeGodweapon(CPlayer* poPlayer, UINT16 wHeroKindID, DT_GODWEAPON_DATA_CLI &stGodweaponInfo, BOOL bUseGold, BOOL bIsFreeUseGold = FALSE/* 免费使用元宝释放 */);
public:
	//VOID			UpgradeGodweapon4Enc(CPlayer* poPlayer); //0831新版神器等级修改补偿转换

protected:
    CGodweaponMgr();
    ~CGodweaponMgr();
protected:
    UINT16			NewGodweapon(CPlayer* poPlayer, CHero* poHero, DT_GODWEAPON_DATA_CLI& stGodweaponInfo);//新增开启器魂

protected:
	//VOID			UpgradeGodweaponByScience(CPlayer* poPlayer, CGodWeapon* poGodweapon, string& ssTxt, UINT8 byIdx);
	//VOID			GetTop3(CGodWeapon* poGodweapon, CGodWeapon* apoGodWeapon[3]);
private:
    CID2GodWeaponMap					m_mapID2GodWeapon;
    CSDObjectPool<CGodWeapon, CSDMutex>	m_oGodWeaponShmemObjMgr;
    UINT32							m_dwUpgradeQualityItemPrice;//进阶符价格
    UINT16							m_wUpgradeQualityItemID;//进阶符ID
	UINT32							m_dwActiveCoin;//激活铜币
};




#endif //#ifndef _ROLEMGR_H_

