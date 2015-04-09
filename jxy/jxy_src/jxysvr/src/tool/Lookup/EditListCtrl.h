#pragma once


// CEditListCtrl

class CEditListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEditListCtrl)

public:
	CEditListCtrl();
	virtual ~CEditListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CEdit * m_edit;//指向编辑框，初值为NULL
	int     m_item;//当前编辑的行号，初值为-1
	int     m_subitem;//当前编辑的列号，初值为-1
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocus(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL DestroyWindow();
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddLine();
	afx_msg void OnDeleteLine();
};




