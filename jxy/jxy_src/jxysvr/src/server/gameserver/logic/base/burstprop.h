#ifndef _BURSTPROP_H_
#define _BURSTPROP_H_

#include <sdtype.h>
#include <sdhashmap.h>
#include <sdxmlfile.h>
#include <sdstring.h>
#include <common/client/commondef.h>

using namespace SGDP;
using namespace std;

//
struct SProp
{
	//
	union 
	{
		UINT16 wMin;
		UINT16 wKindID;		
	};
	//
	union		
	{
		UINT32 dwMax;
		UINT16 wNum; //道具时
		UINT32 dwValue;//固定值时,金钱体力经验应该为正的
		INT32 nValue;//固定值时
		INT32 nNum; //属性时的value字段
	};
	UINT16 wRate;		//百分比
	BOOL bFirstFalg;  //第一次必给标志（TRUE必须给，一个SBurst中只能有1个）

	SProp() 
	{ 
		wMin = 0;
		dwMax = 0;
		wRate =  0;
		bFirstFalg = FALSE;
	}
};

//爆出/奖励/生产物品配置
struct SBurst
{
	EBurstKind	eBurstKind; //爆出物品类型
	BOOL		bRandom;	//是随机类型(子类型)，FALSE为固定值，取第一条记录的wValue (如 <Gold Value="10" /> )
	vector<SProp*>	vecProp;	//保存每条信息，bRandomType为FALSE(固定值)时只有1条记录;若随机则随机取vecProp中的1条SProp，并使用SProp的wRate随机产生一个值，第一次进入则选FirstFalg为1的值
		/*
		<Coin>
			<Prop Min="1" Max="100" Rate="50" />
			<Prop Min="101" Max="200" Rate="30" />
			<Prop Min="201" Max="300" Rate="20" FirstFalg="1" />
		</Coin>
		*/

	SBurst() 
	{ 
		eBurstKind = EBKIND_INVALID;
		bRandom = FALSE;
	}
	~SBurst();
};


typedef vector<SBurst*> CBurstVec;
typedef CBurstVec::iterator CBurstVecItr;

typedef HashMap<string, EBattleAttributeKind> CStr2BattleAttrKindMap;
typedef CStr2BattleAttrKindMap::iterator CStr2BattleAttrKindMapItr;	

//奖励配置
struct SEncourageProp
{
	CBurstVec		vecBurst;	//奖励的配置
	CBurstVec		vecBurstItemGroup;	//奖励道具的配置(分组，每组1个SBurst)，随机取1个
	/*
	<Item>
		<Group>
			<Prop KindID="10001" Num="1" Rate="50" FirstFalg="1" />
			<Prop KindID="10002" Num="1" Rate="10" />
		</Group>				
		<Group>
			<Prop KindID="10002" Num="1" Rate="50" />
			<Prop KindID="10003" Num="1" Rate="10" />
		</Group>	
		<Group>
			<Prop KindID="10001" Num="1" Rate="50" />
			<Prop KindID="10002" Num="1" Rate="10" />
		</Group>
		<Group KindID="10001" Num="1" />				
	</Item>
	*/
	~SEncourageProp();
};

BOOL ReadRandomBurst(EBurstKind eBurstKind, SBurst* pstBurst, CSDXMLNode xmlNode);
BOOL ReadBurstItem(EBurstKind eBurstKind, CBurstVec& vecBurst, CSDXMLNode xmlNode);
EBattleAttributeKind GetBattleAttrKindByStr(string strAttr);
BOOL ReadEncourageItemGroup(CBurstVec& vecBurst, CSDXMLNode xmlNode);
BOOL ReadEncourage(SEncourageProp* pstEncourageProp, CSDXMLNode xmlNode);////读取奖励

#endif

