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
#ifndef h_Window_included
#define h_Window_included
//----------------------------------------------------------------------

//// thread state
//#define ts_Kill  666
//#define ts_Timer 667
//
//// mouse buttons and mod keys
//#define bu_None    0
//#define bu_Left    1
//#define bu_Right   2
//#define bu_Middle  4
//#define bu_Shift   8
//#define bu_Ctrl    16
//#define bu_Alt     32
////const ini bu_Double  = 1 << 6;
////const int bu_Hover   = 1 << 7;
//
//// mouse cursor shapes
//#define cu_Arrow           2
//#define cu_ArrowUp         114
//#define cu_ArrowDown       106
//#define cu_ArrowLeft       110
//#define cu_ArrowRight      112
//#define cu_ArrowUpDown     116
//#define cu_ArrowLeftRight  108
//#define cu_ArrowDiagLeft   12
//#define cu_ArrowDiagRight  14
//#define cu_Move            52
//#define cu_Wait            150
//#define cu_ArrowWait       26
//#define cu_Hand            58
//#define cu_Finger          60
//#define cu_Cross           0
//#define cu_Pencil          86
//#define cu_Plus            90
//#define cu_Question        99
//#define cu_Ibeam           152
//
////#define DEF_CURSOR    cu_Arrow
//#define DEF_CURSOR      -1

//----------

/*

class h_Window_Base
{
  public:
    h_Window_Base(h_WidgetListener* a_Listener, h_Rect a_Rect, void* a_Parent)
    void flush(void)
    void sync(void)
    void lock(void)
    void unlock(void)
    void show(void)
    void hide(void)
    void setPos(int a_Xpos, int a_Ypos)
    void setSize(int a_Width, int a_Height)
    void setTitle(char* a_Title)
    void beginPaint(void)
    void endPaint(void)
    void reparent(void* a_Parent)
    void invalidate(int aX, int aY, int aW, int aH)
    void resetCursor(void)
    void setCursor(int a_Cursor)
    void setCursorPos(int a_Xpos, int a_Ypos)
    void showCursor(void)
    void hideCursor(void)
    void grabCursor(void)
    void releaseCursor(void)
    void startTimer(int a_Speed)
    void stopTimer(void)
    void sendEvent(unsigned int a_Value=0)
    void eventLoop(void)
    h_Painter* getPainter(void);
    //Drawable getSourceDrawable(void); // linux
    //Picture  getSourcePicture(void); // linux
    //Drawable getTargetDrawable(void); // linux
    //GLXDrawable getTargetGLXDrawable(void); // linux
};

*/

//----------------------------------------------------------------------

#ifdef H_WIN32
  #include "gui/impl/h_Window_Win32.h"
#endif

#ifdef H_LINUX
  #include "gui/impl/h_Window_Linux.h"
#endif

//----------------------------------------------------------------------

//typedef h_Window_Impl h_Window;

class h_Window : public h_Window_Impl
{
  public:

    h_Window(h_WidgetListener* a_Listener, h_Rect a_Rect, void* a_Parent)
    : h_Window_Impl(a_Listener,a_Rect,a_Parent)
      {
      }
    virtual ~h_Window() {}

    virtual void do_SetSize(int a_Width, int a_Height)
      {
        trace("do_SetSize");
        m_Rect.setSize(a_Width,a_Height);
        if (m_Flags&wf_Align) do_Realign();
      }
};




//----------------------------------------------------------------------
#endif
