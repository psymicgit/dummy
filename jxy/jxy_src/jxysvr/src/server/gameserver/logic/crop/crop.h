#ifndef _CROP_H_
#define _CROP_H_


#include <logic/base/baseobj.h>
#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <common/server/utility.h>

using namespace SGDP;

class CPlayer;


class CCrop: public CBaseObj
{
public:
    CCrop();
    ~CCrop();

public:

    //初始化相关对象
    BOOL Init(DT_CROP_DATA &m_stDT_CROP_DATA, CPlayer* poOwner);

    DT_CROP_DATA& GetDT_CROP_DATA()
    {
        return m_stDT_CROP_DATA;
    };

public:
	UINT16	GetDT_CROP_SEED_INFO_DATA(DT_CROP_SEED_INFO_DATA stDT_CROP_SEED_INFO_DATA[MAX_CROP_SEED_NUM], UINT8 &byCropSeedNum);
	UINT16	GetDT_CROP_FIELD_INFO_DATA(DT_CROP_FIELD_INFO_DATA stCropFieldInfoList[MAX_CROP_NUM]); //田地信息
	UINT16	GetDT_CROP_FIELD_INFO_DATA(UINT8 byFieldID, DT_CROP_FIELD_INFO_DATA &stCropFieldInfo); //田地信息
	UINT16	GetDT_CROP_MORE_INFO_DATA(DT_CROP_MORE_INFO_DATA stDT_CROP_MORE_INFO_DATA[MAX_CROP_NUM]); 
	UINT16	GetDT_CROP_MORE_INFO_DATA(UINT8 byFieldID,  DT_CROP_MORE_INFO_DATA &stDT_CROP_MORE_INFO_DATA); 

	UINT16	Crop(UINT8 byFieldID, UINT16 wCropKindID);
	UINT16	GainCrop(UINT8 byFieldID, UINT8 &byCropType, UINT32 &dwCropValue, UINT16 &wCropKindID);
	UINT16	OpenCropField(UINT8 byFieldID);
	UINT16	ClrCrop(UINT8 byFieldID);
protected:

private:
    DT_CROP_DATA			m_stDT_CROP_DATA;//
};
DECLARE_CLASS_POOL_MGR(CCrop)
#endif
