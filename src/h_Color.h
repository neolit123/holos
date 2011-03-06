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
/*
  TODO: remove this, make rgb() part of h_Window
  for 16 bit displays, different rgba/bgra formats, etc..
  skin constructur normally have a h_Editor* argument,
  which can be used: h_Color col = a_Editor->rgb(255,255,255);
*/

typedef unsigned long h_Color;

//----------------------------------------------------------------------

#ifdef H_WIN32

  #define H_RGB(r,g,b) RGB(r,g,b)

#endif

//----------------------------------------------------------------------

#ifdef H_LINUX

  /*
    !! this only works for 32-bit, bgra !!!
    we need a Display*, and a Colormap to call the XAllocColor function,
    perhaps we could open a display connection, read some data, root window,
    colormap, etc, and close it again?
    or make a function/method in h_Window
      h_Color col = m_Window->color(r,g,b)
    we did that in axonlib, if i remember correctly..
    and it would be similar to how bitmaps and surfaces are handled (factory)
  */

  h_Color h_xcolor(unsigned char r, unsigned char g, unsigned char b)
      {
        //XColor xcol;
        //xcol.red   = r << 8;
        //xcol.green = g << 8;
        //xcol.blue  = b << 8;
        //xcol.flags = (DoRed or DoGreen or DoBlue);
        //XAllocColor( static_Core.m_Platform->m_WinDisplay, static_Core.m_Platform->m_WinColormap,&xcol);
        //return h_Color(xcol.pixel);
        return (r<<16) + (g<<8) + b;
      }

  #define H_RGB(r,g,b) h_xcolor(r,g,b)

#endif

//----------------------------------------------------------------------

#define H_BLACK         H_RGB(  0,  0,  0)
#define H_DARK_GREY     H_RGB( 64, 64, 64)
#define H_GREY          H_RGB(128,128,128)
#define H_LIGHT_GREY    H_RGB(192,192,192)
#define H_WHITE         H_RGB(255,255,255)

#define H_RED           H_RGB(255,  0,  0)
#define H_ORANGE        H_RGB(255,128,  0)
#define H_YELLOW        H_RGB(255,255,  0)
#define H_GREEN         H_RGB(  0,255,  0)
#define H_CYAN          H_RGB(  0,255,255)
#define H_BLUE          H_RGB(  0,  0,255)
#define H_MAGENTA       H_RGB(255,  0,255)

#define H_DARK_RED      H_RGB(128,  0,  0)
#define H_DARK_ORANGE   H_RGB(128, 64,  0)
#define H_DARK_YELLOW   H_RGB(128,128,  0)
#define H_DARK_GREEN    H_RGB(  0,128,  0)
#define H_DARK_CYAN     H_RGB(  0,128,128)
#define H_DARK_BLUE     H_RGB(  0,  0,128)
#define H_DARK_MAGENTA  H_RGB(128,  0,128)


//----------------------------------------------------------------------
#endif

