#pragma once
#include "resource.h"


// CEditDlg 对话框

class CEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditDlg)

public:
	CEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strTitle;
	CString m_strPro;
	CString m_strValue;
};
