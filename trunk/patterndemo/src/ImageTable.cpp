 // ImageTable.cpp : implementation file
//

#include "stdafx.h"
#include "recogdemo.h"
#include "ImageTable.h"

#include "recognize_engine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 

/*
 tool function
*/
int DrawBitmap( HDC hDC, HBITMAP hBitmap, int x, int y )
{
    HDC hMemDC;
    HBITMAP hOldBmp;
    BITMAP bmp;
    int    bRet;

    hMemDC  = CreateCompatibleDC( hDC );
    hOldBmp = (HBITMAP)SelectObject( hMemDC, hBitmap );
    GetObject( hBitmap, sizeof( BITMAP ), &bmp );
    bRet    = BitBlt( hDC, x, y, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY );
    SelectObject( hMemDC, hOldBmp );
    DeleteDC( hMemDC );

    return bRet;
}


int DrawRectBitmap( HDC hDC, HBITMAP hBitmap, RECT rect )
{
    HDC hMemDC;
    HBITMAP hOldBmp;
    BITMAP bmp;
    int    bRet;

    hMemDC  = CreateCompatibleDC( hDC );
    hOldBmp = (HBITMAP)SelectObject( hMemDC, hBitmap );
    GetObject( hBitmap, sizeof( BITMAP ), &bmp );
    //bRet    = BitBlt( hDC, x, y, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY );
	bRet    = StretchBlt( hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY );
    SelectObject( hMemDC, hOldBmp );
    DeleteDC( hMemDC );

    return bRet;
}


int DrawBitmapStrench( HDC hDC, HBITMAP hBitmap, int x, int y )
{
    HDC hMemDC;
    HBITMAP hOldBmp;
    BITMAP bmp;
    int    bRet;

    hMemDC  = CreateCompatibleDC( hDC );
    hOldBmp = (HBITMAP)SelectObject( hMemDC, hBitmap );
    GetObject( hBitmap, sizeof( BITMAP ), &bmp );
    bRet    = StretchBlt( hDC, x, y, 600, 400, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY );
    SelectObject( hMemDC, hOldBmp );
    DeleteDC( hMemDC );

    return bRet;
}

ImageObject::ImageObject( HBITMAP himg )
{
   m_himg = himg;
   //memcpy( &m_rect, &rect, sizeof(RECT) );
   memset( &m_rect, 0, sizeof(RECT) );
}

ImageObject::~ImageObject()
{
   if( m_himg != NULL )
   {
	   DeleteObject( m_himg );
	   m_himg = NULL;
   }
}


void ImageObject::DrawPic( HDC hdc )
{
   DrawRectBitmap( hdc, m_himg, m_rect );
}
	
void ImageObject::SetRect( RECT rect )
{
	memcpy( &m_rect, &rect, sizeof(RECT) );
}

/////////////////////////////////////////////////////////////////////////////
// CImageTable

CImageTable::CImageTable()
{
	m_width  = 400;
	m_heigth = 400;
	m_iflag  = 0;
	//m_himg   = NULL;
	//m_h_mid_bmp = NULL;
	m_imagedate = NULL;
	//m_h_mid1_bmp = NULL;
	//m_h_mid2_bmp = NULL;

	m_inum_imgobject = 0;

	int i;

	for( i = 0; i < NUM_IMAGE; i++ )
	{ 
		m_imageobjects[i] = NULL;
	}
}

CImageTable::~CImageTable()
{
    int i;

	for( i = 0; i < NUM_IMAGE; i++ )
	{ 
		if ( m_imageobjects[i] != NULL )
		{
			delete m_imageobjects[i];
			m_imageobjects[i] = NULL;
		}
	}

	if ( m_imagedate != NULL )
	{
		free( m_imagedate );
	}
}


BEGIN_MESSAGE_MAP(CImageTable, CWnd)
	//{{AFX_MSG_MAP(CImageTable)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CImageTable::Create( int width, int heigth, LPCTSTR lpszWindowName, CWnd* pParentWnd )
{
   m_width  = width;
   m_heigth = heigth;

   RECT rect;
   rect.top    = 0;
   rect.left   = 0;
   rect.right  = rect.left + m_width;
   rect.bottom = rect.top + m_heigth;
   
   return CreateEx( WS_EX_CLIENTEDGE, _T("ImageTable"), NULL, WS_VISIBLE | WS_BORDER | WS_CHILD,
 		rect, pParentWnd, IDC_TABLE );
}
/////////////////////////////////////////////////////////////////////////////
// CImageTable message handlers

void CImageTable::OnPaint() 
{	
 	CPaintDC dc(this); // device context for painting
// 	
// 	// TODO: Add your message handler code here
	HDC hdc = dc.GetSafeHdc();

	int i = 0;

    for( i = 0; i < NUM_IMAGE; i++ )
	{ 
		if ( m_imageobjects[i] != NULL )
		{
			m_imageobjects[i]->DrawPic( hdc );
		}
	}
	// Do not call CWnd::OnPaint() for painting messages
}


bool CImageTable::SetImage( HBITMAP hbmp )
{   
   BITMAP bmp;
   GetObject( hbmp, sizeof(BITMAP), &bmp );

   RECT rect;
   memset( &rect, 0, sizeof(RECT) );

   m_inum_imgobject = m_inum_imgobject%NUM_IMAGE;

   int heigth_one = m_heigth / 5;
   int width_one  = m_width / 2;
   
   rect.top  = m_inum_imgobject*heigth_one;
   rect.left = (m_inum_imgobject/5)*width_one;

   if ( bmp.bmHeight > m_heigth || bmp.bmWidth > m_width )
   {
	  width_one = (double)(bmp.bmWidth/bmp.bmHeight)*heigth_one;
	  rect.right  = rect.left + width_one;
	  rect.bottom = rect.top + heigth_one;
   }
   else
   {
     rect.right  = rect.left + bmp.bmWidth;
	 rect.bottom = rect.top + bmp.bmHeight;
   }

   
   ImageObject * pObject = new ImageObject( hbmp );
   pObject->SetRect( rect );

   m_imageobjects[m_inum_imgobject] = pObject;
   m_inum_imgobject++;

   return true;
}


void CImageTable::CopyImageBuffer( unsigned char * buffer, int width, int hight )
{
   if ( buffer == 0 )
   {
	   return;
   }

   m_image_width = width;
   m_image_heigth = hight;

   if ( m_imagedate != NULL )
   {
	   free( m_imagedate );
	   m_imagedate = NULL;
   }
   
   m_imagedate = (unsigned char*)malloc( m_image_width*m_image_heigth*3 );
   memset( m_imagedate, 0, m_image_width*m_image_heigth*3 );
   int i;
   int count = 0;
   for( i = m_image_heigth - 1; i >= 0; i-- )
   {
       memcpy( &m_imagedate[count*m_image_width*3], &buffer[i*m_image_width*3], m_image_width*3 );
       count++;
   }
}

void CImageTable::CopyImageBufferTo8( unsigned char * buffer, int width, int hight )
{
   if ( buffer == 0 )
   {
	   return;
   }

   m_image_width = width;
   m_image_heigth = hight;

   int nAdjust; // 用于字节对齐
   
   int real_width = m_image_width*3;
   
   if ( real_width%4 == 0 )
   {
       nAdjust = 0;
   }
   else
   {
       nAdjust = 4 - real_width%4;
   }
   
   real_width += nAdjust;

   if ( m_image_width%4 == 0 )
   {
       nAdjust = 0;
   }
   else
   {
       nAdjust = 4 - m_image_width%4;
   }
   
   m_image_width += nAdjust;

   if ( m_imagedate != NULL )
   {
	   free( m_imagedate );
	   m_imagedate = NULL;
   }

   m_imagedate = (unsigned char*)malloc( m_image_width*m_image_heigth );
   memset( m_imagedate, 0, m_image_width*m_image_heigth );

   int i;
   int j;

   unsigned   char  ired = 0;  
   unsigned   char  igreen = 0;  
   unsigned   char  iblack = 0;
   
   for ( i = 0; i < m_image_heigth; i++ )
   for( j = 0; j < m_image_width; j++ )
   {
	   ired   = buffer[i*real_width + j*3 + 2];
	   igreen = buffer[i*real_width + j*3 + 1];
	   iblack = buffer[i*real_width + j*3 ];
	   m_imagedate[i*m_image_width + j] = (unsigned char)(0.299*(ired)+0.587*(igreen)+0.114*(iblack));
   }



	HBITMAP hgray = CreateGrayBmp( m_imagedate, m_image_width, m_image_heigth );

	SetImage( hgray );
}

void CImageTable::algo_AverageFilter()
{
   int threahold = otsu( m_imagedate, m_image_width, m_image_heigth, 0, 0, m_image_width, m_image_heigth, 0 );

   int i;
   for ( i = 0; i < m_image_width*m_image_heigth; i++ )
   {
	   if ( m_imagedate[i] >= threahold - 30)//经验值 没办法先这样了
	   {
          m_imagedate[i] = 255;
	   }
	   else
	   {
          m_imagedate[i] = 0;
	   }
   }

   HBITMAP hgray = CreateGrayBmp( m_imagedate, m_image_width, m_image_heigth );

   SetImage( hgray );
}


HBITMAP CImageTable::CreateGrayBmp( unsigned char * buffer, int width, int hight )
{
    if ( buffer == NULL )
    {
		return NULL;
    }

    void * bmpinfobuff = malloc( sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD) );
   	BITMAPINFO * bmp_info = (BITMAPINFO *)bmpinfobuff;
	//memset( &bmp_info, 0, sizeof(BITMAPINFO) );
	bmp_info->bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
	bmp_info->bmiHeader.biWidth = width;
	bmp_info->bmiHeader.biHeight = hight;
	bmp_info->bmiHeader.biPlanes = 1;
	bmp_info->bmiHeader.biBitCount = 8;
	bmp_info->bmiHeader.biCompression = BI_RGB;
    bmp_info->bmiHeader.biClrUsed = 0;
    bmp_info->bmiHeader.biSizeImage = hight*width + 256*sizeof(RGBQUAD);
	bmp_info->bmiHeader.biXPelsPerMeter = 0;
    bmp_info->bmiHeader.biYPelsPerMeter = 0;
    bmp_info->bmiHeader.biClrImportant = 0;

    int i;
	
	for ( i = 0; i < 256; i++ )
	{
		bmp_info->bmiColors[i].rgbBlue  = (unsigned char)i;
		bmp_info->bmiColors[i].rgbRed   = (unsigned char)i;
		bmp_info->bmiColors[i].rgbGreen = (unsigned char)i;
		bmp_info->bmiColors[i].rgbReserved = 0;
	}

	HDC hdc = ::GetDC(NULL);
	unsigned char * lpDib = NULL;
   
	HBITMAP hbmp = CreateDIBSection( hdc, bmp_info, DIB_RGB_COLORS, (void**)&lpDib, NULL, 0 );

    memcpy( lpDib, buffer, width*hight );

    return hbmp;
}


void CImageTable::algo_AverageFilter1()
{
   int value = NoizyFilter( m_imagedate, m_image_width, m_image_heigth, 1 );
   //value = NoizyFilter( m_imagedate, m_image_width, m_image_heigth, 3 );

   if ( value == 1 )
   {
	   HBITMAP hbmp = CreateGrayBmp( m_imagedate, m_image_width, m_image_heigth );
       SetImage( hbmp );
   }
}

void CImageTable::algo_findcontent()
{
	DumpImageData();
	int x = 0;
	int y = 0;
	int value = scan_objects( m_imagedate, m_image_width, m_image_heigth);

	HBITMAP hbmp = CreateGrayBmp( m_imagedate, m_image_width, m_image_heigth );
	SetImage( hbmp );
	printf( "content x=%d,y=%d\n", x, y );

}

void CImageTable::ClearImage()
{
	int i;
	
	for( i = 0; i < NUM_IMAGE; i++ )
	{ 
		if ( m_imageobjects[i] != NULL )
		{
			delete m_imageobjects[i];
			m_imageobjects[i] = NULL;
		}
	}

	m_inum_imgobject = 0;
}


void CImageTable::DumpImageData()
{
	int i;
	int j;

	for ( i = 0; i < m_image_heigth; i++ )
	{
		for ( j = 0; j < m_image_width; j++ )
		{
			if ( m_imagedate[j + i*m_image_width] == 0xff )
			{
				printf( "0 " );
			}
			else
			{
				printf( "1 ");
			}
		}

		printf( "\n" );
	}
}