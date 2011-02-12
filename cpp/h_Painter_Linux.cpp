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
#ifndef h_Painter_Linux_cpp_included
#define h_Painter_Linux_cpp_included
//----------------------------------------------------------------------

//#include "lib/h_Point.h"
//#include "lib/h_String.h"
//#include "lib/h_Math.h"
//#include "gui/h_Color.h"
//#include "gui/h_Bitmap.h"
//#include "gui/h_Painter.h"
#include "h/h_Point.h"
#include "h/h_String.h"
#include "h/h_Math.h"
#include "h/h_Color.h"
#include "h/h_Bitmap.h"
#include "h/h_Painter.h"

#include <X11/Xlib.h>

#ifdef H_ALPHA
#include <X11/extensions/Xrender.h>
#endif

#include <math.h>     // floorf, TODO:h_Math.h
//#include <stdio.h>    // printf, TODO:trace()h_Debug

//#include "h_Color.h"
//#include "h_Image.h"

//----------------------------------------------------------------------

// text align
#define ta_Center        0
#define ta_Left          1
#define ta_Top           2
#define ta_Right         4
#define ta_Bottom        8
#define ta_TopLeft       16
#define ta_TopRight      32
#define ta_BottomLeft    64
#define ta_BottomRight   128
#define ta_RightStretch  256
#define ta_BottomStretch 512
#define ta_Horz          1024
#define ta_Vert          2048

#define H_DEFAULT_PENWIDTH -1

//----------

class h_PaintSource
{
  public:
    //virtual HDC getDC(void) { return 0; }
    virtual Drawable getSourceDrawable(void) { return NULL; }
    //virtual XImage*  getSourceImage(void) { return NULL; }
    #ifdef H_ALPHA
    virtual Picture getSourcePicture(void) { return NULL; }
    #endif
};

//class h_PaintTarget
//{
//  public:
//    virtual Drawable getTargetDrawable(void) { return NULL; }
//};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#ifdef H_ALPHA
XTransform h_alpha_mat_ident =
{
  {
    { XDoubleToFixed(1), XDoubleToFixed(0), XDoubleToFixed(0) },
    { XDoubleToFixed(0), XDoubleToFixed(1), XDoubleToFixed(0) },
    { XDoubleToFixed(0), XDoubleToFixed(0), XDoubleToFixed(1) }
  }
};
#endif

#define H_DEF_PENWIDTH 0

//----------------------------------------------------------------------
// h_Painter (aka Canvas)
//----------------------------------------------------------------------

class h_Painter_Linux
{
  private:
    Display*      m_Display;
    Drawable      m_Drawable;
    GC            m_GC;
    XFontStruct*  m_Font;
//    #ifdef H_ALPHA
//    Picture       m_Picture;
//    #endif
    h_Rect        m_ClipRect;
    h_Point       m_Pos;

  public:


  // if a_Ptr=NULL : window-painter, else surface-painter

    //h_Painter(h_PaintTarget* a_PaintTarget)

    h_Painter_Linux(Display* a_Display, Drawable a_Drawable)
      {

        // !!!!!!!!!!

        //m_Display = static_Core.m_Platform->m_WinDisplay;
        m_Display = a_Display;
        m_Drawable  = a_Drawable;

        // !!!!!!!!!!

//        if (window) m_Drawable = (Drawable)window;
//        else m_Drawable = static_Core.m_Platform->m_WinRoot;


        //if (!m_Drawable) m_Drawable = static_Core.m_Platform->m_WinRoot;
        if (!m_Drawable) m_Drawable = XDefaultRootWindow(m_Display);
        m_GC        = XCreateGC(m_Display,m_Drawable,0,NULL);
        m_Font      = XQueryFont(m_Display,XGContextFromGC(m_GC));
        m_ClipRect  = h_Rect(0,0,0,0);
        m_Pos       = h_Point(0,0);
        #ifdef H_ALPHA
        m_Picture   = NULL;
        #endif
      }

    //----------

    virtual ~h_Painter_Linux()
      {
        XFreeFontInfo(0,m_Font,1);   // 1??
        XFreeGC(m_Display,m_GC);
        //picture etc...
      }

    //----------------------------------------
    // internal
    //----------------------------------------

//    h_Color color(int a_Red, int a_Green, int a_Blue)
//      {
//        XColor xcol;
//        xcol.red   = a_Red << 8;
//        xcol.green = a_Green << 8;
//        xcol.blue  = a_Blue << 8;
//        xcol.flags = (DoRed or DoGreen or DoBlue);
//        XAllocColor(m_Display,H_INTERFACE->getColormap(),&xcol);
//        return h_Color(xcol.pixel);
//      }

    //----------------------------------------
    // get, set
    //----------------------------------------

    void setPos(int aX, int aY)
      {
        m_Pos.x = aX;
        m_Pos.y = aY;
      }

    virtual void setDrawColor(h_Color a_Color)
      {
        XSetForeground(m_Display,m_GC,a_Color);
      }

    virtual void setFillColor(h_Color a_Color)
      {
        XSetForeground(m_Display,m_GC,a_Color);
      }

    virtual void setTextColor(h_Color a_Color)
      {
        XSetForeground(m_Display,m_GC,a_Color);
      }

    //----------------------------------------
    // clear, reset
    //----------------------------------------

    void resetPen(void)
      {
        setPenWidth(H_DEF_PENWIDTH);
      }

    //void resetPenWidth(void)
    //  {
    //    mPenWidth = DEF_PENWIDTH;
    //    XSetLineAttributes(gDP,mGC,mPenWidth,LineSolid,CapRound,JoinRound);
    //  };

    //----------------------------------------
    // internal
    //----------------------------------------

    //void setColor(int a_Red, int a_Green, int a_Blue)
    //  {
    //    setColor( color(a_Red,a_Green,a_Blue) );
    //  }

    //----------------------------------------

    void setPenWidth(int a_Width)
      {
        //mPenWidth = aWidth;
        XSetLineAttributes(m_Display,m_GC,a_Width,LineSolid,CapRound,JoinRound);
      }

    virtual void setPenStyle(int aStyle)
      {
      }

    void setBrushStyle(int a_Style)
      {
        XSetFillStyle(m_Display,m_GC,a_Style); // FillSolid
      }

    #ifdef H_ALPHA
    /*virtual*/
    void setPicture(Picture a_Picture)
      {
        m_Picture = a_Picture;
      }
    #endif

    //----------------------------------------
    // clip rect
    //----------------------------------------

    void setClipRect(int aX1, int aY1, int aX2, int aY2)
      {
        //mClipRect.set(aX1,aY1,(aX2-aX1+1),(aY2-aY1+1));
        XRectangle r;
        r.x      = aX1;
        r.y      = aY1;
        r.width  = (aX2-aX1)+2; // xlib seems to cut off one pixel to the right & bottom... ?
        r.height = (aY2-aY1)+2; // so we compensate by adding another pixel
        XSetClipRectangles(m_Display,m_GC,0,0,&r,1,Unsorted);
        //m_ClipX1 = aX1;
        //m_ClipY1 = aY1;
        //m_ClipX2 = aX2;
        //m_ClipY2 = aY2;
        m_ClipRect = h_Rect(aX1,aY1,aX2,aY2);
      }

    //----------

    void resetClipRect(void)
      {
        //clearClipRect();
        setClipRect(m_ClipRect.x,m_ClipRect.y,m_ClipRect.x2(),m_ClipRect.y2());
      }

    //----------

    void clearClipRect(void)
      {
        XSetClipMask(m_Display,m_GC,None);
      }

    //----------------------------------------
    // shapes
    //----------------------------------------

    void drawPoint(int aX, int aY)
      {
        XDrawPoint(m_Display,m_Drawable,m_GC,aX,aY);
      }

    //----------

    void drawPoint(int aX, int aY, h_Color a_Color)
      {
        XSetForeground(m_Display,m_GC,a_Color);
        XDrawPoint(m_Display,m_Drawable,m_GC,aX,aY);
      }

    //----------

    void drawLine(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawLine(m_Display,m_Drawable,m_GC,aX1,aY1,aX2,aY2);
      }

    //----------

    void drawRect(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawRectangle(m_Display,m_Drawable,m_GC,aX1,aY1,aX2-aX1,aY2-aY1);
      }

    //----------

    /*virtual*/
    void fillRect(int x, int y, int x2, int y2)
      {
        //XFillRectangle(m_Display,m_Drawable,m_GC,x,y,w,h);
        //#ifdef H_XRENDER
				//XRenderFillRectangle(mDisplay,PictOpOver,mPicture,&rendercolor,aX1,aY1,aX2-aX1+1,aY2-aY1+1);
        //#else
        XFillRectangle(m_Display,m_Drawable,m_GC,x,y,x2-x+1,y2-y+1);
      }

    void drawCircle(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawArc(m_Display,m_Drawable,m_GC,aX1,aY1,aX2-aX1,aY2-aY1,0*64,360*64);
      }

    //----------

    void fillCircle(int aX1, int aY1, int aX2, int aY2)
      {
        XFillArc(m_Display,m_Drawable,m_GC,aX1,aY1,aX2-aX1,aY2-aY1,0*64,360*64);
      }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise
    void drawArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        //if( axAbs(aAngle2) >= 0.01/*EPSILON*/ )
        if( fabs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          // start angle = 12 o'clock
          float a1 = -aAngle1 + 0.25;
          // positive = clockwise, negative = counter-clockwise
          float a2 = -aAngle2;
          XDrawArc(m_Display, m_Drawable,m_GC, aX1,aY1,(aX2-aX1+1),(aY2-aY1+1),a1*(360*64),a2*(360*64));
        }
      }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise
    void fillArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        //if( axAbs(aAngle2) >= 0.01/*EPSILON*/ )
        if( fabs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          // start angle = 12 o'clock
          float a1 = -aAngle1 + 0.25;
          // positive = clockwise, negative = counter-clockwise
          float a2 = -aAngle2;
          XFillArc(m_Display, m_Drawable,m_GC, aX1,aY1,(aX2-aX1+1),(aY2-aY1+1),a1*(360*64),a2*(360*64));
        }
      }

    //--------------------------------------------------
    // text
    //--------------------------------------------------

    void setTextSize(int aSize)
      {
      }

    //----------

    int textWidth(char* a_Text)
      {
        return XTextWidth(m_Font, a_Text, h_Strlen(a_Text));
      }

    //----------

    int textHeight(char* a_Text)
      {
        return m_Font->ascent + m_Font->descent;
      }

    //----------

    void drawText(int a_Xpos, int a_Ypos, char* a_Text)
      {
        XDrawString(m_Display,m_Drawable,m_GC,a_Xpos,a_Ypos+m_Font->ascent,a_Text,h_Strlen(a_Text));
      }

    //--------

    virtual void drawText(int a_X1, int a_Y1, int a_X2, int a_Y2, char* a_Text, int a_Align)
      {
        int x,y;
        if (a_Align & ta_Top) y = a_Y1 + m_Font->ascent;
        else if (a_Align & ta_Bottom) y = a_Y2 - m_Font->descent;
        else y = a_Y1 + ((m_Font->ascent)>>1) + ((a_Y2-a_Y1)>>1);
        int width = textWidth(a_Text);
        if (a_Align & ta_Left) x = a_X1;
        else if (a_Align & ta_Right) x = a_X2 - width;
        else x = a_X1 + ((a_X2 - a_X1) >> 1) - ( width >> 1);
        XDrawString(m_Display,m_Drawable,m_GC,x,y,a_Text,h_Strlen(a_Text));
      }

    //void setTextSize(int aSize)
    //  {
    //  }

    //----------

    //int textWidth(axString aText)
    //  {
    //    return XTextWidth(m_Font, aText.ptr(), aText.length());
    //  }

    //----------

    //int textHeight(axString aText)
    //  {
    //    return m_Font->ascent + mFont->descent;
    //  }

    //----------

    //void drawText(int aX, int aY, axString aText)
    //  {
    //    XDrawString(m_Display,m_Drawable,m_GC,aX,aY+m_Font->ascent,aText.ptr(),aText.length());
    //  }

    //----------

    //void drawText(int x, int y, char* txt)
    //  {
    //    XDrawString(m_Display,m_Drawable,m_GC,x,y,txt,h_Strlen(txt));
    //  }

    //--------

    //void drawText(int aX1, int aY1, int aX2, int aY2, char* aText, int aAlign)
    //  {
    //    int x,y;
    //    if (aAlign & ta_Top) y = aY1 + m_Font->ascent;
    //    else if (aAlign & ta_Bottom) y = aY2 - m_Font->descent;
    //    else y = aY1 + ((m_Font->ascent)>>1) + ((aY2-aY1)>>1);
    //    int width = textWidth(aText);
    //    if (aAlign & ta_Left) x = aX1;
    //    else if (aAlign & ta_Right) x = aX2 - width;
    //    else x = aX1 + ((aX2 - aX1) >> 1) - ( width >> 1);
    //    XDrawString(m_Display,m_Drawable,m_GC,x,y,aText.ptr(),aText.length());
    //  }

    //----------------------------------------
    // bitmaps
    //----------------------------------------



    virtual void drawBitmap(h_Bitmap* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    //void paint(h_Image* a_Image, int x, int y, int srcx, int srcy, int srcw, int srch)
      {
//        XPutImage(m_Display,m_Drawable,m_GC,a_Image->getImage(),srcx,srcy,x,y,srcw,srch);
      }

    //----------


    virtual void drawSurface(h_PaintSource* aSource, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    //void paint(h_PaintSource* a_PaintSource, int x, int y, int srcx, int srcy, int srcw, int srch)
      {
//        XCopyArea(m_Display,a_PaintSource->getSourceDrawable(),m_Drawable,m_GC,srcx,srcy,srcw,srch,x,y);
      }

    //----------

    virtual void blendSurface( h_PaintSource* aSource,  int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    //void blend(h_PaintSource* a_PaintSource, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        #ifdef H_ALPHA
          int op = PictOpOver;
          XRenderComposite(m_Display,op,a_PaintSource->getSourcePicture(),None,m_Picture,aSrcX,aSrcY,0,0,aX,aY,aSrcW,aSrcH);
        //#else
        //  paint(a_Paintable,aX,aY,aSrcX,aSrcY,aSrcW,aSrcH);
        #endif
      }

    //----------

    virtual void stretchSurface( h_PaintSource* aSource, int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    //void stretch(h_PaintSource* a_PaintSource, int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        #ifdef H_ALPHA
        if (aW > 0)
        {
          if (aH > 0)
          {
            double xs = (float)aSrcW / (float)aW;
            double ys = (float)aSrcH / (float)aH;
            double zs = 1;
            XTransform xform =
            {
              {
                { XDoubleToFixed(xs), XDoubleToFixed(0 ), XDoubleToFixed(0 ) },
                { XDoubleToFixed(0 ), XDoubleToFixed(ys), XDoubleToFixed(0 ) },
                { XDoubleToFixed(0 ), XDoubleToFixed(0 ), XDoubleToFixed(zs) }
              }
            };
            //Picture pic = aImage->getPicture();
            Picture pic = a_PaintSource->getSourcePicture();
            XRenderSetPictureTransform(m_Display, pic, &xform );
            int op = PictOpOver;
            // hmmm.. is srcx, srcy transformed by the matrix too?
            float x = /*axFloor*/floorf( (float)aSrcX / xs );
            float y = /*axFloor*/floorf( (float)aSrcY / ys );
            XRenderComposite(m_Display,op,pic,None,m_Picture,(int)x,(int)y,0,0,aX,aY,aW,aH);
            XRenderSetPictureTransform(m_Display, pic, &h_alpha_mat_ident );
          } //h>0
        } //w>0
        //#else
        #endif
      }

};

//----------------------------------------------------------------------

typedef h_Painter_Linux h_Painter_Impl;

//----------------------------------------------------------------------
#endif
