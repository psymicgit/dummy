#ifndef _SGDPEXCEPTION_H_INCLUDED_
#define _SGDPEXCEPTION_H_INCLUDED_

#include <sdtype.h>
#include <sdtime.h>
#include <stdio.h>

#ifdef _WIN32
#pragma warning(disable:4996)
#include <WinNT.h>
#include <windows.h>
#include <atlstr.h>
#include <vector>
#include <assert.h>
#include "dbghelp.h"
#pragma comment( lib, "dbghelp.lib")
#pragma comment (lib, "version.lib")
#ifndef SYMOPT_LOAD_LINES
#define SYMOPT_LOAD_LINES	0x00000010
#endif  
#else
#include <sys/time.h>
#include <sys/resource.h>
typedef VOID* LPEXCEPTION_POINTERS;
#endif


namespace SGDP 
{
    extern inline UINT32 excep_filter( LPEXCEPTION_POINTERS lpEP,INT32 iLevel = 7);

#ifdef _WIN32

    using namespace std;

    extern inline VOID __stdcall  GetCallStack(CONTEXT& ctStx, CString& strStack, DWORD dwLevel = 50);
    extern inline VOID __stdcall  GetCallStack(CString& strStack, DWORD dwLevel = 50);

    extern inline VOID __stdcall  dump_callstack( CONTEXT *context ,INT32 iLevel);

    extern inline VOID SEHException_translate( UINT dwEC, LPEXCEPTION_POINTERS pep );


#if ( API_VERSION_NUMBER < 7 )
    typedef struct _IMAGEHLP_LINE
    {
        DWORD SizeOfStruct;           // set to sizeof(IMAGEHLP_LINE)
        DWORD Key;                    // internal
        DWORD LineNumber;             // line number in file
        PCHAR FileName;               // full filename
        DWORD Address;                // first instruction of line
    } IMAGEHLP_LINE, *PIMAGEHLP_LINE;
#endif  // API_VERSION_NUMBER < 7


    class SEHToExceptionHandler
    {
    public:
        static VOID  Install_TranslateFunc()
        {
            m_pOldFilter = ::_set_se_translator( SEHException_translate );
        }

        static VOID  UnInstall_TranslateFunc()
        {
            if ( m_pOldFilter )
            {
                ::_set_se_translator( m_pOldFilter );
            }

        }

        static _se_translator_function m_pOldFilter;
    };

    class CSEHexception
    {
    public:
        CSEHexception( LPEXCEPTION_POINTERS lexcept ):lpEP( lexcept )
        {
        }

        LPEXCEPTION_POINTERS GetSEHInfo()
        {
            return lpEP;
        }

        const CHAR* what();

    private:
        LPEXCEPTION_POINTERS lpEP;
    };

    LONG WINAPI SDExceptionFilter(_EXCEPTION_POINTERS *ExceptionInfo );


    struct SSDExceptionHandler
    {
        SSDExceptionHandler()
        {
            ::SetUnhandledExceptionFilter(SDExceptionFilter);

            SEHToExceptionHandler::Install_TranslateFunc();
        }

        ~SSDExceptionHandler()
        {
            SEHToExceptionHandler::UnInstall_TranslateFunc();
        }
    };


#else
    struct SSDExceptionHandler
    {
        SSDExceptionHandler()
        {
            struct rlimit res;
            res.rlim_cur = RLIM_INFINITY;
            res.rlim_max = RLIM_INFINITY;
            setrlimit(RLIMIT_CORE, &res);
        }

        ~SSDExceptionHandler()
        {
        }
    };

    class CSEHexception
    {
    public:
        LPEXCEPTION_POINTERS GetSEHInfo()
        {
            return NULL;
        }
    };

#endif 


    inline VOID MyGetCurTime(CHAR *lpszTimeBuf,INT32 nLen,CHAR nFlag)
    {
        if (NULL == lpszTimeBuf)
        {
            return;
        }
        
        CSDDateTime oNow;

        switch(nFlag)
        {
        case 'M':
            if(nLen < 9) break;
            sprintf(lpszTimeBuf,"%.2d:%.2d",oNow.GetHour(),oNow.GetMinute());
            break;
        case 'T':
            if(nLen < 9) break;
            sprintf(lpszTimeBuf,"%.2d:%.2d:%.2d",oNow.GetHour(),oNow.GetMinute(),oNow.GetSecond());
            break;
        case 'W':
            if(nLen < 11) break;
            sprintf(lpszTimeBuf,"%d-%.2d",oNow.GetYear(),oNow.GetMonth());
            break;
        case 'D':
            if(nLen < 11) break;
            sprintf(lpszTimeBuf,"%d-%.2d-%.2d",oNow.GetYear(),oNow.GetMonth(),oNow.GetDay());
            break;
        default:
            if(nLen < 20) break; 
            sprintf(lpszTimeBuf,"%d-%.2d-%.2d %.2d:%.2d:%.2d",
                oNow.GetYear(),oNow.GetMonth(),oNow.GetDay(),oNow.GetHour(),oNow.GetMinute(),oNow.GetSecond());
            break;
        }
    }

}; // end of namespace SGDP


#endif // _XCBEXCEPTION_H_INCLUDED_

