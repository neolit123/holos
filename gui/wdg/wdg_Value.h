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
#ifndef wdg_Value_included
#define wdg_Value_included
//----------------------------------------------------------------------
// action button
// switch extends this to have on/off state

#include "gui/h_Color.h"
#include "gui/h_Widget.h"

// assumes m_Listener is valid...

class wdg_Value : public h_Widget
{
  private:
    float m_OrigValue;
    int   m_ClickedX;
    float m_ClickedY;
    bool  m_IsDragging;
  protected:
    float m_Value;
    bool  m_DragVertical; // drag direction
    float m_DragSensitivity;     // drag sensitivity

  public:

    wdg_Value(h_WidgetListener* a_Listener,h_Rect a_Rect, int a_Align, float a_Value)
    : h_Widget(a_Listener,a_Rect,a_Align)
      {
        m_Value  = a_Value;
        m_OrigValue  = m_Value;
        m_ClickedX = 0;
        m_ClickedY = 0;
        m_IsDragging = false;
        m_DragVertical = true;
        m_DragSensitivity = 0.1;
      }

    virtual ~wdg_Value()
      {
      }

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        m_Skin->drawValue(a_Painter,m_Rect,a_Mode,m_Value);
        //h_Widget::do_Paint(a_Painter,a_Rect);
      }

    virtual void do_Enter(h_Widget* a_Widget)
      {
        m_Listener->on_Redraw(this,2); // draw border
        if (m_DragVertical) m_Listener->on_Cursor(cu_ArrowUpDown);
        else m_Listener->on_Cursor(cu_ArrowLeftRight);
      }

    virtual void do_Leave(h_Widget* a_Widget)
      {
        m_Listener->on_Redraw(this,3); // remove border
        m_Listener->on_Cursor(H_DEF_CURSOR);
      }

    virtual void do_MouseDown(int x, int y, int b, int s)
      {
        m_IsDragging = true;
        m_ClickedX = x;
        m_ClickedY = y;
        m_OrigValue = m_Value;
        m_Listener->on_Redraw(this,1); // active
        m_Listener->on_Redraw(this,2); // border on
      }

    virtual void do_MouseUp(int x, int y, int b, int s)
      {
        m_IsDragging = false;
        m_OrigValue = m_Value;
        //m_Listener->on_Change(this);
        bool inside = m_Rect.contains(x,y);
        m_Listener->on_Redraw(this,0); // active
        if (inside) m_Listener->on_Redraw(this,2); // border on
      }

    virtual void do_MouseMove(int x, int y, int s)
      {
        if (m_IsDragging)
        {
          float dist;
          if (m_DragVertical) // |
          {
            dist = m_ClickedY - y;
            dist *= 1.0f / (float)m_Rect.h * m_DragSensitivity;
          }
          else // -
          {
            dist = x - m_ClickedX;
            dist *= 1.0f / (float)m_Rect.w * m_DragSensitivity;
          }
          m_Value = m_OrigValue + dist;
          if (m_Value>1) m_Value=1;
          if (m_Value<0) m_Value=0;
          //trace("  m_Value: " << m_Value);
          m_Listener->on_Change(this);
          m_Listener->on_Redraw(this,1); // active
          m_Listener->on_Redraw(this,2); // border on
        }
      }


};

//----------------------------------------------------------------------
#endif

