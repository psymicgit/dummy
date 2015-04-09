
#ifndef _RATEMGR_H_
#define _RATEMGR_H_

#include <sdsingleton.h>
#include <common/server/utility.h>

#define MAX_RATEVEC_NUM 100 //随机序列段数

class CRateMgr
{
public:

    DECLARE_SINGLETON(CRateMgr);

public:

    BOOL Init();

    VOID UnInit();
public:
	vector<UINT8>& GetRateVec(UINT16 wRate); //随机获取1个随机序列
protected:
    CRateMgr();
    virtual ~CRateMgr();
protected:
	VOID InitRate();


private:

	vector<UINT8> m_aavecRadom[MAX_RATEVEC_NUM][101];

};

#endif // 
