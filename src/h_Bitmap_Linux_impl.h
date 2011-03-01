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

class h_Bitmap_Linux
{

  friend class h_Window_Linux;

  private:

    Display*  m_Display;
    XImage*   m_Image;
    int       m_Width;
    int       m_Height;
    int       m_Depth;
    char*     m_Buffer;
    bool      m_Allocated;
    bool      m_Prepared;

  public:

    // accessors
    inline XImage*    getImage(void)  { return m_Image; }
    inline int        getWidth(void)  { return m_Width; }
    inline int        getHeight(void) { return m_Height; }
    inline int        getDepth(void)  { return m_Depth; }
    inline char*      getBuffer(void) { return m_Buffer; }

    // h_PaintSource
    //virtual XImage* getImage(void)  { return m_Image; }

  //public:
  private:
  //must be created from h_Window

    h_Bitmap_Linux(Display* a_Display, int a_Width, int a_Height, int a_Depth)
      {
        m_Display   = a_Display;
        m_Width     = a_Width;
        m_Height    = a_Height;
        m_Depth     = a_Depth;
        m_Buffer    = H_NULL;
        m_Prepared  = false;
        m_Allocated = false;
        allocate();
        prepare();
      }

    h_Bitmap_Linux(Display* a_Display, int a_Width, int a_Height, int a_Depth, char* a_Buffer)
      {
        m_Display   = a_Display;
        m_Width     = a_Width;
        m_Height    = a_Height;
        m_Depth     = a_Depth;
        m_Buffer    = (char*)a_Buffer;
        m_Prepared  = false;
        m_Allocated = false;
        //allocate();
        //prepare();
      }

  public:

    //virtual
    ~h_Bitmap_Linux()
      {
        if (m_Prepared)
        {
          m_Image->data = H_NULL;    // we want to delete it ourselves...
          XDestroyImage(m_Image);
        }
        if (m_Allocated) delete[] m_Buffer;
      }

    //----------

    void prepare(void)
      {
        if (m_Prepared) return;
        //if (!m_Buffer) allocate();
        int screen = XDefaultScreen(m_Display);
        Visual* visual = XDefaultVisual(m_Display,screen);
        m_Image = XCreateImage(
          m_Display,
          visual,
          m_Depth,                // depth
          ZPixmap,                // format
          0,                      // offset
          m_Buffer,               // data
          m_Width, m_Height,      // size
          32,                     // pad
          0                       // bytes per line
        );
        m_Prepared = true;
      }

    //----------

    void allocate(void)
      {
        if (m_Buffer || m_Allocated) return;
        m_Buffer = new char[m_Width*m_Height*4];  // 32bit rgba
        m_Allocated = true;
      }

};

//----------------------------------------------------------------------

typedef h_Bitmap_Linux h_Bitmap_Impl;
//typedef h_Bitmap_Linux h_Bitmap;

//----------------------------------------------------------------------
#endif
