#include "player.h"
#include <net/cli/user.h>
#include <net/gt/gtpktbuilder.h>
#include <logic/race/racepropmgr.h>
#include <logic/build/buildpropmgr.h>
#include <logic/item/equip/strengthenpropmgr.h>
#include <logic/item/equip/openlightpropmgr.h>
#include <framework/gsapi.h>
#include <logic/record/recordmgr.h>
#include <logic/lua/luamgr.h>
#include <logic/record/recordmgr.h>
#include <logic/other/singleprammgr.h>
#include <logic/activityencourage/activityencouragemgr.h>
#include <framework/gsconfig.h>
#include <logic/hero/heropropmgr.h>
#include <net/cli/usermgr.h>
#include <logic/task/taskmgr.h>
#include <logic/build/buildmgr.h>
#include <logic/race/racemgr.h>
#include <logic/other/worship.h>
#include <logic/other/rdchallenge.h>
#include "logic/build/build.h"
#include "logic/jingjie/jingjie.h"
#include "logic/hero/hero.h"
#include "logic/yabiao/yabiaoinfo.h"
#include "logic/gvg/local/gvgbuild.h"
#include "logic/item/equip/equipmakemgr.h"


CBuild* CPlayer::NewBuild(UINT8 byKindID)
{
    DT_BUILD_BASE_DATA stDT_BUILD_BASE_DATA;
    memset(&stDT_BUILD_BASE_DATA, 0, sizeof(DT_BUILD_BASE_DATA));
    stDT_BUILD_BASE_DATA.byKindID = byKindID;
    
	//手冲建筑
	if (EBK_RECHARGE == byKindID)
    {
		if (0 == GetDT_PLAYER_BASE_DATA().dwTotalAmount)
		{
			stDT_BUILD_BASE_DATA.wLevel = 1;	//果断冲值标志
		}
		else
		{
			stDT_BUILD_BASE_DATA.wLevel = 2;//领取冲值奖励标志
		}
    }
	else
	{
		stDT_BUILD_BASE_DATA.wLevel = 0;
	}

    stDT_BUILD_BASE_DATA.bySort = CBuildPropMgr::Instance()->GetBuildSort(stDT_BUILD_BASE_DATA.byKindID);
    //stDT_BUILD_BASE_DATA.bySort = m_mapKind2Build.Size() + 1; //按开放顺序

    //判断是否存在该类型建筑，若存在，则配置有问题
    if(NULL != m_mapKind2Build.GetData(byKindID))
    {
        USR_INFO(_SDT("[%s: %d]: CPlayer::NewBuild failed, rebuild %d !"), MSG_MARK, byKindID);
        return NULL;
    }
    CBuild* poBuild = CBuildMgr::Instance()->CreateBuild(m_dwPlayerID, byKindID);
    if(NULL == poBuild)
    {
        return NULL;
    }
    if(FALSE == poBuild->Init(&stDT_BUILD_BASE_DATA, this))
    {
        CBuildMgr::Instance()->RemoveBuild(m_dwPlayerID, byKindID);
        return NULL;
    }
    //poBuild->SetObjDataSaveType(EODST_INSERT);
    //poBuild->SetSaveState(ESS_WANT_SAVE);//数据修改，则需保存

    //若是主城(表示开放家园)，判断玩家是否注册用户名，未注册用户名则通话客户端注册用户名
    if(EBK_MAINTOWN == byKindID)
    {
        CkRegUserName();
    }

    m_mapKind2Build.AddData(byKindID, poBuild);//添加build管理，AddFunctionModuleAndGuide中使用
    //添加建筑功能模块
    if(!poBuild->AddFunctionModuleAndGuide())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Build[%d] AddFunctionModuleAndGuide failed !"), MSG_MARK, byKindID);
        CBuildMgr::Instance()->RemoveBuild(m_dwPlayerID, byKindID);
        m_mapKind2Build.DelData(byKindID);
        return NULL;
    }

    return poBuild;
}


CBuild* CPlayer::GetBuild(EBuildKind eBuildKind)
{
	return m_mapKind2Build.GetData(eBuildKind);
}

//初始化建筑
BOOL CPlayer::InitBuild(DT_BUILD_DATA_LIST& stBuildData)
{
    UINT16 wBuildNum = stBuildData.wBuildNum;
    for(UINT16 wIdx = 0; wIdx < wBuildNum && wIdx < MAX_BUILD_NUM; wIdx++)
    {
        DT_BUILD_BASE_DATA* pstDT_BUILD_BASE_DATA = &(stBuildData.astBuildInfoLst[wIdx]);

		//if(FALSE == CheckEmBuile(pstDT_BUILD_BASE_DATA->byKindID))
		//{
		//	SYS_CRITICAL(_SDT("[%s: %d]: CPlayer::Build ID failed, ID %d !"), MSG_MARK, pstDT_BUILD_BASE_DATA->byKindID);
		//	continue;
		//}

		//跳过0无效的建筑类型
		if(0 == pstDT_BUILD_BASE_DATA->byKindID)
		{
			continue;
		}

        CBuild* poBuild = CBuildMgr::Instance()->CreateBuild(m_dwPlayerID, pstDT_BUILD_BASE_DATA->byKindID);
        if(NULL == poBuild)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CreateBuild failed !"), MSG_MARK);
            return FALSE;
        }
        if(FALSE == poBuild->Init(pstDT_BUILD_BASE_DATA, this))
        {
            USR_INFO(_SDT("[%s: %d]: Build init failed !"), MSG_MARK);
            CBuildMgr::Instance()->RemoveBuild(m_dwPlayerID, pstDT_BUILD_BASE_DATA->byKindID);
            continue;//跳过有问题数据
        }
        m_mapKind2Build.AddData(poBuild->GetBuildKind(), poBuild);
    }

    m_pstMaintownLevyProp = CBuildPropMgr::Instance()->GetMaintownLevyProp();

    return TRUE;
}


//检测是否需要注册用户名
VOID CPlayer::CkRegUserName()
{
    return ;//todo

    //若是主城(表示开放家园)，判断玩家是否注册用户名，未注册用户名则通话客户端注册用户名

	CUser* poUser=CUserMgr::Instance()->FindUser( GetID() );
    if(poUser)
    {
        tstring strName = poUser->GetUserName();
        if(strName.empty())
        {
            //向GT通知玩家需注册用户名
            UINT16 wMsgID = CGTPktBuilder::Instance()->RegiUserNameNtf();
            SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
        }
    }
}

VOID CPlayer::GetBuildList(UINT16& wBuildNum , DT_BUILD_DATA_CLI astBuildInfo[MAX_BUILD_NUM])
{
	wBuildNum = 0;
	DT_INSTANCE_ID& stMaxInstance = m_poInstance->GetMaxCommonInstanceID();
	SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stMaxInstance.wSceneIdx, stMaxInstance.byTownIdx, stMaxInstance.byInstanceIdx);
	if(NULL == pstInstanceProp)
	{
		return;
	}
	C8Set& setOpenBuild = pstInstanceProp->stInstanceOpenFunc.setOpenBuild;

	UINT16 wNum = 0;
    CBuild* poBuild = m_mapKind2Build.GetFistData();

    UINT32 dwCliVer = GetCliVer();
    
    // 410版本（宠物功能、转生功能）之前的客户端最多只能接收50个建筑，由于410版本后建筑数已经超过50了，所以需要做版本兼容
    UINT16 wMaxBuildNum = (GetCliVer() < VERSION_FLY ?  MAX_BUILD_NUM_BEFORE_v410 : MAX_BUILD_NUM);
    while((poBuild) && (wNum < wMaxBuildNum))
    {
        UINT8 eBuildKind = (EBuildKind)(poBuild->GetDT_BUILD_BASE_DATA().byKindID);

		//冲值活动， 跳过已领取的数据
		if (EBK_RECHARGE == eBuildKind && 
			0 != m_poActivityEncourage->GeDT_ACTIVITY_ENCOURAGE_DATA().byRechargeFlag)
		{
			poBuild = m_mapKind2Build.GetNextData();
			continue;
		}

		else if (EBK_SCORE == eBuildKind && 
			(0 != m_poActivityEncourage->GeDT_ACTIVITY_ENCOURAGE_DATA().byScoreFlag ||
			0 == strlen(CSinglePramMgr::Instance()->GetAppAddress())))
		{
			poBuild = m_mapKind2Build.GetNextData();
			continue;
		}

        //如果是活动奖励建筑，检测一下是否有奖励可领取
        else if (EBK_ACTIVITY_ENCOURAGE == eBuildKind &&
                !m_poActivityEncourage->CkActivityEncourage())
        {
            poBuild = m_mapKind2Build.GetNextData();
            continue;
        }

        //开放有修改，则不下发
        if((setOpenBuild.find(eBuildKind) == setOpenBuild.end())
                && (EBK_FROMATION != eBuildKind && EBK_ENCHANT != eBuildKind && EBK_EQUIP_MAKE != eBuildKind))//阵型由系统给的
        {
            poBuild = m_mapKind2Build.GetNextData();
            continue;
        }

		//聚集灵气在低版本2.90中不下发
		if((EBK_COLLECTGAS == eBuildKind) && (dwCliVer <= 290))
		{
			poBuild = m_mapKind2Build.GetNextData();
			continue;
		}

		if ( EBK_FACTION == eBuildKind && (dwCliVer < 302))
		{
			poBuild = m_mapKind2Build.GetNextData();
			continue;
		}

		if ( EBK_FRIENDS == eBuildKind && (dwCliVer < 302))
		{
			poBuild = m_mapKind2Build.GetNextData();
			continue;
		}

        if ((EBK_GVG == eBuildKind || EBK_MEDAL == eBuildKind || EBK_SOUL == eBuildKind) && dwCliVer < VERSION_GVG_ACTIVITY)
        {
            poBuild = m_mapKind2Build.GetNextData();
            continue;
        }
		
        if (dwCliVer < VERSION_FLY && (EBK_ENCHANT == eBuildKind || EBK_EQUIP_MAKE == eBuildKind || EBK_FLY == eBuildKind || EBK_OUT_BOUND == eBuildKind))
        {
            poBuild = m_mapKind2Build.GetNextData();
            continue;
        }

		poBuild->GetDT_BUILD_DATA_CLI(astBuildInfo[wNum]);

        poBuild = m_mapKind2Build.GetNextData();
        wNum++;
    }

    wBuildNum = wNum;
}


UINT16 CPlayer::GetOpenCruitAck(PKT_CLIGS_OPEN_RECRUIT_ACK& stAck)
{
    CBuild* poBuild = GetBuild(EBK_RECRUIT);
    if(NULL == poBuild)
    {
        return ERR_OPEN_RECUIT::ID_NOT_EXIST_ERR;
    }
    poBuild->OnOpen();

	stAck.wJingJieLevel = 0;
	if(m_poJingJie)
	{
		stAck.wJingJieLevel = m_poJingJie->GetDT_JINGJIE_DATA().stBaseData.wLevel;
	}

    stAck.wHeroNum = 0;

    //获得当前解锁的所有武将
    DT_INSTANCE_ID& stMaxInstance = m_poInstance->GetMaxCommonInstanceID();
    SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stMaxInstance.wSceneIdx, stMaxInstance.byTownIdx, stMaxInstance.byInstanceIdx);
    if(NULL == pstInstanceProp)
    {
        return ERR_OPEN_RECUIT::ID_SUCCESS;
    }

    vector<UINT16> & mapOpenHero = pstInstanceProp->stInstanceOpenFunc.vecOpenHero;
	UINT16 wEncHeroID = CActivityEncourageMgr::Instance()->GetHeroID();

    map<UINT16,UINT16>map_HeroID;
	vector<UINT16>::reverse_iterator ritr;
	for (ritr = mapOpenHero.rbegin(); ritr != mapOpenHero.rend(); ritr++)
	{
		//不是送的，且不是开放的，跳过
		if(*ritr == wEncHeroID)
		{
			continue;
		}

		CHero* poHero = m_mapKind2Hero.GetData(*ritr);
		if((NULL == poHero) || (stAck.wHeroNum >= MAX_HERO_NUM))
		{
			SYS_CRITICAL( _SDT( "[%s %d]Hero:%d not exist!" ), MSG_MARK, *ritr);
			continue;
		}
        map_HeroID[poHero->GetDT_HERO_BASE_DATA().wKindID] = poHero->GetDT_HERO_BASE_DATA().wKindID;
		if(GetDT_RECRUIT_HERO_DATA(poHero, stAck.astHeroInfoLst[stAck.wHeroNum]))
		{
			stAck.wHeroNum++;
		}
	}

	//加上送的伙伴
	CHero* poHero = m_mapKind2Hero.GetData(wEncHeroID);
	if (NULL != poHero)
	{
        map_HeroID[poHero->GetDT_HERO_BASE_DATA().wKindID] = poHero->GetDT_HERO_BASE_DATA().wKindID;
		if(GetDT_RECRUIT_HERO_DATA(poHero, stAck.astHeroInfoLst[stAck.wHeroNum]))
		{
			stAck.wHeroNum++;
		}
	}

    //加上GM直接给的伙伴
    CHero* poHeroNew = m_mapKind2Hero.GetFistData();
    while(poHeroNew)
    {
        UINT16 wKindID =  poHeroNew->GetDT_HERO_BASE_DATA().wKindID;
        if (GetCoachHeroKindID() == poHeroNew->GetDT_HERO_BASE_DATA().wKindID)
        {
            poHeroNew = m_mapKind2Hero.GetNextData();
        }
        else
        {
            map<UINT16,UINT16>::iterator itr = map_HeroID.find(wKindID);
            if (itr == map_HeroID.end())
            {
                if(GetDT_RECRUIT_HERO_DATA_OF_GM(poHeroNew, stAck.astHeroInfoLst[stAck.wHeroNum]))
                {
                    stAck.wHeroNum++;
                }
            }
            poHeroNew = m_mapKind2Hero.GetNextData();
        }
    }

    return ERR_OPEN_RECUIT::ID_SUCCESS;
}

// 根据玩家表现由好到差进行排序
bool SortByTalentAsc(const SHeroProp* pstHeroProp1, const SHeroProp* pstHeroProp2)
{
    if (pstHeroProp1->wTalent == pstHeroProp2->wTalent){
        return pstHeroProp1->wKindID > pstHeroProp2->wKindID;
    }

    return pstHeroProp1->wTalent > pstHeroProp2->wTalent;
}

UINT16 CPlayer::GetOpenCruitAck_v410(PKT_CLIGS_OPEN_RECRUIT_ACK_v410& stAck)
{
    CBuild* poBuild = GetBuild(EBK_RECRUIT);
    if(NULL == poBuild){
        return ERR_OPEN_RECUIT::ID_NOT_EXIST_ERR;
    }
    poBuild->OnOpen();

    stAck.wJingJieLevel = 0;
    if(m_poJingJie){
        stAck.wJingJieLevel = m_poJingJie->GetDT_JINGJIE_DATA().stBaseData.wLevel;
    }

    stAck.wHeroNum = 0;

    //1. 获得当前解锁的所有武将
    DT_INSTANCE_ID& stMaxInstance = m_poInstance->GetMaxCommonInstanceID();
    SInstanceProp* pstInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stMaxInstance.wSceneIdx, stMaxInstance.byTownIdx, stMaxInstance.byInstanceIdx);
    if(NULL == pstInstanceProp){
        return ERR_OPEN_RECUIT::ID_SUCCESS;
    }

    vector<UINT16> & vecOpenHero = pstInstanceProp->stInstanceOpenFunc.vecOpenHero;
    UINT16 wEncHeroID = CActivityEncourageMgr::Instance()->GetHeroID();

    typedef std::set<UINT16> HeroIDSet;
    typedef std::list<UINT16> HeroIDList;

    HeroIDSet setHeroID;          // 前传伙伴
    HeroIDSet setGMHeroID;        // GM直接给的伙伴
    HeroIDList listLegendHeroID;  // 外传伙伴
    HeroIDList listShowHeroID;    // 显示的全部伙伴列表，显示顺序是： 外传伙伴 + 前传伙伴 + 活动奖励伙伴 + GM给的伙伴

    for (vector<UINT16>::reverse_iterator ritr = vecOpenHero.rbegin(); ritr != vecOpenHero.rend(); ritr++){
        if(*ritr == wEncHeroID){
            //不是送的，且不是开放的，跳过
            continue;
        }

        CHero* poHero = m_mapKind2Hero.GetData(*ritr);
        if((NULL == poHero) || (stAck.wHeroNum >= MAX_HERO_NUM)){
            SYS_CRITICAL( _SDT( "[%s %d]Hero:%d not exist!" ), MSG_MARK, *ritr);
            continue;
        }

        SHeroProp* pstHeroProp = poHero->GetHeroProp();
        if(NULL == pstHeroProp){
            return FALSE;
        }

        if (pstHeroProp->wUpgradeToHeroID > 0){
            CHero *poUpgradeHero = GetHero(pstHeroProp->wUpgradeToHeroID);
            if (poUpgradeHero && poUpgradeHero->IsRecruit()){
                // 如果酒馆里该伙伴的进阶版伙伴已被招募，则只显示进阶伙伴
                continue;
            }
        }

        if (pstHeroProp->wUpgradeFromHeroID > 0){
            CHero *poUpgradeFromHero = GetHero(pstHeroProp->wUpgradeFromHeroID);
            if (poUpgradeFromHero && !poHero->IsRecruit()){
                // 如果该伙伴是某个伙伴的进阶版且未被招募，则只显示未进阶伙伴
                continue;
            }
        }

        setHeroID.insert(poHero->GetHeroID());
        listShowHeroID.push_back(poHero->GetHeroID());
    }

    //2. 加上送的伙伴
    CHero* poHero = m_mapKind2Hero.GetData(wEncHeroID);
    if (poHero){
        setHeroID.insert(poHero->GetHeroID());
        listShowHeroID.push_back(poHero->GetHeroID());
    }

    //3. 剩下的伙伴 = 外传伙伴 + GM直接给的伙伴
    for(CHero* poHeroNew = m_mapKind2Hero.GetFistData(); poHeroNew; poHeroNew = m_mapKind2Hero.GetNextData())
    {
        UINT16 wKindID =  poHeroNew->GetDT_HERO_BASE_DATA().wKindID;
        if (poHeroNew->IsCoachHero()){
            // 过滤掉主角
            continue;
        }

        HeroIDSet::iterator itr = setHeroID.find(wKindID);
        if (itr == setHeroID.end()){
            SHeroProp *pstNewHeroProp = poHeroNew->GetHeroProp();
            if (pstNewHeroProp->bIsLegendHero){
                if (pstNewHeroProp->wUpgradeFromHeroID > 0 && !poHeroNew->IsRecruit()){
                    continue;
                }else{
                    listLegendHeroID.push_back(poHeroNew->GetHeroID());
                    // listShowHeroID.push_back(poHeroNew->GetHeroID());
                }
            }else{
                listShowHeroID.push_back(poHeroNew->GetHeroID());
                setGMHeroID.insert(poHeroNew->GetHeroID());
            }
        }
    }

    // listLegendHeroID.sort(std::greater<UINT16>());
    listShowHeroID.insert(listShowHeroID.begin(), listLegendHeroID.begin(), listLegendHeroID.end());

    typedef std::vector<SHeroProp*> HeroPropVec;
    HeroPropVec vecHeroProp;

    for (HeroIDList::iterator itr = listShowHeroID.begin(); itr != listShowHeroID.end(); ++itr){
        UINT16 wHeroID = *itr;
        SHeroProp *pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(wHeroID);
        if (pstHeroProp){
            vecHeroProp.push_back(pstHeroProp);
        }
    }

    std::sort(vecHeroProp.begin(), vecHeroProp.end(), SortByTalentAsc);

    // 取出伙伴存入客户端显示结构体
    //for(HeroIDList::iterator itr = listShowHeroID.begin(); itr != listShowHeroID.end(); ++itr){
    for(UINT16 i = 0; i < vecHeroProp.size(); i++){
        SHeroProp *pstHeroProp = vecHeroProp[i];

        CHero *poHero = GetHero(pstHeroProp->wKindID);
        if (NULL == poHero){
            continue;
        }

        bool bIsGMHero = (setGMHeroID.find(pstHeroProp->wKindID) != setGMHeroID.end());
        if (bIsGMHero){
            if(GetDT_RECRUIT_HERO_DATA_OF_GM_v410(poHero, stAck.astHeroList[stAck.wHeroNum])){
                stAck.wHeroNum++;
            }
        }else{
            if(GetDT_RECRUIT_HERO_DATA_v410(poHero, stAck.astHeroList[stAck.wHeroNum])){
                stAck.wHeroNum++;
            }
        }
    }

    return ERR_OPEN_RECUIT::ID_SUCCESS;
}

BOOL CPlayer::GetDT_RECRUIT_HERO_DATA(CHero* poHero, DT_RECRUIT_HERO_DATA& stDT_RECRUIT_HERO_DATA)
{
    SHeroProp*	pstHeroProp = poHero->GetHeroProp();
    if(NULL == pstHeroProp)
    {
        return FALSE;
    }
    UINT16 wJingjieQualityLevel = 0;
    if (NULL != m_poJingJie)
    {
        wJingjieQualityLevel = m_poJingJie->GetJingjieQualityLevel();
    }

	DT_HERO_BASE_DATA& stDT_HERO_BASE_DATA = poHero->GetDT_HERO_BASE_DATA();
	poHero->GetDT_HERO_BASE_DATA_EQUIP_CLI( stDT_RECRUIT_HERO_DATA.stHeroBaseInfo, TRUE );	
	stDT_RECRUIT_HERO_DATA.stHeroBaseInfo.wTalent = poHero->GetTalent();
	//如果未在携带，那么则不显示加成属性
	if( NULL == m_mapKind2HeroAllCarry.GetData( stDT_RECRUIT_HERO_DATA.stHeroBaseInfo.wKindID ) )
	{
		stDT_RECRUIT_HERO_DATA.stHeroBaseInfo.stBattleAttribute.dwAttack = pstHeroProp->dwAttack;
		stDT_RECRUIT_HERO_DATA.stHeroBaseInfo.stBattleAttribute.dwHP = pstHeroProp->dwHP;
		//stDT_RECRUIT_HERO_DATA.stHeroBaseInfo.wTalent = poHero->GetTalent();//-poHero->GetGodweaponAddTalent();

		stDT_RECRUIT_HERO_DATA.stHeroAttrAddInfo.dwAddAttack = 0;
		stDT_RECRUIT_HERO_DATA.stHeroAttrAddInfo.dwAddHP = 0;
	}
	else
	{
		stDT_RECRUIT_HERO_DATA.stHeroAttrAddInfo.dwAddAttack =
			stDT_RECRUIT_HERO_DATA.stHeroBaseInfo.stBattleAttribute.dwAttack - pstHeroProp->dwAttack;
		stDT_RECRUIT_HERO_DATA.stHeroAttrAddInfo.dwAddHP =
			stDT_RECRUIT_HERO_DATA.stHeroBaseInfo.stBattleAttribute.dwHP - pstHeroProp->dwHP;
	}

	//不管有没有携带，都存在神器增加的资质
	stDT_RECRUIT_HERO_DATA.stHeroAttrAddInfo.dwAddTalent = poHero->GetGodweaponAddTalent();

	stDT_RECRUIT_HERO_DATA.byNeedVisitNum = pstHeroProp->byVisitNum;
	stDT_RECRUIT_HERO_DATA.byCurVisitNum = stDT_HERO_BASE_DATA.byVisitNum > pstHeroProp->byVisitNum ? pstHeroProp->byVisitNum : stDT_HERO_BASE_DATA.byVisitNum;
	stDT_RECRUIT_HERO_DATA.dwVisitCoin = pstHeroProp->dwVisitCoin;
	stDT_RECRUIT_HERO_DATA.wVisitGold = pstHeroProp->wVisitGold;
	poHero->GetHeroSkillCli(stDT_RECRUIT_HERO_DATA.stSkillInfo);
	stDT_RECRUIT_HERO_DATA.wRecuitLevel = pstHeroProp->wRecuitLevel;
	stDT_RECRUIT_HERO_DATA.wRequireJingjieLevel = pstHeroProp->wRequireJingjieLevel;
	stDT_RECRUIT_HERO_DATA.byRecommend = pstHeroProp->byRecommend;	


    if ((GetLevel() >= pstHeroProp->wRecuitLevel) && (wJingjieQualityLevel >= pstHeroProp->wRequireJingjieLevel))
    {
        stDT_RECRUIT_HERO_DATA.byRecuitUnLock = 1;
    }
    else
    {
        stDT_RECRUIT_HERO_DATA.byRecuitUnLock = 0;
    }


	return TRUE;
}

BOOL CPlayer::GetDT_RECRUIT_HERO_DATA_v410(CHero* poHero, DT_RECRUIT_HERO_DATA_v410 &stHeroCli_v410)
{
    memset(&stHeroCli_v410, 0, sizeof(stHeroCli_v410));
    
    SHeroProp* pstHeroProp = poHero->GetHeroProp();
    if(NULL == pstHeroProp){
        return FALSE;
    }

    // 如果未飞升，则不显示外传伙伴
    if (pstHeroProp->bIsLegendHero && !IsFly()){
        return FALSE;
    }

    DT_RECRUIT_HERO_DATA stHeroCli;
    if (!GetDT_RECRUIT_HERO_DATA(poHero, stHeroCli)){
        return FALSE;
    }

    stHeroCli_v410.byNeedVisitNum = stHeroCli.byNeedVisitNum;
    stHeroCli_v410.byCurVisitNum = stHeroCli.byCurVisitNum;
    stHeroCli_v410.dwVisitCoin = stHeroCli.dwVisitCoin;
    stHeroCli_v410.wVisitGold = stHeroCli.wVisitGold;
    stHeroCli_v410.wRecuitLevel = stHeroCli.wRecuitLevel;
    stHeroCli_v410.wRequireJingjieLevel = stHeroCli.wRequireJingjieLevel;
    stHeroCli_v410.byRecuitUnLock = stHeroCli.byRecuitUnLock;
    stHeroCli_v410.byRecommend = stHeroCli.byRecommend;
    
    stHeroCli_v410.stHeroInfo.stHeroBaseInfo = stHeroCli.stHeroBaseInfo;
    stHeroCli_v410.stHeroInfo.stSkillInfo = stHeroCli.stSkillInfo;

    DT_HERO_ATTR_ADD_CLI_v410 &stHeroAttrAddInfo = stHeroCli_v410.stHeroInfo.stHeroAttrAddInfo;
    stHeroAttrAddInfo.dwDefense  = 0;
    stHeroAttrAddInfo.dwAddTalent = stHeroCli.stHeroAttrAddInfo.dwAddTalent;

    //如果已被携带在玩家身上，则显示加成属性
    if(m_mapKind2HeroAllCarry.GetData(stHeroCli_v410.stHeroInfo.stHeroBaseInfo.wKindID ) ){
        stHeroAttrAddInfo.dwAddAttack = stHeroCli.stHeroAttrAddInfo.dwAddAttack;
        stHeroAttrAddInfo.dwAddHP     = stHeroCli.stHeroAttrAddInfo.dwAddHP;

        DT_BATTLE_BASE_ATTRIBUTE &stAttr = poHero->GetDT_BATTLE_ATTRIBUTE();
        stHeroAttrAddInfo.dwAddDefense    = stAttr.dwDefend - 0;
    }

    stHeroCli_v410.byIsFlyHero = pstHeroProp->bIsLegendHero;
    stHeroCli_v410.byCanUpgrade = IsUpgradable(poHero->GetHeroID());

    // 获取进阶材料列表
    if (stHeroCli_v410.byCanUpgrade){
        SHeroProp* pstUpgradeHeroProp = CHeroPropMgr::Instance()->GetHeroProp(pstHeroProp->wUpgradeToHeroID);
        if (pstUpgradeHeroProp){
            CEquipMakeMgr::TakeMaterialListCli(*this, pstUpgradeHeroProp->vecCostMaterial, stHeroCli_v410.stUpgradeMaterials);
        }
    }

    // 获取招募材料列表
    if (pstHeroProp->bIsLegendHero && 0 == pstHeroProp->wUpgradeFromHeroID){
        CEquipMakeMgr::TakeMaterialListCli(*this, pstHeroProp->vecCostMaterial, stHeroCli_v410.stRecruitMaterials);
    }
    
    if (stHeroCli_v410.byCanUpgrade){
        DT_RECRUIT_HERO_INFO &stUpgradeHeroCli_v410 = stHeroCli_v410.astUpgradeHeroInfo[0];

        CHero *poUpgradeHero = GetUpgradeHero(poHero->GetHeroID());
        
        DT_RECRUIT_HERO_DATA stUpgradeHeroCli;
        GetDT_RECRUIT_HERO_DATA(poUpgradeHero, stUpgradeHeroCli);

        DT_HERO_ATTR_ADD_CLI_v410 &stUpgradeHeroAddAttr_v410 = stUpgradeHeroCli_v410.stHeroAttrAddInfo;
        stUpgradeHeroAddAttr_v410.dwAddAttack = stUpgradeHeroCli.stHeroAttrAddInfo.dwAddAttack;
        stUpgradeHeroAddAttr_v410.dwAddHP     = stUpgradeHeroCli.stHeroAttrAddInfo.dwAddHP;
        stUpgradeHeroAddAttr_v410.dwAddTalent = stUpgradeHeroCli.stHeroAttrAddInfo.dwAddTalent;

        stUpgradeHeroCli_v410.stHeroBaseInfo = stUpgradeHeroCli.stHeroBaseInfo;
        stUpgradeHeroCli_v410.stSkillInfo = stUpgradeHeroCli.stSkillInfo;
    }

    return TRUE;
}

BOOL CPlayer::GetDT_RECRUIT_HERO_DATA_OF_GM(CHero* poHero, DT_RECRUIT_HERO_DATA& stDT_RECRUIT_HERO_DATA)
{
    if (!GetDT_RECRUIT_HERO_DATA(poHero, stDT_RECRUIT_HERO_DATA))
    {
        return FALSE;
    }

    stDT_RECRUIT_HERO_DATA.byRecuitUnLock = 1;
    return TRUE;
}

BOOL CPlayer::GetDT_RECRUIT_HERO_DATA_OF_GM_v410(CHero* poHero, DT_RECRUIT_HERO_DATA_v410& stRecruitData)
{
    if (!GetDT_RECRUIT_HERO_DATA_v410(poHero, stRecruitData)){
        return FALSE;
    }

    stRecruitData.byRecuitUnLock = 1;
    return TRUE;
}


//获取打开竞技场信息
UINT16 CPlayer::GetOpenRaceBuildAck(PKT_CLIGS_OPEN_RACE_BUILD_ACK& stAck)
{
    CBuild* poBuild = GetBuild(EBK_RACEBUILD);
    if(NULL == poBuild)
    {
        return ERR_OPEN_RACE_BUILD::ID_NOT_EXIST_ERR;
    }
    poBuild->OnOpen();

    if(NULL == m_poRace)
    {
		SET_OTHER_ERR(stAck.wErrCode);
        return stAck.wErrCode;
    }

    //基本信息
    stAck.wLevel = GetLevel();
    //stAck.dwFame = m_poOwner->GetDT_PLAYER_BASE_DATA().dwFame;
    DT_RACE_BASE_DATA& stDT_RACE_BASE_DATA = m_poRace->GetDT_RACE_BASE_DATA();
    stAck.dwRank = stDT_RACE_BASE_DATA.dwRank;
    stAck.wContiWin = stDT_RACE_BASE_DATA.wContiWin;
    SRaceBaseProp& stRaceBaseProp = CRacePropMgr::Instance()->GetRaceProp();
    stAck.qwScience = m_stDT_PLAYER_BASE_DATA.qwScience;
    stAck.dwPower = m_stDT_PLAYER_BASE_DATA.dwPower;
	stAck.qwJingJie = m_stDT_PLAYER_BASE_DATA.qwJingJie;
    stAck.dwClrCDGold = CRacePropMgr::Instance()->GetRaceProp().dwClrCDGoldPerMinute;

    //判断时间
    stAck.byHaveRecvEncourageFlag = 0;
    //上次领取奖励为今天
    if(IsToday(stDT_RACE_BASE_DATA.qwRecvEncourageTime))
    {
        stAck.byHaveRecvEncourageFlag = 1;

		USR_INFO( _SDT("PlayerID[%u] Not Recv Encourage"), GetDT_PLAYER_BASE_DATA().dwID);
    }

    CRacePropMgr::Instance()->GetRankEncourage(stDT_RACE_BASE_DATA.dwRecvEncourageRank, stAck.stTodayRankEncourage);
    CRacePropMgr::Instance()->GetRankEncourage(stDT_RACE_BASE_DATA.dwRank, stAck.stTomorrowRankEncourage);
    CRacePropMgr::Instance()->GetCanChallengeTimes(this, stDT_RACE_BASE_DATA.qwLastChallTime, stDT_RACE_BASE_DATA.byChallTimes, stDT_RACE_BASE_DATA.wContiWin, stDT_RACE_BASE_DATA.byFailedFlag,
            stAck.wChallengeCountDown, stAck.wTotalChallengeNum, stAck.wCanChallengeNum, stAck.wVipExtNum, stAck.wFreeChallengeNum, stAck.dwChallengeNeedGold);
    CRaceMgr::Instance()->GetChallengePlayerList(stAck.dwRank, stAck.byChallengeNum, stAck.astChallengePlayerList);

	//DBG_INFO( _SDT("PlayerID[%u] Race Info [%u, %u, %u, %u]"), GetDT_PLAYER_BASE_DATA().dwID, 
		//stAck.wChallengeCountDown, stAck.wCanChallengeNum, stAck.wFreeChallengeNum, stAck.dwChallengeNeedGold);
	if (m_poWorship)
	{
		stAck.byHaveWorship = m_poWorship->CKCanWorship(EWT_RACE, FALSE) ? 0 : 1;
	}

	INT32 nTmp = SDNow().DiffDateDay(CGSConfig::Instance()->GetServerOpentTime());
	UINT16 wRecvRankPassDay = CRacePropMgr::Instance()->GetRaceProp().wRecvRankPassDay;
	stAck.byRecvEncCountDown = nTmp < wRecvRankPassDay ? wRecvRankPassDay - nTmp : 0;//2天后才可领取竞技场奖励

    stAck.wErrCode = ERR_OPEN_RACE_BUILD::ID_SUCCESS;
    return stAck.wErrCode;
}

UINT16 CPlayer::GetOpenSimthyAck(PKT_CLIGS_OPEN_SMITHY_ACK& stAck)
{
    CBuild* poBuild = GetBuild(EBK_SMITHY);
    if(NULL == poBuild)
    {
        return ERR_OPEN_SMITHY::ID_NOT_EXIST_ERR;
    }
    poBuild->OnOpen();

    memset(&stAck, 0, sizeof(PKT_CLIGS_OPEN_SMITHY_ACK));

    //获取所有的装备
    GetAllEquip(stAck.stEquipList, stAck.adwStrengthen10CoinInfo);

    // 
    stAck.wEquipNum = stAck.stEquipList.wEquipNum;
    // 获取开光标识字节串
    COpenLightPropMgr::Instance()->GetOpenLightFlagInfo(stAck);

    CBuildPropMgr::Instance()->GetBuyEquipCliData(stAck.astByEquipInfo);

    stAck.dwClrCDGold = CEquipStrengthenPropMgr::Instance()->GetClrStrengthenCDGold();
    stAck.wStrengthenCD = CEquipStrengthenPropMgr::Instance()->GetTotalCD();
    stAck.wStrengthenCountDown = GetStrenthenCountDown();
	stAck.qwBlueGas = m_stDT_PLAYER_BASE_DATA.qwBlueGas;

    return ERR_OPEN_SMITHY::ID_SUCCESS;
}

UINT16 CPlayer::GetOpenSmithyAck_v410(PKT_CLIGS_OPEN_SMITHY_ACK_v410& stAck)
{
    CBuild* poBuild = GetBuild(EBK_SMITHY);
    if(NULL == poBuild)
    {
        return ERR_OPEN_SMITHY::ID_NOT_EXIST_ERR;
    }

    CBuild* poEnchantBuild = GetBuild(EBK_ENCHANT);
    if (poEnchantBuild){
        poEnchantBuild->OnOpen();
    }

    poBuild->OnOpen();
    memset(&stAck, 0, sizeof(stAck));

    //获取所有的装备
    GetAllEquip_v410(stAck.stEquipList);

    // 获取开光标识字节串
    COpenLightPropMgr::Instance()->GetOpenLightFlagInfo_v410(stAck);
    CBuildPropMgr::Instance()->GetBuyEquipCliData(stAck.astByEquipInfo);

    stAck.dwClrCDGold = CEquipStrengthenPropMgr::Instance()->GetClrStrengthenCDGold();
    stAck.wStrengthenCD = CEquipStrengthenPropMgr::Instance()->GetTotalCD();
    stAck.wStrengthenCountDown = GetStrenthenCountDown();
    stAck.qwBlueGas = m_stDT_PLAYER_BASE_DATA.qwBlueGas;
    return ERR_OPEN_SMITHY::ID_SUCCESS;
}

UINT16 CPlayer::Crown(UINT8& byCanCrowFlag)
{
    CBuild* poBuild = GetBuild(EBK_MAINTOWN);
    if(NULL == poBuild)
    {
        return ERR_RECV_MAINTOWN_INCOME::ID_BUILD_NOT_EXIST;
    }

    DT_BUILD_BASE_DATA&  stDT_BUILD_BASE_DATA = poBuild->GetDT_BUILD_BASE_DATA();

    //Value1 ,对于主城为收获时间
    //Value2 ,对于主城为征收时间
    //Value3 ,对于主城为征收的次数

    //判断时间
    //今天已经收获过
    if(IsToday(stDT_BUILD_BASE_DATA.qwValue1))
    {
        byCanCrowFlag = 0;
        return ERR_RECV_MAINTOWN_INCOME::ID_HAVE_RECV;
    }
    UINT16 wCurLevel = GetLevel();
	AddCoin( CLuamgr::Instance()->GetMaintownCrownCoin(wCurLevel), CRecordMgr::EGGT_ADDFLAGCROWN );	
	AddGold( CLuamgr::Instance()->GetMaintownCrownGold(wCurLevel), CRecordMgr::EGGT_ADDFLAGCROWN );
	//if(GetBuild(EBK_JINGJIE))
	//{
	AddJingJie( CLuamgr::Instance()->GetMaintownCrownJingJie(wCurLevel), CRecordMgr::EGGT_ADDFLAGCROWN );
	//}
	
    byCanCrowFlag = 0;
    stDT_BUILD_BASE_DATA.qwValue1 = SDTimeSecs();

    //记录已引导
    AddGuideRecord( EGF_CROWN);

    return ERR_RECV_MAINTOWN_INCOME::ID_SUCCESS;
}

UINT16 CPlayer::Levy(UINT32& dwLevyCountDown, UINT8& byLevyNum)
{
    CBuild* poBuild = GetBuild(EBK_LEVY);
    if(NULL == poBuild)
    {
        return ERR_LEVY_MAINTOWN_INCOME::ID_BUILD_NOT_EXIST;
    }
    poBuild->OnOpen();

    DT_BUILD_BASE_DATA&  stDT_BUILD_BASE_DATA = poBuild->GetDT_BUILD_BASE_DATA();

    //Value1 ,对于主城为收获时间
    //Value2 ,对于主城为征收时间
    //Value3 ,对于主城为征收的次数

    //今天未征收，重新赋值征收次数0
    if(!IsToday(stDT_BUILD_BASE_DATA.qwValue2))
    {
        stDT_BUILD_BASE_DATA.byValue3 = 0; //
    }
	UINT64 qnPassTime = SDTimeSecs() - stDT_BUILD_BASE_DATA.qwValue2;
	if(qnPassTime < m_pstMaintownLevyProp->dwLevyCD)
	{
		dwLevyCountDown = m_pstMaintownLevyProp->dwLevyCD - (UINT32)qnPassTime;
		return ERR_LEVY_MAINTOWN_INCOME::ID_IN_CD;
	}
    byLevyNum = (m_pstMaintownLevyProp->byLevyNumPerDay > stDT_BUILD_BASE_DATA.byValue3) ? m_pstMaintownLevyProp->byLevyNumPerDay - stDT_BUILD_BASE_DATA.byValue3 : 0;
    if(0 == byLevyNum)
    {
        return ERR_LEVY_MAINTOWN_INCOME::ID_REACH_MAX_LEVY_NUM;
    }
	AddCoin(CLuamgr::Instance()->GetMaintownLevyCoin(GetLevel()), CRecordMgr::EACT_LEVY);
	//if(GetBuild(EBK_JINGJIE))
	//{
	AddJingJie(CLuamgr::Instance()->GetMaintownLevyJingJie(GetLevel()), CRecordMgr::EAJT_LEVY);	
	//}	
    stDT_BUILD_BASE_DATA.qwValue2 = SDTimeSecs();

    stDT_BUILD_BASE_DATA.byValue3++;
    byLevyNum--;
    dwLevyCountDown = m_pstMaintownLevyProp->dwLevyCD;

    CTaskMgr::Instance()->OnCrown(this);
    AddGuideRecord( EGF_LEVY);
    return ERR_LEVY_MAINTOWN_INCOME::ID_SUCCESS;
}


UINT32 CPlayer::GetLevyCountDown()
{
    CBuild* poBuild = GetBuild(EBK_LEVY);
    if(NULL == poBuild)
    {
        return 0;
    }

    DT_BUILD_BASE_DATA&  stDT_BUILD_BASE_DATA = poBuild->GetDT_BUILD_BASE_DATA();

    //Value1 ,对于主城为收获时间
    //Value2 ,对于主城为征收时间
    //Value3 ,对于主城为征收的次数

    //判断时间
    UINT64 qnGap = SDTimeSecs() - stDT_BUILD_BASE_DATA.qwValue2;
    if(qnGap < 0)
    {
        return 0;
    }
    else
    {
        if(qnGap >= m_pstMaintownLevyProp->dwLevyCD)
        {
            return 0;
        }
        else
        {
            return m_pstMaintownLevyProp->dwLevyCD - (UINT32)qnGap;
        }
    }

    return 0;
}


UINT16 CPlayer::OpenMainTown(UINT16& wCurLevel,UINT8& byCanCrowFlag, UINT32& dwCrownCoin, UINT32& dwCrownGold, UINT32& dwCrownJingJie,
	UINT32& dwLevyCountDown, UINT32& dwLevyCoin, UINT32& dwLevyJingJie, UINT8& byLevyNum, UINT8 &byTotalLevyNum)
{
    CBuild* poBuild = GetBuild(EBK_MAINTOWN);
    if(NULL == poBuild)
    {
        return ERR_OPEN_MAINTOWN::ID_BUILD_NOT_EXIST;
    }
    poBuild->OnOpen();



    wCurLevel = GetLevel();

    //Value1 ,对于主城为收获时间
    //Value2 ,对于主城为征收时间
    //Value3 ,对于主城为征收的次数

    DT_BUILD_BASE_DATA&  stDT_BUILD_BASE_DATA = poBuild->GetDT_BUILD_BASE_DATA();
    //今天已经收获过
    if(IsToday(stDT_BUILD_BASE_DATA.qwValue1))
    {
        byCanCrowFlag = 0;
    }
    else
    {
        byCanCrowFlag = 1;
        stDT_BUILD_BASE_DATA.byValue3 = 0; //重新赋值征收次数
    }
    dwCrownCoin = CLuamgr::Instance()->GetMaintownCrownCoin(wCurLevel);
    dwCrownGold = CLuamgr::Instance()->GetMaintownCrownGold(wCurLevel);
	//if(GetBuild(EBK_JINGJIE))
	//{
	dwCrownJingJie = CLuamgr::Instance()->GetMaintownCrownJingJie(wCurLevel);
	dwLevyJingJie = CLuamgr::Instance()->GetMaintownLevyJingJie(wCurLevel);
	//}	
	//else
	//{
	//	dwCrownJingJie = 0;
	//	dwLevyJingJie = 0;
	//}
    dwLevyCoin = CLuamgr::Instance()->GetMaintownLevyCoin(wCurLevel);

    CBuild* poBuildEx = GetBuild(EBK_LEVY);
    if(NULL == poBuildEx)
    {
        return ERR_OPEN_MAINTOWN::ID_SUCCESS;
    }
    poBuildEx->OnOpen();
    DT_BUILD_BASE_DATA&  stDT_BUILD_BASE_DATAEX = poBuildEx->GetDT_BUILD_BASE_DATA();

    //今天未征收，重新赋值征收次数0
    if(!IsToday(stDT_BUILD_BASE_DATAEX.qwValue2))
    {
        stDT_BUILD_BASE_DATAEX.byValue3 = 0; //
    }
    byLevyNum = (m_pstMaintownLevyProp->byLevyNumPerDay > stDT_BUILD_BASE_DATAEX.byValue3) ? m_pstMaintownLevyProp->byLevyNumPerDay - stDT_BUILD_BASE_DATAEX.byValue3 : 0;
    if(byLevyNum > 0)
    {
        dwLevyCountDown = GetLevyCountDown();
    }
    byTotalLevyNum = m_pstMaintownLevyProp->byLevyNumPerDay;
    return ERR_OPEN_MAINTOWN::ID_SUCCESS;
}

UINT16 CPlayer::OpenSmithyIcon(UINT8 &byNum, DT_DOGFECES_BUILD_TIP astDogFecesBuildTip[MAX_DOGFECES_BUILD_TIP_NUM], DT_FUNC_PROMPT_DATA &stPromptInfo)
{
    CBuild* poBuild = GetBuild(EBK_SMITHY);
    if(NULL == poBuild)
    {
        return ERR_OPEN_DOGFECES::ID_SUCCESS;
    }

    /*
    if (poBuild = GetBuild(EBK_EQUPUPGRADE)){
        poBuild->OnOpen();
        DT_FUNC_COMMON_PROMPT &stBuildPrompt = stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++];
        stBuildPrompt.byBuildKindID = EBK_EQUPUPGRADE;
    }

    if (poBuild = GetBuild(EBK_XIANGQIAN)){
        poBuild->OnOpen();
        DT_FUNC_COMMON_PROMPT &stBuildPrompt = stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++];
        stBuildPrompt.byBuildKindID = EBK_XIANGQIAN;
    }
    */

    if (poBuild = GetBuild(EBK_SMITHY)){
        poBuild->OnOpen();
        DT_FUNC_COMMON_PROMPT &stBuildPrompt = stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++];
        GetStrengthenPrompt(stBuildPrompt);
    }

    if (poBuild = GetBuild(EBK_ENCHANT)){
        poBuild->OnOpen();
        DT_FUNC_COMMON_PROMPT &stBuildPrompt = stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++];
        GetEnchantPrompt(stBuildPrompt);
    }

    if (poBuild = GetBuild(EBK_EQUIP_MAKE)){
        poBuild->OnOpen();
        DT_FUNC_COMMON_PROMPT &stBuildPrompt = stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++];
        GetEquipMakePrompt(stBuildPrompt);
    }

    return ERR_OPEN_DOGFECES::ID_SUCCESS;
}

UINT16 CPlayer::OpenDogFeces(UINT8 byType, UINT8 &byNum, DT_DOGFECES_BUILD_TIP astDogFecesBuildTip[MAX_DOGFECES_BUILD_TIP_NUM], DT_FUNC_PROMPT_DATA &stPromptInfo)
{
	byNum = 0;
	if (EPDFB_RACE == byType)
	{
		CBuild* poBuild = GetBuild(EBK_PVP);
		if(NULL == poBuild)
		{
			return ERR_OPEN_DOGFECES::ID_SUCCESS;
		}
		poBuild->OnOpen();

		//竞技场
		CC_LOOP_DO
		CC_LOOP_BREAK(NULL == GetBuild(EBK_RACEBUILD))
		CRace *poRace = GetRace();
		CC_LOOP_BREAK (NULL == poRace)
		astDogFecesBuildTip[byNum].wBuildID = EBK_RACEBUILD;
		poRace->GetChallengeTimes(astDogFecesBuildTip[byNum].dwParameter1, astDogFecesBuildTip[byNum].dwParameter2);
		byNum++;
		GetRacePrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]); //竟技场提示信息
		CC_LOOP_WHILE(0)

		
		//试练场
		CC_LOOP_DO
		CC_LOOP_BREAK(NULL == GetBuild(EBK_RDCHALLENGE) || NULL == m_poRdChallenge)
		astDogFecesBuildTip[byNum].wBuildID = EBK_RDCHALLENGE;
		m_poRdChallenge->GetChallengeTimes(astDogFecesBuildTip[byNum].dwParameter1, astDogFecesBuildTip[byNum].dwParameter2);
		byNum++;
		GetRdChallengePrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]); //比武提示信息
		CC_LOOP_WHILE(0)
	}
	else if (EPDFB_ADVENTURE == byType)
	{
		CBuild* poBuild = GetBuild(EBK_ADVENTURE);
		if(NULL == poBuild)
		{
			return ERR_OPEN_DOGFECES::ID_SUCCESS;
		}
		poBuild->OnOpen();

		//精英副本
		CC_LOOP_DO
		CC_LOOP_BREAK(NULL == GetBuild(EBK_ELITEINSTANCE))
		CRace *poRace = GetRace();
		CC_LOOP_BREAK (NULL == poRace)
		astDogFecesBuildTip[byNum].wBuildID = EBK_ELITEINSTANCE;
		m_poInstance->GetEliteInstanceNum(astDogFecesBuildTip[byNum].dwParameter1, astDogFecesBuildTip[byNum].dwParameter2);
		byNum++;
		GetElitePrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]); //精英提示信息
		CC_LOOP_WHILE(0)

        //爬梯副本
        CC_LOOP_DO
        CC_LOOP_BREAK(NULL == GetBuild(EBK_CLIMBTOWER))
        astDogFecesBuildTip[byNum].wBuildID = EBK_CLIMBTOWER;
        m_poInstance->GetClimbTownInstanceNum(astDogFecesBuildTip[byNum].dwParameter1, astDogFecesBuildTip[byNum].dwParameter2);
        byNum++;
        GetClimbTowerPrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]); //爬塔提示信息
        CC_LOOP_WHILE(0)


        //押镖副本
        CC_LOOP_DO
        CC_LOOP_BREAK(NULL == GetBuild(EBK_YABIAO))
        CC_LOOP_BREAK(NULL == m_poYabiaoInfo)

        m_poYabiaoInfo->Check4Reset();
        astDogFecesBuildTip[byNum].wBuildID = EBK_YABIAO;
        astDogFecesBuildTip[byNum].dwParameter1	= m_poYabiaoInfo->GetRemainYabiaoCnt();
        astDogFecesBuildTip[byNum].dwParameter2	= m_poYabiaoInfo->GetRemainJiebiaoCnt();
        DT_FUNC_COMMON_PROMPT &stBuildPromptInfo = stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++];
        GetYabiaoPrompt(stBuildPromptInfo); //押镖提示信息

		//如果提示可领取奖励，但时间还有倒计时。
		if (stBuildPromptInfo.byFuncFlag == ms_nPromotYabiaoRunning && stBuildPromptInfo.dwFuncCountDown > 0)
		{
			stBuildPromptInfo.byFuncFlag = 0;
			//可拦截次数大于0，直接提示可拦截。
			if (astDogFecesBuildTip[byNum].dwParameter2 > 0)
			{
				//客户端只识别三。有BUG
				stBuildPromptInfo.byFuncFlag = ms_nPromotYabiaoSucceed;
			}
		}
		byNum++;
		CC_LOOP_WHILE(0)

		CC_LOOP_DO
		CC_LOOP_BREAK(NULL == GetBuild(EBK_FACTION))
		CC_LOOP_BREAK(NULL == m_poFactionPlayer)
		DT_FUNC_COMMON_PROMPT &stBuildPromptInfo = stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++];
		GetFactionPrompt(stBuildPromptInfo);
		CC_LOOP_WHILE(0)

		//战魂副本
		CC_LOOP_DO
		CC_LOOP_BREAK(NULL == GetBuild(EBK_SOUL))
		CPlayerSoul *poSoul = GetPlayerSoul();
		CC_LOOP_BREAK (NULL == poSoul)
		GetSoulInstancePrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]); //精英提示信息
		CC_LOOP_WHILE(0)
	}
    else if (EPDFB_SMITHY== byType)
    {
        return OpenSmithyIcon(byNum, astDogFecesBuildTip, stPromptInfo);
    }

	return ERR_OPEN_DOGFECES::ID_SUCCESS;
}

