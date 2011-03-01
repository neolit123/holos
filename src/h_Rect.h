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
#ifndef h_Rect_included
#define h_Rect_included
//----------------------------------------------------------------------

struct h_Rect
{
  //union
  //{
  //  //struct { int x,y,w,h; };
  //  //struct { int data[4]; };
  //  int x,y,w,h;
  //  int data[4];
  //};

  int x,y,w,h;

  h_Rect() { }
  h_Rect(int i) { x=i; y=i; w=i; h=i; }
  h_Rect(int a_W, int a_H) { x=0; y=0; w=a_W; h=a_H; }
  h_Rect(int a_X, int a_Y, int a_W, int a_H) { x=a_X; y=a_Y; w=a_W; h=a_H; }

  //----------------------------------------

  inline void set(int i) { x=i; y=i; w=i; h=i; }
  inline void set(int a_X, int a_Y, int a_W, int a_H) { x=a_X; y=a_Y; w=a_W; h=a_H; }

  inline void setPos(int a_X, int a_Y) { x=a_X; y=a_Y; }
  inline void setSize(int a_W, int a_H) { w=a_W; h=a_H; }

  //----------------------------------------

  inline int  x2(void)  { return x+w-1; }
  inline int  y2(void)  { return y+h-1; }
  inline void x2(int i) { w=i-x+1; }
  inline void y2(int i) { h=i-y+1; }

  //----------

  inline void add(int a_X, int a_Y) { x+=a_X; y+=a_Y; }
  inline void add(int a_X, int a_Y, int a_W, int a_H) { x+=a_X; y+=a_Y; w+=a_W; h+=a_H; }

  //----------------------------------------


  // return true if rect contains the specified point

  /*inline*/
  bool contains(int a_X, int a_Y)
    {
      if( a_X < x ) return false;
      if( a_Y < y ) return false;
      if( a_X >= (x+w) ) return false;
      if( a_Y >= (y+h) ) return false;
      return true;
    }

  // return true if rect overlap the specified rect

  /*inline*/
  bool intersects(int a_X, int a_Y, int a_W, int a_H)
    {
      if( (a_X+a_W) < x ) return false;
      if( (a_Y+a_H) < y ) return false;
      if( a_X >= (x+w) ) return false;
      if( a_Y >= (y+h) ) return false;
      return true;
    }

  //  __            _____
  // |  |          :  :  |
  // |__|          :..:  |
  //     __    ->  |   ..|
  //    |  |       |  :  :
  //    |__|       |__:__:
  //

  // create a rect big enough to hold itself and specified rect

  /*inline*/
  void combine(int a_X, int a_Y, int a_W, int a_H)
    {
      if( a_X < x ) { w += (x-a_X); x=a_X; }
      if( a_Y < y ) { h += (y-a_Y); y=a_Y; }
      if( (a_X+a_W) >= (x+w) ) w = (a_X+a_W) - x;
      if( (a_Y+a_H) >= (y+h) ) h = (a_Y+a_H) - y;
    }

  //    ___
  //   |  _|_          _:.
  //   | | | |   ->   | | :
  //   | |_|_|        |_|.:
  //   |___|            :
  //

  // keep only intersecting area

  /*inline*/
  void crop(int a_X, int a_Y, int a_W, int a_H)
    {
      if( a_X > x ) x = a_X;
      if( a_Y > y ) y = a_Y;
      if( (a_X+a_W) <= (x+w) ) w = (a_X+a_W) - x;
      if( (a_Y+a_H) <= (y+h) ) h = (a_Y+a_H) - y;
    }

  //----------------------------------------

  // shortcuts

  inline bool intersects(h_Rect a_Rect)
    {
      return intersects( a_Rect.x, a_Rect.y, a_Rect.w, a_Rect.h );
    }

  inline void combine(h_Rect a_Rect)
    {
      return combine( a_Rect.x, a_Rect.y, a_Rect.w, a_Rect.h );
    }

};

//----------------------------------------------------------------------

#define H_NULL_RECT h_Rect(0,0,0,0)

//#include "lib/h_Array.h"
#include "src/h_Array.h"
typedef h_Array<h_Rect> h_Rects;

//----------------------------------------------------------------------
#endif
