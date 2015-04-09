/******************************************************************************
		

sdfile.h - 文件操作API

******************************************************************************/


#ifndef SDFILE_H
#define SDFILE_H

#include "sdtype.h"
#include <stdio.h>
#include <string>
#include <time.h>
#include <vector>
#include "sdstring.h"
#ifdef __linux__
#include <unistd.h>
#else
#endif

/**
* @file sdfile.h

* @brief 文件工具类
*
**/

namespace SGDP
{
    /**
    * @defgroup groupfile 文件操作
    * @ingroup  SGDP
    * @{
    */

    /* An arbitrary size that is digestable. True max is a bit less than 32000 */
#if (defined(WIN32) || defined(WIN64))
#define SD_MAX_PATH  MAX_PATH
#define SDPATH_DELIMITER '\\'
#else /* !SD_HAS_UNICODE_FS */
#define SD_MAX_PATH 260
#define SDPATH_DELIMITER '/'
#endif

    /* Win32 .h ommissions we really need */
#define SDFILENO_STDIN   0
#define SDFILENO_STDOUT  1
#define SDFILENO_STDERR  2
#define SDPATH_IS_DELIMITER(x)  (x == '\\' || x == '/')
    /**
    *@brief 文件状态结构体
    */
    struct SFileStatus
    {
        UINT32 mode;		/**<文件对应的模式，文件，目录等*/
        UINT32 inode;		/**<i-node节点号, linux-specific*/
        UINT32 dev;			/**<设备号码*/
        UINT32 rdev;		/**<特殊设备号码*/
        UINT32 nlink;		/**<文件的连接数*/
        UINT32 uid;			/**<文件所有者, linux-specific*/
        UINT32 gid;			/**<文件所有者对应的组, linux-specific*/
        UINT32 size;		/**<普通文件，对应的文件字节数*/
        time_t atime;		/**<文件最后被访问的时间*/
        time_t mtime;		/**<文件内容最后被修改的时间*/
        time_t ctime;		/**<文件状态（属性）改变时间*/
    };


    /**
    *@brief 文件权限枚举类
    */
    enum AccessMode
    {
#if (defined(WIN32) || defined(WIN64))
        EXIST_OK = 0,
        RD_OK = 2,
        WD_OK = 4,
#else
        EXIST_OK = F_OK,
        RD_OK = R_OK,
        WD_OK = W_OK,
        EXECUTE_OK = X_OK,
#endif
    };



    /**
    *@brief 文件操作类，完成最基本的文件操作,或使用FileMapping方式读取文件(具体功能参照接口)
    */
    class CSDFile
    {

    public:

		/**
		* @brief 构造函数，初始化文件句柄和文件内存映射
		*/
        CSDFile();

		/**
		* @brief 析构函数，关闭文件句柄
		*/
        ~CSDFile();

        /**
        *@brief seek位置枚举类
        */
        enum SeekOffset
        {
            SK_SET = SEEK_SET,		/**<文件开始位置*/
            SK_END = SEEK_END,		/**<文件结尾位置*/
            SK_CUR = SEEK_CUR,		/**<文件当前位置*/
        };

        /**
        * @brief
        * Open a file, type参数同fopen接口
        * @param pszFileName : 文件名称
        * @param type : 文件访问权限
        * @param bFileMapping : 是否将文件映射到内存
        * @return 成功返回TRUE，失败返回FALSE
        */
        BOOL  Open(const TCHAR* pszFileName, const TCHAR* type, BOOL bFileMapping = FALSE);

        /**
        * @brief
        * 关闭一个文件
        * @return VOID
        */
        VOID  Close();

        /**
        * @brief
        * Flush刷新文件
        * @return VOID
        */
        VOID  Flush();

        /**
        * @brief
        * 从文件中读取数据
        * @param pBuf : 读取数据的存放位置
        * @param nLen : 能够存发数据的最大长度
        * @return 实际读取的数据长度
        */
        UINT32  Read(VOID* pBuf, UINT32 nLen);

        /**
        * @brief
        * 向文件中写入数据
        * @param pBuf : 指向写入数据的指针
        * @param nLen : 最大可写的数据长度
        * @return 实际写入的数据长度
        */
        UINT32  Write(const VOID* pBuf, UINT32 nLen);

        /**
        * @brief
        * 移动文件游标到特定的位置
        * @param offset : 相对初始位置移动的距离
        * @param whence : 初始位置，请输入enum SeekOffset中的值
        * @return 如果成功，返回0
        */
        INT32  Seek(INT32 offset, CSDFile::SeekOffset whence);

        /**
        * @brief
        * 移动文件游标到当前文件的文件头
        * @return 返回－1代表失败
        */
        INT32  SeekToBegin();

        /**
        * @brief
        * 移动文件游标到当前文件的文件尾
        * @return 返回－1代表失败
        */
        INT32  SeekToEnd();

        /**
        * @brief
        * 获取当前文件游标距离文件头的距离，单位byte
        * @return 当前文件游标距离文件头的距离
        */
        LONG  GetPosition();

        /**
        * @brief
        * 更新文件的长度，单位为byte
        * @param newLen : 更新的文件长度，单位为byte
        * @return VOID
        */
        VOID  SetLength(LONG newLen);

        /**
        * @brief
        * 测试是否为文件尾部
        * @return 如果是文件尾部，返回TRUE，否则返回FALSE
        */
        BOOL  Eof();

        /**
        * @brief
        * 获取当前文件的属性
        * @param pStatus : 获取的文件属性结构体
        * @return 如果成功，返回TRUE，否则返回FALSE
        */
        BOOL  GetFileStatus(SFileStatus &pStatus);

    private:

		/**
		* @brief 拷贝构造函数
		* @param origFile : CSDFile对象的引用
		*/
        CSDFile(const CSDFile& origFile);              // no implementation
        
		/**
		* @brief 重载=操作符，实现对象的赋值
		* @param origFile : CSDFile对象的引用
		* @return VOID
		*/
		VOID operator=(const CSDFile& origFile);       // no implementation

        VOID * m_pFileHandle;
        tstring m_filepath;
        BOOL m_bFileMapping; // 是否使用FileMapping方式读取文件
    };


    /**
    * @brief
    * 获取文件的属性
    * @param pszFileName : 需要获取文件属性的文件路径
    * @param pStatus : 获取的文件属性结构体
    * @return 如果成功，返回TRUE，否则返回FALSE
    */
    BOOL  SDGetFileStatus(const TCHAR* pszFileName, SFileStatus& stStatus);

    /**
    * @brief
    * 删除一个文件
    * @param pszFileName : 需要删除的文件的文件路径
    * @return 是否成功，返回TRUE，否则返回FALSE
    */
    BOOL  SDFileRemove(const TCHAR* pszFileName);

    /**
    * @brief
    * 更改一个文件的文件名（或者是文件路径）
    * @param pszOldName : 原先的文件路径
    * @param pszNewName : 新的文件路径
    * @param bForce     : 是否强制重命名文件
    * @return 成功返回0
    */
    INT32  SDFileRename(const TCHAR *pszOldName, const TCHAR *pszNewName, BOOL bForce = FALSE);

    /**
    * @brief
    * 更改一个文件的文件路径（或者是文件名）
    * @param pszOldName : 原先的文件路径
    * @param pszNewName : 新的文件路径
    * @param bForce     : 是否强制移动文件
    * @return 成功返回0
    */
    INT32  SDFileMove(const TCHAR *pszOldName, const TCHAR *pszNewName, BOOL bForce = FALSE);

    /**
    * @brief
    * 确定文件的读写权限
    * @param pszPath : 需要确定读写权限的文件路径
    * @param mode : 需要确认的可访问模式
    * 有如下几种模式：EXIST_OK(是否存在), RD_OK(是否可读), WD_OK(是否可写)
    * @return 如果拥有该模式，返回0
    */
    INT32  SDFileAccess(const TCHAR *pszPath, AccessMode mode);

    /**
    * @brief
    * 获取当前应用的完整路径,包括执行文件的文件名
    * 需要注意的是，此函数即便在外面加所也无法确保线程安全
    * @return 获取的当前应用的路径
    */
	
    const TCHAR*  SDGetModuleName();

    /**
    * @brief
    * 获取当前应用的路径
    * 需要注意的是，此函数即便在外面加所也无法确保线程安全
    * @return 当前应用的路径
    */
    const TCHAR*  SDGetModulePath();

	/**
	* @brief
	* 获取当前应用的工作路径(current work directory) 
	* 需要注意的是，此函数即便在外面加所也无法确保线程安全
	* @return 当前应用的工作路径
	*/
    const TCHAR*  SDGetWorkPath();

    /**
    * @brief
    * 获取文件的文件路径
    * @param pszFileName : 文件路径＋文件名称
    * @return 文件路径
    */
    tstring  SDFileExtractPath(const TCHAR *pszFileName);

    /**
    * @brief
    * 获取文件的文件名
    * @param pszFileName : 文件路径＋文件名称
    * @return 文件名
    */
    tstring  SDFileExtractName(const TCHAR *pszFileName);

    /**
    * @brief
    * 获取文件的文件扩展名
    * @param pszFileName : 文件路径＋文件名称
    * @return 文件扩展名
    */
    tstring  SDFileExtractExt(const TCHAR *pszFileName);

    /**
    * @brief
    * 去除文件路径最后的"\"
    * @param pszPath : 文件路径
    * @return 去除最后的"\"后的文件路径
    */
    tstring  SDFileExcludeTrailingDelimiter(const TCHAR *pszPath);

    /**
    * @brief
    * 添加"\"到文件路径最后，若文件路径后本有"\"，则不添加
    * @param pszPath : 文件路径
    * @return 添加"\"后的文件路径
    */
    tstring  SDFileIncludeTrailingDelimiter(const TCHAR *pszPath);

    /**
    * @brief
    * 添加"\"到文件路径前，若文件路径前本有"\"，则不添加
    * @param pszPath : 文件路径
    * @return 添加"\"后的文件路径
    */
    tstring  SDFileIncludeLeadingDelimiter(const TCHAR *pszPath);

    /**
    * @brief
    * 去除文件路径前的"\"
    * @param pszPath : 文件路径
    * @return 去除"\"后的文件路径
    */
    tstring  SDFileExcludeLeadingDelimiter(const TCHAR *pszPath);

    /**
    * @brief
    * 将文件路径中的"\"和"//"替换为特定的字符delimiter
    * @param pszPath : 文件路径
    * @param delimiter : 替换字符
    * @return 替换后的文件路径
    */
    tstring  SDFileReplaceDelimiter(const TCHAR *pszPath, TCHAR delimiter);

    /**
    * @brief
    * 检测文件是否存在
    * @param pszFileName : 文件路径
    * @return 存在则返回TRUE，不存在则返回FALSE
    */
    BOOL  SDFileExists(const TCHAR *pszFileName);

    /**
    * @brief
    * 获取文件大小
    * @param pszFileName : 文件路径
    * @return 文件大小
    */
    INT32  SDFileGetSize(const TCHAR *pszFileName);

    /**
    * @brief
    * 创建文件
    * @param pszFileName : 文件路径
    * @return 创建成功返回TRUE，否则返回FALSE
    */
    BOOL  SDFileCreate(const TCHAR *pszFileName);

	/**
	* @brief
	* 拷贝文件
	* @param pszExistFile :源文件路径 
	* @param pNewFile :   目标文件路径
	* @param bFailIfExists : 如果目标存在是否返回失败
	* @return 拷贝成功返回TRUE，否则返回FALSE
	*/
	BOOL  SDFileCopy(const TCHAR *pszExistFile, const TCHAR *pNewFile, BOOL bFailIfExists); 

    /** @} */
}

#endif





