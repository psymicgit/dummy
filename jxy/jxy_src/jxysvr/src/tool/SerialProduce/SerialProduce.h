// SerialProduce.h : main header file for the SERIALPRODUCE application
//

#if !defined(AFX_SERIALPRODUCE_H__24FD1710_FC4C_4BCE_A86B_23ADE323172E__INCLUDED_)
#define AFX_SERIALPRODUCE_H__24FD1710_FC4C_4BCE_A86B_23ADE323172E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSerialProduceApp:
// See SerialProduce.cpp for the implementation of this class
//

class CSerialProduceApp : public CWinApp
{
public:
	CSerialProduceApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialProduceApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSerialProduceApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALPRODUCE_H__24FD1710_FC4C_4BCE_A86B_23ADE323172E__INCLUDED_)
