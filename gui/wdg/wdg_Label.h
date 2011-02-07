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
#ifndef wdg_Label_included
#define wdg_Label_included
//----------------------------------------------------------------------
// action button
// switch extends this to have on/off state

#include "gui/h_Color.h"
#include "gui/h_Widget.h"

// assumes m_Listener is valid...

class wdg_Label : public h_Widget
{
  private:
    h_String m_Text;

  public:

    wdg_Label(h_WidgetListener* a_Listener,h_Rect a_Rect, int a_Align, h_String a_Text)
    : h_Widget(a_Listener,a_Rect,a_Align)
      {
        m_Text = a_Text;
      }

    virtual ~wdg_Label()
      {
      }

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        m_Skin->drawLabel(a_Painter,m_Rect,a_Mode,m_Text);
        //h_Widget::do_Paint(a_Painter,a_Rect);
      }

};

//----------------------------------------------------------------------
#endif
