#ifndef _RECORD_MODULE_H_
#define _RECORD_MODULE_H_
#include <deque>
#include <sdlock.h>
#include <sddb.h>

using namespace std;
using namespace SGDP;

class CRecordModule
{
public:
	CRecordModule(ISDDBSession* pDBRecordSession);
	virtual ~CRecordModule();

	BOOL AddData( const string &strInfo );
	deque<string> &GetQueue();
	VOID Clear();
	CSDMutex &GetDataMutex();
	string GetRecordModuleName(UINT32& dwNextID);
	inline VOID SetCurID(UINT32 dwID) { m_dwNextID = dwID; }
private:
	const static INT32 ms_nMaxDataNum=50000; //最大数据缓存数量
	deque<string> m_deqData;
	CSDMutex m_mutex;
	ISDDBSession* m_pDBRecordSession; //数据库连接
	UINT32 m_dwNextID;		//内存中记录，防止取不到时id从1开始
	string m_strTableName;	//上次的表名，防止取不到时写文件没有表名
};


#endif