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
#ifndef h_Host_Vst_included
#define h_Host_Vst_included
//----------------------------------------------------------------------

#include "extern/vst/aeffect.h"
#include "extern/vst/aeffectx.h"
//#include "extern/vst/vstfxstore.h"
//#include "src/h_Rect.h"

class h_Host : public h_Host_Base
{
  friend class h_Instance;
  private:
    audioMasterCallback m_AudioMaster;
    AEffect*            m_AEffect;
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
  public:
    inline audioMasterCallback getAudioMaster(void) { return m_AudioMaster; }
    inline AEffect* getAEffect(void) { return m_AEffect; }
  public:
    h_Host(audioMasterCallback audioMaster, AEffect* m_AEffect);
    ~h_Host();
    virtual h_String getName(void);
};

//----------------------------------------------------------------------
#endif

