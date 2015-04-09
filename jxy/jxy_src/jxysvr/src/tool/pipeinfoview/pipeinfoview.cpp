#include <map>
#include <string>
#include <iostream>
#include <sdtype.h>
#include <sdhashmap.h>
#include <set>
#include <sdshmem.h>
#include <sdtime.h>
#include <sdserverid.h>
#include <sdnetutils.h>
#ifdef __linux__
#include <unistd.h>
#else
#include <windows.h>
#include <conio.h>
#include <tlhelp32.h>
#endif

using namespace SGDP;
using namespace std;

#ifdef __linux__

string GetPidDir(UINT32 dwPid)
{
	CHAR szProc[64] = {0}; 
	CHAR szBuf[ 256 ] = {0}; 

	sprintf( szProc, "/proc/%d/exe", dwPid); 

	INT32 nRet = readlink(szProc, szBuf, 256 ); 

	if( nRet < 0 || nRet >= 256 ) 
	{ 
		cout<<"readlink failed!"<<endl;
		return "";
	} 
	szBuf[nRet] = 0; 
	string strPath = szBuf;
	INT32 nPos = strPath.find_last_of("/");
	if(nPos <= 0)
	{
		return "";
	}
	return strPath.substr(0, nPos);	
}
#else
string GetPidDir(UINT32 dwPid)
{
	HANDLE hModule;
	MODULEENTRY32 minfo;
	minfo.dwSize=sizeof(MODULEENTRY32);
	CHAR szBuf[ 256 ] = {0}; 
	hModule=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,dwPid);
	Module32First(hModule, &minfo);
	GetShortPathName(minfo.szExePath, szBuf, 256);
	szBuf[255] = 0; 
	CloseHandle(hModule);

	string strPath = szBuf;
	INT32 nPos = strPath.find_last_of("\\");
	if(nPos <= 0)
	{
		return "";
	}
	return strPath.substr(0, nPos);	

}

#endif


int main(int argc, char** argv)
{
	if(argc != 2)
	{
		cout<<"usage:"<<argv[0]<<" [pid]"<<endl;
		return -1;
	}
	SShmem	shmemTest;
	shmemTest.handle = NULL;
	CHAR szFile[256] = {0};
	_SDTsnprintf(szFile, 256, "%s/%s", GetPidDir(SDAtou(argv[1])).c_str(),"pipeinfo");

	if(SDShmemExist(szFile))
	{
		SDAttachShm(&shmemTest, szFile);
	}
	if(NULL == shmemTest.handle)
	{
		cout<<szFile<<" not exit!"<<endl;
		return -2;
	}

	while(1)
	{
		SAllPipeInfo* pstAllPipeInfo = (SAllPipeInfo*)(shmemTest.mem);
		for(UINT8 byIdx = 0; byIdx < pstAllPipeInfo->byPipeNum; byIdx++)
		{
			SPipeInfo* pInfo = &(pstAllPipeInfo->astPipeInfo[byIdx]);
			CSDServerID serverId(pInfo->dwID);
			tstring strServerName = GetServerShortName(serverId.GetServerType());
			tstring strIP         = SDInetNtoa(pInfo->dwIP);
			UINT16 wPort          = pInfo->wPort;
			tstring strStatus     = pInfo->bConnect ? _SDT("Connected") : _SDT("Disconnected");
			tstring strDirection  = pInfo->bDirection ? _SDT("Listener") : _SDT("Connection");

			TCHAR szBuf[256];
			_SDTsnprintf(szBuf,256, _SDT("%2s%8s%16s:%6d %s%18s"), 
				serverId.AsString().c_str(), 
				strServerName.c_str(),
				strIP.c_str(),
				wPort,
				strStatus.c_str(),
				strDirection.c_str());

			cout<<szBuf<<endl;
		}
		SDSleep(2000);
	}


	return 0;
}
