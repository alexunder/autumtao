// testbit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Bit_Process.h"

int main(int argc, char* argv[])
{
	printf("testbit\n");

	bit_process_tool imgbit;
	imgbit.init_bit_matrix( 152, 50 );

	int i = 0;
	int j = 0;

	imgbit.SetBitValue( 7, 22, true );
	imgbit.SetBitValue( 65, 31, true );
	imgbit.SetBitValue( 100, 45, true );
	imgbit.SetBitValue( 111, 16, true );
	imgbit.SetBitValue( 144, 27, true );

	for ( i = 0; i < 50; i++ )
	{
		for ( j = 0; j < 152; j++)
		{
			printf( "%d ", imgbit.GetBitValue( j,i ) );		
		}
		
		printf( "\n" );
	}
	return 0;
}

