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
#ifndef wdg_Background_included
#define wdg_Background_included
//----------------------------------------------------------------------

#include "gui/h_Color.h"
#include "gui/h_Widget.h"

class wdg_Background : public h_Widget
{
  private:
    h_Color   m_Color;

  public:

    wdg_Background(h_WidgetListener* a_Listener, h_Color a_Color=H_GREY)
    : h_Widget(a_Listener,NULL_RECT)
      {
        m_Color = a_Color;
      }
    virtual ~wdg_Background()
      {
      }

  public:

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect)
      {
        //trace("do_Paint("<<a_Rect.x<<","<<a_Rect.y <<","<< a_Rect.w<<","<<a_Rect.h<<")");
        a_Painter->setBrushColor(m_Color);
        a_Painter->fillRect(a_Rect.x,a_Rect.y,a_Rect.x2(),a_Rect.y2());
      }

    //test
    virtual void do_MouseDown(int x, int y, int b, int s)
      {
        if (m_Listener) m_Listener->on_Hint((char*)"click");
      }


};

//----------------------------------------------------------------------
#endif
