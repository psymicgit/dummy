

#ifndef _NGSTRING_H_
#define _NGSTRING_H_

#include <sdutil.h>
#include <sdframework.h>
#include <sdnet.h>

class CNGString
{

public:
    CNGString()
    {
        m_strData = "";
    }

    CNGString(const char* pData)
    {
        m_strData = pData;
    }

    CNGString(const string &strData)
    {
        m_strData = strData;
    }

    ~CNGString()
    {

    }

    CNGString& operator = (const char* pData)
    {
        m_strData = pData;
        return *this;
    }

    CNGString& operator = (const string &strData)
    {
        m_strData = strData;
        return *this;
    }

    CNGString& operator = (const UINT64 &nData)
    {
        char v_szData[56] = {0};
        sprintf(v_szData, "%llu", nData);
        m_strData = v_szData;
        return *this;
    }


    CNGString& operator += (const UINT64 &nData)
    {
        char v_szData[56] = {0};
        sprintf(v_szData, "%llu", nData);
        m_strData += v_szData;
        return *this;
    }

    CNGString& operator+=(const string &strData)
    {
        m_strData += strData;
        return *this;
    }

    CNGString& operator << (const UINT64 &nData)
    {
        char v_szData[56] = {0};
        sprintf(v_szData, "%llu", nData);
        m_strData += v_szData;
        return *this;
    }

    CNGString& operator << (const string &strData)
    {
        m_strData += strData;
        return *this;
    }

    const char* c_str() const
    {
        return m_strData.c_str();
    }
    const string &str() const
    {
        return m_strData;
    }
public:
    string m_strData;
};

#endif //#ifndef _PACKETPARSER_H_

