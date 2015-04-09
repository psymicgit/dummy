// DriverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Lookup.h"
#include "DriverDlg.h"
#include "afxdialogex.h"


// CDriverDlg 对话框

IMPLEMENT_DYNAMIC(CDriverDlg, CDialogEx)

CDriverDlg::CDriverDlg(CWnd* pParent /*=NULL*/)
	: CDialogResize(CDriverDlg::IDD, pParent)
	, m_strDriverID(_T(""))
{

}

CDriverDlg::~CDriverDlg()
{
}

void CDriverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DRIVER_ID, m_strDriverID);
}


BEGIN_MESSAGE_MAP(CDriverDlg, CDialogResize)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CDriverDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DLGRESIZE_MAP(CDriverDlg)
	DLGRESIZE_CONTROL(IDC_DRIVER_ID, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X)
END_DLGRESIZE_MAP()

// CDriverDlg 消息处理程序


BOOL CDriverDlg::OnInitDialog()
{
	CDialogResize::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitResizing(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDriverDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogResize::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


void CDriverDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDriverDlg::Init(char* _szDriverID)
{
	m_strDriverID = _szDriverID;
}
