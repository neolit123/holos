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



//----------------------------------------------------------------------
#endif































#if 0
//======================================================================

#ifndef h_Format_Vst_included
#define h_Format_Vst_included
//----------------------------------------------------------------------
// included from Holos.h

#include <stdio.h>

#include "pluginterfaces/vst2.x/aeffect.h"
#include "pluginterfaces/vst2.x/aeffectx.h"

// !!!
// http://www.ohse.de/uwe/articles/gcc-attributes.html#type-may_alias
// see explaination below
typedef audioMasterCallback __may_alias audioMasterCallback_a;
//typedef void* __may_alias void_ptr_a; // (h_Defines.h)
//

#ifdef H_DEBUG
  #include "core/format/vst_dispatcher_opcodes.h"
#endif

#define MAX_REALNAME_LENGTH 256

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

//class h_Instance : public h_InstanceBase
//{
//  friend class h_Format;
//
//  private:
//    audioMasterCallback m_AudioMaster;
//    h_Descriptor*       m_Descriptor;
//
//  public:
//
//    h_Instance(h_Descriptor* a_Descriptor) : h_InstanceBase(a_Descriptor)
//      {
//        //trace("");
//        m_Descriptor = a_Descriptor;
//      }
//
//    virtual ~h_Instance()
//      {
//        //trace("");
//      }
//
//  private:
//
//    void setAudioMaster(audioMasterCallback a_AudioMaster)
//      {
//        m_AudioMaster = a_AudioMaster;
//      }
//
//  private:
//
//    VstIntPtr vst_dispatcher(VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
//      {
//        VstIntPtr result = 0;
//        switch (opcode)
//        {
//
//          case effOpen:
//            dtrace("vst dispatcher: effOpen");
//            do_HandleState(is_Open);
//            break;
//
//          case effClose:
//            dtrace("vst dispatcher: effClose");
//            do_HandleState(is_Close);
//            break;
//
//          case effSetProgram:
//            dtrace("vst dispatcher: effSetProgram");
//            break;
//
//          case effGetProgram:
//            dtrace("vst dispatcher: effGetProgram");
//            break;
//
//          case effSetProgramName:
//            dtrace("vst dispatcher: effSetProgramName");
//            break;
//
//          case effGetProgramName:
//            dtrace("vst dispatcher: effGetProgramName");
//            break;
//
//          case effGetParamLabel:
//            dtrace("vst dispatcher: effGetParamLabel");
//            break;
//
//          case effGetParamDisplay:
//            dtrace("vst dispatcher: effGetParamDisplay");
//            break;
//
//          case effGetParamName:
//            dtrace("vst dispatcher: effGetParamName");
//            break;
//
//          //case effGetVu: // deprecated
//          //  dtrace("vst dispatcher: effGetVu");
//          //  break;
//
//          case effSetSampleRate:
//            dtrace("vst dispatcher: effSetSampleRate");
//            // m_SampleRate
//            do_HandleState(is_SampleRate);
//            break;
//
//          case effSetBlockSize:
//            dtrace("vst dispatcher: effSetBlockSize");
//            // m_BlockSize
//            do_HandleState(is_BlockSize);
//            break;
//
//          case effMainsChanged:
//            dtrace("vst dispatcher: effMainsChanged");
//            //do_HandleState(is_Suspend);
//            //do_HandleState(is_Resume);
//            break;
//
//          case effEditGetRect:
//            dtrace("vst dispatcher: effEditGetRect");
//            // m_EditorRect
//            break;
//
//          case effEditOpen:
//            dtrace("vst dispatcher: effEditOpen");
//            do_OpenEditor();
//            break;
//
//          case effEditClose:
//            dtrace("vst dispatcher: effEditClose");
//            do_CloseEditor();
//            break;
//
//          //case effEditDraw: // deprecated
//          //  dtrace("vst dispatcher: effEditDraw");
//          //  break;
//
//          //case effEditMouse: // deprecated
//          //  dtrace("vst dispatcher: effEditMouse");
//          //  break;
//
//          //case effEditKey: // deprecated
//          //  dtrace("vst dispatcher: effEditKey");
//          //  break;
//
//          case effEditIdle:
//            dtrace("vst dispatcher: effEditIdle");
//            do_IdleEditor();
//            break;
//
//          //case effEditTop: // deprecated
//          //  dtrace("vst dispatcher: effEditTop");
//          //  break;
//
//          //case effEditSleep: // deprecated
//          //  dtrace("vst dispatcher: effEditSleep");
//          //  break;
//
//          //case effIdentify: // deprecated
//          //  dtrace("vst dispatcher: effIdentify");
//          //break;
//
//          case effGetChunk:
//            dtrace("vst dispatcher: effGetChunk");
//            break;
//
//          case effSetChunk:
//            dtrace("vst dispatcher: effSetChunk");
//            break;
//
//          // aeffectx.h
//
//          case effProcessEvents:
//            //dtrace("vst dispatcher: effProcessEvents"); // called before each processblocktoo much, like processblock)
//            break;
//
//          case effCanBeAutomated:
//            dtrace("vst dispatcher: effCanBeAutomated");
//            break;
//
//          case effString2Parameter:
//            dtrace("vst dispatcher: effString2Parameter");
//            break;
//
//          //case effGetNumProgramCategories:// deprecated
//          //  dtrace("vst dispatcher: eff");
//          //  break;
//
//          case effGetProgramNameIndexed:
//            dtrace("vst dispatcher: effGetProgramNameIndexed");
//            break;
//
//          //case effCopyProgram: // deprecated
//          //  dtrace("vst dispatcher: eff");
//          //  break;
//
//          //case effConnectInput: // deprecated
//          //  dtrace("vst dispatcher: eff");
//          //  break;
//
//          //case effConnectOutput: // deprecated
//          //  dtrace("vst dispatcher: eff");
//          //  break;
//
//          case effGetInputProperties:
//            dtrace("vst dispatcher: effGetInputProperties");
//            break;
//
//          case effGetOutputProperties:
//            dtrace("vst dispatcher: effGetOutputProperties");
//            break;
//
//          case effGetPlugCategory:
//            dtrace("vst dispatcher: effGetPlugCategory");
//            break;
//
//          //case effGetCurrentPosition: // deprecated
//          //  dtrace("vst dispatcher: eff");
//          //  break;
//
//          //case effGetDestinationBuffer: // deprecated
//          //  dtrace("vst dispatcher: eff");
//          //  break;
//
//          case effOfflineNotify:
//            dtrace("vst dispatcher: effOfflineNotify");
//            break;
//
//          case effOfflinePrepare:
//            dtrace("vst dispatcher: effOfflinePrepare");
//            break;
//
//          case effOfflineRun:
//            dtrace("vst dispatcher: effOfflineRun");
//            break;
//
//          case effProcessVarIo:
//            dtrace("vst dispatcher: effProcessVarIo");
//            break;
//
//          case effSetSpeakerArrangement:
//            dtrace("vst dispatcher: effSetSpeakerArrangement");
//            break;
//
//          //case effSetBlockSizeAndSampleRate: // deprecated
//          //  dtrace("vst dispatcher: eff");
//          //  break;
//
//          case effSetBypass:
//            dtrace("vst dispatcher: effSetBypass");
//            break;
//
//          case effGetEffectName:
//            dtrace("vst dispatcher: effGetEffectName");
//            //m_Descriptor->m_Name
//            break;
//
//          //case effGetErrorText: // deprecated
//          //  dtrace("vst dispatcher: eff");
//          //  break;
//
//          case effGetVendorString:
//            dtrace("vst dispatcher: effGetVendorString");
//            //m_Descriptor->m_Author
//            break;
//
//          case effGetProductString:
//            dtrace("vst dispatcher: effGetProductString");
//            //m_Descriptor->m_Product (product)
//            break;
//
//          case effGetVendorVersion:
//            dtrace("vst dispatcher: effGetVendorVersion");
//            //m_Descriptor->m_Version
//            break;
//
//          case effVendorSpecific:
//            dtrace("vst dispatcher: effVendorSpecific");
//            break;
//
//          case effCanDo:
//            dtrace("vst dispatcher: effCanDo");
//            break;
//
//          case effGetTailSize:
//            dtrace("vst dispatcher: effGetTailSize");
//            break;
//
//          //case effIdle: // deprecated [energyXt2 calls this]
//          //  dtrace("vst dispatcher: eff");
//          //  break;
//
//          //case effGetIcon: // deprecated
//          //  dtrace("vst dispatcher: eff");
//          //  break;
//
//          //case effSetViewPosition: // deprecated
//          //  dtrace("vst dispatcher: eff");
//          //  break;
//
//          case effGetParameterProperties:
//            dtrace("vst dispatcher: effGetParameterProperties");
//            break;
//
//          //case effKeysRequired: // deprecated
//          //  dtrace("vst dispatcher: eff");
//          //  break;
//
//          case effGetVstVersion:
//            dtrace("vst dispatcher: effGetVstVersion");
//            break;
//
//          // VST_2_1_EXTENSIONS
//
//          case effEditKeyDown:
//            dtrace("vst dispatcher: effEditKeyDown");
//            break;
//
//          case effEditKeyUp:
//            dtrace("vst dispatcher: effEditKeyUp");
//            break;
//
//          case effSetEditKnobMode:
//            dtrace("vst dispatcher: effSetEditKnobMode");
//            break;
//
//          case effGetMidiProgramName:
//            dtrace("vst dispatcher: effGetMidiProgramName");
//            break;
//
//          case effGetCurrentMidiProgram:
//            dtrace("vst dispatcher: effGetCurrentMidiProgram");
//            break;
//
//          case effGetMidiProgramCategory:
//            dtrace("vst dispatcher: effGetMidiprogramCategory");
//            break;
//
//          case effHasMidiProgramsChanged:
//            dtrace("vst dispatcher: effHasMidiProgramsChanged");
//            break;
//
//          case effGetMidiKeyName:
//            dtrace("vst dispatcher: effGetMidiKeyName");
//            break;
//
//          case effBeginSetProgram:
//            dtrace("vst dispatcher: effBeginSetProgram");
//            break;
//
//          case effEndSetProgram:
//            dtrace("vst dispatcher: effEndSetProgram");
//            break;
//
//          // VST_2_3_EXTENSIONS
//
//          case effGetSpeakerArrangement:
//            dtrace("vst dispatcher: effGetSpeakerArrangement");
//            break;
//
//          case effShellGetNextPlugin:
//            dtrace("vst dispatcher: effShellGetNextPlugin");
//            break;
//
//          case effStartProcess:
//            dtrace("vst dispatcher: effStartProcess");
//            break;
//
//          case effStopProcess:
//            dtrace("vst dispatcher: effStopProcess");
//            break;
//
//          case effSetTotalSampleToProcess:
//            dtrace("vst dispatcher: effSetTotalSampleToProcess");
//            break;
//
//          case effSetPanLaw:
//            dtrace("vst dispatcher: effSetPanLaw");
//            break;
//
//          case effBeginLoadBank:
//            dtrace("vst dispatcher: effBeginLoadBank");
//            break;
//
//          case effBeginLoadProgram:
//            dtrace("vst dispatcher: effBeginLoadProgram");
//            break;
//
//          // VST_2_4_EXTENSIONS
//
//          case effSetProcessPrecision:
//            dtrace("vst dispatcher: effSetProcessPrecision");
//            break;
//
//          case effGetNumMidiInputChannels:
//            dtrace("vst dispatcher: effGetNumMidiInputChannels");
//            break;
//
//          case effGetNumMidiOutputChannels:
//            dtrace("vst dispatcher: effGetNumMidiOutputChannels");
//            break;
//
//          //default:
//          //  dtrace("unhandled vst dispatcher: " << opcode << " : " << vst_opcodes[opcode]);
//          //  break;
//
//
//        }
//        return result;
//      }
//
//    //----------
//
//    float vst_getParameter(VstInt32 index)
//      {
//        trace("vst_getParameter");
//        return 0;
//        //return m_Parameters[index]->doGetValue();
//      }
//
//    //----------
//
//    void vst_setParameter(VstInt32 index, float value)
//      {
//        trace("vst_setParameter");
//        //m_Parameters[index]->doSetValue(value);
//      }
//
//    //----------
//
//    void vst_processReplacing(float** inputs, float** outputs, int sampleFrames)
//      {
//        //do_ProcessBlock(inputs,outputs,sampleFrames);
//      }
//
//    //----------
//
//    void vst_processDoubleReplacing(double** inputs, double** outputs, VstInt32 length)
//      {
//      }
//
//};

//
//----------------------------------------------------------------------
//
// format
//
//----------------------------------------------------------------------
//

class h_Format : public h_Interface
{

  friend AEffect* real_main(audioMasterCallback audioMaster);

  private: // friends (real_main)

    // static callbacks (called by vst-host)
    // pass the call over to the proper instance (AEffect.object)

    //----------

    static VstIntPtr vst_dispatcher_callback(AEffect* ae, VstInt32 opCode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
      {
        h_Instance* inst = (h_Instance*)ae->object;
        if (opCode==effClose)
        {
          inst->vst_dispatcher(opCode,index,value,ptr,opt);
          delete inst;
          //deleteInstance(inst);
          return 1;
        }
        return inst->vst_dispatcher(opCode,index,value,ptr,opt);
      }

    //----------

    static float vst_getParameter_callback(AEffect* ae, VstInt32 index)
      {
        h_Instance* inst = (h_Instance*)ae->object;
        return inst->vst_getParameter(index);
      }

    //----------

    static void vst_setParameter_callback(AEffect* ae, VstInt32 index, float value)
      {
        h_Instance* inst = (h_Instance*)ae->object;
        inst->vst_setParameter(index,value);
      }

    //----------

    static void vst_processReplacing_callback(AEffect* ae, float** inputs, float** outputs, VstInt32 sampleFrames)
      {
        h_Instance* inst = (h_Instance*)ae->object;
        inst->vst_processReplacing(inputs,outputs,sampleFrames);
      }

    //----------

    static void vst_processDoubleReplacing_callback(AEffect* ae, double** inputs, double** outputs, VstInt32 sampleFrames)
      {
        h_Instance* inst = (h_Instance*)ae->object;
        inst->vst_processDoubleReplacing(inputs,outputs,sampleFrames);
      }

    //--------------------------------------------------

  private: // protected?

    audioMasterCallback m_AudioMaster;
    AEffect             m_AEffect;
    char                m_RealName[MAX_REALNAME_LENGTH];
    //h_Descriptor*       m_Descriptor;
    //h_Instance*         m_Instance;

  public:

    h_Format() : h_Interface()
      {
        //trace("");
        m_AudioMaster = NULL;
        h_Memset(&m_AEffect,0,sizeof(AEffect));
        h_Memset(&m_RealName,0,sizeof(MAX_REALNAME_LENGTH));
      }

    virtual ~h_Format()
      {
        //trace("");
      }

    //----------

  private: // friends (h_Instance)

    inline audioMasterCallback  getAudioMaster(void) { return m_AudioMaster; }
    inline AEffect*             getAEffect(void)     { return &m_AEffect; }

    //----------

  public:

    void* init_Format(void* a_Ptr)
      {
        m_AudioMaster       = *(audioMasterCallback_a*)(&a_Ptr);    // !!!
        //TODO: host-detection

        h_Descriptor* desc = getDescriptor();
        h_Instance*   inst = createInstance(desc);
        //m_Descriptor = getDescriptor();
        //m_Instance   = createInstance(m_Descriptor);

        h_Strcpy(m_RealName,desc->m_Name);
        #ifdef H_DEBUG
          h_Strcat(m_RealName,(char*)"_debug");
        #endif

        inst->setAudioMaster(m_AudioMaster);                        // ???
        //h_Memset(&m_AEffect,0,sizeof(AEffect));
        m_AEffect.magic                   = kEffectMagic;
        m_AEffect.object                  = inst;                   //
        m_AEffect.user                    = this;                   // ???
        m_AEffect.dispatcher              = vst_dispatcher_callback;
        m_AEffect.setParameter            = vst_setParameter_callback;
        m_AEffect.getParameter            = vst_getParameter_callback;
        m_AEffect.processReplacing        = vst_processReplacing_callback;
        m_AEffect.processDoubleReplacing  = vst_processDoubleReplacing_callback;
        m_AEffect.flags                   = effFlagsCanReplacing;
        m_AEffect.version                 = 0;
        m_AEffect.uniqueID                = H_MAGIC + 0x0000;
        m_AEffect.numPrograms             = 0;
        m_AEffect.numParams               = 0;
        m_AEffect.numInputs               = 2;
        m_AEffect.numOutputs              = 2;
        m_AEffect.initialDelay            = 0;
        return &m_AEffect;
      }

};



//======================================================================
#endif // 0
