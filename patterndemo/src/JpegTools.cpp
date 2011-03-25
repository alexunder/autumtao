/*++ BUILD Version: 0001
 *
 *  JpegTools v1.0
 *
 *  Copyright (c) 1999--2007 Lingtu Software Corporation
 *
 *  JpegTools.cpp
 *  
 *
 *  History:
 *  Created 15:11:2007  by alexu
--*/

#include "StdAfx.h"
#include "JpegTools.h"

//#include "voimage/voimage.h"

extern "C"
{
  #include "jpeglib.h"
}

//HBITMAP LoadJpegFromMemory( unsigned char * buffer, unsigned int sizebuf )
//{
	/*
   if ( filename == NULL )
   {
	   return NULL;
   }

   FILE * pfile = fopen( filename, "rb" );

   if ( pfile == NULL )
   {
	   return NULL;
   }

   fseek( pfile, 0, SEEK_END );

   unsigned int isize_file = ftell( pfile );

   unsigned char * buffer = malloc( isize_file );
   memset( buffer, 0, isize_file );

   fseek( pfile, 0, SEEK_SET );

   unsigned int ret_size = fread( buffer, 1, isize_file, pfile );

   if ( ret_size != isize_file )
   {
      free( buffer );
	  buffer = NULL;
	  fclose( pfile );
	  return NULL;
   }
   */
//	return NULL;
//}


int read_jpeg( const char *filename, int *width, int *height, int *sizebuffer, unsigned char **rgb )
{
	int nAdjust; // 用于字节对齐

	if ( filename == NULL )
	{
		return 0;
	}
	
	FILE * pfile = fopen( filename, "rb" );
	
	if ( pfile == NULL )
	{
		return 0;
	}
	
	// 声明解压缩对象及错误信息管理器
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	
	jpeg_stdio_src(&cinfo, pfile );
	jpeg_read_header(&cinfo, TRUE);


	jpeg_start_decompress(&cinfo);
    int width_bmp = cinfo.output_width * cinfo.output_components;
    *width          = cinfo.output_width;
    *height         = cinfo.output_height;

	if ( width_bmp%4 == 0 )
	{
       nAdjust = 0;
	}
	else
	{
       nAdjust = 4 - width_bmp%4;
	}

	width_bmp += nAdjust;
	
	*sizebuffer = width_bmp*cinfo.image_height;
	//unsigned char * buffer = (unsigned char*)malloc((cinfo.image_width*cinfo.num_components+nAdjust)*cinfo.image_height);
	//unsigned char * buffer = (unsigned char*)malloc(cinfo.image_width*cinfo.image_height*cinfo.num_components);
    unsigned char * buffer = (unsigned char*)malloc( width_bmp*cinfo.image_height);
    //memset( buffer, 0xff, width_bmp*cinfo.image_height );
	rgb[0] = buffer;

	if ( rgb[0] == NULL )
	{
		fclose(pfile);
        return 0;
	}

	//JSAMPROW row_pointer[1];
	unsigned char * line = NULL;
	while (cinfo.output_scanline < cinfo.output_height)
	{
		line = &buffer[(cinfo.output_height - cinfo.output_scanline-1)*width_bmp];
		jpeg_read_scanlines( &cinfo, &line, 1 );

		for ( int i = 0; i < *width; i++, line += 3 )
		{
			unsigned char temp = *line;
			*line = *(line + 2);
			*(line + 2) = temp;
		}
	}

// 	if (cinfo.num_components==3)
// 	{
// 		// 调整rgb顺序
// 		for (int i = 0;i<cinfo.output_height*cinfo.output_width;i++)
// 		{
// 			unsigned char red = buffer[i*3];
// 			buffer[i*3] = buffer[i*3+2];
// 			buffer[i*3+2] = red;
// 		}
// 	}

	jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(pfile);

	return 1;
}


int read_jpeg_v2( const char *filename, int *width, int *height, int *sizebuffer, unsigned char **rgb )
{
   	struct jpeg_decompress_struct cinfo;
	FILE * infile;		/* source file */
	struct jpeg_error_mgr jerr;
	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */
	
	if ((infile = fopen(filename, "rb")) == NULL)
	{
		return 0;
	}
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	
	//fseek(infile,0,SEEK_END);
	//long filesize = ftell(infile);
	//fseek(infile,0,SEEK_SET);
	//JOCTET *filebuffer = new BYTE[filesize];
	//fread(filebuffer,filesize,1,infile);
	
	jpeg_stdio_src(&cinfo, infile);
	//jpeg_arr_src(&cinfo,filebuffer,filesize);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	
	*width = cinfo.output_width;
	*height = cinfo.output_height;
	
	unsigned char * content =(unsigned char*)malloc( cinfo.output_height*cinfo.output_width*cinfo.output_components );
	
	/* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;
	/* Make a one-row-high sample array that will go away when done with image */
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	
	rgb[0] = content;

	unsigned char * line = NULL;

	while (cinfo.output_scanline < cinfo.output_height) {
	/* jpeg_read_scanlines expects an array of pointers to scanlines.
	* Here the array is only one element long, but you could ask for
	* more than one scanline at a time if that's more convenient.
		*/
		line = &content[(cinfo.output_height - cinfo.output_scanline-1)*row_stride];

		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		/* Assume put_scanline_someplace wants a pointer and sample count. */
		//put_scanline_someplace(buffer[0], row_stride);

		memcpy( line, buffer[0], row_stride);

		for ( unsigned int i = 0; i < cinfo.output_width; i++, line += 3 )
		{
			unsigned char temp = *line;
			*line = *(line + 2);
			*(line + 2) = temp;
		}
		
	}
	
	*sizebuffer = row_stride*cinfo.output_height;
	/* Step 7: Finish decompression */
	
	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
	return 1;
}

void jpgtobmp(const char *strSourceFileName, const char *strDestFileName)
{
	BITMAPFILEHEADER bfh;		// bmp文件头
	BITMAPINFOHEADER bih;		// bmp头信息
	RGBQUAD rq[256];			// 调色板

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
	// 
	jpeg_stdio_src(&cinfo, f);
	jpeg_read_header(&cinfo, TRUE);
	data = new BYTE[cinfo.image_width*cinfo.image_height*cinfo.num_components];
	jpeg_start_decompress(&cinfo);

	JSAMPROW row_pointer[1];
	while (cinfo.output_scanline < cinfo.output_height)
	{
		row_pointer[0] = &data[(cinfo.output_height - cinfo.output_scanline-1)*cinfo.image_width*cinfo.num_components];
		jpeg_read_scanlines(&cinfo,row_pointer ,
					1);
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(f);

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

	bfh.bfSize += cinfo.image_width*cinfo.image_height*cinfo.num_components;
	bfh.bfType = 0x4d42;
	fwrite(&bfh,sizeof(bfh),1,f);
	// 写图像信息
	bih.biBitCount = cinfo.num_components*8;
	bih.biSize = sizeof(bih);
	bih.biWidth = cinfo.image_width;
	bih.biHeight = cinfo.image_height;
	bih.biPlanes = 1;
	bih.biCompression = 0;
	bih.biSizeImage = cinfo.image_width*cinfo.image_height*cinfo.num_components;
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
		for (int i = 0;i<bih.biWidth*bih.biHeight;i++)
		{
			BYTE red = data[i*3];
			data[i*3] = data[i*3+2];
			data[i*3+2] = red;
		}
	}
	fwrite(data,cinfo.image_width*cinfo.image_height*cinfo.num_components,1,f);
	fclose(f);
	delete [] data;

}