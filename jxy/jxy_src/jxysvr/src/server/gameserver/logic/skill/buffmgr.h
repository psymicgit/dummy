
#ifndef _BUFFMGR_H_
#define _BUFFMGR_H_

#include "buff.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>

using namespace SGDP;




typedef HashMap<UINT32, CBuff*> CID2BuffMap;
typedef CID2BuffMap::iterator CID2BuffMapItr;


/**
* @brief BUFF管理类
*/
class CBuffMgr
{
public:
    DECLARE_SINGLETON(CBuffMgr)

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
    * 创建Buff
    * @return CBuff*       : Buff对象
    */
	CBuff*		CreateBuff();


    /**
    * @brief
    * 移除Buff，从管理器中移除
    * @return VOID
    */
	VOID            RemoveBuff(UINT32 dwBuffID);
 
protected:
    CBuffMgr();
    ~CBuffMgr();


private:
	CSDObjectPool<CBuff, CSDMutex>		m_oBuffPool;
	CID2BuffMap							m_mapID2Buff;
	static UINT32						m_dwBuffIDGen;//用于生成内存中的ID
};



#endif //#ifndef _BUFFMGR_H_

