#include "sdcommandline.h"
#include <string.h>

using namespace std;

namespace SGDP{
    CSDCommandLine::CSDCommandLine()
    {
        m_pszCommandLine = NULL;
        m_nCurrent = -1;
    }

    CSDCommandLine::CSDCommandLine(const TCHAR* pszCommandLine)
    {
        m_pszCommandLine = pszCommandLine;
        m_nCurrent = -1;
        ParserCommandLine();
    }

    VOID CSDCommandLine::SetCommandLine(const TCHAR *pszCommandLine)
    {
        m_pszCommandLine = pszCommandLine;
        m_nCurrent = -1;
        ParserCommandLine();
    }

    CSDCommandLine::~CSDCommandLine()
    {
        if(m_oCommandVec.size()>0)
            m_oCommandVec.clear();
    }

    VOID CSDCommandLine::ParserCommandLine()
    {
        if(m_oCommandVec.size()>0)
            m_oCommandVec.clear();
        //以空格来分割pszCommandLine，保存在容器中，再查找特殊字符"-"
        tstring str = m_pszCommandLine;
        std::vector<tstring> vCommandStr = _SDTSplitStrings(str,_SDT(' '));
        for( INT32 i = 0 ; i < (INT32)vCommandStr.size() - 1 ; i ++ )
        {
            if(vCommandStr[i].find(_SDT('-'))==0 /*|| vCommandStr[i].find(_SDT('/'))==0*/)
            {
                CCommand* poCommand = SDNew CCommand;
				string strTemp = vCommandStr[i];
                poCommand->SetCommand(strTemp.c_str());
				strTemp = vCommandStr[i + 1];
                poCommand->SetCommandParam(strTemp.c_str());
                m_oCommandVec.push_back(poCommand);
                i++;
            }
        }
    }

    CSDCommandLine::CCommand* CSDCommandLine::GetFirstCommand()
    {
        if(m_oCommandVec.size()>0)
        {
            m_nCurrent = 0;
            return m_oCommandVec[0];
        }
        else
            return NULL;
    }

    CSDCommandLine::CCommand* CSDCommandLine::GetNextCommand()
    {
        if(m_nCurrent==-1)
            return GetFirstCommand();
        else
        {
            ++m_nCurrent;
            if(m_nCurrent>=(INT32)m_oCommandVec.size())
            {
                return NULL;
            }
            else
                return m_oCommandVec[m_nCurrent];
        }
    }
}

