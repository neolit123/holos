#ifndef h_Vst_impl_included
#define h_Vst_impl_included
#ifdef h_Vst_included
//----------------------------------------------------------------------

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

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

h_Instance::h_Instance(h_Host* a_Host, h_Descriptor* a_Descriptor)
//: h_Instance_Base(a_Descriptor)
  {
    m_Host = a_Host;
    m_Descriptor = a_Descriptor;
    m_AudioMaster = a_Host->getAudioMaster();
    //m_AEffect = a_Host->getAEffect();
  }

//----------

h_Instance::~h_Instance()
  {
    // host is created in main()
    if (m_Host) delete m_Host;          // !!!!!
  }

//----------------------------------------
//
//----------------------------------------

//// prepareParameters + transferParameters
//
//virtual void setupParameters(void)
//  {
//    prepareParameters();
//    transferParameters();
//  }
//
////----------
//
//// tells the vst host how many parameters we have
//// needs to be done in the constructor!
//// and initializes parameter-index
//// needed for parameter -> widget mapping if we have an editor
//
//virtual void prepareParameters(void)
//  {
//    int num = mParameters.size();
//    setNumParams(num); // vst
//    for (int i=0; i<num; i++)
//    {
//      axParameter* par = mParameters[i];
//      par->setIndex(i);
//      //doSetParameter(par);
//    }
//  }
//
////----------
//
//// calls doSetParameter for all parameters
//// so that you can fetch them, and setup initial values
//// for your plugin
//
//virtual void transferParameters(void)
//  {
//    int num = mParameters.size();
//    //setNumParams(num); // vst
//    for (int i=0; i<num; i++)
//    {
//      axParameter* par = mParameters[i];
//      //par->setIndex(i);
//      doSetParameter(par);
//    }
//  }
//
////----------
//
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

// called when parameter has changed
// (we must notify the host)

void h_Instance::notifyParameter(h_Parameter* aParameter)
  {
    //int index = aParameter->getIndex();
    //float value = aParameter->doGetValue();//getValue();
    //host_Automate(index,value); // setParameterAutomated();
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
    //trace("updateTimeInfo");
    m_TimeInfo   = host_GetTime( kVstPpqPosValid + kVstTempoValid );
    //trace("..ok. mTimeInfo =  " << hex << mTimeInfo );
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
// midi
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
      _sendMidiClear();
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
        dtrace("vst dispatcher: effOpen");
        do_HandleState(is_Open);
        break;

      //----------

      // called when plug-in will be released

      case effClose: // 01
        dtrace("vst dispatcher: effClose");
        do_HandleState(is_Close);
        //inst->vst_dispatcher(opCode,index,value,ptr,opt);
        //delete inst;
        // desc, host? (in h_Instance_Vst?)
        result = 1;
        break;

      //----------

      // set the current program

      case effSetProgram: // 02
        dtrace("vst dispatcher: effSetProgram");
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
        dtrace("vst dispatcher: effGetProgram");
        result = m_CurrentProgram;
        break;

      //----------

      // stuff the name field of the current program with name.
      // Limited to kVstMaxProgNameLen.

      case effSetProgramName: // 04
        dtrace("vst dispatcher: effSetProgramName");
        //if (m_Programs.size() > 0)
        //{
        //  m_Programs[m_CurrentProgram]->setName( (char*)ptr );
        //} else *(char*)ptr = '\0';
        break;

      //----------

      // stuff name with the name of the current program.
      // Limited to kVstMaxProgNameLen.

      case effGetProgramName: // 05
        dtrace("vst dispatcher: effGetProgramName");
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
        dtrace("vst dispatcher: effGetParamLabel");
        //m_Parameters[index]->doGetLabel((char*)ptr);
        break;

      //----------

      // stuff text with a string representation of the value of parameter index.
      // ("0.5", "-3", "PLATE", etc...)
      // Limited to kVstMaxParamStrLen.

      case effGetParamDisplay: // 07
        dtrace("vst dispatcher: effGetParamDisplay");
        //m_Parameters[index]->doGetDisplay((char*)ptr);
        break;

      //----------

      // stuff text with the name of parameter index.
      // ("Time", "Gain", "RoomType", etc...)
      // Limited to kVstMaxParamStrLen.

      case effGetParamName: // 08
        dtrace("vst dispatcher: effGetParamName");
        //m_Parameters[index]->doGetName((char*)ptr);
        break;

      //----------

      //case effGetVu:  // 09 // deprecated
      //  dtrace("vst dispatcher: effGetVu");
      //  break;

      //----------

      // called when the sample rate changes (always in a suspend state)

      case effSetSampleRate: // 10
        dtrace("vst dispatcher: effSetSampleRate");
        m_SampleRate = opt;
        do_HandleState(is_Rate);
        break;

      //----------

      // called when the maximun block size changes (always in a suspend state).
      // note that the sampleFrames in process calls could be smaller than
      // this block size, but NOT bigger.

      case effSetBlockSize: // 11
        dtrace("vst dispatcher: effSetBlockSize");
        m_BlockSize = (VstInt32)value;
        do_HandleState(is_Block);
        break;

      //----------

      // suspend: called when plug-in is switched to off
      // resume:  called when plug-in is switched to on

      case effMainsChanged: // 12
        dtrace("vst dispatcher: effMainsChanged");
        if (!value) do_HandleState(is_Suspend);
        else do_HandleState(is_Resume);
        break;

      //----------

      case effEditGetRect: // 13
        dtrace("vst dispatcher: effEditGetRect");
        //if (m_FormatFlags&ff_HasEditor)
        //{
        //  rect.left     = m_EditorRect.x;
        //  rect.top      = m_EditorRect.y;
        //  rect.right    = m_EditorRect.x2()+1;
        //  rect.bottom   = m_EditorRect.y2()+1;
        //  //trace("effEditGetRect: " << rect.left << "," << rect.top << " : " << rect.right << "," << rect.bottom);
        //  *(ERect**)ptr = &m_Rect;
        //  result = 1;
        //}
        break;

      //----------

      case effEditOpen: // 14
        dtrace("vst dispatcher: effEditOpen");
        //Window win = (Window)ptr;
        //if ((mFormatFlags&ff_HasEditor) && !mEditorOpen)
        //{
        //  m_EditorWindow = do_OpenEditor();
        //  m_EditorOpen = true;
        //  result = 1;
        //}
        break;

      //----------

      case effEditClose: // 15
        dtrace("vst dispatcher: effEditClose");
        //if ((mFormatFlags&ff_HasEditor) && mEditorOpen)
        //{
        //  mEditorOpen = false;
        //  do_CloseEditor();
        //}
        break;

      //----------

      //case effEditDraw:  // 16 // deprecated
      //  dtrace("vst dispatcher: effEditDraw");
      //  break;

      //case effEditMouse:  // 17 // deprecated
      //  dtrace("vst dispatcher: effEditMouse");
      //  break;

      //case effEditKey:  // 18 // deprecated
      //  dtrace("vst dispatcher: effEditKey");
      //  break;

      //----------

      case effEditIdle: // 19
        dtrace("vst dispatcher: effEditIdle");
        //if ((mFormatFlags&ff_HasEditor) && mEditorOpen)
        //{
        //  doIdleEditor();
        //}
        break;

      //----------

      //case effEditTop:  // 20 // deprecated
      //  dtrace("vst dispatcher: effEditTop");
      //  break;

      //case effEditSleep:  // 21 // deprecated
      //  dtrace("vst dispatcher: effEditSleep");
      //  break;

      //----------

      case DECLARE_VST_DEPRECATED (effIdentify): // 22
        dtrace("vst dispatcher: effIdentify");
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
        dtrace("vst dispatcher: effGetChunk");
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
        dtrace("vst dispatcher: effSetChunk");
        break;

      //---------- aeffectx.h (vst 2)

      case effProcessEvents: // 25
        //dtrace("vst dispatcher: effProcessEvents"); // called before each processblock (too much, like processblock)
        {
          ////v = processEvents ((VstEvents*)ptr);
          ////doProcessEvents();
          ////    //TODO: sort?
          ////    // if so, stuff all events into a buffer
          ////    // a) check last item offset, of later, append after previous
          ////    // if before, search from start, and insert (move later)
          ////    // b) sort
          ////    // c) pre-scan VstEvents array
          //VstEvents* ev = (VstEvents*)ptr;
          ////sendMidiClear();
          //int num = ev->numEvents;
          //for (int i=0; i<num; i++)
          //{
          //  VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
          //  if (event->type==kVstMidiType)
          //  {
          //    doProcessMidi( event->deltaFrames, event->midiData[0], event->midiData[1], event->midiData[2] );
          //  } //=miditype
          //} //numevents
          // sort?
        }
        result = 1;
        break;

      //----------

      case effCanBeAutomated: // 26
        dtrace("vst dispatcher: effCanBeAutomated");
        //if ( mParameters[index]->getFlags() & pf_Automate ) v = 1;
        break;

      //----------

      case effString2Parameter: // 27
        dtrace("vst dispatcher: effString2Parameter");
        break;

      //----------

      //case effGetNumProgramCategories: // 28 // deprecated
      //  dtrace("vst dispatcher: eff");
      //  break;

      //----------

      case effGetProgramNameIndexed: // 29
        dtrace("vst dispatcher: effGetProgramNameIndexed");
        //if (index<mPrograms.size())
        //{
        //  strncpy( (char*)ptr, mPrograms[index]->getName().ptr(), kVstMaxProgNameLen );
        //  result = 1;
        //}
        break;

      //----------

      //case effCopyProgram: // 30 // deprecated
      //  dtrace("vst dispatcher: eff");
      //  break;

      //case effConnectInput: // 31 // deprecated
      //  dtrace("vst dispatcher: eff");
      //  break;

      //case effConnectOutput: // 32 // deprecated
      //  dtrace("vst dispatcher: eff");
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
        dtrace("vst dispatcher: effGetInputProperties");
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
        dtrace("vst dispatcher: effGetOutputProperties");
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
        dtrace("vst dispatcher: effGetPlugCategory");
        //if (get_aeFlag(effFlagsIsSynth)) result = kPlugCategSynth;
        //else result = kPlugCategEffect;
        break;

      //----------

      //case effGetCurrentPosition: // 36 // deprecated
      //  dtrace("vst dispatcher: eff");
      //  break;

      //case effGetDestinationBuffer: // 37 // deprecated
      //  dtrace("vst dispatcher: eff");
      //  break;

      //----------

      case effOfflineNotify: // 38
        dtrace("vst dispatcher: effOfflineNotify");
        break;

      case effOfflinePrepare: // 39
        dtrace("vst dispatcher: effOfflinePrepare");
        break;

      case effOfflineRun: // 40
        dtrace("vst dispatcher: effOfflineRun");
        break;

      case effProcessVarIo: // 41
        dtrace("vst dispatcher: effProcessVarIo");
        break;

      case effSetSpeakerArrangement: // 42
        dtrace("vst dispatcher: effSetSpeakerArrangement");
        break;

      //case effSetBlockSizeAndSampleRate: // 43 // deprecated
      //  dtrace("vst dispatcher: eff");
      //  break;

      case effSetBypass: // 44
        dtrace("vst dispatcher: effSetBypass");
        break;

      case effGetEffectName: // 45
        dtrace("vst dispatcher: effGetEffectName");
        //strcpy((char*)ptr,mEffectName);
        //v = 1;
        break;

      //case effGetErrorText: // 46 // deprecated
      //  dtrace("vst dispatcher: eff");
      //  break;

      case effGetVendorString: // 47
        dtrace("vst dispatcher: effGetVendorString");
        //strcpy((char*)ptr,mVendorString);
        //v = 1;
        break;

      case effGetProductString: // 48
        dtrace("vst dispatcher: effGetProductString");
        //strcpy((char*)ptr,mProductString);
        //v = 1;
        break;

      case effGetVendorVersion: // 49
        dtrace("vst dispatcher: effGetVendorVersion");
        //v = mVendorVersion;
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
        dtrace("vst dispatcher: effVendorSpecific");
        break;

      //----------

      case effCanDo: // 51
        dtrace("vst dispatcher: effCanDo");
        {
          //trace("axFormatVst.dispatcher :: effCanDo");
          //v = canDo ((char*)ptr);
          char* p = (char*)ptr;
          //trace("effCanDo: '" << p << "'");
          if (!h_Strcmp(p,"sendVstEvents"))        result=1; // plug-in will send Vst events to Host
          if (!h_Strcmp(p,"sendVstMidiEvent"))     result=1; // plug-in will send MIDI events to Host
          if (!h_Strcmp(p,"receiveVstEvents"))     result=1; // plug-in can receive MIDI events from Host
          if (!h_Strcmp(p,"receiveVstMidiEvent"))  result=1; // plug-in can receive MIDI events from Host
          if (!h_Strcmp(p,"receiveVstTimeInfo"))   result=1; // plug-in can receive Time info from Host
          //if (h_Strcmp(ptr,"offline"))              return 0; // plug-in supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
          //if (h_Strcmp(ptr,"midiProgramNames"))     return 0; // plug-in supports function #getMidiProgramName ()
          //if (h_Strcmp(ptr,"bypass"))               return 0; // plug-in supports function #setBypass ()
          if (!h_Strcmp(p,"hasCockosExtensions"))  result=0xbeef0000;
          trace("effCanDo: '" << p << "' (return: " << hex << result << dec << ")");
        }
        break;

      //----------

      case effGetTailSize: // 52
        dtrace("vst dispatcher: effGetTailSize");
        break;

      //case effIdle: // 53 // deprecated [energyXt2 calls this]
      //  dtrace("vst dispatcher: eff");
      //  break;

      //case effGetIcon: // 54 // deprecated
      //  dtrace("vst dispatcher: eff");
      //  break;

      //case effSetViewPosition: // 55 // deprecated
      //  dtrace("vst dispatcher: eff");
      //  break;

      case effGetParameterProperties: // 56
        dtrace("vst dispatcher: effGetParameterProperties");
        break;

      //case effKeysRequired: // 57 // deprecated
      //  dtrace("vst dispatcher: eff");
      //  break;

      case effGetVstVersion: // 58
        dtrace("vst dispatcher: effGetVstVersion");
        break;

      //---------- VST_2_1_EXTENSIONS

      case effEditKeyDown: // 59
        dtrace("vst dispatcher: effEditKeyDown");
        break;

      case effEditKeyUp: // 60
        dtrace("vst dispatcher: effEditKeyUp");
        break;

      case effSetEditKnobMode: // 61
        dtrace("vst dispatcher: effSetEditKnobMode");
        break;

      case effGetMidiProgramName: // 62
        dtrace("vst dispatcher: effGetMidiProgramName");
        break;

      case effGetCurrentMidiProgram: // 63
        dtrace("vst dispatcher: effGetCurrentMidiProgram");
        break;

      case effGetMidiProgramCategory: // 64
        dtrace("vst dispatcher: effGetMidiprogramCategory");
        break;

      case effHasMidiProgramsChanged: // 65
        dtrace("vst dispatcher: effHasMidiProgramsChanged");
        break;

      case effGetMidiKeyName: // 66
        dtrace("vst dispatcher: effGetMidiKeyName");
        break;

      case effBeginSetProgram: // 67
        dtrace("vst dispatcher: effBeginSetProgram");
        break;

      case effEndSetProgram: // 68
        dtrace("vst dispatcher: effEndSetProgram");
        break;

      //---------- VST_2_3_EXTENSIONS

      case effGetSpeakerArrangement: // 69
        dtrace("vst dispatcher: effGetSpeakerArrangement");
        break;

      case effShellGetNextPlugin: // 70
        dtrace("vst dispatcher: effShellGetNextPlugin");
        break;

      case effStartProcess: // 71
        dtrace("vst dispatcher: effStartProcess");
        break;

      case effStopProcess: // 72
        dtrace("vst dispatcher: effStopProcess");
        break;

      case effSetTotalSampleToProcess: // 73
        dtrace("vst dispatcher: effSetTotalSampleToProcess");
        break;

      case effSetPanLaw: // 74
        dtrace("vst dispatcher: effSetPanLaw");
        break;

      case effBeginLoadBank: // 75
        dtrace("vst dispatcher: effBeginLoadBank");
        break;

      case effBeginLoadProgram: // 76
        dtrace("vst dispatcher: effBeginLoadProgram");
        break;

      //---------- VST_2_4_EXTENSIONS

      case effSetProcessPrecision: // 77
        dtrace("vst dispatcher: effSetProcessPrecision");
        break;

      case effGetNumMidiInputChannels: // 78
        dtrace("vst dispatcher: effGetNumMidiInputChannels");
        break;

      case effGetNumMidiOutputChannels: // 79
        dtrace("vst dispatcher: effGetNumMidiOutputChannels");
        break;

      //default:
      //  dtrace("unhandled vst dispatcher: " << opcode << " : " << vst_opcodes[opcode]);
      //  break;

    } // switch

    return result;
    //0;//inst->vst_dispatcher(opCode,index,value,ptr,opt);
  }

//----------

// return the value of the parameter with index

float h_Instance::vst_getParameter(VstInt32 index)
  {
    //return m_Parameters[aIndex]->doGetValue();
    return 0;
  }

//----------

//Parameters are the individual parameter settings the user can adjust. A VST Host can automate these
//parameters. Set parameter \e index to \e value.
//Index - Index of the parameter to change
//value - A float value between 0.0 and 1.0 inclusive
//note: Parameter values, like all VST parameters, are declared as floats with an inclusive range of
//0.0 to 1.0. How data is presented to the user is merely in the user-interface handling. This is a
//convention, but still worth regarding. Maybe the VST-Host's automation system depends on this range.

// called when a parameter changed

void h_Instance::vst_setParameter(VstInt32 index, float value)
  {
    //axParameter* par = mParameters[aIndex];
    //par->doSetValue(aValue,true);
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
    //sendMidiClear();
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
    _sendMidiAll();

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

//AEffect* h_Format::entrypoint(void* a_Ptr)
AEffect* h_Format::entrypoint(audioMasterCallback audioMaster)
  {
    // note:
    // the instance (or something) has to remember the host ptr,
    // and delete it when we're finished with it..

    h_Host*       host = new h_Host(audioMaster,&m_AEffect);
    h_Descriptor* desc = getDescriptor();
    h_Instance*   inst = createInstance(host,desc);

    h_Strcpy(m_RealName,desc->m_Name);
    #ifdef H_DEBUG
      h_Strcat(m_RealName,(char*)"_debug");
    #endif

    h_Memset(&m_AEffect,0,sizeof(AEffect));
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
  //trace("static_Debug initialized");
  static_Core.initialize();
  //trace("static_Core initialized");
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

