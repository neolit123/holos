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
#ifndef h_Core_included
#define h_Core_included
//----------------------------------------------------------------------

#include "core/h_Base.h"

class h_Core
{
  private:
    bool              m_Initialized;
    h_Platform_Base*  m_Platform;
    h_Interface_Base* m_Interface;
    h_Format_Base*    m_Format;

  public: // accessors (getters/setters)
    h_Platform_Base*  getPlatform(void)  { return m_Platform; }
    h_Interface_Base* getInterface(void) { return m_Interface; }
    h_Format_Base*    getFormat(void)    { return m_Format; }

  public:

    h_Core()
      {
        m_Initialized = false;
        m_Platform    = H_NULL;
        m_Interface   = H_NULL;
        m_Format      = H_NULL;
      }

    ~h_Core()
      {
        if (m_Platform)  delete m_Platform;
        if (m_Interface) delete m_Interface;
        if (m_Format)    delete m_Format;
      }

  public:

    // implemented in h_Core.cpp
    
    void initialize(void);
    h_Descriptor_Base* createDescriptor(void);
    h_Instance_Base*   createInstance(h_Descriptor_Base* a_Descriptor);
    h_Editor_Base*     createEditor(h_Instance_Base* a_Instance, h_Rect a_Rect, void* a_Paernt);

};

//----------------------------------------------------------------------

static h_Core static_Core;

//----------------------------------------------------------------------
#endif
