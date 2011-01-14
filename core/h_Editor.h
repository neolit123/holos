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

#include "gui/h_Window.h"
#include "gui/h_Widget.h"

class h_Editor : public h_WidgetListener
{
  //private:
  protected:
    h_Window* m_Window;
    h_Rect    m_Rect;

  public:
    inline h_Window* getWindow(void) { return m_Window; }
    inline h_Rect    getRect(void)   { return m_Rect; }

  public:


    h_Editor(h_Instance* a_Instance)
      {
        //trace("h_Editor.constructor");
        m_Window = H_NULL;
        m_Rect   = h_Rect(256,256);
      }

    //----------

    virtual ~h_Editor()
      {
        //trace("h_Editor.destructor");
        if (m_Window) delete m_Window;
      }

    //----------------------------------------
    //
    //----------------------------------------

    virtual void do_Open(h_Window* a_Window) {}
    virtual void do_Close(h_Window* a_Window) {}
    virtual void do_Idle(void) {}

    //----------------------------------------
    // widget listener
    //----------------------------------------

    //virtual void on_Change(h_Widget* a_Widget)
    //  {
    //    //trace("h_Editor.on_Change");
    //  }

    //----------------------------------------
    //
    //----------------------------------------

    virtual
    void open(void* a_Parent)
      {
        //trace("h_Editor.open");
        if (!m_Window)
        {
          m_Window = new h_Window(m_Rect,a_Parent);
          do_Open(m_Window);
          m_Window->show();
        }
      }

    //----------

    virtual
    void close(void)
      {
        //trace("h_Editor.close");
        if (m_Window)
        {
          m_Window->hide();
          do_Close(m_Window);
          delete m_Window;
          m_Window = H_NULL;
        }
      }

    //----------

    virtual
    void eventLoop(void)
      {
        //trace("h_Editor.eventLoop");
        if (m_Window)
        {
          m_Window->eventLoop();
        }
      }

};

//----------------------------------------------------------------------
#endif

