

#ifndef _GATE_SERVER_SETTING_H_
#define _GATE_SERVER_SETTING_H_

#include <sdutil.h>
#include <sdframework.h>
#include <string>
#include <list>
#include <vector>
#include <sddb.h>
#include <keepalive/ctdef.h>


#define MAX_USER_NUM	30
#define MAX_EXEC_PATH 512
#define MAX_EXEC_NAME	512
#define MAX_EXEC_NUM	10
#define MAX_AREA_NUM	100


using namespace std;

typedef struct tagGateListenerInfo
{
	CHAR					m_szIP[MAX_IP_LEN];
	UINT16				m_wPort;
	UINT32				m_dwRecvBuf;
	UINT32				m_dwSendBuf;
} SGateListenerInfo;


typedef struct tagExecInfo
{
	UINT8				useFlag;
	CHAR					szPath[MAX_EXEC_PATH];
	CHAR					szName[MAX_EXEC_NAME];
} SExecInfo;

typedef struct tagExecInfoArea
{
	UINT32		dwAreaID;
	UINT8		byExecNum;
	SExecInfo	stExecInfo[MAX_EXEC_NUM]; 
} SExecInfoArea;

typedef struct tagExecInfoAreaList
{
	UINT8		byAreaNum;
	SExecInfoArea	stExecInfoArea[MAX_AREA_NUM]; 
} SExecInfoAreaList;

typedef struct tagUserInfo
{
	char szUserName[64];
	char szUserPwd[64];
	tagUserInfo()
	{

	}
	void Init()
	{
		memset(this, 0x00, sizeof(tagUserInfo));
	}
}SUserInfo;

typedef struct tagUserInfoList
{
	UINT8		byNum;
	SUserInfo	stUserInfo[MAX_USER_NUM];
	tagUserInfoList()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(tagUserInfoList));
	}
}TUserInfoList;





class CGMConfig
{
public:
    CGMConfig();
    ~CGMConfig();
public:
    virtual VOID			UnInit();
    virtual BOOL			Init(const TCHAR* pszFilename);
	SExecInfoAreaList	&	GetExecInfoAreaList(){return m_stExecInfoAreaList;}

	char				*	GetUserPwd(char *szUserName);

	UINT32				GetKeepAlive(){return m_dwKeepAlive;}

	UINT32				GetPoolCoreSize();
	UINT32				GetPoolMaxSize();
	BOOL					GetDBAccount(SGDP::SDDBAccount& DBAccount);

protected:
	BOOL					_LoadExecInfo(CSDXMLFile* poSDXMLFile);
	BOOL					_LoadUserInfo(CSDXMLFile* poSDXMLFile);

private:
    SGateListenerInfo		m_stGateListenerInfo;
	SExecInfoAreaList		m_stExecInfoAreaList;
	TUserInfoList			m_stUserInfoList;
	UINT32				m_dwKeepAlive;
	SGDP::SDDBAccount		m_oDBAccount;   ///< 数据库连接信息
	UINT32				m_dwPoolCoreSize;
	UINT32				m_dwPoolMaxSize;
};


#endif //#ifndef _GATE_SERVER_SETTING_H_

