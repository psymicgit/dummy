#pragma once
#include "afxwin.h"
#include "singleton.h"

class CCoderParamInfo
	: public Singleton<CCoderParamInfo>
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
    BOOL m_bCPP;					// c++格式的
    BOOL m_bConfig;					// xml文件名
    BOOL m_bOutput;					// 输出路径
    BOOL m_bSilence;
    BOOL m_bHelp;
    BOOL m_bSaveEnDe;				// 是否生成保存编解码方法
    BOOL m_bCliEnDe;				// 是否生成客户端格式编码
    CString m_strConfigFullName;
    CString m_strOutputPath;
    CString m_strCharsetNet;
    CString m_strCharsetHost;
    INT32 m_nCharsetNet;
    INT32 m_nCharsetHost;

	INT32 m_nSidOffset;

};
