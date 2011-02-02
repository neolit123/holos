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
#ifndef par_Float_included
#define par_Float_included
//----------------------------------------------------------------------

#include "core/h_Parameter.h"
#include "lib/h_Math.h"

class par_Float : public h_Parameter
{
  protected:
    float m_Range;
    float m_InvRange;
    float m_NumSteps;
    float m_StepSize;
    float m_HalfStep;
  public:
    float m_Min, m_Max, m_Step;

  public:

    par_Float(h_String a_Name, h_String a_Label, float a_Value, int a_Flags,
              float a_Min=0, float a_Max=1, float a_Step=0)
    : h_Parameter(a_Name,a_Label,a_Value,a_Flags)
      {
        setup(a_Value,a_Min,a_Max,a_Step);
      }

    //virtual ~parFloat() {}

    //--------------------------------------------------

    void setup(float m_Value, float a_Min, float a_Max, float a_Step)
      {
        m_Min      = a_Min;                 // 2
        m_Max      = a_Max;                 // 4
        m_Step     = a_Step;                // 0.5
        m_Range    = m_Max-m_Min;            // 4-2 = 2
        m_InvRange = 1/m_Range;
        if( m_Step > 0 )
        {
          m_NumSteps = 1 + (m_Range/m_Step); // 1+(2/0.5)  = 1+4   = 5
          m_StepSize = 1 / (m_NumSteps-1);  // 1/(5-1)    = 1/4   = 0.25
          m_HalfStep = 0.5 * m_StepSize;    // 0.5 * 0.25 = 0.125
        }
        else
        {
          m_NumSteps = 1;
          m_StepSize = 1;
          m_HalfStep = 0;
        }
        setValue(m_Value);
      }

    //----------

//    int getStep(void)
//    {
//      float n = mValue * mNumSteps;
//      return axMin(n,(mNumSteps-1));
//    }

    //--------------------------------------------------

    virtual void  setValue(const float a_Value)
      {
        m_Value = ( (a_Value-m_Min) * m_InvRange);
        //mValue += mHalfStep;
      }

    //----------

    virtual float getValue(void)
      {
        if( m_Step > 0 )
        {
          const float n = m_Value * m_NumSteps;
          const int st = h_Mini(n,(m_NumSteps-1));
          return m_Min + (st*m_Step);
        }
        else
        {
          return m_Min + (m_Value*m_Range);
        }
      }

};

//----------------------------------------------------------------------

class par_FloatPow : public par_Float
{
  private:
    float m_Power;

  public:
    par_FloatPow(h_String a_Name, h_String a_Label, float a_Value, int a_Flags,
                 float a_Min=0, float a_Max=1, float a_Step=0, float a_Power=1)
    : par_Float(a_Name,a_Label,a_Value,a_Flags,a_Min,a_Max,a_Step)
      {
        m_Power = a_Power;
      }

    virtual float getValue(void)
      {
        const float v = par_Float::getValue();
        // TODO: check?
        if (v>0)
        {
          if (m_Power != 1.f) return h_Pow(v,m_Power);
          else return v;
        } else return 0;
      }

};

//----------------------------------------------------------------------
#endif



