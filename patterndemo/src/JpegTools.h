/*++ BUILD Version: 0001
 *
 *  JpegTools v1.0
 *
 *  Copyright (c) 1999--2007 Lingtu Software Corporation
 *
 *  JpegTools.h
 *  
 *
 *  History:
 *  Created 15:11:2007  by alexu
--*/

#ifndef __H_JPEGTOOLS
#define __H_JPEGTOOLS

int read_jpeg( const char *filename, int *width, int *height, int *sizebuffer, unsigned char **rgb );

int read_jpeg_v2( const char *filename, int *width, int *height, int *sizebuffer, unsigned char **rgb );

void jpgtobmp(const char *strSourceFileName, const char *strDestFileName);

#endif