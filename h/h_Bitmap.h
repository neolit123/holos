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

/*

class h_Bitmap_Base
{
  public:
    h_Bitmap_Base(int a_Width, int a_Height, int a_Depth);
    h_Bitmap_Base(int a_Width, int a_Height, int a_Depth, unsigned long* a_Buffer)
    void prepare()
    void allocate(void)
};

*/

//----------------------------------------------------------------------

#ifdef H_WIN32
  #include "cpp/h_Bitmap_Win32.cpp"
#endif

#ifdef H_LINUX
  #include "cpp/h_Bitmap_Linux.cpp"
#endif

//----------------------------------------------------------------------

typedef h_Bitmap_Impl h_Bitmap;

//----------------------------------------------------------------------
#endif
