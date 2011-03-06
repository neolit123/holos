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
#ifndef h_Painter_Win32_impl_included
#define h_Painter_Win32_impl_included
//----------------------------------------------------------------------

//#include "lib/h_String.h"
//#include "lib/h_Math.h"
//#include "gui/h_Color.h"
//#include "gui/h_Bitmap.h"
//#include "gui/h_Painter.h"

#include "src/h_String.h"
#include "src/h_Math.h"
#include "src/h_Color.h"
#include "src/h_Bitmap.h"
#include "src/h_Painter.h"

#include <windows.h>

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
    virtual HDC getDC(void) = 0;//{ return 0; }
};

//class h_PaintTarget
//{
//};

//----------

class h_Painter_Win32 //: public h_Painter_Base
{
  private:
		HDC     m_DC;
    HWND    m_WinHandle;
    int     m_Xpos,m_Ypos;
    // pen
    //h_Color m_PenColor;
    h_Color m_DrawColor;
		HPEN    m_Pen, m_OldPen;      // set when clearPen, use with resetPen()
    HPEN    m_NullPen;            // empty pen (shapes without borders)
    //int     m_CurrentPen;
    // brush
    //h_Color m_BrushColor;
    h_Color m_FillColor;
		HBRUSH  m_Brush, m_OldBrush;
    HBRUSH  m_NullBrush;
    //int     m_CurrentBrush;
    // text
    h_Color m_TextColor;
    HFONT   m_Font;
    // clipping
    HRGN    m_ClipRegion;
    bool    m_HasClipRect;
    h_Rect  m_ClipRect;
    // alpha
    BLENDFUNCTION m_BlendFunc;


  public:
    inline HDC getDC(void) { return m_DC; }

  public:

    //TODO:
    //h_Painter_Win32(HWND a_Window)

    h_Painter_Win32(void* a_Ptr=H_NULL)
    //: h_Painter()
      {
        HDC tempdc = GetDC(0);

        if (a_Ptr)
        {
          // window
          m_WinHandle = (HWND)a_Ptr;
          m_DC = GetDC(m_WinHandle);
        }
        else
        {
          // surface
          m_WinHandle = H_NULL;
          m_DC = CreateCompatibleDC(tempdc);
        }

        //m_PenColor   = H_RGB(192,192,192); //H_RGB(AX_GREY_LIGHT);
        //m_BrushColor = H_RGB(128,128,128);//getColor(AX_GREY);
        m_DrawColor   = H_RGB(192,192,192); //H_RGB(AX_GREY_LIGHT);
        m_FillColor = H_RGB(128,128,128);//getColor(AX_GREY);
        m_TextColor  = H_RGB(255,255,255); //getColor(AX_WHITE);
        //m_CurrentPen = 0;
        //m_CurrentBrush = 0;
        m_HasClipRect = false;

        // font
        LOGFONT LFont;
        //memset(&LFont,0,sizeof(LFont));
        //strcpy(LFont.lfFaceName,"Arial");
        h_Memset(&LFont,0,sizeof(LFont));
        h_Strcpy(LFont.lfFaceName,"Arial");
        LFont.lfHeight = -MulDiv(8,GetDeviceCaps(tempdc,LOGPIXELSY),72);
        m_Font = CreateFontIndirect(&LFont);
        SelectObject(m_DC,m_Font);

        ReleaseDC(0,tempdc);

        m_NullPen = CreatePen(PS_NULL,0,0);
        LOGBRUSH logbrush;          // http://msdn.microsoft.com/en-us/library/dd145035%28v=VS.85%29.aspx
        logbrush.lbStyle = BS_NULL; // BS_HATCHED, BS_HOLLOW, BS_NULL, BS_SOLID, ++
        logbrush.lbColor = 0;       // ignored if null
        logbrush.lbHatch = 0;       // if BS_HATCHED: HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
        m_NullBrush = CreateBrushIndirect(&logbrush);
        m_Pen   = (HPEN)GetStockObject(DC_PEN);
        m_Brush = (HBRUSH)GetStockObject(DC_BRUSH);
        SelectObject(m_DC,m_Pen);
        SelectObject(m_DC,m_Brush);
        m_BlendFunc.BlendOp              = AC_SRC_OVER; // wingdi.h
        m_BlendFunc.BlendFlags           = 0;
        m_BlendFunc.SourceConstantAlpha  = 255;//128;//0x7f;
        m_BlendFunc.AlphaFormat          = AC_SRC_ALPHA; // 0 =  ignore source alpha channel
      }

//    h_Painter_Win32()
//    //: h_Painter()
//      {
//        // surface
//        HDC tempdc = GetDC(0);
//        m_WinHandle = H_NULL;
//        m_DC = CreateCompatibleDC(tempdc);
//        ReleaseDC(0,tempdc);
//      }
//
//    //----------
//
//    h_Painter_Win32(HWND a_Window)
//    //: h_Painter()
//      {
//        // window
//        m_WinHandle = (HWND)a_Window;
//        m_DC = GetDC(m_WinHandle);
//      }

    //----------

    virtual ~h_Painter_Win32()
      {
        if (m_WinHandle) ReleaseDC(m_WinHandle,m_DC);
        else DeleteDC(m_DC);

//        DeleteObject(mNullPen);
//        DeleteObject(mNullBrush);
//        if (mFont) DeleteObject(mFont);
//        if (mWinHandle) ReleaseDC((HWND)mWinHandle,mDC);
//        else DeleteDC(mDC);

      }

    //----------------------------------------
    //
    //----------------------------------------


    //----------------------------------------
    // get, set
    //----------------------------------------

    virtual void setPos(int a_Xpos, int a_Ypos)
      {
        MoveToEx(m_DC,a_Xpos,a_Ypos,0);
        m_Xpos = a_Xpos;
        m_Ypos = a_Ypos;
      }

    //----------------------------------------

    //virtual h_Color getColor(int a_Red, int a_Green, int a_Blue)
    //  {
    //    h_Color color = RGB(a_Red,a_Green,a_Blue);
    //    return color;
    //  }

    //----------

    virtual void setDrawColor(h_Color a_Color)
      {
        m_DrawColor = a_Color;
        SetDCPenColor(m_DC, a_Color);
      }

    //----------

    virtual void setFillColor(h_Color aColor)
      {
        m_FillColor = aColor;
        SetDCBrushColor(m_DC, aColor);
      }

    //----------

    virtual void setTextColor(h_Color aColor)
      {
        m_TextColor = aColor;
        SetTextColor(m_DC,aColor);
      }

    //----------------------------------------
    // clear, reset
    //----------------------------------------

    virtual void clearPen(void)
      {
        m_OldPen = (HPEN)SelectObject( /*(HDC)*/m_DC, m_NullPen );
      }

    //----------

    virtual void resetPen(void)
      {
        HPEN prev = (HPEN)SelectObject(m_DC,m_OldPen);
        DeleteObject(prev);
      }

    //----------

    virtual void clearBrush(void)
      {
        m_OldBrush = (HBRUSH)SelectObject( (HDC)m_DC, m_NullBrush );
      }

    //----------

    virtual void resetBrush(void)
      {
        HBRUSH prev = (HBRUSH)SelectObject(m_DC,m_OldBrush);
        DeleteObject(prev);
      }

    //----------------------------------------
    // internal
    //----------------------------------------

    void _setPen(int aColor, int aWidth=H_DEFAULT_PENWIDTH)
      {
        HPEN pen = CreatePen(PS_SOLID,aWidth,aColor);
        m_OldPen = (HPEN)SelectObject((HDC)m_DC, pen);
      };

    //----------

    void _setBrush(int aColor)
      {
        if (m_Brush)
        {
          SelectObject(m_DC,m_OldBrush);
          DeleteObject(m_Brush);
        }
        m_Brush = CreateSolidBrush(aColor);
        m_OldBrush = (HBRUSH)SelectObject((HDC)m_DC, m_Brush);
      };

    //----------------------------------------

    virtual void setPenWidth(int aWidth)
      {
        //mPenWidth = aWidth;
        _setPen(m_DrawColor,aWidth);
      };

    virtual void setPenStyle(int aStyle)
      {
      };

    virtual void setBrushStyle(int aStyle)
      {
      };

    //----------------------------------------
    // clip rect
    //----------------------------------------

    // The CreateRectRgn function creates a rectangular region.

    // Regions created by the Create<shape>Rgn methods
    // (such as CreateRectRgn and CreatePolygonRgn) only include
    // the interior of the shape; the shape's outline is excluded
    // from the region. This means that any point on a line between
    // two sequential vertices is not included in the region.
    // If you were to call PtInRegion for such a point, it would return zero as the result.

    // The SelectClipRgn function selects a region as the current clipping region for the specified device context.

    virtual void setClipRect(int aX1, int aY1, int aX2, int aY2)
      {
        //HRGN hrgn = /*hrgn = */CreateRectRgn( aX1-1, aY1-1, aX2+1, aY2+1 );
        if (m_HasClipRect) DeleteObject(m_ClipRegion);
        m_ClipRegion = /*hrgn = */CreateRectRgn( aX1-1, aY1-1, aX2+1, aY2+1 );
        SelectClipRgn(m_DC,m_ClipRegion);
        m_HasClipRect = true;
        //m_ClipX1 = aX1;
        //m_ClipY1 = aY1;
        //m_ClipX2 = aX2;
        //m_ClipY2 = aY2;
        m_ClipRect.x = aX1;
        m_ClipRect.y = aY1;
        m_ClipRect.x2(aX2);
        m_ClipRect.y2(aY2);
      }

    //----------

    virtual void resetClipRect(void)
      {
        //clearClipRect();
        //setClipRect(m_ClipX1,m_ClipY1,m_ClipX2,m_ClipY2);
        setClipRect(m_ClipRect.x, m_ClipRect.y, m_ClipRect.x2(), m_ClipRect.y2() );
      }

    //----------

    // To remove a device-context's clipping region, specify a NULL region handle.

    virtual void clearClipRect(void)
      {
        SelectClipRgn(m_DC,NULL);
        if (m_HasClipRect) DeleteObject(m_ClipRegion);
      }

    //----------------------------------------
    // shapes
    //----------------------------------------

    virtual void drawPoint(int aX, int aY)
      {
        //SetPixel(mDC,aX,aY,mPens[mCurrentPen].mColor.get());
        SetPixel(m_DC,aX,aY,m_DrawColor);
      }

    //----------

    virtual void drawPoint(int aX, int aY, h_Color aColor)
      {
        //SetPixel(mDC,aX,aY,mPens[mCurrentPen].mColor.get());
        SetPixel(m_DC,aX,aY,aColor);
      }

    //----------

    // The LineTo function draws a line from the current position up to, but not including, the specified point.

    virtual void drawLine(int aX1, int aY1, int aX2, int aY2)
      {
        setPos(aX1,aY1);
        LineTo(m_DC,aX2,aY2);
        drawPoint(aX2,aY2);         // !!!
      }

    //----------

    virtual void drawRect(int aX1, int aY1, int aX2, int aY2)
      {
        setPos(aX1,aY1);
        LineTo(m_DC,aX2,aY1);
        LineTo(m_DC,aX2,aY2);
        LineTo(m_DC,aX1,aY2);
        LineTo(m_DC,aX1,aY1);
        //drawPoint(aX2,aY2);         // !!!
      }

    //----------

    // The FillRect function fills a rectangle by using the specified brush.
    // This function includes the left and top borders,
    // but excludes the right and bottom borders of the rectangle.

    virtual void fillRect(int aX1, int aY1, int aX2, int aY2)
      {
        RECT R;
        R.left    = aX1;
        R.top     = aY1;
        R.right   = aX2+1;           // !!!
        R.bottom  = aY2+1;           // !!!
        FillRect(m_DC,&R,m_Brush);
        //Rectangle(mDC,aX1,aY1,aX2,aY2);
      }

    //----------

    virtual void drawCircle(int aX1, int aY1, int aX2, int aY2)
      {
        //Ellipse(mDC, aX1,aY1,aX2,aY2 );
        Arc( m_DC,aX1,aY1,aX2+1,aY2+1,0,0,0,0);
      }

    //----------

    // The Ellipse function draws an ellipse. The center of the ellipse is the
    // center of the specified bounding rectangle. The ellipse is outlined by
    // using the current pen and is filled by using the current brush.

    virtual void fillCircle(int aX1, int aY1, int aX2, int aY2)
      {
        clearPen();
        Ellipse( m_DC,aX1,aY1,aX2,aY2);
        resetPen();
      }

    //----------


    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise
    virtual void drawArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        if( h_Abs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          float a1 = aAngle1 -= 0.25;
          float a2 = a1 + aAngle2;
          if( aAngle2>0 )
          {
            float temp = a1;
            a1 = a2;
            a2 = temp;
          }
          float w = aX2-aX1;
          float h = aY2-aY1;
          float x = aX1 + w*0.5;
          float y = aY1 + h*0.5;
          float size = h_Max(w,h);
          float x1 = x + h_CosF(a1*PI2) * size;
          float y1 = y + h_SinF(a1*PI2) * size;
          float x2 = x + h_CosF(a2*PI2) * size;
          float y2 = y + h_SinF(a2*PI2) * size;
          Arc(m_DC,aX1,aY1,aX2,aY2,(int)x1,(int)y1,(int)x2,(int)y2);
        }
      }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise
    virtual void fillArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        if( h_Abs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          clearPen();
          float a1 = aAngle1 -= 0.25;
          float a2 = a1 + aAngle2;
          if( aAngle2>0 )
          {
            float temp = a1;
            a1 = a2;
            a2 = temp;
          }
          float w = aX2-aX1;
          float h = aY2-aY1;
          float x = aX1 + w*0.5;
          float y = aY1 + h*0.5;
          float size = h_Max(w,h);
          float x1 = x + h_CosF(a1*PI2) * size;
          float y1 = y + h_SinF(a1*PI2) * size;
          float x2 = x + h_CosF(a2*PI2) * size;
          float y2 = y + h_SinF(a2*PI2) * size;
          Pie(m_DC,aX1,aY1,aX2,aY2,(int)x1,(int)y1,(int)x2,(int)y2);
          resetPen();
        }
      }

    //----------------------------------------
    // text
    //----------------------------------------

    virtual void setTextSize(int aSize)
      {

        //mFont = CreateFontIndirect(&LFont);
        //SelectObject(mDC,mFont);

        LOGFONT lfont;
        HFONT font = (HFONT)GetCurrentObject(m_DC,OBJ_FONT);
        GetObject(font,sizeof(LOGFONT),&lfont);
        lfont.lfHeight = -MulDiv(aSize,GetDeviceCaps(m_DC,LOGPIXELSY),72);

        if (m_Font) DeleteObject(m_Font);
        m_Font = CreateFontIndirect(&lfont);
        SelectObject(m_DC,m_Font);
      }

    //----------

    virtual int textWidth(h_String aText)
      {
        SIZE S;
        GetTextExtentPoint32(m_DC, aText.ptr(), aText.length(), &S);
        return S.cx;
      }

    //----------

    virtual int textHeight(h_String aText)
      {
        SIZE S;
        GetTextExtentPoint32(m_DC, aText.ptr(), aText.length(), &S);
        return S.cy;
      }

    //----------

    // hack alert!
    // draw to a rect size 1000x1000, and align to upper left

    virtual void drawText(int aX, int aY, h_String aText)
      {
        //SetBkMode(mDC,TRANSPARENT);
        //TextOut(mDC,aX,aY,aText.ptr(), aText.length() );
        drawText( aX,aY, aX+1000,aY+1000, aText, ta_Left|ta_Top);
      }

    //--------

    virtual void drawText(int aX1, int aY1, int aX2, int aY2, h_String aText, int aAlign)
      {
        SetBkMode(m_DC,TRANSPARENT);
        RECT R;
        R.left    = aX1;
        R.top     = aY1;
        R.right   = aX2;
        R.bottom  = aY2;
        int flags = 0;
        if (aAlign & ta_Left) flags = flags | DT_LEFT;
        else if (aAlign & ta_Right) flags = flags | DT_RIGHT;
        else flags = flags | DT_CENTER;
        if (aAlign & ta_Top) flags = flags | DT_TOP;
        else if (aAlign & ta_Bottom) flags = flags | DT_BOTTOM;
        else flags = flags | DT_VCENTER;
        HFONT oldfont = (HFONT)SelectObject(m_DC, m_Font);
        DrawText(m_DC, aText.ptr(), -1, &R, flags | DT_NOCLIP | DT_SINGLELINE | DT_NOPREFIX);
        SelectObject(m_DC, oldfont);
      }

    //----------------------------------------
    // bitmap
    //----------------------------------------

    //BOOL BitBlt(
    //  HDC hdcDest,  // Handle to the destination device context.
    //  int nXDest,   // logical x-coordinate of the upper-left corner of the destination rectangle.
    //  int nYDest,   // logical y-coordinate of the upper-left corner of the destination rectangle.
    //  int nWidth,   // logical width of the source and destination rectangles.
    //  int nHeight,  // logical height of the source and the destination rectangles.
    //  HDC hdcSrc,   // Handle to the source device context.
    //  int nXSrc,    // logical x-coordinate of the upper-left corner of the source rectangle.
    //  int nYSrc,    // logical y-coordinate of the upper-left corner of the source rectangle.
    //  DWORD dwRop   // raster-operation code.
    //);

    virtual void drawBitmap(h_Bitmap* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        HDC tempdc = CreateCompatibleDC(m_DC);
        SelectObject(tempdc,/*(HBITMAP)*/aBitmap->getBitmap());
        BitBlt(m_DC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,SRCCOPY);
        DeleteDC(tempdc);
      }

    //--------------------------------------------------
    // surface
    //--------------------------------------------------

    //virtual void drawImage(axImage* aImage, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    virtual void drawSurface(h_PaintSource* aSource, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        //HDC tempdc = (HDC)aImage->getHandle();
        //HDC tempdc = (HDC)aSurface->getHandle();
        HDC tempdc = aSource->getDC();
        BitBlt(m_DC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,SRCCOPY);
      }

    //----------

    //virtual void renderBitmap(axBitmap* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    //  {
    //    drawBitmap(aBitmap,aX,aY,aSrcX,aSrcY,aSrcW,aSrcH);
    //  }

    //typedef struct _BLENDFUNCTION {
    //  BYTE BlendOp;
    //  BYTE BlendFlags;
    //  BYTE SourceConstantAlpha;
    //  BYTE AlphaFormat;
    //} BLENDFUNCTION, *PBLENDFUNCTION, *LPBLENDFUNCTION;

    //virtual void renderImage( axImage*  aImage,  int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    virtual void blendSurface( h_PaintSource* aSource,  int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        //HDC tempdc = (HDC)aImage->getHandle();
        HDC tempdc = /*(HDC)*/aSource->getDC();
        AlphaBlend(m_DC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,aSrcW,aSrcH,m_BlendFunc);
        // link with: libmsimg32
      }

    //BOOL StretchBlt(
    //  __in  HDC hdcDest,
    //  __in  int nXOriginDest,
    //  __in  int nYOriginDest,
    //  __in  int nWidthDest,
    //  __in  int nHeightDest,
    //  __in  HDC hdcSrc,
    //  __in  int nXOriginSrc,
    //  __in  int nYOriginSrc,
    //  __in  int nWidthSrc,
    //  __in  int nHeightSrc,
    //  __in  DWORD dwRop
    //);

    //virtual void stretchImage( axImage*  aImage,  int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    virtual void stretchSurface( h_PaintSource* aSource, int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        //HDC tempdc = (HDC)aImage->getHandle();
        HDC tempdc = (HDC)aSource->getDC();
        AlphaBlend(m_DC,aX,aY,aW,aH,tempdc,aSrcX,aSrcY,aSrcW,aSrcH,m_BlendFunc);
        // link with: libmsimg32
      }


};

//----------------------------------------------------------------------

typedef h_Painter_Win32 h_Painter_Impl;

//----------------------------------------------------------------------
#endif
