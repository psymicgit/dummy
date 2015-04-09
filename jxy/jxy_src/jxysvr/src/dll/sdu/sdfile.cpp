#include "sdtype.h"
#include "sdfile.h"
#include "sdstring.h"
#include "sddir.h"
#include "sdmath.h"
#include "sdfilemapping.h"
#include "sddebug.h"
#include <sys/stat.h>
#if (defined(WIN32) || defined(WIN64))
#include <direct.h>
#include <io.h>
#pragma warning(disable : 4996)
#else
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#endif

#define SDPATH_IS_DELIMITER(x)  (x == '\\' || x == '/')

namespace SGDP
{

    CSDFile::CSDFile()
        :m_pFileHandle(NULL), m_bFileMapping(FALSE)
    {
    }

    CSDFile::~CSDFile()
    {
        Close();
    }


    BOOL  CSDFile::Open(const TCHAR* szfilename, const TCHAR* type, BOOL bFileMapping)
    {
        m_filepath = szfilename;
        m_bFileMapping = bFileMapping;
        if (m_bFileMapping)
        {
            m_pFileHandle = SDNew SFileMapping();
            return SDFileMapping(*(SFileMapping*)m_pFileHandle,szfilename);
        }
        else
        {
            m_pFileHandle = fopen(_SDTT2A(szfilename), _SDTT2A(type));
        }
        return m_pFileHandle != NULL;
    }

    void  CSDFile::Close()
    {
        if (m_pFileHandle != NULL)
        {
            if (m_bFileMapping)
            {
                SDFileUnMapping(*(SFileMapping*)m_pFileHandle);
                delete (SFileMapping*)m_pFileHandle;
            }
            else
            {
                fclose((FILE*)m_pFileHandle);
            }
            m_pFileHandle = NULL;
        }
    }

    void  CSDFile::Flush()
    {
        if (m_pFileHandle != NULL)
        {
            if (m_bFileMapping)
            {
                SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
                SDFileFlushMapping(pFileMapping->mem, pFileMapping->size);
            }
            else
            {
                fflush((FILE*)m_pFileHandle);
            }
        }
    }

    UINT32  CSDFile::Read(void* pBuf, UINT32 nLen)
    {
        if (m_pFileHandle == NULL)
        {
            return 0;
        }

        if (m_bFileMapping)
        {
            SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
            UINT32 left = pFileMapping->size -  ( (CHAR*)pFileMapping->pos - (CHAR*)pFileMapping->mem);
            UINT32 readable =SDMin(left,nLen);
            memcpy(pBuf, pFileMapping->pos ,readable);
            pFileMapping->pos = (CHAR*) pFileMapping->pos +  readable;
            return readable;
        }
        else
        {
            return fread(pBuf, 1, nLen, (FILE*)m_pFileHandle);
        }

    }

    UINT32  CSDFile::Write(const void* pBuf, UINT32 nLen)
    {
        if (m_pFileHandle == NULL)
        {
            return false;
        }

        if (m_bFileMapping)
        {
            SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
            UINT32 left = pFileMapping->size -  ( (CHAR*)pFileMapping->pos - (CHAR*)pFileMapping->mem);
            UINT32 writeable =SDMin(left,nLen);
            memcpy(pFileMapping->pos,pBuf,writeable);
            pFileMapping->pos = (CHAR*) pFileMapping->pos +  writeable;
            return writeable;
        }
        else
        {
            return fwrite(pBuf, 1, nLen,(FILE*) m_pFileHandle);
        }
    }

    INT32  CSDFile::Seek( INT32 offset, CSDFile::SeekOffset whence)
    {
        if (m_pFileHandle == NULL)
        {
            return -1;
        }
        if (m_bFileMapping)
        {
            SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
            switch(whence)
            {
            case SK_SET:
                if ((UINT32)abs(offset) <= pFileMapping->size)
                {
                    pFileMapping->pos = (CHAR*)pFileMapping->mem +
                        (offset >0 ? offset : pFileMapping->size + offset);
                    return 0;
                }
                break;

            case SK_END:
                if ((UINT32)abs(offset) <= pFileMapping->size)
                {
                    pFileMapping->pos = (CHAR*)pFileMapping->mem +
                        (offset> 0 ?pFileMapping->size - offset: -offset);
                    return 0;
                }
                break;

            case SK_CUR:
                {
                    UINT32 curPos =  (CHAR*)pFileMapping->pos - (CHAR*)pFileMapping->mem;
                    if ( (offset >=0  && offset <= (INT32)(pFileMapping->size - curPos))
                        || (offset < 0 && offset + curPos >=0 ))
                    {
                        pFileMapping->pos = (CHAR*)pFileMapping->pos + offset;
                        return 0;
                    }
                }
                break;
            default:
                ;
            }

            return -1;
        }
        else
        {
            return fseek((FILE*)m_pFileHandle, offset, whence);
        }
    }

    INT32  CSDFile::SeekToBegin()
    {
        return this->Seek(0, SK_SET);
    }

    INT32  CSDFile::SeekToEnd()
    {
        return this->Seek(0, SK_END);
    }

    LONG  CSDFile::GetPosition()
    {
        if (m_pFileHandle == NULL)
        {
            return -1;
        }
        if (m_bFileMapping)
        {
            SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
            return (CHAR*)pFileMapping->pos - (CHAR*)pFileMapping->mem;
        }
        else
        {
            return ftell((FILE*)m_pFileHandle);
        }

    }

    void  CSDFile::SetLength(LONG newLen)
    {
        if (m_pFileHandle == NULL)
        {
            return;
        }
        if (m_bFileMapping)
        {
            SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
            SDFileUnMapping(*pFileMapping);
            SDFileMapping(*pFileMapping,m_filepath.c_str(),0,newLen);
        }
        else
        {

#if (defined(WIN32) || defined(WIN64))
            _chsize(_fileno((FILE*)m_pFileHandle), newLen);
#else
            ftruncate(fileno((FILE*)m_pFileHandle), newLen);
#endif
        }
    }

    BOOL  CSDFile::Eof()
    {
        if (m_pFileHandle == NULL)
        {
            return FALSE;
        }

        if (m_bFileMapping)
        {
            SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
            return pFileMapping->pos == ((CHAR*)pFileMapping->mem + pFileMapping->size);
        }
        else
        {
            LONG thispos = GetPosition();
            SeekToEnd();
            LONG endpos = GetPosition();
            if(thispos != endpos)
            {
                Seek(thispos, SK_SET);
                return FALSE;
            }
        }
        return TRUE;
    }

    BOOL  CSDFile::GetFileStatus(SFileStatus &fStatus)
    {
        if (m_pFileHandle == NULL)
        {
            return FALSE;
        }

        return  SDGetFileStatus(m_filepath.c_str(), fStatus);
    }

    BOOL  SDGetFileStatus( const TCHAR* szfilename, SFileStatus& fStatus )
    {
        if(NULL == szfilename)
        {
            return FALSE;
        }
#if (defined(WIN32) || defined(WIN64))
        struct _stat st;
        if (_tstat(szfilename, &st) != 0)
        {
            return FALSE;
        }
#else
        struct stat st;
        if (stat(_SDTT2A(szfilename), &st) < 0)
        {
            return FALSE;
        }
#endif

        fStatus.gid = st.st_gid;
        fStatus.atime = st.st_atime;
        fStatus.ctime = st.st_ctime;
        fStatus.dev = st.st_dev;
        fStatus.inode = st.st_ino;
        fStatus.mode = st.st_mode;
        fStatus.mtime = st.st_mtime;
        fStatus.nlink = st.st_nlink;
        fStatus.rdev = st.st_rdev;
        fStatus.size = st.st_size;
        fStatus.uid = st.st_uid;

        return TRUE;
    }

    BOOL  SDFileRemove( const TCHAR* szfilename )
    {
#if (defined(WIN32) || defined(WIN64))
        return _tunlink(szfilename) == 0;
#else
        return unlink(_SDTT2A(szfilename)) == 0;
#endif
    }

    INT32  SDFileRename( const TCHAR* oldname, const TCHAR* newname ,BOOL bForce)
    {
        if (bForce)
        {
            if (SDFileExists(newname))
            {
                SDFileRemove(newname);
            }
            return rename(_SDTT2A(oldname), _SDTT2A(newname));
        }
        else
        {
            return rename(_SDTT2A(oldname), _SDTT2A(newname));
        }
    }
    // 增加强制创建目标目录  是否覆盖目标文件 增加文档注释
    INT32  SDFileMove( const TCHAR* oldname, const TCHAR* newname ,BOOL bForce   )
    {
        return SDFileRename(oldname,newname,bForce);

        //TODO 实现文件跨目录拷贝,或移动.
        if ( SDIsDirectory(oldname))
        {
            UINT32 newNameLen = _SDTStrlen(newname);
            if (SDPATH_IS_DELIMITER(newname[newNameLen-1]))
            {

            }
            return SDFileRename(oldname,newname,bForce);
        }
        else
        {
            return SDFileRename(oldname,newname,bForce);
        }
    }

    INT32  SDFileAccess( const TCHAR *path, AccessMode mode )
    {
        if(NULL == path)
        {
            return -1;
        }
#if (defined(WIN32) || defined(WIN64))
        return _taccess(path, mode);
#else
        return access(_SDTT2A(path), mode);
#endif
    }


    const TCHAR*  SDGetModulePath()
    {
        static TCHAR szPath[SD_MAX_PATH];

        static bool bFirstTime = true;

        if (bFirstTime)
        {
#if (defined(WIN32) || defined(WIN64))
            bFirstTime = false;
            GetModuleFileName(NULL, (TCHAR*)szPath, sizeof(szPath));
            TCHAR *p = _tcsrchr(szPath, _SDT('\\'));
            *p = _SDT('\0');
            _tcscat(szPath, _SDT("\\"));
#else
            CHAR szTmp[SD_MAX_PATH];
            getcwd(szTmp,sizeof(szTmp));
            CHAR szTempPath[SD_MAX_PATH];
            sprintf(szTempPath,"%s/",szTmp);
            _SDTStrncpy(szPath,_SDTA2T(szTempPath),SD_MAX_PATH);
#endif //
        }
        return szPath;
    }

    const TCHAR*  SDGetModuleName()
    {
#if (defined(WIN32) || defined(WIN64))
        static TCHAR szPath[MAX_PATH];
        static bool bFirstTime = true;

        if(bFirstTime)
        {
            bFirstTime = false;
            GetModuleFileName(NULL, (TCHAR*)szPath, sizeof(szPath));
        }

        return szPath;
#else
        return _SDT("./");
#endif
    }

    const TCHAR*  SDGetWorkPath()
    {
#if (defined(WIN32) || defined(WIN64))
        return _tgetcwd(NULL,0);
#else
        static CHAR workPath[SD_MAX_PATH];
        getcwd(workPath,sizeof(workPath));
        return _SDTA2T(workPath);
#endif
    }

    tstring   SDFileExtractPath(const TCHAR* strFileName)
    {
        if(NULL == strFileName)
        {
            return _SDT("");
        }
        tstring::size_type pos = tstring(strFileName).rfind('\\') ;
        if (pos == tstring::npos)
        {
            pos =  tstring(strFileName).rfind('/');
            if (pos == tstring::npos)
            {
                return _SDT("");
            }
        }

        return tstring(strFileName).substr(0, pos);
    }

    tstring   SDFileExtractName(const TCHAR* strFileName)
    {
        if(NULL == strFileName)
        {
            return _SDT("");
        }
        tstring::size_type pos = tstring(strFileName).rfind('\\');
        if (pos == tstring::npos)
        {
            return strFileName;
        }
        return tstring( strFileName).substr(pos + 1);
    }

    tstring   SDFileExtractExt(const TCHAR* strFileName)
    {
        if(NULL == strFileName)
        {
            return _SDT("");
        }
        tstring::size_type pos = tstring(strFileName).find_last_of(_SDT(":.\\"));
        if (pos == tstring::npos || strFileName[pos] != '.')
        {
            return _SDT("");
        }
        return tstring(strFileName).substr(pos + 1);
    }

    tstring   SDFileExcludeTrailingDelimiter(const TCHAR* strPath)
    {
        if(NULL == strPath)
        {
            return _SDT("");
        }
        int len = _SDTStrlen(strPath);

        if (len == 0)
        {
            return _SDT("");
        }

        if (SDPATH_IS_DELIMITER(strPath[len - 1]))
        {
            return tstring(strPath).substr(0, len - 1);
        }
        else
        {
            return strPath;
        }
    }

    tstring   SDFileExcludeLeadingDelimiter(const TCHAR* strPath)
    {
        if(NULL == strPath)
        {
            return _SDT("");
        }
        size_t pos = 0;
        size_t len = _SDTStrlen(strPath);
        while (pos < len )
        {
            if (!SDPATH_IS_DELIMITER(strPath[pos]))
            {
                break;
            }
            pos++;
        }
        return tstring(strPath).substr(pos);
    }

    tstring  SDFileIncludeTrailingDelimiter(const TCHAR* strPath)
    {
        if(NULL == strPath)
        {
            return _SDT("");
        }
        int len = _SDTStrlen(strPath);
        if (len == 0 )
        {
            return _SDT("");
        }
        if (!SDPATH_IS_DELIMITER(strPath[len - 1]))
        {
            return tstring(strPath) + _SDT("\\");
        }
        else
        {
            return strPath;
        }
    }

    tstring   SDFileIncludeLeadingDelimiter(const TCHAR* strPath)
    {
        if(NULL == strPath)
        {
            return _SDT("");
        }
        if (_SDTStrlen(strPath) == 0)
        {
            return _SDT("\\");
        }
        if (!SDPATH_IS_DELIMITER(strPath[0]))
        {
            return tstring(_SDT("\\")) + strPath;
        }
        else
        {
            return strPath;
        }
    }

    tstring  SDFileReplaceDelimiter(const TCHAR * strPath,TCHAR delimiter)
    {
        tstring path = strPath;
        for (tstring::iterator itr = path.begin(); itr != path.end(); ++itr)
        {
            if (SDPATH_IS_DELIMITER(*itr))
            {
                *itr = delimiter;
            }
        }
        return path;
    }

    BOOL  SDFileExists(const TCHAR* strFileName)
    {
        FILE* fp = fopen(_SDTT2A(strFileName), "r");
        if (fp == NULL)
        {
            return FALSE;
        }

        fclose(fp);

        return TRUE;
    }

    INT32  SDFileGetSize(const TCHAR* strFileName)
    {
#if (defined(WIN32) || defined(WIN64))
        struct _stat st;
        if (_tstat(strFileName, &st) < 0)
        {
            return -1;
        }
#else
        struct stat st;
        if (stat(_SDTT2A(strFileName), &st) < 0)
        {
            return -1;
        }
#endif

        return st.st_size;
    }

    BOOL  SDFileCreate(const TCHAR* strFileName)
    {
        if (SDFileExists(strFileName))
        {
            return FALSE;
        }

        tstring strPath = SDFileExtractPath(strFileName);
        if(!strPath.empty())
        {
            if(!SDDirectoryExists(strPath.c_str()))
            {
                if (!SDCreateDirectory(strPath.c_str()))
                {
                    return FALSE;
                }
            }
        }


        FILE* fp = fopen(_SDTT2A(strFileName), ("a+"));
        if (NULL == fp)
        {
            return FALSE;
        }

        fclose(fp);

        return TRUE;
    }

    BOOL  SDFileCopy(const TCHAR * pszExistFile, const TCHAR * pNewFile ,BOOL bFailIfExists)
    {
#if (defined(WIN32) || defined(WIN64))
        return CopyFile(pszExistFile,pNewFile,bFailIfExists); 
#else 
        int fps, fpd;
        struct stat statbuf;
        void *bufs, *bufd;

        if (SDFileExists(pNewFile))
        {
            if (bFailIfExists)
            {
                return FALSE; 
            }
            else 
            {
                SDFileRemove(pNewFile); 
            }
        }

        if((fps = open(_SDTT2A(pszExistFile), O_RDONLY)) == -1) 
        {
            return FALSE; 
        }

        if((fpd = open(_SDTT2A(pNewFile), O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1) 
        {
            return FALSE; 
        }

        if(fstat(fps, &statbuf) == -1) 
        {
            return FALSE; 
        }

        if(pwrite(fpd, "", 1, statbuf.st_size - 1) != 1) 
        {
            return FALSE; 
        }

        if((bufs = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fps, 0)) == MAP_FAILED) 
        {
            return FALSE; 
        }

        if((bufd = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fpd, 0)) == MAP_FAILED) 
        {
            return FALSE; 
        }

        memcpy (bufd, bufs, statbuf.st_size);

        munmap(bufd, statbuf.st_size);
        munmap(bufs, statbuf.st_size);

        close(fpd);
        close(fps);

        return TRUE; 
#endif //		
    }
}

