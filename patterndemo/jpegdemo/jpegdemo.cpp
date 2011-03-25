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
ѹ��ͼ��jpeg��ʽ
���ѹ��ǰͼ��Ϊ�Ҷ�ͼ��24λͼ��ѹ����ͼ��ɫ��ģʽ����
���ѹ��ǰͼ��Ϊ256ɫ����λͼ��ѹ�����Ϊ�Ҷ�ͼ
************************************************************************/
void bmptojpg(const char *strSourceFileName, const char *strDestFileName)
{

}
/**************************************************************************
ѹ��ͼ��jpeg��ʽ�����ѹ��ǰ��ͼ����24λͼ����ǿ��ת��Ϊ24λͼ��ѹ��
**************************************************************************/
void bmptojpg24(const char *strSourceFileName, const char *strDestFileName)
{

}

/***********************************************
*��ѹ��jpeg��bmp��ʽ
*���ڻҶ�ͼ��24λͼ��ͼ���ѹ������
*����256ɫ����λͼ��ͼ���ѹ��Ϊ�Ҷ�ͼ
**************************************************/
void jpgtobmp(const char *strSourceFileName, const char *strDestFileName)
{
	BITMAPFILEHEADER bfh;		// bmp�ļ�ͷ
	BITMAPINFOHEADER bih;		// bmpͷ��Ϣ
	RGBQUAD rq[256];			// ��ɫ��
	int nAdjust; // �����ֽڶ���
	char indata[1000000]; // ���ڴ�Ž�ѹ��ǰ��ͼ�����ݣ�������ֱ�Ӵ�jpg�ļ���ȡ

	BYTE *data= NULL;//new BYTE[bih.biWidth*bih.biHeight];
	//BYTE *pDataConv = NULL;//new BYTE[bih.biWidth*bih.biHeight];
	int nComponent = 0;

	// ������ѹ�����󼰴�����Ϣ������
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
	//int nSize = fread(indata,1,1000000,f); // ��ȡjpgͼ�����ݣ�nSizeΪʵ�ʶ�ȡ��ͼ�����ݴ�С
	//fclose(f);
	// ����������ڽ�ѹ�����ӱ��п�ʼ��ѹ��
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
	// ����������ڽ�ѹ����������Ϊֹ��ѹ�����
    
	fclose(f);

	// ���´��뽲��ѹ�����ͼ������ļ������Ը���ʵ��Ӧ�������������紫��
	f=fopen(strDestFileName,"wb");
	if (f==NULL) 
	{
		delete [] data;
		//delete [] pDataConv;
		return;
	}
	// д�ļ�ͷ
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
	// дͼ����Ϣ
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
	// д��ɫ��
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
		// ����rgb˳��
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
	// ������Ҫ��������ģʽ�����У���Ҫ����������������������ʾ������ʽ
	
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
		printf("������������libjpeg�⣬��libjpeg.lib��vs 2005�����±����������ֱ��ʹ��.\n");
		printf("ת��bmpλͼΪjpg��ʽ�����ѹ��jpg��ʽͼ��Ϊbmp��ʽ\n");
		printf("TestLibjpeg.exe j|j24|b s_name d_name\n");
		printf("�磺TestLibjpeg.exe j 05.bmp 05.jpg\n");
		printf("TestLibjpeg.exe j24 05.bmp 05.jpg\n");
		printf("TestLibjpeg.exe b 05.jpg 05.bmp\n");
		printf("\n��ʽ���£�\n");
		printf("\tj:ѹ��ͼ��jpeg��ʽ,���ѹ��ǰͼ��Ϊ�Ҷ�ͼ��24λͼ��ѹ����ͼ��ɫ��ģʽ����,���ѹ��ǰͼ��Ϊ256ɫ����λͼ��ѹ�����Ϊ�Ҷ�ͼ;\n");
		printf("\tj24:ѹ��ͼ��jpeg��ʽ�����ѹ��ǰ��ͼ����24λͼ����ǿ��ת��Ϊ24λͼ��ѹ��;\n");
		printf("\tb:��ѹ��jpeg��bmp��ʽ,���ڻҶ�ͼ��24λͼ��ͼ���ѹ������,����256ɫ����λͼ��ͼ���ѹ��Ϊ�Ҷ�ͼ\n");
		break;
	}
	
	return 0;
}
