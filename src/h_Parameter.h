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

#include "src/h_Array.h"
#include "src/h_String.h"
#include "src/h_Math.h"
#include "src/h_Stdlib.h"

// parameter flags
#define pf_None     0
#define pf_Automate 1 // effCanBeAutomated

// for ladspa, set automatically..
#define pf_Log      2
#define pf_Int      4
#define pf_Bool     8

#ifndef PF_DEFAULT
  #define PF_DEFAULT pf_Automate
#endif

#ifndef H_DISP_MAXNUMS
  #define H_DISP_MAXNUMS 4
#endif


//----------------------------------------------------------------------

class h_Parameter
{
  private:
    int       m_Connect;      // which widget (if any) this is connected to
    int       m_Index;        // index into the parameter list
    float     m_Default;

  protected:
    h_String  m_Name;
    h_String  m_Label;    
    float     m_Value;    
    float     (*m_Ptr)(float);
    float     (*m_InvPtr)(float);
    
  public:
    //TODO: private + get/set*()
    // untransformed valuies, for ladspa-crap:
    int       m_Flags;
    float     m_Min;
    float     m_Max;

  public: 
    h_Parameter( const h_String a_Name,
                 const h_String a_Label = "",
                 const int a_Flags = PF_DEFAULT,
                 const float a_Value = 0,
                 const float a_Min = 0,
                 const float a_Max = 1,
                 float (*a_Ptr)(float) = H_NULL,
                 float (*a_InvPtr)(float) = H_NULL)
      {
        m_Connect  = -1;
        m_Index    = -1;
        m_Name     = a_Name;
        m_Label    = a_Label;
        m_Flags    = a_Flags;
        m_Default  = m_Value;
        setValue(a_Value);
        
        m_Ptr      = a_Ptr;
        m_InvPtr   = a_InvPtr;
        
        // define limits
        #ifdef H_LADSPA
          if (m_Ptr)
          {
            m_Min = m_Ptr(a_Min);
            m_Max = m_Ptr(a_Max);
            if (!h_Isnormalf(m_Min)) m_Min = 0.f;
            if (!h_Isnormalf(m_Max)) m_Max = 0.f;
          }
          else
          {
            m_Min = a_Min;
            m_Max = a_Max;
          }
          m_Ptr = H_NULL;
          m_InvPtr = H_NULL;
        #else
          m_Min = a_Min;
          m_Max = a_Max;
        #endif // !H_LADSPA
      }

    virtual ~h_Parameter()
      {
      }

    virtual void      setIndex(int a_Index)       { m_Index=a_Index; }
    virtual int       getIndex(void)              { return m_Index; }

    virtual void      setConnect(int a_Connect)   { m_Connect=a_Connect; }
    virtual int       getConnect(void)            { return m_Connect; }

    virtual void      setInternal(float a_Value)  { m_Value=a_Value; }      // 0..1
    virtual float     getInternal(void)           { return m_Value; }       // 0..1

    // ### test with no pointers for default class 
    // there has to be a NAN check here as well
    
    virtual void      setValue(float a_Value)     { /* if (m_InvPtr) m_Value=m_InvPtr(a_Value); else */ m_Value=a_Value; } // transformed
    virtual float     getValue(void)              { /* if (m_Ptr) return m_Ptr(m_Value); else */ return m_Value; } // transformed

    virtual void      reset(void)                 { m_Value=m_Default;}

    virtual void      getName(char* buf)          { h_Strcpy(buf,m_Name.ptr()); }
    virtual void      getLabel(char* buf)         { h_Strcpy(buf,m_Label.ptr()); }
    virtual int       getFlags(void)              { return m_Flags; }
    virtual void      getDisplay(char* buf)       { h_Ftoa(buf, getValue(),H_DISP_MAXNUMS); }
    virtual h_String  getName(void)               { return m_Name; }
    virtual h_String  getLabel(void)              { return m_Label; }

    virtual float     getMin(void)                { return m_Min; }
    virtual float     getMax(void)                { return m_Max; }

};

//----------------------------------------------------------------------

class h_ParFloat : public h_Parameter
{
  private:
    float m_Range;
    float m_InvRange;
    float m_NumSteps;
    float m_StepSize;
    float m_HalfStep;
    float m_Step;

  public:

    h_ParFloat( const h_String a_Name,
                const h_String a_Label = "",
                const int a_Flags = PF_DEFAULT,
                const float a_Value = 0.f,
                const float a_Min = 0.f,
                const float a_Max = 1.f,
                const float a_Step = 0.f,
                float (*a_Ptr)(float) = H_NULL,
                float (*a_InvPtr)(float) = H_NULL)
    : h_Parameter(a_Name,a_Label,a_Flags,a_Value,a_Min,a_Max,a_Ptr,a_InvPtr)
    {
      setup(a_Value,a_Step);
    }

    void setup( float a_Value, float a_Step)
      {
        m_Step = a_Step;
        m_Range = m_Max - m_Min;
        m_InvRange = 1 / m_Range;
        if (m_Step > 0)
        {
          m_NumSteps = 1 + (m_Range / m_Step);
          m_StepSize = 1 / (m_NumSteps - 1);
          m_HalfStep = 0.5 * m_StepSize;
        }
        else
        {
          m_NumSteps = 1;
          m_StepSize = 1;
          m_HalfStep = 0;
        }

        setValue(a_Value);
      }

    float calcSetValue(const float a_Value)
      {
        return ((a_Value - m_Min)*m_InvRange);
      }

    float calcGetValue(float a_Value)
      {
        if (m_Step > 0)
        {
          const float n = a_Value * m_NumSteps;
          const int st = h_Mini((int)n, (int)(m_NumSteps - 1));
          return m_Min + (st * m_Step);
        }
        else
        {
          return m_Min + (m_Value * m_Range);
        }
      }

    virtual void setValue(const float a_Value)
      {
        if (m_InvPtr) m_Value = calcSetValue( m_InvPtr(a_Value) );
        else m_Value = calcSetValue(a_Value);
      }

    virtual float getValue(void)
      {
        float res;
        if (m_Ptr) res = m_Ptr( calcGetValue(m_Value) );
        else res = calcGetValue(m_Value);
        if (!h_Isnormalf(res))
          return 0.f;          
        else
          return res;
      }

};

//----------------------------------------------------------------------

class h_ParInt : public h_Parameter
{
  private:
    float m_Range;
    float m_InvRange;
    float m_NumSteps;
    float m_StepSize;
    float m_HalfStep;

  public:

    h_ParInt( const h_String a_Name,
              const h_String a_Label = "",
              const int a_Flags = PF_DEFAULT,
              const int a_Value = 0,
              const int a_Min = 0,
              const int a_Max = 1,
              float (*a_Ptr)(float) = H_NULL,
              float (*a_InvPtr)(float) = H_NULL)
      : h_Parameter(a_Name,a_Label,a_Flags,a_Value,a_Min,a_Max,a_Ptr,a_InvPtr)
      {
        setup(a_Value);
      }

    void setup( const int a_Value)
      {
        if (m_Min == 0 && m_Max == 1) m_Flags |= pf_Bool;
        else m_Flags |= pf_Int;
        
        m_Range     = m_Max - m_Min + 1;
        m_InvRange  = 1/m_Range;
        m_HalfStep  = (m_InvRange)*0.5;
        setValue(a_Value);
      }

    float calcSetValue(const int a_Value)
      {
        float res = (float)(a_Value - m_Min) * m_InvRange;
        res += m_HalfStep;
        return res;
      }

    float calcGetValue(int a_Value)
      {
        const float n = h_Floor(m_Value * m_Range);
        return (int)(m_Min + h_Mini((int)n, (int)(m_Range - 1)));        
      }

    virtual void setValue(const float a_Value)
      {
        if (m_InvPtr) m_Value = calcSetValue((int)m_InvPtr(a_Value));
        else m_Value = calcSetValue((int)a_Value);
      }

    virtual float getValue(void)
      {
        float res;
        if (m_Ptr) res = m_Ptr(calcGetValue((int)m_Value));
        else res = calcGetValue((int)m_Value);
        if (!h_Isnormalf(res))
          return 0.f;
        else
          return res;          
      }

    virtual void getDisplay(char* buf)
      {
        const int i = (int)getValue();
        h_Itoa(buf,i);
      }

};

//----------------------------------------------------------------------

class h_ParText : public h_ParInt
{
  private:
    const char** m_Strings;

  public:

    h_ParText( const h_String a_Name,
               const h_String a_Label = "",
               const int a_Flags = 0,
               const int a_Value = 0,
               const int a_Num = 1,
               const char** a_Strings = NULL)
      : h_ParInt(a_Name, a_Label, a_Flags, a_Value, 0, a_Num - 1)
      {
        setup(a_Strings);
      }

    void setup(const char** a_Strings)
      {
        m_Strings = a_Strings;
      }

    virtual void getDisplay(char* buf)
      {
        const int i = (int)getValue();
        if (m_Strings) h_Strcpy(buf, m_Strings[i]);
        else h_Itoa(buf, i);
      }
};

//----------------------------------------------------------------------

typedef h_Array<h_Parameter*> h_Parameters;

#endif /* h_Parameter_included */





// comments

    /*
    idea/suggestions:
    swap around the parameters a little, so we could do something like:
      new h_Parameter("Damping", 0.5)
    and the rest oculd be default arguments
    (and, i don't know if we use the flags yet? we assume all parameters are
     automatable? and what about the label? how useful is it?

    ------------------
    - the label seems to be useful:
      <name>    <value>   <label>
      some_par   -32.5      dB
    - automation is ok to be "on" by default, but switchable (as it is now).
    - flags can hold both "automation status" and "parameter type", but the
      type has to be set automatically when creating with "new h_Parameter".
      (and these should be in a bit field)
    - in "h_Format_Ladspa_impl.h" there could be a check for that "type".
    - due to the mess of combining the two formats (ladspa, vst), i think
      that possibly the calls "new h_Parameter", "new h_ParText"
      should be replaced with macros that accept n elements, or something
      (for better control).
    */
