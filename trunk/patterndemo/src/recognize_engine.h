/********************************************************************
	created:	2008/09/07
	created:	7:9:2008   14:57
	filename: 	D:\codingÏ°×÷\patterndemo\src\recognize_engine.h
	file path:	D:\codingÏ°×÷\patterndemo\src
	file base:	recognize_engine
	file ext:	h
	author:		Administrator
	
	purpose:	
*********************************************************************/

#ifndef __H_RECOGNIZE_ENGINE
#define __H_RECOGNIZE_ENGINE

int otsu (unsigned char *image, int rows, int cols, int x0, int y0, int dx, int dy, int vvv);

int averagefilter( unsigned char *image, int width, int heigth );

unsigned char sortretmid( unsigned char * array, int size );

int middlefilter( unsigned char *image, int width, int heigth );

int NoizyFilter( unsigned char *image, int width, int heigth, int Threshold );

int find_content( unsigned char *image, int width, int heigth, int * xpos, int * ypos );

int scan_objects( unsigned char *image, int width, int heigth );
#endif