

#ifndef _UTILITY_H_
#define _UTILITY_H_

/**
 * @file 实用公用方法文件
 *
 * 提供一些独立性较强的基础宏和方法，例如解析字符串
 *
 */
#include <sdtype.h>
#include <sdstring.h>
#include <sdrandom.h>
#include <vector>
#include <map>

using namespace std;

using namespace SGDP;

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif


#define get_bit(var, n) ((var >> n) & 1)
#define set_bit(var, n) (var |= (1<<n))
#define clr_bit(var, n) (var &= ~(1<<n))


#define  AUTOCYCLE	CAutoCycle oAutoCycle(__FUNCTION__);
//次方函数，dwData的byNum次方
UINT32 power(UINT32 dwData, UINT8 byNum);

inline UINT32 GetCycleCount()
{
#ifdef _WIN32
    __asm _emit 0x0F
    __asm _emit 0x31
#else
    //UINT64 qwRet = 0;
    //__asm__ __volatile__ ("rdtsc" : "=r" (qwRet));
    //return (UINT32)qwRet;
    UINT32 low, high;
    __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high));
    return low + high;
#endif
}

INT32 RandomReseed(UINT32 dwRange);


typedef vector<UINT32> CRandomVec;
typedef CRandomVec::iterator CRandomVecItr;
typedef map<UINT32, UINT32> CRandomMap;
typedef CRandomMap::const_iterator CRandomMapItr;

#define RANDOM_UNHIT -1		//-1为未命中
INT32 GetRandomIdx(CRandomVec& vecRandom, UINT32 dwTotalCent = 100); //返回命中的概率序号(vecRandom中的顺序)，-1为未命中. dwTotalCent比值，默认100为百分比
INT32 GetRandomIdxMustHit(CRandomVec& vecRandom); //必须命中算法，返回命中的概率序号(vecRandom中的顺序),没有概率返回0序号
INT32 GetRandomIdxMustHit(CRandomVec& vecRandom, INT32 dwStart, INT32 dwEnd = 0); //必须命中算法，返回命中的概率序号(vecRandom中的顺序),没有概率返回0序号

UINT32 GetRandomIdxMustHit(const CRandomMap& mapRandom);	//Key为返回值，Vlaue为概率

//遍历目录中的文件,文件名为全路径
VOID  ListFile(const CHAR* szDir, const CHAR* szFilePrefixName, const CHAR* szFilePostfixName, BOOL bRecursive, map<INT16, string>& mapFiles);
VOID  ListFile(const CHAR* szDir, const CHAR* szFilePrefixName, const CHAR* szFilePostfixName, BOOL bRecursive, vector<string>& vecFiles);
VOID  ListFile(const CHAR* szDir, const CHAR* szFilePostfixName, BOOL bRecursive, vector<string>& vecFiles);
//遍历目录中的文件夹，文件夹名不是全路径，只是当前名字
VOID  ListDir(const CHAR* szDir, const CHAR* szDirPrefixName, vector<string>& vecDirs);

//获取字符串中的指定前缀后缀包含的部分字符
string GetLimitStr(const CHAR* szSrc, const CHAR* szPrefixName, const CHAR* szPostfixName = NULL);

//检测是否有sql符号字符(串)
BOOL HaveSqlToken(TCHAR* pszSrc);

//过虑(串)
BOOL MisplacedToken(TCHAR* pszSrc, TCHAR*pszDsc);

//检测是否有Utf8mb4字符
BOOL CkHaveUtf8mb4(TCHAR* pszDspName);

//检测是否有特殊字符
BOOL CkHaveSpeCharacter(TCHAR* pszDspName);

//获取初始数
UINT32 GetRdNum();

//初始化随机队列
VOID InitRadomQue(UINT8 byRate, vector<UINT8>& vecRadom);

//四舍五入
INT32 Round( DOUBLE nValue );

UINT32 AddValueByValueType(UINT64 qwSourceValue, UINT32 dwAddValue, UINT8 byAddValueType);

// UINT32转字符串
string dw2str(UINT32 dwNum);

string qw2str(UINT64 qwNum);

string ToLowerByString(const TCHAR* aszString);

//编码
BOOL UrlEncode(const string& strSrc, string& strOut);
//解码
BOOL UrlDecode(string& str);



class CAutoCycle
{
public:
    CAutoCycle(const CHAR* pszMsg);
    ~CAutoCycle();
public:
    VOID PrintCycle(const CHAR* pszMsg = NULL);
private:
    string		m_strMsg;
    UINT32		m_dwStartCycle;
    UINT8		m_byIdx;
};

/**
 * @brief 整数除法,商向上取整
 *
 * @param lDividend [in]
 * @param iDivisor [int]
 * @return int
 * @retval
 */
INT64 DivCeil(INT64 lDividend, INT64 lDivisor);
/**
 * @brief 整数除法,商向下取整
 *
 * @param iDividend [in]
 * @param iDivisor [int]
 * @return int
 * @retval
 */
INT64 DivFloor(INT64 lDividend, INT64 lDivisor);


tstring Time_ToString();
tstring Time_ToString(time_t now);

//设置当前字符集
VOID SetLocateCharSet();

//安全删除宏
#define SAFE_DELETE( p ) if(p){ delete (p); p=NULL; }

//数据库语句字符串的最大长度
const INT32 SQL_STATEMENT_MAX_LEN = 1024;

//把分转换为毛
UINT32 Fen2Mao( UINT32 dwNum );

// [nStart, nEnd]
INT32 Random(INT32 nStart, INT32 nEnd);

//十六进制字符串转ASCII
int HexToAscii(unsigned char *to, unsigned char *from, unsigned int len);
int AsciiToHex(unsigned char *to, unsigned char *from, unsigned int len);

bool IsMobile(TCHAR* pszMobile);
//计算UTF-8字符长度，英文按一个字节，中文按二个字节计算
UINT32 GetUTF8CharWidth( const TCHAR* str );
//UTF-8长度超3个字节 return FALSE
BOOL ChkUTF8Char(  TCHAR* str );
//过滤emoji表情
BOOL  CkEmoji(TCHAR* str);
//检验是否是UTF-8
BOOL IsUTF8(TCHAR* pBuffer);


//检测是否有特殊字符，且长度合法(昵称允许6个中文字符，12个英文)
UINT16 CkHaveSpeCharacterAndValidLen(TCHAR* pszDspName);

class CGenerateJson
{
	struct SoulData {
		UINT16		usSoulId;
		UINT16		usSoulNum;

		SoulData() : usSoulId(0), usSoulNum(0) {
		}
	};

public:
	CGenerateJson();
	~CGenerateJson();
	string GetJson();
	VOID SetGold(UINT32 dwValue);
	VOID SetCoin(UINT32 dwValue);
	VOID SetPhystrength(UINT32 dwValue);
	VOID SetStory(UINT32 dwValue);
	VOID SetScience(UINT32 dwValue);
	VOID SetJingJie(UINT32 dwValue);
	VOID SetDoorsTribute(UINT32 dwValue);
	VOID SetBlueGas(UINT32 dwValue);
	VOID SetFactionFunds(UINT32 dwValue);
	VOID SetPurpleGas(UINT32 dwValue);
	VOID AddItem(UINT16 wKindID, UINT32 dwNum);
	VOID SetResByGm(string strKey, string strValue, vector<string>& rParam);
	VOID SetStudyExp(UINT32 dwValue);
private:
	UINT32 m_dwGold;
	UINT32 m_dwCoin;
	UINT32 m_dwPhystrength;
	UINT32 m_dwScience;
	UINT32 m_dwStory;
	UINT32 m_dwBluegas;
	UINT32 m_dwPruplegas;
	UINT32 m_dwJingjie;
	UINT32 m_dwDoorstribute;
	UINT32 m_dwFactionFunds;
	UINT32 m_dwStudyExp;
	vector<SoulData> m_vSoulData;

	map<UINT32, UINT32> m_mapItem;
};
#define CC_LOOP_DO					do{
#define CC_LOOP_WHILE(condition)		}while(condition);

#define CC_LOOP_WHILE_START(condition)	while(condition){
#define CC_LOOP_WHILE_END				};

#define CC_LOOP_FOR_START(condition)	for(condition){
#define CC_LOOP_FOR_END				};

#define CC_LOOP_BREAK(condition)		if(condition)break;
#define CC_LOOP_CONTINUE(condition)		if(condition)continue;



#define RETURN_OTHER_ERR SYS_CRITICAL(_SDT("[%s: %d]: ERR_COMMON::ID_OTHER_ERR!"), MSG_MARK); \
	return ERR_COMMON::ID_OTHER_ERR;

#define SET_OTHER_ERR(ErrCode) SYS_CRITICAL(_SDT("[%s: %d]: ERR_COMMON::ID_OTHER_ERR!"), MSG_MARK); \
	ErrCode = ERR_COMMON::ID_OTHER_ERR;



#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdmutex.h>

//对象池
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define DECLARE_CLASS_POOL_MGR( ObjType )	\
class ObjType##PoolMgr \
	{\
	public:\
		DECLARE_AUTO_SINGLETON(ObjType##PoolMgr)\
	public:\
		ObjType* Create();\
		VOID  Remove(ObjType*);\
	private:\
		CSDObjectPool<ObjType, CSDMutex>  m_objectPool;\
	};

#define IMPLEMENT_CLASS_POOL_MGR( ObjType )	\
	IMPLEMENT_AUTO_SINGLETON(ObjType##PoolMgr)\
	ObjType* ObjType##PoolMgr::Create()\
	{\
        ObjType *po##ObjType = m_objectPool.Alloc();\
        if(NULL == po##ObjType)\
        {\
            SYS_CRITICAL(_SDT("[%s: %d]: "#ObjType"PoolMgr::alloc "#ObjType" failed!"), MSG_MARK);\
        }\
        return po##ObjType;\
	}\
	VOID ObjType##PoolMgr::Remove(ObjType *pobject)\
	{\
		if(NULL == pobject)\
		{\
			return;\
		}\
		m_objectPool.Free(pobject);\
	}


//建筑功能无关初始化
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define DECLARE_INIT_DESTORY( ObjName, ObjType, ObjArg)	\
	public:\
		ObjType& Get##ObjName() { return *m_po##ObjName; }\
	protected:\
		BOOL		Init##ObjName(ObjArg& stData);	\
		VOID		Destory##ObjName();\
	private:\
		ObjType*	m_po##ObjName;

#define IMPLEMENT_INIT_DESTORY( ObjName, ObjType, ObjArg)	\
	BOOL	CPlayer::Init##ObjName(ObjArg& stData)\
	{\
		m_po##ObjName = NULL;\
		m_vecDestory.push_back(&CPlayer::Destory##ObjName);\
		m_po##ObjName = ObjType##PoolMgr::Instance()->Create();\
		if(NULL == m_po##ObjName)\
		{\
			SYS_CRITICAL(_SDT("[%s: %d]: Init"#ObjName"  Create  PlayerID:%u"), MSG_MARK, m_dwPlayerID);\
			return FALSE;\
		}\
		if(!m_po##ObjName->Init(stData, this))\
		{\
			ObjType##PoolMgr::Instance()->Remove(m_po##ObjName);\
			m_po##ObjName = NULL; \
			SYS_CRITICAL(_SDT("[%s: %d]: Init"#ObjName"  Init  PlayerID:%u"), MSG_MARK, m_dwPlayerID);\
			return FALSE;\
		}\
		return TRUE;\
	}\
	VOID CPlayer::Destory##ObjName()\
	{\
		if (NULL != m_po##ObjName) \
		{\
			ObjType##PoolMgr::Instance()->Remove(m_po##ObjName);\
			m_po##ObjName = NULL;\
		}\
	}

//建筑功能初始化
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define DECLARE_BUILD_INIT_DESTORY( ObjName, ObjType, ObjArg)	\
	public:\
	ObjType* Get##ObjName() { return m_po##ObjName; }\
	ObjType*	New##ObjName(ObjArg& stData);\
	protected:\
	BOOL		Init##ObjName(ObjArg& stData);	\
	VOID		Destory##ObjName();\
	private:\
	ObjType*	m_po##ObjName;

//该版本功能先判断传递的参数是否都为0，用于兼容旧版本用
#define IMPLEMENT_BUILD_INIT_DESTORY( ObjName, ObjType, ObjArg)	\
	BOOL	CPlayer::Init##ObjName(ObjArg& stData)\
	{\
		m_po##ObjName = NULL;\
		m_vecDestory.push_back(&CPlayer::Destory##ObjName);\
		CHAR *pstData = (CHAR*)&stData;\
		BOOL byHaveDataFlag = 0;\
		for (UINT32 idx = 0; idx < sizeof(ObjArg); idx++)\
		{\
			if (0x00 != pstData[idx])\
			{\
				byHaveDataFlag = 1;\
				break;;\
			}\
		}\
		if(0 == byHaveDataFlag)\
		{\
		return TRUE;\
		}\
		stData.byHaveDataFlag = 1;\
		m_po##ObjName = ObjType##PoolMgr::Instance()->Create();\
		if(NULL == m_po##ObjName)\
		{\
		SYS_CRITICAL(_SDT("[%s: %d]: Init"#ObjName" Create  PlayID:%u"), MSG_MARK, m_dwPlayerID);\
		return FALSE;\
		}\
		if(!m_po##ObjName->Init(stData, this))\
		{\
		ObjType##PoolMgr::Instance()->Remove(m_po##ObjName);\
		m_po##ObjName = NULL;		\
		SYS_CRITICAL(_SDT("[%s: %d]: Init"#ObjName" Init  PlayerID:%u"), MSG_MARK, m_dwPlayerID);\
		return FALSE;\
		}\
		return TRUE;\
	}\
	VOID CPlayer::Destory##ObjName()\
	{\
	if (NULL != m_po##ObjName) \
		{\
		ObjType##PoolMgr::Instance()->Remove(m_po##ObjName);\
		m_po##ObjName = NULL;\
		}\
	}\
	ObjType* CPlayer::New##ObjName(ObjArg& stData)\
	{\
		if (NULL != m_po##ObjName)\
		{\
		USR_INFO(_SDT("[%s: %d]: New"#ObjName" Error PlayerID:%u"), MSG_MARK, m_dwPlayerID);\
		return m_po##ObjName;\
		}\
		m_vecDestory.push_back(&CPlayer::Destory##ObjName);\
		m_po##ObjName = ObjType##PoolMgr::Instance()->Create();\
		if(NULL == m_po##ObjName)\
		{\
		SYS_CRITICAL(_SDT("[%s: %d]: New"#ObjName" Create  PlayerID:%u"), MSG_MARK, m_dwPlayerID);\
		return NULL;\
		}\
		if(!m_po##ObjName->Init(stData, this))\
		{\
		ObjType##PoolMgr::Instance()->Remove(m_po##ObjName);\
		m_po##ObjName = NULL;\
		SYS_CRITICAL(_SDT("[%s: %d]: New"#ObjName" Init  PlayerID:%u"), MSG_MARK, m_dwPlayerID);\
		return NULL;\
		}\
		return m_po##ObjName;\
	}

//该版本功能先判断传递的参数是否都为0，用于兼容旧版本用
#define IMPLEMENT_BUILD_INIT_DESTORY_EX( ObjName, ObjType, ObjArg, HaveDataFlag)	\
	BOOL	CPlayer::Init##ObjName(ObjArg& stData)\
	{\
		m_po##ObjName = NULL;\
		m_vecDestory.push_back(&CPlayer::Destory##ObjName);\
		CHAR *pstData = (CHAR*)&stData;\
		BOOL byHaveDataFlag = 0;\
		for (UINT32 idx = 0; idx < sizeof(ObjArg); idx++)\
		{\
			if (0x00 != pstData[idx])\
			{\
				byHaveDataFlag = 1;\
				break;;\
			}\
		}\
		if(0 == byHaveDataFlag)\
		{\
		return TRUE;\
		}\
		stData.HaveDataFlag.byHaveDataFlag = 1;\
		m_po##ObjName = ObjType##PoolMgr::Instance()->Create();\
		if(NULL == m_po##ObjName)\
		{\
		SYS_CRITICAL(_SDT("[%s: %d]: Init"#ObjName"  Create  PlayerID:%u"), MSG_MARK, m_dwPlayerID);\
		return FALSE;\
		}\
		if(!m_po##ObjName->Init(stData, this))\
		{\
		ObjType##PoolMgr::Instance()->Remove(m_po##ObjName);\
		SYS_CRITICAL(_SDT("[%s: %d]: Init"#ObjName"  Init  PlayerID:%u"), MSG_MARK, m_dwPlayerID);\
		return FALSE;\
		}\
		return TRUE;\
	}\
	VOID CPlayer::Destory##ObjName()\
	{\
	if (NULL != m_po##ObjName) \
		{\
		ObjType##PoolMgr::Instance()->Remove(m_po##ObjName);\
		m_po##ObjName = NULL;\
		}\
	}\
	ObjType* CPlayer::New##ObjName(ObjArg& stData)\
	{\
		if (NULL != m_po##ObjName)\
		{\
		USR_INFO(_SDT("[%s: %d]: New"#ObjName" Creat PlayerID:%u"), MSG_MARK, m_dwPlayerID);\
		return m_po##ObjName;\
		}\
		m_vecDestory.push_back(&CPlayer::Destory##ObjName);\
		m_po##ObjName = ObjType##PoolMgr::Instance()->Create();\
		if(NULL == m_po##ObjName)\
		{\
		SYS_CRITICAL(_SDT("[%s: %d]: New"#ObjName" Create faild PlayerID:%u"), MSG_MARK, m_dwPlayerID);\
		return NULL;\
		}\
		if(!m_po##ObjName->Init(stData, this))\
		{\
		ObjType##PoolMgr::Instance()->Remove(m_po##ObjName);\
		m_po##ObjName = NULL;\
		SYS_CRITICAL(_SDT("[%s: %d]: New"#ObjName" Init  faild PlayerID:%u"), MSG_MARK, m_dwPlayerID);\
		return NULL;\
		}\
		return m_po##ObjName;\
	}

#define MG_NET_ERRMSG(_errns_ ,_errno_) ((sizeof(_errns_::ErrMsg) / sizeof(_errns_::ErrMsg[0])) > _errno_ ? _errns_::ErrMsg[_errno_] : ERR_COMMON::STR_OTHER_ERR)

string GetRsponeResult(UINT16 _errno_);
string GetRsponeResult(UINT16 _errno_, map<string, string> *pstrMap);
string GetRsponeResult(UINT16 _errno_, map<string, UINT32> *pstrMap);
string GetRsponeResult(UINT16 _errno_, map<string, UINT64> *pstrMap);
string GetRsponeResult(UINT16 _errno_, vector<string> *pstrVec);

// 错误日志宏
#define	PRINT_ERR_INFO		if(CSDLoggerImpl::NeedLog(LOGLVL_SYS_CRITICAL))CSDLoggerImpl::SysCritical(_SDT("[%s: %d]: "), MSG_MARK);

#endif // #ifndef _UTILITY_H_

