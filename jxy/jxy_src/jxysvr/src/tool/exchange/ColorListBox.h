#if !defined(AFX_COLORLISTBOX_H__D23C2394_326F_11D6_AB38_00D0B70C3D79__INCLUDED_)
#define AFX_COLORLISTBOX_H__D23C2394_326F_11D6_AB38_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorListBox window

class CColorListBox : public CListBox
{
// Construction
public:
	CColorListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	int AddString(LPCTSTR lpszItem, COLORREF itemColor = RGB(255,0,0));
	virtual ~CColorListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	// 计算水平滚动条宽度
	void RefushHorizontalScrollBar( void );

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORLISTBOX_H__D23C2394_326F_11D6_AB38_00D0B70C3D79__INCLUDED_)
