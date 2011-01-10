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
#ifndef h_Bitmap_Win32_included
#define h_Bitmap_Win32_included
//----------------------------------------------------------------------

#include <windows.h>

class h_Bitmap_Win32// : public h_Bitmap_Base
{
  private:
    int       m_Width;
    int       m_Height;
    int       m_Depth;
    bool      m_Allocated;
    char*     m_Buffer;
    bool      m_Prepared;
    HBITMAP   m_Bitmap;

  public:

    inline int      getWidth(void)  { return m_Width; }
    inline int      getHeight(void) { return m_Height; }
    inline int      getDepth(void)  { return m_Depth; }
    inline char*    getBuffer(void) { return m_Buffer; }
    inline HBITMAP  getBitmap(void) { return m_Bitmap; }

  public:

    h_Bitmap_Win32(int a_Width, int a_Height, int a_Depth, char* a_Buffer)
    //: h_Bitmap_Base()
      {
        m_Width     = a_Width;
        m_Height    = a_Height;
        m_Depth     = a_Depth;
        m_Prepared  = false;
        if (a_Buffer)
        {
          m_Allocated = false;
          m_Buffer    = a_Buffer;
        }
        else
        {
          m_Allocated = true;
          m_Buffer    = new char[m_Width*m_Height*4]; // 32bit rgba
        }
      }

    h_Bitmap_Win32(int a_Width, int a_Height, int a_Depth)
    //: h_Bitmap_Base()
      {

        m_Width     = a_Width;
        m_Height    = a_Height;
        m_Depth     = a_Depth;
        m_Allocated = false;
        //m_Buffer    = H_NULL;
        //m_Bitmap    = H_NULL;

        BITMAPINFO bmi;
        h_Memset(&bmi,0,sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth       = m_Width;      // width of the bitmap, in pixels.
        bmi.bmiHeader.biHeight      = -m_Height;    // height (negative)
        bmi.bmiHeader.biPlanes      = 1;            // number of planes for target device. must be set to 1.
        bmi.bmiHeader.biBitCount    = 32;           // bits per pixel
        bmi.bmiHeader.biCompression = BI_RGB;       // uncompressed
        bmi.bmiHeader.biSizeImage   = 0;            // size, in bytes, of the image. may be set to zero for BI_RGB bitmaps.

        //http://msdn.microsoft.com/en-us/library/dd183494%28VS.85%29.aspx
        //
        //HBITMAP CreateDIBSection(
        //  HDC hdc,
        //  const BITMAPINFO *pbmi,
        //  UINT iUsage,
        //  VOID **ppvBits,
        //  HANDLE hSection,
        //  DWORD dwOffset
        //);
        //
        //The CreateDIBSection function creates a DIB that applications can write to directly.
        //The function gives you a pointer to the location of the bitmap bit values.
        //You can supply a handle to a file-mapping object that the function will use to create the bitmap,
        //or you can let the system allocate the memory for the bitmap.
        //
        //If hSection is NULL, the system allocates memory for the DIB.
        //In this case, the CreateDIBSection function ignores the dwOffset parameter.
        //An application cannot later obtain a handle to this memory.
        //The dshSection member of the DIBSECTION structure filled in
        //by calling the GetObject function will be NULL.
        //
        //If the function succeeds, the return value is a handle to the newly created DIB,
        //and *ppvBits points to the bitmap bit values.
        //If the function fails, the return value is NULL, and *ppvBits is NULL.

        void* ptr;
        HDC tempdc = GetDC(0); // GetDC(mWin);
        m_Bitmap = CreateDIBSection(tempdc,&bmi,DIB_RGB_COLORS,&ptr,NULL,0);
        ReleaseDC(0,tempdc);
        m_Buffer = (char*)ptr;
        m_Prepared = true;
      }

    ~h_Bitmap_Win32()
      {
        if (m_Prepared) DeleteObject(m_Bitmap); // deletes allocated buffer
        if (m_Allocated) delete[] m_Buffer;
      }

};

//----------------------------------------------------------------------

typedef h_Bitmap_Win32 h_Bitmap_Impl;

//----------------------------------------------------------------------
#endif

