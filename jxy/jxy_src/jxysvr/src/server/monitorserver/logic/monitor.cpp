#include "monitor.h"
#include "common/server/utility.h"
#include <framework/msapplication.h>
#include "smtp.h"

#define MONITOR_SHEM	0x00
#define MONITOR_PROC	0x01
#define MAX_TRY_CHECK 0x05

enum USER_FLAG
{
    USER_FLAG_STOP = 0,
    USER_FLAG_START = 1,
};




/*********************************************************************************************
 * str_locate_text_reverse()
 * 查找字符串的起始地址
 * @param p_str（要查找的的母字符串）
 * @param p_text（查找子字符串）
 * @return 返回找到子串的位置
 ********************************************************************************************/
int str_locate_text_reverse(const char* p_str, const char* p_text)
{
    unsigned int text_len = strlen(p_text);
    if (text_len == 0 || text_len > strlen(p_str))
    {
        return -1;
    }
    int i = strlen(p_str) - text_len;
    while (1)
    {
        if (memcmp(p_str + i, p_text, text_len) == 0)
        {
            return i;
        }
        if (i == 0)
        {
            break;
        }
        i--;
    }
    return -1;
}




extern CSDApplication* SDGetApp();



IMPLEMENT_SINGLETON(CMonitor)

CMonitor::CMonitor()
{

}

CMonitor::~CMonitor()
{

}

BOOL CMonitor::SetUseFlag(UINT32 dwAreaID, char *szName, UINT8 useFlag)
{
    SProcessDaemon *	poProcessDaemon = NULL;
    SAreaInfo &		stAreaInfo = ((CMSApplication*)SDGetApp())->GetKSConfig()->GetAreaInfo();

    //先检测一下区域ID是否正确
    if (stAreaInfo.dwAreaID != dwAreaID)
    {
        return FALSE;
    }

    BOOL isOk = FALSE;
    for (UINT8 byGroupNum = 0; byGroupNum < stAreaInfo.byGroupNum; byGroupNum++)
    {
        BOOL			byStart = FALSE;
        SGroupInfo & stGroupInfo = stAreaInfo.astGroupInfo[byGroupNum];
        for (UINT8 byProcessNum = 0; byProcessNum < stGroupInfo.byProcessNum; byProcessNum++)
        {
            SProcessInfo	& stProcessInfo = stGroupInfo.astProcessInfo[byProcessNum];
            if (0 != strcmp(stProcessInfo.szName, szName))
            {
                continue;
            }

            isOk = TRUE;
            //////////////////////////////////////////////////////////////////////////
            if (USER_FLAG_START == useFlag)
            {
                SShmem	shmem;
                poProcessDaemon  = GetProcessDaemon(shmem, stProcessInfo);
                if (NULL == poProcessDaemon)
                {
                    if (stGroupInfo.byProcessNum > 1 && FALSE == byStart)//对于个数大于1个的要进行热更新, 并且只能启动一个。
                    {
                        byStart = TRUE;
                        Start(stProcessInfo);
                    }
                }
                else
                {
                    if (0 == poProcessDaemon->alive)
                    {
                        byStart = TRUE;
                        Start(stProcessInfo);
                    }
                    //if (stGroupInfo.byProcessNum <= 1)
                    //{
                    //    poProcessDaemon->useFlag = useFlag;
                    //}
                    SDUnAttachShm(&shmem);
                }
                stGroupInfo.byUseFlag = useFlag;
            }
            else
            {
                SShmem	shmem;
                poProcessDaemon  = GetProcessDaemon(shmem, stProcessInfo);
                if (NULL == poProcessDaemon)
                {
                    continue;
                }
                poProcessDaemon->useFlag = useFlag;
                stGroupInfo.byUseFlag = useFlag;
                SDUnAttachShm(&shmem);
            }
        }
    }
    return isOk;
}

void CMonitor::GET_SERVER_INFO_ACK(PKT_GMGS_GET_SERVER_INFO_ACK & stAck)
{
    SProcessDaemon *	poProcessDaemon = NULL;
    SAreaInfo &		stAreaInfo = ((CMSApplication*)SDGetApp())->GetKSConfig()->GetAreaInfo();

    //获取服务器运行数据
    UINT8 byGroupNum = 0;
    for (byGroupNum = 0; byGroupNum < stAreaInfo.byGroupNum && byGroupNum < GROUP_COUNT; byGroupNum++)
    {
        BOOL			byStart = FALSE;
        SGroupInfo & stGroupInfo = stAreaInfo.astGroupInfo[byGroupNum];

        stAck.astGroupDataList[byGroupNum].byUseFlag = stGroupInfo.byUseFlag;
        UINT8 byProcessNum = 0;
        for (byProcessNum = 0; byProcessNum < stGroupInfo.byProcessNum && byProcessNum < SERVER_COUNT; byProcessNum++)
        {
            SProcessInfo	& stProcessInfo = stGroupInfo.astProcessInfo[byProcessNum];

            memcpy(stAck.astGroupDataList[byGroupNum].astServerDataList[byProcessNum].aszServer, stProcessInfo.szName, strlen(stProcessInfo.szName));

            SShmem	shmem;
            poProcessDaemon  = GetProcessDaemon(shmem, stProcessInfo);
            if (NULL == poProcessDaemon)
            {
                continue;
            }
            stAck.astGroupDataList[byGroupNum].astServerDataList[byProcessNum].byKeepAlive = poProcessDaemon->alive;
            SDUnAttachShm(&shmem);

        }
        stAck.astGroupDataList[byGroupNum].byNum = byProcessNum;
    }
    stAck.byGroupNum = byGroupNum;
}

SProcessDaemon *CMonitor::GetProcessDaemon(SShmem	 & shmem, SProcessInfo &stProcessInfo)
{
    CHAR szFile[256] = {0};
    _SDTsnprintf(szFile, 256, "%s\\%s", stProcessInfo.szPath, "KeepAlive");
    if(SDShmemExist(szFile))
    {
        SDAttachShm(&shmem, szFile);
    }
    if(NULL == shmem.handle)
    {
        return NULL;
    }
    if (NULL != shmem.mem)
    {
        memcpy(&stProcessInfo.stProcessDaemon, shmem.mem, sizeof(SProcessDaemon));
    }
    return (SProcessDaemon*)shmem.mem;
}

BOOL CMonitor::Init()
{
    SAreaInfo &	stAreaInfo = ((CMSApplication*)SDGetApp())->GetKSConfig()->GetAreaInfo();
    printf("*************************************AreaID:%2d*************************************\n", stAreaInfo.dwAreaID);
    printf("%16s %5s %20s %20s %7s %7s\n", "Name", "PID", "StartTime", "LastTime", "UseFlag", "Alive");

    for (UINT8 byGroupNum = 0; byGroupNum < stAreaInfo.byGroupNum; byGroupNum++)
    {
        Check(stAreaInfo.dwAreaID, stAreaInfo.astGroupInfo[byGroupNum]);
    }
    return TRUE;
}

VOID CMonitor::UnInit()
{
#if defined(WIN32) || defined(WIN64)
    Quit();
#endif
}

BOOL CMonitor::Quit()
{
    SAreaInfo &	stAreaInfo = ((CMSApplication*)SDGetApp())->GetKSConfig()->GetAreaInfo();
    for (UINT8 byGroupNum = 0; byGroupNum < stAreaInfo.byGroupNum; byGroupNum++)
    {
        SGroupInfo & stGroupInfo = stAreaInfo.astGroupInfo[byGroupNum];

        //检测是否要启动
        if (0 == stGroupInfo.byUseFlag)
        {
            continue;
        }
        for (UINT8 byProcessNum = 0; byProcessNum < stGroupInfo.byProcessNum; byProcessNum++)
        {
            Kill(stGroupInfo.astProcessInfo[byProcessNum]);
        }
    }
    return TRUE;
}

BOOL CMonitor::OnEvent()
{
    SAreaInfo &	stAreaInfo = ((CMSApplication*)SDGetApp())->GetKSConfig()->GetAreaInfo();
    printf("********************************************************************************\n");
    printf("%16s %5s %20s %20s %7s %7s\n", "Name", "PID", "StartTime", "LastTime", "UseFlag", "Alive");
    printf("----AreaID:%d--------------------------------------------------------------------\n", stAreaInfo.dwAreaID);

    for (UINT8 byGroupNum = 0; byGroupNum < stAreaInfo.byGroupNum; byGroupNum++)
    {
        Check(stAreaInfo.dwAreaID, stAreaInfo.astGroupInfo[byGroupNum]);
    }
    return TRUE;
}

BOOL CMonitor::Check(const UINT32 dwAreaID, SGroupInfo &stGroupInfo)
{
    if (!stGroupInfo.byUseFlag)
    {
        return FALSE;
    }

    BOOL		bAlive		=	FALSE;
    UINT8	byProcessNum	=	0;
    //检测是否有一个存活着
    for ( byProcessNum = 0; byProcessNum < stGroupInfo.byProcessNum; byProcessNum++)
    {
        SProcessInfo  & stProcessInfo = stGroupInfo.astProcessInfo[byProcessNum];
        //检测是否要启动
        if(Check(stProcessInfo))
        {
            bAlive = TRUE;
        }
    }

    if (bAlive)
    {
        return TRUE;
    }

    //获取最后的运行时间
    UINT8	byLastProcessNum = 0;
    UINT64	qwlasttimer = 0;;
    for ( byProcessNum = 0; byProcessNum < stGroupInfo.byProcessNum; byProcessNum++)
    {
        if ((UINT64)stGroupInfo.astProcessInfo[byProcessNum].stProcessDaemon.lasttimer > qwlasttimer)
        {
            byLastProcessNum = byProcessNum;
            qwlasttimer = stGroupInfo.astProcessInfo[byProcessNum].stProcessDaemon.lasttimer;
        }
    }

    //启动最后分组的运行程序
    if (stGroupInfo.astProcessInfo[byLastProcessNum].byCheckTwices > ((CMSApplication*)SDGetApp())->GetKSConfig()->GetTryCheck())
    {
        OnMail(dwAreaID, stGroupInfo.astProcessInfo[byLastProcessNum]);
    }
    else
    {
        Start(stGroupInfo.astProcessInfo[byLastProcessNum]);
    }
    return TRUE;
}


BOOL CMonitor::Start(SProcessInfo &stProcessInfo)
{
    CHAR	szCmdLine[1024] = {0};
    CHAR	szProgName[1024] = {0};
    CHAR	szWorkDir[1024] = {0};

#if defined(WIN32) || defined(WIN64)
    SDSprintf(szCmdLine, "%s %s", stProcessInfo.szName, stProcessInfo.szArgv);
    SDSprintf(szProgName, "%s\\%s", stProcessInfo.szPath, stProcessInfo.szName);
    SDSprintf(szWorkDir, "%s", stProcessInfo.szPath);
#else
    SDSprintf(szCmdLine, "%s %s", stProcessInfo.szName, stProcessInfo.szArgv);
    SDSprintf(szProgName, "%s", stProcessInfo.szName);
    SDSprintf(szWorkDir, "%s", stProcessInfo.szPath);
#endif
    SDCreateProcess(szProgName, szCmdLine, szWorkDir, NULL);

    USR_INFO(_SDT("[%s: %d]: start Path:%s, Name:%s"), MSG_MARK, stProcessInfo.szPath, stProcessInfo.szName);

    return TRUE;
}

void CMonitor::Kill(SProcessInfo &stProcessInfo)
{
    SShmem	shmem;
    SProcessDaemon * poProcessDaemon = NULL;
    poProcessDaemon = GetProcessDaemon(shmem, stProcessInfo);
    if (NULL == poProcessDaemon)
    {
        return;
    }
    poProcessDaemon->useFlag = 0;
    SDUnAttachShm(&shmem);
}


BOOL CMonitor::Check(SProcessInfo &stProcessInfo)
{
    BOOL		bAlive	=	FALSE;
    if(MONITOR_SHEM == stProcessInfo.byType)
    {
        SShmem	shmem;
        SProcessDaemon * poProcessDaemon = NULL;
        poProcessDaemon = GetProcessDaemon(shmem, stProcessInfo);
        if (NULL == poProcessDaemon)
        {
            printf("%16s %5u %20s %20s %7u %7u\n",
                   stProcessInfo.szName,
                   0,
                   Time_ToString(SDTimeSecs()).c_str(),
                   Time_ToString(SDTimeSecs()).c_str(),
                   1,
                   0);
            stProcessInfo.byCheckTwices++;
            return FALSE;
        }
        else
        {
            printf("%16s %5u %20s %20s %7u %7u\n",
                   stProcessInfo.szName,
                   poProcessDaemon->pid,
                   Time_ToString(poProcessDaemon->starttimer).c_str(),
                   Time_ToString(poProcessDaemon->lasttimer).c_str(),
                   poProcessDaemon->useFlag,
                   poProcessDaemon->alive);
        }

        if(1 == poProcessDaemon->alive)
        {
            bAlive = TRUE;
        }
        stProcessInfo.byCheckTwices = 0;
        //重置用户存活标志
        poProcessDaemon->alive = 0;

        SDUnAttachShm(&shmem);
    }
    else
    {
        //成功返回
        int pid = watch_procps_scan(stProcessInfo);
        if (0 == pid)
        {
            printf("%16s %5u %20s %20s %7u %7u\n", stProcessInfo.szName, pid, Time_ToString(SDTimeSecs()).c_str(), Time_ToString(SDTimeSecs()).c_str(), 1, 0);
            Start(stProcessInfo);
        }
        else
        {
            printf("%16s %5u %20s %20s %7u %7u\n", stProcessInfo.szName, pid, Time_ToString(SDTimeSecs()).c_str(), Time_ToString(SDTimeSecs()).c_str(), 1, 1);
        }

    }
    return bAlive;
}

/********************************************************************************************
 FunctionName:     watch_procps_scan()
 Description:
 input:
 return:
 ********************************************************************************************/

int CMonitor::watch_procps_scan(SProcessInfo &stProcessInfo)
{
#if defined(WIN32) || defined(WIN64)
    return TRUE;
#else
    static DIR *dir;
    struct dirent *entry;
    FILE *fp;
    char *name;
    int pid;
    char cmdline_buf[32];
    char buf[128];
    int i;
    dir = opendir("/proc");
    if (!dir)
    {
        perror("--start: procps_scan: Can't open /proc!--\n");
        return 0;
    }

    BOOL	bSearch = FALSE;
    while (1)
    {
        if ((entry = readdir(dir)) == NULL)
        {
            closedir(dir);
            return 0;
        }

        name = entry->d_name;
        if (!(*name >= '0' && *name <= '9'))
        {
            continue;
        }

        bzero(cmdline_buf, sizeof(cmdline_buf));
        int tpid = atoi(name);
        sprintf(cmdline_buf, "/proc/%d/cmdline", tpid);
        if ((fp = fopen(cmdline_buf, "r")) == NULL)
        {
            continue;
        }
        bzero(buf, sizeof(buf));
        if (fgets(buf, sizeof(buf), fp))
        {
            if (str_locate_text_reverse(buf, stProcessInfo.szName) >= 0)
            {
                bSearch = TRUE;
                pid = tpid;
            }
        }
        fclose(fp);
    }
    closedir(dir);
    return pid;
#endif
}

void CMonitor::OnMail(const UINT32 dwAreaID, const SProcessInfo stProcessInfo)
{
    char szSubject[255] = {0};
    sprintf(szSubject, "The AreaID:%u, Server:%s Crash!!!", dwAreaID, stProcessInfo.szName);

    char szMsg[255] = {0};
    sprintf(szMsg, "The AreaID:%u, Server:%s Crash!!! starttime:%s, lasttime:%s", dwAreaID, stProcessInfo.szName,
            Time_ToString(stProcessInfo.stProcessDaemon.starttimer).c_str(),
            Time_ToString(stProcessInfo.stProcessDaemon.lasttimer).c_str());

    SSmtpInfo &	stSmtpInfo = ((CMSApplication*)SDGetApp())->GetKSConfig()->GetSmtpInfo();
    try
    {
        CSmtp mail;
        mail.SetSMTPServer(stSmtpInfo.aszSmtpServer, 25);
        mail.SetLogin(stSmtpInfo.aszUserName);
        mail.SetPassword(stSmtpInfo.aszUserPwd);
        mail.SetSenderName(stSmtpInfo.aszNickName);
        mail.SetSenderMail(stSmtpInfo.aszSendMail);
        mail.SetReplyTo(stSmtpInfo.aszReplyTo);
        mail.SetSubject(szSubject);

        for (UINT8 byMailUserNum = 0; byMailUserNum < stSmtpInfo.byMailUserNum; byMailUserNum++)
        {
            mail.AddRecipient(stSmtpInfo.astMailUserInfo[byMailUserNum].aszMailUser);
        }

        mail.SetXPriority(XPRIORITY_NORMAL);
        mail.SetXMailer("The Bat! (v3.02) Professional");
        mail.AddMsgLine("Hello");
        mail.AddMsgLine(szMsg);
        mail.Send();
    }
    catch(ECSmtp e)
    {
        USR_INFO(_SDT("[%s: %d]: OnMail Error:%s"), MSG_MARK, e.GetErrorText().c_str());
    }
}


