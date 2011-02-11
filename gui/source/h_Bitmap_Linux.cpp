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
#ifndef h_Bitmap_Linux_included
#define h_Bitmap_Linux_included
//----------------------------------------------------------------------

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class h_Bitmap_Linux
{
  private:

//    Display*  m_Display;
//    XImage*   m_Image;
//    h_Bitmap* m_Bitmap;
//    bool      m_BitmapAllocated;
//    int       m_Width;
//    int       m_Height;
//    int       m_Depth;

  public:

//    // accessors
//    inline XImage*    getImage(void)  { return m_Image; }
//    inline h_Bitmap*  getBitmap(void) { return m_Bitmap; }
//    inline int        getWidth(void)  { return m_Bitmap->getWidth(); }
//    inline int        getHeight(void) { return m_Bitmap->getHeight(); }
//    inline int        getDepth(void)  { return m_Bitmap->getDepth(); }
//    inline char*      getBuffer(void) { return m_Bitmap->getBuffer(); }

    // h_PaintSource
    //virtual XImage* getImage(void)  { return m_Image; }

  public:

    h_Bitmap_Linux(int a_Width, int a_Height, int a_Depth)
      {
//        m_Display = H_INTERFACE->getDisplay();
//        m_Width   = a_Width;
//        m_Height  = a_Height;
//        m_Depth   = a_Depth;
//        m_Bitmap  = new h_Bitmap(a_Width,a_Height,a_Depth);
//        m_BitmapAllocated = true;
//        //create_image();
//        m_Image   = XCreateImage(
//          m_Display,
//          H_INTERFACE->getVisual(),
//          m_Depth,                // depth
//          ZPixmap,                // format
//          0,                      // offset
//          m_Bitmap->getBuffer(),  // data
//          m_Width, m_Height,      // size
//          32,                     // pad
//          0                       // bytes per line
//        );
      }

    //----------

    h_Bitmap_Linux(int a_Width, int a_Height, int a_Depth, char* a_Buffer)
      {
//        m_Display = H_INTERFACE->getDisplay();
//        m_Width   = a_Width;
//        m_Height  = a_Height;
//        m_Depth   = a_Depth;
//        m_Bitmap  = new h_Bitmap(a_Width,a_Height,a_Depth,a_Buffer);
//        m_BitmapAllocated = true;
//        //create_image();
//        m_Image   = XCreateImage(
//          m_Display,
//          H_INTERFACE->getVisual(),
//          m_Depth,                // depth
//          ZPixmap,                // format
//          0,                      // offset
//          m_Bitmap->getBuffer(),  // data
//          m_Width, m_Height,      // size
//          32,                     // pad
//          0                       // bytes per line
//        );
      }

    //----------

    virtual ~h_Bitmap_Linux()
      {
//        if (m_BitmapAllocated) delete m_Bitmap;
//        m_Image->data = NULL;    // we want to delete it ourselves...
//        XDestroyImage(m_Image);
      }

    //----------

    void prepare(/*int a_Width, int a_Height*/)
      {
      }

    //----------

    void allocate(void)
      {
      }

};

//----------------------------------------------------------------------

typedef h_Bitmap_Linux h_Bitmap_Impl;
//typedef h_Bitmap_Linux h_Bitmap;

//----------------------------------------------------------------------
#endif
