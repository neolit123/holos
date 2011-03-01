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
#ifndef h_WdgImage_included
#define h_WdgImage_included
//----------------------------------------------------------------------

#include "src/h_Color.h"
#include "src/h_Widget.h"

class h_WdgImage : public h_Widget
{
  private:
    h_Surface* m_Surface;

  public:

    h_WdgImage(h_WidgetListener* a_Listener, h_Rect a_Rect, int a_Align, h_Surface* a_Surface)
    : h_Widget(a_Listener,a_Rect,a_Align)
      {
        m_Surface = a_Surface;
      }

    virtual ~h_WdgImage()
      {
      }

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        if (m_Surface) a_Painter->drawSurface(m_Surface,m_Rect.x,m_Rect.y,0,0,m_Rect.w,m_Rect.h);
        h_Widget::do_Paint(a_Painter,a_Rect,a_Mode);
      }

};

//----------------------------------------------------------------------
#endif

