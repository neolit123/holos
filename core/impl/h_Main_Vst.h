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
#ifndef h_Main_Vst_included
#define h_Main_Vst_included
//----------------------------------------------------------------------

//int main(void)
//{
//  // holos
//  static_Debug.initialize();
//  static_Core.initialize();
//  // instance
//  h_Host* host = new h_Host(H_NULL);
//  h_Descriptor* desc = static_Base.createDescriptor(host);
//  h_Instance*   inst = static_Base.createInstance(desc);
//  // editor
//  if (desc->m_Flags & df_HasEditor)
//  {
//    h_Editor*     edit = static_Base.createEditor(inst);
//      edit->open(H_NULL);
//      edit->eventLoop();
//      edit->close();
//    delete edit;
//  }
//  delete inst;
//  delete desc;
//  delete host;
//  return 0;
//}

//----------------------------------------------------------------------

#ifdef H_LINUX
  #define _H_ASM_MAIN_SYMBOL asm ("main");
#endif

#ifdef H_WIN32
  #define _H_ASM_MAIN_SYMBOL asm ("_main");
#endif

//----------------------------------------------------------------------

// typedef audioMasterCallback __may_alias audioMasterCallback_a;

// a.k.a. createEffectInstance
AEffect*  main_plugin(audioMasterCallback audioMaster) _H_ASM_MAIN_SYMBOL
#define   main main_plugin

//----------

// main is called for each plugin instance..
//
// we assume audioMaster is always the same for all instances...
// (what if each plugin is in a separate process?)

static bool static_isFirstInstance = true;
static AEffect static_AEffect;

AEffect* main(audioMasterCallback audioMaster)
{
  if (!audioMaster(0,audioMasterVersion,0,0,0,0)) return 0; // old version

  static_Debug.initialize();
  static_Core.initialize();
  host = new h_Host(audioMaster);
  static_Base.initialize(host);



    static_Debug.initialize();
    static_Core.initialize();
  static_Base.initialize();

//    h_Host* host = new h_Host(audioMaster);
//    h_Descriptor* desc = static_Base.createDescriptor(host);
//    h_Instance* inst = static_Base.createInstance(desc);



  //void* ptr = static_Base.entrypoint(audioMaster);
  //return (AEffect*)ptr;

//
//
//
//
//  // instance
////  inst->setAudioMaster(m_AudioMaster);
//
//  h_Host* host = new h_Host(audioMaster);
//
//  h_Descriptor* desc = static_Base.createDescriptor(host);
//  h_Instance* inst = static_Base.createInstance(desc);
//  //h_Memset(&m_AEffect,0,sizeof(AEffect));
//  m_AEffect.magic                   = kEffectMagic;
//  m_AEffect.object                  = inst;                   //
//  m_AEffect.user                    = this;                   // ???
//  m_AEffect.dispatcher              = vst_dispatcher_callback;
//  m_AEffect.setParameter            = vst_setParameter_callback;
//  m_AEffect.getParameter            = vst_getParameter_callback;
//  m_AEffect.processReplacing        = vst_processReplacing_callback;
//  m_AEffect.processDoubleReplacing  = vst_processDoubleReplacing_callback;
//  m_AEffect.flags                   = effFlagsCanReplacing;
//  m_AEffect.version                 = 0;
//  m_AEffect.uniqueID                = H_MAGIC + 0x0000;
//  m_AEffect.numPrograms             = 0;
//  m_AEffect.numParams               = 0;
//  m_AEffect.numInputs               = 2;
//  m_AEffect.numOutputs              = 2;
//  m_AEffect.initialDelay            = 0;
//  return &m_AEffect;
//
//

  return H_NULL;
}

//----------------------------------------------------------------------
#endif





//audioMaster = (audioMasterCallback)ptr;
/*
this is one way to solve the warning (the other is with union):
"ISO C++ forbids casting between pointer-to-function
and pointer-to-object"

audioMaster = *(audioMasterCallback_a *)(&ptr)
where "audioMasterCallback_a" is previously defined as:
typedef audioMasterCallback __may_alias audioMasterCallback_a;
^ in axFormatVst.h

same for the other way around:
typedef void* __may_alias void_ptr_a;
^ in axDefines.h
*/
//audioMaster = *(audioMasterCallback_a *)(&ptr);
