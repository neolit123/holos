
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
#ifndef h_BitmapLoader_included
#define h_BitmapLoader_included
//----------------------------------------------------------------------

#define STBI_NO_STDIO
//#define STBI_HEADER_FILE_ONLY
//#define STBI_NO_FAILURE_STRINGS

#include "extern/stb_image.c"

class h_BitmapLoader
{
  private:
    int             m_Width;
    int             m_Height;
    int             m_Depth;
    unsigned char*  m_Buffer;

  public:

    h_BitmapLoader()
      {
        m_Width = 0;
        m_Height = 0;
        m_Depth = 0;
        m_Buffer = H_NULL;
      }

    ~h_BitmapLoader()
      {
        if (m_Buffer) free(m_Buffer);
      }

    void decode(char* buffer, int size)
      {
        int req = 0;
        int comp;
        m_Buffer = stbi_load_from_memory((stbi_uc*)buffer,size,&m_Width,&m_Height,&comp,req);
        m_Depth = comp*8;
      }

    int   getWidth(void)  { return m_Width; }
    int   getHeight(void) { return m_Height; }
    int   getDepth(void)  { return m_Depth; }
    void* getBuffer(void) { return m_Buffer; }
};

//----------------------------------------------------------------------
#endif

