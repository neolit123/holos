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
#ifndef h_Instance_Included
#define h_Instance_Included
//----------------------------------------------------------------------

#ifdef H_EXE
  #include "core/impl/h_Instance_Exe.h"
#endif

#ifdef H_LADSPA
  #include "core/impl/h_Instance_Ladspa.h"
#endif

#ifdef H_VST
  #include "core/impl/h_Instance_Vst.h"
#endif

//----------------------------------------------------------------------
//class h_Instance_Base
//{
//  public:
//    h_Instance_Base(h_Descriptor_Base* a_Descriptor) {}
//    virtual ~h_Instance_Base() {}
//    virtual void do_HandleState(int a_State) {}
//    virtual void do_HandleTransport(int a_Value) {}
//    virtual void do_HandleMidi(int a_Offset, unsigned char aMsg1, unsigned char aMsg2, unsigned char aMsg3) {}
//    virtual void do_HandleParameter(h_Parameter* a_Parameter) {}
//    virtual bool do_ProcessBlock(float** a_Inputs, float** a_Outputs, int a_Length) { return false; }
//    virtual void do_ProcessSample(float** a_Inputs, float** a_Outputs) {}
//    virtual void do_PostProcess(float** a_Inputs, float** a_Outputs, int a_Length) {}
//};

//----------------------------------------------------------------------
#endif

