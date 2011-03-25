/* BUILD Version: 0001
 *
 *  imagewnd v1.0
 *
 *  Copyright (c) 1999--2007 Lingtu Software Corporation
 *
 *  imagewnd.cpp
 *  
 *
 *  History:
 *  Created 14:06:2008  by alexu
*/

#include "StdAfx.h"
#include "imagewnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

CImageWnd::CImageWnd()
{
   m_himg = NULL;
   m_iflag = 0;
}

CImageWnd::~CImageWnd()
{

}

BEGIN_MESSAGE_MAP(CImageWnd, CWnd)
//{{AFX_MSG_MAP(CTable)
ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CImageWnd::OnPaint()
{
   HDC hdc = ::GetDC( m_hWnd );
   
   if ( m_himg == NULL )
   {
	   return;
   }

   if ( m_iflag == 1 )
   {
      DrawBitmapStrench( hdc, m_himg, 0, 0 );
   }
   else
   {
	  DrawBitmap( hdc, m_himg, 0, 0 );
   }
}

void CImageWnd::setimage( HBITMAP hbmp )
{
   m_himg = hbmp;

   BITMAP bmp;
   GetObject( m_himg, sizeof(BITMAP), &bmp );

   if ( bmp.bmHeight > 400 || bmp.bmWidth > 600 )
   {
      m_iflag = 1;
   }
   else
   {
      m_iflag = 0;
   }
}