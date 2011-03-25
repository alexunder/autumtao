#if !defined(AFX_IMAGETABLE_H__430D868D_4227_44E4_A419_6822438DE3D9__INCLUDED_)
#define AFX_IMAGETABLE_H__430D868D_4227_44E4_A419_6822438DE3D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageTable.h : header file
//

#define NUM_IMAGE 10

class ImageObject 
{
public:
	ImageObject( HBITMAP m_himg );
	~ImageObject();

	void DrawPic( HDC hdc );
	void SetRect( RECT rect );
private:
	RECT m_rect;
    HBITMAP m_himg;
};
/////////////////////////////////////////////////////////////////////////////
// CImageTable window

class CImageTable : public CWnd
{
// Construction
public:
	CImageTable();

// Attributes
public:

	BOOL Create( int width, int heigth, LPCTSTR lpszWindowName, CWnd* pParentWnd );
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageTable)
	//}}AFX_VIRTUAL

// Implementation
	bool SetImage( HBITMAP hbmp );
	void CopyImageBuffer( unsigned char * buffer, int width, int hight );
    void CopyImageBufferTo8( unsigned char * buffer, int width, int hight );
	
    void algo_AverageFilter();
	void algo_AverageFilter1();
    void algo_findcontent();
	
	void ClearImage();

	void DumpImageData();
public:
	static HBITMAP CreateGrayBmp( unsigned char * buffer, int width, int hight );
public:
	virtual ~CImageTable();

private:
	int m_width;
	int m_heigth;
	int m_iflag;
	
	int m_inum_imgobject;
    ImageObject * m_imageobjects[NUM_IMAGE];

	unsigned char * m_imagedate;
	int m_image_width;
	int m_image_heigth;
	// Generated message map functions
protected:
	//{{AFX_MSG(CImageTable)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGETABLE_H__430D868D_4227_44E4_A419_6822438DE3D9__INCLUDED_)
