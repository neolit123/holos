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
#ifndef h_Events_included
#define h_Events_included
//----------------------------------------------------------------------

#include "src/h_Queue.h"

#define H_MAX_EVENTS 1024

//----------

// event types
#define et_None         0
//#define et_State      1
//#define et_Transport  2
//#define et_Parameter  3
//#define et_Midi       4

//----------------------------------------

struct h_Event
{
  int   m_Type;
  void* m_Ptr;
};

//----------

class h_EventHandler
{
  private:
    h_Queue<h_Event,H_MAX_EVENTS> m_Events;
};

//----------------------------------------------------------------------
#endif
