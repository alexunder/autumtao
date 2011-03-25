// ***************************************************************
//  point_stack   version:  1.0   ? date: 07/23/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef __H_POINT_STACK
#define __H_POINT_STACK

#define STACK_SIZE 30

#include <windows.h>


class Point_Stack
{
public:
   Point_Stack();
   ~Point_Stack();

   bool PushPoint( POINT pt );
   void PopPoint( POINT & pt );
   bool isStackNotClear();
   void ClearStack();
   void SetThreshold( int value );
private:
   POINT m_stack_point_buffer[STACK_SIZE];
   int   m_curtop;
   int   m_ithreshold;
};

#endif