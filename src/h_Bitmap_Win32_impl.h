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
#ifndef h_Bitmap_Win32_impl_included
#define h_Bitmap_Win32_impl_included
//----------------------------------------------------------------------

#include <windows.h>

class h_Bitmap_Win32 : public h_Bitmap_Base
{

  friend class h_Window_Win32;

  private:
    bool            m_Prepared;
    HBITMAP         m_Bitmap;

  public:
    inline HBITMAP  getBitmap(void) { return m_Bitmap; }

  private: //must be created from h_Window

//    h_Bitmap_Win32()
//    : h_Bitmap_Base()
//      {
//        m_Prepared = false;
//      }

//    #ifndef H_NO_BITMAPLOADER
//      h_Bitmap_Win32(char* filename)
//      : h_Bitmap_Base(filename)
//        {
//        }
//    #endif

    // create & prepare (for rendering) a bitmap

    h_Bitmap_Win32(int a_Width, int a_Height, int a_Depth)
    : h_Bitmap_Base(a_Width,a_Height,a_Depth)
      {
        m_Prepared = false;
        prepare();
      }

    // create a bitmap (but not prepare)
    // if a_Buffer is H_NULL, a memory buffer is allocated internally,
    // and freed (automatically) when bitmap is deleted.
    // if a_Buffer is not null, it should point to a pre-allocated buffer
    // to use..

    h_Bitmap_Win32(int a_Width, int a_Height, int a_Depth, char* a_Buffer)
    : h_Bitmap_Base(a_Width,a_Height,a_Depth,a_Buffer)
      {
        m_Prepared = false;
        allocate();
      }

  public:

    virtual ~h_Bitmap_Win32()
      {
        if (m_Prepared) DeleteObject(m_Bitmap); // deletes allocated buffer
      }

    //----------

    void prepare(void)
      {
        if (m_Prepared) return;
        BITMAPINFO bmi;
        h_Memset(&bmi,0,sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth       = getWidth();     // width of the bitmap, in pixels.
        bmi.bmiHeader.biHeight      = - getHeight();  // height (negative)
        bmi.bmiHeader.biPlanes      = 1;              // number of planes for target device. must be set to 1.
        bmi.bmiHeader.biBitCount    = 32;             // bits per pixel
        bmi.bmiHeader.biCompression = BI_RGB;         // uncompressed
        bmi.bmiHeader.biSizeImage   = 0;              // size, in bytes, of the image. may be set to zero for BI_RGB bitmaps.
        void* ptr;
        HDC tempdc = GetDC(0); // GetDC(mWin);
        m_Bitmap = CreateDIBSection(tempdc,&bmi,DIB_RGB_COLORS,&ptr,NULL,0);
        ReleaseDC(0,tempdc);
        if (getBuffer())
        {
          h_Memcpy(ptr,getBuffer(),getWidth()*getHeight()*4);
        }
        deallocate();
        setBuffer((char*)ptr);
        m_Prepared = true;
      }

};

//----------------------------------------------------------------------

typedef h_Bitmap_Win32 h_Bitmap_Impl;
//typedef h_Bitmap_Win32 h_Bitmap;

//----------------------------------------------------------------------
#endif

