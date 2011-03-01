/*
  Copyright (c) 2010 Tor-Helge Skei, Lubomir I. Ivanov et al

  This file is part of the Holos Library.
  http://holos.googlecode.com

  the Holos Library is free software: you can redistribute it and/or modify
  it under the terms of the Holos Library License, either version 1.0
  of the License, or (at your option) any later version.

  the Holos Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See LICENSE_HOLOS for more details.

  You should have received a copy of the Holos Library License
  If not, see <http://holos.googlecode.com/>.
*/
//----------------------------------------------------------------------
#ifndef h_Point_included
#define h_Point_included
//----------------------------------------------------------------------

struct h_Point
{
  int x,y;

  h_Point()
    {
    }

  h_Point(int i)
    {
      x = i;
      y = i;
    }

  h_Point(int aX, int aY)
    {
      x = aX;
      y = aY;
    }

  //----------

  void clear(void)
    {
    }

  void set(int i)
    {
      x = i;
      y = i;
    }

  void set(int aX, int aY)
    {
      x = aX;
      y = aY;
    }

  inline void add(int aX, int aY)
    {
      x += aX;
      y += aY;
    }

};

//----------------------------------------------------------------------

#define H_NULL_POINT h_Point(0,0)

//#include "lib/h_Array.h"
#include "src/h_Array.h"
typedef h_Array<h_Point> h_Points;

//----------------------------------------------------------------------
#endif


