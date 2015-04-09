// CheckDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "exchange.h"
#include "CheckDataDlg.h"
#include "afxdialogex.h"


// CCheckDataDlg 对话框

IMPLEMENT_DYNAMIC(CCheckDataDlg, CDialogEx)

CCheckDataDlg::CCheckDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckDataDlg::IDD, pParent)
{

}

CCheckDataDlg::~CCheckDataDlg()
{
}

void CCheckDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCheckDataDlg, CDialogEx)
END_MESSAGE_MAP()


// CCheckDataDlg 消息处理程序
