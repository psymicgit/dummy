
#include "taskpropmgr.h"
#include "acceptconditon.h"
#include "finishconditon.h"
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <common/client/errdef.h>
#include <logic/build/buildmgr.h>
#include <logic/player/playermgr.h>
#include <common/server/utility.h>
#include <logic/base/logicfunc.h>
#include <logic/instance/instancepropmgr.h>

#include <framework/gsapi.h>
#include <db/dbmgr.h>

IMPLEMENT_SINGLETON_PROPMGR(CTaskPropMgr)


CTaskPropMgr::CTaskPropMgr()
{
    //默认初始已解锁111副本
    m_stInitUnlockInstanceID.wSceneIdx = 1;
    m_stInitUnlockInstanceID.byTownIdx = 1;
    m_stInitUnlockInstanceID.byInstanceIdx = 1;
    m_stLastInstanceID = m_stInitUnlockInstanceID;
}

CTaskPropMgr::~CTaskPropMgr()
{
}


BOOL CTaskPropMgr::Init()
{

    //初始化字符串到枚举
    InitStr2Enum();
    // 读取配置文件
    if(!ReadTaskFromXML())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: ReadMainTaskFromXML failed!"), MSG_MARK);
        return FALSE;
    }

    /*
    if(!ReadDailyTaskFromXML())
    {
    	SYS_CRITICAL(_SDT("[%s: %d]: ReadDailyTaskFromXML failed!"), MSG_MARK);
    	return FALSE;
    }
    */

    InitNextTaskIdx();

    return TRUE;
}

VOID CTaskPropMgr::InitStr2Enum()
{
    m_mapStr2Enum["Player"] = EACT_PLAYER; //
    m_mapStr2Enum["Instance"] = EACT_INSTANCE; //
    m_mapStr2Enum["Build"] = EACT_BUILD; //
    m_mapStr2Enum["MainTask"] = EACT_MAINTASK; //


    m_mapStr2Enum["Crown"] = CROWN; //收获任务
    m_mapStr2Enum["RecruitHero"] = RECRUITHERO ; //招募武将任务
    m_mapStr2Enum["UpgradeSkill"] = UPGRADESKILL ; //升级技能 任务
    m_mapStr2Enum["UpgradeJingJie"] = UPGRADEJINGJIE ; //招募武将任务
    m_mapStr2Enum["FirstPassInstance"] = FIRSTPASSINSTANCE ; //首次通过副本任务
    m_mapStr2Enum["ScorePassInstance"] = SCOREPASSINSTANCE ; //指定分数通过副本任务
    m_mapStr2Enum["TimesPassInstance"] = TIMESPASSINSTANCE ; //指定次数通过副本任务
    m_mapStr2Enum["KillMonsterInstance"] = KILLMONSTERINSTANCE ; //指定副本杀怪任务
    m_mapStr2Enum["StrengthenEquipLevel"] = STRENGTHENEQUIPLEVEL ; //强化装备到指定等级任务
    m_mapStr2Enum["StrengthenEquipNum"] = STRENGTHENEQUIPNUM ; //强化装备指定次数任务
    m_mapStr2Enum["MosaicGem"] = MOSAICGEM ; //镶嵌宝石
    m_mapStr2Enum["ActivateGodweapon"] = ACTIVATEGODWEAPON ; //激活神器 任务
    m_mapStr2Enum["UpgradeGodweapon"] = UPGRADEGODWEAPON ; //升级神器 任务
    m_mapStr2Enum["Challenge"] = CHALLENGE ; //竞技场挑战 任务
    m_mapStr2Enum["Plunder"] = PLUNDER ; //掠夺 任务
    m_mapStr2Enum["ClimbTower"] = CLIMBTOWER ; //爬塔
    m_mapStr2Enum["DressEquip"] = DRESSEQUIP ; //穿戴 任务
    m_mapStr2Enum["UseExprienceDrug"] = USEEXPRIENCEDRUG ; //服用经验丹
    m_mapStr2Enum["ComposeGood"] = COMPOSEGOOD ; //合成物品任务
    m_mapStr2Enum["EliteInstance"] = ELITEINSTANCE ; //通过指定精武副本任务
    m_mapStr2Enum["UpgradeScience"] = UPGRADSCIENCE ; //升级科技
    m_mapStr2Enum["StrengthAttack"] = STRENGTHATTACK ; //强化攻击
    m_mapStr2Enum["StrengthLife"] = STRENGTHLIFE ; //强化生命
    m_mapStr2Enum["Crop"] = CROP ; //种植任务
    m_mapStr2Enum["MeltEquip"] = MELTEQUIP ; //熔炼任务
    m_mapStr2Enum["Study"] = STUDY ; //熔炼任务
    m_mapStr2Enum["RecruitHeroNum"] = RECRUITHERONUM ; //招募武将任务
    m_mapStr2Enum["OpenLight"] = OPENLIGHT ; //开光任务
}

ETaskType CTaskPropMgr::GetTaskType(string strTaskType)
{
    CStr2EnumMapItr itr = m_mapStr2Enum.find(strTaskType);
    if(itr != m_mapStr2Enum.end())
    {
        return (ETaskType)(itr->second);
    }

    return INVALID;
}

EAcceptConditionType CTaskPropMgr::GetAcceptConditionType(string strTaskType)
{
    CStr2EnumMapItr itr = m_mapStr2Enum.find(strTaskType);
    if(itr != m_mapStr2Enum.end())
    {
        return (EAcceptConditionType)(itr->second);
    }

    return EACT_INVALID;
}


BOOL CTaskPropMgr::ReadTaskFromXML()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("taskfile", " order by TaskLineIdx").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    while(pRes->GetRecord())
    {
        CTaskLineProp* poTaskLineProp = SDNew CTaskLineProp;
        poTaskLineProp->byTaskLineIdx = SGDP::SDAtou(pRes->GetFieldValueByName("TaskLineIdx"));
        if(0 == poTaskLineProp->byTaskLineIdx)
        {
            SDDelete poTaskLineProp;
            poTaskLineProp = NULL;
            SYS_CRITICAL(_SDT("[%s: %d]: TaskLineIdx 0 invalid!"), MSG_MARK);
            return FALSE;
        }
        if(!ReadTaskFromData(poTaskLineProp, pRes->GetFieldValueByName("TaskData"), pRes->GetFieldLengthByName("TaskData")))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: ReadTaskFromXML failed, TaskLineIdx[%d]!"), MSG_MARK, poTaskLineProp->byTaskLineIdx);
            SDDelete poTaskLineProp;
            poTaskLineProp = NULL;
            return FALSE;
        }
        m_mapTaskLine[poTaskLineProp->byTaskLineIdx] = poTaskLineProp;

    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}



BOOL CTaskPropMgr::ReadTaskFromData(CTaskLineProp* poTaskLineProp, const CHAR* pszData, const UINT32 dwDataLen)
{
    UINT32 dwLastTaskIdx = 0;
    SGDP::CSDXMLFile xmlFile;
    if(FALSE == xmlFile.LoadData(pszData, dwDataLen))
    {
        // XML文件解析失败
        return FALSE;
    }
    CSDXMLNode rootNode = xmlFile.GetRootNode();
    if(rootNode == NULL)
    {
        return FALSE;
    }
    CSDXMLNode xmlNode = rootNode["Tasks"];
    if(xmlNode == NULL)
    {
        return FALSE;
    }
    CHAR* pszNodeName = "Task";
    CSDXMLNode xmlNodePropTask = xmlNode[pszNodeName];
    while(xmlNodePropTask != NULL)
    {
        STaskProp* pstTaskProp = SDNew STaskProp;
        pstTaskProp->byTaskLineIdx = poTaskLineProp->byTaskLineIdx;
        pstTaskProp->dwTaskIdx = xmlNodePropTask("Idx").AsInteger();
        //idx必须按大小顺序，但不一定连续
        pstTaskProp->dwTaskIdx = xmlNodePropTask("Idx").AsInteger();
        if((pstTaskProp->dwTaskIdx <= dwLastTaskIdx) || (poTaskLineProp->mapTaskProp.end() != poTaskLineProp->mapTaskProp.find(pstTaskProp->dwTaskIdx)))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: ReadTaskAcceptCondition failed, Idx invalid! LineIdx[%d], TaskIdx[%d]!"), MSG_MARK, pstTaskProp->byTaskLineIdx, pstTaskProp->dwTaskIdx);
            SDDelete pstTaskProp;
            pstTaskProp = NULL;
            return FALSE;
        }
        dwLastTaskIdx = pstTaskProp->dwTaskIdx;
        if(0 == poTaskLineProp->dwFirstTaskIdx)
        {
            poTaskLineProp->dwFirstTaskIdx = dwLastTaskIdx;
        }

        string strTmp = xmlNodePropTask("Type").AsString();
        pstTaskProp->eTaskType = GetTaskType(strTmp);
        if(INVALID == pstTaskProp->eTaskType)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: task type is invalid! LineIdx[%d], TaskIdx[%d]!"), MSG_MARK, pstTaskProp->byTaskLineIdx, pstTaskProp->dwTaskIdx);
            SDDelete pstTaskProp;
            pstTaskProp = NULL;
            return FALSE;
        }
        //需下发utf8编码，配置为utf8
        pstTaskProp->strDispName = xmlNodePropTask("DispName").AsString();
        pstTaskProp->strDesc = xmlNodePropTask("Desc").AsString();

        CSDXMLNode xmlNodeTmp = xmlNodePropTask["AcceptCondition"];
        if(!ReadTaskAcceptCondition(xmlNodeTmp, pstTaskProp))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: ReadTaskAcceptCondition failed! LineIdx[%d], TaskIdx[%d]!"), MSG_MARK, pstTaskProp->byTaskLineIdx, pstTaskProp->dwTaskIdx);
            SDDelete pstTaskProp;
            pstTaskProp = NULL;
            return FALSE;
        }

        xmlNodeTmp = xmlNodePropTask["FinishCondition"];
        if(!ReadTaskFinishCondition(xmlNodeTmp, pstTaskProp))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: ReadTaskFinishCondition failed! LineIdx[%d], TaskIdx[%d]!"), MSG_MARK, pstTaskProp->byTaskLineIdx, pstTaskProp->dwTaskIdx);
            SDDelete pstTaskProp;
            pstTaskProp = NULL;
            return FALSE;
        }

        xmlNodeTmp = xmlNodePropTask["Encourage"];
        if(!ReadTaskEncourage(xmlNodeTmp, pstTaskProp))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: ReadTaskEncourage failed! LineIdx[%d], TaskIdx[%d]!"), MSG_MARK, pstTaskProp->byTaskLineIdx, pstTaskProp->dwTaskIdx);
            SDDelete pstTaskProp;
            pstTaskProp = NULL;
            return FALSE;
        }

        //只有主线才有解锁副本配置
        if(MAIN_LINE_TASK == poTaskLineProp->byTaskLineIdx)
        {
            xmlNodeTmp = xmlNodePropTask["UnlockInstance"];
            if(!ReadTaskUnlockInstance(xmlNodeTmp, pstTaskProp))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: ReadTaskUnlockBuild failed! LineIdx[%d], TaskIdx[%d]!"), MSG_MARK, pstTaskProp->byTaskLineIdx, pstTaskProp->dwTaskIdx);
                SDDelete pstTaskProp;
                pstTaskProp = NULL;
                return FALSE;
            }
        }

        poTaskLineProp->mapTaskProp[pstTaskProp->dwTaskIdx] = pstTaskProp;
        xmlNodePropTask = xmlNodePropTask.Sibling(pszNodeName);
    }

    return TRUE;
}

BOOL CTaskPropMgr::ReadTaskAcceptCondition(CSDXMLNode rootNode, STaskProp* pstTaskProp)
{
    if(rootNode == NULL)
    {
        return FALSE;
    }
    CHAR* pszNodeName = "Condition";
    CSDXMLNode xmlNode = rootNode[pszNodeName];
    BOOL bHaveData = FALSE;
    while(xmlNode != NULL)
    {
        string strTmp = xmlNode("Kind").AsString();
        EAcceptConditionType eAcceptType = GetAcceptConditionType(strTmp);
        CAcceptCondition* poAcceptCondition = NULL;
        switch(eAcceptType)
        {
        case EACT_PLAYER:
        {
            poAcceptCondition = SDNew CAcceptCondition_Player;
            ((CAcceptCondition_Player*)poAcceptCondition)->wPlayerLevel = xmlNode("Level").AsInteger();
            pstTaskProp->wAcceptPlayerLevel = ((CAcceptCondition_Player*)poAcceptCondition)->wPlayerLevel;
        }
        break;
        case EACT_INSTANCE:
        {
            poAcceptCondition = SDNew CAcceptCondition_Instance;
            ((CAcceptCondition_Instance*)poAcceptCondition)->wSceneIdx = xmlNode("SceneIdx").AsInteger();
            ((CAcceptCondition_Instance*)poAcceptCondition)->byTownIdx = xmlNode("TownIdx").AsInteger();
            ((CAcceptCondition_Instance*)poAcceptCondition)->byInstanceIdx = xmlNode("InstanceIdx").AsInteger();
            pstTaskProp->stAcceptInstanceID.wSceneIdx = ((CAcceptCondition_Instance*)poAcceptCondition)->wSceneIdx;
            pstTaskProp->stAcceptInstanceID.byTownIdx = ((CAcceptCondition_Instance*)poAcceptCondition)->byTownIdx;
            pstTaskProp->stAcceptInstanceID.byInstanceIdx = ((CAcceptCondition_Instance*)poAcceptCondition)->byInstanceIdx;
        }
        break;
        //case EACT_BUILD:
        //{
        //    poAcceptCondition = SDNew CAcceptCondition_Build;
        //    ((CAcceptCondition_Build*)poAcceptCondition)->byBuildKindID = xmlNode("BuildKindID").AsInteger();
        //    ((CAcceptCondition_Build*)poAcceptCondition)->wLevel = xmlNode("Level").AsInteger();
        //}
        //break;
        case EACT_MAINTASK:
        {
            //主线没有解锁任务配置
            if(MAIN_LINE_TASK == pstTaskProp->byTaskLineIdx)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: Main line task don't have task type accept condition!"), MSG_MARK);
                return FALSE;
            }

            poAcceptCondition = SDNew CAcceptCondition_MainTask;
            UINT32 dwTaskIdx = xmlNode("TaskIdx").AsInteger();
            ((CAcceptCondition_MainTask*)poAcceptCondition)->dwTaskIdx = dwTaskIdx;
        }
        break;
        default:
            SYS_CRITICAL(_SDT("[%s: %d]: AcceptConditionType is invalid!"), MSG_MARK);
            return FALSE;
        }

        pstTaskProp->mapAcceptCondition[poAcceptCondition->GetAcceptType()] = poAcceptCondition;
        bHaveData = TRUE;
        xmlNode = xmlNode.Sibling(pszNodeName);
    }

    return bHaveData;
}

BOOL CTaskPropMgr::ReadTaskFinishCondition(CSDXMLNode rootNode, STaskProp* pstTaskProp)
{
    if(rootNode == NULL)
    {
        return FALSE;
    }

    switch(pstTaskProp->eTaskType)
    {
    case CROWN:
    {
        UINT16 wCrownTimes = rootNode("CrownTimes").AsInteger();
        if (0 == wCrownTimes)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_Crown;
        ((CFinishCondition_Crown*)(pstTaskProp->poFinishCondition))->wCrownTimes = wCrownTimes;
    }
    break;
    case RECRUITHERO:
    {
        UINT16 wRecruitHeroKindID1 = rootNode("RecruitHeroKindID1").AsInteger();
        UINT16 wRecruitHeroKindID2 = rootNode("RecruitHeroKindID2").AsInteger();
        if ((0 == wRecruitHeroKindID1) && (0 == wRecruitHeroKindID2))
        {
            return FALSE;
        }

        pstTaskProp->poFinishCondition = SDNew CFinishCondition_RecruitHero;
        ((CFinishCondition_RecruitHero*)(pstTaskProp->poFinishCondition))->wRecruitHeroKindID1 = wRecruitHeroKindID1;
        ((CFinishCondition_RecruitHero*)(pstTaskProp->poFinishCondition))->wRecruitHeroKindID2 = wRecruitHeroKindID2;

    }
    break;
    case FIRSTPASSINSTANCE:
    {
        UINT16 wSceneIdx = rootNode("SceneIdx").AsInteger();
        if (0 == wSceneIdx)
        {
            return FALSE;
        }

        UINT8 byTownIdx = rootNode("TownIdx").AsInteger();
        if (0 == byTownIdx)
        {
            return FALSE;
        }

        UINT8 byInstanceIdx = rootNode("InstanceIdx").AsInteger();
        if (0 == byInstanceIdx)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_FirstPassInstance;
        ((CFinishCondition_FirstPassInstance*)(pstTaskProp->poFinishCondition))->wSceneIdx = wSceneIdx;
        ((CFinishCondition_FirstPassInstance*)(pstTaskProp->poFinishCondition))->byTownIdx = byTownIdx;
        ((CFinishCondition_FirstPassInstance*)(pstTaskProp->poFinishCondition))->byInstanceIdx = byInstanceIdx;
    }
    break;
    case SCOREPASSINSTANCE:
    {
        UINT16 wSceneIdx = rootNode("SceneIdx").AsInteger();
        if (0 == wSceneIdx)
        {
            return FALSE;
        }

        UINT8 byTownIdx = rootNode("TownIdx").AsInteger();
        if (0 == byTownIdx)
        {
            return FALSE;
        }

        UINT8 byInstanceIdx = rootNode("InstanceIdx").AsInteger();
        if (0 == byInstanceIdx)
        {
            return FALSE;
        }

        UINT8 byScore = rootNode("Score").AsInteger();
        if (0 == byInstanceIdx)
        {
            return FALSE;
        }

        pstTaskProp->poFinishCondition = SDNew CFinishCondition_ScorePassInstance;
        ((CFinishCondition_ScorePassInstance*)(pstTaskProp->poFinishCondition))->wSceneIdx = wSceneIdx;
        ((CFinishCondition_ScorePassInstance*)(pstTaskProp->poFinishCondition))->byTownIdx = byTownIdx;
        ((CFinishCondition_ScorePassInstance*)(pstTaskProp->poFinishCondition))->byInstanceIdx = byInstanceIdx;
        ((CFinishCondition_ScorePassInstance*)(pstTaskProp->poFinishCondition))->byScore = byScore;

    }
    break;
    case TIMESPASSINSTANCE:
    {
        UINT16 wSceneIdx = rootNode("SceneIdx").AsInteger();
        if (0 == wSceneIdx)
        {
            return FALSE;
        }

        UINT8 byTownIdx = rootNode("TownIdx").AsInteger();
        if (0 == byTownIdx)
        {
            return FALSE;
        }

        UINT8 byInstanceIdx = rootNode("InstanceIdx").AsInteger();
        if (0 == byInstanceIdx)
        {
            return FALSE;
        }

        UINT8 byTimes = rootNode("Times").AsInteger();
        if (0 == byInstanceIdx)
        {
            return FALSE;
        }

        pstTaskProp->poFinishCondition = SDNew CFinishCondition_TimesPassInstance;
        ((CFinishCondition_TimesPassInstance*)(pstTaskProp->poFinishCondition))->wSceneIdx = wSceneIdx;
        ((CFinishCondition_TimesPassInstance*)(pstTaskProp->poFinishCondition))->byTownIdx = byTownIdx;
        ((CFinishCondition_TimesPassInstance*)(pstTaskProp->poFinishCondition))->byInstanceIdx = byInstanceIdx;
        ((CFinishCondition_TimesPassInstance*)(pstTaskProp->poFinishCondition))->byTimes = byTimes;
    }
    break;
    case KILLMONSTERINSTANCE:
    {
        UINT16 wSceneIdx = rootNode("SceneIdx").AsInteger();
        if (0 == wSceneIdx)
        {
            return FALSE;
        }

        UINT8 byTownIdx = rootNode("TownIdx").AsInteger();
        if (0 == byTownIdx)
        {
            return FALSE;
        }

        UINT8 byInstanceIdx = rootNode("InstanceIdx").AsInteger();
        if (0 == byInstanceIdx)
        {
            return FALSE;
        }

        UINT16 wMonsterKindID = rootNode("MonsterKindID").AsInteger();
        if (0 == wMonsterKindID)
        {
            return FALSE;
        }

        UINT8 byKillNum = rootNode("KillNum").AsInteger();
        if (0 == byKillNum)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_KillMonstersInstance;
        ((CFinishCondition_KillMonstersInstance*)(pstTaskProp->poFinishCondition))->wSceneIdx = wSceneIdx;
        ((CFinishCondition_KillMonstersInstance*)(pstTaskProp->poFinishCondition))->byTownIdx = byTownIdx;
        ((CFinishCondition_KillMonstersInstance*)(pstTaskProp->poFinishCondition))->byInstanceIdx = byInstanceIdx;
        ((CFinishCondition_KillMonstersInstance*)(pstTaskProp->poFinishCondition))->wMonsterKindID = wMonsterKindID;
        ((CFinishCondition_KillMonstersInstance*)(pstTaskProp->poFinishCondition))->byKillNum = byKillNum;
    }
    break;
    case STRENGTHENEQUIPLEVEL:
    {
        UINT16 wEquipKindID = rootNode("EquipKindID").AsInteger();
        //if (0 == wEquipKindID)
        //{
        //	return FALSE;
        //}

        UINT16 wLevel = rootNode("Level").AsInteger();
        if (0 == wLevel)
        {
            return FALSE;
        }

        pstTaskProp->poFinishCondition = SDNew CFinishCondition_StrengthenEquipLevel;
        ((CFinishCondition_StrengthenEquipLevel*)(pstTaskProp->poFinishCondition))->wEquipKindID = wEquipKindID;
        ((CFinishCondition_StrengthenEquipLevel*)(pstTaskProp->poFinishCondition))->wLevel = wLevel;

    }
    break;
    case STRENGTHENEQUIPNUM:
    {
        UINT16 wEquipKindID = rootNode("EquipKindID").AsInteger();
        //if (0 == wEquipKindID)
        //{
        //	return FALSE;
        //}

        UINT16 wNum = rootNode("Num").AsInteger();
        if (0 == wNum)
        {
            return FALSE;
        }

        pstTaskProp->poFinishCondition = SDNew CFinishCondition_StrengthenEquipNum;
        ((CFinishCondition_StrengthenEquipNum*)(pstTaskProp->poFinishCondition))->wEquipKindID = wEquipKindID;
        ((CFinishCondition_StrengthenEquipNum*)(pstTaskProp->poFinishCondition))->wNum = wNum;

    }
    break;
    case ACTIVATEGODWEAPON:
    {
        UINT16 wHeroKindID = rootNode("HeroKindID").AsInteger();
        //if (0 == wHeroKindID)
        //{
        //	return FALSE;
        //}

        pstTaskProp->poFinishCondition = SDNew CFinishCondition_ActivateGodweapon;
        ((CFinishCondition_ActivateGodweapon*)(pstTaskProp->poFinishCondition))->wHeroKindID = wHeroKindID;
    }
    break;
    case UPGRADEGODWEAPON:
    {
        UINT16 wHeroKindID = rootNode("HeroKindID").AsInteger();
        //if (0 == wHeroKindID)
        //{
        //	return FALSE;
        //}

        UINT16 wQuality = rootNode("Quality").AsInteger();
        //if (0 == wQuality)
        //{
        //	return FALSE;
        //}
        UINT16 wLevel = rootNode("Level").AsInteger();
        //if (0 == wLevel)
        //{
        //	return FALSE;
        //}
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_UpgradeGodweapon;
        ((CFinishCondition_UpgradeGodweapon*)(pstTaskProp->poFinishCondition))->wHeroKindID = wHeroKindID;
        ((CFinishCondition_UpgradeGodweapon*)(pstTaskProp->poFinishCondition))->wQuality = wQuality;
        ((CFinishCondition_UpgradeGodweapon*)(pstTaskProp->poFinishCondition))->wLevel = wLevel;
    }
    break;
    case USEEXPRIENCEDRUG:
    {
        UINT16 wNum = rootNode("Num").AsInteger();
        if (0 == wNum)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_UseExprienceDrug;
        ((CFinishCondition_UseExprienceDrug*)(pstTaskProp->poFinishCondition))->wNum = wNum;
    }
    break;
    case MELTEQUIP:
    {
        UINT16 wNum = rootNode("Num").AsInteger();
        if (0 == wNum)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_MultEquip;
        ((CFinishCondition_MultEquip*)(pstTaskProp->poFinishCondition))->wNum = wNum;
    }
    break;
    case STUDY:
    {
        UINT8 byLevel = rootNode("Level").AsInteger();
        if (0 == byLevel)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_Study;
        ((CFinishCondition_Study*)(pstTaskProp->poFinishCondition))->byLevel = byLevel;
    }
    break;
    case COMPOSEGOOD:
    {
        UINT16 wMaterialID = rootNode("MaterialID").AsInteger();
        if (0 == wMaterialID)
        {
            return FALSE;
        }
        UINT16 wMaterialNum = rootNode("MaterialNum").AsInteger();
        if (0 == wMaterialNum)
        {
            return FALSE;
        }
        UINT16 wKindID = rootNode("KindID").AsInteger();
        if (0 == wKindID)
        {
            return FALSE;
        }
        UINT16 wNum = rootNode("Num").AsInteger();
        if (0 == wNum)
        {
            wNum = 1;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_ComposeGood;
        ((CFinishCondition_ComposeGood*)(pstTaskProp->poFinishCondition))->wMaterialID = wMaterialID;
        ((CFinishCondition_ComposeGood*)(pstTaskProp->poFinishCondition))->wMaterialNum = wMaterialNum;
        ((CFinishCondition_ComposeGood*)(pstTaskProp->poFinishCondition))->wKindID = wKindID;
        ((CFinishCondition_ComposeGood*)(pstTaskProp->poFinishCondition))->wNum = wNum;
    }
    break;
    case ELITEINSTANCE:
    {
        UINT16 wSceneIdx = rootNode("SceneIdx").AsInteger();
        if (0 == wSceneIdx)
        {
            return FALSE;
        }

        UINT8 byTownIdx = rootNode("TownIdx").AsInteger();
        if (0 == byTownIdx)
        {
            return FALSE;
        }

        UINT8 byInstanceIdx = rootNode("InstanceIdx").AsInteger();
        if (0 == byInstanceIdx)
        {
            return FALSE;
        }

        pstTaskProp->poFinishCondition = SDNew CFinishCondition_EliteInstance;
        ((CFinishCondition_EliteInstance*)(pstTaskProp->poFinishCondition))->wSceneIdx = wSceneIdx;
        ((CFinishCondition_EliteInstance*)(pstTaskProp->poFinishCondition))->byTownIdx = byTownIdx;
        ((CFinishCondition_KillMonstersInstance*)(pstTaskProp->poFinishCondition))->byInstanceIdx = byInstanceIdx;
    }
    break;
    case UPGRADSCIENCE:
    {
        UINT16 wNum = rootNode("Num").AsInteger();
        if (0 == wNum)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_UpgradeScience;
        ((CFinishCondition_UpgradeScience*)(pstTaskProp->poFinishCondition))->wNum = wNum;
    }
    break;
    case STRENGTHATTACK:
    {
        UINT16 wNum = rootNode("Num").AsInteger();
        if (0 == wNum)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_StrengthAttack;
        ((CFinishCondition_StrengthAttack*)(pstTaskProp->poFinishCondition))->wNum = wNum;
    }
    break;
    case STRENGTHLIFE:
    {
        UINT16 wNum = rootNode("Num").AsInteger();
        if (0 == wNum)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_StrengthLife;
        ((CFinishCondition_StrengthLife*)(pstTaskProp->poFinishCondition))->wNum = wNum;
    }
    break;
    case CROP:
    {
        UINT16 wNum = rootNode("Num").AsInteger();
        if (0 == wNum)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_Crop;
        ((CFinishCondition_Crop*)(pstTaskProp->poFinishCondition))->wNum = wNum;
    }
    break;
    case CHALLENGE:
    {
        UINT16 wChallengeNum = rootNode("ChallengeNum").AsInteger();
        if (0 == wChallengeNum)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_Challenge;
        ((CFinishCondition_Challenge*)(pstTaskProp->poFinishCondition))->wChallengeNum = rootNode("ChallengeNum").AsInteger();
    }
    break;
    case PLUNDER:
    {
        UINT16 wPlunderNum = rootNode("PlunderNum").AsInteger();
        if (0 == wPlunderNum)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_Plunder;
        ((CFinishCondition_Plunder*)(pstTaskProp->poFinishCondition))->wPlunderNum = rootNode("PlunderNum").AsInteger();
    }
    break;
    case UPGRADESKILL:
    {
        UINT16 wUnlockIdx = rootNode("UnlockIdx").AsInteger();
        UINT16 wLevel = rootNode("Level").AsInteger();
        if (0 == wLevel)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_UpgradeSkill;
        ((CFinishCondition_UpgradeSkill*)(pstTaskProp->poFinishCondition))->wUnlockIdx = wUnlockIdx;
        ((CFinishCondition_UpgradeSkill*)(pstTaskProp->poFinishCondition))->wLevel = wLevel;
    }
    break;
    case UPGRADEJINGJIE:
    {
        UINT16 wLevel = rootNode("Level").AsInteger();
        if (0 == wLevel)
        {
            return FALSE;
        }
        UINT16 wSubLevel = rootNode("SubLevel").AsInteger();
        if (0 == wSubLevel)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_UpgradeJingJie;
        ((CFinishCondition_UpgradeJingJie*)(pstTaskProp->poFinishCondition))->wLevel = wLevel;
        ((CFinishCondition_UpgradeJingJie*)(pstTaskProp->poFinishCondition))->wSubLevel = wSubLevel;
    }
    break;
    case MOSAICGEM:
    {
        UINT16 wNum = rootNode("Num").AsInteger();
        if (0 == wNum)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_MosaicGem;
        ((CFinishCondition_MosaicGem*)(pstTaskProp->poFinishCondition))->wNum = wNum;
    }
    break;
    case CLIMBTOWER:
    {
        UINT16 wNum = rootNode("Num").AsInteger();
        if (0 == wNum)
        {
            return FALSE;
        }
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_ClimbTower;
        ((CFinishCondition_ClimbTower*)(pstTaskProp->poFinishCondition))->wNum = wNum;
    }
    break;
    case DRESSEQUIP:
    {
        UINT16 wEquipID = rootNode("EquipID").AsInteger();
        //if (0 == wEquipID)
        //{
        //	return FALSE;
        //}
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_DressEquip;
        ((CFinishCondition_DressEquip*)(pstTaskProp->poFinishCondition))->wEquipID = wEquipID;
    }
    break;
    case RECRUITHERONUM:
    {
        UINT16 wNum = rootNode("Num").AsInteger();
        if (0 == wNum)
        {
            return FALSE;
        }

        pstTaskProp->poFinishCondition = SDNew CFinishCondition_RecruitHeroNum;
        ((CFinishCondition_RecruitHeroNum*)(pstTaskProp->poFinishCondition))->wNum = wNum;
    }
    break;
    case OPENLIGHT:
    {
        UINT8 byExt = rootNode("Ext").AsInteger();
        pstTaskProp->poFinishCondition = SDNew CFinishCondition_OpenLight;
        ((CFinishCondition_OpenLight*)(pstTaskProp->poFinishCondition))->byExt = 1;
    }
    break;
    default:
        return FALSE;
    }

    return TRUE;
}

BOOL CTaskPropMgr::ReadTaskEncourage(CSDXMLNode rootNode, STaskProp* pstTaskProp)
{
    if(rootNode == NULL)
    {
        return FALSE;
    }

    STaskEncourageProp& stTaskEncourageProp = pstTaskProp->stTaskEncourageProp;
    CSDXMLNode xmlNode = rootNode["Coin"];
    //不一定有配置
    if(xmlNode != NULL)
    {
        stTaskEncourageProp.dwCoin = xmlNode("Value").AsInteger();
    }

    xmlNode = rootNode["Gold"];
    //不一定有配置
    if(xmlNode != NULL)
    {
        stTaskEncourageProp.dwGold = xmlNode("Value").AsInteger();
    }


    xmlNode = rootNode["PhyStrength"];
    //不一定有配置
    if(xmlNode != NULL)
    {
        stTaskEncourageProp.wPhyStrength = xmlNode("Value").AsInteger();
    }


    xmlNode = rootNode["Science"];
    //不一定有配置
    if(xmlNode != NULL)
    {
        stTaskEncourageProp.dwScience = xmlNode("Value").AsInteger();
    }


    xmlNode = rootNode["Fame"];
    //不一定有配置
    if(xmlNode != NULL)
    {
        stTaskEncourageProp.dwFame = xmlNode("Value").AsInteger();
    }


    xmlNode = rootNode["Experience"];
    //不一定有配置
    if(xmlNode != NULL)
    {
        stTaskEncourageProp.dwExperience = xmlNode("Value").AsInteger();
    }

    xmlNode = rootNode["Story"];
    //不一定有配置
    if(xmlNode != NULL)
    {
        stTaskEncourageProp.dwStory = xmlNode("Value").AsInteger();
    }

    xmlNode = rootNode["BlueGas"];
    //不一定有配置
    if(xmlNode != NULL)
    {
        stTaskEncourageProp.dwBlueGas = xmlNode("Value").AsInteger();
    }

    xmlNode = rootNode["PurpleGas"];
    //不一定有配置
    if(xmlNode != NULL)
    {
        stTaskEncourageProp.dwPurpleGas = xmlNode("Value").AsInteger();
    }

    xmlNode = rootNode["JingJie"];
    //不一定有配置
    if(xmlNode != NULL)
    {
        stTaskEncourageProp.dwJingJie = xmlNode("Value").AsInteger();
    }

    CSDXMLNode xmlItemNodeProp = rootNode["Item"];
    while(xmlItemNodeProp != NULL)
    {
        UINT16 wKindID = xmlItemNodeProp("KindID").AsInteger();
        UINT8 byNum = xmlItemNodeProp("Num").AsInteger();
        if((0 == wKindID) || (0 == byNum))
        {
            xmlItemNodeProp = xmlItemNodeProp.Sibling("Item");
            continue;
        }
        stTaskEncourageProp.mapItemNum[wKindID] = byNum;

        xmlItemNodeProp = xmlItemNodeProp.Sibling("Item");
    }

    return TRUE;

}

BOOL CTaskPropMgr::ReadTaskUnlockInstance(CSDXMLNode rootNode, STaskProp* pstTaskProp)
{
    //不一定有配置,没有则取上次配置
    if(rootNode == NULL)
    {
        pstTaskProp->stInstanceID = m_stLastInstanceID;
        pstTaskProp->bNewUnlockInstanceID = FALSE;
        return TRUE;
    }

    DT_INSTANCE_ID& stInstanceID = pstTaskProp->stInstanceID;
    stInstanceID.wSceneIdx = rootNode("SceneIdx").AsInteger();
    stInstanceID.byTownIdx = rootNode("TownIdx").AsInteger();
    stInstanceID.byInstanceIdx = rootNode("InstanceIdx").AsInteger();
    //解锁配置错误，比之前的小/等
    if((stInstanceID.wSceneIdx < m_stLastInstanceID.wSceneIdx) ||
            ((stInstanceID.wSceneIdx == m_stLastInstanceID.wSceneIdx) && (stInstanceID.byTownIdx < m_stLastInstanceID.byTownIdx)) ||
            ((stInstanceID.wSceneIdx == m_stLastInstanceID.wSceneIdx) && (stInstanceID.byTownIdx == m_stLastInstanceID.byTownIdx) && (stInstanceID.byInstanceIdx <= m_stLastInstanceID.byInstanceIdx)))
    {
        if((1 != stInstanceID.wSceneIdx) && (1 != stInstanceID.byTownIdx) && (1 != stInstanceID.byInstanceIdx))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: ReadTaskUnlockBuild failed! Onlock instance idx invalid,task[%d]"), MSG_MARK, pstTaskProp->dwTaskIdx);
            return FALSE;
        }
    }

    pstTaskProp->bNewUnlockInstanceID = TRUE;
    m_stLastInstanceID = stInstanceID;

    return TRUE;
}

/*
BOOL CTaskPropMgr::ReadBranchTask(CSDXMLNode rootNode, STaskProp* pstLineTaskProp)
{
	if(rootNode == NULL)
	{
		return FALSE;
	}
	UINT8 byBranchIdx = 1;
	CHAR* pszNodeName = "BranchTask";
	CSDXMLNode xmlNode = rootNode[pszNodeName];
	while(xmlNode != NULL)
	{
		STaskProp* pstTaskProp = SDNew STaskProp;
		string strTmp = xmlNode("Type").AsString();
		pstTaskProp->eTaskType = GetTaskType(strTmp);
		if(INVALID == pstTaskProp->eTaskType)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: task type is invalid!"), MSG_MARK);
			SDDelete pstTaskProp;
			pstTaskProp = NULL;
			return FALSE;
		}
		//需下发utf8编码，配置为utf8
		pstTaskProp->strDispName = xmlNode("DispName").AsString();
		pstTaskProp->strDesc = xmlNode("Desc").AsString();

		CSDXMLNode xmlNodeTmp = xmlNode["AcceptCondition"];
		if(!ReadTaskAcceptCondition(xmlNodeTmp, pstTaskProp))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: ReadTaskAcceptCondition failed!"), MSG_MARK);
			SDDelete pstTaskProp;
			pstTaskProp = NULL;
			return FALSE;
		}

		xmlNodeTmp = xmlNode["FinishCondition"];
		if(!ReadTaskFinishCondition(xmlNodeTmp, pstTaskProp))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: ReadTaskFinishCondition failed!"), MSG_MARK);
			SDDelete pstTaskProp;
			pstTaskProp = NULL;
			return FALSE;
		}

		xmlNodeTmp = xmlNode["Encourage"];
		if(!ReadTaskEncourage(xmlNodeTmp, pstTaskProp))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: ReadTaskEncourage failed!"), MSG_MARK);
			SDDelete pstTaskProp;
			pstTaskProp = NULL;
			return FALSE;
		}

		pstLineTaskProp->mapBranchTask[byBranchIdx++] = pstTaskProp;

		xmlNode = xmlNode.Sibling(pszNodeName);
	}

	return TRUE;
}
*/

VOID CTaskPropMgr::UnInit()
{
    for(CTaskLine2TaskLinePropMapItr itr = m_mapTaskLine.begin(); itr != m_mapTaskLine.end(); itr++)
    {
        SDDelete itr->second;
        itr->second = NULL;

    }
    m_mapTaskLine.clear();

    m_mapStr2Enum.clear();

    m_stInitUnlockInstanceID.wSceneIdx = 1;
    m_stInitUnlockInstanceID.byTownIdx = 1;
    m_stInitUnlockInstanceID.byInstanceIdx = 1;
    m_stLastInstanceID = m_stInitUnlockInstanceID;

}


STaskProp* CTaskPropMgr::GetTaksProp(UINT8 byTaskLine, UINT32 dwTaskIdx)
{
    CTaskLine2TaskLinePropMapItr itrLine = m_mapTaskLine.find(byTaskLine);
    if(itrLine == m_mapTaskLine.end())
    {
        return NULL;
    }

    CTaskLineProp* pstTaskLineProp = itrLine->second;

    CTaskIdx2TaskPropMapItr itrTask = pstTaskLineProp->mapTaskProp.find(dwTaskIdx);
    if(itrTask == pstTaskLineProp->mapTaskProp.end())
    {
        return NULL;
    }

    return itrTask->second;
}

CTaskLineProp* CTaskPropMgr::GetTaskLineProp(UINT8 byTaskLine)
{
    CTaskLine2TaskLinePropMapItr itrLine = m_mapTaskLine.find(byTaskLine);
    if(itrLine != m_mapTaskLine.end())
    {
        return itrLine->second;
    }

    return NULL;
}


VOID CTaskPropMgr::InitNextTaskIdx()
{
    for(CTaskLine2TaskLinePropMapItr itrLine = m_mapTaskLine.begin(); itrLine != m_mapTaskLine.end(); itrLine++)
    {
        CTaskLineProp* pstTaskLineProp = itrLine->second;
        CTaskIdx2TaskPropMap& mapTaskProp = pstTaskLineProp->mapTaskProp;
        UINT32 dwNextTaskIdx = 0;
        for(CTaskIdx2TaskPropMap::reverse_iterator itrTask = mapTaskProp.rbegin(); itrTask != mapTaskProp.rend(); itrTask++)
        {
            STaskProp* pstTaskProp = itrTask->second;
            pstTaskProp->dwNextTaskIdx = dwNextTaskIdx;
            dwNextTaskIdx = pstTaskProp->dwTaskIdx;
        }
    }
}

