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
#ifndef wdg_Panel_included
#define wdg_Panel_included
//----------------------------------------------------------------------

#include "gui/h_Widget.h"

class wdg_Panel : public h_Widget
{
  private:

  public:

    wdg_Panel(h_WidgetListener* a_Listener, h_Rect a_Rect)
    : h_Widget(a_Listener,a_Rect)
      {
      }
    virtual ~wdg_Panel()
      {
      }

  public:

};

//----------------------------------------------------------------------
#endif
