
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include "ReadIni.h"
#include "syssocket.h"
#include "base64.h"

#define MAXBUFLEN 	2048
#define PORT 	 	25


char *ServiceDIR = NULL;
char binDir[255] = {0};
char etcDir[255] = {0};
char logDir[255] = {0};
char fifoDir[255] = {0};


static int Currenttime(char * _stime);
static void LOGINFO(char *szFormat, ...);
static int mail(char * user, char *passwd, char * smtp, char *receiver, char *fileContent, char *subject);
static int cmdio_get_cmd(char *cmd);


int main(int argc, char *argv[])
{
	char user[255] = {0};
	char passwd[255] = {0};
	char smtp[255] = {0};
	char receiver[1024] = {0};
	char subject[1024] = {0};
	char content[1024] = {0};
	char ectFileName[255] = {0};
	char fifoName[255] = {0};

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

	sprintf(binDir, "%s/bin", ServiceDIR);
	sprintf(etcDir, "%s/etc", ServiceDIR);
	sprintf(logDir, "%s/log", ServiceDIR);
	sprintf(fifoDir, "%s/fifo", ServiceDIR);
	

	sprintf(ectFileName, "%s/mail.ini", etcDir);


	
	CReadIni cReadIni(ectFileName);
    cReadIni.ReadString("MAIL","USER",user,"asita88");
	cReadIni.ReadString("MAIL","PASSWD",passwd,"morrow88");
	cReadIni.ReadString("MAIL","SMTP",smtp,"smtp.126.com");
	cReadIni.ReadString("MAIL","RECEIVER",receiver,"542581595@qq.com");
	cReadIni.ReadString("MAIL","SUBJECT",subject,"test");
	cReadIni.ReadString("MAIL","CONTENT",content,"this is a test mail!");
	cReadIni.ReadString("MAIL","FIFO",fifoName,"mailfifo");
	
	char fifoPathName[255] = {0};
	sprintf(fifoPathName, "%s/%s", fifoDir, fifoName);
	if((mkfifo(fifoPathName,O_CREAT | O_EXCL)<0)&&(errno!=EEXIST))       //创建并执行   
	{
		printf("cannot create fifo:%s\n", fifoPathName);  
	}
	
	int  fd;  
	int  nread;  
	char buf_r[MAXBUFLEN];  	
	memset(buf_r,0,sizeof(buf_r));  
	//O_NONBLOCK //readonly 不阻塞   
	fd=open(fifoPathName,O_RDONLY, 0);                     
	if(fd == -1)  
	{  
		perror("open");  
		exit(1);      
	}  
	
	while(1)  
	{  
		memset(buf_r,0,sizeof(buf_r));  
		if((nread=read(fd, buf_r, MAXBUFLEN))==-1)
		{                //读取管道   
			if(errno==EAGAIN)  
				printf("no data yet\n");  
		}
		else
		{
			printf("mail read %s from FIFO\n",buf_r);
			mail(user, passwd, smtp, receiver, buf_r, subject);			
		} 
	} 
    return 0;
}



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

	char logFileName[255] = {0};
	sprintf(logFileName, "%s/mail.log", logDir);

	if ((fp = fopen(logFileName, "a+t")) == NULL)
	{
		printf("open log file '%s' failed.\n", logFileName);
		exit(-1);
	}
	fprintf(fp, "[%s]:%s\n", _stime, szBuffer);
	fclose(fp);
}


//发送邮件
static int mail(char * user, char *passwd, char * smtp, char *receiver, char *fileContent, char *subject)
{
    int		sockfd = -1;
    int		retConnect = -1;
    char	msg[MAXBUFLEN] = "";
    char	buf[255] = "";
	char    ip[255] = {0};
	
	sysutil_dns_resolve(ip, sizeof(ip), smtp);
	LOGINFO("[smtp:%s]->[ip:%s]\n", smtp, ip);
	sockfd = sysutil_get_ipv4_sock();
	if(sockfd < 0)
    {
        return -1;       
    }
	
	retConnect = sysutil_connect_timeout(sockfd, ip, PORT, 0);
    if(retConnect < 0)
    {
		close(sockfd);
        return -1;       
    }
	LOGINFO("connect ok\n");
	retConnect = sysutil_read(sockfd, msg, sizeof(msg));
    if(retConnect < 0)
    {
		close(sockfd);
        return -1;        
    }
	LOGINFO("read:%s\n", msg);
	if(220 != cmdio_get_cmd(msg))
	{
		close(sockfd);
		return -1;
	}
	
    memset(msg, '\0', sizeof(msg));
    strcpy(msg, "HELO SERVER\r\n");
    retConnect = sysutil_write(sockfd, msg, strlen(msg));
    if(retConnect < 0)
    {
		close(sockfd);
        return -1;        
    }
	LOGINFO("write:%s\n", msg);
	
    memset(buf, '\0', sizeof(buf));
    retConnect = sysutil_read(sockfd, msg, sizeof(msg));
    if(retConnect < 0)
    {
		close(sockfd);
        return -1;        
    }
	LOGINFO("read:%s\n", msg);
	if(250 != cmdio_get_cmd(msg))
	{
		close(sockfd);
		return -1;
	}
	
    //用户准备登录
    memset(msg, '\0', sizeof(msg));
    strcpy(msg, "AUTH LOGIN \r\n");
    retConnect = sysutil_write(sockfd, msg, strlen(msg));
    if(retConnect < 0)
    {
		close(sockfd);
        return -1;        
    }
	LOGINFO("write:%s\n", msg);

	
    memset(buf, '\0', sizeof(buf));
    retConnect = sysutil_read(sockfd, msg, sizeof(msg));
    if(retConnect < 0)
    {
		close(sockfd);
        return -1;        
    }
	LOGINFO("read:%s\n", msg);
	if(334 != cmdio_get_cmd(msg))
	{
		close(sockfd);
		return -1;
	}

    //用户登录包括二步
    //1、发送用户名
    //2、发送用户密码
    //用户名和密码必须用BASE64进行编码
    //发送用户名
    memset(buf, 0, sizeof(buf));
    base64(buf, (char*)user, strlen(user));
    memset(msg, '\0', sizeof(msg));
    sprintf(msg, "%s\r\n", buf);
    retConnect = sysutil_write(sockfd, msg, strlen(msg));
    if(retConnect < 0)
    {
		close(sockfd);
        return -1;        
    }
	LOGINFO("write:%s\n", msg);

	
    memset(buf, '\0', sizeof(buf));
    retConnect = sysutil_read(sockfd, msg, sizeof(msg));
    if(retConnect < 0)
    {
		close(sockfd);
        return -1;        
    }
	LOGINFO("read:%s\n", msg);
	if(334 != cmdio_get_cmd(msg))
	{
		close(sockfd);
		return -1;
	}

    //发送密码
    memset(buf, 0, sizeof(buf));
    base64(buf, passwd, strlen(passwd));
    memset(msg, '\0', sizeof(msg));
    sprintf(msg, "%s\r\n", buf);
    retConnect = sysutil_write(sockfd, msg, strlen(msg));
    if(retConnect < 0)
    {
		close(sockfd);
        return -1;        
    }
	LOGINFO("write:%s\n", msg);
	
    memset(buf, '\0', sizeof(buf));
    retConnect = sysutil_read(sockfd, msg, sizeof(msg));
    if(retConnect < 0)
    {
		close(sockfd);
        return -1;        
    }
	LOGINFO("read:%s\n", msg);
	if(235 != cmdio_get_cmd(msg))
	{
		close(sockfd);
		return -1;
	}
	
	char *pstmp = strstr(smtp, "smtp.");
	if(NULL != pstmp)
	{
		pstmp += 5;
	}
	else
	{
		pstmp = smtp;
	}
	
    //发送发件人
    memset(msg, '\0', sizeof(msg));
    sprintf(msg, "MAIL FROM: <%s@%s>\r\n", user, pstmp);
    retConnect = sysutil_write(sockfd, msg, strlen(msg));
    if(retConnect < 0)
    {
		close(sockfd);
        return -1;        
    }
	LOGINFO("write:%s\n", msg);
	
    memset(buf, '\0', sizeof(buf));
    retConnect = sysutil_read(sockfd, msg, sizeof(msg));
    if(retConnect < 0)
    {
		close(sockfd);
        return -1;        
    }
	LOGINFO("read:%s\n", msg);
	if(250 != cmdio_get_cmd(msg))
	{
		close(sockfd);
		return -1;
	}

    //发送收件人
    char * pStart		=	(char*)receiver;
    char * pEnd		=	NULL;
    do
    {
        memset(msg, '\0', sizeof(msg));
        //收件人   以<>括起来
        strcpy(msg, "RCPT TO: <");

        pEnd = strchr(pStart, ';');
        if (NULL != pEnd)
        {
            strncat(msg, pStart, pEnd - pStart);
            pStart = pEnd + 1;
        }
        else if (strlen(pStart))
        {
            strcat(msg, pStart);
        }
        else
        {
            break;
        }

        strcat(msg, ">\r\n");
        retConnect = sysutil_write(sockfd, msg, strlen(msg));
		if(retConnect < 0)
		{
			close(sockfd);
			return -1;        
		}
		LOGINFO("write:%s\n", msg);
		
        memset(buf, '\0', sizeof(buf));
        retConnect = sysutil_read(sockfd, msg, sizeof(msg));
		if(retConnect < 0)
		{
			close(sockfd);
			return -1;        
		}
		LOGINFO("read:%s\n", msg);
		if(250 != cmdio_get_cmd(msg))
		{
			close(sockfd);
			return -1;
		}
    }
    while (NULL != pEnd);




    //准备发送邮件内容
    memset(msg, '\0', sizeof(msg));
    strcpy(msg, "DATA\r\n");
    retConnect = sysutil_write(sockfd, msg, strlen(msg));
	if(retConnect < 0)
	{
		close(sockfd);
		return -1;        
	}
	LOGINFO("write:%s\n", msg);
	
	
    memset(buf, '\0', sizeof(buf));
    retConnect = sysutil_read(sockfd, msg, sizeof(msg));
	if(retConnect < 0)
	{
		close(sockfd);
		return -1;        
	}
	LOGINFO("read:%s\n", msg);
	if(354 != cmdio_get_cmd(msg))
	{
		close(sockfd);
		return -1;
	}


    //发送邮件内容
    //包括邮件头:发件人，收件人，主题
    //和邮件正文
    memset(msg, '\0', sizeof(msg));
    //发件人
    strcpy(msg, "From: ");
    strcat(msg, user);
    strcat(msg, "@126.com\r\n");
    //收件人
    strcat(msg, "To: <");
    strcat(msg, receiver);
    strcat(msg, ">\r\n");
    //主题
    strcat(msg, "Subject: ");
    strcat(msg, subject);
    strcat(msg, "\r\n\r\n");
    //邮件内容
    strcat(msg, fileContent);
    strcat(msg, "\r\n");
    //邮件内容结束 以“\r\n.\r\n”结尾
    strcat(msg, "\r\n.\r\n");
    LOGINFO("%s\n", msg);
    retConnect = sysutil_write(sockfd, msg, strlen(msg));
	if(retConnect < 0)
	{
		close(sockfd);
		return -1;        
	}
	LOGINFO("write:%s\n", msg);

    //发送邮件结束
    memset(msg, '\0', sizeof(msg));
    strcpy(msg, "QUIT \r\n");
    retConnect = sysutil_write(sockfd, msg, strlen(msg));
	if(retConnect < 0)
	{
		close(sockfd);
		return -1;        
	}
	LOGINFO("write:%s\n", msg);
    memset(buf, '\0', sizeof(buf));
    retConnect = sysutil_read(sockfd, msg, sizeof(msg));
	if(retConnect < 0)
	{
		close(sockfd);
		return -1;        
	}
	LOGINFO("read:%s\n", msg);
	if(354 != cmdio_get_cmd(msg))
	{
		close(sockfd);
		return -1;
	}
	
    //发送邮件结束，关闭Socket
    close(sockfd);
    return 0;
}

static int cmdio_get_cmd(char *cmd)
{
	char *p = strchr(cmd, ' ');
	if(p == NULL)
	{
		return -1;
	}
	char tempcmd[128] = {0};
	memcpy(tempcmd, cmd, p - cmd);
	return atoi(tempcmd);
	
}





