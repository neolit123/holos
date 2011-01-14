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
#ifndef h_Window_Win32_included
#define h_Window_Win32_included
//----------------------------------------------------------------------

#include <windows.h>
#include "gui/h_Widget.h"
#include "gui/h_Painter.h"
#include "gui/h_Skin.h"

// #DEFINE idc_arrow       (32512)
// #DEFINE idc_ibeam       (32513)
// #DEFINE idc_wait        (32514)
// #DEFINE idc_cross       (32515)
// #DEFINE idc_uparrow     (32516)
// #DEFINE idc_size        (32640)
// #DEFINE idc_icon        (32641)
// #DEFINE idc_sizenwse    (32642)
// #DEFINE idc_sizenesw    (32643)
// #DEFINE idc_sizewe      (32644)
// #DEFINE idc_sizens      (32645)

// mouse cursor shapes
#define cu_Arrow          ((int)IDC_ARROW)
#define cu_ArrowUp        ((int)IDC_UPARROW)
#define cu_ArrowDown      cu_Arrow
#define cu_ArrowLeft      cu_Arrow
#define cu_ArrowRight     cu_Arrow
#define cu_ArrowUpDown    ((int)IDC_SIZENS)
#define cu_ArrowLeftRight ((int)IDC_SIZEWE)
#define cu_ArrowDiagLeft  ((int)IDC_SIZENESW)
#define cu_ArrowDiagRight ((int)IDC_SIZENWSE)
#define cu_Move           ((int)IDC_SIZEALL)
#define cu_Wait           ((int)IDC_WAIT)
#define cu_ArrowWait      ((int)IDC_APPSTARTING)
#define cu_Hand           ((int)IDC_HAND)
//#define cu_Finger         cu_Arrow
#define cu_Finger         ((int)IDC_HAND)
#define cu_Cross          ((int)IDC_CROSS)
#define cu_Pencil         cu_Arrow
#define cu_Plus           cu_Arrow
#define cu_Question       cu_Arrow
#define cu_Ibeam          cu_Arrow

#define H_DEF_CURSOR      cu_Arrow

// mouse buttons and mod keys
#define bu_None    0
#define bu_Left    1
#define bu_Right   2
#define bu_Middle  4
#define bu_Shift   8
#define bu_Ctrl    16
#define bu_Alt     32
//const ini bu_Double  = 1 << 6;
//const int bu_Hover   = 1 << 7;

//----------------------------------------------------------------------

LRESULT CALLBACK h_eventproc_win32(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//----------------------------------------------------------------------

class h_Window_Win32 : public h_Widget,
                       public h_PaintSource
{

  private:


  //win32
    HWND        m_Parent; // override from h_Widget:
    HINSTANCE   m_Instance;
    PAINTSTRUCT m_PS;
    HCURSOR     m_WinCursor;
    HWND        m_Window;

  //holos
    h_Rect      m_Rect;
    h_Painter*  m_Painter;

  //other
    int         m_AdjustHeight;
    int         m_AdjustWidth;
    int         m_ClickedButton;
    bool        m_Embedded;
    int         m_PrevCursor;

  public:
    inline h_Painter* getPainter(void)  { return m_Painter; }
    inline h_Rect     getRect(void)     { return m_Rect; }

    // h_PaintSource
    virtual HDC getDC(void) { return m_Painter->getDC(); }


//  public:
//    // widget owner (called by widgets)
//    virtual void on_Redraw(void) {}
//    virtual void on_Redraw(h_Rect a_Rect) {}
//    virtual void on_Redraw(h_Widget* a_Widget) {}

  public:


    h_Window_Win32(h_Rect a_Rect, void* a_Parent)
    : h_Widget(H_NULL,a_Rect)

      {
        m_Parent        = (HWND)a_Parent;
        m_Instance      = static_Core.getWinInstance();
        //m_ClassName     = static_Core.getWinClassName();
        m_Rect          = a_Rect;
        //m_Root          = H_NULL;
        m_WinCursor     = LoadCursor(NULL,IDC_ARROW);
        m_PrevCursor    = 0;
        m_ClickedButton = bu_None;

        RECT rc = {m_Rect.x, m_Rect.y, m_Rect.x2(), m_Rect.y2()}; // left, top, right, bottom

        if (m_Parent) // --- embedded ---
        {
          m_Embedded = true;
          AdjustWindowRectEx(&rc,WS_POPUP,FALSE,WS_EX_TOOLWINDOW);
          m_Window = CreateWindowEx(
            WS_EX_TOOLWINDOW,
            static_Core.getWinClassName(),//m_ClassName,
            0,
            WS_POPUP,
            rc.left,
            rc.top,
            rc.right-rc.left+1,
            rc.bottom-rc.top+1,
            0,
            0,
            m_Instance,
            0
          );
          reparent(m_Parent);
        }
        else // --- windowed ---
        {
          m_Embedded = false;
          AdjustWindowRectEx(&rc,WS_OVERLAPPEDWINDOW,FALSE,WS_EX_OVERLAPPEDWINDOW);
          const unsigned int wPosX = ((GetSystemMetrics(SM_CXSCREEN)-m_Rect.w)>>1) + rc.left;
          const unsigned int wPosY = ((GetSystemMetrics(SM_CYSCREEN)-m_Rect.h)>>1) + rc.top;
          m_Window = CreateWindowEx(
            WS_EX_OVERLAPPEDWINDOW,   // dwExStyle
            static_Core.getWinClassName(),//m_ClassName,              // lpClassName
            static_Core.getWinClassName(),//m_ClassName, /*0,*/       // lpWindowName
            WS_OVERLAPPEDWINDOW,      // dwStyle
            wPosX,                    // center x
            wPosY,                    // center y
            rc.right-rc.left+1,       // wWidth,
            rc.bottom-rc.top+1,       // wHeight,
            0,                        // hWndParent
            0,                        // hMenu
            m_Instance,               // hInstance
            0                         // lpParam
          );
          SetFocus(m_Window);
        }

        m_AdjustWidth = (rc.right - rc.left + 1) - m_Rect.w;
        m_AdjustHeight = (rc.bottom - rc.top + 1) - m_Rect.h;
        SetWindowLong(m_Window,GWL_USERDATA,(int)this);               // 64bit
        //DragAcceptFiles(m_Window,true);
        m_Painter = new h_Painter(m_Window);
        //if (aWinFlags & AX_WIN_BUFFERED) mSurface = createSurface(mRect.w,mRect.h,32);
      }

    //----------

    virtual ~h_Window_Win32()
      {
        delete m_Painter;
        DestroyWindow(m_Window);
      }

    //----------------------------------------
    //
    //----------------------------------------

    // do/on..

    //----------------------------------------
    //
    //----------------------------------------

    //virtual
    //void setRoot(h_Widget* a_Widget)
    //  {
    //    //a_Widget->setPainter(m_Painter);
    //    //a_Widget->setSkin(m_Skin);
    //    m_Root = a_Widget;
    //  }

    //----------------------------------------
    //
    //----------------------------------------

    virtual void flush(void)
      {
        //GdiFlush();
      }

    //----------

    virtual void sync(void)
      {
      }

    //----------

    virtual void lock(void)
      {
      }

    //----------

    virtual void unlock(void)
      {
      }

    //----------

    virtual void show(void)
      {
        //SetWindowPos(mWindow,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
        ShowWindow(m_Window,SW_SHOW);
      }

    //----------

    virtual void hide(void)
      {
        ShowWindow(m_Window,SW_HIDE);
      }

    //----------

    virtual void setPos(int a_Xpos, int a_Ypos)
      {
        SetWindowPos(m_Window,0,a_Xpos,a_Ypos,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
      }

    //----------

    virtual void setSize(int a_Width, int a_Height)
      {
        int w = a_Width + m_AdjustWidth + 0;
        int h = a_Height + m_AdjustHeight + 0;
        SetWindowPos(m_Window,HWND_TOP,0,0,w,h, SWP_NOMOVE);
        //SetWindowPos(mWindow,0,0,0,aWidth,aHeight,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
      }

    //----------

    virtual void setTitle(char* a_Title)
      {
        SetWindowText(m_Window, a_Title);
      }

    //----------------------------------------
    // mouse cursor
    //----------------------------------------

    virtual void setCursor(int a_Cursor)
      {
        if (a_Cursor<0)
        {
          hideCursor();// aCursor = DEF_CURSOR;
          m_PrevCursor = a_Cursor;
        } //-1
        else
        {
          if (m_PrevCursor<0) showCursor();
          if (a_Cursor!=m_PrevCursor)
          {
            m_WinCursor = LoadCursor(NULL,(char*)a_Cursor);
            SetCursor( m_WinCursor );
            m_PrevCursor = a_Cursor;
          }
        } //>0
      }

    //----------

    virtual void resetCursor(void)
      {
      }

    //----------

    virtual void setCursorPos(int a_Xpos, int a_Ypos)
      {
        SetCursorPos(a_Xpos,a_Ypos);
      }

    //----------

    virtual void showCursor(void)
      {
        ShowCursor(true);
      }

    //----------

    virtual void hideCursor(void)
      {
        ShowCursor(false);
      }

    //----------

    virtual void grabCursor(void)
      {
        SetCapture(m_Window);
      }

    //----------

    virtual void releaseCursor(void)
      {
        ReleaseCapture();
      }

    //----------------------------------------
    // timer
    //----------------------------------------

    virtual void startTimer(int ms)
      {
        /*mTimer = */
        SetTimer(m_Window,667,ms,NULL/*timerProc*/);
      }

    //----------

    virtual void stopTimer(void)
      {
        KillTimer(m_Window,667);
      }

    //----------------------------------------
    // painting
    //----------------------------------------

    // The invalidated areas accumulate in the update region until the region is processed when the next
    // WM_PAINT message occurs or until the region is validated by using the ValidateRect or ValidateRgn function.
    // The system sends a WM_PAINT message to a window whenever its update region is not empty
    // and there are no other messages in the application queue for that window.

    virtual void invalidate(int x, int y, int w, int h)
      {
        RECT R;
        R.left   = x;
        R.top    = y;
        R.right  = x + w - 1;
        R.bottom = y + h - 1;
        InvalidateRect(m_Window,&R,false);
        //UpdateWindow(mWindow);
      }

    //----------

    // The BeginPaint function prepares the specified window for painting
    // and fills a PAINTSTRUCT structure with information about the painting
    //
    // The BeginPaint function automatically sets the clipping region of the device context
    // to exclude any area outside the update region. The update region is set by the InvalidateRect
    // or InvalidateRgn function and by the system after sizing, moving, creating, scrolling,
    // or any other operation that affects the client area. If the update region is marked for erasing,
    // BeginPaint sends a WM_ERASEBKGND message to the window.
    //
    // An application should not call BeginPaint except in response to a WM_PAINT message

    virtual void beginPaint(void)
      {
        /*mPaintDC = */BeginPaint(m_Window,&m_PS);
      }

    //----------

    virtual void endPaint(void)
      {
        EndPaint(m_Window,&m_PS);
      }

    //----------------------------------------
    // hierarchy
    //----------------------------------------

    virtual void reparent(void* a_Parent)
      {
        m_Parent = (HWND)a_Parent;
        SetWindowLong(m_Window,GWL_STYLE,(GetWindowLong(m_Window,GWL_STYLE)&~WS_POPUP)|WS_CHILD);
        SetParent(m_Window, (HWND)a_Parent);
      }

    //----------------------------------------
    // events
    //----------------------------------------

    virtual void sendEvent(unsigned int aValue=0)
      {
      }

    //----------

    virtual void eventLoop(void)
      {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
      }

    //----------

    // [internal]
    int remapButton(int a_Button)
      {
        return a_Button;
      }

    //----------

    // [internal]
    int remapKey(int a_Key)
      {
        int ret = bu_None;
        if (a_Key & MK_SHIFT)       ret |= bu_Shift;
        if (a_Key & MK_CONTROL)     ret |= bu_Ctrl;
        if (GetKeyState(VK_MENU)<0) ret |= bu_Alt;
        return ret;
      }

    //----------------------------------------
    // event handler
    //----------------------------------------

    LRESULT eventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
      {
        LRESULT result = 0;
        h_Rect rc;
        int x,y,b,w,h;
        int left,top,right,bottom;

        switch (message)
        {

          case WM_SIZE:

            w = short(LOWORD(lParam));
            h = short(HIWORD(lParam));
            if (!m_Embedded)
            {
              w += m_AdjustWidth;
              h += m_AdjustHeight;
            }
            //if (w!=m_Rect.w || h!=m_Rect.h)
            //{
            //// hack: ignore this if there is other WM_SIZE messages in the queue
            //if ( PeekMessage(&msg2,mWindow,WM_SIZE,WM_SIZE,PM_NOREMOVE) )
            //{
            //  trace("there are oher WN_SIZE messages in the queue, so we're ignoring this one\n");
            //}
            //else
            //{
              //resizeBuffer(w,h);
              //if (m_Root) m_Root->do_SetSize(w,h);
              do_SetSize(w,h);
            //}
            result = 0;
            break;

          case WM_PAINT:

            beginPaint();
            left   = m_PS.rcPaint.left;
            top    = m_PS.rcPaint.top;
            right  = m_PS.rcPaint.right;
            bottom = m_PS.rcPaint.bottom;

            rc = h_Rect(  m_PS.rcPaint.left,
                          m_PS.rcPaint.top,
                          m_PS.rcPaint.right -  m_PS.rcPaint.left + 2,
                          m_PS.rcPaint.bottom - m_PS.rcPaint.top  + 2);
            ////m_Canvas->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
            //if ((m_WinFlags&AX_WIN_BUFFERED) && m_Surface )
            //{
            //  //m_SurfaceMutex.lock();
            //  h_Canvas* can = m_Surface->getCanvas();
            //  can->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
            //  doPaint(can,rc);
            //  //m_Canvas->drawImage(m_Surface,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);
            //  m_Canvas->drawSurface(m_Surface,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);
            //  can->clearClipRect();
            //  //m_SurfaceMutex.unlock();
            //}
            //else
            //{

            //  m_Painter->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());

              //TODO: blit from back-buffer
              // assume this is already updated by the widgets themselves
              //if (m_Root) m_Root->do_Paint(m_Painter,rc);
              do_Paint(m_Painter,rc);

            //  m_Painter->clearClipRect();

            //}
            ////m_Canvas->clearClipRect();
            endPaint();
            break;

          case WM_LBUTTONDOWN:
          case WM_RBUTTONDOWN:
          case WM_MBUTTONDOWN:

            x = short(LOWORD(lParam));
            y = short(HIWORD(lParam));
            switch (message)
            {
              case WM_LBUTTONDOWN: b = bu_Left;   break;
              case WM_MBUTTONDOWN: b = bu_Middle; break;
              case WM_RBUTTONDOWN: b = bu_Right;  break;
            }
            m_ClickedButton = b;
            //if (m_Root) m_Root->do_MouseDown(x,y,b,remapKey(wParam));
            do_MouseDown(x,y,b,remapKey(wParam));
            //if (m_CapturedWidget) grabCursor();
            break;

          case WM_LBUTTONUP:
          case WM_RBUTTONUP:
          case WM_MBUTTONUP:

            x = short(LOWORD(lParam));
            y = short(HIWORD(lParam));
            switch (message)
            {
              case WM_LBUTTONUP: b = bu_Left;   break;
              case WM_MBUTTONUP: b = bu_Middle; break;
              case WM_RBUTTONUP: b = bu_Right;  break;
            }
            m_ClickedButton = bu_None;
            //if (m_Root) m_Root->do_MouseUp(x,y,b,remapKey(wParam));
            do_MouseUp(x,y,b,remapKey(wParam));
            //if (!mCapturedWidget) releaseCursor();
            break;

          case WM_MOUSEMOVE:

            x = short(LOWORD(lParam));
            y = short(HIWORD(lParam));
            //if (m_Root) m_Root->do_MouseMove(x,y,/*m_ClickedButton|*/remapKey(wParam));
            do_MouseMove(x,y,/*m_ClickedButton|*/remapKey(wParam));
            break;

          //case WM_CHAR:
          //
          //  doKeyChar(wParam,lParam);
          //  break;

          case WM_KEYDOWN:

            //if (m_Root) m_Root->do_KeyDown(wParam,lParam);
            do_KeyDown(wParam,lParam);
            break;

          case WM_KEYUP:

            //if (m_Root) m_Root->do_KeyDown(wParam,lParam);
            do_KeyDown(wParam,lParam);
            break;

          case WM_SETCURSOR:

            if (LOWORD(lParam)==HTCLIENT)
            {
              SetCursor(m_WinCursor);
              result = 1;
            }
            else result = DefWindowProc(hWnd,message,wParam,lParam);
            break;

          case WM_TIMER:

            if (wParam==667) /*if (m_Root) m_Root->*/do_Timer();
            result = 0;
            break;

          case WM_GETDLGCODE:

            result = DLGC_WANTALLKEYS;
            break;

          /*
          note:
            one issue here is that, there is no know way to pause the
            console for a while before closing. this way the windows user
            could see the last bit of messages such as free() and other
            'exit' related ones.
            a log file debug method might be needed on windows after all..
          */

          case WM_DESTROY:

            if (!m_Embedded) PostQuitMessage(0);
            break;

          default:

            result = DefWindowProc(hWnd,message,wParam,lParam);
            break;

        }
        return result;
      }

};

//----------------------------------------------------------------------

typedef h_Window_Win32 h_Window;

//----------------------------------------------------------------------
//
// event_proc
//
//----------------------------------------------------------------------

LRESULT CALLBACK h_eventproc_win32(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  h_Window_Win32* wnd = (h_Window_Win32*)GetWindowLong(hWnd,GWL_USERDATA);
	if (wnd==0) return DefWindowProc(hWnd,message,wParam,lParam);
  return wnd->eventHandler(hWnd, message, wParam, lParam);
}

//----------------------------------------------------------------------
#endif





//    //internal
//    void getWindowSize(HWND pWnd, int* pW, int* pH)
//      {
//        if (pWnd)
//        {
//          RECT r;
//          GetWindowRect(pWnd, &r);
//          *pW = r.right - r.left;
//          *pH = r.bottom - r.top;
//        }
//        else
//        {
//          *pW = *pH = 0;
//        }
//      }
//
//    //----------
//
//    //internal
//    bool isChildWindow(HWND pWnd)
//      {
//        if (pWnd)
//        {
//          int style = GetWindowLong(pWnd, GWL_STYLE);
//          int exStyle = GetWindowLong(pWnd, GWL_EXSTYLE);
//          return ((style & WS_CHILD) && !(exStyle & WS_EX_MDICHILD));
//        }
//        return false;
//      }

//    //----------
//
//    #define SETPOS_FLAGS SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE
//
//    virtual void setParentSize(int aWidth, int aHeight)
//      {
//        int dw = aWidth - mRect.w, dh = aHeight - mRect.h;
//        HWND pParent = 0, pGrandparent = 0;
//        int w=0, h=0, parentW=0, parentH=0, grandparentW=0, grandparentH=0;
//        getWindowSize(mWindow,&w,&h);
//        if (isChildWindow(mWindow))
//        {
//          pParent = GetParent(mWindow);
//          getWindowSize(pParent, &parentW, &parentH);
//          if (isChildWindow(pParent))
//          {
//            pGrandparent = GetParent(pParent);
//            getWindowSize(pGrandparent, &grandparentW, &grandparentH);
//          }
//        }
//        SetWindowPos(mWindow, 0, 0, 0, w + dw, h + dh, SETPOS_FLAGS);
//        if (pParent)
//        {
//          SetWindowPos(pParent, 0, 0, 0, parentW + dw, parentH + dh, SETPOS_FLAGS);
//        }
//        if (pGrandparent)
//        {
//          SetWindowPos(pGrandparent, 0, 0, 0, grandparentW + dw, grandparentH + dh, SETPOS_FLAGS);
//        }
//      }
//
//    #undef SETPOS_FLAGS
//
//    //----------
//
//    virtual void resizeBuffer(int aWidth, int aHeight)
//      {
//        //trace("axWindowWin32.resizeBuffer: " << aWidth << "," << aHeight);
//        //if( aWidth!=mRect.w || aHeight!=mRect.h )
//        //{
//          if (mWinFlags&AX_WIN_BUFFERED)
//          {
//            //mSurfaceMutex.lock();
//            axSurface* srf;
//            if (mSurface)
//            {
//              srf = mSurface;
//              mSurface = NULL;
//              delete srf;
//            }
//            //srf = new axSurface(aWidth,aHeight/*,mWinFlags*/);
//            srf = createSurface(aWidth,aHeight,32);
//            mSurface = srf;
//            //mSurfaceMutex.unlock();
//          }
//          //mRect.w = aWidth;
//          //mRect.h = aHeight;
//          //doSetSize(aWidth,aHeight);
//        //} //newsize
//      }



