
#ifndef _PHYSTRENGTH_H_
#define _PHYSTRENGTH_H_

#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <logic/base/baseobj.h>
#include <common/server/utility.h>
using namespace std ;

class CPlayer;
class CPhystrength: public CBaseObj
{
public:
    CPhystrength();
    ~CPhystrength();
public:
    //初始化相关对象
    BOOL Init(DT_PHYSTRENGTH_DATA& stDT_PHYSTRENGTH_DATA, CPlayer* poOwner);
    DT_PHYSTRENGTH_DATA& GetDT_PHYSTRENGTH_DATA()
    {
        return m_stDT_PHYSTRENGTH_DATA;
    }
    UINT16 BuyPhyStrength(UINT8& byCanBuyNum, UINT32& dwNextBuyGold);
    UINT16 OpenPhyStrength(UINT8& byBuyPhyStrength, UINT8& byTotalBuyTimes, UINT8& byCanBuyNum, UINT16& wVipExtNum, UINT32& dwNextBuyGold);

	BOOL	GetPhyStrengthInfo(DT_PHYSTRENGTH_DATA_CLI& stPhyStrengthInfo);//获取体力信息
	//BOOL	InitPhystrength(DT_PHYSTRENGTH_DATA& stDT_PHYSTRENGTH_DATA);//初始化位置信息
	VOID	PhyStrengthRecover();//体力恢复，由相关协议请求触发，不做定时器
protected:

private:
    DT_PHYSTRENGTH_DATA	m_stDT_PHYSTRENGTH_DATA;

};
DECLARE_CLASS_POOL_MGR(CPhystrength)

#endif //#ifndef 

