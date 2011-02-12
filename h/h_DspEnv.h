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
#ifndef h_DspEnv_included
#define h_DspEnv_included
//----------------------------------------------------------------------
/**
  \file h_DspEnv.h
  \brief envelope
*/


//#define env_off       0
//#define env_offset    1
//#define env_attack    2
//#define env_decay     3
//#define env_sustain   4
//#define env_release   5
//#define env_finished  0xffff

#define env_Off       0
#define env_Attack    1
#define env_Decay     2
#define env_Sustain   3
#define env_Release   4
#define env_Finished  5

#define env_Threshold 0.001f
#define env_MaxStages 5
//#define env_Scale     6

//----------------------------------------------------------------------

struct h_envstage
{
  float target;
  float rate;
};

//----------------------------------------------------------------------

class h_DspEnv
{
  protected:
    float mScale;
    int   mStage;
    float mValue;

  public:

    h_DspEnv()
      {
        mScale = 50.0f;//6.0f;
        mStage = env_Off;
        mValue = 0;
      }

    //virtual ~h_DspEnv()
    //  {
    //  }

    inline void setScale(float aScale)
      {
        mScale=aScale;
        }
    //inline float getValue(void)
    //  {
    //    return mValue;
    //  }

    virtual void noteOn(void)
      {
        mStage = env_Attack;
        mValue = 1;
      }

    virtual void noteOff(void)
      {
        mStage = env_Finished;
        mValue = 0;
      }

    virtual float process(void)
      {
        return mValue;
      }

};

//----------------------------------------------------------------------

class h_DspEnv_ExpADSR : public h_DspEnv
{
  protected:
    h_envstage  mStages[5]; // -,a,d,s,r

  public:

    h_DspEnv_ExpADSR() : h_DspEnv()
      {
        setup();
      }

    void setup(void)
      {
        mStage = env_Off;
        mValue = 0;
      }

    inline void setStage(int stage, float target, float rate)
      {
        float r1 = rate*mScale;
        float r2 = r1*r1*r1 + 1;
        //float r2 = r1*r1 + 1;
        mStages[stage].target  = target;
        mStages[stage].rate    = 1 / r2;
      }

    inline void setADSR(float a, float d, float s, float r)
      {
        setStage(env_Attack, 1,a);
        setStage(env_Decay,  s,d);
        //setStage(env_Sustain,s,1);
        mStages[env_Sustain].target = s;
        mStages[env_Sustain].rate   = 1;
        setStage(env_Release,0,r);
      }

    virtual void noteOn(void)
      {
        mStage = env_Attack;
        mValue = 0;
      }

    virtual void noteOff(void)
      {
        mStage = env_Release;
      }

    virtual float process(void)
      {
        if (mStage==env_Off) return 0;
        if (mStage==env_Finished) return 0;
        if (mStage==env_Sustain) return mValue;
        float target = mStages[mStage].target;
        float rate   = mStages[mStage].rate;
        mValue += (target-mValue) * rate;
        if (h_Abs(target-mValue)<=env_Threshold) { mStage++; }
        return mValue;
      }

};

//----------------------------------------------------------------------
#endif
