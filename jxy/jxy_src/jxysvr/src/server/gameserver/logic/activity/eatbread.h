
#ifndef _EATBREAD_H_
#define _EATBREAD_H_

#include <sdtype.h>
#include <logic/base/baseobj.h>
#include <protocol/server/protocommondata.h>
#include <common/server/utility.h>

#include "activity.h"

struct tagPKT_CLIGS_OPNE_EAT_BREAD_ACK;
typedef struct tagPKT_CLIGS_OPNE_EAT_BREAD_ACK PKT_CLIGS_OPNE_EAT_BREAD_ACK;


struct tagDT_FUNC_COMMON_PROMPT;
typedef struct tagDT_FUNC_COMMON_PROMPT DT_FUNC_COMMON_PROMPT;

class CPlayer;
class CEatBread:public CActivity
{
public:
	CEatBread();
	~CEatBread();
public:
	BOOL Init(const DT_EAT_BREAD_DATA& stDT_EAT_BREAD_DATA, CPlayer* poOwner);
	DT_EAT_BREAD_DATA& GetDT_EAT_BREAD_DATA() { return m_stDT_EAT_BREAD_DATA; }
	virtual VOID GetDT_ACTIVITY_BASE_DATA(DT_ACTIVITY_BASE_DATA& stDT_ACTIVITY_BASE_DATA);
	UINT16 OpenEatBread(PKT_CLIGS_OPNE_EAT_BREAD_ACK& pstAck);
	UINT16 ObtainPhystrengthen(UINT8 byHaveObtainPhyStrengthenCli, UINT32 dwHaveObtainCoinCli, UINT8& byHaveObtainPhyStrengthen, UINT32& dwHaveObtainCoin, UINT16& wHaveUseTime);
	UINT16 LeaveEatBread(UINT8 byHaveObtainPhyStrengthenCli, UINT32 dwHaveObtainCoinCli, UINT8& byHaveObtainPhyStrengthen, UINT32& dwHaveObtainCoin);
	BOOL GetPrompt(DT_FUNC_COMMON_PROMPT& stPrompt); //有且未完成活动则返回TRUE

	UINT16 GetPhystrengthActivityData(UINT8& byActivityNum, DT_ACTIVITY_BASE_DATA astActivityInfo[MAX_ACTIVITY_NUM]); //获取体力活动
//	UINT16 OpenEatBread(PKT_CLIGS_OPNE_EAT_BREAD_ACK& pstAck);
//	UINT16 ObtainPhystrengthen(UINT8 byHaveObtainPhyStrengthenCli, UINT32 dwHaveObtainCoinCli, UINT8& byHaveObtainPhyStrengthen, UINT32& dwHaveObtainCoin, UINT16& wHaveUseTime);
//	UINT16 LeaveEatBread(UINT8 byHaveObtainPhyStrengthenCli, UINT32 dwHaveObtainCoinCli, UINT8& byHaveObtainPhyStrengthen, UINT32& dwHaveObtainCoin);


protected:
	VOID Enter(PKT_CLIGS_OPNE_EAT_BREAD_ACK& pstAck);
private:
	DT_EAT_BREAD_DATA	m_stDT_EAT_BREAD_DATA;
};
DECLARE_CLASS_POOL_MGR(CEatBread)

#endif

