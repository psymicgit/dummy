/*
 ============================================================================
 Name        : start.c
 Author      : wyh
 Version     :
 Copyright   : Your copyright notice
 Description : in C, Ansi-style
 ============================================================================
 */

#include <time.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <getopt.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#include "ReadIni.h"

#define VERSION 		"1.0.0"
#define MAXPROCESSES 	10
#define MONSHMKEY 		20121212
#define MAX_BUFFER		2048
#define MAX_ARGV		10
#define MAX_ARGV_LEN	255


//进程信息
struct TProcessDaemon
{
	char 	argv[MAX_ARGV][MAX_ARGV_LEN];
	int 	pid;
	time_t 	timer;
	int 	useFlag; //使用标志，在monitor进程中用这个标志来判断是否显示
	int 	alive;
	int 	fd;
	char 	fifo[255];
};

struct TProcessState
{
	struct TProcessDaemon 	start;
	struct TProcessDaemon 	mail;
	struct TProcessDaemon 	notify;
	struct TProcessDaemon 	processes[MAXPROCESSES];
};

struct TProcessState* pshm = NULL;
char *ServiceDIR = NULL;
char binDir[255] = {0};
char etcDir[255] = {0};
char logDir[255] = {0};
char fifoDir[255]		=	{0};
char logFileName[255] 	= {0};


/*-----------------------------------------------------------------------
 * name:    取当前时间
 * input:
 * output:  _stime  -- 时间字符串
 * return:
 *-----------------------------------------------------------------------*/
static int Currenttime(char * _stime)
{
	time_t timep;
	struct tm * p;
	time(&timep);
	p = localtime(&timep);
	sprintf(_stime, "%4d-%02d-%02d %02d:%02d:%02d ", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	return 1;
}


/*-----------------------------------------------------------------------
 * name:    记录日志文件信息
 * input:
 * output:  
 * return:
 *-----------------------------------------------------------------------*/
static void LOGINFO(char *szFormat, ...)
{
	FILE* fp = NULL;
	char szBuffer[4096] = { 0x00 };
	va_list pArguments;
	va_start(pArguments, szFormat);
	vsprintf(szBuffer, szFormat, pArguments);
	va_end(pArguments);

	char _stime[128] = { 0 };
	memset(_stime, 0, sizeof(_stime));
	Currenttime(_stime);

	memset(logFileName, 0, 100);
	sprintf(logFileName, "%s/start.log", logDir);

	if ((fp = fopen(logFileName, "a+t")) == NULL)
	{
		printf("open log file '%s' failed.\n", logFileName);
		exit(-1);
	}
	fprintf(fp, "[%s]:%s\n", _stime, szBuffer);
	fclose(fp);
}



/*-----------------------------------------------------------------------
 * name:    
 * input:
 * output:  
 * return:
 *-----------------------------------------------------------------------*/
void clear(struct TProcessDaemon *p)
{
	if(1 != p->useFlag)
		return;

	int i = 0;
	for (i = 0; i < MAX_ARGV; i++)
	{
		memset(p->argv[i], 0x00, MAX_ARGV_LEN);
	}
	p->pid = -1;
	time(&p->timer);
	p->useFlag = 0;
	p->alive = 0;
}



void start(struct TProcessDaemon *p)
{
	if(1 != p->useFlag)
		return;
		
	LOGINFO("start %s \n", p->argv[0]);
	char temp[255] = { 0 };
	if(strcmp("mail", p->argv[0]) == 0 || strcmp("notify", p->argv[0]) == 0)
	{
		sprintf(temp, "%s/%s", ServiceDIR, p->argv[0]);
	}
	else
	{
		sprintf(temp, "%s/%s", binDir, p->argv[0]);
	}

	pid_t pid;
	if ((pid = fork()) == 0)
	{
		char *argv[MAX_ARGV];
		int i = 0;
		for (i = 0; i < MAX_ARGV; i++)
		{
			if (p->argv[i][0] == '\0')
			{
				argv[i] = NULL;
				break;
			}
			argv[i] = p->argv[i];
		}
		argv[i] = NULL;
		execv(temp, argv);

		char temp[255] = {0};
		sprintf(temp, "%s start failed(errno=%d)", p->argv[0], errno);
		LOGINFO(temp);
		exit(0);
	}
	p->alive = 1;
	p->pid = pid;
	time(&p->timer);
}

void dead(struct TProcessDaemon *p)
{
	if(1 != p->useFlag)
		return;
		
	if (p->pid > 0)
	{
		printf("Kill %s , ID:%d \n", p->argv[0], p->pid);
		LOGINFO("Kill %s , ID:%d \n", p->argv[0], p->pid);
		kill(p->pid, SIGKILL);
	}
}


bool check(struct TProcessDaemon *p)
{
	if(1 != p->useFlag)
		return false;
		
	if(0 == p->alive)
	{
		LOGINFO("restart %s %d\n", p->argv[0], p->pid);
		if (p->pid > 0)
		{
			kill(p->pid, SIGKILL);
		}
		start(p);
		return true;
	}
	return false;
}

void fifoopen(TProcessDaemon *p)
{
	if(1 != p->useFlag)
		return;
	p->fd = open(p->fifo, O_WRONLY | O_NONBLOCK,0);                      //不阻塞   
	if(-1 == p->fd)  
	{  
		LOGINFO("mail is not started\n");      
	} 
}

void fifowrite(TProcessDaemon *p, char * buf)
{
	if(1 == p->useFlag && p->fd > 0)
	{
		if(write(p->fd, buf,100) == -1)                                  //write   
		{  
			if(errno==EAGAIN)
			{						
				LOGINFO("The FIFO has not been read yet.Please try later\n");  
			}
			else
			{
				LOGINFO("The FIFO Write Error.Please try later\n");  
			}
		}
		else
		{
			LOGINFO("The FIFO Write OK.\n");  	
		}
	}
}

void catchSigCLD()
{
	LOGINFO("catch SIGCLD");
	pid_t status;
	status = wait((int *) 0);
	if (pshm->mail.useFlag && pshm->mail.pid == status)
	{
		pshm->mail.alive = 0;
		printf("received SIGCHLD from %s\n", pshm->mail.argv[0]);
		LOGINFO("received SIGCHLD from %s ", pshm->mail.argv[0]);
	}
	
	if (pshm->notify.useFlag && pshm->notify.pid == status)
	{
		pshm->notify.alive = 0;
		printf("received SIGCHLD from %s\n", pshm->notify.argv[0]);
		LOGINFO("received SIGCHLD from %s ", pshm->notify.argv[0]);
	}
	
	int i = 0;
	for (i = 0; i < MAXPROCESSES; i++)
	{
		if (pshm->processes[i].useFlag && pshm->processes[i].pid == status)
		{
			pshm->processes[i].alive = 0;
			printf("received SIGCHLD from %s\n", pshm->processes[i].argv[0]);
			LOGINFO("received SIGCHLD from %s ", pshm->processes[i].argv[0]);
			break;
		}
	}
	signal(SIGCHLD, (void(*)(int)) catchSigCLD);
}

void catchSigUsr2()
{
	LOGINFO("catch SIGUSR2");
	signal(SIGUSR2, (void(*)(int)) catchSigUsr2);
}

void catchSigINT()
{
	LOGINFO("catch SIGINT");
	signal(SIGINT, (void(*)(int)) catchSigINT);
}

void catchSigUsr1()
{
	LOGINFO("catch SIGUSR1");
	exit(0);
}

int main(int argc, char **argv)
{
	int opt    = 0;
	int bstart = 1;
	int bstop  = 0;
	int realKey = 0;
	int nProcessNum  = 0;
	if (argc <= 1)
	{
		printf("Version %s\n", VERSION);
		printf("start {--version, --help, --start, --quit}\n");
		printf("CopyRight SMS, All Rights reserved.\n\n");
		exit(0);
	}
	struct option longopts[] =
	{
	{ "version", 0, NULL, 'v' },
	{ "help", 0, NULL, 'h' },
	{ "start", 0, NULL, 's' },
	{ "quit", 0, NULL, 'q' },
	{ "key", 0, NULL, 'k' },
	{ 0, 0, 0, 0 } };

	while ((opt = getopt_long(argc, argv, "vhsqk", longopts, NULL)) != -1)
	{
		switch (opt)
		{
		case 's':
			bstop  = 0;
			bstart = 1;
			break;
		case 'q':
			bstart = 0;
			bstop  = 1;
			break;
		case 'k':
			realKey = atoi(optarg);
			break;
		case 'v':
		case 'h':
		default:
			printf("error argv : %c\n", opt);
			printf("Version %s\n", VERSION);
			printf("start {--version, --help, --start, --quit, --key}\n");
			printf("Copyright(c) 2012-2013, All Rights reserved.\n\n");
			exit(-1);
		}
	}

	if (!bstart && !bstop)
	{
		printf("error argv \n");
		printf("start {--version, --help, --start, --quit, --key}\n");
		printf("CopyRight 2012-2013, All Rights reserved.\n\n");
		exit(0);
	}
	
	char	pathDir[512] = {0};
	ServiceDIR = getenv("ServerDIR");
	if (NULL == ServiceDIR)
	{
		getcwd(pathDir, sizeof(pathDir));
		ServiceDIR = pathDir;
	}
	if (strlen(ServiceDIR) > 128)
	{
		printf("env 'ServerDIR' too long\n");
		exit(0);
	}
	if (ServiceDIR[0] != '/')
	{
		printf("please specify exclusive path\n");
		exit(0);
	}
	
	printf("env 'ServerDIR' = %s\n", ServiceDIR);
	sprintf(binDir, "%s/bin", ServiceDIR);
	sprintf(etcDir, "%s/etc", ServiceDIR);
	sprintf(logDir, "%s/log", ServiceDIR);
	sprintf(fifoDir, "%s/fifo", ServiceDIR);
	
	sprintf(logFileName, "%s/start.log", logDir);

	
	
	int shmid  = 0;
	realKey = MONSHMKEY + getuid() * 100 + realKey;
	//创建共享内存， 先判断共享内存是否存在，不存在则创建
	if ((shmid = shmget((realKey), sizeof(struct TProcessState) + 1000, 0666)) < 0)
	{
		if ((shmid = shmget((realKey), sizeof(struct TProcessState) + 1000, IPC_CREAT | 0666)) < 0)
		{
			LOGINFO("create monitor shm failure");
			printf("create monitor shm failure\n");
			return -1;
		}
	}
	else
	{
		if (bstart)
		{
			printf("system have started!\n");
			LOGINFO("system have started!\n");
			exit(0);
		}
	}

	//连接共享内存
	if ((pshm = (struct TProcessState *) shmat(shmid, NULL, 0)) == (struct TProcessState *) -1)
	{
		LOGINFO("shmat monitor shm failure");
		printf("shmat monitor shm failure\n");
		return -1;
	}

	if (bstop)
	{
		printf("wait stop system...!\n");
		//给进程发送KILL信号
		dead(&pshm->start);
		dead(&pshm->mail);
		dead(&pshm->notify);
		clear(&pshm->start);
		clear(&pshm->mail);
		clear(&pshm->notify);
		for (nProcessNum = 0; nProcessNum < MAXPROCESSES; nProcessNum++)
		{
			if (pshm->processes[nProcessNum].useFlag && pshm->processes[nProcessNum].pid > 0)
			{
				dead(pshm->processes + nProcessNum);
				clear(pshm->processes + nProcessNum);
			}
		}
		
		//共享内存从当前内存内脱离
		if (shmdt(pshm) == -1)
		{
			LOGINFO("shmat monitor shmdt failure");
			printf("shmat monitor shmdt failure\n");
			exit(-1);
		}
		//删除共享内存
		if (shmctl(shmid, IPC_RMID, 0) == -1)
		{
			LOGINFO("shmat monitor shmctl failure");
			printf("shmat monitor shmctl failure\n");
			exit(-1);
		}
		exit(0);
	}
	
	clear(&pshm->start);
	clear(&pshm->mail);
	clear(&pshm->notify);
	for (nProcessNum = 0; nProcessNum < MAXPROCESSES; nProcessNum++)
	{
		clear(pshm->processes + nProcessNum);
	}
	
	int 	nArgc = 0;
	int 	nTotalArgc = 0;
	int 	nTotalProcessNum = 0;
	char  	szIdent[255] = {0};		
	char 	szValues[255] = {0};
	char 	szSection[255] = {0};
	char 	fileName[255] = { 0x00 };
	sprintf(fileName, "%s/start.ini", etcDir);
	CReadIni cReadIni(fileName);
	
	pshm->start.useFlag = 1;
	memcpy(pshm->start.argv[0], "start", strlen("start"));
	
	pshm->mail.useFlag = cReadIni.ReadInteger("mail", "useFlag", 0);
	memcpy(pshm->mail.argv[0], "mail", strlen("mail"));
	
	memset(szValues, 0x00, sizeof(szValues));
	cReadIni.ReadString("mail", "fifo", szValues, "");
	sprintf(pshm->mail.fifo, "%s/%s", fifoDir, szValues);
	
	pshm->notify.useFlag = cReadIni.ReadInteger("notify", "useFlag", 0);;
	memcpy(pshm->notify.argv[0], "notify", strlen("notify"));
	memset(szValues, 0x00, sizeof(szValues));
	cReadIni.ReadString("mail", "fifo", szValues, "");
	sprintf(pshm->notify.fifo, "%s/%s", fifoDir, szValues);
	
	nTotalProcessNum = cReadIni.ReadInteger("START", "ProcessNum", 0);
	for(nProcessNum = 0; nProcessNum < nTotalProcessNum; nProcessNum++)
	{
		memset(szSection, 0x00, sizeof(szSection));
		sprintf(szSection, "Process%d", nProcessNum + 1);
		pshm->processes[nProcessNum].useFlag = cReadIni.ReadInteger(szSection, "useFlag", 0);
		nTotalArgc = cReadIni.ReadInteger(szSection, "argc", 0);
		for(nArgc = 1; nArgc <= nTotalArgc; nArgc++)
		{
			memset(szIdent, 0x00, sizeof(szIdent));
			memset(szValues, 0x00, sizeof(szValues));
			sprintf(szIdent, "argv%d", nArgc);
			cReadIni.ReadString(szSection, szIdent, szValues, "");
			memcpy(pshm->processes[nProcessNum].argv[nArgc - 1], szValues, strlen(szValues));
			printf("ProcessNum:%d, argc:%d, argv:%s \n", nProcessNum, nArgc, szValues);
		}
	}
	
	switch (fork())
	{
	case -1:
		perror("fork:");
		exit(0);
	case 0:
		break;
	default:
		exit(0);
		break;
	}
	
	pshm->start.alive = 1;
	pshm->start.pid = getpid();
	time(&pshm->start.timer);
	start(&pshm->mail);
	start(&pshm->notify);
	for (nProcessNum = 0; nProcessNum < MAXPROCESSES; nProcessNum++)
	{
		if (pshm->processes[nProcessNum].useFlag)
		{
			start(&pshm->processes[nProcessNum]);
		}
	}
	fifoopen(&pshm->mail);
	fifoopen(&pshm->notify);
	
	signal(SIGCHLD, (void(*)(int)) catchSigCLD);
	signal(SIGUSR2, (void(*)(int)) catchSigUsr2);
	signal(SIGUSR1, (void(*)(int)) catchSigUsr1);
	signal(SIGINT, (void(*)(int)) catchSigINT);
	LOGINFO("------- start started ok -------\n");
	printf("------- start started ok -------\n");
	
	fd_set 	fdSet;
	char 	w_buf[255] = {0};
	struct 	timeval waitTime;
	char 	timestr[255]	=	{0};
	for(;;)
	{
		FD_ZERO(&fdSet);
		FD_SET(0,&fdSet);
		waitTime.tv_sec = 0;
		waitTime.tv_usec = 500000;
		if (select(FD_SETSIZE, &fdSet, (fd_set*) 0, (fd_set*) 0, &waitTime) > 0)
		{
			char c;
			read(0, &c, 1);
			switch (c)
			{
			case 'q':
			case 'Q':
				return 0;
			default:
				break;
			}
		}
	
		memset(timestr, 0x00, sizeof(timestr));
		Currenttime(timestr);
		
		pshm->start.alive = 1;
		if(check(&pshm->mail))
		{
			sprintf(w_buf, "mail is crashes, time:%s", timestr);
			LOGINFO("write [%s] to the FIFO\n",w_buf);  
			fifowrite(&pshm->notify, w_buf);
		}
		
		if(check(&pshm->notify))
		{
			char w_buf[255] = {0};
			sprintf(w_buf, "mail is crashes, time:%s", timestr);
			LOGINFO("write [%s] to the FIFO\n",w_buf);  
			fifowrite(&pshm->mail, w_buf);
		}
		
		for (nProcessNum = 0; nProcessNum < MAXPROCESSES; nProcessNum++)
		{
			if(check(&pshm->processes[nProcessNum]))
			{
				char w_buf[255] = {0};
				sprintf(w_buf, "%s is crashes, time:%s", pshm->processes[nProcessNum].argv[0], timestr);
				LOGINFO("write [%s] to the FIFO\n",w_buf);  
				fifowrite(&pshm->mail, w_buf);
				fifowrite(&pshm->notify, w_buf);
			}
		}
	}
	return 0;
}

