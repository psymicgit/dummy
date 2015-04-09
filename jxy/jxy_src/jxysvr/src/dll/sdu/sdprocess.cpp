#include "sdprocess.h"
#include "sdtime.h"
#include "sdthread.h"
#include "sdindexer.h"
#include "sdstring.h"
#if (defined(WIN32) || defined(WIN64))
#include <Windows.h>
#include <Tlhelp32.h>
#else
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include <errno.h>
#endif

namespace SGDP
{

#if (defined(WIN32) || defined(WIN64))
	template <> 
	struct DefaultValue <PROCESS_INFORMATION>
	{
		void operator() (PROCESS_INFORMATION & data)
		{
			data.hProcess = SDINVALID_HANDLE; 
			data.hThread  = SDINVALID_HANDLE; 
			data.dwProcessId = 0; 
			data.dwThreadId  = 0; 
		}
	};
	struct SProcessFinder
	{
		bool operator ()(const PROCESS_INFORMATION& left, const PROCESS_INFORMATION& right) const 
		{
			if (left.hProcess != SDINVALID_HANDLE && right.hProcess != SDINVALID_HANDLE)
			{
				return left.hProcess == right.hProcess; 
			}
			return (left.dwProcessId == right.dwProcessId); 
		}

	};

	typedef SDIndexer<PROCESS_INFORMATION, 1024*10, SProcessFinder> CSDProcessIndexer; 
#else 
	typedef SDIndexer<pid_t, 1024*10 > CSDProcessIndexer; 
#endif
	static CSDProcessIndexer  s_processIndexer; 


#if (defined(WIN32) || defined(WIN64))
	
#else 
	void catchSigCLD()
	{
		pid_t pid;
		int stat;
		while( (pid = waitpid(-1, &stat, WNOHANG)) > 0 ) {
			s_processIndexer.Free(pid);
		}
		signal(SIGCHLD, (void(*)(int)) catchSigCLD);
	}
#endif





    SDHANDLE  SDCreateProcess(const CHAR * progName, const CHAR * pCmdLine ,const CHAR * pWorkDir, SProcessAttr * pAttr )
    {
#if (defined(WIN32) || defined(WIN64))
        STARTUPINFOA m_sinfo;
        PROCESS_INFORMATION m_pinfo;
        void * pEnv = NULL;
        BOOL inherithandle = FALSE;
        UINT32 createFlags =0;
        if (pAttr != NULL)
        {
            pEnv = (LPVOID)pAttr->environment.c_str();
            inherithandle = pAttr->inherithandle;
            createFlags = pAttr->createFlags;
        }

        memset((void*)&m_pinfo, 0, sizeof(PROCESS_INFORMATION));
        memset((void*)&m_sinfo, 0, sizeof(STARTUPINFO));
        m_sinfo.cb = sizeof(STARTUPINFO);
        if( !CreateProcessA( progName,
                             (LPSTR)pCmdLine,
                             NULL,
                             NULL,
                             inherithandle,
                             createFlags,
                             (LPVOID)pEnv,
                             pWorkDir,
                             &m_sinfo,
                             &m_pinfo)
          )
        {
			char szData[1024] = {0};
			printf("command ls is not found, error code: %d (%s) \n", errno, strerror(errno)); 
            return SDINVALID_HANDLE;
        }
		return (SDHANDLE)s_processIndexer.Alloc(m_pinfo); 
#else

		signal(SIGCHLD, (void(*)(int)) catchSigCLD);

        pid_t m_pid;
        if ( (m_pid = fork()) < 0 )
        {
			return s_processIndexer.Alloc(m_pid); 
        }
        else if(m_pid == 0)
        {
			tstring str = pCmdLine;
			std::vector<tstring> vCommandStr = _SDTSplitStrings(str,_SDT(' '));
			char *	argv[10];
			char		argv_temp[10][256];
			memset(&argv_temp, 0x00, sizeof(argv_temp));
			INT32 i = 0;
			for(i = 0 ; i < (INT32)vCommandStr.size(); i ++ )
			{
				sprintf((char*)(&argv_temp[i]), "%s", vCommandStr[i].c_str());
				argv[i] = argv_temp[i];
			}
			argv[i] = NULL;

			if(chdir(pWorkDir) < 0)
			{
				printf("command ls is not found, error code: %d (%s) \n", errno, strerror(errno)); 
				_exit(-1);
			}

			char pathname[SD_MAX_PATH];
			sprintf(pathname, "%s/%s", pWorkDir, progName);

			execv(pathname, argv);

			printf("command ls is not found, error code: %d (%s) \n", errno, strerror(errno)); 
			_exit(-1);
        }
		return s_processIndexer.Alloc(m_pid); 
#endif
    }

	SDPROCESSID  SDGetProcessId(SDHANDLE handle)
	{
#if (defined(WIN32) || defined(WIN64))
		PROCESS_INFORMATION & pInfo = s_processIndexer.Get((UINT32)handle); 
		return GetProcessId(pInfo.hProcess); 
#else 
		return  s_processIndexer.Get(handle); 
#endif //
	}

#if (defined(WIN32) || defined(WIN64))
	static SDHANDLE FindProcessHandle(SDPROCESSID pid )
	{
		PROCESS_INFORMATION toFind; 
		toFind.dwProcessId = pid; 
		toFind.hProcess = SDINVALID_HANDLE; 
		return (SDHANDLE)s_processIndexer.Find(toFind); 
	}
	
#endif 
 
	SDHANDLE  SDOpenProcess(SDPROCESSID pid)
	{
		SDHANDLE handle; 
#if (defined(WIN32) || defined(WIN64))
		PROCESS_INFORMATION procInfo = {0}; 
		procInfo.hProcess  = SDINVALID_HANDLE; 
		procInfo.dwProcessId = pid; //根据进程ID查找对应的Handle 及其他信息
		handle = FindProcessHandle(pid); 
		if (handle == SDINVALID_HANDLE)
		{
			// procInfo.hProcess =  OpenProcess(PROCESS_ALL_ACCESS , FALSE, pid); 
            procInfo.hProcess =  OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ , FALSE, pid); 
			//procInfo.dwProcessId = GetProcessId(procInfo.hProcess); 
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,pid); 
			if (hSnap != SDINVALID_HANDLE)
			{
				THREADENTRY32 thrdEnt   = {0}; 
				thrdEnt.dwSize = sizeof(thrdEnt); 
				BOOL ret = Thread32First(hSnap,&thrdEnt);
				if (ret )
				{
					if (thrdEnt.th32OwnerProcessID == pid)
					{
						procInfo.dwThreadId = thrdEnt.th32ThreadID; 
						procInfo.hThread = OpenThread(THREAD_ALL_ACCESS,FALSE,procInfo.dwThreadId); 
					}
					else 
					{
						while(Thread32Next(hSnap,&thrdEnt))
						{
							if (thrdEnt.th32OwnerProcessID == pid )
							{
								procInfo.dwThreadId = thrdEnt.th32ThreadID; 
								procInfo.hThread = OpenThread(THREAD_ALL_ACCESS,FALSE,procInfo.dwThreadId); 
								break; 
							}
						}
					}
					
				}
				CloseHandle(hSnap); 
			}
			return (SDHANDLE)s_processIndexer.Alloc(procInfo); 
		}
#else 
		handle = s_processIndexer.Find(getpid()); 
		if (handle == SDINVALID_HANDLE)
		{
			pid_t curPid = getpid(); 
			return (SDHANDLE)s_processIndexer.Alloc(curPid); 
		}
#endif //
		return handle; 
	}

	SDHANDLE   SDGetCurrentProcess()
	{
		SDHANDLE handle; 
#if (defined(WIN32) || defined(WIN64))
		PROCESS_INFORMATION procInfo; 
		procInfo.hProcess = GetCurrentProcess(); 
		procInfo.dwProcessId = GetProcessId(procInfo.hProcess); 
		procInfo.hThread  = GetCurrentThread(); 
		procInfo.dwThreadId = SDGetThreadId(); 
		handle = FindProcessHandle(procInfo.dwProcessId);
		if (handle == SDINVALID_HANDLE)
		{
			handle = (SDHANDLE)s_processIndexer.Alloc(procInfo); 
		}
#else 
		pid_t curPid = getpid(); 
		handle = s_processIndexer.Find(curPid); 
		if (handle == SDINVALID_HANDLE)
		{
			handle = s_processIndexer.Alloc(curPid); 
		}
#endif // 
		return handle; 
	}

	SDPROCESSID  SDGetCurrentProcessId()
	{
#if (defined(WIN32) || defined(WIN64))
		return GetCurrentProcessId(); 
#else 
		return getpid(); 
#endif // 
	}

    void  SDCloseProcess(SDHANDLE handle)
    {
#if (defined(WIN32) || defined(WIN64))
		PROCESS_INFORMATION & pInfo = s_processIndexer.Free((UINT32)handle); 
        CloseHandle( pInfo.hThread );
        CloseHandle( pInfo.hProcess );
#endif
    }

    void  SDTerminateProcess(SDHANDLE handle,INT32 err )
    {
#if (defined(WIN32) || defined(WIN64))
		PROCESS_INFORMATION & pInfo = s_processIndexer.Get((UINT32)handle); 
        TerminateProcess(pInfo.hProcess,err);
#else
		pid_t pid = s_processIndexer.Get(handle); 
        kill(pid,SIGKILL);
#endif
    }

    void  SDWaitProcess(SDHANDLE handle)
    {
#if (defined(WIN32) || defined(WIN64))
		PROCESS_INFORMATION & pInfo = s_processIndexer.Get((UINT32)handle); 
        WaitForSingleObject(pInfo.hProcess, INFINITE);
#else
        int status;
		pid_t pid = s_processIndexer.Get(handle); 
        waitpid(pid, &status, WUNTRACED);
#endif
    }


    BOOL  SDSetProcessAttr(SDHANDLE handle, const SProcessAttr &pAttr)
    {
#if (defined(WIN32) || defined(WIN64))
		if (pAttr.secInfo.bSet)
		{
			PROCESS_INFORMATION & pInfo = s_processIndexer.Get((UINT32)handle); 
			UINT32 ret = SetSecurityInfo(pInfo.hProcess,
				pAttr.secInfo.objectType,
				pAttr.secInfo.securityInfo,
				pAttr.secInfo.psidOwner,
				pAttr.secInfo.psidGroup,
				pAttr.secInfo.pDacl,
				pAttr.secInfo.pSacl
				);
			if (ret != ERROR_SUCCESS)
			{
				return FALSE; 
			}
		}
#else 
#endif // 
        return FALSE;
    }

	BOOL  SDGetProcessAttr(SDHANDLE handle, SProcessAttr & procAttr)
	{
#if (defined(WIN32) || defined(WIN64))
		if (procAttr.secInfo.bSet)
		{
			PROCESS_INFORMATION & pInfo = s_processIndexer.Get((UINT32)handle); 
			UINT32 ret = GetSecurityInfo(pInfo.hProcess,
				procAttr.secInfo.objectType, 
				procAttr.secInfo.securityInfo,
				&procAttr.secInfo.psidOwner,
				&procAttr.secInfo.psidGroup,
				&procAttr.secInfo.pDacl,
				&procAttr.secInfo.pSacl,
				&procAttr.secInfo.pSecurityDescriptor
				);
			if (ret != ERROR_SUCCESS)
			{
				return FALSE; 
			}
		}
#endif 

		return TRUE;
	}

    CSDProcess::CSDProcess()
    {
        m_handle = SDINVALID_HANDLE;
    }

    CSDProcess::~CSDProcess()
    {
        if (m_handle != SDINVALID_HANDLE)
        {
            this->Terminate();
        }
    }

    BOOL  CSDProcess::Create(const CHAR * progName, const CHAR * pArgs, const CHAR * pEnvs, SProcessAttr * pAttr)
    {
        m_handle = SDCreateProcess(progName, pArgs, pEnvs, pAttr);
        return SDINVALID_HANDLE != m_handle;
    }

    void  CSDProcess::Terminate()
    {
        SDTerminateProcess(m_handle);
        m_handle = SDINVALID_HANDLE;
    }

    void  CSDProcess::Wait()
    {
        SDWaitProcess(m_handle);
    }

	UINT32  CSDProcess::GetProcessID()
	{
		return SDGetProcessId(m_handle); 
	}

    SDHANDLE  CSDProcess::GetHandle()
    {
        return m_handle;
    }

    BOOL  CSDProcess::SetAttribute(const SProcessAttr &pAttr)
    {
        return SDSetProcessAttr(m_handle,pAttr);
    }

    BOOL  CSDProcess::GetAttribute(SDHANDLE handle, SProcessAttr & procAttr)
    {
        return SDGetProcessAttr(m_handle, procAttr);
    }
    BOOL SDFreeLibrary(SDHANDLE handle)
    {
#if (defined(WIN32) || defined(WIN64))
        return FreeLibrary((HMODULE)handle);
#else
        if(0 == dlclose((void*)handle))//0:成功 1:失败
            return TRUE;
        else
            return FALSE;
#endif
    }

    SDHANDLE SDLoadLibrary(const TCHAR* pszFileName,INT32 nMode )
    {
#if (defined(WIN32) || defined(WIN64))
        return LoadLibrary(pszFileName);
#else
        return (SDHANDLE)dlopen(_SDTT2A(pszFileName),nMode);
#endif
    }
}

