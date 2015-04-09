#pragma once
#include "afxwin.h"

class CCoderDlg : public CDialog
{
public:
	CCoderDlg(CWnd* pParent = NULL);	// standard constructor
	UINT32			 AppendToLog(CString str, COLORREF color) ;
	enum { IDD = IDD_CODER_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void OnCancel();

protected:
	HICON m_hIcon;
	virtual SDBOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnload();
	afx_msg void OnBtncompile();
	afx_msg void OnBtnoutpath();
	afx_msg LRESULT OnLogMsg(WPARAM wParam ,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void LoadPath();
	void SavePath();

	CString m_strLoadFile;
	CString m_strOutName ;
public:
	CEdit m_edtHFilePath;
public:
	CEdit m_edtCppFilePath;
public:
	CString m_strCppFilePath;
public:
	CRichEditCtrl m_richLogs;
public:
	afx_msg void OnBnClickedBtnoutpath2();
public:
	CString m_strHFilePath;
public:
	CEdit m_edtLoadFile;
	afx_msg void OnBnClickedCheckCpp();
	SDBOOL m_bCpp;
	SDBOOL m_bAs;
	afx_msg void OnBnClickedCheckAs();
	CString m_strAsPath;
	afx_msg void OnBnClickedBtnoutAsPath();
	CEdit m_edtAsPath;

    //
    // JAVA code generator
    //

    afx_msg void OnBnClickedCheckJava();         // click java checked button
    CString m_strJavaPath;                       // java string path
    CString m_strType;
    CString m_strNetCharset;
    CString m_strHostCharset;
    INT32 m_nType;
    INT32 m_nCharsetNet;
    INT32 m_nCharsetHost;
    CEdit m_edtJavaPath;
    SDBOOL m_bJava;
    afx_msg void OnBnClickedBtnoutJavaPath();
    afx_msg void OnCbnSelchangeComboType();
    afx_msg void OnCbnSelchangeComboNet();
    afx_msg void OnCbnSelchangeComboHost();
};
