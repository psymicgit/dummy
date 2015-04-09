
#ifndef _MONEYTREE_H_
#define _MONEYTREE_H_

#include <sdtype.h>
#include <logic/base/baseobj.h>
#include <protocol/server/protocommondata.h>
#include <common/server/utility.h>

struct tagDT_MONEY_TREE_DATA_CLI;
typedef tagDT_MONEY_TREE_DATA_CLI DT_MONEY_TREE_DATA_CLI;

class CPlayer;
class CMoneyTree:public CBaseObj
{
public:
	CMoneyTree();
	~CMoneyTree();
public:
	BOOL Init(DT_MONEY_TREE_DATA& stMoneyTreeData, CPlayer* poOwner);
	DT_MONEY_TREE_DATA& GetDT_MONEY_TREE_DATA() { return m_stDT_MONEY_TREE_DATA; }
	VOID GetDT_MONEY_TREE_DATA_CLI(DT_MONEY_TREE_DATA_CLI& stDT_MONEY_TREE_DATA_CLI, UINT16 &wGetCoinRate2); 
	UINT16 RockMoney(UINT32& dwGetCoin, UINT8 byGuideFlag, DT_MONEY_TREE_DATA_CLI& stMoneyTreeInfo, UINT16 &wGetCoinRate2);

protected:
private:
	DT_MONEY_TREE_DATA	m_stDT_MONEY_TREE_DATA;
};
DECLARE_CLASS_POOL_MGR(CMoneyTree)

#endif

