// SHANDA GAMES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Shanda Games and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1999-2010 Shanda Games.
//      All Rights Reserved.
//
// Shanda Games Limited, Shanghai, China, 201203
// http://www.shandagames.com

#ifndef _PROPCONTAINER_H_
#define _PROPCONTAINER_H_

#include <map>
#include <string>
#include <sdtype.h>


/**
* @brief 属性容器类
* 用于游戏中对应配置文件存储的属性管理
* 例如道具属性、技能属性等，都需要特化并实例化该类进行处理
* 提供了基于两个键值的查询功能，可以使用数字编号及属性名称查询
*/
template <typename Prop, typename PrimaryKey = UINT32, typename AssistantKey = tstring>
class CPropContainer
{
public:
    typedef std::map<PrimaryKey, Prop>                      CPriKeyPropMap;
    typedef std::map<AssistantKey, PrimaryKey>              CAssKeyPropMap;

    CPropContainer()
    {
        m_PriKeyPropMap.clear();
        m_AssKeyPropMap.clear();
    }

    virtual ~CPropContainer()
    {
        m_PriKeyPropMap.clear();
        m_AssKeyPropMap.clear();
    }

public:
    /**
    * @brief 获取容器当前装载元素个数
    * @return 容器当前装载元素个数
    */
    UINT16 GetCount()
    {
        return m_PriKeyPropMap.size();
    }

    /**
    * @brief 添加带有主键的元素
    * @param priKey 主键
    * @param property 属性
    * @return 执行结果，TRUE成功，FALSE失败
    */
    BOOL Add(PrimaryKey priKey, Prop& property)
    {
        typename CPriKeyPropMap::iterator itr = m_PriKeyPropMap.find(priKey);
        if(itr != m_PriKeyPropMap.end())
        {
            itr->second = property;
        }
        else
        {
            m_PriKeyPropMap.insert(make_pair(priKey, property));
        }
        return TRUE;
    }

    /**
    * @brief 添加带有主键和附键的元素
    * @param priKey 主键
    * @param assKey 附键
    * @param property 属性
    * @return 执行结果，TRUE成功，FALSE失败
    */
    BOOL Add(PrimaryKey priKey, AssistantKey assKey, Prop& property)
    {
        typename CPriKeyPropMap::iterator itr = m_PriKeyPropMap.find(priKey);
        if(itr != m_PriKeyPropMap.end())
        {
            itr->second = property;
        }
        else
        {
            m_PriKeyPropMap.insert(std::make_pair(priKey, property));
        }
        typename CAssKeyPropMap::iterator iter2 = m_AssKeyPropMap.find(assKey);
        if(iter2 != m_AssKeyPropMap.end())
        {
            iter2->second = priKey;
        }
        else
        {
            m_AssKeyPropMap.insert(make_pair(assKey, priKey));
        }
        return TRUE;
    }

    /**
    * @brief 清理元素
    */
    VOID Clear()
    {
        m_PriKeyPropMap.clear();
        m_AssKeyPropMap.clear();
    }

    /**
    * @brief 根据主键查找元素
    * @param key 主键
    * @return 被查找的元素
    */
    Prop* GetPropByPrimaryKey(PrimaryKey key)
    {
        typename CPriKeyPropMap::iterator itr = m_PriKeyPropMap.find(key);
        if(itr != m_PriKeyPropMap.end())
        {
            return &itr->second;
        }
        return NULL;
    }

    /**
    * @brief 根据附键查找元素
    * @param key 主键
    * @return 被查找的元素
    */
    Prop* GetPropByAssistantKey(AssistantKey key)
    {
        typename CAssKeyPropMap::iterator itr = m_AssKeyPropMap.find(key);
        if(itr != m_AssKeyPropMap.end())
        {
            typename CPriKeyPropMap::iterator itrPri = m_PriKeyPropMap.find(itr->second);
            if(itrPri != m_PriKeyPropMap.end())
            {
                return &itrPri->second;
            }
        }
        return NULL;
    }

protected:
    // 主键表
    CPriKeyPropMap  m_PriKeyPropMap;
    // 附键表
    CAssKeyPropMap  m_AssKeyPropMap;
};



template <typename SType>
struct SBitContainer
{
#define CHAT_BIT_COUNT 8

    SBitContainer()
    {
        tType = SType();
    }

    SBitContainer(const SType& tData)
    {
        tType = tData;
    }

    operator SType() const
    {
        return tType;
    }

    template<typename SOtherType>
    operator SOtherType()
    {
        return static_cast<SOtherType>(tType);
    }

    BOOL operator [](INT32 nBit)
    {
        return Test(nBit);
    }

    VOID operator = (const SType& tVal)
    {
        tType = tVal;
    }

    VOID  Set(INT32 nBit, BOOL bSet)
    {
        if ((sizeof(SType) * CHAT_BIT_COUNT) <= nBit)
        {
            if (bSet)
            {
                // 只要查出范围的值，就将每个bit设置1, gqf
                tType = (0x1 << sizeof(SType)) - 1;
            }
            return;
        }
        INT32 nBitMask = (0x1 << nBit);
        bSet? (tType |= nBitMask) : (tType &= ~nBitMask);
    }

    BOOL Test(INT32 nBit)
    {
        if ((sizeof(SType) * CHAT_BIT_COUNT) <= nBit)
        {
            return FALSE;
        }
        return ((tType & (0x1 << nBit))? TRUE : FALSE);
    }

    SType Count()
    {
        SType tCount = SType();
        for (SType i = tType; i != 0; i &= (i - 1))
        {
            tCount++;
        }
        return tCount;
    }

    SType tType;
};



#endif // #ifndef _PROPCONTAINER_H_

