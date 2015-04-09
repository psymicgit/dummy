#include "recordmgr.h"
#include "dll/sdu/sdlock.h"
#include "dll/sdframework/sdloggerimpl.h"
#include "dll/sddb/sddb.h"
#include "dll/sdframework/sdutil.h"
#include <db/autosqlbuf.h>
#include "logic/other/singleprammgr.h"
#include "framework/gsconfig.h"
#include "common/server/ngstring.h"
#include <fstream>
#include "common/server/utility.h"
#include "logic/player/player.h"

#include <sstream>

IMPLEMENT_SINGLETON(CRecordMgr);
IMPLEMENT_SINGLETON(CConsumeChartMgr);

// 存储日志库错误sql语句的文件名
#define RECORD_ERR_FILE	"record_error"

// 存储游戏库中的元宝记录表错误sql语句的文件名（元宝记录日志表放在游戏库，而不是放在日志库）
#define GOLD_RECORD_ERR_FILE "goldrecord_error"

namespace recordutil
{
// 数据库表中各vip等级对应的字段
string astrVipField[] =
{
    "vip0",
    "vip1",
    "vip2",
    "vip3",
    "vip4",
    "vip5",
    "vip6",
    "vip7",
    "vip8",
    "vip9",
    "vip10",
    "vip11",
    "vip12",
    "vip12+",
};

// 数据库表中各玩家等级区间相对应的字段
string astrLevelField[] =
{
    "level_1_9",
    "level_10",
    "level_20",
    "level_30",
    "level_40",
    "level_50",
    "level_60",
    "level_70",
    "level_80",
    "level_90",
    "level_100",
    "level_110",
    "level_120+",
};

// 获取今天凌晨0点时间的字符串
string GetTodayZeroTime()
{
    CSDDateTime oDateNow      = SDNow();

    UINT64 qwNow              = oDateNow.GetTimeValue();
    UINT32 dwDayPassSec       = GetDayPassSecond(qwNow);

    // 获取今天凌晨时间
    CSDDateTime dateTodayZero = oDateNow.DecSecond(dwDayPassSec);

    // 转为字符串
    string strTodayZero = SDTimeToString(dateTodayZero);
    return strTodayZero;
}

TCHAR* GetYear()
{
    CSDDateTime t = SDNow();

    static TCHAR temp[20];
    temp[0] = '\0';

    _SDTsnprintf(temp, 20, _SDT("%4d"), t.GetYear());
    return temp;
}
}

using namespace recordutil;

CRecordMgr::CRecordMgr()
    : m_poGameDBSession(NULL)
    , m_poGameDBModule(NULL)
    , m_dwCachedYear(0)
{
    m_bStop = FALSE;

    m_tLastWriteTime = SDTimeSecs();
    m_pDBRecordModule = NULL;
    m_pDBRecordSession = NULL;
    m_poRecordModule = NULL;
}

CRecordMgr::~CRecordMgr()
{
}

BOOL CRecordMgr::Init()
{
    m_pDBRecordModule = SGDP::SDDBGetModule();
    if( NULL == m_pDBRecordModule )
    {
        SYS_CRITICAL(_SDT("[%s: %d]:SDDBGetModule failed"), MSG_MARK);
        return FALSE;
    }

    SGDP::SDDBAccount recordAccount;
    CGSConfig::Instance()->GetRecordDBAccount(recordAccount);
    m_pDBRecordSession = m_pDBRecordModule->GetDBSession( &recordAccount,
                         CGSConfig::Instance()->GetRecordPoolCoreSize(),
                         CGSConfig::Instance()->GetRecordPoolMaxSize() );

    if( NULL == m_pDBRecordSession )
    {
        SYS_CRITICAL(_SDT("[%s: %d]:GetDBSession failed"), MSG_MARK);
        return FALSE;
    }

    m_poRecordModule = SDNew CRecordModule(m_pDBRecordSession);
    if(NULL == m_poRecordModule)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:new CRecordModule failed"), MSG_MARK);
        return FALSE;
    }

    // 创建游戏数据库的模块接口
    m_poGameDBModule = SGDP::SDDBGetModule();
    if( NULL == m_poGameDBModule )
    {
        SYS_CRITICAL(_SDT("[%s: %d]:SDDBGetModule failed"), MSG_MARK);
        return FALSE;
    }

    // 通过模块接口创建与游戏数据库的会话连接，注意：因为是多线程，所以要专门再创建一个连接，而不共用CDBMgr中的游戏数据库连接
    SGDP::SDDBAccount gameDBAccount;
    CGSConfig::Instance()->GetDBAccount(gameDBAccount);
    m_poGameDBSession = m_poGameDBModule->GetDBSession( &gameDBAccount,
        CGSConfig::Instance()->GetPoolCoreSize(),
        CGSConfig::Instance()->GetPoolMaxSize());

    if(NULL == m_poGameDBSession)
    {
        SYS_CRITICAL(_SDT("[%s: %d]:GetDBSession failed"), MSG_MARK);
        return FALSE;
    }

    //初始化资源消耗统计器
    if( FALSE == CConsumeChartMgr::CreateInstance() )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CConsumeChartMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CConsumeChartMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CConsumeChartMgr Init failed"), MSG_MARK);
        return FALSE;
    }

    Start();
    return TRUE;
}


VOID CRecordMgr::UnInit()
{
    Stop();
    Wait();
}

VOID CRecordMgr::Stop()
{
    m_bStop = TRUE;
}

BOOL CRecordMgr::WriteDataToSqlServer()
{
    UINT32 dwNextID = 0;
    string strTable = m_poRecordModule->GetRecordModuleName(dwNextID);
    CNGString strSql;
    strSql += "insert into ";
    strSql += strTable;
    strSql += "(ID, PlayerID, ModuleType, OpType, OpValue, OpTime, AfterOpValue, Param1, Param2, Param3, Param4,  PlayerLevel, VipLevel, Comment) values";

    //加锁
    m_poRecordModule->GetDataMutex().Lock();

    deque<string> &deqData = m_poRecordModule->GetQueue();

    BOOL bHaveData = FALSE;

    if( deqData.size() )
    {
        bHaveData = TRUE;
        strSql += "( ";
        strSql += dwNextID++;
        strSql += ", ";
        strSql += deqData[0];
    }

    for( UINT32 dwIndex = 1; dwIndex < deqData.size(); ++dwIndex )
    {
        strSql += ", ( ";
        strSql += dwNextID++;
        strSql += ", ";
        strSql += deqData[dwIndex];
    }
    deqData.clear();//直接清除，避免保存失败越积越多
    m_poRecordModule->SetCurID(dwNextID);
    //解锁
    m_poRecordModule->GetDataMutex().Unlock();

    if( !bHaveData )
    {
        return FALSE;
    }
    string strError;
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = m_pDBRecordSession->ExecuteSqlRs( strSql.c_str(), &pRecordSet, 0, &strError );
    if( pRecordSet )
    {
        pRecordSet->Release();
    }
    if( nRet < 0 )
    {
        SYS_CRITICAL( _SDT( "[%s %d]exec sql error,error:%s SQL:%s" ), MSG_MARK, strError.c_str(), strSql.c_str() );
        AddSaveErrorSql(strSql.str(), RECORD_ERR_FILE);
        return FALSE;
    }

    return TRUE;
}

BOOL CRecordMgr::SplitNewGoldRecordTable(std::string &strNewTableName)
{
    // 先执行一遍存储过程，确保如果表不存在的话则创建表
    string strError;
    SGDP::ISDDBRecordSet* pRecordSet = NULL;

    CHAR szSql[256] = {0};
    sprintf(szSql, "call SplitNewGoldRecordTable('%s')", strNewTableName.c_str());
    INT32 nRet = m_poGameDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError);
    if(SDDB_NO_RECORDSET != nRet)
    {
        if(!strError.empty())
        {
            SYS_CRITICAL(_SDT("[%s: %d]:call SplitNewGoldRecordTable(%s) failed, errstr:%s"), MSG_MARK, strNewTableName.c_str(), strError.c_str());
            return FALSE;
        }
    }
    if( pRecordSet )
    {
        pRecordSet->Release();
    }

    return TRUE;
}

BOOL CRecordMgr::SaveGoldRecordToDB()
{
    if(m_vecGoldSql.empty())
    {
        return TRUE;
    }

    CSDDateTime t = SDNow();

    /*
        元宝记录比较重要，需要做好备份，所以最好是有做主从，但由于目前日志库没有做从，游戏库则有，做日志库的从的话每台服务器都要专门多加一个mysql实例，比较浪费资源，
        因此，将元宝操作日志表放在游戏库里，而不是放在日志库（2014_3_15日）
    */

    // 获取本年度的表名
    std::string strThisYearTableName = std::string("goldrecord_") + recordutil::GetYear();

    // 检测是否到了新年（目前元宝记录每个表对应一年，所以，每到新年，就要通过存储过程创建好当年的元宝记录表）
    UINT32 dwCurYear = t.GetYear();
    BOOL bNewYear = (m_dwCachedYear != dwCurYear);
    if(bNewYear)
    {
        m_dwCachedYear = dwCurYear;
        SplitNewGoldRecordTable(strThisYearTableName);
    }

    // 经统计，1区中的oprecord关于元宝操作的记录数大概为每个月60万条（2014_3_11日统计），每年大约720万条，按年进行存放还是可行的
    std::string strSql = "insert into ";
    strSql += strThisYearTableName.c_str();
    strSql += "(ID,ZoneID,PlayerID,PlayerLevel,VipLevel,ModuleType,OpType,OpTime,OpRMBGold,OpRMBGoldGift,OpGmGold,OpGameGold,AfterRMBGold,AfterRMBGoldGift,AfterGmGold,AfterGameGold,Param1,Param2,Param3,Param4,`comment`) values ";

    for( UINT32 i = 0; i < m_vecGoldSql.size(); ++i )
    {
        std::string &strSqlElem = m_vecGoldSql[i];

        strSql.append("(");
        strSql.append(strSqlElem);
        strSql.append("),");
    }
    
    // 去掉末尾的","
    strSql.erase(strSql.end() - 1);

    string strError;
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = m_poGameDBSession->ExecuteSqlRs(strSql.c_str(), &pRecordSet, 0, &strError );
    if(pRecordSet)
    {
        pRecordSet->Release();
    }

    m_vecGoldSql.clear();
    if( nRet < 0 )
    {
        SYS_CRITICAL( _SDT( "[%s %d] exec sql error,error:%s SQL:%s" ), MSG_MARK, strError.c_str(), strSql.c_str());
        AddSaveErrorSql(strSql, GOLD_RECORD_ERR_FILE);

        return FALSE;
    }

    return TRUE;
}

VOID CRecordMgr::AddSaveErrorSql( const string& strSql, CHAR szErrFileName[], BOOL bTrunc )
{
    if(strSql.empty())
    {
        return;
    }

    CHAR szDir[1024] = {0};
    sprintf( szDir, "%s../%s", (const CHAR*)SDGetModulePath(), szErrFileName);
    CHAR szFile[1024] = {0};
    sprintf( szFile, "%s/%s.err", szDir, szErrFileName);

    if( !SDDirectoryExists( szDir ) )
    {
        if( !SDCreateDirectory( szDir, TRUE ) )
        {
            SYS_CRITICAL(_SDT("[%s: %d]: create directory failed!path:%s"), MSG_MARK, szDir);
            return;
        }
    }

    std::ofstream outFile;
    if(bTrunc)
    {
        outFile.open( szFile, ios::trunc | ios::in);
    }
    else
    {
        outFile.open( szFile, ios::app | ios::in);
    }

    if( !outFile.is_open() )
    {
        SYS_CRITICAL(_SDT("[%s: %d]: create file failed!path:%s"), MSG_MARK, szFile);
        return;
    }

    outFile.write( strSql.c_str(), strSql.size() );
    outFile << endl;
}


list<string> CRecordMgr::GetErrorSql(CHAR szErrFileName[])
{
    list<string> lstStrSqlInfo;

    CHAR szDir[1024] = {0};
    sprintf( szDir, "%s/../%s", (const CHAR*)SDGetModulePath(), szErrFileName);
    CHAR szFile[1024] = {0};
    sprintf( szFile, "%s/%s.err", szDir, szErrFileName);

    ifstream inFile( szFile );
    if( !inFile.is_open() )
    {
        return lstStrSqlInfo;
    }

    string strSqlRead;
    strSqlRead.resize( MAX_SQL_LEN );
    while( !inFile.eof() )
    {
        inFile.getline( (char *)strSqlRead.c_str(), strSqlRead.size() );
        if(0 == strSqlRead[0])
        {
            continue;
        }
        lstStrSqlInfo.push_back(strSqlRead);
    }

    return lstStrSqlInfo;
}


VOID  CRecordMgr::ThrdProc()
{
    while(!m_bStop)
    {
        const UINT64 qwNow = SDTimeSecs();
        if(qwNow - m_tLastWriteTime > CSinglePramMgr::Instance()->GetUpdateRecordDataToSqlInterval() )
        {
            WriteDataToSqlServer();
            SaveGoldRecordToDB();

            m_tLastWriteTime = qwNow;//5倍时间检查一次是否有错误文件，有则更新
            SGDP::SDSleep( ms_dwSleepTime );
            //ExeErrFileSql(); //未进行性能参数，暂时注释
        }

        // 存储资源消耗统计表
        CConsumeChartMgr *poRecordSumUpMgr = CConsumeChartMgr::Instance();
        if(qwNow - poRecordSumUpMgr->GetLastSaveTime() > CSinglePramMgr::Instance()->GetSaveOpHistoryToDBInterval())
        {
            poRecordSumUpMgr->SaveToDB();
            poRecordSumUpMgr->ResetLastSaveTime();
        }

        //强制休眠一秒钟再检测一次
        SDSleep(ms_dwSleepTime);
    }

    WriteDataToSqlServer();
    SaveGoldRecordToDB();

    CConsumeChartMgr::Instance()->SaveToDB();

    m_pDBRecordModule->Close( m_pDBRecordSession );
    m_pDBRecordModule->Release();
    m_pDBRecordSession = NULL;
    m_pDBRecordModule = NULL;


    m_poGameDBModule->Close( m_poGameDBSession);
    m_poGameDBModule->Release();
    m_poGameDBSession = NULL;
    m_poGameDBModule = NULL;
}

VOID CRecordMgr::ExeErrFileSql()
{
    list<string> lstSql = CRecordMgr::Instance()->GetErrorSql(RECORD_ERR_FILE);
    for(list<string>::iterator itr = lstSql.begin(); itr != lstSql.end(); )
    {
        string strError;
        SGDP::ISDDBRecordSet* pRecordSet = NULL;
        string strSql = *itr;
        if(strSql.empty())
        {
            lstSql.erase(itr++);
            continue;
        }
        INT32 nRet = m_pDBRecordSession->ExecuteSqlRs( strSql.c_str(), &pRecordSet, 0, &strError );
        if( pRecordSet )
        {
            pRecordSet->Release();
        }
        if( nRet < 0 )
        {
            SYS_CRITICAL( _SDT( "[%s %d]exec sql error2,error:%s SQL:%s" ), MSG_MARK, strError.c_str(), strSql.c_str() );
            itr++;
        }
        else
        {
            lstSql.erase(itr++);
        }
    }
    for(list<string>::iterator itr = lstSql.begin(); itr != lstSql.end(); itr++)
    {
        if(itr == lstSql.begin())
        {
            AddSaveErrorSql(*itr, RECORD_ERR_FILE, TRUE);
        }
        else
        {
            AddSaveErrorSql(*itr, RECORD_ERR_FILE);
        }
    }
    if(lstSql.empty())
    {
        AddSaveErrorSql("", RECORD_ERR_FILE, TRUE);
    }
}

VOID CRecordMgr::ExeGoldErrFileSql()
{
    list<string> lstSql = CRecordMgr::Instance()->GetErrorSql(GOLD_RECORD_ERR_FILE);
    for(list<string>::iterator itr = lstSql.begin(); itr != lstSql.end(); )
    {
        string strError;
        SGDP::ISDDBRecordSet* pRecordSet = NULL;
        string strSql = *itr;
        if(strSql.empty())
        {
            lstSql.erase(itr++);
            continue;
        }
        INT32 nRet = m_poGameDBSession->ExecuteSqlRs( strSql.c_str(), &pRecordSet, 0, &strError );
        if( pRecordSet )
        {
            pRecordSet->Release();
        }
        if( nRet < 0 )
        {
            SYS_CRITICAL( _SDT( "[%s %d]exec sql error2,error:%s SQL:%s" ), MSG_MARK, strError.c_str(), strSql.c_str() );
            itr++;
        }
        else
        {
            lstSql.erase(itr++);
        }
    }
    for(list<string>::iterator itr = lstSql.begin(); itr != lstSql.end(); itr++)
    {
        if(itr == lstSql.begin())
        {
            AddSaveErrorSql(*itr, GOLD_RECORD_ERR_FILE, TRUE);
        }
        else
        {
            AddSaveErrorSql(*itr, GOLD_RECORD_ERR_FILE);
        }
    }
    if(lstSql.empty())
    {
        AddSaveErrorSql("", GOLD_RECORD_ERR_FILE, TRUE);
    }
}

CConsumeChartMgr::CConsumeChartMgr()
{
    m_qwLastSaveTime = SDTimeSecs();
}

BOOL CConsumeChartMgr::Init()
{
    return LoadFromDB();
}

// 统计玩家获得或消费资源的情况
VOID CConsumeChartMgr::CountUp(UINT16 wPlayerLevel, BYTE byVipLevel, UINT16 wModuleType, UINT16 wOpType, UINT32 dwOpValue, CSDDateTime &oOpTime)
{
    UINT64 qwTime       = oOpTime.GetTimeValue();
    UINT64 qwTodayZero  = qwTime - GetDayPassSecond(qwTime); // 今天凌晨0点时间
    UINT64 qwOptKey     = CLogicFunc::Get3232Key(wModuleType, wOpType); // 键值 = 资源类型 + 对应操作，如：增加元宝 + 充值、消耗元宝 + 强化装备

    wPlayerLevel  = MIN(RECORD_CHART_MAX_LEVEL, wPlayerLevel); // 现在玩家最高等级是120级，但因为玩家最大等级可能会越来越大，所以120级以上的直接归为120+级
    byVipLevel    = MIN(RECORD_CHART_MAX_VIP + 1, byVipLevel); // 现在最高vip等级是12，据说之后可能会扩展到20级，不过vip12级以后的都归为12+级
    wPlayerLevel /= 10; // 消耗的资源按玩家等级区间来划分,[1 - 9], [10, 19].... 每10级为一个区间

    // 上锁以防止SaveToDB方法读到脏数据
    CSDLock<CSDMutex> lock(m_mutex);

    // 统计资源数据
    ConsumeChartMap &mapToDay   = m_mapRecord[qwTodayZero];
    ConsumeRow &recordInfo = mapToDay[qwOptKey];

    recordInfo.wModuleType                  = wModuleType;
    recordInfo.wOpType                      = wOpType;
    recordInfo.vecLevelSumUp[wPlayerLevel] += dwOpValue;
    recordInfo.vecVipSumUp[byVipLevel]     += dwOpValue;
    recordInfo.qwOpTotal                   += dwOpValue;
    recordInfo.dwOpCnt                     += 1;
    recordInfo.dwMaxOpVal                   = MAX(dwOpValue, recordInfo.dwMaxOpVal);
    recordInfo.dwMinOpVal                   = ((recordInfo.dwMinOpVal == 0) ? dwOpValue : MIN(dwOpValue, recordInfo.dwMinOpVal));
    recordInfo.bSavedToDB = FALSE;
}

// 从数据库中取出今天的资源消耗统计数据
BOOL CConsumeChartMgr::LoadFromDB()
{
    ISDDBSession *poDBSession = CRecordMgr::Instance()->GetRecordDBSession();
    if(NULL == poDBSession)
    {
        return FALSE;
    }

    UINT64 qwNow = SDNow().GetTimeValue();
    UINT64 qwTodayZero = qwNow - GetDayPassSecond(qwNow); // 今天凌晨0点

    // 取出今天的资源统计数据
    string strQuerySql = "select * from ophistory where Date = '" + recordutil::GetTodayZeroTime() + "'";

    SGDP::ISDDBRecordSet* pRes = NULL;
    INT32 nRet = poDBSession->ExecuteSqlRs(strQuerySql.c_str(), &pRes);
    if(SDDB_HAS_RECORDSET != nRet && SDDB_NO_RECORDSET != nRet)
    {
        return FALSE;
    }

    if(NULL == pRes)
    {
        return TRUE;
    }

    CSDLock<CSDMutex> lock(m_mutex);

    ConsumeChartMap &mapTodayHistory = m_mapRecord[qwTodayZero];
    while(pRes->GetRecord()) // 当有行结果时
    {
        // 把记录存到今天的资源统计数据
        UINT16 wModuleType        = SGDP::SDAtou(pRes->GetFieldValueByName("ModuleType"));
        UINT16 wOpType            = SGDP::SDAtou(pRes->GetFieldValueByName("OpType"));
        UINT64 qwOpKey            = CLogicFunc::Get3232Key(wModuleType, wOpType);

        ConsumeRow &stHistory = mapTodayHistory[qwOpKey];
        stHistory.qwOpTotal       = SGDP::SDAtou(pRes->GetFieldValueByName("OpTotal"));
        stHistory.dwOpCnt         = SGDP::SDAtou(pRes->GetFieldValueByName("OpCnt"));
        stHistory.dwMaxOpVal      = SGDP::SDAtou(pRes->GetFieldValueByName("MaxOpVal"));
        stHistory.dwMinOpVal      = SGDP::SDAtou(pRes->GetFieldValueByName("MinOpVal"));

        // 读取vip等级统计
        for(int i = 0; i < RECORD_CHART_VIP_SECTION_CNT; ++i)
        {
            stHistory.vecVipSumUp[i] = SGDP::SDAtou(pRes->GetFieldValueByName(astrVipField[i].c_str()));
        }

        // 读取玩家等级统计
        for(int i = 0; i < RECORD_CHART_LEVEL_SECTION_CNT; ++i)
        {
            stHistory.vecLevelSumUp[i] = SGDP::SDAtou(pRes->GetFieldValueByName(astrLevelField[i].c_str()));
        }
    }

    pRes->Release();
    return TRUE;
}

// 返回的结果形如：replace into ophistory(Date, ModuleType, OpType, OpValue....., vip0, vip1, vip2, ......, level_1_9, level_10, .......) values
string& CConsumeChartMgr::BuildInsertFieldSql()
{
    static string strSql;
    if(strSql != "")
    {
        return strSql;
    }

    strSql += "replace into ophistory(Date,ModuleType,OpType,OpTotal,OpCnt,MaxOpVal,MinOpVal,";

    // 拼接出vip等级统计数据
    for(BYTE i = 0; i < RECORD_CHART_VIP_SECTION_CNT; ++i)
    {
        string& strVipField = astrVipField[i];

        strSql += "`";
        strSql += strVipField;
        strSql += "`,";
    }

    // 拼接出level等级统计数据
    for(BYTE i = 0; i < RECORD_CHART_LEVEL_SECTION_CNT; ++i)
    {
        string& strLevelField = astrLevelField[i];

        strSql += "`";
        strSql += strLevelField;
        strSql += "`,";
    }

    strSql.erase(strSql.end() - 1);

    strSql += ") values ";
    return strSql;
}

// 返回的字符串结果形如：("2013/12/10 00:00:00", 4, 5, 0, 0, 0.......), ("2013/12/10 00:00:00", 4, 5, 0, 0, 0.......) .....
string CConsumeChartMgr::BuildInsertValueSql()
{
    // std::stringstream strSql;
    CNGString strSql;

    // 有另一种方法来减少上锁时间，给当前数据生成一份快照，针对快照操作就行，只不过不一定哪个资源消耗更少，算了
    CSDLock<CSDMutex> lock(m_mutex);

    // 取出各天的资源消耗汇总数据（其实也就只有今天、昨天）
    for(Day2ConsumeChartMap::iterator itr = m_mapRecord.begin(); itr != m_mapRecord.end(); ++itr)
    {
        // 取出当天数据
        ConsumeChartMap &mapHistory = itr->second;
        if(mapHistory.empty())
        {
            continue;
        }

        strSql.m_strData.reserve(1024);

        // 获取当天凌晨时间
        UINT64 qwDayZero = itr->first;

        CSDDateTime dateDayZero(qwDayZero);
        string strDayZero = SDTimeToString(dateDayZero);

        // 针对当天数据做sql拼接
        for(ConsumeChartMap::iterator itr = mapHistory.begin(); itr != mapHistory.end(); ++itr)
        {
            ConsumeRow &stHistory = itr->second;
            if(stHistory.bSavedToDB)
            {
                continue;
            }

            strSql += "('";
            strSql += strDayZero;
            strSql += "',";
            strSql += stHistory.wModuleType;
            strSql += ",";
            strSql += stHistory.wOpType;
            strSql += ",";
            strSql += stHistory.qwOpTotal;
            strSql += ",";
            strSql += stHistory.dwOpCnt;
            strSql += ",";
            strSql += stHistory.dwMaxOpVal;
            strSql += ",";
            strSql += stHistory.dwMinOpVal;
            strSql += ",";

            // 更新vip汇总数据
            for(BYTE i = 0; i < RECORD_CHART_VIP_SECTION_CNT; ++i)
            {
                strSql += stHistory.vecVipSumUp[i];
                strSql += ",";
            }

            // 更新玩家等级区间汇总数据
            for(BYTE i = 0; i < RECORD_CHART_LEVEL_SECTION_CNT; ++i)
            {
                strSql += stHistory.vecLevelSumUp[i];
                strSql += ",";
            }

            // 去掉末尾的", "
            string &strInnerSql = strSql.m_strData;
            if(!strInnerSql.empty())
            {
                strInnerSql.erase(strInnerSql.end() - 1);
            }

            strSql += "),";
        }
    }

    // 去掉末尾的","
    string &strInnerSql = strSql.m_strData;
    if(!strInnerSql.empty())
    {
        strInnerSql.erase(strInnerSql.end() - 1);
    }

    return strSql.str();
}

void CConsumeChartMgr::ResetLastSaveTime()
{
    m_qwLastSaveTime = SDTimeSecs();
}

// 成功后把所有的未同步标志设为已同步，并清除昨天的统计记录
void CConsumeChartMgr::OnSaveDBSuccess()
{
    // 清除昨天的历史数据
    ClearYesterday();

    CSDLock<CSDMutex> lock(m_mutex);

    // 取出各天的资源消耗汇总数据（也就今天、昨天了）
    for(Day2ConsumeChartMap::iterator itr = m_mapRecord.begin(); itr != m_mapRecord.end(); ++itr)
    {
        // 取出当天数据
        ConsumeChartMap &mapHistory = itr->second;
        if(mapHistory.empty())
        {
            continue;
        }

        // 针对当天数据做sql拼接
        for(ConsumeChartMap::iterator itr = mapHistory.begin(); itr != mapHistory.end(); ++itr)
        {
            ConsumeRow &stHistory = itr->second;
            stHistory.bSavedToDB = TRUE;
        }
    }
}

// 把消费统计存入数据库表（仅更新变动过的数据）
BOOL CConsumeChartMgr::SaveToDB()
{
    // 统计了一下，截止目前2013_12_10，现在对资源的操作类型共有276种，换言之，每天最多产生276条资源消耗统计记录，就算每天300条，一年下来顶多也就10万条，所以不用拆表
    ISDDBSession *poDBSession = CRecordMgr::Instance()->GetRecordDBSession();
    if(NULL == poDBSession)
    {
        return FALSE;
    }

    // 生成更新语句，格式为("2013/12/10 00:00:00", 4, 5, 0, 0, 0.......), ("2013/12/10 00:00:00", 4, 5, 0, 0, 0.......), ......
    string strValueSql = BuildInsertValueSql();
    if(strValueSql.empty())
    {
        // 当前没有需要保存的数据
        return TRUE;
    }

    // 生成插入字段语句，格式为replace into ophistory(Date, ModuleType, OpType, OpValue, vip0, vip1, vip2, ......, level_1_9, level_10, .......) values
    string &strFieldSql = BuildInsertFieldSql();
    string strSql = strFieldSql + strValueSql;

    string strError;
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = poDBSession->ExecuteSqlRs(strSql.c_str(), &pRecordSet, 0, &strError);
    if(pRecordSet)
    {
        pRecordSet->Release();
    }

    if(nRet < 0)
    {
        SYS_CRITICAL( _SDT( "[%s %d] CResourceChartMgr SaveToDB error,error:%s SQL:%s" ), MSG_MARK, strError.c_str(), strSql.c_str());
        return FALSE;
    }

    // 成功后把所有的未同步标志设为已同步，并清空昨天的记录
    OnSaveDBSuccess();
    return TRUE;
}

// 清除昨天的统计记录
void CConsumeChartMgr::ClearYesterday()
{
    CSDLock<CSDMutex> lock(m_mutex);

    // 检测昨天的历史记录在不在，map里只可能有2种情况：1. 今天和昨天的统计一起存在  2. 只有今天的统计记录
    if(m_mapRecord.size() <= 1)
    {
        return;
    }

    UINT64 qwNow             = SDNow().GetTimeValue();
    const UINT64 qwTodayZero = qwNow - GetDayPassSecond(qwNow);

    // 直接把不是今天的消耗统计给清掉，反正已经保存到数据库了
    for(Day2ConsumeChartMap::iterator itr = m_mapRecord.begin(); itr != m_mapRecord.end();)
    {
        const UINT64 qwDayZero = itr->first;
        if(qwTodayZero != qwDayZero)
        {
            m_mapRecord.erase(itr++);
            continue;
        }

        ++itr;
    }
}

BOOL CRecordMgr::RecordGold(UINT16 wZoneID, UINT32 dwPlayerID, UINT16 wPlayerLevel, UINT16 wVipLevel, ERecordModule wModuleType, UINT16 wOpType,
    UINT32 dwOpRMBGold, UINT32 dwOpRMBGoldGift, UINT32 dwOpGmGold, UINT32 dwOpGameGold, 
    UINT32 dwAfterRMBGold, UINT32 dwAfterRMBGoldGift, UINT32 dwAfterGmGold, UINT64 qwAfterGameGold, 
    UINT64 qwParam1/* = 0 */, UINT64 qwParam2/* = 0 */, UINT64 qwParam3/* = 0 */, UINT64 qwParam4/* = 0 */, string strComment/* = "" */)
{
    if(0 == (dwOpRMBGold | dwOpRMBGoldGift | dwOpGmGold | dwOpGameGold))
    {
        return FALSE;
    }

    CSDDateTime oNow = SDNow();
    string strOpTime = SDTimeToString(oNow);

    CNGString ss;
    //必须要跟数据库的列对齐，如果数据库结构修改，代码也要跟随修改
    ss += "NULL,"; // 对应于ID字段，ID是auto increment字段
    ss += wZoneID;
    ss += ",";
    ss += dwPlayerID;
    ss += ",";
    ss += wPlayerLevel;
    ss += ",";
    ss += wVipLevel;
    ss += ",";
    ss += wModuleType;
    ss += ",";
    ss += wOpType;
    ss += ",'";
    ss += strOpTime;
    ss += "',";
    ss += dwOpRMBGold;
    ss += ",";
    ss += dwOpRMBGoldGift;
    ss += ",";
    ss += dwOpGmGold;
    ss += ",";
    ss += dwOpGameGold;
    ss += ",";
    ss += dwAfterRMBGold;
    ss += ",";
    ss += dwAfterRMBGoldGift;
    ss += ",";
    ss += dwAfterGmGold;
    ss += ",";
    ss += qwAfterGameGold;
    ss += ",";
    ss += qwParam1;
    ss += ",";
    ss += qwParam2;
    ss += ",";
    ss += qwParam3;
    ss += ",";
    ss += qwParam4;
    ss += ",'";
    //过虑是否有特殊数据库关键字
    if(FALSE == HaveSqlToken((CHAR*)strComment.c_str()))
    {
        ss += strComment;
    }
    ss += "'";
    m_vecGoldSql.push_back(ss.str());

    UINT32 dwOpGold = dwOpRMBGold + dwOpRMBGoldGift + dwOpGmGold + dwOpGameGold;
    CConsumeChartMgr::Instance()->CountUp(wPlayerLevel, (BYTE)wVipLevel, wModuleType, wOpType, dwOpGold, oNow);
    return TRUE;
}

BOOL CRecordMgr::RecordInfo(UINT32 dwPlayerID, UINT16 wModuleType, UINT16 wOpType, UINT32 dwOpValue, UINT64 qwAfterOpValue,
                            UINT16 wPlayerLevel, UINT16 wVipLevel, UINT64 qwParam1, UINT64 qwParam2, UINT64 qwParam3, UINT64 qwParam4, string strComment)
{
	if ( ERM_DECFACTIONFUNDS != wModuleType && ERM_GVG_ACTIVITY != wModuleType)
	{
		if(0 == dwOpValue)
		{
			return FALSE;
		}
	}

    CSDDateTime oNow = SDNow();
    string strOpTime = SDTimeToString(oNow);

    CNGString ss;
    //必须要跟数据库的列对齐，如果数据库结构修改，代码也要跟随修改
    //CSDDateTime dt(SDTimeSecs());
    ss += dwPlayerID;
    ss += ",";
    ss += wModuleType;
    ss += ",";
    ss += wOpType;
    ss += ",";
    ss += dwOpValue;
    ss += ",'";
    ss += strOpTime;
    ss += "',";
    ss += qwAfterOpValue;
    ss += ",";
    ss += qwParam1;
    ss += ",";
    ss += qwParam2;
    ss += ",";
    ss += qwParam3;
    ss += ",";
    ss += qwParam4;
    ss += ",";
    ss += wPlayerLevel;
    ss += ",";
    ss += wVipLevel;
    ss += ",'";
	//过虑是否有特殊数据库关键字
	if(HaveSqlToken((CHAR*)strComment.c_str()))
	{
		ss += "";
	}
	else
	{
		ss += strComment;
	}
    ss += "')";
    m_poRecordModule->AddData( ss.str() );

    CConsumeChartMgr::Instance()->CountUp(wPlayerLevel, (BYTE)wVipLevel, wModuleType, wOpType, dwOpValue, oNow);
    return TRUE;
}