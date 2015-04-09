/*
 ============================================================================
 Name        : monitor.c
 Author      : wengyanhai
 Version     :
 Copyright   : Your copyright notice
 Description : Ansi-style
 ============================================================================
 */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <curses.h>

char* version 	= "version 1.0.0";
char* copyRight = "Copyright(c) 2012-2013, All Rights reserved.\n\n";

#define MAXPROCESSES 	10
#define MONSHMKEY    	20121212
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

struct TRoundStick
{
	int x;
	int y;
	char state;
};

struct TProcessState* 	pshm = NULL;
struct TRoundStick* 	pStick = NULL;

void initScreen();
void checkProcesses();
void InitTRoundStick(struct TRoundStick *pRoundStick, int xx, int yy);
void Round(struct TRoundStick *pRoundStick, struct TProcessDaemon  * pProcessDaemon);




int main(int argc, char* argv[])
{
	char ch;
	int  realKey = 0;
	while ((ch = getopt(argc, argv, "hvk:")) != -1)
	{
		switch (ch)
		{
		case 'v':
			printf("%s\n", version);
			printf("%s\n", copyRight);
			exit(0);
			break;
		case 'h':
			printf("Usage\n");
			printf("%s\n", version);
			printf("%s\n", copyRight);
			exit(0);
			break;
		case 'k':
			realKey = atoi(optarg);
			break;
		default:
			printf("命令行参数错误: 没有该选项-%c\n", (char) optopt);
			exit(-1);
		}
	}
	
	int  shmID = 0;
	realKey = MONSHMKEY + getuid() * 100 + realKey;
	printf("realKey:%d\n", realKey);
	if ((shmID = shmget((realKey), sizeof(struct TProcessState) + 1000, 0666)) < 0)
	{
		perror("Error:");
		printf("create monitor shmget failure \n");
		return -1;
	}

	if ((pshm = (struct TProcessState *) shmat(shmID, NULL, 0)) == (struct TProcessState *) -1)
	{
		printf("monitor shmat failure\n");
		return -1;
	}
	initScreen();
	struct timeval waitTime;
	fd_set fdSet;


	while (1)
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
				endwin();
				return 0;
			default:
				break;
			}
		}
		checkProcesses();
	}
	return EXIT_SUCCESS;
}

void InitTRoundStick(struct TRoundStick *pRoundStick, int xx, int yy)
{
	pRoundStick->x = xx;
	pRoundStick->y = yy;
	pRoundStick->state = '|';
}

void Round(struct TRoundStick *pRoundStick, struct TProcessDaemon  * pProcessDaemon)
{
	switch (pRoundStick->state)
	{
	case '|':
		pRoundStick->state = '/';
		break;
	case '/':
		pRoundStick->state = '\\';
		break;
	case '\\':
		pRoundStick->state = '|';
		break;
	}
	move(pRoundStick->x, pRoundStick->y);
	addch(pRoundStick->state);
	
	char szTemp[255] = {0};
	memset(szTemp, 0x00, sizeof(szTemp));
	move(pRoundStick->x, pRoundStick->y + 16);
	sprintf(szTemp, "%5u", pProcessDaemon->pid);
	addstr(szTemp);
	
	struct tm * ptm;
	ptm = localtime(&pProcessDaemon->timer);
	memset(szTemp, 0x00, sizeof(szTemp));
	move(pRoundStick->x, pRoundStick->y + 32);
	sprintf(szTemp, "%4d-%02d-%02d %02d:%02d:%02d ", (1900 + ptm->tm_year), (1 + ptm->tm_mon), ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	addstr(szTemp);
}


void initScreen()
{
	initscr();
	noecho();
	move(0, 0);
	char temp[255];
	sprintf(temp, "========================= server processes monitor ==========================\n");
	addstr(temp);
	addstr("\n");
	addstr("Name         		State               PID                 Timer               \n");
	addstr("--------------------------------------------------------------------------------\n");
	
	int i = 0;
	pStick = (struct TRoundStick*) malloc(sizeof(struct TRoundStick) * (MAXPROCESSES + 3));
	if (pshm->start.useFlag)
	{
		addstr(pshm->start.argv[0]);
		InitTRoundStick(pStick + i, 4 + i * 2, 26);
		i++;
		addstr("\n");
		addstr("--------------------------------------------------------------------------------\n");
	}
	
	if (pshm->mail.useFlag)
	{
		addstr(pshm->mail.argv[0]);
		InitTRoundStick(pStick + i, 4 + i * 2, 26);
		i++;
		addstr("\n");
		addstr("--------------------------------------------------------------------------------\n");
	}
	
	if (pshm->notify.useFlag)
	{
		addstr(pshm->notify.argv[0]);
		InitTRoundStick(pStick + i, 4 + i * 2, 26);
		i++;
		addstr("\n");
		addstr("--------------------------------------------------------------------------------\n");
	}
	
	int nMoveLine =  4 + i * 2;
	for (i = 0; i < MAXPROCESSES ; i++)
	{
		if (pshm->processes[i].useFlag)
		{
			addstr(pshm->processes[i].argv[0]);
			InitTRoundStick(pStick + i + 3, nMoveLine + i * 2, 26);
			addstr("\n");
			addstr("--------------------------------------------------------------------------------\n");
		}
	}
	
	
	addstr("\n");
	addstr("[Q]Quit  ");
	addstr("\n");
	addstr("================================================================================\n");
	refresh();
}

void checkProcesses()
{
	//start进程信息
	int i = 0;
	if (pshm->start.useFlag)
	{
		if(pshm->start.alive)
			Round(pStick + i, &pshm->start);
		i++;
	}
	
	if (pshm->mail.useFlag)
	{
		if (pshm->mail.alive)
			Round(pStick + i, &pshm->mail);
		i++;
	}
	
	if (pshm->notify.useFlag)
	{
		if (pshm->notify.alive)
			Round(pStick + i, &pshm->notify);
		i++;
	}
	for (i = 0; i < MAXPROCESSES; i++)
	{
		if (pshm->processes[i].alive && pshm->processes[i].useFlag)
		{
			Round(pStick + i + 3, pshm->processes + i);
		}
	}
	move(32, 0);
	refresh();
	pshm->start.alive = 0;
}



