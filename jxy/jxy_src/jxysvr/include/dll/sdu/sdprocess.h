/******************************************************************************


sdprocess.h - 进程处理

******************************************************************************/

#ifndef SDPROCESS_H
#define SDPROCESS_H
/**
* @file sdprocess.h

* @brief 进程处理
*
**/
#include "sdtype.h"
#include <string>
#if (defined(WIN32) || defined(WIN64))
#include "Aclapi.h"
#include "Winbase.h"
#else
#include <dlfcn.h>
#endif // 

#include <vector>
using namespace std;


namespace SGDP
{
    /**
    * @defgroup groupthread 多进程多线程(包括线程池)
    * @ingroup  SGDP
    * @{
    */
#if (defined(WIN32) || defined(WIN64))
	typedef struct tagProcessSecInfo 
	{
		BOOL bSet; // 是否设置此安全信息
		SE_OBJECT_TYPE objectType;
		SECURITY_INFORMATION securityInfo;
		PSID psidOwner;
		PSID psidGroup;
		PACL pDacl;
		PACL pSacl;
		PSECURITY_DESCRIPTOR pSecurityDescriptor; // 获取时使用
	} SProcessSecInfo; 
#endif 

	/**
	* @brief 进程属性
	*/
    struct SProcessAttr
    {
        std::string  workDir;  //进程执行的工作目录
        std::string  environment;
        UINT32       createFlags;  // 进程创建标识 fow windows.
        BOOL         inherithandle;// for Windows
#if (defined(WIN32) || defined(WIN64))
		SProcessAttr()
		{
			secInfo.bSet = FALSE; 
		}
		SProcessSecInfo secInfo; 
#endif // 
    };


#if (defined(WIN32) || defined(WIN64))
typedef UINT32 SDPROCESSID; 
#else 
typedef pid_t  SDPROCESSID; 
#endif // 

    /**
    * @brief
    * 创建一个进程
    * @param progName : 该进程的执行文件
    * @param pCmdline : 命令行
    * @param pWorkDir : 该进程的工作目录
    * @param pAttr    : 该进程的属性
    * @return 成功返回TRUE，失败返回FALSE
    */
    SDHANDLE  SDCreateProcess(
        const CHAR *progName,
        const CHAR *pCmdLine = NULL,
        const CHAR *pWorkDir = NULL,
        SProcessAttr *pAttr = NULL);

    /**
    * @brief
    * 终止一个进程
    * @param handle : 进程句柄
    * @param err  :指定进程退出时的错误码信息
    * @return VOID
    */
    VOID  SDTerminateProcess(SDHANDLE handle, INT32 err = 0);

    /**
    * @brief
    * 关闭进程的句柄,对于进程不要使用通用的SDCloseHandle来关闭句柄,否则会导致资源未释放
    * @param handle : 进程句柄
    * @return VOID
    */
    VOID  SDCloseProcess(SDHANDLE handle);

	/**
	* @brief
	* 获取进程ID，进程必须设置属性
	* @param handle : 进程句柄
	* @return VOID
	*/
	SDPROCESSID  SDGetProcessId(SDHANDLE handle); 

	/**
	* @brief
	* 通过进程ID获取进程handle，
	* @param pid: 进程实际ID
	* @return VOID
	*/
	SDHANDLE  SDOpenProcess(SDPROCESSID pid); 

	/**
	* @brief
	* 获取当前的进程
	* @return 当前进程句柄
	*/
	SDHANDLE  SDGetCurrentProcess(); 

	/**
	* @brief
	* 获取当前的进程id
	* @return 当前的进程id
	*/
	SDPROCESSID  SDGetCurrentProcessId(); 

    /**
    * @brief
    * 等待进程终止
    * @param handle : 进程句柄
    * @return VOID
    */
    VOID  SDWaitProcess(SDHANDLE handle);

	/**
	* @brief
	* 设置进程属性
	* @param handle : 进程句柄
    * @param pAttr  : 进程属性
	* @return 设置结果
	*/
    BOOL  SDSetProcessAttr(SDHANDLE handle, const SProcessAttr &pAttr);

	/**
	* @brief
	* 获取进程属性
	* @param handle : 进程句柄
	* @param procAttr : 进程属性
	* @return 获取是否成功
	*/
	BOOL  SDGetProcessAttr(SDHANDLE handle, SProcessAttr & procAttr); 

    /**
    * @brief 进程操作类
    */
    class CSDProcess
    {
    public:

		/**
		* @brief 构造函数，初始化进程句柄
		*/
        CSDProcess();

		/**
		* @brief 析构函数，结束进程
		*/
        ~CSDProcess();

        /**
        * @brief
        * 创建进程
        * @param path : 该进程的执行文件
        * @param pArgs: 该进程的命令参数
        * @param pEnvs: 该进程的环境变量
        * @param pAttr: 该进程的属性
        * @return 如果成功返回TRUE, 失败返回FALSE
        */
        BOOL  Create(
            const CHAR *pProgName,
            const CHAR *pArgs = NULL,
            const CHAR *pEnvs = NULL,
            SProcessAttr *pAttr = NULL);

        /**
        * @brief
        * 终止进程
        * @return VOID
        */
        VOID  Terminate();

        /**
        * @brief
        * 等待进程终止或者执行结束
        * @return VOID
        */
        VOID  Wait();

		/**
		* @brief
		* 获取进程ID
		* @return SDHANDLE
		*/
		UINT32  GetProcessID(); 

        /**
        * @brief
        * 获取进程句柄
        * @return SDHANDLE
        */
        SDHANDLE  GetHandle();

        /**
        * @brief
        * 设置进程属性
        * @param pAttr : 待设置的进程属性
        * @return 成功返回TRUE，失败返回FALSE
        */
        BOOL  SetAttribute(const SProcessAttr &pAttr);

        /**
        * @brief
        * 获取进程属性
        * @param handle : 进程句柄
        * @param procAttr : 保存获取到的进程属性
        * @return TRUE:获取成功 FALSE:获取失败
        */
        BOOL  GetAttribute(SDHANDLE handle, SProcessAttr & procAttr);

    private:

		/**
		* @brief 拷贝构造函数
		* @param CSDProcess对象的引用
		*/
        CSDProcess (const CSDProcess&);

		/**
		* @brief 重载=操作符，实现CSDProcess对象的拷贝
		* @param CSDProcess对象的引用
		* @return CSDProcess对象的引用
		*/
        CSDProcess& operator = (const CSDProcess&);
        SDHANDLE m_handle;
    };

    /**
    * @brief 载入库文件
    * @param pszFileName : 库文件名
    * @param nMode       : linux系统下载入模式
    * @return 库模块的句柄
    */
    SDHANDLE SDLoadLibrary(const TCHAR* pszFileName,INT32 nMode = 0);

    /**
    * @brief 释放库模块句柄
    * @param handle : 库模块句柄
    * @return TRUE:释放成功 FALSE:释放失败
    */
    BOOL SDFreeLibrary(SDHANDLE handle);

    /** @} */
}

#endif


