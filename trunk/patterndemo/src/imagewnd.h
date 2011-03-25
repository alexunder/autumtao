/* BUILD Version: 0001
 *
 *  imagewnd v1.0
 *
 *  Copyright (c) 1999--2007 Lingtu Software Corporation
 *
 *  imagewnd.h
 *  
 *
 *  History:
 *  Created 15:11:2007  by alexu
*/

#ifndef __H_IMAGEWND
#define __H_IMAGEWND

class CImageWnd : public CWnd
{
private:
	HBITMAP m_himg;
	int     m_iflag;
// Construction
public:
	CImageWnd();
public:
	virtual ~CImageWnd();
   
	void setimage( HBITMAP hbmp );
	// Generated message map functions
protected:
	//{{AFX_MSG(CTable)
	afx_msg void OnPaint();
	//afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
