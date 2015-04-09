#include "sddir.h"
#include "sdfile.h"
#include "sdstring.h"

#if (defined(WIN32) || defined(WIN64))
#include <direct.h>
#include <errno.h>
#pragma warning(disable : 4996)
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace SGDP
{

CSDDirectory::CSDDirectory()
{
#if (defined(WIN32) || defined(WIN64))
    m_szPath    = _SDT("");
    m_fHandle   = INVALID_HANDLE_VALUE;
#else
    m_dir       = NULL;
    memset(&m_curDir, 0, sizeof(m_curDir));
#endif

}

CSDDirectory::~CSDDirectory()
{

}

BOOL CSDDirectory::Open( const TCHAR* path )
{
#if (defined(WIN32) || defined(WIN64))
    m_szPath = path;
    m_szPath += _SDT("\\*");
    m_fHandle = INVALID_HANDLE_VALUE;
    CHAR szOldPathDir[1024] = {0};
    getcwd(szOldPathDir, 1024);
    std::string strPath = _SDTT2A(path);
    if(0 != chdir(strPath.c_str()))
    {
        return FALSE;
    }
    chdir(szOldPathDir);
#else
    SDStrncpy(m_curDir, _SDTT2A(path), sizeof(m_curDir));
    m_dir = opendir(_SDTT2A(path));
    if (m_dir == NULL)
    {
        return false;
    }
#endif

    return TRUE;
}

void CSDDirectory::Close()
{
#if (defined(WIN32) || defined(WIN64))
    if (m_fHandle != INVALID_HANDLE_VALUE)
    {
        FindClose(m_fHandle);
        m_fHandle = INVALID_HANDLE_VALUE;
    }
#else
    if (m_dir != NULL)
    {
        closedir(m_dir);
        m_dir = NULL;
    }
#endif
}

BOOL CSDDirectory::Read( TCHAR* name, INT32 buflen, SFileAttr* pAttr)
{
#if (defined(WIN32) || defined(WIN64))
    WIN32_FIND_DATAA find_data;
    if(m_fHandle == INVALID_HANDLE_VALUE)
    {
        m_fHandle = FindFirstFileA(_SDTT2A(m_szPath.c_str()), &find_data);
        if (m_fHandle == INVALID_HANDLE_VALUE)
        {
            return FALSE;
        }
    }
    else
    {
        if (FindNextFileA(m_fHandle, &find_data) == 0)
        {
            return FALSE;
        }
    }

    if (strcmp(find_data.cFileName, ".") == 0 ||
            strcmp(find_data.cFileName, "..") == 0 )
    {
        return Read(name, buflen, pAttr);
    }

    _sntprintf(name, buflen, _SDT("%s"), _SDTLocal2T(find_data.cFileName));

    if (pAttr != NULL)
    {
        pAttr->isDir = (find_data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
    }

#else
    if (m_dir == NULL)
    {
        return FALSE;
    }

    struct dirent* pdir = readdir(m_dir);
    if (pdir == NULL)
    {
        return FALSE;
    }

    if (strcmp(pdir->d_name, ".") == 0 ||
            strcmp(pdir->d_name, "..") == 0 )
    {
        return Read(name, buflen, pAttr);
    }

    _SDTsnprintf(name, buflen, _SDT("%s"), _SDTA2T(pdir->d_name));

    if (pAttr != NULL )
    {
        CHAR szFullPath[SD_MAX_PATH];
        snprintf(szFullPath, buflen, ("%s/%s"), m_curDir, (pdir->d_name));

        struct stat statbuf;
        if (lstat(szFullPath, &statbuf) < 0 )
        {
            return FALSE;
        }

        pAttr->isDir = S_ISDIR(statbuf.st_mode);
    }

#endif

    return TRUE;
}

BOOL CSDDirectory::ReadAllFile(vector<string>& vecFile)
{
#if (defined(WIN32) || defined(WIN64))
	WIN32_FIND_DATAA find_data;
	if(m_fHandle == INVALID_HANDLE_VALUE)
	{
		m_fHandle = FindFirstFileA(_SDTT2A(m_szPath.c_str()), &find_data);
		if (m_fHandle == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}
	}

	while(FindNextFileA(m_fHandle, &find_data))
	{
		if (strcmp(find_data.cFileName, ".") == 0 ||
			strcmp(find_data.cFileName, "..") == 0 )
		{
			continue;
		}

		vecFile.push_back(find_data.cFileName);
	}

#else
	if (m_dir == NULL)
	{
		return FALSE;
	}


	struct dirent* pdir = NULL;
	while( (pdir = readdir(m_dir)) != NULL )
	{
		if (strcmp(pdir->d_name, ".") == 0 ||
			strcmp(pdir->d_name, "..") == 0 )
		{
			continue;
		}

		vecFile.push_back(pdir->d_name);
	}

	


#endif
	return TRUE;
}

BOOL  SDCreateDirectory(const TCHAR *dirName, BOOL bforce)
{

#if (defined(WIN32) || defined(WIN64))
    //Nonzero if successful; otherwise 0
    int ret = CreateDirectory(dirName, NULL);
    if(0 != ret)
    {
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }
#else
    //returns zero on success, or -1 if an error occurred
    int ret = mkdir(_SDTT2A(dirName), S_IREAD | S_IWRITE);
    if(0 == ret)
    {
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }
#endif
    if (!bforce)
    {
        return ret;
    }

    if (FALSE == ret)
    {
        if(bforce)
        {
            TCHAR path[SD_MAX_PATH];
            TCHAR * pos = path;
            _SDTStrncpy(path, dirName, sizeof(path));
            TCHAR * end = path + _SDTStrlen(path);
            while(*pos != _SDT('\0'))
            {
				//对于linux而言，如果“/XXX/XXX”,不能直接过虑掉，要特殊处理才行
                if(SDPATH_IS_DELIMITER(*pos))
                {
					//过虑掉第一的位置
					if (pos !=  path)
					{
						*pos = _SDT('\0');
					}
					else
					{
						*pos = SDPATH_DELIMITER;
					}
                }
                ++pos;
            }
            pos = path;
            bool bfirst = true;
            while(pos < end)
            {
                //====检测第一段路径是否为盘符====
                if(bfirst)
                {
                    bool isdrive = false;
                    TCHAR * p = pos;
                    while(*p != _SDT('\0'))
                    {
                        if(*p == _SDT(':'))
                        {
                            isdrive = true;
                        }
                        ++p;
                    }
                    if(isdrive)
                    {
                        pos = path + _SDTStrlen(path);
                        *pos = SDPATH_DELIMITER;
                        bfirst = false;
                        continue;
                    }
                    bfirst = false;
                }
                //====检测第一段路径是否为盘符完毕====

                //====如果此段路径不存在，创建目录====
                if(!SDDirectoryExists(path))
                {
#if (defined(WIN32) || defined(WIN64))
                    if(0 != _mkdir(_SDTT2A(path)))
#else
                    if(0 != mkdir(_SDTT2A(path), S_IREAD | S_IWRITE))
#endif
                    {
                        return FALSE;
                    }
                }
                pos = path + _SDTStrlen(path);
                *pos = SDPATH_DELIMITER;
            }
        }
        else//bforce == false
        {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL  SDDeleteDirectory( const TCHAR *dirName, BOOL bforce/* = false*/ )
{
    if(NULL == dirName)
    {
        return FALSE;
    }
    if(!bforce)
    {
#if (defined(WIN32) || defined(WIN64))
        return _rmdir(_SDTT2A(dirName)) == 0;
#else
        return rmdir(_SDTT2A(dirName)) == 0;
#endif
    }
    else
    {
        CSDDirectory dir;
        if(dir.Open(dirName))
        {
            TCHAR _filename[SD_MAX_PATH];
            SFileAttr attr = {0};
            while(dir.Read(_filename, SD_MAX_PATH, &attr))
            {
                TCHAR _subfilepath[SD_MAX_PATH];
                _SDTStrncpy(_subfilepath, dirName, SD_MAX_PATH);
                _SDTStrcat(_subfilepath, _SDT("\\") );
                _SDTStrcat(_subfilepath, _filename );
                if(attr.isDir)
                {

                    if(!SDDeleteDirectory(_subfilepath, bforce))
                    {
                        return FALSE;
                    }
                }
                else
                {
#if (defined(WIN32) || defined(WIN64))
                    if(_unlink(_SDTT2A(_subfilepath)) != 0)
#else
                    if(unlink(_SDTT2A(_subfilepath)) != 0)
#endif
                    {
                        return FALSE;
                    }
                }
            }
            dir.Close();
#if (defined(WIN32) || defined(WIN64))
            return _rmdir(_SDTT2A(dirName)) == 0;
#else
            return rmdir(_SDTT2A(dirName)) == 0;
#endif
        }
        return FALSE;
    }
}

BOOL  SDDirectoryExists(const TCHAR *dirName)
{
#if (defined(WIN32) || defined(WIN64))
    int nCode = GetFileAttributesA(_SDTT2Local(dirName));
    if (nCode == -1)
    {
        return false;
    }
    return FILE_ATTRIBUTE_DIRECTORY & nCode;
#else
    struct stat fileStat;
    int ret = stat(_SDTT2A(dirName), &fileStat);
    if ( ret == 0)
    {
        return S_ISDIR(fileStat.st_mode);
    }
    return FALSE;
#endif
}

BOOL  SDIsDirectory(const TCHAR * dirName)
{
#if (defined(WIN32) || defined(WIN64))
    int nCode = GetFileAttributesA(_SDTT2A(dirName));
    if (nCode == -1)
    {
        return false;
    }
    return FILE_ATTRIBUTE_DIRECTORY & nCode;
#else
    struct stat fileStat;
    int ret = stat(_SDTT2A(dirName), &fileStat);
    if ( ret == 0)
    {
        return S_ISDIR(fileStat.st_mode);
    }
    return FALSE;
#endif
}

BOOL  SDIsEmptyDirectory(const TCHAR* dirName)
{
    if (SDIsDirectory(dirName))
    {
#if (defined(WIN32) || defined(WIN64))
        HANDLE hFind = INVALID_HANDLE_VALUE;

        tstring dirFile = SDFileIncludeTrailingDelimiter(dirName);

        BOOL isEmpty = TRUE;
        dirFile += _SDT("*");
        WIN32_FIND_DATA find_data;
        hFind = FindFirstFile(dirFile.c_str(), &find_data);
        if (hFind)
        {
            while( FindNextFile(hFind, &find_data))
            {
                if (_tcscmp(find_data.cFileName, _SDT(".")) == 0 || _tcscmp(find_data.cFileName, _SDT("..")) == 0)
                {
                    continue;
                }
                isEmpty = FALSE;
                break;
            }

            FindClose(hFind);

            return isEmpty;
        }
#else
        DIR * pFd = opendir(_SDTT2A(dirName));
        if (pFd == NULL)
        {
            return FALSE;
        }

        struct dirent* pdir = readdir(pFd);
        if (pdir == NULL)
        {
            closedir(pFd);
            return FALSE;
        }

        while (pdir != NULL)
        {
            if (strcmp(pdir->d_name, ".") == 0 ||
                    strcmp(pdir->d_name, "..") == 0 )
            {
                pdir = readdir(pFd);
                continue;
            }
            else
            {
                closedir(pFd);
                return FALSE;
            }
        }

        closedir(pFd);
#endif // 
    }
    return TRUE;
}
} // namespace SGDP

