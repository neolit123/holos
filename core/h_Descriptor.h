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

#include "lib/h_String.h"
#include "lib/h_Rect.h"
#include "core/h_Parameter.h"

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

    h_Descriptor()
      {
        m_Name        = "no name";
        m_Author      = "no author";
        m_Product     = "holos test plugin";
        m_Version     = 0;
        m_UniqueId    = H_MAGIC;
        m_Flags       = df_None;
        m_NumInputs   = 2;
        m_NumOutputs  = 0;
        m_EditorRect  = h_Rect(256,256);
        //m_Parameters.clear();
      }

    //h_Descriptor(h_ParamDescr* a_Params)
    //  {
    //  }

    virtual ~h_Descriptor(void)
      {
        #ifndef H_NOAUTODELETE
          for (int i=0; i<m_Parameters.size(); i++) delete m_Parameters[i];

        #endif
      }

    h_Parameters* getParameters(void) { return &m_Parameters; }

};

//----------------------------------------------------------------------

//#include "lib/h_Rect.h"
//
//struct h_ParamDescr
//{
//  //const char*   m_Name;
//  //const char*   m_Label;
//  h_String      m_Name;
//  h_String      m_Label;
//  int           m_Type;
//  int           m_Flags;
//  float         m_Value;
//  float         m_Min;
//  float         m_Max;
//  float         m_Step;
//  float         m_Pow;
//  const char**  m_Strings;
//};
//
//struct h_ProgDescr
//{
//  //const char*   m_Name;
//  h_String  m_Name;
//  float*        m_Values; // ptr to m_NumParams values (floats)
//};
//
//// descriptor flags
//#define df_None         0
//#define df_HasEditor    1
//#define df_IsSynth      2
//#define df_SendMidi     4
//#define df_ReceiveMidi  8
//#define df_AutoSync     16
//
//struct h_Descriptor
//{
//  //const char*   m_Name;
//  //const char*   m_Author;
//  //const char*   m_Product;
//  h_String      m_Name;
//  h_String      m_Author;
//  h_String      m_Product;
//  unsigned long m_Version;
//  unsigned long m_UniqueId;
//  int           m_Flags;
//  int           m_NumInputs;
//  int           m_NumOutputs;
//  int           m_NumParams;
//  int           m_NumProgs;
//  h_ParamDescr* m_Params;
//  h_ProgDescr*  m_Progs;
//  h_Rect        m_EditorRect;
//};
//
////----------------------------------------
//
//h_Descriptor static_Descriptor =
//{
//  "plugin",
//  "holos",
//  "(powered by holos)",
//  0,
//  H_MAGIC,
//  df_None,
//  2,
//  2,
//  0,
//  0,
//  H_NULL,
//  H_NULL,
//  h_Rect(256,256)
//};

//----------------------------------------------------------------------
#endif
