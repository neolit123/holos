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
#ifndef h_Format_Vst_included
#define h_Format_Vst_included
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
    h_String      getName(void);
  private:
    static VstIntPtr  vst_dispatcher_callback(AEffect* ae, VstInt32 opCode, VstInt32 index, VstIntPtr value, void* ptr, float opt);
    static float      vst_getParameter_callback(AEffect* ae, VstInt32 index);
    static void       vst_setParameter_callback(AEffect* ae, VstInt32 index, float value);
    static void       vst_processReplacing_callback(AEffect* ae, float** inputs, float** outputs, VstInt32 sampleFrames);
    static void       vst_processDoubleReplacing_callback(AEffect* ae, double** inputs, double** outputs, VstInt32 sampleFrames);
};


//----------------------------------------------------------------------
#endif

