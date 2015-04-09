#pragma once
#include "protocommondata.h"
#include "DialogResize.h"

// CDriverDlg 对话框

class CDriverDlg : public CDialogResize
{
	DECLARE_DYNAMIC(CDriverDlg)

public:
	CDriverDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDriverDlg();

// 对话框数据
	enum { IDD = IDD_DRIVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP() DECLARE_DLGRESIZE_MAP;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();

	void Init(char* _szDriverID);

private:
	CString m_strDriverID;
};
