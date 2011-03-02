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
#ifndef h_Program_included
#define h_Program_included
//----------------------------------------------------------------------

#define MAX_PROGNAMESIZE 64

class h_Program
{
  private:
    int       m_Index;
    char      m_Name[MAX_PROGNAMESIZE];
    int       m_Num;
    float*    m_Values;
  public:

    h_Program(h_String a_Name, int a_Num, float* a_Values)
      {
        h_Strncpy(m_Name,a_Name.ptr(),MAX_PROGNAMESIZE);
        m_Num = a_Num;
        m_Values = new float[a_Num];// a_Values;
        h_Memcpy(m_Values,a_Values,a_Num*sizeof(float));
      }

    ~h_Program()
      {
        //if (m_Name) delete m_Name;
        if (m_Values) delete m_Values;
      }

    void setIndex(int a_Index)
      {
        m_Index = a_Index;
      }

    void setValue(int a_Index, float a_Value)
      {
        m_Values[a_Index] = a_Value;
      }

    float getValue(int a_Index)
      {
        return m_Values[a_Index];
      }

    void setName(char* a_Name)
      {
        h_Strncpy(m_Name,a_Name,MAX_PROGNAMESIZE);
      }

    char* getName(void)
      {
        return (char*)&m_Name;
      }

};

#include "src/h_Array.h"
typedef h_Array<h_Program*> h_Programs;

//----------------------------------------------------------------------
#endif
