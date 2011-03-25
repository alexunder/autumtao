/********************************************************************
	created:	2008/07/09
	created:	9:7:2008   23:46
	filename: 	D:\privateprj\patterndemo\src\Bit_Process.cpp
	file path:	D:\privateprj\patterndemo\src
	file base:	Bit_Process
	file ext:	cpp
	author:		alexu
	
	purpose:	
*********************************************************************/

#include "StdAfx.h"
#include "Bit_Process.h"

bit_process_tool::bit_process_tool()
{
	m_iwidth  = 0;
	m_iheigth = 0;
	m_pdata = NULL;
}

bit_process_tool::~bit_process_tool()
{
	if ( m_pdata != NULL )
	{
		delete [] m_pdata;
		m_pdata = NULL;
	}
}

void bit_process_tool::init_bit_matrix( unsigned int width, unsigned int length, bool flag )
{
   unsigned int size_buffer = width*length;
   int nAdjust; // 用于字节对齐

   m_iwidth  = width;
   m_iheigth = length;

   if ( size_buffer%8 == 0 )
   {
       nAdjust = 0;
   }
   else
   {
       nAdjust = 8 - size_buffer%8;
   }

   size_buffer += nAdjust;

   size_buffer = size_buffer/8;
   //m_pdata= (unsigned char *)malloc( size_buffer );

   m_pdata = new unsigned char[size_buffer];

   if ( flag == true )
   {
      memset( m_pdata, 1, size_buffer );
   }
   else
   {
      memset( m_pdata, 0, size_buffer );
   }
}

bool bit_process_tool::GetBitValue( unsigned int x, unsigned int y )
{
   unsigned int pos = x + y*m_iwidth;
   //int index = pos/8;
   int index = pos>>3;
   int mod   = pos&7;

   unsigned char value = *(m_pdata + index);
   if ( value&(1<<mod) )
   {
	   return true;
   }
   else
   {
	   return false;
   }
}
	
void bit_process_tool::SetBitValue( unsigned int x, unsigned int y, bool value )
{
	unsigned int pos = x + y*m_iwidth;
	//int index = pos/8;
	int index = pos>>3;
    int mod   = pos&7;

	unsigned char * pvalue = m_pdata + index;

	if ( value == true )
	{
       *pvalue = (*pvalue)|(1<<mod);
	}
	else
	{
       *pvalue = (*pvalue)&~(1<<mod);
	}
}