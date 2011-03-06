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
#ifndef port_mverb_skin__included
#define port_mverb_skin__included
//----------------------------------------------------------------------

// default/simple skin
// for testing/developing purposes mainly, but might be fleshed out
// to a proper skin sometime (and renamed... h_SkinSimple perhaps?

//#include "gui/h_Skin.h"
#include "src/h_Skin.h"
#include "src/h_BitmapLoader.h"

/*
  a_Mode:
  0 : default
  1 : active
  2 : hover on
  3 : hover off
  todo: fix these...
*/

class port_mverb_skin : public h_Skin
{
  private:

    h_Surface*      m_Back;
    h_Color         m_Text;
    h_Surface*      m_Knob;
    int             m_KnobCount;
    int             m_KnobWidth;
    int             m_KnobHeight;
    h_Bitmap *bmp1, *bmp2;
    h_BitmapLoader* loader;

  public:

    port_mverb_skin(h_Editor* a_Editor)
      {
        m_Back = H_NULL;
        m_Text = H_RGB(192,192,192);
        m_Knob = H_NULL;
        m_KnobCount = 129;
        m_KnobWidth = 32;
        m_KnobHeight = 32;
      }

    virtual ~port_mverb_skin()
      {
        if (m_Back) delete m_Back;
        if (m_Knob) delete m_Knob;
        if (bmp1) delete bmp1;
        if (bmp2) delete bmp2;
      }

    void load(h_Editor* a_Editor)
      {
//        h_BitmapLoader* m_Loader;
//        m_Loader = new h_BitmapLoader((char*)"mverb_back.png");
//        m_Back = m_Loader->createSurface(a_Editor);
//        delete m_Loader;
//        m_Loader = new h_BitmapLoader((char*)"mverb_knob.png");
//        m_Loader->premultAlpha();
//        m_Knob = m_Loader->createSurface(a_Editor,32);
//        delete m_Loader;

        //h_Bitmap* bmp;
        //bmp1 = a_Editor->createBitmap((char*)"mverb_back.png");

        loader = new h_BitmapLoader();
        loader->load((char*)"mverb_back.png");
        int   w = loader->getWidth();
        int   h = loader->getHeight();
        int   d = 24;//loader->getDepth();
        char* b = loader->getBuffer();
        bmp1 = a_Editor->createBitmap(w,h,d,b);
        bmp1->prepare();
        bmp1->swapRgba();

        //bmp1->swizzle(0,1,0,0,
        //              1,0,0,0,
        //              0,0,1,0,
        //              0,0,0,1);

        m_Back = a_Editor->createSurface(w,h,24);
        m_Back->getPainter()->drawBitmap(bmp1, 0,0, 0,0,w,h);
        delete loader;

        loader = new h_BitmapLoader();
        loader->load((char*)"mverb_knob.png");
        w = loader->getWidth();
        h = loader->getHeight();
        d = 32;//loader->getDepth();
        b = loader->getBuffer();
        bmp2 = a_Editor->createBitmap(w,h,d,b);
        bmp2->prepare();
        bmp2->swapRgba();
        bmp2->premultAlpha();
        m_Knob = a_Editor->createSurface(w,h,32);
        m_Knob->getPainter()->drawBitmap(bmp2, 0,0, 0,0,w,h);
        delete loader;

        //trace("OK");



      }

    virtual void drawBackground(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        //trace("");
        //if (m_Back) a_Painter->drawSurface(m_Back,a_Rect.x,a_Rect.y,0,0,a_Rect.w,a_Rect.h);
        if (m_Back) a_Painter->drawSurface(m_Back,0,0, a_Rect.x,a_Rect.y,a_Rect.w,a_Rect.h);
      }

    //----------

    virtual void drawKnobExt(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode, float a_Value, h_String a_Label, h_String a_Disp)
      {
        int index = (int)h_Floor(a_Value*m_KnobCount);
        index = h_Mini(index,m_KnobCount-1);
        int ky = m_KnobHeight * index;
        #ifdef H_ALPHA
        a_Painter->blendSurface(m_Knob,a_Rect.x,a_Rect.y, 0,ky,m_KnobWidth,m_KnobHeight);
        #else
        a_Painter->drawSurface(m_Knob,a_Rect.x,a_Rect.y, 0,ky,m_KnobWidth,m_KnobHeight);
        #endif

        int x  = a_Rect.x;
        int y  = a_Rect.y;
        a_Painter->setTextColor(m_Text);
        a_Painter->drawText(x+5,y+36,a_Disp.ptr());
      }



};

//----------------------------------------------------------------------
#endif
