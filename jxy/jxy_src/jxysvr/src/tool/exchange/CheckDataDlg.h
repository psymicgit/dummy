#pragma once


// CCheckDataDlg 对话框

class CCheckDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckDataDlg)

public:
	CCheckDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheckDataDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
