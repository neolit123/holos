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

class h_Window_Win32 : public h_Window_Base
{

  private:
    HWND        m_Parent;
    HINSTANCE   m_Instance;
    char*       m_ClassName;
    bool        m_Embedded;
    HWND        m_Window;
    int         m_AdjustWidth, m_AdjustHeight;
    PAINTSTRUCT m_PS;
    int         m_ClickedButton;
    HCURSOR     m_WinCursor;
    int         m_PrevCursor;
  protected:
    int         m_Width, m_Height;

  public:

    h_Window_Win32(int a_Width, int a_Height, void* a_Parent)
    : h_Window_Base()
      {
        m_Width         = a_Width;
        m_Height        = a_Height;
        m_Parent        = (HWND)a_Parent;
        m_WinCursor     = LoadCursor(NULL,IDC_ARROW);
        m_PrevCursor    = 0;
        m_ClickedButton = bu_None;
        m_WinCursor     = LoadCursor(NULL,IDC_ARROW);
        m_PrevCursor    = 0;

        m_Instance      = static_Core.getWinInstance();
        m_ClassName     = static_Core.getWinClassName();

        RECT rc = {0,0,a_Width-1,a_Height-1}; // left, top, right, bottom
        // --- create window ---
        if (m_Parent)
        // embedded ---
        {
          m_Embedded = true;;
          AdjustWindowRectEx(&rc,WS_POPUP,FALSE,WS_EX_TOOLWINDOW);
          m_Window = CreateWindowEx(
            WS_EX_TOOLWINDOW,
            m_ClassName,
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
        } //embedded
        else
        // windowed ---
        {
          m_Embedded = false;
          AdjustWindowRectEx(&rc,WS_OVERLAPPEDWINDOW,FALSE,WS_EX_OVERLAPPEDWINDOW);
          const unsigned int wPosX = ((GetSystemMetrics(SM_CXSCREEN)-a_Width)>>1) + rc.left;
          const unsigned int wPosY = ((GetSystemMetrics(SM_CYSCREEN)-a_Height)>>1) + rc.top;
          m_Window = CreateWindowEx(
            WS_EX_OVERLAPPEDWINDOW,   // dwExStyle
            m_ClassName,              // lpClassName
            m_ClassName, /*0,*/       // lpWindowName
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
        } // windowed
        m_AdjustWidth = (rc.right - rc.left + 1) - m_Width;
        m_AdjustHeight = (rc.bottom - rc.top + 1) - m_Height;
        SetWindowLong(m_Window,GWL_USERDATA,(int)this);
      }

    //----------

    virtual ~h_Window_Win32()
      {
        DestroyWindow(m_Window);
      }

    //----------------------------------------
    //
    //----------------------------------------

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

    //----------------------------------------
    //----------------------------------------

    virtual void reparent(void* a_Parent)
      {
        m_Parent = (HWND)a_Parent;
        SetWindowLong(m_Window,GWL_STYLE,(GetWindowLong(m_Window,GWL_STYLE)&~WS_POPUP)|WS_CHILD);
        SetParent(m_Window, (HWND)a_Parent);
      }

    //----------------------------------------
    //----------------------------------------

    void beginPaint(void)
      {
        /*m_PaintDC = */
        BeginPaint(m_Window,&m_PS);
      }

    //----------

    void endPaint(void)
      {
        EndPaint(m_Window,&m_PS);
      }

    //----------------------------------------
    // events
    //----------------------------------------

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

    LRESULT eventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
      {
        LRESULT result = 0;
        //axRect rc;
        int x,y,b,w,h;
        int left,top,right,bottom;

        switch (message)
        {

          case WM_SIZE:
            w = short(LOWORD(lParam));
            h = short(HIWORD(lParam));
            //resizeBuffer(w,h);
            //doSetSize(w,h);
            //result = 0;
            break;

          case WM_PAINT:
            beginPaint();
            left   = m_PS.rcPaint.left;
            top    = m_PS.rcPaint.top;
            right  = m_PS.rcPaint.right;
            bottom = m_PS.rcPaint.bottom;
            /*
            rc = axRect(  m_PS.rcPaint.left,
                          m_PS.rcPaint.top,
                          m_PS.rcPaint.right -  m_PS.rcPaint.left + 2,
                          m_PS.rcPaint.bottom - m_PS.rcPaint.top  + 2);
            //m_Canvas->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
            if ((m_WinFlags&AX_WIN_BUFFERED) && m_Surface )
            {
              //m_SurfaceMutex.lock();
              h_Canvas* can = m_Surface->getCanvas();
              can->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
              doPaint(can,rc);
              //m_Canvas->drawImage(m_Surface,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);
              m_Canvas->drawSurface(m_Surface,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);
              can->clearClipRect();
              //m_SurfaceMutex.unlock();
            }
            else
            {
              m_Canvas->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
              doPaint(m_Canvas,rc);
              m_Canvas->clearClipRect();
            }
            //m_Canvas->clearClipRect();
            */
            endPaint();
            break;

          case WM_LBUTTONDOWN:
          case WM_RBUTTONDOWN:
          case WM_MBUTTONDOWN:
            x = short(LOWORD(lParam));
            y = short(HIWORD(lParam));
            switch (message)
            {
              case WM_LBUTTONDOWN: b = 1/*bu_Left*/;   break;
              case WM_MBUTTONDOWN: b = 2/*bu_Middle*/; break;
              case WM_RBUTTONDOWN: b = 4/*bu_Right*/;  break;
            }
            m_ClickedButton = b;
            //do_MouseDown(short(LOWORD(lParam)), short(HIWORD(lParam)), btn | remapKey(wParam));
            //if (m_CapturedWidget) grabCursor();
            break;

          case WM_LBUTTONUP:
          case WM_RBUTTONUP:
          case WM_MBUTTONUP:
            x = short(LOWORD(lParam));
            y = short(HIWORD(lParam));
            switch (message)
            {
              case WM_LBUTTONUP: b = 1/*bu_Left*/;   break;
              case WM_MBUTTONUP: b = 2/*bu_Middle*/; break;
              case WM_RBUTTONUP: b = 4/*bu_Right*/;  break;
            }
            m_ClickedButton = 0;//bu_None;
            //doMouseUp(short(LOWORD(lParam)), short(HIWORD(lParam)), btn | remapKey(wParam));
            //if (!mCapturedWidget) releaseCursor();
            break;

          case WM_MOUSEMOVE:
            x = short(LOWORD(lParam));
            y = short(HIWORD(lParam));
            //doMouseMove(short(LOWORD(lParam)), short(HIWORD(lParam)), mClickedButton | remapKey(wParam));
            break;

          //case WM_CHAR:
          //  doKeyChar(wParam,lParam);
          //  break;

          case WM_KEYDOWN:
            //doKeyDown(wParam,lParam);
            break;

          case WM_KEYUP:
            //doKeyDown(wParam,lParam);
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
            //if (wParam==667) doTimer();
            //result = 0;
            break;

          case WM_GETDLGCODE:
            result = DLGC_WANTALLKEYS;
            break;

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

typedef h_Window_Win32 h_Window_Impl;

//----------------------------------------------------------------------
//
// eventProc
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































































//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//#if 0
//
//
////----------------------------------------------------------------------
//
//
//
//
////----------------------------------------------------------------------
//
//class axWindowWin32 : public axWindowBase
//{
//  private:
//    HINSTANCE   mInstance;
//    HWND        mWindow;
//    axString    mWinName;
//    PAINTSTRUCT mPS;
//    //int mWinCursor,mPrevCursor;
//    //HCURSOR mWinCursor;
//    HCURSOR     mWinCursor;
//    int         mPrevCursor;
//    int         mClickedButton;
//    int         mParent;
//    int         mAdjustWidth, mAdjustHeight;
//
//  public:
//    axWindowWin32(axContext* aContext, axRect aRect, int aWinFlags)
//    : axWindowBase(aContext,aRect,aWinFlags)
//      {
//        //trace("axWindowWin32.constructor()");
//        mInstance   = aContext->mInstance;
//        mWinName    = aContext->mWinClassName;
//        mParent     = (int)aContext->mWindow;
//        mWinCursor  = LoadCursor(NULL,IDC_ARROW);
//        mPrevCursor = 0;
//        //trace(mWinName.ptr());
//        mClickedButton = bu_None;
//        //mAdjustWidth = 0;
//        //mAdjustHeight = 0;
//
//        // --- register window class ---
//
//        //http://msdn.microsoft.com/en-us/library/ms633586%28v=VS.85%29.aspx
//        //RegisterClass:
//        //  All window classes that an application registers are unregistered when it terminates.
//        //  Windows NT/2000/XP: No window classes registered by a DLL are unregistered when the DLL is unloaded.
//        //  A DLL must explicitly unregister its classes when it is unloaded.
//
//        // multiple instances:
//        // what happens when we try to register a window with similar name
//        // and shouldn't we unregister it?
//        // (or is it done automatically when dll is unloaded?)
//
//        char* classname = mWinName.ptr();//(char*)"axonlib";
//        //trace("window class name:" << classname);
//        WNDCLASS wc;
//        memset(&wc,0,sizeof(wc));
//        wc.style          = CS_HREDRAW | CS_VREDRAW;
//        wc.lpfnWndProc    = &eventProc;
//        wc.hInstance      = mInstance;
//        wc.lpszClassName  = classname;
//        wc.hCursor        = (HICON)mWinCursor; //LoadCursor(NULL, IDC_ARROW);
//        // rc_default.rc: axicon ICON "rc_axlogo.ico"
//        HICON hIcon = LoadIcon(mInstance, "axicon");
//        if (hIcon) wc.hIcon = hIcon;
//
//        RegisterClass(&wc);
//
//        // The AdjustWindowRectEx function calculates the required size of the
//        // window rectangle, based on the desired client-rectangle size.
//        // The window rectangle can then be passed to the CreateWindow function
//        // to create a window whose client area is the desired size.
//        // To specify an extended window style, use the AdjustWindowRectEx function.
//        //
//        // BOOL AdjustWindowRectEx(
//        //   __inout  LPRECT lpRect,
//        //   __in     DWORD dwStyle,
//        //   __in     BOOL bMenu,
//        //   __in     DWORD dwExStyle
//        // );
//        //
//        // lpRect: Pointer to a RECT  structure that contains the coordinates of
//        // the top-left and bottom-right corners of the desired client area.
//        // When the function returns, the structure contains the coordinates of
//        // the top-left and bottom-right corners of the window to accommodate
//        // the desired client area.
//        // dwStyle: Specifies the window style of the window whose required size
//        // is to be calculated. Note that you cannot specify the WS_OVERLAPPED style.
//        // bMenu: Specifies whether the window has a menu.
//        // dwExStyle: Specifies the extended window style of the window whose
//        // required size is to be calculated. For more information, see CreateWindowEx.
//
//        // --- adjust window size ---
//
//        // Remarks
//        // By convention, the right and bottom edges of the rectangle are normally
//        // considered exclusive. In other words, the pixel whose coordinates are
//        // (right,bottom) lies immediately outside of the the rectangle.
//        // For example, when RECT is passed to the FillRect function, the rectangle
//        // is filled up to, but not including, the right column and bottom row of pixels.
//
//        //trace("width/height: " << mRect.w << "," << mRect.h);
//        RECT rc = { mRect.x, mRect.y, mRect.x2(), mRect.y2() }; // left, top, right, bottom
//        //trace("RECT rc: " << rc.left << "," << rc.top << " : " << rc.right << "," << rc.bottom);
//
////        //RECT rc = {mRect.x,mRect.y,mRect.x2(),mRect.y2()};
////        RECT rc = { mRect.x, mRect.y, mRect.w, mRect.h };
////        // get w, h
////        const u32 wWidth = (rc.right - rc.left - 1);  // -1 reduces the window dim by 1 px
////        const u32 wHeight = (rc.bottom - rc.top - 1);
////        #ifdef AX_FORMAT_EXE
////          // adjust rect for exe
////          AdjustWindowRect(&rc,WS_OVERLAPPEDWINDOW|WS_POPUP,FALSE);
////          // get screen w, h and define a center pos
////          const u32 wPosX = ((GetSystemMetrics(SM_CXSCREEN)-wWidth)>>1) + rc.left;
////          const u32 wPosY = ((GetSystemMetrics(SM_CYSCREEN)-wHeight)>>1) + rc.top;
////        #endif
////        #ifdef AX_FORMAT_VST
////          // no centering for vst
////          const u32 wPosX = rc.left;
////          const u32 wPosY = rc.top;
////        #endif
//
//        // --- create window ---
//
//        if (mWinFlags&AX_WIN_EMBEDDED) // embedded ---
//        {
//          AdjustWindowRectEx(&rc,/*WS_OVERLAPPEDWINDOW|*/WS_POPUP,FALSE,WS_EX_TOOLWINDOW);
//          //trace("adjusted rc (embedded): " << rc.left << "," << rc.top << " : " << rc.right << "," << rc.bottom);
//          mWindow = CreateWindowEx(
//            WS_EX_TOOLWINDOW,
//            classname,
//            0,
//            WS_POPUP,
//            rc.left,//wPosX,          // center x
//            rc.top,//wPosY,           // center y
//            rc.right-rc.left+1,         //wWidth,
//            rc.bottom-rc.top+1,         //wHeight,
//            0,                        //(HWND)mParent,//0,
//            0,
//            mInstance,
//            0
//          );
//          reparent(mParent);
//        } //embedded
//
//        else // windowed ---
//        {
//          AdjustWindowRectEx(&rc,WS_OVERLAPPEDWINDOW,FALSE,WS_EX_OVERLAPPEDWINDOW);
//          //trace("adjusted rc (windowed): " << rc.left << "," << rc.top << " : " << rc.right << "," << rc.bottom);
//          const unsigned int wPosX =
//            ((GetSystemMetrics(SM_CXSCREEN)-mRect.w)>>1) + rc.left;
//          const unsigned int wPosY =
//            ((GetSystemMetrics(SM_CYSCREEN)-mRect.h)>>1) + rc.top;
//          mWindow = CreateWindowEx(
//            WS_EX_OVERLAPPEDWINDOW,   // dwExStyle
//            classname,                // lpClassName
//            0,                        // lpWindowName
//            WS_OVERLAPPEDWINDOW,      // dwStyle
//            wPosX,                    // center x
//            wPosY,                    // center y
//            rc.right-rc.left+1,       // wWidth,
//            rc.bottom-rc.top+1,       // wHeight,
//            0,                        // hWndParent
//            0,                        // hMenu
//            mInstance,                // hInstance
//            0                         // lpParam
//          );
//          SetFocus(mWindow);
//        }
//
//        mAdjustWidth = (rc.right - rc.left + 1) - mRect.w;
//        mAdjustHeight = (rc.bottom - rc.top + 1) - mRect.h;
//        //trace("mAdjustWidth: " << mAdjustWidth << ", mAdjustHeight: " << mAdjustHeight);
//
//        //mRect.x = rc.left;
//        //mRect.y = rc.top;
//        //mRect.w = (rc.right-rc.left);
//        //mRect.h = (rc.bottom-rc.top);
//
//        // ---
//
//        SetWindowLong(mWindow,GWL_USERDATA,(int)this);
//        //DragAcceptFiles(mWindow,true);
//        mCanvas = createCanvas();
//        if (aWinFlags & AX_WIN_BUFFERED) mSurface = createSurface(mRect.w,mRect.h,32);
//
//      }
//
//    //----------------------------------------
//
//    ~axWindowWin32()
//      {
//        //if (mWinCursor>=0) XFreeCursor(mDisplay,mWinCursor);
//        DestroyWindow(mWindow);
//        if (mCanvas) delete mCanvas;
//        if (mSurface) delete mSurface;
//
//        // can this be dangerous if moutlple plugin instances uses the plugin?
//        // or is there some reference-counting going on?
//        //http://msdn.microsoft.com/en-us/library/ms644899%28v=VS.85%29.aspx
//        //UnregisterClass:
//        //  If the function succeeds, the return value is nonzero.
//        //  If the class could not be found or if a window still exists
//        //  that was created with the class, the return value is zero
//        // unregister window?
//        // what if multiple instances is using the same window?
//        UnregisterClass( mWinName.ptr(), mInstance);
//      }
//
//    //----------------------------------------
//
//    //virtual int getHandle(void) { return (int)mWindow; }
//
//    virtual axCanvas* createCanvas(void)
//      {
//        axContext ctx(mWindow);
//        return new axCanvas(&ctx);
//      }
//
//    //----------
//
//    virtual axSurface* createSurface(int aWidth, int aHeight, int aDepth)
//      {
//        //axContext ctx(mParent);
//        axContext ctx(mWindow);
//        return new axSurface(&ctx,aWidth,aHeight, aDepth);
//      }
//
//    //----------
//
//    virtual axBitmap* createBitmap(int aWidth, int aHeight, int aDepth)
//      {
//        axContext ctx(mWindow);
//        return new axBitmap(&ctx,aWidth,aHeight,aDepth);
//      }
//
//
//    //----------------------------------------
//    // low level
//    //----------------------------------------
//
//    //virtual void flush(void)
//    //  {
//    //    //GdiFlush();
//    //  }
//
//    //----------
//
//    //virtual void sync(void)
//    //  {
//    //  }
//
//    //----------
//
//    //virtual void lock(void)
//    //  {
//    //  }
//
//    //----------
//
//    //virtual void unlock(void)
//    //  {
//    //  }
//
//    //----------
//
//    virtual void show(void)
//      {
//        //trace("show");
//        //SetWindowPos(mWindow,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
//        ShowWindow(mWindow,SW_SHOW);
//      }
//
//    //----------
//
//    virtual void hide(void)
//      {
//        //trace("hide");
//        ShowWindow(mWindow,SW_HIDE);
//      }
//
//    //----------
//
//    virtual void setPos(int aX, int aY)
//      {
//        SetWindowPos(mWindow,0,aX,aY,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
//      }
//
//    //----------
//
//    virtual void setSize(int aWidth, int aHeight)
//      {
//        //trace("setSize: " << aWidth << "," << aHeight);
//        int w = aWidth + mAdjustWidth + 0;
//        int h = aHeight + mAdjustHeight + 0;
//        SetWindowPos(mWindow,HWND_TOP,0,0,w,h, SWP_NOMOVE);
//        //SetWindowPos(mWindow,0,0,0,aWidth,aHeight,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
//      }
//
//    //------------------------------
//
//      //internal
//      void getWindowSize(HWND pWnd, int* pW, int* pH)
//      {
//          if (pWnd)
//          {
//            RECT r;
//            GetWindowRect(pWnd, &r);
//            *pW = r.right - r.left;
//            *pH = r.bottom - r.top;
//          }
//          else
//          {
//            *pW = *pH = 0;
//          }
//        }
//
//      //----------
//
//      //internal
//      bool isChildWindow(HWND pWnd)
//        {
//          if (pWnd)
//          {
//            int style = GetWindowLong(pWnd, GWL_STYLE);
//            int exStyle = GetWindowLong(pWnd, GWL_EXSTYLE);
//            return ((style & WS_CHILD) && !(exStyle & WS_EX_MDICHILD));
//          }
//          return false;
//        }
//
//    //------------------------------
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
//    virtual void setTitle(axString aTitle)
//      {
//        SetWindowText(mWindow, aTitle.ptr());
//      }
//
//    //----------
//
//    virtual void reparent(int aParent)
//      {
//        //trace("reparent");
//        mParent = aParent;
//        SetWindowLong(mWindow,GWL_STYLE,(GetWindowLong(mWindow,GWL_STYLE)&~WS_POPUP)|WS_CHILD);
//        SetParent(mWindow, (HWND)aParent);
//      }
//
//    //virtual void resetCursor(void)
//    //  {
//    //  }
//
//    //----------
//
//    virtual void setCursor(int aCursor)
//      {
//        if (aCursor<0)
//        {
//          hideCursor();// aCursor = DEF_CURSOR;
//          mPrevCursor = aCursor;
//        } //-1
//        else
//        {
//          if (mPrevCursor<0) showCursor();
//          if (aCursor!=mPrevCursor)
//          {
//            mWinCursor = LoadCursor(NULL,(char*)aCursor);
//            SetCursor( mWinCursor );
//            mPrevCursor = aCursor;
//          }
//        } //>0
//      }
//
//    //----------
//
//    virtual void setCursorPos(int aX, int aY)
//      {
//        SetCursorPos(aX,aY);
//      }
//
//    //----------
//
//    virtual void showCursor(void)
//      {
//        ShowCursor(true);
//      }
//
//    //----------
//
//    virtual void hideCursor(void)
//      {
//        ShowCursor(false);
//      }
//
//    //----------
//
//    virtual void grabCursor(void)
//      {
//        SetCapture(mWindow);
//      }
//
//    //----------
//
//    virtual void releaseCursor(void)
//      {
//        ReleaseCapture();
//      }
//
//    //----------------------------------------
//    // medium level
//    //----------------------------------------
//
//    // The invalidated areas accumulate in the update region until the region is processed when the next
//    // WM_PAINT message occurs or until the region is validated by using the ValidateRect or ValidateRgn function.
//    // The system sends a WM_PAINT message to a window whenever its update region is not empty
//    // and there are no other messages in the application queue for that window.
//
//    virtual void invalidate(int aX, int aY, int aW, int aH)
//      {
//        RECT R;
//        R.left   = aX;
//        R.top    = aY;
//        R.right  = aX + aW - 1;
//        R.bottom = aY + aH - 1;
//        InvalidateRect(mWindow,&R,false);
//        //UpdateWindow(mWindow);
//      }
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
//
//    //----------
//
//    // The BeginPaint function prepares the specified window for painting
//    // and fills a PAINTSTRUCT structure with information about the painting
//    //
//    //
//    // The BeginPaint function automatically sets the clipping region of the device context
//    // to exclude any area outside the update region. The update region is set by the InvalidateRect
//    // or InvalidateRgn function and by the system after sizing, moving, creating, scrolling,
//    // or any other operation that affects the client area. If the update region is marked for erasing,
//    // BeginPaint sends a WM_ERASEBKGND message to the window.
//    //
//    // An application should not call BeginPaint except in response to a WM_PAINT message
//
//    virtual void beginPaint(void)
//      {
//        //wtrace("axWindowWin32.beginPaint");
//        /*mPaintDC = */BeginPaint(mWindow,&mPS);
//      }
//
//    //----------
//
//    virtual void endPaint(void)
//      {
//        EndPaint(mWindow,&mPS);
//        //wtrace("axWindowWin32.endPaint");
//      }
//
//    //----------------------------------------
//    //
//    //----------------------------------------
//
//    virtual void startTimer(int ms)
//      {
//        /*mTimer = */SetTimer(mWindow,667,ms,NULL/*timerProc*/);
//      }
//
//    //----------
//
//    virtual void stopTimer(void)
//      {
//        KillTimer(mWindow,667);
//      }
//
//    //----------------------------------------
//    // events
//    //----------------------------------------
//
//    virtual void sendEvent(unsigned int aValue=0)
//    {
//    }
//
//    //----------------------------------------
//    //
//    //----------------------------------------
//
//    virtual void eventLoop(void)
//      {
//        MSG msg;
//        while (GetMessage(&msg, NULL, 0, 0))
//        {
//          TranslateMessage(&msg);
//          DispatchMessage(&msg);
//        }
//      }
//
//    //----------------------------------------
//    //
//    //----------------------------------------
//
////#define bu_None    0
////#define bu_Left    1
////#define bu_Right   2
////#define bu_Middle  4
////#define bu_Shift   8
////#define bu_Ctrl    16
////#define bu_Alt     32
//
//    int remapButton(int aButton)
//      {
//        return aButton;
//      }
//
//    //----------
//
//    int remapKey(int aKey)
//      {
//        int ret = bu_None;
//        if (aKey & MK_SHIFT)        ret |= bu_Shift;
//        if (aKey & MK_CONTROL)      ret |= bu_Ctrl;
//        if (GetKeyState(VK_MENU)<0) ret |= bu_Alt;
//        return ret;
//      }
//
//      //------------------------------
//
//    LRESULT eventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//      {
//        //MSG msg2;
//        //HDC dc;
//        LRESULT result = 0;
//        int btn = 0;
//        axRect rc;
//        int w,h;
//        int left,top,right,bottom;
//
//        //trace("win32 eventHandler. msg=%x\n",message);
//
//        switch (message)
//        {
//          case WM_GETDLGCODE:
//            result = DLGC_WANTALLKEYS;
//            break;
//          case WM_PAINT:
//            //wtrace("WM_PAINT\n");
//            beginPaint();
//            left   = mPS.rcPaint.left;
//            top    = mPS.rcPaint.top;
//            right  = mPS.rcPaint.right;
//            bottom = mPS.rcPaint.bottom;
//            //trace("WM_PAINT. RECT: " << left << "," << top << " : " << right << "," << bottom);
//            rc = axRect(  mPS.rcPaint.left,
//                          mPS.rcPaint.top,
//                          mPS.rcPaint.right -  mPS.rcPaint.left + 2,
//                          mPS.rcPaint.bottom - mPS.rcPaint.top  + 2);
//            //mCanvas->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
//            if ((mWinFlags&AX_WIN_BUFFERED) && mSurface )
//            {
//              //mSurfaceMutex.lock();
//              axCanvas* can = mSurface->getCanvas();
//              can->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
//              doPaint(can,rc);
//              //mCanvas->drawImage(mSurface,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);
//              mCanvas->drawSurface(mSurface,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);
//              can->clearClipRect();
//              //mSurfaceMutex.unlock();
//            }
//            else
//            {
//              // http://msdn.microsoft.com/en-us/library/dd183439%28VS.85%29.aspx
//              // if you obtain a device context handle from the  BeginPaint function,
//              // the DC contains a predefined rectangular clipping region that corresponds
//              // to the invalid rectangle that requires repainting
//              mCanvas->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
//              doPaint(mCanvas,rc);
//              mCanvas->clearClipRect();
//            }
//            //mCanvas->clearClipRect();
//            endPaint();
//            break;
//          case WM_LBUTTONDOWN: case WM_RBUTTONDOWN: case WM_MBUTTONDOWN:
//            switch (message)
//            {
//              case WM_LBUTTONDOWN: btn = bu_Left;   break;
//              case WM_MBUTTONDOWN: btn = bu_Middle; break;
//              case WM_RBUTTONDOWN: btn = bu_Right;  break;
//            }
//            mClickedButton = btn;
//            doMouseDown(short(LOWORD(lParam)), short(HIWORD(lParam)), btn | remapKey(wParam));
//            if (mCapturedWidget) grabCursor();
//            break;
//          case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP:
//            switch (message)
//            {
//              case WM_LBUTTONUP: btn = bu_Left;   break;
//              case WM_MBUTTONUP: btn = bu_Middle; break;
//              case WM_RBUTTONUP: btn = bu_Right;  break;
//            }
//            mClickedButton = bu_None;
//            doMouseUp(short(LOWORD(lParam)), short(HIWORD(lParam)), btn | remapKey(wParam));
//            if (!mCapturedWidget) releaseCursor();
//            break;
//          case WM_MOUSEMOVE:
//            doMouseMove(short(LOWORD(lParam)), short(HIWORD(lParam)), mClickedButton | remapKey(wParam));
//            //return 1;
//            break;
//          //case WM_CHAR:
//          //  doKeyChar(wParam,lParam);
//          //  break;
//          case WM_KEYDOWN:
//            doKeyDown(wParam,lParam);
//            break;
//          case WM_KEYUP:
//            doKeyDown(wParam,lParam);
//            break;
//          case WM_SIZE:
//            //trace("WM_SIZE\n");
//            //lParam:
//            //  The low-order word of lParam specifies the new width of the client area.
//            //  The high-order word of lParam specifies the new height of the client area.
//            //TODO: resize surface, if any
//            //int x = ev->xconfigure.x;
//            //int y = ev->xconfigure.y;
//            w = short(LOWORD(lParam));
//            h = short(HIWORD(lParam));
//            //trace("WM_SIZE: " << w << "," << h);
//
//            //if (!(mWinFlags&AX_WIN_EMBEDDED))
//            //{
//            //  w += mAdjustWidth;
//            //  h += mAdjustHeight;
//            //}
//
//            //if (w!=mRect.w || h!=mRect.h)
//            //{
//            //// hack: ignore this if there is other WM_SIZE messages in the queue
//            //if ( PeekMessage(&msg2,mWindow,WM_SIZE,WM_SIZE,PM_NOREMOVE) )
//            //{
//            //  trace("there are oher WN_SIZE messages in the queue, so we're ignoring this one\n");
//            //}
//            //else
//            //{
//              //flush();
//              resizeBuffer(w,h);
//              doSetSize(w,h);
//            //}
//            //}
//            result = 0;
//            break;
//          case WM_DESTROY:
//            if (!(mWinFlags&AX_WIN_EMBEDDED))
//            {
//              //trace("Quit");
//              /*
//              note:
//                one issue here is that, there is no know way to pause the
//                console for a while before closing. this way the windows user
//                could see the last bit of messages such as free() and other
//                'exit' related ones.
//                a log file debug method might be needed on windows after all..
//              */
//              PostQuitMessage(0);
//            }
//            break;
//          case WM_TIMER:
//            //wtrace("timer " << wParam);
//            if (wParam==667)
//            {
//              //wtrace("axWIndowWin32.eventHandler :: timer");
//              doTimer();
//            }
//            result = 0;
//            break;
//          case WM_SETCURSOR:
//            //wtrace("WM_SETCURSOR");
//            if (LOWORD(lParam)==HTCLIENT)
//            {
//              SetCursor(mWinCursor);
//              result = 1;
//            }
//            else result = DefWindowProc(hWnd,message,wParam,lParam);
//            break;
//          default:
//            result = DefWindowProc(hWnd,message,wParam,lParam);
//        }
//        return result;
//      }
//
//};
//
//
//typedef axWindowWin32 axWindowImpl;
//
////----------------------------------------------------------------------
//#endif
