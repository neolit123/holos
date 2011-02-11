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
  #include "gui/source/h_Window_Win32.cpp"
#endif

#ifdef H_LINUX
  #include "gui/source/h_Window_Linux.cpp"
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

    //----------------------------------------

    virtual void redrawAll(void)                  { invalidate( m_Rect.x, m_Rect.y, m_Rect.w, m_Rect.h ); }
    virtual void redrawRect(h_Rect a_Rect)        { invalidate( a_Rect.x, a_Rect.y, a_Rect.w, a_Rect.h ); }
    virtual void redrawWidget(h_Widget* a_Widget) { redrawRect(a_Widget->getRect()); }

    //----------------------------------------

    //virtual void goModal(h_Widget* a_Widget)
    //  {
    //    m_ModalWidget = a_Widget;
    //    m_ModalIndex = appendWidget(m_ModalWidget);
    //    redrawAll();
    //  }

    //----------

    //void unModal(void)
    //  {
    //    removeWidget(m_ModalIndex);
    //    delete m_ModalWidget;
    //    m_ModalWidget = NULL;
    //    redrawAll();
    //  }

    //----------------------------------------

//    /*
//      h_Array.append looks like can be dangerous,
//      because it increases the size before the item itself is added to the array..
//      if a thread is reading the array size just after it has been increased,
//      but before the item has been properly set up and written to the array buffer,
//      things could go wrong...
//    */
//
//    #ifndef AX_WIDGET_NOUPDATELIST
//
//    void clearUpdates(void)
//      {
//        //mutex_dirty.lock();
//        m_UpdateList.clear(false);
//        //mutex_dirty.unlock();
//      }
//
//    //----------
//
//    void appendUpdate(h_Widget* a_Widget)
//      {
//        for( int i=0; i<m_UpdateList.size(); i++ ) if( m_UpdateList[i]==a_Widget ) return;
//        //mutex_dirty.lock();
//        m_UpdateList.append(a_Widget);
//        //mutex_dirty.unlock();
//      }
//
//    //----------
//
//    // if we're inside this redrawDirty (because of idleEditor),
//    // we can't append new widgets to it!!
//    // dangerous if we don't manage the redrawDirty ourselves...
//    // we might need a redrawLock here
//    // or we need to be very certain about which thread is adding
//    // widgets to the array, and which is reading from the list...
//    // appendUpdate vs redrawUpdates
//
//    void redrawUpdates(void)
//      {
//        //mutex_dirty.lock();
//        int num = m_UpdateList.size();
//        //trace("redrawUpdates: " << num);
//        for( int i=0; i<num; i++ )
//        {
//          h_Widget* wdg = m_UpdateList[i];
//          redrawWidget(wdg);
//        }
//        clearUpdates();
//        //mutex_dirty.unlock();
//        //flush();
//
//      }
//
//    #endif // AX_WIDGET_NOUPDATELIST

    //----------------------------------------
    // do...
    //----------------------------------------

    virtual void do_SetSize(int a_Width, int a_Height)
      {
        //trace("do_SetSize");
        m_Rect.setSize(a_Width,a_Height);
        if (m_Flags&wf_Align) do_Realign();
      }

    //----------------------------------------

//    // if we're in a modal state, send these events only to
//    // the modal widget (popup, etc)
//
//    virtual void do_MouseDown(int aXpos, int aYpos, int aButton)
//      {
//        if (m_ModalWidget) m_ModalWidget->do_MouseDown(aXpos,aYpos,aButton);
//        else h_Widget::do_MouseDown(aXpos,aYpos,aButton);
//      }
//
//    virtual void do_MouseUp(int aXpos, int aYpos, int aButton)
//      {
//        if (m_ModalWidget) m_ModalWidget->do_MouseUp(aXpos,aYpos,aButton);
//        else h_Widget::do_MouseUp(aXpos,aYpos,aButton);
//      }
//
//    virtual void do_MouseMove(int aXpos, int aYpos, int aButton)
//      {
//        if (mModalWidget) mModalWidget->doMouseMove(aXpos,aYpos,aButton);
//        else axWidget::doMouseMove(aXpos,aYpos,aButton);
//      }
//
//    virtual void do_KeyDown(int aKeyCode, int aState)
//      {
//        if (mModalWidget) mModalWidget->doKeyDown(aKeyCode,aState);
//        else axWidget::doKeyDown(aKeyCode,aState);
//      }
//
//    virtual void do_KeyUp(int aKeyCode, int aState)
//      {
//        if (mModalWidget) mModalWidget->doKeyUp(aKeyCode,aState);
//        else axWidget::doKeyUp(aKeyCode,aState);
//      }

    //----------------------------------------
    // on..
    //----------------------------------------

//    virtual void on_Change(h_Widget* a_Widget)
//      {
//        redrawWidget(a_Widget);
//      }
//

    virtual void on_Redraw(h_Widget* a_Widget, int a_Mode)
      {
        //trace("h_Window.redraw");
        a_Widget->do_Paint(getPainter(),a_Widget->getRect(),a_Mode);
        //redrawWidget(a_Widget);
      }


    virtual void on_Cursor(int a_Cursor)
      {
        setCursor(a_Cursor);
      }

//    virtual void on_Hint(h_String aHint)
//      {
//      }
//
//    virtual void on_Size(h_Widget* a_Widget, int a_DeltaX, int a_DeltaY, int a_Mode)
//      {
//        if (m_Flags&wf_Align) do_Realign();
//        //if (mFlags&wf_Visible)
//          redrawAll();
//      }
//
//    virtual void on_Modal(bool a_Modal, h_Widget* a_Widget)
//      {
//        if (a_Modal) goModal(a_Widget);
//        else unModal();
//      }

};




//----------------------------------------------------------------------
#endif
