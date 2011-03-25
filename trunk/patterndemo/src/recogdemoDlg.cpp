// recogdemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "recogdemo.h"
#include "recogdemoDlg.h"

#include "JpegTools.h"

//#include "guidebugconsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecogdemoDlg dialog

CRecogdemoDlg::CRecogdemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecogdemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecogdemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	memset( m_szimagepath, 0, MAX_PATH );
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hbmp = NULL;
	m_buffer_bmp = NULL;
	m_img_width  = 0;
	m_img_heigth = 0;
}

void CRecogdemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecogdemoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRecogdemoDlg, CDialog)
	//{{AFX_MSG_MAP(CRecogdemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_COMMAND(AFX_PROCESS_TOGRAY, OnProcessTogray)
	ON_COMMAND(AFX_PROCESS_BINERIFY, OnProcessBinerify)
	ON_COMMAND(AFX_PROCESS_FILTER, OnProcessFilter)
	ON_COMMAND(AFX_PROCESS_FIND, OnProcessFind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecogdemoDlg message handlers

BOOL CRecogdemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
// 	
//     m_imagetable.CreateEx( WS_EX_CLIENTEDGE, _T("ImageTable"), NULL, WS_VISIBLE | WS_BORDER | WS_CHILD,
//  		CRect( 0, 0, 600, 400 ), this, IDC_TABLE );

	//RedirectIOToConsole();
    m_imagetable.Create( 600, 400, _T("ImageTable"), this );
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRecogdemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRecogdemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	//DeleteDC( dc );
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRecogdemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRecogdemoDlg::OnButtonBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog ofn(TRUE," ","*.jpg",NULL," File(*.jpg)|*.JPG|",NULL);
	
	if(ofn.DoModal()==IDOK)
	{
		CString InPath = ofn.GetPathName();
		strcpy( m_szimagepath, InPath.GetBuffer( MAX_PATH ) );

		int width_bmp = 0;
		int height_bmp = 0;
		int correct_width = 0;
		int size_buffer = 0;
		unsigned char * buffer_bmp = NULL;
		int value_ret = read_jpeg( m_szimagepath, &width_bmp, &height_bmp, &size_buffer, &buffer_bmp );
		
		//for debug
		//jpgtobmp( m_szimagepath, "D:\\out.bmp" );

		if ( value_ret == 0 )
		{
			if ( buffer_bmp != NULL )
			{
				free( buffer_bmp );
				buffer_bmp = NULL;
			}

			return;
		}
		

		//correct_width = ( ( width_bmp + 3 ) / 4 ) * 4;

		BITMAPINFO bmp_info;
		memset( &bmp_info, 0, sizeof(BITMAPINFO) );
		bmp_info.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
		bmp_info.bmiHeader.biWidth = width_bmp;
		bmp_info.bmiHeader.biHeight = height_bmp;
		bmp_info.bmiHeader.biPlanes = 1;
		bmp_info.bmiHeader.biBitCount = 24;
		bmp_info.bmiHeader.biCompression = BI_RGB;
        bmp_info.bmiHeader.biClrUsed = 0;
        bmp_info.bmiHeader.biSizeImage = size_buffer;
		bmp_info.bmiHeader.biXPelsPerMeter = 0;
        bmp_info.bmiHeader.biYPelsPerMeter = 0;
        bmp_info.bmiHeader.biClrImportant = 0;
		bmp_info.bmiColors[0].rgbBlue = 0;
        bmp_info.bmiColors[0].rgbGreen = 0;
        bmp_info.bmiColors[0].rgbRed = 0;
        bmp_info.bmiColors[0].rgbReserved = 0;

		HDC hdc = ::GetDC(NULL);
	    unsigned char * lpDib = NULL;

		if ( m_hbmp != NULL )
		{
			DeleteObject( m_hbmp );
			m_hbmp  = NULL;
		}

		m_hbmp = CreateDIBSection( hdc, &bmp_info, DIB_RGB_COLORS, (void**)&lpDib, NULL, 0 );
		
		memcpy( lpDib, buffer_bmp, size_buffer );

		m_imagetable.ClearImage();
		
		m_imagetable.SetImage(m_hbmp);
		//m_imagetable.CopyImageBufferTo8( buffer_bmp, width_bmp, height_bmp );
		m_imagetable.Invalidate();

		m_buffer_bmp = lpDib;
		m_img_width  = width_bmp;
	    m_img_heigth = height_bmp;

		if ( buffer_bmp != NULL )
		{
			free( buffer_bmp );
			buffer_bmp = NULL;
		}

		::ReleaseDC(NULL,hdc);

	}
	else
	{
		AfxMessageBox("File open Failed!");
		return;
	}
}

void CRecogdemoDlg::OnProcessTogray() 
{
	// TODO: Add your command handler code here
	m_imagetable.CopyImageBufferTo8( m_buffer_bmp, m_img_width, m_img_heigth );
    m_imagetable.Invalidate();
}

void CRecogdemoDlg::OnProcessBinerify() 
{
	// TODO: Add your command handler code here
	m_imagetable.algo_AverageFilter();
	m_imagetable.Invalidate();
}

void CRecogdemoDlg::OnProcessFilter() 
{
	// TODO: Add your command handler code here
	m_imagetable.algo_AverageFilter1();
	m_imagetable.Invalidate();
}

void CRecogdemoDlg::OnProcessFind() 
{
	// TODO: Add your command handler code here
	m_imagetable.algo_findcontent();
    m_imagetable.Invalidate();
}
