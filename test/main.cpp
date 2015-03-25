/*************************************************************************
    > File Name: vectordtor_main.cpp
    > Author: 
    > Mail:  
    > Created Time: Sun 22 Dec 2013 09:56:39 PM PST
************************************************************************/

#include<iostream>
#include <vector>
#include <string>
#include <memory.h>
#include "sdobjectpool.h"
#include <map>

using namespace std;

 struct DT_PLUNDER_BASE_DATA{
    int byHaveDataFlag; //是否有数据(用于DB操作，0则没有数据)
    int dwCaptureOwnerPlayerID; //俘虏你的玩家ID，0表示没有被俘虏
    int qwBeCaptureTime; //被俘虏的时间
    int wExtPlunderNum; //额外增加的掠夺次数
    int qwLastPlunderTime; //上次掠夺时间
    int wLastPlunderNum; //上次(当天)掠夺次数
    int wLastDriveAwayNum; //上次(当天)驱赶俘虏主次数
};

struct SPlunderSaveData
{
    SPlunderSaveData() { dwPlayerID = 0; memset(&stPlunderBaseData, 0, sizeof(stPlunderBaseData));}
    ~SPlunderSaveData(){};

    int dwPlayerID;
    DT_PLUNDER_BASE_DATA stPlunderBaseData; //掠夺基本信息
};

typedef vector<SPlunderSaveData*> PlunderVec;
class CPlayersPlunderSaveData
{
public:
    CPlayersPlunderSaveData(){ m_wErrCode = 0; m_vecPlunderSaveData.clear();}; 
    ~CPlayersPlunderSaveData(){};

	void print()
	{
		std::cout << "current size = " << m_vecPlunderSaveData.size() << std::endl;
		for(PlunderVec::iterator itr = m_vecPlunderSaveData.begin(); itr != m_vecPlunderSaveData.end(); ++itr)
		{
			SPlunderSaveData *p = *itr;
			p->dwPlayerID = 0;
		}
	}

public:
    PlunderVec  m_vecPlunderSaveData;
    int				          m_wErrCode;
};

class Test
{
public:
	void vectordtortest()
	{
		CPlayersPlunderSaveData *playerData = m_oPlayersPlunderSaveDataPool.Alloc();
		SPlunderSaveData plunderData;

		for(int i = 0; i < 1000; i++)
		{
			playerData->m_vecPlunderSaveData.push_back(&plunderData);
		}

		m_oPlayersPlunderSaveDataPool.Free(playerData);

		// playerData->~CPlayersPlunderSaveData();
		playerData = m_oPlayersPlunderSaveDataPool.Alloc();
		playerData->print();
	}

private:
    CSDObjectPool<CPlayersPlunderSaveData> m_oPlayersPlunderSaveDataPool;
};

class Player
{
public:
	Player(int playerId)
		: m_playerId(playerId)
	{
	}

private:
	int m_playerId;
};

typedef std::map<int, Player*> PlayerMap;
class MapEraseTest
{
public:
	void test()
	{
		char dummy[102400 * 1024] = {0};
		int len = sizeof(dummy);
		dummy[sizeof(dummy) - 1] = '\0';

		for(int i = 0; i < len; i++)
		{
			dummy[i] = (char)i;
		}

		std::cout << "dummy size = " << sizeof(dummy) << std::endl;

		PlayerMap players;
		for(int i = 1; i < 10000; i++)
		{
			players[i] = new Player(i);
		}


		for(int j = 1; j <= 10000; j++)
		{
			m_players = players;
			for(PlayerMap::iterator itr = m_players.begin(); itr != m_players.end(); )
			{
				m_players.erase(itr++);	
			}
		}


		if(m_players.empty())
		{
			std::cout << "ok: playermap is empty" << std::endl;
		}
		else
		{
			std::cout << "fail: playermap is not empty" << std::endl;
		}
	}

private:
	PlayerMap m_players;
};

int main(int argc, char *argv[])
{
	MapEraseTest test;
	test.test();

	return 0;
}
