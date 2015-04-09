
#ifndef _CROPFILEDMGR_H_
#define _CROPFILEDMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>

//名称最大长度
#define NAME_LEN_MAX	32


typedef struct tagCropProp
{
    UINT16   wCropKindID;
    TCHAR	szName[NAME_LEN_MAX];
    UINT8	byType;
    UINT8	byLevel;
    UINT16	wObtainCD;
    UINT32	dwObtainValue;
    UINT16	wObtainKindID;
} SCropProp;


typedef std::map<UINT16, SCropProp> CCropPropMap; //key为Level
typedef CCropPropMap::iterator CCropPropMapItr;


class CCropSeedPropMgr
{
public:
    DECLARE_SINGLETON(CCropSeedPropMgr);

public:

    BOOL Init();

    VOID UnInit();
public:
    CCropPropMap & GetCropPropMap()
    {
        return m_mapCropProp;
    }

    SCropProp * GetCropProp(UINT16 wItemID)
    {
        CCropPropMapItr itr = m_mapCropProp.find(wItemID);
        if (itr != m_mapCropProp.end())
        {
            return &itr->second;
        }
        return NULL;
    }

    //是否为丹药物品
    BOOL	IsCropGoods(UINT16 wItemID)
    {
        return (m_mapCropProp.end() != m_mapCropProp.find(wItemID));
    };

protected:
    CCropSeedPropMgr();
    virtual ~CCropSeedPropMgr();
protected:

    BOOL LoadFromDB();

private:
    CCropPropMap m_mapCropProp;
};




#endif //#ifndef _ROLEMGR_H_

