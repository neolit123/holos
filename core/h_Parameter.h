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
#ifndef h_Parameter_included
#define h_Parameter_included
//----------------------------------------------------------------------

#include "lib/h_Array.h"
#include "lib/h_String.h"
#include "lib/h_Math.h" // pow

//----------------------------------------------------------------------

// parameter flags
#define pf_None     0
#define pf_Automate 1 // effCanBeAutomated

#define PF_DEFAULT pf_Automate

// parameter types
#define pt_None     0
#define pt_Float    1
#define pt_Int      2
#define pt_FloatPow 3

//----------------------------------------------------------------------

class h_Parameter
{
  private:
    int       m_Connection;      // which widget (if any) this is connected to
    int       m_Index;           // index into the parameter list
    float     m_Default;
  protected:
    h_String  m_Name;
    h_String  m_Label;
    int       m_Flags;
    float     m_Value;   // 0..1
    float     m_Pow;
    char**    m_Strings;
  //public:
  //  int       m_Id;
  //  void*     m_Ptr;

  public:

    h_Parameter(h_String a_Name, h_String a_Label, int a_Flags, float a_Value=0 )
      {
        m_Connection = -1;
        m_Index = -1;
        m_Name = a_Name;
        m_Label = a_Label;
        m_Flags = a_Flags;
        m_Value = a_Value;
        m_Pow = 1;
        m_Strings = H_NULL;
        m_Default = m_Value;
        //m_Id = 0;
        //m_Ptr = H_NULL;
      }

    virtual ~h_Parameter()
      {
      }

    virtual void      setIndex(int a_Index)       { m_Index=a_Index; }
    virtual void      setInternal(float a_Value)  { m_Value=a_Value; }    // 0..1
    virtual void      setValue(float a_Value)     { m_Value=a_Value; }    // transformed

    virtual void      getName(char* buf)          { h_Strcpy(buf,m_Name.ptr()); }
    virtual void      getLabel(char* buf)         { h_Strcpy(buf,m_Label.ptr()); }
    virtual int       getFlags(void)  { return m_Flags; }     // 0..1
    virtual float     getValue(void)              { return m_Value; }     // transformed
    virtual float     getInternal(void)           { return m_Value; }     // 0..1
    virtual void      getDisplay(char* buf)       { h_Ftoa(buf,getValue(),5); }
    virtual int       getIndex(void)              { return m_Index; }

    virtual void      reset(void)                 { m_Value=m_Default;}

    virtual h_String  getName(void)               { return m_Name; }
    virtual h_String  getLabel(void)              { return m_Label; }

};

//----------------------------------------------------------------------

typedef h_Array<h_Parameter*> h_Parameters;



//
//----------------------------------------------------------------------
//
// test !!!
//
//----------------------------------------------------------------------
//

struct h_Parameter2;

typedef float (*h_parameter_func)(h_Parameter2* par, float val);

//class h_Parameter2
struct h_Parameter2
{
  //public:

    h_String          m_Name;
    float             m_Value;
    h_parameter_func  m_Func;
    float             m_Min;
    float             m_Max;
    float             m_Step;
    char**            m_Strings;
    //int               m_Index;
    //h_parameter_func  m_InvFunc;

  void settValue(float a_Value)
    {
      //if (m_InvFunc) m_Value = m_InvFunc(a_Value);
      //else m_Value = a_Value;
    }

  // get value is used in:
  // - h_Instance.do_HandleParameter
  // - getDisplay

  float getValue(float a_Value)
    {
      if (m_Func) return m_Func(this,a_Value);
      return m_Value;
    }

};

// various par_cb functions
float h_par_pow2(h_Parameter2* par, float val) { return h_Pow(val,2); }
float h_par_int(h_Parameter2* par, float val) { return val; } // ???

//----------------------------------------------------------------------
#endif




