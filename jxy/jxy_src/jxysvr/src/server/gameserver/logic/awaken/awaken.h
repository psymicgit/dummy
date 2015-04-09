#ifndef _AWAKEN_H_
#define _AWAKEN_H_

#include <logic/base/baseobj.h>
#include <protocligs.h>
#include <logic/base/basedefine.h>
#include <common/server/utility.h>

using namespace SGDP;
using namespace std;


class CPlayer;
class CHero;

class CAwaken: public CBaseObj
{
public:
    CAwaken();
    ~CAwaken();

public:

    //初始化相关对象,不包括已穿戴的心得
    BOOL Init(DT_AWAKEN_DATA& stAwakenData, CPlayer* poOwner);
    DT_AWAKEN_DATA& GetDT_AWAKEN_DATA()
    {
        return m_stDT_AWAKEN_DATA;
    }

    /////////////////////
    UINT16	OpenAwakenTab(DT_AWAKEN_TAB_DATA& stAwakenInfo);
    UINT16	Awaken(UINT8 byQuickFlag, UINT8 byGoldFlag, UINT8& byAwakenNum, DT_AWAKEN_GET_DATA astAwakenGetInfo[MAX_AWAKEN_NUM], DT_AWAKEN_TAB_DATA& stAwakenInfo);//悟道
    UINT16	OpenDressStudyTab(DT_DRESS_STUDY_TAB_DATA& stDressStudyTabInfo);
    UINT16	ConvertExp(UINT8 byGridIdx, UINT8 byAll, UINT8 abySelectConvertColor[MAX_COLOR_NUM], UINT32& dwGenStudyExp, DT_STUDY_DATA_CLI astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM], UINT64& qwCurStudyExp); //转换心得精英
    UINT16	UpgradeStudy(UINT16 wHeroKindID, UINT8 byGridIdx, UINT8 byToLevel, DT_HERO_STUDY_DATA_CLI& stHeroStudyInfo, UINT64& qwCurStudyExp); //升级心得,只能升级伙伴的
    UINT16	DressStudy(UINT16 wHeroKindID, UINT8 byBagGridIdx, UINT8 byHeroGridIdx, DT_STUDY_DATA_CLI astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM], DT_HERO_STUDY_DATA_CLI& stHeroStudyInfo);
	UINT16 OpenPlayerInfoAwaken(PKT_CLIGS_PLAYER_INFO_AWAKEN_ACK& stAck);
    /////////////////////////////GM///////////////////////////
    VOID AddStudyForGm(UINT8 byStudyColorKind, UINT8 byStudyAttrKind, UINT8 byStudyLevel, UINT8 byStudyAttrKind2 = 0, string strDesc = "");
    VOID AddStudyExpForGm(UINT32 dwStudyExp);
    inline UINT8 GetBagIdleSize()
    {
        return m_mapIdleStudyIdx.size();
    }
protected:
    VOID		GetAwakenTabInfo(DT_AWAKEN_TAB_DATA& stAwakenInfo);
    UINT16	AwakenOnce(UINT8 byGoldFlag, UINT8& byAwakenNum, DT_AWAKEN_GET_DATA astAwakenGetInfo[MAX_AWAKEN_NUM]);//悟道
    UINT16	QuickAwaken(UINT8& byAwakenNum, DT_AWAKEN_GET_DATA astAwakenGetInfo[MAX_AWAKEN_NUM]);
    VOID		AwakenStudy(DT_AWAKEN_GET_DATA& stAwakenGetData);// 悟道心得
    UINT16	ConvertStudyExp(UINT8 byGridIdx, UINT32& dwGetExp); //转换心得精英
    VOID		GetBagStudyInfo(DT_STUDY_DATA_CLI astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]);
    VOID		GetHeroStudyDataCli(CHero* poHero, DT_HERO_STUDY_DATA_CLI& stDT_HERO_STUDY_DATA_CLI);
    VOID		GetDT_STUDY_DATA_CLI(DT_STUDY_DATA& stDT_STUDY_DATA, DT_STUDY_DATA_CLI& stDT_STUDY_DATA_CLI);

    /////////////////
    UINT16	GetCanCallNum(); //获取剩余可元宝召唤次数
    UINT16	GetTotalCallNum(); //获取总共元宝召唤次数

////////////////////////////////////////////////// 222222222222222222222222 //////////////////////////////////////////////////
////////////////////////////////////////////////// 222222222222222222222222 //////////////////////////////////////////////////
public:

    /////////////////////
    UINT16	OpenAwakenTab2(PKT_CLIGS_OPEN_AWAKEN_TAB_ACK2& stAwakenInfo);
    UINT16	Awaken2(UINT8 byQuickFlag, UINT8 byGoldFlag, UINT8& byAwakenNum, DT_AWAKEN_GET_DATA2 astAwakenGetInfo[MAX_AWAKEN_NUM], DT_AWAKEN_TAB_DATA2& stAwakenInfo);//悟道
    UINT16	OpenDressStudyTab2(DT_DRESS_STUDY_TAB_DATA2& stDressStudyTabInfo);
    UINT16	ConvertExp2(UINT8 byGridIdx, UINT8 byAll, UINT8 abySelectConvertColor[MAX_COLOR_NUM], UINT32& dwGenStudyExp, DT_STUDY_DATA_CLI2 astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM], UINT64& qwCurStudyExp); //转换心得精英
    UINT16	UpgradeStudy2(UINT16 wHeroKindID, UINT8 byGridIdx, UINT8 byToLevel, DT_HERO_STUDY_DATA_CLI2& stHeroStudyInfo, UINT64& qwCurStudyExp); //升级心得,只能升级伙伴的
    UINT16	DressStudy2(UINT16 wHeroKindID, UINT8 byBagGridIdx, UINT8 byHeroGridIdx, DT_STUDY_DATA_CLI2 astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM], DT_HERO_STUDY_DATA_CLI2& stHeroStudyInfo, UINT8& byHaveSecondsUndress, UINT8& byBagGridIdx2, UINT8& byHeroGridIdx2);
    UINT16	OpenAwakenExchangeTab(UINT8& byExchangeNum, DT_AWAKEN_STUDY_EXCHANGE_DATA astStudyExchangeInfo[MAX_EXCHANGE_STUDY_NUM], UINT64& qwCurCoin, UINT64& qwCurStudyClip);
    UINT16	ExchangeStudy(UINT8 byStudyColorKind, UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2, UINT8 byStudyLevel, UINT8& byStudyBagIdx, DT_AWAKEN_TAB_DATA2& stAwakenTabInfo);
    VOID		AddStudyClip(UINT32 dwExp, UINT16 wOpType);
    VOID		DecStudyClipForGm(UINT32 dwClip);
    UINT16  OnOpenResolveTrigram(UINT8 byPos, PKT_CLIGS_OPEN_RESOLVE_TRIGRAM_ACK &psAck);
    UINT16  OnResolveTrigramReq(UINT8 byPos, UINT8 byUseGold, PKT_CLIGS_RESOLVE_TRIGRAM_ACK &psAck);
    /////////////////////////////GM///////////////////////////
    VOID		AddStudyForGm2(UINT8 byStudyColorKind, UINT8 byStudyAttrKind, UINT8 byStudyLevel);
    VOID		AddStudyExpForGm2(UINT32 dwStudyExp);
    UINT16	DelStudyForGM(UINT8 byGridIdx, string strDesc = "");
protected:
    VOID		GetAwakenTabInfo2(DT_AWAKEN_TAB_DATA2& stAwakenInfo);
    UINT16	AwakenOnce2(UINT8 byGoldFlag, UINT8& byAwakenNum, DT_AWAKEN_GET_DATA2 astAwakenGetInfo[MAX_AWAKEN_NUM]);//悟道
    UINT16	QuickAwaken2(UINT8& byAwakenNum, DT_AWAKEN_GET_DATA2 astAwakenGetInfo[MAX_AWAKEN_NUM]);
    VOID		AwakenStudy2(DT_AWAKEN_GET_DATA2& stAwakenGetData);// 悟道心得
    UINT16	ConvertStudyExp2(UINT8 byGridIdx, UINT32& dwGetExp, string strDesc = ""); //转换心得精英
    VOID		GetBagStudyInfo2(DT_STUDY_DATA_CLI2 astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]);
    VOID		SetCallStudyInfo2(DT_STUDY_DATA* pstDT_STUDY_DATA);
    VOID		GetCallStudyInfo2(DT_STUDY_DATA_CLI2& stCallStudy);
    VOID		GetHeroStudyDataCli2(CHero* poHero, DT_HERO_STUDY_DATA_CLI2& stDT_HERO_STUDY_DATA_CLI);
    VOID		GetDT_STUDY_DATA_CLI2(DT_STUDY_DATA& stDT_STUDY_DATA, DT_STUDY_DATA_CLI2& stDT_STUDY_DATA_CLI);
    VOID        GetIfResolve(UINT8 &byCommon, UINT8 &bySpecial);
	///////////////////////////分解卦象///////////////////////////////////////////////
	VOID		GetBagStudyInfo2NotDouble( DT_STUDY_DATA_CLI2 astBagStudyInfo[MAX_BAG_STUDY_GRID_NUM]);
    /////////////////
    UINT16 GetCanCallNum2(); //获取剩余可元宝召唤次数
    UINT16 GetTotalCallNum2(); //获取总共元宝召唤次数

public:
    UINT8&	GetAwakenVersion()
    {
        return m_byAwakenVersion;
    }

private:
    UINT8			m_byAwakenVersion;
    DT_AWAKEN_DATA	m_stDT_AWAKEN_DATA;
    C88Map			m_mapIdleStudyIdx;//心得背包空闲的位置(数组下标，0开始)
};
DECLARE_CLASS_POOL_MGR(CAwaken)
#endif
