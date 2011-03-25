// jpegdemo.cpp : Defines the entry point for the console application.
//
// TestLibjpeg.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "memory.h"
#include "string.h"
#include "jpeglib.h"

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef long				LONG;

/***********************************************************************
压缩图像到jpeg格式
如果压缩前图像为灰度图或24位图，压缩后图像色彩模式不变
如果压缩前图像为256色索引位图，压缩后变为灰度图
************************************************************************/
void bmptojpg(const char *strSourceFileName, const char *strDestFileName)
{

}
/**************************************************************************
压缩图像到jpeg格式，如果压缩前的图像不是24位图，则强行转换为24位图后压缩
**************************************************************************/
void bmptojpg24(const char *strSourceFileName, const char *strDestFileName)
{

}

/***********************************************
*解压缩jpeg到bmp格式
*对于灰度图和24位图，图像解压后正常
*对于256色索引位图，图像解压后为灰度图
**************************************************/
void jpgtobmp(const char *strSourceFileName, const char *strDestFileName)
{
	BITMAPFILEHEADER bfh;		// bmp文件头
	BITMAPINFOHEADER bih;		// bmp头信息
	RGBQUAD rq[256];			// 调色板
	int nAdjust; // 用于字节对齐
	char indata[1000000]; // 用于存放解压缩前的图像数据，该数据直接从jpg文件读取

	BYTE *data= NULL;//new BYTE[bih.biWidth*bih.biHeight];
	//BYTE *pDataConv = NULL;//new BYTE[bih.biWidth*bih.biHeight];
	int nComponent = 0;

	// 声明解压缩对象及错误信息管理器
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	FILE *f = fopen(strSourceFileName,"rb");
	if (f==NULL)
	{
		printf("Open file error!\n");
		return;
	}
	//int nSize = fread(indata,1,1000000,f); // 读取jpg图像数据，nSize为实际读取的图像数据大小
	//fclose(f);
	// 下面代码用于解压缩，从本行开始解压缩
	jpeg_stdio_src(&cinfo, f );
	jpeg_read_header(&cinfo, TRUE);
	nAdjust = cinfo.image_width*cinfo.num_components%4;
	if (nAdjust) nAdjust = 4-nAdjust;
	data = new BYTE[(cinfo.image_width*cinfo.num_components+nAdjust)*cinfo.image_height];	

	jpeg_start_decompress(&cinfo);

	JSAMPROW row_pointer[1];
	while (cinfo.output_scanline < cinfo.output_height)
	{
		row_pointer[0] = &data[(cinfo.output_height - cinfo.output_scanline-1)*(cinfo.image_width*cinfo.num_components+nAdjust)];
		jpeg_read_scanlines(&cinfo,row_pointer ,
					1);
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	// 上面代码用于解压缩，到本行为止解压缩完成
    
	fclose(f);

	// 以下代码讲解压缩后的图像存入文件，可以根据实际应用做其他处理，如传输
	f=fopen(strDestFileName,"wb");
	if (f==NULL) 
	{
		delete [] data;
		//delete [] pDataConv;
		return;
	}
	// 写文件头
	memset(&bfh,0,sizeof(bfh));
	bfh.bfSize = sizeof(bfh)+sizeof(bih);
	bfh.bfOffBits = sizeof(bfh)+sizeof(bih);
	if (cinfo.num_components==1)
	{
		bfh.bfOffBits += 1024;
		bfh.bfSize += 1024;
	}

	bfh.bfSize += (cinfo.image_width*cinfo.num_components+nAdjust)*cinfo.image_height;
	bfh.bfType = 0x4d42;
	fwrite(&bfh,sizeof(bfh),1,f);
	// 写图像信息
	bih.biBitCount = cinfo.num_components*8;
	bih.biSize = sizeof(bih);
	bih.biWidth = cinfo.image_width;
	bih.biHeight = cinfo.image_height;
	bih.biPlanes = 1;
	bih.biCompression = 0;
	bih.biSizeImage = (cinfo.image_width*cinfo.num_components+nAdjust)*cinfo.image_height;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;
	fwrite(&bih,sizeof(bih),1,f);
	// 写调色板
	if (cinfo.num_components ==1)
	{
		for (int i=0;i<256;i++)
		{
			rq[i].rgbBlue =i;
			rq[i].rgbGreen = i;
			rq[i].rgbRed = i;
			rq[i].rgbReserved = 0;
		}
		fwrite(rq,1024,1,f);
	}

	if (cinfo.num_components==3)
	{
		// 调整rgb顺序
		for (int j=0;j<bih.biHeight;j++)
		for (int i = 0;i<bih.biWidth;i++)
		{
			BYTE red = data[j*(cinfo.image_width*cinfo.num_components+nAdjust)+i*3];
			data[j*(cinfo.image_width*cinfo.num_components+nAdjust)+i*3] = data[j*(cinfo.image_width*cinfo.num_components+nAdjust)+i*3+2];
			data[j*(cinfo.image_width*cinfo.num_components+nAdjust)+i*3+2] = red;
		}
	}
	fwrite(data,(cinfo.image_width*cinfo.num_components+nAdjust)*cinfo.image_height,1,f);
	fclose(f);
	delete [] data;

}
int main(int argc, char* argv[])
{
	// 本程序要在命令行模式下运行，需要带参数，不带参数运行显示参数格式
	
	switch (argc)
	{
	case 4:
		if (strcmp(argv[1],"j")==0)
		{
			bmptojpg(argv[2],argv[3]);
			break;
		}
		else if (strcmp(argv[1],"j24")==0)
		{
			bmptojpg24(argv[2],argv[3]);
			break;
		}
		else if (strcmp(argv[1],"b")==0)
		{
			jpgtobmp(argv[2],argv[3]);
			break;
		}
	
	default :
		printf("本程序利用了libjpeg库，该libjpeg.lib在vs 2005下重新编译过，可以直接使用.\n");
		printf("转换bmp位图为jpg格式，或解压缩jpg格式图像为bmp格式\n");
		printf("TestLibjpeg.exe j|j24|b s_name d_name\n");
		printf("如：TestLibjpeg.exe j 05.bmp 05.jpg\n");
		printf("TestLibjpeg.exe j24 05.bmp 05.jpg\n");
		printf("TestLibjpeg.exe b 05.jpg 05.bmp\n");
		printf("\n格式如下：\n");
		printf("\tj:压缩图像到jpeg格式,如果压缩前图像为灰度图或24位图，压缩后图像色彩模式不变,如果压缩前图像为256色索引位图，压缩后变为灰度图;\n");
		printf("\tj24:压缩图像到jpeg格式，如果压缩前的图像不是24位图，则强行转换为24位图后压缩;\n");
		printf("\tb:解压缩jpeg到bmp格式,对于灰度图和24位图，图像解压后正常,对于256色索引位图，图像解压后为灰度图\n");
		break;
	}
	
	return 0;
}
