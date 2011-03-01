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
#ifndef h_Surface_Win32_impl_included
#define h_Surface_Win32_impl_included
//----------------------------------------------------------------------

#include <windows.h>
//#include "gui/h_Painter.h"
#include "src/h_Painter.h"

class h_Surface_Win32 : public h_PaintSource
 //: public h_Surface_Base
{
  private:
    int         m_Width;
    int         m_Height;
    int         m_Depth;
    HBITMAP     m_Bitmap;
    HGDIOBJ     m_OldObject;
    //HBITMAP     m_OldBitmap;
    h_Painter*  m_Painter;
    //HWND m_Window;

  public:
    inline h_Painter* getPainter(void) { return m_Painter; }
    //inline HBITMAP    getBitmap(void) { return m_Bitmap; }

    // h_PaintSource
    virtual HDC getDC(void) { return m_Painter->getDC(); }

  public:

    h_Surface_Win32(int a_Width, int a_Height, int a_Depth)
    //: h_Surface()
      {
        m_Width = a_Width;
        m_Height = a_Height;
        m_Depth = a_Depth;
        // ...if value is NULL, GetDC retrieves the
        // device context for the entire screen...
        HDC tempdc = GetDC(0);
        #ifndef H_ALPHA
          m_Bitmap = CreateCompatibleBitmap(tempdc,m_Width,m_Height);
        #else
          BITMAPINFO bmi;
          h_Memset(&bmi,0,sizeof(BITMAPINFO));
          bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
          bmi.bmiHeader.biWidth       = m_Width;        // width of the bitmap, in pixels
          bmi.bmiHeader.biHeight      = -m_Height;      // height (negative)
          bmi.bmiHeader.biPlanes      = 1;              // number of planes for target device. must be 1
          bmi.bmiHeader.biBitCount    = m_Depth;//32;   // bits per pixel
          bmi.bmiHeader.biCompression = BI_RGB;         // uncompressed
          bmi.bmiHeader.biSizeImage   = 0;              // size (in bytes) of image. may be 0 for BI_RGB bitmaps
          void* ptr;
          m_Bitmap = CreateDIBSection(tempdc,&bmi,DIB_RGB_COLORS,&ptr,NULL,0);
        #endif
        ReleaseDC(0,tempdc);

        m_Painter = new h_Painter();
        m_OldObject = SelectObject(m_Painter->getDC(),m_Bitmap);

      }

    //----------

    ~h_Surface_Win32()
      {
        SelectObject(m_Painter->getDC(),m_OldObject);
        delete m_Painter;
        DeleteObject(m_Bitmap);
      }

};

//----------------------------------------------------------------------

typedef h_Surface_Win32 h_Surface_Impl;

//----------------------------------------------------------------------
#endif
