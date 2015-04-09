#ifndef _DRUG_H_
#define _DRUG_H_


#include <logic/base/baseobj.h>
#include <protocol/server/protocommondata.h>

using namespace SGDP;

struct tagDrugProp;
typedef tagDrugProp SDrugProp;

struct tagDT_DRUG_DATA_CLI;
typedef tagDT_DRUG_DATA_CLI DT_DRUG_DATA_CLI;

class CPlayer;
class CDrug: public CBaseObj
{
public:
    CDrug();
    ~CDrug();

public:
    //初始化相关对象
    BOOL Init(DT_DRUG_DATA* pstDT_DRUG_DATA, CPlayer* poOwner);

    DT_DRUG_DATA& GetDT_DRUG_DATA() { return m_stDT_DRUG_DATA; };
    VOID GetDT_DRUG_DATA_CLI(DT_DRUG_DATA_CLI& stDT_DRUG_DATA_CLI);
    SDrugProp* GetDrugProp();
    BOOL CkFull();//检查是否已经达到使用上限，达到最大使用上限返回TRUE

    UINT16 GetTalent() { return m_wTotalTalent; } ;//获取该等级丹药提升的资质

    VOID OnUse();//使用该等级丹药成功时回调，重新计算提升的资质

protected:
    VOID CountTalent();//

private:
    DT_DRUG_DATA		m_stDT_DRUG_DATA;//
    UINT16				m_wTotalTalent;//该等级丹药提升的资质,用于不需每次获取重新计算.使用新丹药时重新计算
};

#endif
