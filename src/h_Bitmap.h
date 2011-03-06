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

  #include "src/h_Defines.h"
  #include "src/h_Stdlib.h"

//#ifndef H_NO_BITMAPLOADER
//  // h_BitmapLoader
//  #define STBI_NO_STDIO
//  //#define STBI_HEADER_FILE_ONLY
//  //#define STBI_NO_FAILURE_STRINGS
//  #include "extern/stb_image.c"
//  #include <stdio.h>
//  #include "src/h_File.h"
//#endif

  //#include "src/h_Surface.h"

//----------------------------------------------------------------------

class h_Bitmap_Base
{
  private:
    int     m_Width;
    int     m_Height;
    int     m_Depth;
    char*   m_Buffer;
    bool    m_Allocated;

  public:
    inline int    getWidth(void)  { return m_Width; }
    inline int    getHeight(void) { return m_Height; }
    inline int    getDepth(void)  { return m_Depth; }
    inline char*  getBuffer(void) { return m_Buffer; }

  protected:
    inline void   setWidth(int a_Width)     { m_Width=a_Width; }
    inline void   setHeight(int a_Height)   { m_Height=a_Height; }
    inline void   setDepth(int a_Depth)     { m_Depth=a_Depth; }
    inline void   setBuffer(char* a_Buffer) { m_Buffer=a_Buffer; }

  public:

    h_Bitmap_Base(int a_Width, int a_Height, int a_Depth)
      {
        m_Width     = a_Width;
        m_Height    = a_Height;
        m_Depth     = a_Depth;
        m_Buffer    = H_NULL;
        m_Allocated = false;
      }

    h_Bitmap_Base(int a_Width, int a_Height, int a_Depth, char* a_Buffer)
      {
        m_Width     = a_Width;
        m_Height    = a_Height;
        m_Depth     = a_Depth;
        m_Buffer    = (char*)a_Buffer;
        m_Allocated = false;
      }

    virtual ~h_Bitmap_Base()
      {
        if (m_Allocated) delete[] m_Buffer;
      }

    //virtual
    void allocate(void)
      {
        if (m_Buffer || m_Allocated) return;
        m_Buffer = new char[m_Width*m_Height*4];  // 32bit rgba
        m_Allocated = true;
      }

    //virtual
    void deallocate(void)
      {
        if (m_Allocated)
        {
          delete[] m_Buffer; // assumes created with new, not malloc
          m_Allocated = false;
        }
      }

    //virtual
    void swapRgba(void)
      {
        unsigned char* ptr = (unsigned char*)m_Buffer;
        for (int x=0; x<m_Width; x++)
        {
          for (int y=0; y<m_Height; y++)
          {
            unsigned char r = ptr[0];
            unsigned char g = ptr[1];
            unsigned char b = ptr[2];
            unsigned char a = ptr[3];
            *ptr++ = b;
            *ptr++ = g;
            *ptr++ = r;
            *ptr++ = a;
          }
        }
      }

    // [internal]
    inline unsigned char _alpha(int c, int a)
      {
        int ret = (c*a) >> 8;
        return ret & 0xff;
      }

    // [internal]
    inline unsigned char _scale(unsigned char c, float n )
      {
        float nc = n * (float)c;
        return (unsigned char)h_Min(255,nc);
      }

    //virtual
    void premultAlpha(void)
      {
        if (m_Buffer)
        {
          for(int y=0; y<m_Height; y++)
          {
            for(int x=0; x<m_Width; x++)
            {
              int pos = (y*m_Width + x) * 4;
              unsigned char r = m_Buffer[pos+0];
              unsigned char g = m_Buffer[pos+1];
              unsigned char b = m_Buffer[pos+2];
              unsigned char a = m_Buffer[pos+3];
              m_Buffer[pos+0] = _alpha(r,a);
              m_Buffer[pos+1] = _alpha(g,a);
              m_Buffer[pos+2] = _alpha(b,a);
              m_Buffer[pos+3] = a;
            } //for x
          } //for y
        } //mBuffer
      }

    void setBackground(unsigned char aR, unsigned char aG, unsigned char aB)
      {
        if (m_Buffer)
        {
          for(int y=0; y<m_Height; y++)
          {
            for(int x=0; x<m_Width; x++)
            {
              int pos = (y*m_Width + x) * 4;
              unsigned char r = m_Buffer[pos+0];
              unsigned char g = m_Buffer[pos+1];
              unsigned char b = m_Buffer[pos+2];
              unsigned char a = m_Buffer[pos+3];
              m_Buffer[pos+0] = _alpha(r,a) + _alpha(aR,(255-a));
              m_Buffer[pos+1] = _alpha(g,a) + _alpha(aG,(255-a));
              m_Buffer[pos+2] = _alpha(b,a) + _alpha(aB,(255-a));
              m_Buffer[pos+3] = a;
            } //for x
          } //for y
        } //mBuffer
      }

    void setBackground(h_Bitmap_Base* aBitmap, h_Rect aRect)
      {
        char* buf2 = aBitmap->getBuffer();
        int h = aBitmap->getHeight();
        //trace("aBitmap->getHeight() = " << h );
        if (m_Buffer)
        {
          int x2 = aRect.x;
          int y2 = aRect.y;
          int w2 = aRect.x2();
          int h2 = aRect.y2();
          for(int y=0; y<m_Height; y++)
          {
            x2 = aRect.x;
            for(int x=0; x<m_Width; x++)
            {
              int pos = (y*m_Width + x) * 4;
              unsigned char r = m_Buffer[pos+0];
              unsigned char g = m_Buffer[pos+1];
              unsigned char b = m_Buffer[pos+2];
              unsigned char a = m_Buffer[pos+3];
              int pos2 = (y2*h + x2) * 4;
              //trace("pos2 = " << pos2 );
              unsigned char r2 = buf2[pos2+0];
              unsigned char g2 = buf2[pos2+1];
              unsigned char b2 = buf2[pos2+2];
              //unsigned char a2 = buf2[pos+3];

              m_Buffer[pos+0] = _alpha(r,a) + _alpha(r2,(255-a));
              m_Buffer[pos+1] = _alpha(g,a) + _alpha(g2,(255-a));
              m_Buffer[pos+2] = _alpha(b,a) + _alpha(b2,(255-a));
              m_Buffer[pos+3] = a;

              x2++;
              if (x2>=w2) x2 = aRect.x;
            } //for x
            y2++;
            if (y2>=h2) y2 = aRect.y;
          } //for y

        } //mBuffer
      }

    void swizzle( float rr, float rg, float rb, float ra,
                  float gr, float gg, float gb, float ga,
                  float br, float bg, float bb, float ba,
                  float ar, float ag, float ab, float aa)
      {
        if (m_Buffer)
        {
          //trace("swizzle");
          for(int y=0; y<m_Height; y++)
          {
            for(int x=0; x<m_Width; x++)
            {
              int pos = (y*m_Width + x) * 4;
              unsigned char r = m_Buffer[pos+0];
              unsigned char g = m_Buffer[pos+1];
              unsigned char b = m_Buffer[pos+2];
              unsigned char a = m_Buffer[pos+3];
              m_Buffer[pos+0] = _scale(r,rr) + _scale(g,rg) + _scale(b,rb) + _scale(a,ra);
              m_Buffer[pos+1] = _scale(r,gr) + _scale(g,gg) + _scale(b,gb) + _scale(a,ga);
              m_Buffer[pos+2] = _scale(r,br) + _scale(g,bg) + _scale(b,bb) + _scale(a,ba);
              m_Buffer[pos+3] = _scale(r,ar) + _scale(g,ag) + _scale(b,ab) + _scale(a,aa);
              //mBuffer[pos+3] = a;
            } //for x
          } //for y
        } //mBuffer
      }

};

//----------------------------------------------------------------------

#ifdef H_WIN32
  #include "src/h_Bitmap_Win32_impl.h"
#endif

#ifdef H_LINUX
  #include "src/h_Bitmap_Linux_impl.h"
#endif

//----------------------------------------------------------------------

typedef h_Bitmap_Impl h_Bitmap;

//----------------------------------------------------------------------
#endif

