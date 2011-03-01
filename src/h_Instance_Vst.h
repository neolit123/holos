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
#ifndef h_Instance_Vst_included
#define h_Instance_Vst_included
//----------------------------------------------------------------------

#include "extern/vst/aeffect.h"
#include "extern/vst/aeffectx.h"
//#include "extern/vst/vstfxstore.h"

#include "src/h_Rect.h"

//struct VstEvents
//{
//	VstInt32 numEvents;		// number of Events in array
//	VstIntPtr reserved;		// zero (Reserved for future use)
//	VstEvent* events[2];	// event pointer array, variable size
//};

// TODO: replace this qith a h_Queue,
// (possibly) making it a little bit more thread-safe?

//#define H_MAX_AUDIO_CHANS 64
#define H_MAX_MIDI_SEND 1024

struct h_VstEvents
{
  VstInt32  numEvents;
  VstIntPtr reserved;
  VstEvent* events[H_MAX_MIDI_SEND];
};

//------------------------------

class h_Editor;

class h_Instance : public h_Instance_Base
{
  friend class h_Format;

  private:
    h_Host*             m_Host;
    audioMasterCallback m_AudioMaster;
    AEffect*            m_AEffect;
    VstTimeInfo*        m_TimeInfo;
    h_VstEvents         m_MidiEventList;
    int                 m_NumPrograms;
    char                m_ProgramName[kVstMaxProgNameLen];
    VstInt32            m_CurrentProgram;
    double              m_SampleRate;
    long                m_BlockSize;
    int                 m_PlayState;
    double              m_SamplePos;
    double              m_BeatPos;
    double              m_Tempo;
    bool                m_EditorIsOpen;
    ERect               m_ERect;
    h_Editor*           m_Editor;
  protected:
    h_Descriptor*       m_Descriptor;
    h_Parameters*       m_Parameters;
    h_Programs*         m_Programs;
    h_Rect              m_EditorRect;
  public:
    h_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor);
    virtual ~h_Instance();
    inline h_Rect getEditorRect(void) { return m_EditorRect; }
  private:
    void            _sendMidiClear(void);
    void            _sendMidiAll(void);
    VstIntPtr       vst_dispatcher(VstInt32 opCode, VstInt32 index, VstIntPtr value, void* ptr, float opt);
    float           vst_getParameter(VstInt32 index);
    void            vst_setParameter(VstInt32 index, float value);
    void            vst_processReplacing(float** inputs, float** outputs,VstInt32 sampleFrames);
    void            vst_processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames);
  public:
    virtual int     getPlayState(void)      { return m_PlayState; }
    virtual double  getSamplePos(void)      { return m_SamplePos; }
    virtual double  getSampleRate(void)     { if (m_SampleRate==0) updateTime(); return m_SampleRate; /*host_GetSampleRate();*/ }
    virtual double  getBeatPos(void)        { return m_BeatPos; }
    virtual double  getTempo(void)          { return m_Tempo; }
    virtual int     getCurrentProgram(void) { return m_CurrentProgram; }
    //virtual h_Rect  getEditorRect(void)     { return m_EditorRect; }
  public:
    virtual void    transferParameters(void);
    virtual void    transferProgram(int a_Index);
    virtual void    saveProgram(int a_Index);
    virtual void    notifyParameter(h_Parameter* aParameter);
    virtual void    notifyResize(int aWidth, int aHeight);
    virtual void    updateTime(void);
    virtual void    sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3);

};

//----------------------------------------------------------------------
#endif

