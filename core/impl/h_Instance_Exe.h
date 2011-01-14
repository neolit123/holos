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
#ifndef h_Instance_Exe_Included
#define h_Instance_Exe_Included
//----------------------------------------------------------------------

#include "gui/h_Window.h"

class h_Instance_Exe
{
  private:
    h_Descriptor* m_Descriptor;
    int           m_ReturnValue;

  public:
    h_Instance_Exe(h_Descriptor* a_Descriptor)
      {
        m_Descriptor = a_Descriptor;
        m_ReturnValue = 0;
      }
    virtual ~h_Instance_Exe()
      {
      }

//    virtual void* entrypoint(void* a_Ptr)
//      {
//        m_ReturnValue = 0;
//        if (m_Descriptor->m_Flags & df_HasEditor)
//        {
//          h_Window* window = (h_Window*)do_OpenEditor(a_Ptr);
//          window->eventLoop();
//          do_CloseEditor();
//          //TODO: timer -> do_IdleEditor
//        }
//        return &m_ReturnValue;
//      }

    //----------

    virtual void* do_OpenEditor(void* a_Parent) { return H_NULL; }
    virtual void  do_CloseEditor(void) {}
    virtual void  do_IdleEditor(void) {}

};

//----------------------------------------------------------------------

typedef h_Instance_Exe h_Instance;

//----------------------------------------------------------------------
#endif

