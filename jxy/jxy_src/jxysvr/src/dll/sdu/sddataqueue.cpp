#include <stdio.h>
#include "sddataqueue.h"
#include "sddebug.h"

using namespace SGDP;

CSDDataQueue::CSDDataQueue()
{
    m_pArray	= NULL;
    m_nHead		= 0;
    m_nTail		= 0;
    m_nSize		= 0;
}

CSDDataQueue::~CSDDataQueue()
{
    if(m_pArray != NULL)
    {
        delete[] m_pArray;
    }
}

BOOL  CSDDataQueue::Init(INT32 nSize)
{
    m_pArray = SDNew VOID*[nSize+1];
    m_nSize = nSize;
    return TRUE;
}

BOOL  CSDDataQueue::PushBack(VOID* ptr)
{
    INT32 nDist = m_nTail + m_nSize - m_nHead;
    INT32 nUsed = nDist >= m_nSize ? (nDist-m_nSize) : nDist;

    if(nUsed >= m_nSize-1)
    {
        return FALSE;
    }

    m_pArray[m_nTail] = ptr;
    if(++m_nTail >= m_nSize)
    {
        m_nTail = 0;
    }

    return TRUE;
}

VOID *  CSDDataQueue::PopFront()
{
    INT32 nDist = m_nTail + m_nSize - m_nHead;
    INT32 nUsed = nDist >= m_nSize ? (nDist-m_nSize) : nDist;
    if(0 == nUsed)
    {
        return NULL;
    }

    VOID* ptr = m_pArray[m_nHead];
    if(++m_nHead >= m_nSize)
    {
        m_nHead = 0;
    }

    return ptr;
}


