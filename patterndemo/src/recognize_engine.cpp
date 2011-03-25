
#include "recognize_engine.h"
#include "Bit_Process.h"
#include "point_stack.h"

#include <vector>
using namespace std;
/*======================================================================*/
/*   OTSU global thresholding routine                                                 */
/*   takes a 2D unsigned char array pointer, number of rows, and        */
/*   number of cols in the array. returns the value of the threshold       */
/*======================================================================*/
int otsu (unsigned char *image, int rows, int cols, int x0, int y0, int dx, int dy, int vvv)
{
	
	unsigned char *np;      // 图像指针
	int thresholdValue=1; // 阈值
	int ihist[256];             // 图像直方图，256个点
	
	int i, j, k;          // various counters
	int n, n1, n2, gmin, gmax;
	double m1, m2, sum, csum, fmax, sb;
	
	// 对直方图置零...
	memset(ihist, 0, sizeof(ihist));
	
	gmin=255; gmax=0;
	// 生成直方图
	for (i = y0 + 1; i < y0 + dy - 1; i++) 
	{
		np = &image[i*cols+x0+1];
		for (j = x0 + 1; j < x0 + dx - 1; j++) 
		{
			ihist[*np]++;
			if(*np > gmax) gmax=*np;
			if(*np < gmin) gmin=*np;
			np++; /* next pixel */
		}
	}
	
	// set up everything
	sum = csum = 0.0;
	n = 0;
	
	for (k = 0; k <= 255; k++) 
	{
		sum += (double) k * (double) ihist[k];    /* x*f(x) 质量矩*/
		n   += ihist[k];                                         /*  f(x)    质量    */
	}
	
	if (!n) 
	{
		// if n has no value, there is problems...
		fprintf (stderr, "NOT NORMAL thresholdValue = 160\n");
		return (160);
	}
	
	// do the otsu global thresholding method
	fmax = -1.0;
	n1 = 0;
	for (k = 0; k < 255; k++) 
	{
		n1 += ihist[k];
		if (!n1) 
		{ 
			continue; 
		}
		
		n2 = n - n1;
		
		if (n2 == 0) 
		{
			break; 
		}
		
		csum += (double) k *ihist[k];
		m1 = csum / n1;
		m2 = (sum - csum) / n2;
		sb = (double) n1 *(double) n2 *(m1 - m2) * (m1 - m2);
		/* bbg: note: can be optimized. */
		
		if (sb > fmax) 
		{
			fmax = sb;
			thresholdValue = k;
		}
	}
	
	// at this point we have our thresholding value
	
	// debug code to display thresholding values
	if ( vvv & 1 )
		fprintf(stderr,"# OTSU: thresholdValue = %d gmin=%d gmax=%d\n",
		thresholdValue, gmin, gmax);
	
	return(thresholdValue);
}

int averagefilter( unsigned char *image, int width, int heigth )
{
   if ( image == NULL )
   {
	   return 0;
   }

   int i;
   int j;

   for( i = 1; i <= heigth - 2; i++  )
   for( j = 1; j <= width - 2; j++ )
   {
	   unsigned char value1 = 2*image[i*width + j];
       unsigned char value2 = image[(i-1)*width + j - 1];
	   unsigned char value3 = image[(i-1)*width + j];
	   unsigned char value4 = image[(i-1)*width + j + 1];
	   unsigned char value5 = image[i*width + j - 1];
       unsigned char value6 = image[i*width + j + 1];
	   unsigned char value7 = image[(i+1)*width + j - 1];
	   unsigned char value8 = image[(i+1)*width + j];
	   unsigned char value9 = image[(i+1)*width + j + 1];
  
	   image[i*width + j] = (/*value1 +*/ value2 + value3 + value4 + value5 + value6 + value7 + value8 + value9)/8;
   }

   return 1;
}

unsigned char sortretmid( unsigned char * array, int size )
{
	int i,j,key;

	for( j=1; j<size; j++ )
	{
		key = array[j];
		i=j-1;

		while( i>=0&&array[i]>key )
		{
			array[i+1] = array[i];
			i--;
		}

		array[i+1] = key;
	}

	int tmp = size/2;

	return array[size/2];
}

int middlefilter( unsigned char *image, int width, int heigth )
{
	if ( image == NULL )
	{
		return 0;
	}

	int i;
    int j;
    unsigned char value[9];
	memset( value, 0, 9 );

	for( i = 1; i <= heigth - 2; i++  )
	for( j = 1; j <= width - 2; j++ )
	{
			value[0] = image[i*width + j];
			value[1] = image[(i-1)*width + j - 1];
			value[2] = image[(i-1)*width + j];
			value[3] = image[(i-1)*width + j + 1];
			value[4] = image[i*width + j - 1];
			value[5] = image[i*width + j + 1];
			value[6] = image[(i+1)*width + j - 1];
			value[7] = image[(i+1)*width + j];
			value[8] = image[(i+1)*width + j + 1];
	
			image[i*width + j] = sortretmid( value, 9 );
	}

	return 1;
}


static int x_seqence[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
static int y_seqence[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

int NoizyFilter( unsigned char *image, int width, int heigth, int Threshold )
{
	if ( image == NULL )
	{
		return 0;
	}

	int i = 0;
	int j = 0;
    int k = 0;

	bit_process_tool bitflag;
	bitflag.init_bit_matrix( width, heigth );

	Point_Stack pt_stk;
	pt_stk.SetThreshold( Threshold );
	vector<POINT> pt_vector;
	
	for( i = 1; i < heigth - 1; i++ )
		for( j = 1; j < width - 1; j++ )
		{ 
			if ( image[i*width + j] == 0 && !bitflag.GetBitValue( j, i ) )
			{
				POINT pt;
				pt.x = j;
				pt.y = i;
				bitflag.SetBitValue( j, i, true );
				pt_stk.PushPoint( pt );
				
				pt_vector.clear();
				pt_vector.push_back( pt );

				while ( pt_stk.isStackNotClear() )
				{
					POINT current;
					pt_stk.PopPoint( current );
					
					for ( k =0; k < 8; k++ )
					{
						int x_position = current.x + x_seqence[k];
						int y_position = current.y + y_seqence[k];
						
						if (  x_position >= 1 && x_position < width - 1 &&
							y_position >= 1 && y_position < heigth - 1  &&
							image[x_position + y_position*width] == 0   &&
							!bitflag.GetBitValue( x_position, y_position ) )
						{
							bitflag.SetBitValue( x_position, y_position, true );
							
							POINT pt;
							pt.x = x_position;
							pt.y = y_position;
							bool ret = pt_stk.PushPoint( pt );
						    
							pt_vector.push_back( pt );

							if ( !ret )
							{
								pt_vector.clear();
								pt_stk.ClearStack();
								break;
							}
						}
					}
				}

				vector<POINT>::iterator ipos;
				for ( ipos = pt_vector.begin(); ipos != pt_vector.end(); ipos++ )
				{
				   POINT pt = *ipos;
				   image[pt.y*width+pt.x] = 0xFF;
				}
				
			}
        }
		
		return 1;
}


int find_content( unsigned char *image, int width, int heigth, 
				  int * xpos, int * ypos )
{
    if ( image == NULL )
	{
		return 0;
	}

    bool * x_project = (bool*)malloc( width*sizeof(bool) );
	memset( x_project, 1, width*sizeof(bool) );

	bool * y_project = (bool*)malloc( heigth );
	memset( y_project, 1, heigth*sizeof(bool)  );

	int i;
	int j;

    //x
	for ( i = 1; i < width; i++ )
	{
		//x_project[i] = 0xff;
        for ( j = 1; j < heigth-1; j++ )
        {
			if ( image[j*width + i] == 0 )
			{
                x_project[i] = 0;
				break;
			}
        }
	}

	//y
	for ( i = 1; i < heigth - 1; i++ )
	{
		//y_project[i] = 0xff;
		for ( j = 1; j < width-5; j++ )
		{
			if ( image[i*width + j] == 0 )
			{
                y_project[i] = 0;
				break;
			}
		}
	}

	printf( "x_project:\n" );
	for ( i = 0; i < width; i++ )
	{
		printf( "%d ", x_project[i] );
	}
    printf( "\n" );

	printf( "y_project:\n" );
	for ( i = 0; i < heigth; i++ )
	{
		printf( "%d ", y_project[i] );
	}
    printf( "\n" );

	//check the content in x 
	int count_x = 0;
	int x_content_pos = 0;
    for ( i = 0; i < width; i++ )
	{
       if ( x_project[i] == 0 )
       {
          count_x++;
		  
		  if ( count_x == 0 )
		  {
             x_content_pos = i;
		  }
       }
	   else
	   {
		   if ( count_x > 10 )
		   {
              break;
		   }

           count_x = 0;
	   }
	}


	//check the content in y 
	int count_y = 0;
	int y_content_pos = 0;
    for ( i = 0; i < heigth; i++ )
	{
       if ( y_project[i] == 0 )
       {
          count_y++;
		  
		  if ( count_y == 0 )
		  {
             y_content_pos = i;
		  }
       }
	   else
	   {
		   if ( count_y > 10 )
		   {
              break;
		   }

           count_y = 0;
	   }
	}

	*xpos = x_content_pos;
    *ypos = y_content_pos;

	free( x_project );
	free( y_project );

	return 0;
}


int scan_objects( unsigned char *image, int width, int heigth )
{
	//horizontal projection
	unsigned char * x_project = (unsigned char*)malloc( width*sizeof(unsigned char) );
	memset( x_project, 0, width*sizeof(unsigned char) );
	
	int i;
	int j;

    //x
	for ( i = 1; i < width; i++ )
	{
		//x_project[i] = 0xff;
        for ( j = 1; j < heigth-1; j++ )
        {
			if ( image[j*width + i] == 0 )
			{
                x_project[i]++;
			}
        }
	}
	
	char bufTemp[20];
	memset(bufTemp, 0, 20);
	int cnt = 0;

	for ( i = 1; i < width; i++ )
	{
		if(x_project[i] > 2 && cnt == 0)
		{
			bufTemp[cnt++] = i - 1;
		}

		if(x_project[i] < 2)
		{
           while(x_project[i++] <= 2);

		   bufTemp[cnt++] = i - 1;
		}
	}

	for(i = 0; i < cnt; i++)
	{
		for(j = 0; j < heigth; j++)
		{
			image[j*width + bufTemp[i]] = 0;
		}
	}

	return 0;
}