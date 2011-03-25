// ***************************************************************
//  point_stack   version:  1.0   ? date: 07/23/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "StdAfx.h"
#include "point_stack.h"

Point_Stack::Point_Stack()
{
   m_curtop = -1;
   memset( m_stack_point_buffer, 0, STACK_SIZE*sizeof(POINT) );
}

Point_Stack::~Point_Stack()
{

}

bool Point_Stack::PushPoint( POINT pt )
{
  m_curtop++;

  if ( m_curtop == m_ithreshold )
  {
	  return false;
  }

  m_stack_point_buffer[m_curtop].x = pt.x;
  m_stack_point_buffer[m_curtop].y = pt.y;

  return true;
}
   
void Point_Stack::PopPoint( POINT & pt )
{
  pt.x = m_stack_point_buffer[m_curtop].x;
  pt.y = m_stack_point_buffer[m_curtop].y;
  
  memset( &m_stack_point_buffer[m_curtop], 0, sizeof(POINT) );

  if ( m_curtop == 0 )
  {
      m_curtop = -1;
	  return;
  }

  m_curtop--;
}

void Point_Stack::ClearStack()
{
   m_curtop = -1;
   memset( m_stack_point_buffer, 0, STACK_SIZE*sizeof(POINT) );
}

bool Point_Stack::isStackNotClear()
{
   return (m_curtop>=0);
}

void Point_Stack::SetThreshold( int value )
{
    m_ithreshold = value;
}