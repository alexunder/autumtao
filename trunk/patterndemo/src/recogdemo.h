// recogdemo.h : main header file for the RECOGDEMO application
//

#if !defined(AFX_RECOGDEMO_H__563A9614_9CF5_4EEF_A63F_7C70086F898B__INCLUDED_)
#define AFX_RECOGDEMO_H__563A9614_9CF5_4EEF_A63F_7C70086F898B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRecogdemoApp:
// See recogdemo.cpp for the implementation of this class
//

class CRecogdemoApp : public CWinApp
{
public:
	CRecogdemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecogdemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRecogdemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECOGDEMO_H__563A9614_9CF5_4EEF_A63F_7C70086F898B__INCLUDED_)
