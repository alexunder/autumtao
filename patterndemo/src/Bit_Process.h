/********************************************************************
	created:	2008/07/09
	created:	9:7:2008   23:23
	filename: 	D:\privateprj\patterndemo\src\Bit_Process.h
	file path:	D:\privateprj\patterndemo\src
	file base:	Bit_Process
	file ext:	h
	author:		alexu
	
	purpose:	
*********************************************************************/

#ifndef __H_BIT_PROCESS
#define __H_BIT_PROCESS


class bit_process_tool
{
public:
    bit_process_tool();
	~bit_process_tool();

//private:
   void init_bit_matrix( unsigned int width, unsigned int length, bool flag = false );
public:
    bool GetBitValue( unsigned int x, unsigned int y );
	void SetBitValue( unsigned int x, unsigned int y, bool value );
private:
    unsigned char * m_pdata;
	unsigned int    m_iwidth;
	unsigned int    m_iheigth;
};

#endif