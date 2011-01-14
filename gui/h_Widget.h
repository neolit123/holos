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
#ifndef h_Widget_included
#define h_Widget_included
//----------------------------------------------------------------------

#include "lib/h_Rect.h"
#include "gui/h_Painter.h"

//----------

// widget flags
#define wf_None       0
#define wf_Active     ( 1 << 0 )
#define wf_Visible    ( 1 << 1 )
#define wf_Timer      ( 1 << 8 )
#define wf_Clipping   ( 1 << 9 )
#define wf_Align      ( 1 << 10 )
#define wf_Capture    ( 1 << 11 )

//----------

class h_Widget;

#include "lib/h_Array.h"
typedef h_Array<h_Widget*> h_Widgets;

//----------------------------------------------------------------------

class h_WidgetBase
{
  public:
    virtual void do_Timer(void) {}
    virtual void do_SetPos(int x, int y) {}
    virtual void do_SetSize(int w, int h) {}
    virtual void do_Realign(void) {}
    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect) {}
    virtual void do_Enter(void) {}
    virtual void do_Leave(void) {}
    virtual void do_MouseDown(int x, int y, int b, int s) {}
    virtual void do_MouseUp(int x, int y, int b, int s) {}
    virtual void do_MouseMove(int x, int t, int s) {}
    virtual void do_KeyDown(int k, int s) {}
    virtual void do_KeyUp(int k, int s) {}
};

//----------------------------------------------------------------------

class h_WidgetListener
{
  public:
    virtual void on_Change(h_Widget* a_Widget) {}
    virtual void on_Hint(char* a_Text) {}
};

//typedef h_Array<h_WidgetListener*> h_WidgetListeners;


//----------

//class h_WidgetOwner // aka window
//{
//  public:
//    virtual void on_Redraw(void) {}
//    virtual void on_Redraw(h_Rect a_Rect) {}
//    virtual void on_Redraw(h_Widget* a_Widget) {}
//    virtual void on_SetCursor(int a_Cursor) {}
//};

//----------------------------------------------------------------------

class h_Widget : public h_WidgetBase,
                 public h_WidgetListener
{
  private:
    int               m_Index;
  protected:
    int               m_Flags;
    h_Rect            m_Rect;
    h_WidgetListener* m_Listener;
    h_Widget*         m_Parent;
    h_Widgets         m_Children;

  public:
    inline int    getFlags(void)        { return m_Flags; }
    inline int    hasFlag(int a_Flag)   { return (m_Flags&a_Flag); }
    inline void   setFlag(int a_Flag)   { m_Flags|=a_Flag; }
    inline void   clearFlag(int a_Flag) { m_Flags&=~a_Flag; }

  public:

    h_Widget(h_WidgetListener* a_Listener, h_Rect a_Rect)
      {
        m_Flags     = wf_None;
        m_Listener  = a_Listener;
        m_Rect      = a_Rect;
        m_Index     = -1;
        m_Parent    = H_NULL;
        //m_Children.clear();
      }

    //----------

    virtual ~h_Widget()
      {
        #ifndef H_NOAUTODELETE
          deleteWidgets();
        #endif
      }

    //----------------------------------------

    virtual void appendWidget(h_Widget* a_Widget)
      {
        int index = m_Children.size();
        a_Widget->m_Index = index;
        a_Widget->m_Parent = this;
        m_Children.append(a_Widget);
      }

    //----------

    virtual void deleteWidgets(void)
      {
        for (int i=0; i<m_Children.size(); i++) delete m_Children[i];
      }

    //----------

    //virtual void appendListener(h_WidgetListener* a_Listener)
    //  {
    //    m_Listeners.append(a_Listener);
    //  }

    //virtual void removeListener(h_WidgetListener* a_Listener)
    //  {
    //    m_Listeners.remove();
    //  }

    //----------

    //virtual void setSkin(h_Skin* a_Skin)
    //  {
    //    m_Skin = a_Skin;
    //    for (int i=0; i<m_Children.size(); i++) setSkin(a_Skin);
    //  }

    //----------

//    virtual void setPainter(h_Painter* a_Painter)
//      {
//        m_Painter = a_Painter;
//        for (int i=0; i<m_Children.size(); i++) { setPainter(a_Painter); }
//      }

    //----------------------------------------
    // widget base/handler
    //----------------------------------------

    virtual void do_Timer(void)
      {
        for (int i=0; i<m_Children.size(); i++)
        {
          h_Widget* widget = m_Children[i];
          if (widget->hasFlag(wf_Timer))
          {
            widget->do_Timer();
          }
        }
      }

    //----------

    virtual void do_SetPos(int x, int y)
      {
        m_Rect.x = x;
        m_Rect.y = y;
      }

    //----------

    virtual void do_SetSize(int w, int h)
      {
        m_Rect.w = w;
        m_Rect.h = h;
      }

    //----------

    virtual void do_Realign(void)
      {
      }

    //----------

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect)
      {
        //paint self
        for (int i=0; i<m_Children.size(); i++)
        {
          do_Paint(a_Painter,a_Rect);
        }
      }

    //----------

    virtual void do_Enter(void)
      {
      }

    //----------

    virtual void do_Leave(void)
      {
      }

    //----------

    virtual void do_MouseDown(int x, int y, int b, int s)
      {
        for (int i=0; i<m_Children.size(); i++)
        {
          do_MouseDown(x,y,b,s);
        }
      }

    //----------

    virtual void do_MouseUp(int x, int y, int b, int s)
      {
        for (int i=0; i<m_Children.size(); i++)
        {
          do_MouseUp(x,y,b,s);
        }
      }

    //----------

    virtual void do_MouseMove(int x, int y, int s)
      {
        for (int i=0; i<m_Children.size(); i++)
        {
          do_MouseMove(x,y,s);
        }
      }

    //----------

    virtual void do_KeyDown(int k, int s)
      {
        for (int i=0; i<m_Children.size(); i++)
        {
          do_KeyDown(k,s);
        }
      }

    //----------

    virtual void do_KeyUp(int k, int s)
      {
        for (int i=0; i<m_Children.size(); i++)
        {
          do_KeyUp(k,s);
        }
      }

    //----------------------------------------
    // widget listener
    //----------------------------------------

    virtual void on_Change(h_Widget* a_Widget)
      {
        if (m_Listener) m_Listener->on_Change(a_Widget);
      }

    //----------

    virtual void on_Hint(char* a_Text)
      {
        if (m_Listener) m_Listener->on_Hint(a_Text);
      }

};

//----------------------------------------------------------------------
#endif
