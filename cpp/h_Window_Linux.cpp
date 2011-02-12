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
#ifndef h_Window_Linux_cpp_included
#define h_Window_Linux_cpp_included
//----------------------------------------------------------------------

#include <GL/glx.h>
#include <GL/gl.h>
#include <pthread.h>
//#include <string.h>

#include "h/h_Widget.h"
#include "h/h_Painter.h"
#include "h/h_Skin.h"
//#include "h/h_Renderer.h"

void* h_linux_threadProc(void* data);
void* h_linux_timerProc(void* data);

//----------------------------------------------------------------------

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

// http://tronche.com/gui/x/xlib/appendix/b/
// mouse cursor shapes
#define cu_Arrow           2
#define cu_ArrowUp         114
#define cu_ArrowDown       106
#define cu_ArrowLeft       110
#define cu_ArrowRight      112
#define cu_ArrowUpDown     116
#define cu_ArrowLeftRight  108
#define cu_ArrowDiagLeft   12
#define cu_ArrowDiagRight  14
#define cu_Move            52
#define cu_Wait            150
#define cu_ArrowWait       26
#define cu_Hand            58
#define cu_Finger          60
#define cu_Cross           0
#define cu_Pencil          86
#define cu_Plus            90
#define cu_Question        99
#define cu_Ibeam           152

#define H_DEF_CURSOR    cu_Arrow
//#define H_DEF_CURSOR      -1

//----------------------------------------------------------------------

// thread state
#define ts_Kill  666
#define ts_Timer 667

//int h_winfbconfig_attr[] =
//  { GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT|GLX_PIXMAP_BIT|GLX_PBUFFER_BIT, None };

int h_window_attr[] =
{
  //GLX_PBUFFER_WIDTH,      256,
  //GLX_PBUFFER_HEIGHT,     256,
  //GLX_PRESERVED_CONTENTS, True,
  None
};

int h_window_gl_attr[] =
{
  GLX_X_RENDERABLE,   True,
  GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
  GLX_RENDER_TYPE,    GLX_RGBA_BIT,
  GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
  GLX_RED_SIZE,       8,
  GLX_GREEN_SIZE,     8,
  GLX_BLUE_SIZE,      8,
  GLX_ALPHA_SIZE,     8,
  GLX_DEPTH_SIZE,     24,
  GLX_STENCIL_SIZE,   8,
  GLX_DOUBLEBUFFER,   True,
  //GLX_SAMPLE_BUFFERS, 1,
  //GLX_SAMPLES,        4,
  None
};

// empty/blank mouse cursor
static char noData[] = { 0,0,0,0,0,0,0,0 };

//----------------------------------------------------------------------

// there's some weird problems with energy xt2, where sometimes, not always,
// the window comes up, but no painting is done. could be a case of xlib
// not having finished the window creation yet? (x11 is async)

static Bool h_WaitForNotify(Display* d, XEvent* e, char* arg)
  {
    return (e->type==MapNotify) & (e->xmap.window==(Window)arg);
  }

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//class h_Window : public h_Widget,
//                 //public h_WidgetListener,
//                 public h_PaintSource,
//                 public h_PaintTarget,
//                 public h_RenderTarget

class h_Window_Linux : public h_Widget,
                       public h_PaintSource
{

  friend void* h_linux_threadProc(void* data);
  friend void* h_linux_timerProc(void* data);
  //friend class axFormatVst;

  private:

    Display*    m_Display;
    Atom        m_DeleteWindowAtom;
    Atom        m_CustomEventAtom;
    Window      m_Parent;
    Window      m_Window;
    h_Painter*  m_Painter;
    #ifdef H_ALPHA
    Picture     m_Picture;
    #endif
    #ifdef H_OPENGL
    GLXWindow   m_GLXWindow;
    h_Renderer* m_Renderer;
    #endif
    pthread_t   m_EventThread;
    pthread_t   m_TimerThread;
    bool        m_TimerRunning;
    int         m_TimerSleep;
    int         m_CurrentCursor;
    Cursor      m_InvisibleCursor;
    Pixmap      m_BitmapNoData;
    XColor      m_Black;

    bool        m_Embedded;
    //int         m_Width;
    //int         m_Height;
    long        m_EventMask;
    int         m_Screen;
    Visual*     m_Visual;
    int         m_Depth;
    Colormap    m_Colormap;
    Window      m_Root;

  public:

    // accessors

    inline h_Painter* getPainter(void) { return m_Painter; }
    //inline h_Renderer* getRenderer(void) { return m_Renderer; }
    // h_PaintSource:
    virtual Drawable getSourceDrawable(void) { return m_Window; }
    #ifdef H_ALPHA
    virtual Picture  getSourcePicture(void)   { return m_Picture; }
    #endif
    // h_PaintTarget
    virtual Drawable getTargetDrawable(void) { return m_Window; }
    // h_RenderTarget
    #ifdef H_OPENGL
    virtual GLXDrawable getTargetGLXDrawable(void) { return m_GLXWindow; }
    #endif

  public:

    //----------------------------------------

    // h_Widget:

    //virtual void do_SetPos(int a_Xpos, int a_Ypos) {}
    //virtual void do_SetSize(int a_Width, int a_Height) {}
    //virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect) {}
    //virtual void do_MouseDown(int a_Xpos, int a_Ypos, int a_Button, int a_State) {}
    //virtual void do_MouseUp(int a_Xpos, int a_Ypos, int a_Button, int a_State) {}
    //virtual void do_MouseMove(int a_Xpos, int a_Ypos, int a_State) {}
    //virtual void do_KeyDown(int a_Ypos, int a_State) {}
    //virtual void do_KeyUp(int a_Ypos, int a_State) {}
    //virtual void do_Timer() {}

    // h_WidgetListener:

    //virtual void on_Change(h_Widget* a_Widget) {}

    //----------------------------------------

  public:

    // if a_Parent == H_NULL, a standalone window is created,
    // else a_Parent is the parent window (embedded)
    //

    h_Window_Linux(h_WidgetListener* a_Listener, h_Rect a_Rect, void* a_Parent)
    : h_Widget(a_Listener,a_Rect)
      {

        //m_Display = static_Core.m_Platform->m_WinDisplay;
        m_Display = static_Core.m_Platform->openDisplay();

        if (a_Parent)
        {
          m_Embedded = true;
          m_Parent = (Window)a_Parent;
        }
        else
        {
          m_Embedded = false;
          //m_Parent = static_Core.m_Platform->m_WinRoot;
          m_Parent = XDefaultRootWindow(m_Display);
        }

        //int screen = H_INTERFACE->getScreen();
        //trace("m_Display: " << m_Display);

//        #ifdef H_OPENGL

//          int fbcount;
//          GLXFBConfig *fbc = glXChooseFBConfig(m_Display,H_INTERFACE->getScreen(),h_window_gl_attr,&fbcount);
//          //dump_config(m_Display,fbc,0/*fbcount)*/);
//          XVisualInfo *vi = glXGetVisualFromFBConfig(m_Display,fbc[0]);
//          Visual*  visual    = vi->visual;
//          int      depth    = vi->depth;
//          Colormap colormap = XCreateColormap(m_Display,H_INTERFACE->getRoot(),visual,AllocNone);

//        #else
          m_Root      = XDefaultRootWindow(m_Display);
          m_Screen    = XDefaultScreen(m_Display);                // the default screen number referenced by the XOpenDisplay() function
          m_Visual    = XDefaultVisual(m_Display,m_Screen);
          m_Depth     = XDefaultDepth(m_Display,m_Screen);
          m_Colormap  = XCreateColormap(m_Display,m_Root,m_Visual,AllocNone);
//        #endif

        //Colormap colormap = XCreateColormap(m_Display,H_INTERFACE->getRoot(),visual,AllocNone);
        long m_WinEventMask = ExposureMask
                            | ButtonPressMask
                            | ButtonReleaseMask
                            | PointerMotionMask
                            | KeyPressMask
                            | KeyReleaseMask
                            | StructureNotifyMask
                            | PropertyChangeMask
                            | ClientMessage;

        XSetWindowAttributes  swa;
        swa.event_mask        = m_WinEventMask;//StructureNotifyMask;
        swa.background_pixmap = None;
        swa.border_pixel      = 0;
        swa.colormap          = m_Colormap;

//        swa.override_redirect = true;

        //CWBackPixmap, CWBackPixel
        //CWBorderPixmap, CWBorderPixel, CWBitGravity, CWWinGravity, CWBackingStore
        //CWBackingPlanes, CWBackingPixel, CWOverrideRedirect, CWSaveUnder,
        //CWDontPropagate, CWCursor

        // http://tronche.com/gui/x/xlib/window/attributes/

        unsigned long swa_mask  = CWEventMask
                                | CWBackPixmap
                                | CWBorderPixel
                                | CWColormap;
//                                | CWOverrideRedirect;

        m_Window = XCreateWindow(
          m_Display,
          m_Parent,//H_INTERFACE->getRoot(),
          a_Rect.x, a_Rect.y,
          a_Rect.w, a_Rect.h,//100,100,
          0, // border width
          m_Depth,//vi->depth,
          InputOutput,
          m_Visual,//vi->visual,
          swa_mask,//CWBorderPixel|CWColormap|CWEventMask,
          &swa
        );

        if (m_Embedded)
        {
          // --- remove title-bar, borders ---
          #define MWM_HINTS_DECORATIONS (1L << 1)
          #define PROP_MOTIF_WM_HINTS_ELEMENTS  5
          typedef struct
          {
            unsigned long flags;
            unsigned long functions;
            unsigned long decorations;
            long          inputMode;
            unsigned long status;
          } PropMotifWmHints;
          PropMotifWmHints motif_hints;
          motif_hints.flags = MWM_HINTS_DECORATIONS;
          motif_hints.decorations = 0;
          Atom prop = XInternAtom(m_Display,"_MOTIF_WM_HINTS",True);
          XChangeProperty(m_Display,m_Window,prop,prop,32,PropModeReplace,(unsigned char *)&motif_hints,PROP_MOTIF_WM_HINTS_ELEMENTS);
          #undef MWM_HINTS_DECORATIONS
          #undef PROP_MOTIF_WM_HINTS_ELEMENTS
        }
        else
        {
          // --- WM_DELETE_WINDOW ClientMessage ---
          m_DeleteWindowAtom = XInternAtom(m_Display,"WM_DELETE_WINDOW",True);
          XSetWMProtocols(m_Display,m_Window,&m_DeleteWindowAtom,1);
        }

        m_Painter  = new h_Painter(m_Display,m_Window);       /// ?????

        //#ifdef H_ALPHA
        //  XWindowAttributes winattralpha;
        //  XGetWindowAttributes(m_Display,m_Window,&winattralpha );
        //  XRenderPictFormat *format = XRenderFindVisualFormat(m_Display,winattralpha.visual);
        //  XRenderPictureAttributes pictattralpha;
        //  m_Picture = XRenderCreatePicture(m_Display,m_Window,format,None,&pictattralpha);
        //  m_Painter->setPicture( m_Picture );
        //  //bool hasAlpha  = ( format->type == PictTypeDirect && format->direct.alphaMask );
        //#endif

        //#ifdef H_OPENGL
        //  m_GLXWindow = glXCreateWindow(m_Display,fbc[0],m_Window,0);
        //  m_Renderer = new h_Renderer(this);
        //  //m_Renderer->setRenderTarget(this);
        //#endif

        // timer
        m_TimerRunning = false;
        m_TimerSleep = 30; // 30 ms between each timer signal

        // (invisible) mouse cursor
        m_BitmapNoData  = XCreateBitmapFromData(m_Display,m_Window,noData,8,8);
        m_CurrentCursor = -1;
        m_Black.red     = 0;
        m_Black.green   = 0;
        m_Black.blue    = 0;
        m_Black.flags   = (DoRed or DoGreen or DoBlue);
        XAllocColor(m_Display,m_Colormap,&m_Black);

        // event handler thread
        if (m_Embedded)
        {
          pthread_create(&m_EventThread,NULL,&h_linux_threadProc,this);
        }

      }

    //----------

    virtual ~h_Window_Linux()
      {
        if (m_Embedded)
        {
          sendEvent(ts_Kill);
          void* ret;
          pthread_join(m_EventThread,&ret);
        }
        if (m_TimerRunning)
        {
          m_TimerRunning = false;
          void* ret;
          pthread_join(m_TimerThread,&ret);
        }
        #ifdef H_OPENGL
          // delete glxwindow
          //XFree(fbc); // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
        #endif
        #ifdef H_ALPHA
          XRenderFreePicture(m_Display,m_Picture);
        #endif
        if (m_Painter) delete m_Painter;
        //if (m_Surface) delete m_Surface;
        XFreePixmap(m_Display, m_BitmapNoData);
        if (m_CurrentCursor>=0) XFreeCursor(m_Display,m_CurrentCursor);

        // http://tronche.com/gui/x/xlib/window/XDestroyWindow.html
        // The XDestroyWindow() function destroys the specified window as well as all of its subwindows
        // (whihc means that a window is automatically destroyed when a host closes the editor?)

        if (!m_Embedded) XDestroyWindow(m_Display,m_Window);

        static_Core.m_Platform->closeDisplay(m_Display);

      }

    //----------------------------------------
    //
    //----------------------------------------

    virtual void flush(void)
      {
        XFlush(m_Display);
      }

    //----------

    virtual void sync(void)
      {
        XSync(m_Display,false);
      }

    //----------

    virtual void lock(void)
      {
        XLockDisplay(m_Display);
      }

    //----------

    virtual void unlock(void)
      {
        XUnlockDisplay(m_Display);
      }

    //----------

    virtual void show(void)
      {
        XMapWindow(m_Display,m_Window);
        XEvent event;
        XIfEvent(m_Display,&event,h_WaitForNotify,(XPointer)m_Window);
        //XIfEvent(m_Display,&event,h_WaitForNotify,(char*)m_Window);
      }

    //----------

    virtual void hide(void)
      {
        XUnmapWindow(m_Display,m_Window);
      }

    //----------

    virtual void setPos(int a_Xpos, int a_Ypos)
      {
        XWindowChanges attr;
        attr.x = a_Xpos;
        attr.y = a_Ypos;
        XConfigureWindow(m_Display,m_Window,CWX|CWY,&attr);
        //XFlush(m_Display);
      }

    //----------

    virtual void setSize(int a_Width, int a_Height)
      {
        XResizeWindow(m_Display,m_Window,a_Width,a_Height);
        //XFlush(m_Display);
      }

    //----------

    // valgrind reports memory leak here ('definitely lost')
    // XStringListToTextProperty, malloc

    // lii:
    // there could be malloc() in XStringListToTextProperty(),
    // so 'free(window_title);' might be needed after XFlush(..); ?

    //-----

    // XStringListToTextProperty docs:
    // "To free the storage for the value field, use XFree()."
    // but should we do it right after XSetWMName, or do we need to wait
    // until the window closes..
    // see also: XSetWMName [name, icon,commandline,

    // i think i saw some other way of doing this somewhere,
    // will try to hunt it down...

    virtual void setTitle(char* a_Title)
      {
        //XTextProperty window_title_property;
        //char* window_title = aTitle.ptr();
        //XStringListToTextProperty(&window_title,1,&window_title_property);
        //XSetWMName(mDisplay,mWindow,&window_title_property);
        //XFlush(mDisplay);
      }

    //----------------------------------------
    //
    //----------------------------------------

    virtual void beginPaint(void)
      {
      }

    //----------

    virtual void endPaint(void)
      {
        // todo: check if actually needed...
        flush();
      }



    virtual void reparent(void* a_Parent)
      {
        m_Parent = (Window)a_Parent;
        XReparentWindow(m_Display,m_Window,m_Parent,0,0);
        //XFlush(m_Display);
      }

    //----------

    virtual void invalidate(int aX, int aY, int aW, int aH)
      {
        static XExposeEvent ev;
        ev.type     = Expose;
        ev.display  = m_Display;
        ev.window   = m_Window; // mParent;
        ev.x        = aX;
        ev.y        = aY;
        ev.width    = aW;
        ev.height   = aH;
        ev.count    = 0;
        XSendEvent(m_Display,m_Window,false,ExposureMask,(XEvent*)&ev);
        //XFlush(m_Display);
      }

    //----------------------------------------
    // mouse cursor
    //----------------------------------------

    virtual void resetCursor(void)
      {
        XUndefineCursor(m_Display,m_Window);
        XFreeCursor(m_Display,m_CurrentCursor);
        m_CurrentCursor = -1;
      }

    //----------

    virtual void setCursor(int a_Cursor)
      {
        //if( aCursor<0 ) aCursor = DEF_CURSOR;
        if (m_CurrentCursor>=0) resetCursor();
        //mWinCursor = XCreatePixmapCursor( gDP,bitmapNoData,bitmapNoData,&mBlack,&mBlack,0,0 );
        if (a_Cursor>=0)
        {
          m_CurrentCursor = XCreateFontCursor(m_Display, a_Cursor);
          XDefineCursor(m_Display, m_Window, m_CurrentCursor);
        }
      }

    //----------

    virtual void setCursorPos(int a_Xpos, int a_Ypos)
      {
      }

    //----------

    virtual void showCursor(void)
      {
        setCursor(H_DEF_CURSOR);
      }

    //----------

    virtual void hideCursor(void)
      {
        if (m_CurrentCursor>=0) resetCursor();
        m_CurrentCursor = XCreatePixmapCursor( m_Display,m_BitmapNoData,m_BitmapNoData,&m_Black,&m_Black,0,0 );
        XDefineCursor(m_Display,m_Window,m_CurrentCursor);
      }

    //----------

    virtual void grabCursor(void)
      {
        int which = ButtonPressMask|ButtonReleaseMask|PointerMotionMask;
        XGrabPointer(m_Display,m_Window,false,which,GrabModeSync,GrabModeAsync,m_Window,/*cursor*/None,CurrentTime);
      }

    //----------

    virtual void releaseCursor(void)
      {
        XUngrabPointer(m_Display,CurrentTime);
      }

    //----------------------------------------
    // timer
    //----------------------------------------

    virtual void startTimer(int a_Speed)
      {
        m_TimerSleep = a_Speed;
        m_TimerRunning = true;
        /*int ret = */pthread_create( &m_TimerThread, NULL, &h_linux_timerProc, this );
      }

    //----------

    virtual void stopTimer(void)
      {
        void* ret;
        m_TimerRunning = false;
        /*int ret = */pthread_join(m_TimerThread, &ret);
      }

    //----------------------------------------
    // events
    //----------------------------------------

    // internal
    int remapButton(int a_Button)
      {
        int ret = bu_None;
        switch (a_Button)
        {
          case 1: ret = bu_Left;   break;
          case 2: ret = bu_Middle; break;
          case 3: ret = bu_Right;  break;
        }
        return ret;
      }

    //----------

    // internal
    int remapKey(int a_Key)
      {
        int ret = 0;
        if (a_Key & 1) ret |= bu_Shift;
        // 2 CapsLock
        if (a_Key & 4) ret |= bu_Ctrl;
        if (a_Key & 8) ret |= bu_Alt;
        // 128 Alt Gr
        return ret;
      }


    //virtual
    void sendEvent(unsigned int a_Value=0)
      {
        static XClientMessageEvent event;
        h_Memset(&event,0,sizeof(event));
        event.type          = ClientMessage;
        event.message_type  = m_CustomEventAtom;
        event.display       = m_Display;
        event.window        = m_Window;
        event.format        = 32;
        event.data.l[0]     = a_Value;
        XSendEvent(m_Display,m_Window,False,NoEventMask,(XEvent*)&event);
        XFlush(m_Display);
      }

    //----------

    // todo: check if the event is really meant for us???
    // do we need to handle the various windows ourselves?
    // or should we have a separate Display* connection for each window?

    virtual void eventLoop(void)
      {
        XEvent ev;
        while (1)
        {
          XNextEvent(m_Display,&ev);
          //if (ev.xany.window == m_Window)
          //{
            unsigned int data = ev.xclient.data.l[0];
            if (ev.type==ClientMessage && data==m_DeleteWindowAtom) { break; }
            else eventHandler(&ev);
          //}
        }
      }

    //----------------------------------------
    //----------------------------------------

    //virtual
    void eventHandler(XEvent* a_Event)
      {
        switch (a_Event->type)
        {
          case ClientMessage:
            {
              int v = a_Event->xclient.data.l[0];
              //printf("ClientMessage: %i\n",v); //dtrace("ClientMessage: " << v);
              if (v==ts_Timer) do_Timer();
            }
            break;
          case ConfigureNotify:
            {
              int w = a_Event->xconfigure.width;
              int h = a_Event->xconfigure.height;
              //printf("ConfigureNotify: %i,%i\n",w,h);
              do_SetSize(w,h);
            }
            break;
          case PropertyNotify:
            {
              //char* atomname = XGetAtomName(m_Display,a_Event->xproperty.atom);
              //dtrace("PropertyNotify: " << a_Event->xproperty.atom << " '" << atomname << "' (" << a_Event->xproperty.state << ")" );
            }
            break;
          case Expose:
            {
              beginPaint();
              h_Rect rc;
              rc.x = a_Event->xexpose.x;
              rc.y = a_Event->xexpose.y;
              rc.w = a_Event->xexpose.width;
              rc.h = a_Event->xexpose.height;
              //while (XCheckTypedWindowEvent(m_Display, a_Event->xexpose.window, Expose, a_Event))
              //  rc.combine(a_Event->xexpose.x,
              //             a_Event->xexpose.y,
              //             a_Event->xexpose.width,
              //             a_Event->xexpose.height);
              //printf("Expose: %i,%i - %i,%i\n",rc.x,rc.y,rc.w,rc.h);
              do_Paint(m_Painter,rc,0);
              endPaint();
            }
            break;
          case ButtonPress:
            {
              int x = a_Event->xbutton.x;
              int y = a_Event->xbutton.y;
              int b = remapButton(a_Event->xbutton.button);
              int k = remapKey(a_Event->xbutton.state);

              //int s = a_Event->xbutton.state;
              //printf("ButtonPress: %i,%i - %i,%i\n",x,y,b,s);
              do_MouseDown(x,y,b,k);
            }
            break;
          case ButtonRelease:
            {
              int x = a_Event->xbutton.x;
              int y = a_Event->xbutton.y;
              int b = remapButton(a_Event->xbutton.button);
              int k = remapKey(a_Event->xbutton.state);
              //printf("ButtonRelease: %i,%i - %i,%i\n",x,y,b,s);
              do_MouseUp(x,y,b,k);
            }
            break;
          case MotionNotify:
            {
              int x = a_Event->xbutton.x;
              int y = a_Event->xbutton.y;
              int b = 0;
              int s = a_Event->xbutton.state;
              //printf("MotionNotify: %i,%i - %i\n",x,y,s);
              do_MouseMove(x,y,b|s);
            }
            break;
          case KeyPress:
            {
              int k = a_Event->xkey.keycode;
              int s = a_Event->xkey.state;
              //printf("KeyPress: %i,%i\n",k,s);
              do_KeyDown(k,s);
            }
            break;
          case KeyRelease:
            {
              int k = a_Event->xkey.keycode;
              int s = a_Event->xkey.state;
              //printf("KeyRelease: %i,%i\n",k,s);
              do_KeyUp(k,s);
            }
            break;
          //default:
          //  //printf("unhandled event: %i\n",a_Event->type);
          //  break;
        } // switch

      }

    //----------------------------------------
    //
    //----------------------------------------

};

//----------------------------------------------------------------------

typedef h_Window_Linux h_Window_Impl;

//----------------------------------------------------------------------
//
// thread proc
//
//----------------------------------------------------------------------

// ignore messages not meant for our window?

void* h_linux_threadProc(void* data)
  {
    h_Window_Impl* win = (h_Window_Impl*)data;
    //trace("threadProc. win=" << win);
    if (win)
    {
      XEvent ev;
      while (1)
      {
        XNextEvent(win->m_Display,&ev);                                         // win->m_Display
        //XWindowEvent(win->m_Display,win->m_Window,win->m_EventMask,&ev );
        //trace("threadProc.event. win=" << ev.xany.window);
        //if (ev.xany.window == win->m_Window)
        //{
          if (ev.type==ClientMessage)
          {
            //wtrace("client message");
            XClientMessageEvent *cev = (XClientMessageEvent *)&ev;
            unsigned int data = ev.xclient.data.l[0];
            if (cev->message_type==win->m_CustomEventAtom)                        // win->m_CustomEventAtom
            {
              if (data==ts_Kill) { /*wtrace("ts_Kill");*/ pthread_exit(NULL); }
              else win->eventHandler(&ev);                                        // win->eventHandler
            }
          } //ClientMessage
          else win->eventHandler(&ev);                                            // win->eventHandler
        }
      //} // = win
    }
    return NULL;
  }

//----------------------------------------------------------------------
//
// timer proc
//
//----------------------------------------------------------------------

#include <unistd.h> // usleep

void* h_linux_timerProc(void* data)
  {
    h_Window_Impl* win = (h_Window_Impl*)data;
    if (win)
    {
      while (win->m_TimerRunning)
      {
        win->sendEvent(ts_Timer);
        usleep(win->m_TimerSleep*1000); //ms*1000;
      }
    }
    return NULL;
  }

//----------------------------------------------------------------------
#endif
