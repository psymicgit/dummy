
#ifndef _CROPFIELDMGRPROP_H_
#define _CROPFIELDMGRPROP_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>

//名称最大长度
#define NAME_LEN_MAX	32

//丹药配置
typedef struct tagCropFieldProp
{
	UINT16	wKindID;					//丹药等级
	TCHAR	szName[NAME_LEN_MAX];		//物品名称
	UINT8	byLevel;					//最大可使用数目
	UINT32	dwGold;					//使用间隔时间
	UINT32	dwCoin;					//资质基础加成值
}SCropFieldProp;


typedef std::map<UINT16, SCropFieldProp> CCropFieldPropMap; //key为Level
typedef CCropFieldPropMap::iterator CCropFieldPropMapItr;


class CCropFieldPropMgr
{
public:
	DECLARE_SINGLETON(CCropFieldPropMgr);

public:

	BOOL Init();

	VOID UnInit();
public:
	CCropFieldPropMap & GetCropFieldPropMap()	
	{ 
		return m_mapCropFieldProp;	
	}

	SCropFieldProp * GetCropFieldProp(UINT16 wFieldID)
	{
		CCropFieldPropMapItr itr = m_mapCropFieldProp.find(wFieldID);
		if (itr != m_mapCropFieldProp.end())
		{
			return &itr->second;
		}
		return NULL;
	}
protected:
	CCropFieldPropMgr();
	virtual ~CCropFieldPropMgr();
protected:

	BOOL LoadFromDB();

private:
	CCropFieldPropMap m_mapCropFieldProp;

};




#endif //#ifndef _ROLEMGR_H_

