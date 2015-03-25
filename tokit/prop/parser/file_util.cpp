#include "file_util.h"

#include <sys/stat.h>
#include <io.h>
#include <fstream>
#include <sstream>

#include "str_util.h"

namespace fileuitl
{
    bool is_dir_exist(const std::string &dir)
    {
        struct _stat fileStat;
        if ((_stat(dir.c_str(), &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR))
        {
            return true;
        }

        return false;
    }

    bool is_file_exist()
    {
        return false;
    }

    bool exist(const std::string &path)
    {
        return access(path.c_str(), 0) != -1;
    }

    bool dir(const std::string &path, /* out */filevec_t &files)
    {
        struct _finddata_t filefind;

        int handle = 0;

        if(-1 == (handle = _findfirst(path.c_str(), &filefind)))
        {
            return false;
        }

        do
        {
            if(_A_SUBDIR != filefind.attrib)
            {// 不是目录，是文件
                files.push_back(filefind.name);
            }
        }
        while(!_findnext(handle, &filefind));

        _findclose(handle);

        return true;
    }

    bool dir_no_ext(const std::string &path, /* out */filevec_t &files)
    {
        filevec_t ext_files;
        if(false == dir(path, ext_files))
        {
            return false;
        }

        for (filevec_t::iterator itr = ext_files.begin(); itr != ext_files.end(); ++itr)
        {
            string &filename = *itr;
            files.push_back(strutil::strip_ext(filename));
        }

        if(files.empty())
        {
            return false;
        }
        return true;
    }

    char* get_whole_file_buf(const std::string &path)
    {
        if(false == exist(path))
        {
            return NULL;
        }

        /* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 */   
        FILE *file = fopen (path.c_str(), "rb" );  
        if (file==NULL)  
        {  
            fputs ("File error",stderr);  
            return NULL;
        }  

        /* 获取文件大小 */  
        fseek (file , 0 , SEEK_END);  
        long size = ftell (file);  
        rewind (file);  

        /* 分配内存存储整个文件 */   
        char *buf = new char[size];  
        if (buf == NULL)  
        {  
            fputs ("Memory error",stderr);   
            return NULL;
        }  

        /* 将文件拷贝到buffer中 */  
        size_t ret = fread (buf, 1, size, file);  
        if (ret != size)  
        {  
            fputs ("Reading error",stderr);  
            return NULL;
        }  

        /* 现在整个文件已经在buffer中 */
        /* 关闭文件并释放内存 */
        fclose (file);  

        return buf;
    }

    string get_whole_file_str(const std::string &path)
    {
        if(false == exist(path))
        {
            return NULL;
        }

        std::ifstream file(path.c_str());
        std::stringstream buf;
        buf << file.rdbuf();
        std::string contents(buf.str());
        return contents;
    }
}