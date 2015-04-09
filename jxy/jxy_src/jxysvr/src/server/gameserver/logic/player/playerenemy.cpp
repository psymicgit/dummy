#include "player.h"
#include <common/client/errdef.h>

#include <logic/enemy/enemymgr.h>


bool sort_enemy_time( CEnemy* p1,  CEnemy* p2)
{
    return p1->GetDT_ENEMY_DATA().qwAddTime > p2->GetDT_ENEMY_DATA().qwAddTime;
}

//初始化仇敌
BOOL CPlayer::InitEnemy(DT_ENEMY_DATA_LIST& stEnemyData)
{
    //按添加的仇敌的时间升序排序，则越新的仇敌越后面
    UINT8 byEnemyNum = stEnemyData.byEnemyNum;
    for(UINT8 byIdx = 0; byIdx < byEnemyNum && byIdx < MAX_ENEMY_NUM; byIdx++)
    {
        DT_ENEMY_DATA* pstDT_ENEMY_DATA = &stEnemyData.astEnemyInfo[byIdx];
        if(NULL == pstDT_ENEMY_DATA)
        {
            continue;
        }
        if(m_mapEnemy.GetData(pstDT_ENEMY_DATA->dwEnemyPlayerID))
        {
            continue;
        }
        CEnemy* poEnemy = CEnemyMgr::Instance()->CreateEnemy(m_dwPlayerID, pstDT_ENEMY_DATA->dwEnemyPlayerID);
        if(NULL == poEnemy)
        {
            return NULL;
        }
        if(FALSE == poEnemy->Init(pstDT_ENEMY_DATA, this))
        {
            CEnemyMgr::Instance()->RemoveEnemy(m_dwPlayerID, pstDT_ENEMY_DATA->dwEnemyPlayerID);
            continue;//跳过有问题数据
        }
        //DT_ENEMY_DATA& stDT_ENEMY_DATA = poEnemy->GetDT_ENEMY_DATA();
        //while(m_mapTimeEnemy.find(stDT_ENEMY_DATA.qwAddTime) != m_mapTimeEnemy.end())
        //{
        //    stDT_ENEMY_DATA.qwAddTime++;
        //}
        //m_mapTimeEnemy[stDT_ENEMY_DATA.qwAddTime] = poEnemy;
        m_mapEnemy.AddData(pstDT_ENEMY_DATA->dwEnemyPlayerID, poEnemy);
        m_vecEnemy.push_back(poEnemy);
        //DBG_INFO(_SDT("[%s: %d]: InitEnemy(playerid:%u, EnemyPlayerID:%u)!"), MSG_MARK, m_dwPlayerID, pstDT_ENEMY_DATA->dwEnemyPlayerID);
    }
    sort(m_vecEnemy.begin(), m_vecEnemy.end(), sort_enemy_time);
    return TRUE;
}


//添加仇敌，若已存在，则交战次数++
CEnemy* CPlayer::AddEnemy(UINT32 dwEnemyID)
{
    //机器人,不需记录
    if(0 != m_stDT_PLAYER_BASE_DATA.byRobotFunc)
    {
        return NULL;
    }

    CEnemy* poEnemy = m_mapEnemy.GetData(dwEnemyID);
    if(NULL != poEnemy)
    {
        DT_ENEMY_DATA& stDT_ENEMY_DATA = poEnemy->GetDT_ENEMY_DATA();
        stDT_ENEMY_DATA.wBattleNum++;
        stDT_ENEMY_DATA.qwAddTime = SDTimeSecs();
    }
    else
    {
        poEnemy = CEnemyMgr::Instance()->CreateEnemy(m_dwPlayerID, dwEnemyID);
        if(NULL == poEnemy)
        {
            return NULL;
        }
        DT_ENEMY_DATA stDT_ENEMY_DATATmp;
        stDT_ENEMY_DATATmp.wBattleNum = 1;
        stDT_ENEMY_DATATmp.dwEnemyPlayerID = dwEnemyID;
        stDT_ENEMY_DATATmp.qwAddTime = SDTimeSecs();
        if(FALSE == poEnemy->Init(&stDT_ENEMY_DATATmp, this))
        {
            CEnemyMgr::Instance()->RemoveEnemy(m_dwPlayerID, dwEnemyID);
            return NULL;
        }
        m_mapEnemy.AddData(dwEnemyID, poEnemy);
        m_vecEnemy.push_back(poEnemy);
    }
    sort(m_vecEnemy.begin(), m_vecEnemy.end(), sort_enemy_time);

    //达到最大个数，清除第1个
    if(m_vecEnemy.size() >= MAX_ENEMY_NUM)
    {
        vector<CEnemy*>::reverse_iterator itr  = m_vecEnemy.rbegin();
        if (itr != m_vecEnemy.rend())
        {
            CEnemy* poEnemy = *itr;
            if(poEnemy)
            {
                OnDelEnemy(poEnemy->GetDT_ENEMY_DATA().dwEnemyPlayerID);
            }
        }
    }


    //DBG_INFO(_SDT("[%s: %d]: AddEnemy(playerid:%u, EnemyPlayerID:%u) id:%u!"), MSG_MARK, m_dwPlayerID, dwEnemyID, poEnemy->GetOwner()->GetDT_PLAYER_BASE_DATA().dwID);
    //poEnemy->SetObjDataSaveType(EODST_INSERT);
    //poEnemy->SetSaveState(ESS_WANT_SAVE);

    return poEnemy;
}

//清除仇敌
BOOL CPlayer::OnDelEnemy(UINT32 dwEnemyID)
{
    CEnemy* poEnemy = m_mapEnemy.GetData(dwEnemyID);
    if(NULL != poEnemy)
    {
        for(vector<CEnemy*>::iterator itr = m_vecEnemy.begin(); (itr != m_vecEnemy.end()); itr++)
        {
            if(poEnemy == *itr)
            {
                m_vecEnemy.erase(itr);
                break;
            }
        }
        m_mapEnemy.DelData(dwEnemyID);
        CEnemyMgr::Instance()->RemoveEnemy(m_dwPlayerID, poEnemy->GetDT_ENEMY_DATA().dwEnemyPlayerID);
        sort(m_vecEnemy.begin(), m_vecEnemy.end(), sort_enemy_time);
        return TRUE;
    }


    DBG_INFO( _SDT( "del enemy id:%d" ), dwEnemyID );
    return FALSE;
}

UINT16 CPlayer::GetEnemyInfo(UINT16 wStartIdx, UINT16& wTotalNum, UINT16& wCurNum, DT_ENEMY_DATA_CLI astEnemyInfo[MAX_GET_ENEMY_NUM])
{
    wCurNum = 0;
    UINT8 bySize = m_mapEnemy.Size();
    wTotalNum = bySize;
    //没有仇敌
    if(0 == m_vecEnemy.size())
    {
        return ERR_OPEN_ENEMY_TAB::ID_SUCCESS;
    }
    if((UINT32)(wStartIdx + 1) > m_vecEnemy.size())
    {
        return ERR_OPEN_ENEMY_TAB::ID_IDX_INVALID;
    }

    UINT32 dwIdx = 0;
    for(vector<CEnemy*>::iterator  itr = m_vecEnemy.begin(); ((itr != m_vecEnemy.end()) && (wCurNum < MAX_GET_ENEMY_NUM)); itr++)
    {
        if(dwIdx >= wStartIdx)
        {
            CEnemy* poEnemy = *itr;
            if(poEnemy)
            {
                if(poEnemy->GetDT_ENEMY_DATA_CLI(astEnemyInfo[wCurNum]))
                {
                    wCurNum++;
                }
            }
        }
        dwIdx++;
    }

    ////最新的放在vector后面，给客户端的数据最新的方前面，则从后向前取
    //for(UINT8 byIdx = bySize - (wStartIdx + 1); ((byIdx < bySize) && (wCurNum < MAX_GET_ENEMY_NUM)); byIdx--)
    //{
    //	CEnemy* poEnemy = m_vecEnemy[byIdx];
    //	if(poEnemy)
    //	{
    //		if(poEnemy->GetDT_ENEMY_DATA_CLI(astEnemyInfo[wCurNum]))
    //		{
    //			wCurNum++;
    //		}
    //	}
    //}

    return ERR_OPEN_ENEMY_TAB::ID_SUCCESS;
}


CEnemy* CPlayer::GetEnemy(UINT32 dwEnemyID)
{
    return m_mapEnemy.GetData(dwEnemyID);
}

UINT16 CPlayer::DelEnemy(UINT32 dwEnemyID, UINT16& wStartIdx, UINT16& wTotalNum, UINT16& wCurNum, DT_ENEMY_DATA_CLI astEnemyInfo[MAX_GET_ENEMY_NUM])
{

    UINT16 wSize = m_vecEnemy.size();

    //最后1页且不是第一页的第一个，则要返回上一页
    if((0 == wStartIdx % MAX_GET_ENEMY_NUM) && (wStartIdx + 2 > wSize) && (wSize > MAX_GET_ENEMY_NUM))
    {
        wStartIdx = wStartIdx - MAX_GET_ENEMY_NUM;
    }
    if(!OnDelEnemy(dwEnemyID))
    {
        return ERR_DEL_ENEMY::ID_ENEMY_NOT_EXIST;
    }

    return GetEnemyInfo(wStartIdx, wTotalNum, wCurNum, astEnemyInfo);
}

vector<CEnemy *> CPlayer::GetEnemyList()
{
    return m_vecEnemy;
}