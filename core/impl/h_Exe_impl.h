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
#ifndef h_Exe_impl_included
#define h_Exe_impl_included
#ifdef h_Exe_included
//----------------------------------------------------------------------

#ifndef H_NOGUI
  #include "gui/h_Window.h"
#endif

#include "core/impl/h_Exe_Host_impl.h"
#include "core/impl/h_Exe_Instance_impl.h"
#include "core/impl/h_Exe_Format_impl.h"

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

//int main(void)
int main(int argc, char** argv)
{
  static_Debug.initialize();
  static_Core.initialize();
  //parse cmdline args -> entrypoint
  return static_Core.m_Format->entrypoint(H_NULL);
}

//----------------------------------------------------------------------
#endif
#endif
