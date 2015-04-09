
#ifndef _BUILDMGR_H_
#define _BUILDMGR_H_

#include "buildpropmgr.h"
#include <sdsingleton.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <sdobjectpool.h>

using namespace SGDP;

class CBuild;

typedef HashMap<UINT64, CBuild*> CID2BuildMap;//key为内存唯一ID
typedef CID2BuildMap::iterator	CID2BuildMapItr;

typedef HashMap<UINT8, CBuild*> CKind2BuildMap; //key为建筑类型
typedef CKind2BuildMap::iterator CKind2BuildMapItr;


class CPlayer;
/**
* @brief 建筑管理类
*/
class CBuildMgr
{
public:
    DECLARE_SINGLETON(CBuildMgr)

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

public:

	CBuild*		CreateBuild(UINT32 dwPlayerID, UINT16 wKindID);
	VOID        RemoveBuild(UINT32 dwPlayerID, UINT16 wKindID);
 
protected:
    CBuildMgr();
    ~CBuildMgr();

//////////////////////////////////////////////////////
public:


///////////////////////////////////////////////////////


private:
    CID2BuildMap			m_mapID2Build;
	CSDObjectPool<CBuild, CSDMutex> m_oBuildShmemObjMgr;	
	
};




#endif //#ifndef _ROLEMGR_H_

