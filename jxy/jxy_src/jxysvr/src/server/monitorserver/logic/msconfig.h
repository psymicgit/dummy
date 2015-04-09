

#ifndef _GATE_SERVER_SETTING_H_
#define _GATE_SERVER_SETTING_H_

#include <sdutil.h>
#include <sdframework.h>
#include <string>
#include <list>
#include <vector>
#include <sddb.h>
#include <keepalive/ctdef.h>


#define MAX_USER_NUM		30
#define MAX_PROCESS_PATH	512
#define MAX_PROCESS_NAME	512
#define MAX_PROCESS_ARGV	512
#define MAX_PROCESS_NUM	10
#define MAX_AREA_NUM		100


using namespace std;

typedef struct tagGateListenerInfo
{
	CHAR					m_szIP[MAX_IP_LEN];
	UINT16				m_wPort;
	UINT32				m_dwRecvBuf;
	UINT32				m_dwSendBuf;
} SGateListenerInfo;


typedef struct tagProcessInfo
{
	UINT8				byType;			//监控类型
	UINT8				byCheckTwices;	//连续检测次数
	CHAR					szPath[MAX_PROCESS_PATH];
	CHAR					szName[MAX_PROCESS_NAME];
	CHAR					szArgv[MAX_PROCESS_ARGV];
	SProcessDaemon		stProcessDaemon;
} SProcessInfo;

typedef struct tagGroupInfo
{
	UINT8		byUseFlag;	//是否监控
	UINT8		byProcessNum;
	SProcessInfo	astProcessInfo[MAX_PROCESS_NUM]; 
} SGroupInfo;


typedef struct tagAreaInfo
{
	UINT32		dwAreaID;
	UINT8		byGroupNum;
	SGroupInfo	astGroupInfo[MAX_PROCESS_NUM]; 
} SAreaInfo;

typedef struct _tagMailUserInfo
{
	_tagMailUserInfo()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(_tagMailUserInfo));
	}
	CHAR	 aszMailUser[255];
}SMailUserInfo;


typedef struct _tagSmtpInfo
{
	_tagSmtpInfo()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(_tagSmtpInfo));
	}

	CHAR			aszSmtpServer[255];
	CHAR			aszUserName[255];
	CHAR			aszUserPwd[255];
	CHAR			aszNickName[255];
	CHAR			aszSendMail[255];
	CHAR			aszReplyTo[255];
	UINT8		byMailUserNum;
	SMailUserInfo	astMailUserInfo[255]; 
} SSmtpInfo;


class CKSConfig
{
public:
    CKSConfig();
    ~CKSConfig();
public:
    virtual VOID			UnInit();
    virtual BOOL			Init(const TCHAR* pszFilename);
	SAreaInfo	&		GetAreaInfo(){return m_stAreaInfo;}
	SSmtpInfo	&		GetSmtpInfo(){return m_stSmtpInfo;}
	UINT32				GetKeepAlive(){return m_dwKeepAlive;}
	UINT32				GetTryCheck(){return m_dwTryCheck;}
protected:
	BOOL					_LoadProcessAreaInfo(CSDXMLFile* poSDXMLFile);
	BOOL					_LoadSmtpInfo(CSDXMLFile* poSDXMLFile);
private:
	SAreaInfo			m_stAreaInfo;
	SSmtpInfo			m_stSmtpInfo;
	UINT32				m_dwKeepAlive;
	UINT32				m_dwTryCheck;
};


#endif //#ifndef _GATE_SERVER_SETTING_H_

