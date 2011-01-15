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
#ifndef h_Format_Exe_included
#define h_Format_Exe_included
//----------------------------------------------------------------------

class h_Format : public h_Format_Base
{
  private:
    int m_Result;

  public:

    h_Format()
    : h_Format_Base()
      {
      }

    virtual ~h_Format()
      {
      }

    //-------

    virtual void* entrypoint(h_Host_Base* a_Host)
      {
        h_Descriptor_Base* descriptor = static_Core.createDescriptor();
        h_Instance_Base* instance = static_Core.createInstance(descriptor);
        // if HasEditor
        if (descriptor->m_Flags & df_HasEditor)
        {
          h_Editor_Base* editor = static_Core.createEditor(instance,h_Rect(256,256),H_NULL);
          editor->do_Open(H_NULL);
          editor->do_EventLoop();
          editor->do_Close();
          delete editor;
        }
        delete instance;
        delete descriptor;
        m_Result = 0;
        return &m_Result;
      }

};

//----------------------------------------------------------------------
#endif
