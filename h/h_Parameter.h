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
    float     m_Value;   // 0..1
//    float     m_Pow;
//    char**    m_Strings;
    //h_ParameterListener* m_Listener;
  //public:
  //  int       m_Id;
  //  void*     m_Ptr;

  public:

    h_Parameter(h_String a_Name, h_String a_Label, int a_Flags = 0, float a_Value=0 )
      {
        m_Connect = -1;
        m_Index = -1;
        m_Name = a_Name;
        m_Label = a_Label;
        m_Flags = a_Flags;
        m_Value = a_Value;
        m_Default = m_Value;
        //m_Pow = 1;
        //m_Strings = H_NULL;
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

typedef h_Array<h_Parameter*> h_Parameters;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class h_ParFloat : public h_Parameter
{
  private:
    float m_Range;
    float m_InvRange;
    float m_NumSteps;
    float m_StepSize;
    float m_HalfStep;
  private:
    float m_Min, m_Max, m_Step;

  public:

    h_ParFloat( const h_String a_Name,
                const h_String a_Label = "",
                const int a_Flags = 0,
                const float a_Value = 0.f,
                const float a_Min = 0.f,
                const float a_Max = 1.f,
                const float a_Step = 0.f)
    : h_Parameter(a_Name, a_Label, a_Flags)
    {
      setup(a_Value, a_Min, a_Max, a_Step);
    }
    
    ~h_ParFloat() {}

    void setup(float a_Value, float a_Min, float a_Max, float a_Step)
      {
        m_Value    = a_Value;
        m_Min      = a_Min;
        m_Max      = a_Max;
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
        setValue(m_Value);
      }

    virtual void  setValue(const float a_Value)
      {
        m_Value = ((a_Value - m_Min) * m_InvRange);
      }

    virtual float getValue(void)
      {
        if(m_Step > 0)
        {
          const float n = m_Value * m_NumSteps;
          const int st = h_Mini(n, (m_NumSteps - 1));
          return m_Min + (st * m_Step);
        }
        else
        {
          return m_Min + (m_Value * m_Range);
        }
      }

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
    const char**  m_Strings;

  private:
    int           m_Min, m_Max, m_Step;

  public:

    h_ParInt( const h_String a_Name,
              const h_String a_Label = "",
              const int a_Flags = 0,
              const int a_Value = 0,
              const int a_Min = 0,
              const int a_Max = 1,
              const char** a_Strings = NULL)
      : h_Parameter(a_Name, a_Label, a_Flags)
      {
        setup(a_Value, a_Min, a_Max, a_Strings);
      }
      
    ~h_ParInt() {}

    void setup( const int a_Value,
                const int a_Min,
                const int a_Max,
                const char** a_Strings)
      {
        m_Strings = a_Strings;
        m_Min       = a_Min;
        m_Max       = a_Max;
        m_Range     = m_Max - m_Min + 1;
        m_InvRange  = 1/m_Range;
        m_HalfStep  = (m_InvRange)*0.5;
        setInt(a_Value);
      }

    virtual void setInt(int a_Value)
      {
        m_Value = (float)(a_Value - m_Min) * m_InvRange;
        //m_Value += m_HalfStep;
      }

    virtual int getInt(void)
      {
        const float n = h_Floor (m_Value * m_Range );
        return (m_Min + h_Mini(n, (m_Range - 1)));
      }

    virtual void  setValue(const float a_Value)
      {
        setInt((int)a_Value);
      }

    virtual float getValue(void)
      {
        return (float)getInt();
      }

    virtual void  doGetDisplay(char* buf)
      {
        const int i = getInt();
        if (m_Strings) h_Strcpy(buf, m_Strings[i]);
        else h_Itoa(buf, i);
      }

};


class h_ParText : public h_Parameter
{
  private:
    float         m_Range;
    float         m_InvRange;
    float         m_NumSteps;
    float         m_StepSize;
    float         m_HalfStep;
    const char**  m_Strings;

  private:
    int           m_Min, m_Max, m_Step;

  public:

    h_ParText( const h_String a_Name,
              const h_String a_Label = "",
              const int a_Flags = 0,
              const int a_Value = 0,
              const int a_Min = 0,
              const int a_Max = 1,
              const char** a_Strings = NULL)
      : h_Parameter(a_Name, a_Label, a_Flags)
      {
        setup(a_Value, a_Min, a_Max, a_Strings);
      }
      
    ~h_ParText() {}

    void setup( const int a_Value,
                const int a_Min,
                const int a_Max,
                const char** a_Strings)
      {
        m_Strings = a_Strings;
        m_Min       = a_Min;
        m_Max       = a_Max;
        m_Range     = m_Max - m_Min + 1;
        m_InvRange  = 1/m_Range;
        m_HalfStep  = (m_InvRange)*0.5;
        setInt(a_Value);
      }

    virtual void setInt(int a_Value)
      {
        m_Value = (float)(a_Value - m_Min) * m_InvRange;
        //m_Value += m_HalfStep;
      }

    virtual int getInt(void)
      {
        const float n = h_Floor (m_Value * m_Range );
        return (m_Min + h_Mini(n, (m_Range - 1)));
      }

    virtual void  setValue(const float a_Value)
      {
        setInt((int)a_Value);
      }

    virtual float getValue(void)
      {
        return (float)getInt();
      }

    virtual void  doGetDisplay(char* buf)
      {
        const int i = getInt();
        if (m_Strings) h_Strcpy(buf, m_Strings[i]);
        else h_Itoa(buf, i);
      }

};

//----------------------------------------------------------------------
#endif /* h_Parameter_included */
