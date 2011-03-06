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
#ifndef h_Bitmap_Linux_impl_included
#define h_Bitmap_Linux_impl_included
//----------------------------------------------------------------------

// TODO

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class h_Bitmap_Linux : public h_Bitmap_Base
{

  friend class h_Window_Linux;

  private:

    Display*  m_Display;
    XImage*   m_Image;
    bool      m_Prepared;

  public:
    inline XImage*    getImage(void)  { return m_Image; }

  private: //must be created from h_Window

//    h_Bitmap_Linux(Display* a_Display)
//    : h_Bitmap_Base()
//      {
//        m_Display = a_Display;
//        m_Prepared  = false;
//      }

//    #ifndef H_NO_BITMAPLOADER
//      h_Bitmap_Linux(Display* a_Display, char* filename)
//      : h_Bitmap_Base(filename)
//        {
//          m_Display = a_Display;
//          m_Prepared = false;
//        }
//    #endif

    h_Bitmap_Linux(Display* a_Display, int a_Width, int a_Height, int a_Depth)
    : h_Bitmap_Base(a_Width,a_Height,a_Depth)
      {
        m_Display   = a_Display;
        m_Prepared  = false;
        allocate();
        prepare();
      }

    h_Bitmap_Linux(Display* a_Display, int a_Width, int a_Height, int a_Depth, char* a_Buffer)
    : h_Bitmap_Base(a_Width,a_Height,a_Depth,a_Buffer)
      {
        m_Display   = a_Display;
        m_Prepared  = false;
      }

  public:

    //virtual
    virtual ~h_Bitmap_Linux()
      {
        if (m_Prepared)
        {
          m_Image->data = H_NULL;    // we want to delete it ourselves...
          XDestroyImage(m_Image);
        }
      }

    //----------

    void prepare(void)
      {
        if (m_Prepared) return;
        int screen = XDefaultScreen(m_Display);
        Visual* visual = XDefaultVisual(m_Display,screen);
        m_Image = XCreateImage(
          m_Display,
          visual,
          getDepth(),             // depth
          ZPixmap,                // format
          0,                      // offset
          getBuffer(),            // data
          getWidth(),getHeight(), // size
          32,                     // pad
          0                       // bytes per line
        );
        m_Prepared = true;
      }

};

//----------------------------------------------------------------------

typedef h_Bitmap_Linux h_Bitmap_Impl;
//typedef h_Bitmap_Linux h_Bitmap;

//----------------------------------------------------------------------
#endif
