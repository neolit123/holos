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
#ifndef h_PngDecoder_included
#define h_PngDecoder_included
//----------------------------------------------------------------------

// read /extern/readme.txt for additional info about these files...

#include "extern/yspng/yspng.h"
#include "extern/yspng/yspng.cpp"

// search for 'ccernn' for some warning fixes...

// todo/maybe:
// rewrite it to fit more closely our coding style,
// to learn about it, and to be sure there's no
// name conflicts, or global variables, etc...



class h_PngDecoder
{
  private:
    //YsRawPngDecoder*  m_Decoder;
    int               m_Width;
    int               m_Height;
    int               m_BufferSize;
    unsigned char*    m_Buffer;

  public:

    h_PngDecoder()
      {
        m_Buffer = H_NULL;
        clear();
      }

    ~h_PngDecoder()
      {
        clear();
      }

    void clear(void)
      {
        m_Width = 0;
        m_Height = 0;
        m_BufferSize = 0;
        if (m_Buffer) delete[] m_Buffer;
        m_Buffer = H_NULL;
      }

    void decode(char* filename)
      {
        YsRawPngDecoder* m_Decoder;
        m_Decoder = new YsRawPngDecoder();
        m_Decoder->Decode("knobs.png");
        m_Decoder->Flip();   // Need to flip upside down because glDrawPixels draws y=0 bottom.
        //glDrawPixels(png.wid,png.hei,GL_RGBA,GL_UNSIGNED_BYTE,png.rgba);
        m_Width      = m_Decoder->wid;
        m_Height     = m_Decoder->hei;
        m_BufferSize = m_Width * m_Height * 4;
        m_Buffer     = new unsigned char[m_BufferSize];
        h_Memcpy(m_Buffer,m_Decoder->rgba,m_BufferSize);
        delete m_Decoder;
        m_Decoder = H_NULL;
        clear();
      }


};

//----------------------------------------------------------------------
#endif
