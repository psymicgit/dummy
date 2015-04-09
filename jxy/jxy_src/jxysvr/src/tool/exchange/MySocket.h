#pragma once

// MySocket ÃüÁîÄ¿±ê
#include <afxsock.h>


class CExchangeDlg;
class MySocket : public CAsyncSocket
{
public:
	MySocket();
	virtual ~MySocket();

	void SetParent(CExchangeDlg * poExchangeDlg);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

private:
	CExchangeDlg * m_poExchangeDlg;
};


