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
#ifndef h_Debug_impl_included
#define h_Debug_impl_included
#ifdef h_Debug_included
//----------------------------------------------------------------------

#include "lib/h_Globals.h"

//----------

h_Debug::h_Debug()
  {
    m_Initialized = false;
    #ifdef H_DEBUG_MEM
      m_MemTracer = H_NULL;
    #endif
    #ifdef H_DEBUG_LOG
      m_LogFile = H_NULL;
    #endif
    #ifdef H_DEBUG_CON
      m_Console = H_NULL;
    #endif
  }

//----------

h_Debug::~h_Debug()
  {
    #ifdef H_DEBUG_MEM
      if (m_MemTracer)
      {
        // dump eventual leaks...
        delete m_MemTracer;
      }
    #endif
    #ifdef H_DEBUG_LOG
      if (m_LogFile) delete m_LogFile;
    #endif
    #ifdef H_DEBUG_CON
      if (m_Console) delete m_Console;
    #endif
  }

//----------------------------------------------------------------------

void h_Debug::initialize(void)
  {
    if (!m_Initialized)
    {
      #ifdef H_DEBUG_CON
        m_Console = new h_Console(); // xbtn,ontop,resize
      #endif
      #ifdef H_DEBUG_LOG
        m_LogFile = new h_LogFile(H_DEBUG_LOG);
      #endif
      //_dbg_trace("h_Debug.initialized");
      #ifdef H_DEBUG_MEM
        m_MemTracer = new h_MemTracer();
      #endif
      //_dbg_trace("h_Debug.initialized");
      m_Initialized = true;
    }
  }

//----------------------------------------------------------------------

#ifdef H_DEBUG_MEM

void* h_Debug::_malloc(size_t size)
  {
    return m_MemTracer->trace_malloc(size);
  }

//----------

void* h_Debug::_calloc(size_t num, size_t size)
  {
    return m_MemTracer->trace_calloc(num,size);
  }

//----------

void* h_Debug::_realloc(void* ptr, size_t size)
  {
    return m_MemTracer->trace_realloc(ptr,size);
  }

//----------

void  h_Debug::_free(void* ptr)
  {
    return m_MemTracer->trace_free(ptr);
  }

#endif

//----------------------------------------------------------------------
#endif
#endif
