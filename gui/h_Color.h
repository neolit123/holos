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
#ifndef h_Color_included
#define h_Color_included
//----------------------------------------------------------------------

typedef unsigned long h_Color;

//----------------------------------------------------------------------

/*
#define RED_SHIFT   24
#define GREEN_SHIFT 16
#define BLUE_SHIFT  8
#define ALPHA_SHIFT 0
#define RED_MASK    0xff000000
#define GREEN_MASK  0x00ff0000
#define BLUE_MASK   0x0000ff00
#define ALPHA_MASK  0x000000ff
*/

/*
#define H_RGBA(r,g,b,a) \
  ( (r<<RED_SHIFT)   +  \
    (g<<GREEN_SHIFT) +  \
    (b<<BLUE_SHIFT)  +  \
    (a<<ALPHA_SHIFT) )
*/

/*
#define H_RGB(r,g,b)    \
  ( (r<<RED_SHIFT)   +  \
    (g<<GREEN_SHIFT) +  \
    (b<<BLUE_SHIFT)  +  \
    (0) )
*/

//----------------------------------------------------------------------

#ifdef H_WIN32
  #define H_RGB(r,g,b)  RGB(r,g,b)
#endif

//TODO
#ifdef H_LINUX
  #define H_RGB(r,g,b)  0
#endif

//----------------------------------------------------------------------
#endif

