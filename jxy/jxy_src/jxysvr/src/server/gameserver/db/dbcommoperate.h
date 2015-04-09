#ifndef _DB_COMM_OPERATE_H
#define _DB_COMM_OPERATE_H

#include <sdutil.h>
#include <common/server/utility.h>
#include "protocol/server/protocommondata.h"
#include <db/basedbccmd.h>
#include <sdtime.h>

using namespace SGDP;

#define EscapeData(pszBuff, objStruct, szData )\
{\
	INT32 nRet = objStruct.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
	if(nRet <= 0)\
	{\
	SYS_CRITICAL(_SDT("[%s: %d]: %s.EncodeSaveData  failed"), MSG_MARK, #objStruct );\
	return FALSE;\
	}\
	UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
	nRet,\
	szData,\
	sizeof(szData));\
	if(dwRetCode <= 0)\
	{\
	SYS_CRITICAL(_SDT("[%s: %d]: %s EscapeString failed!"), MSG_MARK, #objStruct);\
	return FALSE;\
	}\
}

#define GetEncodeDataFromDB( szFieldName, objStruct, dwPlayerID )\
{\
	UINT32 dwLen = recordSet.GetFieldLengthByName( szFieldName );\
	if (0 != dwLen)\
	{\
		if( -1 == objStruct.DecodeSaveData( recordSet.GetFieldValueByName(szFieldName), dwLen ) )\
		{\
			SYS_CRITICAL( _SDT("[%s: %d] [PlayerID:%u]: %s.DecodeSaveData Failed!"), MSG_MARK, dwPlayerID, #objStruct );\
			return FALSE;\
		}\
	}\
}

#define GetEncodeDataFromDB2( objStruct, dwPlayerID )\
{\
	UINT32 dwLen = pRecordSet->GetFieldLengthByName( "Value" );\
	if (0 != dwLen)\
	{\
	if( -1 == objStruct.DecodeSaveData( pRecordSet->GetFieldValueByName("Value"), dwLen ) )\
		{\
		SYS_CRITICAL( _SDT("[%s: %d] [PlayerID:%u]: %s.DecodeSaveData Failed!"), MSG_MARK, dwPlayerID, #objStruct );\
		return FALSE;\
		}\
	}\
}

#define GetEncodeDataFromDB3( FieldName, objStruct, dwPlayerID )\
{\
	UINT32 dwLen = pRecordSet->GetFieldLengthByName( FieldName );\
	if (0 != dwLen)\
	{\
	if( -1 == objStruct.DecodeSaveData( pRecordSet->GetFieldValueByName(FieldName), dwLen ) )\
		{\
		SYS_CRITICAL( _SDT("[%s: %d] [PlayerID:%u]: %s.DecodeSaveData Failed!"), MSG_MARK, dwPlayerID, #objStruct );\
		return FALSE;\
		}\
	}\
}

// 非玩家的解码宏
#define DecodeSaveDataFromDB( FieldName, objStruct)\
{\
	UINT32 dwLen = pRecordSet->GetFieldLengthByName( FieldName );\
	if (0 != dwLen)\
	{\
	if( -1 == objStruct.DecodeSaveData( pRecordSet->GetFieldValueByName(FieldName), dwLen ) )\
		{\
		SYS_CRITICAL( _SDT("[%s: %d] %s.DecodeSaveData Failed!"), MSG_MARK, #objStruct );\
		return FALSE;\
		}\
	}\
}



#define GetDateTime2Stamp( TimeValue, FieldName)\
{\
	CHAR szTime[256] = {0};\
	memset(szTime, 0x00, sizeof(szTime));\
	UINT32 dwLen = pRecordSet->GetFieldLengthByName(FieldName);\
	dwLen = dwLen > 255 ? 255 : dwLen;\
	memcpy(szTime, (VOID*)(pRecordSet->GetFieldValueByName(FieldName)), dwLen);\
	CSDDateTime stDateTime;\
	tstring    strDateTime = szTime;\
	if(SDTimeFromString(strDateTime, stDateTime))\
	{\
		TimeValue = stDateTime.GetTimeValue();\
	}\
	else\
	{\
		TimeValue = SDTimeSecs();\
	}\
}

#define ENCODEDATA(FieldName, MaxFieldLen)\
memset(pszBuff, 0, MAX_SQL_LEN);\
nRet = pstPlayerData->st##FieldName.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
if(nRet <= 0)\
{\
	SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);\
	return FALSE;\
}\
CHAR sz##FieldName[MaxFieldLen * 2 + 1] = {0};\
{\
	UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
		nRet,\
		sz##FieldName,\
		sizeof(sz##FieldName));\
	if(dwRetCode <= 0)\
	{\
		SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);\
		return FALSE;\
	}\
}\
strcat(pszSql, ", ");\
strcat(pszSql, #FieldName);\
strcat(pszSqlValues,", '");\
strcat(pszSqlValues, sz##FieldName);\
strcat(pszSqlValues,"'");



#define ENCODEDATA2(objStruct, FieldName, Key, MaxFieldLen)\
	nRet = objStruct.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
	if(nRet <= 0)\
{\
	SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);\
	return FALSE;\
}\
	CHAR sz##FieldName[MaxFieldLen * 2 + 1] = {0};\
{\
	UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
	nRet,\
	sz##FieldName,\
	sizeof(sz##FieldName));\
	if(dwRetCode <= 0)\
	{\
    SYS_CRITICAL(_SDT("[%s: %d]: EscapeString %s failed, FieldName = %s!"), MSG_MARK, pszBuff, #FieldName);\
	return FALSE;\
	}\
}\
sprintf(pszBuff, ", ( '%u', '%u', '%s', now() )", dwPlayerID, Key, sz##FieldName);\
strcat(pszSql, pszBuff);


#define ENCODEDATA22(objStruct, FieldName, Key, MaxFieldLen)\
	nRet = objStruct.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
	if(nRet <= 0)\
{\
	SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);\
	return FALSE;\
}\
	CHAR sz##FieldName[MaxFieldLen * 2 + 1] = {0};\
{\
	UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
	nRet,\
	sz##FieldName,\
	sizeof(sz##FieldName));\
	if(dwRetCode <= 0)\
	{\
    SYS_CRITICAL(_SDT("[%s: %d]: EscapeString %s failed, FieldName = %s!"), MSG_MARK, pszBuff, #FieldName);\
	return FALSE;\
	}\
}\
sprintf(pszBuff, "( '%u', '%u', '%s', now() )", dwPlayerID, Key, sz##FieldName);\
strcat(pszSql, pszBuff);



#define ENCODEDATA3(objStruct, FieldName, MaxFieldLen)\
	nRet = objStruct.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
	if(nRet <= 0)\
{\
	SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);\
	return FALSE;\
}\
	CHAR sz##FieldName[MaxFieldLen * 2 + 1] = {0};\
{\
	UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
	nRet,\
	sz##FieldName,\
	sizeof(sz##FieldName));\
	if(dwRetCode <= 0)\
	{\
    SYS_CRITICAL(_SDT("[%s: %d]: EscapeString %s failed, FieldName = %s!"), MSG_MARK, pszBuff, #FieldName);\
	return FALSE;\
	}\
}\
strcat(pszSql, ", ");\
strcat(pszSql, #FieldName);\
strcat(pszSqlValues,", '");\
strcat(pszSqlValues, sz##FieldName);\
strcat(pszSqlValues,"'");


#define ENCODEDATA_SAVE2(objStruct, Key)\
if( m_oPlayerSaveInfo.bSaveAll || gsapi::GetDBMgr()->HaveChanged(dwPlayerID, EDT_PlayerExt, Key) )\
{\
	nRet = objStruct.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
	if(nRet <= 0)\
	{\
		SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);\
		return FALSE;\
	}\
		memset(pszSqlValues, 0, MAX_SQL_LEN);\
	{\
		UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
		nRet,\
		pszSqlValues,\
		MAX_SQL_LEN);\
		if(dwRetCode <= 0)\
		{\
		SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);\
		return FALSE;\
		}\
	}\
	SDSnprintf(pszBuff, MAX_SQL_LEN - 1, "( '%u', '%u', '%s', now() )", dwPlayerID, Key, pszSqlValues);\
	if(0 != wDataNum++)\
	{\
		SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ", ");\
	}\
	SDStrSafeCat(pszSql, MAX_SQL_LEN - 1,pszBuff);\
}

#define ENCODEDATA_SAVE_LOCAL(objStruct, FieldName, Key)\
	memset(pszTmp, 0, MAX_SQL_LEN);\
	if(0 != memcmp(&objStruct, pszTmp, sizeof(objStruct)))\
{\
	nRet = objStruct.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
	if(nRet <= 0)\
	{\
	SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);\
	return FALSE;\
	}\
	{\
	UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
	nRet,\
	pszTmp,\
	MAX_SQL_LEN - 1);\
	if(dwRetCode <= 0)\
		{\
        SYS_CRITICAL(_SDT("[%s: %d]: EscapeString %s failed, FieldName = %s!"), MSG_MARK, pszBuff, #FieldName);\
		return FALSE;\
		}\
	}\
	sprintf(pszBuff, "( '%u', '%u', '%s', now() )", dwPlayerID, Key, pszTmp);\
	if(0 != wDataNum++)\
	{\
	strcat(pszSql, ", ");\
	}\
	strcat(pszSql, pszBuff);\
}



#define ADDSIMPLEFIELD(FieldName, Formation, FieldData)\
strcat(pszSql, ", ");\
strcat(pszSql, #FieldName);\
strcat(pszSqlValues,", '");\
sprintf(pszTmp, #Formation, FieldData);\
strcat(pszSqlValues, pszTmp);\
strcat(pszSqlValues,"'");


#define COMBSQL()\
strcat(pszSql, ")");\
strcat(pszSqlValues, ")");\
strcat(pszSql, pszSqlValues);

#define UPDATEDATA_PREPARED_PARAMS( FieldName, MaxFieldLen )\
{\
	nRet = pstPlayerData->st##FieldName.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
	if(nRet <= 0)\
{\
	SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);\
	return FALSE;\
}\
	CHAR sz##FieldName[MaxFieldLen * 2 + 1] = {0};\
{\
	UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
	nRet,\
	sz##FieldName,\
	sizeof(sz##FieldName));\
	if(dwRetCode <= 0)\
	{\
	SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);\
	return FALSE;\
	}\
}\
	strcat(pszSql, ", ");\
	strcat(pszSql, #FieldName);\
	strcat(pszSql, "='");\
	strcat(pszSql, "?");\
	strcat(pszSql, "'");\
	nIndex++;\
	bindParam[nIndex].buffer_type = MYSQL_TYPE_BLOB;\
	bindParam[nIndex].buffer = (char *)sz##FieldName;\
	bindParam[nIndex].length = 0;\
	bindParam[nIndex].is_null = 0;\
}

#define CHECK_UPDATEDATA(FieldName, MaxFieldLen)\
	if( 0!=memcmp( &m_oPlayerSaveInfo.poPlayerSaveData->m_stLastPlayerData.st##FieldName, \
			&m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.st##FieldName, sizeof(m_oPlayerSaveInfo.poPlayerSaveData->m_stPlayerData.st##FieldName) ) )\
{\
		nRet = pstPlayerData->st##FieldName.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
		if(nRet <= 0)\
	{\
		SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);\
		return FALSE;\
	}\
		CHAR sz##FieldName[MaxFieldLen * 2 + 1] = {0};\
	{\
		UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
		nRet,\
		sz##FieldName,\
		sizeof(sz##FieldName));\
		if(dwRetCode <= 0)\
		{\
		SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);\
		return FALSE;\
		}\
	}\
	strcat(pszSql, ", ");\
	strcat(pszSql, #FieldName);\
	strcat(pszSql, "='");\
	strcat(pszSql, sz##FieldName);\
	strcat(pszSql, "'");\
}

#define UPDATEDATA(FieldName, MaxFieldLen)\
	nRet = pstPlayerData->st##FieldName.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
	if(nRet <= 0)\
{\
	SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);\
	return FALSE;\
}\
	CHAR sz##FieldName[MaxFieldLen * 2 + 1] = {0};\
{\
	UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
	nRet,\
	sz##FieldName,\
	sizeof(sz##FieldName));\
	if(dwRetCode <= 0)\
	{\
	SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);\
	return FALSE;\
	}\
}\
	strcat(pszSql, ", ");\
	strcat(pszSql, #FieldName);\
	strcat(pszSql, "='");\
	strcat(pszSql, sz##FieldName);\
	strcat(pszSql, "'");



#define UPDATEDATA2(objStruct, FieldName, MaxFieldLen)\
	nRet = objStruct.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
	if(nRet <= 0)\
{\
	SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);\
	return FALSE;\
}\
	CHAR sz##FieldName[MaxFieldLen * 2 + 1] = {0};\
{\
	UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
	nRet,\
	sz##FieldName,\
	sizeof(sz##FieldName));\
	if(dwRetCode <= 0)\
	{\
    SYS_CRITICAL(_SDT("[%s: %d]: EscapeString %s failed, FieldName = %s!"), MSG_MARK, pszBuff, #FieldName);\
	return FALSE;\
	}\
}\
strcat(pszSql, ", ");\
strcat(pszSql, #FieldName);\
strcat(pszSql, "='");\
strcat(pszSql, sz##FieldName);\
strcat(pszSql, "'");





#define UPDATESIMPLEFIELD1(FieldName, Formation, FieldData)\
	strcat(pszSql, #FieldName);\
	strcat(pszSql, "='");\
	sprintf(pszTmp, #Formation, FieldData);\
	strcat(pszSql, pszTmp);\
	strcat(pszSql, "'");


#define UPDATESIMPLEFIELD2(FieldName, Formation, FieldData)\
strcat(pszSql, ", ");\
strcat(pszSql, #FieldName);\
strcat(pszSql, "='");\
sprintf(pszTmp, #Formation, FieldData);\
strcat(pszSql, pszTmp);\
strcat(pszSql, "'");

#define UPDATESIMPLEFIELD_STRUCT1(FieldName, objStruct)\
	nRet = objStruct.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
	if(nRet <= 0)\
	{\
	SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);\
	return FALSE;\
	}\
	{\
		UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
		nRet,\
		pszTmp,\
		MAX_SQL_LEN);\
		if(dwRetCode <= 0)\
		{\
		SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);\
		return FALSE;\
		}\
	}\
	strcat(pszSql, #FieldName);\
	strcat(pszSql, "='");\
	strcat(pszSql, pszTmp);\
	strcat(pszSql, "'");

#define UPDATESIMPLEFIELD_STRUCT2(FieldName, objStruct)\
	nRet = objStruct.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
	if(nRet <= 0)\
	{\
	SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);\
	return FALSE;\
	}\
	{\
	UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
	nRet,\
	pszTmp,\
	MAX_SQL_LEN);\
	if(dwRetCode <= 0)\
		{\
        SYS_CRITICAL(_SDT("[%s: %d]: EscapeString %s failed, FieldName = %s!"), MSG_MARK, pszBuff, #FieldName);\
		return FALSE;\
		}\
	}\
	strcat(pszSql, ", ");\
	strcat(pszSql, #FieldName);\
	strcat(pszSql, "='");\
	strcat(pszSql, pszTmp);\
	strcat(pszSql, "'");

#define INSERT_STRUCT(FieldName, objStruct)\
    nRet = objStruct.EncodeSaveData(pszBuff, MAX_SQL_LEN);\
    if(nRet <= 0)\
    {\
    SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);\
    return FALSE;\
    }\
    {\
    UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,\
    nRet,\
    pszTmp,\
    MAX_SQL_LEN);\
    if(dwRetCode <= 0)\
        {\
        SYS_CRITICAL(_SDT("[%s: %d]: EscapeString %s failed, FieldName = %s!"), MSG_MARK, pszBuff, #FieldName);\
        return FALSE;\
        }\
    }\
    strcat(pszSql, ", '");\
    strcat(pszSql, pszTmp);\
    strcat(pszSql, "'");

#define INSERTINTOSIMPLEFIELD1(FieldName, Formation, FieldData)\
	strcat(pszSql, #FieldName);\
	sprintf(pszSqlValues, #Formation, FieldData);\
	strcat(pszTmp, pszSqlValues);\
 
#define INSERTINTOSIMPLEFIELD2(FieldName, Formation, FieldData)\
	strcat(pszSql, ", ");\
	strcat(pszSql, #FieldName);\
	strcat(pszTmp, ", ");\
	sprintf(pszSqlValues, #Formation, FieldData);\
	strcat(pszTmp, pszSqlValues);\
 
#define INSERTINTOSIMPLEFIELD3(FieldName, FieldData)\
	strcat(pszSql, ", ");\
	strcat(pszSql, #FieldName);\
	strcat(pszTmp, ", ");\
	strcat(pszTmp, #FieldData);\
 




//class CDBCommOperate
//{
//public:
//    CDBCommOperate();
//    virtual ~CDBCommOperate();
//    static BOOL FillPlayerData( ISDDBRecordSet &recordSet, DT_PLAYER_NOPVP_DATA& stDT_PLAYER_DATA, UINT32 dwPlayerID );
//
//};

#endif
