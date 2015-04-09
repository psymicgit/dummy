#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H


#include "sdtype.h"
#include <sdnetutils.h>
#include <sdutil.h>
#include <sdframework.h>
#include "logic/getplayernotifyinfo.h"


#include "sslcomm.h"
#include "defapns.h"
#include "iapns.h"

using namespace SGDP;


#include <list>
using namespace std;


typedef struct NOTIFY_NODE
{
	NOTIFY_NODE()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(NOTIFY_NODE));
	}
	int			nLen;
	char			szBuf[2048];
}CNOTIFY_NODE, *pNOTIFY_NODE;

typedef std::list<pNOTIFY_NODE> CNotifyNodeList; 
typedef CNotifyNodeList::iterator CNotifyNodeListItr;




class CNotifications : public IAPNS
{
public:
	CNotifications();
	~CNotifications();
	bool SSL_PushNotice(const char *pToken, const char *pMsg, const SPlayerNotifyInfo &stNoticeInfo);
	bool SSL_PushNoticeEx(const char *pToken, const char *pMsg, const SPlayerNotifyInfo &stNoticeInfo);

private:
	virtual void Read();
	virtual void Write();

private:
	UINT32			m_dwIdentifier;
	CSDCondition		m_oCondition;
	CSDMutex			m_oMutex;
	CNotifyNodeList	m_oNotifyNodeList;
	CSDObjectPool<CNOTIFY_NODE, CSDMutex>	m_oNotifyNodePool;
};

#endif
