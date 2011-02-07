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
#ifndef wdg_Button_included
#define wdg_Button_included
//----------------------------------------------------------------------
// action button
// switch extends this to have on/off state

#include "gui/h_Color.h"
#include "gui/h_Widget.h"

class wdg_Button : public h_Widget
{
  private:
    h_String m_Text;

  public:

    wdg_Button(h_WidgetListener* a_Listener,h_Rect a_Rect, int a_Align, h_String a_Text)
    : h_Widget(a_Listener,a_Rect,a_Align)
      {
        m_Text = a_Text;
      }

    virtual ~wdg_Button()
      {
      }

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        m_Skin->drawButton(a_Painter,m_Rect,a_Mode,m_Text);
        //h_Widget::do_Paint(a_Painter,a_Rect);
      }

    virtual void do_Enter(h_Widget* a_Widget)
      {
        if (m_Listener) m_Listener->on_Redraw(this,2);
      }

    virtual void do_Leave(h_Widget* a_Widget)
      {
        if (m_Listener) m_Listener->on_Redraw(this,3);
      }

    virtual void do_MouseDown(int x, int y, int b, int s)
      {
        //trace("click");
        //m_Clicked = true;
        //m_Skin->drawButton(a_Painter,m_Rect,1);

        //m_Parent->on_Redraw(this,1);
        if (m_Listener) m_Listener->on_Redraw(this,1);
        if (m_Listener) m_Listener->on_Change(this);
      }

    virtual void do_MouseUp(int x, int y, int b, int s)
      {
        //m_Skin->drawButton(a_Painter,m_Rect,0);
        //m_Parent->on_Redraw(this,0);
        if (m_Listener) m_Listener->on_Redraw(this,0);
      }

//    virtual void do_MouseMove(int x, int y, int s)
//      {
//        if (!m_Rect.contains(x,y))
//        {
//          if (m_Clicked)
//          {
//            m_Clicked = false;
//            if (m_Listener) m_Listener->on_Redraw(this,0);
//          }
//        }
//      }

};

//----------------------------------------------------------------------
#endif
