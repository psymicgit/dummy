#include <stdio.h>
#include <string>
#include "sdstring.h"
#include "sdshmem.h"
#include "sdprocess.h"
#if (defined(WIN32) || defined(WIN64))
#include <Windows.h>
#else
#include <sys/types.h>
#include <sys/shm.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#endif

namespace SGDP
{
	using namespace std;

#if (defined(WIN32) || defined(WIN64))
    static VOID ThansPath(std::string& file)
    {
        for (std::string::iterator itr = file.begin(); itr != file.end(); ++itr)
        {
            if (*itr == '/')
            {
                *itr = '\\';
            }
        }
    }
#else
    static VOID ThansPath(std::string& file)
    {
        for (std::string::iterator itr = file.begin(); itr != file.end(); ++itr)
        {
            if (*itr == '\\')
            {
                *itr = '/';
            }
        }
    }
#endif

	string& replace_all(string& str,const string& old_value,const string& new_value)    
	{   
		while(true)   
		{    
			string::size_type   pos(0);
			if((pos=str.find(old_value))!=string::npos) 
				str.replace(pos,old_value.length(),new_value); 
			else   
				break;    
		}    
		return   str;    
	} 

	std::string GetFileName(std::string strFilePath)
	{
#if (defined(WIN32) || defined(WIN64))
		INT32 pos = strFilePath.find_last_of("\\");
		if((-1 == pos) || (0 == pos))
		{
			return "";
		}
		return strFilePath.substr(pos+1, strFilePath.length() - pos -1);
#else
		INT32 pos = strFilePath.find_last_of("/");
		if((-1 == pos) || (0 == pos))
		{
			return "";
		}
		return strFilePath.substr(pos+1, strFilePath.length() - pos -1);
#endif

	}

	std::string GetFileNameEx(std::string strFilePath)
	{
#if (defined(WIN32) || defined(WIN64))
		strFilePath = replace_all(strFilePath, ":", "_");
		strFilePath = replace_all(strFilePath, " ", "_");
		strFilePath = replace_all(strFilePath, "\\", "_");
		strFilePath = replace_all(strFilePath, "/", "_");
		return strFilePath;
#else
		INT32 pos = strFilePath.find_last_of("/");
		if((-1 == pos) || (0 == pos))
		{
			return "";
		}
		return strFilePath.substr(pos+1, strFilePath.length() - pos -1);
#endif

	}

    BOOL SDAttachShm(SShmem *shm, const CHAR *file)
    {
        std::string path = file;
        ThansPath(path);
#if (defined(WIN32) || defined(WIN64))

		CHAR szName[512] = {0};
		sprintf(szName, "%s.shmem", GetFileNameEx(string(file)).c_str());

        shm->handle = OpenFileMappingA(FILE_MAP_READ|FILE_MAP_WRITE, FALSE, szName);
        if (shm->handle == NULL)
        {
            return FALSE;
        }

        shm->mem = MapViewOfFile(shm->handle, FILE_MAP_READ|FILE_MAP_WRITE,0,0,0);

        if (shm->mem == NULL)
        {
            CloseHandle(shm->handle);
            return FALSE;
        }
#else
        key_t shmkey = ftok(path.c_str(), 'a');
        if(shmkey == -1)
        {
            return FALSE;
        }

        struct stat st;
        if (stat(path.c_str(), &st) == -1)
        {
            return FALSE;
        }

        int id = shmget(shmkey, st.st_size, SHM_R | SHM_W);
        if (id == -1)
        {
            return FALSE;
        }

        shm->mem = shmat(id, NULL, 0);
        if(shm->mem == (VOID*)-1)
        {
            return FALSE;
        }
        shm->handle = id;
#endif
        shm->file = path;

        return TRUE;;
    }

    static BOOL SDCreateShm(SShmem *shm, const CHAR *file, UINT64 size)
    {
        std::string path = file;
        ThansPath(path);		
	
#if (defined(WIN32) || defined(WIN64))
		CHAR szName[512] = {0};
		sprintf(szName, "%s.shmem", GetFileNameEx(string(file)).c_str());

        HANDLE filehandle = CreateFileA(file, GENERIC_READ | GENERIC_WRITE,
                                        FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                                        FILE_ATTRIBUTE_HIDDEN, NULL);
        if (filehandle == INVALID_HANDLE_VALUE)
        {
            return FALSE;
        }

        SetFilePointer(filehandle, (LONG)size, NULL, FILE_BEGIN);

		UINT32 dwMaximumSizeHigh = size >> 32;
		DWORD dwMaximumSizeLow = (UINT32)size;
        shm->handle = CreateFileMappingA(filehandle, NULL,
                                         PAGE_READWRITE, dwMaximumSizeHigh, dwMaximumSizeLow, szName);

        if (shm->handle == NULL)
        {
            CloseHandle(filehandle);
            return FALSE;
        }

        shm->mem = MapViewOfFile(shm->handle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

        if (shm->mem == NULL)
        {
            CloseHandle(filehandle);
            CloseHandle(shm->handle);
            return FALSE;
        }

        CloseHandle(filehandle);

#else
        FILE *fp = fopen(path.c_str(), "w+");
        if (fp == NULL)
        {
            return FALSE;
        }
        fclose(fp);

        size += 4;
        if (truncate(path.c_str(), size) == -1)
        {
            return FALSE;
        }

        key_t shmkey = ftok(path.c_str(), 'a');
        if(shmkey == -1)
        {
            return FALSE;
        }

        int id = shmget(shmkey, size, SHM_R | SHM_W | IPC_CREAT);
        if (id == -1)
        {
            return FALSE;
        }

        shm->mem = shmat(id, NULL, 0);
        if(shm->mem == (VOID*)-1)
        {
            shmctl(id, IPC_RMID, NULL);
            return FALSE;
        }
        shm->handle = id;

#endif

        shm->file = path;

        return TRUE;
    }

    SShmem  SDOpenShmem( const CHAR *file, BOOL bCreate,UINT64 size )
    {
		if (SDShmemExist(file))
		{
			SShmem handle;
			SDAttachShm(&handle, file);
			return handle;
		}
		else
		{
			return SDCreateShmem(file, size);
		}
    }

    SShmem  SDCreateShmem(const CHAR *pFile, UINT64 size )
    {
        SShmem handle;
		handle.Init();
        SDCreateShm(&handle, pFile, size);
        return handle;
    }

    BOOL  SDShmemExist(const CHAR *file)
    {
        std::string path = file;
        ThansPath(path);
#if (defined(WIN32) || defined(WIN64))
		CHAR szName[512] = {0};
		sprintf(szName, "%s.shmem", GetFileNameEx(string(file)).c_str());

        SDHANDLE handle= OpenFileMappingA(FILE_MAP_READ, FALSE, szName);
        BOOL ret = (handle!= NULL);
        CloseHandle(handle);

        return ret;
#else
        FILE *fp = fopen(path.c_str(), "r");
		if(NULL == fp)
		{
			return FALSE;
		}
		else
		{
			fclose(fp);
			return TRUE;
		}
#endif // 

    }

	VOID  SDUnAttachShm(SShmem *shm)
	{
		if (shm != NULL)
		{

#if (defined(WIN32) || defined(WIN64))
			UnmapViewOfFile(shm->mem);
			CloseHandle(shm->handle);
			DeleteFileA(shm->file.c_str());
#else
			shmdt(shm->mem);
			//shmctl(shm->handle, IPC_RMID, NULL);
			//unlink(shm->file.c_str());
#endif
		}
	}

    VOID  SDCloseShmem(SShmem *shm)
    {
        if (shm != NULL)
        {

#if (defined(WIN32) || defined(WIN64))
            UnmapViewOfFile(shm->mem);
            CloseHandle(shm->handle);
            DeleteFileA(shm->file.c_str());
#else
            shmdt(shm->mem);
            shmctl(shm->handle, IPC_RMID, NULL);
            unlink(shm->file.c_str());
#endif
        }
    }

	VOID  SDCloseShmemByPath(CHAR* pszFilePath, UINT32 dwSize)
	{
#ifdef __linux__

		FILE *fp = fopen(pszFilePath, "r");
		if (fp != NULL)
		{
			fclose(fp);
		}
		unlink(pszFilePath);

		key_t shmkey = ftok(pszFilePath, 'a');
		if(shmkey == -1)
		{
			return;
		}

		int id = shmget(shmkey, dwSize, SHM_R | SHM_W | IPC_CREAT);
		if (id == -1)
		{
			return ;
		}
		shmctl(id, IPC_RMID, NULL);
#endif
	}
	

    CSDShmem::CSDShmem()
    {
        m_shm.mem    = NULL;
        m_shm.handle = SDINVALID_HANDLE;
    }

    VOID*  CSDShmem::Open(const CHAR *file)
    {
        m_shm = SDOpenShmem(file);
        return m_shm.mem;
    }

    VOID*  CSDShmem::Create(const CHAR *file, UINT64 size )
    {
        m_shm = SDCreateShmem(file,size);
        return m_shm.mem;
    }

    VOID  CSDShmem::Close()
    {
        SDCloseShmem(&m_shm);
    }

    SDHANDLE CSDShmem::GetHandle()
    {
        return m_shm.handle;
    }

} // namespace SGDP


