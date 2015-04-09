#ifndef _CSHMEMMAP_H_
#define _CSHMEMMAP_H_


#include <map>
#include <deque>
#include <sdtype.h>
#include <sdshmem.h>
#include <sdsingleton.h>
#include <sdhashmap.h>

using namespace std;
using namespace SGDP;

template <typename DATAKEY, typename DATAVALUE>
class CShmemMap
{
public:
    typedef map<DATAKEY, DATAVALUE*> CHaveDataDataMap;
    typedef typename CHaveDataDataMap::iterator CHaveDataDataMapItr;
    typedef typename CHaveDataDataMap::reverse_iterator CHaveDataDataMapRitr;
public:
    typedef vector< DATAKEY > CArrayKeyList;

public:
    CShmemMap()
    {
        m_mapData.clear();
        m_itrForEach = m_mapData.begin();
        m_ritrForEach = m_mapData.rbegin();
        m_arrKeyList.clear();
    }

    BOOL AddData(DATAKEY key, DATAVALUE* pdata)
    {
        CHaveDataDataMapItr itr = m_mapData.find(key);
        if(itr != m_mapData.end())
        {
            return TRUE;
        }
        m_mapData[key] = pdata;

        m_arrKeyList.push_back( key );
        return TRUE;
    }

    DATAVALUE* GetData(DATAKEY key)
    {
        CHaveDataDataMapItr itr = m_mapData.find(key);
        if(itr != m_mapData.end())
        {
            return itr->second;
        }

        return NULL;
    }

    VOID DelData(DATAKEY key)
    {
        CHaveDataDataMapItr itr = m_mapData.find(key);
        if(itr != m_mapData.end())
        {
            BOOL bForEqual = (m_itrForEach == itr) ? TRUE : FALSE;
            m_mapData.erase(itr++);
            m_itrForEach = bForEqual ? itr : m_itrForEach;
            m_ritrForEach = m_mapData.rbegin();//
        }
        typename CArrayKeyList::iterator itrKeyList = m_arrKeyList.begin();
        for( ; itrKeyList != m_arrKeyList.end(); ++itrKeyList )
        {
            if( *itrKeyList == key )
            {
                m_arrKeyList.erase( itrKeyList );
                break;
            }
        }
    }

    DATAVALUE* GetDataByIndex( std::size_t nIndex )
    {
        if( nIndex >= m_arrKeyList.size() )
        {
            return NULL;
        }

        return GetData( m_arrKeyList[nIndex] );
    }
    CArrayKeyList &GetArrayList()
    {
        return m_arrKeyList;
    }
    //??????,??-1
    const INT64 GetDataIndexByKey( DATAKEY key )
    {
        for( std::size_t nIndex = 0; nIndex < m_arrKeyList.size(); ++nIndex )
        {
            if( m_arrKeyList[nIndex] == key )
            {
                return nIndex;
            }
        }

        return -1;
    }
    VOID Clear()
    {
        m_arrKeyList.clear();
        m_mapData.clear();
    }

    UINT32 Size()
    {
        return m_mapData.size();
    }

    DATAVALUE* GetFistData()
    {
		//如果map表为空，直接返回
		if (0 == m_mapData.size())
		{
			return NULL;
		}

        DATAVALUE* pData = NULL;
        m_itrForEach = m_mapData.begin();
        if(m_itrForEach != m_mapData.end())
        {
            pData = m_itrForEach->second;
            m_itrForEach++;
        }
        return pData;
    }

    //必须使用GetFistData后才能使用GetNextData
    DATAVALUE* GetNextData()
    {
        DATAVALUE* pData = NULL;
        if(m_itrForEach != m_mapData.end())
        {
            pData = m_itrForEach->second;
            m_itrForEach++;
        }

        return pData;
    }

    DATAVALUE* GetRFistData()
    {
        DATAVALUE* pData = NULL;
        m_ritrForEach = m_mapData.rbegin();
        if(m_ritrForEach != m_mapData.rend())
        {
            pData = m_ritrForEach->second;
            m_ritrForEach++;
        }
        return pData;
    }

    //必须使用GetRFistData后才能使用GetRNextData
    DATAVALUE* GetRNextData()
    {
        DATAVALUE* pData = NULL;
        if(m_ritrForEach != m_mapData.end())
        {
            pData = m_ritrForEach->second;
            m_ritrForEach++;
        }

        return pData;
    }

    CHaveDataDataMap& GetDataMap()
    {
        return m_mapData;
    }

protected:

private:
    CHaveDataDataMap		m_mapData;
    CHaveDataDataMapItr	m_itrForEach;
    CHaveDataDataMapRitr	m_ritrForEach;
    CArrayKeyList			m_arrKeyList;
};


#endif
