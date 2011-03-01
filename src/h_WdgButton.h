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
#ifndef h_WdgButton_included
#define h_WdgButton_included
//----------------------------------------------------------------------
// action button
// [h_WdgSwitch] extends this to have on/off state

#include "src/h_Color.h"
#include "src/h_Widget.h"

class h_WdgButton : public h_Widget
{
  private:
    h_String  m_Text;
    bool      m_Clicked;
    bool      m_WasInside;

  public:

    h_WdgButton(h_WidgetListener* a_Listener,h_Rect a_Rect, int a_Align, h_String a_Text)
    : h_Widget(a_Listener,a_Rect,a_Align)
      {
        m_Text = a_Text;
        m_Clicked = false;
        m_WasInside = false;
      }

    virtual ~h_WdgButton()
      {
      }

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        m_Skin->drawButton(a_Painter,m_Rect,a_Mode,m_Text);
      }

    virtual void do_Enter(h_Widget* a_Widget)
      {
        m_Listener->on_Redraw(this,dm_Enter); // draw border
        m_Listener->on_Cursor(cu_Finger);
      }

    virtual void do_Leave(h_Widget* a_Widget)
      {
        m_Listener->on_Redraw(this,dm_Leave); // remove border
        m_Listener->on_Cursor(H_DEF_CURSOR);
      }

    virtual void do_MouseDown(int x, int y, int b, int s)
      {
        m_Listener->on_Redraw(this,dm_Active); // highlight
        m_Clicked = true;
        m_WasInside = true;
      }

    virtual void do_MouseUp(int x, int y, int b, int s)
      {
        bool inside = m_Rect.contains(x,y);
        m_Listener->on_Redraw(this,dm_Normal); // background
        if (inside)
        {
          m_Listener->on_Change(this);
          m_Listener->on_Redraw(this,dm_Enter); // border
        }
        m_Clicked = false;
      }

    virtual void do_MouseMove(int x, int y, int s)
      {
        if (m_Clicked)
        {
          bool inside = m_Rect.contains(x,y);
          if (m_WasInside && !inside) m_Listener->on_Redraw(this,dm_Normal); // normal (off)
          if (!m_WasInside && inside) m_Listener->on_Redraw(this,dm_Active); // on (highlight)
          m_WasInside = inside;
        }
      }

};

//----------------------------------------------------------------------
#endif
