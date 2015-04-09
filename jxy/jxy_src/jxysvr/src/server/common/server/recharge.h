#ifndef _RECHARGE_H_
#define _RECHARGE_H_

#include <sdbase.h>
#include <string>

using namespace std;
using namespace SGDP;


//充值错误码类型
enum ERechargeErrCode
{
	EREC_SUCCESS = 0,  //成功
	EREC_TIMEOUT = 1, //超时
	EREC_INVALID_CHART = 2, //包含无效字符
	EREC_INNERR = 3, //内部错误
};



//充值来源类型
enum ERechargeFromType
{
	ERFT_TONGBU = 1, //同步
	ERFT_PP = 2, //PP助手
};


class CRechargeClient
{
public:
	virtual	VOID SDAPI	OnRechargeSuccess()=0;	
	virtual	VOID SDAPI	OnRechargeFailed(UINT16 wErrCode, TCHAR* pszErrCode)=0;
};


class CRechargeClientMgr
{
public:
	virtual CRechargeClient* SDAPI FindClient(UINT32 dwTransID) = 0;
};

class CRechargeModule
{
public:

	virtual BOOL SDAPI Init(const CHAR* pszModuleName, const CHAR* pszConfig, CRechargeClientMgr* poRechargeClientMgr) = 0;

	virtual BOOL SDAPI Recharge(UINT32 dwTransID, string strOrderInfo) = 0;

	virtual BOOL SDAPI Run(INT32 nCount = -1) = 0;

	virtual VOID SDAPI Release(VOID) = 0;
};

extern "C" CRechargeModule* SDAPI GetRechargeModule(void);

	
#endif 
