#ifndef h_Vst_impl_included
#define h_Vst_impl_included
#ifdef h_Vst_included
//----------------------------------------------------------------------

//#define libtrace(x) trace(x)
#define libtrace(x)   {}

#include "extern/vst/aeffect.h"
#include "extern/vst/aeffectx.h"
//#include "extern/vst/vstfxstore.h"

//
//----------------------------------------------------------------------
//
// host
//
//----------------------------------------------------------------------
//

// TODO:
// move all methods from instance that uses audioMaster to host

h_Host::h_Host(audioMasterCallback audioMaster, AEffect* a_AEffect)
//: h_Host_Base()
  {
    //NOTE: AEffect has not been prepared yet..
    m_AudioMaster = audioMaster;
    m_AEffect = a_AEffect;
  }

//----------

h_Host::~h_Host()
  {
  }

//
//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------
//

h_Instance::h_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
//: h_Instance_Base(a_Descriptor)
  {
    m_Host = a_Host;
    m_Descriptor = a_Descriptor;
    m_AudioMaster = a_Host->getAudioMaster();
    m_AEffect = a_Host->getAEffect();
    h_Memset(&m_MidiEventList,0,sizeof(h_VstEvents));
    m_PlayState = 0;
    m_SamplePos = 0;
    m_SampleRate = 0;
    m_BeatPos = 0;
    m_Tempo = 0;
    m_BlockSize = 0;
    m_CurrentProgram = 0;
    m_EditorRect = m_Descriptor->m_EditorRect;// h_Rect(0,0,320,240);
    m_EditorIsOpen = false;
    initParameters();

  }

//----------

h_Instance::~h_Instance()
  {
    #ifndef H_NOAUTODELETE
      deleteParameters();
    #endif
    if (m_AEffect) h_Free(m_AEffect); // !!!!!  created in entrypoint()
    if (m_Host) delete m_Host;        // !!!!!  created in entrypoint()
  }

//----------------------------------------
// public methods
//----------------------------------------

void h_Instance::appendParameter(h_Parameter* a_Parameter)
  {
    int index = m_Parameters.size();
    a_Parameter->setIndex(index);
    m_Parameters.append(a_Parameter);
  }

//----------

void h_Instance::deleteParameters(void)
  {
    for (int i=0; i<m_Parameters.size(); i++) delete m_Parameters[i];
  }

//----------

// create parameters from descriptor

void h_Instance::initParameters(void)
  {
    int numpar = m_Descriptor->m_NumParams;
    for (int i=0; i<numpar; i++)
    {
      h_String name = m_Descriptor->m_Params[i].m_Name;
      h_String label = m_Descriptor->m_Params[i].m_Label;
      float value = m_Descriptor->m_Params[i].m_Value;
      int flags = m_Descriptor->m_Params[i].m_Flags;
      // todo: param type flag
      h_Parameter* par = new h_Parameter(name,label,value,flags);
      appendParameter(par);

    }
  }

//----------

// call do_HandleParameter for all

void h_Instance::prepareParameters(void)
  {
    int num = m_Parameters.size();
    for (int i=0; i<num; i++)
    {
      h_Parameter* par = m_Parameters[i];
      do_HandleParameter(par);
    }
  }

//----------

//virtual void setupPrograms()
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

// called when parameter has changed (from editor)
// (we must notify the host)

void h_Instance::notifyParameter(h_Parameter* aParameter)
  {
    int index = aParameter->getIndex();
    float value = aParameter->do_GetValue();//getValue();
    host_Automate(index,value); // setParameterAutomated();
  }

//----------

// called when we want the host to resize the window

void h_Instance::notifyResize(int aWidth, int aHeight)
  {
    //m_EditorRect.w = aWidth;
    //m_EditorRect.h = aHeight;
    host_SizeWindow(aWidth, aHeight); // vst
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
    m_TimeInfo   = host_GetTime( kVstPpqPosValid + kVstTempoValid );
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

//----------------------------------------
// internal (midi)
//----------------------------------------

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
      host_ProcessEvents( (VstEvents*)events );
      //_sendMidiClear();
    }
  }

//----------------------------------------
// vst
//----------------------------------------

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
        libtrace("vst dispatcher: effOpen");
        do_HandleState(is_Open);
        break;

      //----------

      // called when plug-in will be released

      case effClose: // 01
        libtrace("vst dispatcher: effClose");
        do_HandleState(is_Close);
        //inst->vst_dispatcher(opCode,index,value,ptr,opt);
        //delete inst;
        // desc, host? (in h_Instance_Vst?)
        result = 1;
        break;

      //----------

      // set the current program

      case effSetProgram: // 02
        libtrace("vst dispatcher: effSetProgram");
        //do_PreProgram(m_CurrentProgram);
        ////#ifndef AX_NOAUTOSAVE_PROGRAMS
        ////saveProgram( getCurrentProgram() );
        ////#endif
        m_CurrentProgram = (VstInt32)value;
        //_loadProgram(m_CurrentProgram);
        ////if (mPrograms.size()>0)
        ////{
        ////  axProgram* prog = mPrograms[mCurrentProgram];
        ////  int num = mParameters.size();
        ////  for (int i=0; i<num; i++)
        ////  {
        ////    float val = prog->getValue(i);
        ////    mParameters[i]->doSetValue(val,true);
        ////  }
        //// let plugin know the program have changed
        //do_SetProgram(m_CurrentProgram);
        ////}
        break;

      //----------

      // return the index to the current program

      case effGetProgram: // 03
        libtrace("vst dispatcher: effGetProgram");
        result = m_CurrentProgram;
        break;

      //----------

      // stuff the name field of the current program with name.
      // Limited to kVstMaxProgNameLen.

      case effSetProgramName: // 04
        libtrace("vst dispatcher: effSetProgramName");
        //if (m_Programs.size() > 0)
        //{
        //  m_Programs[m_CurrentProgram]->setName( (char*)ptr );
        //} else *(char*)ptr = '\0';
        break;

      //----------

      // stuff name with the name of the current program.
      // Limited to kVstMaxProgNameLen.

      case effGetProgramName: // 05
        libtrace("vst dispatcher: effGetProgramName");
        //if (m_Programs.size() > 0)
        //{
        //  strncpy( (char*)ptr, m_Programs[m_CurrentProgram]->getName().ptr(), kVstMaxProgNameLen );
        //} else *(char*)ptr = '\0';
        break;

      //----------

      // Stuff label with the units in which parameter index is displayed
      // (i.e. "sec", "dB", "type", etc...).
      // Limited to kVstMaxParamStrLen.

      case effGetParamLabel: // 06
        libtrace("vst dispatcher: effGetParamLabel");
        m_Parameters[index]->do_GetLabel((char*)ptr);
        //h_Strcpy((char*)ptr,"label");
        break;

      //----------

      // stuff text with a string representation of the value of parameter index.
      // ("0.5", "-3", "PLATE", etc...)
      // Limited to kVstMaxParamStrLen.

      case effGetParamDisplay: // 07
        libtrace("vst dispatcher: effGetParamDisplay");
        m_Parameters[index]->do_GetDisplay((char*)ptr);
        //h_Strcpy((char*)ptr,"display");
        break;

      //----------

      // stuff text with the name of parameter index.
      // ("Time", "Gain", "RoomType", etc...)
      // Limited to kVstMaxParamStrLen.

      case effGetParamName: // 08
        libtrace("vst dispatcher: effGetParamName");
        m_Parameters[index]->do_GetName((char*)ptr);
        //h_Strcpy((char*)ptr,"name");
        break;

      //----------

      //case effGetVu:  // 09 // deprecated
      //  libtrace("vst dispatcher: effGetVu");
      //  break;

      //----------

      // called when the sample rate changes (always in a suspend state)

      case effSetSampleRate: // 10
        libtrace("vst dispatcher: effSetSampleRate");
        m_SampleRate = opt;
        do_HandleState(is_Rate);
        break;

      //----------

      // called when the maximun block size changes (always in a suspend state).
      // note that the sampleFrames in process calls could be smaller than
      // this block size, but NOT bigger.

      case effSetBlockSize: // 11
        libtrace("vst dispatcher: effSetBlockSize");
        m_BlockSize = (VstInt32)value;
        do_HandleState(is_Block);
        break;

      //----------

      // suspend: called when plug-in is switched to off
      // resume:  called when plug-in is switched to on

      case effMainsChanged: // 12
        libtrace("vst dispatcher: effMainsChanged");
        if (!value) do_HandleState(is_Suspend);
        else do_HandleState(is_Resume);
        break;

      //----------

      case effEditGetRect: // 13
        libtrace("vst dispatcher: effEditGetRect");
        if (m_Descriptor->m_Flags&df_HasEditor)
        {
          //ERect rect;
          m_ERect.left     = m_EditorRect.x;
          m_ERect.top      = m_EditorRect.y;
          m_ERect.right    = m_EditorRect.x2()+1;
          m_ERect.bottom   = m_EditorRect.y2()+1;
          //trace("effEditGetRect: " << rect.left << "," << rect.top << " : " << rect.right << "," << rect.bottom);
          *(ERect**)ptr = &m_ERect;
          result = 1;
        }
        break;

      //----------

      case effEditOpen: // 14
        libtrace("vst dispatcher: effEditOpen");
        if (m_Descriptor->m_Flags&df_HasEditor)
        {
          if (!m_EditorIsOpen)
          {
            do_OpenEditor(ptr);
            m_EditorIsOpen = true;
            result = 1;
          }
        }
        break;

      //----------

      case effEditClose: // 15
        libtrace("vst dispatcher: effEditClose");
        if (m_Descriptor->m_Flags&df_HasEditor)
        {
          if (m_EditorIsOpen)
          {
            do_CloseEditor();
            m_EditorIsOpen = false;
          }
        }
        break;

      //----------

      //case effEditDraw:  // 16 // deprecated
      //  libtrace("vst dispatcher: effEditDraw");
      //  break;

      //case effEditMouse:  // 17 // deprecated
      //  libtrace("vst dispatcher: effEditMouse");
      //  break;

      //case effEditKey:  // 18 // deprecated
      //  libtrace("vst dispatcher: effEditKey");
      //  break;

      //----------

      case effEditIdle: // 19
        libtrace("vst dispatcher: effEditIdle");
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
      //  libtrace("vst dispatcher: effEditTop");
      //  break;

      //case effEditSleep:  // 21 // deprecated
      //  libtrace("vst dispatcher: effEditSleep");
      //  break;

      //----------

      case DECLARE_VST_DEPRECATED (effIdentify): // 22
        libtrace("vst dispatcher: effIdentify");
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
        libtrace("vst dispatcher: effGetChunk");
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
        libtrace("vst dispatcher: effSetChunk");
        break;

      //---------- aeffectx.h (vst 2)

      case effProcessEvents: // 25
        //libtrace("vst dispatcher: effProcessEvents"); // called before each processblock (too much, like processblock)
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
        libtrace("vst dispatcher: effCanBeAutomated");
        //if ( mParameters[index]->getFlags() & pf_Automate ) v = 1;
        break;

      //----------

      case effString2Parameter: // 27
        libtrace("vst dispatcher: effString2Parameter");
        break;

      //----------

      //case effGetNumProgramCategories: // 28 // deprecated
      //  libtrace("vst dispatcher: eff");
      //  break;

      //----------

      case effGetProgramNameIndexed: // 29
        libtrace("vst dispatcher: effGetProgramNameIndexed");
        //if (index<mPrograms.size())
        //{
        //  strncpy( (char*)ptr, mPrograms[index]->getName().ptr(), kVstMaxProgNameLen );
        //  result = 1;
        //}
        break;

      //----------

      //case effCopyProgram: // 30 // deprecated
      //  libtrace("vst dispatcher: eff");
      //  break;

      //case effConnectInput: // 31 // deprecated
      //  libtrace("vst dispatcher: eff");
      //  break;

      //case effConnectOutput: // 32 // deprecated
      //  libtrace("vst dispatcher: eff");
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
        libtrace("vst dispatcher: effGetInputProperties");
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
        libtrace("vst dispatcher: effGetOutputProperties");
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
        libtrace("vst dispatcher: effGetPlugCategory");
        //if (get_aeFlag(effFlagsIsSynth)) result = kPlugCategSynth;
        //else result = kPlugCategEffect;
        break;

      //----------

      //case effGetCurrentPosition: // 36 // deprecated
      //  libtrace("vst dispatcher: eff");
      //  break;

      //case effGetDestinationBuffer: // 37 // deprecated
      //  libtrace("vst dispatcher: eff");
      //  break;

      //----------

      case effOfflineNotify: // 38
        libtrace("vst dispatcher: effOfflineNotify");
        break;

      case effOfflinePrepare: // 39
        libtrace("vst dispatcher: effOfflinePrepare");
        break;

      case effOfflineRun: // 40
        libtrace("vst dispatcher: effOfflineRun");
        break;

      case effProcessVarIo: // 41
        libtrace("vst dispatcher: effProcessVarIo");
        break;

      case effSetSpeakerArrangement: // 42
        libtrace("vst dispatcher: effSetSpeakerArrangement");
        break;

      //case effSetBlockSizeAndSampleRate: // 43 // deprecated
      //  libtrace("vst dispatcher: eff");
      //  break;

      case effSetBypass: // 44
        libtrace("vst dispatcher: effSetBypass");
        break;

      case effGetEffectName: // 45
        libtrace("vst dispatcher: effGetEffectName");
        h_Strcpy((char*)ptr,m_Descriptor->m_Name);
        result = 1;
        break;

      //case effGetErrorText: // 46 // deprecated
      //  libtrace("vst dispatcher: eff");
      //  break;

      case effGetVendorString: // 47
        libtrace("vst dispatcher: effGetVendorString");
        h_Strcpy((char*)ptr,m_Descriptor->m_Author);
        result = 1;
        break;

      case effGetProductString: // 48
        libtrace("vst dispatcher: effGetProductString");
        h_Strcpy((char*)ptr,m_Descriptor->m_Product);
        //v = 1;
        break;

      case effGetVendorVersion: // 49
        libtrace("vst dispatcher: effGetVendorVersion");
        result = m_Descriptor->m_Version;;
        break;

      //----------

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
        libtrace("vst dispatcher: effVendorSpecific");
        break;

      //----------

      case effCanDo: // 51
        libtrace("vst dispatcher: effCanDo");
        {
          //trace("axFormatVst.dispatcher :: effCanDo");
          //v = canDo ((char*)ptr);
          char* p = (char*)ptr;
          libtrace("effCanDo: '" << p << "'");

          bool _send = (m_Descriptor->m_Flags&df_SendMidi);
          if (_send)
          {
            if (!h_Strcmp(p,"sendVstEvents")) return 1;
            if (!h_Strcmp(p,"sendVstMidiEvents")) return 1;
          }
          bool _receive = (m_Descriptor->m_Flags&df_ReceiveMidi);
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
        libtrace("vst dispatcher: effGetTailSize");
        break;

      //case effIdle: // 53 // deprecated [energyXt2 calls this]
      //  libtrace("vst dispatcher: eff");
      //  break;

      //case effGetIcon: // 54 // deprecated
      //  libtrace("vst dispatcher: eff");
      //  break;

      //case effSetViewPosition: // 55 // deprecated
      //  libtrace("vst dispatcher: eff");
      //  break;

      case effGetParameterProperties: // 56
        libtrace("vst dispatcher: effGetParameterProperties");
        break;

      //case effKeysRequired: // 57 // deprecated
      //  libtrace("vst dispatcher: eff");
      //  break;

      case effGetVstVersion: // 58
        libtrace("vst dispatcher: effGetVstVersion");
        break;

      //---------- VST_2_1_EXTENSIONS

      case effEditKeyDown: // 59
        libtrace("vst dispatcher: effEditKeyDown");
        break;

      case effEditKeyUp: // 60
        libtrace("vst dispatcher: effEditKeyUp");
        break;

      case effSetEditKnobMode: // 61
        libtrace("vst dispatcher: effSetEditKnobMode");
        break;

      case effGetMidiProgramName: // 62
        libtrace("vst dispatcher: effGetMidiProgramName");
        break;

      case effGetCurrentMidiProgram: // 63
        libtrace("vst dispatcher: effGetCurrentMidiProgram");
        break;

      case effGetMidiProgramCategory: // 64
        libtrace("vst dispatcher: effGetMidiprogramCategory");
        break;

      case effHasMidiProgramsChanged: // 65
        libtrace("vst dispatcher: effHasMidiProgramsChanged");
        break;

      case effGetMidiKeyName: // 66
        libtrace("vst dispatcher: effGetMidiKeyName");
        break;

      case effBeginSetProgram: // 67
        libtrace("vst dispatcher: effBeginSetProgram");
        break;

      case effEndSetProgram: // 68
        libtrace("vst dispatcher: effEndSetProgram");
        break;

      //---------- VST_2_3_EXTENSIONS

      case effGetSpeakerArrangement: // 69
        libtrace("vst dispatcher: effGetSpeakerArrangement");
        break;

      case effShellGetNextPlugin: // 70
        libtrace("vst dispatcher: effShellGetNextPlugin");
        break;

      case effStartProcess: // 71
        libtrace("vst dispatcher: effStartProcess");
        break;

      case effStopProcess: // 72
        libtrace("vst dispatcher: effStopProcess");
        break;

      case effSetTotalSampleToProcess: // 73
        libtrace("vst dispatcher: effSetTotalSampleToProcess");
        break;

      case effSetPanLaw: // 74
        libtrace("vst dispatcher: effSetPanLaw");
        break;

      case effBeginLoadBank: // 75
        libtrace("vst dispatcher: effBeginLoadBank");
        break;

      case effBeginLoadProgram: // 76
        libtrace("vst dispatcher: effBeginLoadProgram");
        break;

      //---------- VST_2_4_EXTENSIONS

      case effSetProcessPrecision: // 77
        libtrace("vst dispatcher: effSetProcessPrecision");
        break;

      case effGetNumMidiInputChannels: // 78
        libtrace("vst dispatcher: effGetNumMidiInputChannels");
        break;

      case effGetNumMidiOutputChannels: // 79
        libtrace("vst dispatcher: effGetNumMidiOutputChannels");
        break;

      //default:
      //  libtrace("unhandled vst dispatcher: " << opcode << " : " << vst_opcodes[opcode]);
      //  break;

    } // switch

    return result;
    //0;//inst->vst_dispatcher(opCode,index,value,ptr,opt);
  }

//----------

// return the value of the parameter with index

float h_Instance::vst_getParameter(VstInt32 index)
  {
    return m_Parameters[index]->do_GetValue();
  }

//----------

//Parameters are the individual parameter settings the user can adjust. A VST Host can automate these
//parameters. Set parameter \e index to \e value.
//Index - Index of the parameter to change
//value - A float value between 0.0 and 1.0 inclusive
//note: Parameter values, like all VST parameters, are declared as floats with an inclusive range of
//0.0 to 1.0. How data is presented to the user is merely in the user-interface handling. This is a
//convention, but still worth regarding. Maybe the VST-Host's automation system depends on this range.

// called (from host) when a parameter changed

void h_Instance::vst_setParameter(VstInt32 index, float value)
  {
    h_Parameter* par = m_Parameters[index];
    par->do_SetValue(value);
    do_HandleParameter(par);
    // notify editor
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
    #ifdef H_AUTOSYNC
      updateTime();
      if (m_PlayState&1) do_HandleTransport(m_PlayState);
    #endif
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

//----------------------------------------
// host_*
//----------------------------------------

// returns the host's version (for example 2400 for VST 2.4)
// [return value]: Host VST version (for example 2400 for VST 2.4)
// @see AudioEffect::getMasterVersion

VstInt32 h_Instance::host_Version(void)
  {
    VstInt32 version = 1;
    if (m_AudioMaster) version = (VstInt32)m_AudioMaster(m_AEffect,audioMasterVersion,0,0,0,0);
    if (!version)	version = 1; // old
    return version;
  }

//----------

// returns current unique identifier when loading shell plug-ins
// [return value]: current unique identifier on shell plug-in
// @see AudioEffect::getCurrentUniqueId

VstInt32 h_Instance::host_CurrentId()
  {
    VstInt32 id = 0;
    if (m_AudioMaster) id = (VstInt32)m_AudioMaster(m_AEffect,audioMasterCurrentId,0,0,0,0);
    return id;
  }

//----------

// called after a control has changed in the editor and when the associated parameter should be automated
// [index]: parameter index
// [opt]: parameter value
// @see AudioEffect::setParameterAutomated
//
// An important thing to notice is that if the user changes a parameter in your editor, which is
// out of the Host's control if you are not using the default string based interface, you should
// call setParameterAutomated (). This ensures that the Host is notified of the parameter change, which
// allows it to record these changes for automation.

void h_Instance::host_Automate(VstInt32 index, float value)
  {
    //wtrace("  axFormatVst.setParameterAutomated  index: " << index << " value: " << value);
    if (m_AudioMaster) m_AudioMaster(m_AEffect,audioMasterAutomate,index,0,0,value);
    //setParameter(index,value);
  }

//----------

// Give idle time to Host application,
// e.g. if plug-in editor is doing mouse tracking in a modal loop.
// no arguments
// @see AudioEffect::masterIdle
// masterIdle

void h_Instance::host_Idle(void)
  {
    if (m_AudioMaster) m_AudioMaster(m_AEffect,audioMasterIdle,0,0,0,0);
  }

//----------

// audioMasterGetTime,
// [value]: request mask
// [return value]: #VstTimeInfo* or null if not supported
// @see VstTimeInfoFlags @see AudioEffectX::getTimeInfo

VstTimeInfo* h_Instance::host_GetTime(VstInt32 filter)
  {
    if (m_AudioMaster)
    {
      VstIntPtr ret = m_AudioMaster(m_AEffect,audioMasterGetTime,0,filter,0,0);
      return FromVstPtr<VstTimeInfo> (ret);
    }
    return 0;
  }

//----------

//audioMasterProcessEvents,
// [ptr]: pointer to #VstEvents  @see VstEvents @see AudioEffectX::sendVstEventsToHost
// Can be called inside processReplacing.
// param events Fill with VST events
// return Returns \e true on success

bool h_Instance::host_ProcessEvents(VstEvents* events)
  {
    if (m_AudioMaster) return m_AudioMaster(m_AEffect,audioMasterProcessEvents,0,0,events,0)==1;
    return false;
  }

//----------

//audioMasterIOChanged,
// [return value]: 1 if supported
// @see AudioEffectX::ioChanged
//
// The Host could call a suspend() (if the plug-in was enabled (in resume() state)) and then ask for
// getSpeakerArrangement() and/or check the \e numInputs and \e numOutputs and \e initialDelay and then call a
// resume().
// return true on success
// see also: setSpeakerArrangement(), getSpeakerArrangement()

bool h_Instance::host_IOChanged(void)
  {
    if (m_AudioMaster) return (m_AudioMaster(m_AEffect,audioMasterIOChanged,0,0,0,0) != 0);
    return false;
  }

//----------

//audioMasterSizeWindow,
// [index]: new width
// [value]: new height
// [return value]: 1 if supported  @see AudioEffectX::sizeWindow

bool h_Instance::host_SizeWindow(int aWidth, int aHeight)
  {
    if (m_AudioMaster) return m_AudioMaster(m_AEffect,audioMasterSizeWindow,aWidth,aHeight,0,0)==1;
    return false;
  }

//----------

//audioMasterGetSampleRate,
// [return value]: current sample rate
// @see AudioEffectX::updateSampleRate

float h_Instance::host_GetSampleRate(void)
//virtual void updateSampleRate(void)
  {
    float rate = 0;
    //mSampleRate = 0;
    if (m_AudioMaster)
    {
      VstIntPtr res = m_AudioMaster(m_AEffect, audioMasterGetSampleRate, 0, 0, 0, 0);
      if (res>0) rate = (float)res;//mSampleRate = (float)res;
      //trace(res);
    }
    //return mSampleRate;
    return rate;
  }

//----------

//audioMasterGetBlockSize,
// [return value]: current block size
// @see AudioEffectX::updateBlockSize

//audioMasterGetInputLatency,
// [return value]: input latency in audio samples
// @see AudioEffectX::getInputLatency

//audioMasterGetOutputLatency,
// [return value]: output latency in audio samples
// @see AudioEffectX::getOutputLatency

//audioMasterGetCurrentProcessLevel,
// [return value]: current process level
// @see VstProcessLevels

//audioMasterGetAutomationState,
// [return value]: current automation state
// @see VstAutomationStates

//audioMasterOfflineStart,
// [index]: numNewAudioFiles [value]: numAudioFiles [ptr]: #VstAudioFile*
// @see AudioEffectX::offlineStart

//audioMasterOfflineRead,
// [index]: bool readSource [value]: #VstOfflineOption*
// @see VstOfflineOption [ptr]: #VstOfflineTask*
// @see VstOfflineTask
// @see AudioEffectX::offlineRead

//audioMasterOfflineWrite,
// @see audioMasterOfflineRead
// @see AudioEffectX::offlineRead

//audioMasterOfflineGetCurrentPass,
// @see AudioEffectX::offlineGetCurrentPass

//audioMasterOfflineGetCurrentMetaPass,
// @see AudioEffectX::offlineGetCurrentMetaPass

//audioMasterGetVendorString,
// [ptr]: char buffer for vendor string, limited to #kVstMaxVendorStrLen
// @see AudioEffectX::getHostVendorString

//audioMasterGetProductString,
// [ptr]: char buffer for vendor string, limited to #kVstMaxProductStrLen
// @see AudioEffectX::getHostProductString

//audioMasterGetVendorVersion,
// [return value]: vendor-specific version
// @see AudioEffectX::getHostVendorVersion

//audioMasterVendorSpecific,
// no definition, vendor specific handling
// @see AudioEffectX::hostVendorSpecific

//

//audioMasterCanDo,
// [ptr]: "can do" string
// [return value]: 1 for supported

//audioMasterGetLanguage,
// [return value]: language code
// @see VstHostLanguage

//audioMasterGetDirectory,
// [return value]: FSSpec on MAC, else char*
// @see AudioEffectX::getDirectory

//audioMasterUpdateDisplay,
// no arguments

//----------

//audioMasterBeginEdit,
// [index]: parameter index
// @see AudioEffectX::beginEdit
//
// tells the Host that if it needs to, it has to record automation data for this control.
// param index Index of the parameter
// Returns true on success

bool h_Instance::host_BeginEdit(VstInt32 index)
  {
    if (m_AudioMaster) return (m_AudioMaster(m_AEffect,audioMasterBeginEdit,index,0,0,0)) ? true : false;
    return 0;
  }

//----------

//audioMasterEndEdit,
// [index]: parameter index
// @see AudioEffectX::endEdit
//
// notifies the Host that this control is no longer moved by the mouse.
// param index Index of the parameter
// Returns \e true on success

bool h_Instance::host_EndEdit(VstInt32 index)
{
  if (m_AudioMaster) return (m_AudioMaster(m_AEffect,audioMasterEndEdit,index,0,0,0)) ? true : false;
  return 0;
}

//----------

//audioMasterOpenFileSelector,
// [ptr]: VstFileSelect*
// [return value]: 1 if supported
// @see AudioEffectX::openFileSelector

//audioMasterCloseFileSelector,
// [ptr]: VstFileSelect*
// @see AudioEffectX::closeFileSelector

//
//----------------------------------------------------------------------
//
// format
//
//----------------------------------------------------------------------
//

h_Format::h_Format()
  {
    m_Initialized = false;
  }

//----------

h_Format::~h_Format()
  {
  }

//----------------------------------------

h_Descriptor* h_Format::getDescriptor(void)
  {
    return &H_DESCRIPTOR;
  }

//----------

h_Instance* h_Format::createInstance(h_Host* a_Host, h_Descriptor* a_Descriptor)
  {
    return new H_INSTANCE(a_Host, a_Descriptor);
  }

//----------------------------------------

// this is called for each instance of the vst plugins

AEffect* h_Format::entrypoint(audioMasterCallback audioMaster)
  {
    AEffect*      effect  = (AEffect*)h_Malloc( sizeof(AEffect) );
    h_Host*       host    = new h_Host(audioMaster,effect);
    h_Descriptor* desc    = getDescriptor();
    h_Instance*   inst    = createInstance(host,desc);

    // instance must remember the above effect and host objects,
    // (read them in its constructor)
    // and delete them in its destructor..

    //h_Strcpy(m_RealName,desc->m_Name);
    //#ifdef H_DEBUG
    //  h_Strcat(m_RealName,(char*)"_debug");
    //#endif

    h_Memset(effect,0,sizeof(effect));
    effect->magic                   = kEffectMagic;
    effect->object                  = inst;                   //
    effect->user                    = this;                   // ???
    effect->dispatcher              = vst_dispatcher_callback;
    effect->setParameter            = vst_setParameter_callback;
    effect->getParameter            = vst_getParameter_callback;
    effect->processReplacing        = vst_processReplacing_callback;
    effect->processDoubleReplacing  = vst_processDoubleReplacing_callback;
    effect->flags                   = effFlagsCanReplacing;
    effect->version                 = 0;
    effect->uniqueID                = desc->m_UniqueId;//H_MAGIC + 0x0000;
    effect->numPrograms             = desc->m_NumProgs;
    effect->numParams               = desc->m_NumParams;
    effect->numInputs               = desc->m_NumInputs;
    effect->numOutputs              = desc->m_NumOutputs;
    effect->initialDelay            = 0;
    if (desc->m_Flags & df_HasEditor) effect->flags |= effFlagsHasEditor;
    if (desc->m_Flags & df_IsSynth)   effect->flags |= effFlagsIsSynth;
    return effect;
  }

//----------------------------------------------------------------------

// static callbacks (called by vst-host)
// pass the call over to the proper instance (AEffect.object)

//static
VstIntPtr h_Format::vst_dispatcher_callback(AEffect* ae, VstInt32 opCode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
  {
    h_Instance* inst = (h_Instance*)ae->object;
    if (opCode==effClose)
    {
      inst->vst_dispatcher(opCode,index,value,ptr,opt);
      //delete inst->getAEffect();
      delete inst;
      return 1;
    }
    return inst->vst_dispatcher(opCode,index,value,ptr,opt);
  }

//----------

//static
float h_Format::vst_getParameter_callback(AEffect* ae, VstInt32 index)
  {
    h_Instance* inst = (h_Instance*)ae->object;
    return inst->vst_getParameter(index);
  }

//----------

//static
void h_Format::vst_setParameter_callback(AEffect* ae, VstInt32 index, float value)
  {
    h_Instance* inst = (h_Instance*)ae->object;
    inst->vst_setParameter(index,value);
  }

//----------

//static
void h_Format::vst_processReplacing_callback(AEffect* ae, float** inputs, float** outputs, VstInt32 sampleFrames)
  {
    h_Instance* inst = (h_Instance*)ae->object;
    inst->vst_processReplacing(inputs,outputs,sampleFrames);
  }

//----------

//static
void h_Format::vst_processDoubleReplacing_callback(AEffect* ae, double** inputs, double** outputs, VstInt32 sampleFrames)
  {
    h_Instance* inst = (h_Instance*)ae->object;
    inst->vst_processDoubleReplacing(inputs,outputs,sampleFrames);
  }

//
//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------
//

#ifdef H_LINUX
  #define _H_ASM_MAIN_SYMBOL asm ("main");
#endif
#ifdef H_WIN32
  #define _H_ASM_MAIN_SYMBOL asm ("_main");
#endif

//----------

// a.k.a. createEffectInstance
AEffect*  main_plugin(audioMasterCallback audioMaster) _H_ASM_MAIN_SYMBOL
#define   main main_plugin

//----------

AEffect* main(audioMasterCallback audioMaster)
{
  static_Debug.initialize();
  static_Core.initialize();
  if (!audioMaster(0,audioMasterVersion,0,0,0,0)) return 0;
  return static_Core.m_Format->entrypoint(audioMaster);
}

//TODO:
// be sure we handle multiple instances properly..
// main() is called for each instance
// (and for the initial enumeration)

//----------------------------------------------------------------------
#endif
#endif

