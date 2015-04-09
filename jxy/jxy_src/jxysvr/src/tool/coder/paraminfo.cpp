#include "StdAfx.h"
#include "paraminfo.h"
#include "charset.h"

inline INT32 GetCharsetIndex(const CHAR* pszName)
{
    if (NULL == pszName)
    {
        return -1;
    }

    for (int i = 0; i < nCharset; i++)
    {
        if (0 == strcmp(szCharset[i], pszName))
        {
            return i;
        }
    }

    return -1;
}

CCoderParamInfo::CCoderParamInfo()
: m_bJAVA(FALSE), m_bAS(FALSE), m_bCPP(FALSE), m_bConfig(FALSE),
  m_bOutput(FALSE), m_bSilence(FALSE), m_bHelp(FALSE), m_nCharsetNet(-1),
  m_nCharsetHost(-1)
{
}

CCoderParamInfo::~CCoderParamInfo()
{
}

BOOL CCoderParamInfo::ParseParam(int iArgNum, char** pszParams)
{
    if (iArgNum <= 1)
    {
        return FALSE;
    }
    for (int i = 1; i < iArgNum; i++)
    {
        BOOL bType = m_bJAVA || m_bAS || m_bCPP;
        CString sArg(pszParams[i]);

        if (!m_bHelp)
        {
            m_bHelp = !sArg.CompareNoCase("/h");
            if (m_bHelp)
            {
                return TRUE;
            }
        }

        if (!m_bSilence)
        {
            m_bSilence = !sArg.CompareNoCase("/s");
        }

        // determine output type
        if (!bType)
        {
            m_bJAVA = !sArg.CompareNoCase("/java");
            m_bAS   = !sArg.CompareNoCase("/as");
            m_bCPP  = !sArg.CompareNoCase("/cpp");
        }

        // get config file full path
        BOOL bConfig = m_bConfig;
        if (!bConfig)
        {
            m_bConfig = !sArg.CompareNoCase("/config");
            if (m_bConfig)
            {
                if (i + 1 < iArgNum)
                {
                    m_strConfigFullName = pszParams[++i];
                    continue;
                }
                else
                {
                    return FALSE;
                }
            }
        }

        // get output dir
        BOOL bOutput = m_bOutput;
        if (!bOutput)
        {
            m_bOutput = !sArg.CompareNoCase("/out");
            if (m_bOutput)
            {
                if (i + 1 < iArgNum)
                {
                    m_strOutputPath = pszParams[++i];
                    continue;
                }
                else
                {
                    return FALSE;
                }
            }
        }

        // TODO: compare and get index

        if (sArg.CompareNoCase("/net"))
        {
            if (i + 1 < iArgNum)
            {
                m_strCharsetNet = pszParams[++i];
                m_nCharsetNet   = GetCharsetIndex(m_strCharsetNet.GetString());
                continue;
            }
            else
            {
                return FALSE;
            }
        }

        if (sArg.CompareNoCase("/host"))
        {
            if (i + 1 < iArgNum)
            {
                m_strCharsetHost = pszParams[++i];
                m_nCharsetHost   = GetCharsetIndex(m_strCharsetHost.GetString());
                continue;
            }
            else
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}
