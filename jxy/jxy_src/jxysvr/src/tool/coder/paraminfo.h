#pragma once
#include "afxwin.h"

class CCoderParamInfo
{
public:
    enum
    {
        JAVA = 1,
        AS   = 2,
        CPP  = 3
    };

public:
    CCoderParamInfo();
    ~CCoderParamInfo();
    BOOL ParseParam(int iArgNum, char** pszParams);

    BOOL m_bJAVA;
    BOOL m_bAS;
    BOOL m_bCPP;
    BOOL m_bConfig;
    BOOL m_bOutput;
    BOOL m_bSilence;
    BOOL m_bHelp;
    CString m_strConfigFullName;
    CString m_strOutputPath;
    CString m_strCharsetNet;
    CString m_strCharsetHost;
    INT32 m_nCharsetNet;
    INT32 m_nCharsetHost;
};
