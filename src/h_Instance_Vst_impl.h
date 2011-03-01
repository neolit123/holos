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
#ifndef h_Instance_Vst_impl_included
#define h_Instance_Vst_impl_included
//#ifdef h_Vst_included
//----------------------------------------------------------------------

//#define trace_vst(x) trace(x)
#define trace_vst(x) {}

//----------------------------------------------------------------------

h_Instance::h_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
//: h_Instance_Base(a_Descriptor)
  {
    m_Host            = a_Host;
    m_Descriptor      = a_Descriptor;
    m_Parameters      = m_Descriptor->getParameters();
    m_Programs        = m_Descriptor->getPrograms();
    m_AudioMaster     = a_Host->getAudioMaster();
    m_AEffect         = a_Host->getAEffect();
    h_Memset(&m_MidiEventList,0,sizeof(h_VstEvents));
    m_PlayState       = 0;
    m_SamplePos       = 0;
    m_SampleRate      = 0;
    m_BeatPos         = 0;
    m_Tempo           = 0;
    m_BlockSize       = 0;
    m_CurrentProgram  = 0;
    m_EditorRect      = m_Descriptor->m_EditorRect;// h_Rect(0,0,320,240);
    m_EditorIsOpen    = false;
    m_Editor          = H_NULL;
  }

//----------

h_Instance::~h_Instance()
  {
    if (m_AEffect) h_Free(m_AEffect); // !!!!!  created in entrypoint()
    if (m_Host) delete m_Host;        // !!!!!  created in entrypoint()
    if (m_Descriptor) delete m_Descriptor;        // !!!!!  created in entrypoint()
  }

//----------------------------------------------------------------------

// create parameters from descriptor

//void h_Instance::initParameters(void)
//  {
//    //int numpar = m_Descriptor->m_NumParams;
//    int numpar = m_Descriptor->m_Parameters.size();
//    for (int i=0; i<numpar; i++)
//    {
//      h_String name  = m_Descriptor->m_Parameters[i]->getName();
//      h_String label = m_Descriptor->m_Parameters[i]->getLabel();
//      float value    = m_Descriptor->m_Parameters[i]->getInternal();
//      int flags      = m_Descriptor->m_Parameters[i]->getFlags();
//      // todo: param type flag
//      h_Parameter* par = new h_Parameter();
//      //switch (m_Descriptor->m_Params[i].m_Type)
//      //{
//      //  case pt_None:
//      //    par = new h_Parameter(name,label,flags,value);
//      //    break;
//      //}
//      appendParameter(par);
//
//    }
//  }

//----------

// call this to initializa your parameters before yuo use them.
// it will call do_HandleParameter for all od them, so that you
// have a chance to grab the 'transformed' variables in
// your instance.

void h_Instance::transferParameters(void)
  {
    int num = m_Parameters->size();
    for (int i=0; i<num; i++)
    {
      h_Parameter* par = m_Parameters->item(i);//[i];
      do_HandleParameter(par);
      //notifyParameter(par); // ?? do we need to tell the host our values?
    }
  }

//----------

void h_Instance::transferProgram(int a_Index)
  {
    if (a_Index >= m_Programs->size()) return;
    int num = m_Parameters->size();
    h_Program* prog = m_Programs->item(a_Index);
    for (int i=0; i<num; i++)
    {
      h_Parameter* par = m_Parameters->item(i);//[i];
      float val = prog->getValue(i);
      par->setInternal(val);
    }
  }

void h_Instance::saveProgram(int a_Index)
  {
    h_Program* prog = m_Programs->item(a_Index);
    for (int i=0; i<m_Parameters->size(); i++)
    {
      prog->setValue( i, m_Parameters->item(i)->getInternal() );
    }
  }

//void setupPrograms()
//  {
//    int num = mPrograms.size();
//    if (num>0) { setNumPrograms(num); } // vst
//    else
//    {
//      axProgram* prog = createDefaultProgram();
//      appendProgram(prog);
//      setNumPrograms(1);
//    }
//  }

//----------

// called when a widget connected to a parameter is changed (from editor)

// (we must notify the host)
// and call do_HandleParameter


void h_Instance::notifyParameter(h_Parameter* a_Parameter)
  {
    //h_Instance_Base::notifyParameter(a_Parameter);
    int index = a_Parameter->getIndex();
    float value = a_Parameter->getInternal();//getValue();
    //trace("h_Instance::notifyParameter(" << index << "," << value <<  ")");
    m_Host->vst_Automate(index,value); // setParameterAutomated();
    do_HandleParameter(a_Parameter);
  }

//----------

// called when we want the host to resize the window

void h_Instance::notifyResize(int a_Width, int a_Height)
  {
    //h_Instance_Base::notifyResize(a_Width,a_Height);
    //m_EditorRect.w = aWidth;
    //m_EditorRect.h = aHeight;
    //host_SizeWindow(aWidth, aHeight); // vst
    //m_Host->vst_SizeWindow(a_Width,a_Height); // vst
  }

//----------

//enum VstTimeInfoFlags
//{
//	kVstTransportChanged     = 1,		///< indicates that play, cycle or record state has changed
//	kVstTransportPlaying     = 1 << 1,	///< set if Host sequencer is currently playing
//	kVstTransportCycleActive = 1 << 2,	///< set if Host sequencer is in cycle mode
//	kVstTransportRecording   = 1 << 3,	///< set if Host sequencer is in record mode
//	kVstAutomationWriting    = 1 << 6,	///< set if automation write mode active (record parameter changes)
//	kVstAutomationReading    = 1 << 7,	///< set if automation read mode active (play parameter changes)

//	kVstNanosValid           = 1 << 8,	///< VstTimeInfo::nanoSeconds valid
//	kVstPpqPosValid          = 1 << 9,	///< VstTimeInfo::ppqPos valid
//	kVstTempoValid           = 1 << 10,	///< VstTimeInfo::tempo valid
//	kVstBarsValid            = 1 << 11,	///< VstTimeInfo::barStartPos valid
//	kVstCyclePosValid        = 1 << 12,	///< VstTimeInfo::cycleStartPos and VstTimeInfo::cycleEndPos valid
//	kVstTimeSigValid         = 1 << 13,	///< VstTimeInfo::timeSigNumerator and VstTimeInfo::timeSigDenominator valid
//	kVstSmpteValid           = 1 << 14,	///< VstTimeInfo::smpteOffset and VstTimeInfo::smpteFrameRate valid
//	kVstClockValid           = 1 << 15	///< VstTimeInfo::samplesToNextClock valid
//};

void h_Instance::updateTime(void)
  {
    //m_TimeInfo   = host_GetTime( kVstPpqPosValid + kVstTempoValid );
    m_TimeInfo   = m_Host->vst_GetTime( kVstPpqPosValid + kVstTempoValid );
    m_PlayState  = m_TimeInfo->flags & 0xff;
    m_SamplePos  = m_TimeInfo->samplePos;
    m_SampleRate = m_TimeInfo->sampleRate;
    m_BeatPos    = m_TimeInfo->ppqPos;
    m_Tempo      = m_TimeInfo->tempo;
  }

//----------

// append midi event to list
// the entire list will be sent to the host at the end of process()
// with _sendMidiAll().
// we probably shouldn't call this in places that can (theoretically)
// happen at the same time (in another thread) as our process()

void h_Instance::sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3)
  {
    int num = m_MidiEventList.numEvents;
    VstMidiEvent* event = (VstMidiEvent*)( m_MidiEventList.events[ num ] );
    event->type         = kVstMidiType;
    event->deltaFrames  = offset;
    event->midiData[0]  = msg1;
    event->midiData[1]  = msg2;
    event->midiData[2]  = msg3;
    event->midiData[3]  = 0;
    event->byteSize     = sizeof(VstMidiEvent);
    event->flags        = 0;
    event->noteLength   = 0;
    event->noteOffset   = 0;
    event->detune       = 0;
    m_MidiEventList.numEvents+=1;
  }

//----------------------------------------------------------------------
//
// internal (midi)
//
//----------------------------------------------------------------------

// these are called from process()

// [internal]
void h_Instance::_sendMidiClear(void)
  {
    m_MidiEventList.numEvents = 0;
  }

//----------

// send all midi event in list to host (and clear the list)

// [internal]
void h_Instance::_sendMidiAll(void)
  {
    int num = m_MidiEventList.numEvents;
    if( num>0 )
    {
      h_VstEvents* events = &m_MidiEventList;
      //host_ProcessEvents( (VstEvents*)events );
      m_Host->vst_ProcessEvents( (VstEvents*)events );
      //_sendMidiClear();
    }
  }

//----------------------------------------------------------------------
//
// vst
//
//----------------------------------------------------------------------

// called from host, via the dispatcher callbacks in AEffect
// which we returned from main(), and prepared in h_Format::entrypoint()

VstIntPtr h_Instance::vst_dispatcher(VstInt32 opCode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
  {
    VstIntPtr result = 0;
    switch (opCode)
    {

      //----------

      // called when plug-in is initialized

      case effOpen: // 00
        trace_vst("vst dispatcher: effOpen");
        do_HandleState(is_Open);
        break;

      //----------

      // called when plug-in will be released

      case effClose: // 01
        trace_vst("vst dispatcher: effClose");
        do_HandleState(is_Close);
        //inst->vst_dispatcher(opCode,index,value,ptr,opt);
        //delete inst;
        // desc, host? (in h_Instance_Vst?)
        result = 1;
        break;

      //----------

      // set the current program

      case effSetProgram: // 02
        trace_vst("vst dispatcher: effSetProgram");
        if (m_Programs->size() > 0)
        {
          do_PreProgram(m_CurrentProgram);
          #ifndef H_PROGRAMS_NOAUTOSAVE
          saveProgram(m_CurrentProgram);
          #endif
          m_CurrentProgram = (VstInt32)value;
          transferProgram(m_CurrentProgram);
          do_SetProgram(m_CurrentProgram);
          transferParameters();
        }
        break;

      //----------

      // return the index to the current program

      case effGetProgram: // 03
        //trace_vst("vst dispatcher: effGetProgram");                           // too many"!!
        result = m_CurrentProgram;
        break;

      //----------

      // stuff the name field of the current program with name.
      // Limited to kVstMaxProgNameLen.

      case effSetProgramName: // 04
        trace_vst("vst dispatcher: effSetProgramName");
        if (m_Programs->size() > 0)
        {
          m_Programs->item(m_CurrentProgram)->setName((char*)ptr);
          result = 1;
        }
        break;

      //----------

      // stuff name with the name of the current program.
      // Limited to kVstMaxProgNameLen.

      case effGetProgramName: // 05
        trace_vst("vst dispatcher: effGetProgramName");
        if (m_Programs->size() > 0)
        {
          h_Strncpy( (char*)ptr, m_Programs->item(m_CurrentProgram)->getName(), kVstMaxProgNameLen );
          result = 1;
        }
        else *(char*)ptr = '\0';
        break;

      //----------

      // Stuff label with the units in which parameter index is displayed
      // (i.e. "sec", "dB", "type", etc...).
      // Limited to kVstMaxParamStrLen.

      case effGetParamLabel: // 06
        trace_vst("vst dispatcher: effGetParamLabel");
        m_Parameters->item(index)->getLabel((char*)ptr);
        //h_Strcpy((char*)ptr,"label");
        break;

      //----------

      // stuff text with a string representation of the value of parameter index.
      // ("0.5", "-3", "PLATE", etc...)
      // Limited to kVstMaxParamStrLen.

      case effGetParamDisplay: // 07
        trace_vst("vst dispatcher: effGetParamDisplay");
        m_Parameters->item(index)->getDisplay((char*)ptr);
        break;

      //----------

      // stuff text with the name of parameter index.
      // ("Time", "Gain", "RoomType", etc...)
      // Limited to kVstMaxParamStrLen.

      case effGetParamName: // 08
        trace_vst("vst dispatcher: effGetParamName");
        m_Parameters->item(index)->getName((char*)ptr);
        break;

      //----------

      //case effGetVu:  // 09 // deprecated
      //  trace_vst("vst dispatcher: effGetVu");
      //  break;

      //----------

      // called when the sample rate changes (always in a suspend state)

      case effSetSampleRate: // 10
        trace_vst("vst dispatcher: effSetSampleRate");
        m_SampleRate = opt;
        do_HandleState(is_Rate);
        break;

      //----------

      // called when the maximun block size changes (always in a suspend state).
      // note that the sampleFrames in process calls could be smaller than
      // this block size, but NOT bigger.

      case effSetBlockSize: // 11
        trace_vst("vst dispatcher: effSetBlockSize");
        m_BlockSize = (VstInt32)value;
        do_HandleState(is_Block);
        break;

      //----------

      // suspend: called when plug-in is switched to off
      // resume:  called when plug-in is switched to on

      case effMainsChanged: // 12
        trace_vst("vst dispatcher: effMainsChanged");
        if (!value) do_HandleState(is_Suspend);
        else do_HandleState(is_Resume);
        break;

      //----------

      case effEditGetRect: // 13
        trace_vst("vst dispatcher: effEditGetRect");
        if (m_Descriptor->m_Flags&df_HasEditor)
        {
          //ERect rect;
          m_ERect.left     = m_EditorRect.x;
          m_ERect.top      = m_EditorRect.y;
          m_ERect.right    = m_EditorRect.x2(); // +1
          m_ERect.bottom   = m_EditorRect.y2(); // +1
          *(ERect**)ptr = &m_ERect;
          result = 1;
        }
        break;

      //----------

      case effEditOpen: // 14
        trace_vst("vst dispatcher: effEditOpen");
        if (m_Descriptor->m_Flags&df_HasEditor)
        {
          if (!m_EditorIsOpen)
          {
            //do_OpenEditor(&ptr);
            m_Editor = (h_Editor*)do_OpenEditor(ptr);                   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            m_EditorIsOpen = true;
            result = 1;
          }
        }
        break;

      //----------

      case effEditClose: // 15
        trace_vst("vst dispatcher: effEditClose");
        if (m_Descriptor->m_Flags&df_HasEditor)
        {
          if (m_EditorIsOpen)
          {
            do_CloseEditor();
            m_EditorIsOpen = false;
            m_Editor = H_NULL;
            result = 1; // ??? test
          }
        }
        break;

      //----------

      //case effEditDraw:  // 16 // deprecated
      //  trace_vst("vst dispatcher: effEditDraw");
      //  break;

      //case effEditMouse:  // 17 // deprecated
      //  trace_vst("vst dispatcher: effEditMouse");
      //  break;

      //case effEditKey:  // 18 // deprecated
      //  trace_vst("vst dispatcher: effEditKey");
      //  break;

      //----------

      case effEditIdle: // 19
        //trace_vst("vst dispatcher: effEditIdle");
        if (m_Descriptor->m_Flags&df_HasEditor)
        {
          if (!m_EditorIsOpen)
          {
            do_IdleEditor();
          }
        }
        break;

      //----------

      //case effEditTop:  // 20 // deprecated
      //  trace_vst("vst dispatcher: effEditTop");
      //  break;

      //case effEditSleep:  // 21 // deprecated
      //  trace_vst("vst dispatcher: effEditSleep");
      //  break;

      //----------

      case DECLARE_VST_DEPRECATED (effIdentify): // 22
        trace_vst("vst dispatcher: effIdentify");
        result = CCONST('N','v','E','f');
        break;

      //----------

      // VstInt32 AudioEffect::getChunk (void** data, bool isPreset)
      // data - should point to the newly allocated memory block containg state data. You can savely release it in next suspend/resume call.
      // isPreset - true when saving a single program, false for all programs
      // note: If your plug-in is configured to use chunks (see AudioEffect::programsAreChunks),
      //  the Host will ask for a block of memory describing the current plug-in state for saving.
      // To restore the state at a later stage, the same data is passed back to AudioEffect::setChunk.
      // Alternatively, when not using chunk, the Host will simply save all parameter values.

      // host stores plug-in state. Returns the size in bytes of the chunk
      // (plug-in allocates the data array)
      //trace("axFormatVst.dispatcher :: effGetChunk");
      //v = getChunk((void**)ptr, index ? true : false);

      case effGetChunk: // 23
        trace_vst("vst dispatcher: effGetChunk");
        break;

      //----------

      //VstInt32 AudioEffect::setChunk (void* data, VstInt32 byteSize, bool isPreset)
      //data - pointer to state data (owned by Host)
      //byteSize - size of state data
      //isPreset - true when restoring a single program, false for all programs

      // host restores plug-in state
      //trace("axFormatVst.dispatcher :: effSetChunk");
      //v = setChunk(ptr, (VstInt32)value, index ? true : false);

      case effSetChunk: // 24
        trace_vst("vst dispatcher: effSetChunk");
        break;

      //---------- aeffectx.h (vst 2)

      case effProcessEvents: // 25
        //trace_vst("vst dispatcher: effProcessEvents"); // called before each processblock (too much, like processblock)
        {
          if (m_Descriptor->m_Flags&df_ReceiveMidi)
          {
            VstEvents* ev = (VstEvents*)ptr;
            int num = ev->numEvents;
            for (int i=0; i<num; i++)
            {
              VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
              if (event->type==kVstMidiType)
              {
                do_HandleMidi( event->deltaFrames, event->midiData[0], event->midiData[1], event->midiData[2] );
              } //=miditype
            } //numevents
          }
          // sort?
        }
        result = 1;
        break;

      //----------

      case effCanBeAutomated: // 26
        trace_vst("vst dispatcher: effCanBeAutomated");
        if ( m_Parameters->item(index)->getFlags() & pf_Automate ) result = 1;
        break;

      //----------

      case effString2Parameter: // 27
        trace_vst("vst dispatcher: effString2Parameter");
        break;

      //----------

      //case effGetNumProgramCategories: // 28 // deprecated
      //  trace_vst("vst dispatcher: eff");
      //  break;

      //----------

      case effGetProgramNameIndexed: // 29
        trace_vst("vst dispatcher: effGetProgramNameIndexed");
        if (m_Programs->size() > 0)
        {
          trace(m_Programs->item(index)->getName());
          h_Strncpy( (char*)ptr, m_Programs->item(index)->getName(), kVstMaxProgNameLen );
          result = 1;
        }
        else *(char*)ptr = '\0';
        break;

      //----------

      //case effCopyProgram: // 30 // deprecated
      //  trace_vst("vst dispatcher: eff");
      //  break;

      //case effConnectInput: // 31 // deprecated
      //  trace_vst("vst dispatcher: eff");
      //  break;

      //case effConnectOutput: // 32 // deprecated
      //  trace_vst("vst dispatcher: eff");
      //  break;

      //----------

      // struct VstPinProperties
      // {
      //   char label[kVstMaxLabelLen];	          ///< pin name
      //   VstInt32 flags;					              ///< @see VstPinPropertiesFlags
      //   VstInt32 arrangementType;		          ///< @see VstSpeakerArrangementType
      //   char shortLabel[kVstMaxShortLabelLen];	///< short name (recommended: 6 + delimiter)
      //   char future[48];				                ///< reserved for future use
      // };

      // enum VstPinPropertiesFlags
      // {
      // 	kVstPinIsActive   = 1 << 0,		///< pin is active, ignored by Host
      // 	kVstPinIsStereo   = 1 << 1,		///< pin is first of a stereo pair
      // 	kVstPinUseSpeaker = 1 << 2		///< #VstPinProperties::arrangementType is valid and can be used to get the wanted arrangement
      // };

      // enum VstSpeakerArrangementType
      // {
      // 	kSpeakerArrUserDefined = -2,///< user defined
      // 	kSpeakerArrEmpty = -1,		///< empty arrangement
      // 	kSpeakerArrMono  =  0,		///< M
      // 	kSpeakerArrStereo,			///< L R
      // 	kSpeakerArrStereoSurround,	///< Ls Rs
      // 	...
      // };

      case effGetInputProperties: // 33
        trace_vst("vst dispatcher: effGetInputProperties");
        {
          VstPinProperties* pin = (VstPinProperties*)ptr;
          char name[16];
          char num[16];
          name[0] = 0;
          h_Strcpy(name,"input ");
          h_Strcat(name, h_Itoa(num,index,3) );
          h_Strcpy(pin->label,name);
          pin->flags = 1; // active
          if ((index&1)==0) pin->flags |= 2; // first of stereo pair
          result = 1;
        }
        break;

      //----------

      case effGetOutputProperties: // 34
        trace_vst("vst dispatcher: effGetOutputProperties");
        {
          VstPinProperties* pin = (VstPinProperties*)ptr;
          char name[16];
          char num[16];
          name[0] = 0;
          h_Strcpy(name,"output ");
          h_Strcat(name, h_Itoa(num,index,3) );
          h_Strcpy(pin->label,name);
          pin->flags = 1; // active
          if ((index&1)==0) pin->flags |= 2; // first of stereo pair
          result = 1;
        }
        break;

      //----------

      // kPlugCategUnknown = 0,		  ///< Unknown, category not implemented
      // kPlugCategEffect,			    ///< Simple Effect
      // kPlugCategSynth,			      ///< VST Instrument (Synths, samplers,...)
      // kPlugCategAnalysis,			  ///< Scope, Tuner, ...
      // kPlugCategMastering,		    ///< Dynamics, ...
      // kPlugCategSpacializer,		  ///< Panners, ...
      // kPlugCategRoomFx,			    ///< Delays and Reverbs
      // kPlugSurroundFx,			      ///< Dedicated surround processor
      // kPlugCategRestoration,		  ///< Denoiser, ...
      // kPlugCategOfflineProcess,  ///< Offline Process
      // kPlugCategShell,			      ///< Plug-in is container of other plug-ins  @see effShellGetNextPlugin
      // kPlugCategGenerator,		    ///< ToneGenerator, ...

      case effGetPlugCategory: // 35
        trace_vst("vst dispatcher: effGetPlugCategory");
        //if (get_aeFlag(effFlagsIsSynth)) result = kPlugCategSynth;
        //else result = kPlugCategEffect;
        break;

      //----------

      //case effGetCurrentPosition: // 36 // deprecated
      //  trace_vst("vst dispatcher: eff");
      //  break;

      //case effGetDestinationBuffer: // 37 // deprecated
      //  trace_vst("vst dispatcher: eff");
      //  break;

      //----------

      case effOfflineNotify: // 38
        trace_vst("vst dispatcher: effOfflineNotify");
        break;

      case effOfflinePrepare: // 39
        trace_vst("vst dispatcher: effOfflinePrepare");
        break;

      case effOfflineRun: // 40
        trace_vst("vst dispatcher: effOfflineRun");
        break;

      case effProcessVarIo: // 41
        trace_vst("vst dispatcher: effProcessVarIo");
        break;

      case effSetSpeakerArrangement: // 42
        trace_vst("vst dispatcher: effSetSpeakerArrangement");
        break;

      //case effSetBlockSizeAndSampleRate: // 43 // deprecated
      //  trace_vst("vst dispatcher: eff");
      //  break;

      case effSetBypass: // 44
        trace_vst("vst dispatcher: effSetBypass");
        break;

      case effGetEffectName: // 45
        trace_vst("vst dispatcher: effGetEffectName");
        h_Strcpy((char*)ptr,m_Descriptor->m_Name.ptr());
        result = 1;
        break;

      //case effGetErrorText: // 46 // deprecated
      //  trace_vst("vst dispatcher: eff");
      //  break;

      case effGetVendorString: // 47
        trace_vst("vst dispatcher: effGetVendorString");
        h_Strcpy((char*)ptr, m_Descriptor->m_Author.ptr());
        result = 1;
        break;

      case effGetProductString: // 48
        trace_vst("vst dispatcher: effGetProductString");
        h_Strcpy((char*)ptr, m_Descriptor->m_Product.ptr());
        //v = 1;
        break;

      case effGetVendorVersion: // 49
        trace_vst("vst dispatcher: effGetVendorVersion");
        result = m_Descriptor->m_Version;;
        break;

      // case effVendorSpecific:
      //    if (index == effGetParamDisplay && ptr)
      //    {
      //      if (value>=0 && value<NUM_PARAMS)
      //      {
      //        sprintf(ptr,"%f",opt);
      //        return 0xbeef;
      //      }
      //    }

      case effVendorSpecific: // 50
        trace_vst("vst dispatcher: effVendorSpecific");
        break;

      //----------

      //namespace PlugCanDos
      //{
      //	const char* canDoSendVstEvents = "sendVstEvents"; ///< plug-in will send Vst events to Host
      //	const char* canDoSendVstMidiEvent = "sendVstMidiEvent"; ///< plug-in will send MIDI events to Host
      //	const char* canDoReceiveVstEvents = "receiveVstEvents"; ///< plug-in can receive MIDI events from Host
      //	const char* canDoReceiveVstMidiEvent = "receiveVstMidiEvent"; ///< plug-in can receive MIDI events from Host
      //	const char* canDoReceiveVstTimeInfo = "receiveVstTimeInfo"; ///< plug-in can receive Time info from Host
      //	const char* canDoOffline = "offline"; ///< plug-in supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
      //	const char* canDoMidiProgramNames = "midiProgramNames"; ///< plug-in supports function #getMidiProgramName ()
      //	const char* canDoBypass = "bypass"; ///< plug-in supports function #setBypass ()
      //}

      case effCanDo: // 51
        trace_vst("vst dispatcher: effCanDo");
        {
          char* p = (char*)ptr;
          trace_vst("effCanDo: '" << p << "'");

          bool _send = (m_Descriptor->m_Flags&df_SendMidi);
          bool _receive = (m_Descriptor->m_Flags&df_ReceiveMidi);

          if (_send)
          {
            if (!h_Strcmp(p,"sendVstEvents")) return 1;
            if (!h_Strcmp(p,"sendVstMidiEvents")) return 1;
          }
          if (_receive)
          {
            if (!h_Strcmp(p,"receiveVstEvents")) return 1;
            if (!h_Strcmp(p,"receiveVstMidiEvents")) return 1;
          }

          // plug-in can receive Time info from Host
          if (!h_Strcmp(p,"receiveVstTimeInfo")) return 1;


          //if (h_Strcmp(ptr,"offline")) return 0; // plug-in supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
          //if (h_Strcmp(ptr,"midiProgramNames")) return 0; // plug-in supports function #getMidiProgramName ()
          //if (h_Strcmp(ptr,"bypass")) return 0; // plug-in supports function #setBypass ()

          if (!h_Strcmp(p,"hasCockosExtensions"))  return 0xbeef0000;

          return 0;
        }
        break;

      //----------

      case effGetTailSize: // 52
        trace_vst("vst dispatcher: effGetTailSize");
        break;

      //case effIdle: // 53 // deprecated [energyXt2 calls this]
      //  trace_vst("vst dispatcher: eff");
      //  break;

      //case effGetIcon: // 54 // deprecated
      //  trace_vst("vst dispatcher: eff");
      //  break;

      //case effSetViewPosition: // 55 // deprecated
      //  trace_vst("vst dispatcher: eff");
      //  break;

      case effGetParameterProperties: // 56
        trace_vst("vst dispatcher: effGetParameterProperties");
        break;

      //case effKeysRequired: // 57 // deprecated
      //  trace_vst("vst dispatcher: eff");
      //  break;

      case effGetVstVersion: // 58
        trace_vst("vst dispatcher: effGetVstVersion");
        break;

      //---------- VST_2_1_EXTENSIONS

      case effEditKeyDown: // 59
        trace_vst("vst dispatcher: effEditKeyDown");
        break;

      case effEditKeyUp: // 60
        trace_vst("vst dispatcher: effEditKeyUp");
        break;

      case effSetEditKnobMode: // 61
        trace_vst("vst dispatcher: effSetEditKnobMode");
        break;

      case effGetMidiProgramName: // 62
        trace_vst("vst dispatcher: effGetMidiProgramName");
        break;

      case effGetCurrentMidiProgram: // 63
        trace_vst("vst dispatcher: effGetCurrentMidiProgram");
        break;

      case effGetMidiProgramCategory: // 64
        trace_vst("vst dispatcher: effGetMidiprogramCategory");
        break;

      case effHasMidiProgramsChanged: // 65
        trace_vst("vst dispatcher: effHasMidiProgramsChanged");
        break;

      case effGetMidiKeyName: // 66
        trace_vst("vst dispatcher: effGetMidiKeyName");
        break;

      case effBeginSetProgram: // 67
        trace_vst("vst dispatcher: effBeginSetProgram");
        break;

      case effEndSetProgram: // 68
        trace_vst("vst dispatcher: effEndSetProgram");
        break;

      //---------- VST_2_3_EXTENSIONS

      case effGetSpeakerArrangement: // 69
        trace_vst("vst dispatcher: effGetSpeakerArrangement");
        break;

      case effShellGetNextPlugin: // 70
        trace_vst("vst dispatcher: effShellGetNextPlugin");
        break;

      case effStartProcess: // 71
        trace_vst("vst dispatcher: effStartProcess");
        break;

      case effStopProcess: // 72
        trace_vst("vst dispatcher: effStopProcess");
        break;

      case effSetTotalSampleToProcess: // 73
        trace_vst("vst dispatcher: effSetTotalSampleToProcess");
        break;

      case effSetPanLaw: // 74
        trace_vst("vst dispatcher: effSetPanLaw");
        break;

      case effBeginLoadBank: // 75
        trace_vst("vst dispatcher: effBeginLoadBank");
        break;

      case effBeginLoadProgram: // 76
        trace_vst("vst dispatcher: effBeginLoadProgram");
        break;

      //---------- VST_2_4_EXTENSIONS

      case effSetProcessPrecision: // 77
        trace_vst("vst dispatcher: effSetProcessPrecision");
        break;

      case effGetNumMidiInputChannels: // 78
        trace_vst("vst dispatcher: effGetNumMidiInputChannels");
        break;

      case effGetNumMidiOutputChannels: // 79
        trace_vst("vst dispatcher: effGetNumMidiOutputChannels");
        break;

      //default:
      //  trace_vst("unhandled vst dispatcher: " << opcode << " : " << vst_opcodes[opcode]);
      //  break;

    } // switch

    return result;
    //0;//inst->vst_dispatcher(opCode,index,value,ptr,opt);
  }

//----------

// return the value of the parameter with index

float h_Instance::vst_getParameter(VstInt32 index)
  {
    return m_Parameters->item(index)->getInternal();
  }

//----------

//Parameters are the individual parameter settings the user can adjust. A VST Host can automate these
//parameters. Set parameter \e index to \e value.
//Index - Index of the parameter to change
//value - A float value between 0.0 and 1.0 inclusive
//note: Parameter values, like all VST parameters, are declared as floats with an inclusive range of
//0.0 to 1.0. How data is presented to the user is merely in the user-interface handling. This is a
//convention, but still worth regarding. Maybe the VST-Host's automation system depends on this range.

// called (from host) when a parameter is changed

void h_Instance::vst_setParameter(VstInt32 index, float value)
  {
    h_Parameter* par = m_Parameters->item(index);
    par->setInternal(value);
    do_HandleParameter(par);
    #ifndef H_NOGUI
    if (m_Editor && m_EditorIsOpen) m_Editor->notifyParameter(par);
    #endif
    //trace("todo: update editor");
  }

//----------

// process 32 bit (single precision) floats (always in a resume state)
// This process method must be provided. It takes input data,
// applies its pocessing algorithm, and then puts the
// result to the output by overwriting the output buffer.
// inputs       - An array of pointers to the data
// outputs      - An array of pointers to where the data can be written to
// sampleFrames - Number of sample frames to process
// warning! Never call any Mac OS 9 functions (or other functions which call
// into the OS) inside your audio process function! This will crash the system
// when your plug-in is run in MP (multiprocessor) mode.
// If you must call into the OS, you must use MPRemoteCall () (see Apples'
// documentation), or explicitly use functions which are documented by Apple to
// be MP safe. On Mac OS X read the system header files to be sure that you
// only call thread safe functions.

#ifdef H_MULTICHANNEL

//TODO: H_NUMINPUTS, H_NUMOUTPUTS (from descriptor)

void h_Instance::vst_processReplacing(float** aInputs, float** aOutputs, int aLength)
  {
//    //_sendMidiClear();
//    #ifdef AX_AUTOSYNC
//      updateTime();
//      if (m_PlayState&1) do_HandleTransport(mPlayState);
//    #endif
//    //int numin = m_Descriptor->m_NumInputs;
//    //int numout = m_Descriptor->m_NumOutputs;
//    m_BlockSize = sampleFrames;
//    if ( !do_ProcessBlock(inputs,outputs,sampleFrames) )
//    {
//      // save the pointers
//      // these are pointers to the _start_ of each channel buffer
//      // so we save them in our own variables,
//      // and increase them one by one when calling doProcessSample
//      int i;
//      float*  ins[H_NUMINPUTS];
//      float* outs[H_NUMOUTPUTS];
//      for( i=0; i<H_NUMINPUTS;  i++ ) ins[i]  = inputs[i];
//      for( i=0; i<H_NUMOUTPUTS; i++ ) outs[i] = outputs[i];
//      while (--sampleFrames >= 0)
//      {
//        do_ProcessSample(ins,outs);
//        for( i=0; i<H_NUMINPUTS;  i++ ) ins[i]++;
//        for( i=0; i<H_NUMOUTPUTS; i++ ) outs[i]++;
//      } //sampleflrames
//    } //process_block
//    do_PostProcess(inputs,outputs,sampleFrames);
//    _sendMidiAll();
  }

#else

//----------

void h_Instance::vst_processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
  {
    //#ifdef H_AUTOSYNC
    if (m_Descriptor->m_Flags&df_AutoSync)
    {
      updateTime();
      if (m_PlayState&1) do_HandleTransport(m_PlayState);
    }
    //#endif
    m_BlockSize = sampleFrames;
    if ( !do_ProcessBlock(inputs,outputs,sampleFrames) )
    {
      float* ins[2];
      float* outs[2];
      ins[0]  = inputs[0];
      ins[1]  = inputs[1];
      outs[0] = outputs[0];
      outs[1] = outputs[1];
      while (--sampleFrames >= 0)
      {
        do_ProcessSample(ins,outs);
        ins[0]++;   ins[1]++;
        outs[0]++;  outs[1]++;
      } //sampleflrames
    } //process_block
    do_PostProcess(inputs,outputs,sampleFrames);
    if (m_Descriptor->m_Flags&df_SendMidi)
    {
      _sendMidiAll();
      _sendMidiClear();
    }
  }

#endif // H_MULTICHANNEL

//----------

// process 64 bit (double precision) floats (always in a resume state)

void h_Instance::vst_processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
  {
    //TODO: need to convert input & output buffers to SPL, and back again
    //doProcessBlock((SPL**)aInputs,(SPL**)aOutputs,aLength);
  }

//----------------------------------------------------------------------
//#endif
#endif
