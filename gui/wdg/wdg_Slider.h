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

#include "gui/h_Color.h"
#include "gui/h_Widget.h"

class wdg_Slider : public wdg_Value
{

  public:

    wdg_Slider(h_WidgetListener* a_Listener,h_Rect a_Rect, int a_Align, float a_Value)
    : wdg_Value(a_Listener,a_Rect,a_Align,a_Value)
      {
        m_DragVertical = false;
        m_DragSensitivity = 1;
      }

    virtual ~wdg_Slider()
      {
      }

    virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect, int a_Mode)
      {
        m_Skin->drawSlider(a_Painter,m_Rect,a_Mode,m_Value);
      }

};

//----------------------------------------------------------------------
#endif
