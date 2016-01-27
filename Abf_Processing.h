// Abf_Processing.h : main header file for the ABF_PROCESSING application
//

#if !defined(AFX_ABF_PROCESSING_H__829A0805_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_)
#define AFX_ABF_PROCESSING_H__829A0805_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAbf_ProcessingApp:
// See Abf_Processing.cpp for the implementation of this class
//

class CAbf_ProcessingApp : public CWinApp
{
public:
	CAbf_ProcessingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbf_ProcessingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAbf_ProcessingApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABF_PROCESSING_H__829A0805_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_)
