#ifndef SDDEBUG_H
#define SDDEBUG_H
#include <cassert>
#include "sdtime.h"
#include "sdfile.h"
#ifdef _SDEXCEPTION
#include <sdexception.h>
#endif

#define SDDEBUG             DEBUG
#define SDASSERT            assert
#define SDNew               new
#define SDDelete            delete

#ifdef DEBUG
#define ODS(s)      OutputDebugString(s)
#else
#define ODS(s)
#endif


//编译时显示代码中的TODO列表和ATTENTION列表
//#pragma TODO("your description")
//#pragma ATTENTION("your description")
#define TOSTR(x)        #x
#define TOSTR2(x)       TOSTR(x)
#if (defined(WIN32) || defined(WIN64))
#define TODO(x)         message(__FILE__ "(" TOSTR2(__LINE__) "): TODO " x)
#define ATTENTION(x)    message(__FILE__ "(" TOSTR2(__LINE__) "): ATTENTION " x)
#endif

/*
* @brief 异常捕获宏的定义
*/

/*

#ifdef _SDEXCEPTION
#define   SDTRY_BEGIN	\
    try{
#define   SDTRY_END \
    } \
    catch(CSEHexception& e)\
    {\
        CHAR line[10];	\
        CHAR szPath[256]={0};	\
        sprintf(szPath,"%ssdexception.log",_SDTT2Local(SDGetModulePath()));\
        FILE * fp = fopen(szPath, "a+"); \
        if(fp) \
        { \
            CHAR TempLine[128] = {0}; \
            CHAR cCurDate[11] = {0};  \
            MyGetCurTime(cCurDate,11,'D'); \
            CHAR cCurTime[11] = {0}; \
            MyGetCurTime(cCurTime, 11, 'T'); \
            sprintf(TempLine, "[%s %s]", cCurDate, cCurTime); \
            fputs(TempLine, fp); fputs(__FILE__, fp); \
            sprintf(line, ":%d:", __LINE__); \
            fputs(line, fp); \
            fputs(e.what(), fp); \
            fputs("\n", fp); \
            fclose(fp);  \
        } \
        excep_filter( e.GetSEHInfo() );\
    }\
    catch(...) \
    { \
      CHAR szPath[256]={0};	\
      sprintf(szPath,"%ssdexception.log",_SDTT2Local(SDGetModulePath()));\
      FILE * fp = fopen(szPath, "a+"); \
      if(fp) \
      { \
          CHAR szTempLine[128] = {0}; \
          CSDDateTime ti = SDNow(); \
          sprintf(szTempLine, "[%s]",("")); \
          fputs(szTempLine, fp); \
          fputs(__FILE__, fp); \
          sprintf(szPath, "(%d): %s", __LINE__, __FUNCTION__); \
          fputs(szPath, fp); \
          fputs("\r\n", fp); \
          fclose(fp); \
      }\
    }
#else
#define SDTRY_BEGIN	\
    try{
#define SDTRY_END \
	} \
    catch(...) \
    { \
        CHAR szPath[256]={0};	\
        sprintf(szPath,"%ssdexception.log",_SDTT2Local(SDGetModulePath()));\
        FILE * fp = fopen(szPath, "a+"); \
        if(fp) \
        { \
            CHAR szTempLine[128] = {0}; \
            CSDDateTime ti = SDNow(); \
            sprintf(szTempLine, "[%s]",("")); \
            fputs(szTempLine, fp); \
            fputs(__FILE__, fp); \
            sprintf(szPath, "(%d): %s", __LINE__, __FUNCTION__); \
            fputs(szPath, fp); \
            fputs("\r\n", fp); \
            fclose(fp); \
        }\
    }

#endif
	*/

#define SDTRY_BEGIN	
#define SDTRY_END 

#endif

