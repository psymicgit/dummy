#ifndef _RECHARGEENCOURAGE_H_
#define _RECHARGEENCOURAGE_H_


#include <logic/base/baseobj.h>
#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <common/server/utility.h>


using namespace SGDP;

class CPlayer;
class CActivityEncourage: public CBaseObj
{
public:
    CActivityEncourage();
    ~CActivityEncourage();

public:

    //初始化相关对象
    BOOL Init(DT_ACTIVITY_ENCOURAGE_DATA &stDT_ACTIVITY_ENCOURAGE_DATA, CPlayer* poOwner);

    DT_ACTIVITY_ENCOURAGE_DATA& GeDT_ACTIVITY_ENCOURAGE_DATA()
    {
        return m_stDT_ACTIVITY_ENCOURAGE_DATA;
    };

    //增加活动ID
    VOID AddActivityEncourageID(UINT64 qwActivityID);

    //检测活动ID是否可领取
    BOOL CkActivityEncourage(UINT64 qwActivityID);

    //检测活动是否可领取
    BOOL CkActivityEncourage();

    //获取活动领取可领取个数
    UINT8 GetCanActivityEncourageNum();

    UINT16 OpenRechargeEncourage(PKT_CLILS_OPEN_RECHARGE_ENCOURAGE_ACK &stAck);
    UINT16 RecvRechargeEncourage(PKT_CLIGS_RECV_RECHARGE_ENCOURAGE_ACK &stAck);

    UINT16 OpenScoreEncourage(PKT_CLILS_OPEN_SCORE_ENCOURAGE_ACK &stAck);
    UINT16 RecvScoreEncourage(PKT_CLIGS_RECV_SCORE_ENCOURAGE_ACK &stAck);

	UINT16 GetActivityEncourageInfo(PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK &stAck);
    UINT16 GetActivityEncourageInfo(PKT_CLIGS_GET_ACTIVITY_ENCOURRAGE_INFO_ACK2 &stAck);
    UINT8  GetActivityEncourageNum();
    UINT16 RecvActivityEncourage(PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK &stAck, UINT16 wActivityID, TCHAR aszSeriesNo[MAX_SERIES_NO_NUM], BOOL bCdKeyValid);
	UINT16 RecvActivityEncourage(PKT_CLIGS_RECV_ACTIVITY_ENCOURRAGE_ACK2 &stAck, UINT64 qwActivityID, TCHAR aszSeriesNo[MAX_SERIES_NO_NUM], BOOL bCdKeyValid);
    inline UINT8  GetLastActivityEncourageNum()
    {
        return m_byLastActivityEncourageNum;
    }
private:
    UINT8				m_byLastActivityEncourageNum;//用于判断是否发关闭推送
    DT_ACTIVITY_ENCOURAGE_DATA m_stDT_ACTIVITY_ENCOURAGE_DATA;//充值奖励信息
};

DECLARE_CLASS_POOL_MGR(CActivityEncourage)

#endif
