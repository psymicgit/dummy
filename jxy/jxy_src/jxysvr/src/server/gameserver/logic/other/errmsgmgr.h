
#ifndef _MSGDEFMGR_H_
#define _MSGDEFMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <map>
#include <vector>
#include <logic/base/basepropmgr.h>
#include <logic/base/logicfunc.h>
#include <string>

using namespace std;
using namespace SGDP;

//各个地区语言错误信息
struct SRegionErrMsgProp
{
	UINT8	byCliRegionType;	//客户端平台区域类型，0中文简体，1中文繁体，2英文. ProtocolID为0为表示当前使用的CliRegionType
	//UINT16	wProtocolID;		//需响应操作的协议号
	string	strErrKey;			//错误码
	string  strMsg;
	//string	strErrMsg;		//错误信息,格式化使用{变量名},如hp={HP},mp={MP}。格式变量不能连续粘在一起({HP}{MP}不行),也不能在第一个字符
	vector<string> vecErrMsg;	//错误信息,格式化使用${变量名},如hp={HP},mp={MP}，若已经
	//
};

typedef map<string, SRegionErrMsgProp> CID2RegionErrMsgPropMap; //key为byCliRegionType+strErrKey
typedef CID2RegionErrMsgPropMap::iterator CID2RegionErrMsgPropMapItr;

class CMsgDefMgr:public CBasePopMgr
{
	friend class CGSProcessor;
public:

    DECLARE_SINGLETON_PROPMGR(CMsgDefMgr);

	virtual EConfigModule	GetConfigModuleType() { return ECM_ERRMSG ; }
public:

    BOOL Init();

    VOID UnInit();
public:
	string GetErrMsg(string strErrKey, vector<string>* pvecFillMsg = NULL);

public:
    CMsgDefMgr();
    virtual ~CMsgDefMgr();
protected:
	BOOL LoadFromDB();

private:
	UINT8					m_byCliRegionType;
	CID2RegionErrMsgPropMap	m_mapID2RegionErrMsgProp;
};

#endif // 
