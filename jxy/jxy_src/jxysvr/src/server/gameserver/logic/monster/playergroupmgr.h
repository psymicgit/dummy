
#ifndef _PLAYERGROUPMGR_H_
#define _PLAYERGROUPMGR_H_


#include "playergroup.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <protocol/server/protocommondata.h>


using namespace SGDP;

/**
* @brief 怪物管理类
*/
class CPlayerGroupMgr
{
    friend class CPlayerGroup;
public:
    DECLARE_SINGLETON(CPlayerGroupMgr)

    /**
    * @brief
    * 初始化函数,读取配置文件等
    * @return 执行结果，成功返回TRUE，失败返回FALSE
    */
    BOOL Init();

    /**
    * @brief
    * 卸载函数
    */
    VOID UnInit();

    /**
    * @brief
    * 在管理器创建玩家组队信息
    * @param vector<CHero*> vecHero		:	武将信息
    * @return CPlayerGroup*			:	玩家组
    * 对于分配出来的姐信息在使用完后要删除掉，否则会有内存泄露
    */
    CPlayerGroup*			CreatePlayerGroup(vector<CHero*> vecHero);
    /**
    * @brief
    * 删除玩家组队信息
    * @param CPlayerGroup* poPlayerGroup		:	武将信息
    */
    VOID					RemovePlayerGroup(CPlayerGroup* poPlayerGroup);
protected:
    CPlayerGroupMgr();
    ~CPlayerGroupMgr();

private:
    CSDObjectPool<CPlayerGroup, CSDMutex>	m_oPlayerGroupPool;
};




#endif //#ifndef

