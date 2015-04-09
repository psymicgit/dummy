#include "sdmemorypool.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

namespace SGDP
{
    CSDVarMemoryPool::CSDVarMemoryPool()
        :m_pHeadPage(NULL), m_pWorkPage(NULL), m_pPageBuf(NULL)
    {
        for (UINT32 i = 0; i < UNIT_TYPE_COUNT; ++ i)
        {
            m_pFreeHead[i] = NULL;
            m_nFreeCount[i] = 0;
        }
    }

    CSDVarMemoryPool::~CSDVarMemoryPool()
    {
        MemoryPage* pMemoryPage = m_pHeadPage;
        while (m_pHeadPage != NULL)
        {
            pMemoryPage = m_pHeadPage->Next;
            free(m_pHeadPage);
            m_pHeadPage = pMemoryPage;
        }
    }

    VOID*  CSDVarMemoryPool::Malloc(UINT32 Len)
    {
        assert(Len > 0);

        Len ++;
        if (Len > MAX_UNIT_SIZE)
        {
            // allocate memory from system if requery Len is too large
            VOID* buf = malloc(Len);
            if (buf == NULL)
            {
                return NULL;
            }

            //if content of 1 byte before memory means allocate form system
            *(char*)buf = 0;

            return (BYTE*)buf + 1;
        }
        else
        {
            return GetPoolMemory(Len);
        }
    }

    VOID  CSDVarMemoryPool::Free(VOID* p)
    {
        assert(p != NULL);

        BYTE* temp = (BYTE*)p - 1;
        unsigned char type = *temp;
        if (type == 0)	//if content of 1 byte before memory means allocate form system
        {
            free(temp);
        }
        else
        {
            FreePoolMemory(temp, type);
        }
    }

    VOID* CSDVarMemoryPool::GetPoolMemory(UINT32 Len)
    {
        Len = (Len + (ALIGNMENT-1)) & ~(ALIGNMENT-1);
        int idx = (Len - 1) / ALIGNMENT;

        //if free memory unit is not enough, first get some free units
        if (m_nFreeCount[idx] == 0
                && !AddFreeMemory(idx))
        {
            return NULL;
        }

        -- m_nFreeCount[idx];
        BYTE* buf = m_pFreeHead[idx];
        m_pFreeHead[idx] = (BYTE*)(*((INT64*)m_pFreeHead[idx]));
        *buf = idx + 1;

        return buf + 1;
    }

    VOID CSDVarMemoryPool::FreePoolMemory(VOID* memblock, unsigned char type)
    {
        int idx = type - 1;
        *(INT64*)memblock = (INT64)m_pFreeHead[idx];
        m_pFreeHead[idx] = (BYTE*)memblock;
        ++ m_nFreeCount[idx];
    }

    BOOL CSDVarMemoryPool::AddFreeMemory(int idx)
    {
        const int UNIT_SIZE = (idx + 1) * ALIGNMENT;

        if ((m_pPageBuf + UNIT_SIZE ) > GetPageBufEnd(m_pWorkPage)
                && !SetMemoryPage())
        {
            return false;
        }

        BYTE* page_end = GetPageBufEnd(m_pWorkPage);
        for (UINT32 i = 0; i < ALLOC_COUNT; ++ i)
        {
            *(INT64*)m_pPageBuf = (INT64)m_pFreeHead[idx];
            m_pFreeHead[idx] = m_pPageBuf;

            m_pPageBuf += UNIT_SIZE;
            ++ m_nFreeCount[idx];

            if (m_pPageBuf + UNIT_SIZE > page_end)
                break;
        }

        return true;
    }

    BOOL CSDVarMemoryPool::SetMemoryPage()
    {
        if(m_pWorkPage->Next != NULL)
        {
            m_pWorkPage = m_pWorkPage->Next;
        }
        else
        {
            VOID* buf = malloc(sizeof(MemoryPage) + m_nPageSize);
            if (buf == NULL)
            {
                return false;
            }
            else
            {
                MemoryPage* pMemoryPage = (MemoryPage*)(buf);
                pMemoryPage->Next = NULL;
                m_pWorkPage->Next = pMemoryPage;
                m_pWorkPage = pMemoryPage;
            }
        }
        m_pPageBuf = GetPageBufGegin(m_pWorkPage);
        return true;
    }

    int CSDVarMemoryPool::GetMemUsed()
    {
        int used = 0;
        const int PAGE_SIZE = sizeof(MemoryPage) + m_nPageSize;

        MemoryPage* pMemoryPage = m_pHeadPage;
        while (pMemoryPage != NULL)
        {
            pMemoryPage = pMemoryPage->Next;
            used += PAGE_SIZE;
        }

        return used;
    }

    VOID  CSDVarMemoryPool::Clear()
    {
        m_pWorkPage = m_pHeadPage;
        m_pPageBuf = GetPageBufGegin(m_pWorkPage);
    }

    BOOL CSDVarMemoryPool::Create( UINT32 PageSize /*= 0x80000*/ )
    {
        PageSize = (PageSize + (ALIGNMENT-1)) & ~(ALIGNMENT-1);
        if (PageSize < MIN_PAGESIZE)
        {
            m_nPageSize = MIN_PAGESIZE;
        }
        else
        {
            m_nPageSize = PageSize;
        }

        VOID* buf = malloc(sizeof(MemoryPage) + m_nPageSize);
        if (buf == NULL)
        {
            return false;
        }
        else
        {
            MemoryPage* pMemoryPage = (MemoryPage*)(buf);
            pMemoryPage->Next = NULL;
            m_pWorkPage = pMemoryPage;
            m_pPageBuf = GetPageBufGegin(m_pWorkPage);
            m_pHeadPage = m_pWorkPage;
        }

        return true;
    }

// Fix Memory pool size

    CSDFixMemoryPool::CSDFixMemoryPool()
        :m_pHeadPage(NULL), m_nUnitSize(0), m_nPageSize(0)
    {
    }

    CSDFixMemoryPool::~CSDFixMemoryPool()
    {
        MemoryPage* pMemoryPage = m_pHeadPage;
        while (m_pHeadPage != NULL)
        {
            pMemoryPage = m_pHeadPage->Next;
            free(m_pHeadPage);
            m_pHeadPage = pMemoryPage;
        }
    }

    VOID* CSDFixMemoryPool::Malloc()
    {
        MemoryPage* pMemoryPage = m_pHeadPage;
        while (pMemoryPage != NULL && pMemoryPage->nFreecount == 0)
        {
            pMemoryPage = pMemoryPage->Next;
        }

        // add SDNew page if space is not enough
        if (pMemoryPage == NULL)
        {
            if(!AddMemoryPage())
            {
                return NULL;
            }
            pMemoryPage = m_pHeadPage;
        }

        // get unused memory
        -- pMemoryPage->nFreecount;
        char* buf = GetPageBuf(pMemoryPage) + pMemoryPage->nFreeHead * m_nUnitSize;
        pMemoryPage->nFreeHead = *(int*)(buf);

        return buf;
    }

    VOID CSDFixMemoryPool::Free(VOID* p)
    {
        // don't check null point for fast
        MemoryPage* pMemoryPage = m_pHeadPage;
        char* buf = GetPageBuf(m_pHeadPage);

        // find point in which page
        while((p < buf ||
                p > buf + m_nPageSize) &&
                pMemoryPage != NULL)
        {
            pMemoryPage = pMemoryPage->Next;
            buf = GetPageBuf(pMemoryPage);
        }

        // do not in any page
        if (pMemoryPage == NULL)
        {
            return;
        }

        *(int*)p = pMemoryPage->nFreeHead;
        pMemoryPage->nFreeHead = ((char*)p - buf) / m_nUnitSize;
        ++ pMemoryPage->nFreecount;

        return;
    }

    BOOL CSDFixMemoryPool::AddMemoryPage()
    {
        VOID* buf = malloc(sizeof(MemoryPage) + m_nPageSize);
        if (buf == NULL)
        {
            return false;
        }

        MemoryPage* pMemoryPage = (MemoryPage*)(buf);
        InitPage(pMemoryPage);

        if (m_pHeadPage == NULL)
        {
            pMemoryPage->Next = NULL;
            m_pHeadPage = pMemoryPage;
        }
        else
        {
            pMemoryPage->Next = m_pHeadPage;
            m_pHeadPage = pMemoryPage;
        }

        return true;
    }

    INT32  CSDFixMemoryPool::GetMemUsed()
    {
        int used = 0;
        const int PAGE_SIZE = sizeof(MemoryPage) + m_nPageSize;

        MemoryPage* pMemoryPage = m_pHeadPage;
        while (pMemoryPage != NULL)
        {
            pMemoryPage = pMemoryPage->Next;
            used += PAGE_SIZE;
        }

        return used;
    }

    VOID  CSDFixMemoryPool::Clear()
    {
        MemoryPage* pMemoryPage = m_pHeadPage;
        while (pMemoryPage != NULL)
        {
            InitPage(pMemoryPage);
            pMemoryPage = pMemoryPage->Next;
        }
    }

    VOID CSDFixMemoryPool::InitPage(MemoryPage *Page)
    {
        Page->nFreecount = m_nPageSize / m_nUnitSize;
        Page->nFreeHead = 0;

        VOID* head = GetPageBuf(Page);
        for (int i = 1; i < Page->nFreecount; ++i)
        {
            *(int*)head = i;
            head = (int*)((char*)head + m_nUnitSize);
        }
    }

    BOOL CSDFixMemoryPool::Create( UINT32 UnitSize, UINT32 PageSize /*= 0x40000*/ )
    {
        if (UnitSize < 4)
        {
            m_nUnitSize = 4;
        }
        {
            m_nUnitSize = (UnitSize + (ALIGNMENT-1)) & ~(ALIGNMENT-1);
        }

        if (PageSize < MIN_PAGESIZE)
        {
            m_nPageSize = MIN_PAGESIZE;
        }
        else
        {
            m_nPageSize = (PageSize / m_nUnitSize) * m_nUnitSize;
        }

        return AddMemoryPage();
    }

}



