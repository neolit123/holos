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

#include "h/h_Array.h"
#include "h/h_String.h"
#include "h/h_Math.h"
#include "h/h_Stdlib.h"

//----------------------------------------------------------------------

// parameter flags
#define pf_None     0
#define pf_Automate 1 // effCanBeAutomated

#define PF_DEFAULT pf_Automate

//// parameter types
//#define pt_None     0
//#define pt_Float    1
//#define pt_Int      2
//// #define pt_FloatPow 3

//----------------------------------------------------------------------
//class h_Parameter;
//
//class h_ParameterListener
//{
//  public:
//    virtual void onChange(h_Parameter* h_Parameter) {}
//};


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
    int       m_Flags;
    float     m_Value;
    float     (*m_Ptr)(float);
    float     (*m_InvPtr)(float);

  public:

    h_Parameter(h_String a_Name,
                h_String a_Label,
                int a_Flags = 0,
                float a_Value = 0,
                float (*a_Ptr)(float) = H_NULL,
                float (*a_InvPtr)(float) = H_NULL)
      {
        m_Connect = -1;
        m_Index = -1;
        m_Name = a_Name;
        m_Label = a_Label;
        m_Flags = a_Flags;
        m_Value = a_Value;
        m_Default = m_Value;
        m_Ptr = a_Ptr;
        m_InvPtr = a_InvPtr;
      }

    virtual ~h_Parameter()
      {
      }

    virtual void      setIndex(int a_Index)       { m_Index=a_Index; }
    virtual void      setInternal(float a_Value)  { m_Value=a_Value; }    // 0..1
    virtual void      setValue(float a_Value)     { m_Value=a_Value; }    // transformed
    virtual void      getName(char* buf)          { h_Strcpy(buf,m_Name.ptr()); }
    virtual void      getLabel(char* buf)         { h_Strcpy(buf,m_Label.ptr()); }
    virtual int       getFlags(void)              { return m_Flags; }     // 0..1
    virtual float     getValue(void)              { return m_Value; }     // transformed
    virtual float     getInternal(void)           { return m_Value; }     // 0..1
    virtual void      getDisplay(char* buf)       { h_Ftoa(buf, getValue(),5); }
    virtual int       getIndex(void)              { return m_Index; }
    virtual void      reset(void)                 { m_Value=m_Default;}
    virtual h_String  getName(void)               { return m_Name; }
    virtual h_String  getLabel(void)              { return m_Label; }
    virtual void      setConnect(int a_Connect)   { m_Connect=a_Connect; }
    virtual int       getConnect(void)            { return m_Connect; }
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
    float m_Min;
    float m_Max;
    float m_Step;
    float (*m_Ptr)(float);
    float (*m_InvPtr)(float);

  public:
    h_ParFloat( const h_String a_Name,
                const h_String a_Label = "",
                const int a_Flags = 0,
                const float a_Value = 0.f,
                const float a_Min = 0.f,
                const float a_Max = 1.f,
                const float a_Step = 0.f,
                float (*a_Ptr)(float) = H_NULL,
                float (*a_InvPtr)(float) = H_NULL)
    : h_Parameter(a_Name, a_Label, a_Flags, a_Value, a_Ptr, a_InvPtr)
    {
      setup(a_Value, a_Min, a_Max, a_Step, a_Ptr, a_InvPtr);
    }

    void setup( float a_Value,
                float a_Min,
                float a_Max,
                float a_Step,
                float (*a_Ptr)(float),
                float (*a_InvPtr)(float) )
      {
        m_Ptr      = a_Ptr;
        m_InvPtr   = a_InvPtr;
        
        m_Value  = a_Value;
        m_Min    = a_Min;
        m_Max    = a_Max;
        
        m_Step     = a_Step;
        m_Range    = m_Max - m_Min;
        m_InvRange = 1 / m_Range;
        
        if(m_Step > 0)
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
        setValueDefault(m_Value);
      }
      
    virtual void  setValueDefault(const float a_Value)
      {
        m_Value = ((a_Value - m_Min) * m_InvRange);
      }

    virtual void  setValue(const float a_Value)
      {
        if (m_Ptr == H_NULL && m_InvPtr == H_NULL)
          setValueDefault(a_Value);
        else
          m_Value = m_InvPtr((a_Value - m_Min) * m_InvRange);
      }

    virtual float getValueDefault(void)
      {
        if(m_Step > 0)
        {
          const float n = m_Value * m_NumSteps;
          const int st = h_Mini((int)n, (int)(m_NumSteps - 1));          
          return m_Min + (st * m_Step);
        }
        else
        {
          return m_Min + (m_Value * m_Range);
        }
      }
    
    virtual float getValue(void)
      {
        if (m_Ptr == H_NULL && m_InvPtr == H_NULL)
          return getValueDefault();
        else
          return m_Ptr(getValueDefault());
      }
};

//----------------------------------------------------------------------
class h_ParDb : public h_ParFloat
{
  private:
    float m_Range;
    float m_Min;
    float m_Max;
    float m_Step;

  public:
    h_ParDb(const h_String a_Name,
            const int a_Flags = 0,
            const float a_Value = 0.f,
            const float a_Min = 0.f,
            const float a_Max = 1.f,
            const float a_Step = 0.f)                
    : h_ParFloat( a_Name, "dB", a_Flags, a_Value, a_Min, a_Max, a_Step)
    {}
};


//----------------------------------------------------------------------
class h_ParInt : public h_Parameter
{
  private:
    float         m_Range;
    float         m_InvRange;
    float         m_NumSteps;
    float         m_StepSize;
    float         m_HalfStep;
    int           m_Min;
    int           m_Max;
    int           m_Step;
    float         (*m_Ptr)(float);
    float         (*m_InvPtr)(float);

  public:
    h_ParInt( const h_String a_Name,
              const h_String a_Label = "",
              const int a_Flags = 0,
              const int a_Value = 0,
              const int a_Min = 0,
              const int a_Max = 1,
              float (*a_Ptr)(float) = H_NULL,
              float (*a_InvPtr)(float) = H_NULL)
      : h_Parameter(a_Name, a_Label, a_Flags, a_Value, a_Ptr, a_InvPtr)
      {
        setup(a_Value, a_Min, a_Max, a_Ptr, a_InvPtr);
      }

    void setup( const int a_Value,
                const int a_Min,
                const int a_Max,
                float (*a_Ptr)(float),
                float (*a_InvPtr)(float))
      {
        m_Min       = a_Min;
        m_Max       = a_Max;
        m_Range     = m_Max - m_Min + 1;
        m_InvRange  = 1/m_Range;
        m_HalfStep  = (m_InvRange)*0.5;
        m_Ptr       = a_Ptr;
        m_InvPtr    = a_InvPtr;
        
        setInt(a_Value);
      }

    virtual void setInt(const int a_Value)
      {
        m_Value = (float)(a_Value - m_Min) * m_InvRange;
        m_Value += m_HalfStep;
      }

    virtual int getInt(void)
      {
        const float n = h_Floor(m_Value * m_Range);
        return (m_Min + h_Mini((int)n, (int)(m_Range - 1)));
      }

    virtual void setValue(const float a_Value)
      {
        if (m_Ptr == H_NULL && m_InvPtr == H_NULL)
          setInt((int)a_Value);
        else
          return setInt((int)m_InvPtr(a_Value));
      }

    virtual float getValue(void)
      {
        if (m_Ptr == H_NULL && m_InvPtr == H_NULL)
          return (float)getInt();
        else
          return m_Ptr((float)getInt());
      }

    virtual void doGetDisplay(char* buf)
      {
        const int i = getInt();
        h_Itoa(buf, i);
      }

};

//----------------------------------------------------------------------
class h_ParText : public h_ParInt
{
  private:
    const char**  m_Strings;
   
  public:

    h_ParText( const h_String a_Name,
              const h_String a_Label = "",
              const int a_Flags = 0,
              const int a_Value = 0,              
              const int a_Max = 1,
              const char** a_Strings = NULL)
      : h_ParInt(a_Name, a_Label, a_Flags, a_Value, 0, a_Max - 1)
      {
        setup(a_Strings);
      }
      
    void setup(const char** a_Strings)
      {
        m_Strings = a_Strings;
      }
    
    virtual void getDisplay(char* buf)
      {
        const int i = getInt();
        if (m_Strings)
          h_Strcpy(buf, m_Strings[i]);
        else
          h_Itoa(buf, i);
      }
};

//----------------------------------------------------------------------

typedef h_Array<h_Parameter*> h_Parameters;

#endif /* h_Parameter_included */
