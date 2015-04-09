

#ifndef _NS_APPLICATION_H
#define _NS_APPLICATION_H

#include <sdtype.h>
#include <sdframework.h>
#include "net/gs/gsprocessor.h"
#include "logic/dbmgr.h"
#include "logic/nsconfig.h"
#include "common/server/packetparser.h"
#include "apns/notifications.h"
#include "apns/feedback.h"
#include "logic/twodaysnotonlinenotifyevent.h"
#include "logic/getplayernotifyinfoevent.h"
#include "common/client/commondef.h"
#include "ksclient.h"

typedef map<UINT32, CSDPipeChannel*> CPipeChannelMap; //key为武将ID+神器品质
typedef CPipeChannelMap::iterator CPipeChannelMapItr;





class CNSApplication : public CSDApplication
{
public:
	CNSApplication();
	~CNSApplication();
    virtual BOOL			Init(const TCHAR* pszConfigFilename);
    virtual VOID			UnInit();
    virtual BOOL			Run();
    virtual CSDPipeChannel* OnPipeChannelConnected(UINT32 dwPipeID);
    virtual VOID			OnPipeChannelDisconnected(CSDPipeChannel* poChannel);
    virtual VOID			UpdateSvrStatus(UINT32 dwID,UINT32 dwIP, BOOL bConnect);

    //////////////////////////////
    CNSConfig*			GetNSConfig() { return &m_oNSConfig; }
    CCliPacketParser*		GetCliPacketParser() { return &m_oCliPacketParser; }
	CDBMgr*				GetDBMgr() { return &m_oDBMgr; }
	CNotifications *		GetNotifications(){return m_poNotifications;}
	CFeedBack *			GetFeedBack(){return m_poFeedBack;}

	CKSClientMgr		&	GetClientMgr(){return m_oksclientMgr;}

	void					SetPlayerNotifyInfoMap(CPlayerNotifyInfoMap vValues);
	CPlayerNotifyInfoMap & GetPlayerNotifyInfoMap(){return m_mapPlayerNotifyInfo; };

protected:
    virtual VOID			OnHandleUserCmd(const TCHAR* szCmd);
	BOOL					DealUserCmd(const TCHAR* szCmd);

private:    

    CCliPacketParser				m_oCliPacketParser;
    CNSConfig					m_oNSConfig;
	CDBMgr						m_oDBMgr;
	CEventReactor<CTwoDaysNotOnlineNotityEvent>	m_oTwoDaysNotOnlineNotityReactor;
	CEventReactor<CGetPlayerNotifyInfoEvent>		m_oGetPlayerNotifyInfoEventReactor;

	CPipeChannelMap				m_mapPipeChannel;
	CGSProcessor					m_oGSProcessor;
	CNotifications *				m_poNotifications;
	CFeedBack	*				m_poFeedBack;
	ISDListener*					m_poListener;
	CKSClientMgr					m_oksclientMgr;

private:
	CPlayerNotifyInfoMap			m_mapPlayerNotifyInfo;
};


#endif

