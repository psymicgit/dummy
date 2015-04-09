#include "sdexception.h"
#include <sdstring.h>

namespace SGDP
{
	/*
    SSDExceptionHandler stExceptionHandler;

    VOID catch_dumpinfo( LPEXCEPTION_POINTERS lpEP );

    extern inline UINT32 excep_filter( LPEXCEPTION_POINTERS lpEP,INT32 iLevel)
    {
#ifdef __linux__
        return 0;
#else
        static INT32 count = 0;
        if ( count > 50 )
            return EXCEPTION_EXECUTE_HANDLER;

        count++;

        // init dbghelp.dll
        if( !SymInitialize( GetCurrentProcess(), NULL, TRUE ) )
        {
            printf( "Init dbghelp faile .\n" );
        }

        catch_dumpinfo( lpEP );

        dump_callstack( lpEP->ContextRecord ,iLevel);

        if( !SymCleanup( GetCurrentProcess() ) )
        {
            printf( "Cleanup dbghelp faile.\n" );
        }
        return EXCEPTION_EXECUTE_HANDLER;
#endif
    }


#ifdef _WIN32

#include <tchar.h>


    static BOOL g_bSymIsInit = FALSE;

    _se_translator_function SEHToExceptionHandler::m_pOldFilter = NULL;

#ifdef _WIN64
    static DWORD64 __stdcall GetModBase ( HANDLE hProcess , DWORD64 dwAddr )
#else
    static DWORD __stdcall GetModBase ( HANDLE hProcess , DWORD dwAddr )
#endif
    {
        // Check in the symbol engine first.
        IMAGEHLP_MODULE stIHM ;

        // This is what the MFC stack trace routines forgot to do so their
        //  code will not get the info out of the symbol engine.
        stIHM.SizeOfStruct = sizeof ( IMAGEHLP_MODULE ) ;

        if (SymGetModuleInfo (hProcess, dwAddr , &stIHM ) )
        {
            return ( stIHM.BaseOfImage ) ;
        }
        else
        {
            // Let's go fishing.
            MEMORY_BASIC_INFORMATION stMBI ;

            if ( 0 != VirtualQueryEx(hProcess, (LPCVOID)dwAddr, &stMBI, sizeof(stMBI)))
            {
                // Try and load it.
                DWORD dwNameLen = 0 ;
                TCHAR szFile[ MAX_PATH ] ;
                memset(szFile, 0, sizeof(szFile));
                HANDLE hFile = NULL ;

                dwNameLen = GetModuleFileName((HINSTANCE)stMBI.AllocationBase, szFile, MAX_PATH);
                if ( 0 != dwNameLen )
                {
                    hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0) ;
                    SymLoadModule(hProcess, hFile, (PSTR)(dwNameLen ? szFile : NULL),NULL,(DWORD)stMBI.AllocationBase,0);
                    CloseHandle(hFile);
                }
                return ( (DWORD)stMBI.AllocationBase ) ;
            }
        }

        return ( 0 ) ;
    }

#define STACK_LEN 4000
#ifdef _WIN64
    static VOID __stdcall ConvertAddress (HANDLE hProcess, CString& strAppend, DWORD64 dwAddr, DWORD64 dwParm1, DWORD64 dwParm2,DWORD64 dwParm3,DWORD64 dwParm4)
#else
    static VOID __stdcall ConvertAddress (HANDLE hProcess, CString& strAppend, DWORD dwAddr, DWORD dwParm1, DWORD dwParm2,DWORD dwParm3,DWORD dwParm4)
#endif
    {
        TCHAR szTemp [ STACK_LEN + sizeof ( IMAGEHLP_SYMBOL ) ] ;

        PIMAGEHLP_SYMBOL pIHS = (PIMAGEHLP_SYMBOL)&szTemp ;

        IMAGEHLP_MODULEW stIHM ;

        ZeroMemory ( pIHS , STACK_LEN + sizeof ( IMAGEHLP_SYMBOL ) ) ;
        ZeroMemory ( &stIHM , sizeof ( IMAGEHLP_MODULE ) ) ;

        pIHS->SizeOfStruct = sizeof ( IMAGEHLP_SYMBOL ) ;
        pIHS->Address = dwAddr ;
        pIHS->MaxNameLength = STACK_LEN ;

        stIHM.SizeOfStruct = sizeof ( IMAGEHLP_MODULE ) ;

        // Always stick the address in first.
        CString strTemp;

        // Get the module name.
        if (SymGetModuleInfoW(hProcess, dwAddr , &stIHM ) )
        {
            // Strip off the path.
            WCHAR chDelimeter = '\\';
#ifdef UNICODE
            LPTSTR szName = _tcsrchr(stIHM.ImageName, chDelimeter) ;
#else
            LPTSTR szName = _tcsrchr((CHAR*)(SDW2A(stIHM.ImageName).c_str()) , chDelimeter) ; 
#endif
            if ( NULL != szName )
            {
                szName++ ;
            }
            else
            {
                szName = (LPTSTR)stIHM.ImageName ;
            }
            strTemp.Format(_T("0x%08X[%X] %s: ") , stIHM.BaseOfImage, dwAddr - stIHM.BaseOfImage, szName);
            strAppend += strTemp;
        }
        else
        {
            strTemp.Format(_T ( "0x%08X " ) , dwAddr);
            strAppend += strTemp;

            strAppend += _T ( "<unknown module>: " );
        }

        // Get the function.
#ifdef _WIN64
        PDWORD64 dwDisp = 0;
         if (SymGetSymFromAddr (hProcess, dwAddr , dwDisp , pIHS ) )
#else
        DWORD dwDisp ;
         if (SymGetSymFromAddr (hProcess, dwAddr , &dwDisp , pIHS ) )
#endif       
        {
            strAppend += pIHS->Name;
            strTemp.Format(_T("(%d,%d,%d,%d)"), dwParm1, dwParm2, dwParm3, dwParm4);
            strAppend += strTemp;

#ifdef _DEBUG
#if (_WIN32_WINNT >= 0x0500)
            {
                // If I got a symbol, give the source and line a whirl.
                IMAGEHLP_LINE stIHL ;
                ZeroMemory ( &stIHL , sizeof ( IMAGEHLP_LINE)) ;
                stIHL.SizeOfStruct = sizeof ( IMAGEHLP_LINE ) ;

#ifdef _WIN64
                PDWORD dwDisp2 = 0;
                if (SymGetLineFromAddr(hProcess, dwAddr,dwDisp2,&stIHL))
#else
                if (SymGetLineFromAddr(hProcess, dwAddr,&dwDisp,&stIHL))
#endif
                {
                    // Put this on the next line and indented a bit.
                    strTemp.Format(_T("\t\t%s, Line %d"), stIHL.FileName, stIHL.LineNumber);
                    strAppend += strTemp;
                }
            }
#endif //_WIN32_WINNT >= 0x0500
#endif //_DEBUG
        }
        else
        {
            strTemp.Format(_T("(%d,%d,%d,%d)"), dwParm1, dwParm2, dwParm3, dwParm4);
            strAppend += strTemp;
        }

        strAppend += _T("\n");
    }

    extern inline VOID __stdcall GetCallStack(CString& strStack, DWORD dwLevel)
    {
        if (dwLevel < 2)
            dwLevel = 2;

        CONTEXT ctx;
        memset(&ctx, 0, sizeof(ctx));
        ctx.ContextFlags = CONTEXT_FULL ;
        if (GetThreadContext(GetCurrentThread(), &ctx))
            GetCallStack(ctx, strStack, dwLevel);
    }
    extern inline VOID __stdcall GetCallStack(CONTEXT& stCtx,CString& strStack, DWORD dwLevel)
    {
        if (dwLevel < 2)
            dwLevel = 2;

        HANDLE hProcess = GetCurrentProcess();

        // If the symbol engine is not initialized, do it now.
        if ( FALSE == g_bSymIsInit )
        {
            SymSetOptions(SYMOPT_UNDNAME |SYMOPT_LOAD_LINES) ;

            if (SymInitialize(hProcess, NULL, FALSE))
                g_bSymIsInit = TRUE ;
        }

        //	CDWordArray vAddrs ;
#ifdef _WIN64   
        vector<DWORD64> vAddrs;
#else
    vector<DWORD> vAddrs;
#endif
        STACKFRAME stFrame ;
        DWORD      dwMachine ;

        ZeroMemory ( &stFrame , sizeof ( STACKFRAME ) ) ;
        dwMachine                = IMAGE_FILE_MACHINE_I386 ;
#ifdef _WIN64        
       stFrame.AddrPC.Offset    = stCtx.Rip   ;
        stFrame.AddrFrame.Offset = stCtx.Rbp    ;
#else
        stFrame.AddrPC.Offset    = stCtx.Eip   ;
        stFrame.AddrFrame.Offset = stCtx.Ebp    ;
#endif

        stFrame.AddrPC.Mode = AddrModeFlat ;
        stFrame.AddrFrame.Mode   = AddrModeFlat ;

        for ( DWORD i = 0 ; i <= dwLevel ; i++ )
        {
            if (StackWalk(dwMachine,hProcess,hProcess,&stFrame,&stCtx,NULL,SymFunctionTableAccess,GetModBase, NULL))
            {
                // Also check that the address is not zero.  Sometimes StackWalk returns TRUE with a frame of zero.
                if (0 != stFrame.AddrPC.Offset)
                {
                    vAddrs.push_back(stFrame.AddrPC.Offset);
                    vAddrs.push_back(stFrame.Params[0]);
                    vAddrs.push_back(stFrame.Params[1]);
                    vAddrs.push_back(stFrame.Params[2]);
                    vAddrs.push_back(stFrame.Params[3]);
                }
            }
        }

        UINT32 dwSize = vAddrs.size();
        assert(dwSize % 5 == 0);
        for (UINT32 n = 0; n < dwSize; n += 5)
        {
            assert(n + 4 < dwSize);
            CString strTemp;
            ConvertAddress(hProcess, strTemp, vAddrs[n], vAddrs[n+1],vAddrs[n+2],vAddrs[n+3], vAddrs[n+4]);
            strStack += strTemp;
        }
    }


    VOID catch_dumpinfo( LPEXCEPTION_POINTERS lpEP )
    {
		TCHAR szDir[MAX_PATH] = {0};
		_SDTsnprintf(szDir, MAX_PATH, _SDT("%s%s"), (TCHAR*)(SDGetModulePath()),"dump"); 
        CreateDirectory(_T(szDir), NULL);

        SYSTEMTIME tm;
        GetLocalTime(&tm);
        TCHAR dumpName[128] = {0};
        wsprintf(dumpName, _T("%s/dump_%04d_%02d_%02d_%02d_%02d_%02d_%03d.dmp")
			,szDir
            ,tm.wYear
            ,tm.wMonth
            ,tm.wDay
            ,tm.wHour
            ,tm.wMinute
            ,tm.wSecond
            ,tm.wMilliseconds);

        HANDLE lhDumpFile = CreateFile(dumpName, GENERIC_WRITE,  FILE_SHARE_READ, NULL, 
            CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL );
        if( lhDumpFile != INVALID_HANDLE_VALUE )
        {
            MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
            loExceptionInfo.ExceptionPointers = lpEP;
            loExceptionInfo.ThreadId = GetCurrentThreadId();
            loExceptionInfo.ClientPointers = TRUE;

            if( MiniDumpWriteDump( 
                GetCurrentProcess(), 
                GetCurrentProcessId(), 
                lhDumpFile,
				MiniDumpNormal,
                &loExceptionInfo,
                NULL,
                NULL ) != TRUE )
            {       
                HRESULT hreult = (HRESULT)GetLastError();
                VOID* pMsgBuf;
                ::FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    hreult,
                    MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
                    (LPTSTR)&pMsgBuf,
                    0,
                    NULL
                    );            
                LocalFree( pMsgBuf );	
            }
            CloseHandle( lhDumpFile );
        }
        else
        {
            HRESULT hreult = (HRESULT)GetLastError();
            VOID* pMsgBuf;
            ::FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                hreult,
                MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
                (LPTSTR)&pMsgBuf,
                0,
                NULL
                );        
            LocalFree( pMsgBuf );	
        }

    }

    //////////////////////////////////////////////////////////////////////////

    extern inline VOID SEHException_translate( UINT dwEC, LPEXCEPTION_POINTERS pep )
    {
        (dwEC);
        throw CSEHexception( pep );
    }

    const CHAR* CSEHexception::what()
    {
        if ( !lpEP )
        {
            static CHAR SEPErr[] = {"SEH Exception"};
            return SEPErr;
        }
        else
        {
            static string strTemp;
            strTemp = "";
            strTemp = string("SEH Exception Err:");
            EXCEPTION_RECORD * pERec = lpEP->ExceptionRecord;
            switch(pERec->ExceptionCode)
            {
            case EXCEPTION_ACCESS_VIOLATION:
                strTemp += "EXCEPTION_ACCESS_VIOLATION";
                break;
            case EXCEPTION_DATATYPE_MISALIGNMENT:
                strTemp += "EXCEPTION_DATATYPE_MISALIGNMENT";
                break;
            case EXCEPTION_BREAKPOINT:
                strTemp += "EXCEPTION_BREAKPOINT";
                break;
            case EXCEPTION_SINGLE_STEP:
                strTemp += "EXCEPTION_SINGLE_STEP";
                break;
            case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
                strTemp += "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
                break;
            case EXCEPTION_FLT_DENORMAL_OPERAND:
                strTemp += "EXCEPTION_FLT_DENORMAL_OPERAND";
                break;
            case EXCEPTION_FLT_DIVIDE_BY_ZERO:
                strTemp += "EXCEPTION_FLT_DIVIDE_BY_ZERO";
                break;
            case EXCEPTION_FLT_INEXACT_RESULT:
                strTemp += "EXCEPTION_FLT_INEXACT_RESULT";
                break;
            case EXCEPTION_FLT_INVALID_OPERATION:
                strTemp += "EXCEPTION_FLT_INVALID_OPERATION";
                break;
            case EXCEPTION_FLT_OVERFLOW:
                strTemp += "EXCEPTION_FLT_OVERFLOW";
                break;
            case EXCEPTION_FLT_STACK_CHECK:
                strTemp += "EXCEPTION_FLT_STACK_CHECK";
                break;
            case EXCEPTION_FLT_UNDERFLOW:
                strTemp += "EXCEPTION_FLT_UNDERFLOW";
                break;
            case EXCEPTION_INT_DIVIDE_BY_ZERO:
                strTemp += "EXCEPTION_INT_DIVIDE_BY_ZERO";
                break;
            case EXCEPTION_INT_OVERFLOW:
                strTemp += "EXCEPTION_INT_OVERFLOW";
                break;
            case EXCEPTION_PRIV_INSTRUCTION:
                strTemp += "EXCEPTION_PRIV_INSTRUCTION";
                break;
            case EXCEPTION_IN_PAGE_ERROR:
                strTemp += "EXCEPTION_IN_PAGE_ERROR";
                break;
            case EXCEPTION_ILLEGAL_INSTRUCTION:
                strTemp += "EXCEPTION_ILLEGAL_INSTRUCTION";
                break;
            case EXCEPTION_NONCONTINUABLE_EXCEPTION:
                strTemp += "EXCEPTION_NONCONTINUABLE_EXCEPTION";
                break;
            case EXCEPTION_STACK_OVERFLOW:
                strTemp += "EXCEPTION_STACK_OVERFLOW";
                break;
            case EXCEPTION_INVALID_DISPOSITION:
                strTemp += "EXCEPTION_INVALID_DISPOSITION";
                break;
            case EXCEPTION_GUARD_PAGE:
                strTemp += "EXCEPTION_GUARD_PAGE";
                break;
            case EXCEPTION_INVALID_HANDLE:
                strTemp += "EXCEPTION_INVALID_HANDLE";
                break;
            }
            return strTemp.c_str();
        }
    }


    LONG WINAPI SDExceptionFilter(_EXCEPTION_POINTERS *ExceptionInfo )
    {    
		::MessageBox( NULL, "test", "test", NULL);
        CString strTemp;

        EXCEPTION_RECORD * pERec = ExceptionInfo->ExceptionRecord;
        switch(pERec->ExceptionCode)
        {
        case EXCEPTION_ACCESS_VIOLATION:
            strTemp += "EXCEPTION_ACCESS_VIOLATION";
            break;
        case EXCEPTION_DATATYPE_MISALIGNMENT:
            strTemp += "EXCEPTION_DATATYPE_MISALIGNMENT";
            break;
        case EXCEPTION_BREAKPOINT:
            strTemp += "EXCEPTION_BREAKPOINT";
            break;
        case EXCEPTION_SINGLE_STEP:
            strTemp += "EXCEPTION_SINGLE_STEP";
            break;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            strTemp += "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
            break;
        case EXCEPTION_FLT_DENORMAL_OPERAND:
            strTemp += "EXCEPTION_FLT_DENORMAL_OPERAND";
            break;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            strTemp += "EXCEPTION_FLT_DIVIDE_BY_ZERO";
            break;
        case EXCEPTION_FLT_INEXACT_RESULT:
            strTemp += "EXCEPTION_FLT_INEXACT_RESULT";
            break;
        case EXCEPTION_FLT_INVALID_OPERATION:
            strTemp += "EXCEPTION_FLT_INVALID_OPERATION";
            break;
        case EXCEPTION_FLT_OVERFLOW:
            strTemp += "EXCEPTION_FLT_OVERFLOW";
            break;
        case EXCEPTION_FLT_STACK_CHECK:
            strTemp += "EXCEPTION_FLT_STACK_CHECK";
            break;
        case EXCEPTION_FLT_UNDERFLOW:
            strTemp += "EXCEPTION_FLT_UNDERFLOW";
            break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            strTemp += "EXCEPTION_INT_DIVIDE_BY_ZERO";
            break;
        case EXCEPTION_INT_OVERFLOW:
            strTemp += "EXCEPTION_INT_OVERFLOW";
            break;
        case EXCEPTION_PRIV_INSTRUCTION:
            strTemp += "EXCEPTION_PRIV_INSTRUCTION";
            break;
        case EXCEPTION_IN_PAGE_ERROR:
            strTemp += "EXCEPTION_IN_PAGE_ERROR";
            break;
        case EXCEPTION_ILLEGAL_INSTRUCTION:
            strTemp += "EXCEPTION_ILLEGAL_INSTRUCTION";
            break;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            strTemp += "EXCEPTION_NONCONTINUABLE_EXCEPTION";
            break;
        case EXCEPTION_STACK_OVERFLOW:
            strTemp += "EXCEPTION_STACK_OVERFLOW";
            break;
        case EXCEPTION_INVALID_DISPOSITION:
            strTemp += "EXCEPTION_INVALID_DISPOSITION";
            break;
        case EXCEPTION_GUARD_PAGE:
            strTemp += "EXCEPTION_GUARD_PAGE";
            break;
        case EXCEPTION_INVALID_HANDLE:
            strTemp += "EXCEPTION_INVALID_HANDLE";
            break;
        }

        //出错类型:\n%s\n出错地址:\n 0x%08X\n
        CString m_strError;

        m_strError.Format(_T("-----------------------------------\n出错类型： %s\n出错地址： 0x%08X\n"),strTemp,pERec->ExceptionAddress);
        m_strError = m_strError;

        if(pERec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION &&
            pERec->NumberParameters >1 &&
            pERec->ExceptionInformation )
        {
            CString str;
            if(pERec->ExceptionInformation[0]==0)
                str = "读取";
            else
                str = "写入";
            //出错原因:\n对 0x%08X 地址进行 %s 操作
            strTemp.Format(_T("出错原因： 对 0x%08X 地址进行%s操作"),pERec->ExceptionInformation[1],str);
            m_strError += strTemp;
        }	

        m_strError += "\n\n出错过程：\n";
        strTemp.Empty();
        GetCallStack(*(ExceptionInfo->ContextRecord), strTemp);
        m_strError += strTemp;   

        SYSTEMTIME time;
        ZeroMemory(&time, sizeof(SYSTEMTIME));
        GetLocalTime(&time);

        CHAR lpszLog[256]  = {0};
        sprintf(lpszLog, "\n%.4d-%.2d-%.2d %.2d:%.2d:%.2d\n", 
            time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);        

        FILE * fp = fopen("UnHandledException.log", "a+"); 
        if(fp) 
        { 
            fputs(lpszLog, fp);
            fputs((CHAR*)m_strError.GetBuffer(0), fp);
            fclose(fp); 
        }     


        return EXCEPTION_EXECUTE_HANDLER;
    }


    extern inline VOID __stdcall  dump_callstack( CONTEXT *context ,INT32 iLevel)
    {
        DWORD dwTickCount;
        dwTickCount = GetTickCount();
        STACKFRAME sf;
        string strErro;
        CHAR buf[2048] = {0};
        memset( &sf, 0, sizeof( STACKFRAME ) );

#ifdef _WIN64
        sf.AddrPC.Offset = context->Rip;
        sf.AddrStack.Offset = context->Rsp;
        sf.AddrFrame.Offset = context->Rsp;
#else
        sf.AddrPC.Offset = context->Eip;
        sf.AddrStack.Offset = context->Esp;
        sf.AddrFrame.Offset = context->Ebp;
#endif

        sf.AddrPC.Mode = AddrModeFlat;
        sf.AddrStack.Mode = AddrModeFlat;
        sf.AddrFrame.Mode = AddrModeFlat;

        DWORD machineType = IMAGE_FILE_MACHINE_I386;

        HANDLE hProcess = GetCurrentProcess();
        HANDLE hThread = GetCurrentThread();


        SYSTEMTIME time;
        ZeroMemory(&time,sizeof(SYSTEMTIME));
        GetLocalTime(&time);
        CHAR cCurDate[11] = {0};
        sprintf(cCurDate,"%d-%.2d-%.2d",time.wYear,time.wMonth,time.wDay);
        CHAR cCurTime[11] = {0};
        sprintf(cCurTime,"%.2d:%.2d:%.2d",time.wHour,time.wMinute,time.wSecond);
        sprintf(buf, "[%s %s]异常开始********************\n", cCurDate, cCurTime);
        strErro += buf;
        for( INT32 i = 0; i < iLevel; i++)
        {
            if( !StackWalk(machineType, hProcess, hThread, &sf, context, 0, SymFunctionTableAccess, SymGetModuleBase, 0 ) )
            {
                break;
            }

            if( sf.AddrFrame.Offset == 0 )
            {
                break;
            }
            BYTE symbolBuffer[ sizeof( SYMBOL_INFO ) + 1024 ];
            PSYMBOL_INFO pSymbol = ( PSYMBOL_INFO ) symbolBuffer;

            pSymbol->SizeOfStruct = sizeof( symbolBuffer );
            pSymbol->MaxNameLen = 1024;

            //DWORD64 symDisplacement = 0;
            CHAR buf2[1024] = {0};
            if( SymFromAddr( hProcess, sf.AddrPC.Offset, 0, pSymbol) )
            {
                sprintf(buf2,"%s", pSymbol->Name);            
            }

            IMAGEHLP_LINE lineInfo = { sizeof(IMAGEHLP_LINE) };
            DWORD dwLineDisplacement;

            if( SymGetLineFromAddr( hProcess, sf.AddrPC.Offset,&dwLineDisplacement, &lineInfo ))
            {
                sprintf(buf,"[%s:%d]%s\n",lineInfo.FileName, lineInfo.LineNumber, buf2);
                strErro += buf;
            }
        }
        sprintf(buf,"记录用时：%d 毫秒 \n",(GetTickCount() - dwTickCount));
        strErro += buf;
        sprintf(buf,"异常结束******************************\n\n");
        strErro += buf;
		TCHAR szDir[MAX_PATH] = {0};
		_SDTsnprintf(szDir, MAX_PATH, _SDT("%s%s"), (TCHAR*)(SDGetModulePath()),"dump"); 
		CreateDirectory(_T(szDir), NULL);
		TCHAR szFile[MAX_PATH] = {0};
		_SDTsnprintf(szFile, MAX_PATH, _SDT("%s%s"), szDir,"exception.log"); 
        FILE * fp = fopen(szFile, "a+");
        if(fp)
        {
            fputs(strErro.c_str(), fp);
            fputs("\n", fp); 
            fclose(fp); 
        } 
    } 
	
#endif
	*/

}// end of namespace SGDP



