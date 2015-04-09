#ifndef _CBREAKPAD_H_
#define _CBREAKPAD_H_
#include <sdtype.h>
#include <memory>
using std::auto_ptr;

#ifndef __linux__
#include "client/windows/handler/exception_handler.h"
#else
#include "client/linux/handler/exception_handler.h"
#endif

class CBreakPad
{
public:
	CBreakPad();
	virtual ~CBreakPad();

	/**
	* @brief 初始化异常捕获信息
	* @return VOID
	*/
	static VOID Init();

	/**
	* @brief 逆初始化，释放资源
	* @return VOID
	*/
	static VOID UnInit();
private:

#ifndef __linux
	static bool DumpsCallback(const wchar_t *pStrDumpPath, const wchar_t *pStrID,  
		void *pContext, EXCEPTION_POINTERS *pExceptInfo,  
		MDRawAssertionInfo *pAssertion,  
		bool bSucceeded);

#else
	static bool DumpsCallback(const google_breakpad::MinidumpDescriptor& descriptor,
		void* pContext,
		bool bSucceeded);

#endif


private:

#ifndef __linux__
	static google_breakpad::ExceptionHandler *sm_pHandler; //捕获异常的对象
#else
	static google_breakpad::MinidumpDescriptor *sm_pDumpDescript; //导出的描述参数
	static google_breakpad::ExceptionHandler *sm_pHandler; //捕获异常的对象
#endif

};

#endif