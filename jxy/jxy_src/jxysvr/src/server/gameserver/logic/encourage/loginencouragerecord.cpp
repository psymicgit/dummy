#include "loginencouragerecord.h"
#include "loginencouragemgr.h"
#include "onlineencouragemgr.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>
#include <logic/other/singleprammgr.h>
#include "logic/record/recordmgr.h"
#include <logic/base/basedefine.h>
#include <logic/other/state.h>
#include <logic/bag/bag.h>
#include <logic/item/itemmgr.h>
#include <sdloggerimpl.h>

IMPLEMENT_CLASS_POOL_MGR(CLoginEncourageRecord)
CLoginEncourageRecord::CLoginEncourageRecord()
{
    memset(&m_stDT_LOGIN_ENCOURAGE_RECORD_DATA, 0 , sizeof(m_stDT_LOGIN_ENCOURAGE_RECORD_DATA));
}

CLoginEncourageRecord::~CLoginEncourageRecord()
{

}

BOOL CLoginEncourageRecord::Init(DT_LOGIN_ENCOURAGE_RECORD_DATA &stDT_ONLINE_ENCOURAGE_RECORD_DATA, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }

    memcpy(&m_stDT_LOGIN_ENCOURAGE_RECORD_DATA, &stDT_ONLINE_ENCOURAGE_RECORD_DATA, sizeof(DT_LOGIN_ENCOURAGE_RECORD_DATA));
    m_poOwner = poOwner;

    return TRUE;
}

UINT16 CLoginEncourageRecord::GetEncourageInfo(DT_LOGIN_ENCOURAG_DATA& stLoginEncourageInfo)
{
    CSDDateTime oNow = SDNow();

    //注册昵称后第二天才可以领取登录奖励
    if(0 == m_poOwner->GetDT_PLAYER_BASE_DATA().aszDispName[0])
    {
        stLoginEncourageInfo.byCanRecvNumToday = 0;
        return  ERR_LOGIN_ENCOURAGE::ID_SUCCESS;
    }
    else
    {
            if(IsToday(m_poOwner->GetState().GetDT_STATE_DATA().qwRegisterDispNameTime)) //当天注册的昵称，不可领取
            {
                stLoginEncourageInfo.byCanRecvNumToday = 0;
                return  ERR_LOGIN_ENCOURAGE::ID_SUCCESS;
            }
    }


    UINT8 byLoginEncourageKeepDays = CSinglePramMgr::Instance()->GetLoginEncourageKeepDays();
    //判断时间
    CSDDateTime oLastTime(m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.qwLastRecvTime);

    INT32 nGap = 0;
    //第一次登录奖励
    if(0 == m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.qwLastRecvTime)
    {
        m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.wKeepDays = 1;
    }
    else if(0 == (nGap = oNow.DiffDateDay(oLastTime))) //上次登录时间是 今天
    {
        //上次的未放入背包中（StateFlag状态(LastSelectIndex不为0时有效)(0已下发，1已选择，2已入包)）
        if((0 != m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastSelectIndex) && (2 != m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byStateFlag))
        {
            //未选择，提示客户端未选择，对应服务器来说上次打开已经给ta选择好了，不需再次选择，否则容易被外挂利用重复刷到好道具才放背包中
            if(0 == m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byStateFlag)
            {
                stLoginEncourageInfo.byHaveSelectFlag = 0;
            }
            //已选择，提示客户端未选择
            if(1 == m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byStateFlag)
            {
                stLoginEncourageInfo.byHaveSelectFlag = 1;//已经选择过，不需再次随机选
            }
            stLoginEncourageInfo.byHitIdx = m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastSelectIndex;
            stLoginEncourageInfo.wKeepDays = m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.wKeepDays;
            stLoginEncourageInfo.byCanRecvNumToday = m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.wKeepDays >  byLoginEncourageKeepDays ?
                    (byLoginEncourageKeepDays - m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastRecvNum) : (m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.wKeepDays - m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastRecvNum);
            //取奖励信息
            CLoginEncourageMgr::Instance()->GetEncourageItem(stLoginEncourageInfo.astItemInfo);

            return ERR_LOGIN_ENCOURAGE::ID_SUCCESS;
        }

        if((m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastRecvNum >= m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.wKeepDays) ||
                (m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastRecvNum >= byLoginEncourageKeepDays))
        {
            return ERR_LOGIN_ENCOURAGE::ID_USE_OUT_NUM;
        }
    }
    else if(1 == nGap)//上次登录时间是 昨天，则连续登录次数++
    {
        m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.wKeepDays++;
        m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastRecvNum = 0;
    }
    else //否则连续登录次数重置，从1开始
    {
        m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.wKeepDays = 1;
        m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastRecvNum = 0;
    }

    //记录
    m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.qwLastRecvTime = SDTimeSecs();
    m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastSelectIndex = CLoginEncourageMgr::Instance()->RateHitItem();
    m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byStateFlag = 0;

    stLoginEncourageInfo.byHitIdx = m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastSelectIndex;
    stLoginEncourageInfo.byHaveSelectFlag = 0;
    stLoginEncourageInfo.wKeepDays = m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.wKeepDays;
    stLoginEncourageInfo.byCanRecvNumToday = m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.wKeepDays >  byLoginEncourageKeepDays ?
            (byLoginEncourageKeepDays - m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastRecvNum) : (m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.wKeepDays - m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastRecvNum);

    stLoginEncourageInfo.byHitIdx = CLoginEncourageMgr::Instance()->RateHitItem();

    //取奖励信息
    CLoginEncourageMgr::Instance()->GetEncourageItem(stLoginEncourageInfo.astItemInfo);

    //SetSaveState(ESS_WANT_SAVE);//保存数据

    return  ERR_LOGIN_ENCOURAGE::ID_SUCCESS;
}


VOID CLoginEncourageRecord::OnRate()
{
    //StateFlag状态(LastSelectIndex不为0时有效)(0已下发，1已选择，2已入包)
    if(0 != m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byStateFlag)
    {
        return;
    }

    m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byStateFlag = 1;
    m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastRecvNum++;
    //SetSaveState(ESS_WANT_SAVE);//保存数据
}

UINT16 CLoginEncourageRecord::PutIntoBag(UINT8& byHaveEncourageFlag, DT_LOGIN_ENCOURAG_DATA& stLoginEncourageInfo)
{
    if((m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastSelectIndex < 1) || (m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastSelectIndex > MAX_LOGIN_ENCOURAGE_ITEM_NUM))
    {
        RETURN_OTHER_ERR;
    }

    //StateFlag状态(LastSelectIndex不为0时有效)(0已下发，1已选择，2已入包)
    if(1 != m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byStateFlag)
    {
        return ERR_LOGIN_ENCOURAGE::ID_STATE_INVALID;
    }

    SLoginEncourageProp* pstLoginEncourageProp = CLoginEncourageMgr::Instance()->GetLoginEncourageProp(m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastSelectIndex);
    if(NULL == pstLoginEncourageProp)
    {
        RETURN_OTHER_ERR;
    }
    CBag& oBag = m_poOwner->GetBag();
    if((oBag.GetIdleSize() == 0) && (NULL == oBag.GetGoodsItem(pstLoginEncourageProp->wItemKindID)))
    {
        return ERR_LOGIN_ENCOURAGE::ID_BAG_FULL;
    }

    //若不可以直接使用掉的物品，则放入背包中
    if(!CItemMgr::Instance()->SysUseGoods(m_poOwner, pstLoginEncourageProp->wItemKindID, pstLoginEncourageProp->wPileNum))
    {
        ECreateItemRet eRet;
        if( NULL == CItemMgr::Instance()->CreateItemIntoBag(
                    m_poOwner, pstLoginEncourageProp->wItemKindID, pstLoginEncourageProp->wPileNum, eRet, 0, CRecordMgr::EAIT_LOGINENCOURAGE) )
        {
            //异常
            SYS_CRITICAL(_SDT("[%s: %d]: give login encourage record failed: CreateItemIntoBag failed, PlayerID:%u, ItemKindID = %u, pilenum = %u" ), MSG_MARK, 
                m_poOwner->GetID(), pstLoginEncourageProp->wItemKindID, pstLoginEncourageProp->wPileNum);

            return ERR_LOGIN_ENCOURAGE::ID_BAG_FULL;
        }
    }

    m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byStateFlag = 2;//放入背包状态

    //获取下个奖励信息
    if(ERR_LOGIN_ENCOURAGE::ID_SUCCESS == GetEncourageInfo(stLoginEncourageInfo))
    {
        byHaveEncourageFlag = 1;
    }

    //SetSaveState(ESS_WANT_SAVE);//保存数据

    return ERR_LOGIN_ENCOURAGE::ID_SUCCESS;
}

//登录奖励状态(0,不需显示，1需显示，2需弹出)
UINT8 CLoginEncourageRecord::GetLoginEncourageState()
{


    //注册昵称后第二天才可以领取登录奖励
    if(0 == m_poOwner->GetDT_PLAYER_BASE_DATA().aszDispName[0])
    {
        return 0;
    }
    else
    {
        if(IsToday(m_poOwner->GetState().GetDT_STATE_DATA().qwRegisterDispNameTime)) //当天注册的昵称，不可领取
        {
            return 0;
        }
    }

    UINT8 byLoginEncourageKeepDays = CSinglePramMgr::Instance()->GetLoginEncourageKeepDays();
    //判断时间
    CSDDateTime oNow = SDNow();
    CSDDateTime oLastTime(m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.qwLastRecvTime);

    INT32 nGap = 0;
    //第一次登录奖励
    if(0 == m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.qwLastRecvTime)
    {
        return 2;
    }
    if(0 == (nGap = oNow.DiffDateDay(oLastTime))) //上次登录时间是 今天
    {
        //上次的未放入背包中（StateFlag状态(LastSelectIndex不为0时有效)(0已下发，1已选择，2已入包)）
        if((0 != m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastSelectIndex) && (2 != m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byStateFlag))
        {
            return 1;
        }

        //已经领取完所有次数
        if((m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastRecvNum >= m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.wKeepDays) ||
                (m_stDT_LOGIN_ENCOURAGE_RECORD_DATA.byLastRecvNum >= byLoginEncourageKeepDays))
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else//上次登录时间不是今天,需弹出
    {
        return 2;
    }

    return 0;
}



