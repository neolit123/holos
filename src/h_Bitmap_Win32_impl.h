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

class h_Bitmap_Win32// : public h_Bitmap_Base
{
  private:
    int             m_Width;
    int             m_Height;
    int             m_Depth;
    bool            m_Allocated;
    //unsigned long*  m_Buffer;
    char*           m_Buffer;
    bool            m_Prepared;
    HBITMAP         m_Bitmap;

  public:

    inline int      getWidth(void)  { return m_Width; }
    inline int      getHeight(void) { return m_Height; }
    inline int      getDepth(void)  { return m_Depth; }
    inline char*    getBuffer(void) { return m_Buffer; }
    inline HBITMAP  getBitmap(void) { return m_Bitmap; }

  public:

    // create & prepare (for rendering) a bitmap

    h_Bitmap_Win32(int a_Width, int a_Height, int a_Depth)
      {
        m_Width  = a_Width;
        m_Height = a_Height;
        m_Depth  = a_Depth;
        m_Buffer = H_NULL;
        m_Allocated = false;
        m_Prepared = false;
        prepare();
      }

    // create a bitmap (but not prepare)
    // if a_Buffer is H_NULL, a memory buffer is allocated internally,
    // and freed (automatically) when bitmap is deleted.
    // if a_Buffer is not null, it should point to a pre-allocated buffer
    // to use..

    h_Bitmap_Win32(int a_Width, int a_Height, int a_Depth, char* a_Buffer)
    //: h_Bitmap_Base()
      {
        m_Width  = a_Width;
        m_Height = a_Height;
        m_Depth  = a_Depth;
        m_Buffer = a_Buffer;
        m_Allocated = false;
        m_Prepared = false;
        allocate();
      }

    ~h_Bitmap_Win32()
      {
        if (m_Prepared) DeleteObject(m_Bitmap); // deletes allocated buffer
        if (m_Allocated) delete[] m_Buffer;
      }

    //----------

    void prepare(void)
      {
        if (m_Prepared) return;
        BITMAPINFO bmi;
        h_Memset(&bmi,0,sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth       = m_Width;      // width of the bitmap, in pixels.
        bmi.bmiHeader.biHeight      = -m_Height;    // height (negative)
        bmi.bmiHeader.biPlanes      = 1;            // number of planes for target device. must be set to 1.
        bmi.bmiHeader.biBitCount    = 32;           // bits per pixel
        bmi.bmiHeader.biCompression = BI_RGB;       // uncompressed
        bmi.bmiHeader.biSizeImage   = 0;            // size, in bytes, of the image. may be set to zero for BI_RGB bitmaps.
        void* ptr;
        HDC tempdc = GetDC(0); // GetDC(mWin);
        m_Bitmap = CreateDIBSection(tempdc,&bmi,DIB_RGB_COLORS,&ptr,NULL,0);
        ReleaseDC(0,tempdc);
        if (m_Buffer)
        {
          h_Memcpy(ptr,m_Buffer,m_Width*m_Height*4);
        }
        if (m_Allocated)
        {
          delete[] m_Buffer; // assumes created with new, not malloc
          m_Allocated = false;
        }
        m_Buffer = (char*)ptr;
        m_Prepared = true;
      }

    //----------

    void allocate(void)
      {
        if (m_Buffer || m_Allocated) return;// false;
        m_Buffer = new char[m_Width*m_Height*4];// 32bit rgba
        m_Allocated = true; // DeleteObject() in destructor deletes allocated buffer
        //return true;
      }
};

//----------------------------------------------------------------------

typedef h_Bitmap_Win32 h_Bitmap_Impl;
//typedef h_Bitmap_Win32 h_Bitmap;

//----------------------------------------------------------------------
#endif

