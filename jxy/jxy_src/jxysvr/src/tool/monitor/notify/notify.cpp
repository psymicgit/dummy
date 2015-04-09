#include <stdio.h>
#include "SSLComm.h"
#include "ReadIni.h"

#define MAXBUFLEN 2048

char *ServiceDIR = NULL;
char binDir[255] = {0};
char etcDir[255] = {0};
char logDir[255] = {0};
char fifoDir[255] = {0};


int main(int argc, char **argv)
{
	char pathDir[512] = {0};
	char key[255] = {0};
	char ectFileName[255] = {0};
	char fifoName[255]	=	{0};
	char driverId[255] = {0};
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
	
	sprintf(ectFileName, "%s/notify.ini", etcDir);
	CReadIni cReadIni(ectFileName);
    	cReadIni.ReadString("NOTIFY","KEY",key,"a.cap");
	cReadIni.ReadString("NOTIFY","FIFO",fifoName,"notifyfifo");
	cReadIni.ReadString("NOTIFY","DRIVERID",driverId,"abba27b01f61e0c2852eb20dc87d4c265aefec031c8a87fcce45db77376b4c07");
	
	CSSLComm m_nSSLComm;
	m_nSSLComm.PushNotificationEx(driverId, "12345678987654321");
	
	return 0;
	
	
	char fifoPathName[255] = {0};
	sprintf(fifoPathName, "%s/%s", fifoDir, fifoName);
	if((mkfifo(fifoPathName,O_CREAT | O_EXCL)<0)&&(errno!=EEXIST))       //创建并执行   
	{
		printf("cannot create fifo:%s\n", fifoPathName);  
	}
	
	int  fd;  
	int  nread;  
	char r_buf[MAXBUFLEN];  	
	memset(r_buf,0,sizeof(r_buf));   
	fd=open(fifoPathName,O_RDONLY, 0);                     
	if(fd == -1)  
	{  
		perror("open");  
		exit(1);      
	}  
	
	while(1)  
	{  
		memset(r_buf,0,sizeof(r_buf));  
		if((nread=read(fd, r_buf, MAXBUFLEN))==-1)
		{                //读取管道   
			if(errno==EAGAIN)  
				printf("no data yet\n");  
		}
		else
		{
			printf("read %s from FIFO\n",r_buf);	
			CSSLComm m_nSSLComm;
			m_nSSLComm.PushNotification(driverId, "123456789");
		} 
	}
	return 0;
}