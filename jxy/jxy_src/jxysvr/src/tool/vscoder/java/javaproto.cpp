#include "stdafx.h"

#include <cassert>
#include <stdexcept>

#include "javaproto.h"

CJavaType::CJavaType()
{
    m_strClass = "struct";
}

CJavaType::~CJavaType()
{
    CJavaTypeItemList::iterator guard = m_listTypeItems.begin();

    for (; guard != m_listTypeItems.end(); guard++)
    {
        delete *guard;
    }

    m_listTypeItems.clear();
}

void CJavaType::InsertItem(CJavaTypeItem* poItem)
{
    assert(poItem);

    CJavaTypeItemList::iterator guard = m_listTypeItems.begin();

    for (; guard != m_listTypeItems.end(); guard++)
    {
        if (poItem->GetName() == (*guard)->GetName()) {
            throw std::runtime_error("CJavaType Error : InsertItem -> duplicated item name !");
        }
    }

    m_listTypeItems.push_back(poItem);
}

CJavaTypeItem* CJavaType::HasItem(CString& strItemName)
{
    CJavaTypeItemList::iterator guard = m_listTypeItems.begin();
    for (; guard != m_listTypeItems.end(); guard++)
    {
        if (strItemName == (*guard)->GetName()) {
            return *guard;
        }
    }

    return NULL;
}

CJavaProto::CJavaProto()
{
}

CJavaProto::~CJavaProto()
{
}

