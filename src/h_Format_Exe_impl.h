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
#ifndef h_Format_Exe_impl_included
#define h_Format_Exe_impl_included
//#ifdef h_Exe_included
//----------------------------------------------------------------------

h_Format::h_Format()
  {
  }

//----------

h_Format::~h_Format()
  {
  }

//----------

h_Descriptor* h_Format::createDescriptor(void)
  {
    return new H_DESCRIPTOR();
  }

//----------

h_Instance* h_Format::createInstance(h_Host* a_Host, h_Descriptor* a_Descriptor)
  {
    return new H_INSTANCE(a_Host,a_Descriptor);
  }

//----------

int h_Format::entrypoint(void* a_Ptr)
  {

    h_Host* host = new h_Host();
    h_Descriptor* descriptor = createDescriptor();
    h_Instance* instance = createInstance(host,descriptor);
    //TODO: do_HandleState: is_Open, is_Resume, etc [fake a host]
    #ifndef H_NOGUI
    if (descriptor->m_Flags & df_HasEditor)
    {
      h_Editor* editor = (h_Editor*)instance->do_OpenEditor(a_Ptr);
      editor->/*getWindow()->*/eventLoop();
      instance->do_CloseEditor();
    }
    #endif
    delete instance;
    delete descriptor;
    delete host;
    return 0;
  }

//----------------------------------------------------------------------
//#endif
#endif

