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

#include "lib/h_Array.h"
#include "lib/h_Rect.h"
#include "gui/h_Painter.h"
#include "gui/h_Skin.h"

class h_Widget;
typedef h_Array<h_Widget*> h_Widgets;

//----------------------------------------------------------------------

// widget flags
#define wf_None       0x0000
#define wf_Active     0x0001
#define wf_Visible    0x0002
#define wf_Clipping   0x0100
#define wf_Align      0x0200
#define wf_Capture    0x0400

//----------------------------------------------------------------------

class h_WidgetListener
{
  public:
    virtual void on_Change(h_Widget* a_Widget) {}
};

//----------

class h_WidgetOwner // aka window
{
  public:
    virtual void on_Redraw(void) {}
    virtual void on_Redraw(h_Rect a_Rect) {}
    virtual void on_Redraw(h_Widget* a_Widget) {}
    virtual void on_SetCursor(/*h_Widget* a_Widget,*/ int a_Cursor) {}
    virtual void on_SetHint(char* a_Text) {}
};

//----------------------------------------------------------------------

class h_Widget
{
  private:
    h_WidgetListener* m_Listener;
    h_WidgetOwner*    m_Owner;
    h_Widget*         m_Parent;
    h_Widgets         m_Children;
    int               m_Index;
    h_Rect            m_Rect;
    int               m_Flags;
    h_Skin*           m_Skin;

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

  public:

    h_Widget()
      {
        m_Listener  = H_NULL;
        m_Owner     = H_NULL;
        m_Parent    = H_NULL;
        m_Children.clear();
        m_Index     = -1;
        m_Rect      = h_Rect(0,0,64,64);
        m_Flags     = wf_None;
        m_Skin      = H_NULL;
      }

    virtual ~h_Widget()
      {
        #ifndef H_NOAUTODELETE
          deleteWidgets();
        #endif
      }

    void appendWidget(h_Widget* a_Widget)
      {
        int index = m_Children.size();
        a_Widget->m_Owner  = m_Owner;
        a_Widget->m_Parent = this;
        a_Widget->m_Index  = index;
        a_Widget->m_Skin   = m_Skin;
        m_Children.append(a_Widget);
      }

    void deleteWidgets(void)
      {
        for (int i=0; i<m_Children.size(); i++) delete m_Children[i];
      }

};

//----------------------------------------------------------------------
#endif
