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
#ifndef h_Core_impl_included
#define h_Core_impl_included
#ifdef h_Core_included
//----------------------------------------------------------------------

h_Core::h_Core()
  {
    m_Initialized = false;
    m_Platform  = H_NULL;
    m_Format = H_NULL;
  }

//----------

h_Core::~h_Core()
  {
    if (m_Initialized)
    {
      /*if (m_Platform)*/  delete m_Platform;
      /*if (m_Format)*/ delete m_Format;
    }
  }

//----------

void h_Core::initialize(void)
  {
    if (!m_Initialized)
    {
      m_Platform  = new h_Platform();
      m_Format = new h_Format();
      m_Initialized = true;
    }
  }

//----------------------------------------------------------------------
#endif
#endif
