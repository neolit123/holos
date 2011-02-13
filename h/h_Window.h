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
  #include "cpp/h_Window_Win32.cpp"
#endif

#ifdef H_LINUX
  #include "cpp/h_Window_Linux.cpp"
#endif

//----------------------------------------------------------------------

struct h_WidgetUpdateInfo
{
  h_Widget* m_Widget;
  int       m_Mode;
  h_WidgetUpdateInfo(h_Widget* a_Widget, int a_Mode)
    {
      m_Widget = a_Widget;
      m_Mode = a_Mode;
    }
};

typedef h_Array<h_WidgetUpdateInfo> h_WidgetUpdates;

//#include "h/h_Queue.h"
//#define H_WIDGET_QUEUE_SIZE 256
//typedef h_Queue<h_WidgetUpdateInfo,H_WIDGET_QUEUE_SIZE> m_WidgetUpdates;

//----------

class h_Window : public h_Window_Impl
{
  private:
    h_WidgetUpdates m_WidgetUpdates;

  public:

    h_Window(h_WidgetListener* a_Listener, h_Rect a_Rect, void* a_Parent)
    : h_Window_Impl(a_Listener,a_Rect,a_Parent)
      {
      }

    virtual ~h_Window()
      {
      }

    //----------------------------------------
    // drawing
    //----------------------------------------

    //virtual
    void redraw(void)
      {
        invalidate( m_Rect.x, m_Rect.y, m_Rect.w, m_Rect.h );
        flush();
      }

    //virtual
    void redraw(h_Rect a_Rect)
      {
        invalidate( a_Rect.x, a_Rect.y, a_Rect.w, a_Rect.h );
        flush();
      }

    //virtual
    void redraw(h_Widget* a_Widget)
      {
        redraw(a_Widget);
        flush();
      }

    //----------------------------------------
    // modal
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
    // cached widget updates
    //----------------------------------------

    //#ifndef H_WIDGET_NOUPDATELIST

    void clearUpdates(void)
      {
        //mutex_dirty.lock();
        m_WidgetUpdates.clear(false);
        //mutex_dirty.unlock();
      }

    //----------

    void appendUpdate(h_Widget* a_Widget, int a_Mode)
      {
        //for( int i=0; i<m_WidgetUpdates.size(); i++ ) if( m_WidgetUpdates[i].m_Widget==a_Widget ) return;
        m_WidgetUpdates.append( h_WidgetUpdateInfo(a_Widget,a_Mode));
      }

    //----------

    //TODO: consider threads, idleeditor, widget tweaking (do_MouseDown) ...

    void redrawUpdates(void)
      {
        h_Rect rect;
        int num = m_WidgetUpdates.size();
        //trace("redrawUpdates: " << num);
        for( int i=0; i<num; i++ )
        {
          h_Widget* widget = m_WidgetUpdates[i].m_Widget;
          int mode = m_WidgetUpdates[i].m_Mode;
          h_Rect wr = widget->getRect();
          widget->do_Paint( getPainter(), wr, mode );
          rect.combine( wr.x, wr.y, wr.w, wr.h );
        }
        clearUpdates();
        redraw(rect);
      }

    //#endif // H_WIDGET_NOUPDATELIST

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
    // modal (do..)
    //----------------------------------------

    // if we're in a modal state, send these events only to
    // the modal widget (popup, etc)

    //virtual void do_MouseDown(int aXpos, int aYpos, int aButton)
    //  {
    //    if (m_ModalWidget) m_ModalWidget->do_MouseDown(aXpos,aYpos,aButton);
    //    else h_Widget::do_MouseDown(aXpos,aYpos,aButton);
    //  }

    //virtual void do_MouseUp(int aXpos, int aYpos, int aButton)
    //  {
    //    if (m_ModalWidget) m_ModalWidget->do_MouseUp(aXpos,aYpos,aButton);
    //    else h_Widget::do_MouseUp(aXpos,aYpos,aButton);
    //  }

    //virtual void do_MouseMove(int aXpos, int aYpos, int aButton)
    //  {
    //    if (mModalWidget) mModalWidget->doMouseMove(aXpos,aYpos,aButton);
    //    else axWidget::doMouseMove(aXpos,aYpos,aButton);
    //  }

    //virtual void do_KeyDown(int aKeyCode, int aState)
    //  {
    //    if (mModalWidget) mModalWidget->doKeyDown(aKeyCode,aState);
    //    else axWidget::doKeyDown(aKeyCode,aState);
    //  }

    //virtual void do_KeyUp(int aKeyCode, int aState)
    //  {
    //    if (mModalWidget) mModalWidget->doKeyUp(aKeyCode,aState);
    //    else axWidget::doKeyUp(aKeyCode,aState);
    //  }

    //----------------------------------------
    // on..
    //----------------------------------------

    //virtual void on_Change(h_Widget* a_Widget)
    //  {
    //    redrawWidget(a_Widget);
    //  }


    // a widget says it needs to be redrawn..

    // when we tweak a widget, we want to redraw immediately, so the
    // response feels smooth...

    virtual void on_Redraw(h_Widget* a_Widget, int a_Mode)
      {
        appendUpdate(a_Widget,a_Mode);
        // this should be done in do_IdleEditor() if vst,
        // or timer thread if exe
        // NOT per widget redraw!
        redrawUpdates();

      }

    // a widget want to change the mouse cursor

    virtual void on_Cursor(int a_Cursor)
      {
        setCursor(a_Cursor);
      }

    //virtual void on_Hint(h_String aHint)
    //  {
    //  }

    //virtual void on_Size(h_Widget* a_Widget, int a_DeltaX, int a_DeltaY, int a_Mode)
    //  {
    //    if (m_Flags&wf_Align) do_Realign();
    //    //if (mFlags&wf_Visible)
    //      redrawAll();
    //  }

    //virtual void on_Modal(bool a_Modal, h_Widget* a_Widget)
    //  {
    //    if (a_Modal) goModal(a_Widget);
    //    else unModal();
    //  }

};

//----------------------------------------------------------------------
#endif
