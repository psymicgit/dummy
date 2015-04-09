
#include "msconfig.h"
#include <sdutil.h>

#define ATTRI_USERNAME			"username"
#define ATTRI_PWD				"pwd"
#define ATTRI_DBNAME0			"dbname"
#define ATTRI_CHARACTERSET		"CharacterSet"
#define ELEM_POOLMAXSIZE		"PoolMaxSize"
#define ELEM_POOLCORESIZE		"PoolCoreSize"
#define CA_CERT_PATH			"ca_cert_path"
#define RSA_CLIENT_CERT		"rsa_client_cert"
#define RSA_CLIENT_KEY			"rsa_client_key"


CKSConfig::CKSConfig()
{
    m_dwKeepAlive = 0;
    memset(&m_stAreaInfo, 0x00, sizeof(m_stAreaInfo));
}

CKSConfig::~CKSConfig()
{
    UnInit();
}

BOOL CKSConfig::Init(const TCHAR* pszFilename)
{
    CSDXMLFile oSDXMLFile;
    if (FALSE == oSDXMLFile.Load(_SDTT2A(pszFilename)))
    {
        return FALSE;
    }

    if (!_LoadProcessAreaInfo(&oSDXMLFile))
    {
        SYS_CRITICAL(_SDT("[%s:%d]--Load exec info failed."), MSG_MARK);
        return FALSE;
    }

    if (!_LoadSmtpInfo(&oSDXMLFile))
    {
        SYS_CRITICAL(_SDT("[%s:%d]--Load Smtp info failed."), MSG_MARK);
        return FALSE;
    }

    CSDXMLNode rootNode = oSDXMLFile.GetRootNode();
    m_dwKeepAlive = rootNode["KeepAlive"]("value").AsInteger();

    if (0 == m_dwKeepAlive)
    {
        m_dwKeepAlive = 5;
    }

	m_dwTryCheck = rootNode["try-check"]("value").AsInteger();

	if (0 == m_dwTryCheck)
	{
		m_dwTryCheck = 5;
	}

    return TRUE;
}


VOID  CKSConfig::UnInit()
{

}

BOOL	CKSConfig::_LoadProcessAreaInfo(CSDXMLFile* poSDXMLFile)
{
    CSDXMLNode	rootNode = poSDXMLFile->GetRootNode();
    CSDXMLNode	areaNode = rootNode["area"];
    m_stAreaInfo.dwAreaID = areaNode("AreaID").AsInteger();
    CSDXMLNode	groupNode =	areaNode["group"];

    while (groupNode != NULL)
    {
        SGroupInfo	 &	stGroupInfo	=	m_stAreaInfo.astGroupInfo[m_stAreaInfo.byGroupNum];
        CSDXMLNode		execNode		=	groupNode["exec"];
        stGroupInfo.byUseFlag			=	groupNode("useflag").AsInteger();
        while (execNode != NULL)
        {
            SProcessInfo	 & stExecInfo		= stGroupInfo.astProcessInfo[stGroupInfo.byProcessNum];

            SDStrcpy(stExecInfo.szPath, execNode("path").AsString().c_str());
            if (0 == SDStrlen(stExecInfo.szPath))
            {
                SDStrcpy(stExecInfo.szPath, SDGetModulePath());
            }

            SDStrcpy(stExecInfo.szName, execNode("name").AsString().c_str());
            if (0 == SDStrlen(stExecInfo.szName))
            {
                return FALSE;
            }

            SDStrcpy(stExecInfo.szArgv, execNode("argv").AsString().c_str());
            stExecInfo.byType = execNode("type").AsInteger();

            execNode = execNode.Sibling();
            stGroupInfo.byProcessNum++;
        }
        groupNode = groupNode.Sibling();
        m_stAreaInfo.byGroupNum++;
    }

    return TRUE;
}


BOOL	CKSConfig::_LoadSmtpInfo(CSDXMLFile* poSDXMLFile)
{
    CSDXMLNode	rootNode = poSDXMLFile->GetRootNode();
    CSDXMLNode	smtpNode = rootNode["smtp"];

    SDStrcpy(m_stSmtpInfo.aszSmtpServer, smtpNode("smtp-server").AsString().c_str());
    SDStrcpy(m_stSmtpInfo.aszUserName, smtpNode("user-name").AsString().c_str());
    SDStrcpy(m_stSmtpInfo.aszUserPwd, smtpNode("user-pwd").AsString().c_str());
    SDStrcpy(m_stSmtpInfo.aszNickName, smtpNode("nick-name").AsString().c_str());
    SDStrcpy(m_stSmtpInfo.aszSendMail, smtpNode("send-mail").AsString().c_str());
    SDStrcpy(m_stSmtpInfo.aszReplyTo, smtpNode("reply-to").AsString().c_str());


    CSDXMLNode	userNode =	smtpNode["user"];
    while (userNode != NULL)
    {
        SMailUserInfo	 &	stMailUserInfo	=	m_stSmtpInfo.astMailUserInfo[m_stSmtpInfo.byMailUserNum];
        SDStrcpy(stMailUserInfo.aszMailUser, userNode("mail-to").AsString().c_str());
        userNode = userNode.Sibling();
        m_stSmtpInfo.byMailUserNum++;
    }
    return TRUE;
}


