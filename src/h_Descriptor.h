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

#include "src/h_String.h"
#include "src/h_Rect.h"
#include "src/h_Parameter.h"
#include "src/h_Program.h"

// descriptor flags
#define df_None         0
#define df_HasEditor    1
#define df_IsSynth      2
#define df_SendMidi     4
#define df_ReceiveMidi  8
#define df_AutoSync     16

#define H_MAX_PROGNAMESIZE 64

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
    h_Programs    m_Programs;
    //int           m_NumPrograms;
    //char*         m_ProgramNames;
    //float*        m_ProgramValues;

  public:

    //h_Descriptor();
    //virtual ~h_Descriptor(void);
    //virtual h_Parameter*  getParameter(int a_Index);
    //virtual h_Parameters* getParameters(void);
    //virtual void appendParameter(h_Parameter* a_Parameter);
    //virtual void deleteParameters(void);

    h_Descriptor()
      {
        m_Name          = "no name";
        m_Author        = "no author";
        m_Product       = "holos test plugin";
        m_Version       = 0;
        m_UniqueId      = H_MAGIC;
        m_Flags         = df_None;
        m_NumInputs     = 2;
        m_NumOutputs    = 2;
        m_EditorRect    = h_Rect(256,256);
        //m_NumPrograms   = 0;
        //m_ProgramNames  = H_NULL;
        //m_ProgramValues = H_NULL;
        m_Parameters.clear();
        m_Programs.clear();
      }

    //h_Descriptor(h_ParamDescr* a_Params)
    //  {
    //  }

    virtual ~h_Descriptor()
      {
        #ifndef H_NOAUTODELETE
          deleteParameters();
          deletePrograms();
        #endif
      }

    // ---------- parameters ----------

    h_Parameters* getParameters(void)
      {
        return &m_Parameters;
      }

    h_Parameter*  getParameter(int a_Index)
      {
        return m_Parameters[a_Index];
      }

    void appendParameter(h_Parameter* a_Parameter)
      {
        int index = m_Parameters.size();
        a_Parameter->setIndex(index);
        m_Parameters.append(a_Parameter);
      }

    void deleteParameters(void)
      {
        for (int i=0; i<m_Parameters.size(); i++) { delete m_Parameters[i]; };
      }

    // ---------- programs ----------

    h_Programs* getPrograms(void)
      {
        return &m_Programs;
      }

    h_Program*  getProgram(int a_Index)
      {
        return m_Programs[a_Index];
      }

    void appendProgram(h_Program* a_Program)
      {
        int index = m_Programs.size();
        a_Program->setIndex(index);
        m_Programs.append(a_Program);
      }

    void deletePrograms(void)
      {
        for (int i=0; i<m_Programs.size(); i++) { delete m_Programs[i]; };
      }

    void initPrograms(int a_NumProgs, h_Parameters* a_Parameters)
      {
        int pnum = a_Parameters->size();

        float* val = (float*)h_Malloc(pnum*sizeof(float));

        for (int i=0; i<pnum; i++) val[i] = a_Parameters->item(i)->getInternal();
        for (int j=0; j<a_NumProgs; j++)
        {
          char str[H_MAX_PROGNAMESIZE];
          h_Strcpy(str,(char*)"program ");
          char str2[8];
          h_Strcat( str, h_Itoa(str2,j) );
          trace(str);
          appendProgram( new h_Program( h_String(str), pnum, (float*)&val) );
        }

        h_Free(val);

      }

//    void deletePrograms(void)
//      {
//        if (m_ProgramNames) delete[] m_ProgramNames;
//        if (m_ProgramValues) delete[] m_ProgramValues;
//      }

//    void saveProgram(int a_Index)
//      {
//        h_Program* prog = m_Programs[a_Index];
//        for (int i=0; i<m_Parameters.size(); i++)
//        {
//          prog->setValue( i, m_Parameters[i]->getInternal() );
//        }
//      }

//    void setProgramName(int a_Index, char* a_Name)
//      {
//        h_Strncpy( &m_ProgramNames[H_MAX_PROGNAMESIZE*a_Index], a_Name, H_MAX_PROGNAMESIZE );
//      }
//
//    char* getProgramName(int a_Index)
//      {
//        return &m_ProgramNames[H_MAX_PROGNAMESIZE*a_Index];
//      }

};

//----------------------------------------------------------------------
#endif
