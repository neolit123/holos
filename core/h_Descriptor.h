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

#include "lib/h_Rect.h"

struct h_ParamDescr
{
  const char*   m_Name;
  const char*   m_Label;
  int           m_Type;
  int           m_Flags;
  float         m_Value;
  float         m_Min;
  float         m_Max;
  float         m_Step;
  float         m_Pow;
  const char**  m_Strings;
};

struct h_ProgDescr
{
  const char*   m_Name;
  float*        m_Values; // ptr to m_NumParams values (floats)
};

// descriptor flags
#define df_None         0
#define df_HasEditor    1
#define df_IsSynth      2
#define df_SendMidi     4
#define df_ReceiveMidi  8
#define df_AutoSync     16

struct h_Descriptor
{
  const char*   m_Name;
  const char*   m_Author;
  const char*   m_Product;
  unsigned long m_Version;
  unsigned long m_UniqueId;
  int           m_Flags;
  int           m_NumInputs;
  int           m_NumOutputs;
  int           m_NumParams;
  int           m_NumProgs;
  h_ParamDescr* m_Params;
  h_ProgDescr*  m_Progs;
  h_Rect        m_EditorRect;
};

//----------------------------------------

h_Descriptor static_Descriptor =
{
  "plugin",
  "holos",
  "(powered by holos)",
  0,
  H_MAGIC,
  df_None,
  2,
  2,
  0,
  0,
  H_NULL,
  H_NULL,
  h_Rect(256,256)
};

//----------------------------------------------------------------------
#endif
