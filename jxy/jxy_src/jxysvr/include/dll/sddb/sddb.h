
#ifndef SGDP_SDDB_H_20100728
#define SGDP_SDDB_H_20100728

#include <string>
#include <vector>
#include	<stdlib.h>
#include <string.h>
#include "sdbase.h"
#include "sdlogger.h"

using namespace std;

/**
* @brief 数据库信息名称的最大长度
*/
#define SDDB_MAX_NAME_SIZE		        64

/**
* @brief 数据库字符集名称的最大长度
*/
#define SDDB_MAX_CHARACTSET_SIZE		32

namespace SGDP{

static const SSGDPVersion SDDB_VERSION = {5, 0, 0, 0};

/**
* @brief 数据库模块的名称
*/
const CHAR SDDB_MODULENAME[]        = "SDDB";

/**
* @brief SQL执行结果的返回值
*/
enum ESDDBCode
{
    SDDB_ERR_UNKNOWN    = -999, ///< 未知错误
	SDDB_ERR_DUPLICATE  = -3,   ///< 主键重复
    SDDB_ERR_CONN       = -2,   ///< 连接错误
    SDDB_DISCONNECT     = -1,   ///< 断开与服务器的连接
    SDDB_SUCCESS        = 0,    ///< 操作成功
    SDDB_NO_RECORDSET   = 1,    ///< 操作成功，但没有结果集
    SDDB_HAS_RECORDSET  = 2     ///< 操作成功，有结果集
};

/**
* @brief 数据库服务器登陆信息结构体
*/
typedef struct tagSDDBAccount
{
    CHAR m_szHostName[SDDB_MAX_NAME_SIZE];         ///< 数据库服务器IP地址
    CHAR m_szDBName[SDDB_MAX_NAME_SIZE];           ///< 欲连接的数据库名称
    CHAR m_szLoginName[SDDB_MAX_NAME_SIZE];        ///< 登陆用户名
    CHAR m_szLoginPwd[SDDB_MAX_NAME_SIZE];         ///< 登陆密码
	CHAR m_szCharactSet[SDDB_MAX_CHARACTSET_SIZE]; ///< 数据库编码集
	CHAR m_szUnixSocket[SDDB_MAX_NAME_SIZE];	   ///< mysql的unix socket
    UINT16 m_wConnPort;					           ///< 数据库监听端口
	INT32 m_wDBType;						       ///< SDDBDataBaseType数据库类型
	tagSDDBAccount()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(tagSDDBAccount));
	}
} SDDBAccount, * PSDDBAccount;

/**
* @brief 数据库类型
*/
enum SDDBDataBaseType
{
	SDDB_DBTYPE_MOCK = -1,  ///<模拟的数据库
	SDDB_DBTYPE_MYSQL = 0,  ///< MySQL数据库
	SDDB_DBTYPE_ODBC = 1,   ///< ODBC数据库
	SDDB_DBTYPE_ADO = 2     ///< ADO数据库
};

/**
*  @brief SQL结果集接口类，SQL结果集的接口，提供方法操作处理结果集合
*/
class ISDDBRecordSet
{
public:
	/**
	* @brief 获取结果的条数
	*
	* @return 获取结果的条数
	*/
	virtual UINT32 SDAPI GetRecordCount(void) = 0;
    
	/**
	* @brief 获取每条结果的域的数量
	*
	* @return 获取每条结果的域的数量
	*/
    virtual UINT32 SDAPI GetFieldCount(void) = 0;
    
	/**
	* @brief 获取下一条结果
	*
	* @return 是否有下一条结果，有为true，没有为false
	*/
    virtual bool SDAPI GetRecord(void) = 0;
    
	/**
	* @brief 获取当前结果的特定index的域的值
	*
	* @param dwIndex : 域的Index
	* @return 如果有该Index的域，返回域的值；如果没有该Index的域，返回NULL；
	*/
    virtual const CHAR * SDAPI GetFieldValue(UINT32 dwIndex) = 0;
    
	/**
	* @brief 获取当前结果的特定index的域的长度
	*
	* @param dwIndex : 域的Index
	* @return 如果有该Index的域，返回域的长度；如果没有该Index的域，返回0
	*/
    virtual INT32 SDAPI GetFieldLength(UINT32 dwIndex) = 0;
    
	/**
	* @brief 获取字段名
	* @param dwIndex : 字段的Index
	* @return 如果有该Index的域，返回字段名；如果没有该Index的域，返回NULL；
	*/
	virtual const CHAR * SDAPI GetFieldName(UINT32 dwIndex) { return NULL; }

	/**
	* @brief 释放掉此结果集
	*/
    virtual void SDAPI Release(void)=0;

	/**
	* @brief 获取当前结果的特定域名的域的值
	* @param pszFieldName : 域的名字
	* @return 如果找到该域，返回该域的值；如果返回NULL，代表该结果为NULL或者域名错误
	*/
	virtual const CHAR* SDAPI GetFieldValueByName(const CHAR *pszFieldName) = 0;

	/**
	* @brief 获取当前结果的特定域名的域的长度
	* @param pszFieldName : 域的名字
	* @return 如果找到该域，返回该域的长度；如果返回0，代表该结果为NULL或者域名错误
	*/
	virtual INT32 SDAPI GetFieldLengthByName(const CHAR *pszFieldName) = 0;
};

/**
* @brief DBConnection接口类，代表数据库的连接，只在异步回调中使用。
* 注意：此类中所有的函数都“不是”线程安全的
*/
class ISDDBConnection
{
public:
	/**
	* @brief 检查连接是否正常
	*
	* @return 检查连接是否正常，true为正常，false为不正常
	*/
	virtual bool SDAPI CheckConnection() = 0;

	/**
	* @brief 将二进制字符串转化为数据库可识别的以0结尾的字符串
	*
	* @param pSrc : 源字符串的地址
	* @param nSrcSize : 源字符串的长度
	* @param pDest : 目标字符串的地址，该地址空间由调用者提供。目标字符串的内容将写入到该地址空间中
	* @param nDstSize : 目标字符串的最大长度，一般应该定义为2×nSrcSize ＋ 1
	* @return 转化后的字符串的长度，转化失败返回0
	*/
    virtual UINT32 SDAPI EscapeString(const CHAR *pSrc, INT32 nSrcSize, 
                                      CHAR *pDest,    INT32 nDstSize) = 0;
	/**
	* @brief 执行无返回值的SQL语句，如果为插入操作，可从pInsertId获取插入行的ID号
	*
	* @param pSQL : 需要执行的SQL语句地址
	* @param pInsertId : 如果为插入SQL语句，可以通过pInsertId获取插入数据的行数。该地址空间由调用者提供
	*                  注意：MS SQL Server暂不支持此操作，请设置为NULL
	* @return 执行SQL语句的结果
	* SDDB_ERR_CONN 连接错误
	* SDDB_ERR_UNKNOWN 未知数据库错误
	* 正数 影响的行数
	*/
	virtual INT32 SDAPI ExecuteSql(const CHAR *pSQL, UINT64 *pInsertId = NULL, UINT32* pEffectRowNum = NULL, UINT32* pdwErr = NULL, string* pstrErr = NULL) = 0;

	/**
	* @brief 执行有返回结果的SQL语句
	*
	* @param pSQL : 需要执行的SQL语句
	* @param ppoRs : 返回的结果集合，该地址空间由SDDB提供。
	* @return 执行SQL语句的结果
	* SDDB_ERR_CONN 连接错误
	* SDDB_ERR_UNKNOWN 未知错误
	* SDDB_NO_RECORDSET 执行成功，没有结果集合
	* SDDB_HAS_RECORDSET 执行成功，有结果集合，注意，完成操作后，需要(*ppoRs)->Release();
	*/
	virtual INT32 SDAPI ExecuteSqlRs(const CHAR *pSQL, SGDP::ISDDBRecordSet **ppoRs, UINT32* pdwErr = NULL, string* pstrErr = NULL) = 0;

	//多结果集
	virtual INT32 SDAPI ExecuteSqlRs(const CHAR *pSQL, std::vector<SGDP::ISDDBRecordSet*>& vecRs, UINT32* pdwErr = NULL, string* pstrErr = NULL) { return SDDB_NO_RECORDSET; };

	/**
	* @brief 进入一个事务
	*/
	virtual void SDAPI BeginTransaction() = 0;

	/**
	* @brief 完成一个事务
	*/
	virtual void SDAPI CommitTransaction() = 0;

	/**
	* @brief 回滚一个事务
	*/
	virtual void SDAPI RollbackTransaction() = 0;

	/**
	* @brief 创建DB
	*
	* @param pcDBName : 需要创建的DBName
	* @param bForce : 是否强制执行
	* @param pcCharSet : DB的编码集
	* @return 创建DB是否成功，true代表成功，false代表失败 
	*/
	virtual bool SDAPI CreateDB(const CHAR *pcDBName, bool bForce, const CHAR *pcCharSet) = 0;

	/**
	* @brief 选择操作的DB
	*
	* @param pcDBName : 选择操作的DBName
	* @return 选择DB是否成功，true代表成功，false代表失败
	*/
	virtual bool SDAPI SelectDB(const CHAR *pcDBName) = 0;

	/**
	* @brief 释放ISDDBConn
	*/
	virtual void SDAPI Release() = 0;
};

/**
* @brief DBCommand接口类，代表异步执行SQL的DB命令，整个过程为：
* 插入命令->OnExecuteSql(异步)->(执行ISDDBSession::Run时)OnExecuted(同步)->Release(同步)
*/
class ISDDBCommand 
{
public:
	/**
	* @brief 该命令的分组ID。
	* 需要注意的是，异步执行的SQL命令，如果在同一个Group中，我们保证按插入命令时的时间逐个执行SQL命令
	* 如果返回为－1，我们不保证按插入命令时的时间逐个执行SQL命令
	* 使用QuickAddDBCommand，我们同样不保证执行SQL命令的时间顺序
	*/
	virtual int SDAPI GetGroupId() {return -1;};

	/**
	* @brief 执行SQL命令
	*
	* @param poDBConn : 执行命令所用的连接
	* 注意：在连接池中有多个连接的情况下，此函数“不是”线程安全的。
	*/
	virtual void SDAPI OnExecuteSql(ISDDBConnection *poDBConnection) = 0;

	/**
	* @brief 执行完毕SQL命令后，在用户线程中执行ISDDBSession::Run时，
	* 执行此函数中的业务逻辑，对用户而言，此函数中的执行是同步的。
	* 注意：对于在一个Session中执行的命令，此函数在此Session::Run()中顺序运行，
	* 也就是说，如果全局只有一个地方执行Session::Run()，则，此函数是线程安全的。
	*/
    virtual void SDAPI OnExecuted(void) = 0;
    
	/**
	* @brief 执行完毕SQL命令后，在用户线程中执行ISDDBSession::Run时，
	* 执行完业务逻辑后，执行此Release操作，对用户而言，此函数中的执行是同步的
	* 绝大多数情况下，推荐内容为：delete this;
	* 注意：对于在一个Session中执行的命令，此函数在此Session::Run()中顺序运行，
	* 也就是说，如果全局只有一个地方执行Session::Run()，则，此函数是线程安全的。
	*/
    virtual void SDAPI Release(void) = 0;
};

/**
* @brief DBSession接口类，代表与一个数据库服务器连接的会话，该会话也可以维护多个连接
*/
class ISDDBSession
{
public:
	/**
	* @brief 将二进制字符串转化为数据库可识别的以0结尾的字符串
	*
	* @param pSrc : 源字符串的地址
	* @param nSrcSize : 源字符串的长度
	* @param pDest : 目标字符串的地址，该地址空间由调用者提供。目标字符串的内容将写入到该地址空间中
	* @param nDstSize : 目标字符串的最大长度，一般应该定义为2×nSrcSize ＋ 1
	* @param timeout : 执行的超时时间，负数为无限时间
	* @return 转化后的字符串的长度，转化失败返回0
	*/
    virtual UINT32 SDAPI EscapeString(const CHAR *pSrc, INT32 nSrcSize, 
                                      CHAR *pDest,    INT32 nDstSize, INT32 timeout = -1) = 0;

	/**
	* @brief 执行无返回值的SQL语句，如果为插入操作，可从pInsertId获取插入行的ID号
	*
	* @param pSQL : 需要执行的SQL语句地址
	* @param pInsertId : 如果为插入SQL语句，可以通过pInsertId获取插入数据的行数。该地址空间由调用者提供
	* @param timeout : 执行的超时时间，负数为无限时间
	* @return 执行SQL语句的结果
	* SDDB_ERR_CONN 连接错误
	* SDDB_ERR_UNKNOWN 未知数据库错误
	* 正数 影响的行数
	*/
	virtual INT32 SDAPI ExecuteSql(const CHAR *pSQL, UINT64 *pInsertId = NULL, INT32 timeout = -1) = 0;

	/**
	* @brief 执行有返回结果的SQL语句
	*
	* @param pSQL : 需要执行的SQL语句
	* @param ppoRs : 返回的结果集合，该地址空间由SDDB提供。
	* @param timeout : 执行的超时时间，负数为无限时间
	* @return 执行SQL语句的结果
	* SDDB_ERR_CONN 连接错误
	* SDDB_ERR_UNKNOWN 未知错误
	* SDDB_NO_RECORDSET 执行成功，没有结果集合
	* SDDB_HAS_RECORDSET 执行成功，有结果集合，注意，完成操作后，需要(*ppoRs)->Release();
	*/
    virtual INT32 SDAPI ExecuteSqlRs(const CHAR *pSQL, ISDDBRecordSet **ppoRs, UINT32* pdwErr = NULL, string* pstrErr = NULL, INT32 timeout = -1) = 0;

	/**
	* @brief 创建DB
	*
	* @param pcDBName : 需要创建的DBName
	* @param bForce : 是否强制执行
	* @param pcCharSet : DB的编码集
	* @param timeout : 执行的超时时间，负数为无限时间
	* @return 创建DB是否成功，true代表成功，false代表失败 
	*/
	virtual bool SDAPI CreateDB(const CHAR *pcDBName, bool bForce, const CHAR *pcCharSet, INT32 timeout = -1) = 0;

	/**
	* @brief 选择操作的DB
	*
	* @param pcDBName : 选择操作的DBName
	* @param timeout : 执行的超时时间，负数为无限时间
	* @return 选择DB是否成功，true代表成功，false代表失败
	*/
	virtual bool SDAPI SelectDB(const CHAR *pcDBName, INT32 timeout = -1) = 0;

	/**
	* @brief 添加DB命令
	*
	* @param poDBCommand : 添加的DB命令
	* @return 是否添加成功，true阿代表成功，false代表失败
	*/
	virtual bool SDAPI AddDBCommand(ISDDBCommand *poDBCommand) = 0;

	/**
	* @brief 快速添加DB命令
	*
	* @param poDBCommand : 快速添加的DB命令
	* @return 是否添加成功，true阿代表成功，false代表失败
	*/
    virtual bool SDAPI QuickAddDBCommand(ISDDBCommand *poDBCommand) = 0;

	/**
	* @brief 执行完成的DB命令的逻辑代码，即：DBCommand的OnExecuted(同步)->Release(同步)部分
	*
	* @param nCount : 需要执行DB命令的逻辑代码的数量,并返回实际执行数量
	* @return 是否执行完成nCount数量的SQL命令
	*/
	virtual bool SDAPI Run(UINT32& dwCount) = 0;

	/**
	 * @desc 获取可执行db操作的DB命令的数量
	 * 
	 * @return 当前可执行db操作的DB命令的数量
	 **/
	virtual UINT32 SDAPI GetExcuteCmdCount() = 0;

	/**
	 * @desc 获取已执行db操作的DB命令的数量
	 * 
	 * @return 当前可运行的DB命令的数量
	 **/
	virtual UINT32 SDAPI GetExcutedCmdCount() = 0;

	/**
	* @brief 获取一个连接
	*/
	virtual SGDP::ISDDBConnection* SDAPI GetDBConnection() = 0;

	/**
	* @brief 释放一个连接
	*/
	virtual void SDAPI FreeDBConnection(SGDP::ISDDBConnection *poConn) = 0 ;

};

/**
* @brief DB模块类接口，用于创建和关闭DB会话
*/
class ISDDBModule : public ISDBase
{
public:
	/**
	* @brief 根据ConfigString信息创建DBSession.
	* 示例: HostName=IPAddress;LoginName=YourLoginName;LoginPwd=Password;DBName=YourDBName;CharacterSet=latin1;CoreSize=5;MaxSize=10;DBType=0;Port=3306;
	* 其中,缺省DBType为0,即MySQL(为3则是MS SQL);MySQL的缺省端口号为3306,MS SQL的缺省端口号为1433.
	*
	* @param pszConfigString : 数据库连接信息
	* @return 创建的DBSession，如果创建失败，返回NULL
	*/
	virtual ISDDBSession * SDAPI GetDBSession(const CHAR * pszConfigString) = 0;

	/**
	* @brief 根据DBAccount信息创建DBSession，这个DBSession中只有单个DBConnection连接
	*
	* @param pstDBAccount : 数据库连接信息
	* @return 创建的DBSession，如果创建失败，返回NULL
	*/
	virtual ISDDBSession * SDAPI GetDBSession(SDDBAccount *pstDBAccount) = 0;

	/**
	* @brief 创建的DBSession
	*
	* @param pstDBAccount : 数据库连接信息
	* @param coreSize : 创建核心（最小）连接数
	* @param maxSize : 创建最大连接数
	* @return 创建的DBSession，如果创建失败，返回NULL
	*/
	virtual ISDDBSession * SDAPI GetDBSession(SDDBAccount *pstDBAccount, UINT32 coreSize, UINT32 maxSize) = 0;

	/**
	* @brief 关闭一个会话，所有本DBModule创建的DBSession都需要关闭会话
	*
	* @param pDBSession : 一个本DBModule创建的DBSession
	*/
	virtual void SDAPI Close(ISDDBSession * pDBSession) = 0; 
};

/**
* @brief 根据版本号创建相应的DBModule
*/
ISDDBModule* SDAPI SDDBGetModule();

/**
* @brief 定义的指向SDDBGetModule的指针
*/
typedef ISDDBModule* (SDAPI *PFN_SDDBGetModule)(const SSGDPVersion *pstVersion);

/**
* @brief 设置DBModule内部使用的Logger和日志级别
*
* @param poLogger : 使用的Logger
* @param dwLevel : 日志级别
* @return 设置是否成功，如果成功返回true，失败返回false
*/
bool SDAPI SDDBSetLogger(ISDLogger *poLogger, UINT32 dwLevel);

/**
* @brief 定义的指向SDDBSetLogger的指针
*/
typedef bool (SDAPI * PFN_SDDBSetLogger)(ISDLogger *poLogger, UINT32 dwLevel);

}// namespace SGDP

#endif

