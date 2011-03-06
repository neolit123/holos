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
#ifndef h_Instance_Ladspa_impl_included
#define h_Instance_Ladspa_impl_included
//#ifdef h_Ladspa_included
//----------------------------------------------------------------------

h_Instance::h_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
  {
    m_Host        = a_Host;
    m_Descriptor  = a_Descriptor;
    m_Parameters  = m_Descriptor->getParameters();
    m_PlayState   = 0;
    m_SampleRate  = 44100;
    m_SamplePos   = 0;
    m_BeatPos     = 0;
    m_Tempo       = 120;

    // set the default values, so we can compare them in lad_run(), and
    // notify our instance if they changes

    for (int i=0; i<m_Parameters->size(); i++)
    {
      //float val = m_Parameters->item(i)->getInternal();                       // !!!!!!!!!!
      float val = m_Parameters->item(i)->getValue();
      trace("param " << i << " = " << val);
      m_ParamPrev[i] = val;
    }
  }

h_Instance::~h_Instance()
  {
    if (m_Host) delete m_Host;        // !!!!!  created in entrypoint()
    if (m_Descriptor) delete m_Descriptor;        // !!!!!  created in entrypoint()
  }


//inline h_Rect getEditorRect(void) { return m_EditorRect; }

void h_Instance::transferParameters(void)
  {
    int num = m_Parameters->size();
    for (int i=0; i<num; i++)
    {
      h_Parameter* par = m_Parameters->item(i);//[i];
      do_HandleParameter(par);
    }
  }

void h_Instance::notifyParameter(h_Parameter* a_Parameter) {} // useless for ladspa
void h_Instance::notifyResize(int aWidth, int aHeight) {} // useless for ladspa
void h_Instance::updateTime(void) {} // useless for ladspa
void h_Instance::sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3) {} // useless for ladspa

//----------------------------------------------------------------------

void h_Instance::lad_connect_port(unsigned long Port, LADSPA_Data * DataLocation)
  {
    //trace("Port: " << Port << " DataLocation: " << (int)DataLocation);
    unsigned int io = m_Descriptor->m_NumInputs + m_Descriptor->m_NumOutputs;// mNumInputs + mNumOutputs;
    if (Port<io) // audio in/out
    {
      //TODO: don't hardcode ports!!! (multichannel...)
      switch (Port)
      {
        case 0: m_Inputs[0]  = DataLocation; break;
        case 1: m_Inputs[1]  = DataLocation; break;
        case 2: m_Outputs[0] = DataLocation; break;
        case 3: m_Outputs[1] = DataLocation; break;
      }
    }
    else // parameter
    {
      int po = Port - io;
      m_ParamPtr[po] = DataLocation;
    }
  }

void h_Instance::lad_activate(void)
  {
    trace("");
    do_HandleState(is_Resume);
  }

void h_Instance::lad_run(unsigned long SampleCount)
  {
    //int io = m_Descriptor->m_NumInputs + m_Descriptor->m_NumOutputs;
    int par = m_Descriptor->m_Parameters.size();
    // check if any parameter have changed tjhe values since last time,
    // and if so, call doSetValue, so the new value is propagated
    // throughout the library
    for (int i=0; i<par; i++)
    {
      float val = *m_ParamPtr[i];
      if (val!=m_ParamPrev[i])
      {
        //m_Parameters->item(i)->setInternal(val);                              // !!!!!!!!!!
        m_Parameters->item(i)->setValue(val);
        do_HandleParameter( m_Parameters->item(i) );
        m_ParamPrev[i] = val;
      }
    }
    // process audio.. if doProcessBlock returns false, we call
    // doProcessSample for each sample in the block..
    // (and keep track of the pointers to the input/output buffers)
    bool swallowed = do_ProcessBlock(m_Inputs,m_Outputs,SampleCount);
    if ( !swallowed )
    {
      //TODO: don't hardcode stereo...
      // when we add multi-channel (not only stereo) support to
      // vst, we should change this too...
      float* ins[2];
      float* outs[2];
      ins[0]  = m_Inputs[0];
      ins[1]  = m_Inputs[1];
      outs[0] = m_Outputs[0];
      outs[1] = m_Outputs[1];
      int num = SampleCount;
      while (--num >= 0)
      {
        do_ProcessSample(ins,outs);
        ins[0]++;   ins[1]++;
        outs[0]++;  outs[1]++;
      } //SampleCount
    } //process_block
    // and eventual posrprocessing...
    do_PostProcess(m_Inputs,m_Outputs,SampleCount);
  }

//void h_Instance::lad_run_adding(unsigned long SampleCount) {}
//void h_Instance::lad_set_run_adding_gain(LADSPA_Data Gain) {}

void h_Instance::lad_deactivate(void)
  {
    trace("");
    do_HandleState(is_Suspend);
  }

void h_Instance::lad_cleanup(void)
  {
    trace("");
    do_HandleState(is_Close);
  }


//----------------------------------------------------------------------
//#endif
#endif











/*
    for (int i=0; i<a_Descriptor->m_Parameters.size(); i++)
    {
      axParamInfo p = mDescriptor->getParamInfo(i);
      float val = p.mDef;
      if (val<0.33) val=0.25;
      else if (val>0.66) val=0.75;
      else val=0.5;
      switch(p.mType)
      {
        case pa_Par:
          appendParameter( new axParameter( this,p.mName,"", val ) );
          break;
        case pa_Float:
          appendParameter( new parFloat(    this,p.mName,"",val, p.mMin, p.mMax, p.mStep ) );
          break;
        case pa_Pow:
          appendParameter( new parFloatPow( this,p.mName,"",val, p.mMin, p.mMax, p.mStep, p.mAux ) );
          break;
        case pa_Int:
          appendParameter( new parInteger(  this,p.mName,"",val, p.mMin, p.mMax, p.mStr ) );
          break;
        case pa_Ctm:
          appendParameter( new parFloatCustom( this,p.mName,"",p.mDef, p.mMin, p.mMax, p.mStep, p.mPtr ) );
          break;
      }
    }
    setupParameters();
*/
