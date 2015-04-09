
/*       Version Number: 1          */

#ifndef LOGPROTO_H_INCLUDE_VERSION_1
#define LOGPROTO_H_INCLUDE_VERSION_1

#include "sddatastream.h"

/*Defines Begin*/
#define LOG_MAX_LENGTH           1024 * 10
#define LOG_DIV_SINGLE           1
#define LOG_DIV_PER_DAY          2
#define LOGSERVER_TYPE           79
/*Defines End*/

using namespace SGDP;

/*Define Structs and Unions        Begin*/
typedef struct tagCliDate{
	unsigned int     dwYear;
	unsigned int     dwMonth;
	unsigned int     dwDay;
}CliDate;

typedef struct tagCLIPKG_LOG{
	unsigned char    byServerType;  //说明封包哪一种Server发出的
	unsigned char    byServerID;    //客户端自身的Id，用于标示封包是哪个服务器发出的
	unsigned char    byLogFileType; 
	CliDate          stDate;
	char             szLog[LOG_MAX_LENGTH];
}CLIPKG_LOG;

/*Define Structs and Unions        End*/


enum EN_LogProto_MessageID
{
	CLIID_LOG                = 2001,
};

class CLogProto;
typedef int (CLogProto::*EnFuncCLogProto)(void *pHost);
typedef int (CLogProto::*DeFuncCLogProto)(void *pHost);

class CLogProto
{
public:
	CLogProto();
	~CLogProto();
	int Encode(int iMsgID, void *pHost, char *pNet, int iNetSize);
	int Decode(int iMsgID, char *pNet,int iNetSize, void *pHost);
	EnFuncCLogProto FindEncodeFunc(int iMsgID);
	DeFuncCLogProto FindDecodeFunc(int iMsgID);
	int EncodeCliDate(void *pHost);
	int DecodeCliDate(void *pHost);
	int EncodeCLIPKG_LOG(void *pHost);
	int DecodeCLIPKG_LOG(void *pHost);
	bool Init();

private:
	EnFuncCLogProto m_EncodeFuncArray[8192];
	DeFuncCLogProto m_DecodeFuncArray[8192];
	CSDDataStream m_oData;
};

#endif

