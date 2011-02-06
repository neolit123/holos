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
#ifndef h_Vst_included
#define h_Vst_included
//----------------------------------------------------------------------

#include "extern/vst/aeffect.h"
#include "extern/vst/aeffectx.h"
//#include "extern/vst/vstfxstore.h"

#include "lib/h_Rect.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class h_Host : public h_Host_Base
{
  friend class h_Instance;
  private:
    audioMasterCallback m_AudioMaster;
    AEffect*            m_AEffect;
  public:
    inline audioMasterCallback getAudioMaster(void) { return m_AudioMaster; }
    inline AEffect* getAEffect(void) { return m_AEffect; }
  public:
    h_Host(audioMasterCallback audioMaster, AEffect* m_AEffect);
    ~h_Host();
  private:
    VstInt32      vst_Version(void);
    VstInt32      vst_CurrentId(void);
    void          vst_Automate(VstInt32 param, float val);
    void          vst_Idle(void);
    VstTimeInfo*  vst_GetTime(VstInt32 filter);
    bool          vst_ProcessEvents(VstEvents* events);
    bool          vst_IOChanged(void);
    bool          vst_SizeWindow(int aWidth, int aHeight);
    float         vst_GetSampleRate(void); // float?
    bool          vst_GetVendorString(char* buffer);
    bool          vst_GetProductString(char* buffer);
    int           vst_GetVendorVersion(void);
    bool          vst_CanDo(const char* text);
    bool          vst_BeginEdit(VstInt32 index);
    bool          vst_EndEdit(VstInt32 index);
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

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

class h_Instance : public h_Instance_Base
{
  friend class h_Format;
  private:
    h_Host*             m_Host;
    h_Descriptor*       m_Descriptor;
    audioMasterCallback m_AudioMaster;
    AEffect*            m_AEffect;
    VstTimeInfo*        m_TimeInfo;
    h_VstEvents         m_MidiEventList;
    char                m_ProgramName[kVstMaxProgNameLen];
    int                 m_NumPrograms;
    VstInt32            m_CurrentProgram;
    double              m_SampleRate;
    long                m_BlockSize;
    int                 m_PlayState;
    double              m_SamplePos;
    double              m_BeatPos;
    double              m_Tempo;
    h_Rect              m_EditorRect;
    bool                m_EditorIsOpen;
    ERect               m_ERect;
    //h_Parameters        m_Parameters;
    h_Parameters*       m_Parameters;
  public:
    // accessors
    inline int    getPlayState(void)                    { return m_PlayState; }
    inline double getSamplePos(void)                    { return m_SamplePos; }
    inline double getSampleRate(void)                   { if (m_SampleRate==0) updateTime(); return m_SampleRate; /*host_GetSampleRate();*/ }
    inline double getBeatPos(void)                      { return m_BeatPos; }
    inline double getTempo(void)                        { return m_Tempo; }
    inline int    getCurrentProgram(void)               { return m_CurrentProgram; }
    inline h_Rect getEditorRect(void)                   { return m_EditorRect; }
  public:
    h_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor);
    virtual ~h_Instance();
  public:
    void          appendParameter(h_Parameter* a_Parameter);
    void          deleteParameters(void);
    void          initParameters(void);
    void          prepareParameters(void);
    void          notifyParameter(h_Parameter* aParameter);
    void          notifyResize(int aWidth, int aHeight);
    void          updateTime(void);
    void          sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3);
  private:
//    inline void   _clear_aeFlags(void)                  { m_AEffect->flags = 0; }
//    inline void   _clear_aeFlag(int aFlag)              { m_AEffect->flags &= ~aFlag; }
//    inline bool   _get_aeFlag(int aFlag)                { return (m_AEffect->flags|aFlag); }
//    inline void   _set_aeFlag(int aFlag)                { m_AEffect->flags |= aFlag; }
//    inline void   _set_aeFlag(int aFlag, bool aState)   { if (aState) _set_aeFlag(aFlag); else _clear_aeFlag(aFlag); }
//  //
//    inline void   _setUniqueID(int aID)                  { m_AEffect->uniqueID = aID; }         // Must be called to set the plug-ins unique ID!
//    inline void   _setNumInputs(int aNum)                { m_AEffect->numInputs = aNum; }       // set the number of inputs the plug-in will handle. For a plug-in which could change its IO configuration, this number is the maximun available inputs.
//    inline void   _setNumOutputs(int aNum)               { m_AEffect->numOutputs = aNum; }      // set the number of outputs the plug-in will handle. For a plug-in which could change its IO configuration, this number is the maximun available ouputs.
//    inline void   _setInitialDelay(int aDelay)           { m_AEffect->initialDelay = aDelay; }  // use to report the plug-in's latency (Group Delay)
//    inline void   _setVersion(int aVer)                  { m_AEffect->version = aVer; }
//    inline void   _setNumPrograms(int aNum)              { m_AEffect->numPrograms = aNum; }
//    inline void   _setNumParams(int aNum)                { m_AEffect->numParams = aNum; }
//  //
//    inline void   _canProcessReplacing(bool aState=true) { _set_aeFlag(effFlagsCanReplacing,aState); }        // tells that processReplacing() could be used. Mandatory in VST 2.4!
//    inline void   _canDoubleReplacing(bool aState=true)  { _set_aeFlag(effFlagsCanDoubleReplacing,aState); }  // tells that processDoubleReplacing() is implemented.
//    inline void   _programsAreChunks(bool aState=false)  { _set_aeFlag(effFlagsProgramChunks,aState); }       // program data is handled in formatless chunks (using getChunk-setChunks)
//    inline void   _isSynth(bool aState=false)            { _set_aeFlag(effFlagsIsSynth,aState); }
//    inline void   _hasEditor(bool aState=false)          { _set_aeFlag(effFlagsHasEditor,aState); }
//    inline void   _noSoundInStop(bool aState=true)       { _set_aeFlag(effFlagsNoSoundInStop,aState); }
    void          sendMidiClear(void);
    void          sendMidiAll(void);
    VstIntPtr     vst_dispatcher(VstInt32 opCode, VstInt32 index, VstIntPtr value, void* ptr, float opt);
    float         vst_getParameter(VstInt32 index);
    void          vst_setParameter(VstInt32 index, float value);
    void          vst_processReplacing(float** inputs, float** outputs,VstInt32 sampleFrames);
    void          vst_processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames);
//    VstInt32      host_Version(void);
//    VstInt32      host_CurrentId(void);
//    void          host_Automate(VstInt32 param, float val);
//    void          host_Idle(void);
//    VstTimeInfo*  host_GetTime(VstInt32 filter);
//    bool          host_ProcessEvents(VstEvents* events);
//    bool          host_IOChanged(void);
//    bool          host_SizeWindow(int aWidth, int aHeight);
//    float         host_GetSampleRate(void); // float?
//    bool          host_BeginEdit(VstInt32 index);
//    bool          host_EndEdit(VstInt32 index);

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class h_Format //: public h_Format_Base
{
  private:
    bool  m_Initialized;
  public:
    h_Format();
    ~h_Format();
  public:
    //h_Descriptor* getDescriptor(void);
    h_Descriptor* createDescriptor(void);
    h_Instance*   createInstance(h_Host* a_Host, h_Descriptor* a_Descriptor);
    AEffect*      entrypoint(audioMasterCallback audioMaster);
    h_String      getName(void) { return "vst"; }
  private:
    static VstIntPtr  vst_dispatcher_callback(AEffect* ae, VstInt32 opCode, VstInt32 index, VstIntPtr value, void* ptr, float opt);
    static float      vst_getParameter_callback(AEffect* ae, VstInt32 index);
    static void       vst_setParameter_callback(AEffect* ae, VstInt32 index, float value);
    static void       vst_processReplacing_callback(AEffect* ae, float** inputs, float** outputs, VstInt32 sampleFrames);
    static void       vst_processDoubleReplacing_callback(AEffect* ae, double** inputs, double** outputs, VstInt32 sampleFrames);
};

//----------------------------------------------------------------------
#endif
