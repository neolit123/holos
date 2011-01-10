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
#ifndef h_Bitmap_included
#define h_Bitmap_included
//----------------------------------------------------------------------

//class h_Bitmap_Base
//{
//  protected:
//
//  public:
//
//    h_Bitmap_Base()
//      {
//      }
//
//    virtual ~h_Bitmap_Base()
//      {
//      }
//
//    //----------
//};

//----------------------------------------------------------------------

#ifdef H_WIN32
  #include "gui/impl/h_Bitmap_Win32.h"
#endif

#ifdef H_LINUX
  #include "gui/impl/h_Bitmap_Linux.h"
#endif

//----------------------------------------------------------------------

class h_Bitmap : public h_Bitmap_Impl
{
  public:
    h_Bitmap(int a_Width, int a_Height, int a_Depth, char* a_Buffer)
    : h_Bitmap_Impl(a_Width,a_Height,a_Depth,a_Buffer)
      {
      }
    h_Bitmap(int a_Width, int a_Height, int a_Depth)
    : h_Bitmap_Impl(a_Width,a_Height,a_Depth)
      {
      }
    virtual ~h_Bitmap()
      {
      }
    //TODO:
    // bitmap-functions (h_BitmapUtils.h)
};

//----------------------------------------------------------------------
#endif

