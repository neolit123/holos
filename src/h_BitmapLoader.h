
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
#include <stdio.h>
#include "src/h_Defines.h"
#include "src/h_Stdlib.h"
#include "src/h_File.h"

class h_BitmapLoader
{
  private:
    int             m_Width;
    int             m_Height;
    int             m_Depth;
    unsigned char*  m_Buffer;
    h_File          m_File;

  public:
    int   getWidth(void)  { return m_Width; }
    int   getHeight(void) { return m_Height; }
    int   getDepth(void)  { return m_Depth; }
    char* getBuffer(void) { return (char*)m_Buffer; }


  public:

    h_BitmapLoader()
      {
        m_Width = 0;
        m_Height = 0;
        m_Depth = 0;
        m_Buffer = H_NULL;
      }

    h_BitmapLoader(char* buffer, int size)
      {
        decode(buffer,size);
      }

    h_BitmapLoader(char* filename)
      {
        load(filename);
      }

    ~h_BitmapLoader()
      {
        if (m_Buffer) h_Free(m_Buffer);
      }

    void decode(char* buffer, int size)
      {
        int req = 4; // force rgba, else 0
        int comp;
        m_Buffer = stbi_load_from_memory((stbi_uc*)buffer,size,&m_Width,&m_Height,&comp,req);
        m_Depth = comp*8;
        //swapRgba();
      }

    void load(char* filename)
      {
        unsigned long length = m_File.length(filename);
        char* buffer = (char*)h_Malloc(length);

        m_File.read(filename, buffer, length, sizeof(char), H_FILE_RB);
        decode(buffer, length);
        m_File.freebuf();

        /*
        char filepath[H_MAX_PATHSIZE] = "";
        h_Strcat(filepath, h_GetBasePath(filepath));
        h_Strcat(filepath, filename);
        FILE* fp = fopen(filepath,"rb");
        fseek(fp,0,SEEK_END);
        int size = ftell(fp);
        fseek(fp,0,SEEK_SET);
        char* buffer = (char*)h_Malloc(size);
        int num = fread(buffer,1,size,fp);
        // if (size!=num) { error }
        fclose(fp);
        decode(buffer,size);
        h_Free(buffer);
        */
      }

//    h_Surface* createSurface(h_Window* a_Window, int a_Depth=24)
//      {
//        int w = m_Width;
//        int h = m_Height;
//        int d = a_Depth;//24;//loader.getDepth(); // x crash with 32 (on 24 bit nvidia)
//        char* b = (char*)m_Buffer;
//        h_Bitmap* bmp = a_Window->createBitmap(w,h,d,b);
//        bmp->prepare();
//        h_Surface* srf = a_Window->createSurface(w,h,d);
//        h_Painter* paint = srf->getPainter();
//        paint->drawBitmap( bmp, 0,0, 0,0,w,h );
//        delete bmp;
//        return srf;
//      }

//    void swapRgba(void)
//      {
//        unsigned char* ptr = m_Buffer;
//        for (int x=0; x<m_Width; x++)
//        {
//          for (int y=0; y<m_Height; y++)
//          {
//            unsigned char r = ptr[0];
//            unsigned char g = ptr[1];
//            unsigned char b = ptr[2];
//            unsigned char a = ptr[3];
//            *ptr++ = b;
//            *ptr++ = g;
//            *ptr++ = r;
//            *ptr++ = a;
//          }
//        }
//      }

//    inline unsigned char alpha(int c, int a)
//      {
//        int ret = (c*a) >> 8;
//        return ret & 0xff;
//      }
//
//
//    void premultAlpha(void)
//      {
//        if (m_Buffer)
//        {
//          for(int y=0; y<m_Height; y++)
//          {
//            for(int x=0; x<m_Width; x++)
//            {
//              int pos = (y*m_Width + x) * 4;
//              unsigned char r = m_Buffer[pos+0];
//              unsigned char g = m_Buffer[pos+1];
//              unsigned char b = m_Buffer[pos+2];
//              unsigned char a = m_Buffer[pos+3];
//              m_Buffer[pos+0] = alpha(r,a);
//              m_Buffer[pos+1] = alpha(g,a);
//              m_Buffer[pos+2] = alpha(b,a);
//              m_Buffer[pos+3] = a;
//            } //for x
//          } //for y
//        } //mBuffer
//      }


};

//----------------------------------------------------------------------
#endif

// --- test png loader ---
/*

char temp[H_MAX_STRINGSIZE];
temp[0] = 0;
char* path = (char*)h_GetBasePath(temp);
h_Strcat(path,(char*)"../extern/mverb/background.png");
trace("path: " << path);

FILE* fp = fopen(path,"rb");
fseek(fp,0,SEEK_END);
int size = ftell(fp);
trace("size: " << size);
fseek(fp,0,SEEK_SET);
char* buffer = (char*)h_Malloc(size);
int num = fread(buffer,1,size,fp);
trace("num: " << num);
//if (num!=size) trace("bitmap size mismatch!");
fclose(fp);

h_BitmapLoader bmpload;
bmpload.decode(buffer,size);
trace("width:  " << bmpload.getWidth());
trace("height: " << bmpload.getHeight());
trace("depth:  " << bmpload.getDepth());

h_Free(buffer);

*/
