#include "utility.h"
#ifndef __linux__
#include <wtypes.h>
#include <WinBase.h>
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include <math.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <math.h>
#include "common/server/ngstring.h"
#include "common/client/errdef.h"
#include "common/client/commondef.h"
using namespace SGDP;



//次方函数，dwData的byNum次方
UINT32 power(UINT32 dwData, UINT8 byNum)
{
    UINT32 dwRet = 1;
    for(UINT8 byIdx = 1; byIdx <= byNum; byIdx++)
    {
        dwRet = dwRet * dwData;
    }

    return dwRet;
}

INT32 RandomReseed(UINT32 dwRange)
{
    static UINT32 dwSeed = 0;
    dwSeed = (GetCycleCount() + (dwSeed++)) << 7;
    SDSetSeed(dwSeed);
    return SDRandom(dwRange);
}

INT32 GetRandomIdx(CRandomVec& vecRandom, UINT32 dwTotalCent)
{
    UINT32 dwSize = vecRandom.size();
    if(0 == dwSize)
    {
        return RANDOM_UNHIT;
    }
    UINT32 dwTotal = 0;
    UINT32 dwIdx = 0;
    for(dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        dwTotal += vecRandom[dwIdx];
    }

    dwTotal = dwTotal < dwTotalCent ? dwTotalCent : dwTotal; //dwTotalCent比值，100表示百分比

    static UINT32 dwSeed = 0;
    dwSeed = (GetCycleCount() + (dwSeed++)) << 7;
    SDSetSeed(dwSeed);
    UINT32 dwHit = SDRandom(dwTotal);

    dwTotal = 0;
    for(dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        dwTotal += vecRandom[dwIdx];
        if(dwHit < dwTotal)
        {
            return dwIdx;
        }

    }

    return RANDOM_UNHIT;
}


INT32 GetRandomIdxMustHit(CRandomVec& vecRandom)
{
    UINT32 dwSize = vecRandom.size();
    if(0 == dwSize)
    {
        return RANDOM_UNHIT;
    }
    UINT32 dwTotal = 0;
    UINT32 dwIdx = 0;
    for(dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        dwTotal += vecRandom[dwIdx];
    }
    if(0 == dwTotal)
    {
        return 0;
    }

    static UINT32 dwTime = 0;
    UINT32 dwSeed = (GetCycleCount() + (dwTime++)) << 7;
    SDSetSeed(dwSeed);
    UINT32 dwHit = SDRandom(dwTotal);

    dwTotal = 0;
    for(dwIdx = 0; dwIdx < dwSize; dwIdx++)
    {
        dwTotal += vecRandom[dwIdx];
        if(dwHit < dwTotal)
        {
            return dwIdx;
        }
    }

    return 0;
}

UINT32 GetRandomIdxMustHit(const CRandomMap& mapRandom)
{
	UINT32 dwSize = mapRandom.size();
	if(0 == dwSize)
	{
		return 0;
	}
	UINT32 dwTotal = 0;
	CRandomMapItr itr = mapRandom.begin();
	for ( ; itr != mapRandom.end(); itr++)
	{
		dwTotal += itr->second;
	}

	if(0 == dwTotal)
	{
		return 0;
	}

	static UINT32 dwTime = 0;
	UINT32 dwSeed = (GetCycleCount() + (dwTime++)) << 7;
	SDSetSeed(dwSeed);
	UINT32 dwHit = SDRandom(dwTotal);

	dwTotal = 0;
	itr = mapRandom.begin();
	for ( ; itr != mapRandom.end(); itr++)
	{
		dwTotal += itr->second;
		if(dwHit < dwTotal)
		{
			return itr->first;
		}
	}
	return 0;
}

INT32 GetRandomIdxMustHit(CRandomVec& vecRandom, INT32 dwStart, INT32 dwEnd)
{
    INT32 dwIdx = 0;
    INT32 dwTotal = 0;
    INT32 dwSize = vecRandom.size();
    if (dwStart > dwSize || dwEnd > dwSize)
    {
        return RANDOM_UNHIT;
    }

    if (0 == dwEnd )
    {
        dwEnd = dwSize;
    }

    //优先在【dwStart ~ dwEnd】 区间中查找
    for(dwIdx = dwStart; dwIdx < dwEnd; dwIdx++)
    {
        if(0 != vecRandom[dwIdx])
        {
            dwTotal++;
        }
    }

    static UINT32 dwTime = 0;
    UINT32 dwSeed = (GetCycleCount() + (dwTime++)) << 7;
    SDSetSeed(dwSeed);

    INT32 dwTemp = 0;
    INT32 dwHit  = 0;
    if(dwTotal > 0)
    {
        dwHit = SDRandom(dwTotal);
        for(dwIdx = dwStart; dwIdx < dwEnd; dwIdx++)
        {
            if(0 == vecRandom[dwIdx])
            {
                continue;
            }
            if (dwTemp == dwHit)
            {
                return dwIdx;
            }
            dwTemp++;
        }
    }

    //在【0 ~ dwStart】 区间中查找
    //////////////////////////////////////////////////////////////////////////
    dwTotal = 0;
    for(dwIdx = 0; dwIdx < dwStart; dwIdx++)
    {
        if(0 != vecRandom[dwIdx])
        {
            dwTotal++;
        }
    }

    dwTime = 0;
    dwTemp = 0;
    dwSeed = (GetCycleCount() + (dwTime++)) << 7;
    SDSetSeed(dwSeed);
    if(dwTotal > 0)
    {
        dwHit = SDRandom(dwTotal);
        for(dwIdx = 0; dwIdx < dwStart; dwIdx++)
        {
            if(0 == vecRandom[dwIdx])
            {
                continue;
            }
            if (dwTemp == dwHit)
            {
                return dwIdx;
            }
            dwTemp++;
        }
    }

    //在【dwEnd ~ dwSize】 区间中查找
    //////////////////////////////////////////////////////////////////////////
    dwTotal = 0;
    for(dwIdx = dwEnd; dwIdx < dwSize; dwIdx++)
    {
        if(0 != vecRandom[dwIdx])
        {
            dwTotal++;
        }
    }

    dwTime = 0;
    dwTemp = 0;
    dwSeed = (GetCycleCount() + (dwTime++)) << 7;
    SDSetSeed(dwSeed);
    if(dwTotal > 0)
    {
        dwHit = SDRandom(dwTotal);
        for(dwIdx = dwEnd; dwIdx < dwSize; dwIdx++)
        {
            if(0 == vecRandom[dwIdx])
            {
                continue;
            }
            if (dwTemp == dwHit)
            {
                return dwIdx;
            }
            dwTemp++;
        }
    }
    return RANDOM_UNHIT;
}





VOID ListFile(const CHAR* szDir, const CHAR* szFilePrefixName, const CHAR* szFilePostfixName, BOOL bRecursive, map<INT16, string>& mapFiles)
{
#ifndef __linux__
    HANDLE hSearch;
    WIN32_FIND_DATA data;
    CHAR szTmp[512] = {0};
    sprintf(szTmp, "%s/*.*", szDir);
    hSearch = FindFirstFile(szTmp, &data);
    INT32 nPrefixLen = strlen(szFilePrefixName);
    INT32 nPostfixLen = strlen(szFilePostfixName);
    SetCurrentDirectory(szDir);
    do
    {
        CHAR* pszFileName = data.cFileName;
        if((data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) && (0 != strcmp(pszFileName, ".")) && (0 != strcmp(pszFileName, "..")))
        {
            if(bRecursive)
            {
                string strSubDir = string(szDir) +  string("/") + string(pszFileName);
                SetCurrentDirectory(strSubDir.c_str());
                ListFile(strSubDir.c_str(), szFilePrefixName, szFilePostfixName, bRecursive, mapFiles);
                SetCurrentDirectory("..");
            }
        }
        else
        {
            if((0 != strcmp(pszFileName, ".")) && (0 != strcmp(pszFileName, "..")))
            {
                CHAR* pTmp = strstr(pszFileName, szFilePostfixName);
                if(pTmp)
                {
                    pTmp += nPostfixLen;
                    if((0 == *pTmp) && (0 == strncmp(pszFileName, szFilePrefixName, nPrefixLen)))
                    {
                        CHAR szTmp[32] = {0};
                        CHAR* pszStrart = strstr(pszFileName, szFilePrefixName);
                        CHAR* pszEnd = strstr(pszFileName, szFilePostfixName);
                        pszStrart += nPrefixLen;
                        memcpy(szTmp, pszStrart, pszEnd - pszStrart);
                        UINT16 wIdx = SDAtou(szTmp);
                        CHAR szCurDir[512] = {0};
                        GetCurrentDirectory(512, szCurDir);
                        CHAR szPath[512] = {0};
                        sprintf(szPath, "%s/%s", szCurDir, pszFileName);
                        mapFiles[wIdx] = szPath;
                    }
                }

            }
        }
    }
    while(FindNextFile(hSearch, &data));
    FindClose(hSearch);

#else
    CHAR szOldWorkDir[512] = {0};
    sprintf(szOldWorkDir, "%s", _SDTT2A(SDGetModulePath()));

    INT32 nPrefixLen = strlen(szFilePrefixName);
    INT32 nPostfixLen = strlen(szFilePostfixName);
    struct dirent* ent = NULL;
    DIR *pDir;
    pDir = opendir(szDir);
    if (pDir == NULL)
    {
        return;
    }
    chdir(szDir);
    while (NULL != (ent = readdir(pDir)))
    {
        CHAR* pszFileName = ent->d_name;
        if (ent->d_type == DT_REG)
        {
            CHAR* pTmp = strstr(pszFileName, szFilePostfixName);
            if(pTmp)
            {
                pTmp += nPostfixLen;
                if((0 == *pTmp) && (0 == strncmp(pszFileName, szFilePrefixName, nPrefixLen)))
                {
                    CHAR szTmp[32] = {0};
                    CHAR* pszStrart = strstr(pszFileName, szFilePrefixName);
                    CHAR* pszEnd = strstr(pszFileName, szFilePostfixName);
                    pszStrart += nPrefixLen;
                    memcpy(szTmp, pszStrart, pszEnd - pszStrart);
                    UINT16 wIdx = SDAtou(szTmp);
                    CHAR szCurDir[512] = {0};
                    getcwd(szCurDir, 512);
                    CHAR szPath[512] = {0};
                    sprintf(szPath, "%s/%s", szCurDir, pszFileName);
                    mapFiles[wIdx] = szPath;
                }
            }
        }
        else
        {
            if (strcmp(pszFileName, ".") == 0 || strcmp(pszFileName, "..") == 0)
            {
                continue;
            }

            if(bRecursive)
            {
                string strSubDir = string(szDir) +  string("/") + string(pszFileName);
                ListFile(pszFileName, szFilePrefixName, szFilePostfixName, bRecursive, mapFiles);
            }
        }
    }
    chdir(szOldWorkDir);
#endif
}

VOID  ListFile(const CHAR* szDir, const CHAR* szFilePrefixName, const CHAR* szFilePostfixName, BOOL bRecursive, vector<string>& vecFiles)
{
#ifndef __linux__
    HANDLE hSearch;
    WIN32_FIND_DATA data;
    CHAR szTmp[512] = {0};
    sprintf(szTmp, "%s/*.*", szDir);
    hSearch = FindFirstFile(szTmp, &data);
    INT32 nPrefixLen = strlen(szFilePrefixName);
    INT32 nPostfixLen = strlen(szFilePostfixName);

    char szOldDir[512];
    GetCurrentDirectoryA( strlen(szOldDir), szOldDir );

    SetCurrentDirectory(szDir);
    do
    {
        CHAR* pszFileName = data.cFileName;
        if((data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) && (0 != strcmp(pszFileName, ".")) && (0 != strcmp(pszFileName, "..")))
        {
            if(bRecursive)
            {
                string strSubDir = string(szDir) +  string("/") + string(pszFileName);
                SetCurrentDirectory(strSubDir.c_str());
                ListFile(strSubDir.c_str(), szFilePrefixName, szFilePostfixName, bRecursive, vecFiles);
                SetCurrentDirectory("..");
            }
        }
        else
        {
            if((0 != strcmp(pszFileName, ".")) && (0 != strcmp(pszFileName, "..")))
            {
                CHAR* pTmp = strstr(pszFileName, szFilePostfixName);
                if(pTmp)
                {
                    pTmp += nPostfixLen;
                    if((0 == *pTmp) && (0 == strncmp(pszFileName, szFilePrefixName, nPrefixLen)))
                    {
                        CHAR szCurDir[512] = {0};
                        GetCurrentDirectory(512, szCurDir);
                        CHAR szPath[512] = {0};
                        sprintf(szPath, "%s/%s", szCurDir, pszFileName);
                        vecFiles.push_back(szPath);
                    }
                }

            }
        }
    }
    while(FindNextFile(hSearch, &data));
    FindClose(hSearch);
    SetCurrentDirectory( szOldDir );
#else
    CHAR szOldWorkDir[512] = {0};
    sprintf(szOldWorkDir, "%s", _SDTT2A(SDGetModulePath()));

    INT32 nPrefixLen = strlen(szFilePrefixName);
    INT32 nPostfixLen = strlen(szFilePostfixName);
    struct dirent* ent = NULL;
    DIR *pDir;
    pDir = opendir(szDir);
    if (pDir == NULL)
    {
        return;
    }
    chdir(szDir);
    while (NULL != (ent = readdir(pDir)))
    {
        CHAR* pszFileName = ent->d_name;
        if (ent->d_type == DT_REG)
        {
            CHAR* pTmp = strstr(pszFileName, szFilePostfixName);
            if(pTmp)
            {
                pTmp += nPostfixLen;
                if((0 == *pTmp) && (0 == strncmp(pszFileName, szFilePrefixName, nPrefixLen)))
                {
                    CHAR szCurDir[512] = {0};
                    getcwd(szCurDir, 512);
                    CHAR szPath[512] = {0};
                    sprintf(szPath, "%s/%s", szCurDir, pszFileName);
                    vecFiles.push_back(szPath);
                }
            }
        }
        else
        {
            if (strcmp(pszFileName, ".") == 0 || strcmp(pszFileName, "..") == 0)
            {
                continue;
            }

            if(bRecursive)
            {
                string strSubDir = string(szDir) +  string("/") + string(pszFileName);
                ListFile(pszFileName, szFilePrefixName, szFilePostfixName, bRecursive, vecFiles);
            }
        }
    }
    chdir(szOldWorkDir);
#endif
}


VOID  ListFile(const CHAR* szDir, const CHAR* szFilePostfixName, BOOL bRecursive, vector<string>& vecFiles)
{
#ifndef __linux__
    HANDLE hSearch;
    WIN32_FIND_DATA data;
    CHAR szTmp[512] = {0};
    sprintf(szTmp, "%s/*.*", szDir);
    hSearch = FindFirstFile(szTmp, &data);
    INT32 nPostfixLen = strlen(szFilePostfixName);

    char szOldDir[512];
    GetCurrentDirectoryA( strlen(szOldDir), szOldDir );

    SetCurrentDirectory(szDir);
    do
    {
        CHAR* pszFileName = data.cFileName;
        if((data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) && (0 != strcmp(pszFileName, ".")) && (0 != strcmp(pszFileName, "..")))
        {
            if(bRecursive)
            {
                string strSubDir = string(szDir) +  string("/") + string(pszFileName);
                SetCurrentDirectory(strSubDir.c_str());
                ListFile(strSubDir.c_str(), szFilePostfixName, bRecursive, vecFiles);
                SetCurrentDirectory("..");
            }
        }
        else
        {
            if((0 != strcmp(pszFileName, ".")) && (0 != strcmp(pszFileName, "..")))
            {
                CHAR* pTmp = strstr(pszFileName, szFilePostfixName);
                if(pTmp)
                {
                    pTmp += nPostfixLen;
                    if(0 == *pTmp)
                    {
                        CHAR szCurDir[512] = {0};
                        GetCurrentDirectory(512, szCurDir);
                        CHAR szPath[512] = {0};
                        sprintf(szPath, "%s/%s", szCurDir, pszFileName);
                        vecFiles.push_back(szPath);
                    }
                }

            }
        }
    }
    while(FindNextFile(hSearch, &data));
    FindClose(hSearch);
    SetCurrentDirectory( szOldDir );
#else
    CHAR szOldWorkDir[512] = {0};
    sprintf(szOldWorkDir, "%s", _SDTT2A(SDGetModulePath()));

    INT32 nPostfixLen = strlen(szFilePostfixName);
    struct dirent* ent = NULL;
    DIR *pDir;
    pDir = opendir(szDir);
    if (pDir == NULL)
    {
        return;
    }
    chdir(szDir);
    while (NULL != (ent = readdir(pDir)))
    {
        CHAR* pszFileName = ent->d_name;
        if (ent->d_type == DT_REG)
        {
            CHAR* pTmp = strstr(pszFileName, szFilePostfixName);
            if(pTmp)
            {
                pTmp += nPostfixLen;
                if(0 == *pTmp)
                {
                    CHAR szCurDir[512] = {0};
                    getcwd(szCurDir, 512);
                    CHAR szPath[512] = {0};
                    sprintf(szPath, "%s/%s", szCurDir, pszFileName);
                    vecFiles.push_back(szPath);
                }
            }
        }
        else
        {
            if (strcmp(pszFileName, ".") == 0 || strcmp(pszFileName, "..") == 0)
            {
                continue;
            }

            if(bRecursive)
            {
                string strSubDir = string(szDir) +  string("/") + string(pszFileName);
                ListFile(pszFileName, szFilePostfixName, bRecursive, vecFiles);
            }
        }
    }
    chdir(szOldWorkDir);
#endif
}



VOID  ListDir(const CHAR* szDir, const CHAR* szDirPrefixName, vector<string>& vecDirs)
{
#ifndef __linux__
    HANDLE hSearch;
    WIN32_FIND_DATA data;
    CHAR szTmp[512] = {0};
    sprintf(szTmp, "%s/*.*", szDir);
    hSearch = FindFirstFile(szTmp, &data);
    INT32 nPrefixLen = strlen(szDirPrefixName);
    SetCurrentDirectory(szDir);
    do
    {
        if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (0 != strcmp(data.cFileName, ".")) && (0 != strcmp(data.cFileName, "..")))
        {
            CHAR* pTmp = strstr(data.cFileName, szDirPrefixName);
            if(pTmp)
            {
                vecDirs.push_back(data.cFileName);
            }
        }

    }
    while(FindNextFile(hSearch, &data));
    FindClose(hSearch);

#else
    CHAR szOldWorkDir[512] = {0};
    sprintf(szOldWorkDir, "%s", _SDTT2A(SDGetModulePath()));

    struct dirent* ent = NULL;
    DIR *pDir;
    pDir = opendir(szDir);
    if (pDir == NULL)
    {
        return;
    }
    chdir(szDir);
    while (NULL != (ent = readdir(pDir)))
    {
        CHAR* pszFileName = ent->d_name;
        if (ent->d_type == DT_DIR)
        {
            if (strcmp(pszFileName, ".") == 0 || strcmp(pszFileName, "..") == 0)
            {
                continue;
            }

            CHAR* pTmp = strstr(pszFileName, szDirPrefixName);
            if(pTmp)
            {
                vecDirs.push_back(pszFileName);
                string strSubDir = string(szDir) +  string("/") + string(pszFileName);
                ListDir(strSubDir.c_str(), szDirPrefixName, vecDirs);
            }
        }
    }
    chdir(szOldWorkDir);
#endif
}


string GetLimitStr(const CHAR* szSrc, const CHAR* szPrefixName, const CHAR* szPostfixName)
{
    if(NULL == szSrc)
    {
        return "";
    }
    string strSrc(szSrc);
    INT32 nPrefixIdx = 0;
    if(szPrefixName)
    {
        nPrefixIdx = strSrc.find(szPrefixName);
    }
    if(nPrefixIdx < 0)
    {
        return "";
    }
    INT32 nPostfixIdx = strSrc.length();
    if(szPostfixName)
    {
        nPostfixIdx	= strSrc.find(szPostfixName);
    }
    if(nPostfixIdx < 0)
    {
        return "";
    }
    INT32 nPrefixLen = strlen(szPrefixName);
    if(nPostfixIdx <= (nPrefixIdx + nPrefixLen))
    {
        return "";
    }

    INT32 nBeginIdx = nPrefixIdx + nPrefixLen;
    return strSrc.substr(nBeginIdx, nPostfixIdx - nBeginIdx);
}


BOOL HaveSqlToken(TCHAR* pszSrc)
{
    INT8 cIdx = 100;//最多100次，避免错误数据死循环
    TCHAR* pszTmp = pszSrc;
    while((0 != *pszTmp) && (--cIdx > 0))
    {
        if((*pszTmp == '\'') ||
                (*pszTmp == '\"') ||
                (*pszTmp == ';') ||
                (*pszTmp == '>') ||
                (*pszTmp == '<') ||
                (*pszTmp == '(') ||
                (*pszTmp == ')') ||
                (*pszTmp == '|'))
        {
            return TRUE;
        }

        //过虑‘--’又字符
        if (*pszTmp == '-')
        {
            pszTmp++;
            if (*pszTmp == '-')
            {
                return TRUE;
            }
            continue;
        }
        pszTmp++;
    }

    return FALSE;
}


BOOL CkHaveUtf8mb4(TCHAR* pszDspName)
{
    UINT16 wLen = strlen(pszDspName);
    UINT16 wIdx = 0;
    INT8* byTmp = (INT8*)(pszDspName);
    while (wIdx < wLen)
    {
        INT16 shTmp = byTmp[wIdx];

        if (shTmp > 0)
        {
            wIdx++;
            continue;
        }

        UINT16 wTmp = shTmp & 0x00FF;
        if((wTmp >= 0xC0) && (wTmp <= 0xDF))
        {
            wIdx += 2;
        }
        else if((wTmp >= 0xE0) && (wTmp <= 0xEF))
        {
            wIdx += 3;
        }
        else if((wTmp >= 0xF0) && (wTmp <= 0xF7))
        {
            return TRUE;
        }
        else
        {
            wIdx++;
        }
    }

    return FALSE;
}

//过虑(串)
BOOL MisplacedToken(TCHAR* pszSrc, TCHAR*pszDsc)
{
    INT32 nTemp = 0;
    INT32 nLen = strlen(pszSrc);
    for (int i = 0; i < nLen; i++)
    {
        if ('\t' == pszSrc[i])
        {
            continue;
        }
        if ('\n' == pszSrc[i])
        {
            continue;
        }
        if ('\r' == pszSrc[i])
        {
            continue;
        }
        if ((0 == i || nLen - 1 == i ) && ' ' == pszSrc[i])
        {
            continue;
        }
        pszDsc[nTemp++] = pszSrc[i];
    }
    return TRUE;
}



BOOL CkHaveSpeCharacter(TCHAR* pszDspName)
{
    wstring strName = SDUTF82W(pszDspName);
    UINT16 wLen = strName.length();
    WCHAR *lpwSz = SDNew WCHAR[wLen + 1];
    SDWcscpy(lpwSz, strName.c_str());
    BOOL bRet = FALSE;
    UINT16 wIdx = 0;
    while (wIdx < wLen)
    {
        UINT16 wTmp = lpwSz[wIdx++];

        if((wTmp >= '1') && (wTmp <= '9'))
        {
            continue;
        }
        else if((wTmp >= 'a') && (wTmp <= 'z'))
        {
            continue;
        }
        else if((wTmp >= 'A') && (wTmp <= 'Z'))
        {
            continue;
        }
        else if((wTmp == '.') || (wTmp == '@') || (wTmp == '_'))
        {
            continue;
        }
        else if(wTmp == 0x3002)
        {
            continue;
        }
        else if((wTmp >= 0x4e00)  && (wTmp <= 0x9fa5 ))
        {
            continue;
        }

        bRet = TRUE;
        break;
    }
    SDDelete []lpwSz;
    lpwSz = NULL;

    return bRet;
}


UINT32 GetRdNum()
{
    static UINT32 dwSeed = 0;
    dwSeed = GetCycleCount() + ((++dwSeed) << 7);
    srand(dwSeed);
    return rand();
}

VOID InitRadomQue(UINT8 byRate, vector<UINT8>& vecRadom)
{
    vecRadom.clear();

    if((byRate >= 100 ) || (0 == byRate))
    {
        UINT8 byHit = 0 == byRate ? 0 : 1;
        for(UINT8 byIdx = 0; byIdx < 100; byIdx++)
        {
            vecRadom.push_back(byHit);
        }

        return;
    }


    UINT8 byOddRate = 0;	//剩余概率
    UINT8 byItemNum = 100 / byRate;
    byOddRate = 100 % byRate;
    UINT8 byItem = 0;
    if(byOddRate > 0)
    {
        if((100 / (byItemNum + 1) * (byItemNum + 1)) >= (100 / byItemNum * byItemNum))
        {
            byItemNum++;
        }
        byItem = 100 / byItemNum;
        byOddRate = byRate > byItem ?  byRate - byItem : 0;
    }
    else
    {
        byItem = 100 / byItemNum;
    }

    UINT8 byOddRateHitNum = 0;
    for(UINT8 byItemIdx = 0; byItemIdx < byItem; byItemIdx++)
    {
        UINT8 byRdNum = GetRdNum() % byItemNum;
        for(UINT8 byIdx = 0; byIdx < byItemNum; byIdx++)
        {
            if(byIdx == byRdNum)
            {
                vecRadom.push_back(1);//命中
            }
            else
            {
                //赋值剩余概率,最后的值有可能小于byOddRate，可忽略
                if(byOddRateHitNum < byOddRate)
                {
                    if(GetRdNum() % 50 < byOddRate)
                    {
                        vecRadom.push_back(1);//命中
                        byOddRateHitNum++;
                        continue;
                    }
                }
                vecRadom.push_back(0);//未命中
            }
        }
    }
}

INT32 Round( DOUBLE nValue )
{
    DOUBLE nTmp = fabs(nValue);
    if( nValue > 0 )
    {
        return INT32( nTmp + 0.5 );
    }
    else
    {
        return INT32( -(nTmp + 0.5) );
    }
}

UINT32 AddValueByValueType(UINT64 qwSourceValue, UINT32 dwAddValue, UINT8 byAddValueType)
{
	if ( EVT_FIX == byAddValueType)
	{
		return dwAddValue;
	}
	else if ( EVT_PERCENT == byAddValueType)
	{
		return static_cast<UINT32>((qwSourceValue * dwAddValue) / 100);
	}
	return 0;
}

string dw2str(UINT32 dwNum)
{
    char szNum[32] = {0};
    sprintf(szNum, "%u", dwNum);
    string strNum = szNum;
    return strNum;
}

string qw2str(UINT64 qwNum)
{
    char szNum[64] = {0};
    sprintf(szNum, "%llu", qwNum);
    string strNum = szNum;
    return strNum;
}

CAutoCycle::CAutoCycle(const CHAR* pszMsg)
{
    m_strMsg = pszMsg;
    m_dwStartCycle = GetCycleCount();
    //m_dwStartCycle = SDGetTickCount();
    m_byIdx = 0;
}

CAutoCycle::~CAutoCycle()
{
    DBG_INFO(_SDT("[%s: %d]: PastCycle[%s] [end]:[%u]\n"), MSG_MARK, m_strMsg.c_str(), GetCycleCount() - m_dwStartCycle);
    //DBG_INFO(_SDT("[%s: %d]: PastCycle[%s] [end]:[%u]\n"), MSG_MARK, m_strMsg.c_str(), SDGetTickCount() - m_dwStartCycle);
}

VOID CAutoCycle::PrintCycle(const CHAR* pszMsg)
{
    if(pszMsg)
    {
        DBG_INFO(_SDT("[%s: %d]: PastCycle[%s] [%s]:[%u]"), MSG_MARK, m_strMsg.c_str(), pszMsg, GetCycleCount() - m_dwStartCycle);
        //DBG_INFO(_SDT("[%s: %d]: PastCycle[%s] [%s]:[%u]"), MSG_MARK, m_strMsg.c_str(), pszMsg, SDGetTickCount() - m_dwStartCycle);
    }
    else
    {
        DBG_INFO(_SDT("[%s: %d]: PastCycle[%s] [%d]:[%u]"), MSG_MARK, m_strMsg.c_str(), m_byIdx++, GetCycleCount() - m_dwStartCycle);
        //DBG_INFO(_SDT("[%s: %d]: PastCycle[%s] [%d]:[%u]"), MSG_MARK, m_strMsg.c_str(), m_byIdx++, SDGetTickCount() - m_dwStartCycle);
    }
}

/*整数相除,商向上取整,有余就商就+1
iDividend为被除数
iDivisor为除数
*/
INT64 DivCeil(INT64 lDividend, INT64 lDivisor)
{
    if(lDivisor == 0)
    {
        return 0;
    }
    if((lDividend % lDivisor) > 0)
    {
        return INT64 (lDividend / lDivisor + 1);
    }
    else
    {
        return INT64 (lDividend / lDivisor);
    }
}

/*整数相除,商向下取整,有余忽略*/
INT64 DivFloor(INT64 lDividend, INT64 lDivisor)
{
    if(lDividend == 0)
    {
        return 0;
    }
    return (lDividend / lDivisor);
}

/*********************************************************************************************
	*函   数   名：Time_ToString
	*功        能：获得时间
	*输        入：
	*
	*输        出：2010-09-25 09:17:14
	*返        回：
	*修 改  记 录：NONE
*********************************************************************************************/

tstring Time_ToString()
{
    time_t now; //实例化time_t结构
    time(&now); //time函数读取现在的时间(
    struct tm *timenow; //实例化tm结构指针
    timenow = localtime(&now); //localtime函数把从time取得的时间now换算成电脑中的时间
    char btime[256] = {0};
    strftime(btime, 256, "%Y-%m-%d %H:%M:%S", timenow);
    tstring v_time = btime;
    return v_time;
}


/*********************************************************************************************
	*函   数   名：Time_ToString
	*功        能：获得时间
	*输        入：
	*
	*输        出：2010-09-25 09:17:14
	*返        回：
	*修 改  记 录：NONE
*********************************************************************************************/

tstring Time_ToString(time_t now)
{
    struct tm *timenow; //实例化tm结构指针
    timenow = localtime(&now); //localtime函数把从time取得的时间now换算成电脑中的时间
    char btime[256] = {0};
    strftime(btime, 256, "%Y-%m-%d %H:%M:%S", timenow);
    tstring v_time = btime;
    return v_time;
}


VOID SetLocateCharSet()
{
#ifndef __linux__
    setlocale(LC_ALL, "GBK");
#else
    setlocale(LC_ALL, "zh_CN.UTF-8");
#endif
}

UINT32 Fen2Mao( UINT32 dwNum )
{
    return dwNum / 10;
}

/*********************************************************************************************
 *函   数   名：十六进制字符串转ASCII
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：
 *修 改  记 录：NONE
 *********************************************************************************************/
int HexToAscii(unsigned char *to, unsigned char *from, unsigned int len)
{
    if (len % 2 == 1)
    {
        return -1;
    }
    unsigned char hb;
    unsigned char lb;
    unsigned int i = 0;
    for (i = 0; i < len; i += 2)
    {
        if (from[i] >= '0' && from[i] <= '9')
        {
            hb = from[i] - '0';
        }
        else if (from[i] >= 'A' && from[i] <= 'F')
            hb = from[i] + 10 - 'A';
        else if (from[i] >= 'a' && from[i] <= 'f')
            hb = from[i] - 'a' + 10;
        else
            return -1;
        if (from[i + 1] >= '0' && from[i + 1] <= '9')
            lb = from[i + 1] - '0';
        else if (from[i + 1] >= 'A' && from[i + 1] <= 'F')
            lb = from[i + 1] + 10 - 'A';
        else if (from[i + 1] >= 'a' && from[i + 1] <= 'f')
            lb = from[i + 1] + 10 - 'a';
        else
            return -1;
        to[i / 2] = (hb << 4) | lb;
    }
    return 0;
}

/*********************************************************************************************
 *函   数   名：ASCII字符串转十六进制字符串
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：
 *修 改  记 录：NONE
 *********************************************************************************************/
int AsciiToHex(unsigned char *to, unsigned char *from, unsigned int len)
{
    unsigned char a[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                            'a', 'b', 'c', 'd', 'e', 'f'
                          };
    unsigned int i = 0;
    for (i = 0; i < len; i++)
    {
        to[i * 2] = a[(from[i] & 0xf0) >> 4];
        to[i * 2 + 1] = a[(from[i] & 0x0f)];
    }
    return 0;
}

INT32 Random(INT32 nStart, INT32 nEnd)
{
	static UINT32 unSeed = 0;
    srand((UINT32)(time(NULL) + rand() + unSeed++)); //初始化随机数的种子
	if ((nEnd - nStart + 1) < 0) {
		return nEnd;
	}
    return nStart + rand() % (nEnd - nStart + 1);
}

bool IsMobile(TCHAR* pszMobile)
{
#define MOBILE_LEN 11
    static string strMobile[] = {"133", "153", "180", "181", "189", "130", "131", "132", "145", "155", "156", "185", "186", "134", "135", "136", "137", "138", "139", "147", "150", "151", "152", "157", "158", "159", "182", "183", "184", "187", "188"};

    //检测手机号长度
    if(MOBILE_LEN != strlen(pszMobile))
    {
        return false;
    }

    //检测是不是是数字
    for(int i = 0; i != strlen(pszMobile); i++)
    {
        if(!isdigit(pszMobile[i]))
        {
            return false;
        }
    }

    //比较发上来的数据的前三位是否属于号码段
    for (int j = 0; j < sizeof(strMobile) / sizeof(string); j++)
    {
        if(0 == strncmp(strMobile[j].c_str(), pszMobile, 3))
        {
            return true;
        }
    }
    return false;
}


UINT32 GetUTF8CharWidth( const TCHAR* str )
{
	if(!str)
		return 0;

	UINT8 code;
	INT32 bytes  = 1;
	INT32 cCount = 0;
	INT32 len = SDStrlen(str);

	INT32 i = 0;

	while (len > 0)
	{
		code = (UINT8)str[i];

		if(code >= 0xFC)
		{
			bytes = 6;
			cCount += 2;
		}
		else if(code >= 0xF8)
		{
			bytes = 5;
			cCount += 2;
		}
		else if (code >= 0xF0)
		{
			bytes = 4;
			cCount += 2;
		}
		else if(code >= 0xE0)
		{
			bytes = 3;
			cCount += 2;
		}
		else if(code >= 0xC0)
		{
			bytes = 2;
			cCount += 2;
		}
		else
		{
			bytes = 1;
			cCount += 1;
		}

		len -= bytes;
		i   += bytes;
	}

	return cCount;

}


BOOL ChkUTF8Char(  TCHAR* str )
{
	if(!str)
		return FALSE;

	UINT8 code;
	INT32 bytes  = 1;
	INT32 len = SDStrlen(str);
	INT32 i = 0;
	while (len > 0)
	{
		code = (UINT8)str[i];

		if(code >= 0xFC)
		{
			bytes = 6;
			return FALSE;
		}
		else if(code >= 0xF8)
		{
			bytes = 5;
			return FALSE;
		}
		else if (code >= 0xF0)
		{
			bytes = 4;
			return FALSE;
		}
		else if(code >= 0xE0)
		{
			bytes = 3;
		}
		else if(code >= 0xC0)
		{
			bytes = 2;
		}
		else
		{
			bytes = 1;

		}

		len -= bytes;
		i   += bytes;
	}
	return TRUE;
}

BOOL  CkEmoji(TCHAR* str)
{
	if(!str)
		return FALSE;
	string strTarget;

	UINT8 code;
	INT32 bytes  = 1;
	INT32 len = SDStrlen(str);
	INT32 i = 0;
	while (len > 0)
	{
		code = (UINT8)str[i];

		if(code >= 0xFC)
		{
			bytes = 6;
		}
		else if(code >= 0xF8)
		{
			bytes = 5;
		}
		else if (code >= 0xF0)
		{
			bytes = 4;
		}
		else if(code >= 0xE0)
		{
			bytes = 3;
			strTarget.append(&str[i], 3);
		}
		else if(code >= 0xC0)
		{
			bytes = 2;
			strTarget.append(&str[i], 2);
		}
		else
		{
			bytes = 1;
			strTarget.append(&str[i], 1);
		}

		len -= bytes;
		i   += bytes;
	}
	memset(str, 0, SDStrlen(str));
	SDStrncpy(str, strTarget.c_str(), strTarget.size());
	return TRUE;
}

BOOL IsUTF8(TCHAR* pBuffer)  
{  
	UINT32 dwSize = SDStrlen(pBuffer);
	BOOL bIsUTF8 = TRUE;  
	TCHAR* start = (TCHAR*)pBuffer;
	TCHAR* end = (TCHAR*)pBuffer + dwSize;  
	while (start < end)  
	{  
		if (*start < 0x80) // (10000000): 值小于0x80的为ASCII字符  
		{  
			start++;  
		}  
		else if (*start < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符  
		{  
			bIsUTF8 = FALSE;  
			break;  
		}  
		else if (*start < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符  
		{  
			if (start >= end - 1)   
				break;  
			if ((start[1] & (0xC0)) != 0x80)  
			{  
				bIsUTF8 = FALSE;  
				break;  
			}  
			start += 2;  
		}   
		else if (*start < (0xF0)) // (1111 0000): 此范围内为3字节UTF-8字符  
		{  
			if (start >= end - 2)   
				break;  
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)  
			{  
				bIsUTF8 = FALSE;  
				break;  
			}  
			start += 3;  
		}   
		else if (*start < (0xF8))//(1111 1000):此范围内为4个字节UTF-8字符
		{
			if ( start >= end - 3)
			{
				break;
			}
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80 
				|| (start[3] & (0xC0)) != 0x80)  
			{  
				bIsUTF8 = FALSE;  
				break;  
			}
			start += 4;
		}
		else if (*start < (0xFC))//(1111 1100):此范围内为5个字节UTF-8字符
		{
			if ( start >= end - 4)
			{
				break;
			}
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80 
				|| (start[3] & (0xC0)) != 0x80 || (start[4] & (0xC0)) != 0x80)  
			{  
				bIsUTF8 = FALSE;  
				break;  
			}
			start += 5;
		}
		else if (*start < (0xFE))//(1111 1110):此范围内为6个字节UTF-8字符
		{
			if ( start >= end - 5)
			{
				break;
			}
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80 
				|| (start[3] & (0xC0)) != 0x80 || (start[4] & (0xC0)) != 0x80
				|| (start[5] & (0xC0)) != 0x80)  
			{  
				bIsUTF8 = FALSE;  
				break;  
			}
			start += 6;
		}
		else
		{  
			bIsUTF8 = FALSE;  
			break; 
		}  
	}  
	return bIsUTF8;  
}

string ToLowerByString(const TCHAR* aszString)
{
    string strLower = aszString;
    string::iterator itr = strLower.begin();
    for ( ; itr != strLower.end(); itr++)
    {
        *itr = SDToLower(*itr);
    }
    return strLower;
}

//编码
BOOL UrlEncode(const string& strSrc, string& strOut)
{
	const char* str = strSrc.c_str();
	const int strSize = strSrc.length();
	strOut.clear();
	//char* result, const int resultSize
	int i;
	char ch;
	char strTmp[56] = {0};
	if ( strSize <= 0 ) {
		return FALSE;
	}

	for ( i = 0;  i < strSize; ++i) {
		ch = str[i];
		if (((ch>='A') && (ch<'Z')) ||
			((ch>='a') && (ch<'z')) ||
			((ch>='0') && (ch<'9'))) {
				strOut.push_back(ch);
		} else if (ch == ' ') {
			strOut.push_back('+');
		} else if (ch == '.' || ch == '-' || ch == '_' || ch == '*') {
			strOut.push_back(ch);
		} else {
			sprintf(strTmp, "%%%02X", (unsigned char)ch);
			strOut += strTmp;
		}
	}
	return TRUE;
}

//解码
BOOL UrlDecode(string& str)
{
	string strTmp = str;
	UINT32 dwLen = str.length();
	const char *data = strTmp.c_str();
	str.clear();

	UINT32 value;
	UINT32 c;

	while (dwLen--) {
		if (*data == '+') 
		{
			str.push_back(' ');
		}
		else if (*data == '%' && dwLen >= 2 && isxdigit((UINT32) *(data + 1)) && isxdigit((UINT32) *(data + 2)))
		{

			c = ((unsigned char *)(data+1))[0];
			if (isupper(c))
				c = tolower(c);
			value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;
			c = ((unsigned char *)(data+1))[1];
			if (isupper(c))
				c = tolower(c);
			value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

			str.push_back((char)value) ;
			data += 2;
			dwLen -= 2;
		} 
		else 
		{
			str.push_back(*data);
		}
		data++;
	}
	return TRUE;
}

string GetRsponeResult(UINT16 _errno_)
{
    CNGString strMsg = "";
    strMsg = "{\"errno\":";
    strMsg += _errno_;
    strMsg += ",\"desc\":\"";
    strMsg += SDGBK2UTF8(MG_NET_ERRMSG(ERR_GM_PARM_INFO, _errno_));
    strMsg += "\"";
    strMsg += "}";
    return strMsg.str();
}


string GetRsponeResult( UINT16 _errno_, map<string, string> *pstrMap)
{
    CNGString strMsg = "";
    strMsg = "{\"errno\":";
    strMsg += _errno_;
    strMsg += ",\"desc\":\"";
    strMsg += SDGBK2UTF8(MG_NET_ERRMSG(ERR_GM_PARM_INFO, _errno_));
    strMsg += "\"";
    if (NULL != pstrMap && pstrMap->size() > 0)
    {
        strMsg += ",\"datas\":{";
        map<string, string>::iterator itr;
        for (itr = pstrMap->begin(); itr != pstrMap->end(); itr++)
        {
            if (itr != pstrMap->begin())
            {
                strMsg += ",";
            }
            strMsg += "\"";
            strMsg += itr->first;
            strMsg += "\":\"";
            strMsg += itr->second;
            strMsg += "\"";
        }
        strMsg += "}";
    }
    strMsg += "}";
    return strMsg.str();
}

string GetRsponeResult(UINT16 _errno_, map<string, UINT32> *pstrMap)
{
	CNGString strMsg = "";
	strMsg = "{\"errno\":";
	strMsg += _errno_;
	strMsg += ",\"desc\":\"";
	strMsg += SDGBK2UTF8(MG_NET_ERRMSG(ERR_GM_PARM_INFO, _errno_));
	strMsg += "\"";
	if (NULL != pstrMap && pstrMap->size() > 0)
	{
		strMsg += ",\"datas\":{";
		map<string, UINT32>::iterator itr;
		for (itr = pstrMap->begin(); itr != pstrMap->end(); itr++)
		{
			if (itr != pstrMap->begin())
			{
				strMsg += ",";
			}
			strMsg += "\"";
			strMsg += itr->first;
			strMsg += "\":";
			strMsg += itr->second;
		}
		strMsg += "}";
	}
	strMsg += "}";
	return strMsg.str();
}

string GetRsponeResult(UINT16 _errno_, map<string, UINT64> *pstrMap)
{
	CNGString strMsg = "";
	strMsg = "{\"errno\":";
	strMsg += _errno_;
	strMsg += ",\"desc\":\"";
	strMsg += SDGBK2UTF8(MG_NET_ERRMSG(ERR_GM_PARM_INFO, _errno_));
	strMsg += "\"";
	if (NULL != pstrMap && pstrMap->size() > 0)
	{
		strMsg += ",\"datas\":{";
		map<string, UINT64>::iterator itr;
		for (itr = pstrMap->begin(); itr != pstrMap->end(); itr++)
		{
			if (itr != pstrMap->begin())
			{
				strMsg += ",";
			}
			strMsg += "\"";
			strMsg += itr->first;
			strMsg += "\":";
			strMsg += itr->second;
		}
		strMsg += "}";
	}
	strMsg += "}";
	return strMsg.str();
}

string GetRsponeResult(UINT16 _errno_, vector<string> *pstrVec)
{
	CNGString strMsg = "";
	strMsg += "{\"errno\":";
	strMsg += _errno_;
	strMsg += ",\"desc\":\"";
	strMsg += SDGBK2UTF8(MG_NET_ERRMSG(ERR_GM_PARM_INFO, _errno_));
	strMsg += "\"";
	if (NULL != pstrVec && pstrVec->size() > 0)
	{
		strMsg += ",\"datas\":[";
		vector<string>::iterator itr;
		for (itr = pstrVec->begin(); itr != pstrVec->end(); itr++)
		{
			if (itr != pstrVec->begin())
			{
				strMsg += ",";
			}
			strMsg += *itr;
		}
		strMsg +=  + "]";
	}
	strMsg +=  "}";
	return strMsg.str();
}

//检测是否有特殊字符，且长度合法(昵称允许6个中文字符，12个英文)
UINT16 CkHaveSpeCharacterAndValidLen(TCHAR* pszDspName)
{
	wstring strName = SDUTF82W(pszDspName);
	UINT16 wLen = strName.length();
	WCHAR *lpwSz = SDNew WCHAR[wLen + 1];
	SDWcscpy(lpwSz, strName.c_str());
	UINT16 wErrCode = 0;
	UINT16 wIdx = 0;
	UINT8 byLogicLen = 0;
	while (wIdx < wLen)
	{
		UINT16 wTmp = lpwSz[wIdx++];

		if((wTmp >= '1') && (wTmp <= '9'))
		{
			byLogicLen++;
			continue;
		}
		else if((wTmp >= 'a') && (wTmp <= 'z'))
		{
			byLogicLen++;
			continue;
		}
		else if((wTmp >= 'A') && (wTmp <= 'Z'))
		{
			byLogicLen++;
			continue;
		}
		else if((wTmp == '.') || (wTmp == '@') || (wTmp == '_'))
		{
			byLogicLen++;
			continue;
		}
		else if(wTmp == 0x3002) //句号
		{
			byLogicLen += 2;
			continue;
		}
		else if((wTmp >= 0x4e00)  && (wTmp <= 0x9fa5 ))
		{
			byLogicLen += 2;
			continue;
		}

		wErrCode = ERR_REGISTER_DSPNAME::ID_INVALID_CHARACTER;
		break;
	}
	SDDelete []lpwSz;
	lpwSz = NULL;

	if(byLogicLen > 12)
	{
		wErrCode = ERR_REGISTER_DSPNAME::ID_TOO_LONG;
	}

	return wErrCode;
}


	CGenerateJson::CGenerateJson()
	{
		m_dwGold = 0;
		m_dwCoin = 0;
		m_dwPhystrength = 0;
		m_dwScience = 0;
		m_dwStory = 0;
		m_dwBluegas = 0;
		m_dwPruplegas = 0;
		m_dwJingjie = 0;
		m_dwDoorstribute = 0;
		m_dwFactionFunds = 0;
		m_mapItem.clear();
		m_dwStudyExp = 0;
	}
	CGenerateJson::~CGenerateJson()
	{
		m_dwGold = 0;
		m_dwCoin = 0;
		m_dwPhystrength = 0;
		m_dwScience = 0;
		m_dwStory = 0;
		m_dwBluegas = 0;
		m_dwPruplegas = 0;
		m_dwJingjie = 0;
		m_dwDoorstribute = 0;
		m_dwFactionFunds = 0;
		m_mapItem.clear();
		m_dwStudyExp = 0;
	}
	string CGenerateJson::GetJson()
	{
		string strJson;
		TCHAR aszBuf[255] = {0};
		strJson += "{";
		if ( 0 < m_mapItem.size() )
		{
			strJson += "\"item\": [";
			for (map<UINT32, UINT32>::iterator itr = m_mapItem.begin(); itr != m_mapItem.end(); itr++ )
			{
				if ( itr != m_mapItem.begin())
				{
					strJson += ","; 
				}
				SDSprintf(aszBuf, "{\"value\": %u,\"num\": %u,\"percent\": 100}", itr->first, itr->second);
				strJson += aszBuf;
			}
			strJson += "],";
		}

		if (m_vSoulData.size() > 0) {
			strJson += "\"soul\": [";
			for (size_t n = 0; n < m_vSoulData.size(); ++ n) {
				if (n > 0) {
					strJson += ","; 
				}
				SDSprintf(aszBuf, "{\"value\": %u,\"num\": %u,\"percent\": 100}", m_vSoulData[n].usSoulId, m_vSoulData[n].usSoulNum);
				strJson += aszBuf;
			}
			strJson += "],";
		}

		SDSprintf(aszBuf, "\"coin\": {\"value\": %u,\"num\": 1,\"percent\": 100},",m_dwCoin);
		strJson += aszBuf;
		SDSprintf(aszBuf, "\"gold\": {\"value\": %u,\"num\": 1,\"percent\": 100},",m_dwGold);
		strJson += aszBuf;
		SDSprintf(aszBuf, "\"phystrength\": {\"value\": %u,\"num\": 1,\"percent\": 100},",m_dwPhystrength);
		strJson += aszBuf;
		SDSprintf(aszBuf, "\"science\": {\"value\": %u,\"num\": 1,\"percent\": 100},",m_dwScience);
		strJson += aszBuf;
		SDSprintf(aszBuf, "\"story\": {\"value\": %u,\"num\": 1,\"percent\": 100},",m_dwStory);
		strJson += aszBuf;
		SDSprintf(aszBuf, "\"bluegas\": {\"value\": %u,\"num\": 1,\"percent\": 100},",m_dwBluegas);
		strJson += aszBuf;
		SDSprintf(aszBuf, "\"pruplegas\": {\"value\": %u,\"num\": 1,\"percent\": 100},",m_dwPruplegas);
		strJson += aszBuf;
		SDSprintf(aszBuf, "\"jingjie\": {\"value\": %u,\"num\": 1,\"percent\": 100},",m_dwJingjie);
		strJson += aszBuf;
		SDSprintf(aszBuf, "\"doorstribute\": {\"value\": %u,\"num\": 1,\"percent\": 100},",m_dwDoorstribute);
		strJson += aszBuf;
		SDSprintf(aszBuf, "\"factionfunds\": {\"value\": %u,\"num\": 1,\"percent\": 100},",m_dwFactionFunds);
		strJson += aszBuf;
		SDSprintf(aszBuf, "\"studyexp\": {\"value\": %u,\"num\": 1,\"percent\": 100}",m_dwStudyExp);
		strJson += aszBuf;
		strJson += "}";
		return strJson;
	}
	VOID CGenerateJson::SetGold(UINT32 dwValue)
	{
		m_dwGold = dwValue;
	}
	VOID CGenerateJson::SetCoin(UINT32 dwValue)
	{
		m_dwCoin = dwValue;
	}
	VOID CGenerateJson::SetPhystrength(UINT32 dwValue)
	{
		m_dwPhystrength = dwValue;
	}
	VOID CGenerateJson::SetStory(UINT32 dwValue)
	{
		m_dwStory = dwValue;
	}
	VOID CGenerateJson::SetScience(UINT32 dwValue)
	{
		m_dwScience = dwValue;
	}
	VOID CGenerateJson::SetJingJie(UINT32 dwValue)
	{
		m_dwJingjie = dwValue;
	}
	VOID CGenerateJson::SetDoorsTribute(UINT32 dwValue)
	{
		m_dwDoorstribute = dwValue;
	}
	VOID CGenerateJson::SetBlueGas(UINT32 dwValue)
	{
		m_dwBluegas = dwValue;
	}
	VOID CGenerateJson::SetFactionFunds(UINT32 dwValue)
	{
		m_dwFactionFunds = dwValue;
	}
	VOID CGenerateJson::SetPurpleGas(UINT32 dwValue)
	{
		m_dwPruplegas = dwValue;
	}
	VOID CGenerateJson::AddItem(UINT16 wKindID, UINT32 dwNum)
	{
		m_mapItem[wKindID] = dwNum;
	}
	VOID CGenerateJson::SetStudyExp(UINT32 dwValue)
	{
		m_dwStudyExp = dwValue;
	}
	VOID CGenerateJson::SetResByGm(string strKey, string strValue, vector<string>& rParam)
	{
		if ( strKey == "bluegas")
		{
			SetBlueGas(SDAtou(strValue.c_str()));
		}
		else if ( strKey == "coin")
		{
			SetCoin(SDAtou(strValue.c_str()));
		}
		else if ( strKey == "gold")
		{
			SetGold(SDAtou(strValue.c_str()));
		}
		else if ( strKey == "jingjie" )
		{
			SetJingJie(SDAtou(strValue.c_str()));
		}
		else if ( strKey == "phystrength")
		{
			SetPhystrength(SDAtou(strValue.c_str()));
		}
		else if ( strKey == "purplegas")
		{
			SetPurpleGas(SDAtou(strValue.c_str()));
		}
		else if ( strKey == "science")
		{
			SetScience(SDAtou(strValue.c_str()));
		}
		else if ( strKey == "story")
		{
			SetStory(SDAtou(strValue.c_str()));
		}
		else if ( strKey == "doorstribute")
		{
			SetDoorsTribute(SDAtou(strValue.c_str()));
		}
		else if ( strKey == "factionfunds")
		{
			SetFactionFunds(SDAtou(strValue.c_str()));
		}
		else if (strKey == "soul") 
		{
			if (rParam.size() != 3) {
				return;
			}
			SoulData soulData;
			soulData.usSoulId = SDAtou(rParam[1].c_str());
			soulData.usSoulNum = SDAtou(rParam[2].c_str());
			m_vSoulData.push_back(soulData);
		}
		else if ( strKey == "studyexp" )
		{
			SetStudyExp(SDAtou(strValue.c_str()));
		}
		else 
		{
			AddItem(SDAtou(strKey.c_str()), SDAtou(strValue.c_str()));
		}
	}