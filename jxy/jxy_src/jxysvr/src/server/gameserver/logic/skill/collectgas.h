
#ifndef _COLLECTGAS_H_
#define _COLLECTGAS_H_


#include <sdtype.h>
#include <logic/base/baseobj.h>
#include <protocol/server/protocommondata.h>
#include <common/server/utility.h>
using namespace std;


class CPlayer;

struct tagDT_GEN_GAS_DATA_CLI;
typedef tagDT_GEN_GAS_DATA_CLI DT_GEN_GAS_DATA_CLI;

struct tagPKT_CLIGS_OPEN_GEN_GAS_TAB_ACK;
typedef tagPKT_CLIGS_OPEN_GEN_GAS_TAB_ACK PKT_CLIGS_OPEN_GEN_GAS_TAB_ACK;

class CCollectGas:public CBaseObj
{
public:
	CCollectGas();
	~CCollectGas();
public:
	BOOL	Init(DT_COLLECT_GAS_DATA& stCollectData, CPlayer* poOwner);
	DT_COLLECT_GAS_DATA& GetDT_COLLECT_GAS_DATA() { return m_stCollectData; }	
	//////////////////////////////
	UINT16	GenGas(UINT8 byFlag, DT_GEN_GAS_DATA_CLI& stDT_GEN_GAS_DATA_CLI);
	UINT16	UpGasLevel(UINT8 byFullLevelFlag, DT_GEN_GAS_DATA_CLI& stDT_GEN_GAS_DATA_CLI);
	UINT16	CollectGas(DT_GEN_GAS_DATA_CLI& stDT_GEN_GAS_DATA_CLI);	
	UINT16	OpenGenGasTab(PKT_CLIGS_OPEN_GEN_GAS_TAB_ACK& stAck);

protected:
	UINT16	GetMaxGenNum();
	UINT16	CanGenNum(/*UINT8 byPurpleFlag*/);
	VOID		GetDT_GEN_GAS_DATA_CLI(DT_GEN_GAS_DATA_CLI& stDT_GEN_GAS_DATA_CLI);
	VOID		Recover();
private:
	DT_COLLECT_GAS_DATA m_stCollectData;
};
DECLARE_CLASS_POOL_MGR(CCollectGas);

#endif

