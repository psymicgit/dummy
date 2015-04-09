#include "dbmgr.h"
#include "convert.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/multi_index/detail/bidir_node_iterator.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "dll/sdframework/sdloggerimpl.h"
#include "dll/sdframework/sdutil.h"
#include "../../server/common/server/base64.h"


Convert::Convert()
{
	m_strSql.resize(2048000);
}

Convert::~Convert()
{

}

BOOL Convert::Process()
{
	if( !ConvertPlayerData() )
	{
		return FALSE;
	}

	if( !ConvertPlayerPvpData() )
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL Convert::Init()
{
	SDDBAccount stAccount={0};

	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini("config.ini", pt);
	try
	{
		strcpy( stAccount.m_szCharactSet, pt.get< std::string >( "DB.Charset" ).c_str() );
		strcpy( stAccount.m_szDBName, pt.get< std::string >( "DB.DBName" ).c_str() );
		strcpy( stAccount.m_szHostName, pt.get< std::string >( "DB.IP" ).c_str() );
		strcpy( stAccount.m_szLoginPwd, pt.get< std::string >( "DB.PSW" ).c_str() );
		strcpy( stAccount.m_szLoginName, pt.get< std::string >( "DB.User" ).c_str() );
		stAccount.m_wConnPort=pt.get< UINT16 >( "DB.Port" );
	}
	catch( exception &e )
	{
		cout<<"读取数据库配置文件发生了错误："<<endl;
		cout<<e.what()<<endl;
		return FALSE;
	}

	
	if( !CDBMgr::Instance()->Init(stAccount) )
	{
		return FALSE;
	}


	return TRUE;
}

//逆初始化
BOOL Convert::UnInit()
{

	return TRUE;
}

BOOL Convert::ConvertPlayerData()
{
	SGDP::ISDDBSession*	pDBSession = CDBMgr::Instance()->GetDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	m_strSql.clear();
	sprintf( const_cast<char *>( m_strSql.c_str() ), "truncate table jsplayer" );
	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	string strError;
	pDBSession->ExecuteSqlRs(m_strSql.c_str(), &pRecordSet, 0, &strError);
	if( !strError.empty() )
	{
		SYS_CRITICAL( _SDT("清空jsplayer表发生错误，错误信息:%s"), strError.c_str() );
	}

	if(NULL != pRecordSet)
	{			
		pRecordSet->Release();
		pRecordSet = NULL;			
	}

	CHAR szSql[1024] = {0};
	DT_PLAYER_DATA*	pstDT_PLAYER_DATA = NULL;
	sprintf(szSql, "select * from player");
	pRecordSet = NULL;
	pDBSession->ExecuteSqlRs(szSql, &pRecordSet);
	INT32 nIndex=0;
	INT32 nSuccessNum=0;

	while(pRecordSet->GetRecord())
	{
		++nIndex;
		cout<<"正在处理playerdata第"<<nIndex<<"条"<<endl;
		DT_PLAYER_DATA stDT_PLAYER_DATA={0};

		UINT32 dwLen = pRecordSet->GetFieldLengthByName("BaseData");

		if (sizeof(stDT_PLAYER_DATA.stBaseData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] Index:%d: DataBase(BaseData) Error!"), MSG_MARK, nIndex );
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stBaseData), (VOID*)(pRecordSet->GetFieldValueByName("BaseData")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("Bag");
		if (sizeof(stDT_PLAYER_DATA.stBagData) != dwLen)
		{
			SYS_CRITICAL( _SDT("[%s: %d] [PlayerID:%u] DataBase(Bag) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID );
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stBagData), (VOID*)(pRecordSet->GetFieldValueByName("Bag")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("Build");
		if (sizeof(stDT_PLAYER_DATA.stBuildData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(Build) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID );
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stBuildData), (VOID*)(pRecordSet->GetFieldValueByName("Build")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("Curstate");
		if (sizeof(stDT_PLAYER_DATA.stCurState) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(Curstate) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stCurState), (VOID*)(pRecordSet->GetFieldValueByName("Curstate")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("Curtask");
		if (sizeof(stDT_PLAYER_DATA.stTaskData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(Curtask) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stTaskData), (VOID*)(pRecordSet->GetFieldValueByName("Curtask")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("Equip");
		if (sizeof(stDT_PLAYER_DATA.stBagEquipData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(Equip) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stBagEquipData), (VOID*)(pRecordSet->GetFieldValueByName("Equip")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("Goods");
		if (sizeof(stDT_PLAYER_DATA.stBagGoodsData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(Goods) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stBagGoodsData), (VOID*)(pRecordSet->GetFieldValueByName("Goods")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("Hero");
		if (sizeof(stDT_PLAYER_DATA.stHeroData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(Hero) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stHeroData), (VOID*)(pRecordSet->GetFieldValueByName("Hero")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("CommonBattleRcd");
		if (sizeof(stDT_PLAYER_DATA.stCommonBattleRcd) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(CommonBattleRcd) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stCommonBattleRcd), (VOID*)(pRecordSet->GetFieldValueByName("CommonBattleRcd")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("EliteBattleRcd");
		if (sizeof(stDT_PLAYER_DATA.stEliteBattleRcd) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(EliteBattleRcd) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stEliteBattleRcd), (VOID*)(pRecordSet->GetFieldValueByName("EliteBattleRcd")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("CommonInstanceData");
		if (sizeof(stDT_PLAYER_DATA.stCommonInstanceData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(CommonInstanceData) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stCommonInstanceData), (VOID*)(pRecordSet->GetFieldValueByName("CommonInstanceData")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("EliteInstanceData");
		if (sizeof(stDT_PLAYER_DATA.stEliteInstanceData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(EliteInstanceData) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stEliteInstanceData), (VOID*)(pRecordSet->GetFieldValueByName("EliteInstanceData")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("Locale");
		if (sizeof(stDT_PLAYER_DATA.stLocaleData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(Locale) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stLocaleData), (VOID*)(pRecordSet->GetFieldValueByName("Locale")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("LoginEncourageRecord");
		if (sizeof(stDT_PLAYER_DATA.stLoginEncRecdData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(LoginEncourageRecord) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stLoginEncRecdData), (VOID*)(pRecordSet->GetFieldValueByName("LoginEncourageRecord")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("MoneyTree");
		if (sizeof(stDT_PLAYER_DATA.stMoneyTreeData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(MoneyTree) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stMoneyTreeData), (VOID*)(pRecordSet->GetFieldValueByName("MoneyTree")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("OnlineEncourageRecord");
		if (sizeof(stDT_PLAYER_DATA.stOnlineEncRecdData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(OnlineEncourageRecord) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stOnlineEncRecdData), (VOID*)(pRecordSet->GetFieldValueByName("OnlineEncourageRecord")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("Phystrength");
		if (sizeof(stDT_PLAYER_DATA.stPhystrengthData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(Phystrength) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stPhystrengthData), (VOID*)(pRecordSet->GetFieldValueByName("Phystrength")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("Skill");
		if (sizeof(stDT_PLAYER_DATA.stSkillData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(Skill) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stSkillData), (VOID*)(pRecordSet->GetFieldValueByName("Skill")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("Crop");
		if (sizeof(stDT_PLAYER_DATA.stCropData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(Crop) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stCropData), (VOID*)(pRecordSet->GetFieldValueByName("Crop")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("ScienceTree");
		if (sizeof(stDT_PLAYER_DATA.stScienceTree) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(ScienceTree) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stScienceTree), (VOID*)(pRecordSet->GetFieldValueByName("ScienceTree")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("FewParam");
		if (sizeof(stDT_PLAYER_DATA.stFewParamData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(FewParam) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stFewParamData), (VOID*)(pRecordSet->GetFieldValueByName("FewParam")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("FormationInfo");
		if (sizeof(stDT_PLAYER_DATA.stFormationInfo) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(FormationInfo) Error!"), MSG_MARK, stDT_PLAYER_DATA.stBaseData.dwID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stFormationInfo), (VOID*)(pRecordSet->GetFieldValueByName("FormationInfo")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("LastOrderID");
		dwLen = dwLen > MAX_ORDER_ID_LEN ? MAX_ORDER_ID_LEN : dwLen;
		memcpy(stDT_PLAYER_DATA.szLastOrderID, (VOID*)(pRecordSet->GetFieldValueByName("LastOrderID")), dwLen);


		if( !SavePlayerDataAsJson( stDT_PLAYER_DATA, pRecordSet->GetFieldValueByName("DeviceID"), 
			pRecordSet->GetFieldValueByName("NotifyID") , pRecordSet->GetFieldValueByName( "CreateTime" ),
			pRecordSet->GetFieldValueByName( "UserName" ) ) )
		{
			continue;
		}

		nSuccessNum++;
	}

	m_stPlayerDataProcess.nSuccessNum=nSuccessNum;
	m_stPlayerDataProcess.nMaxNum=nIndex;
	
	return TRUE;
}

BOOL Convert::ConvertPlayerPvpData()
{
	SGDP::ISDDBSession*	pDBSession = CDBMgr::Instance()->GetDBSession();
	
	//先清空表
	m_strSql.clear();
	sprintf( const_cast<char *>( m_strSql.c_str() ), "truncate table jsplayerpvp" );
	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	string strError;
	pDBSession->ExecuteSqlRs( m_strSql.c_str(), &pRecordSet, 0, &strError );
	if( !strError.empty() )
	{
		SYS_CRITICAL( "清空jsplayerpvp表发生错误,错误信息:%s", strError.c_str() );
	}
	
	
	pDBSession->ExecuteSqlRs(m_strSql.c_str(), &pRecordSet, 0, &strError);
	if( !strError.empty() )
	{
		SYS_CRITICAL( _SDT("清空jsplayer表发生错误，错误信息:%s"), strError.c_str() );
	}

	if(NULL != pRecordSet)
	{			
		pRecordSet->Release();
		pRecordSet = NULL;			
	}

	m_strSql.clear();
	sprintf( const_cast<char *>( m_strSql.c_str() ), "select * from playerpvp" );
	DT_PLAYER_DATA stDT_PLAYER_DATA={0};
	pDBSession->ExecuteSqlRs(m_strSql.c_str(), &pRecordSet);
	INT32 nIndex=0;
	INT32 nSuccessNum=0;
	while(pRecordSet->GetRecord())
	{
		nIndex++;
		cout<<"正在处理pvpdata第"<<nIndex<<"条"<<endl;
		UINT32 dwPlayerID=SDAtou( pRecordSet->GetFieldValueByName( "ID" ) );

		UINT32 dwLen = pRecordSet->GetFieldLengthByName("PlunderBase");
		if (sizeof(stDT_PLAYER_DATA.stPlunderBaseData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(PlunderBase) Error!"), MSG_MARK,dwPlayerID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stPlunderBaseData), (VOID*)(pRecordSet->GetFieldValueByName("PlunderBase")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("PlunderBattleLog");
		if (sizeof(stDT_PLAYER_DATA.stPlunderBattleLog) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(PlunderBattleLog) Error!"), MSG_MARK,dwPlayerID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stPlunderBattleLog), (VOID*)(pRecordSet->GetFieldValueByName("PlunderBattleLog")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("PlunderCapture");
		if (sizeof(stDT_PLAYER_DATA.stCaptureData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(PlunderCapture) Error!"), MSG_MARK,dwPlayerID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stCaptureData), (VOID*)(pRecordSet->GetFieldValueByName("PlunderCapture")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("Enemy");
		if (sizeof(stDT_PLAYER_DATA.stEnemyData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(Enemy) Error!"), MSG_MARK,dwPlayerID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stEnemyData), (VOID*)(pRecordSet->GetFieldValueByName("Enemy")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("Race");
		if (sizeof(stDT_PLAYER_DATA.stRaceData) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(Race) Error!"), MSG_MARK,dwPlayerID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stRaceData), (VOID*)(pRecordSet->GetFieldValueByName("Race")), dwLen);

		dwLen = pRecordSet->GetFieldLengthByName("RaceBattleLog");
		if (sizeof(stDT_PLAYER_DATA.stRaceBattleLog) != dwLen)
		{
			SYS_CRITICAL(_SDT("[%s: %d] [PlayerID:%u]: DataBase(RaceBattleLog) Error!"), MSG_MARK,dwPlayerID);
			return FALSE;
		}
		memcpy(&(stDT_PLAYER_DATA.stRaceBattleLog), (VOID*)(pRecordSet->GetFieldValueByName("RaceBattleLog")), dwLen);

		const char *szCreateTime=pRecordSet->GetFieldValueByName( "CreateTime" );
		
		INT32 nVersion=SDAtou( pRecordSet->GetFieldValueByName( "Version" ) );

		string strPlunderBaseData=GetJsonDT_PLUNDER_BASE_DATA( stDT_PLAYER_DATA.stPlunderBaseData ).toStyledString();
		string strPlunderBattleLog=GetJsonDT_PLUNDER_BATTLELOG_DATA_LIST( stDT_PLAYER_DATA.stPlunderBattleLog ).toStyledString();
		string strPlunderCapture=GetJsonDT_CAPTURE_DATA_LIST( stDT_PLAYER_DATA.stCaptureData ).toStyledString();
		string strEnemy=GetJsonDT_ENEMY_DATA_LIST( stDT_PLAYER_DATA.stEnemyData ).toStyledString();
		string strRace=GetJsonDT_RACE_DATA( stDT_PLAYER_DATA.stRaceData ).toStyledString();
		string strRaceBattleLog=GetJsonDT_RACE_BATTLELOG_DATA_LIST( stDT_PLAYER_DATA.stRaceBattleLog ).toStyledString();

		m_strSql.clear();
		sprintf( const_cast<char *>( m_strSql.c_str() ), "insert into jsplayerpvp ("\
			"ID, PlunderBase, PlunderBattleLog, PlunderCapture, Enemy, Race, RaceBattleLog, Version," \
			"UpdateTime, CreateTime) values( '%u', '%s', '%s', '%s', '%s',"
			"'%s', '%s', '%d',"
			"now(), '%s' )", 
			dwPlayerID, strPlunderBaseData.c_str(), strPlunderBattleLog.c_str(), strPlunderCapture.c_str(), strEnemy.c_str(),
			strRace.c_str(), strRaceBattleLog.c_str(), nVersion,
			szCreateTime );

		INT32 nRet=0;
		UINT32 dwErrorCode=0;
		ISDDBRecordSet*pRecordSet = NULL;
		string strErrorDes;
		nRet = pDBSession->ExecuteSqlRs(m_strSql.c_str(), &pRecordSet, &dwErrorCode, &strErrorDes );
		if(NULL != pRecordSet)
		{			
			pRecordSet->Release();
			pRecordSet = NULL;			
		}
		if(nRet < 0)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed!Error:%s PlayerID=%d"), MSG_MARK, strErrorDes.c_str(), dwPlayerID );   
			continue;
		}
		nSuccessNum++;
	}

	m_stPvpDataProcess.nMaxNum=nIndex;
	m_stPvpDataProcess.nSuccessNum=nSuccessNum;
	return TRUE;
}

BOOL Convert::SavePlayerDataAsJson( DT_PLAYER_DATA &oDT_PLAYER_DATA, const string &strDeviceName, 
	const string &strNotifyID, const string &strCreateTime, const string &strUserName )
{
	SGDP::ISDDBSession*	pDBSession = CDBMgr::Instance()->GetDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}

	INT32 nRet=0;
	UINT32 dwErrorCode=0;
	ISDDBRecordSet*pRecordSet = NULL;

	string strBuildDataList=GetJsonDT_BUILD_DATA_LIST( oDT_PLAYER_DATA.stBuildData ).toStyledString();
	//string strTaskData=GetJsonDT_TASK_DATA_LIST( oDT_PLAYER_DATA.stTaskData ).toStyledString();

	CHAR szTaskData[ sizeof(DT_TASK_DATA_LIST) * 2 + 1] = {0};
	auto dwRetCode = pDBSession->EscapeString((CHAR *)&(oDT_PLAYER_DATA.stTaskData),
		sizeof(DT_TASK_DATA_LIST),
		szTaskData, 
		sizeof(szTaskData));
	if(dwRetCode <= 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);   
		return FALSE;
	}

 	//string strDispName=SDUTF82Local( oDT_PLAYER_DATA.stBaseData.aszDispName );
 	//strcpy( oDT_PLAYER_DATA.stBaseData.aszDispName, strDispName.c_str() );
	//strcpy( oDT_PLAYER_DATA.stBaseData.aszDispName, "asdas" );
	
// 	Base64Encode( oDT_PLAYER_DATA.stBaseData.aszDispName, sizeof( oDT_PLAYER_DATA.stBaseData.aszDispName ), 
// 		(UINT8 *)oDT_PLAYER_DATA.stBaseData.aszDispName, strlen( oDT_PLAYER_DATA.stBaseData.aszDispName ) );
	string strBaseData=GetJsonDT_PLAYER_BASE_DATA( oDT_PLAYER_DATA.stBaseData ).toStyledString();
	string strBagEquipDataList=GetJsonDT_BAG_EQUIP_DATA_LST( oDT_PLAYER_DATA.stBagEquipData ).toStyledString();
	string strBagGoodsData=GetJsonDT_BAG_GOODS_DATA_LST( oDT_PLAYER_DATA.stBagGoodsData ).toStyledString();
	string strHeroData=GetJsonDT_HERO_DATA_LIST( oDT_PLAYER_DATA.stHeroData ).toStyledString();

	string strCommonBattleRcd=GetJsonDT_INSTANCE_BATTLE_DATA( oDT_PLAYER_DATA.stCommonBattleRcd ).toStyledString();
	string strEliteBattleRcd=GetJsonDT_INSTANCE_BATTLE_DATA( oDT_PLAYER_DATA.stEliteBattleRcd ).toStyledString();
	string strComonInstanceData=GetJsonDT_INSTANCE_DATA_LIST( oDT_PLAYER_DATA.stCommonInstanceData ).toStyledString();
	string strEliteInstanceData=GetJsonDT_INSTANCE_DATA_LIST( oDT_PLAYER_DATA.stEliteInstanceData ).toStyledString();

	CHAR szLocaleData[ sizeof(DT_LOCALE_DATA) * 2 + 1] = {0};
	dwRetCode = pDBSession->EscapeString((CHAR *)&(oDT_PLAYER_DATA.stLocaleData),
		sizeof(DT_LOCALE_DATA),
		szLocaleData, 
		sizeof(szLocaleData));
	if(dwRetCode <= 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);   
		return FALSE;
	}
	//string strLocaleData=GetJsonDT_LOCALE_DATA( oDT_PLAYER_DATA.stLocaleData ).toStyledString();
	string strLoginEncRecdData=GetJsonDT_LOGIN_ENCOURAGE_RECORD_DATA( oDT_PLAYER_DATA.stLoginEncRecdData ).toStyledString();
	string strMoneyTreeData=GetJsonDT_MONEY_TREE_DATA( oDT_PLAYER_DATA.stMoneyTreeData ).toStyledString();
	string strOnhookRcdData=GetJsonDT_ONHOOK_RCD_DATA( oDT_PLAYER_DATA.stOnhookRcdData ).toStyledString();
	string strOnlineEncRecdData=GetJsonDT_ONLINE_ENCOURAGE_RECORD_DATA( oDT_PLAYER_DATA.stOnlineEncRecdData ).toStyledString();

	string strPhystrengthData=GetJsonDT_PHYSTRENGTH_DATA( oDT_PLAYER_DATA.stPhystrengthData ).toStyledString();
	string strSkillData=GetJsonDT_SKILL_DATA_LIST( oDT_PLAYER_DATA.stSkillData ).toStyledString();
	string strGropData=GetJsonDT_CROP_DATA(oDT_PLAYER_DATA.stCropData).toStyledString();
	string strFewParamData=GetJsonDT_FEW_PARAM_DATA( oDT_PLAYER_DATA.stFewParamData ).toStyledString();

	string strFormationInfo=GetJsonDT_FORMATION_DATA( oDT_PLAYER_DATA.stFormationInfo ).toStyledString();
	string strScienceTree=GetJsonDT_SCIENCE_TREE_DATA( oDT_PLAYER_DATA.stScienceTree ).toStyledString();

	string strB64DispName;
	strB64DispName.resize(USERNAME_LEN*2);
	Base64Encode( const_cast<char *>( strB64DispName.c_str() ), strB64DispName.size(), 
		(UINT8*)oDT_PLAYER_DATA.stBaseData.aszDispName, strlen(oDT_PLAYER_DATA.stBaseData.aszDispName) );

// 	if( !IsPlayerDataExist( oDT_PLAYER_DATA.stBaseData.dwID ) )
// 	{
// 		//在更新玩家数据前，先插入玩家数据
// 		InsertPalyerData( pDBSession, oDT_PLAYER_DATA.stBaseData.dwID, 
// 			strB64DispName.c_str(), strDeviceName.c_str(), strNotifyID.c_str(), &oDT_PLAYER_DATA );
// 	}
	
	
	m_strSql.clear();
	sprintf( const_cast<char *>( m_strSql.c_str() ), "insert into jsplayer( "\
		"BaseData, Bag, "\
		"Build, Curstate, "\
		"Curtask, Equip, Goods, Hero, "\
		"CommonBattleRcd, EliteBattleRcd, CommonInstanceData, EliteInstanceData, "\
		"Locale, LoginEncourageRecord, MoneyTree, OnhookRecord, OnlineEncourageRecord, "\
		"Phystrength, Skill, Crop, FewParam,"\
		"FormationInfo, ScienceTree, DeviceID, NotifyID,"\
		"UpdateTime, CreateTime, Gold, DispName, LastOrderID, ID,"\
		"UserName ) values("\
		"'%s', '%s', '%s', '%s',"\
		"'%s', '%s', '%s', '%s',"\
		"'%s', '%s', '%s', '%s',"\
		"'%s', '%s', '%s', '%s', '%s',"\
		"'%s', '%s', '%s', '%s',"\
		"'%s', '%s', '%s', '%s',"\
		"now(), '%s', '%lld', '%s', '%s', '%u',"
		"'%s'"\
		")",
 
		strBaseData.c_str(), GetJsonDT_BAG_DATA( oDT_PLAYER_DATA.stBagData ).toStyledString().c_str(), 
		strBuildDataList.c_str(), GetJsonDT_STATE_DATA( oDT_PLAYER_DATA.stCurState ).toStyledString().c_str(),

		szTaskData, strBagEquipDataList.c_str(), strBagGoodsData.c_str(), strHeroData.c_str(),

		strCommonBattleRcd.c_str(), strEliteBattleRcd.c_str(), strComonInstanceData.c_str(), strEliteInstanceData.c_str(),

		szLocaleData, strLoginEncRecdData.c_str(), strMoneyTreeData.c_str(), strOnhookRcdData.c_str(), strOnlineEncRecdData.c_str(),

		strPhystrengthData.c_str(), strSkillData.c_str(), strGropData.c_str(), strFewParamData.c_str(),

		strFormationInfo.c_str(), strScienceTree.c_str(), strDeviceName.c_str(), strNotifyID.c_str(),

		strCreateTime.c_str(), oDT_PLAYER_DATA.stBaseData.qwGold, strB64DispName.c_str(), oDT_PLAYER_DATA.szLastOrderID, oDT_PLAYER_DATA.stBaseData.dwID, 
		strUserName.c_str());
	string strErrorDes;
	nRet = pDBSession->ExecuteSqlRs( const_cast<char *>( m_strSql.c_str() ), &pRecordSet, &dwErrorCode, &strErrorDes );
	if(NULL != pRecordSet)
	{
		SYS_CRITICAL( _SDT("[%s: %d]: 更新玩家数据失败，PlayerID=%d Error:%s"), MSG_MARK, 
			oDT_PLAYER_DATA.stBaseData.dwID, strErrorDes.c_str() );   
		pRecordSet->Release();
		pRecordSet = NULL;			
	}

	if(nRet < 0)
	{
		SYS_CRITICAL( _SDT("[%s: %d]: PlayerID=%d !Error:%s"), MSG_MARK, oDT_PLAYER_DATA.stBaseData.dwID, strErrorDes.c_str() );   
		return FALSE;
	}

	return TRUE;
}

BOOL Convert::IsPlayerDataExist( UINT32 dwPlayerID )
{
	m_strSql.clear();
	sprintf( const_cast<char *>( m_strSql.c_str() ), "select * from jsplayer where ID='%d'", dwPlayerID );

	ISDDBRecordSet *pRecordSet = NULL;
	string strError;
	INT32 nRet = CDBMgr::Instance()->GetDBSession()->ExecuteSqlRs( const_cast<char *>( m_strSql.c_str() ), &pRecordSet, 
		NULL, &strError );
	if(NULL != pRecordSet)
	{
		pRecordSet->Release();
		pRecordSet = NULL;			
	}

	if( !strError.empty() )
	{
		USR_IMPORTANT( "从playerdata中查询玩家:%u 失败,错误信息:%s", dwPlayerID, strError.c_str() );
		return FALSE;
	}

	return nRet > 0;
}



BOOL Convert::InsertPalyerData(SGDP::ISDDBSession* pDBSession, UINT32 dwPlayerID, const string &strUserName, 
	const string &strDeviceID, const string &strNotifyID, DT_PLAYER_DATA* pstPlayerData)
{
	string strBaseData=GetJsonDT_PLAYER_BASE_DATA( pstPlayerData->stBaseData ).toStyledString();
	string strBagData=GetJsonDT_BAG_DATA( pstPlayerData->stBagData ).toStyledString();
	string strBuildDataList=GetJsonDT_BUILD_DATA_LIST( pstPlayerData->stBuildData ).toStyledString();
	string strStateData=GetJsonDT_STATE_DATA( pstPlayerData->stCurState ).toStyledString();
	//string strTaskData=GetJsonDT_TASK_DATA_LIST( pstPlayerData->stTaskData ).toStyledString();

	CHAR szTaskData[ sizeof(DT_TASK_DATA_LIST) * 2 + 1] = {0};
	auto dwTaskSize = pDBSession->EscapeString((CHAR *)&(pstPlayerData->stTaskData),
		sizeof(DT_TASK_DATA_LIST),
		szTaskData, 
		sizeof(szTaskData));
	if(dwTaskSize <= 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);   
		return FALSE;
	}

	string strBagEquipDataList=GetJsonDT_BAG_EQUIP_DATA_LST( pstPlayerData->stBagEquipData ).toStyledString();
	string strBagGoodsData=GetJsonDT_BAG_GOODS_DATA_LST( pstPlayerData->stBagGoodsData ).toStyledString();
	string strHeroData=GetJsonDT_HERO_DATA_LIST( pstPlayerData->stHeroData ).toStyledString();

	string strCommonBattleRcd=GetJsonDT_INSTANCE_BATTLE_DATA( pstPlayerData->stCommonBattleRcd ).toStyledString();
	string strEliteBattleRcd=GetJsonDT_INSTANCE_BATTLE_DATA( pstPlayerData->stEliteBattleRcd ).toStyledString();
	string strComonInstanceData=GetJsonDT_INSTANCE_DATA_LIST( pstPlayerData->stCommonInstanceData ).toStyledString();
	string strEliteInstanceData=GetJsonDT_INSTANCE_DATA_LIST( pstPlayerData->stEliteInstanceData ).toStyledString();

	CHAR szLocaleData[ sizeof(DT_LOCALE_DATA) * 2 + 1] = {0};
	auto dwLocaleSize = pDBSession->EscapeString((CHAR *)&(pstPlayerData->stLocaleData),
		sizeof(DT_LOCALE_DATA),
		szLocaleData, 
		sizeof(szLocaleData));
	if(dwLocaleSize <= 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);   
		return FALSE;
	}

	string strLoginEncRecdData=GetJsonDT_LOGIN_ENCOURAGE_RECORD_DATA( pstPlayerData->stLoginEncRecdData ).toStyledString();
	string strMoneyTreeData=GetJsonDT_MONEY_TREE_DATA( pstPlayerData->stMoneyTreeData ).toStyledString();
	string strOnhookRcdData=GetJsonDT_ONHOOK_RCD_DATA( pstPlayerData->stOnhookRcdData ).toStyledString();
	string strOnlineEncRecdData=GetJsonDT_ONLINE_ENCOURAGE_RECORD_DATA( pstPlayerData->stOnlineEncRecdData ).toStyledString();

	string strPhystrengthData=GetJsonDT_PHYSTRENGTH_DATA( pstPlayerData->stPhystrengthData ).toStyledString();
	string strSkillData=GetJsonDT_SKILL_DATA_LIST( pstPlayerData->stSkillData ).toStyledString();
	string strCropData=GetJsonDT_CROP_DATA(pstPlayerData->stCropData).toStyledString();
	string strFewParamData=GetJsonDT_FEW_PARAM_DATA( pstPlayerData->stFewParamData ).toStyledString();

	string strFormationInfo=GetJsonDT_FORMATION_DATA( pstPlayerData->stFormationInfo ).toStyledString();
	string strScienceTree=GetJsonDT_SCIENCE_TREE_DATA( pstPlayerData->stScienceTree ).toStyledString();

	m_strSql.clear();
	sprintf( const_cast<char *>( m_strSql.c_str() ), "insert into jsplayer (ID, DeviceID,NotifyID, UserName, "\
		"BaseData, Bag, Build, Curstate, "\
		"Curtask, Equip, Goods, Hero, "\
		"CommonBattleRcd, EliteBattleRcd, CommonInstanceData, EliteInstanceData, "\
		"Locale, LoginEncourageRecord, MoneyTree, OnhookRecord, OnlineEncourageRecord, "\
		"Phystrength, Skill, Crop, ScienceTree, FewParam, "\
		"FormationInfo, Gold, LastOrderID, "\
		"CreateTime, Version, DispName) "\
		" values( '%u','%s', '%s', '%s', "\
		" '%s', '%s', '%s', '%s', "\
		" '%s', '%s', '%s', '%s', "\
		" '%s', '%s', '%s', '%s', "\
		" '%s', '%s', '%s', '%s', '%s', "\
		" '%s', '%s', '%s', '%s', '%s',"\
		" '%s', '%llu', '', "\
		" now(), %d, '')",  
		dwPlayerID, SDLocal2UTF8(strDeviceID.c_str()).c_str(), SDLocal2UTF8(strNotifyID.c_str()).c_str(), SDLocal2UTF8(strUserName.c_str()).c_str(),
		strBaseData.c_str(), strBagData.c_str(), strBuildDataList.c_str(), strStateData.c_str(),
		szTaskData, strBagEquipDataList.c_str(), strBagGoodsData.c_str(), strHeroData.c_str(), 
		strCommonBattleRcd.c_str(), strEliteBattleRcd.c_str(), strComonInstanceData.c_str(), strEliteInstanceData.c_str(),
		szLocaleData, strLoginEncRecdData.c_str(), strMoneyTreeData.c_str(), strOnhookRcdData.c_str(), strOnlineEncRecdData.c_str(),
		strPhystrengthData.c_str(), strSkillData.c_str(), strCropData.c_str(), strScienceTree.c_str(), strFewParamData.c_str(), 
		strFormationInfo.c_str(), pstPlayerData->stBaseData.qwGold,
		PROTOCOL_VERSION);

	ISDDBRecordSet *pRecordSet = NULL;
	string strError;
	INT32 nRet = pDBSession->ExecuteSqlRs( const_cast<char *>( m_strSql.c_str() ), &pRecordSet, 
		NULL, &strError );
	if(NULL != pRecordSet)
	{
		pRecordSet->Release();
		pRecordSet = NULL;			
	}
	if(nRet < 0)
	{
		SYS_CRITICAL( _SDT("插入玩家数据失败 [%s: %d]: 玩家ID=%d ! error:%s tasksize=%d localesize=%d"), 
			MSG_MARK, dwPlayerID, strError.c_str(), dwTaskSize, dwLocaleSize );   
		return FALSE;
	}

	return TRUE;
}


string Convert::GetResultDes()
{
	stringstream ss;
	ss<<"pvp数据转换："<<m_stPvpDataProcess.nSuccessNum<<"/"<<m_stPvpDataProcess.nMaxNum
		<<"  playerdata数据转换："<<m_stPlayerDataProcess.nSuccessNum<<"/"<<m_stPlayerDataProcess.nMaxNum<<endl;

	return ss.str();
}