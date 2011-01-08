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

class h_Editor
{
  private:
    h_Window* m_Window;

  public:

    h_Editor(h_Instance* a_Instance)
      {
        m_Window = H_NULL;
      }

    virtual ~h_Editor()
      {
        if (m_Window) delete m_Window;
      }

    void open(void* a_Parent)
      {
        if (!m_Window)
        {
          m_Window = new h_Window(320,240,a_Parent);
          m_Window->show();
        }
      }

    void close(void)
      {
        if (m_Window)
        {
          m_Window->hide();
          delete m_Window;
          m_Window = H_NULL;
        }
      }

    void eventLoop(void)
      {
        if (m_Window)
        {
          m_Window->eventLoop();
        }
      }

};

//----------------------------------------------------------------------
#endif

