
#ifndef _TASKPROPMGR_H_
#define _TASKPROPMGR_H_

#include "taskprop.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <logic/base/basepropmgr.h>

using namespace SGDP;

typedef HashMap<UINT8, CTaskLineProp*> CTaskLine2TaskLinePropMap;//key为任务线号
typedef CTaskLine2TaskLinePropMap::iterator CTaskLine2TaskLinePropMapItr;

typedef HashMap<string, UINT8> CStr2EnumMap;  //字符串到枚举的转换
typedef CStr2EnumMap::iterator CStr2EnumMapItr;


/**
* @brief 任务管理类
*/
class CTaskPropMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CTaskPropMgr)
public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_TASK ; }
    BOOL Init();
    VOID UnInit();

    STaskProp* GetTaksProp(UINT8 byTaskLine, UINT32 dwTaskIdx);
    inline CTaskLine2TaskLinePropMap& GetTaskLine2TaskLinePropMap()  { return m_mapTaskLine; }
    inline DT_INSTANCE_ID& GetInitUnlockInstanceID() {  return m_stInitUnlockInstanceID;  } //返回默认初始解锁的副本
	CTaskLineProp* GetTaskLineProp(UINT8 byTaskLine);
public:
    CTaskPropMgr();
    ~CTaskPropMgr();
protected:
    BOOL	ReadTaskFromXML();
    BOOL	ReadTaskFromData(CTaskLineProp* poTaskLineProp, const CHAR* pszData, const UINT32 dwDataLen);
    BOOL	ReadTaskAcceptCondition(CSDXMLNode rootNode, STaskProp* pstTaskProp);
    BOOL	ReadTaskFinishCondition(CSDXMLNode rootNode, STaskProp* pstTaskProp);
    BOOL	ReadTaskEncourage(CSDXMLNode rootNode, STaskProp* pstTaskProp);
    BOOL	ReadTaskUnlockInstance(CSDXMLNode rootNode, STaskProp* pstTaskProp);
    //BOOL	ReadBranchTask(CSDXMLNode rootNode, STaskProp* pstLineTaskProp);
    //BOOL	ReadDailyTaskFromXML();


    VOID	InitStr2Enum();
    ETaskType GetTaskType(string strTaskType);
    EAcceptConditionType GetAcceptConditionType(string strTaskType);
    VOID	InitNextTaskIdx();
private:
    CTaskLine2TaskLinePropMap	m_mapTaskLine;//所有任务线任务
    CStr2EnumMap				m_mapStr2Enum;//字符串到枚举的转换，用于读取配置
    DT_INSTANCE_ID			m_stLastInstanceID;//用于读取配置文件
    DT_INSTANCE_ID			m_stInitUnlockInstanceID;//初始解锁副本ID
};

#endif //#ifndef _ROLEMGR_H_

