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
    afx_msg LRESULT OnLogMsg(WPARAM wParam , LPARAM lParam);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
    void LoadPath();
    void SavePath();
	//void ProcessCommandLine(); //处理命令行的逻辑

private:
	void CompileCode();

public:
    CString m_strHFilePath;
    CString m_strOutName ;
    CString m_strCppFilePath;
    CRichEditCtrl m_richLogs;
    SDBOOL m_bCpp;
    SDBOOL m_bAs;
	SDBOOL m_bLua;
    CString m_strAsPath;
    CEdit m_edtAsPath;
    CString m_strJavaPath;                       // java string path
	CString m_strLuaPath; 
    CString m_strType;
    CString m_strNetCharset;
    CString m_strHostCharset;
    INT32 m_nType;
    INT32 m_nCharsetNet;
    INT32 m_nCharsetHost;
    CEdit m_edtJavaPath;
	CEdit m_edtLuaPath;
    SDBOOL m_bJava;
    CString m_strLoadFile;

    afx_msg void OnBnClickedCheckCpp();
    afx_msg void OnBnClickedCheckAs();
    afx_msg void OnBnClickedBtnoutAsPath();
    afx_msg void OnBnClickedBtnoutJavaPath();
    afx_msg void OnCbnSelchangeComboNet();
    afx_msg void OnCbnSelchangeComboHost();
    afx_msg void OnBnClickedCheckJava();         // click java checked button
    afx_msg void OnBnClickedBtnoutpath2();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckVercmp2();

private:
	BOOL m_bGenJsonInfo;
	BOOL m_bSaveEnDe;
	BOOL m_bCliEnDe;
public:
	afx_msg void OnBnClickedCheckLua();
	afx_msg void OnBnClickedBtnoutLuaPath();
};
