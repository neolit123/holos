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
#ifndef wdg_Slider_included
#define wdg_Slider_included
//----------------------------------------------------------------------
// action button
// switch extends this to have on/off state

#include "gui/h_Color.h"
#include "gui/h_Widget.h"

// assumes m_Listener is valid...

class wdg_Slider : public h_Widget
{
  private:
    float m_Value;
    float origval;
    int   clickedx;
    float clickedy;
    bool  dragging;

  public:

    wdg_Slider(h_WidgetListener* a_Listener,h_Rect a_Rect, int a_Align, float a_Value)
    : h_Widget(a_Listener,a_Rect,a_Align)
      {
        m_Value  = a_Value;
        origval  = m_Value;
        clickedx = 0;
        clickedy = 0;
        dragging = false;
      }

    virtual ~wdg_Slider()
      {
      }

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        m_Skin->drawSlider(a_Painter,m_Rect,a_Mode,m_Value);
        //h_Widget::do_Paint(a_Painter,a_Rect);
      }

    virtual void do_Enter(h_Widget* a_Widget)
      {
        m_Listener->on_Redraw(this,2); // draw border
      }

    virtual void do_Leave(h_Widget* a_Widget)
      {
        m_Listener->on_Redraw(this,3); // remove border
      }

    virtual void do_MouseDown(int x, int y, int b, int s)
      {
        dragging = true;
        clickedx = x;
        //clickedy = y;
        origval = m_Value;
        m_Listener->on_Redraw(this,1); // active
        m_Listener->on_Redraw(this,2); // border on
      }

    virtual void do_MouseUp(int x, int y, int b, int s)
      {
        dragging = false;
        origval = m_Value;
        //m_Listener->on_Change(this);
        bool inside = m_Rect.contains(x,y);
        m_Listener->on_Redraw(this,0); // active
        if (inside) m_Listener->on_Redraw(this,2); // border on
      }

    virtual void do_MouseMove(int x, int y, int s)
      {
        if (dragging)
        {
          float distx = x - clickedx;
          //trace("  distx: " << distx);
          float sens = 1.0f / (float)m_Rect.w;
          //trace("  sens: " << sens);
          m_Value = origval + distx*sens;
          //trace("  m_Value: " << m_Value);
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
