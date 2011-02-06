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
#ifndef h_BitmapUtils_included
#define h_BitmapUtils_included
//----------------------------------------------------------------------

// blur, scale, ...


//
//// - linux/ubuntu (32bit), amd phenom (x86 quadcore), nvidia (geforce 8600 gt)
////   32 bits, bgra
////   0xAARRGGBB
////   little endian
////   buffer[0] = b
////   buffer[1] = g
////   buffer[2] = r
////   buffer[3] = a
//
//#include "core/axDefines.h"
//#include "core/axMath.h"
//#include "core/axRand.h"
//#include "core/axRect.h"
////#include "base/axContext.h"
////#include "base/axBitmapBase.h"
//#include "gui/axBitmapBase.h"
//
////----------
//
//#ifdef AX_LINUX
//  #include "gui/linux/axBitmapLinux.h"
//#endif
//
//#ifdef AX_WIN32
//  #include "gui/win32/axBitmapWin32.h"
//#endif
//
//
//class axBitmap : public axBitmapImpl
//{
//  friend class axWindowWin32;
//  friend class axWindowLinux;
//  //public:
//
//  private:
//    axRand rand;
//  protected:
//
//    axBitmap(axBase* aBase, int aWidth, int aHeight, int aDepth)
//    : axBitmapImpl(aBase,aWidth, aHeight, aDepth)
//      {
//        //wtrace("axBitmap.constructor");
//      }
//
//    //----------
//
//    // the mBuffer is handled a bit differently for win32/linux,
//    // depending on if we prepare() the bitmap.
//    // so we delete it in axBitmaplinux/Win32 destructors..
//
//  public:
//
//    virtual ~axBitmap()
//      {
//        //if (mBuffer) delete[] mBuffer;
//        //mBuffer = NULL;
//      }
//
//    //----------------------------------------
//
//    inline unsigned int makeColor(unsigned char r,unsigned char g,unsigned char b,unsigned char a)
//      {
//        return  ((unsigned int)a<<24)
//               +((unsigned int)r<<16)
//               +((unsigned int)g<<8)
//               + b;
//      }
//
//    //----------------------------------------
//
//    char* createBuffer(char* aData=NULL)
//      {
//        int size = mWidth*mHeight*4;
//        // malloc ?
//        mBuffer = new char[size];
//
//        /*
//          lii:
//          drwtsn32.exe reports a "shared violation" for the axMemcpy()
//          call bellow. but only for some images - strange...
//        */
//        //trace((void*)&mBuffer);
//        //trace((void*)&aData);
//        //trace(size);
//        // ---
//        if (aData)
//          axMemcpy(mBuffer,/*(int)*/aData,size);
//        else
//          axMemset(mBuffer,0,size);
//        return mBuffer;
//      }
//
//    void clearBuffer(unsigned char aValue=0)
//      {
//        int size = mWidth*mHeight*4;
//        axMemset(mBuffer,aValue,size);
//      }
//
//    void fillBuffer(unsigned int aValue)
//      {
//        int size = mWidth*mHeight;
//        unsigned int* ptr = (unsigned int*)mBuffer;
//        for (int i=0; i<size; i++) *ptr++ = aValue;
//      }
//
//    unsigned int getPixel(int x, int y)
//      {
//        unsigned int* ptr = (unsigned int*)mBuffer;
//        return ptr[y*mWidth+x];
//      }
//
//    void setPixel(int x, int y, unsigned int col)
//      {
//        unsigned int* ptr = (unsigned int*)mBuffer;
//        ptr[y*mWidth+x] = col;
//      }
//
//    //----------------------------------------
//
//    inline unsigned char alpha(int c, int a)
//      {
//        int ret = (c*a) >> 8;
//        return ret & 0xff;
//      }
//
//    //----------------------------------------
//
//    unsigned char scale(unsigned char c, float n )
//      {
//        float nc = n * (float)c;
//        return (unsigned char)axMin(255,nc);
//      }
//
//    //----------------------------------------
//
//    void convertRgbaBgra(void)
//      {
//        if(mBuffer)
//        {
//          for(int y=0; y<mHeight; y++)
//          {
//            for(int x=0; x<mWidth; x++)
//            {
//              int pos = (y*mWidth + x) * 4;
//              unsigned char r = mBuffer[pos+0];
//              unsigned char g = mBuffer[pos+1];
//              unsigned char b = mBuffer[pos+2];
//              unsigned char a = mBuffer[pos+3];
//              mBuffer[pos+0] = b;
//              mBuffer[pos+1] = g;
//              mBuffer[pos+2] = r;
//              mBuffer[pos+3] = a;
//            } //for x
//          } //for y
//        } //mBuffer
//      }
//
//    //----------
//
//    void setBackground(unsigned char aR, unsigned char aG, unsigned char aB)
//      {
//        if(mBuffer)
//        {
//          for(int y=0; y<mHeight; y++)
//          {
//            for(int x=0; x<mWidth; x++)
//            {
//              int pos = (y*mWidth + x) * 4;
//              unsigned char r = mBuffer[pos+0];
//              unsigned char g = mBuffer[pos+1];
//              unsigned char b = mBuffer[pos+2];
//              unsigned char a = mBuffer[pos+3];
//              mBuffer[pos+0] = alpha(r,a) + alpha(aR,(255-a));
//              mBuffer[pos+1] = alpha(g,a) + alpha(aG,(255-a));
//              mBuffer[pos+2] = alpha(b,a) + alpha(aB,(255-a));
//              mBuffer[pos+3] = a;
//            } //for x
//          } //for y
//        } //mBuffer
//      }
//
//    //----------
//
//    //TODO:
//    void setBackground(axBitmap* aBitmap, axRect aRect)
//      {
//        char* buf2 = aBitmap->getBuffer();
//        int h = aBitmap->getHeight();
//        //trace("aBitmap->getHeight() = " << h );
//        if (mBuffer)
//        {
//          int x2 = aRect.x;
//          int y2 = aRect.y;
//          int w2 = aRect.x2();
//          int h2 = aRect.y2();
//          for(int y=0; y<mHeight; y++)
//          {
//            x2 = aRect.x;
//            for(int x=0; x<mWidth; x++)
//            {
//              int pos = (y*mWidth + x) * 4;
//              unsigned char r = mBuffer[pos+0];
//              unsigned char g = mBuffer[pos+1];
//              unsigned char b = mBuffer[pos+2];
//              unsigned char a = mBuffer[pos+3];
//              int pos2 = (y2*h + x2) * 4;
//              //trace("pos2 = " << pos2 );
//              unsigned char r2 = buf2[pos2+0];
//              unsigned char g2 = buf2[pos2+1];
//              unsigned char b2 = buf2[pos2+2];
//              //unsigned char a2 = buf2[pos+3];
//
//              mBuffer[pos+0] = alpha(r,a) + alpha(r2,(255-a));
//              mBuffer[pos+1] = alpha(g,a) + alpha(g2,(255-a));
//              mBuffer[pos+2] = alpha(b,a) + alpha(b2,(255-a));
//              mBuffer[pos+3] = a;
//
//              x2++;
//              if (x2>=w2) x2 = aRect.x;
//            } //for x
//            y2++;
//            if (y2>=h2) y2 = aRect.y;
//          } //for y
//
//        } //mBuffer
//      }
//
//    //----------
//
//    void swizzle( float rr, float rg, float rb, float ra,
//                  float gr, float gg, float gb, float ga,
//                  float br, float bg, float bb, float ba,
//                  float ar, float ag, float ab, float aa)
//      {
//        if(mBuffer)
//        {
//          //trace("swizzle");
//          for(int y=0; y<mHeight; y++)
//          {
//            for(int x=0; x<mWidth; x++)
//            {
//              int pos = (y*mWidth + x) * 4;
//              unsigned char r = mBuffer[pos+0];
//              unsigned char g = mBuffer[pos+1];
//              unsigned char b = mBuffer[pos+2];
//              unsigned char a = mBuffer[pos+3];
//              mBuffer[pos+0] = scale(r,rr) + scale(g,rg) + scale(b,rb) + scale(a,ra);
//              mBuffer[pos+1] = scale(r,gr) + scale(g,gg) + scale(b,gb) + scale(a,ga);
//              mBuffer[pos+2] = scale(r,br) + scale(g,bg) + scale(b,bb) + scale(a,ba);
//              mBuffer[pos+3] = scale(r,ar) + scale(g,ag) + scale(b,ab) + scale(a,aa);
//              //mBuffer[pos+3] = a;
//            } //for x
//          } //for y
//        } //mBuffer
//      }
//
//    //----------
//
//    void randomAlpha(void)
//      {
//        if(mBuffer)
//        {
//          for(int y=0; y<mHeight; y++)
//          {
//            for(int x=0; x<mWidth; x++)
//            {
//              int pos = (y*mWidth + x) * 4;
//              //unsigned char r = mBuffer[pos+0];
//              //unsigned char g = mBuffer[pos+1];
//              //unsigned char b = mBuffer[pos+2];
//              //unsigned char a = mBuffer[pos+3];
//              unsigned char a = rand.randInt(255);
//              //mBuffer[pos+0] = b;
//              //mBuffer[pos+1] = g;
//              //mBuffer[pos+2] = r;
//              mBuffer[pos+3] = a;
//            } //for x
//          } //for y
//        } //mBuffer
//      }
//
//    //----------
//
//    void premultAlpha(void)
//      {
//        if (mBuffer)
//        {
//          for(int y=0; y<mHeight; y++)
//          {
//            for(int x=0; x<mWidth; x++)
//            {
//              int pos = (y*mWidth + x) * 4;
//              unsigned char r = mBuffer[pos+0];
//              unsigned char g = mBuffer[pos+1];
//              unsigned char b = mBuffer[pos+2];
//              unsigned char a = mBuffer[pos+3];
//              mBuffer[pos+0] = alpha(r,a);
//              mBuffer[pos+1] = alpha(g,a);
//              mBuffer[pos+2] = alpha(b,a);
//              mBuffer[pos+3] = a;
//            } //for x
//          } //for y
//        } //mBuffer
//      }
//
//    //----------
//
//
//
//};
//

//----------------------------------------------------------------------
#endif

