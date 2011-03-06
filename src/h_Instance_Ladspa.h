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
#ifndef h_Instance_Ladspa_included
#define h_Instance_Ladspa_included
//----------------------------------------------------------------------

#include "../extern/ladspa/ladspa.h"

#define MAX_LADSPA_PORTS 256

class h_Instance : public h_Instance_Base
{
  friend class h_Format;
  private:
    h_Host*   m_Host;
    //h_Rect    m_EditorRect;
    //bool      m_EditorIsOpen;
    int       m_PlayState;
    double    m_SampleRate;
    double    m_SamplePos;
    double    m_BeatPos;
    double    m_Tempo;
    //
    float*        m_Inputs[MAX_LADSPA_PORTS];
    float*        m_Outputs[MAX_LADSPA_PORTS];
    LADSPA_Data*  m_ParamPtr[MAX_LADSPA_PORTS];
    LADSPA_Data   m_ParamPrev[MAX_LADSPA_PORTS];

  protected:
    h_Descriptor* m_Descriptor;
    h_Parameters* m_Parameters;

  public:

    h_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor);
    virtual ~h_Instance();

    //inline h_Rect getEditorRect(void) { return m_EditorRect; }

    virtual double  getSampleRate(void)     { return m_SampleRate; }

    virtual void  transferParameters(void);
    virtual void  notifyParameter(h_Parameter* a_Parameter);

    virtual void  notifyResize(int aWidth, int aHeight); // not used
    virtual void  updateTime(void); // not used
    virtual void  sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3);  // not used

    virtual void lad_connect_port(unsigned long Port, LADSPA_Data * DataLocation);
    virtual void lad_activate(void);
    virtual void lad_run(unsigned long SampleCount);
    //virtual void lad_run_adding(unsigned long SampleCount); // not used
    //virtual void lad_set_run_adding_gain(LADSPA_Data Gain); // not used
    virtual void lad_deactivate(void);
    virtual void lad_cleanup(void);

};


//----------------------------------------------------------------------
#endif

