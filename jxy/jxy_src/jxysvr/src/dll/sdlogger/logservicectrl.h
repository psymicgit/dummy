/******************************************************************************
            

所属模块：共享内存日志 中日志控制块信息描述 
文件内容：定义了 日志共享内存的控制块信息
功    能：

******************************************************************************/

#ifndef LOGSERVICECTRL_H
#define LOGSERVICECTRL_H

#include <stdio.h>
#include <string.h>
#include "sdtype.h"

using namespace SGDP; 

#pragma pack(push, 1)

struct  SFileLogDesc
{

    char  m_szFileDir[256];
    char  m_szFileName[128];
    INT32 m_nId;
    INT32 m_nMode;
};

union ULogType
{
    SFileLogDesc  m_stFilelogdesc;
    char          m_acReserved[800];

};

struct SLogInfo
{
    SLogInfo()
    {

    }

    //
    //数据块的共享内存 key,-1表示当前共享内存
    //
    INT32   m_nKey;

    //
    //块大小，对用户透明，用户在写的时候需要注意不能超过这个大小
    //
    UINT32  m_dwSize;

    //
    //在共享内存中的偏移
    //
    UINT32  m_dwOffSet;
};

struct SLogServiceCtrItem
{
    enum STATE 
    {
        FREE=0,
        APPLY,
        INUSE,
        RELEASE
    };
    enum LOGTYPE 
    {
        FILE_LOG=0
    };

    SLogServiceCtrItem():m_nState(FREE),m_dwSecond(0)
    {
        memset(this,0,sizeof(SLogServiceCtrItem));
    }
    //
    //状态，表明该控制结构的状态
    //
    INT32     m_nState;

    //
    //表明该日志类型，目前只实现了文件日志
    //
    INT32     m_nType;
    
    //
    //当前时间，用来表明该控制快对应的日志对象是否存活
    //
    UINT32    m_dwSecond;

    //
    //日志详细记录方式说明
    //
    ULogType  m_stLogCtrl;

    //
    //日志记录的位置
    //
    SLogInfo  m_stLogPos;


};
#pragma pack(pop)

class CLogServiceCtr
{
public:
    CLogServiceCtr()
    {
        m_pnCount   = NULL;
        m_pnSize    = NULL;
        m_pItem     = NULL;
        m_pAddress  = NULL;
        m_pDataAddress = NULL;
    }


    CLogServiceCtr(char* pAddress)
    {
        m_pnCount = (INT32*)pAddress;
        m_pnSize = (INT32*)(pAddress + sizeof(INT32));
        m_pAddress = pAddress;
        m_pItem = (SLogServiceCtrItem*)(pAddress + sizeof(INT32) * 2);
        m_pDataAddress = pAddress + sizeof(INT32) * 2 + (*m_pnCount) * sizeof(SLogServiceCtrItem);
    }



    ~CLogServiceCtr()
    {
        m_pnCount     = NULL;
        m_pnSize      = NULL;
        m_pAddress    = NULL;
        m_pDataAddress= NULL;
        m_pItem       = NULL;
    }
    //
    //控制块的个数
    //
    INT32*             m_pnCount;

    //
    //指向共享内存头，表明每个数据块的大小
    //
    INT32*             m_pnSize;

    //
    //控制块数组，指向共享内存
    //
    SLogServiceCtrItem *m_pItem;

    //
    //共享内存起始地址
    //
    char*              m_pAddress;

    //
    //数据段起始地址
    //
    char*              m_pDataAddress;
};

#endif

