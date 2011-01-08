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
#ifndef h_Main_Exe_included
#define h_Main_Exe_included
//----------------------------------------------------------------------

int main(void)
{
  static_Debug.initialize();
  static_Core.initialize();
  h_Host* host = new h_Host(H_NULL);
  h_Descriptor* desc = static_Base.createDescriptor(host);
  h_Instance*   inst = static_Base.createInstance(desc);
  h_Editor*     edit = static_Base.createEditor(inst);
  edit->open(H_NULL);
  edit->eventLoop();
  edit->close();
  delete edit;
  delete inst;
  delete desc;
  delete host;
  return 0;
}

//----------------------------------------------------------------------
#endif
