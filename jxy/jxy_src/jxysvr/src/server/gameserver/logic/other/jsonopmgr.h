#ifndef _JSONOP_MGR_H_
#define _JSONOP_MGR_H_

#include <sdsingleton.h>
#include <sdtype.h>
#include <sdstring.h>
#include <dll/sdu/sdthread.h>
#include <dll/sddb/sddb.h>
#include <deque>
#include <sdmutex.h>
#include <protocol/server/protocommondata.h>

using namespace SGDP;
using namespace std;

struct SDT_PLAYER_DATA_TRANSID
{
	UINT32 dwTransID;
	DT_PLAYER_DATA stDT_PLAYER_DATA;
};

struct SJVALUE_TRANSID
{
	UINT32 dwTransID;
	string JValue;
};


class CJsonOpMgr:public CSDThread
{
	DECLARE_SINGLETON(CJsonOpMgr);

public:
	CJsonOpMgr();
	virtual ~CJsonOpMgr();
	BOOL Init();
	VOID UnInit();

public:
	VOID	Run();
	VOID	AddJsonParsePlayerData(const DT_PLAYER_DATA& stDT_PLAYER_DATA, UINT32 dwTransID);

protected:
	//Ïß³Ì
	virtual VOID  ThrdProc();

private:
	VOID Stop();
	

private:
	BOOL					m_bStop;
	CSDMutex				m_oJsonParsePlayerDataMutex;
	CSDMutex				m_oParsedPlayerJsonDataMutex;
	deque<SDT_PLAYER_DATA_TRANSID>	m_deqJsonParsePlayerData;
	deque<SJVALUE_TRANSID>	m_deqParsedPlayerJsonData;

};


#endif