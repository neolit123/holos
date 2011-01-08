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
#ifndef h_Window_included
#define h_Window_included
//----------------------------------------------------------------------

#include "gui/h_Widget.h"

class h_Window_Base
{
  public:
    h_Window_Base() {}
    virtual ~h_Window_Base() {}
};

#ifdef H_WIN32
  #include "gui/impl/h_Window_Win32.h"
#endif

#ifdef H_LINUX
  #include "gui/impl/h_Window_Linux.h"
#endif

class h_Window : public h_Window_Impl,
                 public h_WidgetOwner
{
  public:
    h_Window(int a_Width, int a_Height, void* a_Parent)
    : h_Window_Win32(a_Width,a_Height,a_Parent)
      {
      }
    virtual ~h_Window() {}

    // widget owner
    virtual void on_Redraw(void) {}
    virtual void on_Redraw(h_Rect a_Rect) {}
    virtual void on_Redraw(h_Widget* a_Widget) {}


};

//----------------------------------------------------------------------
#endif
