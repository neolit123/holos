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
#ifndef h_Editor_Included
#define h_Editor_Included
//----------------------------------------------------------------------

#include "lib/h_Rect.h"
#include "gui/h_Window.h"

//----------------------------------------------------------------------

//#ifdef H_EXE
//  #include "core/impl/h_Editor_Exe.h"
//#endif
//
//#ifdef H_LADSPA
//  #include "core/impl/h_Editor_Ladspa.h"
//#endif
//
//#ifdef H_VST
//  #include "core/impl/h_Editor_Vst.h"
//#endif

//----------------------------------------------------------------------

class h_Editor : public h_Editor_Base,
                 public h_Window//h_WidgetListener
{
  private:

  public:

//    h_Editor()
//    : h_Editor_Base()
//      {
//      }

    h_Editor(h_Instance_Base* a_Instance, h_Rect a_Rect, void* a_Parent)
    : h_Editor_Base(a_Instance,a_Rect/*,a_Parent*/)
    , h_Window(this,a_Rect,a_Parent)
      {
      }

    //----------

    virtual ~h_Editor()
      {
      }

  public:

    //----------------------------------------
    // editor
    //----------------------------------------

//    // h_Window*
    virtual void do_Open(void* a_Ptr)
      {
        show();
      }

    //----------

    virtual void do_Close(void)
      {
        hide();
      }

    //----------

    // only for exe's
    virtual void do_EventLoop(void)
      {
        eventLoop();
      }

    //----------------------------------------
    // widget base
    //----------------------------------------

    //virtual void do_Paint(h_Painter* a_Painter, h_Rect a_Rect)
    //  {
    //    trace("paint");
    //  }

    //----------------------------------------
    // widget listener
    //----------------------------------------

    //virtual void on_Hint(char* a_Text)
    //  {
    //    trace("h_Editor.hint: " << a_Text);
    //  }

};

//----------------------------------------------------------------------
#endif

