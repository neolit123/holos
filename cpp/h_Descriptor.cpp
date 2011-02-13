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
#ifndef h_Descriptor_cpp_included
#define h_Descriptor_cpp_included
//#ifdef h_Descriptor_included
//----------------------------------------------------------------------

h_Descriptor::h_Descriptor()
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
    m_Parameters.clear();
  }

//----------

//h_Descriptor(h_ParamDescr* a_Params)
//  {
//  }

//----------

h_Descriptor::~h_Descriptor(void)
  {
    #ifndef H_NOAUTODELETE
      deleteParameters();
    #endif
  }

// ---------- parameters ----------

h_Parameters* h_Descriptor::getParameters(void)
  {
    return &m_Parameters;
  }

//----------

h_Parameter*  h_Descriptor::getParameter(int a_Index)
  {
    return m_Parameters[a_Index];
  }

//----------

void h_Descriptor::appendParameter(h_Parameter* a_Parameter)
  {
    int index = m_Parameters.size();
    a_Parameter->setIndex(index);
    m_Parameters.append(a_Parameter);
  }

//----------

void h_Descriptor::deleteParameters(void)
  {
    for (int i=0; i<m_Parameters.size(); i++) { delete m_Parameters[i]; };
  }

// ---------- programs ----------

//TODO


//----------------------------------------------------------------------
//#endif
#endif
