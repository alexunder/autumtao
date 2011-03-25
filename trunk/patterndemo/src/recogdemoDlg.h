// recogdemoDlg.h : header file
//

#if !defined(AFX_RECOGDEMODLG_H__8850DCAB_B2D2_4587_9A87_85AB40E356C4__INCLUDED_)
#define AFX_RECOGDEMODLG_H__8850DCAB_B2D2_4587_9A87_85AB40E356C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRecogdemoDlg dialog
#include "ImageTable.h"

class CRecogdemoDlg : public CDialog
{
// Construction
public:
	CRecogdemoDlg(CWnd* pParent = NULL);	// standard constructor

	~CRecogdemoDlg()
	{
		if ( m_hbmp != NULL )
		{
			DeleteObject( m_hbmp );
		}
	}

	
// Dialog Data
	//{{AFX_DATA(CRecogdemoDlg)
	enum { IDD = IDD_RECOGDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	CImageTable m_imagetable;
	//}}AFX_DATA
    
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecogdemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRecogdemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonBrowse();
	afx_msg void OnProcessTogray();
	afx_msg void OnProcessBinerify();
	afx_msg void OnProcessFilter();
	afx_msg void OnProcessFind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	char m_szimagepath[MAX_PATH];
	HBITMAP  m_hbmp;
	unsigned char * m_buffer_bmp;
	int m_img_width;
	int m_img_heigth;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECOGDEMODLG_H__8850DCAB_B2D2_4587_9A87_85AB40E356C4__INCLUDED_)
