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

// may crash if two new instances is created in separate threads,
// at the _exact_ same time.. (or before we set he static_FirstCall..)

//static bool static_FirstCall = true;

int main(void)
{
  static_Debug.initialize();
  static_Core.initialize();
  h_Host* host = new h_Host(H_NULL);
  static_Base.initialize(host);

  // vst
  //h_Descriptor* desc = static_Base.getDescriptor();
  //AEffect* aeffect = (AEffect*)desc->getStruct();
  //h_Instance* inst = static_Base.createInstance(desc);
  //aeffect->ptr = inst;
  //return aeffect;

  h_Descriptor* desc = static_Base.getDescriptor();
  int* result = (int*)desc->entrypoint();
  h_Instance* inst = static_Base.createInstance(desc);

  h_Editor* editor = (h_Editor*)inst->do_OpenEditor(H_NULL);
  trace(editor);
  editor->eventLoop();
  inst->do_CloseEditor();

  trace("...");

  //delete window;
  delete inst;
  delete host;
  return *result;
}

//----------------------------------------------------------------------
#endif
