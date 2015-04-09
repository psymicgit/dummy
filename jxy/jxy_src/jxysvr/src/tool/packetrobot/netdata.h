

#ifndef _NETDATA_H_
#define _NETDATA_H_

#include "sdtype.h"
#include <vector>
#include <string>
//#include "MGNetDataDecoder.h"


using namespace std;

class CNetData
{
public:
    CNetData();
    virtual ~CNetData();

    VOID  Prepare(CHAR *pNetData, INT32 iSize);
    VOID  Prepare(CHAR *pNetData, INT32 iSize, INT32 iType, INT32 iCharsetNet, INT32 iCharsetHost);
    VOID  Reset();

    CHAR* GetData(){ return m_pBuf; }
    INT32  GetDataLen(){ return m_iPos; }

	INT32 AddByte(UCHAR byByte, UINT16 wLen);
	INT32 DelByte(UCHAR &byByte, UINT16 wLen);

	INT32 AddChar(CHAR chChar, UINT16 wLen);
	INT32 DelChar(CHAR &chChar, UINT16 wLen);

	INT32 AddWord(UINT16 wWord, UINT16 wLen);
	INT32 DelWord(UINT16 &wWord, UINT16 wLen);

	INT32 AddShort(INT16 shShort, UINT16 wLen);
	INT32 DelShort(INT16 &shShort, UINT16 wLen);

	INT32 AddDword(UINT32 dwDword, UINT16 wLen);
	INT32 DelDword(UINT32 &dwDword, UINT16 wLen);

	INT32 AddInt(INT32 iInt, UINT16 wLen);
	INT32 DelInt(INT32 &iInt, UINT16 wLen);

	INT32 AddUint64(UINT64 qwUint64, UINT16 wLen);
	INT32 DelUint64(UINT64& qwUint64, UINT16 wLen);

	INT32 AddInt64(INT64 llInt64, UINT16 wLen);
	INT32 DelInt64(INT64 &llInt64, UINT16 wLen);

	INT32 AddFloat(FLOAT fFloat, UINT16 wLen);
	INT32 DelFloat(FLOAT &fFloat, UINT16 wLen);

	INT32 AddDouble(DOUBLE dbDouble, UINT16 wLen);
	INT32 DelDouble(DOUBLE &dbDouble, UINT16 wLen);	

    INT32 AddString(const CHAR *pszString, INT32 iSize);
    INT32 DelString(CHAR *pszOut, INT32 iSize);

    INT32 AddTString(const TCHAR *pszString, INT32 iSize);
    INT32 DelTString(TCHAR *pszOut, INT32 iSize);

	INT32 AddWString(const WCHAR *pszString, INT32 iSize);
	INT32 DelWString(WCHAR *pszOut, INT32 iSize);

    INT32 Strnlen(const CHAR *pszString, INT32 iSize);

    INT32 AddBuf(const UCHAR* pbyBuf, INT32 iSize);//09-11-18 cwy add, enhance byte array copy
    INT32 DelBuf(UCHAR* pbyBuf, INT32 iSize);//09-11-18 cwy add, enhance byte array copy

	INT32 PassLen(UINT32 dwLen);
	
protected:
    CHAR * m_pBuf;
    INT32 m_iSize;
    INT32 m_iPos;
    INT32 m_iType;
    INT32 m_iCharsetNet;
    INT32 m_iCharsetHost;
};


class CSDProtocol 
{
public:
	virtual INT32 Encode(INT32 iMsgID, VOID *pHost, CHAR *pNet, INT32 iNetSize) = 0;
	virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, VOID *pHost, INT32 iHostSize) = 0;

	std::string      GetName ();
	VOID             SetName(const std::string strName);

	UINT32			 GetMaxMsgId ();
	VOID             SetMaxMsgId(const UINT32 nMaxMsgId);

	UINT32			 GetMinMsgId ();
	VOID             SetMinMsgId(const UINT32 nMinMsgId);

	vector<INT32>*	 GetMsgList();
private:
	std::string      m_strName;
	UINT32           m_nMinMsgId;
	UINT32           m_nMaxMsgId;
protected:
	vector<INT32>	 m_vecMsgList;
};

#endif //#ifndef _NETDATA_H_

