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
#ifndef h_WdgColor_included
#define h_WdgColor_included
//----------------------------------------------------------------------

//TODO: remove? we have h_WdgBackground

#include "src/h_Color.h"
#include "src/h_Widget.h"

class h_WdgColor : public h_Widget
{
  private:
    bool      m_Client;
    h_Color   m_Color;

  public:

    h_WdgColor(h_WidgetListener* a_Listener, h_Rect a_Rect, h_Color a_Color=H_GREY)
    : h_Widget(a_Listener,a_Rect)
      {
        m_Color = a_Color;
      }
    virtual ~h_WdgColor()
      {
      }

  public:

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect)
      {
        a_Painter->setBrushColor(m_Color);
        a_Painter->fillRect(a_Rect.x,a_Rect.y,a_Rect.x2(),a_Rect.y2());
      }

};

//----------------------------------------------------------------------
#endif
