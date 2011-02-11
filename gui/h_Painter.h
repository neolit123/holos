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
#ifndef h_Painter_included
#define h_Painter_included
//----------------------------------------------------------------------

/*

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

class h_PaintSource;
class h_PaintTarget;

class h_Painter_Base
{
  public:
    void setPos(int aX, int aY)
    void setDrawColor(h_Color a_Color)
    void setFillColor(h_Color a_Color)
    void setTextColor(h_Color a_Color)
    void clearPen(void)   // win32
    void resetPen(void)
    void clearBrush(void) // win32
    void resetBrush(void) // win32
    void setPenWidth(int a_Width)
    void setPenStyle(int aStyle)
    void setBrushStyle(int a_Style)
    void setClipRect(int aX1, int aY1, int aX2, int aY2)
    void resetClipRect(void)
    void clearClipRect(void)
    void drawPoint(int aX, int aY)
    void drawPoint(int aX, int aY, h_Color a_Color)
    void drawLine(int aX1, int aY1, int aX2, int aY2)
    void drawRect(int aX1, int aY1, int aX2, int aY2)
    void fillRect(int x, int y, int w, int h)
    void drawCircle(int aX1, int aY1, int aX2, int aY2)
    void fillCircle(int aX1, int aY1, int aX2, int aY2)
    void drawArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
    void fillArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
    void setTextSize(int aSize)
    int textWidth(char* a_Text)
    int textHeight(char* a_Text)
    void drawText(int a_Xpos, int a_Ypos, char* a_Text)
    void drawText(int a_X1, int a_Y1, int a_X2, int a_Y2, char* a_Text, int a_Align)
    void drawBitmap(h_Bitmap* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    void drawSurface(h_PaintSource* aSource, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    void blendSurface( h_PaintSource* aSource,  int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    void stretchSurface( h_PaintSource* aSource, int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
};

*/

//----------------------------------------------------------------------

#ifdef H_WIN32
  #include "gui/source/h_Painter_Win32.cpp"
#endif

#ifdef H_LINUX
  #include "gui/source/h_Painter_Linux.cpp"
#endif

//----------------------------------------------------------------------

typedef h_Painter_Impl h_Painter;

//----------------------------------------------------------------------
#endif

