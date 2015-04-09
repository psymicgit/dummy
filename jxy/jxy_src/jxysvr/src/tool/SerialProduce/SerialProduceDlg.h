// SerialProduceDlg.h : header file
//

#if !defined(AFX_SERIALPRODUCEDLG_H__FE1DDB87_A190_42DF_9478_E2358BACAB57__INCLUDED_)
#define AFX_SERIALPRODUCEDLG_H__FE1DDB87_A190_42DF_9478_E2358BACAB57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSerialProduceDlg dialog

class CSerialProduceDlg : public CDialog
{
// Construction
public:
	CString m_strIniPath;
	CSerialProduceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSerialProduceDlg)
	enum { IDD = IDD_SERIALPRODUCE_DIALOG };
	CEdit	m_ctrShow;
	CString	m_strActivityID;
	CString	m_strStart;
	CString	m_strEnd;
	CString	m_strShow;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialProduceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
protected:
	CStdioFile m_File;
	CString m_strExePath;
	CString m_strSaveToFile;
    BOOL m_bCreateState;

public:
	void DeleteNotNumber(CString &cs)
	{
		for(int i = 0; i < cs.GetLength();) 
		{   
			if   (cs[i] < '0' || cs[i] > '9')   
				cs.Delete(i,1);   
			else   
				i++;   
		}   
		
	}
	

	// Generated message map functions
	//{{AFX_MSG(CSerialProduceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBCreat();
	afx_msg void OnBOutput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALPRODUCEDLG_H__FE1DDB87_A190_42DF_9478_E2358BACAB57__INCLUDED_)
