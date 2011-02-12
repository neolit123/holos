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
#ifndef h_Descriptor_included
#define h_Descriptor_included
//----------------------------------------------------------------------

#include "h/h_String.h"
#include "h/h_Rect.h"
#include "h/h_Parameter.h"

// descriptor flags
#define df_None         0
#define df_HasEditor    1
#define df_IsSynth      2
#define df_SendMidi     4
#define df_ReceiveMidi  8
#define df_AutoSync     16

class h_Descriptor
{
  public:
    h_String      m_Name;
    h_String      m_Author;
    h_String      m_Product;
    unsigned long m_Version;
    unsigned long m_UniqueId;
    int           m_Flags;
    int           m_NumInputs;
    int           m_NumOutputs;
    h_Rect        m_EditorRect;
    h_Parameters  m_Parameters;
    //h_Programs    m_Programs;
  public:
    h_Descriptor();
    virtual ~h_Descriptor(void);
    virtual h_Parameter*  getParameter(int a_Index);
    virtual h_Parameters* getParameters(void);
    virtual void appendParameter(h_Parameter* a_Parameter);
    virtual void deleteParameters(void);

};

//----------------------------------------------------------------------
#endif
