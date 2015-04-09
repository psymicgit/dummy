// EditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "exchange.h"
#include "EditDlg.h"
#include "afxdialogex.h"


// CEditDlg 对话框

IMPLEMENT_DYNAMIC(CEditDlg, CDialogEx)

CEditDlg::CEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditDlg::IDD, pParent)
	, m_strPro(_T(""))
	, m_strValue(_T(""))
{

}

CEditDlg::~CEditDlg()
{
}

void CEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PRO, m_strPro);
	DDX_Text(pDX, IDC_EDIT1, m_strValue);
}


BEGIN_MESSAGE_MAP(CEditDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEditDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEditDlg 消息处理程序

BOOL CEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!m_strTitle.IsEmpty())
		SetWindowText(m_strTitle);
	CenterWindow();

	int index = 	m_strTitle.Find(':');
	if (index > 0)
	{
		m_strPro = m_strTitle.Left(index + 1);
		m_strValue = m_strTitle.Mid(index + 1);
	}
	else
	{
		m_strValue = "";
		m_strPro = m_strTitle;
	}
	UpdateData(FALSE);
	return TRUE;
}

void CEditDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	m_strTitle = m_strPro + m_strValue;
	CDialogEx::OnOK();
}
