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
#ifndef h_WdgBackground_included
#define h_WdgBackground_included
//----------------------------------------------------------------------

#include "src/h_Color.h"
#include "src/h_Widget.h"

class h_WdgBackground : public h_Widget
{
  public:

    h_WdgBackground(h_WidgetListener* a_Listener)
    : h_Widget(a_Listener,H_NULL_RECT,wa_Client)
      {
      }

    virtual ~h_WdgBackground()
      {
      }

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        h_Rect rect = m_Rect;
        rect.crop( a_Rect.x, a_Rect.y, a_Rect.w, a_Rect.h );
        //trace( "" << rect.x << "," <<  rect.y << "," << rect.w << "," << rect.h );
        m_Skin->drawBackground(a_Painter,rect,0);
        h_Widget::do_Paint(a_Painter,a_Rect,0);
      }

};

//----------------------------------------------------------------------
#endif
