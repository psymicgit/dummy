#ifndef _SCIENCE_TREE_H_
#define _SCIENCE_TREE_H_

#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <common/server/utility.h>
#include <logic/base/baseobj.h>

using namespace SGDP;


class CScienceTree: public CBaseObj
{
public:
    CScienceTree();
    ~CScienceTree();
public:
    //初始化相关对象
    BOOL Init(DT_SCIENCE_TREE_DATA &stDT_SCIENCE_TREE_DATA, CPlayer* poOwner);

    DT_SCIENCE_TREE_DATA& GetDT_SCIENCE_TREE_DATA()
    {
        return m_stDT_SCIENCE_TREE_DATA;
    };

public:
    //CScienceTree *GetScienceTree() { return m_poScienceTree; };
    UINT16 GetScienceTreeBuildInfo( UINT64 &qwCurStory, UINT8 &byAlreadyOpenScienceNum, DT_SCIENCE_ALREADY_OPEN_DATA astAlreadyOpenScienceList[MAX_SCIENCE_KIND_NUM],
                                    UINT8 &byWillOpenNum, DT_SCIENCE_WILL_OPEN_DATA astWillOpenScienceList[MAX_SCIENCE_KIND_NUM] );

    INT32 UpdateScience( UINT8 wKind, UINT16 wLevel, DT_SCIENCE_ALREADY_OPEN_DATA &stUpdatedData,
                         DT_POWER_CHANGE_DATA &stPower, UINT64 &qwCurStory );

    VOID OnUpdateScience();
protected:
    //BOOL	InitScienceTree(DT_SCIENCE_TREE_DATA& stScienceTree);//初始化
    BOOL	GetScienceLevel( UINT8 byKind, UINT16 &wLevel ); //获得指定类型的等级

    //获得科技信息
    BOOL	GetScienceValue( UINT8 byKind, UINT16 wLevel, UINT32 &dwNextCost, UINT32 &dwCurValue, UINT32 &dwNextValue );

private:
    DT_SCIENCE_TREE_DATA	m_stDT_SCIENCE_TREE_DATA;
};
DECLARE_CLASS_POOL_MGR(CScienceTree);
#endif