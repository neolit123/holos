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
#ifndef h_Instance_Ladspa_impl_included
#define h_Instance_Ladspa_impl_included
//#ifdef h_Ladspa_included
//----------------------------------------------------------------------

h_Instance::h_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
  {
  }

h_Instance::~h_Instance()
  {
  }


//inline h_Rect getEditorRect(void) { return m_EditorRect; }

void h_Instance::transferParameters(void)
  {
  }

void h_Instance::notifyParameter(h_Parameter* a_Parameter) {}
//void h_Instance::notifyResize(int aWidth, int aHeight) {}
void h_Instance::updateTime(void) {}
void h_Instance::sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3) {}



void h_Instance::lad_connect_port(unsigned long Port, LADSPA_Data * DataLocation)
  {
  }

void h_Instance::lad_activate(void)
  {
  }

void h_Instance::lad_run(unsigned long SampleCount)
  {
  }

//void h_Instance::lad_run_adding(unsigned long SampleCount) {}
//void h_Instance::lad_set_run_adding_gain(LADSPA_Data Gain) {}

void h_Instance::lad_deactivate(void)
  {
  }

void h_Instance::lad_cleanup(void)
  {
  }


//----------------------------------------------------------------------
//#endif
#endif
