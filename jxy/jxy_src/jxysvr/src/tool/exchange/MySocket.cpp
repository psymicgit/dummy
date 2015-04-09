// MySocket.cpp : 实现文件
//

#include "stdafx.h"
#include "exchange.h"
#include "MySocket.h"
#include "exchangeDlg.h"


// MySocket

MySocket::MySocket()
{

}

MySocket::~MySocket()
{
}


// MySocket 成员函数


void MySocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	//if (0 == nErrorCode)
	{
		m_poExchangeDlg->OnClose(nErrorCode);
	}
	CAsyncSocket::OnClose(nErrorCode);
}


void MySocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (0 == nErrorCode)
	{
		m_poExchangeDlg->OnConnect(nErrorCode);
	}
	CAsyncSocket::OnConnect(nErrorCode);
}


void MySocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (0 == nErrorCode)
	{
		m_poExchangeDlg->OnReceive(nErrorCode);
	}
	CAsyncSocket::OnReceive(nErrorCode);
}


void MySocket::SetParent(CExchangeDlg * poExchangeDlg)
{
	m_poExchangeDlg = poExchangeDlg;
}