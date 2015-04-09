#ifndef _XIANGQIAN_MGR_H
#define _XIANGQIAN_MGR_H
#include <sdtype.h>
#include <sdsingleton.h>
#include <vector>
#include <logic/base/basepropmgr.h>

using namespace std;

class CPlayer;
class CEquip;

struct tagDT_EQUIP_DATA_CLI;
typedef tagDT_EQUIP_DATA_CLI DT_EQUIP_DATA_CLI;

struct SXiangqianItemProp
{
	SXiangqianItemProp()
	{
		dwType=0;
		dwLevel=0;
		dwAddValue=0;
		dwAddPercentValue=0;
		dwItemID=0;
	}
	~SXiangqianItemProp()
	{

	}

	UINT32 dwType;
	UINT32 dwLevel;
	UINT32 dwAddValue;
	UINT32 dwAddPercentValue;
	UINT32 dwItemID;
};

struct SXiangqianEquipDes
{
	string strEquipPos1Des;
	string strEquipPos2Des;
	string strEquipPos3Des;
	string strEquipPos4Des;
};

class CXiangqianMgr : public CBasePopMgr
{
public:
	DECLARE_SINGLETON_PROPMGR(CXiangqianMgr);


public:
	typedef map< UINT32, SXiangqianItemProp > TYPE_MAP_XIANGQIAN_ITEM_PROP;
	virtual EConfigModule	GetConfigModuleType() { return ECM_XIANQIAN ; }
	BOOL Init();
	VOID UnInit();
	
	//ÊÇ·ñ½âËø
	BOOL IsUnlock( UINT8 byIndex, UINT16 wLevel );
	UINT32 GetUnlockNum( UINT16 wLevel );
	BOOL GetAddValue( UINT16 wItemID, UINT32 &dwAddValue, UINT32 &dwAddPercentValue, UINT32 &dwType );
	BOOL IsEquipCanXiangqian( UINT16 wItemID, CEquip* poEquip );
	vector<UINT16> GetXiangqianPosList( UINT16 wItemID );

	UINT16 OnXiangqian( DT_EQUIP_DATA_CLI &equipData, CPlayer *poPlayer, UINT16 wEquipID, 
		UINT16 wEquipIdx,UINT16 wItemID, UINT8 byXiangqianPos, UINT16 wHeroID );

	VOID GetXiangqianEquipDes( char *pszDes1, char *pszDes2, char *pszDes3, char *pszDes4 );
protected:
	BOOL LoadFromDB();

public:
	CXiangqianMgr();
	~CXiangqianMgr();
private:
	vector< UINT16 > m_vecUnlockLevel;
	SXiangqianEquipDes m_xiangqianEuipDes;
};

#endif